#pragma once

#include "mc/payoff/payoff.hpp"

namespace mc::payoff {

enum class AverageType { Arithmetic, Geometric };

class AsianOption : public Payoff {
public:
    AsianOption(double strike, AverageType avg_type, OptionType opt_type);

    double evaluate(const mc::core::Path& path) const override;

private:
    double strike_;
    AverageType avg_type_;
    OptionType opt_type_;
};

} // namespace mc::payoff