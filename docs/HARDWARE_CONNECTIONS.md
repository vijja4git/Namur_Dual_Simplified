# Hardware Connections

Target MCU: **Nuvoton MS51FB9AE** (TSSOP20). Pin assignments are centralized in `src/bsp/ms51/platform_ms51.h`.

## Reference pin map

| Signal | MCU pin | ADC / GPIO | Notes |
|--------|---------|------------|-------|
| CH1 loop sense | P1.7 | ADC_CH0 | Via 500 Ω shunt to GND |
| CH2 loop sense | P3.0 | ADC_CH1 | Via 500 Ω shunt to GND |
| CH1 LED | P0.0 | Output, active high | Off during fault |
| CH2 LED | P0.1 | Output, active high | Off during fault |
| Global fault LED | P0.2 | Output, active high | On if any channel fault |
| CH1 DIP (NC) | P1.0 | Input, pull-up | 1 = NC, 0 = NO |
| CH2 DIP (NC) | P1.1 | Input, pull-up | 1 = NC, 0 = NO |

## Analog front end

- 12-bit ADC, internal conversion, **5.0 V** reference (`AVREF` / VDD assumption documented in `VOLTAGE_REFERENCES.md`).
- Shunt: **500 Ω** between loop return and ground; ADC measures shunt voltage.

## Changing the pinout

1. Edit masks and ADC channel constants in `platform_ms51.h`.
2. Update this document and an entry in `docs/DECISION_LOG.md`.
3. Rebuild (`make`) and verify on hardware.

## ICE / programming

Use Nuvoton Nu-Link or compatible ISP tool. SWD/ICE pins share P1.6/P0.2 on MS51 — avoid driving ICE pins as GPIO when debugging.
