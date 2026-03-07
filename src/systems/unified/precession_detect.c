/* precession_detect.c -- Precession Encoder Detector implementation
 *
 * Tests whether ancient time systems encode the axial precession cycle
 * (~25,772 years) through integer multiples of their fundamental cycles.
 *
 * Pure functions only. No globals, no malloc, no side effects. */

#include "precession_detect.h"
#include <math.h>
#include <string.h>

/* Static lookup table of known cultural precession encodings */
static const pd_cycle_t CYCLES[] = {
    { "Long Count Great Cycle", "Mayan",      5125.36,  5    },
    { "Precessional Age",       "Hindu",      2160.0,   12   },
    { "Jubilee-Month cycle",    "Hebrew",     600.0,    43   },
    { "Sothic cycle",           "Egyptian",   1461.0,   18   },
    { "Saros cycle",            "Babylonian", 18.03,    1430 },
    { "Sexagenary cycle",       "Chinese",    60.0,     429  },
    { "Great Year (Plato)",     "Greek",      25920.0,  1    },
    { "Kalpa subdivision",      "Buddhist",   4320.0,   6    },
    { "30-year Hijri cycle",    "Islamic",    30.0,     859  }
};

static const int CYCLE_COUNT = (int)(sizeof(CYCLES) / sizeof(CYCLES[0]));

int pd_cycle_count(void)
{
    return CYCLE_COUNT;
}

pd_cycle_t pd_cycle_get(int index)
{
    if (index < 0 || index >= CYCLE_COUNT) {
        pd_cycle_t empty;
        memset(&empty, 0, sizeof(empty));
        empty.name = NULL;
        empty.culture = NULL;
        empty.period_years = 0.0;
        empty.known_multiplier = 0;
        return empty;
    }
    return CYCLES[index];
}

double pd_error(double product_years)
{
    return fabs(product_years - PRECESSION_CYCLE_YEARS) / PRECESSION_CYCLE_YEARS * 100.0;
}

int pd_best_multiplier(double period_years)
{
    if (period_years <= 0.0) {
        return 1;
    }

    /* Start from rounded estimate, then check nearby values */
    double ratio = PRECESSION_CYCLE_YEARS / period_years;
    int center = (int)round(ratio);

    /* Clamp to search range 1-1000 */
    if (center < 1) center = 1;
    if (center > 1000) center = 1000;

    int best = center;
    double best_err = fabs(period_years * (double)center - PRECESSION_CYCLE_YEARS);

    /* Check neighbors within a small window */
    int lo = center - 2;
    int hi = center + 2;
    if (lo < 1) lo = 1;
    if (hi > 1000) hi = 1000;

    for (int m = lo; m <= hi; m++) {
        double err = fabs(period_years * (double)m - PRECESSION_CYCLE_YEARS);
        if (err < best_err) {
            best_err = err;
            best = m;
        }
    }

    return best;
}

pd_match_t pd_test(double period_years, int multiplier,
                   const char *name, const char *culture)
{
    pd_match_t m;
    memset(&m, 0, sizeof(m));

    m.cycle_name = name;
    m.culture = culture;
    m.period_years = period_years;

    if (multiplier == 0) {
        multiplier = pd_best_multiplier(period_years);
    }

    m.multiplier = multiplier;
    m.product_years = period_years * (double)multiplier;
    m.error_percent = pd_error(m.product_years);
    m.rank = 0;

    return m;
}

double pd_degrees_per_year(void)
{
    return 360.0 / PRECESSION_CYCLE_YEARS;
}

double pd_years_per_degree(void)
{
    return PRECESSION_CYCLE_YEARS / 360.0;
}

/* Simple insertion sort for matches by error_percent ascending */
static void sort_matches(pd_match_t *matches, int count)
{
    for (int i = 1; i < count; i++) {
        pd_match_t key = matches[i];
        int j = i - 1;
        while (j >= 0 && matches[j].error_percent > key.error_percent) {
            matches[j + 1] = matches[j];
            j--;
        }
        matches[j + 1] = key;
    }
}

pd_report_t pd_report(void)
{
    pd_report_t r;
    memset(&r, 0, sizeof(r));

    r.match_count = 0;
    r.best_error_percent = 1e9;
    r.best_culture = NULL;

    /* Test each registered cycle with its known multiplier */
    for (int i = 0; i < CYCLE_COUNT && r.match_count < PRECESSION_MAX_MATCHES; i++) {
        const pd_cycle_t *c = &CYCLES[i];
        pd_match_t m = pd_test(c->period_years, c->known_multiplier,
                               c->name, c->culture);
        r.matches[r.match_count] = m;
        r.match_count++;
    }

    /* Sort by error_percent ascending */
    sort_matches(r.matches, r.match_count);

    /* Assign ranks (1-based) */
    for (int i = 0; i < r.match_count; i++) {
        r.matches[i].rank = i + 1;
    }

    /* Set best */
    if (r.match_count > 0) {
        r.best_error_percent = r.matches[0].error_percent;
        r.best_culture = r.matches[0].culture;
    }

    return r;
}

pd_match_t pd_report_rank(const pd_report_t *report, int rank)
{
    if (rank < 1 || rank > report->match_count) {
        pd_match_t empty;
        memset(&empty, 0, sizeof(empty));
        empty.cycle_name = NULL;
        empty.culture = NULL;
        empty.rank = 0;
        return empty;
    }
    return report->matches[rank - 1];
}

int pd_cultures_within(const pd_report_t *report, double max_error_percent)
{
    int count = 0;
    for (int i = 0; i < report->match_count; i++) {
        if (report->matches[i].error_percent < max_error_percent) {
            count++;
        }
    }
    return count;
}
