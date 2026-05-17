# How This Device Works (Plain Language)

This guide explains what the board does, what the LEDs mean, how the switches change behavior, and what voltage/current appears on the MS51 pins. No prior NAMUR experience required.

---

## What is this device?

A **2-channel NAMUR interface** watches the **current** in two separate sensor loops (typical industrial “NAMUR” wiring). Each loop is like a wire carrying a small DC current:

- **Low current** often means “sensor says OFF” or a **broken wire**.
- **Higher current** often means “sensor says ON”.
- **Very low** or **very high** current means something is **wrong** (fault).

The **MS51 microcontroller** measures that current, decides ON/OFF/fault, and lights **LEDs** so you can see the state.

```
  Sensor loop ----[ 500 ohm shunt ]---- GND
                        |
                     ADC pin (MS51 measures voltage here)
```

Current flows through a **500 Ω resistor** (shunt) to ground. The MS51 ADC reads the **voltage across that resistor**. From voltage we calculate current.

---

## Big picture: what happens every 20 ms

The firmware runs the same steps **50 times per second** (every **20 ms**):

1. **Measure** loop current on channel 1 and channel 2 (ADC, averaged 8 times).
2. **Convert** measurement to microamps (µA).
3. **Check faults** (wire break or short).
4. If OK, **update ON/OFF latch** with hysteresis (avoids flicker).
5. **Apply DIP switch** (NO or NC mode) for that channel.
6. **Update channel LED** for that channel only (off if that channel faults).
7. **Update that channel’s fault LED** (P0.2 or P0.3) if that channel faults — unless the **master DIP** disables all fault LEDs.
8. **Wait** 20 ms and repeat.

Steps 1–7 run **separately for CH1 and CH2**. CH2 never turns CH1’s LEDs on or off.

See `src/main.c` (orchestration) and `src/app/namur_logic.c` (rules).

---

## The two channels

| Channel | ADC sense | Status LED | Fault LED | NO/NC DIP |
|---------|-----------|------------|-----------|-----------|
| **CH1** | P1.7      | P0.0       | **P0.2**  | P1.0      |
| **CH2** | P3.0      | P0.1       | **P0.3**  | P1.1      |

**Shared (both channels):** master DIP on **P1.2** — disables **both** fault LEDs when OFF (LOW).

Channels are **fully independent**: CH1 fault LED, status LED, latch, and DIP only reflect CH1’s loop current. CH2 does not affect CH1 in any condition, and vice versa.

---

## MS51 pin connections (summary)

| MS51 pin | Direction | Connected to |
|----------|-----------|--------------|
| **P1.7** | Analog in | CH1 shunt voltage (ADC_CH0) |
| **P3.0** | Analog in | CH2 shunt voltage (ADC_CH1) |
| **P0.0** | Digital out | CH1 status LED (+) |
| **P0.1** | Digital out | CH2 status LED (+) |
| **P0.2** | Digital out | CH1 **fault** LED (+) |
| **P0.3** | Digital out | CH2 **fault** LED (+) |
| **P1.0** | Digital in | CH1 NO/NC DIP (pull-up) |
| **P1.1** | Digital in | CH2 NO/NC DIP (pull-up) |
| **P1.2** | Digital in | **Fault LED enable** (pull-up) |

LEDs are **active high**: pin **high (3.3 V / 5 V)** → LED on (with resistor to GND).

DIP: pin **low** when switch connects pin to **GND**; **high** when open (pull-up).

Full table: [HARDWARE_CONNECTIONS.md](HARDWARE_CONNECTIONS.md).

---

## How current becomes a voltage on the ADC pin

Formula:

```
Voltage at ADC pin (V) = Loop current (A) × 500 Ω
```

Examples:

| Loop current | Voltage on ADC pin |
|--------------|-------------------|
| 150 µA (0.00015 A) | 0.075 V (75 mV) |
| 1200 µA | 0.60 V |
| 2100 µA | 1.05 V |
| 6000 µA | 3.00 V |

The ADC uses a **5.0 V** reference and **12 bits** (0–4095 counts):

```
ADC count ≈ Voltage × 4096 / 5.0 V
```

| Loop current | ≈ ADC count |
|--------------|-------------|
| 150 µA | ~61 |
| 1200 µA | ~492 |
| 2100 µA | ~860 |
| 6000 µA | ~2458 |

(Firmware uses: `current_µA = adc × 10000 / 4096`.)

---

## Operating modes (DIP switches)

Each channel has **one DIP** for contact logic (**NO** vs **NC**). This does **not** change how current is measured — only how the **output/LED** relates to the internal **latch**.

| DIP position | Name | Meaning in **normal** (no fault) state |
|--------------|------|----------------------------------------|
| **0** (pin read LOW, tied to GND on many boards) | **NO** | Output/LED **follows** latch: latch ON → LED ON |
| **1** (pin read HIGH, switch open) | **NC** | Output/LED **inverted**: latch ON → LED OFF |

**During any fault**, DIP is **ignored** for the channel LED: the channel LED is **forced OFF**.

> **Note:** Exact “0/1” vs physical switch up/down depends on your PCB wiring. Firmware: `platform_read_dip_nc()` returns **true** when the pin is **high** (NC mode).

There is **no separate firmware mode** beyond: measure → fault check → latch → DIP → LEDs.

---

## Normal operation: ON / OFF / “hold” (hysteresis)

When current is **not** a fault, the firmware keeps an internal **latch** (memory of ON vs OFF):

| Measured current | Latch behavior |
|------------------|----------------|
| **> 2100 µA** | Latch turns **ON** |
| **< 1200 µA** | Latch turns **OFF** |
| **1200 µA to 2100 µA** | **Keep** previous latch (no change) |

**Why?** Real sensors hover near the threshold. Without hysteresis the LED would flicker.

**Voltage view (normal band):**

| Threshold | Current | ≈ Shunt voltage |
|-----------|---------|-----------------|
| OFF | below 1200 µA | below ~0.60 V |
| Hold zone | 1200–2100 µA | ~0.60 V – ~1.05 V |
| ON | above 2100 µA | above ~1.05 V |

---

## Faults: when they happen and what you see

Faults are checked **before** hysteresis.

### 1. Lead break / open circuit (`FAULT_LEAD_BREAK`)

| Condition | Current **below** **150 µA** |
|-----------|------------------------------|
| ≈ ADC pin voltage | **below ~75 mV** |
| Typical cause | Broken wire, unplugged sensor, very low loop current |

**What the firmware does:**

- Sets **fault** for that channel.
- **Channel LED → OFF** (even if latch was ON).
- **That channel’s fault LED → ON** (P0.2 for CH1, P0.3 for CH2), unless master DIP on P1.2 disables fault LEDs.

### 2. Short circuit (`FAULT_SHORT_CIRCUIT`)

| Condition | Current **above** **6000 µA** |
|-----------|-------------------------------|
| ≈ ADC pin voltage | **above ~3.0 V** (below 5 V full scale) |
| Typical cause | Wiring short, failed sensor, abnormal loop supply |

**Same LED behavior** as lead break.

### Fault LED summary (per channel)

| Situation | CH1 status (P0.0) | CH1 fault (P0.2) | CH2 status (P0.1) | CH2 fault (P0.3) |
|-----------|---------------------|------------------|-------------------|------------------|
| CH1 fault only | **OFF** | **ON** | Follow CH2 | **OFF** |
| CH2 fault only | Follow CH1 | **OFF** | **OFF** | **ON** |
| Both fault | **OFF** | **ON** | **OFF** | **ON** |
| Both normal | Follow latch/DIP | **OFF** | Follow latch/DIP | **OFF** |

Each **fault LED** means only: **“this channel’s loop current is out of range.”** It does not indicate the other channel.

---

## Master DIP: disable all fault LEDs (P1.2)

| P1.2 state | Switch typical position | Fault LEDs P0.2 & P0.3 |
|------------|-------------------------|-------------------------|
| **HIGH** | Open (pull-up) — “enable” | May turn **ON** when that channel has a fault |
| **LOW** | Closed to GND — “disable” | **Forced OFF** for both channels |

**What still works when fault LEDs are disabled:**

- Current measurement and fault detection **unchanged**
- Channel status LEDs (P0.0 / P0.1) still follow normal rules (status LED off on fault)

**What does not cross channels:** disabling fault LEDs on P1.2 does not change the other channel’s status LED or latch — it only gates the two fault LED outputs.

---

## Simple decision flow (one channel)

```
Start
  |
  v
Is current < 150 uA? ----YES----> LEAD BREAK fault, CH status LED OFF, CH fault LED ON (if P1.2 enable)
  |
  NO
  v
Is current > 6000 uA? ---YES----> SHORT fault, CH status LED OFF, CH fault LED ON (if P1.2 enable)
  |
  NO
  v
Update latch (2100 ON / 1200 OFF / else hold)
  |
  v
Apply DIP (NO=follow, NC=invert)
  |
  v
CH LED = output (ON)
```

---

## Where to read the code

| File | What it explains |
|------|------------------|
| `src/app/namur_logic.c` | All threshold rules (commented line by line) |
| `src/main.c` | 20 ms loop, LED updates |
| `include/namur_config.h` | Threshold numbers |
| `src/bsp/ms51/adc.c` | How ADC reads shunt voltage |
| `src/bsp/ms51/gpio.c` | LEDs and DIP pins |

---

## Related documents

- [WORKING_LOGIC.md](WORKING_LOGIC.md) — compact technical spec  
- [VOLTAGE_REFERENCES.md](VOLTAGE_REFERENCES.md) — formulas  
- [HARDWARE_CONNECTIONS.md](HARDWARE_CONNECTIONS.md) — pin list  
