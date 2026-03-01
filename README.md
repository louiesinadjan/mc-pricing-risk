# Monte Carlo Pricing & Risk

High-performance Monte Carlo simulations for options pricing and risk analysis.

## Prerequisites

```bash
brew install yaml-cpp cmake
```

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Usage

Run the Monte Carlo simulation with a configuration file:

```bash
./build/mc-pricing ./config/config.yaml
```

Or with a custom config path:

```bash
./build/mc-pricing ./path/to/custom/config.yaml
```

## Configuration

Edit `config/config.yaml` to customize the simulation:

```yaml
simulation:
  num_paths: 100000      # Number of Monte Carlo paths
  num_steps: 252         # Time steps per path

model:
  type: "GBM"            # Options: GBM, Heston
  spot_price: 100.0
  risk_free_rate: 0.05
  volatility: 0.2

payoff:
  type: "EuropeanCall"   # Options: EuropeanCall, EuropeanPut, AsianCall, AsianPut
  strike: 100.0
  maturity: 1.0

randomness:
  type: "StdNormal"      # Options: StdNormal, Sobol
```

## Supported Components

- **Models**: GBM, Heston
- **Payoffs**: EuropeanCall, EuropeanPut, AsianCall, AsianPut  
- **RNG**: StdNormal, Sobol

## Validation

Configuration is automatically validated for:
- Valid component names
- Positive spot price, strike, maturity
- Non-negative volatility and risk-free rate
- Positive path and timestep counts