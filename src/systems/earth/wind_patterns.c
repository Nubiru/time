/* wind_patterns.c — Coriolis effect and global wind pattern data.
 * Pure functions modeling atmospheric circulation cells,
 * prevailing winds, ITCZ, and Coriolis deflection. */

#include "wind_patterns.h"

#include <math.h>
#include <stddef.h>

/* ITCZ latitude by month (index 0..11 for Jan..Dec) */
static const double ITCZ_LAT_BY_MONTH[12] = {
    5.0,   /* Jan */
    5.0,   /* Feb */
    7.0,   /* Mar */
    10.0,  /* Apr */
    12.0,  /* May */
    14.0,  /* Jun */
    15.0,  /* Jul */
    14.0,  /* Aug */
    12.0,  /* Sep */
    10.0,  /* Oct */
    7.0,   /* Nov */
    5.0    /* Dec */
};

/* Static circulation cell data */
static const circulation_cell_t CELLS[WIND_CELL_COUNT] = {
    {
        .id = 0,
        .name = "Hadley",
        .lat_min = 0.0,
        .lat_max = 30.0,
        .surface_wind = "Trade Winds",
        .surface_direction = 45.0,
        .description = "Tropical cell: warm air rises at ITCZ, flows poleward aloft, descends at ~30 degrees"
    },
    {
        .id = 1,
        .name = "Ferrel",
        .lat_min = 30.0,
        .lat_max = 60.0,
        .surface_wind = "Westerlies",
        .surface_direction = 225.0,
        .description = "Mid-latitude cell: indirect circulation driven by Hadley and Polar cells"
    },
    {
        .id = 2,
        .name = "Polar",
        .lat_min = 60.0,
        .lat_max = 90.0,
        .surface_wind = "Polar Easterlies",
        .surface_direction = 45.0,
        .description = "Polar cell: cold air descends at poles, flows equatorward at surface"
    }
};

double wind_coriolis_parameter(double lat_deg) {
    double lat_rad = lat_deg * PI / 180.0;
    return 2.0 * EARTH_OMEGA * sin(lat_rad);
}

wind_pattern_t wind_prevailing(double lat_deg) {
    wind_pattern_t w;
    double abs_lat = fabs(lat_deg);
    int is_nh = (lat_deg >= 0.0);

    if (abs_lat < 30.0) {
        /* Trade winds */
        w.name = "Trade Winds";
        w.cell_type = WIND_CELL_HADLEY;
        w.direction_deg = is_nh ? 45.0 : 135.0;  /* NE in NH, SE in SH */
        w.strength_relative = 0.7;
    } else if (abs_lat < 60.0) {
        /* Westerlies */
        w.name = "Westerlies";
        w.cell_type = WIND_CELL_FERREL;
        w.direction_deg = is_nh ? 225.0 : 315.0;  /* SW in NH, NW in SH */
        w.strength_relative = 0.6;
    } else {
        /* Polar easterlies */
        w.name = "Polar Easterlies";
        w.cell_type = WIND_CELL_POLAR;
        w.direction_deg = is_nh ? 45.0 : 135.0;  /* NE in NH, SE in SH */
        w.strength_relative = 0.4;
    }

    return w;
}

circulation_cell_t wind_cell_get(int index) {
    if (index < 0 || index >= WIND_CELL_COUNT) {
        circulation_cell_t invalid = {
            .id = -1,
            .name = NULL,
            .lat_min = 0.0,
            .lat_max = 0.0,
            .surface_wind = NULL,
            .surface_direction = 0.0,
            .description = NULL
        };
        return invalid;
    }
    return CELLS[index];
}

int wind_cell_count(void) {
    return WIND_CELL_COUNT;
}

int wind_cell_at_latitude(double lat_deg) {
    double abs_lat = fabs(lat_deg);
    if (abs_lat < 30.0) {
        return WIND_CELL_HADLEY;
    } else if (abs_lat < 60.0) {
        return WIND_CELL_FERREL;
    }
    return WIND_CELL_POLAR;
}

double wind_itcz_latitude(int month) {
    /* Clamp to valid range 1-12 */
    if (month < 1) {
        month = 1;
    }
    if (month > 12) {
        month = 12;
    }
    return ITCZ_LAT_BY_MONTH[month - 1];
}

double wind_coriolis_deflection(double lat_deg) {
    double lat_rad = lat_deg * PI / 180.0;
    return 45.0 * sin(lat_rad);
}

int wind_is_doldrums(double lat_deg, int month) {
    double itcz = wind_itcz_latitude(month);
    return (fabs(lat_deg - itcz) <= 5.0) ? 1 : 0;
}

int wind_is_horse_latitudes(double lat_deg) {
    double abs_lat = fabs(lat_deg);
    return (fabs(abs_lat - 30.0) <= 5.0) ? 1 : 0;
}
