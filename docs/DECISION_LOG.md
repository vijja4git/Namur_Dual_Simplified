# Decision Log

## D001 — SDCC-only, MS51-only repository

| | |
|---|---|
| **Decision** | New repo targets MS51 with SDCC; no N76E003 code paths. |
| **Alternatives** | Shared dual-MCU tree; Keil µVision project. |
| **Rejected because** | User requirement for simplified MS51-only scope and open-toolchain CI. |
| **Impact / risk** | Pin map and BSP are MS51-specific; porting requires new BSP only. |

## D002 — Curated `MS51_16K.h` instead of full Nuvoton Keil BSP

| | |
|---|---|
| **Decision** | Minimal SFR definitions under `external/nuvoton/ms51/include/MS51_16K.h`. |
| **Alternatives** | Vendor MS51_BSP (Keil-centric); full community port clone. |
| **Rejected because** | Keil BSP pulls incompatible headers and project format; full port is heavy for 2-channel app. |
| **Impact / risk** | Extended SFRs (e.g. CLKDIV) not modeled; clock init uses reset defaults. Document if custom clock needed. |

## D003 — Portable logic in `src/app`, orchestration in `main.c`

| | |
|---|---|
| **Decision** | `namur_logic.c` has no `platform.h`; `main.c` reads ADC/DIP and drives LEDs. |
| **Alternatives** | `namur_logic_tick()` calling platform from app layer. |
| **Rejected because** | Violates MCU-independent app rule and complicates host tests. |
| **Impact / risk** | Slightly more code in `main.c`; clear separation for reviewers. |

## D004 — 8-sample ADC average in BSP

| | |
|---|---|
| **Decision** | `ms51_adc_read_averaged()` in `adc.c` with `NAMUR_ADC_SAMPLE_COUNT`. |
| **Alternatives** | Average in app; DMA/ interrupt-driven ADC. |
| **Rejected because** | Noise filtering is hardware-timing sensitive; keeps app pure. |
| **Impact / risk** | Loop time includes 16 conversions; acceptable at 20 ms period. |

## D005 — Default pin map on MS51FB9AE TSSOP20

| | |
|---|---|
| **Decision** | CH0: ADC_CH0/P1.7, CH1: ADC_CH1/P3.0; LEDs P0.0–P0.2; DIP P1.0–P1.1. |
| **Alternatives** | Copy undocumented legacy board netlist. |
| **Rejected because** | No N76 reference in workspace; map documented for change. |
| **Impact / risk** | **Must verify against actual PCB** before production. |

## D006 — Integer µA conversion

| | |
|---|---|
| **Decision** | `I_µA = adc × 10000 / 4096` with `uint32_t` intermediate. |
| **Alternatives** | Floating point; calibrated lookup table. |
| **Rejected because** | No FPU; integer matches 5 V / 500 Ω definition. |
| **Impact / risk** | ±1–2 µA quantization near thresholds; tests use direct µA for edges. |

## D007 — Output and LED off on fault

| | |
|---|---|
| **Decision** | Channel LED forced off; `output_on` not asserted on fault paths. |
| **Alternatives** | Hold last output; latch reset on fault. |
| **Rejected because** | Spec explicitly requires channel LED off during fault; safe de-energize assumed. |
| **Impact / risk** | Relay drivers must match fail-safe wiring (NO/NC field wiring). |
