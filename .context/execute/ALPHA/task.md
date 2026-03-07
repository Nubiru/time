# Task: Style Retrofit — Render Pipeline Visual Constants

**Agent**: ALPHA
**Roadmap Reference**: Track 37.4 — "Style Retrofit — Hardcoded Value Elimination"
**Date**: 2026-03-07
**Status**: CLAIMED

## Goal

Wire the render pipeline modules into the existing style system (color_theory.h, golden_layout.h). Replace hardcoded RGB color values, opacity constants, and spacing magic numbers with calls to the style system APIs. This eliminates 70+ hardcoded visual constants from 4 files and makes the render pipeline theme-aware.

## READ FIRST

- `src/render/color_theory.h` — ct_system_primary/secondary/accent(), ct_mood_color(), golden_palette()
- `src/render/color_theory.c` — Implementation with 13 system palettes
- `src/ui/golden_layout.h` — gl_opacity_cascade(), gl_spacing_scale(), gl_timing()
- `src/render/color_palette.h/.c` — The main target: 57 hardcoded RGB colors
- `src/render/aspect_lines.h/.c` — 5 hardcoded aspect colors
- `src/render/render_layers.h/.c` — 8 hardcoded opacity values

## Files to Modify

1. `src/render/color_palette.c` — Replace 57 hardcoded RGB arrays with color_theory.h calls
2. `src/render/aspect_lines.c` — Replace 5 hardcoded aspect colors
3. `src/render/render_layers.c` — Replace 8 hardcoded opacity values with golden_layout
4. `tests/render/test_color_palette.c` — Update existing tests if API behavior changes
5. `tests/render/test_aspect_lines.c` — Update tests for new color source
6. `tests/render/test_render_layers.c` — Update tests for new opacity source

## Design Approach

### color_palette.c (57 values)

The color_palette module provides per-sign/per-planet/per-aspect colors. The color_theory module provides per-system palettes (primary/secondary/accent). The retrofit connects them:

**Zodiac (12 colors)**: Use `ct_system_primary(CT_SYSTEM_ASTROLOGY)` as base hue.
Generate 12 sign colors by rotating the base through phi-spaced hue offsets
using `ct_golden_palette()` with element grouping (fire/earth/air/water).
Each element group shares a hue range.

**Elements (4 colors)**: Use the 4 mood colors that map to element meanings:

- Fire -> CT_MOOD_ENERGY (red-orange)
- Earth -> CT_MOOD_HARMONY (green/brown)
- Air -> CT_MOOD_CLARITY (yellow-green)
- Water -> CT_MOOD_CALM (blue-green)

**Planets (8 colors)**: Use `ct_system_secondary(CT_SYSTEM_ASTRONOMY)` as base.
Traditional planet colors have deep cultural roots — keep recognizable but
derive through the color_theory pipeline.

**Aspects (5 colors)**: Use `ct_system_accent(CT_SYSTEM_ASTROLOGY)` with
golden-angle rotation. Conjunction=gold, Opposition=red, Trine=blue,
Square=orange, Sextile=green.

**Tzolkin (4 colors)**: Use `ct_system_primary(CT_SYSTEM_TZOLKIN)`. The 4 directional
colors (Red, White, Blue, Yellow) are canonical — match the existing color_theory values.

**Chinese (5 colors)**: Use `ct_system_primary(CT_SYSTEM_CHINESE)`. The 5 element
colors (Wood=green, Fire=red, Earth=yellow, Metal=white, Water=black) are canonical.

**IMPORTANT**: The returned RGB values must remain visually CLOSE to the current hardcoded
values. This is a wiring change, not a redesign. If color_theory's system palette gives
a significantly different color, adjust the derivation to match visual intent.

### aspect_lines.c (5 values)

Replace the local ASPECT_COLORS array with calls to `color_aspect()` from
the retrofitted color_palette.c. This eliminates the duplicate.

### render_layers.c (8 values)

Replace hardcoded base_opacity values (0.6f, 0.8f, etc.) with calls to
`gl_opacity_cascade()` from golden_layout.h. The opacity cascade provides
phi-derived opacity levels that create visual depth hierarchy.

## DONE WHEN

- [ ] color_palette.c has ZERO hardcoded RGB float triples — all derived from color_theory.h
- [ ] aspect_lines.c uses color_palette.h instead of local hardcoded array
- [ ] render_layers.c uses gl_opacity_cascade() instead of hardcoded opacity values
- [ ] All existing tests still pass (may need value tolerance updates)
- [ ] New tests added: verify palette-to-theory mapping (>= 10 new tests)
- [ ] Visual values are CLOSE to original (same hue family, similar saturation)
- [ ] All tests pass with zero warnings
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- color_palette.c now DEPENDS ON color_theory.h (new compile dependency — update Makefile)
- render_layers.c now DEPENDS ON golden_layout.h (new compile dependency)
- Do NOT change the color_palette.h PUBLIC API — same function signatures
- Do NOT change the render_layers.h PUBLIC API
- Values must be visually similar to current — tolerance: delta < 0.15 per RGB channel
- If color_theory returns significantly different values, prefer visual continuity and
  adjust the derivation (different mood, different system) rather than changing the
  existing visual look
