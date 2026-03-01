#include "mc/randomness/stdnormalrng.hpp"

namespace mc::randomness {

StdNormalRng::StdNormalRng(std::mt19937::result_type seed) : engine_(seed), distribution_(0.0, 1.0) {}

StdNormalRng::~StdNormalRng() = default;

double StdNormalRng::nextNormal() { return distribution_(engine_); }
} // namespace mc::randomness