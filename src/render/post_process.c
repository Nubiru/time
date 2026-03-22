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

    /* Depth of Field — GPU Gems 3, Ch 28 (Earl Hammon Jr., Infinity Ward) */
    cfg.dof_enabled     = 1;
    cfg.dof_focus_depth = 0.1f;
    cfg.dof_strength    = 0.5f;

    /* God rays — GPU Gems 3, Ch 13 (Kenny Mitchell, EA) */
    cfg.godrays_enabled  = 1;
    cfg.godrays_density  = 1.0f;
    cfg.godrays_weight   = 0.01f;
    cfg.godrays_decay    = 0.97f;
    cfg.godrays_exposure = 0.3f;

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
    if (config.godrays_enabled) {
        fbos += 1;
        textures += 1;
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
    if (config.godrays_enabled) {
        textures += 1;
    }

    /* RGBA16F = 4 channels * 2 bytes = 8 bytes per pixel */
    return textures * width * height * 8;
}

/* -----------------------------------------------------------------------
 * pp_pack_quad — fullscreen oversized triangle
 *
 * 3 vertices, each: position(vec2) + texcoord(vec2) = 4 floats
 * Single oversized triangle covering the full screen.
 * GPU clips the excess — bilinear sampling handles out-of-bounds UV.
 * 10% faster than a quad on AMD/Intel (RTR 4th Ed, Ch.12).
 * ----------------------------------------------------------------------- */

int pp_pack_quad(float *out)
{
    if (!out) return 0;

    /* Vertex 0: bottom-left: pos(-1,-1) uv(0,0) */
    out[0]  = -1.0f; out[1]  = -1.0f; out[2]  = 0.0f; out[3]  = 0.0f;
    /* Vertex 1: bottom-right-extended: pos(3,-1) uv(2,0) */
    out[4]  =  3.0f; out[5]  = -1.0f; out[6]  = 2.0f; out[7]  = 0.0f;
    /* Vertex 2: top-left-extended: pos(-1,3) uv(0,2) */
    out[8]  = -1.0f; out[9]  =  3.0f; out[10] = 0.0f; out[11] = 2.0f;

    return PP_QUAD_VERTEX_COUNT;  /* 3 */
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
 * Self-contained: scene + bloom, Reinhard tone mapping, exposure,
 * gamma correction, vignette, film grain (simple hash, no noise lib). */

static const char *s_composite_frag =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D u_scene;\n"
    "uniform sampler2D u_bloom;\n"
    "uniform sampler2D u_depth;\n"
    "uniform float u_bloom_intensity;\n"
    "uniform float u_exposure;\n"
    "uniform float u_gamma;\n"
    "uniform float u_vignette_strength;\n"
    "uniform float u_vignette_radius;\n"
    "uniform float u_grain_intensity;\n"
    "uniform float u_time;\n"
    "uniform float u_dof_focus;\n"
    "uniform float u_dof_strength;\n"
    "uniform vec4 u_tint;\n"
    "out vec4 frag_color;\n"
    "\n"
    "float grain_hash(vec2 p) {\n"
    "    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec3 scene = texture(u_scene, v_uv).rgb;\n"
    "\n"
    "    /* Depth of Field — 12-tap Poisson disc blur before tone mapping.\n"
    "     * GPU Gems 3, Ch 28 (Hammon/Infinity Ward) simplified. */\n"
    "    if (u_dof_strength > 0.001) {\n"
    "        float depth = texture(u_depth, v_uv).r;\n"
    "        float coc = clamp(abs(depth - u_dof_focus) * 8.0, 0.0, 1.0) * u_dof_strength;\n"
    "        if (coc > 0.01) {\n"
    "            vec2 texel = 1.0 / vec2(textureSize(u_scene, 0));\n"
    "            float r = coc * 6.0;\n"
    "            vec3 s = scene;\n"
    "            s += texture(u_scene, v_uv + vec2(-0.942,-0.399) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2( 0.946,-0.769) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2(-0.094,-0.929) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2( 0.345, 0.294) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2(-0.916, 0.458) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2(-0.815,-0.879) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2(-0.383, 0.277) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2( 0.975, 0.756) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2( 0.443,-0.975) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2( 0.537,-0.474) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2(-0.265,-0.419) * texel * r).rgb;\n"
    "            s += texture(u_scene, v_uv + vec2( 0.792, 0.191) * texel * r).rgb;\n"
    "            scene = mix(scene, s / 13.0, coc);\n"
    "        }\n"
    "    }\n"
    "\n"
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
    "    /* Vignette */\n"
    "    float dist = length(v_uv - 0.5) * 2.0;\n"
    "    color *= 1.0 - u_vignette_strength * smoothstep(u_vignette_radius, 1.0, dist);\n"
    "\n"
    "    /* Film grain */\n"
    "    float grain = (grain_hash(v_uv * 500.0 + u_time) - 0.5) * u_grain_intensity;\n"
    "    color += grain;\n"
    "\n"
    "    /* Depth tier tint — subtle color wash for navigation depth cues */\n"
    "    color = mix(color, u_tint.rgb, u_tint.a);\n"
    "\n"
    "    frag_color = vec4(color, 1.0);\n"
    "}\n";

/* --- God rays radial blur fragment shader (GPU Gems 3, Ch 13) --- */
static const char s_godrays_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform sampler2D u_frame;\n"
    "uniform vec2 u_sun_screen_pos;\n"
    "uniform float u_density;\n"
    "uniform float u_weight;\n"
    "uniform float u_decay;\n"
    "uniform float u_exposure;\n"
    "out vec4 frag_color;\n"
    "#define NUM_SAMPLES 64\n"
    "void main() {\n"
    "    vec2 delta = (v_uv - u_sun_screen_pos) / float(NUM_SAMPLES) * u_density;\n"
    "    vec2 tc = v_uv;\n"
    "    vec3 color = texture(u_frame, tc).rgb;\n"
    "    float illumination_decay = 1.0;\n"
    "    for (int i = 0; i < NUM_SAMPLES; i++) {\n"
    "        tc -= delta;\n"
    "        vec3 s = texture(u_frame, tc).rgb;\n"
    "        s *= illumination_decay * u_weight;\n"
    "        color += s;\n"
    "        illumination_decay *= u_decay;\n"
    "    }\n"
    "    frag_color = vec4(color * u_exposure, 1.0);\n"
    "}\n";

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
    return s_composite_frag;
}

const char *pp_godrays_frag_source(void)
{
    return s_godrays_frag;
}
