# Connection management

For existing applications, read the [migration guide](migration.md).

`BrotherPrinter` is a plain TypeScript controller exported from the package root.
It manages native discovery and channel reuse without depending on Angular, Ionic,
React or a particular storage implementation. The controller serializes discovery, availability checks and printing across its instances.
The native SDK has its own serial worker. Raw plugin events remain global: avoid mixing
raw plugin calls and controller-owned flows when associating events with a print job.

```ts
import {
  BrotherPrint,
  BrotherPrinter,
  BRLMPrinterModelName,
  BRLMPrinterPort,
  BRLMPrinterLabelName,
  brotherPrinterPorts,
  type BrotherPrinterCache,
  type BRLMPrintOptions,
} from '@rdlabo/capacitor-brotherprint';

// Replace this in-memory adapter with your persistence if desired.
let remembered: BrotherPrinterCache | null = null;
const printer = new BrotherPrinter({
  plugin: BrotherPrint,
  cache: {
    read: async () => remembered,
    write: async (channel) => {
      remembered = channel;
    },
  },
  onPrintersChanged: (channels) => {
    console.log('Available printers:', channels);
  },
  onPrintError: (error) => {
    console.error(error);
  },
});
const model = BRLMPrinterModelName.QL_820NWB;
// Supply your platform detection; false means iOS capabilities, not web support.
const choices = brotherPrinterPorts(model, true);
console.log(choices);

await printer.listen(); // Before searching; await registration.
try {
  await printer.prepare(model, BRLMPrinterPort.wifi);
  const channel = await printer.selectChannel(async (channels) => {
    // Replace with your picker. Return its channel, or null/undefined to cancel.
    console.log('Choose from:', channels);
    return undefined;
  });
  if (channel) {
    // Supply a real image and model-specific settings as described in print.md.
    const settings: BRLMPrintOptions = {
      modelName: model,
      encodedImage: 'YOUR_BASE64_IMAGE_WITHOUT_DATA_URL_PREFIX',
      labelName: BRLMPrinterLabelName.RollW62,
    };
    await printer.print(settings, channel);
  }
} finally {
  await printer.removeListeners(); // When the printing flow ends.
}
```

- Prefer `search(port, { model, searchDuration: 10, silent: false, requireResult: true,
  bluetoothPrintersOnly: true })`. Duration, empty-result policy, class filtering and UI
  presentation are independent. Errors reject even with `silent: true`. The legacy boolean
  overload retains its 3/10 second and silent-error behavior. Empty required results use
  `PrinterNotFoundError` (`NOT_FOUND`); `PrinterSearchTimeoutError` is a deprecated alias.
- `prepare(model, port)` waits for discovery. USB always rediscovers to reacquire
  access after unplugging. A stored channel is reused only for the same model and
  port, after native availability validation; otherwise discovery runs again.
- `printers` exposes a readonly snapshot. `onPrintersChanged` supplies updates:
  Angular can call `signal.set([...channels])`, React can call its state setter.
- `selectChannel(present)` returns undefined when there are no results, selects a
  single result automatically, and calls your picker for multiple results. Ionic
  callers should translate `role === 'confirm'` and `data.values` in their adapter;
  the controller never inspects dialog roles.
- `print(settings, channel)` remembers the model/channel and awaits native printing.
  Paper, font size, margins, number of copies and image generation belong to the app.
- `onSearchStart` can return an async loading cleanup; `onSearchError` presents visible
  failures. `onPrint`, `onPrintError`, and `onPrintFailedCommunication` receive native
  notifications. Callback failures are isolated and sent to `onAdapterError`. Cache is
  optional; read failures cause rediscovery and write failures do not prevent printing.

`brotherPrinterPorts(model, isAndroid)` covers every model in the README, including
both QL-820NWB variants. `wifi` denotes the SDK network transport, including wired
Ethernet for TD-2320D. TD-2350D includes BLE. USB is excluded for iOS, and unknown
model names return an empty array. This is a capability list, not a promise that
hardware is present or a claim of real-device verification. See the README status
matrix. Choose defaults and migrate existing preferences in the app.

## Known or manually entered printers

Keep the full channel (especially `port`), not just an address. Addresses are opaque:
colons can also appear in a network address, and a BLE local name is not a MAC address.
The native SDK remains responsible for validating/supporting the supplied address.
For a manually entered network address, use the following explicit channel:

```ts
const manual = {
  port: BRLMPrinterPort.wifi,
  channelInfo: '192.0.2.10',
  modelName: 'QL-820NWB',
  serialNumber: '',
  macAddress: '',
  nodeName: '',
  location: '',
};
const available = await printer.useChannel(BRLMPrinterModelName.QL_820NWB, manual);
// If false, ask the user to check this destination; do not silently print elsewhere.
// If true, manual is ready to pass to printer.print(settings, manual).
```

`useChannel(model, channel)` checks availability in the same queue as discovery,
clears stale results, and returns a boolean for reachability. Permission/SDK failures reject
with `BrotherPrinterError`; a mismatched model rejects with `INVALID_ARGUMENT`. It does not scan for a replacement or
persist the address until printing. Empty addresses and USB return false; USB needs
`prepare`/discovery to obtain native permission. The app can persist a manual entry
or selected preference separately, even before its first print.

Maintain multiple known printers, labels, preferred order, and hidden devices in the
app. Merge discovered metadata into those records using `(port, channelInfo)` as the
identity; do not imply a stored printer is currently reachable. The controller also
replaces repeated discovery notifications for the same identity with fresh metadata.

## React lifecycle and diagnostics

Keep one controller in a stable ref or application service. Call `listen` on entry
and `removeListeners` on cleanup; registration/removal requests run in order, so
React's setup → cleanup → setup sequence leaves the new registration active. Retired
callbacks are ignored. In an asynchronous effect, use a cancelled flag before starting
search after `await printer.listen()`; do not start discovery for an already closed view.

Use existing callbacks to feed an app-owned logger: search start/failure, result
changes, print success, native print errors and communication failures. Also log the
`print()` promise's start/settlement in the caller, with a per-job identifier. Native
events do not carry a job ID; keep one active print and record its terminal outcome
once (an error event and promise rejection may describe the same failure). Use native
promise completion to unlock submission, not a fixed timeout; never retry printing
automatically when the physical outcome is uncertain.

A bounded history and an explicit diagnostic export UI are useful for field support.
Keep storage, export destinations, app/device versions, and any identifying data in
the app's adapter. Avoid copying image payloads into connection logs. No logging or
network upload is enabled by the controller itself.

## Cancellation, validation and migration

`cancelPending()` invalidates work not yet handed to the SDK. `removeListeners()` also
cancels pending work and permits later `listen()`; `dispose()` permanently closes the
controller and waits for already-running work to settle. Wi-Fi and BLE cancellation are
best effort. USB permission dialogs and iOS Bluetooth accessory UI cannot be dismissed
by these APIs: close them on the device. A cancellation request never releases the SDK
queue early. Printing already handed to native code retains its actual completion result.

Model filtering accepts only aliases in `printer-models.json`, including Brother-prefixed
product names and QL-820NWBc. Unknown or mismatched models are excluded. TD product names
omit resolution; the catalog maps TD-2320D to the supported 203 dpi model and TD-2350D to
the supported 300 dpi model. Explicit other-resolution names are rejected. For manual
entries, the caller must provide the correct physical model; opening an address alone
does not prove its model.

Android USB requires exactly one connected Brother device. The SDK has no public
USB-target argument; ambiguous multi-device operation is rejected instead of selecting
arbitrarily. The SDK itself returns an empty USB address. The bridge supplies a nonempty token from the permitted Android device identity and verifies it before opening. Always preserve the token from new discovery; old empty USB results must be rediscovered. iOS USB
and all Web operations are unsupported. Web never logs print image data or reports
successful printing.

`BRLMPrintOptions` now discriminates QL versus TD settings by `modelName`; TD requires
custom paper settings. Raw JavaScript input is also validated in both native bridges.
`BrotherPrinterError.code` provides stable categories (`INVALID_ARGUMENT`,
`PERMISSION_DENIED`, `NOT_FOUND`, `UNSUPPORTED`, `CANCELLED`, `BUSY`, `COMMUNICATION`,
`PRINT_FAILED`). `cause` preserves the original rejection and `nativeCode` preserves
available SDK details. Error event `code` remains numeric and platform-specific;
`category` and `nativeCode` are additive. Do not compare SDK numeric codes between OSes.

The typechecked [plain TypeScript example](../examples/plain-typescript.ts) has no Angular
or Ionic dependencies. CJS, ESM and package type resolution are checked by `npm run
pack:verified`, which stages production SPM paths without rewriting the checkout.
