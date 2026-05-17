# Voltage References

## ADC reference

- **Vref = 5.0 V** (5000 mV) assumed for current calculation.
- 12-bit full scale: **4096** counts (0–4095).

## Shunt

- **R_shunt = 500 Ω**

## Current formula

```
V_shunt = (adc_avg / 4096) × 5.0 V
I_A     = V_shunt / 500 Ω
I_µA    = I_A × 1e6 = adc_avg × 10000 / 4096
```

## Example levels

| adc_avg | V_shunt (mV) | I (µA) | Typical interpretation |
|---------|--------------|--------|-------------------------|
| 82 | ~100 | ~200 | Near NAMUR “on” band (logic uses thresholds, not table) |
| 2048 | ~2500 | ~5000 | Mid-scale (test sanity) |
| 12 | ~15 | ~29 | Open / lead break region |

Threshold constants live in `include/namur_config.h`.

## Calibration note

If the board uses a divider before the ADC or a reference other than 5.0 V, adjust `NAMUR_VREF_MV` and/or add a gain factor in BSP — document any change in `DECISION_LOG.md`.
