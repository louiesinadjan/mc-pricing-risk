#include "mc/randomness/sobol.hpp"
#include <algorithm>
#include <bit>

namespace mc::randomness {

SobolEngine::SobolEngine(seed_type scramble) : scramble_(scramble) {
    for (std::size_t i = 0; i < 32; ++i) {
        direction_[i] = 1u << (31u - static_cast<unsigned>(i));
    }
}

double SobolEngine::nextUniform() {
    // Rightmost 0-bit position of index_ = number of trailing 1-bits.
    // Capped at 31 so direction_[] access stays in bounds.
    const auto c = static_cast<std::size_t>(std::min(std::countr_one(index_), 31));
    x_ ^= direction_[c];
    ++index_;
    return (x_ ^ scramble_) * (1.0 / 4294967296.0); // 2^32
}

} // namespace mc::randomness
