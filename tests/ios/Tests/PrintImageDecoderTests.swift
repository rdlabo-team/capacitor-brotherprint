import XCTest
import UIKit
@testable import BrotherPrintLogic

class PrintImageDecoderTests: XCTestCase {
    func testInvalidImagesReturnNil() {
        for image in ["", "A", "SGVsbG8=", "iVBORw0KGgo="] {
            XCTAssertNil(decodePrintImage(image), "Input: \(image)")
        }
    }

    func testValidImageDecodes() throws {
        let format = UIGraphicsImageRendererFormat()
        format.scale = 1
        let renderer = UIGraphicsImageRenderer(size: CGSize(width: 2, height: 3), format: format)
        let data = renderer.pngData { context in
            UIColor.black.setFill()
            context.fill(CGRect(x: 0, y: 0, width: 2, height: 3))
        }
        let image = try XCTUnwrap(decodePrintImage(data.base64EncodedString()))
        XCTAssertEqual(image.width, 2)
        XCTAssertEqual(image.height, 3)
    }
}
