/*
 * orbit_trail_pack.h — Orbit trail render pack: GPU-ready vertex packing
 *
 * Packs Keplerian orbit ellipses for all 8 planets as line strip vertex
 * data with gradient fade (bright at planet position, fading behind).
 * Includes GLSL ES 3.00 shader source strings for alpha-blended lines.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - orbit.h — JPL orbital elements (Keplerian approximation)
 *   - kepler.h — Kepler's equation solver
 *   - planet_data.h — NASA Planetary Fact Sheet (colors)
 */

#ifndef TIME_ORBIT_TRAIL_PACK_H
#define TIME_ORBIT_TRAIL_PACK_H

#define OTP_VERTEX_FLOATS 7    /* pos(3) + alpha(1) + color(3) */
#define OTP_MAX_VERTICES 4096  /* 8 planets x 512 samples each */
#define OTP_POINTS_PER_ORBIT 256  /* samples around each ellipse */

/* Configuration for orbit trail generation. */
typedef struct {
    float orbit_scale;         /* sqrt distance scale factor (default: 3.0) */
    int   points_per_orbit;    /* line segments per orbit (default: 256) */
    float fade_fraction;       /* fraction of orbit that's visible (default: 0.85) */
    float line_alpha_max;      /* brightest alpha at planet position (default: 0.6) */
} otp_config_t;

/* Get default orbit trail configuration.
 * orbit_scale: 3.0 (matches planet_pass ORBIT_SCALE)
 * points_per_orbit: 256
 * fade_fraction: 0.85
 * line_alpha_max: 0.6 */
otp_config_t otp_default_config(void);

/* Pack result metadata. */
typedef struct {
    int vertex_count;          /* total vertices across all orbits */
    int orbit_count;           /* number of planets with trails */
} otp_info_t;

/* Pack orbit trails for all 8 planets at given Julian Day.
 * out_verts must hold at least OTP_MAX_VERTICES * OTP_VERTEX_FLOATS floats.
 * Returns info about what was packed. */
otp_info_t otp_pack(double jd, otp_config_t config, float *out_verts);

/* Pack orbit trail for a single planet.
 * planet_index: 0-7 (Mercury-Neptune). Returns 0 for invalid index.
 * Returns vertex count for this orbit (clamped to max_verts). */
int otp_pack_planet(int planet_index, double jd, otp_config_t config,
                    float *out_verts, int max_verts);

/* Get the sqrt-scaled scene position of an orbit point.
 * planet_index: 0-7. mean_anomaly_rad: radians [0, 2*PI].
 * orbit_scale: sqrt distance scale factor.
 * Uses J2000.0 epoch elements for the ellipse shape. */
void otp_orbit_point(int planet_index, double mean_anomaly_rad,
                     float orbit_scale, float xyz[3]);

/* Orbit trail vertex shader source (GLSL ES 3.00). */
const char *otp_vert_source(void);

/* Orbit trail fragment shader source (GLSL ES 3.00). */
const char *otp_frag_source(void);

#endif /* TIME_ORBIT_TRAIL_PACK_H */
