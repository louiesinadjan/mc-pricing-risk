#pragma once

#include <vector>

namespace mc::core {

struct Path {
    std::vector<double> spot;

    void resize(std::size_t n) { spot.resize(n); }
    double& operator[](std::size_t i) { return spot[i]; }
    const double& operator[](std::size_t i) const { return spot[i]; }
    std::size_t size() const { return spot.size(); }
};

} // namespace mc::core