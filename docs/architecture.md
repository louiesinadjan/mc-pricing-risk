# Monte Carlo Pricing - Architecture

## Overview

This Monte Carlo pricing engine is designed for computing European and Asian option prices using various stochastic models.

## Core Components

### Model Layer (`mc/model/`)
- **Model**: Abstract base class for all stochastic models
- **GBM**: Geometric Brownian Motion implementation
- **Heston**: Stochastic volatility model

### Randomness Layer (`mc/randomness/`)
- **Rng**: Abstract random number generator interface
- **StdNormalRng**: Standard normal RNG using MT19937
- **SobolRng**: Low-discrepancy Sobol sequence generator

### Payoff Layer (`mc/payoff/`)
- **Payoff**: Abstract payoff interface
- **EuropeanCall**: European call option payoff
- **AsianCall**: Asian call option payoff

### Risk Analytics (`mc/risk/`)
- **Aggregator**: Collects and analyzes simulation results
- **Statistics**: Computes mean, variance, std dev, min, max
- **VaR**: Value at Risk and Conditional VaR calculations

### Core Utilities (`mc/core/`)
- **Types**: Common type definitions
- **TimeGrid**: Time discretization
- **Path**: Price path representation
- **State**: State representation

## Design Patterns

1. **Strategy Pattern**: Models, RNGs, and Payoffs are pluggable
2. **Composite Pattern**: Simulation aggregates all components
3. **Template Method**: Base classes define interfaces
