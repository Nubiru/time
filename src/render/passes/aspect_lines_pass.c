/*
 * aspect_lines_pass.c — Birth chart aspect lines (S106)
 *
 * When birth sky is active (B key), draws colored lines between
 * planets that form astrological aspects at the birth moment.
 * Uses natal_chart_layout for planet screen positions and
 * aspect detection from aspects.h.
 *
 * Lines rendered as 3D world-space connections between planet
 * orbital positions, not screen-space 2D.
 */

#ifdef __EMSCRIPTEN__

#include "aspect_lines_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../systems/astrology/aspects.h"
#include "../../systems/astronomy/planets.h"
#include "../shader.h"

#define AL_MAX_LINES    28   /* C(8,2) = 28 possible planet pairs */
#define AL_MAX_VERTS   (AL_MAX_LINES * 2)
#define AL_VERT_FLOATS   7   /* x, y, z, r, g, b, a */

static const float ASPECT_COLORS[5][4] = {
    { 1.0f, 0.85f, 0.55f, 0.8f },   /* Conjunction — gold */
    { 0.9f, 0.25f, 0.25f, 0.6f },   /* Opposition — red */
    { 0.85f, 0.3f, 0.2f, 0.5f },    /* Square — warm red */
    { 0.3f, 0.6f, 0.9f, 0.6f },     /* Trine — blue */
    { 0.3f, 0.8f, 0.5f, 0.5f },     /* Sextile — green */
};

static GLuint s_program;
static GLint  s_loc_mvp;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;

static float s_verts[AL_MAX_VERTS * AL_VERT_FLOATS];
static int   s_vert_count;

static const char *VS_SRC =
    "#version 300 es\n"
    "precision highp float;\n"
    "uniform mat4 u_mvp;\n"
    "layout(location=0) in vec3 a_pos;\n"
    "layout(location=1) in vec4 a_color;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "  gl_Position = u_mvp * vec4(a_pos, 1.0);\n"
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

void aspect_lines_pass_init(void)
{
    if (s_initialized) return;
    s_program = shader_create_program(VS_SRC, FS_SRC);
    s_loc_mvp = glGetUniformLocation(s_program, "u_mvp");

    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, AL_MAX_VERTS * AL_VERT_FLOATS * sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);
    /* position: 3 floats */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, AL_VERT_FLOATS * sizeof(float), (void*)0);
    /* color: 4 floats */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, AL_VERT_FLOATS * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    s_initialized = 1;
}

void aspect_lines_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    /* Only draw in birth sky mode — check if birth_sky is active */
    /* For now: always draw if called. The caller gates this. */

    s_vert_count = 0;

    /* Get planet positions at current JD */
    double lons[8];
    for (int i = 0; i < 8; i++) {
        lons[i] = frame->planet_lon[i];
    }

    /* Orbit scale factor (sqrt distance scale from main.c) */
    float orbit_scale = 4.0f;

    /* Check all planet pairs for aspects */
    for (int a = 0; a < 8; a++) {
        for (int b = a + 1; b < 8; b++) {
            double diff = lons[a] - lons[b];
            if (diff < 0) diff = -diff;
            if (diff > 180.0) diff = 360.0 - diff;

            int aspect_type = -1;
            double orb = 8.0;

            if (diff < orb)              aspect_type = 0; /* Conjunction */
            else if (diff > 180.0 - orb) aspect_type = 1; /* Opposition */
            else if (diff > 90.0 - orb && diff < 90.0 + orb) aspect_type = 2; /* Square */
            else if (diff > 120.0 - orb && diff < 120.0 + orb) aspect_type = 3; /* Trine */
            else if (diff > 60.0 - orb && diff < 60.0 + orb) aspect_type = 4; /* Sextile */

            if (aspect_type < 0) continue;

            /* Convert ecliptic longitude to XZ position on orbital plane */
            float rad_a = (float)(lons[a] * 3.14159265 / 180.0);
            float rad_b = (float)(lons[b] * 3.14159265 / 180.0);

            /* Approximate orbital radius (sqrt scale) */
            float r_a = sqrtf((float)(a + 1)) * orbit_scale;
            float r_b = sqrtf((float)(b + 1)) * orbit_scale;

            float x1 = cosf(rad_a) * r_a, z1 = sinf(rad_a) * r_a;
            float x2 = cosf(rad_b) * r_b, z2 = sinf(rad_b) * r_b;

            if (s_vert_count + 2 > AL_MAX_VERTS) break;

            int i = s_vert_count * AL_VERT_FLOATS;
            const float *c = ASPECT_COLORS[aspect_type];
            s_verts[i+0] = x1; s_verts[i+1] = 0.0f; s_verts[i+2] = z1;
            s_verts[i+3] = c[0]; s_verts[i+4] = c[1]; s_verts[i+5] = c[2]; s_verts[i+6] = c[3];
            s_vert_count++;

            i = s_vert_count * AL_VERT_FLOATS;
            s_verts[i+0] = x2; s_verts[i+1] = 0.0f; s_verts[i+2] = z2;
            s_verts[i+3] = c[0]; s_verts[i+4] = c[1]; s_verts[i+5] = c[2]; s_verts[i+6] = c[3];
            s_vert_count++;
        }
    }

    if (s_vert_count < 2) return;

    glUseProgram(s_program);
    glUniformMatrix4fv(s_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_vert_count * AL_VERT_FLOATS * sizeof(float), s_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_LINES, 0, s_vert_count);

    glBindVertexArray(0);
    glUseProgram(0);
}

void aspect_lines_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
void aspect_lines_pass_init(void) {}
void aspect_lines_pass_draw(const render_frame_t *frame) { (void)frame; }
void aspect_lines_pass_destroy(void) {}
#endif
