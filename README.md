# Monte Carlo Pricing & Risk

High-performance Monte Carlo simulations for options pricing and risk analysis.

## Prerequisites

```bash
brew install yaml-cpp cmake googletest
```

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/mc-pricing config/config.yaml
```

## Tests

```bash
# Run all tests
ctest --test-dir build

# Run with output on failure
ctest --test-dir build --output-on-failure

# Run a specific test by name
ctest --test-dir build -R BoxMullerTest
```

## Benchmarks

```bash
./build/benchmark_simulation
```

## Profiling

Kernel-level observability via eBPF and perf using a Linux VM on macOS. See [docs/profiling.md](docs/profiling.md) for full details.

### Setup

```bash
brew install lima
limactl start profiling/lima.yaml --name mc-profiler
limactl shell mc-profiler
```

### Build profile binary (inside VM)

```bash
bash profiling/build_profile.sh
```

Builds with `-O2 -g -fno-omit-frame-pointer` - required for stack unwinding.

### Run

```bash
# Hardware counters: cache misses, IPC, branch mispredicts
bash profiling/perf_stat.sh

# Per-function latency histograms (bpftrace)
sudo bpftrace profiling/traces/path_latency.bt -- ./build-profile/mc-pricing config/config.yaml
sudo bpftrace profiling/traces/rng_hotspot.bt  -- ./build-profile/mc-pricing config/config.yaml

# Flamegraph
bash profiling/flamegraph.sh
open flamegraph.svg
```

## Configuration

Edit `config/config.yaml` to change the simulation:

```yaml
market:
  spot: 100.0
  rate: 0.05

contract:
  type: european_call   # european_call, european_put,
                        # asian_arithmetic_call, asian_arithmetic_put,
                        # asian_geometric_call, asian_geometric_put
  strike: 100.0
  maturity: 1.0

model:
  type: gbm
  gbm:
    volatility: 0.20

simulation:
  paths: 100000
  steps: 252
  seed: 42              # 0 = random seed

rng:
  type: std_normal      # std_normal, mersenne_twister
```
