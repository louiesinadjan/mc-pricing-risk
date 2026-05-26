#include "mc/model/heston.hpp"
#include <cmath>

namespace mc::model {

Heston::Heston(double spot, double drift, double v0,
               double kappa, double theta, double sigma,
               double rho, std::size_t steps)
    : spot_(spot), drift_(drift), v0_(v0),
      kappa_(kappa), theta_(theta), sigma_(sigma),
      rho_(rho),
      sqrt_one_minus_rho_sq_(std::sqrt(1.0 - rho * rho)),
      steps_(steps) {}

void Heston::generate_path(mc::core::Path& path, double maturity,
                           mc::randomness::Rng& rng) const {
    path.resize(steps_ + 1);
    const double dt = maturity / static_cast<double>(steps_);
    const double sqrt_dt = std::sqrt(dt);

    path[0] = spot_;
    double S = spot_;
    double v = v0_;

    for (std::size_t i = 1; i <= steps_; ++i) {
        const double z1 = rng.nextNormal();
        const double z2 = rng.nextNormal();

        const double v_eff  = std::max(v, 0.0);
        const double sqrt_v = std::sqrt(v_eff);

        S += drift_ * S * dt + sqrt_v * S * sqrt_dt * z1;
        v += kappa_ * (theta_ - v_eff) * dt
           + sigma_ * sqrt_v * sqrt_dt
             * (rho_ * z1 + sqrt_one_minus_rho_sq_ * z2);
        v = std::max(v, 0.0);

        path[i] = S;
    }
}

} // namespace mc::model
