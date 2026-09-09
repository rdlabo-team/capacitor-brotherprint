import type { PluginListenerHandle } from '@capacitor/core';

import type { BRLMPrinterModelName } from './brother-printer.enum';
import { BRLMPrinterPort } from './brother-printer.enum';
import type { BrotherPrintPlugin } from './definitions';
import { BrotherPrinterError, printerError } from './errors';
import { BrotherPrintEventsEnum } from './events.enum';
import type { BRLMChannelResult, BRLMPrintOptions, ErrorInfo } from './interfaces';
import { printerAliases, printerPorts } from './models';

/**
 * Connection choices for every exported model. `wifi` is the SDK network transport
 * (also wired Ethernet on TD-2320D). USB is Android-only. Availability still depends
 * on device configuration and permissions; see the README for hardware test status.
 */
export function brotherPrinterPorts(model: string, isAndroid: boolean): BRLMPrinterPort[] {
  if (!Object.prototype.hasOwnProperty.call(printerPorts, model)) return [];
  return printerPorts[model as BRLMPrinterModelName].filter((port) => isAndroid || port !== BRLMPrinterPort.usb);
}

/** A remembered physical printer, scoped to the model chosen by the user. */
export type BrotherPrinterCache = BRLMChannelResult & { configuredModel?: BRLMPrinterModelName };

/** Native capabilities used by the connection controller. The app supplies its installed plugin. */
export type BrotherPrinterTransport = Partial<
  Pick<BrotherPrintPlugin, 'cancelSearchWiFiPrinter' | 'cancelSearchBluetoothPrinter'>
> &
  Pick<BrotherPrintPlugin, 'search' | 'isChannelAvailable' | 'printImage' | 'addListener'>;

/** Context passed to the app's loading and error presentation callbacks. */
export interface BrotherPrinterSearch {
  readonly port: BRLMPrinterPort;
  readonly silent: boolean;
}

/** App adapters for persistence, native transport and localized presentation. No app strings are bundled. */
export interface BrotherPrinterOptions {
  readonly plugin: BrotherPrinterTransport;
  /** Receive discovery/cache snapshots. Adapt this callback to your framework's state. */
  readonly onPrintersChanged?: (printers: readonly BRLMChannelResult[]) => void;
  readonly cache?: {
    read(): Promise<BrotherPrinterCache | null>;
    write(printer: BrotherPrinterCache): Promise<void>;
  };
  /** Observer/cache failures are isolated from native operations and reported here. */
  readonly onAdapterError?: (error: unknown) => void;
  /** Start a loading indicator and return its cleanup, if needed. Called within the search queue. */
  readonly onSearchStart?: (context: BrotherPrinterSearch) => Promise<(() => Promise<unknown>) | undefined>;
  /** Called once for a non-silent search failure, before the error is rethrown. */
  readonly onSearchError?: (error: unknown, context: BrotherPrinterSearch) => Promise<void>;
  readonly onPrint?: () => void | Promise<void>;
  readonly onPrintError?: (error: ErrorInfo) => void | Promise<void>;
  readonly onPrintFailedCommunication?: (error: ErrorInfo) => void | Promise<void>;
}

/** Discovery completed without any available printer. */
export class PrinterNotFoundError extends BrotherPrinterError {
  constructor() {
    super('NOT_FOUND', 'No matching printer found');
    this.name = 'PrinterNotFoundError';
  }
}

/** @deprecated Use PrinterNotFoundError. Discovery completion is not a timeout. */
export { PrinterNotFoundError as PrinterSearchTimeoutError };

/** Explicit discovery behavior, independent of presentation. */
export interface BrotherPrinterSearchOptions {
  silent?: boolean;
  searchDuration?: number;
  bluetoothPrintersOnly?: boolean;
  requireResult?: boolean;
  model?: BRLMPrinterModelName;
}

/** Normalize only known product aliases; never infer a model from an address. */
export function brotherPrinterModel(name: unknown): BRLMPrinterModelName | undefined {
  if (typeof name !== 'string') return undefined;
  const key = name
    .trim()
    .toUpperCase()
    .replace(/^BROTHER\s+/, '')
    .replace(/[-_ ]/g, '');
  return Object.prototype.hasOwnProperty.call(printerAliases, key) ? printerAliases[key] : undefined;
}

/**
 * Framework-independent Brother connection lifecycle.
 * Own one instance per printing service. The app owns navigation, paper/layout settings and UI labels.
 */
export class BrotherPrinter {
  /** Results of the most recent discovery or cache reuse. */
  private results: readonly BRLMChannelResult[] = [];

  /** Current discovery results. Treat channels as read-only. */
  get printers(): readonly BRLMChannelResult[] {
    return this.results;
  }

  private setPrinters(printers: readonly BRLMChannelResult[]): void {
    this.results = printers;
    this.observe(() => this.options.onPrintersChanged?.(printers));
  }
  private static eventOwner: BrotherPrinter | undefined;
  private static connectionQueue: Promise<void> | undefined;
  private operationGeneration = 0;
  private disposed = false;
  private reportAdapterError(error: unknown): void {
    try {
      void Promise.resolve(this.options.onAdapterError?.(error)).catch(() => undefined);
    } catch {
      /* Diagnostic observers cannot alter operations. */
    }
  }
  private observe(callback: () => unknown): void {
    try {
      void Promise.resolve(callback()).catch((error: unknown) => this.reportAdapterError(error));
    } catch (error) {
      this.reportAdapterError(error);
    }
  }
  private checkGeneration(generation: number): void {
    if (generation !== this.operationGeneration || this.disposed)
      throw new BrotherPrinterError('CANCELLED', 'Printer operation cancelled');
  }
  private async adapter<T>(callback: () => Promise<T> | undefined): Promise<T | undefined> {
    try {
      return await callback();
    } catch (error) {
      this.reportAdapterError(error);
      return undefined;
    }
  }
  private searchQueue: Promise<void> | undefined;
  private searchPort: BRLMPrinterPort | undefined;
  private searchModel: BRLMPrinterModelName | undefined;
  private listeners: Promise<PluginListenerHandle[]> | undefined;
  private listenerQueue: Promise<void> | undefined;
  private listenerGeneration = 0;

  constructor(private readonly options: BrotherPrinterOptions) {}

  /** Register native callbacks once; concurrent callers share the same registration. */
  listen(): Promise<void> {
    return this.queueListeners(async () => {
      if (this.disposed) throw new BrotherPrinterError('CANCELLED', 'Printer controller is disposed');
      if (!this.listeners) {
        this.listeners = this.registerListeners().catch((error: unknown) => {
          this.listeners = undefined;
          throw error;
        });
      }
      await this.listeners;
    });
  }

  private queueListeners(operation: () => Promise<void>): Promise<void> {
    const result = this.listenerQueue ? this.listenerQueue.then(operation) : operation();
    this.listenerQueue = result.catch(() => undefined);
    return result;
  }

  private async registerListeners(): Promise<PluginListenerHandle[]> {
    const generation = ++this.listenerGeneration;
    const registered: PluginListenerHandle[] = [];
    const failed = async (error: unknown): Promise<never> => {
      ++this.listenerGeneration;
      await Promise.all(registered.map((listener) => listener.remove()));
      throw error;
    };
    registered.push(
      await this.options.plugin
        .addListener(BrotherPrintEventsEnum.onPrint, () => {
          if (
            generation !== this.listenerGeneration ||
            (BrotherPrinter.eventOwner && BrotherPrinter.eventOwner !== this)
          )
            return;
          this.observe(() => this.options.onPrint?.());
        })
        .catch(failed),
    );
    registered.push(
      await this.options.plugin
        .addListener(BrotherPrintEventsEnum.onPrintError, (error) => {
          if (
            generation !== this.listenerGeneration ||
            (BrotherPrinter.eventOwner && BrotherPrinter.eventOwner !== this)
          )
            return;
          this.observe(() => this.options.onPrintError?.(error));
        })
        .catch(failed),
    );
    registered.push(
      await this.options.plugin
        .addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, (error) => {
          if (
            generation !== this.listenerGeneration ||
            (BrotherPrinter.eventOwner && BrotherPrinter.eventOwner !== this)
          )
            return;
          this.observe(() => this.options.onPrintFailedCommunication?.(error));
        })
        .catch(failed),
    );
    registered.push(
      await this.options.plugin
        .addListener(BrotherPrintEventsEnum.onPrinterAvailable, (printer) => {
          if (
            generation !== this.listenerGeneration ||
            (BrotherPrinter.eventOwner !== undefined && BrotherPrinter.eventOwner !== this) ||
            printer.port !== this.searchPort ||
            !printer.channelInfo?.trim() ||
            (this.searchModel !== undefined && brotherPrinterModel(printer.modelName) !== this.searchModel)
          )
            return;
          const index = this.results.findIndex(
            (current) => current.port === printer.port && current.channelInfo === printer.channelInfo,
          );
          const next = [...this.results];
          if (index < 0) next.push(printer);
          else next[index] = printer;
          this.setPrinters(next);
        })
        .catch(failed),
    );
    return registered;
  }

  /** Remove callbacks, including those whose registration was still pending when the view closed. */
  removeListeners(): Promise<void> {
    this.cancelPending();
    return this.queueListeners(async () => {
      const pending = this.listeners;
      if (!pending) return;
      ++this.listenerGeneration;
      const registered = await pending;
      await Promise.all(registered.map((listener) => listener.remove()));
      if (this.listeners === pending) this.listeners = undefined;
    });
  }

  /** Invalidate queued operations; active native work retains the queue until it settles. */
  cancelPending(): void {
    ++this.operationGeneration;
    this.searchPort = undefined;
    if (this.activeSearch === BRLMPrinterPort.wifi) this.observe(() => this.options.plugin.cancelSearchWiFiPrinter?.());
    if (this.activeSearch === BRLMPrinterPort.bluetoothLowEnergy)
      this.observe(() => this.options.plugin.cancelSearchBluetoothPrinter?.());
    this.setPrinters([]);
  }
  /** Permanently close this controller. Printing already handed to the SDK is not interrupted. */
  async dispose(): Promise<void> {
    this.disposed = true;
    await this.removeListeners();
    await this.searchQueue;
  }
  private activeSearch?: BRLMPrinterPort;

  /** Discover with explicit options. The boolean overload retains legacy silent behavior. */
  search(
    port: BRLMPrinterPort,
    options: boolean | BrotherPrinterSearchOptions = false,
    model?: BRLMPrinterModelName,
  ): Promise<void> {
    const config =
      typeof options === 'boolean'
        ? { silent: options, model, searchDuration: options ? 3 : 10, requireResult: !options }
        : options;
    return this.queueConnection(() =>
      this.discover(
        { port, silent: config.silent ?? false },
        config.model,
        config,
        typeof options === 'boolean' && options,
      ),
    );
  }

  private queueConnection<T>(run: () => Promise<T>, cancelResult = true): Promise<T> {
    const generation = this.operationGeneration;
    const guarded = async (): Promise<T> => {
      if (this.disposed || generation !== this.operationGeneration)
        throw new BrotherPrinterError('CANCELLED', 'Printer operation cancelled');
      BrotherPrinter.eventOwner = this;
      let value: T;
      try {
        value = await run();
      } finally {
        BrotherPrinter.eventOwner = undefined;
      }
      if (cancelResult && generation !== this.operationGeneration)
        throw new BrotherPrinterError('CANCELLED', 'Printer operation cancelled');
      return value;
    };
    const operation = BrotherPrinter.connectionQueue ? BrotherPrinter.connectionQueue.then(guarded) : guarded();
    this.searchQueue = operation.then(
      () => undefined,
      () => undefined,
    );
    BrotherPrinter.connectionQueue = this.searchQueue;
    return operation;
  }

  /**
   * Validate an explicitly selected/manual channel without discovery or automatic fallback.
   * Returns false for an unavailable/empty channel or USB (use prepare for USB permissions).
   * Supply the actual port; addresses are opaque, never classified as IP/MAC by their shape.
   * Persistence still happens in print(), just like a discovered channel.
   */
  useChannel(model: BRLMPrinterModelName, channel: BRLMChannelResult): Promise<boolean> {
    return this.queueConnection(async () => {
      const generation = this.operationGeneration;
      this.searchModel = model;
      this.searchPort = channel.port;
      this.setPrinters([]);
      if (channel.port === BRLMPrinterPort.usb || !channel.channelInfo.trim()) return false;
      if (brotherPrinterModel(channel.modelName) !== model)
        throw new BrotherPrinterError('INVALID_ARGUMENT', 'Channel model does not match the selected model');
      const available = await this.options.plugin.isChannelAvailable(channel).catch((error: unknown) => {
        throw printerError(error, 'COMMUNICATION');
      });
      this.checkGeneration(generation);
      this.setPrinters(available.result ? [channel] : []);
      return available.result;
    });
  }

  private async discover(
    context: BrotherPrinterSearch,
    model?: BRLMPrinterModelName,
    config: BrotherPrinterSearchOptions = {},
    suppressErrors = false,
  ): Promise<void> {
    const duration = config.searchDuration ?? 10;
    if (!Number.isInteger(duration) || duration <= 0)
      throw new BrotherPrinterError('INVALID_ARGUMENT', 'searchDuration must be a positive integer');
    this.setPrinters([]);
    this.searchPort = context.port;
    this.searchModel = model;
    const generation = this.operationGeneration;
    const finish = await this.adapter(() => this.options.onSearchStart?.(context));
    try {
      if (generation !== this.operationGeneration) throw new BrotherPrinterError('CANCELLED', 'Search cancelled');
      this.activeSearch = context.port;
      await this.options.plugin.search({
        port: context.port,
        searchDuration: duration,
        bluetoothPrintersOnly: config.bluetoothPrintersOnly ?? true,
      });
      if (generation !== this.operationGeneration) throw new BrotherPrinterError('CANCELLED', 'Search cancelled');
      if ((config.requireResult ?? true) && this.printers.length === 0) throw new PrinterNotFoundError();
    } catch (cause) {
      this.setPrinters([]);
      const error = printerError(cause, 'COMMUNICATION');
      if (!context.silent && error.code !== 'CANCELLED')
        await this.adapter(() => this.options.onSearchError?.(error, context));
      if (!suppressErrors || error.code === 'CANCELLED') throw error;
    } finally {
      this.activeSearch = undefined;
      await this.adapter(() => finish?.());
    }
  }

  /** Prepare the chosen connection. USB always rediscovers to reacquire permission after unplugging. */
  prepare(model: BRLMPrinterModelName, port: BRLMPrinterPort): Promise<void> {
    return this.queueConnection(async () => {
      const generation = this.operationGeneration;
      if (port !== BRLMPrinterPort.usb) {
        if (this.searchModel === model && this.searchPort === port && this.printers.length > 0) {
          const remembered = this.printers;
          this.setPrinters([]);
          const available: BRLMChannelResult[] = [];
          for (const channel of remembered) {
            const result = await this.options.plugin.isChannelAvailable(channel).catch((error: unknown) => {
              throw printerError(error, 'COMMUNICATION');
            });
            this.checkGeneration(generation);
            if (result.result) available.push(channel);
          }
          if (available.length > 0) {
            this.setPrinters(available);
            return;
          }
          await this.discover({ port, silent: false }, model);
          return;
        }
        this.setPrinters([]);
        const cached = await this.adapter(() => this.options.cache?.read());
        this.checkGeneration(generation);
        if (
          cached?.configuredModel === model &&
          cached.port === port &&
          typeof cached.channelInfo === 'string' &&
          cached.channelInfo.trim().length > 0 &&
          brotherPrinterModel(cached.modelName) === model
        ) {
          const available = await this.options.plugin.isChannelAvailable(cached).catch((error: unknown) => {
            throw printerError(error, 'COMMUNICATION');
          });
          this.checkGeneration(generation);
          if (available.result) {
            this.searchModel = model;
            this.searchPort = port;
            this.setPrinters([cached]);
            return;
          }
        }
      }
      await this.discover({ port, silent: false }, model);
    });
  }

  /** Select a physical printer, asking the app to choose only when several were found. Return null/undefined to cancel. */
  async selectChannel(
    present: (printers: readonly BRLMChannelResult[]) => Promise<BRLMChannelResult | null | undefined>,
  ): Promise<BRLMChannelResult | undefined> {
    const generation = this.operationGeneration;
    this.checkGeneration(generation);
    const printers = this.printers;
    const selected = printers.length < 2 ? printers[0] : ((await present(printers)) ?? undefined);
    if (this.disposed || generation !== this.operationGeneration) return undefined;
    return selected;
  }

  /** Remember the selected device and await native printing; artwork and print settings are supplied unchanged. */
  print(settings: BRLMPrintOptions, channel: BRLMChannelResult): Promise<void> {
    return this.queueConnection(async () => {
      const generation = this.operationGeneration;
      if (!channel.channelInfo?.trim() || brotherPrinterModel(channel.modelName) !== settings.modelName) {
        throw new BrotherPrinterError('INVALID_ARGUMENT', 'Channel address or model is invalid');
      }
      await this.adapter(() => this.options.cache?.write({ ...channel, configuredModel: settings.modelName }));
      this.checkGeneration(generation);
      await this.options.plugin
        .printImage({ ...settings, port: channel.port, channelInfo: channel.channelInfo })
        .catch((error: unknown) => {
          throw printerError(error, 'PRINT_FAILED');
        });
    }, false);
  }
}
