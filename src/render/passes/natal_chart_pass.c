/*
 * natal_chart_pass.c — Natal chart wheel render pass (S121)
 *
 * Stateful module (S1): renders a traditional astrology natal chart
 * wheel as a mandala — concentric rings with house cusps, planet
 * positions, and aspect lines.
 *
 * Jung said: "the horoscope is itself a mandala." We render it as one.
 *
 * Geometry approach:
 *   - Circle arcs approximated as line strip segments (36 per arc)
 *   - Cusps as thick quads (same technique as convergence_pass)
 *   - Planets as small colored quads
 *   - Aspects as thin colored lines inside the inner ring
 *   - All rendered in screen-space (2D overlay), not world-space
 *
 * Uses a simple position(2) + color(4) vertex format with orthographic
 * projection. The chart occupies the right half of the screen when
 * Astrology focus mode is active.
 */

#ifdef __EMSCRIPTEN__

#include "natal_chart_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../ui/natal_chart_layout.h"
#include "../../systems/astrology/zodiac.h"
#include "../../systems/astronomy/planets.h"
#include "../color_theory.h"
#include "../shader.h"

/* --- Constants --- */

#define NC_ARC_SEGMENTS    72   /* segments per full circle */
#define NC_MAX_VERTS     2048
#define NC_VERT_FLOATS      6   /* x, y, r, g, b, a */

#define PI_F 3.14159265f

/* Aspect colors (from Inner Sky extraction: Trine=harmony, Square=friction) */
static const float ASPECT_COLORS[5][4] = {
    { 1.0f, 0.85f, 0.55f, 0.9f },  /* Conjunction — solar gold (fusion) */
    { 0.9f, 0.25f, 0.25f, 0.7f },  /* Opposition — red (tension) */
    { 0.85f, 0.3f, 0.2f, 0.6f },   /* Square — warm red (friction) */
    { 0.3f, 0.6f, 0.9f, 0.7f },    /* Trine — celestial blue (harmony) */
    { 0.3f, 0.8f, 0.5f, 0.6f },    /* Sextile — green (excitation) */
};

/* Element colors for zodiac sign arcs */
static const float ELEMENT_COLORS[4][4] = {
    { 0.9f, 0.3f, 0.2f, 0.4f },    /* Fire — red (Aries, Leo, Sagittarius) */
    { 0.6f, 0.8f, 0.3f, 0.4f },    /* Earth — green (Taurus, Virgo, Capricorn) */
    { 0.9f, 0.85f, 0.3f, 0.4f },   /* Air — yellow (Gemini, Libra, Aquarius) */
    { 0.3f, 0.5f, 0.9f, 0.4f },    /* Water — blue (Cancer, Scorpio, Pisces) */
};

/* Sign → element: Fire=0, Earth=1, Air=2, Water=3 */
static const int SIGN_ELEMENT[12] = {
    0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3
};

/* --- Module-static GL handles --- */

static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

/* --- Vertex buffer --- */

static float s_verts[NC_MAX_VERTS * NC_VERT_FLOATS];
static int   s_vert_count;

/* --- Vertex shader: screen-space 2D --- */

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

/* --- Fragment shader: simple colored --- */

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
    if (s_vert_count >= NC_MAX_VERTS) return;
    int i = s_vert_count * NC_VERT_FLOATS;
    s_verts[i+0] = x;
    s_verts[i+1] = y;
    s_verts[i+2] = r;
    s_verts[i+3] = g;
    s_verts[i+4] = b;
    s_verts[i+5] = a;
    s_vert_count++;
}

static void push_line(float x1, float y1, float x2, float y2,
                       float r, float g, float b, float a)
{
    push_vert(x1, y1, r, g, b, a);
    push_vert(x2, y2, r, g, b, a);
}

static void push_arc(float cx, float cy, float radius,
                      float start_deg, float end_deg,
                      float r, float g, float b, float a,
                      float screen_w, float screen_h)
{
    float scale = (screen_w < screen_h) ? screen_w : screen_h;
    float steps = 36.0f;
    float span = end_deg - start_deg;

    for (int i = 0; i < (int)steps; i++) {
        float a1 = (start_deg + span * (float)i / steps) * PI_F / 180.0f;
        float a2 = (start_deg + span * (float)(i+1) / steps) * PI_F / 180.0f;

        float x1 = cx * screen_w + cosf(a1) * radius * scale;
        float y1 = cy * screen_h + sinf(a1) * radius * scale;
        float x2 = cx * screen_w + cosf(a2) * radius * scale;
        float y2 = cy * screen_h + sinf(a2) * radius * scale;

        push_line(x1, y1, x2, y2, r, g, b, a);
    }
}

static void push_point_to_screen(const chart_point_t *p,
                                  float screen_w, float screen_h,
                                  float *out_x, float *out_y)
{
    /* natal_chart_layout uses 0-1 normalized. Map to screen pixels. */
    /* Chart centered in right half of screen */
    float chart_size = (screen_w < screen_h) ? screen_w * 0.45f : screen_h * 0.8f;
    float chart_cx = screen_w * 0.65f;
    float chart_cy = screen_h * 0.5f;

    *out_x = chart_cx + (p->x - 0.5f) * chart_size;
    *out_y = chart_cy + (p->y - 0.5f) * chart_size;
}

/* --- Public API --- */

void natal_chart_pass_init(void)
{
    if (s_initialized) return;

    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_resolution = glGetUniformLocation(s_program, "u_resolution");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 NC_MAX_VERTS * NC_VERT_FLOATS * sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);

    /* position: 2 floats */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          NC_VERT_FLOATS * sizeof(float), (void*)0);
    /* color: 4 floats */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          NC_VERT_FLOATS * sizeof(float),
                          (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    s_initialized = 1;
}

void natal_chart_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized) return;
    if (!frame) return;

    /* Only draw when Astrology focus is active */
    /* Check focus_mode or overlay flag in frame */
    /* For now: always draw if called — the caller gates this */

    float W = (float)frame->viewport_width;
    float H = (float)frame->viewport_height;
    if (W < 1.0f || H < 1.0f) return;

    /* Compute chart layout from current planet positions */
    double planet_lons[8];
    for (int i = 0; i < 8 && i < 8; i++) {
        planet_lons[i] = frame->planet_lon[i];
    }

    /* Use sun longitude as rough ascendant estimate
     * (real ascendant needs observer lat/lon + sidereal time) */
    double asc_lon = frame->sun_lon + 180.0;
    if (asc_lon >= 360.0) asc_lon -= 360.0;

    natal_chart_layout_t chart = natal_chart_compute(asc_lon, planet_lons, 8.0);

    /* Reset vertex buffer */
    s_vert_count = 0;

    /* --- Draw outer ring: 12 sign arcs colored by element --- */
    for (int s = 0; s < 12; s++) {
        float start = (float)(s * 30);
        float end = start + 30.0f;
        /* Rotate so Ascendant is at left (180 deg offset) */
        float offset = -(float)chart.ascendant_deg + 180.0f;
        int elem = SIGN_ELEMENT[s];
        push_arc(0.65f, 0.5f, chart.outer_radius,
                 start + offset, end + offset,
                 ELEMENT_COLORS[elem][0],
                 ELEMENT_COLORS[elem][1],
                 ELEMENT_COLORS[elem][2],
                 ELEMENT_COLORS[elem][3],
                 W, H);
    }

    /* --- Draw inner ring: single golden circle --- */
    push_arc(0.65f, 0.5f, chart.inner_radius,
             0.0f, 360.0f,
             1.0f, 0.85f, 0.55f, 0.3f, W, H);

    /* --- Draw house cusps: 12 radial lines --- */
    for (int c = 0; c < 12; c++) {
        float ix, iy, ox, oy;
        push_point_to_screen(&chart.cusp_inner[c], W, H, &ix, &iy);
        push_point_to_screen(&chart.cusp_outer[c], W, H, &ox, &oy);

        /* Ascendant cusp (c=0) is brighter */
        float alpha = (c == 0) ? 0.8f : 0.25f;
        float bright = (c == 0) ? 1.0f : 0.6f;
        push_line(ix, iy, ox, oy,
                  1.0f * bright, 0.85f * bright, 0.55f * bright, alpha);
    }

    /* --- Draw planets: small colored dots --- */
    for (int p = 0; p < chart.planet_count; p++) {
        float px, py;
        push_point_to_screen(&chart.planets[p].pos, W, H, &px, &py);

        /* Planet dot as a small cross (4 lines) */
        float sz = 4.0f;
        push_line(px - sz, py, px + sz, py, 1.0f, 0.9f, 0.6f, 0.9f);
        push_line(px, py - sz, px, py + sz, 1.0f, 0.9f, 0.6f, 0.9f);
    }

    /* --- Draw aspect lines between planets --- */
    for (int a = 0; a < chart.aspect_count; a++) {
        float sx, sy, ex, ey;
        push_point_to_screen(&chart.aspects[a].start, W, H, &sx, &sy);
        push_point_to_screen(&chart.aspects[a].end, W, H, &ex, &ey);

        int type = chart.aspects[a].type;
        if (type < 0 || type > 4) type = 0;

        push_line(sx, sy, ex, ey,
                  ASPECT_COLORS[type][0],
                  ASPECT_COLORS[type][1],
                  ASPECT_COLORS[type][2],
                  ASPECT_COLORS[type][3]);
    }

    /* --- Upload and render --- */
    if (s_vert_count < 2) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_vert_count * NC_VERT_FLOATS * sizeof(float),
                    s_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_LINES, 0, s_vert_count);

    glBindVertexArray(0);
    glUseProgram(0);
}

void natal_chart_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
/* Native stub — no GL on native builds */
void natal_chart_pass_init(void) {}
void natal_chart_pass_draw(const render_frame_t *frame) { (void)frame; }
void natal_chart_pass_destroy(void) {}
#endif
