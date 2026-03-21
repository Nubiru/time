#ifndef TIME_FORCE_LINES_H
#define TIME_FORCE_LINES_H

/* Lines of Telepathic Force — vertical tube pairings and horizontal circuits.
 *
 * The Telektonon board has two types of force lines:
 *   - 12 Vertical Lines: day-pairings in the 28-day tube (upper+lower=29)
 *   - 5 Horizontal Lines: the 5 telepathy circuits connecting planet pairs
 *
 * Vertical lines pair upper days 1-12 with lower days 28-17.
 * Days 13-16 (Stairway to Heaven / center of cube) have no named vertical
 * force line — they pair numerically (13+16=29, 14+15=29) but are not among
 * the 12 named lines.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

#define FL_VERTICAL_COUNT    12
#define FL_HORIZONTAL_COUNT   5

/* --- Structs --- */

typedef struct {
    int         upper_day;   /* 1-12 */
    int         lower_day;   /* 28-17 */
    const char *meaning;     /* symbolic meaning of the pairing */
} force_line_vertical_t;

typedef struct {
    int         circuit;     /* 1-5 */
    const char *planet_a;    /* outer planet */
    const char *planet_b;    /* inner planet */
    int         seal_codes[4]; /* four solar seal codes */
    const char *powers;      /* comma-separated power names */
} force_line_horizontal_t;

/* --- Functions --- */

/* Number of named vertical force lines (always 12). */
int force_line_vertical_count(void);

/* Get vertical force line by index (0-11).
 * Returns zeroed struct for invalid index. */
force_line_vertical_t force_line_vertical_get(int index);

/* Get the vertical force line for a given day (1-28).
 * Days 1-12 return the line where that day is the upper_day.
 * Days 17-28 return the line where that day is the lower_day.
 * Days 13-16 return zeroed struct (no named vertical force line).
 * Invalid days return zeroed struct. */
force_line_vertical_t force_line_vertical_for_day(int day);

/* Paired day in the 28-day tube.
 * Returns 29 - day for valid days 1-28.
 * Returns 0 for invalid. */
int force_line_paired_day(int day);

/* Number of horizontal force lines (always 5). */
int force_line_horizontal_count(void);

/* Get horizontal force line by index (0-4).
 * Returns zeroed struct for invalid index. */
force_line_horizontal_t force_line_horizontal_get(int index);

#endif /* TIME_FORCE_LINES_H */
