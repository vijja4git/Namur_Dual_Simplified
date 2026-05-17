# PLANNING.md

## Current objective
- [x] Initialize MS51-only NAMUR repo with layered architecture, docs, build, and host tests

## Constraints
- Keep NAMUR logic portable (no register headers in `src/app/*`)
- Keep MS51 register access isolated under `src/bsp/ms51/*`
- SDCC-first toolchain; no Keil/N76E003 compatibility in this repo

## Plan (must be updated before each implementation cycle)
1. [x] Define folder structure and curated MS51 headers (`external/`, `ms51_registers.h`)
2. [x] Implement shared app logic (`namur_types`, `namur_logic`, `namur_config`)
3. [x] Implement MS51 BSP (clock, gpio, adc, delay, `platform_ms51`)
4. [x] Wire `main.c` 20 ms loop
5. [x] Makefile: `make` → `.ihx`/`.hex`, `make test` → host unit tests
6. [x] Write docs (purpose, logic, hardware, setup, decisions)
7. [x] Validate `make` and `make test` from workspace root

## Why this sequence
- Portable logic and host tests validate thresholds before silicon integration
- BSP stays swappable; pin map documented for hardware bring-up
- Curated headers avoid Keil BSP dependency chains under SDCC

## Risks and mitigations
| Risk | Mitigation |
|------|------------|
| Vendor header incompatibility with SDCC | Curated `MS51_16K.h` + `ms51_registers.h`; rationale in `DECISION_LOG.md` |
| Hardware pin mux mismatch vs reference board | Explicit `HARDWARE_CONNECTIONS.md`; pins centralized in `platform_ms51.h` |
| ADC timing / channel crosstalk | 8-sample average; channel settle delay in `adc.c` |
| Integer overflow in µA conversion | `uint32_t` math; unit-tested conversion helper |

## Validation plan
- [x] `make` produces `build/firmware.ihx` and `build/firmware.hex`
- [x] `make test` passes threshold, hysteresis, DIP, and fault LED aggregation cases
- [x] Docs consistent with code (`WORKING_LOGIC.md`, `VOLTAGE_REFERENCES.md`)
- [x] `cursor/CURSOR.md` reflects final architecture
- [x] Fresh-clone steps in `docs/SETUP_AND_COMMANDS.md`

## Iteration log
### Iteration 1
- Planned: Full repo bootstrap per mandatory structure
- Implemented: App logic, MS51 BSP, Makefile, host tests, documentation set
- Deviations: Default pin map assigned for MS51FB9AE TSSOP20 (documented; not from legacy N76 repo)
- Next step: Hardware validation on target PCB; adjust `platform_ms51.h` if routing differs
