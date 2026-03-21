#include "mc/randomness/mt19937.hpp"

namespace mc::randomness {

MersenneTwisterRng::MersenneTwisterRng(seed_type seed)
    : engine_(seed), uniform_(0.0, 1.0) {}

MersenneTwisterRng::~MersenneTwisterRng() = default;

double MersenneTwisterRng::nextUniform() {
    return uniform_(engine_);
}

} // namespace mc::randomness
