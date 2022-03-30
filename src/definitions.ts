import { PluginListenerHandle } from '@capacitor/core';

export interface BrotherPrintPlugin {
  printImage(options: BrotherPrintOptions): Promise<{ value: boolean }>;
  searchWiFiPrinter(): Promise<void>;
  searchBLEPrinter(): Promise<void>;

  addListener(
    eventName: 'onPrint',
    listenerFunc: () => void,
  ): PluginListenerHandle;

  addListener(
    eventName: 'onBLEAvailable',
    listenerFunc: () => void,
  ): PluginListenerHandle;

  addListener(
    eventName: 'onBLEAvailable',
    listenerFunc: () => void,
  ): PluginListenerHandle;

  addListener(
    eventName: 'onIpAddressAvailable',
    listenerFunc: () => void,
  ): PluginListenerHandle;

  addListener(
    eventName: 'onPrintFailedCommunication',
    listenerFunc: (info: { value: string }) => void,
  ): PluginListenerHandle;

  addListener(
    eventName: 'onPrintError',
    listenerFunc: (info: { value: string }) => void,
  ): PluginListenerHandle;
}

export interface BrotherPrintOptions {
  encodedImage: string;
  printerType: 'QL-820NWB' | 'QL-800';
  numberOfCopies: number;
  labelNameIndex: 16 | 38;
  ipAddress?: string;
  localName?: string;
}
