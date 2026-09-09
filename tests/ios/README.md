# SDK-free iOS regression tests

With Xcode and an installed iPhone simulator, run from the repository root:

```sh
bash tests/ios/test.sh
```

The script copies the current production `PrintImageDecoder.swift` into an ignored build-input directory and runs XCTest on an available iPhone simulator. There are no Brother or Capacitor dependencies and no hand-maintained copy of the decoding logic.

Tests exercise actual Foundation/UIKit decoding of invalid Base64, non-image bytes, truncated image data, and a valid PNG. These tests do not cover plugin Promise/event delivery, SDK compatibility, or printer communication; existing local native integration tests remain necessary for those paths.
