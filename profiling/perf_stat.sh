#!/bin/bash
# perf stat: cache misses, IPC, branch mispredictions for mc-pricing
# Run inside Lima VM after build_profile.sh

set -e

BINARY="${1:-./build-profile/mc-pricing}"
CONFIG="${2:-./config/config.yaml}"

echo "=== perf stat: $BINARY ==="

perf stat \
    -e cycles,instructions \
    -e cache-misses,cache-references \
    -e L1-dcache-loads,L1-dcache-load-misses \
    -e LLC-loads,LLC-load-misses \
    -e branch-misses,branches \
    -- "$BINARY" "$CONFIG"
