#pragma once

#include "mc/model/model.hpp"

namespace mc::model {

class Heston : public Model {
public:
    Heston(double spot, double drift, double v0,
           double kappa, double theta, double sigma,
           double rho, std::size_t steps);

    void generate_path(mc::core::Path& path, double maturity,
                       mc::randomness::Rng& rng) const override;

private:
    double spot_, drift_, v0_, kappa_, theta_, sigma_, rho_;
    double sqrt_one_minus_rho_sq_;
    std::size_t steps_;
};

} // namespace mc::model
