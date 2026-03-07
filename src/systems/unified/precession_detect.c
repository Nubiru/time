/* precession_detect.c — Precession Encoder Detector implementation
 *
 * Tests whether ancient time systems encode the axial precession cycle
 * (~25,772 years) through integer multiples of their fundamental cycles.
 *
 * Pure functions only. No globals, no malloc, no side effects. */

#include "precession_detect.h"
#include <math.h>
#include <string.h>

/* Zodiacal age names in precession order from J2000.
 * Precession is retrograde: Pisces -> Aquarius -> Capricorn -> ... */
static const char *const AGE_NAMES[12] = {
    "Pisces",      /* 0  — current at J2000 */
    "Aquarius",    /* 1  — next */
    "Capricorn",   /* 2 */
    "Sagittarius", /* 3 */
    "Scorpio",     /* 4 */
    "Libra",       /* 5 */
    "Virgo",       /* 6 */
    "Leo",         /* 7 */
    "Cancer",      /* 8 */
    "Gemini",      /* 9 */
    "Taurus",      /* 10 */
    "Aries"        /* 11 */
};

/* Pre-registered cultural system definitions.
 * Each entry: { system_name, culture, cycle_length_years, best_multiplier } */
typedef struct {
    const char *system_name;
    const char *culture;
    double cycle_length;
    int best_multiplier;
} precession_seed_t;

static const precession_seed_t SEEDS[] = {
    { "Mayan",      "Mesoamerican", 5125.36,  5   },
    { "Hindu",      "Indian",       2160.0,   12  },
    { "Hebrew",     "Semitic",      50.0,     516 },
    { "Egyptian",   "African",      1461.0,   18  },
    { "Babylonian", "Mesopotamian", 3600.0,   7   },
    { "Greek",      "Hellenic",     25772.0,  1   },
    { "Norse",      "Scandinavian", 540.0,    48  },
    { "Chinese",    "East Asian",   60.0,     430 }
};

static const int SEED_COUNT = (int)(sizeof(SEEDS) / sizeof(SEEDS[0]));

/* Approximate precession angle at J2000 epoch (degrees).
 * The vernal equinox point is at roughly 5 degrees into Pisces. */
static const double J2000_PRECESSION_ANGLE = 5.145;

/* ===== Core functions ===== */

precession_match_t precession_test(double cycle_years, int multiplier)
{
    precession_match_t m;
    m.cycle_length = cycle_years;
    m.multiplier = multiplier;
    m.product = cycle_years * (double)multiplier;
    m.error_years = fabs(m.product - PRECESSION_PERIOD_YEARS);
    m.error_percent = (m.error_years / PRECESSION_PERIOD_YEARS) * 100.0;
    return m;
}

precession_match_t precession_best_match(double cycle_years, int max_mult)
{
    precession_match_t best = precession_test(cycle_years, 1);

    for (int m = 2; m <= max_mult; m++) {
        precession_match_t candidate = precession_test(cycle_years, m);
        if (candidate.error_percent < best.error_percent) {
            best = candidate;
        }
    }

    return best;
}

int precession_is_match(precession_match_t match, double threshold_percent)
{
    return match.error_percent < threshold_percent ? 1 : 0;
}

double precession_degrees(double cycle_years)
{
    return cycle_years * PRECESSION_RATE_DEG_PER_YEAR;
}

double precession_cycles_per_precession(double cycle_years)
{
    if (cycle_years <= 0.0) {
        return 0.0;
    }
    return PRECESSION_PERIOD_YEARS / cycle_years;
}

double precession_angle(double years_from_j2000)
{
    double raw = J2000_PRECESSION_ANGLE + years_from_j2000 * PRECESSION_RATE_DEG_PER_YEAR;
    double angle = fmod(raw, 360.0);
    if (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

int precession_current_age(double years_from_j2000)
{
    double angle = precession_angle(years_from_j2000);
    int age = (int)(angle / 30.0);
    if (age < 0) age = 0;
    if (age > 11) age = 11;
    return age;
}

const char *precession_age_name(int age_index)
{
    if (age_index < 0 || age_index > 11) {
        return "Unknown";
    }
    return AGE_NAMES[age_index];
}

int precession_system_count(void)
{
    return SEED_COUNT;
}

precession_system_t precession_system_get(int index)
{
    precession_system_t sys;
    memset(&sys, 0, sizeof(sys));

    if (index < 0 || index >= SEED_COUNT) {
        sys.system_name = NULL;
        sys.culture = NULL;
        sys.match_count = 0;
        sys.best_error_percent = 100.0;
        return sys;
    }

    const precession_seed_t *seed = &SEEDS[index];
    sys.system_name = seed->system_name;
    sys.culture = seed->culture;

    /* Compute the pre-registered match */
    sys.best = precession_test(seed->cycle_length, seed->best_multiplier);
    sys.best_error_percent = sys.best.error_percent;
    sys.matches[0] = sys.best;
    sys.match_count = 1;

    return sys;
}

precession_report_t precession_full_report(double threshold_percent, int max_mult)
{
    precession_report_t report;
    memset(&report, 0, sizeof(report));

    report.system_count = SEED_COUNT;
    report.best_error_percent = 1e9;
    report.best_system = NULL;
    report.total_matches = 0;

    for (int i = 0; i < SEED_COUNT; i++) {
        const precession_seed_t *seed = &SEEDS[i];
        precession_system_t *sys = &report.systems[i];

        sys->system_name = seed->system_name;
        sys->culture = seed->culture;
        sys->match_count = 0;
        sys->best_error_percent = 1e9;

        /* Search all multipliers for matches within threshold */
        for (int m = 1; m <= max_mult; m++) {
            precession_match_t candidate = precession_test(seed->cycle_length, m);

            if (candidate.error_percent < threshold_percent) {
                if (sys->match_count < PRECESSION_MAX_MATCHES) {
                    sys->matches[sys->match_count] = candidate;
                    sys->match_count++;
                }
                report.total_matches++;
            }

            if (candidate.error_percent < sys->best_error_percent) {
                sys->best = candidate;
                sys->best_error_percent = candidate.error_percent;
            }
        }

        if (sys->best_error_percent < report.best_error_percent) {
            report.best_error_percent = sys->best_error_percent;
            report.best_system = sys->system_name;
        }
    }

    return report;
}

double precession_years_per_degree(void)
{
    return PRECESSION_PERIOD_YEARS / 360.0;
}
