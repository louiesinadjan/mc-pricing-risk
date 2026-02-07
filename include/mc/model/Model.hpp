#pragma once

#include "mc/core/path.hpp"
#include "mc/randomness/rng.hpp"

namespace mc::model {

class Model {
public:
    virtual ~Model() = default;

    virtual void generate_path(mc::core::   Path& path, double maturity, mc::randomness::Rng& rng) const = 0;
};

} // namespace mc::model