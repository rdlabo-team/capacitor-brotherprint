# Installation

## Install

```
% npm install @rdlabo/capacitor-brotherprint
```

## Initialize the Brother SDK

### Android configuration

1. Place the following files in the android folder of your Capacitor project:

- `android/BrotherPrintLibrary/BrotherPrintLibrary.aar`
- `android/BrotherPrintLibrary/build.gradle`

The `BrotherPrintLibrary.aar` file is the Brother Print SDK library, which you can download from the Brother website: https://support.brother.co.jp/j/s/es/dev/ja/mobilesdk/android/index.html?c=jp&lang=ja&navi=offall&comple=on&redirect=on#ver4

2. In the `android/BrotherPrintLibrary/build.gradle file`, include the following content:

```
configurations.maybeCreate(“default”)
artifacts.add(“default”, file('BrotherPrintLibrary.aar'))
```

3. Open `android/settings.gradle` and add the following lines:

```
include ':BrotherPrintLibrary'
project(':BrotherPrintLibrary').projectDir = new File('./BrotherPrintLibrary/')
```

These steps will integrate the Brother Print SDK with your Capacitor Android project.

### iOS configuration

1. Place the following files in the ios folder of your Capacitor project:

- `ios/LocalPackages/BRLMPrinterKit/Sources/BRLMPrinterKit.xcframework`
- `ios/LocalPackages/BRLMPrinterKit/BRLMPrinterKit.podspec`
- `ios/LocalPackages/BRLMPrinterKit/Package.swift`

The `BRLMPrinterKit.xcframework` file is the Brother Print SDK library, which you can download from the Brother website: https://support.brother.co.jp/j/s/es/dev/ja/mobilesdk/android/index.html?c=jp&lang=ja&navi=offall&comple=on&redirect=on#ver4

`BRLMPrinterKit.podspec` content is here:

```podspec
Pod::Spec.new do |s|
  s.name             = 'BRLMPrinterKit'
  s.version          = '4.12.0'
  s.homepage         = 'https://support.brother.co.jp/j/s/support/html/mobilesdk/index.html'
  s.source           = { :path => './Sources' }
  s.summary          = "Pod for the BRLMPrinterKit / Brother's printers"
  s.description      = "This project is only a Pod for the Brother SDK v#{s.version}"
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'Masahiko Sakakibara' => 'sakakibara@rdlabo.jp' }
  s.ios.deployment_target = '11.0'
  s.ios.vendored_frameworks = 'Sources/BRLMPrinterKit.xcframework'
  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
  s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
end
```

`Package.swift` content is here:

```swift
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
```

2. Update the `ios/App/Podfile` file at your project.

```diff
  target 'App' do
    capacitor_pods
    # Add your Pods here
+   pod 'BRLMPrinterKit', :path => '../LocalPackages/BRLMPrinterKit'
  end
```

After set, run `pod update` in the `ios` directory.

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

More information is here: https://support.brother.co.jp/j/s/support/html/mobilesdk/guide/getting-started/getting-started-android.html

### iOS configuration

Update `Info.plist` to include the following permissions:

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
+ <string>com.brother.ptcbp</string>
```

More information is here: https://support.brother.co.jp/j/s/support/html/mobilesdk/guide/getting-started/getting-started-ios.html
