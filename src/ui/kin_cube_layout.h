#ifndef TIME_KIN_CUBE_LAYOUT_H
#define TIME_KIN_CUBE_LAYOUT_H

/* Kin Cube Layout — Warrior's Cube from Telektonon (L16).
 *
 * 16-day journey (moon days 7-22) displayed as a 4x4 grid.
 * Each position carries a codon number, a warrior power name,
 * and the corresponding moon day. Today's position highlighted
 * when in the cube phase.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

#define KIN_CUBE_SIZE       4
#define KIN_CUBE_POSITIONS 16

typedef struct {
    int         position;       /* 1-16 (Warrior's Cube position) */
    int         moon_day;       /* 7-22 (the actual moon day) */
    int         codon_number;   /* I Ching codon (1-64) for this position */
    const char *power;          /* "Memory", "Breath", "Instinct", etc. */
    int         highlighted;    /* 1 if today is this cube day */
    float       x, y;           /* grid position */
} kin_cube_cell_t;

typedef struct {
    kin_cube_cell_t cells[KIN_CUBE_POSITIONS]; /* 16 positions */
    int             today_position;             /* 0-15 (-1 if not in cube phase) */
    int             is_active;                  /* 1 if today is in Warrior's Cube (days 7-22) */
} kin_cube_layout_t;

/* Compute the Warrior's Cube layout for a Julian Day.
 * 16 positions in a 4x4 grid:
 *   x = ((pos-1)%4) * 0.25, y = ((pos-1)/4) * 0.25
 * Each position carries:
 *   - Moon day mapping (pos + 6 = moon day 7-22)
 *   - A codon number (I Ching hexagram for this cube face)
 *   - A warrior power name
 * Today's position highlighted if in cube phase.
 * Returns struct with is_active=0 if not in cube phase or invalid JD. */
kin_cube_layout_t kin_cube_compute(double jd);

#endif /* TIME_KIN_CUBE_LAYOUT_H */
