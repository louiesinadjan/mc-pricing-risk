# Performance Considerations

## Optimization Strategies

### 1. Memory Layout
- Contiguous `std::vector` for cache efficiency
- Pre-allocation of paths to avoid reallocations

### 2. Compiler Optimizations
- Release builds with `-O3 -march=native`
- Enables SIMD instructions automatically

### 3. Vectorization Opportunities
- Path generation is embarrassingly parallel
- Time loops can be optimized
- RNG can be parallelized with threading

### 4. Benchmarking
- Use `benchmark_simulation` for wall-clock metrics
- Measure with 100K+ paths for stable results

### 5. Kernel-Level Profiling
- eBPF uprobes via bpftrace for per-function latency histograms
- `perf stat` for hardware counters (cache misses, IPC, branch mispredicts)
- Flamegraphs for call stack visualization
- See [profiling.md](profiling.md) for full setup and workflow

## Bottlenecks
- Random number generation (can use GPU)
- Memory allocation (consider pre-allocation pools)
- Exp/log functions (use fast approximations if needed)

## Future Improvements
- OpenMP for parallelization
- GPU acceleration with CUDA
- SIMD optimizations
- Better RNG implementations
