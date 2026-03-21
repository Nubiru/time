/*
 * gates_mandala_pass.c — Sefer Yetzirah 231 Gates mandala (S125)
 *
 * The most mathematically beautiful geometry in Time.
 * 22 Hebrew letters arranged on a circle. Every pair connected.
 * C(22,2) = 231 lines. The complete graph K₂₂.
 *
 * From Aryeh Kaplan's commentary on Sefer Yetzirah:
 * "231 Gates are the number of two-letter combinations that can
 *  be formed from the 22 letters. These represent all the roots
 *  of the Hebrew language, and hence, all basic concepts."
 *
 * The mandala reveals: 3 + 7 + 12 = 22 is THE universal pattern.
 * 3 Mothers (elements), 7 Doubles (planets), 12 Simples (zodiac).
 * Same structure appears in astrology, Chinese medicine, chakras.
 */

#ifdef __EMSCRIPTEN__

#include "gates_mandala_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../systems/kabbalah/tree_geometry.h"
#include "../shader.h"

#define GM_LETTER_COUNT   22
#define GM_GATE_COUNT    231    /* C(22,2) */
#define GM_MAX_VERTS    8192   /* 231 lines × 2 verts + 22 circles × 24 */
#define GM_VERT_FLOATS     6   /* x, y, r, g, b, a */
#define PI_F 3.14159265f

/* Letter type → visual properties */
/* Mother letters (3): large nodes, element colors */
static const float MOTHER_COLOR[3][4] = {
    { 0.9f, 0.85f, 0.3f, 0.95f },   /* Aleph — Air — golden yellow */
    { 0.3f, 0.5f, 0.9f, 0.95f },    /* Mem — Water — deep blue */
    { 0.9f, 0.3f, 0.2f, 0.95f },    /* Shin — Fire — sacred red */
};

/* Double letters (7): medium nodes, planetary warm gold */
static const float DOUBLE_COLOR[4] = { 1.0f, 0.85f, 0.55f, 0.8f };

/* Simple letters (12): small nodes, zodiac teal */
static const float SIMPLE_COLOR[4] = { 0.2f, 0.75f, 0.8f, 0.7f };

/* Gate line base color — very faint, barely visible */
static const float GATE_LINE_DIM[4] = { 1.0f, 0.85f, 0.55f, 0.04f };

/* Active gate highlight */
static const float GATE_LINE_ACTIVE[4] = { 1.0f, 0.85f, 0.55f, 0.35f };

/* Module-static GL handles */
static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

static float s_verts[GM_MAX_VERTS * GM_VERT_FLOATS];
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
    if (s_vert_count >= GM_MAX_VERTS) return;
    int i = s_vert_count * GM_VERT_FLOATS;
    s_verts[i+0] = x; s_verts[i+1] = y;
    s_verts[i+2] = r; s_verts[i+3] = g;
    s_verts[i+4] = b; s_verts[i+5] = a;
    s_vert_count++;
}

static void push_line(float x1, float y1, float x2, float y2,
                       const float c[4])
{
    push_vert(x1, y1, c[0], c[1], c[2], c[3]);
    push_vert(x2, y2, c[0], c[1], c[2], c[3]);
}

static void push_circle(float cx, float cy, float radius,
                         const float c[4], int segs)
{
    for (int i = 0; i < segs; i++) {
        float a1 = 2.0f * PI_F * (float)i / (float)segs;
        float a2 = 2.0f * PI_F * (float)(i+1) / (float)segs;
        push_vert(cx + cosf(a1)*radius, cy + sinf(a1)*radius, c[0], c[1], c[2], c[3]);
        push_vert(cx + cosf(a2)*radius, cy + sinf(a2)*radius, c[0], c[1], c[2], c[3]);
    }
}

void gates_mandala_pass_init(void)
{
    if (s_initialized) return;

    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 GM_MAX_VERTS * GM_VERT_FLOATS * sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          GM_VERT_FLOATS * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          GM_VERT_FLOATS * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    s_initialized = 1;
}

void gates_mandala_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = (float)(frame->viewport_width > 0 ? frame->viewport_width : 1280);
    float H = (float)(frame->viewport_height > 0 ? frame->viewport_height : 720);

    /* Mandala centered in right portion of screen */
    float cx = W * 0.62f;
    float cy = H * 0.5f;
    float radius = (W < H ? W : H) * 0.35f;

    s_vert_count = 0;

    /* Compute 22 letter positions on the circle */
    float letter_x[GM_LETTER_COUNT];
    float letter_y[GM_LETTER_COUNT];

    for (int i = 0; i < GM_LETTER_COUNT; i++) {
        /* Start from top, go clockwise */
        float angle = -PI_F / 2.0f + 2.0f * PI_F * (float)i / (float)GM_LETTER_COUNT;
        letter_x[i] = cx + cosf(angle) * radius;
        letter_y[i] = cy + sinf(angle) * radius;
    }

    /* --- Draw 231 gate lines (every pair) --- */
    /* Determine today's active letter for highlighting */
    int active_letter = -1;
    /* Simple: use day-of-week mapped to letter classification */
    /* More accurate: derive from Hebrew calendar date via tree_path */
    {
        /* Approximate: cycle through 22 letters based on JD */
        int day_cycle = (int)(frame->simulation_jd) % 22;
        if (day_cycle >= 0 && day_cycle < 22)
            active_letter = day_cycle;
    }

    for (int a = 0; a < GM_LETTER_COUNT; a++) {
        for (int b = a + 1; b < GM_LETTER_COUNT; b++) {
            /* Highlight lines connected to today's active letter */
            int is_active = (a == active_letter || b == active_letter);
            const float *color = is_active ? GATE_LINE_ACTIVE : GATE_LINE_DIM;
            push_line(letter_x[a], letter_y[a],
                      letter_x[b], letter_y[b], color);
        }
    }

    /* --- Draw 22 letter nodes --- */
    for (int i = 0; i < GM_LETTER_COUNT; i++) {
        tree_path_t path = tree_path_get(i);
        float node_radius;
        const float *color;

        if (path.letter_type == TREE_LETTER_MOTHER) {
            node_radius = 8.0f;
            /* Map to specific mother color (0=Aleph, depends on path order) */
            int mi = 0;
            if (i < 3) mi = i; /* First 3 paths are mothers in most orderings */
            color = MOTHER_COLOR[mi];
        } else if (path.letter_type == TREE_LETTER_DOUBLE) {
            node_radius = 6.0f;
            color = DOUBLE_COLOR;
        } else {
            node_radius = 4.0f;
            color = SIMPLE_COLOR;
        }

        /* Active letter gets extra glow */
        if (i == active_letter) {
            /* Outer glow ring */
            float glow[4] = { color[0], color[1], color[2], 0.3f };
            push_circle(letter_x[i], letter_y[i], node_radius * 2.5f, glow, 24);
        }

        push_circle(letter_x[i], letter_y[i], node_radius, color, 16);
    }

    /* --- Outer circle (faint) --- */
    {
        float ring_color[4] = { 1.0f, 0.85f, 0.55f, 0.12f };
        push_circle(cx, cy, radius + 15.0f, ring_color, 48);
    }

    /* --- Upload and render --- */
    if (s_vert_count < 2) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_vert_count * GM_VERT_FLOATS * sizeof(float),
                    s_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_LINES, 0, s_vert_count);

    glBindVertexArray(0);
    glUseProgram(0);
}

void gates_mandala_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
void gates_mandala_pass_init(void) {}
void gates_mandala_pass_draw(const render_frame_t *frame) { (void)frame; }
void gates_mandala_pass_destroy(void) {}
#endif
