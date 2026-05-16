# Testing

## Host unit tests (required)

From repository root:

```bash
make test
```

Runs `tests/test_namur_logic.c` against `src/app/namur_logic.c` (no MCU registers).

### Covered cases

- ADC → µA conversion sanity (~5 mA at mid-scale)
- Lead break (< 150 µA) and short (> 6000 µA)
- Hysteresis: latch on, latch off, hold band
- DIP NO vs NC in normal state
- Global fault OR aggregation
- ADC round-trip magnitude check

### Mock platform

`tests/mocks/mock_platform.c` implements `platform.h` for future integration tests. Current suite tests logic directly without the mock.

## Firmware build smoke test

```bash
make clean && make
ls -l build/firmware.ihx build/firmware.hex
```

## On-target checklist

1. Power MS51 at documented VDD; confirm 5 V ADC reference.
2. Inject known currents (or use NAMUR simulator) on CH1/CH2.
3. Verify LEDs:
   - Channel LED follows output in normal band; off on fault.
   - Fault LED on for either channel fault.
4. Toggle DIP: NO follows latch; NC inverts in normal state only.
5. Confirm 20 ms loop (scope on LED or GPIO toggle if added for debug).

## Regression

After logic changes: `make test`. After BSP changes: `make` + on-target checklist.
