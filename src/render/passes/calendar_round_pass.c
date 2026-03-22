/*
 * calendar_round_pass.c — Calendar Round mandala render pass (S104)
 *
 * 52 sections around a wheel, each representing one year of the
 * Calendar Round cycle. Color-coded by the year-bearer seal's color
 * family (Red/White/Blue/Yellow). Current year highlighted.
 *
 * The Calendar Round is the most important cycle in Mesoamerican
 * timekeeping: 52 Haab years = 73 Tzolkin cycles = 18,980 days.
 * Every culture in pre-Columbian America tracked this cycle.
 */

#ifdef __EMSCRIPTEN__

#include "calendar_round_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../systems/tzolkin/tzolkin.h"
#include "../../systems/tzolkin/dreamspell.h"
#include "../shader.h"

#define CR_SECTIONS     52
#define CR_MAX_VERTS  4096
#define CR_VERT_FLOATS   6
#define PI_F 3.14159265f

/* Year-bearer colors (4 families cycling through 52 years) */
static const float FAMILY_COLORS[4][4] = {
    { 0.9f, 0.3f, 0.2f, 0.6f },    /* Red — initiate */
    { 0.9f, 0.9f, 0.95f, 0.5f },   /* White — refine */
    { 0.3f, 0.5f, 0.9f, 0.6f },    /* Blue — transform */
    { 1.0f, 0.85f, 0.4f, 0.6f },   /* Yellow — ripen */
};

static const float HIGHLIGHT[4] = { 1.0f, 0.85f, 0.55f, 0.95f };
static const float DIM_GOLD[4] = { 1.0f, 0.85f, 0.55f, 0.15f };

static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

static float s_verts[CR_MAX_VERTS * CR_VERT_FLOATS];
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
    if (s_vert_count >= CR_MAX_VERTS) return;
    int i = s_vert_count * CR_VERT_FLOATS;
    s_verts[i+0] = x; s_verts[i+1] = y;
    s_verts[i+2] = r; s_verts[i+3] = g;
    s_verts[i+4] = b; s_verts[i+5] = a;
    s_vert_count++;
}

static void push_arc_filled(float cx, float cy, float r_inner, float r_outer,
                             float start_deg, float end_deg,
                             const float color[4], int segs)
{
    for (int i = 0; i < segs; i++) {
        float a1 = (start_deg + (end_deg - start_deg) * (float)i / (float)segs) * PI_F / 180.0f;
        float a2 = (start_deg + (end_deg - start_deg) * (float)(i+1) / (float)segs) * PI_F / 180.0f;

        /* Two triangles per segment (inner-outer quad) */
        float ix1 = cx + cosf(a1) * r_inner, iy1 = cy + sinf(a1) * r_inner;
        float ox1 = cx + cosf(a1) * r_outer, oy1 = cy + sinf(a1) * r_outer;
        float ix2 = cx + cosf(a2) * r_inner, iy2 = cy + sinf(a2) * r_inner;
        float ox2 = cx + cosf(a2) * r_outer, oy2 = cy + sinf(a2) * r_outer;

        push_vert(ix1, iy1, color[0], color[1], color[2], color[3]);
        push_vert(ox1, oy1, color[0], color[1], color[2], color[3]);
        push_vert(ix2, iy2, color[0], color[1], color[2], color[3]);

        push_vert(ox1, oy1, color[0], color[1], color[2], color[3]);
        push_vert(ox2, oy2, color[0], color[1], color[2], color[3]);
        push_vert(ix2, iy2, color[0], color[1], color[2], color[3]);
    }
}

void calendar_round_pass_init(void)
{
    if (s_initialized) return;
    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, CR_MAX_VERTS * CR_VERT_FLOATS * sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, CR_VERT_FLOATS * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, CR_VERT_FLOATS * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    s_initialized = 1;
}

void calendar_round_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = (float)(frame->viewport_width > 0 ? frame->viewport_width : 1280);
    float H = (float)(frame->viewport_height > 0 ? frame->viewport_height : 720);

    float cx = W * 0.62f;
    float cy = H * 0.5f;
    float r_outer = (W < H ? W : H) * 0.38f;
    float r_inner = r_outer * 0.55f;

    /* Compute current year in Calendar Round (0-51) */
    int kin = tzolkin_kin_number(frame->simulation_jd);
    int year_in_round = (kin / 365) % 52; /* approximate */

    s_vert_count = 0;

    /* Draw 52 sections */
    float section_deg = 360.0f / 52.0f;
    for (int y = 0; y < 52; y++) {
        float start = -90.0f + (float)y * section_deg;
        float end = start + section_deg - 0.5f; /* tiny gap between sections */

        int color_family = y % 4;
        int is_current = (y == year_in_round);

        const float *color = is_current ? HIGHLIGHT : FAMILY_COLORS[color_family];
        float ri = is_current ? r_inner * 0.9f : r_inner; /* current year section extends inward */

        push_arc_filled(cx, cy, ri, r_outer, start, end, color, 3);
    }

    /* Inner circle (faint gold) */
    {
        int segs = 48;
        for (int i = 0; i < segs; i++) {
            float a1 = 2.0f * PI_F * (float)i / (float)segs;
            float a2 = 2.0f * PI_F * (float)(i+1) / (float)segs;
            push_vert(cx + cosf(a1) * r_inner, cy + sinf(a1) * r_inner,
                      DIM_GOLD[0], DIM_GOLD[1], DIM_GOLD[2], DIM_GOLD[3]);
            push_vert(cx + cosf(a2) * r_inner, cy + sinf(a2) * r_inner,
                      DIM_GOLD[0], DIM_GOLD[1], DIM_GOLD[2], DIM_GOLD[3]);
        }
    }

    if (s_vert_count < 3) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_vert_count * CR_VERT_FLOATS * sizeof(float), s_verts);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Filled triangles for sections, then lines for inner circle */
    int tri_count = 52 * 3 * 6; /* 52 sections × 3 subsegments × 6 verts */
    if (tri_count > s_vert_count) tri_count = s_vert_count - 96;
    glDrawArrays(GL_TRIANGLES, 0, tri_count);
    glDrawArrays(GL_LINES, tri_count, s_vert_count - tri_count);

    glBindVertexArray(0);
    glUseProgram(0);
}

void calendar_round_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
void calendar_round_pass_init(void) {}
void calendar_round_pass_draw(const render_frame_t *frame) { (void)frame; }
void calendar_round_pass_destroy(void) {}
#endif
