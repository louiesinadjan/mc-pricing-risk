#pragma once

#include "mc/core/path.hpp"
#include "mc/model/model.hpp"
#include "mc/payoff/payoff.hpp"
#include "mc/randomness/rng.hpp"
#include "mc/simulation/configuration.hpp"

#include <memory>

namespace mc::simulation {

class Simulation {
public:
    Simulation(Configuration config,
               std::unique_ptr<mc::model::Model> model,
               std::unique_ptr<mc::randomness::Rng> rng,
               std::unique_ptr<mc::payoff::Payoff> payoff);

    double run();

private:
    Configuration config_;
    std::unique_ptr<mc::model::Model> model_;
    std::unique_ptr<mc::randomness::Rng> rng_;
    std::unique_ptr<mc::payoff::Payoff> payoff_;
};

} // namespace mc::simulation