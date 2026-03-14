/*
 * orbit_trail_pass.c — Orbit trail ellipse render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs Keplerian orbit ellipses for all 8 planets per frame, uploads to GPU,
 * draws as GL_LINE_STRIP with alpha-gradient fade per orbit.
 */

#ifdef __EMSCRIPTEN__

#include "orbit_trail_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../orbit_trail_pack.h"

/* --- Module-static GL handles --- */
static GLuint s_otp_program;
static GLint  s_otp_loc_mvp;
static GLuint s_otp_vao;
static GLuint s_otp_vbo;

int orbit_trail_pass_init(void) {
    s_otp_program = shader_create_program(
        otp_vert_source(), otp_frag_source());
    if (s_otp_program == 0) {
        printf("Failed to create orbit trail shader\n");
        return 1;
    }
    s_otp_loc_mvp = glGetUniformLocation(s_otp_program, "u_mvp");

    /* Pre-allocate VBO for all orbit trails */
    glGenVertexArrays(1, &s_otp_vao);
    glBindVertexArray(s_otp_vao);

    glGenBuffers(1, &s_otp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_otp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(OTP_MAX_VERTICES * OTP_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + alpha(1) + color(3) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0); /* a_position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1); /* a_alpha */
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 28, (void*)12);
    glEnableVertexAttribArray(2); /* a_color */
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 28, (void*)16);

    glBindVertexArray(0);
    printf("Orbit trails: 8 planet ellipses, shaders compiled\n");

    return 0;
}

void orbit_trail_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_ORBITS))
        return;

    /* Pack orbit ellipses for current time */
    otp_config_t config = otp_default_config();
    float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(frame->simulation_jd, config, verts);

    if (info.vertex_count == 0) return;

    glUseProgram(s_otp_program);
    glUniformMatrix4fv(s_otp_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindVertexArray(s_otp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_otp_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(info.vertex_count * OTP_VERTEX_FLOATS * (int)sizeof(float)),
                    verts);

    /* Draw each orbit as a separate line strip.
     * Each planet gets points_per_orbit vertices. */
    int offset = 0;
    int pts = config.points_per_orbit;
    for (int i = 0; i < info.orbit_count; i++) {
        int count = (offset + pts <= info.vertex_count) ? pts
                    : info.vertex_count - offset;
        if (count > 0) {
            glDrawArrays(GL_LINE_STRIP, offset, count);
        }
        offset += pts;
    }

    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void orbit_trail_pass_destroy(void) {
    glDeleteProgram(s_otp_program);
    glDeleteBuffers(1, &s_otp_vbo);
    glDeleteVertexArrays(1, &s_otp_vao);
}

#endif /* __EMSCRIPTEN__ */
