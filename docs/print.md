# Print

`printImage` sends a base64 image to a Brother printer. Call this after [Installation](/docs/readme#installation). Discover printers with [Search](/docs/search) and register [Events](/docs/events) for print results.

```typescript
import {
  BrotherPrint,
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
  BRLMPrinterPort,
} from '@rdlabo/capacitor-brotherprint';
import type { BRLMPrintOptions } from '@rdlabo/capacitor-brotherprint';

const printImage = async () => {
  const options: BRLMPrintOptions = {
    modelName: BRLMPrinterModelName.QL_820NWB,
    labelName: BRLMPrinterLabelName.RollW62,
    encodedImage: 'base64 removed mime-type',
    numberOfCopies: 1,
    autoCut: true,
    port: BRLMPrinterPort.wifi,
    channelInfo: '192.168.0.10',
  };

  await BrotherPrint.printImage(options);
};
```

`port` and `channelInfo` come from an `onPrinterAvailable` result. See the demo for a complete page:

https://github.com/rdlabo-dev/capacitor-brotherprint/blob/v8.1.1/demo/src/app/home/home.page.ts

Option fields are on the [API](/docs/api#brlmprintoptions) page.
