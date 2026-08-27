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
