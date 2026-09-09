import XCTest

@testable import BrotherPrintLogic

final class PrinterValidationTests: XCTestCase {
    private let td: [String: Any] = [
        "modelName": "TD_2350D_300", "channelInfo": "192.0.2.1", "paperType": "dieCutPaper", "paperUnit": "mm", "tapeWidth": 60,
        "tapeLength": 60,
    ]
    func testValidSettings() {
        XCTAssertNil(validatePrinterOptions(td))
        XCTAssertNil(validatePrinterOptions(["modelName": "QL_820NWB", "channelInfo": "192.0.2.1"]))
    }
    func testInvalidInputsReturnErrors() {
        let cases: [[String: Any]] = [
            ["paperType": ""], ["paperUnit": "px"], ["tapeWidth": 0], ["tapeLength": -1], ["marginLeft": -1], ["tapeWidth": Double.nan],
            ["numberOfCopies": 1.5], ["numberOfCopies": "2"], ["halftoneThreshold": 256], ["numberOfCopies": true],
        ]
        for patch in cases { XCTAssertNotNil(validatePrinterOptions(td.merging(patch) { _, value in value })) }
        XCTAssertNotNil(validatePrinterOptions(["modelName": "TD_2350D_300", "channelInfo": "192.0.2.1"]))
        XCTAssertNotNil(validatePrinterOptions(["modelName": "QL_820NWB", "channelInfo": "192.0.2.1", "labelName": "invalid"]))
    }

    func testValidationFailureEmitsOnceThenRejectsOnce() {
        var sequence: [String] = []
        reportPrintFailure("Invalid model", category: "INVALID_ARGUMENT", emit: { info in
            XCTAssertEqual(info["code"] as? Int, 0)
            XCTAssertEqual(info["category"] as? String, "INVALID_ARGUMENT")
            sequence.append("event")
        }, reject: { sequence.append("reject") })
        XCTAssertEqual(sequence, ["event", "reject"])
    }
}
