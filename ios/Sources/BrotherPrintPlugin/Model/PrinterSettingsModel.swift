import BRLMPrinterKit
import BRPtouchPrinterKit
import Foundation
import Capacitor

class PrinterSettingsModel {
    static func TDModelSettings(_ call: CAPPluginCall, printSettings: BRLMTDPrintSettings) -> BRLMTDPrintSettings {
        let baseSettings = self.BaseModelSettings(call, printSettings: printSettings)

        let margins = BrotherModel.getMargin(call.getDouble("marginTop", 0), call.getDouble("marginRight", 0), call.getDouble("marginBottom", 0), call.getDouble("marginLeft", 0))

        let unit = BrotherModel.getCustomPaperSizeLengthUnit(unit: call.getString("paperUnit", "mm"))

        baseSettings.customPaperSize = BrotherModel.getCustomPaper(
            type: call.getString("paperType")!,
            width: call.getFloat("tapeWidth", 0),
            length: call.getFloat("tapeLength", 0),
            margins: margins,
            markPosition: call.getFloat("paperMarkPosition", 0),
            markLength: call.getFloat("paperMarkLength", 0),
            gapLength: call.getFloat("gapLength", 0),
            unit: unit
        )

        if let autoCut = call.getBool("autoCut") ?? nil {
            baseSettings.autoCut = autoCut
            baseSettings.cutAtEnd = true
        }

        let report = BRLMValidatePrintSettings.validate(baseSettings)
        NSLog(report.description())

        return printSettings
    }

    static func QLModelSettings(_ call: CAPPluginCall, printSettings: BRLMQLPrintSettings) -> BRLMQLPrintSettings {
        let baseSettings = self.BaseModelSettings(call, printSettings: printSettings)
        baseSettings.labelSize = BrotherModel.getLabelSize(from: call.getString("labelName", "rollW62"))

        if let autoCut = call.getBool("autoCut") ?? nil {
            baseSettings.autoCut = autoCut
            baseSettings.cutAtEnd = true
        }

        let report = BRLMValidatePrintSettings.validate(baseSettings)
        NSLog(report.description())

        return baseSettings
    }

    static func BaseModelSettings<T: BRLMPrintImageSettings>(_ call: CAPPluginCall, printSettings: T) -> T {
        printSettings.numCopies = UInt(call.getInt("numberOfCopies", 1))

        if let scaleMode = call.getString("scaleMode") ?? nil {
            switch scaleMode {
            case "ActualSize":
                printSettings.scaleMode = BRLMPrintSettingsScaleMode.actualSize
            case "FitPageAspect":
                printSettings.scaleMode = BRLMPrintSettingsScaleMode.fitPageAspect
            case "FitPaperAspect":
                printSettings.scaleMode = BRLMPrintSettingsScaleMode.fitPaperAspect
            case "ScaleValue":
                printSettings.scaleMode = BRLMPrintSettingsScaleMode.scaleValue
                if call.getInt("scaleValue") != nil {
                    printSettings.scaleValue = CGFloat(call.getFloat("scaleValue")!)
                }
            default: break
            }
        }

        if let halftone = call.getString("halftone") ?? nil {
            switch halftone {
            case "Threshold":
                printSettings.halftone = BRLMPrintSettingsHalftone.threshold
                if call.getInt("halftoneThreshold") != nil {
                    printSettings.halftoneThreshold = UInt8(call.getInt("halftoneThreshold")!)
                }
            case "ErrorDiffusion":
                printSettings.halftone = BRLMPrintSettingsHalftone.errorDiffusion
            case "PatternDither":
                printSettings.halftone = BRLMPrintSettingsHalftone.patternDither
            default: break
            }
        }

        if let imageRotation = call.getString("imageRotation") ?? nil {
            switch imageRotation {
            case "Rotate0":
                printSettings.imageRotation = BRLMPrintSettingsRotation.rotate0
            case "Rotate90":
                printSettings.imageRotation = BRLMPrintSettingsRotation.rotate90
            case "Rotate180":
                printSettings.imageRotation = BRLMPrintSettingsRotation.rotate180
            case "Rotate270":
                printSettings.imageRotation = BRLMPrintSettingsRotation.rotate270
            default: break
            }
        }

        if let verticalAlignment = call.getString("verticalAlignment") ?? nil {
            switch verticalAlignment {
            case "Top":
                printSettings.vAlignment = BRLMPrintSettingsVerticalAlignment.top
            case "Center":
                printSettings.vAlignment = BRLMPrintSettingsVerticalAlignment.center
            case "Bottom":
                printSettings.vAlignment = BRLMPrintSettingsVerticalAlignment.bottom
            default: break
            }
        }

        if let horizontalAlignment = call.getString("horizontalAlignment") ?? nil {
            switch horizontalAlignment {
            case "Left":
                printSettings.hAlignment = BRLMPrintSettingsHorizontalAlignment.left
            case "Center":
                printSettings.hAlignment = BRLMPrintSettingsHorizontalAlignment.center
            case "Right":
                printSettings.hAlignment = BRLMPrintSettingsHorizontalAlignment.right
            default: break
            }
        }

        if let compressMode = call.getString("compressMode") ?? nil {
            switch compressMode {
            case "None":
                printSettings.compress = BRLMPrintSettingsCompressMode.none
            case "Tiff":
                printSettings.compress = BRLMPrintSettingsCompressMode.tiff
            case "Mode9":
                printSettings.compress = BRLMPrintSettingsCompressMode.mode9
            default: break
            }
        }

        if let printQuality = call.getString("printQuality") ?? nil {
            switch printQuality {
            case "Best":
                printSettings.printQuality = BRLMPrintSettingsPrintQuality.best
            case "Fast":
                printSettings.printQuality = BRLMPrintSettingsPrintQuality.fast
            default: break
            }
        }

        //        if let resolution = call.getString("resolution") ?? nil {
        //            switch resolution {
        //            case "Low":
        //                printSettings.resolution = BRLMPrintSettingsResolution.low
        //            case "Normal":
        //                printSettings.resolution = BRLMPrintSettingsResolution.normal
        //            case "High":
        //                printSettings.resolution = BRLMPrintSettingsResolution.high
        //            default: break
        //            }
        //        }

        return printSettings
    }
}
