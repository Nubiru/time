/* planet_data.h — Physical properties for the 8 planets.
 * Data source: NASA Planetary Fact Sheet (public domain). */

#ifndef TIME_PLANET_DATA_H
#define TIME_PLANET_DATA_H

#define PLANET_DATA_COUNT 8

/* RGB color (0.0-1.0 per channel) */
typedef struct {
    float r, g, b;
} planet_color_t;

/* Physical properties for one planet */
typedef struct {
    double radius_km;        /* equatorial radius in km */
    double mass_kg;          /* mass in kg */
    double albedo;           /* geometric albedo (0.0-1.0) */
    double axial_tilt_deg;   /* obliquity to orbit in degrees */
    double rotation_hours;   /* sidereal rotation period in hours (negative = retrograde) */
    double ring_inner_radii; /* inner ring radius in planet radii (0 = no rings) */
    double ring_outer_radii; /* outer ring radius in planet radii (0 = no rings) */
    planet_color_t color;    /* primary apparent color */
    planet_color_t color_alt;/* secondary color (banding, poles, etc.) */
    int has_rings;           /* 1 if planet has visible ring system */
    int is_gas_giant;        /* 1 if gas/ice giant */
} planet_physical_t;

/* Get physical data for a planet by index (0-7, matches planets.h). */
planet_physical_t planet_data_get(int planet_index);

/* Get planet primary color. */
planet_color_t planet_data_color(int planet_index);

/* Get planet radius in km. */
double planet_data_radius(int planet_index);

/* Get relative size (radius / Earth radius). */
double planet_data_relative_size(int planet_index);

/* Get planet type string: "Terrestrial" or "Gas Giant" or "Ice Giant". */
const char *planet_data_type(int planet_index);

/* Get number of known major moons (approximate). */
int planet_data_major_moons(int planet_index);

/* Scene-space radius: planet radius scaled for rendering.
 * base_scale: multiplier (e.g., 0.01 = 1 Earth radius = 0.01 scene units). */
double planet_data_scene_radius(int planet_index, double base_scale);

/* Saturn ring radii in scene-space units. Returns 0 for non-ringed planets. */
double planet_data_ring_inner(int planet_index, double base_scale);
double planet_data_ring_outer(int planet_index, double base_scale);

#endif /* TIME_PLANET_DATA_H */
