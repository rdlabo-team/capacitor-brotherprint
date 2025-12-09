#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
ANDROID_DIR="$ROOT_DIR/android"

WARN_ONLY=${WARN_ONLY:-false}
STRICT=${STRICT:-false}
AAR_PATH=${AAR_PATH:-}
GRADLE_JVM_ARGS=${GRADLE_JVM_ARGS:--Dorg.gradle.jvmargs="-Xmx2g"}

cd "$ANDROID_DIR"

ARGS=( "$GRADLE_JVM_ARGS" :verifyBrotherNative16KB )

if [[ -n "$AAR_PATH" ]]; then
  ARGS+=( -PbrotherPrint.aarPath="$AAR_PATH" )
fi

if [[ "$WARN_ONLY" == "true" ]]; then
  ARGS+=( -PbrotherPrint.warnOnly=true )
else
  ARGS+=( -PbrotherPrint.warnOnly=false )
fi

if [[ "$STRICT" == "true" ]]; then
  ARGS+=( -PbrotherPrint.strict16kb=true )
fi

echo "Running: ./gradlew ${ARGS[*]}"
./gradlew "${ARGS[@]}"

