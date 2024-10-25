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
    private var networkManager: BRPtouchNetworkManager?

    @objc func printImage(_ call: CAPPluginCall) {
        let encodedImage: String = call.getString("encodedImage") ?? "";
        if (encodedImage == "") {
            call.reject("Error - Image data is not found.");
            return;
        }

        let newImageData = Data(base64Encoded: encodedImage, options: []);

        let printerType: String = call.getString("printerType") ?? "";
        if (printerType == "") {
            call.reject("Error - printerType is not found.");
            return;
        }

        if (printerType != "QL-820NWB") {
            // iOS非対応
            call.reject("Error - connection is not found.");
            return;
        }


        // 検索からデバイス情報が得られた場合
        let localName: String = call.getString("localName") ?? "";
        let ipAddress: String = call.getString("ipAddress") ?? "";

        // メインスレッドにて処理
        DispatchQueue.main.async {
            var channel: BRLMChannel;
            if (localName != "") {
                channel = BRLMChannel(bleLocalName: localName);
            } else if (ipAddress != "") {
                channel = BRLMChannel(wifiIPAddress: ipAddress);
            } else {
                // iOSは有線接続ができない
                self.notifyListeners("onPrintFailedCommunication", data: [
                    "value": true
                ]);
                return;
            }

            let generateResult = BRLMPrinterDriverGenerator.open(channel);
            guard generateResult.error.code == BRLMOpenChannelErrorCode.noError,
                let printerDriver = generateResult.driver else {
                    self.notifyListeners("onPrintError", data: [
                        "value": generateResult.error.code
                    ]);
                    NSLog("Error - Open Channel: \(generateResult.error.code)")
                    return
            }

            guard
                let decodedByte = UIImage(data: newImageData! as Data),
                let printSettings = BRLMQLPrintSettings(defaultPrintSettingsWith: BRLMPrinterModel.QL_820NWB)
                else {
                    printerDriver.closeChannel();
                    self.notifyListeners("onPrintError", data: [
                        "value": "Error - Image file is not found."
                    ]);
                    return
            }

            let labelNameIndex = call.getInt("labelNameIndex") ?? 16;
            printSettings.labelSize = labelNameIndex == 16 ?
                BRLMQLPrintSettingsLabelSize.rollW62 : BRLMQLPrintSettingsLabelSize.rollW62RB;
            printSettings.autoCut = true
            printSettings.numCopies = UInt(call.getInt("numberOfCopies") ?? 1);

            let printError = printerDriver.printImage(with: decodedByte.cgImage!, settings: printSettings);


            if printError.code != .noError {
                printerDriver.closeChannel();
                self.notifyListeners("onPrintError", data: [
                    "value": printError.code
                ]);
                return;
            }
            else {
                NSLog("Success - Print Image")
                printerDriver.closeChannel();
                call.resolve();
            }
        }
    }

    @objc func search(_ call: CAPPluginCall) {
        switch call.getString("port", "wifi") {
        case "wifi":
            self.searchWiFiPrinter(call);
        case "bluetooth":
            self.checkBLEChannel(call);
        case "bluetoothLowEnergy":
            self.searchBLEPrinter(call);
        default:
            call.reject("port is not 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'")
        }
    }

    // BRPtouchNetworkDelegate
    private func searchWiFiPrinter(_ call: CAPPluginCall) {
        var printersJSObject: JSArray = []
        DispatchQueue.main.async {
            let option = BRLMNetworkSearchOption()
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15));
            let result = BRLMPrinterSearcher.startNetworkSearch(option) { channel in
                let modelName = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyModelName) as? String ?? ""
                let ipaddress = channel.channelInfo
                print("Model : \(modelName), IP Address: \(ipaddress)")
                printersJSObject.append([
                    "modelName": modelName,
                    "ipAddress": ipaddress,
                ]);
            }
            call.resolve([
                "printers": printersJSObject,
            ])
        }
    }

    private func checkBLEChannel(_ call: CAPPluginCall) {
        var printersJSObject: JSArray = []
        DispatchQueue.main.async {
            let channels = BRLMPrinterSearcher.startBluetoothSearch().channels;
            for channel in channels {
                let modelName = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyModelName) as? String ?? ""
                let serialNumber = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeySerialNumber) as? String ?? ""
                printersJSObject.append([
                    "modelName": modelName,
                    "serialNumber": serialNumber,
                ]);
            }
            call.resolve([
                "printers": printersJSObject,
            ])
        }
    }

    private func searchBLEPrinter(_ call: CAPPluginCall) {
        var printersJSObject: JSArray = []
        DispatchQueue.main.async {
            let option = BRLMBLESearchOption()
            option.searchDuration = TimeInterval(call.getInt("searchDuration", 15));
            let result = BRLMPrinterSearcher.startBLESearch(option) { channel in
                let modelName = channel.extraInfo?.value(forKey: BRLMChannelExtraInfoKeyModelName) as? String ?? ""
                let advertiseLocalName = channel.channelInfo
                printersJSObject.append([
                    "modelName": modelName,
                    "localName": advertiseLocalName,
                ]);
            }
            call.resolve([
                "printers": printersJSObject,
            ])
        }
    }

    @objc func cancelSearchWiFiPrinter(_ call: CAPPluginCall) {
        DispatchQueue.main.async {
            BRLMPrinterSearcher.cancelNetworkSearch()
        }
    }

    @objc func cancelSearchBluetoothPrinter(_ call: CAPPluginCall) {
        DispatchQueue.main.async {
            BRLMPrinterSearcher.cancelBLESearch()
        }
    }
}

