/*
 * billboard_shader.c — GPU-side billboard computation shader source
 *
 * GLSL ES 3.00 vertex shader that extracts camera right/up vectors from
 * the view matrix and computes billboard quad corners entirely on the GPU.
 * Fragment shader is a simple textured-tint passthrough.
 *
 * Quad geometry helpers pack offsets, UVs, and triangle indices for
 * a standard unit billboard quad.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "billboard_shader.h"

/* --- Billboard vertex shader (GLSL ES 3.00) ---
 *
 * Camera right = column 0 of view matrix (rows 0-2)
 * Camera up    = column 1 of view matrix (rows 0-2)
 *
 * World position = center + right * offset.x * size + up * offset.y * size
 */
static const char *s_vert_source =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "layout(location = 0) in vec3 a_center;\n"
    "layout(location = 1) in vec2 a_offset;\n"
    "layout(location = 2) in float a_size;\n"
    "layout(location = 3) in vec2 a_uv;\n"
    "\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "\n"
    "out vec2 v_uv;\n"
    "\n"
    "void main() {\n"
    "    /* Extract camera axes from view matrix columns */\n"
    "    vec3 cam_right = vec3(u_view[0][0], u_view[1][0], u_view[2][0]);\n"
    "    vec3 cam_up    = vec3(u_view[0][1], u_view[1][1], u_view[2][1]);\n"
    "\n"
    "    /* Compute billboard corner in world space */\n"
    "    vec3 world_pos = a_center\n"
    "        + cam_right * a_offset.x * a_size\n"
    "        + cam_up    * a_offset.y * a_size;\n"
    "\n"
    "    v_uv = a_uv;\n"
    "    gl_Position = u_proj * u_view * vec4(world_pos, 1.0);\n"
    "}\n";

/* --- Billboard fragment shader (GLSL ES 3.00) ---
 *
 * Simple textured quad with color tint and alpha test. */
static const char *s_frag_source =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec2 v_uv;\n"
    "\n"
    "uniform sampler2D u_texture;\n"
    "uniform vec4 u_color;\n"
    "\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    vec4 tex = texture(u_texture, v_uv);\n"
    "    frag_color = tex * u_color;\n"
    "}\n";

const char *billboard_shader_vert_source(void)
{
    return s_vert_source;
}

const char *billboard_shader_frag_source(void)
{
    return s_frag_source;
}

int billboard_shader_pack_offsets(float *out)
{
    if (!out) return 0;

    /* 4 vertices: BL, BR, TR, TL — unit quad [-1, 1] */
    out[0] = -1.0f; out[1] = -1.0f;  /* bottom-left */
    out[2] =  1.0f; out[3] = -1.0f;  /* bottom-right */
    out[4] =  1.0f; out[5] =  1.0f;  /* top-right */
    out[6] = -1.0f; out[7] =  1.0f;  /* top-left */

    return 4;
}

int billboard_shader_pack_uvs(float *out)
{
    if (!out) return 0;

    /* UVs match offset layout: BL, BR, TR, TL */
    out[0] = 0.0f; out[1] = 0.0f;  /* bottom-left */
    out[2] = 1.0f; out[3] = 0.0f;  /* bottom-right */
    out[4] = 1.0f; out[5] = 1.0f;  /* top-right */
    out[6] = 0.0f; out[7] = 1.0f;  /* top-left */

    return 4;
}

int billboard_shader_pack_indices(unsigned int *out)
{
    if (!out) return 0;

    /* Two triangles, CCW winding:
     * Triangle 1: BL(0), BR(1), TR(2)
     * Triangle 2: BL(0), TR(2), TL(3) */
    out[0] = 0; out[1] = 1; out[2] = 2;
    out[3] = 0; out[4] = 2; out[5] = 3;

    return 6;
}
