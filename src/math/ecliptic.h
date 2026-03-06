#ifndef TIME_ECLIPTIC_H
#define TIME_ECLIPTIC_H

/* Equatorial coordinates: right ascension and declination (degrees). */
typedef struct {
    double ra;  /* right ascension (degrees, 0-360) */
    double dec; /* declination (degrees, -90 to +90) */
} equatorial_t;

/* Convert ecliptic longitude/latitude (degrees) to equatorial RA/Dec (degrees).
 * obliquity is the obliquity of the ecliptic in degrees. */
equatorial_t ecliptic_to_equatorial(double longitude, double latitude, double obliquity);

/* Mean obliquity of the ecliptic (degrees) at a given Julian Day.
 * IAU formula (Meeus Ch. 22). */
double mean_obliquity(double jd);

#endif
