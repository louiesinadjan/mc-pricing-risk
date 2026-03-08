#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace mc {

struct Configuration {
    // Simulation parameters
    std::size_t num_paths;
    std::size_t num_timesteps;
    double maturity;
    double risk_free_rate;
    std::uint32_t seed = 0; // 0 means random seed from device

    // Model parameters (GBM)
    double spot;       // Initial spot price
    double drift;      // Not used in risk-neutral pricing, but can be set to risk-free rate
    double volatility; // Volatility of the underlying asset

    // Model parameters (Heston)
    double v0 = 0.0;    // Initial variance
    double kappa = 0.0; // Mean reversion speed
    double theta = 0.0; // Long-term variance
    double sigma = 0.0; // Volatility of variance
    double rho = 0.0;   // Correlation between spot and variance

    // Payoff parameters
    double strike;

    // Component names
    std::string model_name;
    std::string payoff_name;
    std::string rng_name;

    // Output options
    bool print_price = true;
    bool print_runtime = false;
    bool print_paths_per_second = false;
};

} // namespace mc