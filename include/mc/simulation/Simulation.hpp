#pragma once

#include "mc/core/Path.hpp"
#include "mc/model/Model.hpp"
#include "mc/payoff/Payoff.hpp"
#include "mc/randomness/Rng.hpp"
#include "mc/simulation/Configuration.hpp"

#include <memory>

namespace mc::simulation {

class Simulation {
public:
    Simulation(Configuration config, std::unique_ptr<mc::model::Model> model, std::unique_ptr<mc::randomness::Rng> rng, std::unique_ptr<mc::payoff::Payoff> payoff);

    double run() const;

private:
    Configuration config_;
    std::unique_ptr<mc::model::Model> model_;
    std::unique_ptr<mc::randomness::Rng> rng_;
    std::unique_ptr<mc::payoff::Payoff> payoff_;
};

} // namespace mc::simulation