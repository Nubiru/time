/* earth_atmosphere.h — Pure data module for Earth atmosphere rendering.
 * Rayleigh scattering sky color approximation, atmosphere shell mesh
 * generation (UV-sphere), cloud band latitude data, limb glow, air mass,
 * optical depth. All pure functions, no GL, no malloc, no globals.
 *
 * Physical basis:
 *   - Scale height H = 8.5 km (exponential density decay)
 *   - Rayleigh scattering: wavelength^-4 (blue ~5.5x more than red)
 *   - Kasten-Young air mass formula
 *   - 7 cloud band zones (ITCZ, subtropical highs, storm tracks, polar fronts)
 *
 * Data sources: NASA atmospheric model, Kasten & Young 1989. */

#ifndef TIME_EARTH_ATMOSPHERE_H
#define TIME_EARTH_ATMOSPHERE_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Atmosphere parameters */
#define EARTH_ATMO_SCALE_HEIGHT_KM  8.5
#define EARTH_ATMO_THICKNESS_KM     100.0
#define EARTH_RADIUS_KM             6371.0
#define EARTH_ATMO_RADIUS_RATIO     1.0157  /* (6371+100)/6371 */

/* Atmosphere shell mesh resolution */
#define EARTH_ATMO_LAT_SEGMENTS     32
#define EARTH_ATMO_LON_SEGMENTS     64
#define EARTH_ATMO_MAX_VERTICES     ((EARTH_ATMO_LAT_SEGMENTS + 1) * (EARTH_ATMO_LON_SEGMENTS + 1))
#define EARTH_ATMO_MAX_INDICES      (EARTH_ATMO_LAT_SEGMENTS * EARTH_ATMO_LON_SEGMENTS * 6)

/* Cloud band regions */
#define EARTH_CLOUD_BAND_COUNT      7

/* Atmosphere vertex: position + normal + opacity */
typedef struct {
    float x, y, z;       /* position on atmosphere shell */
    float nx, ny, nz;    /* normal (radial outward) */
    float opacity;        /* atmospheric density at this latitude */
} atmo_vertex_t;

/* Atmosphere shell mesh info */
typedef struct {
    int vertex_count;
    int index_count;
} atmo_mesh_info_t;

/* RGB color result */
typedef struct {
    float r, g, b;
} atmo_color_t;

/* Cloud band data */
typedef struct {
    float lat_center;     /* center latitude (degrees, + = north) */
    float lat_width;      /* width in degrees */
    float density;        /* cloud density 0-1 */
    const char *name;     /* descriptive name */
    int is_seasonal;      /* varies significantly with season */
} cloud_band_t;

/* Rayleigh scattering sky color.
 * altitude_km: 0 (sea level) to 100 (edge of space).
 * sun_elevation_deg: -90 to 90 (below/above horizon).
 * Returns RGB in [0,1]. */
atmo_color_t earth_atmo_color(double altitude_km, double sun_elevation_deg);

/* Atmosphere color at sunset/sunrise (warm tones from long path).
 * sun_elevation_deg: typically -10 to +10 degrees.
 * Returns RGB with warm sunset palette. */
atmo_color_t earth_atmo_sunset_color(double sun_elevation_deg);

/* Atmospheric density at altitude (exponential decay).
 * altitude_km: height above sea level.
 * Returns normalized density (1.0 at sea level, ~0 at 100km). */
double earth_atmo_density(double altitude_km);

/* Optical depth through atmosphere at zenith angle.
 * zenith_angle_deg: 0 (overhead) to 90 (horizon).
 * Returns relative optical depth (1.0 at zenith). */
double earth_atmo_optical_depth(double zenith_angle_deg);

/* Air mass factor (how much atmosphere light passes through).
 * elevation_deg: sun elevation above horizon.
 * Returns air mass (1.0 at zenith, ~38 at horizon). */
double earth_atmo_air_mass(double elevation_deg);

/* Generate atmosphere shell UV-sphere (slightly larger than unit sphere).
 * radius_ratio: atmosphere shell radius / earth radius (e.g., 1.0157).
 * Fills out_vertices and out_indices.
 * Returns mesh info. */
atmo_mesh_info_t earth_atmo_shell_generate(double radius_ratio,
                                            atmo_vertex_t *out_vertices,
                                            unsigned int *out_indices);

/* How many vertices for atmosphere shell? */
int earth_atmo_shell_vertex_count(void);

/* How many indices for atmosphere shell? */
int earth_atmo_shell_index_count(void);

/* Limb brightening factor for atmosphere glow at edge.
 * view_dot_normal: dot(view_direction, surface_normal), range [0,1].
 * Returns glow intensity (bright at edges, dim at center). */
float earth_atmo_limb_glow(float view_dot_normal);

/* Get cloud band by index (0 to EARTH_CLOUD_BAND_COUNT-1). */
cloud_band_t earth_cloud_band_get(int index);

/* Cloud band count. */
int earth_cloud_band_count(void);

/* Cloud density at latitude (sum of all bands via Gaussian).
 * lat_deg: -90 to 90.
 * Returns density 0-1. */
double earth_cloud_density_at(double lat_deg);

/* Seasonal cloud band adjustment.
 * month: 1-12.
 * Returns latitude offset for ITCZ migration (degrees). */
double earth_cloud_itcz_offset(int month);

#endif /* TIME_EARTH_ATMOSPHERE_H */
