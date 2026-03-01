#include "mc/simulation/simulation.hpp"
#include "mc/model/gbm.hpp"
#include "mc/randomness/stdnormalrng.hpp"
#include "mc/payoff/european.hpp"
#include "mc/config_loader.hpp"

#include <memory>
#include <iostream>


int main(int argc, char* argv[]) {
    // Load configuration from YAML
    std::string config_path = (argc > 1) ? argv[1] : "./config/config.yaml";
    
    mc::Configuration config;
    try {
        config = mc::loadConfigFromYaml(config_path);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load config: " << e.what() << std::endl;
        return 1;
    }

    // Extract config values for model construction
    auto model  = std::make_unique<mc::model::GBM>(config.spot, config.drift, config.volatility, config.num_timesteps);
    auto rng    = std::make_unique<mc::randomness::StdNormalRng>();
    auto payoff = std::make_unique<mc::payoff::EuropeanCall>(config.strike);

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

