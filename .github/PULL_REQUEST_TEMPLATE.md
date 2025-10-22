## Summary

Add Android 15+ 16 KB page size verification for embedded Brother AAR and document how consumers can supply an updated AAR built with NDK r28+.

## Context

- Android 15+ on 64-bit devices requires 16 KB memory pages.
- Existing Brother SDK `.so` files are aligned to 4096 and need rebuilding with flexible/16KB page support.
- Ref: https://developer.android.com/guide/practices/page-sizes

## What changed

- Gradle task `:verifyBrotherNative16KB` that inspects `jni/*/*.so` PT_LOAD alignment and warns/fails accordingly.
- AAR override via `-PbrotherPrint.aarPath=/abs/path/BrotherPrintLibrary.aar` or `BROTHER_PRINT_AAR_PATH` env var.
- CI workflow to run verification.
- README instructions and `package.json` verify script updated.

## How to test

```
cd android
./gradlew :verifyBrotherNative16KB -PbrotherPrint.warnOnly=false

# test a custom AAR
./gradlew :verifyBrotherNative16KB -PbrotherPrint.aarPath=/path/to/BrotherPrintLibrary.aar -PbrotherPrint.strict16kb=true
```

Expected: Build fails if any PT_LOAD alignment < 16384; otherwise succeeds.

## Notes for maintainers

This PR does not bundle a new Brother AAR. It enables verification and an override path so we can accept a future Brother AAR rebuilt with NDK r28+ without changing plugin code.

