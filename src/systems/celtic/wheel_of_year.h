/* wheel_of_year.h — Celtic/Pagan Wheel of the Year
 * Pure data module for the 8-festival solar cycle.
 * 4 solar events (solstices, equinoxes) at 0/90/180/270 degrees.
 * 4 cross-quarter days (fire festivals) at 45/135/225/315 degrees.
 * Each festival carries name, Celtic name, solar longitude, meaning,
 * element, direction, and solar event flag.
 *
 * Sources: Ronald Hutton, "The Stations of the Sun" (1996);
 * Celtic Druid tradition (Alban names from Iolo Morganwg). */

#ifndef TIME_WHEEL_OF_YEAR_H
#define TIME_WHEEL_OF_YEAR_H

#include <stdbool.h>

/* The 8 festivals of the Wheel */
typedef enum {
    WHEEL_SAMHAIN = 0,     /* Oct 31/Nov 1, solar long ~225 */
    WHEEL_YULE,            /* Winter solstice, solar long 270 */
    WHEEL_IMBOLC,          /* Feb 1-2, solar long ~315 */
    WHEEL_OSTARA,          /* Spring equinox, solar long 0 */
    WHEEL_BELTANE,         /* May 1, solar long ~45 */
    WHEEL_LITHA,           /* Summer solstice, solar long 90 */
    WHEEL_LUGHNASADH,      /* Aug 1, solar long ~135 */
    WHEEL_MABON,           /* Autumn equinox, solar long 180 */
    WHEEL_FESTIVAL_COUNT
} wheel_festival_t;

/* Element association */
typedef enum {
    WHEEL_ELEMENT_EARTH = 0,
    WHEEL_ELEMENT_AIR,
    WHEEL_ELEMENT_FIRE,
    WHEEL_ELEMENT_WATER,
    WHEEL_ELEMENT_COUNT
} wheel_element_t;

/* Cardinal direction */
typedef enum {
    WHEEL_DIR_NORTH = 0,
    WHEEL_DIR_EAST,
    WHEEL_DIR_SOUTH,
    WHEEL_DIR_WEST,
    WHEEL_DIR_COUNT
} wheel_direction_t;

/* Festival data */
typedef struct {
    wheel_festival_t id;
    const char *name;           /* English name */
    const char *celtic_name;    /* Traditional Celtic name */
    double solar_longitude;     /* Ecliptic longitude of sun (degrees) */
    const char *meaning;        /* Seasonal meaning */
    wheel_element_t element;
    wheel_direction_t direction;
    bool is_solar_event;        /* true for solstices/equinoxes */
    int approx_month;           /* Approximate Gregorian month (1-12) */
    int approx_day;             /* Approximate Gregorian day */
} wheel_festival_info_t;

/* Get festival info by index. Returns zeroed struct for invalid. */
wheel_festival_info_t wheel_festival_info(wheel_festival_t festival);

/* Festival name string. Returns "?" for invalid. */
const char *wheel_festival_name(wheel_festival_t festival);

/* Celtic name string. Returns "?" for invalid. */
const char *wheel_celtic_name(wheel_festival_t festival);

/* Solar longitude for a festival (degrees). Returns -1 for invalid. */
double wheel_solar_longitude(wheel_festival_t festival);

/* Element for a festival. Returns WHEEL_ELEMENT_COUNT for invalid. */
wheel_element_t wheel_festival_element(wheel_festival_t festival);

/* Element name string. Returns "?" for invalid. */
const char *wheel_element_name(wheel_element_t element);

/* Direction for a festival. Returns WHEEL_DIR_COUNT for invalid. */
wheel_direction_t wheel_festival_direction(wheel_festival_t festival);

/* Direction name string. Returns "?" for invalid. */
const char *wheel_direction_name(wheel_direction_t direction);

/* Find which festival is nearest to a given solar longitude.
 * Solar longitude wraps at 360. */
wheel_festival_t wheel_current_festival(double solar_longitude_deg);

/* Find next festival after a given solar longitude.
 * If exactly at a festival, returns the one AFTER it. */
wheel_festival_t wheel_next_festival(double solar_longitude_deg);

/* Angular distance to the next festival (degrees, 0-360). */
double wheel_degrees_until_next(double solar_longitude_deg);

/* Is a given solar longitude within threshold of ANY festival? */
bool wheel_is_festival_active(double solar_longitude_deg, double threshold_deg);

/* Which festival is active (within threshold), or -1 if none. */
int wheel_active_festival(double solar_longitude_deg, double threshold_deg);

/* Number of festivals (always 8). */
int wheel_festival_count(void);

/* Is this festival a solar event (solstice/equinox)? */
bool wheel_is_solar_event(wheel_festival_t festival);

#endif /* TIME_WHEEL_OF_YEAR_H */
