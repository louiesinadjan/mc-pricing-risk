#include <algorithm>
#include "mc/payoff/european.hpp"

namespace mc::payoff {

    EuropeanCall::EuropeanCall(double strike) : strike_(strike) {}
    EuropeanPut::EuropeanPut(double strike) : strike_(strike) {}

    double EuropeanCall::evaluate(const mc::core::Path& path) const {
        const double ST = path[path.size() - 1];
        return std::max(ST - strike_, 0.0);
    }

    double EuropeanPut::evaluate(const mc::core::Path& path) const {
        const double ST = path[path.size() - 1];
        return std::max(strike_ - ST, 0.0);
    }
    
}