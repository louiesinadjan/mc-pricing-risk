#include "mc/simulation/simulation.hpp"
#include "mc/config_loader.hpp"
#include "mc/factory.hpp"

#include <iostream>


int main(int argc, char* argv[]) {
    std::string config_path = (argc > 1) ? argv[1] : "./config/config.yaml";
    
    mc::Configuration config;
    try {
        config = mc::loadConfigFromYaml(config_path);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load config: " << e.what() << std::endl;
        return 1;
    }

    auto model  = mc::factory::createModel(config);
    auto rng    = mc::factory::createRng(config);
    auto payoff = mc::factory::createPayoff(config);

    mc::simulation::Simulation simulation(
        config,
        std::move(model),
        std::move(rng),
        std::move(payoff)
    );

    const double price = simulation.run();

    std::cout << "Monte Carlo Results:\n"
              << "  Model: " << config.model_name << "\n"
              << "  Payoff: " << config.payoff_name << "\n"
              << "  RNG: " << config.rng_name << "\n"
              << "  Price: " << price << std::endl;

    return 0;
}

