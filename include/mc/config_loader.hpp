#pragma once

#include "mc/simulation/configuration.hpp"
#include <string>

namespace mc {

Configuration loadConfigFromYaml(const std::string& filepath);
void validateConfiguration(const Configuration& config);

} // namespace mc
