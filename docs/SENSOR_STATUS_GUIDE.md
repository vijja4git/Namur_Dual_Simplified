# Sensor Status Guide (Simple)

Measured levels for this project (effective **200 Ω** shunt, **5.0 V** ADC ref).  
Voltage is at the ADC pin (**P1.7** CH1, **P3.0** CH2).

**Formulas:** `V = I × 200 Ω` · `I (mA) = V / 0.2` · `ADC = V × 4096 / 5.0`

---

## Quick reference (your calibration)

| Sensor status | Voltage at pin | Current | Firmware |
|---------------|----------------|---------|----------|
| **No sensor connected** | **0 V** | **0 mA** | Lead-break fault |
| **Sensor connected** (idle, no object) | **~0.67 V** | **~3.36 mA** | Normal, latch **OFF** |
| **Sensing** (object detected) | **0.2 – 0.8 V** | **0.3 – 1 mA** | Normal, latch **ON** |
| **Short circuit** | **~1.55 V** | **~8 mA** | Short fault |

---

## Threshold bands (firmware)

| Check | Limit |
|-------|--------|
| Open | V &lt; 80 mV **or** I &lt; 80 µA |
| Short | V &gt; 1.3 V **or** I &gt; 7 mA |
| Sensing ON | I **0.25–1.1 mA** (bench V typically **0.2–0.8 V**) |
| Idle OFF | I ≥ **2.8 mA** (~0.67 V on bench) |
| Hold | I between **1.1 mA** and **2.8 mA** |
| Open / short | By **voltage** and **current** limits above |

**Note:** Bench V and I do not follow one fixed Ω law; **current** sets ON/OFF, **voltage** helps open/short detection.

---

## Example ADC values (approx.)

| Status | V | I | ADC count |
|--------|---|---|-----------|
| Open | 0 V | 0 mA | 0 |
| Sensing | 0.5 V | 0.8 mA | ~410 |
| Connected idle | 0.67 V | 3.36 mA | ~549 |
| Short | 1.55 V | 8 mA | ~1270 |

---

## LEDs (per channel)

| Status | Status LED | Fault LED |
|--------|------------|-----------|
| No sensor | OFF | ON (if P1.2 enable) |
| Connected idle | OFF (NO) / ON (NC) | OFF |
| Sensing ON | ON (NO) / OFF (NC) | OFF |
| Short | OFF | ON (if P1.2 enable) |

Constants: `include/namur_config.h`
