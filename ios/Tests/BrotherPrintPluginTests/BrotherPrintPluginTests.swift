import XCTest
import Capacitor
@testable import BrotherPrintPlugin

class PrintImageValidationTests: XCTestCase {
    func testInvalidImagesRejectBeforeOpeningPrinter() {
        for image in ["", "A", "SGVsbG8=", "iVBORw0KGgo="] {
            var rejected = 0
            var errorEvents = 0
            var errorMessage: String?
            let plugin = BrotherPrintPlugin()
            // Normally initialized by Capacitor when attaching the bridge.
            plugin.eventListeners = NSMutableDictionary()
            for eventName in ["onPrintError", "onPrint", "onPrintFailedCommunication"] {
                let listener = CAPPluginCall(callbackId: eventName, methodName: "addListener", options: ["eventName": eventName], success: { result, _ in
                    XCTAssertEqual(eventName, "onPrintError")
                    XCTAssertEqual(result?.data?["code"] as? Int, 0)
                    errorMessage = result?.data?["message"] as? String
                    errorEvents += 1
                }, error: { _ in
                    XCTFail("Listener must not reject")
                })!
                plugin.addListener(listener)
            }
            let call = CAPPluginCall(callbackId: "test", methodName: "printImage", options: ["encodedImage": image], success: { _, _ in
                XCTFail("Invalid image must not resolve")
            }, error: { error in
                XCTAssertEqual(errorMessage, error?.message)
                rejected += 1
            })!
            plugin.printImage(call)
            XCTAssertEqual(rejected, 1, "Input: \(image)")
            XCTAssertEqual(errorEvents, 1, "Input: \(image)")
        }
    }
}
