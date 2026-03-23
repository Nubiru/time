/*
 * wheel_pass.c — The Concentric Wheel of Time
 *
 * Step 1: Two golden circles on space-black.
 * The outer ring border and inner ring border of the navigation wheel.
 * PHI-proportioned. Anti-aliased via SDF.
 *
 * This pass will grow to include segments, glyphs, text, interaction.
 * But it starts with two circles. The skeleton of the wheel.
 *
 * Design sources: Tufte (data-ink), Da Vinci (flowing geometry),
 * Kepes (gaps are structure), Daniels (watchmaking precision),
 * Ware (3-object working memory), Iliinsky (5 default rings).
 */

#ifdef __EMSCRIPTEN__

#include "wheel_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../shader.h"

/* --- PHI constants --- */
#define PHI       1.6180339887f
#define PHI_INV   0.6180339887f   /* 1/φ = φ-1 */
#define PHI_INV2  0.3819660113f   /* φ⁻² */
#define PHI_INV3  0.2360679775f   /* φ⁻³ */
#define PI_F      3.14159265f

/* --- Colors (Solar gold on space-black) --- */
#define GOLD_R 1.0f
#define GOLD_G 0.85f
#define GOLD_B 0.55f

/* --- Geometry limits --- */
#define WHEEL_MAX_VERTS  8192
#define WHEEL_VERT_FLOATS   6    /* x, y, r, g, b, a */

/* --- Module-static GL handles --- */
static GLuint s_program;
static GLint  s_loc_resolution;
static GLint  s_loc_time;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

/* s_verts/s_vert_count reserved for future vertex-based ring segments */

/* --- SDF-quality circle shader ---
 *
 * Instead of drawing line segments, we render a fullscreen quad
 * and evaluate circle SDFs in the fragment shader. This gives
 * pixel-perfect anti-aliased rings at any resolution.
 */

static const char *VS_SRC =
    "#version 300 es\n"
    "precision highp float;\n"
    "layout(location=0) in vec2 a_pos;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "  v_uv = a_pos * 0.5 + 0.5;\n"
    "  gl_Position = vec4(a_pos, 0.0, 1.0);\n"
    "}\n";

static const char *FS_SRC =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform vec2 u_resolution;\n"
    "uniform float u_time;\n"
    "\n"
    "in vec2 v_uv;\n"
    "out vec4 frag_color;\n"
    "\n"
    "/* PHI constants */\n"
    "#define PHI     1.6180339887\n"
    "#define PHI_INV 0.6180339887\n"
    "#define PHI_INV2 0.3819660113\n"
    "#define PHI_INV3 0.2360679775\n"
    "\n"
    "/* Solar gold */\n"
    "#define GOLD vec3(1.0, 0.85, 0.55)\n"
    "\n"
    "/* SDF circle: returns signed distance to a ring edge */\n"
    "float sdf_ring(vec2 p, vec2 center, float radius, float thickness) {\n"
    "  float d = length(p - center) - radius;\n"
    "  return abs(d) - thickness * 0.5;\n"
    "}\n"
    "\n"
    "void main() {\n"
    "  vec2 px = v_uv * u_resolution;\n"
    "  float W = u_resolution.x;\n"
    "  float H = u_resolution.y;\n"
    "\n"
    "  /* Wheel center: golden point (slightly above vertical center) */\n"
    "  vec2 center = vec2(W * 0.5, H * PHI_INV);\n"
    "\n"
    "  /* Outer radius: fills ~62% of shortest dimension */\n"
    "  float R_outer = min(W, H) * PHI_INV * 0.5;\n"
    "\n"
    "  /* Inner radius: outer × φ⁻¹ */\n"
    "  float R_inner = R_outer * PHI_INV;\n"
    "\n"
    "  /* Ring width (the navigable band between outer and inner) */\n"
    "  float ring_width = R_outer - R_inner;\n"
    "\n"
    "  /* Gap between outer ring and inner display area */\n"
    "  float gap = ring_width * PHI_INV;\n"
    "\n"
    "  /* Inner display area border */\n"
    "  float R_display = R_inner - gap;\n"
    "\n"
    "  /* Center point radius */\n"
    "  float R_center = R_display * PHI_INV2;\n"
    "\n"
    "  /* Line weights (PHI cascade): 3px, 2px, 1px */\n"
    "  float line_outer = 3.0;\n"
    "  float line_inner = 3.0 * PHI_INV;  /* ~1.85px */\n"
    "  float line_display = 3.0 * PHI_INV2; /* ~1.15px */\n"
    "\n"
    "  /* Evaluate SDFs */\n"
    "  float d_outer = sdf_ring(px, center, R_outer, line_outer);\n"
    "  float d_inner = sdf_ring(px, center, R_inner, line_inner);\n"
    "  float d_display = sdf_ring(px, center, R_display, line_display);\n"
    "  float d_center_ring = sdf_ring(px, center, R_center, line_display);\n"
    "\n"
    "  /* Center point: filled circle with pulse */\n"
    "  float d_center_fill = length(px - center) - R_center * 0.3;\n"
    "  float pulse = 0.7 + 0.3 * sin(u_time * 2.0 * 3.14159 * PHI_INV);\n"
    "\n"
    "  /* Anti-aliased rendering via smoothstep */\n"
    "  float aa = 1.5;  /* anti-aliasing width in pixels */\n"
    "\n"
    "  /* Outer ring border: 60% opacity */\n"
    "  float alpha_outer = (1.0 - smoothstep(-aa, aa, d_outer)) * 0.60;\n"
    "\n"
    "  /* Inner ring border: 60% opacity */\n"
    "  float alpha_inner = (1.0 - smoothstep(-aa, aa, d_inner)) * 0.60;\n"
    "\n"
    "  /* Display area border: 38% opacity (φ⁻²) */\n"
    "  float alpha_display = (1.0 - smoothstep(-aa, aa, d_display)) * PHI_INV2;\n"
    "\n"
    "  /* Center ring: 38% opacity */\n"
    "  float alpha_center_ring = (1.0 - smoothstep(-aa, aa, d_center_ring)) * PHI_INV2;\n"
    "\n"
    "  /* Center fill: pulsing golden dot */\n"
    "  float alpha_center = (1.0 - smoothstep(-aa, aa, d_center_fill)) * 0.5 * pulse;\n"
    "\n"
    "  /* Combine all rings (max blending — no double-bright overlaps) */\n"
    "  float alpha = max(max(alpha_outer, alpha_inner),\n"
    "                    max(max(alpha_display, alpha_center_ring), alpha_center));\n"
    "\n"
    "  /* Subtle ring fill: very faint gold between outer and inner borders */\n"
    "  float dist_to_center = length(px - center);\n"
    "  float in_ring_band = step(R_inner, dist_to_center) * step(dist_to_center, R_outer);\n"
    "  float ring_fill = in_ring_band * 0.03;  /* 3% opacity fill — barely visible */\n"
    "\n"
    "  /* Subtle inner display fill */\n"
    "  float in_display = step(dist_to_center, R_display);\n"
    "  float display_fill = in_display * 0.02;  /* 2% opacity — hint of warmth */\n"
    "\n"
    "  alpha = max(alpha, max(ring_fill, display_fill));\n"
    "\n"
    "  if (alpha < 0.001) discard;\n"
    "\n"
    "  frag_color = vec4(GOLD * alpha, alpha);\n"
    "}\n";

/* Fullscreen quad vertices */
static const float QUAD_VERTS[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f,
};

void wheel_pass_init(void)
{
    if (s_initialized) return;

    s_program = shader_create_program(VS_SRC, FS_SRC);
    if (s_program == 0) return;

    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");
    s_loc_time = glGetUniformLocation(s_program, "u_time");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTS), QUAD_VERTS, GL_STATIC_DRAW);

    /* position: 2 floats */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    s_initialized = 1;
}

void wheel_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = (float)(frame->viewport_width > 0 ? frame->viewport_width : 1280);
    float H = (float)(frame->viewport_height > 0 ? frame->viewport_height : 720);

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);
    glUniform1f(s_loc_time, frame->time_sec);

    glBindVertexArray(s_vao);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    glUseProgram(0);
}

void wheel_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
/* Native stub */
#include "wheel_pass.h"
void wheel_pass_init(void) {}
void wheel_pass_draw(const render_frame_t *frame) { (void)frame; }
void wheel_pass_destroy(void) {}
#endif
