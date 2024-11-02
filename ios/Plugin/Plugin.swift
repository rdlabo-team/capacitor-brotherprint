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
    private var cancelRoutineWiFi: (() -> Void)?
    private var cancelRoutineBluetooth: (() -> Void)?

    @objc func printImage(_ call: CAPPluginCall) {
        let encodedImage: String = call.getString("encodedImage", "")
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

        let modelName: String = call.getString("modelName", "QL-820NWB")
        let printerModel = BrotherModel.getModelName(from: modelName)

        NSLog(call.getString("modelName", "not set"))
        NSLog(call.getString("labelName", "not set"))

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
                let message = OpenChannelErrorModel.fetchChannelErrorCode(error: generateResult.error.code)
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "message": message,
                    "code": generateResult.error.code
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
                    "message": "Error - Create decodedByte From ImageData is failed."
                ])
                call.reject("Error - Create decodedByte From ImageData is failed.")
                return
            }

            var printSettings: BRLMPrintSettingsProtocol

            if modelName.hasPrefix("QL") {
                guard
                    let _printSettings = BRLMQLPrintSettings(defaultPrintSettingsWith: printerModel)
                else {
                    printerDriver.closeChannel()
                    self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                        "code": 0,
                        "message": "Error - Create BRLMQLPrintSettings with " + modelName + " is failed."
                    ])
                    call.reject("Error - Create BRLMQLPrintSettings with " + modelName + " is failed.")
                    return
                }
                printSettings = PrinterSettingsModel.QLModelSettings(call, printSettings: _printSettings)

            } else if modelName.hasPrefix("TD") {
                guard
                    let _printSettings = BRLMTDPrintSettings(defaultPrintSettingsWith: printerModel)
                else {
                    printerDriver.closeChannel()
                    self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                        "code": 0,
                        "message": "Error - Create BRLMTDPrintSettings with " + modelName + " is failed."
                    ])
                    call.reject("Error - Create BRLMTDPrintSettings with " + modelName + " is failed.")
                    return
                }
                printSettings = PrinterSettingsModel.TDModelSettings(call, printSettings: _printSettings)

            } else {
                printerDriver.closeChannel()
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "code": 0,
                    "message": "Error - " + modelName + " is not supported"
                ])
                call.reject("Error - " + modelName + " is not supported")
                return
            }

            let printError = printerDriver.printImage(with: decodedByte.cgImage!, settings: printSettings)
            
            if printError.code != BRLMPrintErrorCode.noError {
                printerDriver.closeChannel()
                let message = PrintErrorModel.fetchErrorCode(errorCode: Int32(printError.code.rawValue))
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: [
                    "message": message,
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
        DispatchQueue.global().async {
            self.cancelRoutineWiFi = {
                BRLMPrinterSearcher.cancelNetworkSearch()
            }

            let option = BRLMNetworkSearchOption()
            option.printerList = PrinterModel.allCases.map { $0.searchModelName }
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15))

            NSLog("BRLMPrinterSearcher.startNetworkSearch")
            BRLMPrinterSearcher.startNetworkSearch(option) { channel in
                NSLog(channel.channelInfo)
                let printer = self.chanelToPrinter(port: "wifi", channel: channel)
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: printer)
            }
            self.cancelRoutineWiFi = nil
            call.resolve()
        }
    }

    private func checkBLEChannel(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            let searcher = BRLMPrinterSearcher.startBluetoothSearch()
            if (searcher.error.code != BRLMPrinterSearchErrorCode.noError) {
                call.reject("Error - Bluetooth is not found. Error code: " + String(searcher.error.code.rawValue))
                return
            }
            if (!searcher.channels.isEmpty) {
                for channel in searcher.channels {
                    NSLog(channel.channelInfo)
                    self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "bluetooth", channel: channel))
                }
                call.resolve()
            } else {
                NSLog("startBluetoothSearch can't find. Next start startBluetoothAccessorySearch.")
                BRLMPrinterSearcher.startBluetoothAccessorySearch() { result in
                    for channel in result.channels {
                        self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "bluetooth", channel: channel))
                    }
                    call.resolve()
                }
            }
        }
    }

    private func searchBLEPrinter(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            self.cancelRoutineBluetooth = {
                BRLMPrinterSearcher.cancelBLESearch()
            }
            let option = BRLMBLESearchOption()
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15))
            NSLog("BRLMPrinterSearcher.startBLESearch")
            BRLMPrinterSearcher.startBLESearch(option) { channel in
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "bluetoothLowEnergy", channel: channel))
            }
            self.cancelRoutineBluetooth = nil
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
            "location": location
        ]
    }

    @objc func cancelSearchWiFiPrinter(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            self.cancelRoutineWiFi?()
            self.cancelRoutineWiFi = nil
        }
    }

    @objc func cancelSearchBluetoothPrinter(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            self.cancelRoutineBluetooth?()
            self.cancelRoutineBluetooth = nil
        }
    }
}
