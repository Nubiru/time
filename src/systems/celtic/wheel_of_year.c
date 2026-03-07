/* wheel_of_year.c — Celtic/Pagan Wheel of the Year implementation
 * Pure functions, no malloc, no globals, no side effects. */

#include "wheel_of_year.h"
#include <math.h>

/* Static festival data table.
 * Ordered by enum value: Samhain(0) through Mabon(7).
 * Solar longitudes in ecliptic order: 225, 270, 315, 0, 45, 90, 135, 180. */
static const wheel_festival_info_t FESTIVALS[WHEEL_FESTIVAL_COUNT] = {
    {
        .id = WHEEL_SAMHAIN,
        .name = "Samhain",
        .celtic_name = "Samhain",
        .solar_longitude = 225.0,
        .meaning = "Celtic New Year, veil between worlds thinnest",
        .element = WHEEL_ELEMENT_EARTH,
        .direction = WHEEL_DIR_NORTH,
        .is_solar_event = false,
        .approx_month = 11,
        .approx_day = 1
    },
    {
        .id = WHEEL_YULE,
        .name = "Yule",
        .celtic_name = "Alban Arthan",
        .solar_longitude = 270.0,
        .meaning = "Rebirth of the sun",
        .element = WHEEL_ELEMENT_EARTH,
        .direction = WHEEL_DIR_NORTH,
        .is_solar_event = true,
        .approx_month = 12,
        .approx_day = 21
    },
    {
        .id = WHEEL_IMBOLC,
        .name = "Imbolc",
        .celtic_name = "Imbolc",
        .solar_longitude = 315.0,
        .meaning = "First stirrings of spring, Brigid's festival",
        .element = WHEEL_ELEMENT_AIR,
        .direction = WHEEL_DIR_EAST,
        .is_solar_event = false,
        .approx_month = 2,
        .approx_day = 1
    },
    {
        .id = WHEEL_OSTARA,
        .name = "Ostara",
        .celtic_name = "Alban Eiler",
        .solar_longitude = 0.0,
        .meaning = "Balance of light and dark",
        .element = WHEEL_ELEMENT_AIR,
        .direction = WHEEL_DIR_EAST,
        .is_solar_event = true,
        .approx_month = 3,
        .approx_day = 20
    },
    {
        .id = WHEEL_BELTANE,
        .name = "Beltane",
        .celtic_name = "Beltane",
        .solar_longitude = 45.0,
        .meaning = "Fire festival, summer begins",
        .element = WHEEL_ELEMENT_FIRE,
        .direction = WHEEL_DIR_SOUTH,
        .is_solar_event = false,
        .approx_month = 5,
        .approx_day = 1
    },
    {
        .id = WHEEL_LITHA,
        .name = "Litha",
        .celtic_name = "Alban Hefin",
        .solar_longitude = 90.0,
        .meaning = "Peak of light",
        .element = WHEEL_ELEMENT_FIRE,
        .direction = WHEEL_DIR_SOUTH,
        .is_solar_event = true,
        .approx_month = 6,
        .approx_day = 21
    },
    {
        .id = WHEEL_LUGHNASADH,
        .name = "Lughnasadh",
        .celtic_name = "Lughnasadh",
        .solar_longitude = 135.0,
        .meaning = "First harvest, games of Lugh",
        .element = WHEEL_ELEMENT_WATER,
        .direction = WHEEL_DIR_WEST,
        .is_solar_event = false,
        .approx_month = 8,
        .approx_day = 1
    },
    {
        .id = WHEEL_MABON,
        .name = "Mabon",
        .celtic_name = "Alban Elfed",
        .solar_longitude = 180.0,
        .meaning = "Second harvest, gratitude",
        .element = WHEEL_ELEMENT_WATER,
        .direction = WHEEL_DIR_WEST,
        .is_solar_event = true,
        .approx_month = 9,
        .approx_day = 22
    }
};

static const char *ELEMENT_NAMES[WHEEL_ELEMENT_COUNT] = {
    "Earth", "Air", "Fire", "Water"
};

static const char *DIRECTION_NAMES[WHEEL_DIR_COUNT] = {
    "North", "East", "South", "West"
};

/* Normalize angle to [0, 360) */
static double normalize_angle(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0) {
        deg += 360.0;
    }
    return deg;
}

/* Angular distance between two angles on a circle (shortest path) */
static double angular_distance(double a, double b)
{
    double d = fabs(normalize_angle(a) - normalize_angle(b));
    if (d > 180.0) {
        d = 360.0 - d;
    }
    return d;
}

/* Solar longitudes in ecliptic order for lookup.
 * Index maps: 0->Ostara(0), 1->Beltane(45), 2->Litha(90),
 * 3->Lughnasadh(135), 4->Mabon(180), 5->Samhain(225),
 * 6->Yule(270), 7->Imbolc(315). */
static const int ECLIPTIC_ORDER[WHEEL_FESTIVAL_COUNT] = {
    WHEEL_OSTARA,       /* 0 */
    WHEEL_BELTANE,      /* 45 */
    WHEEL_LITHA,        /* 90 */
    WHEEL_LUGHNASADH,   /* 135 */
    WHEEL_MABON,        /* 180 */
    WHEEL_SAMHAIN,      /* 225 */
    WHEEL_YULE,         /* 270 */
    WHEEL_IMBOLC        /* 315 */
};

wheel_festival_info_t wheel_festival_info(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        wheel_festival_info_t empty = {
            .id = WHEEL_FESTIVAL_COUNT,
            .name = "?",
            .celtic_name = "?",
            .solar_longitude = -1.0,
            .meaning = "?",
            .element = WHEEL_ELEMENT_COUNT,
            .direction = WHEEL_DIR_COUNT,
            .is_solar_event = false,
            .approx_month = 0,
            .approx_day = 0
        };
        return empty;
    }
    return FESTIVALS[festival];
}

const char *wheel_festival_name(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        return "?";
    }
    return FESTIVALS[festival].name;
}

const char *wheel_celtic_name(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        return "?";
    }
    return FESTIVALS[festival].celtic_name;
}

double wheel_solar_longitude(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        return -1.0;
    }
    return FESTIVALS[festival].solar_longitude;
}

wheel_element_t wheel_festival_element(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        return WHEEL_ELEMENT_COUNT;
    }
    return FESTIVALS[festival].element;
}

const char *wheel_element_name(wheel_element_t element)
{
    if (element < 0 || element >= WHEEL_ELEMENT_COUNT) {
        return "?";
    }
    return ELEMENT_NAMES[element];
}

wheel_direction_t wheel_festival_direction(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        return WHEEL_DIR_COUNT;
    }
    return FESTIVALS[festival].direction;
}

const char *wheel_direction_name(wheel_direction_t direction)
{
    if (direction < 0 || direction >= WHEEL_DIR_COUNT) {
        return "?";
    }
    return DIRECTION_NAMES[direction];
}

wheel_festival_t wheel_current_festival(double solar_longitude_deg)
{
    double lon = normalize_angle(solar_longitude_deg);
    double best_dist = 999.0;
    int best_idx = 0;

    for (int i = 0; i < WHEEL_FESTIVAL_COUNT; i++) {
        double dist = angular_distance(lon, FESTIVALS[i].solar_longitude);
        if (dist < best_dist) {
            best_dist = dist;
            best_idx = i;
        }
    }
    return (wheel_festival_t)best_idx;
}

wheel_festival_t wheel_next_festival(double solar_longitude_deg)
{
    double lon = normalize_angle(solar_longitude_deg);

    /* Walk ecliptic order to find first festival strictly after lon.
     * Ecliptic order: 0, 45, 90, 135, 180, 225, 270, 315. */
    for (int i = 0; i < WHEEL_FESTIVAL_COUNT; i++) {
        int idx = ECLIPTIC_ORDER[i];
        double fest_lon = FESTIVALS[idx].solar_longitude;
        if (fest_lon > lon) {
            return (wheel_festival_t)idx;
        }
    }
    /* All festival longitudes <= lon, wrap to first in ecliptic order */
    return (wheel_festival_t)ECLIPTIC_ORDER[0];
}

double wheel_degrees_until_next(double solar_longitude_deg)
{
    double lon = normalize_angle(solar_longitude_deg);
    wheel_festival_t next = wheel_next_festival(solar_longitude_deg);
    double next_lon = FESTIVALS[next].solar_longitude;
    double dist = next_lon - lon;
    if (dist <= 0.0) {
        dist += 360.0;
    }
    return dist;
}

bool wheel_is_festival_active(double solar_longitude_deg, double threshold_deg)
{
    double lon = normalize_angle(solar_longitude_deg);
    for (int i = 0; i < WHEEL_FESTIVAL_COUNT; i++) {
        double dist = angular_distance(lon, FESTIVALS[i].solar_longitude);
        if (dist <= threshold_deg) {
            return true;
        }
    }
    return false;
}

int wheel_active_festival(double solar_longitude_deg, double threshold_deg)
{
    double lon = normalize_angle(solar_longitude_deg);
    for (int i = 0; i < WHEEL_FESTIVAL_COUNT; i++) {
        double dist = angular_distance(lon, FESTIVALS[i].solar_longitude);
        if (dist <= threshold_deg) {
            return i;
        }
    }
    return -1;
}

int wheel_festival_count(void)
{
    return WHEEL_FESTIVAL_COUNT;
}

bool wheel_is_solar_event(wheel_festival_t festival)
{
    if (festival < 0 || festival >= WHEEL_FESTIVAL_COUNT) {
        return false;
    }
    return FESTIVALS[festival].is_solar_event;
}
