#include "mc/factory.hpp"
#include "mc/simulation/configuration.hpp"
#include "mc/simulation/simulation.hpp"

#include <chrono>
#include <iostream>

int main() {
    mc::Configuration config;

    config.num_paths = 10000;
    config.num_timesteps = 252;
    config.model_name = "GBM";
    config.payoff_name = "EuropeanCall";
    config.rng_name = "StdNormal";
    config.spot = 100;
    config.strike = 100;
    config.risk_free_rate = 0.05;
    config.drift = config.risk_free_rate;
    config.volatility = 0.2;
    config.maturity = 1.0;
    config.seed = 42;

    auto model = mc::factory::createModel(config);
    auto rng = mc::factory::createRng(config);
    auto payoff = mc::factory::createPayoff(config);

    mc::simulation::Simulation simulation(config, std::move(model), std::move(rng), std::move(payoff));

    const int runs = 10;
    double total_time = 0;
    simulation.run(); // Warm-up run 
    for (std::size_t i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        simulation.run();
        auto end = std::chrono::high_resolution_clock::now();

        double t = std::chrono::duration<double>(end - start).count();

        total_time += t;
    }
    double avg = total_time / runs;
    double paths_per_second = config.num_paths / avg;
    double ns_per_path = avg * 1e9 / config.num_paths;

    std::cout << "Benchmark Results\n";
    std::cout << "-----------------\n";
    std::cout << "paths: " << config.num_paths << "\n";
    std::cout << "steps: " << config.num_timesteps << "\n";
    std::cout << "avg runtime: " << avg << " s\n";
    std::cout << "paths/sec: " << paths_per_second << "\n";
    std::cout << "ns/path: " << ns_per_path << "\n";
}
