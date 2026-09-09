# Search

Search finds nearby Brother printers. Results arrive on [Events](/docs/events) `onPrinterAvailable`. Call this after [Installation](/docs/readme#installation).

## search

The call returns `void`. Collect printers from the event.

```typescript
import { BrotherPrint, BRLMPrinterPort } from '@rdlabo/capacitor-brotherprint';

await BrotherPrint.search({
  port: BRLMPrinterPort.wifi,
  searchDuration: 15, // seconds
});
```

`searchDuration` applies to `wifi` and `bluetoothLowEnergy`. `usb` is Android only. If nothing is found, you get no error and no printers.

On iOS, `bluetooth` first lists connected MFi printers. If none are connected, the app displays the system Bluetooth accessory picker so you can select and pair a printer. The search promise completes after the picker callback; picker errors reject the promise.

For BLE-capable printers, use `port: BRLMPrinterPort.bluetoothLowEnergy`. On iOS this uses `startBLESearch`, without the Bluetooth accessory picker. Pass the discovered printer's `channelInfo` (BLE local name) unchanged to `isChannelAvailable` or `printImage`. BLE search errors reject the search promise. QL-820NWB/QL-820NWBc do not support BLE printing; use `bluetooth` or `wifi` for these models.

On Android, pair a Bluetooth printer in the system settings before calling `search` with `bluetooth`; the SDK lists paired printers and does not provide the iOS accessory picker. Bluetooth and BLE searches resolve after the search finishes, or reject on SDK errors. Android 12 and later request Nearby devices permissions; Android 11 and earlier request location permission for BLE. `isChannelAvailable` returns `false` when Bluetooth permission is missing.

<!-- !::search:: -->

<!-- !::BRLMSearchOption:: -->

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

Search already times out. Use these only when you run several `search` `port` values at once and want to stop one yourself.

```typescript
import { BrotherPrint } from '@rdlabo/capacitor-brotherprint';

await BrotherPrint.cancelSearchWiFiPrinter();
await BrotherPrint.cancelSearchBluetoothPrinter();
```

<!-- !::cancelSearchWiFiPrinter:: -->

<!-- !::cancelSearchBluetoothPrinter:: -->
