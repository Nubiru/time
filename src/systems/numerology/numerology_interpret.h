/* numerology_interpret.h — Numerology interpretation data
 *
 * Archetype data for the 9 root numbers (1-9) and 3 master
 * numbers (11, 22, 33) of Pythagorean numerology.  Each number
 * carries a vibrational quality — a personality archetype that
 * colors the life path, personal year, and daily energy.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_NUMEROLOGY_INTERPRET_H
#define TIME_NUMEROLOGY_INTERPRET_H

/* Per-number interpretation (12 entries: 1-9 + 11, 22, 33) */
typedef struct {
    int number;                 /* 1-9, 11, 22, or 33 */
    const char *name;           /* Archetype name */
    const char *quality;        /* Essential vibration */
    const char *strength;       /* Gift of this number */
    const char *challenge;      /* Shadow of this number */
    const char *brief;          /* One-sentence meaning */
} ni_number_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: the number itself */
    char glance[128];           /* T1: "Life Path 7 — The Seeker" */
    char detail[512];           /* T3: Full narrative */
} numerology_interp_t;

/* Return interpretation for a root or master number.
 * Valid: 1-9, 11, 22, 33.
 * Invalid: returns struct with number=-1, all strings "?". */
ni_number_t ni_number_data(int number);

/* Compose tiered interpretation.
 * life_path: 1-9 or 11/22/33 (primary number).
 * personal_year: 1-9 or 11/22/33 (current year vibration).
 * personal_year < 0 means omit from output.
 * Invalid life_path: "?" strings. */
numerology_interp_t ni_interpret(int life_path, int personal_year);

/* Returns 12 (9 roots + 3 masters). */
int ni_number_count(void);

#endif /* TIME_NUMEROLOGY_INTERPRET_H */
