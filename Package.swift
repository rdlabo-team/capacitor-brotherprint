// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "RdlaboCapacitorBrotherprint",
    platforms: [.iOS(.v14)],
    products: [
        .library(
            name: "RdlaboCapacitorBrotherprint",
            targets: ["BrotherPrintPlugin"])
    ],
    dependencies: [
        .package(url: "https://github.com/ionic-team/capacitor-swift-pm.git", from: "7.0.0"),
        .package(path: "./demo/ios/LocalPackages/BRLMPrinterKit")
    ],
    targets: [
        .target(
            name: "BrotherPrintPlugin",
            dependencies: [
                .product(name: "Capacitor", package: "capacitor-swift-pm"),
                .product(name: "Cordova", package: "capacitor-swift-pm"),
                .product(name: "BRLMPrinterKit", package: "BRLMPrinterKit")
            ],
            path: "ios/Sources/BrotherPrintPlugin"),
        .testTarget(
            name: "BrotherPrintPluginTests",
            dependencies: ["BrotherPrintPlugin"],
            path: "ios/Tests/BrotherPrintPluginTests")
    ]
)
