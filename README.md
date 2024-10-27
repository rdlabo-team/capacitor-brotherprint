# @rdlabo/capacitor-brotherprint

Capacitor Brother Print is a native Brother Print SDK implementation for iOS & Android. Support These models.

## Supported models

Each product link is an Amazon affiliate link. If you choose to make a purchase through these links, it would be greatly appreciated and **would help** support development costs. Thank you!

| Product                               | Model        | iOS/WiFi | iOS/BT | iOS/BLE | Android/USB | Android/WiFi | Android/BT | Android/BLE |
| ------------------------------------- | ------------ | -------- | ------ | ------- | ----------- | ------------ | ---------- | ----------- |
| QL-810W                               | QL_810W      | ✗        | ✗      | ✗       | △           | ✗            | ✗          | ✗           |
| [QL-820NWB](https://amzn.to/3BXQ1aj)  | QL_820NWB    | ◯        | ✗      | ✗       | △           | ◯            | △          | ✗           |
| [QL-820NWBc](https://amzn.to/4fjhUIe) | QL_820NWB    | ◯        | ※1     | ✗       | ✗           | ◯            | ◯          | ✗           |
| [TD-2320D](https://amzn.to/48EFCN3)   | TD_2320D_203 | ✗        | ✗      | ✗       | △           | ✗            | ✗          | ✗           |
| [TD-2350D](https://amzn.to/48ma6TK)   | TD_2350D_203 | △        | △      | △       | △           | △            | △          | △           |

Amazon Affiliate Links:　**https://amzn.to/3AiiOFT**

**Supplement**

|     | description                |
| --- | -------------------------- |
| ◯   | Supported and tested       |
| △   | Implemented but not tested |
| ✗   | Not supported              |
| BT  | Bluetooth                  |
| BLE | Bluetooth Low Energy       |

※1 The iOS/BT implementation for the QL-820NWBc is in place, but it’s uncertain if it functions correctly. It’s unclear whether this is an implementation issue, as Brother’s official app also doesn’t work well.

## How to install

```
% npm install @rdlabo/capacitor-brotherprint@next
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
include ':BrotherPrintLibrary.aar''
include ':BrotherPrintLibrary'.
project(':BrotherPrintLibrary').projectDir = new File('./BrotherPrintLibrary/')
```

These steps will integrate the Brother Print SDK with your Capacitor Android project.

### iOS configuration

Update the `ios/App/Podfile` file at your project.https://github.com/BernardRadman/BRLMPrinterKit 's SDK is version 4.7.2. This is old. So should link to direct own repository.

```diff
  target 'App' do
    capacitor_pods
    # Add your Pods here
+   pod 'BRLMPrinterKit_v4', :git => 'https://github.com/rdlabo/BRLMPrinterKit.git', :branch => 'feat/update_4_9_1'
  end
```

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
    <key>NSBluetoothAlwaysUsageDescription</key>
	<string>【Why use Bluetooth for your app.】</string>
	<key>NSBluetoothPeripheralUsageDescription</key>
	<string>【Why use Bluetooth for your app.】</string>
	<key>NSBonjourServices</key>
	<array>
		<string>_pdl-datastream._tcp</string>
		<string>_printer._tcp</string>
		<string>_ipp._tcp</string>
	</array>
	<key>NSLocalNetworkUsageDescription</key>
	<string>【Why use WiFi for your app.】</string>
	<key>UISupportedExternalAccessoryProtocols</key>
	<string>com.brother.ptcbp</string>
```

More information is here: https://support.brother.co.jp/j/s/support/html/mobilesdk/guide/getting-started/getting-started-ios.html

## How to use

```typescript
import {
  BrotherPrint,
  BRLMPrintOptions,
  BRLMPrinterLabelName,
  BrotherPrintEventsEnum,
  BRLMPrinterModelName,
  BRLMChannelResult,
} from '@rdlabo/capacitor-brotherprint';

@Component({
  selector: 'brother-print',
  templateUrl: 'brother.component.html',
  styleUrls: ['brother.component.scss'],
})
export class BrotherComponent implements OnInit, OnDestroy {
  readonly #listenerHandlers: PluginListenerHandle[] = [];
  readonly printers = signal<BRLMChannelResult[]>([]);

  async ngOnInit() {
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrint, () => {
        console.log('onPrint');
      }),
    );
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrintError,
        info => {
          console.log('onPrintError');
        },
      ),
    );
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrintFailedCommunication,
        info => {
          console.log('onPrintFailedCommunication');
        },
      ),
    );
    this.#listenerHandlers.push(
      await BrotherPrint.addListener(
        BrotherPrintEventsEnum.onPrinterAvailable,
        printer => {
          this.printers.update(prev => [...prev, printer]);
        },
      ),
    );
  }

  async ngOnDestroy() {
    this.#listenerHandlers.forEach(handler => handler.remove());
  }

  async searchPrinter(port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy') {
    // This method return void. Get the printer list by listening to the event.
    await BrotherPrint.searchPrinter({
      port,
      searchDuration: 15, // seconds
    });
  }

  print() {
    if (this.printers().length === 0) {
      console.error('No printer found');
      return;
    }

    const defaultPrintSettings: BRLMPrintOptions = {
      modelName: BRLMPrinterModelName.QL_820NW,
      labelName: BRLMPrinterLabelName.W62,
      encodedImage: 'base64 removed mime-type', // base64
      numberOfCopies: 1, // default 1
      autoCut: true, // default true
    };

    BrotherPrint.printImage({
      ...defaultPrintSettings,
      ...{
        ipAddress: this.printers()[0].ipAddress,
        localName: this.printers()[0].nodeName,
        macAddress: this.printers()[0].macAddress,
        serialNumber: this.printers()[0].serialNumber,
        port: this.printers()[0].port,
      },
    });
  }
}
```
