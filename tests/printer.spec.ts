import { describe, it, expect, vi } from 'vitest';

import { BRLMPrinterModelName as Model, BRLMPrinterPort as Port } from '../src/brother-printer.enum';
import { BrotherPrintEventsEnum as Events } from '../src/events.enum';
import type { BRLMChannelResult, BRLMPrintOptions, BRLMSearchOption } from '../src/interfaces';
import { BrotherPrinter, PrinterSearchTimeoutError, brotherPrinterPorts } from '../src/printer';

function deferred<T>() {
  let resolve!: (value: T) => void;
  let reject!: (error: unknown) => void;
  const promise = new Promise<T>((yes, no) => {
    resolve = yes;
    reject = no;
  });
  return { promise, resolve, reject };
}

const channel: BRLMChannelResult = {
  port: Port.wifi,
  modelName: 'QL-820NWB',
  channelInfo: '192.0.2.1',
  serialNumber: '',
  macAddress: '',
  nodeName: '',
  location: '',
};

async function setup() {
  const native = {
    search: vi.fn<(options: BRLMSearchOption) => Promise<void>>(async () => undefined),
    isChannelAvailable: vi.fn<(options: BRLMChannelResult) => Promise<{ result: boolean }>>(async () => ({
      result: true,
    })),
    printImage: vi.fn<(options: BRLMPrintOptions) => Promise<void>>(async () => undefined),
    addListener: vi.fn<(event: unknown, listener: unknown) => Promise<{ remove: () => Promise<void> }>>(async () => ({
      remove: vi.fn(async () => undefined),
    })),
  };
  const cache = { read: vi.fn(), write: vi.fn(async () => undefined) };
  const cleanup = vi.fn(async () => undefined);
  const onSearchError = vi.fn(async () => undefined);
  const printer = new BrotherPrinter({ plugin: native, cache, onSearchStart: async () => cleanup, onSearchError });
  await printer.listen();
  const emit = (channels: BRLMChannelResult[]) => {
    const call = native.addListener.mock.calls.find(([event]) => event === Events.onPrinterAvailable);
    if (!call) {
      throw new Error('onPrinterAvailable listener was not registered');
    }
    const listener = call[1] as (channel: BRLMChannelResult) => void;
    channels.forEach(listener);
  };
  return { native, cache, printer, cleanup, onSearchError, emit };
}

describe('all supported model connections', () => {
  const cases = [
    [Model.QL_800, [], [Port.usb]],
    [Model.QL_810W, [Port.wifi], [Port.wifi, Port.usb]],
    [Model.QL_820NWB, [Port.wifi, Port.bluetooth], [Port.wifi, Port.bluetooth, Port.usb]],
    [Model.TD_2320D_203, [Port.wifi], [Port.wifi, Port.usb]],
    [Model.TD_2030AD, [], [Port.usb]],
    [
      Model.TD_2350D_300,
      [Port.wifi, Port.bluetooth, Port.bluetoothLowEnergy],
      [Port.wifi, Port.bluetooth, Port.usb, Port.bluetoothLowEnergy],
    ],
  ] as const;
  it.each(cases)('%s exposes its iOS and Android transports', (model, ios, android) => {
    expect(brotherPrinterPorts(model, false)).toEqual(ios);
    expect(brotherPrinterPorts(model, true)).toEqual(android);
  });
  it('covers every exported model and rejects unknown names', () => {
    expect(cases.map(([model]) => model)).toEqual(Object.values(Model));
    expect(brotherPrinterPorts('general', true)).toEqual([]);
    expect(brotherPrinterPorts('toString', true)).toEqual([]);
  });
});

describe('BrotherPrinter', () => {
  it('serializes three searches and recovers after an error', async () => {
    const { native, printer } = await setup();
    const firstNative = deferred<void>();
    const secondNative = deferred<void>();
    native.search.mockReturnValueOnce(firstNative.promise).mockReturnValueOnce(secondNative.promise);
    const first = printer.search(Port.wifi, true);
    const second = printer.search(Port.bluetooth, true);
    const third = printer.search(Port.usb, true);
    await vi.waitFor(() => expect(native.search).toHaveBeenCalledTimes(1));
    firstNative.reject(new Error('permission denied'));
    await first;
    await vi.waitFor(() => expect(native.search).toHaveBeenCalledTimes(2));
    secondNative.resolve();
    await Promise.all([second, third]);
    expect(native.search.mock.calls.map(([options]) => options.port)).toEqual([Port.wifi, Port.bluetooth, Port.usb]);
  });

  it('reports a visible empty search once and always closes its loading UI', async () => {
    const { printer, onSearchError, cleanup } = await setup();
    await expect(printer.search(Port.bluetooth)).rejects.toBeInstanceOf(PrinterSearchTimeoutError);
    expect(onSearchError).toHaveBeenCalledOnce();
    expect(cleanup).toHaveBeenCalledOnce();
  });

  it.each(Object.values(Model))('rediscovers USB on every print of %s after reconnecting', async (model) => {
    const { printer, native, emit } = await setup();
    native.search.mockImplementation(async () => {
      emit([{ ...channel, modelName: model, port: Port.usb }]);
    });
    await printer.search(Port.usb, true, model);
    native.search.mockClear();
    await printer.prepare(model, Port.usb);
    await printer.prepare(model, Port.usb);
    expect(native.search).toHaveBeenCalledTimes(2);
    expect(native.isChannelAvailable).not.toHaveBeenCalled();
  });

  it.each([
    [Model.QL_800, Port.wifi],
    [Model.QL_820NWB, Port.bluetooth],
    [undefined, Port.wifi],
  ])('does not reuse a cache from another configuration (%s, %s)', async (configuredModel, port) => {
    const { cache, printer, native, emit } = await setup();
    cache.read.mockResolvedValue({ ...channel, configuredModel, port });
    native.search.mockImplementation(async () => {
      emit([channel]);
    });
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    expect(native.search).toHaveBeenCalledOnce();
    expect(native.isChannelAvailable).not.toHaveBeenCalled();
  });

  it('reuses an available cache and rediscovers an unavailable cache', async () => {
    const { cache, printer, native, emit } = await setup();
    cache.read.mockResolvedValue({ ...channel, configuredModel: Model.QL_820NWB });
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    expect(native.search).not.toHaveBeenCalled();
    await printer.search(Port.bluetooth, true);
    native.search.mockClear();
    native.isChannelAvailable.mockResolvedValue({ result: false });
    native.search.mockImplementation(async () => {
      emit([channel]);
    });
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    expect(native.search).toHaveBeenCalledOnce();
  });

  it('awaits discovery before considering the cached printer', async () => {
    const { printer, native, cache, emit } = await setup();
    const searching = deferred<void>();
    native.search.mockReturnValue(searching.promise);
    const search = printer.search(Port.wifi, true, Model.QL_820NWB);
    const prepare = printer.prepare(Model.QL_820NWB, Port.wifi);
    await vi.waitFor(() => expect(native.search).toHaveBeenCalledOnce());
    expect(cache.read).not.toHaveBeenCalled();
    emit([channel]);
    searching.resolve();
    await Promise.all([search, prepare]);
    expect(native.search).toHaveBeenCalledOnce();
  });

  it('selects a sole printer automatically and accepts cancellation for multiple printers', async () => {
    const { printer, emit } = await setup();
    await printer.search(Port.wifi, true);
    const present = vi.fn(async (): Promise<BRLMChannelResult | undefined> => undefined);
    expect(await printer.selectChannel(present)).toBeUndefined();
    emit([channel]);
    expect(await printer.selectChannel(present)).toEqual(channel);
    expect(present).not.toHaveBeenCalled();
    emit([{ ...channel, channelInfo: '192.0.2.2' }]);
    expect(await printer.selectChannel(present)).toBeUndefined();
    expect(present).toHaveBeenCalledOnce();
    present.mockResolvedValue(channel);
    expect(await printer.selectChannel(present)).toEqual(channel);
    expect(await printer.selectChannel(async () => null)).toBeUndefined();
  });

  it('preserves artwork settings, scopes the cache and waits for native printing', async () => {
    const { printer, native, cache } = await setup();
    const printing = deferred<void>();
    native.printImage.mockReturnValue(printing.promise);
    const settings = { modelName: Model.QL_820NWB, encodedImage: 'image', numberOfCopies: 2 } as BRLMPrintOptions;
    let finished = false;
    const result = printer.print(settings, channel).then(() => {
      finished = true;
    });
    await vi.waitFor(() =>
      expect(native.printImage).toHaveBeenCalledWith({
        ...settings,
        port: channel.port,
        channelInfo: channel.channelInfo,
      }),
    );
    expect(finished).toBe(false);
    expect(cache.write).toHaveBeenCalledWith({ ...channel, configuredModel: Model.QL_820NWB });
    printing.resolve();
    await result;
  });

  it('registers callbacks once and ignores results from a different port', async () => {
    const { printer, native } = await setup();
    await Promise.all([printer.listen(), printer.listen()]);
    expect(native.addListener).toHaveBeenCalledTimes(4);
    const availableCall = native.addListener.mock.calls.find(([event]) => event === Events.onPrinterAvailable);
    if (!availableCall) {
      throw new Error('onPrinterAvailable listener was not registered');
    }
    const available = availableCall[1] as (printer: BRLMChannelResult) => void;
    await printer.search(Port.bluetooth, true);
    available(channel);
    expect(printer.printers).toEqual([]);
    available({ ...channel, port: Port.bluetooth });
    expect(printer.printers).toHaveLength(1);
    await printer.removeListeners();
    for (const result of native.addListener.mock.results) expect((await result.value).remove).toHaveBeenCalledOnce();
    await printer.listen();
    expect(native.addListener).toHaveBeenCalledTimes(8);
  });
});

describe('all model/transport flows', () => {
  const combinations = Object.values(Model).flatMap((model) =>
    brotherPrinterPorts(model, true).map((port) => ({ model, port })),
  );
  it.each(combinations)('$model / $port discovers, selects and prints', async ({ model, port }) => {
    const { printer, native, emit, cache } = await setup();
    const device = { ...channel, port, modelName: model };
    native.search.mockImplementation(async () => {
      emit([device]);
    });
    await printer.prepare(model, port);
    const selected = await printer.selectChannel(async () => undefined);
    expect(selected).toEqual(device);
    if (!selected) {
      throw new Error('expected selected channel');
    }
    const settings = { modelName: model, encodedImage: 'image' } as BRLMPrintOptions;
    await printer.print(settings, selected);
    expect(native.printImage).toHaveBeenCalledWith({ ...settings, port, channelInfo: device.channelInfo });
    expect(cache.write).toHaveBeenCalledWith({ ...device, configuredModel: model });
  });

  it('notifies framework state through plain snapshots', async () => {
    const { native, cache } = await setup();
    native.addListener.mockClear();
    const changed = vi.fn();
    const printer = new BrotherPrinter({ plugin: native, cache, onPrintersChanged: changed });
    await printer.listen();
    await printer.search(Port.wifi, true);
    const availableCall = native.addListener.mock.calls.find(([event]) => event === Events.onPrinterAvailable);
    if (!availableCall) {
      throw new Error('onPrinterAvailable listener was not registered');
    }
    const callback = availableCall[1] as (value: BRLMChannelResult) => void;
    callback(channel);
    expect(changed.mock.calls.map(([channels]) => channels)).toEqual([[], [channel]]);
    expect(printer.printers).toEqual([channel]);
  });
});

describe('consumer integration regressions', () => {
  it('deduplicates discovery by explicit port/address and refreshes metadata in place', async () => {
    const { printer, emit } = await setup();
    await printer.search(Port.wifi, true);
    emit([channel, { ...channel, nodeName: 'updated' }, { ...channel, channelInfo: '' }]);
    expect(printer.printers).toEqual([{ ...channel, nodeName: 'updated' }]);
    expect(await printer.selectChannel(vi.fn())).toEqual({ ...channel, nodeName: 'updated' });
  });

  it.each([
    [Port.wifi, '2001:db8::1'],
    [Port.bluetooth, 'AA:BB:CC:DD:EE:FF'],
    [Port.bluetoothLowEnergy, 'BRW_printer_local_name'],
  ])('verifies an explicit %s channel without inferring the port from its address', async (port, address) => {
    const { printer, native, cache } = await setup();
    const explicit = { ...channel, modelName: Model.TD_2350D_300, port, channelInfo: address };
    expect(await printer.useChannel(Model.TD_2350D_300, explicit)).toBe(true);
    expect(native.isChannelAvailable).toHaveBeenCalledWith(explicit);
    expect(native.search).not.toHaveBeenCalled();
    expect(cache.write).not.toHaveBeenCalled();
    expect(printer.printers).toEqual([explicit]);
    await printer.prepare(Model.TD_2350D_300, port);
    expect(native.search).not.toHaveBeenCalled();
  });

  it('does not switch to another printer when an explicit destination is unavailable', async () => {
    const { printer, native } = await setup();
    expect(await printer.useChannel(Model.QL_820NWB, channel)).toBe(true);
    native.isChannelAvailable.mockResolvedValue({ result: false });
    expect(await printer.useChannel(Model.QL_820NWB, { ...channel, channelInfo: '192.0.2.99' })).toBe(false);
    expect(printer.printers).toEqual([]);
    expect(native.search).not.toHaveBeenCalled();
    native.isChannelAvailable.mockRejectedValue(new Error('permission denied'));
    await expect(printer.useChannel(Model.QL_820NWB, channel)).rejects.toMatchObject({ code: 'COMMUNICATION' });
  });

  it('requires USB discovery and rejects empty manual addresses', async () => {
    const { printer, native } = await setup();
    expect(await printer.useChannel(Model.QL_820NWB, { ...channel, port: Port.usb })).toBe(false);
    expect(await printer.useChannel(Model.QL_820NWB, { ...channel, channelInfo: '  ' })).toBe(false);
    expect(native.isChannelAvailable).not.toHaveBeenCalled();
    expect(native.search).not.toHaveBeenCalled();
  });

  it('serializes explicit channel checks between native searches', async () => {
    const { printer, native } = await setup();
    const discovery = deferred<void>();
    const availability = deferred<{ result: boolean }>();
    native.search.mockReturnValueOnce(discovery.promise);
    native.isChannelAvailable.mockReturnValueOnce(availability.promise);
    const first = printer.search(Port.wifi, true);
    const selection = printer.useChannel(Model.QL_820NWB, channel);
    const next = printer.search(Port.bluetooth, true);
    await vi.waitFor(() => expect(native.search).toHaveBeenCalledOnce());
    expect(native.isChannelAvailable).not.toHaveBeenCalled();
    discovery.resolve();
    await first;
    await vi.waitFor(() => expect(native.isChannelAvailable).toHaveBeenCalledOnce());
    expect(native.search).toHaveBeenCalledOnce();
    availability.resolve({ result: true });
    await Promise.all([selection, next]);
    expect(native.search).toHaveBeenCalledTimes(2);
  });

  it('re-registers after cleanup requested during registration, ignoring retired callbacks', async () => {
    const { printer, native } = await setup();
    await printer.removeListeners();
    native.addListener.mockClear();
    const pending = deferred<{ remove: () => Promise<void> }>();
    const remove = vi.fn(async () => undefined);
    native.addListener.mockReturnValueOnce(pending.promise);
    const first = printer.listen();
    const closing = printer.removeListeners();
    const reopened = printer.listen();
    await vi.waitFor(() => expect(native.addListener).toHaveBeenCalledOnce());
    pending.resolve({ remove });
    await Promise.all([first, closing, reopened]);
    expect(remove).toHaveBeenCalledOnce();
    expect(native.addListener).toHaveBeenCalledTimes(8);
    const callbacks = native.addListener.mock.calls.filter(([event]) => event === Events.onPrinterAvailable);
    const oldCallback = callbacks[0][1] as (value: BRLMChannelResult) => void;
    const newCallback = callbacks[1][1] as (value: BRLMChannelResult) => void;
    await printer.search(Port.wifi, true);
    oldCallback(channel);
    expect(printer.printers).toEqual([]);
    newCallback(channel);
    expect(printer.printers).toEqual([channel]);
  });
});

describe('OSS safety contracts', () => {
  const settings = { modelName: Model.QL_820NWB, encodedImage: 'image' } as BRLMPrintOptions;

  it('excludes mismatched and unknown models, accepts documented product aliases', async () => {
    const { printer, native, emit } = await setup();
    native.search.mockImplementation(async () =>
      emit([
        { ...channel, modelName: 'TD-2350D' },
        { ...channel, modelName: 'Unknown', channelInfo: '192.0.2.2' },
        { ...channel, modelName: 'Brother QL-820NWBc', channelInfo: '192.0.2.3' },
      ]),
    );
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    expect(printer.printers.map((p) => p.channelInfo)).toEqual(['192.0.2.3']);
    await expect(printer.print(settings, { ...channel, modelName: 'TD-2350D' })).rejects.toMatchObject({
      code: 'INVALID_ARGUMENT',
    });
    expect(native.printImage).not.toHaveBeenCalled();
  });

  it('serializes printing across controllers until native completion', async () => {
    const { printer, native } = await setup();
    const other = new BrotherPrinter({ plugin: native });
    const pending = deferred<void>();
    native.printImage.mockReturnValueOnce(pending.promise);
    const first = printer.print(settings, channel);
    const second = other.print(settings, channel);
    await vi.waitFor(() => expect(native.printImage).toHaveBeenCalledOnce());
    pending.resolve();
    await Promise.all([first, second]);
    expect(native.printImage).toHaveBeenCalledTimes(2);
  });

  it('prints despite cache/observer failures and reports adapter errors', async () => {
    const { native } = await setup();
    const error = new Error('storage full');
    const onAdapterError = vi.fn();
    const printer = new BrotherPrinter({
      plugin: native,
      cache: {
        read: async () => null,
        write: async () => {
          throw error;
        },
      },
      onAdapterError,
    });
    await printer.print(settings, channel);
    expect(native.printImage).toHaveBeenCalledOnce();
    expect(onAdapterError).toHaveBeenCalledWith(error);
  });

  it('preserves the original failure when error presentation and cleanup fail', async () => {
    const { native } = await setup();
    const cause = { code: 'PERMISSION_DENIED', message: 'Denied' };
    native.search.mockRejectedValue(cause);
    const printer = new BrotherPrinter({
      plugin: native,
      onSearchStart: async () => async () => {
        throw new Error('cleanup');
      },
      onSearchError: async () => {
        throw new Error('UI');
      },
    });
    await expect(printer.search(Port.wifi)).rejects.toMatchObject({ code: 'PERMISSION_DENIED', cause });
  });

  it('cancels queued work and keeps the active native operation until it settles', async () => {
    const { native } = await setup();
    const cancel = vi.fn(async () => undefined);
    const printer = new BrotherPrinter({ plugin: { ...native, cancelSearchWiFiPrinter: cancel } });
    const pending = deferred<void>();
    native.search.mockReturnValueOnce(pending.promise);
    const active = printer.search(Port.wifi, true);
    const queued = printer.print(settings, channel);
    const completed = Promise.allSettled([active, queued]);
    await vi.waitFor(() => expect(native.search).toHaveBeenCalledOnce());
    await printer.removeListeners();
    expect(cancel).toHaveBeenCalledOnce();
    expect(native.printImage).not.toHaveBeenCalled();
    pending.resolve();
    const results = await completed;
    expect(results.every((result) => result.status === 'rejected' && result.reason.code === 'CANCELLED')).toBe(true);
    expect(native.printImage).not.toHaveBeenCalled();
  });

  it('does not report completed printing as cancelled when the screen closes mid-print', async () => {
    const { printer, native } = await setup();
    const pending = deferred<void>();
    native.printImage.mockReturnValueOnce(pending.promise);
    const printing = printer.print(settings, channel);
    await vi.waitFor(() => expect(native.printImage).toHaveBeenCalledOnce());
    await printer.removeListeners();
    pending.resolve();
    await expect(printing).resolves.toBeUndefined();
  });

  it('makes duration/filter independent of presentation and permanently disposes', async () => {
    const { printer, native } = await setup();
    await printer.search(Port.wifi, {
      silent: true,
      searchDuration: 7,
      bluetoothPrintersOnly: false,
      requireResult: false,
    });
    expect(native.search).toHaveBeenCalledWith({ port: Port.wifi, searchDuration: 7, bluetoothPrintersOnly: false });
    await expect(printer.search(Port.wifi, { searchDuration: 0 })).rejects.toMatchObject({ code: 'INVALID_ARGUMENT' });
    await printer.dispose();
    await expect(printer.listen()).rejects.toMatchObject({ code: 'CANCELLED' });
    await expect(printer.print(settings, channel)).rejects.toMatchObject({ code: 'CANCELLED' });
  });
});

describe('cancellation during adapter work', () => {
  it('never begins native printing after cancellation while saving a preference', async () => {
    const { native } = await setup();
    const pending = deferred<void>();
    const write = vi.fn(() => pending.promise);
    const printer = new BrotherPrinter({ plugin: native, cache: { read: async () => null, write } });
    const printing = printer.print({ modelName: Model.QL_820NWB, encodedImage: 'image' } as BRLMPrintOptions, channel);
    const settled = Promise.allSettled([printing]);
    await vi.waitFor(() => expect(write).toHaveBeenCalledOnce());
    printer.cancelPending();
    pending.resolve();
    expect(await settled).toEqual([
      expect.objectContaining({ status: 'rejected', reason: expect.objectContaining({ code: 'CANCELLED' }) }),
    ]);
    expect(native.printImage).not.toHaveBeenCalled();
  });
  it('ignores a legacy cache without a model and rediscovers safely', async () => {
    const { printer, native, cache, emit } = await setup();
    cache.read.mockResolvedValue({ configuredModel: Model.QL_820NWB, port: Port.wifi });
    native.search.mockImplementation(async () => emit([channel]));
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    expect(native.isChannelAvailable).not.toHaveBeenCalled();
    expect(printer.printers).toEqual([channel]);
  });
});

describe('picker cancellation', () => {
  it.each(['cancelPending', 'removeListeners'] as const)('invalidates a late selection after %s', async (close) => {
    const { printer, native, emit } = await setup();
    await printer.search(Port.wifi, true);
    emit([channel, { ...channel, channelInfo: '192.0.2.2' }]);
    const picker = deferred<BRLMChannelResult>();
    const flow = printer
      .selectChannel(() => picker.promise)
      .then(async (selected) => {
        if (selected)
          await printer.print({ modelName: Model.QL_820NWB, encodedImage: 'image' } as BRLMPrintOptions, selected);
      });
    const settled = Promise.allSettled([flow]);
    await printer[close]();
    picker.resolve(channel);
    expect(await settled).toEqual([{ status: 'fulfilled', value: undefined }]);
    expect(native.printImage).not.toHaveBeenCalled();
  });
});

describe('revalidating in-memory discovery', () => {
  it('checks each prepare and rediscovers a printer disconnected since the last flow', async () => {
    const { printer, native, emit, cache } = await setup();
    native.search.mockImplementation(async () => emit([channel]));
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    expect(native.isChannelAvailable).toHaveBeenCalledWith(channel);
    expect(native.search).toHaveBeenCalledOnce();
    native.isChannelAvailable.mockResolvedValue({ result: false });
    native.search.mockImplementation(async () => undefined);
    await expect(printer.prepare(Model.QL_820NWB, Port.wifi)).rejects.toMatchObject({ code: 'NOT_FOUND' });
    expect(printer.printers).toEqual([]);
    expect(native.search).toHaveBeenCalledTimes(2);
    expect(cache.read).toHaveBeenCalledOnce();
  });

  it('keeps only reachable candidates and checks them sequentially', async () => {
    const { printer, native, emit } = await setup();
    const other = { ...channel, channelInfo: '192.0.2.2' };
    native.search.mockImplementation(async () => emit([channel, other]));
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    const pending = deferred<{ result: boolean }>();
    native.isChannelAvailable.mockReturnValueOnce(pending.promise).mockResolvedValueOnce({ result: true });
    const prepare = printer.prepare(Model.QL_820NWB, Port.wifi);
    await vi.waitFor(() => expect(native.isChannelAvailable).toHaveBeenCalledOnce());
    expect(printer.printers).toEqual([]);
    pending.resolve({ result: false });
    await prepare;
    expect(native.isChannelAvailable.mock.calls.map(([value]) => value)).toEqual([channel, other]);
    expect(printer.printers).toEqual([other]);
  });

  it('preserves permission failures while clearing stale results', async () => {
    const { printer, native, emit } = await setup();
    native.search.mockImplementation(async () => emit([channel]));
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    const cause = { code: 'PERMISSION_DENIED', message: 'Denied' };
    native.isChannelAvailable.mockRejectedValue(cause);
    await expect(printer.prepare(Model.QL_820NWB, Port.wifi)).rejects.toMatchObject({
      code: 'PERMISSION_DENIED',
      cause,
    });
    expect(printer.printers).toEqual([]);
    expect(native.search).toHaveBeenCalledOnce();
  });

  it('does not restore stale results when cancelled during availability checks', async () => {
    const { printer, native, emit } = await setup();
    native.search.mockImplementation(async () => emit([channel]));
    await printer.prepare(Model.QL_820NWB, Port.wifi);
    const pending = deferred<{ result: boolean }>();
    native.isChannelAvailable.mockReturnValueOnce(pending.promise);
    const prepare = printer.prepare(Model.QL_820NWB, Port.wifi);
    const settled = Promise.allSettled([prepare]);
    await vi.waitFor(() => expect(native.isChannelAvailable).toHaveBeenCalledOnce());
    printer.cancelPending();
    pending.resolve({ result: true });
    expect(await settled).toEqual([
      expect.objectContaining({ status: 'rejected', reason: expect.objectContaining({ code: 'CANCELLED' }) }),
    ]);
    expect(printer.printers).toEqual([]);
    expect(native.search).toHaveBeenCalledOnce();
  });
});
