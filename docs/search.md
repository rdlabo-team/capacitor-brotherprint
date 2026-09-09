# Search

Search finds nearby Brother printers. Results arrive on `onPrinterAvailable`. Call this after [Installation](/docs/installation). Register the listener before `search`, keep the discovered `BRLMChannelResult` (especially `channelInfo` and `port`), then continue to [Print](/docs/print). Full event list: [Events](/docs/events).

## search

Register `onPrinterAvailable`, retain the channel, then start a Wi-Fi search. The `search` call itself returns `void`.

```typescript
import type { PluginListenerHandle } from '@capacitor/core';
import {
  BrotherPrint,
  BrotherPrintEventsEnum,
  BRLMPrinterPort,
} from '@rdlabo/capacitor-brotherprint';
import type { BRLMChannelResult } from '@rdlabo/capacitor-brotherprint';

let discovered: BRLMChannelResult | undefined;
let availableHandle: PluginListenerHandle | undefined;

const searchWifiPrinters = async () => {
  if (!availableHandle) {
    availableHandle = await BrotherPrint.addListener(
    BrotherPrintEventsEnum.onPrinterAvailable,
    (printer) => {
      discovered = printer;
      console.log('channelInfo', printer.channelInfo);
    },
    );
  }

  await BrotherPrint.search({
    port: BRLMPrinterPort.wifi,
    searchDuration: 15, // seconds
  });
};

const stopSearching = async () => {
  try {
    await BrotherPrint.cancelSearchWiFiPrinter();
  } finally {
    await availableHandle?.remove();
    availableHandle = undefined;
  }
};
```

Call `searchWifiPrinters` from the search button and await `stopSearching` when leaving the screen.

On iOS, `bluetooth` first lists connected MFi printers. If none are connected, the app displays the system Bluetooth accessory picker so you can select and pair a printer. The search promise completes after the picker callback; picker errors reject the promise.

For BLE-capable printers, use `port: BRLMPrinterPort.bluetoothLowEnergy`. On iOS this uses `startBLESearch`, without the Bluetooth accessory picker. Pass the discovered printer's `channelInfo` (BLE local name) unchanged to `isChannelAvailable` or `printImage`. BLE search errors reject the search promise. QL-820NWB/QL-820NWBc do not support BLE printing; use `bluetooth` or `wifi` for these models.

On Android, pair a Bluetooth printer in the system settings before calling `search` with `bluetooth`; the SDK lists paired printers and does not provide the iOS accessory picker. Bluetooth and BLE searches resolve after the search finishes, or reject on SDK errors. Android 12 and later request Nearby devices permissions; Android 11 and earlier request location permission for BLE. `isChannelAvailable` returns `false` when Bluetooth permission is missing.

`searchDuration` applies to `wifi` and `bluetoothLowEnergy`. `usb` is Android only. If nothing is found, you get no error and no printers. Signatures are on the [API](/docs/api#brlmsearchoption) page.

On Android, Bluetooth Classic searches return paired devices. To include only devices that report the Bluetooth Imaging/Printer class:

```typescript
await BrotherPrint.search({
  port: BRLMPrinterPort.bluetooth,
  searchDuration: 15,
  bluetoothPrintersOnly: true,
});
```

`bluetoothPrintersOnly` defaults to `false`, preserving the unfiltered results. It is ignored on iOS and for other ports, including BLE. The filter does not depend on device names and does not identify Brother products: other manufacturers' printers can still appear. Printers with a missing or non-printer Bluetooth class are excluded when enabled.

## isChannelAvailable

If you saved the last `BRLMChannelResult`, check whether that channel is still usable before [Print](/docs/print).

```typescript
import { BrotherPrint } from '@rdlabo/capacitor-brotherprint';
import type { BRLMChannelResult } from '@rdlabo/capacitor-brotherprint';

const checkChannel = async (lastPrinter: BRLMChannelResult) => {
  const { result } = await BrotherPrint.isChannelAvailable(lastPrinter);
  if (!result) {
    await BrotherPrint.search({
      port: lastPrinter.port,
      searchDuration: 15,
    });
  }
};
```

<!-- !::isChannelAvailable:: -->

<!-- !::isChannelAvailableResult:: -->

<!-- !::BRLMChannelResult:: -->

## cancelSearchWiFiPrinter / cancelSearchBluetoothPrinter

Use these to stop an active search before its timeout, including when leaving the screen.

```typescript
import { BrotherPrint } from '@rdlabo/capacitor-brotherprint';

await BrotherPrint.cancelSearchWiFiPrinter();
await BrotherPrint.cancelSearchBluetoothPrinter();
```

See [API](/docs/api#cancelsearchwifiprinter) for the cancellation signatures.
