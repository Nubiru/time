/*
 * precession_pass.c — Precession clock (S90)
 *
 * The Great Year: Earth's axis traces a circle every 25,772 years.
 * Each 30° arc = one Astrological Age (~2,148 years).
 * We are transitioning from Pisces to Aquarius.
 *
 * The clock shows: where we are in the cycle, which age is active,
 * and how much of the Great Year has elapsed since a reference epoch.
 */

#ifdef __EMSCRIPTEN__

#include "precession_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include <math.h>

#include "../../systems/astronomy/precession.h"
#include "../shader.h"

#define PC_MAX_VERTS  2048
#define PC_VERT_FLOATS   6
#define PI_F 3.14159265f

#define GREAT_YEAR 25772.0  /* years */
#define AGE_YEARS  (GREAT_YEAR / 12.0)  /* ~2148 years per age */

/* Zodiac age names (reverse order — precession goes backward) */
static const char *AGE_NAMES[12] = {
    "Aquarius", "Pisces", "Aries", "Taurus", "Gemini", "Cancer",
    "Leo", "Virgo", "Libra", "Scorpio", "Sagittarius", "Capricorn"
};

/* Age colors matching zodiac element */
static const float AGE_COLORS[12][4] = {
    { 0.9f, 0.85f, 0.3f, 0.5f },   /* Aquarius — Air/Yellow */
    { 0.3f, 0.5f, 0.9f, 0.5f },    /* Pisces — Water/Blue */
    { 0.9f, 0.3f, 0.2f, 0.5f },    /* Aries — Fire/Red */
    { 0.5f, 0.8f, 0.3f, 0.5f },    /* Taurus — Earth/Green */
    { 0.9f, 0.85f, 0.3f, 0.5f },   /* Gemini — Air */
    { 0.3f, 0.5f, 0.9f, 0.5f },    /* Cancer — Water */
    { 0.9f, 0.3f, 0.2f, 0.5f },    /* Leo — Fire */
    { 0.5f, 0.8f, 0.3f, 0.5f },    /* Virgo — Earth */
    { 0.9f, 0.85f, 0.3f, 0.5f },   /* Libra — Air */
    { 0.3f, 0.5f, 0.9f, 0.5f },    /* Scorpio — Water */
    { 0.9f, 0.3f, 0.2f, 0.5f },    /* Sagittarius — Fire */
    { 0.5f, 0.8f, 0.3f, 0.5f },    /* Capricorn — Earth */
};

static const float HIGHLIGHT[4] = { 1.0f, 0.85f, 0.55f, 0.9f };
static const float DIM_GOLD[4] = { 1.0f, 0.85f, 0.55f, 0.12f };

static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

static float s_verts[PC_MAX_VERTS * PC_VERT_FLOATS];
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
    if (s_vert_count >= PC_MAX_VERTS) return;
    int i = s_vert_count * PC_VERT_FLOATS;
    s_verts[i+0]=x; s_verts[i+1]=y; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a;
    s_vert_count++;
}

static void push_arc_line(float cx, float cy, float r, float start, float end,
                           const float c[4], int segs)
{
    for (int i = 0; i < segs; i++) {
        float a1 = (start + (end-start) * (float)i/(float)segs) * PI_F / 180.0f;
        float a2 = (start + (end-start) * (float)(i+1)/(float)segs) * PI_F / 180.0f;
        push_vert(cx+cosf(a1)*r, cy+sinf(a1)*r, c[0], c[1], c[2], c[3]);
        push_vert(cx+cosf(a2)*r, cy+sinf(a2)*r, c[0], c[1], c[2], c[3]);
    }
}

void precession_pass_init(void)
{
    if (s_initialized) return;
    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");
    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_verts), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, PC_VERT_FLOATS*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, PC_VERT_FLOATS*sizeof(float), (void*)(2*sizeof(float)));
    glBindVertexArray(0);
    s_initialized = 1;
}

void precession_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = (float)(frame->viewport_width > 0 ? frame->viewport_width : 1280);
    float H = (float)(frame->viewport_height > 0 ? frame->viewport_height : 720);

    float cx = W * 0.62f;
    float cy = H * 0.5f;
    float r_outer = (W < H ? W : H) * 0.35f;
    float r_inner = r_outer * 0.7f;

    /* Compute current position in Great Year */
    /* Reference: vernal equinox was at 0° Aries around 68 BCE */
    double jd = frame->simulation_jd;
    double years_since_ref = (jd - 2434468.5) / 365.25; /* J1953.0 reference */
    double prec_angle = fmod(years_since_ref / GREAT_YEAR * 360.0, 360.0);
    if (prec_angle < 0) prec_angle += 360.0;

    int current_age = (int)(prec_angle / 30.0) % 12;

    s_vert_count = 0;

    /* 12 age arcs */
    for (int a = 0; a < 12; a++) {
        float start = -90.0f + (float)a * 30.0f;
        float end = start + 29.5f;
        int is_current = (a == current_age);
        const float *color = is_current ? HIGHLIGHT : AGE_COLORS[a];

        push_arc_line(cx, cy, r_outer, start, end, color, 8);
        push_arc_line(cx, cy, r_inner, start, end, color, 8);

        /* Radial divider */
        float rad = start * PI_F / 180.0f;
        push_vert(cx+cosf(rad)*r_inner, cy+sinf(rad)*r_inner, DIM_GOLD[0], DIM_GOLD[1], DIM_GOLD[2], DIM_GOLD[3]);
        push_vert(cx+cosf(rad)*r_outer, cy+sinf(rad)*r_outer, DIM_GOLD[0], DIM_GOLD[1], DIM_GOLD[2], DIM_GOLD[3]);
    }

    /* Current position indicator — bright radial line */
    {
        float rad = (-90.0f + (float)prec_angle) * PI_F / 180.0f;
        push_vert(cx+cosf(rad)*r_inner*0.9f, cy+sinf(rad)*r_inner*0.9f, 1.0f, 1.0f, 1.0f, 0.9f);
        push_vert(cx+cosf(rad)*r_outer*1.05f, cy+sinf(rad)*r_outer*1.05f, 1.0f, 1.0f, 1.0f, 0.9f);
    }

    if (s_vert_count < 2) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, s_vert_count * PC_VERT_FLOATS * sizeof(float), s_verts);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_LINES, 0, s_vert_count);
    glBindVertexArray(0);
    glUseProgram(0);
}

void precession_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
void precession_pass_init(void) {}
void precession_pass_draw(const render_frame_t *frame) { (void)frame; }
void precession_pass_destroy(void) {}
#endif
