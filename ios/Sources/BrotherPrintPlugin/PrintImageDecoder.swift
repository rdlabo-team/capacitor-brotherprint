import UIKit

func decodePrintImage(_ encodedImage: String) -> CGImage? {
    guard let data = Data(base64Encoded: encodedImage, options: []),
          let image = UIImage(data: data) else {
        return nil
    }
    return image.cgImage
}
