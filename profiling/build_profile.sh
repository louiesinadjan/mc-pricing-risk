#!/bin/bash
# Build with Profile flags (-O2 -g -fno-omit-frame-pointer)
# Run this INSIDE the Lima VM

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

cmake -S "$SCRIPT_DIR" -B "$SCRIPT_DIR/build-profile" \
    -DCMAKE_BUILD_TYPE=Profile \
    -DENABLE_TESTS=OFF

cmake --build "$SCRIPT_DIR/build-profile" --parallel

echo "Profile binary: $SCRIPT_DIR/build-profile/mc-pricing"
