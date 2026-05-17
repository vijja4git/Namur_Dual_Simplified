# LED and Switch Test Results (Automated)

This file records what the **host integration tests** verify. Run them anytime:

```bash
make test
```

Tests live in:

- `tests/test_namur_logic.c` — threshold math and per-channel logic only  
- `tests/test_channel_integration.c` — full tick (ADC → logic → LEDs) with mock hardware  

Production tick function: `namur_app_tick()` in `src/app/namur_app.c` (same path as `main.c`).

---

## Test matrix and expected behavior

| Test | CH0 current | CH1 current | DIP / notes | CH0 status LED | CH0 fault LED | CH1 status LED | CH1 fault LED |
|------|-------------|-------------|-------------|----------------|---------------|----------------|---------------|
| CH0 fault isolation | 100 µA (break) | 2500 µA (ON) | Both NO | OFF | ON | ON | OFF |
| Hysteresis ramp | 1100→2200→1500→1100 µA | 800 µA idle | CH0 NO | OFF→ON→ON→OFF | OFF throughout | (unchanged) | (unchanged) |
| NO vs NC | 2200 µA | — | NO then NC | ON then OFF | — | — | — |
| NC during fault | 100 µA | — | NC | OFF (not inverted ON) | ON | — | — |
| CH1 short only | 2500 µA | 7000 µA | — | ON | OFF | OFF | ON |
| Master DIP off | 100 µA | 7000 µA | P1.2 disable | OFF | **OFF** | OFF | **OFF** |
| Mixed NO/NC | 2200 µA | 1100 µA | CH0 NO, CH1 NC | ON | OFF | ON (NC inv.) | OFF |

**Master DIP off:** faults still set in software; only fault **LED outputs** are suppressed.

---

## Questions answered by tests

### When current varies, do status LEDs follow the rules?

**Yes** — `test_varying_current_hysteresis_ch0` steps CH0 through OFF → ON → hold → OFF and checks `mock_platform_get_channel_led(0)` after each `namur_app_tick()`.

### If CH0 faults, is CH1 affected?

**No** — `test_ch0_fault_does_not_affect_ch1_leds` forces CH0 lead-break and CH1 normal ON; CH1 status LED stays **ON**, CH1 fault LED **OFF**.

### Is NO / NC accurate?

**Yes** — `test_no_vs_nc_same_current` at 2200 µA: NO → status LED ON, NC → OFF, NO again → ON.  
`test_nc_does_not_apply_during_fault` confirms NC does **not** turn the status LED on during a fault.

### Per-channel fault LEDs?

**Yes** — `test_short_circuit_only_ch1_fault_led` — only CH1 fault LED on when only CH1 is shorted.

### Master disable switch?

**Yes** — `test_master_dip_disables_fault_leds_only` — both fault LEDs off when disabled; fault flags still set.

---

## Last run

Re-run `make test` after any logic change and update this section if you add tests.

Expected console output:

```
All namur_logic tests passed.
All channel integration tests passed.
```

---

## Related docs

- [SENSOR_STATUS_GUIDE.md](SENSOR_STATUS_GUIDE.md) — status vs voltage vs current  
- [HOW_THE_DEVICE_WORKS.md](HOW_THE_DEVICE_WORKS.md) — full operation guide  
