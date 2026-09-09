// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "BrotherPrintLogic",
    platforms: [.iOS(.v15)],
    products: [.library(name: "BrotherPrintLogic", targets: ["BrotherPrintLogic"])],
    targets: [
        .target(name: "BrotherPrintLogic", path: "Sources"),
        .testTarget(name: "BrotherPrintLogicTests", dependencies: ["BrotherPrintLogic"], path: "Tests")
    ]
)
