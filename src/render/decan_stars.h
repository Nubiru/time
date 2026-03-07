/* decan_stars.h — The 36 Egyptian decans as star groups.
 *
 * Pure data module providing the ancient Egyptian decan system: 36 groups
 * of stars dividing the ecliptic into 10-degree segments. Predates the
 * Greco-Roman zodiac but maps perfectly onto it (3 decans per sign).
 *
 * Sources: Neugebauer & Parker "Egyptian Astronomical Texts" (EAT) for
 * traditional decan names. Modern star identifications follow the scholarly
 * consensus summarized by Conman (2003) and Lull & Belmonte (2009).
 *
 * The decan star clock divides the night into 12 hours, each ruled by
 * the decan currently rising on the eastern horizon. A new decan rises
 * approximately every 40 minutes (10 degrees of sidereal rotation). */

#ifndef TIME_DECAN_STARS_H
#define TIME_DECAN_STARS_H

#define DECAN_COUNT 36
#define DECAN_DEGREES 10.0  /* each decan spans 10 degrees of ecliptic */

/* Element associated with each decan (via zodiac triplicity) */
typedef enum {
    DECAN_ELEM_FIRE = 0,
    DECAN_ELEM_EARTH,
    DECAN_ELEM_AIR,
    DECAN_ELEM_WATER,
    DECAN_ELEM_COUNT
} decan_element_t;

/* Decan data */
typedef struct {
    int index;                  /* 0-35 */
    const char *egyptian_name;  /* traditional Egyptian name (transliterated) */
    const char *principal_star; /* modern star identification (brightest in group) */
    double ecl_lon_start;       /* ecliptic longitude start (degrees, 0=vernal equinox) */
    double ecl_lon_end;         /* ecliptic longitude end (degrees) */
    int zodiac_sign;            /* 0=Aries..11=Pisces */
    int decan_in_sign;          /* 1, 2, or 3 within the sign */
    decan_element_t element;    /* element from zodiac triplicity */
    const char *description;    /* brief description of star group */
} decan_t;

/* Night hour data for the decan star clock */
typedef struct {
    int decan_index;            /* which decan is rising */
    double hour_angle;          /* hour angle of the decan's principal star */
    int is_visible;             /* 1 if above horizon */
} decan_hour_t;

/* Get total number of decans (always 36). */
int decan_count(void);

/* Get decan by index (0-35). Returns sentinel with index=-1 if invalid. */
decan_t decan_get(int index);

/* Get decan for a given ecliptic longitude (0-360 degrees).
 * Handles wrapping for values outside [0,360). Returns decan index (0-35). */
int decan_for_ecliptic_longitude(double ecl_lon_deg);

/* Get the 3 decans for a zodiac sign (0=Aries..11=Pisces).
 * Writes up to min(3, max_out) indices to out[].
 * Returns count written (up to 3), or 0 if sign invalid. */
int decan_for_sign(int sign, int *out, int max_out);

/* Decan element name as string. Returns "Unknown" for invalid. */
const char *decan_element_name(decan_element_t elem);

/* Compute which decan is currently rising at observer location.
 * lst_deg: local sidereal time in degrees (0-360).
 * lat_deg: observer latitude in degrees.
 * Returns decan index (0-35). */
int decan_rising_now(double lst_deg, double lat_deg);

/* Get the decan star clock: which decan rules each of the 12 night hours.
 * lst_deg: local sidereal time in degrees at start of observation.
 * lat_deg: observer latitude.
 * Writes up to max_out decan_hour_t entries to out[].
 * Returns count written (up to 12). */
int decan_night_hours(double lst_deg, double lat_deg, decan_hour_t *out, int max_out);

/* Get the "ruler" decan for a given night hour (0-11) at a given LST.
 * Returns decan index, or -1 if hour invalid. */
int decan_for_night_hour(double lst_deg, double lat_deg, int hour);

/* Principal star ecliptic longitude for a decan (midpoint of range).
 * Returns -1.0 if index invalid. */
double decan_star_longitude(int index);

#endif /* TIME_DECAN_STARS_H */
