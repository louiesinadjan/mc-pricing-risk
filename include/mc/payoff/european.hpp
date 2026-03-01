#pragma once

#include "mc/payoff/payoff.hpp"

namespace mc::payoff {

class EuropeanCall : public Payoff {
public:
    explicit EuropeanCall(double strike);
    double evaluate(const mc::core::Path& path) const override;

private:
    double strike_;
};

class EuropeanPut : public Payoff {
public:
    explicit EuropeanPut(double strike);
    double evaluate(const mc::core::Path& path) const override;

private:
    double strike_;
};
} // namespace mc::payoff