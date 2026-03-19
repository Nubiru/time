/* annual_summary.h — "Your Year in Time" data backbone.
 *
 * Computes a year-in-review summary across time systems for a given
 * birth profile and year. Spotify Wrapped equivalent: highlights,
 * cross-system coincidences, personal milestones, story cards.
 *
 * L3.5 data backbone — story format, screenshot-ready cards.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_ANNUAL_SUMMARY_H
#define TIME_ANNUAL_SUMMARY_H

#include "birth_profile.h"

#define AS_MAX_HIGHLIGHTS  12
#define AS_HEADLINE_MAX    80
#define AS_BODY_MAX        256

/* Highlight categories */
typedef enum {
    AS_CAT_BIRTHDAY = 0,       /* Birthday-related */
    AS_CAT_CYCLE,              /* Cycle completions or transitions */
    AS_CAT_COINCIDENCE,        /* Cross-system echoes */
    AS_CAT_MILESTONE,          /* Personal milestones (age) */
    AS_CAT_JOURNEY,            /* User engagement / exploration */
    AS_CAT_COUNT
} as_category_t;

/* Significance levels */
typedef enum {
    AS_SIG_ROUTINE = 0,        /* Normal yearly occurrence */
    AS_SIG_NOTABLE,            /* Worth highlighting */
    AS_SIG_RARE,               /* Happens every few years */
    AS_SIG_EXTRAORDINARY       /* Once in a lifetime */
} as_significance_t;

/* A single highlight card */
typedef struct {
    char headline[AS_HEADLINE_MAX];
    char body[AS_BODY_MAX];
    as_category_t category;
    as_significance_t significance;
    int month;                 /* Month of occurrence (1-12), 0=year-wide */
} as_highlight_t;

/* Year statistics */
typedef struct {
    int year;
    int age;                   /* Age reached during this year */
    int birthday_kin;          /* Tzolkin kin on birthday (1-260) */
    int birthday_seal;         /* Tzolkin seal on birthday (0-19) */
    int birthday_tone;         /* Tzolkin tone on birthday (1-13) */
    int birthday_hexagram;     /* I Ching hexagram on birthday (1-64) */
    int chinese_animal;        /* Chinese animal for this year (0-11) */
    int chinese_element;       /* Chinese element for this year (0-4) */
    int galactic_returns;      /* Times birth kin recurs during year (1-2) */
    int wavespells;            /* Wavespells traversed (~28) */
    int is_leap;               /* 1 if leap year */
} as_stats_t;

/* User engagement data for the year (optional — pass NULL to as_compute) */
typedef struct {
    int systems_explored;      /* 0-16 */
    int total_visits;          /* Session count */
    int longest_streak;        /* Consecutive days */
    int bookmarks_saved;
    double total_hours;        /* Hours spent in Time */
} as_engagement_t;

/* Complete annual summary */
typedef struct {
    as_stats_t stats;
    as_highlight_t highlights[AS_MAX_HIGHLIGHTS];
    int highlight_count;
    char opening[AS_BODY_MAX]; /* Opening narrative */
    char closing[AS_BODY_MAX]; /* Closing narrative */
} as_summary_t;

/* Create default (zero) engagement data. */
as_engagement_t as_default_engagement(void);

/* Compute annual summary. engagement can be NULL.
 * NULL profile returns empty summary. */
as_summary_t as_compute(const birth_profile_t *profile, int year,
                         const as_engagement_t *engagement);

/* Get highlights sorted by significance (highest first).
 * Returns count written (<= max). 0 for NULL args. */
int as_top_highlights(const as_summary_t *summary,
                      as_highlight_t *out, int max);

/* Get highlights filtered by category.
 * Returns count written (<= max). 0 for NULL args. */
int as_by_category(const as_summary_t *summary,
                   as_category_t category,
                   as_highlight_t *out, int max);

/* Count highlights at or above a significance level. 0 for NULL. */
int as_count_significant(const as_summary_t *summary,
                          as_significance_t min_sig);

/* Get category display name. Returns "?" for invalid. */
const char *as_category_name(as_category_t cat);

/* Get significance display name. Returns "?" for invalid. */
const char *as_significance_name(as_significance_t sig);

/* Format summary as multi-line text.
 * Returns bytes written (excluding NUL). 0 for NULL args. */
int as_format_summary(const as_summary_t *summary, char *buf, int buf_size);

#endif /* TIME_ANNUAL_SUMMARY_H */
