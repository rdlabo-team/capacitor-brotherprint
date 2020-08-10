import Foundation
import Capacitor
import BRLMPrinterKit

/**
 *  こちらもインポートできないです。 `No such module 'BRLMPrinterKit'` とエラーがでます。
 *  Framework Search Paths、Header Search Paths は追加済み
 */
// import BRLMPrinterKit

/**
 * Please read the Capacitor iOS Plugin Development Guide
 * here: https://capacitorjs.com/docs/plugins/ios
 */
@objc(BrotherPrint)
public class BrotherPrint: CAPPlugin {

    @objc func echo(_ call: CAPPluginCall) {
        let channel = BRLMChannel(wifiIPAddress: "IPAddress.of.your.printer")

        let generateResult = BRLMPrinterDriverGenerator.open(channel)
        guard generateResult.error.code == BRLMOpenChannelErrorCode.noError,
            let printerDriver = generateResult.driver else {
                print("Error - Open Channel: %d", generateResult.error.code)
                return
        }

        print("Success - Open Channel")
        
//        guard let ptp = BRPtouchPrinter(printerName: deviceName, interface: CONNECTION_TYPE.BLUETOOTH) else {
//            print("*** Prepare Print Error ***")
//            return
//        }
//
        let value = call.getString("value") ?? ""
        call.success([
            "value": value
        ])
    }
}
