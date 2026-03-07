/* seasons.c — Seasonal cycle visualizer data module.
 * Pure computation: season identification, day length, growing season,
 * cardinal dates, temperature deviation, solar declination.
 * All functions are pure (no state, no I/O). */

#include "seasons.h"
#include <math.h>

/* ===== Constants ===== */

#define OBLIQUITY 23.44    /* Earth's axial tilt in degrees */
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)

/* Season boundaries (NH, approximate DOY) */
#define SPRING_START  80   /* Mar 20 — Vernal Equinox */
#define SUMMER_START 172   /* Jun 21 — Summer Solstice */
#define AUTUMN_START 265   /* Sep 22 — Autumnal Equinox */
#define WINTER_START 355   /* Dec 21 — Winter Solstice */

/* Season lengths (days) */
#define SPRING_LENGTH  92  /* 80 to 171 inclusive */
#define SUMMER_LENGTH  93  /* 172 to 264 inclusive */
#define AUTUMN_LENGTH  90  /* 265 to 354 inclusive */
#define WINTER_LENGTH  90  /* 355 to 79 inclusive (wraps) */

/* ===== Static Data ===== */

static const char *SEASON_NAMES[] = {
    "Spring", "Summer", "Autumn", "Winter"
};

static const char *CARDINAL_EVENT_NAMES[] = {
    "Vernal Equinox",
    "Summer Solstice",
    "Autumnal Equinox",
    "Winter Solstice"
};

/* ===== season_name ===== */

const char *season_name(season_id_t season) {
    if (season < 0 || season >= SEASON_COUNT) {
        return "Unknown";
    }
    return SEASON_NAMES[season];
}

/* ===== season_cardinal_dates ===== */

cardinal_dates_t season_cardinal_dates(void) {
    cardinal_dates_t d;
    d.vernal_equinox   = SPRING_START;
    d.summer_solstice  = SUMMER_START;
    d.autumnal_equinox = AUTUMN_START;
    d.winter_solstice  = WINTER_START;
    return d;
}

/* ===== season_solar_declination ===== */

double season_solar_declination(int day_of_year) {
    /* dec = 23.44 * sin(2*PI*(doy - 81) / 365) */
    return OBLIQUITY * sin(2.0 * PI * (day_of_year - 81) / 365.0);
}

/* ===== season_day_length ===== */

double season_day_length(double lat_deg, int day_of_year) {
    double dec_deg = season_solar_declination(day_of_year);
    double lat_rad = lat_deg * DEG_TO_RAD;
    double dec_rad = dec_deg * DEG_TO_RAD;

    /* Hour angle formula: cos(ha) = -tan(lat)*tan(dec) */
    double cos_ha = -tan(lat_rad) * tan(dec_rad);

    /* Clamp for polar regions */
    if (cos_ha <= -1.0) {
        return 24.0; /* Midnight sun */
    }
    if (cos_ha >= 1.0) {
        return 0.0;  /* Polar night */
    }

    double ha = acos(cos_ha); /* radians */
    double hours = (ha / PI) * 24.0;

    return hours;
}

/* ===== Helper: determine NH season from DOY ===== */

static season_id_t nh_season_from_doy(int doy) {
    if (doy >= SPRING_START && doy < SUMMER_START) {
        return SEASON_SPRING;
    } else if (doy >= SUMMER_START && doy < AUTUMN_START) {
        return SEASON_SUMMER;
    } else if (doy >= AUTUMN_START && doy < WINTER_START) {
        return SEASON_AUTUMN;
    } else {
        return SEASON_WINTER;
    }
}

/* Flip season for southern hemisphere: spring<->autumn, summer<->winter */
static season_id_t flip_season(season_id_t s) {
    switch (s) {
        case SEASON_SPRING: return SEASON_AUTUMN;
        case SEASON_SUMMER: return SEASON_WINTER;
        case SEASON_AUTUMN: return SEASON_SPRING;
        case SEASON_WINTER: return SEASON_SUMMER;
        default:            return s;
    }
}

/* Get start DOY and length for an NH season */
static void season_bounds(season_id_t s, int *start, int *length) {
    switch (s) {
        case SEASON_SPRING:
            *start = SPRING_START; *length = SPRING_LENGTH; break;
        case SEASON_SUMMER:
            *start = SUMMER_START; *length = SUMMER_LENGTH; break;
        case SEASON_AUTUMN:
            *start = AUTUMN_START; *length = AUTUMN_LENGTH; break;
        case SEASON_WINTER:
            *start = WINTER_START; *length = WINTER_LENGTH; break;
        default:
            *start = 1; *length = 365; break;
    }
}

/* ===== season_at ===== */

season_info_t season_at(double lat_deg, int day_of_year) {
    season_info_t info;
    int is_south = (lat_deg < 0.0) ? 1 : 0;
    info.is_southern = is_south;

    /* Determine NH season first */
    season_id_t nh_season = nh_season_from_doy(day_of_year);

    /* Flip if southern hemisphere */
    season_id_t actual = is_south ? flip_season(nh_season) : nh_season;
    info.season = actual;
    info.name = season_name(actual);

    /* Compute progress within the NH season boundaries
     * (boundaries are always defined in NH terms) */
    int start, length;
    season_bounds(nh_season, &start, &length);

    /* Day within this NH season (handles winter wrapping) */
    int day_in;
    if (nh_season == SEASON_WINTER) {
        /* Winter wraps: 355-365 then 1-79 */
        if (day_of_year >= WINTER_START) {
            day_in = day_of_year - WINTER_START;
        } else {
            day_in = (365 - WINTER_START) + day_of_year;
        }
    } else {
        day_in = day_of_year - start;
    }

    info.day_in_season = day_in;
    info.season_length = length;
    info.progress = (double)day_in / (double)length;

    /* Clamp progress to [0, 1] */
    if (info.progress < 0.0) info.progress = 0.0;
    if (info.progress > 1.0) info.progress = 1.0;

    return info;
}

/* ===== season_opposite ===== */

season_id_t season_opposite(season_id_t season) {
    return flip_season(season);
}

/* ===== season_summer_hemisphere ===== */

int season_summer_hemisphere(int day_of_year) {
    double dec = season_solar_declination(day_of_year);
    return (dec >= 0.0) ? 1 : -1;
}

/* ===== season_days_to_next_cardinal ===== */

int season_days_to_next_cardinal(int day_of_year, const char **event_name) {
    static const int cardinal_days[] = {
        SPRING_START, SUMMER_START, AUTUMN_START, WINTER_START
    };

    /* Find the next cardinal date strictly after day_of_year */
    for (int i = 0; i < 4; i++) {
        if (cardinal_days[i] > day_of_year) {
            if (event_name) {
                *event_name = CARDINAL_EVENT_NAMES[i];
            }
            return cardinal_days[i] - day_of_year;
        }
    }

    /* Wrap to next year's vernal equinox */
    if (event_name) {
        *event_name = CARDINAL_EVENT_NAMES[0];
    }
    return (365 - day_of_year) + SPRING_START;
}

/* ===== growing_season ===== */

growing_season_t growing_season(double lat_deg) {
    growing_season_t g;
    double abs_lat = fabs(lat_deg);

    /* Determine growing season length from latitude bands */
    int length;
    const char *class_name;

    if (abs_lat <= 10.0) {
        length = 365;
        class_name = "Tropical";
    } else if (abs_lat <= 23.0) {
        /* Interpolate ~365 to ~300 */
        double t = (abs_lat - 10.0) / 13.0;
        length = (int)(365.0 - t * 35.0);
        class_name = "Tropical";
    } else if (abs_lat <= 35.0) {
        /* ~300 to ~240 */
        double t = (abs_lat - 23.0) / 12.0;
        length = (int)(300.0 - t * 60.0);
        class_name = "Long";
    } else if (abs_lat <= 45.0) {
        /* ~240 to ~170 */
        double t = (abs_lat - 35.0) / 10.0;
        length = (int)(240.0 - t * 70.0);
        class_name = "Moderate";
    } else if (abs_lat <= 55.0) {
        /* ~170 to ~120 */
        double t = (abs_lat - 45.0) / 10.0;
        length = (int)(170.0 - t * 50.0);
        class_name = "Short";
    } else if (abs_lat <= 65.0) {
        /* ~120 to ~70 */
        double t = (abs_lat - 55.0) / 10.0;
        length = (int)(120.0 - t * 50.0);
        class_name = "Short";
    } else if (abs_lat <= 75.0) {
        /* ~70 to ~20 */
        double t = (abs_lat - 65.0) / 10.0;
        length = (int)(70.0 - t * 50.0);
        class_name = "Short";
    } else {
        length = 0;
        class_name = "None";
    }

    g.length_days = length;
    g.classification = class_name;

    if (length == 0) {
        g.start_doy = 0;
        g.end_doy = 0;
    } else if (length >= 365) {
        g.start_doy = 1;
        g.end_doy = 365;
        g.length_days = 365;
    } else {
        /* Center the growing season around the summer solstice
         * NH: summer solstice ~day 172
         * SH: summer solstice ~day 355 (winter solstice in NH terms) */
        int mid;
        if (lat_deg >= 0.0) {
            mid = SUMMER_START; /* ~172 */
        } else {
            mid = WINTER_START; /* ~355 */
        }

        int half = length / 2;
        int start = mid - half;
        int end = start + length - 1;

        /* Clamp to valid range 1-365 */
        if (start < 1) start = 1;
        if (end > 365) end = 365;

        g.start_doy = start;
        g.end_doy = end;
        g.length_days = end - start + 1;
    }

    return g;
}

/* ===== season_temp_deviation ===== */

double season_temp_deviation(double lat_deg, int day_of_year) {
    /* amplitude = |lat| * 0.4 degrees C */
    double amplitude = fabs(lat_deg) * 0.4;

    /* Base sinusoidal deviation for northern hemisphere */
    double deviation = amplitude * sin(2.0 * PI * (day_of_year - 81) / 365.0);

    /* Southern hemisphere: invert */
    if (lat_deg < 0.0) {
        deviation = -deviation;
    }

    return deviation;
}
