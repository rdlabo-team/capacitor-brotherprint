# Migrating to connection management

`BrotherPrinter` adds a framework-independent connection lifecycle to the existing
`BrotherPrint` bridge. It owns discovery, selected-channel validation and serialization;
your application still owns image generation, paper/layout settings, persistence and UI.
Start with [usage](usage.md), then see the [controller reference](connection-management.md).

## Existing bridge users

The existing method names remain available. Review these behavior and type changes
before upgrading an application:

| Area | Required action |
| --- | --- |
| Web | Handle `UNSUPPORTED`. Web calls no longer log arguments and resolve as if printing succeeded. Inject an explicit mock for browser previews/tests. |
| Android USB | Connect exactly one Brother USB device and retain the channel returned by discovery. Multiple Brother devices are rejected. Search again after unplugging or permission loss. New USB discovery results contain a bridge-generated identity token; discard older USB results with empty `channelInfo`. |
| Availability | Treat `{ result: false }` as unavailable and handle promise rejections separately. Missing Android Bluetooth permission rejects with `PERMISSION_DENIED`. |
| Print options | Narrow `modelName` before constructing settings. QL options require `labelName`; TD options require `paperType`, `paperUnit` and custom-paper dimensions/margins. |
| Invalid input | Await and catch bridge promises. Native validation rejects malformed paper settings, invalid copy counts/thresholds and missing destinations. |
| Errors | Branch on controller `BrotherPrinterError.code`. Preserve `cause`/`nativeCode` for diagnostics; SDK numeric event codes are platform-specific. |
| Imports | Prefer the package root. CJS and ESM entry points are provided; older extensionless and `.js`-suffixed `dist/esm/*` imports, and the CJS bundle path, remain available. |

Use the [installation baseline](installation.md#build-and-verification-baseline) when
supplying the proprietary SDK. No SDK binaries are included in the npm archive.
Physical verification limits are recorded in [verification](verification.md).

## Adopting the controller

1. Construct one controller for the printing flow with `{ plugin: BrotherPrint }`.
   Add a `cache` adapter only if remembered printers are needed. Adapter failures can
   be observed with `onAdapterError`; storage failure does not stop printing.
2. Await `listen()` before discovery. Call `prepare(model, port)` to use a matching
   cached channel or discover, or `search(port, options)` for an explicit scan.
3. Call `selectChannel(presentPicker)`. Return `null` or `undefined` from the picker
   when the user cancels; stop the print flow when selection is undefined.
4. Pass the selected channel and model-specific image settings to `print()` and await
   its promise. Do not automatically retry a failure with an uncertain physical outcome.
5. Call `removeListeners()` when leaving a reusable view, or `dispose()` when permanently
   discarding the controller. Catch `CANCELLED` from unfinished discovery/queued work.
   Printing already handed to native code retains its actual result.

Cached channels must contain both the discovered `modelName` and the `configuredModel`
used by the controller, plus `port` and `channelInfo`. Missing or mismatched legacy data
causes rediscovery. Do not infer transport from the presence of colons in an address.
Manual destinations require an explicit model and port; see
[known printers](connection-management.md#known-or-manually-entered-printers).

## Search options and cancellation

Prefer `search(port, { model, searchDuration, requireResult, silent,
bluetoothPrintersOnly })`. `silent` controls presentation only; failures still reject.
Use `requireResult: false` to allow empty results. The legacy boolean overload keeps
its previous silent-error and 3/10-second behavior, except cancellation rejects.
`PrinterNotFoundError` means discovery completed without a required matching printer;
`PrinterSearchTimeoutError` is a deprecated alias, not a separate timeout condition.

`cancelPending()` discards work not yet sent to native code and requests best-effort
Wi-Fi/BLE cancellation. The queue stays locked until the running SDK call finishes.
USB permission dialogs and the iOS accessory picker must be closed on the device.
Native events are global and carry no job ID: avoid mixing raw bridge calls with
controller-owned flows, and use promise completion to record one terminal job outcome.
