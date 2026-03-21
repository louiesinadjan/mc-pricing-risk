#pragma once

#include <random>

namespace mc::randomness {

// Mersenne Twister uniform engine. Produces U(0,1) samples.
// Intended to be paired with a distribution (e.g. BoxMullerNormal) rather than
// used directly as a Rng — see BoxMullerRng<MersenneTwisterRng> in box_muller.hpp.
class MersenneTwisterRng {
public:
    using seed_type = std::mt19937::result_type;

    explicit MersenneTwisterRng(seed_type seed = std::random_device{}());

    double nextUniform();

private:
    std::mt19937 engine_;
    std::uniform_real_distribution<double> uniform_;
};

} // namespace mc::randomness
