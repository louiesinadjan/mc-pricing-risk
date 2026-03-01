#include "mc/factory.hpp"
#include "mc/model/gbm.hpp"
#include "mc/randomness/stdnormalrng.hpp"
#include "mc/payoff/european.hpp"

#include <stdexcept>
#include <map>
#include <functional>

namespace mc::factory {

// Model factory
using ModelCreator = std::function<std::unique_ptr<mc::model::Model>(const Configuration&)>;

static const std::map<std::string, ModelCreator> model_creators = {
    {"GBM", [](const Configuration& cfg) {
        return std::make_unique<mc::model::GBM>(
            cfg.spot, 
            cfg.drift, 
            cfg.volatility, 
            cfg.num_timesteps
        );
    }}
    // Add more models here:
    // {"Heston", [](const Configuration& cfg) {
    //     return std::make_unique<mc::model::Heston>(...);
    // }}
};

std::unique_ptr<mc::model::Model> createModel(const Configuration& config) {
    auto it = model_creators.find(config.model_name);
    if (it == model_creators.end()) {
        throw std::invalid_argument("Unknown model type: " + config.model_name);
    }
    return it->second(config);
}

// RNG factory
using RngCreator = std::function<std::unique_ptr<mc::randomness::Rng>(const Configuration&)>;

static const std::map<std::string, RngCreator> rng_creators = {
    {"StdNormal", [](const Configuration& /*cfg*/) {
        return std::make_unique<mc::randomness::StdNormalRng>();
    }}
    // Add more RNGs here:
    // {"Sobol", [](const Configuration& cfg) {
    //     return std::make_unique<mc::randomness::SobolRng>();
    // }}
};

std::unique_ptr<mc::randomness::Rng> createRng(const Configuration& config) {
    auto it = rng_creators.find(config.rng_name);
    if (it == rng_creators.end()) {
        throw std::invalid_argument("Unknown RNG type: " + config.rng_name);
    }
    return it->second(config);
}

// Payoff factory
using PayoffCreator = std::function<std::unique_ptr<mc::payoff::Payoff>(const Configuration&)>;

static const std::map<std::string, PayoffCreator> payoff_creators = {
    {"EuropeanCall", [](const Configuration& cfg) {
        return std::make_unique<mc::payoff::EuropeanCall>(cfg.strike);
    }},
    {"EuropeanPut", [](const Configuration& cfg) {
        return std::make_unique<mc::payoff::EuropeanPut>(cfg.strike);
    }}
    // Add more payoffs here:
    // {"AsianCall", [](const Configuration& cfg) {
    //     return std::make_unique<mc::payoff::AsianCall>(cfg.strike);
    // }},
    // {"AsianPut", [](const Configuration& cfg) {
    //     return std::make_unique<mc::payoff::AsianPut>(cfg.strike);
    // }}
};

std::unique_ptr<mc::payoff::Payoff> createPayoff(const Configuration& config) {
    auto it = payoff_creators.find(config.payoff_name);
    if (it == payoff_creators.end()) {
        throw std::invalid_argument("Unknown payoff type: " + config.payoff_name);
    }
    return it->second(config);
}

} // namespace mc::factory
