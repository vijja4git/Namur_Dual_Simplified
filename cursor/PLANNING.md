# PLANNING.md

## Current objective
- [ ] Initialize MS51-only NAMUR repo with layered architecture

## Constraints
- Keep NAMUR logic portable
- Keep MS51 register access isolated
- SDCC-first toolchain

## Plan (must be updated before each implementation cycle)
1. Define folder structure
2. Implement shared app logic and types
3. Implement MS51 platform layer
4. Create Makefile build/test targets
5. Add host-side tests
6. Write docs (purpose, logic, hardware, setup, decisions)
7. Validate commands from fresh clone

## Why this sequence
- De-risks architecture before hardware details
- Enables test-first validation of logic
- Keeps integration issues localized

## Risks and mitigations
- Vendor header incompatibility with SDCC  
  - Mitigation: curated minimal register headers
- Hardware pin mux mismatch  
  - Mitigation: explicit mapping docs + checklist

## Validation checklist
- [ ] make
- [ ] make hex
- [ ] make test
- [ ] docs updated
- [ ] CURSOR.md rewritten after latest decisions

## Iteration log
### Iteration 1
- Planned:
- Implemented:
- Deviations:
- Next step: