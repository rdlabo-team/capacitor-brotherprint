import { PluginListenerHandle } from '@capacitor/core';

export interface BrotherPrintPlugin {
  printImage(options: BrotherPrintOptions): Promise<{ value: boolean }>;
  searchWiFiPrinter(): Promise<void>;
  searchBLEPrinter(): Promise<void>;

  addListener(
    eventName: 'onPrint',
    listenerFunc: () => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: 'onBLEAvailable',
    listenerFunc: () => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: 'onBLEAvailable',
    listenerFunc: () => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: 'onIpAddressAvailable',
    listenerFunc: (info: { ipAddressList: string[] }) => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: 'onPrintFailedCommunication',
    listenerFunc: (info: { value: string }) => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: 'onPrintError',
    listenerFunc: (info: { value: string }) => void,
  ): Promise<PluginListenerHandle>;
}

export interface BrotherPrintOptions {
  encodedImage: string;
  printerType: 'QL-820NWB' | 'QL-800';
  numberOfCopies: number;
  labelNameIndex: 16 | 38;
  ipAddress?: string;
  localName?: string;
}
