// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "BRLMPrinterKit",
    platforms: [
        .iOS(.v13)
    ],
    products: [
        .library(name: "BRLMPrinterKit", targets: ["BRLMPrinterKit"])
    ],
    targets: [
        .binaryTarget(
            name: "BRLMPrinterKit",
            path: "Sources/BRLMPrinterKit.xcframework"
        )
    ]
)
