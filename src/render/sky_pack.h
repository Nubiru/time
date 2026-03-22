/*
 * sky_pack.h — Preetham/Perez physically-based sky color model
 *
 * Computes sky color at any direction from sun position and atmospheric
 * turbidity using the Perez five-parameter luminance distribution model.
 *
 * Based on: Preetham, Shirley, Smits — "A Practical Analytic Model for
 * Daylight" (SIGGRAPH 1999). Shader implementation from Marco Sporl,
 * ShaderX3 Ch 8.3 (2004).
 *
 * CPU-side evaluation for testing + GLSL ES 3.00 shader source for Earth
 * View sky dome rendering. No textures needed — fully analytic.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#ifndef TIME_SKY_PACK_H
#define TIME_SKY_PACK_H

/* Sky configuration */
typedef struct {
    float turbidity;    /* atmospheric turbidity (2=clear, 10=hazy, 20=foggy) */
    float sun_theta;    /* sun zenith angle in radians (0=overhead, PI/2=horizon) */
    float sun_phi;      /* sun azimuth angle in radians */
} sky_config_t;

/* Pre-computed sky parameters (from turbidity + sun position).
 * Compute once per frame, then evaluate at many view directions. */
typedef struct {
    /* Perez distribution coefficients for Y, x, y */
    float A_Y, B_Y, C_Y, D_Y, E_Y;
    float A_x, B_x, C_x, D_x, E_x;
    float A_y, B_y, C_y, D_y, E_y;
    /* Zenith values */
    float Y_z;  /* zenith luminance */
    float x_z;  /* zenith chromaticity x */
    float y_z;  /* zenith chromaticity y */
    /* Sun position */
    float sun_theta;
    float sun_phi;
} sky_params_t;

/* Sky color result (linear RGB, 0-1 range, may exceed 1 near sun) */
typedef struct {
    float r, g, b;
} sky_rgb_t;

/* Default configuration: clear sky, sun at 45 degrees */
sky_config_t sky_default_config(void);

/* Compute sky parameters from config (once per frame). */
sky_params_t sky_compute_params(sky_config_t config);

/* Evaluate sky color at a view direction (many times per frame).
 * view_theta: zenith angle of view direction (radians)
 * view_phi: azimuth angle of view direction (radians)
 * Returns linear RGB sky color. */
sky_rgb_t sky_color_at(const sky_params_t *params,
                        float view_theta, float view_phi);

/* Evaluate the Perez function F(theta, gamma, A-E). */
float sky_perez(float theta, float gamma,
                float A, float B, float C, float D, float E);

/* GLSL ES 3.00 fragment shader source for sky dome rendering.
 * Uniforms: u_sun_dir (vec3), u_turbidity (float).
 * Evaluates Preetham model at each fragment's view direction. */
const char *sky_frag_source(void);

/* Vertex shader source for sky dome (fullscreen quad or hemisphere mesh). */
const char *sky_vert_source(void);

#endif /* TIME_SKY_PACK_H */
