#ifndef TIME_GALAXY_GEOMETRY_H
#define TIME_GALAXY_GEOMETRY_H

#define GALAXY_ARM_COUNT 4
#define GALAXY_MAX_ARM_POINTS 128

typedef struct {
    float positions[GALAXY_MAX_ARM_POINTS * 3]; /* x,y,z per point (y=0) */
    float brightnesses[GALAXY_MAX_ARM_POINTS];  /* 0.0-1.0, fades toward edges */
    int count;
} galaxy_arm_t;

typedef struct {
    galaxy_arm_t arms[GALAXY_ARM_COUNT];
    float sun_x, sun_y, sun_z; /* Sun's position on galactic plane */
} galaxy_mesh_t;

/* Generate Milky Way spiral arm geometry.
 * radius: galaxy radius (caller's units).
 * sun_distance: Sun's distance from center (same units).
 * rotation_deg: galactic rotation (degrees, animates over galactic year).
 * points_per_arm: vertices per arm (clamped to GALAXY_MAX_ARM_POINTS). */
galaxy_mesh_t galaxy_generate(float radius, float sun_distance,
                               float rotation_deg, int points_per_arm);

/* Milky Way spiral pitch angle (degrees). ~12 degrees. */
float galaxy_pitch_angle(void);

/* Galactic year in Earth years (~225 million). */
double galaxy_year(void);

/* Sun's angular position on galactic plane (degrees).
 * rotation_deg offsets the base angle. */
float galaxy_sun_angle(float rotation_deg);

#endif
