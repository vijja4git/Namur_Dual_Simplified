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
- Faults: &lt; 150 µA lead break, &gt; 6000 µA short; channel LED off.
- Normal: hysteresis 2100 µA ON / 1200 µA OFF / hold between.
- DIP 0 = NO (follow), 1 = NC (invert when not faulting).
- Global fault LED if either channel faults.
- ADC: 12-bit, 8-sample average; 5 V ref, 500 Ω shunt.

## License

Add your project license here.
