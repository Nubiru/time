#ifndef TIME_STAR_CATALOG_H
#define TIME_STAR_CATALOG_H

/* Bright star catalog entry. */
typedef struct {
    float ra_hours;    /* Right Ascension 0-24h */
    float dec_deg;     /* Declination -90 to +90 degrees */
    float magnitude;   /* Visual magnitude (lower = brighter) */
    float bv;          /* B-V color index */
} star_entry_t;

/* 3D position on unit sphere. */
typedef struct {
    float x, y, z;
} star_xyz_t;

/* Number of stars in the catalog. */
int star_catalog_count(void);

/* Get star data by index (0 = brightest). Returns sentinel (mag=99) if invalid. */
star_entry_t star_catalog_entry(int index);

/* Get star common name by index. Returns NULL if unnamed or invalid. */
const char *star_catalog_name(int index);

/* Convert RA/Dec to equatorial Cartesian on unit sphere.
 * x = toward RA=0, y = toward RA=6h, z = toward north pole. */
star_xyz_t star_to_equatorial_xyz(float ra_hours, float dec_deg);

/* Convert RA/Dec to ecliptic Cartesian in project convention.
 * x = vernal equinox, z = 90 deg ecliptic longitude, y = ecliptic north.
 * obliquity_deg: Earth's axial tilt (~23.44 degrees). */
star_xyz_t star_to_ecliptic_xyz(float ra_hours, float dec_deg, float obliquity_deg);

/* Convert B-V color index to RGB (0-1 range).
 * Blue stars (BV<0) -> blue-white, red stars (BV>1.5) -> orange-red. */
void star_bv_to_rgb(float bv, float *r, float *g, float *b);

/* Convert visual magnitude to point size.
 * Brighter stars (lower mag) get larger size. Always positive.
 * base_size: size for mag=0 star. */
float star_mag_to_size(float magnitude, float base_size);

#endif
