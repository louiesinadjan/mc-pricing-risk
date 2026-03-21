#include "mc/config_loader.hpp"
#include "mc/factory.hpp"
#include "mc/simulation/simulation.hpp"

#include <chrono>
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

    auto model = mc::factory::createModel(config);
    auto rng = mc::factory::createRng(config);
    auto payoff = mc::factory::createPayoff(config);

    mc::simulation::Simulation simulation(config, std::move(model), std::move(rng), std::move(payoff));

    // Time the simulation
    auto start_time = std::chrono::high_resolution_clock::now();
    const double price = simulation.run();
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate runtime metrics
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    double elapsed_seconds = elapsed.count() / 1000.0;
    double paths_per_second = config.num_paths / elapsed_seconds;

    // Print output based on config
    std::cout << "Monte Carlo Results:\n";
    std::cout << "  Model: " << config.model_name << "\n";
    std::cout << "  Payoff: " << config.payoff_name << "\n";
    std::cout << "  RNG: " << config.rng_name
              << (config.distribution_name.empty() ? "" : " + " + config.distribution_name) << "\n";

    if (config.print_price) {
        std::cout << "  Price: " << price << "\n";
    }
    if (config.print_runtime) {
        std::cout << "  Runtime: " << elapsed_seconds << " seconds\n";
    }
    if (config.print_paths_per_second) {
        std::cout << "  Paths/second: " << paths_per_second << "\n";
    }

    std::cout << std::endl;

    return 0;
}
