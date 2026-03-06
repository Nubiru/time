#ifndef TIME_OBSERVER_H
#define TIME_OBSERVER_H

/* Observer geographic location. */
typedef struct {
    double latitude;    /* degrees, positive north */
    double longitude;   /* degrees, positive east */
} observer_t;

/* Compute the Ascendant (ecliptic longitude of the rising point).
 * lst_deg: Local Sidereal Time in degrees (0-360)
 * obliquity_deg: obliquity of the ecliptic in degrees
 * latitude_deg: observer latitude in degrees
 * Returns: ecliptic longitude in degrees (0-360) */
double ascendant_longitude(double lst_deg, double obliquity_deg, double latitude_deg);

#endif
