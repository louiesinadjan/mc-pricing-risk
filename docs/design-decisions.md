# Design Decisions

## C++20 Standard
- Modern C++ features for safety and performance
- Strong typing with type aliases
- Structured bindings and concepts ready

## Header-Only Interfaces
- Abstract base classes define contracts
- Implementations in separate compilation units
- Clean separation of concerns

## Smart Pointers
- `std::shared_ptr` for shared ownership
- Automatic memory management
- No manual `delete` calls

## Standard Library Algorithms
- `std::accumulate` for aggregation
- `std::max_element` for extrema
- `<random>` for random number generation

## CMake Build System
- Modern CMake 3.15+
- Modular configuration files
- Separate test and benchmark targets

## Numerical Methods
- GBM uses log-normal discretization
- Heston uses simplified Feller process
- First-order Euler scheme for simplicity
