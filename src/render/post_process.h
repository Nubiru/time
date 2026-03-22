/*
 * post_process.h — Post-processing FBO pipeline: config, quad, shader sources
 *
 * Pure data module providing configuration structs, fullscreen quad geometry,
 * and GLSL ES 3.00 shader source strings for a multi-pass post-processing
 * pipeline: bloom (bright extract + Gaussian blur), Reinhard tone mapping,
 * vignette, and optional film grain.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Algorithm sources:
 *   - Reinhard tone mapping: Erik Reinhard et al., "Photographic Tone
 *     Reproduction for Digital Images" (SIGGRAPH 2002)
 *   - Gaussian blur: standard 9-tap separable filter (DSP textbook)
 *   - Luminance: ITU-R BT.709 coefficients
 *   - Film grain: Ashima Arts / Ian McEwan simplex noise (via noise_shader.h)
 */

#ifndef TIME_POST_PROCESS_H
#define TIME_POST_PROCESS_H

/* Post-processing effect configuration */
typedef struct {
    /* Bloom */
    int bloom_enabled;        /* 1 = bloom on, 0 = off */
    float bloom_threshold;    /* brightness threshold for bloom extraction (default: 0.8) */
    float bloom_intensity;    /* bloom strength multiplier (default: 0.5) */
    int bloom_passes;         /* number of blur passes (default: 5) */

    /* Tone mapping */
    int tonemap_enabled;      /* 1 = tone mapping on */
    float exposure;           /* exposure multiplier (default: 1.0) */
    float gamma;              /* gamma correction (default: 2.2) */

    /* Vignette */
    int vignette_enabled;     /* 1 = vignette on */
    float vignette_strength;  /* 0.0-1.0 (default: 0.3) */
    float vignette_radius;    /* 0.0-1.0 normalized (default: 0.75) */

    /* Film grain */
    int grain_enabled;        /* 1 = subtle film grain */
    float grain_intensity;    /* 0.0-1.0 (default: 0.03) */

    /* God rays (volumetric light scattering, GPU Gems 3 Ch 13) */
    int godrays_enabled;      /* 1 = radial blur from Sun position */
    float godrays_density;    /* sample spacing (default: 1.0) */
    float godrays_weight;     /* per-sample intensity (default: 0.01) */
    float godrays_decay;      /* exponential falloff (default: 0.97) */
    float godrays_exposure;   /* final intensity scale (default: 0.3) */

    /* Screen dimensions (set before each frame) */
    int screen_width;
    int screen_height;
} pp_config_t;

/* Default configuration (bloom + tonemap + vignette, no grain). */
pp_config_t pp_default_config(void);

/* FBO info for reporting/debugging */
typedef struct {
    int fbo_count;          /* number of FBOs allocated */
    int texture_count;      /* number of textures */
    int total_vram_bytes;   /* estimated VRAM usage */
} pp_info_t;

/* Get pipeline info (pure query, no GL). */
pp_info_t pp_info(pp_config_t config);

/* Estimate VRAM usage in bytes for a given resolution and config. */
int pp_estimate_vram(int width, int height, pp_config_t config);

/* --- Screen-space quad (for fullscreen shader passes) --- */

/* Fullscreen triangle vertex data: 3 vertices, position(vec2) + texcoord(vec2) = 4 floats each */
#define PP_QUAD_VERTEX_FLOATS 4
#define PP_QUAD_VERTEX_COUNT 3

/* Pack a fullscreen triangle (oversized, covers -1 to +1 NDC) into out[].
 * Layout per vertex: position.x, position.y, texcoord.u, texcoord.v.
 * Single oversized triangle: 10% faster than quad on AMD/Intel GPUs
 * (Real-Time Rendering, 4th Ed, Ch.12).
 * Returns vertex count (3), or 0 if out is NULL. */
int pp_pack_quad(float *out);

/* --- Shader sources (GLSL ES 3.00) --- */

/* Passthrough vertex shader for fullscreen quad. */
const char *pp_quad_vert_source(void);

/* Brightness extraction fragment shader (bloom step 1).
 * Uniforms: u_scene (sampler2D), u_threshold (float). */
const char *pp_bright_extract_frag_source(void);

/* Gaussian blur fragment shader (bloom step 2).
 * Uniforms: u_image (sampler2D), u_horizontal (int).
 * Run twice per pass: once horizontal, once vertical. */
const char *pp_blur_frag_source(void);

/* Final composite fragment shader (scene + bloom + tonemap + vignette + grain).
 * Uniforms: u_scene, u_bloom (sampler2D), u_bloom_intensity, u_exposure,
 *           u_gamma, u_vignette_strength, u_vignette_radius,
 *           u_grain_intensity, u_time (float).
 * Includes noise_shader_source() for film grain. */
const char *pp_composite_frag_source(void);

/* God rays radial blur fragment shader (GPU Gems 3, Ch 13).
 * 64-sample screen-space radial blur from Sun position.
 * Uniforms: u_frame (sampler2D), u_sun_screen_pos (vec2),
 *           u_density, u_weight, u_decay, u_exposure (float). */
const char *pp_godrays_frag_source(void);

#endif /* TIME_POST_PROCESS_H */
