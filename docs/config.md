# Config Reference

Full reference for YAML config files passed to `./build/mc-pricing <config.yaml>`.

---

## `market`

| Field | Type | Required | Constraints | Description |
|-------|------|----------|-------------|-------------|
| `spot` | float | yes | > 0 | Initial spot price S₀ |
| `rate` | float | yes | ≥ 0 | Risk-free rate r (annualised, continuously compounded) |

---

## `contract`

| Field | Type | Required | Constraints | Description |
|-------|------|----------|-------------|-------------|
| `type` | string | yes | see below | Payoff type |
| `strike` | float | yes | > 0 | Strike price K |
| `maturity` | float | yes | > 0 | Time to expiry T (years) |

**Contract types:**

| `type` | Payoff |
|--------|--------|
| `european_call` | max(S_T − K, 0) |
| `european_put` | max(K − S_T, 0) |
| `asian_arithmetic_call` | max(arithmetic_avg(path) − K, 0) |
| `asian_arithmetic_put` | max(K − arithmetic_avg(path), 0) |
| `asian_geometric_call` | max(geometric_avg(path) − K, 0) |
| `asian_geometric_put` | max(K − geometric_avg(path), 0) |

---

## `model`

| Field | Type | Required | Valid values | Description |
|-------|------|----------|-------------|-------------|
| `type` | string | yes | `gbm`, `heston` | Pricing model |

### GBM

Geometric Brownian Motion: dS = r·S·dt + σ·S·dW

```yaml
model:
  type: gbm
  gbm:
    volatility: 0.20
```

| Field | Type | Required | Constraints | Description |
|-------|------|----------|-------------|-------------|
| `gbm.volatility` | float | yes | ≥ 0 | Constant annualised volatility σ |

### Heston

Stochastic volatility: dS = r·S·dt + √v·S·dW_S, dv = κ(θ−v)dt + σ√v·dW_v, Corr(dW_S, dW_v) = ρ

Discretisation: Euler-Maruyama with full truncation (variance floored to 0 at each step).

```yaml
model:
  type: heston
  heston:
    v0: 0.04
    kappa: 2.0
    theta: 0.04
    sigma: 0.3
    rho: -0.7
```

| Field | Type | Required | Constraints | Description |
|-------|------|----------|-------------|-------------|
| `heston.v0` | float | yes | ≥ 0 | Initial variance v₀ (e.g. 0.04 = 20% vol) |
| `heston.kappa` | float | yes | > 0 | Mean reversion speed κ |
| `heston.theta` | float | yes | ≥ 0 | Long-term variance θ |
| `heston.sigma` | float | yes | ≥ 0 | Volatility of variance ("vol of vol") |
| `heston.rho` | float | yes | [−1, 1] | Spot/variance correlation. Equities typically −0.7 to −0.3 |

> **Feller condition:** `2·κ·θ > σ²` ensures variance stays positive under the continuous SDE. Full truncation keeps the simulation stable regardless, but accuracy degrades for extreme parameters.

---

## `simulation`

| Field | Type | Required | Default | Constraints | Description |
|-------|------|----------|---------|-------------|-------------|
| `paths` | int | yes | — | > 0 | Number of Monte Carlo paths |
| `steps` | int | yes | — | > 0 | Time steps per path (e.g. 252 = daily for 1 year) |
| `seed` | int | no | `0` | ≥ 0 | RNG seed. `0` = random seed from hardware |

---

## `rng`

| Field | Type | Required | Valid values | Description |
|-------|------|----------|-------------|-------------|
| `type` | string | yes | `std_normal`, `mt19937`, `sobol` | RNG type |
| `distribution` | string | required for `mt19937` | `box_muller` | Normal transform (engine-only RNGs) |

### `std_normal`

`std::normal_distribution` backed by `std::default_random_engine`. No sub-fields.

```yaml
rng:
  type: std_normal
```

### `mt19937` + Box-Muller

Mersenne Twister engine with Box-Muller transform. Higher quality than `std_normal`.

```yaml
rng:
  type: mt19937
  distribution: box_muller
```

### `sobol`

1D Gray-code Sobol quasi-random sequence with inverse-CDF normal transform. Low-discrepancy sequence converges faster than pseudo-random (O((log N)/N) vs O(1/√N)). `seed` is used as an XOR scramble — `0` gives the unscrambled deterministic sequence.

```yaml
rng:
  type: sobol
```

> **Note:** Sobol produces one quasi-random normal per draw (1D). Suitable for GBM (one Brownian per step). For Heston (two correlated Brownians per step) use `std_normal` or `mt19937` to preserve the correlation structure.

---

## `output` (optional)

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `print_price` | bool | `true` | Print discounted price estimate |
| `print_runtime` | bool | `false` | Print wall-clock runtime (seconds) |
| `print_paths_per_second` | bool | `false` | Print throughput |

```yaml
output:
  print_price: true
  print_runtime: true
  print_paths_per_second: true
```

---

## Examples

### GBM + European Call + StdNormal

```yaml
market:
  spot: 100.0
  rate: 0.05
contract:
  type: european_call
  strike: 100.0
  maturity: 1.0
model:
  type: gbm
  gbm:
    volatility: 0.20
simulation:
  paths: 100000
  steps: 252
  seed: 42
rng:
  type: std_normal
```

### GBM + Asian Arithmetic Call + mt19937 + Box-Muller

```yaml
market:
  spot: 100.0
  rate: 0.05
contract:
  type: asian_arithmetic_call
  strike: 100.0
  maturity: 1.0
model:
  type: gbm
  gbm:
    volatility: 0.20
simulation:
  paths: 100000
  steps: 252
  seed: 42
rng:
  type: mt19937
  distribution: box_muller
```

### Heston + European Call

```yaml
market:
  spot: 100.0
  rate: 0.05
contract:
  type: european_call
  strike: 100.0
  maturity: 1.0
model:
  type: heston
  heston:
    v0: 0.04
    kappa: 2.0
    theta: 0.04
    sigma: 0.3
    rho: -0.7
simulation:
  paths: 100000
  steps: 252
  seed: 42
rng:
  type: std_normal
output:
  print_price: true
  print_runtime: true
  print_paths_per_second: true
```

### GBM + European Call + Sobol (quasi-Monte Carlo)

```yaml
market:
  spot: 100.0
  rate: 0.05
contract:
  type: european_call
  strike: 100.0
  maturity: 1.0
model:
  type: gbm
  gbm:
    volatility: 0.20
simulation:
  paths: 100000
  steps: 252
  seed: 0
rng:
  type: sobol
```
