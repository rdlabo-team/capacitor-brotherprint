import { PluginListenerHandle } from '@capacitor/core';
import { BrotherPrintEventsEnum } from './events.enum';

type BRLMChannelResult = {
  modelName: string;
  serialNumber: string;
  macAddress: string;
  nodeName: string;
  location: string;
  ipAddress: string;
  localName: string;
};

export type IpAddress = Pick<BRLMChannelResult, 'modelName' | 'ipAddress'>;
export type BluetoothLowEnergy = Pick<
  BRLMChannelResult,
  'modelName' | 'localName'
>;

// TODO: Confirm if this is the correct type
export type Bluetooth = Pick<BRLMChannelResult, 'serialNumber' | 'ipAddress'>;

export type printOptions = {
  encodedImage: string;
  printerType: 'QL-820NWB' | 'QL-800';
  numberOfCopies: number;
  labelNameIndex: 16 | 38;
  ipAddress?: string;
  localName?: string;
};

export type searchOption = {
  port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy';
  /**
   * searchDuration is the time to end search for devices.
   * default is 15 seconds.
   * use only port is 'wifi' or 'bluetoothLowEnergy'.
   */
  searchDuration: number;
};

export interface BrotherPrintPlugin {
  printImage(options: printOptions): Promise<void>;
  search(option: searchOption): Promise<{
    printers: (IpAddress | Bluetooth | BluetoothLowEnergy)[];
  }>;

  /**
   * Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.
   */
  cancelSearchWiFiPrinter(): Promise<void>;

  /**
   * Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.
   */
  cancelSearchBluetoothPrinter(): Promise<void>;

  addListener(
    eventName: BrotherPrintEventsEnum.onPrint,
    listenerFunc: () => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: BrotherPrintEventsEnum.onBLEAvailable,
    listenerFunc: () => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: BrotherPrintEventsEnum.onIpAddressAvailable,
    listenerFunc: (info: { ipAddressList: string[] }) => void,
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
