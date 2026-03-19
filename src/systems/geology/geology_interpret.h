/* geology_interpret.h — Geological time interpretation data
 *
 * Archetype data for the 4 eons, 5 major extinction events, and
 * 7 supercontinents of Earth's 4.54-billion-year history.  The
 * geological time scale is the deepest layer of Time — the rhythm
 * of tectonics, climate, and evolution that underlies all human
 * calendar systems.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_GEOLOGY_INTERPRET_H
#define TIME_GEOLOGY_INTERPRET_H

/* Per-eon interpretation (4 eons) */
typedef struct {
    int eon;                    /* 0-3 (Hadean, Archean, Proterozoic, Phanerozoic) */
    const char *name;           /* "Hadean", etc. */
    const char *meaning;        /* What the name means */
    const char *character;      /* Essential character of this eon */
    const char *life;           /* State of life during this eon */
    const char *brief;          /* One-sentence meaning */
} gli_eon_t;

/* Per-extinction interpretation (5 major mass extinctions) */
typedef struct {
    int extinction;             /* 0-4 */
    const char *name;           /* Common name */
    const char *period;         /* When it occurred */
    const char *cause;          /* Primary cause */
    const char *aftermath;      /* What came after */
    const char *brief;          /* One-sentence meaning */
} gli_extinction_t;

/* Per-supercontinent interpretation (7 supercontinents) */
typedef struct {
    int index;                  /* 0-6 */
    const char *name;           /* "Pangaea", etc. */
    const char *age;            /* Approximate age range */
    const char *significance;   /* Why this assembly matters */
    const char *brief;          /* One-sentence meaning */
} gli_supercontinent_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: eon abbreviation (3 chars) */
    char glance[128];           /* T1: "Phanerozoic — Age of Visible Life" */
    char detail[512];           /* T3: Full narrative */
} geology_interp_t;

/* Return interpretation for a geological eon (0-3).
 * Invalid: returns struct with eon=-1, all strings "?". */
gli_eon_t gli_eon_data(int eon);

/* Return interpretation for a mass extinction (0-4).
 * Invalid: returns struct with extinction=-1, all strings "?". */
gli_extinction_t gli_extinction_data(int extinction);

/* Return interpretation for a supercontinent (0-6).
 * Invalid: returns struct with index=-1, all strings "?". */
gli_supercontinent_t gli_supercontinent_data(int index);

/* Compose tiered interpretation for the present geological moment.
 * eon: 0-3 (current eon index).
 * nearest_extinction: 0-4 if relevant, -1 if none.
 * supercontinent: 0-6 if in/near one, -1 if none.
 * Invalid eon: "?" strings in result. */
geology_interp_t gli_interpret(int eon, int nearest_extinction,
                               int supercontinent);

/* Returns 4 (number of eons). */
int gli_eon_count(void);

/* Returns 5 (number of major mass extinctions). */
int gli_extinction_count(void);

/* Returns 7 (number of supercontinents). */
int gli_supercontinent_count(void);

#endif /* TIME_GEOLOGY_INTERPRET_H */
