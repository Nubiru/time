/*
 * heatmap_pass.c — Convergence heatmap (S88)
 *
 * Renders decade_heatmap data as a colored grid.
 * Each cell = one day of one year. Color = convergence score.
 * Cold (dark blue) → Warm (gold) → Hot (white).
 *
 * The Islamic calendar drifts ~11 days/year through Gregorian,
 * creating a visible diagonal band. The Hebrew Metonic cycle
 * creates a 19-year wobble pattern. Both are visible in the grid.
 */

#ifdef __EMSCRIPTEN__

#include "heatmap_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../../systems/unified/decade_heatmap.h"
#include "../shader.h"

#define HM_MAX_VERTS   (366 * 12 * 6)  /* 366 days × 12 years × 6 verts per quad */
#define HM_VERT_FLOATS  6

static GLuint s_program;
static GLint  s_loc_resolution;
static GLuint s_vao;
static GLuint s_vbo;
static int    s_initialized;
static int    s_cached_vert_count;

/* Static scratch — computed once, reused until year range changes */
static float s_verts[HM_MAX_VERTS * HM_VERT_FLOATS];

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

/* Map convergence score (0-8) to color */
static void score_to_color(int score, float *r, float *g, float *b, float *a)
{
    float t = (float)score / 8.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    /* Cold (space dark) → Warm (gold) → Hot (white) */
    if (t < 0.3f) {
        /* Dark → deep blue */
        float s = t / 0.3f;
        *r = 0.05f + s * 0.1f;
        *g = 0.05f + s * 0.1f;
        *b = 0.1f + s * 0.3f;
    } else if (t < 0.7f) {
        /* Deep blue → gold */
        float s = (t - 0.3f) / 0.4f;
        *r = 0.15f + s * 0.85f;
        *g = 0.15f + s * 0.7f;
        *b = 0.4f - s * 0.3f;
    } else {
        /* Gold → white hot */
        float s = (t - 0.7f) / 0.3f;
        *r = 1.0f;
        *g = 0.85f + s * 0.15f;
        *b = 0.1f + s * 0.9f;
    }
    *a = 0.3f + t * 0.6f;
}

void heatmap_pass_init(void)
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, HM_VERT_FLOATS * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, HM_VERT_FLOATS * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    s_initialized = 1;
    s_cached_vert_count = 0;
}

void heatmap_pass_draw(const render_frame_t *frame)
{
    if (!s_initialized || !frame) return;

    float W = (float)(frame->viewport_width > 0 ? frame->viewport_width : 1280);
    float H = (float)(frame->viewport_height > 0 ? frame->viewport_height : 720);

    /* Grid positioned in center-right */
    float grid_x = W * 0.15f;
    float grid_y = H * 0.1f;
    float grid_w = W * 0.7f;
    float grid_h = H * 0.8f;

    int years = 12;
    int days = 366;
    float cell_w = grid_w / (float)days;
    float cell_h = grid_h / (float)years;

    /* Rebuild if not cached */
    if (s_cached_vert_count == 0) {
        int vc = 0;
        int base_year = 2020;

        for (int y = 0; y < years; y++) {
            for (int d = 0; d < days; d++) {
                int score = dh_score(base_year + y, d + 1);
                if (score <= 0) continue;

                float r, g, b, a;
                score_to_color(score, &r, &g, &b, &a);

                float x0 = grid_x + (float)d * cell_w;
                float y0 = grid_y + (float)y * cell_h;
                float x1 = x0 + cell_w;
                float y1 = y0 + cell_h;

                int i = vc * HM_VERT_FLOATS;
                if (vc + 6 > HM_MAX_VERTS) break;

                /* Two triangles per cell */
                s_verts[i+0]=x0; s_verts[i+1]=y0; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a; vc++;
                i+=HM_VERT_FLOATS;
                s_verts[i+0]=x1; s_verts[i+1]=y0; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a; vc++;
                i+=HM_VERT_FLOATS;
                s_verts[i+0]=x0; s_verts[i+1]=y1; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a; vc++;
                i+=HM_VERT_FLOATS;
                s_verts[i+0]=x1; s_verts[i+1]=y0; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a; vc++;
                i+=HM_VERT_FLOATS;
                s_verts[i+0]=x1; s_verts[i+1]=y1; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a; vc++;
                i+=HM_VERT_FLOATS;
                s_verts[i+0]=x0; s_verts[i+1]=y1; s_verts[i+2]=r; s_verts[i+3]=g; s_verts[i+4]=b; s_verts[i+5]=a; vc++;
            }
        }

        s_cached_vert_count = vc;
        if (vc > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vc * HM_VERT_FLOATS * sizeof(float), s_verts);
        }
    }

    if (s_cached_vert_count < 3) return;

    glUseProgram(s_program);
    glUniform2f(s_loc_resolution, W, H);
    glBindVertexArray(s_vao);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, s_cached_vert_count);
    glBindVertexArray(0);
    glUseProgram(0);
}

void heatmap_pass_destroy(void)
{
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    s_initialized = 0;
}

#else
#include "heatmap_pass.h"
void heatmap_pass_init(void) {}
void heatmap_pass_draw(const render_frame_t *frame) { (void)frame; }
void heatmap_pass_destroy(void) {}
#endif
