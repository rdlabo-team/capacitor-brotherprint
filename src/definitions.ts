import { PluginListenerHandle } from '@capacitor/core';
import { BrotherPrintEventsEnum } from './events.enum';

export interface BrotherPrintPlugin {
  printImage(options: BrotherPrintOptions): Promise<void>;
  searchWiFiPrinter(): Promise<void>;
  searchBLEPrinter(): Promise<void>;

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

export interface BrotherPrintOptions {
  encodedImage: string;
  printerType: 'QL-820NWB' | 'QL-800';
  numberOfCopies: number;
  labelNameIndex: 16 | 38;
  ipAddress?: string;
  localName?: string;
}
