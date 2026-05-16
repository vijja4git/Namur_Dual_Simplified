# Setup and Commands

## Prerequisites

| Tool | Purpose | Install (macOS example) |
|------|---------|-------------------------|
| SDCC | MS51 firmware build | `brew install sdcc` |
| packihx | Intel HEX from IHX (SDCC) | Included with SDCC |
| gcc or clang | Host unit tests | Xcode CLT or `brew install gcc` |
| make | Build orchestration | Xcode CLT |

Verify:

```bash
sdcc --version
packihx 2>&1 | head -1
gcc --version
```

## Fresh clone workflow

```bash
git clone <repository-url>
cd Namur_Dual_Simplified
make clean
make
make test
```

### Expected artifacts

- `build/firmware.ihx` — SDCC load image
- `build/firmware.hex` — Intel HEX for programmer
- `build/test_namur_logic` — host test binary (after `make test`)

## Make targets

| Target | Action |
|--------|--------|
| `make` / `make all` | Build firmware `.ihx` and `.hex` |
| `make hex` | Regenerate `.hex` from `.ihx` |
| `make test` | Build and run host logic tests |
| `make clean` | Remove `build/` |

## Flashing

Use Nuvoton Nu-Link (or vendor tool) to program `build/firmware.hex` into MS51FB9AE. Refer to Nuvoton ISP user guide for your OS.

## Troubleshooting

| Symptom | Check |
|---------|--------|
| `sdcc: command not found` | Install SDCC, reopen shell |
| `packihx: command not found` | Reinstall SDCC; ensure install prefix on `PATH` |
| Link errors on BSP | Include paths in `Makefile`; only `src/bsp/ms51` should include `MS51_16K.h` |
| Wrong current reading | `NAMUR_VREF_MV`, shunt value, ADC channel pins in `platform_ms51.h` |

## Related docs

- Architecture trace: `cursor/CURSOR.md`
- Logic spec: `docs/WORKING_LOGIC.md`
- Hardware pins: `docs/HARDWARE_CONNECTIONS.md`
