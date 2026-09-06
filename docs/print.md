# Print

`printImage` sends a base64 image (MIME type removed) to a Brother printer. Call this after [Installation](/docs/installation). Discover a printer with [Search](/docs/search) and register [Events](/docs/events) for print results before printing.

Prepare a real image yourself (for example encode a PNG/JPEG from your app and strip any `data:...;base64,` prefix). Use `port` and `channelInfo` from the `BRLMChannelResult` you retained from `onPrinterAvailable`. Pick a `modelName` / `labelName` that matches your device and the [supported models](/docs/readme#supported-models) table.

```typescript
import {
  BrotherPrint,
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
} from '@rdlabo/capacitor-brotherprint';
import type { BRLMChannelResult, BRLMPrintOptions } from '@rdlabo/capacitor-brotherprint';

const printImage = async (printer: BRLMChannelResult, encodedImage: string) => {
  const options: BRLMPrintOptions = {
    modelName: BRLMPrinterModelName.QL_820NWB,
    labelName: BRLMPrinterLabelName.RollW62,
    encodedImage,
    numberOfCopies: 1,
    autoCut: true,
    port: printer.port,
    channelInfo: printer.channelInfo,
  };

  await BrotherPrint.printImage(options);
};
```

See the demo for a complete page:

https://github.com/rdlabo-dev/capacitor-brotherprint/blob/v8.1.1/demo/src/app/home/home.page.ts

Option fields are on the [API](/docs/api#brlmprintoptions) page.
