#include "mc/model/gbm.hpp"
#include "mc/payoff/asian.hpp"
#include "mc/payoff/european.hpp"
#include "mc/randomness/stdnormalrng.hpp"
#include "mc/simulation/configuration.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace {

double black_scholes_call(double S, double K, double r, double sigma, double T) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    return S * 0.5 * (1 + std::erf(d1 / std::sqrt(2))) -
           K * std::exp(-r * T) * 0.5 * (1 + std::erf(d2 / std::sqrt(2)));
}

struct SampleStats {
    double mean = 0.0;
    double variance = 0.0;
    double std_error = 0.0;
};

template <typename F>
SampleStats sample_stats(std::size_t n, F&& sample_fn) {
    double mean = 0.0;
    double m2 = 0.0;

    for (std::size_t i = 0; i < n; ++i) {
        const double x = sample_fn();
        const double delta = x - mean;
        mean += delta / static_cast<double>(i + 1);
        const double delta2 = x - mean;
        m2 += delta * delta2;
    }

    const double variance = m2 / static_cast<double>(n - 1);
    const double std_error = std::sqrt(variance / static_cast<double>(n));
    return {mean, variance, std_error};
}

} // namespace

TEST(GBMTest, TerminalMeanUsesManyPaths) {
    constexpr double S0 = 100.0;
    constexpr double r = 0.05;
    constexpr double sigma = 0.2;
    constexpr std::size_t steps = 252;
    constexpr std::size_t npaths = 100000;

    mc::model::GBM gbm(S0, r, sigma, steps);
    mc::randomness::StdNormalRng rng(42);

    const auto stats = sample_stats(npaths, [&]() {
        mc::core::Path path;
        gbm.generate_path(path, 1.0, rng);
        return path[path.size() - 1];
    });

    const double expected = S0 * std::exp(r * 1.0);
    const double tolerance = 4.0 * stats.std_error;
    EXPECT_NEAR(stats.mean, expected, tolerance);
}

TEST(SimulationTest, EuropeanCallPrice) {
    mc::Configuration cfg;
    cfg.num_paths = 200000;
    cfg.num_timesteps = 252;
    cfg.spot = 100.0;
    cfg.risk_free_rate = 0.05;
    cfg.strike = 100.0;
    cfg.maturity = 1.0;
    cfg.volatility = 0.2;
    cfg.drift = cfg.risk_free_rate;

    mc::model::GBM model(cfg.spot, cfg.drift, cfg.volatility, cfg.num_timesteps);
    mc::randomness::StdNormalRng rng(7);
    mc::payoff::EuropeanOption payoff(cfg.strike, mc::payoff::OptionType::Call);

    const auto stats = sample_stats(cfg.num_paths, [&]() {
        mc::core::Path path;
        model.generate_path(path, cfg.maturity, rng);
        const double undiscounted = payoff.evaluate(path);
        return std::exp(-cfg.risk_free_rate * cfg.maturity) * undiscounted;
    });

    const double bs_price =
        black_scholes_call(cfg.spot, cfg.strike, cfg.risk_free_rate, cfg.volatility, cfg.maturity);
    const double tolerance = 4.0 * stats.std_error;
    EXPECT_NEAR(stats.mean, bs_price, tolerance);
}

TEST(GBMTest, DiscountedStockIsMartingaleInExpectation) {
    constexpr double S0 = 100.0;
    constexpr double r = 0.05;
    constexpr double sigma = 0.2;
    constexpr std::size_t steps = 252;
    constexpr std::size_t npaths = 100000;

    mc::model::GBM gbm(S0, r, sigma, steps);
    mc::randomness::StdNormalRng rng(123);

    const auto stats = sample_stats(npaths, [&]() {
        mc::core::Path path;
        gbm.generate_path(path, 1.0, rng);
        const double ST = path[path.size() - 1];
        return std::exp(-r * 1.0) * ST;
    });

    const double tolerance = 4.0 * stats.std_error;
    EXPECT_NEAR(stats.mean, S0, tolerance);
}

TEST(PayoffTest, AsianArithmeticCall) {
    mc::core::Path path;
    path.spot = {100, 102, 101, 103};
    double strike = 100;
    mc::payoff::AsianOption payoff(strike, mc::payoff::AverageType::Arithmetic, mc::payoff::OptionType::Call);

    double expected_avg = (100 + 102 + 101 + 103) / 4.0;
    double expected = std::max(expected_avg - strike, 0.0);
    EXPECT_DOUBLE_EQ(payoff.evaluate(path), expected);
}
