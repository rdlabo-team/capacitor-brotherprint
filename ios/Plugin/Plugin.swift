import Foundation
import Capacitor
import BRLMPrinterKit
import BRPtouchPrinterKit

/**
 * Please read the Capacitor iOS Plugin Development Guide
 * here: https://capacitorjs.com/docs/plugins/ios
 */
@objc(BrotherPrint)
public class BrotherPrint: CAPPlugin {
    private var cancelRoutine: (() -> Void)?
    
    @objc func printImage(_ call: CAPPluginCall) {
        let encodedImage: String = call.getString("encodedImage") ?? ""
        if encodedImage == "" {
            call.reject("Error - Image data is not found.")
            return
        }

        let newImageData = Data(base64Encoded: encodedImage, options: [])

        // 検索からデバイス情報が得られた場合
        let port: String = call.getString("port", "wifi")
        let localName: String = call.getString("localName", "")
        let ipAddress: String = call.getString("ipAddress", "")
        let serialNumber: String = call.getString("serialNumber", "")
        let autoCut: Bool = call.getBool("autoCut", true)

        let modelName = self.getModelName(from: call.getString("modelName", "QL-820NWB"))
        let labelSize = self.getLabelSize(from: call.getString("labelName", "rollW62"))
        
        NSLog(call.getString("modelName", "not set"));
        NSLog(call.getString("labelName", "not set"));

        // メインスレッドにて処理
        DispatchQueue.main.async {
            var channel: BRLMChannel

            switch port {
            case "wifi":
                channel = BRLMChannel(wifiIPAddress: ipAddress)
            case "bluetooth":
                channel = BRLMChannel(bluetoothSerialNumber: serialNumber)
            case "bluetoothLowEnergy":
                channel = BRLMChannel(bleLocalName: localName)
            default:
                call.reject("Error - connection is not found.")
                return
            }

            let generateResult = BRLMPrinterDriverGenerator.open(channel)
            guard generateResult.error.code == BRLMOpenChannelErrorCode.noError,
                  let printerDriver = generateResult.driver else {
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "value": generateResult.error.code
                ])
                NSLog("Error - Open Channel: \(generateResult.error.code)")
                return
            }

            guard
                let decodedByte = UIImage(data: newImageData! as Data),
                let printSettings = BRLMQLPrintSettings(defaultPrintSettingsWith: modelName)
            else {
                printerDriver.closeChannel()
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "value": "Error - Image file is not found."
                ])
                return
            }

            printSettings.labelSize = labelSize
            printSettings.autoCut = autoCut
            printSettings.numCopies = UInt(call.getInt("numberOfCopies", 1))

            let printError = printerDriver.printImage(with: decodedByte.cgImage!, settings: printSettings)

            if printError.code != .noError {
                printerDriver.closeChannel()
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "value": printError.code
                ])
                return
            } else {
                NSLog("Success - Print Image")
                printerDriver.closeChannel()
                call.resolve()
            }
        }
    }

    @objc func search(_ call: CAPPluginCall) {
        switch call.getString("port", "wifi") {
        case "wifi":
            self.searchWiFiPrinter(call)
        case "bluetooth":
            self.checkBLEChannel(call)
        case "bluetoothLowEnergy":
            self.searchBLEPrinter(call)
        default:
            call.reject("port is not 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'")
        }
    }

    private func searchWiFiPrinter(_ call: CAPPluginCall) {
        DispatchQueue.main.async {
            self.cancelRoutine = {
                BRLMPrinterSearcher.cancelNetworkSearch()
            }
            
            let option = BRLMNetworkSearchOption()
            option.printerList = PrinterModel.allCases.map { $0.searchModelName }
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15))

            NSLog("BRLMPrinterSearcher.startNetworkSearch")
            BRLMPrinterSearcher.startNetworkSearch(option) { channel in
                NSLog(channel.channelInfo)
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "wifi", channel: channel))
            }
            self.cancelRoutine = nil
            call.resolve()
        }
    }
    

    private func checkBLEChannel(_ call: CAPPluginCall) {
        DispatchQueue.main.async {
            let channels = BRLMPrinterSearcher.startBluetoothSearch().channels
            for channel in channels {
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "wifi", channel: channel))
            }
            call.resolve()
        }
    }

    private func searchBLEPrinter(_ call: CAPPluginCall) {
        DispatchQueue.main.async {
            let option = BRLMBLESearchOption()
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15))
            let result = BRLMPrinterSearcher.startBLESearch(option) { channel in
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "wifi", channel: channel))
            }
            call.resolve()
        }
    }
    
    private func chanelToPrinter(port: String, channel: BRLMChannel) -> [String: String] {
        let modelName = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyModelName) as? String ?? ""
        let serialNumber = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeySerialNumber) as? String ?? ""
        let macAddress = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyMacAddress) as? String ?? ""
        let nodeName = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyNodeName) as? String ?? ""
        let location = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyLocation) as? String ?? ""
        let ipaddress = channel.channelInfo
        
        return [
            "port": port,
            "modelName": modelName,
            "serialNumber": serialNumber,
            "macAddress": macAddress,
            "nodeName": nodeName,
            "location": location,
            "ipAddress": ipaddress
        ]
    }

    @objc func cancelSearchWiFiPrinter(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            self.cancelRoutine?()
            self.cancelRoutine = nil
        }
    }

    @objc func cancelSearchBluetoothPrinter(_ call: CAPPluginCall) {
        DispatchQueue.main.async {
            BRLMPrinterSearcher.cancelBLESearch()
        }
    }

    private func getModelName(from: String) -> BRLMPrinterModel {
        switch from {
        case "QL-810W":
            return BRLMPrinterModel.QL_810W
        case "QL-820NWB":
            return BRLMPrinterModel.QL_820NWB
        default:
            return BRLMPrinterModel.unknown
        }
    }

    private func getLabelSize(from: String) -> BRLMQLPrintSettingsLabelSize {
        switch from {
        case "rollW29":
            return BRLMQLPrintSettingsLabelSize.rollW29
        case "rollW62":
            return BRLMQLPrintSettingsLabelSize.rollW62
        case "rollW62RB":
            return BRLMQLPrintSettingsLabelSize.rollW62RB
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
    //    case TD_2030A = "TD-2030A" // swiftlint:disable:this identifier_name
    //    case TD_2125N = "TD-2125N" // swiftlint:disable:this identifier_name
    //    case TD_2125NWB = "TD-2125NWB" // swiftlint:disable:this identifier_name
    //    case TD_2135N = "TD-2135N" // swiftlint:disable:this identifier_name
    //    case TD_2135NWB = "TD-2135NWB" // swiftlint:disable:this identifier_name
    //    case PT_E310BT = "PT-E310BT" // swiftlint:disable:this identifier_name
    //    case PT_E510 = "PT-E510" // swiftlint:disable:this identifier_name
    //    case PT_E560BT = "PT-E560BT" // swiftlint:disable:this identifier_name
    //    case TD_2310D_203 = "TD-2310D_203" // swiftlint:disable:this identifier_name
    //    case TD_2310D_300 = "TD-2310D_300" // swiftlint:disable:this identifier_name
    //    case TD_2320D_203 = "TD-2320D_203" // swiftlint:disable:this identifier_name
    //    case TD_2320D_300 = "TD-2320D_300" // swiftlint:disable:this identifier_name
    //    case TD_2320DF_203 = "TD-2320DF_203" // swiftlint:disable:this identifier_name
    //    case TD_2320DF_300 = "TD-2320DF_300" // swiftlint:disable:this identifier_name
    //    case TD_2320DSA_203 = "TD-2320DSA_203" // swiftlint:disable:this identifier_name
    //    case TD_2320DSA_300 = "TD-2320DSA_300" // swiftlint:disable:this identifier_name
    //    case TD_2350D_203 = "TD-2350D_203" // swiftlint:disable:this identifier_name
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
        //        case .TD_2030A: return .TD_2030A
        //        case .TD_2125N: return .TD_2125N
        //        case .TD_2125NWB: return .TD_2125NWB
        //        case .TD_2135N: return .TD_2135N
        //        case .TD_2135NWB: return .TD_2135NWB
        //        case .PT_E310BT: return .PT_E310BT
        //        case .PT_E510: return .PT_E510
        //        case .PT_E560BT: return .PT_E560BT
        //        case .TD_2310D_203: return .TD_2310D_203
        //        case .TD_2310D_300: return .TD_2310D_300
        //        case .TD_2320D_203: return .TD_2320D_203
        //        case .TD_2320D_300: return .TD_2320D_300
        //        case .TD_2320DF_203: return .TD_2320DF_203
        //        case .TD_2320DF_300: return .TD_2320DF_300
        //        case .TD_2320DSA_203: return .TD_2320DSA_203
        //        case .TD_2320DSA_300: return .TD_2320DSA_300
        //        case .TD_2350D_203: return .TD_2350D_203
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

public enum BrotherPrinterEvent: String {
    case onPrinterAvailable = "onPrinterAvailable"
    case onPrint = "onPrint"
    case onPrintFailedCommunication = "onPrintFailedCommunication"
    case onPrintError = "onPrintError"
}
