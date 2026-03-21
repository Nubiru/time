#ifndef TIME_KIN_BOARD_LAYOUT_H
#define TIME_KIN_BOARD_LAYOUT_H

/* Kin Board Layout — 260-day Tzolkin harmonic module.
 *
 * Full 20x13 grid: rows = seals (0-19), columns = tones (1-13).
 * Each cell is a kin_cell_t with normalized position and size.
 * Today's kin is highlighted.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#include "kin_cell.h"

#define KIN_BOARD_ROWS  20   /* seals */
#define KIN_BOARD_COLS  13   /* tones */
#define KIN_BOARD_TOTAL 260

typedef struct {
    kin_cell_t cells[KIN_BOARD_TOTAL]; /* 260 cells, row-major: cells[seal * 13 + (tone-1)] */
    int today_kin;                      /* 1-260: which kin is highlighted */
    int today_row;                      /* seal index (0-19) of today */
    int today_col;                      /* tone-1 index (0-12) of today */
    int portal_count;                   /* number of GAP days in the board */
} kin_board_layout_t;

/* Build the full 260-cell board with today's kin highlighted.
 * today_kin: 1-260 (which kin to highlight). 0 = no highlight.
 * Cells arranged in 20 rows (seals) x 13 columns (tones).
 * Each cell positioned in normalized coords:
 *   x = col * (1.0/13.0), y = row * (1.0/20.0)
 *   w = 1.0/13.0, h = 1.0/20.0
 * Returns zeroed struct if today_kin is out of range (but 0 = valid no-highlight). */
kin_board_layout_t kin_board_compute(int today_kin);

/* Get the kin number at board position (row=seal 0-19, col=tone-1 0-12).
 * Uses tzolkin_board_kin_number(col+1, row). Returns 0 for invalid. */
int kin_board_kin_at(int row, int col);

/* Get the cell index in the cells[] array for a given kin (1-260).
 * Returns -1 for invalid kin. */
int kin_board_cell_index(int kin);

#endif /* TIME_KIN_BOARD_LAYOUT_H */
