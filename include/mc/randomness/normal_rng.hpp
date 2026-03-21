#pragma once

#include "rng.hpp"

namespace mc::randomness {

// Adapter: owns an Engine and a Distribution, satisfying the Rng interface.
// Decouples the simulation from any specific engine/distribution pairing.
//
// Engine must provide:       double nextUniform()
// Distribution must provide: double next()  (constructed with Engine&)
//
// Example:
//   auto rng = std::make_unique<NormalRng<MersenneTwisterRng, BoxMullerNormal>>(seed);
template <typename Engine, template <typename> typename Distribution>
class NormalRng : public Rng {
public:
    using seed_type = typename Engine::seed_type;

    explicit NormalRng(seed_type seed) : engine_(seed), dist_(engine_) {}

    double nextNormal() override { return dist_.next(); }

private:
    Engine engine_;
    Distribution<Engine> dist_;
};

} // namespace mc::randomness
