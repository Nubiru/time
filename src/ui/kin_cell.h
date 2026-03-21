#ifndef TIME_KIN_CELL_H
#define TIME_KIN_CELL_H

/* Kin Maya cell template — single cell in the 260-day Tzolkin.
 *
 * Building block for all Kin Maya visual layouts: harmonic grid,
 * oracle cross, wavespell strip, castle display. Each cell holds
 * the decoded kin identity (seal, tone, color) plus layout geometry.
 *
 * Color mapping (Arguelles Dreamspell):
 *   seal % 4 → 0=Red, 1=White, 2=Blue, 3=Yellow
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

typedef struct {
    int kin;              /* 1-260 */
    int seal;             /* 0-19 (solar seal index) */
    int tone;             /* 1-13 (galactic tone) */
    int color;            /* 0=Red, 1=White, 2=Blue, 3=Yellow */
    float x, y;           /* position (normalized 0.0-1.0) */
    float w, h;           /* size (normalized) */
    int highlighted;      /* 1 = this cell is today / active */
} kin_cell_t;

/* Create a cell from kin number (1-260).
 * Position/size default to 0. highlighted = 0.
 * Returns zeroed struct for invalid kin. */
kin_cell_t kin_cell_from_kin(int kin);

/* Create a cell with explicit position and size.
 * Decodes seal/tone/color from kin. highlighted = 0.
 * Returns zeroed struct for invalid kin. */
kin_cell_t kin_cell_create(int kin, float x, float y, float w, float h);

/* Get the Arguelles directional color for a seal (0-19).
 * seal % 4: 0=Red(0), 1=White(1), 2=Blue(2), 3=Yellow(3).
 * Returns -1 for invalid seal. */
int kin_cell_color(int seal);

/* Get RGBA color values for a cell's directional color.
 * Uses the canonical Arguelles colors:
 *   Red   = {0.85, 0.20, 0.20, 1.0}
 *   White = {0.90, 0.90, 0.90, 1.0}
 *   Blue  = {0.20, 0.40, 0.80, 1.0}
 *   Yellow= {0.90, 0.80, 0.20, 1.0}
 * Writes 4 floats to rgba. Returns 1 on success, 0 on invalid color. */
int kin_cell_rgba(int color, float rgba[4]);

#endif /* TIME_KIN_CELL_H */
