import XCTest
import Capacitor
@testable import BrotherPrintPlugin

class PrintImageValidationTests: XCTestCase {
    func testInvalidImagesRejectBeforeOpeningPrinter() {
        for image in ["", "A", "SGVsbG8=", "iVBORw0KGgo="] {
            var rejected = 0
            let call = CAPPluginCall(callbackId: "test", methodName: "printImage", options: ["encodedImage": image], success: { _, _ in
                XCTFail("Invalid image must not resolve")
            }, error: { _ in
                rejected += 1
            })!
            BrotherPrintPlugin().printImage(call)
            XCTAssertEqual(rejected, 1, "Input: \(image)")
        }
    }
}
