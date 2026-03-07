/* achievement.c — historical astronomical achievement data implementation.
 * Pure data + lookup. All data is static const. No malloc, no globals. */

#include "achievement.h"
#include <stddef.h>
#include <string.h>
#include <math.h>

/* Category names indexed by achievement_category_t */
static const char *const CATEGORY_NAMES[ACH_CAT_COUNT] = {
    "Orbital",
    "Geometric",
    "Lunar",
    "Planetary",
    "Calendar",
    "Mathematical"
};

/* Pre-computed error values:
 *  0  Eratosthenes:  |39375-40075|=700,       700/40075*100=1.74688
 *  1  Hipparchus yr: |365.2467-365.2422|=0.0045, 0.0045/365.2422*100=0.00123138
 *  2  Hipparchus pr: |46-50.3|=4.3,           4.3/50.3*100=8.54871
 *  3  Ptolemy:       |23.855-23.71|=0.145,    0.145/23.71*100=0.61155
 *  4  Aryabhata ci:  |39968-40075|=107,       107/40075*100=0.26701
 *  5  Aryabhata yr:  |365.25858-365.25636|=0.00222, 0.00222/365.25636*100=0.00060781
 *  6  Aryabhata lu:  |29.5306-29.53059|=0.00001,   0.00001/29.53059*100=0.00003386
 *  7  Brahmagupta:   |29.530582-29.530589|=0.000007, 0.000007/29.530589*100=0.00002370
 *  8  Hillel II:     |29.530594-29.530589|=0.000005, 0.000005/29.530589*100=0.00001693
 *  9  Al-Khwarizmi:  |0.01745-0.017452|=0.000002, 0.000002/0.017452*100=0.01146
 * 10  Maya Venus:    |583.92-583.93|=0.01,    0.01/583.93*100=0.00171258
 * 11  Maya lunar:    |11959.888-11959.889|=0.001, 0.001/11959.889*100=0.00000836
 * 12  Ulugh Beg:     |365.25636-365.25636|=0.0, 0.0/365.25636*100=0.0
 * 13  Su Song:       |365.2436-365.2422|=0.0014, 0.0014/365.2422*100=0.00038331
 * 14  Copernicus:    |365.25671-365.25636|=0.00035, 0.00035/365.25636*100=0.00009582
 * 15  Aristarchus:   |19-389|=370,            370/389*100=95.11568
 * 16  Tycho Brahe:   |1.0-0.017|=0.983,      0.983/0.017*100=5782.35294
 */

static const achievement_t ACHIEVEMENTS[] = {
    /* 0: Eratosthenes — Earth circumference */
    {
        .id = 0,
        .person = "Eratosthenes",
        .year = -240,
        .culture = "Greek",
        .description = "Earth circumference measurement",
        .value_computed = 39375.0,
        .modern_value = 40075.0,
        .unit = "km",
        .absolute_error = 700.0,
        .relative_error_pct = 1.74688,
        .tools_available = "gnomon, pace counting",
        .tools_NOT_available = "telescope, sextant",
        .category = ACH_CAT_GEOMETRIC,
        .significance = "Measured the Earth with a stick and geometry."
    },
    /* 1: Hipparchus — tropical year */
    {
        .id = 1,
        .person = "Hipparchus",
        .year = -130,
        .culture = "Greek",
        .description = "Tropical year length",
        .value_computed = 365.2467,
        .modern_value = 365.2422,
        .unit = "days",
        .absolute_error = 0.0045,
        .relative_error_pct = 0.00123138,
        .tools_available = "armillary sphere, gnomon",
        .tools_NOT_available = "clock, telescope",
        .category = ACH_CAT_ORBITAL,
        .significance = "6.5-minute annual error with naked-eye astronomy."
    },
    /* 2: Hipparchus — precession rate */
    {
        .id = 2,
        .person = "Hipparchus",
        .year = -130,
        .culture = "Greek",
        .description = "Precession of the equinoxes rate",
        .value_computed = 46.0,
        .modern_value = 50.3,
        .unit = "arcsec/year",
        .absolute_error = 4.3,
        .relative_error_pct = 8.54871,
        .tools_available = "star catalogs, armillary sphere",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_ORBITAL,
        .significance = "First to discover the precession of the equinoxes."
    },
    /* 3: Ptolemy — obliquity of ecliptic */
    {
        .id = 3,
        .person = "Ptolemy",
        .year = 150,
        .culture = "Greco-Egyptian",
        .description = "Obliquity of the ecliptic",
        .value_computed = 23.855,
        .modern_value = 23.71,
        .unit = "degrees",
        .absolute_error = 0.145,
        .relative_error_pct = 0.61155,
        .tools_available = "mural quadrant, armillary sphere",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_GEOMETRIC,
        .significance = "Systematized 800 years of Greek astronomical observation."
    },
    /* 4: Aryabhata — Earth circumference */
    {
        .id = 4,
        .person = "Aryabhata",
        .year = 499,
        .culture = "Indian",
        .description = "Earth circumference measurement",
        .value_computed = 39968.0,
        .modern_value = 40075.0,
        .unit = "km",
        .absolute_error = 107.0,
        .relative_error_pct = 0.26701,
        .tools_available = "gnomon, mathematical models",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_GEOMETRIC,
        .significance = "0.27% error — among the most accurate ancient measurements."
    },
    /* 5: Aryabhata — sidereal year */
    {
        .id = 5,
        .person = "Aryabhata",
        .year = 499,
        .culture = "Indian",
        .description = "Sidereal year length",
        .value_computed = 365.25858,
        .modern_value = 365.25636,
        .unit = "days",
        .absolute_error = 0.00222,
        .relative_error_pct = 0.00060781,
        .tools_available = "gnomon, mathematical models",
        .tools_NOT_available = "clock, telescope",
        .category = ACH_CAT_ORBITAL,
        .significance = "3.2-minute annual error, 1000 years before European precision matched this."
    },
    /* 6: Aryabhata — lunar month */
    {
        .id = 6,
        .person = "Aryabhata",
        .year = 499,
        .culture = "Indian",
        .description = "Synodic lunar month",
        .value_computed = 29.5306,
        .modern_value = 29.53059,
        .unit = "days",
        .absolute_error = 0.00001,
        .relative_error_pct = 0.00003386,
        .tools_available = "gnomon, mathematical models",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_LUNAR,
        .significance = "Calculated independently of Greek astronomy."
    },
    /* 7: Brahmagupta — lunar month */
    {
        .id = 7,
        .person = "Brahmagupta",
        .year = 628,
        .culture = "Indian",
        .description = "Synodic lunar month",
        .value_computed = 29.530582,
        .modern_value = 29.530589,
        .unit = "days",
        .absolute_error = 0.000007,
        .relative_error_pct = 0.00002370,
        .tools_available = "gnomon, mathematical models",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_LUNAR,
        .significance = "0.6-second error per month — jaw-dropping pre-telescope accuracy."
    },
    /* 8: Hillel II — molad (mean lunar month) */
    {
        .id = 8,
        .person = "Hillel II",
        .year = 359,
        .culture = "Jewish",
        .description = "Molad (mean lunar month)",
        .value_computed = 29.530594,
        .modern_value = 29.530589,
        .unit = "days",
        .absolute_error = 0.000005,
        .relative_error_pct = 0.00001693,
        .tools_available = "naked eye, mathematical deduction",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_CALENDAR,
        .significance = "0.4-second monthly error — the fixed Hebrew calendar still used today."
    },
    /* 9: Al-Khwarizmi — sine of 1 degree */
    {
        .id = 9,
        .person = "Al-Khwarizmi",
        .year = 820,
        .culture = "Arab",
        .description = "Sine of 1 degree",
        .value_computed = 0.01745,
        .modern_value = 0.017452,
        .unit = "ratio",
        .absolute_error = 0.000002,
        .relative_error_pct = 0.01146,
        .tools_available = "Hindu numeral system, geometric methods",
        .tools_NOT_available = "calculator, telescope",
        .category = ACH_CAT_MATHEMATICAL,
        .significance = "Algorithms so reliable they were used for 400 years."
    },
    /* 10: Maya — Venus synodic period */
    {
        .id = 10,
        .person = "Maya astronomers",
        .year = 800,
        .culture = "Maya",
        .description = "Venus synodic period",
        .value_computed = 583.92,
        .modern_value = 583.93,
        .unit = "days",
        .absolute_error = 0.01,
        .relative_error_pct = 0.00171258,
        .tools_available = "naked eye, horizon observations",
        .tools_NOT_available = "telescope, clock, metal instruments",
        .category = ACH_CAT_PLANETARY,
        .significance = "2-hour error per 584-day cycle — from a stone-age civilization."
    },
    /* 11: Maya — lunar period over 405 lunations */
    {
        .id = 11,
        .person = "Maya astronomers",
        .year = 800,
        .culture = "Maya",
        .description = "Lunar period over 405 lunations",
        .value_computed = 11959.888,
        .modern_value = 11959.889,
        .unit = "days",
        .absolute_error = 0.001,
        .relative_error_pct = 0.00000836,
        .tools_available = "naked eye, horizon markers",
        .tools_NOT_available = "telescope, clock, metal instruments",
        .category = ACH_CAT_LUNAR,
        .significance = "Eclipse prediction accurate to hours over 32.8 years."
    },
    /* 12: Ulugh Beg — sidereal year */
    {
        .id = 12,
        .person = "Ulugh Beg",
        .year = 1437,
        .culture = "Timurid",
        .description = "Sidereal year length",
        .value_computed = 365.25636,
        .modern_value = 365.25636,
        .unit = "days",
        .absolute_error = 0.0,
        .relative_error_pct = 0.0,
        .tools_available = "40-meter sextant at Samarkand Observatory",
        .tools_NOT_available = "telescope",
        .category = ACH_CAT_ORBITAL,
        .significance = "Essentially exact — achieved with a giant sextant, no telescope."
    },
    /* 13: Su Song — tropical year */
    {
        .id = 13,
        .person = "Su Song",
        .year = 1088,
        .culture = "Chinese",
        .description = "Tropical year length",
        .value_computed = 365.2436,
        .modern_value = 365.2422,
        .unit = "days",
        .absolute_error = 0.0014,
        .relative_error_pct = 0.00038331,
        .tools_available = "water-powered armillary sphere, clock tower",
        .tools_NOT_available = "telescope",
        .category = ACH_CAT_ORBITAL,
        .significance = "2-minute error using the world's first astronomical clock tower."
    },
    /* 14: Copernicus — sidereal year */
    {
        .id = 14,
        .person = "Copernicus",
        .year = 1543,
        .culture = "Polish",
        .description = "Sidereal year length",
        .value_computed = 365.25671,
        .modern_value = 365.25636,
        .unit = "days",
        .absolute_error = 0.00035,
        .relative_error_pct = 0.00009582,
        .tools_available = "triquetrum, parallactic ruler",
        .tools_NOT_available = "telescope",
        .category = ACH_CAT_ORBITAL,
        .significance = "Reframed the solar system, but Ulugh Beg had a better year measurement."
    },
    /* 15: Aristarchus — Earth-Sun distance ratio */
    {
        .id = 15,
        .person = "Aristarchus",
        .year = -270,
        .culture = "Greek",
        .description = "Earth-Sun distance as multiple of lunar distance",
        .value_computed = 19.0,
        .modern_value = 389.0,
        .unit = "lunar distances",
        .absolute_error = 370.0,
        .relative_error_pct = 95.11568,
        .tools_available = "naked eye, geometry",
        .tools_NOT_available = "telescope, clock",
        .category = ACH_CAT_GEOMETRIC,
        .significance = "Method was correct even though the measurement was 20x off."
    },
    /* 16: Tycho Brahe — star position accuracy */
    {
        .id = 16,
        .person = "Tycho Brahe",
        .year = 1590,
        .culture = "Danish",
        .description = "Star position accuracy",
        .value_computed = 1.0,
        .modern_value = 0.017,
        .unit = "arcminutes",
        .absolute_error = 0.983,
        .relative_error_pct = 5782.35294,
        .tools_available = "mural quadrant, sextants",
        .tools_NOT_available = "telescope",
        .category = ACH_CAT_GEOMETRIC,
        .significance = "Most accurate naked-eye observations ever recorded."
    }
};

static const int ACHIEVEMENT_COUNT =
    (int)(sizeof(ACHIEVEMENTS) / sizeof(ACHIEVEMENTS[0]));

/* Distinct cultures, deduplicated from the data table */
static const char *const CULTURES[] = {
    "Arab",
    "Chinese",
    "Danish",
    "Greco-Egyptian",
    "Greek",
    "Indian",
    "Jewish",
    "Maya",
    "Polish",
    "Timurid"
};

static const int CULTURE_COUNT =
    (int)(sizeof(CULTURES) / sizeof(CULTURES[0]));

/* Distinct person count (deduplicated from the data table) */
static const int PERSON_COUNT = 13;

/* ===== Public API ===== */

int achievement_count(void)
{
    return ACHIEVEMENT_COUNT;
}

achievement_t achievement_get(int index)
{
    if (index < 0 || index >= ACHIEVEMENT_COUNT) {
        achievement_t invalid = {
            .id = -1,
            .person = NULL,
            .year = 0,
            .culture = NULL,
            .description = NULL,
            .value_computed = 0.0,
            .modern_value = 0.0,
            .unit = NULL,
            .absolute_error = 0.0,
            .relative_error_pct = 0.0,
            .tools_available = NULL,
            .tools_NOT_available = NULL,
            .category = ACH_CAT_COUNT,
            .significance = NULL
        };
        return invalid;
    }
    return ACHIEVEMENTS[index];
}

int achievement_by_culture(const char *culture, int *out_indices, int out_max)
{
    if (culture == NULL || out_indices == NULL || out_max <= 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT && found < out_max; i++) {
        if (strcmp(ACHIEVEMENTS[i].culture, culture) == 0) {
            out_indices[found] = i;
            found++;
        }
    }
    return found;
}

int achievement_by_person(const char *person, int *out_indices, int out_max)
{
    if (person == NULL || out_indices == NULL || out_max <= 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT && found < out_max; i++) {
        if (strcmp(ACHIEVEMENTS[i].person, person) == 0) {
            out_indices[found] = i;
            found++;
        }
    }
    return found;
}

int achievement_by_category(achievement_category_t cat, int *out_indices, int out_max)
{
    if (out_indices == NULL || out_max <= 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < ACHIEVEMENT_COUNT && found < out_max; i++) {
        if (ACHIEVEMENTS[i].category == cat) {
            out_indices[found] = i;
            found++;
        }
    }
    return found;
}

double achievement_error_ratio(int index)
{
    if (index < 0 || index >= ACHIEVEMENT_COUNT) {
        return 0.0;
    }
    double abs_err = ACHIEVEMENTS[index].absolute_error;
    if (abs_err == 0.0) {
        return 0.0;
    }
    return fabs(ACHIEVEMENTS[index].modern_value) / abs_err;
}

double achievement_accuracy_pct(int index)
{
    if (index < 0 || index >= ACHIEVEMENT_COUNT) {
        return 0.0;
    }
    return 100.0 - ACHIEVEMENTS[index].relative_error_pct;
}

const char *achievement_category_name(achievement_category_t cat)
{
    if ((int)cat < 0 || (int)cat >= ACH_CAT_COUNT) {
        return "?";
    }
    return CATEGORY_NAMES[(int)cat];
}

int achievement_culture_count(void)
{
    return CULTURE_COUNT;
}

const char *achievement_culture_get(int index)
{
    if (index < 0 || index >= CULTURE_COUNT) {
        return "?";
    }
    return CULTURES[index];
}

int achievement_person_count(void)
{
    return PERSON_COUNT;
}
