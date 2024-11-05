export enum BRKMPrinterPort {
  usb = 'usb',
  wifi = 'wifi',
  bluetooth = 'bluetooth',
  bluetoothLowEnergy = 'bluetoothLowEnergy',
}

export enum BRKMPrinterCustomPaperUnit {
  mm = 'mm',
  inch = 'inch',
}

export enum BRKMPrinterCustomPaperType {
  rollPaper = 'rollPaper',
  dieCutPaper = 'dieCutPaper',
  markRollPaper = 'markRollPaper',
}

/**
 * Note: If you update this file, you should update the following files:
 * - ios/Plugin/Model/PrinterModel.swift
 *
 * And name is follow android naming convention.
 * Android naming: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printermodel.html
 */
export enum BRLMPrinterModelName {
  QL_800 = 'QL_800',
  QL_810W = 'QL_810W',
  QL_820NWB = 'QL_820NWB',
  TD_2320D_203 = 'TD_2320D_203',
  TD_2030AD = 'TD_2030AD',
  TD_2350D_300 = 'TD_2350D_300',
}

/**
 * Android naming: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android/labelinfo.html
 */
export enum BRLMPrinterLabelName {
  DieCutW17H54 = 'DieCutW17H54',
  DieCutW17H87 = 'DieCutW17H87',
  DieCutW23H23 = 'DieCutW23H23',
  DieCutW29H42 = 'DieCutW29H42',
  DieCutW29H90 = 'DieCutW29H90',
  DieCutW38H90 = 'DieCutW38H90',
  DieCutW39H48 = 'DieCutW39H48',
  DieCutW52H29 = 'DieCutW52H29',
  DieCutW62H29 = 'DieCutW62H29',
  DieCutW62H60 = 'DieCutW62H60',
  DieCutW62H75 = 'DieCutW62H75',
  DieCutW62H100 = 'DieCutW62H100',
  DieCutW60H86 = 'DieCutW60H86',
  DieCutW54H29 = 'DieCutW54H29',
  DieCutW102H51 = 'DieCutW102H51',
  DieCutW102H152 = 'DieCutW102H152',
  DieCutW103H164 = 'DieCutW103H164',
  RollW12 = 'RollW12',
  RollW29 = 'RollW29',
  RollW38 = 'RollW38',
  RollW50 = 'RollW50',
  RollW54 = 'RollW54',
  RollW62 = 'RollW62',
  RollW62RB = 'RollW62RB',
  RollW102 = 'RollW102',
  RollW103 = 'RollW103',
  DTRollW90 = 'DTRollW90',
  DTRollW102 = 'DTRollW102',
  DTRollW102H51 = 'DTRollW102H51',
  DTRollW102H152 = 'DTRollW102H152',
  RoundW12DIA = 'RoundW12DIA',
  RoundW24DIA = 'RoundW24DIA',
  RoundW58DIA = 'RoundW58DIA',
}

export type BRLMPrinterAutoCutType = boolean;
export type BRLMPrinterScaleValueType = number;
export type BRLMPrinterHalftoneThresholdType = number;
export type BRLMPrinterNumberOfCopies = number;

/**
 * @url https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#imagerotation
 */
export enum BRLMPrinterImageRotation {
  Rotate0 = 'Rotate0',
  Rotate90 = 'Rotate90',
  Rotate180 = 'Rotate180',
  Rotate270 = 'Rotate270',
}

/**
 * url https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#scalemode
 */
export enum BRLMPrinterScaleMode {
  ActualSize = 'ActualSize',
  FitPageAspect = 'FitPageAspect',
  FitPaperAspect = 'FitPaperAspect',
  ScaleValue = 'ScaleValue',
}

/**
 * url: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#halftone
 */
export enum BRLMPrinterHalftone {
  Threshold = 'Threshold',
  ErrorDiffusion = 'ErrorDiffusion',
  PatternDither = 'PatternDither',
}

/**
 * url: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#verticalalignment
 */
export enum BRLMPrinterVerticalAlignment {
  Top = 'Top',
  Center = 'Center',
  Bottom = 'Bottom',
}

/**
 * url: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#horizontalalignment
 */
export enum BRLMPrinterHorizontalAlignment {
  Left = 'Left',
  Center = 'Center',
  Right = 'Right',
}

/**
 * url: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#compressmode
 */
export enum BRLMPrinterCompressMode {
  None = 'None',
  Tiff = 'Tiff',
  Mode9 = 'Mode9',
}

/**
 * url: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#printquality
 */
export enum BRLMPrinterPrintQuality {
  Best = 'Best',
  Fast = 'Fast',
}

/**
 * url: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printimagesettings.html#resolution
 */
// export enum BRLMPrinterPrintResolution {
//   Low = 'Low',
//   Normal = 'Normal',
//   High = 'High',
// }
