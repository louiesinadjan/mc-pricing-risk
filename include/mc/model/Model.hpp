#pragma once

#include "mc/core/Path.hpp"

namespace mc::model {

class Model {
public:
    virtual ~Model() = default;

    virtual void generate_path(mc::core::Path& path, double maturity) const = 0;
};

} // namespace mc::model