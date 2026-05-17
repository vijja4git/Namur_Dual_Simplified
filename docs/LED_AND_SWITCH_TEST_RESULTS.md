# LED and Switch Test Results (Automated)

Run: `make test`

## Calibrated test matrix

| Test | CH0 | CH1 | Expected CH0 status | CH0 fault | CH1 status | CH1 fault |
|------|-----|-----|-------------------|-----------|------------|-----------|
| Open vs sensing | 0 mA | 0.8 mA | OFF | ON | ON | OFF |
| Idle ↔ sense | 3.36 mA → 0.8 mA → hold → 3.36 mA | 3.36 mA idle | OFF→ON→hold→OFF | OFF | — | — |
| NO / NC @ 0.8 mA | 0.8 mA | — | ON / OFF / ON | — | — | — |
| Short CH1 only | 3.36 mA idle | 8 mA | OFF | OFF | OFF | ON |
| Master DIP off | 0 mA | 8 mA | fault LEDs forced off | | | |

Nominal currents use **200 Ω** shunt model matching measured V/I.

## Behaviour notes

- **Open:** 0 V / 0 mA → lead-break fault.
- **Sensing ON:** both V (0.2–0.8 V) and I (0.3–1 mA) must be in band (object detected).
- **Connected idle:** ~3.36 mA → latch OFF even though V ≈ 0.67 V is inside sense voltage window.
- **Short:** ~1.55 V / 8 mA → short fault.
- Channels remain independent in all tests.
