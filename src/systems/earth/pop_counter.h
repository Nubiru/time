/* pop_counter.h — Live population counter for per-frame animation.
 * Takes a year + fractional seconds offset and returns a smoothly
 * incrementing population estimate using birth/death rates.
 * Data source: UN 2024 estimates, historical demographic data. */

#ifndef TIME_POP_COUNTER_H
#define TIME_POP_COUNTER_H

#define PC_TEXT_MAX 256

typedef struct {
    double population;           /* current estimate (as exact people count) */
    double births_per_second;    /* ~4.4 births/sec (UN 2024 estimate) */
    double deaths_per_second;    /* ~1.8 deaths/sec */
    double net_per_second;       /* ~2.6 net growth/sec */
    double net_per_day;          /* ~224,640 per day */
    char count_text[PC_TEXT_MAX];   /* "8,100,234,567" (with commas) */
    char rate_text[PC_TEXT_MAX];    /* "Growing by ~2.6 people per second" */
    char daily_text[PC_TEXT_MAX];   /* "~224,640 more people today than yesterday" */
} pc_counter_t;

/* Compute population counter for a given year + fractional seconds offset.
 * seconds_offset: seconds since midnight Jan 1 of ce_year (0 to ~31.5M).
 * The counter increments smoothly by net_per_second. */
pc_counter_t pc_compute(int ce_year, double seconds_offset);

/* Get just the population estimate. */
double pc_estimate(int ce_year, double seconds_offset);

/* Get net growth rate (people per second) for a year. */
double pc_growth_rate(int ce_year);

/* Format the count with comma separators (e.g., "8,100,234,567").
 * Returns chars written. */
int pc_format_count(double population, char *buf, int buf_size);

/* Format full counter as display text. Returns chars written. */
int pc_format(const pc_counter_t *c, char *buf, int buf_size);

#endif /* TIME_POP_COUNTER_H */
