#pragma once

#include "rng.hpp"
#include <random>

namespace mc::randomness {

class StdNormalRng : public Rng {
public:
    explicit StdNormalRng(std::mt19937::result_type seed = std::random_device{}())
        : engine_(seed), distribution_(0.0, 1.0) {}

    ~StdNormalRng() override = default;

    double nextNormal() override { return distribution_(engine_); }

private:
    std::mt19937 engine_;
    std::normal_distribution<double> distribution_;
};

} // namespace mc::randomness