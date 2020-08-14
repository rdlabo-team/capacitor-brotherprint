import Foundation
import Capacitor
import BRLMPrinterKit

/**
 * Please read the Capacitor iOS Plugin Development Guide
 * here: https://capacitorjs.com/docs/plugins/ios
 */
@objc(BrotherPrint)
public class BrotherPrint: CAPPlugin {

    @objc func printImage(_ call: CAPPluginCall) {
        let encodedImage: String = call.getString("encodedImage") ?? "";
        if (encodedImage == "") {
            call.error("Error - Image data is not found.");
            return;
        }

        let newImageData = Data(base64Encoded: encodedImage, options: []);
        
        let printerType: String = call.getString("printerType") ?? "";
        if (printerType == "") {
            call.error("Error - printerType is not found.");
            return;
        }
        
        if (printerType != "QL-820NW") {
            // iOS非対応
            call.error("Error - connection is not found.");
            return;
        }


        // 検索からデバイス情報が得られた場合
        let localName: String = call.getString("localName") ?? "";
        let ipAddress: String = call.getString("ipAddress") ?? "";
        
        
        DispatchQueue.main.async {
            var channel: BRLMChannel;
            if (localName != "") {
                channel = BRLMChannel(bleLocalName: localName);
            } else if (ipAddress != "") {
                channel = BRLMChannel(wifiIPAddress: ipAddress);
            } else {
                // iOSは有線接続ができない
                call.error("Error - connection is not found.");
                return;
            }
            
            let generateResult = BRLMPrinterDriverGenerator.open(channel);
            guard generateResult.error.code == BRLMOpenChannelErrorCode.noError,
                let printerDriver = generateResult.driver else {
                    self.notifyListeners("onPrintFailedCommunication", data: [
                        "value": true
                    ]);
                    NSLog("Error - Open Channel: \(generateResult.error.code)")
                    return
            }
            
            guard
                let decodedByte = UIImage(data: newImageData! as Data),
                let printSettings = BRLMQLPrintSettings(defaultPrintSettingsWith: BRLMPrinterModel.QL_820NWB)
                else {
                    NSLog("Error - Image file is not found.")
                    return
            }
            
            let labelNameIndex = call.getInt("labelNameIndex") ?? 16;
            printSettings.labelSize = labelNameIndex == 16 ?
                BRLMQLPrintSettingsLabelSize.rollW62 : BRLMQLPrintSettingsLabelSize.rollW62RB;
            printSettings.autoCut = true

            
            let printError = printerDriver.printImage(with: decodedByte.cgImage!, settings: printSettings);


            if printError.code != .noError {
                self.notifyListeners("onPrintFailedCommunication", data: [
                    "value": true
                ]);
                return;
            }
            else {
                NSLog("Success - Print Image")
                call.success([
                    "value": true
                ]);
            }
        }
    }
}

