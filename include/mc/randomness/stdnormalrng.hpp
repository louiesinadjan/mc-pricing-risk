#pragma once

#include "rng.hpp"
#include <random>

namespace mc::randomness {

class StdNormalRng : public Rng {
public:
    explicit StdNormalRng(std::mt19937::result_type seed = std::random_device{}());
    ~StdNormalRng() override;

    double nextNormal() override;

private:
    std::mt19937 engine_;
    std::normal_distribution<double> distribution_;
};

} // namespace mc::randomness