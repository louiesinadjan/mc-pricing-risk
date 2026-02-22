#pragma once

#include "mc/core/path.hpp"

namespace mc::payoff {

class Payoff {
public:
    virtual ~Payoff() = default;

    virtual double evaluate(const mc::core::Path& path) const = 0;
};

} // namespace mc::payoff