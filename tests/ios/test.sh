#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"

# Compile the actual production source without the plugin's Brother SDK dependency.
mkdir -p Sources
cp ../../ios/Sources/BrotherPrintPlugin/PrintImageDecoder.swift ../../ios/Sources/BrotherPrintPlugin/PrinterValidation.swift ../../ios/Sources/BrotherPrintPlugin/PrinterModelCatalog.swift Sources/

simulator_id=$(xcrun simctl list devices available -j | python3 -c '
import json, sys
devices = json.load(sys.stdin)["devices"]
print(next(device["udid"] for runtime, entries in devices.items()
           if "iOS" in runtime for device in entries if device["name"].startswith("iPhone")))
')
xcodebuild -scheme BrotherPrintLogic \
  -destination "platform=iOS Simulator,id=$simulator_id" \
  -derivedDataPath .build test
