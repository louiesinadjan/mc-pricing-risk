#pragma once

#include "mc/simulation/configuration.hpp"
#include <string>

namespace mc {

Configuration loadConfigFromYaml(const std::string& filepath);

void validateConfiguration(const Configuration& config);
void validateModelName(const std::string& model_name);
void validatePayoffName(const std::string& payoff_name);
void validateRngName(const std::string& rng_name);
void validateNumericalParameters(const Configuration& config);

} // namespace mc
