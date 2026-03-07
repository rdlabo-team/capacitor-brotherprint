import type { PluginListenerHandle } from '@capacitor/core';

import type { BrotherPrintEventsEnum } from './events.enum';
import type {
  BRLMChannelResult,
  BRLMPrintOptions,
  BRLMSearchOption,
  ErrorInfo,
  isPortAvailableResult,
} from './interfaces';

export interface BrotherPrintPlugin {
  printImage(options: BRLMPrintOptions): Promise<void>;

  /**
   * Search for printers. If not found, it will return an empty array.(not error)
   */
  search(option: BRLMSearchOption): Promise<void>;

  /**
   * If you have saved the last connected BRLMChannelResult,
   * you can use it to verify whether it is currently usable.
   */
  isPortAvailable(option: BRLMChannelResult): Promise<isPortAvailableResult>;

  /**
   * Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.
   */
  cancelSearchWiFiPrinter(): Promise<void>;

  /**
   * Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.
   */
  cancelSearchBluetoothPrinter(): Promise<void>;

  /**
   * Find the printer that can connected to the device.
   */
  addListener(
    eventName: BrotherPrintEventsEnum.onPrinterAvailable,
    listenerFunc: (printers: BRLMChannelResult) => void,
  ): Promise<PluginListenerHandle>;

  /**
   * Success Print Event
   */
  addListener(eventName: BrotherPrintEventsEnum.onPrint, listenerFunc: () => void): Promise<PluginListenerHandle>;

  /**
   * Failed to connect to the printer.
   * ex: Bluetooth is off, Printer is off, etc.
   */
  addListener(
    eventName: BrotherPrintEventsEnum.onPrintFailedCommunication,
    listenerFunc: (info: ErrorInfo) => void,
  ): Promise<PluginListenerHandle>;

  /**
   * Failed to print.
   */
  addListener(
    eventName: BrotherPrintEventsEnum.onPrintError,
    listenerFunc: (info: ErrorInfo) => void,
  ): Promise<PluginListenerHandle>;
}
