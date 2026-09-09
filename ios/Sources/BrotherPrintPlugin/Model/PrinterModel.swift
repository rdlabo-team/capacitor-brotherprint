import Foundation
import BRLMPrinterKit

class BrotherModel {
    static func getCustomPaper(type: String, width: Float, length: Float, margins: BRLMCustomPaperSizeMargins, markPosition: Float, markLength: Float, gapLength: Float, unit: BRLMCustomPaperSizeLengthUnit) -> BRLMCustomPaperSize {
        switch type {
        case "rollPaper":
            return BRLMCustomPaperSize(rollWithTapeWidth: CGFloat(width), margins: margins, unitOfLength: unit)
        case "dieCutPaper":
            return BRLMCustomPaperSize(dieCutWithTapeWidth: CGFloat(width), tapeLength: CGFloat(length), margins: margins, gapLength: CGFloat(gapLength), unitOfLength: unit)
        case "markRollPaper":
            return BRLMCustomPaperSize(markRollWithTapeWidth: CGFloat(width), tapeLength: CGFloat(length), margins: margins, markPosition: CGFloat(markPosition), markHeight: CGFloat(markLength), unitOfLength: unit)
        default:
            // File doesn't support
            fatalError(type + "is not supported.")
        }
    }

    static func getCustomPaperSizeLengthUnit(unit: String) -> BRLMCustomPaperSizeLengthUnit {
        switch unit {
        case "inch":
            return BRLMCustomPaperSizeLengthUnit.inch
        case "mm":
            return BRLMCustomPaperSizeLengthUnit.mm
        default:
            // other unit is not support
            fatalError(unit + "is not supported.")
        }

    }

    static func getMargin(_ marginTop: Double, _ marginRight: Double, _ marginBottom: Double, _ marginLeft: Double) -> BRLMCustomPaperSizeMargins {
        return BRLMCustomPaperSizeMargins(
            top: CGFloat(marginTop),
            left: CGFloat(marginLeft),
            bottom: CGFloat(marginBottom),
            right: CGFloat(marginRight)
        )
    }

    static func getModelName(from: String) -> BRLMPrinterModel {
        nativePrinterModel(from)
    }

    static func getLabelSize(from: String) -> BRLMQLPrintSettingsLabelSize {
        switch from {
        case "DieCutW17H54":
            return BRLMQLPrintSettingsLabelSize.dieCutW17H54
        case "DieCutW17H87":
            return BRLMQLPrintSettingsLabelSize.dieCutW17H87
        case "DieCutW23H23":
            return BRLMQLPrintSettingsLabelSize.dieCutW23H23
        case "DieCutW29H42":
            return BRLMQLPrintSettingsLabelSize.dieCutW29H42
        case "DieCutW29H90":
            return BRLMQLPrintSettingsLabelSize.dieCutW29H90
        case "DieCutW38H90":
            return BRLMQLPrintSettingsLabelSize.dieCutW38H90
        case "DieCutW39H48":
            return BRLMQLPrintSettingsLabelSize.dieCutW39H48
        case "DieCutW52H29":
            return BRLMQLPrintSettingsLabelSize.dieCutW52H29
        case "DieCutW62H29":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H29
        case "DieCutW62H60":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H60
        case "DieCutW62H75":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H75
        case "DieCutW62H100":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H100
        case "DieCutW60H86":
            return BRLMQLPrintSettingsLabelSize.dieCutW60H86
        case "DieCutW54H29":
            return BRLMQLPrintSettingsLabelSize.dieCutW54H29
        case "DieCutW102H51":
            return BRLMQLPrintSettingsLabelSize.dieCutW102H51
        case "DieCutW102H152":
            return BRLMQLPrintSettingsLabelSize.dieCutW102H152
        case "DieCutW103H164":
            return BRLMQLPrintSettingsLabelSize.dieCutW103H164
        case "RollW12":
            return BRLMQLPrintSettingsLabelSize.rollW12
        case "RollW29":
            return BRLMQLPrintSettingsLabelSize.rollW29
        case "RollW38":
            return BRLMQLPrintSettingsLabelSize.rollW38
        case "RollW50":
            return BRLMQLPrintSettingsLabelSize.rollW50
        case "RollW54":
            return BRLMQLPrintSettingsLabelSize.rollW54
        case "RollW62":
            return BRLMQLPrintSettingsLabelSize.rollW62
        case "RollW62RB":
            return BRLMQLPrintSettingsLabelSize.rollW62RB
        case "RollW102":
            return BRLMQLPrintSettingsLabelSize.rollW102
        case "RollW103":
            return BRLMQLPrintSettingsLabelSize.rollW103
        case "DTRollW90":
            return BRLMQLPrintSettingsLabelSize.dtRollW90
        case "DTRollW102":
            return BRLMQLPrintSettingsLabelSize.dtRollW102
        case "DTRollW102H51":
            return BRLMQLPrintSettingsLabelSize.dtRollW102H51
        case "DTRollW102H152":
            return BRLMQLPrintSettingsLabelSize.dtRollW102H152
        case "RoundW12DIA":
            return BRLMQLPrintSettingsLabelSize.roundW12DIA
        case "RoundW24DIA":
            return BRLMQLPrintSettingsLabelSize.roundW24DIA
        case "RoundW58DIA":
            return BRLMQLPrintSettingsLabelSize.roundW58DIA
        default:
            fatalError("Unsupported label size: \(from)")
        }
    }
}
