/*
 * deep_sky_pass.c — Deep sky object billboard render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs 110 Messier DSOs as camera-facing billboard quads each frame,
 * uploads to GPU, draws as GL_TRIANGLES with alpha blending.
 */

#ifdef __EMSCRIPTEN__

#include "deep_sky_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../deep_sky_pack.h"
#include "../deep_sky.h"
#include "../billboard.h"

/* Max 120 DSOs * 6 verts = 720 vertices */
#define DSP_MAX_OBJECTS 120
#define DSP_MAX_VERTS (DSP_MAX_OBJECTS * DSP_VERTS_PER_OBJECT)

/* --- Module-static GL handles --- */
static GLuint s_dsp_program;
static GLint  s_dsp_loc_mvp;
static GLuint s_dsp_vao;
static GLuint s_dsp_vbo;

/* --- Static scratch buffer (BSS, zero stack cost) --- */
static float s_dsp_scratch[DSP_MAX_VERTS * DSP_VERTEX_FLOATS];

int deep_sky_pass_init(void) {
    s_dsp_program = shader_create_program(
        dsp_vert_source(), dsp_frag_source());
    if (s_dsp_program == 0) {
        printf("Failed to create deep sky shader\n");
        return 1;
    }
    s_dsp_loc_mvp = glGetUniformLocation(s_dsp_program, "u_mvp");

    /* Pre-allocate VBO for all DSOs */
    glGenVertexArrays(1, &s_dsp_vao);
    glBindVertexArray(s_dsp_vao);

    glGenBuffers(1, &s_dsp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_dsp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(DSP_MAX_VERTS * DSP_VERTEX_STRIDE),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(3) + uv(2) + color(4) = 9 floats = 36 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DSP_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, DSP_VERTEX_STRIDE, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, DSP_VERTEX_STRIDE, (void*)20);

    glBindVertexArray(0);
    printf("Deep sky: %d Messier DSOs, shaders compiled\n", dso_catalog_count());

    return 0;
}

void deep_sky_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_STARS))
        return;

    /* Extract camera right/up vectors for billboarding */
    vec3_t cam_right, cam_up;
    billboard_camera_vectors(&frame->view, &cam_right, &cam_up);

    float right[3] = { cam_right.x, cam_right.y, cam_right.z };
    float up[3] = { cam_up.x, cam_up.y, cam_up.z };

    /* Pack DSO billboards for current camera orientation */
    dsp_config_t config = dsp_default_config();
    int count = dsp_pack(right, up, &config, s_dsp_scratch);

    if (count == 0) return;

    int vert_count = count * DSP_VERTS_PER_OBJECT;

    glUseProgram(s_dsp_program);
    glUniformMatrix4fv(s_dsp_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(s_dsp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_dsp_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(vert_count * DSP_VERTEX_STRIDE),
                    s_dsp_scratch);
    glDrawArrays(GL_TRIANGLES, 0, vert_count);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void deep_sky_pass_destroy(void) {
    glDeleteProgram(s_dsp_program);
    glDeleteBuffers(1, &s_dsp_vbo);
    glDeleteVertexArrays(1, &s_dsp_vao);
}

#endif /* __EMSCRIPTEN__ */
