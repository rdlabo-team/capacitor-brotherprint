import type { PluginListenerHandle } from '@capacitor/core';

import type {
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
  BRLMPrinterAutoCutType,
  BRLMPrinterCompressMode,
  BRLMPrinterHalftone,
  BRLMPrinterHalftoneThresholdType,
  BRLMPrinterHorizontalAlignment,
  BRLMPrinterNumberOfCopies,
  BRLMPrinterPrintQuality,
  BRLMPrinterImageRotation,
  BRLMPrinterScaleMode,
  BRLMPrinterScaleValueType,
  BRLMPrinterVerticalAlignment,
} from './brother-printer.enum';
import type { BrotherPrintEventsEnum } from './events.enum';

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
} & Partial<BRLMChannelResult> &
  BRLMPrinterSettings;

/**
 * These are optional. If these are not set, default values are assigned by the printer.
 */
export type BRLMPrinterSettings = {
  /**
   * The number of copies you print.
   */
  numberOfCopies?: BRLMPrinterNumberOfCopies;

  /**
   * Whether the auto-cut is enabled or not. If true, your printer cut the paper each page.
   */
  autoCut?: BRLMPrinterAutoCutType;

  /**
   * A scale mode that specifies how your data is scaled in a print area of your printer.
   */
  scaleMode?: BRLMPrinterScaleMode;

  /**
   * A scale value. This is effective when ScaleMode is ScaleValue.
   */
  scaleValue?: BRLMPrinterScaleValueType;

  /**
   * A way to rasterize your data.
   */
  halftone?: BRLMPrinterHalftone;

  /**
   * A threshold value. This is effective when the Halftone is Threshold.
   */
  halftoneThreshold?: BRLMPrinterHalftoneThresholdType;

  /**
   * An image rotation that specifies the angle in which your data is placed in the print area. Rotation direction is clockwise.
   */
  imageRotation?: BRLMPrinterImageRotation;

  /**
   * A vertical alignment that specifies how your data is placed in the printable area.
   */
  verticalAlignment?: BRLMPrinterVerticalAlignment;

  /**
   * A horizontal alignment that specifies how your data is placed in the printable area.
   */
  horizontalAlignment?: BRLMPrinterHorizontalAlignment;

  /**
   * A compress mode that specifies how to compress your data.
   * note: This is ios only.
   */
  compressMode?: BRLMPrinterCompressMode;

  /**
   * A priority that is print speed or print quality. Whether or not this has an effect is depend on your printer.
   */
  printQuality?: BRLMPrinterPrintQuality;

  /**
   * A priority that is print speed or print quality. Whether or not this has an effect is depend on your printer.
   * note: This is ios only. And for some reason, the aspect ratio is broken, so we do not provide this as an API.
   * resolution?: BRLMPrinterPrintResolution;
   */
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
