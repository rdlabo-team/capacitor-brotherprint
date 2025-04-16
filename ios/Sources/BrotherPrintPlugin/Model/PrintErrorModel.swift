//
//  PrintErrorModel.swift
//  Brother Print SDK Demo
//
//  Created by Brother Industries, Ltd. on 2023/2/23.
//

import BRLMPrinterKit
import BRPtouchPrinterKit
import Foundation

class PrinterSearchErrorModel {
    static func fetchChannelErrorCode(error: BRLMPrinterSearchErrorCode) -> String {
        switch error {
        case .noError:
            return "noError"
        case .canceled:
            return "canceled"
        case .alreadySearching:
            return "alreadySearching"
        case .unsupported:
            return "unsupported"
        case .unknownError:
            return "unknownError"
        @unknown default:
            return "unknown"
        }
    }
}

class RequestPrinterInfoErrorModel {
    static func fetchGetPrinterInfoErrorCode(error: BRLMRequestPrinterInfoErrorCode) -> String {
        switch error {
        case .noError:
            return "noError"
        case .connectionFailed:
            return "connectionFailed"
        case .unsupported:
            return "unsupported"
        case .unknownError:
            return "unknownError"
        @unknown default:
            return "unknownError"
        }
    }
}

class OpenChannelErrorModel {
    static func fetchChannelErrorCode(error: BRLMOpenChannelErrorCode) -> String {
        switch error {
        case .noError:
            return "noError"
        case .openStreamFailure:
            return "openStreamFailure"
        case .timeout:
            return "timeout"
        @unknown default:
            return "unknown"
        }
    }
}

class PrintErrorModel { // swiftlint:disable:this type_body_length
    static func fetchErrorCode(errorCode: Int32?) -> String { // swiftlint:disable:this function_body_length
        let errorCodeDic: [Int32: String] = [
            ERROR_NONE_: "ERROR_NONE_",
            ERROR_TIMEOUT: "ERROR_TIMEOUT",
            ERROR_BADPAPERRES: "ERROR_BADPAPERRES",
            ERROR_IMAGELARGE: "ERROR_IMAGELARGE",
            ERROR_CREATESTREAM: "ERROR_CREATESTREAM",
            ERROR_OPENSTREAM: "ERROR_OPENSTREAM",
            ERROR_FILENOTEXIST: "ERROR_FILENOTEXIST",
            ERROR_PAGERANGEERROR: "ERROR_PAGERANGEERROR",
            ERROR_NOT_SAME_MODEL_: "ERROR_NOT_SAME_MODEL_",
            ERROR_BROTHER_PRINTER_NOT_FOUND_: "ERROR_BROTHER_PRINTER_NOT_FOUND_",
            ERROR_PAPER_EMPTY_: "ERROR_PAPER_EMPTY_",
            ERROR_BATTERY_EMPTY_: "ERROR_BATTERY_EMPTY_",
            ERROR_COMMUNICATION_ERROR_: "ERROR_COMMUNICATION_ERROR_",
            ERROR_OVERHEAT_: "ERROR_OVERHEAT_",
            ERROR_PAPER_JAM_: "ERROR_PAPER_JAM_",
            ERROR_HIGH_VOLTAGE_ADAPTER_: "ERROR_HIGH_VOLTAGE_ADAPTER_",
            ERROR_CHANGE_CASSETTE_: "ERROR_CHANGE_CASSETTE_",
            ERROR_FEED_OR_CASSETTE_EMPTY_: "ERROR_FEED_OR_CASSETTE_EMPTY_",
            ERROR_SYSTEM_ERROR_: "ERROR_SYSTEM_ERROR_",
            ERROR_NO_CASSETTE_: "ERROR_NO_CASSETTE_",
            ERROR_WRONG_CASSENDTE_DIRECT_: "ERROR_WRONG_CASSENDTE_DIRECT_",
            ERROR_CREATE_SOCKET_FAILED_: "ERROR_CREATE_SOCKET_FAILED_",
            ERROR_CONNECT_SOCKET_FAILED_: "ERROR_CONNECT_SOCKET_FAILED_",
            ERROR_GET_OUTPUT_STREAM_FAILED_: "ERROR_GET_OUTPUT_STREAM_FAILED_",
            ERROR_GET_INPUT_STREAM_FAILED_: "ERROR_GET_INPUT_STREAM_FAILED_",
            ERROR_CLOSE_SOCKET_FAILED_: "ERROR_CLOSE_SOCKET_FAILED_",
            ERROR_OUT_OF_MEMORY_: "ERROR_OUT_OF_MEMORY_",
            ERROR_SET_OVER_MARGIN_: "ERROR_SET_OVER_MARGIN_",
            ERROR_NO_SD_CARD_: "ERROR_NO_SD_CARD_",
            ERROR_FILE_NOT_SUPPORTED_: "ERROR_FILE_NOT_SUPPORTED_",
            ERROR_EVALUATION_TIMEUP_: "ERROR_EVALUATION_TIMEUP_",
            ERROR_WRONG_CUSTOM_INFO_: "ERROR_WRONG_CUSTOM_INFO_",
            ERROR_NO_ADDRESS_: "ERROR_NO_ADDRESS_",
            ERROR_NOT_MATCH_ADDRESS_: "ERROR_NOT_MATCH_ADDRESS_",
            ERROR_FILE_NOT_FOUND_: "ERROR_FILE_NOT_FOUND_",
            ERROR_TEMPLATE_FILE_NOT_MATCH_MODEL_: "ERROR_TEMPLATE_FILE_NOT_MATCH_MODEL_",
            ERROR_TEMPLATE_NOT_TRANS_MODEL_: "ERROR_TEMPLATE_NOT_TRANS_MODEL_",
            ERROR_COVER_OPEN_: "ERROR_COVER_OPEN_",
            ERROR_WRONG_LABEL_: "ERROR_WRONG_LABEL_",
            ERROR_PORT_NOT_SUPPORTED_: "ERROR_PORT_NOT_SUPPORTED_",
            ERROR_WRONG_TEMPLATE_KEY_: "ERROR_WRONG_TEMPLATE_KEY_",
            ERROR_BUSY_: "ERROR_BUSY_",
            ERROR_TEMPLATE_NOT_PRINT_MODEL_: "ERROR_TEMPLATE_NOT_PRINT_MODEL_",
            ERROR_CANCEL_: "ERROR_CANCEL_",
            ERROR_PRINTER_SETTING_NOT_SUPPORTED_: "ERROR_PRINTER_SETTING_NOT_SUPPORTED_",
            ERROR_INVALID_PARAMETER_: "ERROR_INVALID_PARAMETER_",
            ERROR_INTERNAL_ERROR_: "ERROR_INTERNAL_ERROR_",
            ERROR_TEMPLATE_NOT_CONTROL_MODEL_: "ERROR_TEMPLATE_NOT_CONTROL_MODEL_",
            ERROR_TEMPLATE_NOT_EXIST_: "ERROR_TEMPLATE_NOT_EXIST_",
            ERROR_BADENCRYPT_: "ERROR_BADENCRYPT_",
            ERROR_BUFFER_FULL_: "ERROR_BUFFER_FULL_",
            ERROR_TUBE_EMPTY_: "ERROR_TUBE_EMPTY_",
            ERROR_TUBE_RIBON_EMPTY_: "ERROR_TUBE_RIBON_EMPTY_",
            ERROR_UPDATE_FRIM_NOT_SUPPORTED_: "ERROR_UPDATE_FRIM_NOT_SUPPORTED_",
            ERROR_OS_VERSION_NOT_SUPPORTED_: "ERROR_OS_VERSION_NOT_SUPPORTED_",
            ERROR_MINIMUM_LENGTH_LIMIT_: "ERROR_MINIMUM_LENGTH_LIMIT_",
            ERROR_FAIL_TO_CONVERT_CSV_TO_BLF_: "ERROR_FAIL_TO_CONVERT_CSV_TO_BLF_",
            ERROR_RESOLUTION_MODE_: "ERROR_RESOLUTION_MODE_"
        ]
        if let errorCode = errorCode {
            return errorCodeDic[errorCode] ?? "ERROR_UNKNOWN"
        } else {
            return "ERROR_UNKNOWN"
        }
    }

    static func fetchPrinterStatusErrorCode(error: BRLMPrinterStatusErrorCode?) -> String {
        switch error {
        case .noError:
            return "noError"
        case .noPaper:
            return "noPaper"
        case .coverOpen:
            return "coverOpen"
        case .busy:
            return "busy"
        case .paperJam:
            return "paperJam"
        case .highVoltageAdapter:
            return "highVoltageAdapter"
        case .batteryEmpty:
            return "batteryEmpty"
        case .batteryTrouble:
            return "batteryTrouble"
        case .tubeNotDetected:
            return "tubeNotDetected"
        case .systemError:
            return "systemError"
        case .anotherError:
            return "anotherError"
        case .motorSlow:
            return "motorSlow"
        case .unsupportedCharger:
            return "unsupportedCharger"
        case .incompatibleOptionalEquipment:
            return "incompatibleOptionalEquipment"
        case .none:
            return "none"
        @unknown default:
            return "unknown"
        }
    }

    static func fetchPrintErrorCode(error: BRLMPrintErrorCode?) -> String { // swiftlint:disable:this cyclomatic_complexity function_body_length line_length
        switch error {
        case .noError:
            return "noError"
        case .printSettingsError:
            return "printSettingsError"
        case .filepathURLError:
            return "filepathURLError"
        case .pdfPageError:
            return "pdfPageError"
        case .printSettingsNotSupportError:
            return "printSettingsNotSupportError"
        case .dataBufferError:
            return "dataBufferError"
        case .printerModelError:
            return "printerModelError"
        case .canceled:
            return "canceled"
        case .channelTimeout:
            return "channelTimeout"
        case .setModelError:
            return "setModelError"
        case .unsupportedFile:
            return "unsupportedFile"
        case .setMarginError:
            return "setMarginError"
        case .setLabelSizeError:
            return "setLabelSizeError"
        case .customPaperSizeError:
            return "customPaperSizeError"
        case .setLengthError:
            return "setLengthError"
        case .tubeSettingError:
            return "tubeSettingError"
        case .channelErrorStreamStatusError:
            return "channelErrorStreamStatusError"
        case .channelErrorUnsupportedChannel:
            return "channelErrorUnsupportedChannel"
        case .printerStatusErrorPaperEmpty:
            return "printerStatusErrorPaperEmpty"
        case .printerStatusErrorCoverOpen:
            return "printerStatusErrorCoverOpen"
        case .printerStatusErrorBusy:
            return "printerStatusErrorBusy"
        case .printerStatusErrorPrinterTurnedOff:
            return "printerStatusErrorPrinterTurnedOff"
        case .printerStatusErrorBatteryWeak:
            return "printerStatusErrorBatteryWeak"
        case .printerStatusErrorExpansionBufferFull:
            return "printerStatusErrorExpansionBufferFull"
        case .printerStatusErrorCommunicationError:
            return "printerStatusErrorCommunicationError"
        case .printerStatusErrorPaperJam:
            return "printerStatusErrorPaperJam"
        case .printerStatusErrorMediaCannotBeFed:
            return "printerStatusErrorMediaCannotBeFed"
        case .printerStatusErrorOverHeat:
            return "printerStatusErrorOverHeat"
        case .printerStatusErrorHighVoltageAdapter:
            return "printerStatusErrorHighVoltageAdapter"
        case .printerStatusErrorUnknownError:
            return "printerStatusErrorUnknownError"
        case .templatePrintNotSupported:
            return "templatePrintNotSupported"
        case .invalidTemplateKey:
            return "invalidTemplateKey"
        case .printerStatusErrorMotorSlow:
            return "printerStatusErrorMotorSlow"
        case .unsupportedCharger:
            return "unsupportedCharger"
        case .printerStatusErrorIncompatibleOptionalEquipment:
            return "printerStatusErrorIncompatibleOptionalEquipment"
        case .unknownError, .none:
            return "unknownError"
        @unknown default:
            return "unknownError"
        }
    }

    static func fetchMediaTypeName(mediaType: BRLMMediaInfoMediaType?) -> String {
        switch mediaType {
        case .ptLaminate:
            return "ptLaminate"
        case .ptNonLaminate:
            return "ptNonLaminate"
        case .ptFabric:
            return "ptFabric"
        case .qlInfiniteLable:
            return "qlInfiniteLable"
        case .qlDieCutLable:
            return "qlDieCutLable"
        case .ptHeatShrink:
            return "ptHeatShrink"
        case .ptfLe:
            return "ptfLe"
        case .ptFlexibleID:
            return "ptFlexibleID"
        case .ptSatin:
            return "ptSatin"
        case .ptSelfLaminate:
            return "ptSelfLaminate"
        case .incompatible:
            return "incompatible"
        case .unknown:
            return "unknown"
        case .none:
            return "none"
        @unknown default:
            return "unknown"
        }
    }

    static func fetchPrinterBatteryStatusTernary(ternary: BRLMPrinterBatteryStatusTernary?) -> String {
        switch ternary {
        case .yes:
            return "YES"
        case .no:
            return "NO"
        case .unknown, .none:
            return "unknown"
        @unknown default:
            return "unknown"
        }
    }

    static func fetchMediaBackgroundColor( // swiftlint:disable:this cyclomatic_complexity function_body_length
        color: BRLMMediaInfoBackgroundColor?
    ) -> String {
        switch color {
        case .standard:
            return "standard"
        case .white:
            return "white"
        case .others:
            return "others"
        case .clear:
            return "clear"
        case .red:
            return "red"
        case .blue:
            return "blue"
        case .yellow:
            return "yellow"
        case .green:
            return "green"
        case .black:
            return "black"
        case .clearWithWhiteInk:
            return "clearWithWhiteInk"
        case .premiumGold:
            return "premiumGold"
        case .premiumSilver:
            return "premiumSilver"
        case .premiumOthers:
            return "premiumOthers"
        case .maskingOthers:
            return "maskingOthers"
        case .matteWhite:
            return "matteWhite"
        case .matteClear:
            return "matteClear"
        case .matteSilver:
            return "matteSilver"
        case .satinGold:
            return "satinGold"
        case .satinSilver:
            return "satinSilver"
        case .pastelPurple:
            return "pastelPurple"
        case .blueWithWhiteInk:
            return "blueWithWhiteInk"
        case .redWithWhiteInk:
            return "redWithWhiteInk"
        case .fluorescentOrange:
            return "fluorescentOrange"
        case .fluorescentYellow:
            return "fluorescentYellow"
        case .berryPink:
            return "berryPink"
        case .lightGray:
            return "lightGray"
        case .limeGreen:
            return "limeGreen"
        case .satinNavyBlue:
            return "satinNavyBlue"
        case .satinWineRed:
            return "satinWineRed"
        case .fabricYellow:
            return "fabricYellow"
        case .fabricPink:
            return "fabricPink"
        case .fabricBlue:
            return "fabricBlue"
        case .tubeWhite:
            return "tubeWhite"
        case .selfLaminatedWhite:
            return "selfLaminatedWhite"
        case .flexibleWhite:
            return "flexibleWhite"
        case .flexibleYellow:
            return "flexibleYellow"
        case .cleaningWhite:
            return "cleaningWhite"
        case .stencilWhite:
            return "stencilWhite"
        case .lightBlue_Satin:
            return "lightBlue_Satin"
        case .mint_Satin:
            return "mint_Satin"
        case .silver_Satin:
            return "silver_Satin"
        case .incompatible:
            return "incompatible"
        case .unknown:
            return "unknown"
        case .none:
            return "none"
        @unknown default:
            return "unknown"
        }
    }

    static func fetchMediaInkColor(color: BRLMMediaInfoInkColor?) -> String {
        switch color {
        case .standard:
            return "standard"
        case .white:
            return "white"
        case .others:
            return "others"
        case .red:
            return "red"
        case .blue:
            return "blue"
        case .black:
            return "black"
        case .gold:
            return "gold"
        case .redAndBlack:
            return "redAndBlack"
        case .fabricBlue:
            return "fabricBlue"
        case .cleaningBlack:
            return "cleaningBlack"
        case .stencilBlack:
            return "stencilBlack"
        case .incompatible:
            return "incompatible"
        case .unknown:
            return "unknown"
        case .none:
            return "none"
        @unknown default:
            return "unknown"
        }
    }

    static func fetchQLLabelSize( // swiftlint:disable:this cyclomatic_complexity function_body_length
        size: BRLMQLPrintSettingsLabelSize?
    ) -> String {
        switch size {
        case .dieCutW17H54:
            return "dieCutW17H54"
        case .dieCutW17H87:
            return "dieCutW17H87"
        case .dieCutW23H23:
            return "dieCutW23H23"
        case .dieCutW29H42:
            return "dieCutW29H42"
        case .dieCutW29H90:
            return "dieCutW29H90"
        case .dieCutW38H90:
            return "dieCutW38H90"
        case .dieCutW39H48:
            return "dieCutW39H48"
        case .dieCutW52H29:
            return "dieCutW52H29"
        case .dieCutW62H29:
            return "dieCutW62H29"
        case .dieCutW62H60:
            return "dieCutW62H60"
        case .dieCutW62H75:
            return "dieCutW62H75"
        case .dieCutW62H100:
            return "dieCutW62H100"
        case .dieCutW60H86:
            return "dieCutW60H86"
        case .dieCutW54H29:
            return "dieCutW54H29"
        case .dieCutW102H51:
            return "dieCutW102H51"
        case .dieCutW102H152:
            return "dieCutW102H152"
        case .dieCutW103H164:
            return "dieCutW103H164"
        case .rollW12:
            return "rollW12"
        case .rollW29:
            return "rollW29"
        case .rollW38:
            return "rollW38"
        case .rollW50:
            return "rollW50"
        case .rollW54:
            return "rollW54"
        case .rollW62:
            return "rollW62"
        case .rollW62RB:
            return "rollW62RB"
        case .rollW102:
            return "rollW102"
        case .rollW103:
            return "rollW103"
        case .dtRollW90:
            return "dtRollW90"
        case .dtRollW102:
            return "dtRollW102"
        case .dtRollW102H51:
            return "dtRollW102H51"
        case .dtRollW102H152:
            return "dtRollW102H152"
        case .roundW12DIA:
            return "roundW12DIA"
        case .roundW24DIA:
            return "roundW24DIA"
        case .roundW58DIA:
            return "roundW58DIA"
        case .none:
            return "none"
        @unknown default:
            return "unknown"
        }
    }

    static func fetchPTLabelSize( // swiftlint:disable:this cyclomatic_complexity function_body_length
        size: BRLMPTPrintSettingsLabelSize?
    ) -> String {
        switch size {
        case .width3_5mm:
            return "width3_5mm"
        case .width6mm:
            return "width6mm"
        case .width9mm:
            return "width9mm"
        case .width12mm:
            return "width12mm"
        case .width18mm:
            return "width18mm"
        case .width24mm:
            return "width24mm"
        case .width36mm:
            return "width36mm"
        case .widthHS_5_8mm:
            return "widthHS_5_8mm"
        case .widthHS_8_8mm:
            return "widthHS_8_8mm"
        case .widthHS_11_7mm:
            return "widthHS_11_7mm"
        case .widthHS_17_7mm:
            return "widthHS_17_7mm"
        case .widthHS_23_6mm:
            return "widthHS_23_6mm"
        case .widthFL_21x45mm:
            return "widthFL_21x45mm"
        case .widthHS_5_2mm:
            return "widthHS_5_2mm"
        case .widthHS_9_0mm:
            return "widthHS_9_0mm"
        case .widthHS_11_2mm:
            return "widthHS_11_2mm"
        case .widthHS_21_0mm:
            return "widthHS_21_0mm"
        case .widthHS_31_0mm:
            return "widthHS_31_0mm"
        case .none:
            return "none"
        @unknown default:
            return "unknown"
        }
    }
}
