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
                    "message": OpenChannelErrorModel.fetchChannelErrorCode(error: generateResult.error.code),
                    "code": generateResult.error.code,
                ])
                call.reject("Error - Open Channel: \(generateResult.error.code)")
                return
            }

            guard
                let decodedByte = UIImage(data: newImageData! as Data)
            else {
                printerDriver.closeChannel()
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "code": 0,
                    "message": "Error - Create decodedByte From ImageData is failed.",
                ])
                call.reject("Error - Create decodedByte From ImageData is failed.")
                return
            }
            
            guard
                let printSettings = BRLMQLPrintSettings(defaultPrintSettingsWith: modelName)
            else {
                printerDriver.closeChannel()
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "code": 0,
                    "message": "Error - Create BRLMQLPrintSettings is failed.",
                ])
                call.reject("Error - Create BRLMQLPrintSettings is failed.")
                return
            }

            printSettings.labelSize = labelSize
            printSettings.autoCut = autoCut
            printSettings.numCopies = UInt(call.getInt("numberOfCopies", 1))

            let printError = printerDriver.printImage(with: decodedByte.cgImage!, settings: printSettings)

            if printError.code != .noError {
                printerDriver.closeChannel()
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "message": PrintErrorModel.fetchErrorCode(errorCode: Int32(printError.code.rawValue)),
                    "code": printError.code.rawValue
                ])
                call.reject("Error - Print Image: \(printError.code)")
                return
            }
            
            printerDriver.closeChannel()
            call.resolve()
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
                let printer = self.chanelToPrinter(port: "wifi", channel: channel);
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: printer)
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
    
    private func chanelToPrinter(port: String, channel: BRLMChannel) -> JSObject {
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
            "ipAddress": ipaddress,
            "nodeName": nodeName,
            "location": location,
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
        case "dieCutW29H90":
            return BRLMQLPrintSettingsLabelSize.dieCutW29H90
        case "rollW62":
            return BRLMQLPrintSettingsLabelSize.rollW62
        case "rollW62RB":
            return BRLMQLPrintSettingsLabelSize.rollW62RB
        default:
            return BRLMQLPrintSettingsLabelSize.rollW62
        }
    }
}


public enum BrotherPrinterEvent: String {
    case onPrinterAvailable = "onPrinterAvailable"
    case onPrint = "onPrint"
    case onPrintFailedCommunication = "onPrintFailedCommunication"
    case onPrintError = "onPrintError"
}
