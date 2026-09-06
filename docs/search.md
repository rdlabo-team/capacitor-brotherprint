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


`searchDuration` applies to `wifi` and `bluetoothLowEnergy`. `usb` is Android only. If nothing is found, you get no error and no printers. Signatures are on the [API](/docs/api#brlmsearchoption) page.

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

<!-- !::cancelSearchWiFiPrinter:: -->

<!-- !::cancelSearchBluetoothPrinter:: -->
