#pragma once

#include "mc/simulation/configuration.hpp"
#include "mc/model/model.hpp"
#include "mc/randomness/rng.hpp"
#include "mc/payoff/payoff.hpp"

#include <memory>

namespace mc::factory {

std::unique_ptr<mc::model::Model> createModel(const Configuration& config);

std::unique_ptr<mc::randomness::Rng> createRng(const Configuration& config);

std::unique_ptr<mc::payoff::Payoff> createPayoff(const Configuration& config);

} // namespace mc::factory
