/* seasonal_lighting.h — Seasonal sky gradient color ramps for Earth View.
 *
 * Pure render data module providing sky gradient color ramps that vary by
 * season, latitude, and time of day. For any given latitude, day-of-year,
 * and sun elevation, produces a vertical sky color gradient (horizon to
 * zenith) suitable for rendering a sky backdrop in Earth View.
 *
 * Physically plausible: Rayleigh scattering approximation for blue sky,
 * warm path-length tones at low sun angles, seasonal saturation/warmth
 * modulation, latitude-based variation (tropical stability, polar extremes).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Color science:
 *   - Rayleigh scattering: wavelength^-4 (sky blue)
 *   - Mie scattering: forward-scatter (horizon haze, sun glow)
 *   - Path length: warm tones at low elevation (more atmosphere)
 *   - Seasonal: solar declination modulates sky warmth/saturation
 */

#ifndef TIME_SEASONAL_LIGHTING_H
#define TIME_SEASONAL_LIGHTING_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Sky gradient color stop */
typedef struct {
    float altitude;     /* 0.0 = horizon, 1.0 = zenith */
    float r, g, b;      /* sRGB color [0-1] */
} slp_color_stop_t;

/* Sky gradient result */
#define SLP_MAX_STOPS 8

typedef struct {
    slp_color_stop_t stops[SLP_MAX_STOPS];
    int stop_count;
    float ambient_r, ambient_g, ambient_b;  /* overall ambient light color */
    float ambient_intensity;                 /* 0.0 (night) to 1.0 (noon) */
} slp_sky_t;

/* Configuration */
typedef struct {
    double latitude_deg;      /* observer latitude (-90 to 90) */
    int    day_of_year;       /* 1-365 */
    float  sun_elevation_deg; /* sun altitude above horizon (-90 to 90) */
} slp_config_t;

/* Default configuration (equator, spring equinox, noon). */
slp_config_t slp_default_config(void);

/* Compute sky gradient for given conditions.
 * Returns gradient stops from horizon to zenith + ambient light. */
slp_sky_t slp_sky_gradient(slp_config_t config);

/* Interpolate sky color at a specific altitude within the gradient.
 * altitude: 0.0 = horizon, 1.0 = zenith.
 * Returns RGB color via rgb[3]. */
void slp_sky_color_at(const slp_sky_t *sky, float altitude, float rgb[3]);

/* Get ambient light color and intensity for the given conditions.
 * Useful for setting GL clear color or ambient uniform. */
void slp_ambient_light(slp_config_t config, float rgb[3], float *intensity);

/* Sun color at given elevation (shifts from white at noon to orange/red
 * at horizon). Returns RGB color via rgb[3]. */
void slp_sun_color(float sun_elevation_deg, float rgb[3]);

/* Moon/night sky color at given latitude and day of year.
 * Returns dim blue-black with slight seasonal variation via rgb[3]. */
void slp_night_sky_color(double latitude_deg, int day_of_year, float rgb[3]);

/* Fog/haze color at given conditions (for depth atmosphere blending).
 * Returns color for far-distance atmospheric fade via rgb[3]. */
void slp_fog_color(slp_config_t config, float rgb[3]);

/* Pack sky gradient as vertex strip for rendering.
 * Produces a vertical quad strip with 2 vertices per stop.
 * Vertex layout: position(vec2) + color(vec3) = 5 floats.
 * GL_TRIANGLE_STRIP format.
 * Returns vertex count. */
#define SLP_STRIP_VERTEX_FLOATS 5
#define SLP_MAX_STRIP_VERTICES 32
int slp_pack_gradient_strip(const slp_sky_t *sky, float *out_verts);

/* Shader sources for sky gradient rendering (GLSL ES 3.00). */
const char *slp_vert_source(void);
const char *slp_frag_source(void);

#endif /* TIME_SEASONAL_LIGHTING_H */
