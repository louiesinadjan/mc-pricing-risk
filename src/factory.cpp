#include "mc/factory.hpp"
#include "mc/model/gbm.hpp"
#include "mc/payoff/asian.hpp"
#include "mc/payoff/european.hpp"
#include "mc/randomness/box_muller.hpp"
#include "mc/randomness/mt19937.hpp"
#include "mc/randomness/normal_rng.hpp"
#include "mc/randomness/stdnormalrng.hpp"
#include "mc/simulation/configuration.hpp"

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

namespace mc::factory {

// Model factory
using ModelCreator = std::function<std::unique_ptr<mc::model::Model>(const Configuration&)>;

static const std::map<std::string, ModelCreator> model_creators = {
    {"GBM",
     [](const Configuration& cfg) {
         return std::make_unique<mc::model::GBM>(cfg.spot, cfg.drift, cfg.volatility, cfg.num_timesteps);
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
    {"StdNormal",
     [](const Configuration& cfg) {
         if (cfg.seed == 0) {
             return std::make_unique<mc::randomness::StdNormalRng>();
         } else {
             return std::make_unique<mc::randomness::StdNormalRng>(cfg.seed);
         }
     }},
    {"mt19937",
     [](const Configuration& cfg) -> std::unique_ptr<mc::randomness::Rng> {
         auto seed = cfg.seed == 0
             ? static_cast<mc::randomness::MersenneTwisterRng::seed_type>(std::random_device{}())
             : static_cast<mc::randomness::MersenneTwisterRng::seed_type>(cfg.seed);

         if (cfg.distribution_name == "BoxMuller") {
             return std::make_unique<mc::randomness::NormalRng<mc::randomness::MersenneTwisterRng,
                                                               mc::randomness::BoxMullerNormal>>(seed);
         }
         throw std::invalid_argument("Unsupported distribution for mt19937: " + cfg.distribution_name);
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
    {"EuropeanCall",
     [](const Configuration& cfg) {
         return std::make_unique<mc::payoff::EuropeanOption>(cfg.strike, mc::payoff::OptionType::Call);
     }},
    {"EuropeanPut",
     [](const Configuration& cfg) {
         return std::make_unique<mc::payoff::EuropeanOption>(cfg.strike, mc::payoff::OptionType::Put);
     }},
    {"AsianArithmeticCall",
     [](const Configuration& cfg) {
         return std::make_unique<mc::payoff::AsianOption>(
             cfg.strike, mc::payoff::AverageType::Arithmetic, mc::payoff::OptionType::Call);
     }},
    {"AsianArithmeticPut",
     [](const Configuration& cfg) {
         return std::make_unique<mc::payoff::AsianOption>(
             cfg.strike, mc::payoff::AverageType::Arithmetic, mc::payoff::OptionType::Put);
     }},
    {"AsianGeometricCall",
     [](const Configuration& cfg) {
         return std::make_unique<mc::payoff::AsianOption>(
             cfg.strike, mc::payoff::AverageType::Geometric, mc::payoff::OptionType::Call);
     }},
    {"AsianGeometricPut", [](const Configuration& cfg) {
         return std::make_unique<mc::payoff::AsianOption>(
             cfg.strike, mc::payoff::AverageType::Geometric, mc::payoff::OptionType::Put);
     }}};

std::unique_ptr<mc::payoff::Payoff> createPayoff(const Configuration& config) {
    auto it = payoff_creators.find(config.payoff_name);
    if (it == payoff_creators.end()) {
        throw std::invalid_argument("Unknown payoff type: " + config.payoff_name);
    }
    return it->second(config);
}

} // namespace mc::factory
