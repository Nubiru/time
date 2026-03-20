/* heartbeat_counter.c — Global heartbeat counter since first humans.
 * Pure functions only. No malloc, no globals, no side effects.
 *
 * Estimates total heartbeats of all humans since ~300,000 years ago.
 * Uses population curve from human_population.h x 72 bpm average.
 *
 * Cumulative total uses pre-computed anchor points with linear
 * interpolation — too expensive to integrate every frame. */

#include "heartbeat_counter.h"
#include "human_population.h"

#include <stdio.h>
#include <string.h>

/* ===== Constants ===== */

#define HB_AVG_BPM          72.0
#define HB_MINUTES_PER_YEAR 525960.0  /* 365.25 * 24 * 60 */
#define HB_SECONDS_PER_YEAR 31557600.0 /* 365.25 * 24 * 3600 */

/* Beats per person per year: 72 * 525960 ≈ 3.787e7 */
#define HB_BEATS_PER_PERSON_YEAR (HB_AVG_BPM * HB_MINUTES_PER_YEAR)

/* ===== Anchor points for cumulative exabeats ===== */

/* Pre-computed cumulative heartbeats at key years.
 * Units: exabeats (1e18 beats).
 * These are rough estimates based on integrating
 * pop_alive(year) * HB_BEATS_PER_PERSON_YEAR over time.
 * Exact numbers matter less than the curve shape. */

#define HB_ANCHOR_COUNT 15

static const int HB_ANCHOR_YEARS[HB_ANCHOR_COUNT] = {
    -300000, -10000, -5000, -1000, 1, 500, 1000,
    1500, 1700, 1800, 1900, 1950, 1975, 2000, 2024
};

static const double HB_ANCHOR_EXABEATS[HB_ANCHOR_COUNT] = {
    0.0, 30.0, 35.0, 50.0, 80.0, 90.0, 100.0,
    130.0, 170.0, 230.0, 400.0, 700.0, 1200.0, 2000.0, 3500.0
};

/* ===== Helper: linear interpolation in anchor table ===== */

static double interp_anchors(int year)
{
    /* Before first anchor */
    if (year <= HB_ANCHOR_YEARS[0]) {
        return HB_ANCHOR_EXABEATS[0];
    }

    /* After last anchor: extrapolate using current rate */
    if (year >= HB_ANCHOR_YEARS[HB_ANCHOR_COUNT - 1]) {
        double last_exa = HB_ANCHOR_EXABEATS[HB_ANCHOR_COUNT - 1];
        int last_year = HB_ANCHOR_YEARS[HB_ANCHOR_COUNT - 1];
        double years_past = (double)(year - last_year);
        /* Use population at last anchor year for extrapolation rate */
        double pop_millions = pop_alive_at_year(last_year);
        /* exabeats per year = pop * 1e6 * beats_per_person_year / 1e18 */
        double exa_per_year = pop_millions * 1.0e6 * HB_BEATS_PER_PERSON_YEAR
                              / 1.0e18;
        return last_exa + years_past * exa_per_year;
    }

    /* Find bracketing interval and interpolate */
    for (int i = 1; i < HB_ANCHOR_COUNT; i++) {
        if (year <= HB_ANCHOR_YEARS[i]) {
            double t = (double)(year - HB_ANCHOR_YEARS[i - 1]) /
                       (double)(HB_ANCHOR_YEARS[i] - HB_ANCHOR_YEARS[i - 1]);
            return HB_ANCHOR_EXABEATS[i - 1] +
                   t * (HB_ANCHOR_EXABEATS[i] - HB_ANCHOR_EXABEATS[i - 1]);
        }
    }

    return HB_ANCHOR_EXABEATS[HB_ANCHOR_COUNT - 1];
}

/* ===== Format helpers ===== */

static void format_total_text(double exabeats, char *buf, int buf_size)
{
    if (exabeats >= 1000.0) {
        /* Use zettabeats (1 zettabeat = 1000 exabeats) */
        double zetta = exabeats / 1000.0;
        snprintf(buf, (unsigned long)buf_size,
                 "~%.1f zettabeats", zetta);
    } else if (exabeats >= 1.0) {
        snprintf(buf, (unsigned long)buf_size,
                 "~%.0f exabeats", exabeats);
    } else {
        /* Use petabeats (1 petabeat = 0.001 exabeats) */
        double peta = exabeats * 1000.0;
        snprintf(buf, (unsigned long)buf_size,
                 "~%.0f petabeats", peta);
    }
}

static void format_rate_text(double bps, char *buf, int buf_size)
{
    if (bps >= 1.0e9) {
        double billions = bps / 1.0e9;
        snprintf(buf, (unsigned long)buf_size,
                 "~%.1f billion heartbeats per second", billions);
    } else if (bps >= 1.0e6) {
        double millions = bps / 1.0e6;
        snprintf(buf, (unsigned long)buf_size,
                 "~%.0f million heartbeats per second", millions);
    } else {
        snprintf(buf, (unsigned long)buf_size,
                 "~%.0f heartbeats per second", bps);
    }
}

static void format_poetic_text(double bps, char *buf, int buf_size)
{
    if (bps >= 1.0e9) {
        double billions = bps / 1.0e9;
        snprintf(buf, (unsigned long)buf_size,
                 "Every second, %.1f billion hearts beat as one",
                 billions);
    } else if (bps >= 1.0e6) {
        double millions = bps / 1.0e6;
        snprintf(buf, (unsigned long)buf_size,
                 "Every second, %.0f million hearts beat as one",
                 millions);
    } else {
        snprintf(buf, (unsigned long)buf_size,
                 "Every second, hearts beat across the Earth");
    }
}

/* ===== Public API ===== */

double hb_beats_per_second(int ce_year)
{
    double pop_millions = pop_alive_at_year(ce_year);
    return pop_millions * 1.0e6 * HB_AVG_BPM / 60.0;
}

double hb_total_exabeats(int ce_year)
{
    return interp_anchors(ce_year);
}

hb_counter_t hb_compute(int ce_year)
{
    hb_counter_t c;
    memset(&c, 0, sizeof(c));

    c.total_exabeats = hb_total_exabeats(ce_year);
    c.beats_per_second = hb_beats_per_second(ce_year);
    c.beats_per_year = c.beats_per_second * HB_SECONDS_PER_YEAR;

    format_total_text(c.total_exabeats, c.total_text, HB_TEXT_MAX);
    format_rate_text(c.beats_per_second, c.rate_text, HB_TEXT_MAX);
    format_poetic_text(c.beats_per_second, c.poetic_text, HB_TEXT_MAX);

    return c;
}

int hb_format(const hb_counter_t *c, char *buf, int buf_size)
{
    if (!c || !buf || buf_size <= 0) {
        return 0;
    }

    return snprintf(buf, (unsigned long)buf_size,
                    "%s | %s", c->total_text, c->rate_text);
}
