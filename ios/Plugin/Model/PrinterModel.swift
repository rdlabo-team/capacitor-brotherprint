import Foundation
import BRLMPrinterKit
import BRPtouchPrinterKit

class BrotherModel {
    static func getModelName(from: String) -> BRLMPrinterModel {
        switch from {
        case "QL_810W":
            return BRLMPrinterModel.QL_810W
        case "QL_820NWB":
            return BRLMPrinterModel.QL_820NWB
        case "TD_2320D_203":
            return BRLMPrinterModel.TD_2320D_203
        case "TD_2030AD":
            return BRLMPrinterModel.TD_2030A
        case "TD_2350D_203":
            return BRLMPrinterModel.TD_2350D_203
        default:
            return BRLMPrinterModel.unknown
        }
    }

    static func getLabelSize(from: String) -> BRLMQLPrintSettingsLabelSize {
        switch from {
        case "W17H54":
            return BRLMQLPrintSettingsLabelSize.dieCutW17H54
        case "W17H87":
            return BRLMQLPrintSettingsLabelSize.dieCutW17H87
        case "W23H23":
            return BRLMQLPrintSettingsLabelSize.dieCutW23H23
        case "W29H42":
            return BRLMQLPrintSettingsLabelSize.dieCutW29H42
        case "W29H90":
            return BRLMQLPrintSettingsLabelSize.dieCutW29H90
        case "W38H90":
            return BRLMQLPrintSettingsLabelSize.dieCutW38H90
        case "W39H48":
            return BRLMQLPrintSettingsLabelSize.dieCutW39H48
        case "W52H29":
            return BRLMQLPrintSettingsLabelSize.dieCutW52H29
        case "W62H29":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H29
        case "W62H100":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H100
        case "W12":
            return BRLMQLPrintSettingsLabelSize.rollW12
        case "W38":
            return BRLMQLPrintSettingsLabelSize.rollW38
        case "W50":
            return BRLMQLPrintSettingsLabelSize.rollW50
        case "W54":
            return BRLMQLPrintSettingsLabelSize.rollW54
        case "W62":
            return BRLMQLPrintSettingsLabelSize.rollW62
        case "W60H86":
            return BRLMQLPrintSettingsLabelSize.dieCutW60H86
        case "W62RB":
            return BRLMQLPrintSettingsLabelSize.rollW62RB
        case "W54H29":
            return BRLMQLPrintSettingsLabelSize.dieCutW54H29
        case "W12DIA":
            return BRLMQLPrintSettingsLabelSize.roundW12DIA
        case "W24DIA":
            return BRLMQLPrintSettingsLabelSize.roundW24DIA
        case "W58DIA":
            return BRLMQLPrintSettingsLabelSize.roundW58DIA
        case "W62H60":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H60
        case "W62H75":
            return BRLMQLPrintSettingsLabelSize.dieCutW62H75
        default:
            return BRLMQLPrintSettingsLabelSize.rollW62
        }
    }
}

enum PrinterModel: String, CaseIterable {
    //    case PJ_673 = "PJ-673" // swiftlint:disable:this identifier_name
    //    case PJ_763MFi = "PJ-763" // swiftlint:disable:this identifier_name
    //    case PJ_773 = "PJ-773" // swiftlint:disable:this identifier_name
    //    case MW_145MFi = "MW-145MFi" // swiftlint:disable:this identifier_name
    //    case MW_260MFi = "MW-260MFi" // swiftlint:disable:this identifier_name
    //    case MW_170 = "MW-170" // swiftlint:disable:this identifier_name
    //    case MW_270 = "MW-270" // swiftlint:disable:this identifier_name
    //    case RJ_4030Ai = "RJ-4030Ai" // swiftlint:disable:this identifier_name
    //    case RJ_4040 = "RJ-4040" // swiftlint:disable:this identifier_name
    //    case RJ_3050 = "RJ-3050" // swiftlint:disable:this identifier_name
    //    case RJ_3150 = "RJ-3150" // swiftlint:disable:this identifier_name
    //    case RJ_3050Ai = "RJ-3050Ai" // swiftlint:disable:this identifier_name
    //    case RJ_3150Ai = "RJ-3150Ai" // swiftlint:disable:this identifier_name
    //    case RJ_2050 = "RJ-2050" // swiftlint:disable:this identifier_name
    //    case RJ_2140 = "RJ-2140" // swiftlint:disable:this identifier_name
    //    case RJ_2150 = "RJ-2150" // swiftlint:disable:this identifier_name
    //    case RJ_4230B = "RJ-4230B" // swiftlint:disable:this identifier_name
    //    case RJ_4250WB = "RJ-4250WB" // swiftlint:disable:this identifier_name
    //    case TD_2120N = "TD-2120N" // swiftlint:disable:this identifier_name
    //    case TD_2130N = "TD-2130N" // swiftlint:disable:this identifier_name
    //    case TD_4100N = "TD-4100N" // swiftlint:disable:this identifier_name
    //    case TD_4420DN = "TD-4420DN" // swiftlint:disable:this identifier_name
    //    case TD_4520DN = "TD-4520DN" // swiftlint:disable:this identifier_name
    //    case TD_4550DNWB = "TD-4550DNWB" // swiftlint:disable:this identifier_name
    //    case QL_710W = "QL-710W" // swiftlint:disable:this identifier_name
    //    case QL_720NW = "QL-720NW" // swiftlint:disable:this identifier_name
    case QL_810W = "QL-810W" // swiftlint:disable:this identifier_name
    case QL_820NWB = "QL-820NWB" // swiftlint:disable:this identifier_name
    //    case QL_1110NWB = "QL-1110NWB" // swiftlint:disable:this identifier_name
    //    case QL_1115NWB = "QL-1115NWB" // swiftlint:disable:this identifier_name
    //    case PT_E550W = "PT-E550W" // swiftlint:disable:this identifier_name
    //    case PT_P750W = "PT-P750W" // swiftlint:disable:this identifier_name
    //    case PT_D800W = "PT-D800W" // swiftlint:disable:this identifier_name
    //    case PT_E800W = "PT-E800W" // swiftlint:disable:this identifier_name
    //    case PT_E850TKW = "PT-E850TKW" // swiftlint:disable:this identifier_name
    //    case PT_P900W = "PT-P900W" // swiftlint:disable:this identifier_name
    //    case PT_P950NW = "PT-P950NW" // swiftlint:disable:this identifier_name
    //    case PT_P300BT = "PT-P300BT" // swiftlint:disable:this identifier_name
    //    case PT_P710BT = "PT-P710BT" // swiftlint:disable:this identifier_name
    //    case PT_P715eBT = "PT-P715eBT" // swiftlint:disable:this identifier_name
    //    case PT_P910BT = "PT-P910BT" // swiftlint:disable:this identifier_name
    //    case RJ_3230B = "RJ-3230B" // swiftlint:disable:this identifier_name
    //    case RJ_3250WB = "RJ-3250WB" // swiftlint:disable:this identifier_name
    //    case PT_D410 = "PT-D410" // swiftlint:disable:this identifier_name
    //    case PT_D460BT = "PT-D460BT" // swiftlint:disable:this identifier_name
    //    case PT_D610BT = "PT-D610BT" // swiftlint:disable:this identifier_name
    //    case PJ_822 = "PJ-822" // swiftlint:disable:this identifier_name
    //    case PJ_823 = "PJ-823" // swiftlint:disable:this identifier_name
    //    case PJ_862 = "PJ-862" // swiftlint:disable:this identifier_name
    //    case PJ_863 = "PJ-863" // swiftlint:disable:this identifier_name
    //    case PJ_883 = "PJ-883" // swiftlint:disable:this identifier_name
        case TD_2030A = "TD-2030A" // swiftlint:disable:this identifier_name
    //    case TD_2125N = "TD-2125N" // swiftlint:disable:this identifier_name
    //    case TD_2125NWB = "TD-2125NWB" // swiftlint:disable:this identifier_name
    //    case TD_2135N = "TD-2135N" // swiftlint:disable:this identifier_name
    //    case TD_2135NWB = "TD-2135NWB" // swiftlint:disable:this identifier_name
    //    case PT_E310BT = "PT-E310BT" // swiftlint:disable:this identifier_name
    //    case PT_E510 = "PT-E510" // swiftlint:disable:this identifier_name
    //    case PT_E560BT = "PT-E560BT" // swiftlint:disable:this identifier_name
    //    case TD_2310D_203 = "TD-2310D_203" // swiftlint:disable:this identifier_name
    //    case TD_2310D_300 = "TD-2310D_300" // swiftlint:disable:this identifier_name
        case TD_2320D_203 = "TD-2320D_203" // swiftlint:disable:this identifier_name
    //    case TD_2320D_300 = "TD-2320D_300" // swiftlint:disable:this identifier_name
    //    case TD_2320DF_203 = "TD-2320DF_203" // swiftlint:disable:this identifier_name
    //    case TD_2320DF_300 = "TD-2320DF_300" // swiftlint:disable:this identifier_name
    //    case TD_2320DSA_203 = "TD-2320DSA_203" // swiftlint:disable:this identifier_name
    //    case TD_2320DSA_300 = "TD-2320DSA_300" // swiftlint:disable:this identifier_name
        case TD_2350D_203 = "TD-2350D_203" // swiftlint:disable:this identifier_name
    //    case TD_2350D_300 = "TD-2350D_300" // swiftlint:disable:this identifier_name
    //    case TD_2350DF_203 = "TD-2350DF_203" // swiftlint:disable:this identifier_name
    //    case TD_2350DF_300 = "TD-2350DF_300" // swiftlint:disable:this identifier_name
    //    case TD_2350DSA_203 = "TD-2350DSA_203" // swiftlint:disable:this identifier_name
    //    case TD_2350DSA_300 = "TD-2350DSA_300" // swiftlint:disable:this identifier_name
    //    case TD_2350DFSA_203 = "TD-2350DFSA_203" // swiftlint:disable:this identifier_name
    //    case TD_2350DFSA_300 = "TD-2350DFSA_300" // swiftlint:disable:this identifier_name

    init?(sdkModelName: String) {
        guard let printer = PrinterModel.allCases.first(where: { $0.sdkModelName == sdkModelName }) else { return nil }
        self = printer
    }

    init?(modelName: String) {
        guard let printer = PrinterModel.allCases.first(where: { $0.rawValue == modelName }) else { return nil }
        self = printer
    }

    static let defaultPrinter = PrinterModel.QL_820NWB

    var sdkModelName: String {
        return "Brother \(rawValue)"
    }

    var searchModelName: String {
        var modelName = sdkModelName

        // Remove resolution info for TD-2a series
        let string203 = "_203"
        let string300 = "_300"
        if rawValue.contains(string203) {
            modelName = modelName.replacingOccurrences(of: string203, with: "")
        } else if rawValue.contains(string300) {
            modelName = modelName.replacingOccurrences(of: string300, with: "")
        }
        return modelName

    }

    var printerModel: BRLMPrinterModel {
        switch self {
        //        case .PJ_673: return .PJ_673
        //        case .PJ_763MFi: return .pj_763MFi
        //        case .PJ_773: return .PJ_773
        //        case .MW_145MFi: return .mw_145MFi
        //        case .MW_260MFi: return .mw_260MFi
        //        case .MW_170: return .MW_170
        //        case .MW_270: return .MW_270
        //        case .RJ_4030Ai: return .rj_4030Ai
        //        case .RJ_4040: return .RJ_4040
        //        case .RJ_3050: return .RJ_3050
        //        case .RJ_3150: return .RJ_3150
        //        case .RJ_3050Ai: return .rj_3050Ai
        //        case .RJ_3150Ai: return .rj_3150Ai
        //        case .RJ_2050: return .RJ_2050
        //        case .RJ_2140: return .RJ_2140
        //        case .RJ_2150: return .RJ_2150
        //        case .RJ_4230B: return .RJ_4230B
        //        case .RJ_4250WB: return .RJ_4250WB
        //        case .TD_2120N: return .TD_2120N
        //        case .TD_2130N: return .TD_2130N
        //        case .TD_4100N: return .TD_4100N
        //        case .TD_4420DN: return .TD_4420DN
        //        case .TD_4520DN: return .TD_4520DN
        //        case .TD_4550DNWB: return .TD_4550DNWB
        //        case .QL_710W: return .QL_710W
        //        case .QL_720NW: return .QL_720NW
        case .QL_810W: return .QL_810W
        case .QL_820NWB: return .QL_820NWB
        //        case .QL_1110NWB: return .QL_1110NWB
        //        case .QL_1115NWB: return .QL_1115NWB
        //        case .PT_E550W: return .PT_E550W
        //        case .PT_P750W: return .PT_P750W
        //        case .PT_D800W: return .PT_D800W
        //        case .PT_E800W: return .PT_E800W
        //        case .PT_E850TKW: return .PT_E850TKW
        //        case .PT_P900W: return .PT_P900W
        //        case .PT_P950NW: return .PT_P950NW
        //        case .PT_P300BT: return .PT_P300BT
        //        case .PT_P710BT: return .PT_P710BT
        //        case .PT_P715eBT: return .pt_P715eBT
        //        case .PT_P910BT: return .PT_P910BT
        //        case .RJ_3230B: return .RJ_3230B
        //        case .RJ_3250WB: return .RJ_3250WB
        //        case .PT_D410: return .PT_D410
        //        case .PT_D460BT: return .PT_D460BT
        //        case .PT_D610BT: return .PT_D610BT
        //        case .PJ_822: return .PJ_822
        //        case .PJ_823: return .PJ_823
        //        case .PJ_862: return .PJ_862
        //        case .PJ_863: return .PJ_863
        //        case .PJ_883: return .PJ_883
                case .TD_2030A: return .TD_2030A
        //        case .TD_2125N: return .TD_2125N
        //        case .TD_2125NWB: return .TD_2125NWB
        //        case .TD_2135N: return .TD_2135N
        //        case .TD_2135NWB: return .TD_2135NWB
        //        case .PT_E310BT: return .PT_E310BT
        //        case .PT_E510: return .PT_E510
        //        case .PT_E560BT: return .PT_E560BT
        //        case .TD_2310D_203: return .TD_2310D_203
        //        case .TD_2310D_300: return .TD_2310D_300
                case .TD_2320D_203: return .TD_2320D_203
        //        case .TD_2320D_300: return .TD_2320D_300
        //        case .TD_2320DF_203: return .TD_2320DF_203
        //        case .TD_2320DF_300: return .TD_2320DF_300
        //        case .TD_2320DSA_203: return .TD_2320DSA_203
        //        case .TD_2320DSA_300: return .TD_2320DSA_300
                case .TD_2350D_203: return .TD_2350D_203
        //        case .TD_2350D_300: return .TD_2350D_300
        //        case .TD_2350DF_203: return .TD_2350DF_203
        //        case .TD_2350DF_300: return .TD_2350DF_300
        //        case .TD_2350DSA_203: return .TD_2350DSA_203
        //        case .TD_2350DSA_300: return .TD_2350DSA_300
        //        case .TD_2350DFSA_203: return .TD_2350DFSA_203
        //        case .TD_2350DFSA_300: return .TD_2350DFSA_300
        }
    }
}
