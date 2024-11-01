# @rdlabo/capacitor-brotherprint

Capacitor Brother Print is a native Brother Print SDK implementation for iOS & Android. Support These models.

**This plugin is still in the RC (release candidate) phase.**

## Supported models

Each product link is an Amazon affiliate link. If you choose to make a purchase through these links, it would be greatly appreciated and **would help** support development costs. Thank you!

| Product                               | Model        | iOS/WiFi | iOS/BT | iOS/BLE | Android/USB | Android/WiFi | Android/BT | Android/BLE |
| ------------------------------------- | ------------ | -------- | ------ | ------- | ----------- | ------------ | ---------- | ----------- |
| QL-810W                               | QL_810W      | ✗        | ✗      | ✗       | △           | ✗            | ✗          | ✗           |
| [QL-820NWB](https://amzn.to/3BXQ1aj)  | QL_820NWB    | ◯        | ※1     | ✗       | △           | ◯            | △          | ✗           |
| [QL-820NWBc](https://amzn.to/4fjhUIe) | QL_820NWB    | ◯        | ※2     | ✗       | ✗           | ◯            | ◯          | ✗           |
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

※1 Due to low Bluetooth version, connection is not possible with iOS. Ref: https://okbizcs.okwave.jp/brother/qa/q9932082.html
※2 The iOS/BT implementation for the QL-820NWBc is in place, but it’s uncertain if it functions correctly. It’s unclear whether this is an implementation issue, as Brother’s official app also doesn’t work well.

## How to install

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

Update the `ios/App/Podfile` file at your project. https://github.com/BernardRadman/BRLMPrinterKit 's SDK is version 4.7.2. This is old. So should link to direct own repository.

```diff
  target 'App' do
    capacitor_pods
    # Add your Pods here
+   pod 'BRLMPrinterKit_v4', :git => 'https://github.com/rdlabo/BRLMPrinterKit.git', :branch => 'feat/update_4_9_1'
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
    await BrotherPrint.search({
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
      modelName: BRLMPrinterModelName.QL_820NWB,
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

## API

<docgen-index>

- [`printImage(...)`](#printimage)
- [`search(...)`](#search)
- [`cancelSearchWiFiPrinter()`](#cancelsearchwifiprinter)
- [`cancelSearchBluetoothPrinter()`](#cancelsearchbluetoothprinter)
- [`addListener(BrotherPrintEventsEnum.onPrinterAvailable, ...)`](#addlistenerbrotherprinteventsenumonprinteravailable-)
- [`addListener(BrotherPrintEventsEnum.onPrint, ...)`](#addlistenerbrotherprinteventsenumonprint-)
- [`addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, ...)`](#addlistenerbrotherprinteventsenumonprintfailedcommunication-)
- [`addListener(BrotherPrintEventsEnum.onPrintError, ...)`](#addlistenerbrotherprinteventsenumonprinterror-)
- [Interfaces](#interfaces)
- [Type Aliases](#type-aliases)
- [Enums](#enums)

</docgen-index>

<docgen-api>
<!--Update the source file JSDoc comments and rerun docgen to update the docs below-->

### printImage(...)

```typescript
printImage(options: BRLMPrintOptions) => Promise<void>
```

| Param         | Type                                                          |
| ------------- | ------------------------------------------------------------- |
| **`options`** | <code><a href="#brlmprintoptions">BRLMPrintOptions</a></code> |

---

### search(...)

```typescript
search(option: BRLMSearchOption) => Promise<void>
```

Search for printers. If not found, it will return an empty array.(not error)

| Param        | Type                                                          |
| ------------ | ------------------------------------------------------------- |
| **`option`** | <code><a href="#brlmsearchoption">BRLMSearchOption</a></code> |

---

### cancelSearchWiFiPrinter()

```typescript
cancelSearchWiFiPrinter() => Promise<void>
```

Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.

---

### cancelSearchBluetoothPrinter()

```typescript
cancelSearchBluetoothPrinter() => Promise<void>
```

Basically, it times out, so there is no need to use it. Use it when you want to run multiple connectType searches at the same time and time out any of them manually.

---

### addListener(BrotherPrintEventsEnum.onPrinterAvailable, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrinterAvailable, listenerFunc: (printers: BRLMChannelResult) => void) => Promise<PluginListenerHandle>
```

| Param              | Type                                                                                         |
| ------------------ | -------------------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrinterAvailable</a></code> |
| **`listenerFunc`** | <code>(printers: <a href="#brlmchannelresult">BRLMChannelResult</a>) =&gt; void</code>       |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

---

### addListener(BrotherPrintEventsEnum.onPrint, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrint, listenerFunc: () => void) => Promise<PluginListenerHandle>
```

| Param              | Type                                                                              |
| ------------------ | --------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrint</a></code> |
| **`listenerFunc`** | <code>() =&gt; void</code>                                                        |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

---

### addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrintFailedCommunication, listenerFunc: (info: ErrorInfo) => void) => Promise<PluginListenerHandle>
```

| Param              | Type                                                                                                 |
| ------------------ | ---------------------------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrintFailedCommunication</a></code> |
| **`listenerFunc`** | <code>(info: <a href="#errorinfo">ErrorInfo</a>) =&gt; void</code>                                   |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

---

### addListener(BrotherPrintEventsEnum.onPrintError, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrintError, listenerFunc: (info: ErrorInfo) => void) => Promise<PluginListenerHandle>
```

| Param              | Type                                                                                   |
| ------------------ | -------------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrintError</a></code> |
| **`listenerFunc`** | <code>(info: <a href="#errorinfo">ErrorInfo</a>) =&gt; void</code>                     |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

---

### Interfaces

#### PluginListenerHandle

| Prop         | Type                                      |
| ------------ | ----------------------------------------- |
| **`remove`** | <code>() =&gt; Promise&lt;void&gt;</code> |

### Type Aliases

#### BRLMPrintOptions

<code>{ encodedImage: string; /** _ Should use enum <a href="#brlmprinterlabelname">BRLMPrinterLabelName</a> _/ labelName: <a href="#brlmprinterlabelname">BRLMPrinterLabelName</a>; /** _ Should use enum <a href="#brlmprintermodelname">BRLMPrinterModelName</a> _/ modelName: <a href="#brlmprintermodelname">BRLMPrinterModelName</a>; } & <a href="#partial">Partial</a>&lt;<a href="#brlmchannelresult">BRLMChannelResult</a>&gt; & <a href="#brlmprintersettings">BRLMPrinterSettings</a></code>

#### Partial

Make all properties in T optional

<code>{
[P in keyof T]?: T[P];
}</code>

#### BRLMChannelResult

<code>{ port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'; modelName: string; serialNumber: string; macAddress: string; nodeName: string; location: string; ipAddress: string; }</code>

#### BRLMPrinterSettings

These are optional. If these are not set, default values are assigned by the printer.

<code>{ /** _ The number of copies you print. _/ numberOfCopies?: <a href="#brlmprinternumberofcopies">BRLMPrinterNumberOfCopies</a>; /** _ Whether the auto-cut is enabled or not. If true, your printer cut the paper each page. _/ autoCut?: <a href="#brlmprinterautocuttype">BRLMPrinterAutoCutType</a>; /** _ A scale mode that specifies how your data is scaled in a print area of your printer. _/ scaleMode?: <a href="#brlmprinterscalemode">BRLMPrinterScaleMode</a>; /** _ A scale value. This is effective when ScaleMode is ScaleValue. _/ scaleValue?: <a href="#brlmprinterscalevaluetype">BRLMPrinterScaleValueType</a>; /** _ A way to rasterize your data. _/ halftone?: <a href="#brlmprinterhalftone">BRLMPrinterHalftone</a>; /** _ A threshold value. This is effective when the Halftone is Threshold. _/ halftoneThreshold?: <a href="#brlmprinterhalftonethresholdtype">BRLMPrinterHalftoneThresholdType</a>; /** _ An image rotation that specifies the angle in which your data is placed in the print area. Rotation direction is clockwise. _/ imageRotation?: <a href="#brlmprinterimagerotation">BRLMPrinterImageRotation</a>; /** _ A vertical alignment that specifies how your data is placed in the printable area. _/ verticalAlignment?: <a href="#brlmprinterverticalalignment">BRLMPrinterVerticalAlignment</a>; /** _ A horizontal alignment that specifies how your data is placed in the printable area. _/ horizontalAlignment?: <a href="#brlmprinterhorizontalalignment">BRLMPrinterHorizontalAlignment</a>; /** _ A compress mode that specifies how to compress your data. _ note: This is ios only. _/ compressMode?: <a href="#brlmprintercompressmode">BRLMPrinterCompressMode</a>; /\*\* _ A priority that is print speed or print quality. Whether or not this has an effect is depend on your printer. _/ printQuality?: <a href="#brlmprinterprintquality">BRLMPrinterPrintQuality</a>; /\*\* _ A priority that is print speed or print quality. Whether or not this has an effect is depend on your printer. _ note: This is ios only. And for some reason, the aspect ratio is broken, so we do not provide this as an API. _ resolution?: BRLMPrinterPrintResolution; \*/ }</code>

#### BRLMPrinterNumberOfCopies

<code>number</code>

#### BRLMPrinterAutoCutType

<code>boolean</code>

#### BRLMPrinterScaleValueType

<code>number</code>

#### BRLMPrinterHalftoneThresholdType

<code>number</code>

#### BRLMSearchOption

<code>{ port: 'wifi' | 'bluetooth' | 'bluetoothLowEnergy'; /\*\* _ searchDuration is the time to end search for devices. _ default is 15 seconds. _ use only port is 'wifi' or 'bluetoothLowEnergy'. _/ searchDuration: number; }</code>

#### ErrorInfo

<code>{ message: string; code: number; }</code>

### Enums

#### BRLMPrinterLabelName

| Members       | Value                  |
| ------------- | ---------------------- |
| **`W17H54`**  | <code>'W17H54'</code>  |
| **`W17H87`**  | <code>'W17H87'</code>  |
| **`W23H23`**  | <code>'W23H23'</code>  |
| **`W29H42`**  | <code>'W29H42'</code>  |
| **`W29H90`**  | <code>'W29H90'</code>  |
| **`W38H90`**  | <code>'W38H90'</code>  |
| **`W39H48`**  | <code>'W39H48'</code>  |
| **`W52H29`**  | <code>'W52H29'</code>  |
| **`W62H29`**  | <code>'W62H29'</code>  |
| **`W62H100`** | <code>'W62H100'</code> |
| **`W12`**     | <code>'W12'</code>     |
| **`W29`**     | <code>'W29'</code>     |
| **`W38`**     | <code>'W38'</code>     |
| **`W50`**     | <code>'W50'</code>     |
| **`W54`**     | <code>'W54'</code>     |
| **`W62`**     | <code>'W62'</code>     |
| **`W60H86`**  | <code>'W60H86'</code>  |
| **`W62RB`**   | <code>'W62RB'</code>   |
| **`W54H29`**  | <code>'W54H29'</code>  |
| **`W12DIA`**  | <code>'W12DIA'</code>  |
| **`W24DIA`**  | <code>'W24DIA'</code>  |
| **`W58DIA`**  | <code>'W58DIA'</code>  |
| **`W62H60`**  | <code>'W62H60'</code>  |
| **`W62H75`**  | <code>'W62H75'</code>  |

#### BRLMPrinterModelName

| Members            | Value                       |
| ------------------ | --------------------------- |
| **`QL_810W`**      | <code>'QL_810W'</code>      |
| **`QL_820NWB`**    | <code>'QL_820NWB'</code>    |
| **`TD_2320D_203`** | <code>'TD_2320D_203'</code> |
| **`TD_2030AD`**    | <code>'TD_2030AD'</code>    |
| **`TD_2350D_203`** | <code>'TD_2350D_203'</code> |

#### BRLMPrinterScaleMode

| Members              | Value                         |
| -------------------- | ----------------------------- |
| **`ActualSize`**     | <code>'ActualSize'</code>     |
| **`FitPageAspect`**  | <code>'FitPageAspect'</code>  |
| **`FitPaperAspect`** | <code>'FitPaperAspect'</code> |
| **`ScaleValue`**     | <code>'ScaleValue'</code>     |

#### BRLMPrinterHalftone

| Members              | Value                         |
| -------------------- | ----------------------------- |
| **`Threshold`**      | <code>'Threshold'</code>      |
| **`ErrorDiffusion`** | <code>'ErrorDiffusion'</code> |
| **`PatternDither`**  | <code>'PatternDither'</code>  |

#### BRLMPrinterImageRotation

| Members         | Value                    |
| --------------- | ------------------------ |
| **`Rotate0`**   | <code>'Rotate0'</code>   |
| **`Rotate90`**  | <code>'Rotate90'</code>  |
| **`Rotate180`** | <code>'Rotate180'</code> |
| **`Rotate270`** | <code>'Rotate270'</code> |

#### BRLMPrinterVerticalAlignment

| Members      | Value                 |
| ------------ | --------------------- |
| **`Top`**    | <code>'Top'</code>    |
| **`Center`** | <code>'Center'</code> |
| **`Bottom`** | <code>'Bottom'</code> |

#### BRLMPrinterHorizontalAlignment

| Members      | Value                 |
| ------------ | --------------------- |
| **`Left`**   | <code>'Left'</code>   |
| **`Center`** | <code>'Center'</code> |
| **`Right`**  | <code>'Right'</code>  |

#### BRLMPrinterCompressMode

| Members     | Value                |
| ----------- | -------------------- |
| **`None`**  | <code>'None'</code>  |
| **`Tiff`**  | <code>'Tiff'</code>  |
| **`Mode9`** | <code>'Mode9'</code> |

#### BRLMPrinterPrintQuality

| Members    | Value               |
| ---------- | ------------------- |
| **`Best`** | <code>'Best'</code> |
| **`Fast`** | <code>'Fast'</code> |

#### BrotherPrintEventsEnum

| Members                          | Value                                     |
| -------------------------------- | ----------------------------------------- |
| **`onPrinterAvailable`**         | <code>'onPrinterAvailable'</code>         |
| **`onPrint`**                    | <code>'onPrint'</code>                    |
| **`onPrintFailedCommunication`** | <code>'onPrintFailedCommunication'</code> |
| **`onPrintError`**               | <code>'onPrintError'</code>               |

</docgen-api>
