/* calendar_reform.c — historical calendar systems and reform proposals.
 * Pure data + computation. All data is static const. No malloc, no globals. */

#include "calendar_reform.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

/* ===== Static data: calendar systems ===== */

#define SYSTEM_COUNT_VAL 10

static const calendar_system_t SYSTEMS[SYSTEM_COUNT_VAL] = {
    /* 0: Egyptian Civil Calendar */
    {
        .id = 0,
        .name = "Egyptian Civil Calendar",
        .year = -3000,
        .originator = "Egyptian priesthood",
        .culture = "Egyptian",
        .type = CAL_TYPE_SOLAR,
        .days_per_year = 360,
        .months = 12,
        .intercalary_days = 5,
        .month_structure = "12x30+5",
        .description = "First solar calendar. 12 months of 30 days plus "
                        "5 epagomenal (extra) days. No leap year correction.",
        .problem_solved = "Replaced pure lunar calendar with predictable solar calendar",
        .weakness = "Drifted 1 day every 4 years — no leap year mechanism"
    },
    /* 1: Babylonian Calendar */
    {
        .id = 1,
        .name = "Babylonian Calendar",
        .year = -2000,
        .originator = "Babylonian astronomers",
        .culture = "Babylonian",
        .type = CAL_TYPE_LUNISOLAR,
        .days_per_year = 354,
        .months = 12,
        .intercalary_days = 30,
        .month_structure = "12x29-30+intercalary",
        .description = "Lunisolar calendar. 12 months alternating 29-30 days. "
                        "Intercalary 13th month added by royal decree to keep "
                        "seasons aligned.",
        .problem_solved = "Tracked both lunar phases and solar seasons",
        .weakness = "Intercalation was irregular and politically motivated"
    },
    /* 2: Roman Republican Calendar */
    {
        .id = 2,
        .name = "Roman Republican Calendar",
        .year = -713,
        .originator = "Numa Pompilius",
        .culture = "Roman",
        .type = CAL_TYPE_LUNISOLAR,
        .days_per_year = 355,
        .months = 12,
        .intercalary_days = 22,
        .month_structure = "12 unequal (28-31)",
        .description = "Originally 10 months (304 days), reformed to 12 months. "
                        "Pontifex maximus inserted intercalary month — used as "
                        "political tool.",
        .problem_solved = "Extended calendar to full year",
        .weakness = "Intercalation was political — pontiffs extended terms of "
                     "allies, shortened enemies'"
    },
    /* 3: Julian Calendar */
    {
        .id = 3,
        .name = "Julian Calendar",
        .year = -46,
        .originator = "Julius Caesar / Sosigenes of Alexandria",
        .culture = "Roman",
        .type = CAL_TYPE_ADOPTED,
        .days_per_year = 365,
        .months = 12,
        .intercalary_days = 1,
        .month_structure = "12 unequal (28-31)",
        .description = "365 days with leap year every 4 years. Average year = "
                        "365.25 days. Reformed the chaotic Roman calendar.",
        .problem_solved = "Eliminated political manipulation of intercalation",
        .weakness = "11.5 minutes too long per year — accumulated 10 days of "
                     "error by 1582"
    },
    /* 4: Gregorian Calendar */
    {
        .id = 4,
        .name = "Gregorian Calendar",
        .year = 1582,
        .originator = "Pope Gregory XIII / Aloysius Lilius",
        .culture = "European",
        .type = CAL_TYPE_ADOPTED,
        .days_per_year = 365,
        .months = 12,
        .intercalary_days = 1,
        .month_structure = "12 unequal (28-31)",
        .description = "Leap year every 4 years, except centuries not divisible "
                        "by 400. 10 days deleted (Oct 4 -> Oct 15, 1582). "
                        "Average year = 365.2425 days.",
        .problem_solved = "Fixed Julian drift — Easter back to its proper date",
        .weakness = "Months still unequal length. 26 seconds/year too long. "
                     "Protestant nations refused for centuries."
    },
    /* 5: French Republican Calendar */
    {
        .id = 5,
        .name = "French Republican Calendar",
        .year = 1793,
        .originator = "French National Convention",
        .culture = "French",
        .type = CAL_TYPE_ADOPTED,
        .days_per_year = 360,
        .months = 12,
        .intercalary_days = 5,
        .month_structure = "12x30+5-6",
        .description = "12 months of 30 days + 5-6 complementary days. Decimal "
                        "time: 10 hours/day, 100 minutes/hour. Months named "
                        "after nature.",
        .problem_solved = "Rationalized calendar with equal months and decimal time",
        .weakness = "Incompatible with 7-day week and international trade. "
                     "Abolished 1805."
    },
    /* 6: 13 Moon Calendar */
    {
        .id = 6,
        .name = "13 Moon Calendar",
        .year = 1992,
        .originator = "Jose Arguelles",
        .culture = "Global",
        .type = CAL_TYPE_PROPOSED,
        .days_per_year = 364,
        .months = 13,
        .intercalary_days = 1,
        .month_structure = "13x28+1",
        .description = "13 months of exactly 28 days + 1 'Day Out of Time' "
                        "(July 25). Perfect symmetry: every month starts on "
                        "the same day of the week.",
        .problem_solved = "Eliminates the disorder of unequal months. Aligns "
                          "with 28-day biological cycles.",
        .weakness = "Never adopted by any nation. 'Day Out of Time' complicates "
                     "week continuity."
    },
    /* 7: World Calendar */
    {
        .id = 7,
        .name = "World Calendar",
        .year = 1930,
        .originator = "Elisabeth Achelis",
        .culture = "International",
        .type = CAL_TYPE_PROPOSED,
        .days_per_year = 364,
        .months = 12,
        .intercalary_days = 1,
        .month_structure = "4x(31+30+30)+1",
        .description = "4 identical quarters of 91 days (31+30+30). 1 extra "
                        "'Worldsday' (Dec 31) outside the week. League of "
                        "Nations nearly adopted 1937.",
        .problem_solved = "Every date falls on the same weekday every year",
        .weakness = "Worldsday breaks the 7-day week cycle — religious "
                     "opposition killed it."
    },
    /* 8: International Fixed Calendar */
    {
        .id = 8,
        .name = "International Fixed Calendar",
        .year = 1902,
        .originator = "Moses Cotsworth",
        .culture = "International",
        .type = CAL_TYPE_PROPOSED,
        .days_per_year = 364,
        .months = 13,
        .intercalary_days = 1,
        .month_structure = "13x28+1",
        .description = "13 months of 28 days + Year Day. 13th month 'Sol' "
                        "between June and July. Kodak used it internally "
                        "from 1928-1989.",
        .problem_solved = "Perfect month equality — every month identical, "
                          "every year identical",
        .weakness = "13 is not divisible by 2 or 4 — impossible to divide "
                     "year into halves or quarters."
    },
    /* 9: Seasonal Hours */
    {
        .id = 9,
        .name = "Seasonal Hours",
        .year = -3000,
        .originator = "Ancient civilizations",
        .culture = "Universal",
        .type = CAL_TYPE_SOLAR,
        .days_per_year = 365,
        .months = 12,
        .intercalary_days = 0,
        .month_structure = "variable",
        .description = "Day divided into 12 hours of daylight + 12 hours of "
                        "night. Hour length varies by season and latitude. "
                        "Used in Rome, Japan (until 1873), Jewish law.",
        .problem_solved = "Natural time: work hours scale with available daylight",
        .weakness = "Clocks become impossible — every hour has different length "
                     "depending on date and location."
    }
};

/* Year lengths (exact, as double) for each system, indexed same as SYSTEMS */
static const double YEAR_LENGTHS[SYSTEM_COUNT_VAL] = {
    365.0,      /* Egyptian: 360 + 5 epagomenal */
    354.0,      /* Babylonian: base without intercalation */
    355.0,      /* Roman Republican */
    365.25,     /* Julian: leap every 4 years */
    365.2425,   /* Gregorian */
    365.0,      /* French Republican: 360 + 5-6 */
    365.0,      /* 13 Moon: 364 + 1 Day Out of Time */
    365.0,      /* World Calendar: 364 + 1 Worldsday */
    365.0,      /* IFC: 364 + 1 Year Day */
    365.25      /* Seasonal Hours: same as Julian solar year */
};

/* Distinct cultures, deduplicated and sorted */
static const char *const CULTURES[] = {
    "Babylonian",
    "Egyptian",
    "European",
    "French",
    "Global",
    "International",
    "Roman",
    "Universal"
};

#define CULTURE_COUNT 8

/* Type name strings */
static const char *const TYPE_NAMES[CAL_TYPE_COUNT] = {
    "Lunar",
    "Solar",
    "Lunisolar",
    "Proposed",
    "Adopted"
};

/* Gregorian month days (index 0 unused, months 1-12) */
static const int MONTH_DAYS[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* ===== Public API ===== */

int calendar_system_count(void)
{
    return SYSTEM_COUNT_VAL;
}

calendar_system_t calendar_system_get(int index)
{
    if (index < 0 || index >= SYSTEM_COUNT_VAL) {
        calendar_system_t invalid = {
            .id = -1,
            .name = NULL,
            .year = 0,
            .originator = NULL,
            .culture = NULL,
            .type = CAL_TYPE_COUNT,
            .days_per_year = 0,
            .months = 0,
            .intercalary_days = 0,
            .month_structure = NULL,
            .description = NULL,
            .problem_solved = NULL,
            .weakness = NULL
        };
        return invalid;
    }
    return SYSTEMS[index];
}

int calendar_systems_by_type(calendar_type_t type, int *out_indices, int out_max)
{
    int found = 0;
    for (int i = 0; i < SYSTEM_COUNT_VAL && found < out_max; i++) {
        if (SYSTEMS[i].type == type) {
            out_indices[found] = i;
            found++;
        }
    }
    return found;
}

const char *calendar_type_name(calendar_type_t type)
{
    if (type < 0 || type >= CAL_TYPE_COUNT) {
        return "?";
    }
    return TYPE_NAMES[type];
}

int calendar_culture_count(void)
{
    return CULTURE_COUNT;
}

const char *calendar_culture_get(int index)
{
    if (index < 0 || index >= CULTURE_COUNT) {
        return NULL;
    }
    return CULTURES[index];
}

int gregorian_month_days(int month, int is_leap)
{
    if (month < 1 || month > 12) {
        return 0;
    }
    if (month == 2 && is_leap) {
        return 29;
    }
    return MONTH_DAYS[month];
}

double calendar_year_length(int index)
{
    if (index < 0 || index >= SYSTEM_COUNT_VAL) {
        return 0.0;
    }
    return YEAR_LENGTHS[index];
}

double calendar_day_length(double lat_deg, int day_of_year)
{
    /* Solar declination (degrees) */
    double dec_deg = 23.44 * sin(2.0 * PI * (day_of_year - 81) / 365.0);
    double dec_rad = dec_deg * PI / 180.0;
    double lat_rad = lat_deg * PI / 180.0;

    /* Hour angle at sunset: cos(ha) = -tan(lat) * tan(dec) */
    double cos_ha = -tan(lat_rad) * tan(dec_rad);

    /* Clamp for polar day/night */
    if (cos_ha < -1.0) {
        /* Polar day — sun never sets */
        return 24.0;
    }
    if (cos_ha > 1.0) {
        /* Polar night — sun never rises */
        return 0.0;
    }

    double ha = acos(cos_ha);
    double day_hours = 2.0 * ha * 12.0 / PI;

    return day_hours;
}

double calendar_night_length(double lat_deg, int day_of_year)
{
    return 24.0 - calendar_day_length(lat_deg, day_of_year);
}

double seasonal_hour_length(double lat_deg, int day_of_year, int is_day_hour)
{
    double day_hours = calendar_day_length(lat_deg, day_of_year);
    double night_hours = 24.0 - day_hours;

    if (is_day_hour) {
        /* Each of 12 seasonal day hours = day_hours / 12 (in hours) * 60 (to minutes) */
        return day_hours / 12.0 * 60.0;
    } else {
        return night_hours / 12.0 * 60.0;
    }
}
