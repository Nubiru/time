/* human_population.h — Human population through history.
 * Provides estimates of total humans ever born, currently alive,
 * and historical population data from 10000 BCE to present.
 * Data source: Population Reference Bureau (2023), UN estimates. */

#ifndef TIME_HUMAN_POPULATION_H
#define TIME_HUMAN_POPULATION_H

#define POP_MILESTONE_COUNT  8
#define POP_DATAPOINT_COUNT  26

/* A population milestone (billion thresholds) */
typedef struct {
    int billions;           /* 1, 2, 3, 4, 5, 6, 7, 8 */
    int year;               /* 1804, 1927, 1960, 1974, 1987, 1999, 2011, 2022 */
    int years_to_next;      /* years until next billion (0 for last) */
    const char *context;    /* "Industrial revolution" */
} pop_milestone_t;

/* A historical population data point */
typedef struct {
    int year;               /* negative for BCE (e.g., -10000 for 10000 BCE) */
    double population;      /* in millions */
    const char *era;        /* "Pre-agriculture", "Roman Empire", etc. */
    const char *note;       /* notable event context */
} pop_datapoint_t;

/* Population summary for a given year */
typedef struct {
    double alive_millions;           /* estimated alive at this year */
    double ever_born_billions;       /* cumulative humans born up to this year */
    int nearest_milestone_billions;  /* closest billion milestone (1-8) */
    const char *era_name;            /* era description */
    char summary[256];               /* formatted summary string */
} pop_summary_t;

/* Estimate population alive at a given year. Negative for BCE.
 * Returns millions. Interpolates between data points. */
double pop_alive_at_year(int year);

/* Estimate cumulative humans ever born up to this year.
 * Returns billions. Based on PRB methodology. */
double pop_ever_born_at_year(int year);

/* Full summary for a given year. */
pop_summary_t pop_summary(int year);

/* Get milestone by index (0 to POP_MILESTONE_COUNT-1).
 * Invalid: billions=0. */
pop_milestone_t pop_milestone_get(int index);

/* Milestone count. */
int pop_milestone_count(void);

/* Get historical data point by index (0 to POP_DATAPOINT_COUNT-1).
 * Invalid: year=0, population=0. */
pop_datapoint_t pop_datapoint_get(int index);

/* Data point count. */
int pop_datapoint_count(void);

/* Format population number for display: "8.1 billion", "300 million".
 * Returns chars written. */
int pop_format_number(double millions, char *buf, int buf_size);

/* Year when world reached N billion. Returns 0 if N is invalid (outside 1-8). */
int pop_year_of_billion(int n);

#endif /* TIME_HUMAN_POPULATION_H */
