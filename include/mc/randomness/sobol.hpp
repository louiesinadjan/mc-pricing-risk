#pragma once

#include <array>
#include <cstdint>

namespace mc::randomness {

// 1D Sobol sequence using Gray-code increments (Van der Corput base 2).
// seed_type used as XOR scramble; seed 0 = unscrambled deterministic sequence.
class SobolEngine {
public:
    using seed_type = uint32_t;

    explicit SobolEngine(seed_type scramble = 0);

    double nextUniform();

private:
    uint32_t x_ = 0;
    uint32_t index_ = 0;
    uint32_t scramble_;
    std::array<uint32_t, 32> direction_;
};

} // namespace mc::randomness
