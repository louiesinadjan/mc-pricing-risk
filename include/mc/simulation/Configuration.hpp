#pragma once

#include <cstddef>
#include <string>

namespace mc {

struct Configuration {
    std::size_t num_paths;
    std::size_t num_timesteps;
    double maturity;
    double risk_free_rate;

    std::string model_name;
    std::string payoff_name;
    std::string rng_name;
};

} // namespace mc