#ifndef TIME_KIN_ORACLE_LAYOUT_H
#define TIME_KIN_ORACLE_LAYOUT_H

#include "kin_cell.h"

/* Oracle cross layout — 5-kin Dreamspell arrangement.
 *
 * The daily Kin Maya view: destiny kin at center, surrounded by
 * guide (top), analog (right), antipode (left), occult (bottom).
 *
 * Positions are in normalized 0-1 coordinates:
 *   destiny:  center (0.50, 0.50)
 *   guide:    top    (0.50, 0.15)
 *   analog:   right  (0.85, 0.50)
 *   antipode: left   (0.15, 0.50)
 *   occult:   bottom (0.50, 0.85)
 *
 * Cell size: 0.25 x 0.20 for each cell.
 *
 * Source: Arguelles, "Dreamspell" (1990).
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_ORACLE_POSITIONS 5

typedef struct {
    kin_cell_t destiny;   /* center position */
    kin_cell_t guide;     /* top */
    kin_cell_t analog;    /* right */
    kin_cell_t antipode;  /* left */
    kin_cell_t occult;    /* bottom */
    int hidden_kin;       /* 261 - destiny kin */
} kin_oracle_layout_t;

/* Compute oracle cross layout for a kin (1-260).
 * Uses dreamspell_oracle() for the 5 relationships.
 * Marks destiny cell as highlighted=1.
 * Returns zeroed struct for invalid kin. */
kin_oracle_layout_t kin_oracle_compute(int kin);

/* Get oracle position name.
 * index: 0=Destiny, 1=Guide, 2=Analog, 3=Antipode, 4=Occult.
 * Returns "?" for invalid index. */
const char *kin_oracle_position_name(int index);

/* Get the cell at oracle position index.
 * 0=destiny, 1=guide, 2=analog, 3=antipode, 4=occult.
 * Returns cell with kin=0 for invalid index. */
kin_cell_t kin_oracle_cell_at(const kin_oracle_layout_t *layout, int index);

#endif /* TIME_KIN_ORACLE_LAYOUT_H */
