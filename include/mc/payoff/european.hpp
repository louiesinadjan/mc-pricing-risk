#pragma once

#include "mc/payoff/payoff.hpp"

namespace mc::payoff {

class EuropeanOption : public Payoff {
public:
    EuropeanOption(double strike, OptionType type);

    double evaluate(const mc::core::Path& path) const override;

private:
    double strike_;
    OptionType type_;
};

} // namespace mc::payoff