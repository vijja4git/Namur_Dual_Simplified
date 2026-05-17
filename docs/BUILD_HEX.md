# Build Intel HEX (MS51)

## Prerequisites

Install **SDCC** and **make** (see [SETUP_AND_COMMANDS.md](SETUP_AND_COMMANDS.md)).

```bash
sdcc --version
make --version
```

## Build steps

From the repository root:

```bash
make clean
make
```

## Output files

| File | Use |
|------|-----|
| `build/firmware.hex` | Flash to MS51 with Nu-Link / ISP tool |
| `build/firmware.ihx` | SDCC image (intermediate) |

## Optional: logic tests (host PC)

```bash
make test
```

## Branch note

This document applies to any branch. Threshold values differ by branch:

| Branch | Thresholds |
|--------|------------|
| `cursor/ms51-namur-sdcc-firmware` | Calibrated (0 V / 0.67 V / 0.2–0.8 V / 1.55 V bench levels) |
| `cursor/namur-standard-500ohm` | Original 500 Ω shunt, 150 / 1200 / 2100 / 6000 µA |

Switch branch before building:

```bash
git checkout cursor/namur-standard-500ohm   # original references
# or
git checkout cursor/ms51-namur-sdcc-firmware  # calibrated references
make clean && make
```
