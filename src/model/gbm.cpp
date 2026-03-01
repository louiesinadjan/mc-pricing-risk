#include "mc/model/gbm.hpp"
#include <cmath>

namespace mc::model {

GBM::GBM(double spot, double drift, double vol, std::size_t steps)
    : spot_(spot), drift_(drift), vol_(vol), steps_(steps) {}

void GBM::generate_path(mc::core::Path& path, double maturity, mc::randomness::Rng& rng) const {
    path.resize(steps_ + 1);

    const double dt = maturity / static_cast<double>(steps_);
    const double sqrt_dt = std::sqrt(dt);

    path[0] = spot_;

    for (std::size_t i = 1; i <= steps_; ++i) {
        const double z = rng.nextNormal();

        const double prev = path[i - 1];

        const double drift_term = (drift_ - 0.5 * vol_ * vol_) * dt;
        const double diffusion_term = vol_ * sqrt_dt * z;

        path[i] = prev * std::exp(drift_term + diffusion_term);
    }
}

} // namespace mc::model