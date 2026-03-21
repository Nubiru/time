#ifndef TIME_KIN_WAVESPELL_LAYOUT_H
#define TIME_KIN_WAVESPELL_LAYOUT_H

/* Kin Wavespell Layout — 13 connected cells showing the current wavespell.
 *
 * The wavespell is a 13-day cycle in the Tzolkin. Each tone (1-13) occupies
 * one cell in a horizontal strip. The cell matching today's kin is highlighted.
 *
 * Power days (gates of power): tones 1 (Magnetic), 5 (Overtone),
 * 9 (Solar), and 13 (Cosmic).
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#include "kin_cell.h"

#define KIN_WS_CELLS 13

typedef struct {
    kin_cell_t cells[KIN_WS_CELLS]; /* 13 cells, one per tone */
    int today_position;             /* 1-13: which cell is today's kin */
    int wavespell_number;           /* 1-20 */
    int start_kin;                  /* first kin in this wavespell */
    const char *purpose;            /* wavespell purpose keyword */
} kin_wavespell_layout_t;

/* Compute wavespell layout for a kin (1-260).
 * All 13 cells of the wavespell are populated.
 * Cells are arranged in a horizontal strip:
 *   cell[0] at x=0.04, cell[12] at x=0.96
 *   all at y=0.5, each 0.065 wide, 0.25 tall
 * The cell matching today's kin is marked highlighted=1.
 * Returns zeroed struct for invalid kin. */
kin_wavespell_layout_t kin_wavespell_compute(int kin);

/* Get the tone name for a wavespell position (1-13).
 * Returns the Arguelles tone name: "Magnetic", "Lunar", "Electric", etc.
 * Uses dreamspell_tone(). Returns "?" for invalid. */
const char *kin_wavespell_tone_name(int position);

/* Check if a cell in the wavespell is a "power day" (gates of power).
 * Positions 1 (Magnetic), 5 (Overtone), 9 (Solar), 13 (Cosmic)
 * are considered power days. Returns 1 if power day, 0 otherwise. */
int kin_wavespell_is_power_day(int position);

#endif /* TIME_KIN_WAVESPELL_LAYOUT_H */
