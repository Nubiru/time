#ifndef TIME_KIN_SYNC_MATRIX_H
#define TIME_KIN_SYNC_MATRIX_H

/* Kin Sync Matrix — Synchronotron 441-cell circuit visualization (L12).
 *
 * 21x21 grid with 11 concentric circuits, each mapped to a planetary
 * orbit. Today's active positions (destiny HK, heptad gate HK, tonal
 * value) are highlighted as markers. Shows Tubo Mauri axis (col 11)
 * and galactic equatorial line (row 11).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 */

#define KIN_SM_SIZE       21
#define KIN_SM_TOTAL     441
#define KIN_SM_MAX_ACTIVE  4

typedef struct {
    int   row, col;     /* 1-21 (1-indexed like matrix_441) */
    int   circuit;      /* 1-11 (which concentric circuit) */
    int   is_axis;      /* 1 if on Tubo Mauri (col=11) or equatorial (row=11) */
    int   is_center;    /* 1 if (11,11) */
    int   active;       /* 1 if today's HK/tonal maps here */
    float x, y;         /* normalized position */
} kin_sm_cell_t;

typedef struct {
    int         number;  /* HK or tonal value */
    int         row, col; /* grid position */
    const char *label;   /* "Destiny", "Heptad Gate", "Tonal Value", "Center" */
} kin_sm_marker_t;

typedef struct {
    kin_sm_cell_t   cells[KIN_SM_TOTAL];        /* 441 cells, row-major [(r-1)*21+(c-1)] */
    kin_sm_marker_t markers[KIN_SM_MAX_ACTIVE];
    int             marker_count;
    int             circuit_count;               /* always 11 */
} kin_sync_matrix_t;

/* Compute the Synchronotron circuit matrix for a Julian Day.
 * 441 cells in 21x21 grid (1-indexed rows/cols).
 * Each cell knows its circuit number and axis status.
 * Cell positions: x = (col-1)/20.0, y = (row-1)/20.0.
 * Today's active positions from sync_compute highlighted.
 * Returns struct with marker_count=0 for invalid JD (< 0). */
kin_sync_matrix_t kin_sync_matrix_compute(double jd);

#endif /* TIME_KIN_SYNC_MATRIX_H */
