/* biorhythm.c — Biological rhythms mapped to astronomical cycles.
 * Pure data + computation. No GL, no malloc, no globals, no side effects. */

#include "biorhythm.h"

/* ===== Constants ===== */

#define DEFAULT_BPM       72.0    /* average resting heart rate */
#define DEFAULT_RPM       15.0    /* average resting breaths per minute */
#define MINUTES_PER_HOUR  60.0
#define HOURS_PER_DAY     24.0
#define DAYS_PER_YEAR     365.25

/* Sacred numbers for matching */
static const int SACRED_NUMBERS[] = {
    1, 7, 12, 13, 22, 27, 36, 64, 72, 108, 260, 360, 432
};
#define SACRED_COUNT (int)(sizeof(SACRED_NUMBERS) / sizeof(SACRED_NUMBERS[0]))

/* ===== Biological Rhythm Data ===== */

static const bio_rhythm_t RHYTHMS[BIORHYTHM_CYCLE_COUNT] = {
    {
        BIO_HEARTBEAT,
        "Heartbeat",
        72.0,
        0.01389,
        "Average resting heart rate"
    },
    {
        BIO_BREATH,
        "Breath",
        15.0,
        0.0667,
        "Average resting respiration"
    },
    {
        BIO_CIRCADIAN,
        "Circadian",
        0.000694,
        24.0,
        "Sleep-wake cycle (Earth rotation)"
    },
    {
        BIO_MENSTRUAL,
        "Menstrual",
        0.0000234,
        708.0,
        "Synodic month alignment"
    },
    {
        BIO_SLEEP_ULTRADIAN,
        "Sleep Cycle",
        0.0111,
        1.5,
        "90-minute REM cycle"
    },
    {
        BIO_BLINK,
        "Blink",
        17.0,
        0.000981,
        "Eye blink rate"
    },
    {
        BIO_CELL_DIVISION,
        "Cell Division",
        0.000694,
        24.0,
        "Typical mitotic cycle"
    },
    {
        BIO_BONE_REMODEL,
        "Bone Remodeling",
        0.0000058,
        2880.0,
        "Osteoclast/osteoblast cycle"
    }
};

/* ===== Bio-Astronomical Correlation Data ===== */

static const bio_correlation_t CORRELATIONS[BIORHYTHM_CORRELATION_COUNT] = {
    {
        "72 bpm heartbeat",
        "72 years per degree of precession",
        72.0,
        72.0,
        1.0,
        "Same number: heartbeat rate = precession rate in years/degree"
    },
    {
        "4,320 beats/hour",
        "432,000 years = Kali Yuga",
        4320.0,
        432000.0,
        100.0,
        "Factor of 100: beats/hour x 100 = Kali Yuga years"
    },
    {
        "21,600 breaths/day",
        "25,920 years = precession cycle",
        21600.0,
        25920.0,
        1.2,
        "Ratio 1.2: 25920/21600 = 1.2 (close to 6/5)"
    },
    {
        "29.5-day menstrual cycle",
        "29.53-day synodic month",
        29.5,
        29.53,
        1.001017,
        "Near-identical: evolved under lunar influence"
    },
    {
        "103,680 beats/day",
        "25,920 x 4 = 103,680",
        103680.0,
        103680.0,
        1.0,
        "Heartbeats/day = 4 x precession cycle"
    },
    {
        "900 breaths/hour",
        "108 beads on mala",
        900.0,
        108.0,
        8.333333,
        "900/108 = 8.33; 108 x 200 = 21,600 breaths/day"
    }
};

/* ===== Public Functions ===== */

bio_rhythm_t biorhythm_get(bio_cycle_t type) {
    if (type >= 0 && type < BIORHYTHM_CYCLE_COUNT) {
        return RHYTHMS[type];
    }
    /* Return heartbeat as safe default */
    return RHYTHMS[0];
}

int biorhythm_cycle_count(void) {
    return BIORHYTHM_CYCLE_COUNT;
}

double biorhythm_heartbeats(double hours) {
    return hours * MINUTES_PER_HOUR * DEFAULT_BPM;
}

double biorhythm_breaths(double hours) {
    return hours * MINUTES_PER_HOUR * DEFAULT_RPM;
}

double biorhythm_beats_per_day(double bpm) {
    return bpm * MINUTES_PER_HOUR * HOURS_PER_DAY;
}

double biorhythm_breaths_per_day(double rate_per_min) {
    return rate_per_min * MINUTES_PER_HOUR * HOURS_PER_DAY;
}

bio_correlation_t biorhythm_correlation_get(int index) {
    if (index >= 0 && index < BIORHYTHM_CORRELATION_COUNT) {
        return CORRELATIONS[index];
    }
    /* Return first correlation as safe default */
    return CORRELATIONS[0];
}

int biorhythm_correlation_count(void) {
    return BIORHYTHM_CORRELATION_COUNT;
}

int biorhythm_match_sacred(double bio_value, double astro_value) {
    if (astro_value == 0.0 || bio_value == 0.0) {
        return 0;
    }

    double ratio = bio_value / astro_value;

    /* If ratio < 1, invert to get ratio >= 1 */
    if (ratio < 1.0) {
        ratio = astro_value / bio_value;
    }

    /* Check each sacred number: within 5% tolerance */
    for (int i = 0; i < SACRED_COUNT; i++) {
        double sacred = (double)SACRED_NUMBERS[i];
        double lower = sacred * 0.95;
        double upper = sacred * 1.05;
        if (ratio >= lower && ratio <= upper) {
            return SACRED_NUMBERS[i];
        }
    }

    return 0;
}

double biorhythm_lifetime_heartbeats(double years) {
    double hours = years * DAYS_PER_YEAR * HOURS_PER_DAY;
    return biorhythm_heartbeats(hours);
}

double biorhythm_lifetime_breaths(double years) {
    double hours = years * DAYS_PER_YEAR * HOURS_PER_DAY;
    return biorhythm_breaths(hours);
}

const char *biorhythm_cycle_name(bio_cycle_t type) {
    if (type >= 0 && type < BIORHYTHM_CYCLE_COUNT) {
        return RHYTHMS[type].name;
    }
    return "Unknown";
}
