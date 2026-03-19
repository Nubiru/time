/* japanese_interpret.h — Japanese calendar interpretation data
 *
 * Archetype data for the 5 imperial eras, 6 rokuyo (divination
 * cycle), and 24 sekki (solar terms).  The Japanese calendar
 * layers Chinese astronomical concepts with indigenous Shinto
 * and Buddhist meaning.  The rokuyo governs daily auspiciousness;
 * the sekki mark the breath of the seasons in 15-degree steps.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_JAPANESE_INTERPRET_H
#define TIME_JAPANESE_INTERPRET_H

/* Per-era interpretation (5 modern eras) */
typedef struct {
    int era;                    /* 0-4 (Meiji through Reiwa) */
    const char *name;           /* "Meiji", etc. */
    const char *kanji;          /* Japanese characters */
    const char *meaning;        /* Translation of era name */
    const char *spirit;         /* Cultural spirit of the era */
    const char *brief;          /* One-sentence meaning */
} ji_era_t;

/* Per-rokuyo interpretation (6-day divination cycle) */
typedef struct {
    int rokuyo;                 /* 0-5 */
    const char *name;           /* "Sensho", "Taian", etc. */
    const char *meaning;        /* English translation */
    const char *guidance;       /* What to do / avoid */
    const char *brief;          /* One-sentence meaning */
} ji_rokuyo_t;

/* Per-sekki interpretation (24 solar terms) */
typedef struct {
    int sekki;                  /* 0-23 */
    const char *name;           /* Romanized name */
    const char *kanji;          /* Japanese characters */
    const char *meaning;        /* English translation */
    const char *nature;         /* What happens in nature */
    const char *brief;          /* One-sentence meaning */
} ji_sekki_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: era abbreviation (3 chars) */
    char glance[128];           /* T1: "Reiwa 8 — Shunbun (Vernal Equinox)" */
    char detail[512];           /* T3: Full narrative */
} japanese_interp_t;

/* Return interpretation for a Japanese era (0-4).
 * Invalid: returns struct with era=-1, all strings "?". */
ji_era_t ji_era_data(int era);

/* Return interpretation for a rokuyo day (0-5).
 * Invalid: returns struct with rokuyo=-1, all strings "?". */
ji_rokuyo_t ji_rokuyo_data(int rokuyo);

/* Return interpretation for a sekki solar term (0-23).
 * Invalid: returns struct with sekki=-1, all strings "?". */
ji_sekki_t ji_sekki_data(int sekki);

/* Compose tiered interpretation from Japanese calendar components.
 * era: 0-4, era_year: year within era, rokuyo: 0-5, sekki: 0-23.
 * Invalid era: "?" strings in result. */
japanese_interp_t ji_interpret(int era, int era_year, int rokuyo, int sekki);

/* Returns 5 (number of modern eras). */
int ji_era_count(void);

/* Returns 6 (number of rokuyo). */
int ji_rokuyo_count(void);

/* Returns 24 (number of sekki). */
int ji_sekki_count(void);

#endif /* TIME_JAPANESE_INTERPRET_H */
