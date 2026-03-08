#include "mc/config_loader.hpp"
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace mc {

void validateModelName(const std::string& model_name) {
    static const std::set<std::string> valid_models = {"GBM", "Heston"};

    if (valid_models.find(model_name) == valid_models.end()) {
        throw std::invalid_argument("Invalid model name: '" + model_name +
                                    "'. Valid models are: GBM, Heston");
    }
}

void validatePayoffName(const std::string& payoff_name) {
    static const std::set<std::string> valid_payoffs = {"EuropeanCall",
                                                        "EuropeanPut",
                                                        "AsianArithmeticCall",
                                                        "AsianArithmeticPut",
                                                        "AsianGeometricCall",
                                                        "AsianGeometricPut"};

    if (valid_payoffs.find(payoff_name) == valid_payoffs.end()) {
        throw std::invalid_argument("Invalid payoff name: '" + payoff_name +
                                    "'. Valid payoffs are: EuropeanCall, EuropeanPut, "
                                    "AsianArithmeticCall, AsianArithmeticPut, "
                                    "AsianGeometricCall, AsianGeometricPut");
    }
}

void validateRngName(const std::string& rng_name) {
    static const std::set<std::string> valid_rngs = {"StdNormal", "Sobol"};

    if (valid_rngs.find(rng_name) == valid_rngs.end()) {
        throw std::invalid_argument("Invalid RNG name: '" + rng_name + "'. Valid RNGs are: StdNormal, Sobol");
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

    if (!config["market"] || !config["contract"] || !config["model"] || !config["simulation"] ||
        !config["rng"]) {
        throw std::runtime_error(
            "Invalid YAML config: missing required sections (market, contract, model, simulation, rng)");
    }

    Configuration cfg;

    // Market parameters
    cfg.spot = config["market"]["spot"].as<double>();
    cfg.risk_free_rate = config["market"]["rate"].as<double>();

    // Simulation parameters
    cfg.num_paths = config["simulation"]["paths"].as<std::size_t>();
    cfg.num_timesteps = config["simulation"]["steps"].as<std::size_t>();
    if (config["simulation"]["seed"]) {
        cfg.seed = config["simulation"]["seed"].as<std::uint32_t>();
    }

    // Contract parameters
    cfg.strike = config["contract"]["strike"].as<double>();
    cfg.maturity = config["contract"]["maturity"].as<double>();

    std::string contract_type = config["contract"]["type"].as<std::string>();

    // Model parameters
    std::string model_type = config["model"]["type"].as<std::string>();

    // RNG parameters
    std::string rng_type = config["rng"]["type"].as<std::string>();

    // Helper lambda for mapping and validating
    auto map_name = [](const std::string& key,
                       const std::unordered_map<std::string, std::string>& table,
                       const std::string& err_msg) -> std::string {
        auto it = table.find(key);
        if (it == table.end())
            throw std::invalid_argument(err_msg + key);
        return it->second;
    };

    // Contract → payoff mapping
    static const std::unordered_map<std::string, std::string> contract_to_payoff = {
        {"european_call", "EuropeanCall"},
        {"european_put", "EuropeanPut"},
        {"asian_arithmetic_call", "AsianArithmeticCall"},
        {"asian_arithmetic_put", "AsianArithmeticPut"},
        {"asian_geometric_call", "AsianGeometricCall"},
        {"asian_geometric_put", "AsianGeometricPut"}};
    cfg.payoff_name = map_name(contract_type, contract_to_payoff, "Unknown contract type: ");

    // Model → model name mapping
    static const std::unordered_map<std::string, std::string> model_to_name = {{"gbm", "GBM"},
                                                                               {"heston", "Heston"}};
    cfg.model_name = map_name(model_type, model_to_name, "Unknown model type: ");

    // RNG → rng name mapping
    static const std::unordered_map<std::string, std::string> rng_to_name = {{"std_normal", "StdNormal"},
                                                                             {"sobol", "Sobol"}};
    cfg.rng_name = map_name(rng_type, rng_to_name, "Unknown RNG type: ");

    // Only read relevant model parameter block
    if (cfg.model_name == "GBM") {
        cfg.volatility = config["model"]["gbm"]["volatility"].as<double>();
        cfg.drift = cfg.risk_free_rate; // risk-neutral drift
    } else if (cfg.model_name == "Heston") {
        cfg.v0 = config["model"]["heston"]["v0"].as<double>();
        cfg.kappa = config["model"]["heston"]["kappa"].as<double>();
        cfg.theta = config["model"]["heston"]["theta"].as<double>();
        cfg.sigma = config["model"]["heston"]["sigma"].as<double>();
        cfg.rho = config["model"]["heston"]["rho"].as<double>();
        cfg.drift = cfg.risk_free_rate;
    }

    // Output options (optional)
    if (config["output"]) {
        cfg.print_price = config["output"]["print_price"].as<bool>(cfg.print_price);
        cfg.print_runtime = config["output"]["print_runtime"].as<bool>(cfg.print_runtime);
        cfg.print_paths_per_second =
            config["output"]["print_paths_per_second"].as<bool>(cfg.print_paths_per_second);
    }

    validateConfiguration(cfg);
    return cfg;
}

} // namespace mc
