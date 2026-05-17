# Sensor Status Guide (Simple)

This table describes what the firmware **sees** on each channel when the loop current passes through the **500 Ω shunt**.  
Voltage is measured at the **ADC pin** (P1.7 for CH1, P3.0 for CH2). Reference: **5.0 V** ADC full scale.

Formula: **Voltage (V) = Current (A) × 500 Ω**  
Example: **2100 µA** → 0.0021 A × 500 = **1.05 V**

---

## Quick reference table

| Sensor status | What it usually means | Typical current | Typical voltage at ADC pin |
|---------------|----------------------|-----------------|---------------------------|
| **No sensor connected** (open / lead break) | Wire broken, unplugged, or almost no current | **Below 150 µA** | **Below ~0.08 V** (75 mV at 150 µA) |
| **Sensor connected — OFF band** | Sensor wired, “off” or low signal | **Below 1200 µA** (and above 150 µA) | **~0.08 V to ~0.60 V** |
| **Sensor connected — HOLD band** | Between off and on; latch keeps previous state | **1200 µA to 2100 µA** | **~0.60 V to ~1.05 V** |
| **Sensing ON** (normal) | Strong enough signal to latch **ON** | **Above 2100 µA** | **Above ~1.05 V** |
| **Sensor short circuit** | Too much current — wiring or sensor fault | **Above 6000 µA** | **Above ~3.0 V** |

---

## What the board does for each status

| Sensor status | Status LED (P0.0 / P0.1) | Fault LED (P0.2 / P0.3) |
|---------------|--------------------------|-------------------------|
| No sensor connected | **OFF** | **ON** (if P1.2 enable) |
| Connected — OFF (latched off) | **OFF** (NO) or **ON** (NC inverted) | **OFF** |
| Connected — HOLD | Stays like last ON/OFF | **OFF** |
| Sensing ON (latched on) | **ON** (NO) or **OFF** (NC) | **OFF** |
| Short circuit | **OFF** | **ON** (if P1.2 enable) |

**P1.2 master switch:** LOW = both fault LEDs forced off (faults still detected internally).

---

## Example currents and voltages

| Current | Voltage across 500 Ω | Firmware region |
|---------|------------------------|-----------------|
| 50 µA | 0.025 V (25 mV) | Lead break fault |
| 150 µA | 0.075 V (75 mV) | Border of lead break |
| 800 µA | 0.40 V | Normal — latch OFF |
| 1200 µA | 0.60 V | Border of OFF / hold |
| 1500 µA | 0.75 V | Hold zone (if latch was on or off, it stays) |
| 2100 µA | 1.05 V | Border of hold / ON |
| 2500 µA | 1.25 V | Normal — latch ON |
| 6000 µA | 3.00 V | Border of short fault |
| 8000 µA | 4.00 V | Short circuit fault |

---

## Two channels

CH1 and CH2 each have their **own** row in the tables above.  
CH1 fault does **not** change CH2’s status LED unless CH2’s own current is in a fault or ON/OFF range.

More detail: [HOW_THE_DEVICE_WORKS.md](HOW_THE_DEVICE_WORKS.md)
