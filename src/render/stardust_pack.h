/*
 * stardust_pack.h — Procedural background star field shader source
 *
 * Generates thousands of faint procedural stars on a fullscreen quad
 * using hash-based noise for deterministic star positions. Fills the
 * gap between the 600 named bright stars (star_pass) and the smooth
 * Milky Way band (milkyway_pass) at galaxy zoom levels.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Algorithm: For each pixel, hash the UV coordinate at multiple scales
 * to generate star candidates. Stars appear as bright pixels where the
 * hash value exceeds a threshold (density control). Star brightness
 * follows a power-law distribution (many dim, few bright).
 */

#ifndef TIME_STARDUST_PACK_H
#define TIME_STARDUST_PACK_H

/* Star dust configuration */
typedef struct {
    float density;       /* star density (0.0-1.0, default: 0.5) */
    float brightness;    /* overall brightness multiplier (default: 0.3) */
    float twinkle_speed; /* twinkle animation speed (default: 0.5) */
} sd_config_t;

/* Default star dust configuration */
sd_config_t sd_default_config(void);

/* Vertex shader source (passthrough fullscreen quad, same as post_process). */
const char *sd_vert_source(void);

/* Fragment shader source (procedural star field, GLSL ES 3.00).
 * Uniforms: u_mvp (mat4), u_time (float), u_density (float),
 *           u_brightness (float), u_zoom_alpha (float).
 * Renders additive bright points on a transparent background. */
const char *sd_frag_source(void);

#endif /* TIME_STARDUST_PACK_H */
