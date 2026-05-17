# Namur Dual — MS51 (SDCC)

2-channel **NAMUR** interface firmware for **Nuvoton MS51**, built with **SDCC**. Application logic is MCU-independent; register access is isolated under `src/bsp/ms51/`.

## Quick start

```bash
make          # build/firmware.ihx + build/firmware.hex
make test     # host unit tests for NAMUR logic
```

Toolchain setup and troubleshooting: [docs/SETUP_AND_COMMANDS.md](docs/SETUP_AND_COMMANDS.md).

## Documentation

| Document | Contents |
|----------|----------|
| **[docs/HOW_THE_DEVICE_WORKS.md](docs/HOW_THE_DEVICE_WORKS.md)** | **Start here** — plain-language operation, faults, voltages, DIP modes, MS51 pins |
| [docs/SENSOR_STATUS_GUIDE.md](docs/SENSOR_STATUS_GUIDE.md) | Sensor status vs voltage vs current (simple table) |
| [docs/LED_AND_SWITCH_TEST_RESULTS.md](docs/LED_AND_SWITCH_TEST_RESULTS.md) | Automated LED/switch/isolation test matrix |
| [docs/PURPOSE.md](docs/PURPOSE.md) | Project goals and scope |
| [docs/WORKING_LOGIC.md](docs/WORKING_LOGIC.md) | Thresholds, hysteresis, DIP, LEDs |
| [docs/HARDWARE_CONNECTIONS.md](docs/HARDWARE_CONNECTIONS.md) | Pin map and analog front end |
| [docs/VOLTAGE_REFERENCES.md](docs/VOLTAGE_REFERENCES.md) | 5 V ref, 500 Ω shunt, µA formula |
| [docs/TESTING.md](docs/TESTING.md) | Host and on-target test checklist |
| [docs/DECISION_LOG.md](docs/DECISION_LOG.md) | Architecture decisions and tradeoffs |
| [cursor/CURSOR.md](cursor/CURSOR.md) | File map, call flow, dependencies |
| [cursor/PLANNING.md](cursor/PLANNING.md) | Implementation plan and validation |

## Architecture

```
include/          namur_config.h, platform.h (contract)
src/app/          NAMUR logic (no register headers)
src/bsp/ms51/     MS51 clock, GPIO, ADC, delay
src/main.c        20 ms super-loop
tests/            Host tests + mock platform
external/         Curated MS51_16K.h for SDCC
```

## Behavior summary

- 2 independent channels; 20 ms loop.
- Open: 0 V / 0 mA; short: ~1.55 V / 8 mA; sensing ON: 0.2–0.8 V & 0.3–1 mA; idle: ~0.67 V / 3.36 mA.
- DIP 0 = NO (follow), 1 = NC (invert when not faulting).
- Per-channel fault LEDs (P0.2 CH1, P0.3 CH2); P1.2 master DIP disables both fault LEDs when LOW.
- Channels fully independent (no cross-channel LED or latch coupling).
- ADC: 12-bit, 8-sample average; 5 V ref, ~200 Ω effective shunt.

## License

Add your project license here.
