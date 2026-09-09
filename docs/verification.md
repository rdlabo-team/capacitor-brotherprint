# Verification and release checks

The current changes were checked on 2026-09-09. README device checkmarks record
prior device tests; they do not claim that the new cancellation, model filtering or
USB ambiguity handling has been retested on every physical printer.

- `npm test`: controller regression tests, Web unsupported behavior, public API
  typechecking and generated model table consistency.
- `bash android/gradlew -p tests/android test`: production Bluetooth filtering,
  input validation and single-Brother-USB policy without proprietary SDKs.
- `bash tests/ios/test.sh`: production image decoding and input validation on the
  iOS simulator, without the Brother SDK.
- Native compilation: Android SDK v4 AAR described in installation.md; iOS SDK
  4.13.2, Xcode 26.5, generic iOS arm64 destination, deployment target iOS 15.
- `npm run pack:verified`: stages npm's actual file list, checks SDK exclusion,
  rewrites production SPM paths only in the stage, loads CJS and ESM entry points,
  and typechecks a framework-independent consumer against the packaged files.
  The resulting archive is local; this command does not publish to npm.

The canonical catalog is `printer-models.json`. After editing, run `npm run
generate:models`. Generated TypeScript/Kotlin/Swift tables must remain in sync;
CI checks them before the TypeScript tests. Adding a model still requires confirming
SDK availability, paper settings, transport capabilities and physical behavior.

The native queues serialize SDK work, including across plugin instances. The iOS
accessory picker runs on the main queue while the SDK worker waits for its completion.
Driver open/print/close execute together on a worker, consistent with Brother's
[single-thread requirement](https://support.brother.com/g/s/es/htmldoc/mobilesdk/reference/ios_v4/brlmprinterdriver.html).
Cancellation does not unlock the worker until native work returns. Native events
remain global and have no job ID; use promise completion as the authoritative job
outcome, and do not mix raw and controller calls when correlating events.

Still requiring physical verification: multiple USB devices and hot unplug, OS
permission denial/recovery, cancellation during discovery, Bluetooth accessory UI,
and printing each supported model/transport. Unit tests and compilation do not
establish these hardware outcomes. No physical print was performed during this
hardening work.

`prepublishOnly` retains the legacy in-place path replacement for direct npm publish.
Use `pack:verified` for a reviewable staged artifact; if running `prepublishOnly`
manually, restore development paths with `npm run replace:development` afterward.
