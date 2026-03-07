/* storm_data.h — Storm season data: cyclone basins, monsoon regions,
 * swell generation zones. Pure data module, no side effects. */

#ifndef TIME_STORM_DATA_H
#define TIME_STORM_DATA_H

#include <stdbool.h>

#define STORM_BASIN_COUNT 7
#define MONSOON_REGION_COUNT 5
#define SWELL_ZONE_COUNT 8

typedef enum {
    BASIN_NORTH_ATLANTIC = 0,
    BASIN_EAST_PACIFIC,
    BASIN_WEST_PACIFIC,
    BASIN_NORTH_INDIAN,
    BASIN_SOUTH_INDIAN,
    BASIN_SOUTH_PACIFIC,
    BASIN_AUSTRALIAN
} storm_basin_t;

typedef struct {
    storm_basin_t id;
    const char *name;
    double lat_min, lat_max;
    double lon_min, lon_max;
    int season_start;        /* month 1-12 */
    int season_end;          /* month 1-12 */
    int peak_month;          /* month of peak activity */
    double avg_storms_year;  /* average named storms per year */
    const char *description;
} cyclone_basin_t;

typedef struct {
    int id;
    const char *name;
    double lat_min, lat_max;
    double lon_min, lon_max;
    int onset_month;
    int withdrawal_month;
    const char *description;
} monsoon_region_t;

typedef struct {
    double cyclone_risk;     /* 0.0-1.0 */
    bool monsoon_active;
    const char *basin_name;  /* NULL if no cyclone basin */
    const char *monsoon_name; /* NULL if no monsoon */
} storm_risk_t;

typedef struct {
    int id;
    const char *name;
    double lat_center, lon_center;
    double primary_dir_deg;    /* compass degrees of dominant swell direction */
    int active_start;          /* month */
    int active_end;            /* month */
    const char *description;
} swell_zone_t;

/* Cyclone basin queries */
cyclone_basin_t storm_basin_get(int index);
int storm_basin_count(void);
const char *storm_basin_name(storm_basin_t basin);
bool storm_basin_active(storm_basin_t basin, int month);
double storm_basin_risk(storm_basin_t basin, int month);

/* Monsoon region queries */
monsoon_region_t storm_monsoon_get(int index);
int storm_monsoon_count(void);
bool storm_monsoon_active(int monsoon_index, int month);

/* Combined spatial query */
storm_risk_t storm_risk_at(double lat, double lon, int month);

/* Swell zone queries */
swell_zone_t storm_swell_zone_get(int index);
int storm_swell_zone_count(void);
double storm_swell_direction(double lat, double lon, int month);

#endif /* TIME_STORM_DATA_H */
