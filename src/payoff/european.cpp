#include "mc/payoff/european.hpp"
#include <algorithm>

namespace mc::payoff {

EuropeanOption::EuropeanOption(double strike, OptionType type) : strike_(strike), type_(type) {}

double EuropeanOption::evaluate(const mc::core::Path& path) const {
    const double ST = path[path.size() - 1];

    if (type_ == OptionType::Call) {
        return std::max(ST - strike_, 0.0);
    } else { 
        return std::max(strike_ - ST, 0.0);
    }
}

} // namespace mc::payoff