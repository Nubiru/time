/* wind_patterns.h — Coriolis effect and global wind pattern data.
 * Models the three atmospheric circulation cells (Hadley, Ferrel, Polar),
 * prevailing winds, ITCZ seasonal migration, and Coriolis deflection.
 * Pure functions, no side effects. */

#ifndef TIME_WIND_PATTERNS_H
#define TIME_WIND_PATTERNS_H

#define PI 3.14159265358979323846
#define EARTH_OMEGA 7.2921e-5  /* rad/s */

#define WIND_CELL_HADLEY  0
#define WIND_CELL_FERREL  1
#define WIND_CELL_POLAR   2
#define WIND_CELL_COUNT 3

typedef struct {
    double direction_deg;     /* wind FROM direction (0=N, 90=E, 180=S, 270=W) */
    double strength_relative; /* 0.0-1.0 */
    int cell_type;
    const char *name;         /* "Trade Winds", "Westerlies", "Polar Easterlies" */
} wind_pattern_t;

typedef struct {
    int id;
    const char *name;
    double lat_min;
    double lat_max;
    const char *surface_wind;
    double surface_direction;
    const char *description;
} circulation_cell_t;

/* Coriolis parameter f = 2 * omega * sin(lat).
 * Positive in NH, negative in SH, zero at equator.
 * lat_deg: latitude in degrees (-90 to 90). */
double wind_coriolis_parameter(double lat_deg);

/* Prevailing wind pattern at a given latitude.
 * Returns direction, strength, cell type, and name.
 * lat_deg: latitude in degrees (negative for SH). */
wind_pattern_t wind_prevailing(double lat_deg);

/* Get circulation cell data by index (0=Hadley, 1=Ferrel, 2=Polar).
 * Returns cell with id=-1 and name=NULL for invalid index. */
circulation_cell_t wind_cell_get(int index);

/* Number of circulation cells (always 3). */
int wind_cell_count(void);

/* Which circulation cell contains this latitude?
 * Returns WIND_CELL_HADLEY, WIND_CELL_FERREL, or WIND_CELL_POLAR.
 * Uses absolute value of latitude. */
int wind_cell_at_latitude(double lat_deg);

/* ITCZ latitude for a given month (1-12).
 * Returns degrees north (always positive).
 * Out-of-range months are clamped to nearest valid month. */
double wind_itcz_latitude(int month);

/* Coriolis deflection angle at a given latitude.
 * deflection = 45.0 * sin(lat_rad).
 * Positive in NH (rightward), negative in SH (leftward).
 * Returns degrees. */
double wind_coriolis_deflection(double lat_deg);

/* Is this latitude in the doldrums? (within 5 degrees of ITCZ).
 * lat_deg: latitude in degrees.
 * month: month 1-12 (ITCZ position varies seasonally).
 * Returns 1 if doldrums, 0 otherwise. */
int wind_is_doldrums(double lat_deg, int month);

/* Is this latitude in the horse latitudes? (|lat| within 5 degrees of 30).
 * Returns 1 if horse latitudes, 0 otherwise. */
int wind_is_horse_latitudes(double lat_deg);

#endif /* TIME_WIND_PATTERNS_H */
