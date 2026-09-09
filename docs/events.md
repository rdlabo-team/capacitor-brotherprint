# Events

Listen for discovered printers and print results. Register listeners before [Search](/docs/search) and [Print](/docs/print) so the first events are not missed.

```typescript
import type { PluginListenerHandle } from '@capacitor/core';
import { BrotherPrint, BrotherPrintEventsEnum } from '@rdlabo/capacitor-brotherprint';

const handles: PluginListenerHandle[] = [];

const registerPrintListeners = async () => {
  handles.push(
    await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrinterAvailable, (printer) => {
      console.log('printer', printer.channelInfo);
    }),
  );
  handles.push(
    await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrint, () => {
      console.log('onPrint');
    }),
  );
  handles.push(
    await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrintFailedCommunication, (info) => {
      console.log('onPrintFailedCommunication', info);
    }),
  );
  handles.push(
    await BrotherPrint.addListener(BrotherPrintEventsEnum.onPrintError, (info) => {
      console.log('onPrintError', info);
    }),
  );
};

const removePrintListeners = async () => {
  await Promise.all(handles.map((handle) => handle.remove()));
};
```

| Event                        | When it fires                        |
| ---------------------------- | ------------------------------------ |
| `onPrinterAvailable`         | A printer that can connect was found |
| `onPrint`                    | Print succeeded                      |
| `onPrintFailedCommunication` | The printer could not be reached     |
| `onPrintError`               | Print failed                         |

When `printImage` rejects an invalid image, unsupported model or port, or a print-settings creation failure, it also emits `onPrintError` with `code: 0` and an explanatory `message`. This code denotes a plugin validation error, not an SDK error. Failure to open the printer channel emits `onPrintFailedCommunication` instead.

See the demo for a complete page:

https://github.com/rdlabo-dev/capacitor-brotherprint/blob/v8.1.1/demo/src/app/home/home.page.ts

<!-- !::addListener.BrotherPrintEventsEnum:: -->

<!-- !::BrotherPrintEventsEnum:: -->

<!-- !::PluginListenerHandle:: -->

<!-- !::ErrorInfo:: -->
