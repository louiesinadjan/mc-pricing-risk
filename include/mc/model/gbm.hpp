#pragma once

#include "mc/model/model.hpp"

namespace mc::model {

class GBM : public Model {
public:
    GBM(double spot, double drift, double vol, std::size_t steps);

    void generate_path(mc::core::Path& path, double maturity, mc::randomness::Rng& rng) const override;

private:
    double spot_;
    double drift_;
    double vol_;
    std::size_t steps_;
};

} // namespace mc::model