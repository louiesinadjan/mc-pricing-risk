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
- Use `mc_bench` for performance metrics
- Measure with 100K+ paths for stable results

## Bottlenecks
- Random number generation (can use GPU)
- Memory allocation (consider pre-allocation pools)
- Exp/log functions (use fast approximations if needed)

## Future Improvements
- OpenMP for parallelization
- GPU acceleration with CUDA
- SIMD optimizations
- Better RNG implementations
