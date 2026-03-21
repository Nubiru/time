#ifndef TIME_KIN_BASE_MATRIX_H
#define TIME_KIN_BASE_MATRIX_H

/* Base Matrix 441 — 21x21 numbered grid for Synchronotron practice.
 *
 * The 441 cells (0-440) form the Synchronotron base matrix. Each day,
 * the seal HK number, heptad gate HK number, and tonal value map onto
 * specific grid positions, highlighting the day's active frequencies.
 *
 * Cell 220 (row 10, col 10) is the Hunab Ku center.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_BM_SIZE 21
#define KIN_BM_TOTAL 441       /* 21 x 21 */
#define KIN_BM_MAX_ACTIVE 8

typedef struct {
    int number;       /* 0-440 (position number in the matrix) */
    float x, y;       /* normalized position (col/21, row/21) */
    int active;       /* 1 if this position is active today */
    int is_center;    /* 1 if position 220 (Hunab Ku center) */
} kin_bm_cell_t;

typedef struct {
    int number;           /* the HK or tonal value (0-440) */
    int row, col;         /* position in grid */
    const char *label;    /* "Destiny", "Heptad Gate", "Tonal Value" */
} kin_bm_active_t;

typedef struct {
    kin_bm_cell_t cells[KIN_BM_TOTAL]; /* 441 cells */
    kin_bm_active_t active[KIN_BM_MAX_ACTIVE]; /* today's active positions */
    int active_count;    /* number of active positions */
    int center_number;   /* always 220 (center of 21x21) */
} kin_base_matrix_t;

/* Compute the base matrix for a Julian Day.
 * 441 cells in a 21x21 grid, numbered 0-440 row-major.
 * Cell positions: x = col/21.0, y = row/21.0.
 * Center cell (row 10, col 10) = position 220 = Hunab Ku.
 *
 * Active positions from today's Synchronotron data:
 *   - Destiny seal HK number (sync_seal_hk)
 *   - Heptad gate HK number (sync_heptad_gate_hk)
 *   - Tonal value (tone * lens_value)
 * Only values in range 0-440 are mapped. Duplicates suppressed.
 *
 * Returns struct with active_count=0 for invalid JD (< 0). */
kin_base_matrix_t kin_base_matrix_compute(double jd);

/* Map a HK/tonal number to grid position.
 * number: 0-440. row = number/21, col = number%21.
 * Sets row and col to -1 if number is out of range. */
void kin_bm_number_to_pos(int number, int *row, int *col);

/* Map grid position to number.
 * Returns row*21 + col, or -1 if row or col is out of range. */
int kin_bm_pos_to_number(int row, int col);

#endif /* TIME_KIN_BASE_MATRIX_H */
