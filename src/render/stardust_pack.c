/*
 * stardust_pack.c — Procedural background star field shader source
 *
 * Hash-based noise for deterministic star positions on a fullscreen quad.
 * Stars are generated procedurally in the fragment shader using Voronoi-like
 * cell partitioning: divide screen into cells, hash each cell to get a star
 * position within it, then compute brightness based on distance from center.
 *
 * Pure module: no GL calls, no malloc, no globals.
 */

#include "stardust_pack.h"

sd_config_t sd_default_config(void)
{
    sd_config_t cfg;
    cfg.density = 0.5f;
    cfg.brightness = 0.3f;
    cfg.twinkle_speed = 0.5f;
    return cfg;
}

/* Vertex shader: passthrough for fullscreen quad */
static const char s_sd_vert[] =
    "#version 300 es\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec2 a_texcoord;\n"
    "uniform mat4 u_mvp;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "    v_uv = a_texcoord;\n"
    "    gl_Position = u_mvp * vec4(a_position, 0.0, 1.0);\n"
    "}\n";

/* Fragment shader: procedural star field
 *
 * Grid-based approach: divide the sky into cells, each cell may contain
 * one star. The hash determines star position within the cell, brightness,
 * and color temperature. Multiple grid scales create depth layers.
 *
 * Color: stars have subtle spectral variation — cool blue, warm yellow,
 * white, following the B-V color index distribution.
 */
static const char s_sd_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform float u_time;\n"
    "uniform float u_density;\n"
    "uniform float u_brightness;\n"
    "uniform float u_zoom_alpha;\n"
    "out vec4 frag_color;\n"
    "\n"
    "/* Hash functions for deterministic star placement */\n"
    "float hash21(vec2 p) {\n"
    "    vec3 p3 = fract(vec3(p.xyx) * 0.1031);\n"
    "    p3 += dot(p3, p3.yzx + 33.33);\n"
    "    return fract((p3.x + p3.y) * p3.z);\n"
    "}\n"
    "\n"
    "vec2 hash22(vec2 p) {\n"
    "    vec3 p3 = fract(vec3(p.xyx) * vec3(0.1031, 0.1030, 0.0973));\n"
    "    p3 += dot(p3, p3.yzx + 33.33);\n"
    "    return fract((p3.xx + p3.yz) * p3.zy);\n"
    "}\n"
    "\n"
    "/* Star color from spectral hash (0=blue, 0.5=white, 1=warm yellow) */\n"
    "vec3 star_spectral_color(float h) {\n"
    "    vec3 cool = vec3(0.7, 0.8, 1.0);   /* blue-white O/B stars */\n"
    "    vec3 warm = vec3(1.0, 0.9, 0.7);   /* yellow-white G/K stars */\n"
    "    return mix(cool, warm, h);\n"
    "}\n"
    "\n"
    "/* One layer of stars at a given grid scale */\n"
    "vec3 star_layer(vec2 uv, float scale, float threshold) {\n"
    "    vec2 grid = floor(uv * scale);\n"
    "    vec2 frac_uv = fract(uv * scale);\n"
    "\n"
    "    /* Hash determines if this cell has a star */\n"
    "    float h = hash21(grid);\n"
    "    if (h > threshold) return vec3(0.0);\n"
    "\n"
    "    /* Star position within cell */\n"
    "    vec2 star_pos = hash22(grid + vec2(7.13, 3.71));\n"
    "    float dist = length(frac_uv - star_pos);\n"
    "\n"
    "    /* Star brightness: inverse square falloff, magnitude distribution */\n"
    "    float mag = hash21(grid + vec2(1.7, 9.2));\n"
    "    float brightness = pow(mag, 3.0);  /* few bright, many dim */\n"
    "\n"
    "    /* Point-like with tiny glow (1-2 pixels) */\n"
    "    float pixel_size = 0.5 / scale;\n"
    "    float star = exp(-dist * dist / (pixel_size * pixel_size)) * brightness;\n"
    "\n"
    "    /* Subtle twinkle */\n"
    "    float twinkle = 0.8 + 0.2 * sin(u_time * (1.0 + mag * 3.0) + h * 6.28);\n"
    "    star *= twinkle;\n"
    "\n"
    "    /* Spectral color */\n"
    "    vec3 color = star_spectral_color(hash21(grid + vec2(5.3, 2.1)));\n"
    "    return color * star;\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec3 stars = vec3(0.0);\n"
    "\n"
    "    /* 3 layers at different scales for depth */\n"
    "    float thresh = 1.0 - u_density;\n"
    "    stars += star_layer(v_uv, 200.0, thresh * 0.3) * 1.0;   /* sparse bright */\n"
    "    stars += star_layer(v_uv, 400.0, thresh * 0.5) * 0.6;   /* medium density */\n"
    "    stars += star_layer(v_uv, 800.0, thresh * 0.7) * 0.3;   /* dense faint */\n"
    "\n"
    "    stars *= u_brightness * u_zoom_alpha;\n"
    "\n"
    "    /* Discard near-black pixels for performance */\n"
    "    float lum = dot(stars, vec3(0.299, 0.587, 0.114));\n"
    "    if (lum < 0.005) discard;\n"
    "\n"
    "    frag_color = vec4(stars, lum);\n"
    "}\n";

const char *sd_vert_source(void) { return s_sd_vert; }
const char *sd_frag_source(void) { return s_sd_frag; }
