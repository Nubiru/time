#ifndef TIME_HOUSES_H
#define TIME_HOUSES_H

/* Whole Sign house system. */
typedef struct {
    int signs[12];      /* signs[0] = sign of 1st house, etc. (0=Aries..11=Pisces) */
    double cusps[12];   /* cusp longitudes in degrees (start of each house's sign) */
    double ascendant;   /* exact ascendant longitude */
} house_system_t;

/* Compute Whole Sign houses from ascendant longitude.
 * ascendant_lon: ecliptic longitude of ascendant (degrees, 0-360) */
house_system_t houses_whole_sign(double ascendant_lon);

#endif
