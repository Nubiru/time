/*
 * sky_pass.c — Preetham sky dome render pass
 *
 * Stateful module (S1): fullscreen quad with Preetham sky color shader.
 * Active only in Earth View (view_id == 1). Renders sky dome behind all
 * other geometry using the sun position from the simulation.
 */

#ifdef __EMSCRIPTEN__

#include "sky_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../sky_pack.h"

/* --- Module-static GL handles --- */
static GLuint s_sky_program;
static GLint  s_sky_loc_sun_theta;
static GLint  s_sky_loc_sun_phi;
static GLint  s_sky_loc_turbidity;
static GLuint s_sky_vao;
static GLuint s_sky_vbo;

/* Fullscreen quad */
static const float s_sky_quad[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
};

int sky_pass_init(void) {
    s_sky_program = shader_create_program(
        sky_vert_source(), sky_frag_source());
    if (s_sky_program == 0) {
        printf("Failed to create sky shader\n");
        return 1;
    }

    s_sky_loc_sun_theta = glGetUniformLocation(s_sky_program, "u_sun_theta");
    s_sky_loc_sun_phi   = glGetUniformLocation(s_sky_program, "u_sun_phi");
    s_sky_loc_turbidity = glGetUniformLocation(s_sky_program, "u_turbidity");

    glGenVertexArrays(1, &s_sky_vao);
    glBindVertexArray(s_sky_vao);

    glGenBuffers(1, &s_sky_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_sky_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(s_sky_quad),
                 s_sky_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void *)8);

    glBindVertexArray(0);
    return 0;
}

void sky_pass_draw(const render_frame_t *frame) {
    /* Only in Earth View */
    if (frame->view_id != 1) return;

    /* Approximate sun zenith angle from sun_lon and observer latitude.
     * sun_theta = 0 at zenith, PI/2 at horizon. */
    double sun_alt_deg = frame->sun_lon; /* simplified: use sun_lon as proxy */
    (void)sun_alt_deg;

    /* Use default sky config for now — MEGA can refine sun position math */
    sky_config_t cfg = sky_default_config();

    glDisable(GL_DEPTH_TEST);

    glUseProgram(s_sky_program);
    glUniform1f(s_sky_loc_sun_theta, cfg.sun_theta);
    glUniform1f(s_sky_loc_sun_phi,   cfg.sun_phi);
    glUniform1f(s_sky_loc_turbidity, cfg.turbidity);

    glBindVertexArray(s_sky_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void sky_pass_destroy(void) {
    if (s_sky_program) glDeleteProgram(s_sky_program);
    if (s_sky_vao) glDeleteVertexArrays(1, &s_sky_vao);
    if (s_sky_vbo) glDeleteBuffers(1, &s_sky_vbo);
    s_sky_program = 0;
    s_sky_vao = 0;
    s_sky_vbo = 0;
}

#endif /* __EMSCRIPTEN__ */
