#include "mc/payoff/asian.hpp"
#include <algorithm>
#include <numeric>

namespace mc::payoff {

AsianOption::AsianOption(double strike, AverageType avg_type, OptionType opt_type)
    : strike_(strike), avg_type_(avg_type), opt_type_(opt_type) {}

double AsianOption::evaluate(const mc::core::Path& path) const {
    double average_price = 0.0;
    if (avg_type_ == AverageType::Arithmetic) {
        average_price = std::accumulate(path.spot.begin(), path.spot.end(), 0.0) / path.size();
    } else {
        double log_sum = 0.0;
        for (const auto& price : path.spot) {
            log_sum += std::log(price);
        }
        average_price = std::exp(log_sum / path.size());
    }

    if (opt_type_ == OptionType::Call) {
        return std::max(average_price - strike_, 0.0);
    } else {
        return std::max(strike_ - average_price, 0.0);
    }
}

} // namespace mc::payoff