/*
 * bagua_pass.c — I Ching Bagua circular render pass (S124)
 *
 * Renders 8 trigrams in the Later Heaven (King Wen) arrangement:
 *   South(top): Li (Fire, ☲)
 *   North(bottom): Kan (Water, ☵)
 *   East(left): Zhen (Thunder, ☳)
 *   West(right): Dui (Lake, ☱)
 *   SE: Xun (Wind), NE: Gen (Mountain), SW: Kun (Earth), NW: Qian (Heaven)
 *
 * Each trigram is 3 horizontal lines (solid=yang, broken=yin) drawn
 * radially at the trigram's compass position.
 *
 * Art direction: ink-wash aesthetic. Warm ivory lines (#F5E6C8) on
 * dark background. NOT neon gold. Contemplative, ancient, quiet.
 */

#ifdef __EMSCRIPTEN__

#include "bagua_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../systems/iching/iching.h"
#include "../shader.h"

#define BAG_MAX_VERTS   1024
#define BAG_VERT_FLOATS    6
#define PI_F 3.14159265f

/* Later Heaven arrangement: index = position (0=South/top, clockwise)
 * Trigram bits: bottom to top, 0=yin, 1=yang.
 * Reserved for future radial line rendering. */
/*
static const int LATER_HEAVEN_TRIGRAMS[8] = {
    5, 2, 4, 7, 2, 1, 3, 6
};
*/

/* Correct Later Heaven trigram binary (bottom-to-top): */
static const int TRIGRAM_LINES[8][3] = {
    /* Li:   yang, yin, yang */  {1, 0, 1},
    /* Kun:  yin, yin, yin */    {0, 0, 0},
    /* Dui:  yin, yang, yang */  {0, 1, 1},
    /* Qian: yang, yang, yang */ {1, 1, 1},
    /* Kan:  yin, yang, yin */   {0, 1, 0},
    /* Gen:  yang, yin, yin */   {1, 0, 0},
    /* Zhen: yin, yin, yang */   {0, 0, 1},
    /* Xun:  yang, yang, yin */  {1, 1, 0},
};

/* Ink-wash colors */
static const float INK_IVORY[4] = { 0.96f, 0.90f, 0.78f, 0.85f };
static const float INK_DIM[4]   = { 0.96f, 0.90f, 0.78f, 0.3f };

/* Module-static GL handles */
static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

static float s_verts[BAG_MAX_VERTS * BAG_VERT_FLOATS];
static int   s_vert_count;

static const char *VS_SRC =
    "#version 300 es\n"
    "precision highp float;\n"
    "layout(location=0) in vec2 a_pos;\n"
    "layout(location=1) in vec4 a_color;\n"
    "uniform vec2 u_resolution;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "  vec2 ndc = (a_pos / u_resolution) * 2.0 - 1.0;\n"
    "  ndc.y = -ndc.y;\n"
    "  gl_Position = vec4(ndc, 0.0, 1.0);\n"
    "  v_color = a_color;\n"
    "}\n";

static const char *FS_SRC =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec4 v_color;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "  fragColor = v_color;\n"
    "}\n";

static void push_vert(float x, float y, float r, float g, float b, float a)
{
    if (s_vert_count >= BAG_MAX_VERTS) return;
    int i = s_vert_count * BAG_VERT_FLOATS;
    s_verts[i+0] = x; s_verts[i+1] = y;
    s_verts[i+2] = r; s_verts[i+3] = g;
    s_verts[i+4] = b; s_verts[i+5] = a;
    s_vert_count++;
}

static void push_line(float x1, float y1, float x2, float y2,
                       const float color[4])
{
    push_vert(x1, y1, color[0], color[1], color[2], color[3]);
    push_vert(x2, y2, color[0], color[1], color[2], color[3]);
}

/* Draw a single trigram line (solid or broken) at angle */
static void draw_trigram_line(float cx, float cy, float radius,
                               float angle_rad, float line_offset,
                               int is_yang, const float color[4],
                               float line_half_w)
{
    /* Line perpendicular to radial direction */
    float cos_a = cosf(angle_rad);
    float sin_a = sinf(angle_rad);

    /* Position along the radius */
    float px = cx + cos_a * (radius + line_offset);
    float py = cy + sin_a * (radius + line_offset);

    /* Perpendicular direction */
    float perp_x = -sin_a;
    float perp_y = cos_a;

    if (is_yang) {
        /* Solid line */
        push_line(px - perp_x * line_half_w, py - perp_y * line_half_w,
                  px + perp_x * line_half_w, py + perp_y * line_half_w,
                  color);
    } else {
        /* Broken line: two halves with gap */
        float gap = line_half_w * 0.25f;
        push_line(px - perp_x * line_half_w, py - perp_y * line_half_w,
                  px - perp_x * gap, py - perp_y * gap,
                  color);
        push_line(px + perp_x * gap, py + perp_y * gap,
                  px + perp_x * line_half_w, py + perp_y * line_half_w,
                  color);
    }
}

void bagua_pass_init(void)
{
    if (s_initialized) return;

    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 BAG_MAX_VERTS * BAG_VERT_FLOATS * sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          BAG_VERT_FLOATS * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          BAG_VERT_FLOATS * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    s_initialized = 1;
}

void bagua_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = 1280.0f;
    float H = 720.0f;

    /* Bagua circle on right side of screen */
    float cx = W * 0.65f;
    float cy = H * 0.5f;
    float radius = H * 0.3f;
    float line_half_w = 25.0f;

    s_vert_count = 0;

    /* Draw outer circle (faint) */
    int segs = 48;
    for (int i = 0; i < segs; i++) {
        float a1 = 2.0f * PI_F * (float)i / (float)segs;
        float a2 = 2.0f * PI_F * (float)(i+1) / (float)segs;
        push_line(cx + cosf(a1) * radius, cy + sinf(a1) * radius,
                  cx + cosf(a2) * radius, cy + sinf(a2) * radius,
                  INK_DIM);
    }

    /* Draw 8 trigrams at compass positions */
    for (int t = 0; t < 8; t++) {
        /* Angle: 0=top (South in Chinese), clockwise */
        float angle = (-PI_F / 2.0f) + (float)t * (2.0f * PI_F / 8.0f);

        /* 3 lines per trigram, spaced along the radius */
        for (int line = 0; line < 3; line++) {
            float offset = (float)(line - 1) * 12.0f;
            draw_trigram_line(cx, cy, radius, angle, offset,
                              TRIGRAM_LINES[t][line], INK_IVORY, line_half_w);
        }
    }

    /* Inner yin-yang circle (simple) */
    float inner_r = radius * 0.15f;
    for (int i = 0; i < 24; i++) {
        float a1 = 2.0f * PI_F * (float)i / 24.0f;
        float a2 = 2.0f * PI_F * (float)(i+1) / 24.0f;
        push_line(cx + cosf(a1) * inner_r, cy + sinf(a1) * inner_r,
                  cx + cosf(a2) * inner_r, cy + sinf(a2) * inner_r,
                  INK_IVORY);
    }

    if (s_vert_count < 2) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_vert_count * BAG_VERT_FLOATS * sizeof(float),
                    s_verts);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_LINES, 0, s_vert_count);
    glBindVertexArray(0);
    glUseProgram(0);
}

void bagua_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
void bagua_pass_init(void) {}
void bagua_pass_draw(const render_frame_t *frame) { (void)frame; }
void bagua_pass_destroy(void) {}
#endif
