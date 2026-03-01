#include "mc/simulation/simulation.hpp"
#include "mc/model/gbm.hpp"
#include "mc/randomness/stdnormalrng.hpp"
#include "mc/payoff/european.hpp"
#include "mc/simulation/configuration.hpp"

#include <memory>
#include <iostream>


int main() {
    const double spot = 100.0;
    const double drift = 0.05;          // risk-neutral drift (r)
    const double vol = 0.2;
    const std::size_t steps = 252;

    const double strike = 100.0;
    const double maturity = 1.0;        // 1 year
    const double risk_free_rate = 0.05;

    const std::size_t num_paths = 100000;

    auto model  = std::make_unique<mc::model::GBM>(spot, drift, vol, steps);
    auto rng    = std::make_unique<mc::randomness::StdNormalRng>();
    auto payoff = std::make_unique<mc::payoff::EuropeanCall>(strike);

    mc::Configuration config{
        num_paths,
        steps,
        maturity,
        risk_free_rate,
        "GBM",
        "EuropeanCall",
        "StdNormalRng"
    };

    mc::simulation::Simulation simulation(
        config,
        std::move(model),
        std::move(rng),
        std::move(payoff)
    );

    const double price = simulation.run();

    std::cout << "Monte Carlo European Call Price: " << price << std::endl;

    return 0;
}

