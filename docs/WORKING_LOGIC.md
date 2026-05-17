# Working Logic

## Overview

Each 20 ms cycle, **each channel is processed alone**:

1. Read 8-sample averaged 12-bit ADC.
2. Convert to mV and µA; classify fault vs normal; update latch.
3. Apply NO/NC DIP (normal only).
4. Drive status LED (off on fault).
5. Drive fault LED (if fault and P1.2 enable).

## Calibrated levels (bench)

| State | Voltage | Current |
|-------|---------|---------|
| No sensor | 0 V | 0 mA |
| Object detected (sensing ON) | 0.2–0.8 V | 0.3–1 mA |
| Connected idle | ~0.67 V | ~3.36 mA |
| Short | ~1.55 V | ~8 mA |

## Thresholds

| Condition | Result |
|-----------|--------|
| V < 80 mV **or** I < 80 µA | `FAULT_LEAD_BREAK` |
| V > 1.3 V **or** I > 7 mA | `FAULT_SHORT_CIRCUIT` |
| I in 0.25–1.1 mA | Latch **ON** (sensing) |
| I ≥ 2.8 mA | Latch **OFF** (idle) |
| I between 1.1 mA and 2.8 mA | Hold previous latch |

**ON/OFF uses current** (bench V and I are not a single fixed Ω). **Open/short** use voltage and current.

## Conversions

```
V_mV = adc × 5000 / 4096
I_µA = adc × 5000 × 1000 / (4096 × 200)
```

`NAMUR_SHUNT_OHM = 200` (effective). See `namur_config.h`.
