# Usage

Install the native SDK and permissions using [installation](installation.md).
`BrotherPrinter` works with plain TypeScript, Angular, Ionic and React; framework
state, dialogs and label layout remain in the application.

```typescript
import {
  BrotherPrint,
  BrotherPrinter,
  BrotherPrinterError,
  BRLMPrinterLabelName,
  BRLMPrinterModelName,
  BRLMPrinterPort,
  type BRLMChannelResult,
} from '@rdlabo/capacitor-brotherprint';

export async function printLabel(
  encodedImage: string,
  choose: (channels: readonly BRLMChannelResult[]) => Promise<BRLMChannelResult | null>,
): Promise<void> {
  const printer = new BrotherPrinter({ plugin: BrotherPrint });
  try {
    await printer.listen();
    await printer.prepare(BRLMPrinterModelName.QL_820NWB, BRLMPrinterPort.wifi);
    const channel = await printer.selectChannel(choose);
    if (!channel) return; // The user cancelled, or discovery produced no channel.
    await printer.print({
      modelName: BRLMPrinterModelName.QL_820NWB,
      labelName: BRLMPrinterLabelName.RollW62,
      encodedImage, // PNG/JPEG Base64 without the data URL prefix.
      numberOfCopies: 1,
    }, channel);
  } catch (error) {
    if (error instanceof BrotherPrinterError && error.code === 'CANCELLED') return;
    throw error; // Let the caller show a localized error; do not automatically retry.
  } finally {
    await printer.dispose();
  }
}
```

The application supplies `choose`, including its own cancel behavior. A sole printer
is selected automatically; multiple matching printers are passed to the picker.
For a reusable screen, keep the controller in a service or stable ref and use
`listen()`/`removeListeners()` on entry/exit. See
[connection management](connection-management.md) for caching, manual destinations,
React lifecycle, cancellation and diagnostics.

The [plain TypeScript example](../examples/plain-typescript.ts) is typechecked by CI.
The [Angular demo](https://github.com/rdlabo-dev/capacitor-brotherprint/tree/main/demo)
shows the lower-level native bridge. Raw bridge methods are described in
[search](search.md), [print](print.md) and [events](events.md).
Existing applications should read [migration](migration.md) before upgrading.
