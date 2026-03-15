/*
 * hexagram_pass.c — I Ching hexagram render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Renders current hexagram (computed from JD) as line geometry with
 * yin/yang line patterns and optional nuclear hexagrams.
 */

#ifdef __EMSCRIPTEN__

#include "hexagram_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../hexagram_geometry.h"
#include "../../systems/iching/iching.h"

/* --- Module-static GL handles --- */
static GLuint s_hg_program;
static GLint  s_hg_loc_mvp;
static GLuint s_hg_vao;
static GLuint s_hg_vbo;

/* --- Dirty-flag caching --- */
#define HG_DIRTY_THRESHOLD 0.01  /* Re-pack when JD changes by ~14 minutes */
static double s_hg_last_jd = 0.0;
static int    s_hg_cached_count = 0;

int hexagram_pass_init(void) {
    s_hg_program = shader_create_program(
        hg_line_vert_source(), hg_line_frag_source());
    if (s_hg_program == 0) {
        printf("Failed to create hexagram shader\n");
        return 1;
    }
    s_hg_loc_mvp = glGetUniformLocation(s_hg_program, "u_mvp");

    /* Pre-allocate VBO */
    glGenVertexArrays(1, &s_hg_vao);
    glBindVertexArray(s_hg_vao);

    glGenBuffers(1, &s_hg_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_hg_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(HG_MAX_VERTICES * HG_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(2) + color(4) = 6 floats = 24 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (void*)8);

    glBindVertexArray(0);
    printf("Hexagram: I Ching line geometry, shaders compiled\n");

    return 0;
}

void hexagram_pass_draw(const render_frame_t *frame) {
    /* Hexagram view — only draw when LAYER_CARDS visible */
    if (!layer_is_visible(frame->layers, LAYER_CARDS))
        return;

    /* Only recompute when time has changed beyond threshold */
    if (fabs(frame->simulation_jd - s_hg_last_jd) > HG_DIRTY_THRESHOLD) {
        /* Compute current hexagram from Julian Day */
        hexagram_t hex = iching_from_jd(frame->simulation_jd);
        if (hex.king_wen < 1 || hex.king_wen > 64) {
            s_hg_cached_count = 0;
            s_hg_last_jd = frame->simulation_jd;
            return;
        }

        /* Pack hexagram with nuclear hexagrams */
        hg_data_t data = hg_pack_with_nuclear(
            hex.king_wen, 0.8f, 1.2f, NULL, 1.2f, 0.5f);

        s_hg_cached_count = data.vertex_count;

        if (s_hg_cached_count > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, s_hg_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            (GLsizeiptr)hg_vertex_bytes(&data),
                            data.vertices);
        }
        s_hg_last_jd = frame->simulation_jd;
    }

    if (s_hg_cached_count == 0) return;

    /* Build ortho projection for screen-space NDC */
    mat4_t ortho = mat4_identity();

    glUseProgram(s_hg_program);
    glUniformMatrix4fv(s_hg_loc_mvp, 1, GL_FALSE, ortho.m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(s_hg_vao);
    glDrawArrays(GL_LINES, 0, s_hg_cached_count);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void hexagram_pass_destroy(void) {
    glDeleteProgram(s_hg_program);
    glDeleteBuffers(1, &s_hg_vbo);
    glDeleteVertexArrays(1, &s_hg_vao);
}

#endif /* __EMSCRIPTEN__ */
