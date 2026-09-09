# @rdlabo/capacitor-brotherprint

Capacitor Brother Print binds the native Brother Print SDK for iOS and Android so you can search supported Brother label printers and print images from a Capacitor app.

**This plugin is still in the RC (release candidate) phase.** iOS requires **Swift Package Manager** and a minimum of **iOS 15**. The Brother Print SDK is not compatible with CocoaPods for this plugin.

<!-- rdlabo-docs-omit -->

**Documentation:** [Read the full documentation](https://docs.rdlabo.dev/projects/capacitor-brotherprint)

<!-- /rdlabo-docs-omit -->

## Install

```
npm install @rdlabo/capacitor-brotherprint
```

For SDK placement, SPM layout, and permissions, see [Installation](https://docs.rdlabo.dev/projects/capacitor-brotherprint/docs/installation).

## Print your first label

1. [Installation](https://docs.rdlabo.dev/projects/capacitor-brotherprint/docs/installation) — npm install, place the Brother SDK, permissions, then `npx cap sync`.
2. [Search](https://docs.rdlabo.dev/projects/capacitor-brotherprint/docs/search) — register `onPrinterAvailable`, keep the discovered channel, run Wi-Fi (or other) search.
3. [Print](https://docs.rdlabo.dev/projects/capacitor-brotherprint/docs/print) — print with that channel, a supported model/label, and a real base64 image you prepare.
4. [Events](https://docs.rdlabo.dev/projects/capacitor-brotherprint/docs/events) — print success and error listeners in more detail.

## Connection controller

`BrotherPrinter` is an optional framework-independent controller for discovery,
remembered channels, device selection and print completion. It has no Angular or
Ionic dependency; existing `BrotherPrint` calls remain available.
See [connection management](docs/connection-management.md) for a complete example,
UI/state adapters and lifecycle requirements.

## Supported models

Each product link is an Amazon affiliate link. Purchases through these links help support development costs.

| Product                               | Model        | iOS/Network | iOS/BT | iOS/BLE | Android/USB | Android/Network | Android/BT | Android/BLE |
| ------------------------------------- | ------------ | ----------- | ------ | ------- | ----------- | --------------- | ---------- | ----------- |
| QL-800                                | QL_800       | ❌          | ❌     | ❌      | △           | ❌              | ❌         | ❌          |
| QL-810W                               | QL_810W      | △           | ❌     | ❌      | ✅          | △               | ❌         | ❌          |
| [QL-820NWB](https://amzn.to/3BXQ1aj)  | QL_820NWB    | ✅          | ※1     | ❌      | △           | ✅              | △          | ❌          |
| [QL-820NWBc](https://amzn.to/4fjhUIe) | QL_820NWB    | ✅          | ✅     | ❌      | △           | ✅              | ✅         | ❌          |
| [TD-2320D](https://amzn.to/48EFCN3)   | TD_2320D_203 | △           | ❌     | ❌      | △           | △               | ❌         | ❌          |
| TD-2030A                              | TD_2030AD    | ❌          | ❌     | ❌      | △           | ❌              | ❌         | ❌          |
| [TD-2350D](https://amzn.to/48ma6TK)   | TD_2350D_300 | ✅          | △      | △       | ✅          | ✅              | ✅         | △           |

Network uses `BRLMPrinterPort.wifi`: Wi-Fi or wired Ethernet, depending on the device.
TD-2320D supports wired Ethernet, not Wi-Fi. USB is Android-only in this plugin.
QL-820NWB and QL-820NWBc share the `QL_820NWB` SDK model. The existing
`TD_2030AD` API spelling maps to the SDK's `TD_2030A` model.

The connection controller covers every row above. A supported transport does not
imply hardware verification: new paths remain marked △ until tested on a device.
Sources: [QL-810W interfaces](https://origin.supportbrothercom.brother.co.jp/g/b/spec.aspx?c=us_ot&lang=en&prod=lpql810weus),
[QL-820NWBc interfaces](https://www.brother-usa.com/p/thermal-printers-labelers/QL820NWBC),
[TD-2320D/2350D interfaces](https://support.brother.co.jp/j/s/support/html/td2320d_jp/html/GUID-86558F68-0131-4849-8951-4B36D3BF185A_1.html),
[TD-2030A SDK transport](https://support.brother.co.jp/j/s/support/html/mobilesdk/reference/appendix/printer-configurations.html#td-2030a).

Amazon Affiliate Links: **https://amzn.to/3AiiOFT**

**Supplement**

|     | description                |
| --- | -------------------------- |
| ✅  | Supported and tested       |
| △   | Implemented but not tested |
| -   | Plugin is not supported    |
| ❌  | Device is not supported    |
| BT  | Bluetooth                  |
| BLE | Bluetooth Low Energy       |

※1 Due to low Bluetooth version, connection is not possible with iOS. Ref: https://okbizcs.okwave.jp/brother/qa/q9932082.html


See [connection lifecycle](docs/connection-management.md), [migration](docs/migration.md), and
[verification baseline and limitations](docs/verification.md). Android USB accepts
exactly one connected Brother device; multiple Brother USB devices are rejected.

## API

<docgen-index>

* [`printImage(...)`](#printimage)
* [`search(...)`](#search)
* [`isChannelAvailable(...)`](#ischannelavailable)
* [`cancelSearchWiFiPrinter()`](#cancelsearchwifiprinter)
* [`cancelSearchBluetoothPrinter()`](#cancelsearchbluetoothprinter)
* [`addListener(BrotherPrintEventsEnum.onPrinterAvailable, ...)`](#addlistenerbrotherprinteventsenumonprinteravailable-)
* [`addListener(BrotherPrintEventsEnum.onPrint, ...)`](#addlistenerbrotherprinteventsenumonprint-)
* [`addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, ...)`](#addlistenerbrotherprinteventsenumonprintfailedcommunication-)
* [`addListener(BrotherPrintEventsEnum.onPrintError, ...)`](#addlistenerbrotherprinteventsenumonprinterror-)
* [Interfaces](#interfaces)
* [Type Aliases](#type-aliases)
* [Enums](#enums)

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

--------------------


### search(...)

```typescript
search(option: BRLMSearchOption) => Promise<void>
```

Search for printers, delivered through onPrinterAvailable events. Resolves when discovery ends, including when no printers are found.

| Param        | Type                                                          |
| ------------ | ------------------------------------------------------------- |
| **`option`** | <code><a href="#brlmsearchoption">BRLMSearchOption</a></code> |

--------------------


### isChannelAvailable(...)

```typescript
isChannelAvailable(option: BRLMChannelResult) => Promise<isChannelAvailableResult>
```

If you have saved the last connected <a href="#brlmchannelresult">BRLMChannelResult</a>,
you can use it to verify whether it is currently usable.

| Param        | Type                                                            |
| ------------ | --------------------------------------------------------------- |
| **`option`** | <code><a href="#brlmchannelresult">BRLMChannelResult</a></code> |

**Returns:** <code>Promise&lt;<a href="#ischannelavailableresult">isChannelAvailableResult</a>&gt;</code>

--------------------


### cancelSearchWiFiPrinter()

```typescript
cancelSearchWiFiPrinter() => Promise<void>
```

Stop an active search before its timeout, including when leaving the screen.

--------------------


### cancelSearchBluetoothPrinter()

```typescript
cancelSearchBluetoothPrinter() => Promise<void>
```

Stop an active search before its timeout, including when leaving the screen.

--------------------


### addListener(BrotherPrintEventsEnum.onPrinterAvailable, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrinterAvailable, listenerFunc: (printers: BRLMChannelResult) => void) => Promise<PluginListenerHandle>
```

Find the printer that can connected to the device.

| Param              | Type                                                                                         |
| ------------------ | -------------------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrinterAvailable</a></code> |
| **`listenerFunc`** | <code>(printers: <a href="#brlmchannelresult">BRLMChannelResult</a>) =&gt; void</code>       |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

--------------------


### addListener(BrotherPrintEventsEnum.onPrint, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrint, listenerFunc: () => void) => Promise<PluginListenerHandle>
```

Success Print Event

| Param              | Type                                                                              |
| ------------------ | --------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrint</a></code> |
| **`listenerFunc`** | <code>() =&gt; void</code>                                                        |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

--------------------


### addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrintFailedCommunication, listenerFunc: (info: ErrorInfo) => void) => Promise<PluginListenerHandle>
```

Failed to connect to the printer.
ex: Bluetooth is off, Printer is off, etc.

| Param              | Type                                                                                                 |
| ------------------ | ---------------------------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrintFailedCommunication</a></code> |
| **`listenerFunc`** | <code>(info: <a href="#errorinfo">ErrorInfo</a>) =&gt; void</code>                                   |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

--------------------


### addListener(BrotherPrintEventsEnum.onPrintError, ...)

```typescript
addListener(eventName: BrotherPrintEventsEnum.onPrintError, listenerFunc: (info: ErrorInfo) => void) => Promise<PluginListenerHandle>
```

Failed to print.

| Param              | Type                                                                                   |
| ------------------ | -------------------------------------------------------------------------------------- |
| **`eventName`**    | <code><a href="#brotherprinteventsenum">BrotherPrintEventsEnum.onPrintError</a></code> |
| **`listenerFunc`** | <code>(info: <a href="#errorinfo">ErrorInfo</a>) =&gt; void</code>                     |

**Returns:** <code>Promise&lt;<a href="#pluginlistenerhandle">PluginListenerHandle</a>&gt;</code>

--------------------


### Interfaces


#### PluginListenerHandle

| Prop         | Type                                      |
| ------------ | ----------------------------------------- |
| **`remove`** | <code>() =&gt; Promise&lt;void&gt;</code> |


### Type Aliases


#### BRLMPrintOptions

Model-specific settings. The native bridge also validates calls from untyped JavaScript.

<code>{ encodedImage: string } & <a href="#omit">Omit</a>&lt;<a href="#partial">Partial</a>&lt;<a href="#brlmchannelresult">BRLMChannelResult</a>&gt;, 'modelName'&gt; & ( | ({ modelName: <a href="#brlmprintermodelname">BRLMPrinterModelName.QL_800</a> | <a href="#brlmprintermodelname">BRLMPrinterModelName.QL_810W</a> | <a href="#brlmprintermodelname">BRLMPrinterModelName.QL_820NWB</a>; } & <a href="#brlmprinterqlmodelsettings">BRLMPrinterQLModelSettings</a>) | ({ modelName: | <a href="#brlmprintermodelname">BRLMPrinterModelName.TD_2320D_203</a> | <a href="#brlmprintermodelname">BRLMPrinterModelName.TD_2030AD</a> | <a href="#brlmprintermodelname">BRLMPrinterModelName.TD_2350D_300</a>; } & <a href="#brlmprintertdmodelsettings">BRLMPrinterTDModelSettings</a>) )</code>


#### Omit

Construct a type with the properties of T except for those in type K.

<code><a href="#pick">Pick</a>&lt;T, <a href="#exclude">Exclude</a>&lt;keyof T, K&gt;&gt;</code>


#### Pick

From T, pick a set of properties whose keys are in the union K

<code>{ [P in K]: T[P]; }</code>


#### Exclude

<a href="#exclude">Exclude</a> from T those types that are assignable to U

<code>T extends U ? never : T</code>


#### Partial

Make all properties in T optional

<code>{ [P in keyof T]?: T[P]; }</code>


#### BRLMChannelResult

<code>{ port: <a href="#brlmprinterport">BRLMPrinterPort</a>; modelName: string; serialNumber: string; macAddress: string; nodeName: string; location: string; /** * This need to connect to the printer. * wifi: IP Address * bluetooth: Android MAC address; iOS serial number * bluetoothLowEnergy: SDK local name; usb: opaque discovery token */ channelInfo: string; }</code>


#### BRLMPrinterQLModelSettings

<code>{ /** * Should use enum <a href="#brlmprinterlabelname">BRLMPrinterLabelName</a> */ labelName: <a href="#brlmprinterlabelname">BRLMPrinterLabelName</a>; } & <a href="#brlmprintersettings">BRLMPrinterSettings</a></code>


#### BRLMPrinterSettings

These are optional. If these are not set, default values are assigned by the printer.

<code>{ /** * The number of copies you print. */ numberOfCopies?: <a href="#brlmprinternumberofcopies">BRLMPrinterNumberOfCopies</a>; /** * Whether the auto-cut is enabled or not. If true, your printer cut the paper each page. */ autoCut?: <a href="#brlmprinterautocuttype">BRLMPrinterAutoCutType</a>; /** * A scale mode that specifies how your data is scaled in a print area of your printer. */ scaleMode?: <a href="#brlmprinterscalemode">BRLMPrinterScaleMode</a>; /** * A scale value. This is effective when ScaleMode is ScaleValue. */ scaleValue?: <a href="#brlmprinterscalevaluetype">BRLMPrinterScaleValueType</a>; /** * A way to rasterize your data. */ halftone?: <a href="#brlmprinterhalftone">BRLMPrinterHalftone</a>; /** * A threshold value. This is effective when the Halftone is Threshold. */ halftoneThreshold?: <a href="#brlmprinterhalftonethresholdtype">BRLMPrinterHalftoneThresholdType</a>; /** * An image rotation that specifies the angle in which your data is placed in the print area. Rotation direction is clockwise. */ imageRotation?: <a href="#brlmprinterimagerotation">BRLMPrinterImageRotation</a>; /** * A vertical alignment that specifies how your data is placed in the printable area. */ verticalAlignment?: <a href="#brlmprinterverticalalignment">BRLMPrinterVerticalAlignment</a>; /** * A horizontal alignment that specifies how your data is placed in the printable area. */ horizontalAlignment?: <a href="#brlmprinterhorizontalalignment">BRLMPrinterHorizontalAlignment</a>; /** * A compress mode that specifies how to compress your data. * note: This is ios only. */ compressMode?: <a href="#brlmprintercompressmode">BRLMPrinterCompressMode</a>; /** * A priority that is print speed or print quality. Whether or not this has an effect is depend on your printer. */ printQuality?: <a href="#brlmprinterprintquality">BRLMPrinterPrintQuality</a>; }</code>


#### BRLMPrinterNumberOfCopies

<code>number</code>


#### BRLMPrinterAutoCutType

<code>boolean</code>


#### BRLMPrinterScaleValueType

<code>number</code>


#### BRLMPrinterHalftoneThresholdType

<code>number</code>


#### BRLMPrinterTDModelSettings

<code>{ /** * Should use enum BRKMPrinterCustomPaperType */ paperType: <a href="#brlmprintercustompapertype">BRLMPrinterCustomPaperType</a>; /** * The width of the label. For example, the RD-U04J1 is 60.0 wide. */ tapeWidth: number; /** * The length of the label. For example, the RD-U04J1 is 60.0 wide. */ tapeLength: number; /** * It is the difference between a sticker and a mount. * For example, the RD-U04J1 is `1.0, 2.0, 1.0, 2.0` */ marginTop: number; marginRight: number; marginBottom: number; marginLeft: number; /** * The spacing between seals. For example, the RD-U04J1 is 0.2. */ gapLength: number; paperMarkPosition: number; paperMarkLength: number; /** * Should use enum BRKMPrinterCustomPaperUnit. * For example, the RD-U04J1 is mm. */ paperUnit: <a href="#brlmprintercustompaperunit">BRLMPrinterCustomPaperUnit</a>; } & <a href="#brlmprintersettings">BRLMPrinterSettings</a></code>


#### BRLMSearchOption

<code>{ /** * 'usb' is android only, and now developing. */ port: <a href="#brlmprinterport">BRLMPrinterPort</a>; /** * searchDuration is the time to end search for devices. * default is 15 seconds. * use only port is 'wifi' or 'bluetoothLowEnergy'. */ searchDuration: number; /** * Android Bluetooth Classic only. Include only devices whose Bluetooth class * reports a printer. Defaults to false; ignored for other ports and on iOS. * This does not identify Brother devices. Devices with an unknown class are excluded when true. */ bluetoothPrintersOnly?: boolean; }</code>


#### isChannelAvailableResult

<code>{ result: boolean; }</code>


#### ErrorInfo

<code>{ message: string; /** Legacy platform-specific numeric SDK code. Prefer category for branching. */ code: number; category?: <a href="#brotherprintererrorcode">BrotherPrinterErrorCode</a>; nativeCode?: string | number; }</code>


#### BrotherPrinterErrorCode

Stable application-level error categories; native SDK values remain available separately.

<code>'INVALID_ARGUMENT' | 'PERMISSION_DENIED' | 'NOT_FOUND' | 'UNSUPPORTED' | 'CANCELLED' | 'BUSY' | 'COMMUNICATION' | 'PRINT_FAILED'</code>


### Enums


#### BRLMPrinterPort

| Members                  | Value                             |
| ------------------------ | --------------------------------- |
| **`usb`**                | <code>'usb'</code>                |
| **`wifi`**               | <code>'wifi'</code>               |
| **`bluetooth`**          | <code>'bluetooth'</code>          |
| **`bluetoothLowEnergy`** | <code>'bluetoothLowEnergy'</code> |


#### BRLMPrinterModelName

| Members            | Value                       |
| ------------------ | --------------------------- |
| **`QL_800`**       | <code>'QL_800'</code>       |
| **`QL_810W`**      | <code>'QL_810W'</code>      |
| **`QL_820NWB`**    | <code>'QL_820NWB'</code>    |
| **`TD_2320D_203`** | <code>'TD_2320D_203'</code> |
| **`TD_2030AD`**    | <code>'TD_2030AD'</code>    |
| **`TD_2350D_300`** | <code>'TD_2350D_300'</code> |


#### BRLMPrinterLabelName

| Members               | Value                          | Description   |
| --------------------- | ------------------------------ | ------------- |
| **`DieCutW17H54`**    | <code>'DieCutW17H54'</code>    |               |
| **`DieCutW17H87`**    | <code>'DieCutW17H87'</code>    |               |
| **`DieCutW23H23`**    | <code>'DieCutW23H23'</code>    |               |
| **`DieCutW29H42`**    | <code>'DieCutW29H42'</code>    |               |
| **`DieCutW29H90`**    | <code>'DieCutW29H90'</code>    |               |
| **`DieCutW38H90`**    | <code>'DieCutW38H90'</code>    |               |
| **`DieCutW39H48`**    | <code>'DieCutW39H48'</code>    |               |
| **`DieCutW52H29`**    | <code>'DieCutW52H29'</code>    |               |
| **`DieCutW62H29`**    | <code>'DieCutW62H29'</code>    |               |
| **`DieCutW62H60`**    | <code>'DieCutW62H60'</code>    |               |
| **`DieCutW62H75`**    | <code>'DieCutW62H75'</code>    |               |
| **`DieCutW62H100`**   | <code>'DieCutW62H100'</code>   |               |
| **`DieCutW60H86`**    | <code>'DieCutW60H86'</code>    |               |
| **`DieCutW54H29`**    | <code>'DieCutW54H29'</code>    |               |
| **`DieCutW102H51`**   | <code>'DieCutW102H51'</code>   |               |
| **`DieCutW102H152`**  | <code>'DieCutW102H152'</code>  |               |
| **`DieCutW103H164`**  | <code>'DieCutW103H164'</code>  |               |
| **`RollW12`**         | <code>'RollW12'</code>         |               |
| **`RollW29`**         | <code>'RollW29'</code>         |               |
| **`RollW38`**         | <code>'RollW38'</code>         |               |
| **`RollW50`**         | <code>'RollW50'</code>         |               |
| **`RollW54`**         | <code>'RollW54'</code>         |               |
| **`RollW62`**         | <code>'RollW62'</code>         |               |
| **`RollW62RB`**       | <code>'RollW62RB'</code>       |               |
| **`RollW102`**        | <code>'RollW102'</code>        |               |
| **`RollW103`**        | <code>'RollW103'</code>        |               |
| **`DTRollW90`**       | <code>'DTRollW90'</code>       |               |
| **`DTRollW102`**      | <code>'DTRollW102'</code>      |               |
| **`DTRollW102H51`**   | <code>'DTRollW102H51'</code>   |               |
| **`DTRollW102H152`**  | <code>'DTRollW102H152'</code>  |               |
| **`RoundW12DIA`**     | <code>'RoundW12DIA'</code>     |               |
| **`RoundW24DIA`**     | <code>'RoundW24DIA'</code>     |               |
| **`RoundW58DIA`**     | <code>'RoundW58DIA'</code>     |               |
| **`RDDieCutW60H60`**  | <code>'RDDieCutW60H60'</code>  | For TD series |
| **`RDDieCutW50H30`**  | <code>'RDDieCutW50H30'</code>  |               |
| **`RDDieCutW40H60`**  | <code>'RDDieCutW40H60'</code>  |               |
| **`RDDieCutW40H50`**  | <code>'RDDieCutW40H50'</code>  |               |
| **`RDDieCutW40H40`**  | <code>'RDDieCutW40H40'</code>  |               |
| **`RDDieCutW30H30`**  | <code>'RDDieCutW30H30'</code>  |               |
| **`RDDieCutW50H35`**  | <code>'RDDieCutW50H35'</code>  |               |
| **`RDDieCutW60H80`**  | <code>'RDDieCutW60H80'</code>  |               |
| **`RDDieCutW60H100`** | <code>'RDDieCutW60H100'</code> |               |


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


#### BRLMPrinterCustomPaperType

| Members             | Value                        |
| ------------------- | ---------------------------- |
| **`rollPaper`**     | <code>'rollPaper'</code>     |
| **`dieCutPaper`**   | <code>'dieCutPaper'</code>   |
| **`markRollPaper`** | <code>'markRollPaper'</code> |


#### BRLMPrinterCustomPaperUnit

| Members    | Value               |
| ---------- | ------------------- |
| **`mm`**   | <code>'mm'</code>   |
| **`inch`** | <code>'inch'</code> |


#### BrotherPrintEventsEnum

| Members                          | Value                                     |
| -------------------------------- | ----------------------------------------- |
| **`onPrinterAvailable`**         | <code>'onPrinterAvailable'</code>         |
| **`onPrint`**                    | <code>'onPrint'</code>                    |
| **`onPrintFailedCommunication`** | <code>'onPrintFailedCommunication'</code> |
| **`onPrintError`**               | <code>'onPrintError'</code>               |

</docgen-api>

<!-- rdlabo-docs-omit -->

## Prerelease channels

An open, non-draft pull request can be published to the npm `beta` dist-tag after its `Validation` and `Package Candidate` workflows pass. A repository owner or maintainer must add a comment whose entire body is:

```text
/beta
```

The request authorizes only the pull request head SHA that existed when the comment was added. The workflow revalidates the owner or maintainer permission and head SHA immediately before publishing. Any new commit requires CI to pass again and a fresh owner or maintainer `/beta` comment. Fork pull requests are supported. Pull requests that change a release-gating workflow cannot be beta-published until those workflow changes land on `main`.

Beta versions use `<base>-beta.pr<PR number>.sha<12-character SHA>`. The candidate is built in a read-only workflow without npm publishing credentials. The privileged release workflow publishes only the validated immutable package artifact with lifecycle scripts disabled. A notification failure cannot invalidate a successful npm publish.

When a pull request is merged into `main`, it is automatically published to `beta` only after the required CI and `Package Candidate` succeed for that exact merge commit. Direct pushes to `main` do not publish a candidate.

Only `npm run release` creates a release tag. Stable `vX.Y.Z` tags publish to npm `latest`; revision/prerelease tags publish to `next`. Neither `beta` nor `next` publishing changes the npm `latest` dist-tag.

## Maintainers

- [rdlabo](https://rdlabo.dev/)
<!-- /rdlabo-docs-omit -->

<!-- rdlabo-docs-omit -->

## License

MIT

<!-- /rdlabo-docs-omit -->
