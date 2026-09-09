# Installation

## Install

```
npm install @rdlabo/capacitor-brotherprint
```

The published plugin package declares an SPM dependency on a **local** Brother kit at your app root:

`ios/LocalPackages/BRLMPrinterKit`

That path is relative from `node_modules/@rdlabo/capacitor-brotherprint` (`../../../ios/LocalPackages/BRLMPrinterKit`). Place the Brother iOS SDK under your Capacitor app’s `ios` tree as shown below, then run `npx cap sync`. This plugin requires **iOS 15** and **Swift Package Manager** only (no CocoaPods / Podfile steps).

This plugin does not redistribute the Brother SDK. Download it from Brother’s official pages for your platform.

## Initialize the Brother SDK

### Android configuration

1. Place the following files in the android folder of your Capacitor project:

- `android/BrotherPrintLibrary/BrotherPrintLibrary.aar`
- `android/BrotherPrintLibrary/build.gradle`

Download the Android SDK from Brother: https://support.brother.co.jp/j/s/es/dev/ja/mobilesdk/android/index.html?c=jp&lang=ja&navi=offall&comple=on&redirect=on#ver4

2. In `android/BrotherPrintLibrary/build.gradle`, include:

```
configurations.maybeCreate("default")
artifacts.add("default", file('BrotherPrintLibrary.aar'))
```

3. Open `android/settings.gradle` and add:

```
include ':BrotherPrintLibrary'
project(':BrotherPrintLibrary').projectDir = new File('./BrotherPrintLibrary/')
```

### iOS configuration

1. Under your Capacitor app (not inside `node_modules`), place:

- `ios/LocalPackages/BRLMPrinterKit/Sources/BRLMPrinterKit.xcframework`
- `ios/LocalPackages/BRLMPrinterKit/Package.swift`

Download the iOS SDK from Brother: https://support.brother.com/g/s/es/dev/en/mobilesdk/ios/index.html

2. Create `ios/LocalPackages/BRLMPrinterKit/Package.swift` for that local binary package (minimum iOS 15 to match the plugin):

```swift
// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "BRLMPrinterKit",
    platforms: [
        .iOS(.v15)
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
```

3. After the SDK files are in place, run `npx cap sync` so the app’s iOS project picks up the plugin and the local package path.

## Permission configuration

### Android configuration

Update `AndroidManifest.xml` to include the following permissions:

```diff
- <manifest xmlns:android="http://schemas.android.com/apk/res/android">
+ <manifest xmlns:android="http://schemas.android.com/apk/res/android"
+    xmlns:tools="http://schemas.android.com/tools">
...
+     <!-- For Bluetooth -->
+     <uses-permission android:name="android.permission.BLUETOOTH" />
+     <uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
+     <uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />

+     <!-- For Bluetooth Low Energy, Android 11 and earlier-->
+     <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
+     <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />

+     <!-- For Bluetooth Low Energy, Android 12 and later -->
+     <uses-permission android:name="android.permission.BLUETOOTH_SCAN"
+         android:usesPermissionFlags="neverForLocation"
+         tools:targetApi="s" />
```

More information: https://support.brother.co.jp/j/s/support/html/mobilesdk/guide/getting-started/getting-started-android.html

### iOS configuration

Update `Info.plist` to include the following keys. `UISupportedExternalAccessoryProtocols` must be an **array of strings**.

```diff
+ <key>NSBluetoothAlwaysUsageDescription</key>
+ <string>【Why use Bluetooth for your app.】</string>
+ <key>NSBluetoothPeripheralUsageDescription</key>
+ <string>【Why use Bluetooth for your app.】</string>
+ <key>NSBonjourServices</key>
+ <array>
+ 	<string>_pdl-datastream._tcp</string>
+ 	<string>_printer._tcp</string>
+ 	<string>_ipp._tcp</string>
+ </array>
+ <key>NSLocalNetworkUsageDescription</key>
+ <string>【Why use WiFi for your app.】</string>
+ <key>UISupportedExternalAccessoryProtocols</key>
+ <array>
+ 	<string>com.brother.ptcbp</string>
+ </array>
```

#### Bluetooth plist types (verified September 9, 2026)

`UISupportedExternalAccessoryProtocols` must be an **array of strings**, even when `com.brother.ptcbp` is the only protocol. Earlier versions of this repository's example incorrectly used a single `<string>`, which caused Bluetooth discovery to crash with `-[__NSCFString count]: unrecognized selector`. Use the `<array>` shown above. See [Apple's type definition](https://developer.apple.com/documentation/bundleresources/information-property-list/uisupportedexternalaccessoryprotocols).

The `NSBluetoothAlwaysUsageDescription` and `NSBluetoothPeripheralUsageDescription` values are **strings**, not arrays. The examples for these keys in [Brother's official iOS setup guide](https://support.brother.com/g/s/es/htmldoc/mobilesdk/guide/getting-started/getting-started-ios.html) are correct as of September 9, 2026; they were not the cause of this crash. Brother's guide separately instructs adding `com.brother.ptcbp` as an item under `UISupportedExternalAccessoryProtocols`. It requires `NSBluetoothPeripheralUsageDescription` additionally only for deployment targets earlier than iOS 13.

More information: https://support.brother.co.jp/j/s/support/html/mobilesdk/guide/getting-started/getting-started-ios.html
