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
    "void main() {\n"
    "  vec2 px = v_uv * u_resolution;\n"
    "  float W = u_resolution.x;\n"
    "  float H = u_resolution.y;\n"
    "\n"
    "  /* PHI constants */\n"
    "  float phi_inv = 0.6180339887;\n"
    "  float phi_inv2 = 0.3819660113;\n"
    "\n"
    "  /* Solar gold */\n"
    "  vec3 gold = vec3(1.0, 0.85, 0.55);\n"
    "\n"
    "  /* Wheel center: golden point */\n"
    "  vec2 wc = vec2(W * 0.5, H * phi_inv);\n"
    "\n"
    "  /* Radii */\n"
    "  float R_out = min(W, H) * phi_inv * 0.5;\n"
    "  float R_in = R_out * phi_inv;\n"
    "  float rw = R_out - R_in;\n"
    "  float gap = rw * phi_inv;\n"
    "  float R_disp = R_in - gap;\n"
    "  float R_ctr = R_disp * phi_inv2;\n"
    "\n"
    "  /* Distance from pixel to wheel center */\n"
    "  float dist = length(px - wc);\n"
    "\n"
    "  /* SDF for each ring border */\n"
    "  float d_out = abs(dist - R_out) - 1.5;\n"
    "  float d_in = abs(dist - R_in) - 1.0;\n"
    "  float d_disp = abs(dist - R_disp) - 0.6;\n"
    "  float d_ctr = abs(dist - R_ctr) - 0.6;\n"
    "\n"
    "  /* Center fill */\n"
    "  float d_fill = dist - R_ctr * 0.3;\n"
    "  float pulse = 0.7 + 0.3 * sin(u_time * 2.0 * 3.14159 * phi_inv);\n"
    "\n"
    "  /* Anti-aliased edges */\n"
    "  float aa = 1.5;\n"
    "  float a_out = (1.0 - smoothstep(-aa, aa, d_out)) * 0.6;\n"
    "  float a_in = (1.0 - smoothstep(-aa, aa, d_in)) * 0.6;\n"
    "  float a_disp = (1.0 - smoothstep(-aa, aa, d_disp)) * phi_inv2;\n"
    "  float a_ctr = (1.0 - smoothstep(-aa, aa, d_ctr)) * phi_inv2;\n"
    "  float a_fill = (1.0 - smoothstep(-aa, aa, d_fill)) * 0.5 * pulse;\n"
    "\n"
    "  /* Ring band fill (very subtle) */\n"
    "  float band = step(R_in, dist) * step(dist, R_out) * 0.03;\n"
    "  float inner = step(dist, R_disp) * 0.02;\n"
    "\n"
    "  /* Combine */\n"
    "  float alpha = max(max(a_out, a_in), max(max(a_disp, a_ctr), a_fill));\n"
    "  alpha = max(alpha, max(band, inner));\n"
    "\n"
    "  if (alpha < 0.001) discard;\n"
    "\n"
    "  frag_color = vec4(gold * alpha, alpha);\n"
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
