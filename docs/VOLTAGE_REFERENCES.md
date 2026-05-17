# Voltage References

## ADC and shunt

- **Vref = 5.0 V** (12-bit, 0–4095)
- **R_shunt = 200 Ω** (effective, from measured 0.67 V / 3.36 mA)

## Conversions

```
V_mV = adc × 5000 / 4096
I_µA = adc × 5000 × 1000 / (4096 × 200) = adc × 6104 / 4096
```

## Calibrated operating points

| Condition | V | I |
|-----------|---|---|
| No sensor | 0 V | 0 mA |
| Object detected (sensing) | 0.2 – 0.8 V | 0.3 – 1 mA |
| Connected, not detecting | ~0.67 V | ~3.36 mA |
| Short circuit | ~1.55 V | ~8 mA |

Threshold constants: `include/namur_config.h`
