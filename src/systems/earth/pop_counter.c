/* pop_counter.c — Live population counter for per-frame animation.
 * Pure functions only. No malloc, no globals, no side effects.
 * Data source: UN 2024 estimates, historical demographic data. */

#include "pop_counter.h"
#include "human_population.h"

#include <stdio.h>
#include <string.h>

/* ===== Vital Rate Anchor Points ===== */

/* Historical birth/death rates per 1000 people per year.
 * Sources: UN Population Division, Our World in Data. */

#define RATE_ANCHOR_COUNT 4

static const int RATE_YEARS[RATE_ANCHOR_COUNT] = {
    1900, 1950, 2000, 2024
};

static const double BIRTH_RATES[RATE_ANCHOR_COUNT] = {
    40.0, 37.0, 21.0, 18.1  /* per 1000/year */
};

static const double DEATH_RATES[RATE_ANCHOR_COUNT] = {
    20.0, 19.0, 9.0, 7.7    /* per 1000/year */
};

/* ===== Internal: interpolate rate tables ===== */

static double interp_rate(const double *values, int year)
{
    /* Before first anchor: use first value */
    if (year <= RATE_YEARS[0]) {
        return values[0];
    }
    /* After last anchor: use last value */
    if (year >= RATE_YEARS[RATE_ANCHOR_COUNT - 1]) {
        return values[RATE_ANCHOR_COUNT - 1];
    }
    /* Linear interpolation between anchors */
    for (int i = 1; i < RATE_ANCHOR_COUNT; i++) {
        if (year <= RATE_YEARS[i]) {
            double t = (double)(year - RATE_YEARS[i - 1]) /
                       (double)(RATE_YEARS[i] - RATE_YEARS[i - 1]);
            return values[i - 1] + t * (values[i] - values[i - 1]);
        }
    }
    return values[RATE_ANCHOR_COUNT - 1];
}

/* Seconds in a mean year */
#define SECONDS_PER_YEAR (365.25 * 86400.0)

/* ===== Public API ===== */

double pc_growth_rate(int ce_year)
{
    double pop_millions = pop_alive_at_year(ce_year);
    double population = pop_millions * 1e6;

    double birth_rate = interp_rate(BIRTH_RATES, ce_year);
    double death_rate = interp_rate(DEATH_RATES, ce_year);
    double net_rate = birth_rate - death_rate; /* per 1000/year */

    double net_per_second = population * net_rate / 1000.0 / SECONDS_PER_YEAR;
    return net_per_second;
}

double pc_estimate(int ce_year, double seconds_offset)
{
    double pop_millions = pop_alive_at_year(ce_year);
    double base = pop_millions * 1e6;
    double net_per_sec = pc_growth_rate(ce_year);

    return base + seconds_offset * net_per_sec;
}

int pc_format_count(double population, char *buf, int buf_size)
{
    if (buf == NULL || buf_size <= 0) {
        return 0;
    }

    /* Convert to integer (truncate fractional people) */
    long long count = (long long)population;
    if (count < 0) count = 0;

    /* Format without commas into temp buffer first */
    char raw[64];
    int raw_len = snprintf(raw, sizeof(raw), "%lld", count);
    if (raw_len <= 0) {
        buf[0] = '0';
        if (buf_size > 1) buf[1] = '\0';
        return 1;
    }

    /* Calculate output length with commas */
    int commas = (raw_len - 1) / 3;
    int out_len = raw_len + commas;

    if (out_len >= buf_size) {
        /* Truncate: write what fits */
        int avail = buf_size - 1;
        if (avail <= 0) {
            buf[0] = '\0';
            return 0;
        }
        /* Just write truncated digits without commas for safety */
        int copy = avail < raw_len ? avail : raw_len;
        memcpy(buf, raw, (unsigned long)copy);
        buf[copy] = '\0';
        return copy;
    }

    /* Build comma-separated string from right to left */
    buf[out_len] = '\0';
    int src = raw_len - 1;
    int dst = out_len - 1;
    int digit_count = 0;

    while (src >= 0) {
        buf[dst] = raw[src];
        dst--;
        src--;
        digit_count++;
        if (digit_count == 3 && src >= 0) {
            buf[dst] = ',';
            dst--;
            digit_count = 0;
        }
    }

    return out_len;
}

int pc_format(const pc_counter_t *c, char *buf, int buf_size)
{
    if (c == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    return snprintf(buf, (unsigned long)buf_size,
                    "%s | %s | %s",
                    c->count_text, c->rate_text, c->daily_text);
}

pc_counter_t pc_compute(int ce_year, double seconds_offset)
{
    pc_counter_t c;
    memset(&c, 0, sizeof(c));

    double pop_millions = pop_alive_at_year(ce_year);
    double population = pop_millions * 1e6;

    double birth_rate = interp_rate(BIRTH_RATES, ce_year);
    double death_rate = interp_rate(DEATH_RATES, ce_year);

    c.births_per_second = population * birth_rate / 1000.0 / SECONDS_PER_YEAR;
    c.deaths_per_second = population * death_rate / 1000.0 / SECONDS_PER_YEAR;
    c.net_per_second = c.births_per_second - c.deaths_per_second;
    c.net_per_day = c.net_per_second * 86400.0;

    c.population = population + seconds_offset * c.net_per_second;

    /* Format count text */
    pc_format_count(c.population, c.count_text, PC_TEXT_MAX);

    /* Format rate text */
    snprintf(c.rate_text, PC_TEXT_MAX,
             "Growing by ~%.1f people per second", c.net_per_second);

    /* Format daily text */
    char daily_buf[64];
    pc_format_count(c.net_per_day, daily_buf, sizeof(daily_buf));
    snprintf(c.daily_text, PC_TEXT_MAX,
             "~%s more people today than yesterday", daily_buf);

    return c;
}
