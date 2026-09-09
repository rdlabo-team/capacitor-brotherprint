# SDK-free Android regression tests

Run with JDK 21 from the repository root:

```sh
bash android/gradlew -p tests/android test
```

This standalone Kotlin/JVM project compiles the production Bluetooth filter and its existing tests directly. It needs neither npm dependencies nor the Android/Brother SDK. The device-class reader is replaced with a lambda in tests; the filter logic is not copied or mocked.

Coverage: printer class bits (including observed `0x140680`), non-printer/unknown classes, and enabled/disabled/default filtering. The disabled filter must not read device information.

This is not a native plugin build or a hardware integration test. USB permission calls, SDK search completion, image validation, and print events still require the existing local native tests; they are not covered by this CI job. No Brother SDK binaries are downloaded, cached, or published by this workflow.
