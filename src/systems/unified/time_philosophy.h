/* time_philosophy.h -- Philosophy of Time Database.
 *
 * 30 philosophical perspectives on time spanning 11 traditions: Greek,
 * Christian, Islamic, Indian, Buddhist, Chinese, Jewish, Persian, Mayan,
 * Modern, and Scientific. Each entry is tagged by philosophical theme
 * for algorithmic retrieval.
 *
 * Supports: tradition-based filtering, theme-based filtering, author
 * search (case-insensitive substring), tradition counting, and distinct
 * tradition enumeration.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_TIME_PHILOSOPHY_H
#define TIME_TIME_PHILOSOPHY_H

#define TP_MAX_THEMES 4
#define TP_MAX_RESULTS 16

typedef enum {
    TP_TRADITION_GREEK = 0,
    TP_TRADITION_ROMAN,
    TP_TRADITION_CHRISTIAN,
    TP_TRADITION_ISLAMIC,
    TP_TRADITION_INDIAN,
    TP_TRADITION_BUDDHIST,
    TP_TRADITION_CHINESE,
    TP_TRADITION_MAYAN,
    TP_TRADITION_JEWISH,
    TP_TRADITION_PERSIAN,
    TP_TRADITION_MODERN,
    TP_TRADITION_SCIENTIFIC,
    TP_TRADITION_COUNT
} tp_tradition_t;

typedef enum {
    TP_THEME_NATURE_OF_TIME = 0,
    TP_THEME_CYCLICAL,
    TP_THEME_LINEAR,
    TP_THEME_SUBJECTIVE,
    TP_THEME_COSMIC,
    TP_THEME_SACRED,
    TP_THEME_MEASUREMENT,
    TP_THEME_IMPERMANENCE,
    TP_THEME_COUNT
} tp_theme_t;

typedef struct {
    int id;
    const char *quote;
    const char *author;
    const char *work;           /* source work, NULL if general */
    int year;                   /* CE (negative for BCE) */
    tp_tradition_t tradition;
    tp_theme_t themes[TP_MAX_THEMES];
    int theme_count;
    const char *context;        /* 1-2 sentence significance */
} tp_entry_t;

/* Total number of entries in the database. */
int tp_entry_count(void);

/* Get entry by index. Returns struct with id=-1 for invalid index. */
tp_entry_t tp_entry_get(int index);

/* Find entries by tradition. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int tp_by_tradition(tp_tradition_t tradition, int *results, int max_results);

/* Find entries by theme. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int tp_by_theme(tp_theme_t theme, int *results, int max_results);

/* Find entry by author (case-insensitive substring match).
 * Returns first matching index, or -1 if not found. */
int tp_by_author(const char *author);

/* Get tradition name string. Returns "Unknown" for invalid values. */
const char *tp_tradition_name(tp_tradition_t tradition);

/* Get theme name string. Returns "Unknown" for invalid values. */
const char *tp_theme_name(tp_theme_t theme);

/* Count entries for a given tradition.
 * Returns 0 for invalid tradition values. */
int tp_tradition_count(tp_tradition_t tradition);

/* Count distinct traditions represented in the database. */
int tp_distinct_traditions(void);

#endif /* TIME_TIME_PHILOSOPHY_H */
