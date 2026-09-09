import BRLMPrinterKit
import Capacitor
import Foundation

/// Please read the Capacitor iOS Plugin Development Guide
/// here: https://capacitorjs.com/docs/plugins/ios
@objc(BrotherPrintPlugin)
public class BrotherPrintPlugin: CAPPlugin, CAPBridgedPlugin {
    public let identifier = "BrotherPrint"
    public let jsName = "BrotherPrint"
    public let pluginMethods: [CAPPluginMethod] = [
        CAPPluginMethod(name: "printImage", returnType: CAPPluginReturnPromise),
        CAPPluginMethod(name: "isChannelAvailable", returnType: CAPPluginReturnPromise),
        CAPPluginMethod(name: "search", returnType: CAPPluginReturnPromise),
        CAPPluginMethod(name: "cancelSearchWiFiPrinter", returnType: CAPPluginReturnPromise),
        CAPPluginMethod(name: "cancelSearchBluetoothPrinter", returnType: CAPPluginReturnPromise),
    ]
    private static let sdkWorker = DispatchQueue(label: "jp.rdlabo.brotherprint.sdk")
    private let cancellationLock = NSLock()
    private var storedWiFiCancel: (() -> Void)?
    private var storedBluetoothCancel: (() -> Void)?
    private var cancelRoutineWiFi: (() -> Void)? {
        get {
            cancellationLock.lock()
            defer { cancellationLock.unlock() }
            return storedWiFiCancel
        }
        set {
            cancellationLock.lock()
            defer { cancellationLock.unlock() }
            storedWiFiCancel = newValue
        }
    }
    private var cancelRoutineBluetooth: (() -> Void)? {
        get {
            cancellationLock.lock()
            defer { cancellationLock.unlock() }
            return storedBluetoothCancel
        }
        set {
            cancellationLock.lock()
            defer { cancellationLock.unlock() }
            storedBluetoothCancel = newValue
        }
    }

    private func rejectPrintValidation(_ call: CAPPluginCall, message: String, category: String = "INVALID_ARGUMENT") {
        reportPrintFailure(
            message, category: category,
            emit: { info in
                self.notifyListeners(BrotherPrinterEvent.onPrintError.rawValue, data: info)
            }, reject: { call.reject(message, category) })
    }

    @objc func printImage(_ call: CAPPluginCall) {
        if let message = validatePrinterOptions(call.options) {
            rejectPrintValidation(call, message: message)
            return
        }
        let encodedImage: String = call.getString("encodedImage", "")
        if encodedImage == "" {
            self.notifyListeners(
                BrotherPrinterEvent.onPrintError.rawValue, data: ["code": 0, "message": "Error - Image data is not found."])
            call.reject("Error - Image data is not found.", "INVALID_ARGUMENT")
            return
        }

        guard let image = decodePrintImage(encodedImage) else {
            self.notifyListeners(
                BrotherPrinterEvent.onPrintError.rawValue,
                data: ["code": 0, "message": "Error - Create decodedByte From ImageData is failed."])
            call.reject("Error - Create decodedByte From ImageData is failed.", "INVALID_ARGUMENT")
            return
        }

        // 検索からデバイス情報が得られた場合
        let port: String = call.getString("port", "wifi")
        let channelInfo: String = call.getString("channelInfo", "")

        //        let localName: String = call.getString("localName", "")
        //        let serialNumber: String = call.getString("serialNumber", "")

        let modelName: String = call.getString("modelName", "QL_820NWB")
        let printerModel = BrotherModel.getModelName(from: modelName)

        NSLog(call.getString("modelName", "not set"))
        NSLog(call.getString("labelName", "not set"))

        Self.sdkWorker.async {
            var channel: BRLMChannel

            switch port {
            case "wifi":
                channel = BRLMChannel(wifiIPAddress: channelInfo)
            case "bluetooth":
                channel = BRLMChannel(bluetoothSerialNumber: channelInfo)
            case "bluetoothLowEnergy":
                channel = BRLMChannel(bleLocalName: channelInfo)
            default:
                self.notifyListeners(
                    BrotherPrinterEvent.onPrintError.rawValue, data: ["code": 0, "message": "Error - connection is not found."])
                call.reject("Unsupported connection port", "UNSUPPORTED")
                return
            }

            let generateResult = BRLMPrinterDriverGenerator.open(channel)
            guard generateResult.error.code == BRLMOpenChannelErrorCode.noError,
                let printerDriver = generateResult.driver
            else {
                let message = OpenChannelErrorModel.fetchChannelErrorCode(error: generateResult.error.code)
                self.notifyListeners(
                    BrotherPrinterEvent.onPrintFailedCommunication.rawValue,
                    data: [
                        "message": message,
                        "code": generateResult.error.code.rawValue, "nativeCode": generateResult.error.code.rawValue,
                        "category": "COMMUNICATION",
                    ])
                call.reject("Error - Open Channel: \(message)", "COMMUNICATION", nil, ["nativeCode": generateResult.error.code.rawValue])
                return
            }

            var channelClosed = false
            defer { if !channelClosed { printerDriver.closeChannel() } }
            var printSettings: BRLMPrintSettingsProtocol

            if modelName.hasPrefix("QL") {
                guard
                    let _printSettings = BRLMQLPrintSettings(defaultPrintSettingsWith: printerModel)
                else {
                    self.notifyListeners(
                        BrotherPrinterEvent.onPrintError.rawValue,
                        data: [
                            "code": 0,
                            "message": "Error - Create BRLMQLPrintSettings with " + modelName + " is failed.",
                        ])
                    call.reject("Error - Create BRLMQLPrintSettings with " + modelName + " is failed.")
                    return
                }
                printSettings = PrinterSettingsModel.QLModelSettings(call, printSettings: _printSettings)

            } else if modelName.hasPrefix("TD") {
                guard
                    let _printSettings = BRLMTDPrintSettings(defaultPrintSettingsWith: printerModel)
                else {
                    self.notifyListeners(
                        BrotherPrinterEvent.onPrintError.rawValue,
                        data: [
                            "code": 0,
                            "message": "Error - Create BRLMTDPrintSettings with " + modelName + " is failed.",
                        ])
                    call.reject("Error - Create BRLMTDPrintSettings with " + modelName + " is failed.")
                    return
                }
                printSettings = PrinterSettingsModel.TDModelSettings(call, printSettings: _printSettings)

            } else {
                self.notifyListeners(
                    BrotherPrinterEvent.onPrintError.rawValue, data: ["code": 0, "message": "Error - " + modelName + " is not supported"])
                call.reject("Error - " + modelName + " is not supported")
                return
            }

            let printError = printerDriver.printImage(with: image, settings: printSettings)
            printerDriver.closeChannel()
            channelClosed = true

            if printError.code != BRLMPrintErrorCode.noError {
                let message = PrintErrorModel.fetchErrorCode(errorCode: Int32(printError.code.rawValue))
                self.notifyListeners(
                    BrotherPrinterEvent.onPrintError.rawValue,
                    data: [
                        "message": message,
                        "code": printError.code.rawValue, "nativeCode": printError.code.rawValue, "category": "PRINT_FAILED",
                    ])
                call.reject("Error - Print Image: " + message, "PRINT_FAILED", nil, ["nativeCode": printError.code.rawValue])
                return
            }

            self.notifyListeners(BrotherPrinterEvent.onPrint.rawValue, data: [:])
            call.resolve()
        }
    }

    @objc func isChannelAvailable(_ call: CAPPluginCall) {
        guard !call.getString("channelInfo", "").trimmingCharacters(in: .whitespacesAndNewlines).isEmpty else {
            call.reject("channelInfo is required", "INVALID_ARGUMENT")
            return
        }
        let port: String = call.getString("port", "wifi")
        let channelInfo: String = call.getString("channelInfo", "")

        Self.sdkWorker.async {
            var channel: BRLMChannel
            switch port {
            case "wifi":
                channel = BRLMChannel(wifiIPAddress: channelInfo)
            case "bluetooth":
                channel = BRLMChannel(bluetoothSerialNumber: channelInfo)
            case "bluetoothLowEnergy":
                channel = BRLMChannel(bleLocalName: channelInfo)
            default:
                call.reject("Unsupported connection port", "UNSUPPORTED")
                return
            }
            let generateResult = BRLMPrinterDriverGenerator.open(channel)

            guard generateResult.error.code == BRLMOpenChannelErrorCode.noError,
                let printerDriver = generateResult.driver
            else {
                call.resolve(["result": false])
                return
            }
            printerDriver.closeChannel()
            call.resolve(["result": true])
        }
    }

    @objc func search(_ call: CAPPluginCall) {
        let duration = call.getDouble("searchDuration", 15)
        guard duration.isFinite, duration > 0, duration.rounded(.towardZero) == duration, duration < Double(Int.max) else {
            call.reject("searchDuration must be a positive integer", "INVALID_ARGUMENT")
            return
        }
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
        Self.sdkWorker.async {
            self.cancelRoutineWiFi = {
                BRLMPrinterSearcher.cancelNetworkSearch()
            }

            defer { self.cancelRoutineWiFi = nil }
            let option = BRLMNetworkSearchOption()
            option.printerList = printerSearchModels
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15))

            NSLog("BRLMPrinterSearcher.startNetworkSearch")
            let searcher = BRLMPrinterSearcher.startNetworkSearch(option) { channel in
                NSLog(channel.channelInfo)
                let printer = self.chanelToPrinter(port: "wifi", channel: channel)
                self.notifyListeners(BrotherPrinterEvent.onPrinterAvailable.rawValue, data: printer)
            }
            if searcher.error.code != BRLMPrinterSearchErrorCode.noError {
                self.rejectSearch(call, operation: "startNetworkSearch", code: searcher.error.code)
                return
            }
            print(searcher.channels.count)
            self.cancelRoutineWiFi = nil
            call.resolve()
        }
    }

    private func checkBLEChannel(_ call: CAPPluginCall) {
        Self.sdkWorker.async {
            let searcher = BRLMPrinterSearcher.startBluetoothSearch()
            if searcher.error.code != BRLMPrinterSearchErrorCode.noError {
                self.rejectSearch(call, operation: "startBluetoothSearch", code: searcher.error.code)
                return
            }
            if searcher.channels.isEmpty {
                let completion = DispatchSemaphore(value: 0)
                DispatchQueue.main.async {
                    BRLMPrinterSearcher.startBluetoothAccessorySearch { result in
                        defer { completion.signal() }
                        guard result.error.code == BRLMPrinterSearchErrorCode.noError else {
                            self.rejectSearch(call, operation: "startBluetoothAccessorySearch", code: result.error.code)
                            return
                        }
                        for channel in result.channels {
                            self.notifyListeners(
                                BrotherPrinterEvent.onPrinterAvailable.rawValue,
                                data: self.chanelToPrinter(port: "bluetooth", channel: channel))
                        }
                        call.resolve()
                    }
                }
                completion.wait()
                return
            }
            for channel in searcher.channels {
                NSLog(channel.channelInfo)
                self.notifyListeners(
                    BrotherPrinterEvent.onPrinterAvailable.rawValue, data: self.chanelToPrinter(port: "bluetooth", channel: channel))
            }
            call.resolve()
        }

    }

    private func searchBLEPrinter(_ call: CAPPluginCall) {
        Self.sdkWorker.async {
            self.cancelRoutineBluetooth = {
                BRLMPrinterSearcher.cancelBLESearch()
            }
            defer { self.cancelRoutineBluetooth = nil }
            let option = BRLMBLESearchOption()
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15))
            NSLog("BRLMPrinterSearcher.startBLESearch")
            let searcher = BRLMPrinterSearcher.startBLESearch(option) { channel in
                self.notifyListeners(
                    BrotherPrinterEvent.onPrinterAvailable.rawValue,
                    data: self.chanelToPrinter(port: "bluetoothLowEnergy", channel: channel))
            }
            guard searcher.error.code == BRLMPrinterSearchErrorCode.noError else {
                self.rejectSearch(call, operation: "startBLESearch", code: searcher.error.code)
                return
            }
            call.resolve()
        }
    }

    private func rejectSearch(_ call: CAPPluginCall, operation: String, code: BRLMPrinterSearchErrorCode) {
        let category: String
        switch code {
        case .canceled: category = "CANCELLED"
        case .alreadySearching: category = "BUSY"
        case .unsupported: category = "UNSUPPORTED"
        default: category = "COMMUNICATION"
        }
        call.reject(
            "\(operation): \(PrinterSearchErrorModel.fetchChannelErrorCode(error: code))", category, nil, ["nativeCode": code.rawValue])
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
            "nodeName": nodeName,
            "location": location,
            "channelInfo": ipaddress,
        ]
    }

    @objc func cancelSearchWiFiPrinter(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            self.cancelRoutineWiFi?()
            call.resolve()
        }
    }

    @objc func cancelSearchBluetoothPrinter(_ call: CAPPluginCall) {
        DispatchQueue.global().async {
            self.cancelRoutineBluetooth?()
            call.resolve()
        }
    }
}
