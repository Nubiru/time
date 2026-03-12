/*
 * post_process.c — Post-processing FBO pipeline: config, quad, shader sources
 *
 * Provides pure data for a multi-pass post-processing pipeline:
 *   1. Brightness extraction (bloom threshold)
 *   2. Separable Gaussian blur (9-tap, horizontal then vertical)
 *   3. Final composite: scene + bloom, Reinhard tone mapping, exposure,
 *      gamma correction, vignette, optional film grain
 *
 * Pure module: no GL calls, no malloc, no globals.
 * Exception: composite fragment shader uses a static buffer for lazy
 * concatenation with noise_shader_source() (same pattern as sun_shader.c).
 *
 * Algorithm sources:
 *   - Reinhard tone mapping: Erik Reinhard et al., SIGGRAPH 2002
 *   - Gaussian blur weights: standard 9-tap separable kernel
 *   - Luminance: ITU-R BT.709 (Rec. 709) coefficients
 *   - Film grain noise: Ashima Arts / Ian McEwan simplex noise (MIT)
 */

#include "post_process.h"
#include "noise_shader.h"

/* -----------------------------------------------------------------------
 * pp_default_config
 * ----------------------------------------------------------------------- */

pp_config_t pp_default_config(void)
{
    pp_config_t cfg;

    /* Bloom */
    cfg.bloom_enabled   = 1;
    cfg.bloom_threshold = 0.8f;
    cfg.bloom_intensity = 0.5f;
    cfg.bloom_passes    = 5;

    /* Tone mapping */
    cfg.tonemap_enabled = 1;
    cfg.exposure        = 1.0f;
    cfg.gamma           = 2.2f;

    /* Vignette */
    cfg.vignette_enabled  = 1;
    cfg.vignette_strength = 0.3f;
    cfg.vignette_radius   = 0.75f;

    /* Grain disabled by default */
    cfg.grain_enabled   = 0;
    cfg.grain_intensity = 0.03f;

    /* User must set screen dimensions */
    cfg.screen_width  = 0;
    cfg.screen_height = 0;

    return cfg;
}

/* -----------------------------------------------------------------------
 * pp_info — FBO/texture resource counts
 *
 * Pipeline FBO layout:
 *   1. Scene FBO (always) — render scene to texture
 *   2. Bright extract FBO (bloom only) — extract bright pixels
 *   3. Ping-pong blur FBOs x2 (bloom only) — Gaussian blur
 *
 * Each FBO has one color attachment texture (RGBA16F).
 * ----------------------------------------------------------------------- */

pp_info_t pp_info(pp_config_t config)
{
    pp_info_t info;

    /* Scene FBO is always needed */
    int fbos = 1;
    int textures = 1;

    if (config.bloom_enabled) {
        /* Bright extract FBO + 2 ping-pong blur FBOs */
        fbos += 3;
        textures += 3;
    }

    info.fbo_count = fbos;
    info.texture_count = textures;

    /* Estimate VRAM: each texture is width * height * 8 bytes (RGBA16F) */
    int w = config.screen_width;
    int h = config.screen_height;
    int bytes_per_tex = w * h * 8;  /* RGBA16F = 4 channels * 2 bytes */
    info.total_vram_bytes = textures * bytes_per_tex;

    return info;
}

/* -----------------------------------------------------------------------
 * pp_estimate_vram
 * ----------------------------------------------------------------------- */

int pp_estimate_vram(int width, int height, pp_config_t config)
{
    /* Scene FBO texture */
    int textures = 1;

    if (config.bloom_enabled) {
        /* Bright extract + 2 ping-pong blur */
        textures += 3;
    }

    /* RGBA16F = 4 channels * 2 bytes = 8 bytes per pixel */
    return textures * width * height * 8;
}

/* -----------------------------------------------------------------------
 * pp_pack_quad — fullscreen NDC quad as triangle strip
 *
 * 4 vertices, each: position(vec2) + texcoord(vec2) = 4 floats
 * Winding: BL, BR, TL, TR (triangle strip order)
 * ----------------------------------------------------------------------- */

int pp_pack_quad(float *out)
{
    if (!out) return 0;

    /* Bottom-left: pos(-1,-1) uv(0,0) */
    out[0]  = -1.0f; out[1]  = -1.0f; out[2]  = 0.0f; out[3]  = 0.0f;
    /* Bottom-right: pos(+1,-1) uv(1,0) */
    out[4]  =  1.0f; out[5]  = -1.0f; out[6]  = 1.0f; out[7]  = 0.0f;
    /* Top-left: pos(-1,+1) uv(0,1) */
    out[8]  = -1.0f; out[9]  =  1.0f; out[10] = 0.0f; out[11] = 1.0f;
    /* Top-right: pos(+1,+1) uv(1,1) */
    out[12] =  1.0f; out[13] =  1.0f; out[14] = 1.0f; out[15] = 1.0f;

    return PP_QUAD_VERTEX_COUNT;
}

/* -----------------------------------------------------------------------
 * Shader sources — GLSL ES 3.00
 * ----------------------------------------------------------------------- */

/* --- Passthrough vertex shader for fullscreen quad --- */
static const char s_quad_vert[] =
    "#version 300 es\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec2 a_texcoord;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "    v_uv = a_texcoord;\n"
    "    gl_Position = vec4(a_position, 0.0, 1.0);\n"
    "}\n";

/* --- Brightness extraction fragment shader ---
 *
 * Extracts pixels brighter than u_threshold using ITU-R BT.709 luminance.
 * Below-threshold pixels become black. */
static const char s_bright_extract_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D u_scene;\n"
    "uniform float u_threshold;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    vec3 color = texture(u_scene, v_uv).rgb;\n"
    "    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));\n"
    "    frag_color = vec4(color * step(u_threshold, brightness), 1.0);\n"
    "}\n";

/* --- 9-tap separable Gaussian blur fragment shader ---
 *
 * Direction controlled by u_horizontal uniform (1 = horizontal, 0 = vertical).
 * Weights: standard Gaussian kernel, 5 unique values (center + 4 neighbors).
 * Pixel offset derived from textureSize(). */
static const char s_blur_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D u_image;\n"
    "uniform int u_horizontal;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);\n"
    "    vec2 tex_offset = 1.0 / vec2(textureSize(u_image, 0));\n"
    "    vec3 result = texture(u_image, v_uv).rgb * weight[0];\n"
    "    if (u_horizontal == 1) {\n"
    "        for (int i = 1; i < 5; i++) {\n"
    "            result += texture(u_image, v_uv + vec2(tex_offset.x * float(i), 0.0)).rgb * weight[i];\n"
    "            result += texture(u_image, v_uv - vec2(tex_offset.x * float(i), 0.0)).rgb * weight[i];\n"
    "        }\n"
    "    } else {\n"
    "        for (int i = 1; i < 5; i++) {\n"
    "            result += texture(u_image, v_uv + vec2(0.0, tex_offset.y * float(i))).rgb * weight[i];\n"
    "            result += texture(u_image, v_uv - vec2(0.0, tex_offset.y * float(i))).rgb * weight[i];\n"
    "        }\n"
    "    }\n"
    "    frag_color = vec4(result, 1.0);\n"
    "}\n";

/* --- Final composite fragment shader ---
 *
 * Combines scene + bloom, applies Reinhard tone mapping, exposure,
 * gamma correction, vignette darkening, and optional film grain.
 *
 * This shader is built lazily because it concatenates noise_shader_source()
 * for the grain effect (same pattern as sun_shader.c). */

static const char *s_composite_preamble =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D u_scene;\n"
    "uniform sampler2D u_bloom;\n"
    "uniform float u_bloom_intensity;\n"
    "uniform float u_exposure;\n"
    "uniform float u_gamma;\n"
    "uniform float u_vignette_strength;\n"
    "uniform float u_vignette_radius;\n"
    "uniform float u_grain_intensity;\n"
    "uniform float u_time;\n"
    "out vec4 frag_color;\n"
    "\n";

static const char *s_composite_body =
    "\n"
    "void main() {\n"
    "    vec3 scene = texture(u_scene, v_uv).rgb;\n"
    "    vec3 bloom = texture(u_bloom, v_uv).rgb;\n"
    "    vec3 color = scene + bloom * u_bloom_intensity;\n"
    "\n"
    "    /* Reinhard tone mapping (Erik Reinhard et al., 2002) */\n"
    "    color = color / (color + vec3(1.0));\n"
    "\n"
    "    /* Exposure */\n"
    "    color *= u_exposure;\n"
    "\n"
    "    /* Gamma correction */\n"
    "    color = pow(color, vec3(1.0 / u_gamma));\n"
    "\n"
    "    /* Vignette: darken edges based on distance from center */\n"
    "    float dist = length(v_uv - 0.5) * 2.0;\n"
    "    color *= 1.0 - u_vignette_strength * smoothstep(u_vignette_radius, 1.0, dist);\n"
    "\n"
    "    /* Film grain: subtle noise overlay */\n"
    "    float grain = (snoise2(v_uv * 500.0 + u_time) * 0.5 + 0.5) * u_grain_intensity;\n"
    "    color += grain;\n"
    "\n"
    "    frag_color = vec4(color, 1.0);\n"
    "}\n";

/* Concatenated composite: preamble + noise library + body */
static char s_full_composite[16384];
static int s_composite_built = 0;

static const char *build_composite_source(void)
{
    if (!s_composite_built) {
        char *dst = s_full_composite;
        const char *parts[] = {
            s_composite_preamble,
            noise_shader_source(),
            s_composite_body
        };
        for (int i = 0; i < 3; i++) {
            const char *src = parts[i];
            while (*src && (dst - s_full_composite) < 16382) {
                *dst++ = *src++;
            }
        }
        *dst = '\0';
        s_composite_built = 1;
    }
    return s_full_composite;
}

/* -----------------------------------------------------------------------
 * Public shader source accessors
 * ----------------------------------------------------------------------- */

const char *pp_quad_vert_source(void)
{
    return s_quad_vert;
}

const char *pp_bright_extract_frag_source(void)
{
    return s_bright_extract_frag;
}

const char *pp_blur_frag_source(void)
{
    return s_blur_frag;
}

const char *pp_composite_frag_source(void)
{
    return build_composite_source();
}
