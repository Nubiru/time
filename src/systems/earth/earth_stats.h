/* earth_stats.h — Display-ready Earth/human statistics.
 * S91: "Humans have existed for 0.0066% of Earth's history"
 * S92: "7.4% of all humans who ever lived are alive right now"
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Data: Earth age 4.54 Ga, Homo sapiens ~300 ka (geo_time.h knowledge).
 * Population data from human_population.h. */

#ifndef TIME_EARTH_STATS_H
#define TIME_EARTH_STATS_H

#define ES_STAT_MAX    256
#define ES_ANALOGY_MAX 128

/* Human existence fraction (S91) */
typedef struct {
    double earth_age_years;                  /* 4.54e9 */
    double human_age_years;                  /* 300000 */
    double fraction;                         /* ~0.0000661 (0.00661%) */
    double percentage;                       /* ~0.00661 */
    char fraction_text[ES_STAT_MAX];         /* "Humans have existed for 0.0066% of Earth's history" */
    char ruler_analogy[ES_ANALOGY_MAX];      /* "If Earth's history were a 30cm ruler, humans are the last 0.002mm" */
    char day_analogy[ES_ANALOGY_MAX];        /* "If Earth's history were 24 hours, humans appeared 4 seconds ago" */
    char book_analogy[ES_ANALOGY_MAX];       /* "If Earth's history were a 1000-page book, humans appear in the last word" */
} es_existence_t;

/* "You are alive during..." stats (S92) */
typedef struct {
    double alive_billions;                   /* ~8.1 */
    double ever_born_billions;               /* ~109 */
    double alive_fraction;                   /* ~0.074 (7.4%) */
    double alive_percentage;                 /* ~7.4 */
    char alive_text[ES_STAT_MAX];            /* "7.4% of all humans who ever lived are alive right now" */
    char moment_text[ES_STAT_MAX];           /* "You are in the most populated moment in human history" */
    char per_dead_text[ES_STAT_MAX];         /* "For every person alive, ~12 have lived and died before" */
} es_alive_t;

/* Compute existence fraction stats. */
es_existence_t es_existence(void);

/* Compute "alive during" stats for a given year. */
es_alive_t es_alive(int ce_year);

/* Format both stats as a combined display block. Returns chars written.
 * Returns 0 if any pointer is NULL or buf_size <= 0. */
int es_format(const es_existence_t *ex, const es_alive_t *al,
              char *buf, int buf_size);

#endif /* TIME_EARTH_STATS_H */
