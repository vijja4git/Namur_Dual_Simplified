# CURSOR.md

## Repository purpose
MS51-only 2-channel NAMUR firmware with portable logic + MS51 BSP layer.

## File capability map
- src/main.c: boot + main loop orchestration
- src/app/namur_logic.c: state machine and LED decision logic
- src/bsp/ms51/platform_ms51.c: HAL implementation for ADC/DIP/LED/delay
- include/platform.h: app-to-BSP contract

## Dependency graph
main -> namur_logic -> platform.h -> platform_ms51
tests -> namur_logic + mock_platform

## Runtime flow
1. platform init
2. app init
3. periodic update (read ADC -> convert -> classify -> DIP -> LEDs)

## Decisions (latest)
- D001:
  - Decision:
  - Why:
  - Rejected alternatives:
  - Impact:

## Last update
- Date:
- Changed by:
- Summary: