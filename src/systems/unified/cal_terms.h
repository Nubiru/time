/* cal_terms.h -- Calendar Term Database.
 *
 * 115 terms spanning 9 calendar/knowledge systems: Gregorian, Tzolkin,
 * Hebrew, Islamic, Chinese, Hindu, Astrology, I Ching, and Human Design.
 * Each term includes native text, transliteration, translation, and
 * ISO 15924 script code.
 *
 * Supports: system-based filtering, kind-based filtering, combined
 * system+kind filtering, and positional lookup.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_CAL_TERMS_H
#define TIME_CAL_TERMS_H

#define CAL_TERM_MAX_RESULTS 32

typedef enum {
    CAL_SYS_GREGORIAN = 0,
    CAL_SYS_TZOLKIN,
    CAL_SYS_HEBREW,
    CAL_SYS_ISLAMIC,
    CAL_SYS_CHINESE,
    CAL_SYS_HINDU,
    CAL_SYS_BUDDHIST,
    CAL_SYS_ICHING,
    CAL_SYS_ASTROLOGY,
    CAL_SYS_HUMAN_DESIGN,
    CAL_SYS_KABBALAH,
    CAL_SYS_GEOLOGY,
    CAL_SYS_CELTIC,
    CAL_SYS_COPTIC,
    CAL_SYS_PERSIAN,
    CAL_SYS_JAPANESE,
    CAL_SYS_COUNT
} cal_system_t;

typedef enum {
    CAL_KIND_MONTH = 0,
    CAL_KIND_DAY,
    CAL_KIND_CYCLE,
    CAL_KIND_ELEMENT,
    CAL_KIND_SIGN,
    CAL_KIND_FESTIVAL,
    CAL_KIND_COUNT
} cal_kind_t;

typedef struct {
    int id;
    cal_system_t system;
    cal_kind_t kind;
    int index;                  /* position within category (month 1-12, seal 1-20, etc.) */
    const char *native;         /* source language text */
    const char *transliterated; /* romanized form */
    const char *translated;     /* English meaning */
    const char *script;         /* ISO 15924 code: "Latn", "Hebr", "Arab", "Hans", etc. */
} cal_term_t;

/* Total number of calendar terms in the database. */
int cal_term_count(void);

/* Get term by index. Returns struct with id=-1 for invalid. */
cal_term_t cal_term_get(int index);

/* Find terms by system. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int cal_terms_by_system(cal_system_t system, int *results, int max_results);

/* Find terms by kind. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int cal_terms_by_kind(cal_kind_t kind, int *results, int max_results);

/* Find terms by system AND kind. Returns count. */
int cal_terms_by_system_kind(cal_system_t system, cal_kind_t kind,
                              int *results, int max_results);

/* Lookup a specific term by system, kind, and 1-based position.
 * Returns struct with id=-1 if not found. */
cal_term_t cal_term_lookup(cal_system_t system, cal_kind_t kind, int position);

/* Get human-readable system name. Returns "Unknown" for invalid. */
const char *cal_system_name(cal_system_t system);

/* Get human-readable kind name. Returns "Unknown" for invalid. */
const char *cal_kind_name(cal_kind_t kind);

#endif /* TIME_CAL_TERMS_H */
