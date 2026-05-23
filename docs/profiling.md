# Kernel-Level Profiling with eBPF and perf

Production-grade observability for the Monte Carlo engine using Linux eBPF tools via a Lima VM on macOS.

## Why eBPF

`std::chrono` measures wall-clock time end-to-end. eBPF instruments specific functions at the kernel level with near-zero overhead (~1-3%), no recompilation, and per-call latency histograms. This is the standard approach at latency-sensitive trading firms.

## Setup

### 1. Start Lima VM

```bash
brew install lima
limactl start profiling/lima.yaml --name mc-profiler
limactl shell mc-profiler
```

Lima mounts your home directory at the same path inside the VM.

### 2. Build with Profile flags

```bash
cd ~/Documents/code/monte-carlo-cpp
bash profiling/build_profile.sh
```

This builds `build-profile/mc-pricing` with `-O2 -g -fno-omit-frame-pointer`. The frame pointer flag is required for perf and bpftrace to unwind the call stack correctly.

## Tools

### perf stat — hardware counters

```bash
bash profiling/perf_stat.sh
```

Reports: cycles, IPC, L1/L2/LLC cache miss rates, branch mispredictions. Run this first — it identifies whether the bottleneck is compute-bound or memory-bound.

Example output interpretation:
- **IPC < 1.0**: likely memory-bound — cache misses stalling the pipeline
- **LLC miss rate > 5%**: path data not fitting in L3 — consider reducing `num_paths` batch size
- **Branch miss rate > 2%**: payoff conditionals causing mispredicts

### bpftrace — per-function latency

All scripts in `profiling/traces/` attach uprobes to the profile binary at runtime. No recompilation needed.

#### Simulation::run() latency

```bash
sudo bpftrace profiling/traces/simulation_latency.bt \
    -- ./build-profile/mc-pricing config/config.yaml
```

Prints a histogram of total simulation time in nanoseconds.

#### GBM::generate_path() latency

```bash
sudo bpftrace profiling/traces/path_latency.bt \
    -- ./build-profile/mc-pricing config/config.yaml
```

Per-call ns distribution across all Monte Carlo paths. Shows variance in path generation time — useful for identifying outlier paths.

#### RNG vs payoff hotspot

```bash
sudo bpftrace profiling/traces/rng_hotspot.bt \
    -- ./build-profile/mc-pricing config/config.yaml
```

Compares time in `nextNormal()` vs `payoff::evaluate()`. Identifies which component dominates simulation time.

### Flamegraph — call stack visualization

```bash
bash profiling/flamegraph.sh
```

Generates `flamegraph.svg` using Brendan Gregg's tools (auto-installed on first run). Shows the full call stack with time proportional widths — the standard tool for identifying unexpected hotspots.

Copy to macOS for viewing:
```bash
limactl copy mc-profiler:/path/to/flamegraph.svg ./flamegraph.svg
open flamegraph.svg
```

## Build Types

| Type | Flags | Use for |
|------|-------|---------|
| `Release` | `-O3 -march=native` | production, benchmarks |
| `Profile` | `-O2 -g -fno-omit-frame-pointer` | perf / bpftrace tracing |
| `Debug` | `-O0 -g` | gdb, sanitizers |

```bash
cmake -S . -B build-profile -DCMAKE_BUILD_TYPE=Profile
cmake --build build-profile
```

## Symbol Resolution

bpftrace uses glob patterns to match mangled C++ symbols. If a script fails to attach (no output), verify the symbol exists:

```bash
nm ./build-profile/mc-pricing | c++filt | grep -i "generate_path\|evaluate\|nextNormal"
```

Then update the uprobe pattern in the relevant `.bt` file to match.

## Workflow Summary

```
limactl shell mc-profiler
  └─ bash profiling/build_profile.sh          # build with frame pointers
  └─ bash profiling/perf_stat.sh              # hardware counters first
  └─ sudo bpftrace traces/rng_hotspot.bt ...  # identify bottleneck
  └─ sudo bpftrace traces/path_latency.bt ... # drill into hot function
  └─ bash profiling/flamegraph.sh             # full call stack picture
```
