# Working Logic

## Overview

Each 20 ms cycle:

1. Read 8-sample averaged 12-bit ADC per channel.
2. Convert ADC counts to loop current (µA).
3. Classify fault vs normal; update hysteresis latch in normal band.
4. Apply DIP NO/NC inversion (normal state only).
5. Drive channel LEDs (off on fault) and global fault LED (on if any channel faults).

## Thresholds (µA)

| Condition | Result |
|-----------|--------|
| I < 150 | `FAULT_LEAD_BREAK` |
| I > 6000 | `FAULT_SHORT_CIRCUIT` |
| I > 2100 (normal) | Latch ON |
| I < 1200 (normal) | Latch OFF |
| 1200 ≤ I ≤ 2100 (normal) | Hold previous latch |

Faults are evaluated **before** hysteresis. During any fault, the channel LED is forced **off**.

## DIP switches

| DIP | Mode | Output in normal state |
|-----|------|------------------------|
| 0 | NO | Follow latch (`output = latch`) |
| 1 | NC | Invert latch (`output = !latch`) |

Inversion applies only when `fault == NONE`. DIP does not re-enable the channel LED during fault.

## Global fault LED

ON when **either** channel has `FAULT_LEAD_BREAK` or `FAULT_SHORT_CIRCUIT`.

## Current from ADC

```
I_µA = adc_avg × VREF_mV × 1000 / (4096 × R_Ω)
```

With VREF = 5000 mV and R = 500 Ω:

```
I_µA = adc_avg × 10000 / 4096
```

Implemented in `namur_adc_to_current_ua()` (`src/app/namur_logic.c`).

## Code map

| Concern | Location |
|---------|----------|
| Thresholds / constants | `include/namur_config.h` |
| Pure state evaluation | `namur_logic_evaluate_channel()` |
| Loop orchestration | `src/main.c` |
| ADC average, GPIO | `src/bsp/ms51/` |
