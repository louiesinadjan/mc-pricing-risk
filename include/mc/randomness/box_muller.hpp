#pragma once

#include <cmath>
#include <numbers>
#include <optional>

namespace mc::randomness {

// Box-Muller transform: converts pairs of U(0,1) samples into N(0,1) variates.
// Engine must provide: double nextUniform()
template <typename Engine>
class BoxMullerNormal {
public:
    explicit BoxMullerNormal(Engine& engine) : engine_(engine) {}

    double next() {
        if (spare_) {
            double val = *spare_;
            spare_.reset();
            return val;
        }

        double u1, u2;
        do { u1 = engine_.nextUniform(); } while (u1 == 0.0); // avoid log(0)
        u2 = engine_.nextUniform();

        double mag = std::sqrt(-2.0 * std::log(u1));
        double angle = 2.0 * std::numbers::pi * u2;

        spare_ = mag * std::sin(angle);
        return mag * std::cos(angle);
    }

private:
    Engine& engine_;
    std::optional<double> spare_;
};

} // namespace mc::randomness
