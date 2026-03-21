#ifndef TIME_MATRIX_441_H
#define TIME_MATRIX_441_H

/* 441 Base Matrix — the 21x21 grid underlying Hunab Ku 21.
 *
 * 11 concentric circuits (10 planetary orbits + galactic nucleus)
 * numbered from the outermost ring (circuit 1 = Mercury, 80 units)
 * inward to the center position (circuit 11 = Galactic Nucleus, 1 unit).
 * Units decrease by 8 per circuit: 80, 72, 64, ..., 8, 1.
 *
 * Key axes:
 *   Column 11 = Tubo Mauri (vertical axis)
 *   Row 11    = Galactic equatorial line of force (horizontal axis)
 *   (11,11)   = Center, value 441
 *
 * 9 dimensions of time: 441 = 9 x 49.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 */

#define M441_SIZE          21
#define M441_CENTER        11
#define M441_TOTAL        441
#define M441_CIRCUIT_COUNT 11

/* --- Circuit data --- */

typedef struct {
    int         number;   /* 1-11 */
    const char *planet;   /* "Mercury", ..., "Galactic Nucleus" */
    int         units;    /* units in this circuit */
} m441_circuit_t;

/* Number of circuits (always 11). */
int m441_circuit_count(void);

/* Get circuit data by number (1-11).
 * Returns zeroed struct for invalid number. */
m441_circuit_t m441_circuit_get(int num);

/* Circuit number for a grid position (row, col both 1-21).
 * Circuit = 11 - max(|row - 11|, |col - 11|).
 * Returns 0 for out-of-bounds positions. */
int m441_circuit_for_position(int row, int col);

/* Total units across all circuits (always 441). */
int m441_total_units(void);

/* 1 if col == M441_CENTER, 0 otherwise.
 * Returns 0 for out-of-bounds positions. */
int m441_is_tubo_mauri(int row, int col);

/* 1 if row == M441_CENTER, 0 otherwise.
 * Returns 0 for out-of-bounds positions. */
int m441_is_equatorial(int row, int col);

/* 1 if row == M441_CENTER && col == M441_CENTER, 0 otherwise. */
int m441_is_center(int row, int col);

/* Dimensions of time: 441 / 49 = 9. */
int m441_dimensions_of_time(void);

#endif /* TIME_MATRIX_441_H */
