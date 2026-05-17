# Purpose

This repository contains **MS51-only** firmware for a **2-channel NAMUR** current-loop interface. It detects loop current via a shunt and ADC, applies NAMUR-style thresholds with hysteresis, and drives per-channel status and fault indicators (independent channels).

## Goals

- Production-style layering: portable application logic, MS51 BSP, documented hardware assumptions.
- SDCC toolchain only — no N76E003 or Keil compatibility in this repo.
- Host-testable threshold and hysteresis logic before on-target bring-up.

## Non-goals

- Dual-MCU or N76E003 support.
- Bootloader, fieldbus, or configuration over UART (unless added in a future change).

## Audience

Embedded developers bringing up the PCB, integrators verifying NAMUR behavior, and reviewers tracing requirements to code via `docs/WORKING_LOGIC.md` and `docs/DECISION_LOG.md`.
