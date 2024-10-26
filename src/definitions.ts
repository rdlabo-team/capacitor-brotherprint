import type { PluginListenerHandle } from '@capacitor/core';

import type { BrotherPrintEventsEnum } from './events.enum';
import {
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
} from './brother-printer.enum';

export type BRLMChannelResult = {
  port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy';
  modelName: string;
  serialNumber: string;
  macAddress: string;
  nodeName: string;
  location: string;
  ipAddress: string;
};

export type BRLMPrintOptions = {
  encodedImage: string;
  numberOfCopies: number;
  autoCut: boolean;

  /**
   * Should use getPlatformName.label function.
   * ex: getPlatformName.label('android', 'BRLMQLPrintSettingsLabelSizeRollW62')
   */
  labelName: BRLMPrinterLabelName;

  /**
   * Should use getPlatformName.model function.
   * ex: getPlatformName.model('android', 'BRLMPrinterModelQL_820NWB')
   */
  modelName: BRLMPrinterModelName;

  /**
   * These are get from search result. mostly, it is like below.
   * BrotherPrint.printImage({
   *       ...defaultPrintSettings,
   *       ...{
   *         port: selectPrinter.port,
   *         ipAddress: selectPrinter.ipAddress,
   *         localName: selectPrinter.nodeName,
   *         macAddress: selectPrinter.macAddress,
   *         serialNumber: selectPrinter.serialNumber,
   *       },
   *  })
   */
  port?: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy' | 'usb';
  ipAddress?: string;
  localName?: string;
  serialNumber?: string;
};

export type BRLMSearchOption = {
  port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy';
  /**
   * searchDuration is the time to end search for devices.
   * default is 15 seconds.
   * use only port is 'wifi' or 'bluetoothLowEnergy'.
   */
  searchDuration: number;
};

export interface BrotherPrintPlugin {
  printImage(options: BRLMPrintOptions): Promise<void>;

  /**
   * Search for printers. If not found, it will return an empty array.(not error)
   */
  search(option: BRLMSearchOption): Promise<void>;

  /**
   * Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.
   */
  cancelSearchWiFiPrinter(): Promise<void>;

  /**
   * Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.
   */
  cancelSearchBluetoothPrinter(): Promise<void>;

  addListener(
    eventName: BrotherPrintEventsEnum.onPrinterAvailable,
    listenerFunc: (printers: BRLMChannelResult) => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: BrotherPrintEventsEnum.onPrint,
    listenerFunc: () => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: BrotherPrintEventsEnum.onPrintFailedCommunication,
    listenerFunc: (info: { value: string }) => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: BrotherPrintEventsEnum.onPrintError,
    listenerFunc: (info: { value: string }) => void,
  ): Promise<PluginListenerHandle>;
}
