/* kalpa.c — Buddhist cosmic time cycle data module
 * Implementation of kalpa hierarchy, antarakalpa lifespan cycles,
 * and mappings from geological/cosmic timescales.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "kalpa.h"

/* Estimated age of the universe in years */
static const double UNIVERSE_AGE_YEARS = 13.8e9;

/* Age of Earth in millions of years (used for geological mapping) */
static const double EARTH_AGE_MA = 4540.0;

/* Antarakalpa lifespan bounds */
static const double MAX_LIFESPAN = 84000.0;
static const double MIN_LIFESPAN = 10.0;

/* Phase data: English names */
static const char *const PHASE_NAMES[KALPA_PHASE_COUNT] = {
    "Formation",
    "Existence",
    "Destruction",
    "Emptiness"
};

/* Phase data: Pali names */
static const char *const PHASE_MEANINGS[KALPA_PHASE_COUNT] = {
    "Vivatta-kappa",
    "Vivatta-tthayi",
    "Samvatta-kappa",
    "Samvatta-tthayi"
};

kalpa_position_t kalpa_position(double cosmic_age)
{
    kalpa_position_t pos;

    /* Clamp negative ages to zero */
    if (cosmic_age < 0.0) {
        cosmic_age = 0.0;
    }

    /* Which maha-kalpa are we in? */
    double maha_kalpa_f = cosmic_age / BUDDHIST_MAHA_KALPA_YEARS;
    pos.maha_kalpa_number = (int)maha_kalpa_f;

    /* Position within current maha-kalpa */
    double within_maha = cosmic_age - (double)pos.maha_kalpa_number * BUDDHIST_MAHA_KALPA_YEARS;
    pos.maha_kalpa_fraction = within_maha / BUDDHIST_MAHA_KALPA_YEARS;

    /* Which phase (kalpa) within the maha-kalpa? */
    double phase_f = within_maha / BUDDHIST_KALPA_YEARS;
    int phase_idx = (int)phase_f;
    if (phase_idx >= BUDDHIST_MAHA_KALPA_KALPAS) {
        phase_idx = BUDDHIST_MAHA_KALPA_KALPAS - 1;
    }
    pos.phase = (kalpa_phase_t)phase_idx;

    /* Position within current phase */
    pos.years_in_phase = within_maha - (double)phase_idx * BUDDHIST_KALPA_YEARS;
    pos.phase_fraction = pos.years_in_phase / BUDDHIST_KALPA_YEARS;

    /* Antarakalpa number within the phase */
    double antara_f = pos.years_in_phase / BUDDHIST_ANTARAKALPA_YEARS;
    pos.antarakalpa_number = (int)antara_f;
    if (pos.antarakalpa_number >= BUDDHIST_KALPA_ANTARAKALPAS) {
        pos.antarakalpa_number = BUDDHIST_KALPA_ANTARAKALPAS - 1;
    }

    return pos;
}

const char *kalpa_phase_name(kalpa_phase_t phase)
{
    if (phase < 0 || phase >= KALPA_PHASE_COUNT) {
        return "?";
    }
    return PHASE_NAMES[phase];
}

const char *kalpa_phase_meaning(kalpa_phase_t phase)
{
    if (phase < 0 || phase >= KALPA_PHASE_COUNT) {
        return "?";
    }
    return PHASE_MEANINGS[phase];
}

antarakalpa_info_t kalpa_antarakalpa_info(int antarakalpa_number,
                                          double fraction_within)
{
    antarakalpa_info_t info;
    info.antarakalpa_number = antarakalpa_number;
    info.max_lifespan_years = MAX_LIFESPAN;
    info.min_lifespan_years = MIN_LIFESPAN;

    /* Clamp fraction */
    if (fraction_within < 0.0) {
        fraction_within = 0.0;
    }
    if (fraction_within > 1.0) {
        fraction_within = 1.0;
    }

    /* Even antarakalpas: increasing. Odd: decreasing. */
    info.is_increasing = (antarakalpa_number % 2 == 0);

    double range = MAX_LIFESPAN - MIN_LIFESPAN;

    if (info.is_increasing) {
        /* 0.0 -> min, 1.0 -> max */
        info.current_lifespan = MIN_LIFESPAN + range * fraction_within;
    } else {
        /* 0.0 -> max, 1.0 -> min */
        info.current_lifespan = MAX_LIFESPAN - range * fraction_within;
    }

    return info;
}

kalpa_position_t kalpa_from_geological(double ma)
{
    /* Convert Ma (millions of years ago) to years since Earth formation.
     * cosmic_age = (EARTH_AGE_MA - ma) * 1e6
     * Clamp: if ma > EARTH_AGE_MA, cosmic_age = 0 */
    double cosmic_age_years;
    if (ma >= EARTH_AGE_MA) {
        cosmic_age_years = 0.0;
    } else {
        cosmic_age_years = (EARTH_AGE_MA - ma) * 1.0e6;
    }
    return kalpa_position(cosmic_age_years);
}

kalpa_position_t kalpa_from_cosmic_age(double years_since_big_bang)
{
    return kalpa_position(years_since_big_bang);
}

int kalpa_phase_count(void)
{
    return KALPA_PHASE_COUNT;
}

double kalpa_phase_duration(void)
{
    return BUDDHIST_KALPA_YEARS;
}

double kalpa_maha_kalpa_duration(void)
{
    return BUDDHIST_MAHA_KALPA_YEARS;
}

double kalpa_antarakalpa_duration(void)
{
    return BUDDHIST_ANTARAKALPA_YEARS;
}

double kalpa_cosmic_ratio(void)
{
    return BUDDHIST_MAHA_KALPA_YEARS / UNIVERSE_AGE_YEARS;
}
