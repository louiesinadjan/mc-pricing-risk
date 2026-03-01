#include "mc/config_loader.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <set>

namespace mc {

void validateModelName(const std::string& model_name) {
    static const std::set<std::string> valid_models = {"GBM", "Heston"};
    
    if (valid_models.find(model_name) == valid_models.end()) {
        throw std::invalid_argument("Invalid model name: '" + model_name + 
                                    "'. Valid models are: GBM, Heston");
    }
}

void validatePayoffName(const std::string& payoff_name) {
    static const std::set<std::string> valid_payoffs = {"EuropeanCall", "EuropeanPut", "AsianCall", "AsianPut"};
    
    if (valid_payoffs.find(payoff_name) == valid_payoffs.end()) {
        throw std::invalid_argument("Invalid payoff name: '" + payoff_name + 
                                    "'. Valid payoffs are: EuropeanCall, EuropeanPut, AsianCall, AsianPut");
    }
}

void validateRngName(const std::string& rng_name) {
    static const std::set<std::string> valid_rngs = {"StdNormal", "Sobol"};
    
    if (valid_rngs.find(rng_name) == valid_rngs.end()) {
        throw std::invalid_argument("Invalid RNG name: '" + rng_name + 
                                    "'. Valid RNGs are: StdNormal, Sobol");
    }
}

void validateNumericalParameters(const Configuration& cfg) {
    if (cfg.num_paths == 0) {
        throw std::invalid_argument("num_paths must be greater than 0");
    }
    
    if (cfg.num_timesteps == 0) {
        throw std::invalid_argument("num_timesteps must be greater than 0");
    }
    
    if (cfg.spot <= 0.0) {
        throw std::invalid_argument("spot price must be positive");
    }
    
    if (cfg.volatility < 0.0) {
        throw std::invalid_argument("volatility must be non-negative");
    }
    
    if (cfg.maturity <= 0.0) {
        throw std::invalid_argument("maturity must be positive");
    }
    
    if (cfg.risk_free_rate < 0.0) {
        throw std::invalid_argument("risk-free rate must be non-negative");
    }
    
    if (cfg.strike <= 0.0) {
        throw std::invalid_argument("strike price must be positive");
    }
}

void validateConfiguration(const Configuration& config) {
    validateModelName(config.model_name);
    validatePayoffName(config.payoff_name);
    validateRngName(config.rng_name);
    validateNumericalParameters(config);
}

Configuration loadConfigFromYaml(const std::string& filepath) {
    YAML::Node config = YAML::LoadFile(filepath);
    
    if (!config["simulation"] || !config["model"] || !config["payoff"] || !config["randomness"]) {
        std::cout<< "Config file missing required sections. Please ensure 'simulation', 'model', 'payoff', and 'randomness' sections are present." << std::endl;

        throw std::runtime_error("Invalid YAML config: missing required sections");
    }
    
    Configuration cfg;
    
    // Simulation parameters
    cfg.num_paths = config["simulation"]["num_paths"].as<std::size_t>();
    cfg.num_timesteps = config["simulation"]["num_steps"].as<std::size_t>();
    
    // Model parameters
    cfg.model_name = config["model"]["type"].as<std::string>();
    cfg.spot = config["model"]["spot_price"].as<double>();
    cfg.drift = config["model"]["risk_free_rate"].as<double>();
    cfg.volatility = config["model"]["volatility"].as<double>();
    cfg.risk_free_rate = config["model"]["risk_free_rate"].as<double>();
    
    // Payoff parameters
    cfg.payoff_name = config["payoff"]["type"].as<std::string>();
    cfg.strike = config["payoff"]["strike"].as<double>();
    cfg.maturity = config["payoff"]["maturity"].as<double>();
    
    // Randomness
    cfg.rng_name = config["randomness"]["type"].as<std::string>();
    
    // Validate configuration
    validateConfiguration(cfg);
    
    return cfg;
}

} // namespace mc
