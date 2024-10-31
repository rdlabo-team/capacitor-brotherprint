/**
 * Note: If you update this file, you should update the following files:
 * - ios/Plugin/Model/PrinterModel.swift
 *
 * And name is follow android naming convention.
 */

export enum BRKMPrinterCustomPaperUnit {
  mm = 'mm',
  inch = 'inch',
}

export enum BRKMPrinterCustomPaperType {
  rollPaper = 'rollPaper',
  dieCutLabel = 'dieCutLabel',
  markRollPaper = 'markRollPaper',
}

/**
 * Android naming: https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/android_v4/printermodel.html
 */
export enum BRLMPrinterModelName {
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
  W17H54 = 'W17H54',
  W17H87 = 'W17H87',
  W23H23 = 'W23H23',
  W29H42 = 'W29H42',
  W29H90 = 'W29H90',
  W38H90 = 'W38H90',
  W39H48 = 'W39H48',
  W52H29 = 'W52H29',
  W62H29 = 'W62H29',
  W62H100 = 'W62H100',
  W12 = 'W12',
  W29 = 'W29',
  W38 = 'W38',
  W50 = 'W50',
  W54 = 'W54',
  W62 = 'W62',
  W60H86 = 'W60H86',
  W62RB = 'W62RB',
  W54H29 = 'W54H29',
  W12DIA = 'W12DIA',
  W24DIA = 'W24DIA',
  W58DIA = 'W58DIA',
  W62H60 = 'W62H60',
  W62H75 = 'W62H75',
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
