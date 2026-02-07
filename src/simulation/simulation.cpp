#include "mc/simulation/simulation.hpp"

namespace mc::simulation {

Simulation::Simulation(Configuration config,
                       std::unique_ptr<mc::model::Model> model,
                       std::unique_ptr<mc::randomness::Rng> rng,
                       std::unique_ptr<mc::payoff::Payoff> payoff)
    : config_(std::move(config)), model_(std::move(model)), rng_(std::move(rng)),
      payoff_(std::move(payoff)) {}

double Simulation::run() {
    const std::size_t N = config_.num_paths;
    const double T = config_.maturity;
    const double r = config_.risk_free_rate;

    double sum = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        mc::core::Path path;
        model_->generate_path(path, T, *rng_);
        const double payoff = payoff_->evaluate(path);
        sum += payoff;
    }

    const double mean = sum / static_cast<double>(N);

    return std::exp(-r * T) * mean;
}

} // namespace mc::simulation