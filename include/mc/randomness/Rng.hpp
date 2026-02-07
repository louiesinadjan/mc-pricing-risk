#pragma once

namespace mc::randomness {

class Rng {
public:
    virtual ~Rng() = default;

    // Standard normal N(0,1)
    virtual double nextNormal() = 0;
};

} // namespace mc::randomness