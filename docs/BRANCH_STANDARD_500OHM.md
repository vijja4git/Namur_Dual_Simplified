# Branch: cursor/namur-standard-500ohm

This branch keeps the **original NAMUR-style** voltage/current references:

- **500 Ω** shunt (`I_µA = adc × 10000 / 4096`)
- Open fault: **&lt; 150 µA**
- Short fault: **&gt; 6000 µA**
- Latch ON: **&gt; 2100 µA**
- Latch OFF: **&lt; 1200 µA**
- Hold: **1200–2100 µA**

Also includes per-channel fault LEDs (P0.2 / P0.3) and P1.2 master fault-LED disable.

For **bench-calibrated** levels (0 V / 0.67 V / 0.3–1 mA / 1.55 V), use branch `cursor/ms51-namur-sdcc-firmware`.

Build: see [BUILD_HEX.md](BUILD_HEX.md).
