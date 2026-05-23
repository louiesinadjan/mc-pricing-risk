#!/bin/bash
# Generate flamegraph for mc-pricing using perf + Brendan Gregg's flamegraph tools
# Run inside Lima VM after build_profile.sh
#
# Install flamegraph tools first:
#   git clone https://github.com/brendangregg/FlameGraph /opt/FlameGraph

set -e

BINARY="${1:-./build-profile/mc-pricing}"
CONFIG="${2:-./config/config.yaml}"
FLAMEGRAPH_DIR="${FLAMEGRAPH_DIR:-/opt/FlameGraph}"
OUT="flamegraph.svg"

if [ ! -d "$FLAMEGRAPH_DIR" ]; then
    echo "FlameGraph not found. Installing to /opt/FlameGraph..."
    sudo git clone https://github.com/brendangregg/FlameGraph "$FLAMEGRAPH_DIR"
fi

echo "Recording perf data..."
perf record -F 999 -g -- "$BINARY" "$CONFIG"

echo "Generating flamegraph..."
perf script | \
    "$FLAMEGRAPH_DIR/stackcollapse-perf.pl" | \
    "$FLAMEGRAPH_DIR/flamegraph.pl" > "$OUT"

echo "Flamegraph written to $OUT"
echo "Copy to macOS: limactl copy mc-profiler:$(pwd)/$OUT ./$OUT"
