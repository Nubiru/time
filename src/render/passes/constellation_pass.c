/*
 * constellation_pass.c — IAU constellation boundary render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs 88 IAU constellation boundary polygons on init, uploads to GPU,
 * draws each frame as GL_LINES with alpha blending.
 */

#ifdef __EMSCRIPTEN__

#include "constellation_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../constellation_bounds.h"
#include "../zoom_fade.h"

/* Sphere radius must match star_pass (100.0 scene units) */
#define SPHERE_RADIUS 100.0f

/* --- Module-static GL handles --- */
static GLuint s_cb_program;
static GLint  s_cb_loc_mvp;
static GLuint s_cb_vao;
static GLuint s_cb_vbo;
static int    s_cb_vertex_count;

/* --- Static scratch buffer (BSS, zero stack cost) --- */
static float s_cb_init_verts[CB_MAX_TOTAL_VERTICES * CB_VERTEX_FLOATS];

int constellation_pass_init(void) {
    /* Compile boundary line shaders */
    s_cb_program = shader_create_program(
        cb_vert_source(), cb_frag_source());
    if (s_cb_program == 0) {
        printf("Failed to create constellation boundary shader\n");
        return 1;
    }
    s_cb_loc_mvp = glGetUniformLocation(s_cb_program, "u_mvp");

    /* Pack all 88 constellation boundaries */
    int total_segments = cb_total_segments();
    if (total_segments == 0) {
        printf("Constellation bounds: no segments\n");
        s_cb_vertex_count = 0;
        return 0;
    }

    s_cb_vertex_count = total_segments * 2;

    cb_pack(SPHERE_RADIUS, 0.15f, 0.35f, s_cb_init_verts);

    /* Upload to GPU */
    glGenVertexArrays(1, &s_cb_vao);
    glBindVertexArray(s_cb_vao);

    glGenBuffers(1, &s_cb_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_cb_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(s_cb_vertex_count * CB_VERTEX_STRIDE),
                 s_cb_init_verts, GL_STATIC_DRAW);

    /* Interleaved: position(vec3) + color(vec4) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          CB_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          CB_VERTEX_STRIDE, (void*)12);

    glBindVertexArray(0);
    printf("Constellation bounds: %d segments, %d vertices, shaders compiled\n",
           total_segments, s_cb_vertex_count);

    return 0;
}

void constellation_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_STARS))
        return;
    if (s_cb_vertex_count == 0)
        return;
    float zoom_alpha = zf_opacity(ZF_CONSTELLATION, frame->log_zoom);
    if (zoom_alpha < 0.01f) return;

    glUseProgram(s_cb_program);
    glUniformMatrix4fv(s_cb_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    /* Alpha blending for subtle boundary lines */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindVertexArray(s_cb_vao);
    glDrawArrays(GL_LINES, 0, s_cb_vertex_count);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void constellation_pass_destroy(void) {
    glDeleteProgram(s_cb_program);
    glDeleteBuffers(1, &s_cb_vbo);
    glDeleteVertexArrays(1, &s_cb_vao);
}

#endif /* __EMSCRIPTEN__ */
