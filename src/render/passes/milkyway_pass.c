/*
 * milkyway_pass.c — Milky Way galaxy band render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs procedural Milky Way band mesh on init, uploads to GPU,
 * draws each frame as GL_TRIANGLES with additive blending and noise animation.
 */

#ifdef __EMSCRIPTEN__

#include "milkyway_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../milkyway_pack.h"

/* --- Module-static GL handles --- */
static GLuint s_mw_program;
static GLint  s_mw_loc_mvp;
static GLint  s_mw_loc_time;
static GLint  s_mw_loc_resolution;
static GLuint s_mw_vao;
static GLuint s_mw_vbo;
static GLuint s_mw_ebo;
static int    s_mw_index_count;

int milkyway_pass_init(void) {
    /* Compile shaders */
    s_mw_program = shader_create_program(
        mw_vert_source(), mw_frag_source());
    if (s_mw_program == 0) {
        printf("Failed to create milkyway shader\n");
        return 1;
    }
    s_mw_loc_mvp        = glGetUniformLocation(s_mw_program, "u_mvp");
    s_mw_loc_time       = glGetUniformLocation(s_mw_program, "u_time");
    s_mw_loc_resolution = glGetUniformLocation(s_mw_program, "u_resolution");

    /* Pack galaxy band mesh */
    mw_config_t config = mw_default_config();

    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(config, verts, indices);

    s_mw_index_count = info.index_count;
    if (s_mw_index_count == 0) {
        printf("Milkyway: no geometry packed\n");
        return 0;
    }

    /* Upload to GPU */
    glGenVertexArrays(1, &s_mw_vao);
    glBindVertexArray(s_mw_vao);

    glGenBuffers(1, &s_mw_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_mw_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(info.vertex_count * MW_VERTEX_FLOATS * (int)sizeof(float)),
                 verts, GL_STATIC_DRAW);

    glGenBuffers(1, &s_mw_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_mw_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(info.index_count * (int)sizeof(unsigned short)),
                 indices, GL_STATIC_DRAW);

    /* Interleaved: pos(3) + galcoord(2) + brightness(1) = 6 floats = 24 bytes */
    int stride = MW_VERTEX_FLOATS * (int)sizeof(float);

    glEnableVertexAttribArray(0); /* a_position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1); /* a_galcoord */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)12);

    glEnableVertexAttribArray(2); /* a_brightness */
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)20);

    glBindVertexArray(0);
    printf("Milkyway: %d vertices, %d triangles, shaders compiled\n",
           info.vertex_count, info.triangle_count);

    return 0;
}

void milkyway_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_GALAXY))
        return;
    if (s_mw_index_count == 0)
        return;

    glUseProgram(s_mw_program);
    glUniformMatrix4fv(s_mw_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
    glUniform1f(s_mw_loc_time, frame->time_sec);
    glUniform2f(s_mw_loc_resolution, 1920.0f, 1080.0f);

    /* Additive blending for soft galaxy glow */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(s_mw_vao);
    glDrawElements(GL_TRIANGLES, s_mw_index_count, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void milkyway_pass_destroy(void) {
    glDeleteProgram(s_mw_program);
    glDeleteBuffers(1, &s_mw_vbo);
    glDeleteBuffers(1, &s_mw_ebo);
    glDeleteVertexArrays(1, &s_mw_vao);
}

#endif /* __EMSCRIPTEN__ */
