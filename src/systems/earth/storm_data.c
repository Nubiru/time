/* storm_data.c — Storm season data: cyclone basins, monsoon regions,
 * swell generation zones. Pure data module, no side effects. */

#include "storm_data.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

/* ============================================================
 * Static data tables
 * ============================================================ */

static const cyclone_basin_t BASINS[STORM_BASIN_COUNT] = {
    {
        BASIN_NORTH_ATLANTIC, "North Atlantic",
        5.0, 35.0, -100.0, -10.0,
        6, 11, 9, 14.0,
        "Hurricanes affecting Caribbean, Gulf of Mexico, US East Coast"
    },
    {
        BASIN_EAST_PACIFIC, "East Pacific",
        5.0, 25.0, -180.0, -80.0,
        5, 11, 8, 17.0,
        "Hurricanes off Mexican and Central American coasts"
    },
    {
        BASIN_WEST_PACIFIC, "West Pacific",
        5.0, 35.0, 100.0, 180.0,
        1, 12, 8, 26.0,
        "Typhoons, most active basin on Earth, year-round"
    },
    {
        BASIN_NORTH_INDIAN, "North Indian",
        5.0, 25.0, 55.0, 100.0,
        4, 12, 11, 5.0,
        "Cyclones in Bay of Bengal and Arabian Sea"
    },
    {
        BASIN_SOUTH_INDIAN, "South Indian",
        -35.0, -5.0, 30.0, 110.0,
        11, 5, 2, 12.0,
        "Cyclones in southern Indian Ocean, Mozambique Channel"
    },
    {
        BASIN_SOUTH_PACIFIC, "South Pacific",
        -35.0, -5.0, 140.0, 240.0,
        11, 4, 2, 7.0,
        "Cyclones affecting Fiji, Vanuatu, Polynesia"
    },
    {
        BASIN_AUSTRALIAN, "Australian",
        -35.0, -5.0, 90.0, 160.0,
        11, 4, 2, 11.0,
        "Cyclones in Australian region, Coral Sea, Timor Sea"
    }
};

static const monsoon_region_t MONSOONS[MONSOON_REGION_COUNT] = {
    {
        0, "Indian",
        5.0, 30.0, 60.0, 100.0,
        6, 9,
        "South Asian monsoon, strongest on Earth, Jun-Sep"
    },
    {
        1, "East Asian",
        20.0, 45.0, 100.0, 145.0,
        6, 8,
        "East Asian monsoon, Meiyu/Baiu front, Jun-Aug"
    },
    {
        2, "West African",
        5.0, 20.0, -20.0, 30.0,
        6, 10,
        "West African monsoon, Sahel rains, Jun-Oct"
    },
    {
        3, "North American",
        15.0, 40.0, -115.0, -100.0,
        7, 9,
        "North American monsoon, SW USA and NW Mexico, Jul-Sep"
    },
    {
        4, "Australian",
        -25.0, -10.0, 115.0, 155.0,
        12, 3,
        "Australian monsoon, northern Australia wet season, Dec-Mar"
    }
};

static const swell_zone_t SWELL_ZONES[SWELL_ZONE_COUNT] = {
    {
        0, "North Atlantic Storm Track",
        50.0, -30.0, 225.0,
        10, 3,
        "Generates SW swells for Europe, W Africa"
    },
    {
        1, "North Pacific Storm Track",
        45.0, -170.0, 225.0,
        10, 4,
        "Generates NW swells for Hawaii, California"
    },
    {
        2, "Roaring Forties South Atlantic",
        -45.0, -20.0, 45.0,
        1, 12,
        "Generates S/SW swells for South Africa"
    },
    {
        3, "Roaring Forties South Indian",
        -45.0, 70.0, 45.0,
        1, 12,
        "Generates S/SW swells for Indonesia, Australia"
    },
    {
        4, "Roaring Forties South Pacific",
        -45.0, -150.0, 315.0,
        1, 12,
        "Generates S/SW swells for Polynesia, Chile"
    },
    {
        5, "Antarctic Circumpolar",
        -60.0, 0.0, 0.0,
        1, 12,
        "Massive long-period S swells worldwide"
    },
    {
        6, "Tropical East Pacific",
        10.0, -120.0, 180.0,
        6, 10,
        "Hurricane swells for Mexico, Central America"
    },
    {
        7, "Bay of Bengal",
        15.0, 85.0, 180.0,
        4, 11,
        "Cyclone swells for Sri Lanka, Indonesia"
    }
};

/* ============================================================
 * Helper: month-in-range with wrap-around
 * ============================================================ */

static bool month_in_range(int month, int start, int end) {
    if (month < 1 || month > 12) return false;
    if (start <= end) {
        return month >= start && month <= end;
    }
    /* Wrap-around (e.g., Nov-May = 11..5) */
    return month >= start || month <= end;
}

/* ============================================================
 * Helper: season length accounting for wrap
 * ============================================================ */

static int season_length(int start, int end) {
    if (start <= end) {
        return end - start + 1;
    }
    return (12 - start + 1) + end;
}

/* ============================================================
 * Helper: circular distance between months
 * ============================================================ */

static int month_distance(int a, int b) {
    int d = abs(a - b);
    if (d > 6) d = 12 - d;
    return d;
}

/* ============================================================
 * Helper: haversine-like squared distance for nearest search
 * ============================================================ */

static double deg_to_rad(double deg) {
    return deg * PI / 180.0;
}

static double haversine_dist_sq(double lat1, double lon1,
                                double lat2, double lon2) {
    double dlat = deg_to_rad(lat2 - lat1);
    double dlon = deg_to_rad(lon2 - lon1);
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(deg_to_rad(lat1)) * cos(deg_to_rad(lat2)) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    return a;
}

/* ============================================================
 * Cyclone basin API
 * ============================================================ */

cyclone_basin_t storm_basin_get(int index) {
    if (index < 0 || index >= STORM_BASIN_COUNT) {
        cyclone_basin_t invalid = {0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL};
        invalid.id = (storm_basin_t)-1;
        return invalid;
    }
    return BASINS[index];
}

int storm_basin_count(void) {
    return STORM_BASIN_COUNT;
}

const char *storm_basin_name(storm_basin_t basin) {
    int idx = (int)basin;
    if (idx < 0 || idx >= STORM_BASIN_COUNT) return NULL;
    return BASINS[idx].name;
}

bool storm_basin_active(storm_basin_t basin, int month) {
    int idx = (int)basin;
    if (idx < 0 || idx >= STORM_BASIN_COUNT) return false;
    return month_in_range(month, BASINS[idx].season_start,
                          BASINS[idx].season_end);
}

double storm_basin_risk(storm_basin_t basin, int month) {
    int idx = (int)basin;
    if (idx < 0 || idx >= STORM_BASIN_COUNT) return 0.0;
    if (!storm_basin_active(basin, month)) return 0.0;

    int peak = BASINS[idx].peak_month;
    int start = BASINS[idx].season_start;
    int end = BASINS[idx].season_end;
    int slen = season_length(start, end);

    if (month == peak) return 1.0;

    int dist = month_distance(month, peak);
    double half_season = slen / 2.0;
    double risk = 1.0 - (double)dist / half_season;
    if (risk < 0.0) risk = 0.0;
    if (risk > 1.0) risk = 1.0;
    return risk;
}

/* ============================================================
 * Monsoon region API
 * ============================================================ */

monsoon_region_t storm_monsoon_get(int index) {
    if (index < 0 || index >= MONSOON_REGION_COUNT) {
        monsoon_region_t invalid = {-1, NULL, 0, 0, 0, 0, 0, 0, NULL};
        return invalid;
    }
    return MONSOONS[index];
}

int storm_monsoon_count(void) {
    return MONSOON_REGION_COUNT;
}

bool storm_monsoon_active(int monsoon_index, int month) {
    if (monsoon_index < 0 || monsoon_index >= MONSOON_REGION_COUNT)
        return false;
    return month_in_range(month, MONSOONS[monsoon_index].onset_month,
                          MONSOONS[monsoon_index].withdrawal_month);
}

/* ============================================================
 * Combined spatial query
 * ============================================================ */

storm_risk_t storm_risk_at(double lat, double lon, int month) {
    storm_risk_t result = {0.0, false, NULL, NULL};

    /* Check cyclone basins */
    for (int i = 0; i < STORM_BASIN_COUNT; i++) {
        const cyclone_basin_t *b = &BASINS[i];
        if (lat >= b->lat_min && lat <= b->lat_max &&
            lon >= b->lon_min && lon <= b->lon_max) {
            double risk = storm_basin_risk(b->id, month);
            if (risk > result.cyclone_risk) {
                result.cyclone_risk = risk;
                result.basin_name = b->name;
            }
        }
    }

    /* Check monsoon regions */
    for (int i = 0; i < MONSOON_REGION_COUNT; i++) {
        const monsoon_region_t *m = &MONSOONS[i];
        if (lat >= m->lat_min && lat <= m->lat_max &&
            lon >= m->lon_min && lon <= m->lon_max) {
            if (storm_monsoon_active(i, month)) {
                result.monsoon_active = true;
                result.monsoon_name = m->name;
            }
        }
    }

    return result;
}

/* ============================================================
 * Swell zone API
 * ============================================================ */

swell_zone_t storm_swell_zone_get(int index) {
    if (index < 0 || index >= SWELL_ZONE_COUNT) {
        swell_zone_t invalid = {-1, NULL, 0, 0, 0, 0, 0, NULL};
        return invalid;
    }
    return SWELL_ZONES[index];
}

int storm_swell_zone_count(void) {
    return SWELL_ZONE_COUNT;
}

double storm_swell_direction(double lat, double lon, int month) {
    double best_dist = 1e30;
    int best_idx = -1;

    for (int i = 0; i < SWELL_ZONE_COUNT; i++) {
        if (!month_in_range(month, SWELL_ZONES[i].active_start,
                            SWELL_ZONES[i].active_end)) {
            continue;
        }
        double d = haversine_dist_sq(lat, lon,
                                     SWELL_ZONES[i].lat_center,
                                     SWELL_ZONES[i].lon_center);
        if (d < best_dist) {
            best_dist = d;
            best_idx = i;
        }
    }

    if (best_idx < 0) return -1.0;
    return SWELL_ZONES[best_idx].primary_dir_deg;
}
