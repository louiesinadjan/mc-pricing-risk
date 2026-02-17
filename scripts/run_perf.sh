#!/bin/bash

# Run performance benchmarks

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make mc_bench

echo "Running benchmark..."
./benchmarks/mc_bench
