#pragma once

#include <vector>

namespace mc::core {

struct Path {
    std::vector<double> spot; // S0, S1, ..., ST
};

} // namespace mc::core