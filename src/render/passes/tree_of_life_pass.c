/*
 * tree_of_life_pass.c — Kabbalah Tree of Life render pass (S123)
 *
 * Renders the Tree of Life as a sacred geometric diagram:
 * 10 Sefirot nodes + Da'at, connected by 22 paths colored by
 * Hebrew letter classification (3 Mothers, 7 Doubles, 12 Simples).
 *
 * The Tree is the map of creation. Each path carries a Hebrew letter,
 * a Tarot arcanum, and a zodiac/planet/element attribution.
 * Sefer Yetzirah: "Twenty-two foundation letters... with them He
 * depicted all that was formed and all that would be formed."
 */

#ifdef __EMSCRIPTEN__

#include "tree_of_life_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../systems/kabbalah/tree_geometry.h"
#include "../../systems/kabbalah/sefirot.h"
#include "../shader.h"

#define TOL_MAX_VERTS    4096
#define TOL_VERT_FLOATS     6   /* x, y, r, g, b, a */
#define PI_F 3.14159265f

/* Sefirah colors (traditional Kircher/Golden Dawn attribution) */
static const float SEFIRAH_COLORS[11][3] = {
    { 1.0f, 1.0f, 1.0f },    /* 0 Keter — brilliant white */
    { 0.6f, 0.6f, 0.9f },    /* 1 Chokhmah — grey-blue (wisdom) */
    { 0.2f, 0.2f, 0.2f },    /* 2 Binah — black (understanding) */
    { 0.3f, 0.3f, 0.8f },    /* 3 Chesed — blue (mercy) */
    { 0.9f, 0.2f, 0.2f },    /* 4 Gevurah — red (severity) */
    { 1.0f, 0.85f, 0.3f },   /* 5 Tiferet — gold (beauty) */
    { 0.3f, 0.8f, 0.3f },    /* 6 Netzach — green (victory) */
    { 0.9f, 0.5f, 0.2f },    /* 7 Hod — orange (splendor) */
    { 0.7f, 0.3f, 0.8f },    /* 8 Yesod — violet (foundation) */
    { 0.4f, 0.3f, 0.2f },    /* 9 Malkhut — earth tones (kingdom) */
    { 0.5f, 0.5f, 0.5f },    /* 10 Da'at — grey (hidden knowledge) */
};

/* Path colors by letter type */
static const float MOTHER_COLORS[3][4] = {
    { 0.9f, 0.85f, 0.3f, 0.8f },  /* Aleph — Air — yellow */
    { 0.3f, 0.5f, 0.9f, 0.8f },   /* Mem — Water — blue */
    { 0.9f, 0.3f, 0.2f, 0.8f },   /* Shin — Fire — red */
};

/* Module-static GL handles */
static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

static float s_verts[TOL_MAX_VERTS * TOL_VERT_FLOATS];
static int   s_vert_count;

/* Screen-space 2D shaders (same as natal_chart_pass) */
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

/* --- Helpers --- */

static void push_vert(float x, float y, float r, float g, float b, float a)
{
    if (s_vert_count >= TOL_MAX_VERTS) return;
    int i = s_vert_count * TOL_VERT_FLOATS;
    s_verts[i+0] = x;  s_verts[i+1] = y;
    s_verts[i+2] = r;  s_verts[i+3] = g;
    s_verts[i+4] = b;  s_verts[i+5] = a;
    s_vert_count++;
}

static void push_line(float x1, float y1, float x2, float y2,
                       float r, float g, float b, float a)
{
    push_vert(x1, y1, r, g, b, a);
    push_vert(x2, y2, r, g, b, a);
}

static void push_circle(float cx, float cy, float radius,
                         float r, float g, float b, float a, int segments)
{
    for (int i = 0; i < segments; i++) {
        float a1 = 2.0f * PI_F * (float)i / (float)segments;
        float a2 = 2.0f * PI_F * (float)(i+1) / (float)segments;
        push_line(cx + cosf(a1) * radius, cy + sinf(a1) * radius,
                  cx + cosf(a2) * radius, cy + sinf(a2) * radius,
                  r, g, b, a);
    }
}

/* --- Public API --- */

void tree_of_life_pass_init(void)
{
    if (s_initialized) return;

    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 TOL_MAX_VERTS * TOL_VERT_FLOATS * sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          TOL_VERT_FLOATS * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          TOL_VERT_FLOATS * sizeof(float),
                          (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    s_initialized = 1;
}

void tree_of_life_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = 1280.0f;  /* default, should come from frame */
    float H = 720.0f;

    /* Generate tree layout scaled to screen */
    float tree_w = W * 0.35f;
    float tree_h = H * 0.85f;
    tree_layout_t layout = tree_layout_generate(tree_w, tree_h);

    /* Offset: right side of screen */
    float offset_x = W * 0.55f;
    float offset_y = H * 0.075f;

    s_vert_count = 0;

    /* --- Draw 22 paths (colored by letter type) --- */
    for (int p = 0; p < layout.line_count; p++) {
        tree_path_t path = tree_path_get(p);
        float r, g, b, a;

        if (path.letter_type == TREE_LETTER_MOTHER) {
            /* Mother letters: Fire/Water/Air colors */
            int mi = (p < 3) ? p : 0;
            r = MOTHER_COLORS[mi][0];
            g = MOTHER_COLORS[mi][1];
            b = MOTHER_COLORS[mi][2];
            a = MOTHER_COLORS[mi][3];
        } else if (path.letter_type == TREE_LETTER_DOUBLE) {
            /* Double letters: golden planetary glow */
            r = 1.0f; g = 0.85f; b = 0.55f; a = 0.5f;
        } else {
            /* Simple letters: teal zodiac glow */
            r = 0.2f; g = 0.75f; b = 0.8f; a = 0.4f;
        }

        push_line(layout.lines[p].x0 + offset_x,
                  layout.lines[p].y0 + offset_y,
                  layout.lines[p].x1 + offset_x,
                  layout.lines[p].y1 + offset_y,
                  r, g, b, a);
    }

    /* --- Draw 11 Sefirot nodes as circles --- */
    for (int n = 0; n < layout.node_count && n < 11; n++) {
        float nx = layout.node_x[n] + offset_x;
        float ny = layout.node_y[n] + offset_y;
        float radius = (n == 10) ? 8.0f : 12.0f;  /* Da'at smaller */
        float alpha = (n == 10) ? 0.4f : 0.8f;     /* Da'at dimmer */

        push_circle(nx, ny, radius,
                    SEFIRAH_COLORS[n][0],
                    SEFIRAH_COLORS[n][1],
                    SEFIRAH_COLORS[n][2],
                    alpha, 24);
    }

    /* --- Upload and render --- */
    if (s_vert_count < 2) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_vert_count * TOL_VERT_FLOATS * sizeof(float),
                    s_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_LINES, 0, s_vert_count);

    glBindVertexArray(0);
    glUseProgram(0);
}

void tree_of_life_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
void tree_of_life_pass_init(void) {}
void tree_of_life_pass_draw(const render_frame_t *frame) { (void)frame; }
void tree_of_life_pass_destroy(void) {}
#endif
