import type { PluginListenerHandle } from '@capacitor/core';

import type {
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
} from './brother-printer.enum';
import type { BrotherPrintEventsEnum } from './events.enum';
import {
  BRLMPrinterAutoCutType,
  BRLMPrinterCompressMode,
  BRLMPrinterHalftone,
  BRLMPrinterHalftoneThresholdType,
  BRLMPrinterHorizontalAlignment,
  BRLMPrinterNumberOfCopies,
  BRLMPrinterPrintOrientation,
  BRLMPrinterPrintQuality,
  BRLMPrinterPrintResolution,
  BRLMPrinterRotation,
  BRLMPrinterScaleMode,
  BRLMPrinterScaleValueType,
  BRLMPrinterVerticalAlignment,
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

  /**
   * Should use enum BRLMPrinterLabelName
   */
  labelName: BRLMPrinterLabelName;

  /**
   * Should use enum BRLMPrinterModelName
   */
  modelName: BRLMPrinterModelName;
} & BRLMChannelResult &
  BRLMPrinterSettings;

/**
 * These are optional. If these are not set, default values are assigned by the printer.
 */
export type BRLMPrinterSettings = {
  numberOfCopies?: BRLMPrinterNumberOfCopies;
  autoCut?: BRLMPrinterAutoCutType;
  scaleValue?: BRLMPrinterScaleValueType;
  halftoneThreshold?: BRLMPrinterHalftoneThresholdType;
  rotation?: BRLMPrinterRotation;
  scaleMode?: BRLMPrinterScaleMode;
  halftone?: BRLMPrinterHalftone;
  verticalAlignment?: BRLMPrinterVerticalAlignment;
  horizontalAlignment?: BRLMPrinterHorizontalAlignment;
  compressMode?: BRLMPrinterCompressMode;
  printQuality?: BRLMPrinterPrintQuality;
  orientation?: BRLMPrinterPrintOrientation;
  resolution?: BRLMPrinterPrintResolution;
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

export type ErrorInfo = {
  message: string;
  code: number;
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
    listenerFunc: (info: ErrorInfo) => void,
  ): Promise<PluginListenerHandle>;

  addListener(
    eventName: BrotherPrintEventsEnum.onPrintError,
    listenerFunc: (info: ErrorInfo) => void,
  ): Promise<PluginListenerHandle>;
}
