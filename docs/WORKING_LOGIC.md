# Working Logic

## Overview

Each 20 ms cycle, **each channel is processed alone**:

1. Read 8-sample averaged 12-bit ADC for that channel.
2. Convert to µA; classify fault vs normal; update hysteresis latch.
3. Apply that channel’s NO/NC DIP (normal only).
4. Drive that channel’s status LED (off on fault).
5. Drive that channel’s fault LED (on if fault, and P1.2 master enable is HIGH).

No step reads the other channel’s state.

## Thresholds (µA)

| Condition | Result |
|-----------|--------|
| I < 150 | `FAULT_LEAD_BREAK` |
| I > 6000 | `FAULT_SHORT_CIRCUIT` |
| I > 2100 (normal) | Latch ON |
| I < 1200 (normal) | Latch OFF |
| 1200 ≤ I ≤ 2100 (normal) | Hold previous latch |

## DIP switches

| Pin | Function |
|-----|----------|
| P1.0 | CH1 NO/NC |
| P1.1 | CH2 NO/NC |
| P1.2 | Master fault LED enable (LOW = both fault LEDs forced off) |

## Fault LEDs

| Pin | Function |
|-----|----------|
| P0.2 | CH1 fault LED |
| P0.3 | CH2 fault LED |

ON when that channel has a fault and `platform_read_fault_leds_enabled()` is true.

## Current from ADC

```
I_µA = adc_avg × 10000 / 4096
```

See `namur_adc_to_current_ua()` in `src/app/namur_logic.c`.
