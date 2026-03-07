# Task: Cross-System Number Scanner

**Agent**: GAMMA
**Roadmap Reference**: Track 26.3 — "Agent: Cross-System Number Scanner (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure computation module that scans for appearances of any number across all registered time cycle lengths. Given a target number (e.g., 432), checks if it's a factor/multiple of known cycles, whether it appears at different scales, and computes a "coincidence score." Answers: is this number's ubiquity meaningful or coincidental?

## Files to Create

- `src/systems/unified/number_scanner.h`
- `src/systems/unified/number_scanner.c`
- `tests/systems/unified/test_number_scanner.c`

## DONE WHEN

- [ ] >= 20 registered cycles, >= 10 sacred numbers
- [ ] number_scan finds factors, multiples, scale appearances
- [ ] factor_scan, coincidence_score work correctly
- [ ] >= 30 tests, all pass, zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- Standalone module
