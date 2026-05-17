# Hardware Connections

Target MCU: **Nuvoton MS51FB9AE** (TSSOP20). Pin assignments are in `src/bsp/ms51/platform_ms51.h`.

## Reference pin map

| Signal | MCU pin | ADC / GPIO | Notes |
|--------|---------|------------|-------|
| CH1 loop sense | P1.7 | ADC_CH0 | 500 Ω shunt to GND |
| CH2 loop sense | P3.0 | ADC_CH1 | 500 Ω shunt to GND |
| CH1 status LED | P0.0 | Output, active high | Off during fault |
| CH2 status LED | P0.1 | Output, active high | Off during fault |
| CH1 fault LED | P0.2 | Output, active high | CH1 faults only |
| CH2 fault LED | P0.3 | Output, active high | CH2 faults only |
| CH1 DIP (NO/NC) | P1.0 | Input, pull-up | HIGH = NC |
| CH2 DIP (NO/NC) | P1.1 | Input, pull-up | HIGH = NC |
| Fault LED enable | P1.2 | Input, pull-up | LOW = disable P0.2 & P0.3 |

Channels are independent: no shared fault LED and no cross-channel logic in firmware.

## Analog front end

- 12-bit ADC, **5.0 V** reference — see [VOLTAGE_REFERENCES.md](VOLTAGE_REFERENCES.md).
- **500 Ω** shunt per channel.

## Changing the pinout

1. Edit `platform_ms51.h`.
2. Update this file and [HOW_THE_DEVICE_WORKS.md](HOW_THE_DEVICE_WORKS.md).
3. Add entry in [DECISION_LOG.md](DECISION_LOG.md).
4. Rebuild and test on hardware.

## ICE / programming

Nu-Link may share pins with P1.6 / P0.2 on some boards; verify your PCB before using P0.2 as fault LED during debug.
