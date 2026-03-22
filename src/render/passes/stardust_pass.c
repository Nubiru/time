/*
 * stardust_pass.c — Procedural background star field render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Compiles stardust_pack shader, creates fullscreen quad, draws each frame
 * with additive blending at zoom-appropriate opacity.
 */

#ifdef __EMSCRIPTEN__

#include "stardust_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../stardust_pack.h"
#include "../zoom_fade.h"

/* --- Module-static GL handles --- */
static GLuint s_sd_program;
static GLint  s_sd_loc_time;
static GLint  s_sd_loc_density;
static GLint  s_sd_loc_brightness;
static GLint  s_sd_loc_zoom_alpha;
static GLuint s_sd_vao;
static GLuint s_sd_vbo;

/* Fullscreen quad: 6 vertices, each: pos(2) + uv(2) = 4 floats */
static const float s_sd_quad[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
};

int stardust_pass_init(void) {
    s_sd_program = shader_create_program(
        sd_vert_source(), sd_frag_source());
    if (s_sd_program == 0) {
        printf("Failed to create stardust shader\n");
        return 1;
    }

    s_sd_loc_time       = glGetUniformLocation(s_sd_program, "u_time");
    s_sd_loc_density    = glGetUniformLocation(s_sd_program, "u_density");
    s_sd_loc_brightness = glGetUniformLocation(s_sd_program, "u_brightness");
    s_sd_loc_zoom_alpha = glGetUniformLocation(s_sd_program, "u_zoom_alpha");

    /* Fullscreen quad VAO */
    glGenVertexArrays(1, &s_sd_vao);
    glBindVertexArray(s_sd_vao);

    glGenBuffers(1, &s_sd_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_sd_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(s_sd_quad),
                 s_sd_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); /* a_position */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void *)0);
    glEnableVertexAttribArray(1); /* a_uv */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void *)8);

    glBindVertexArray(0);
    return 0;
}

void stardust_pass_draw(const render_frame_t *frame) {
    float alpha = zf_opacity(ZF_DEEP_SKY, frame->log_zoom);
    if (alpha < 0.01f) return;

    sd_config_t cfg = sd_default_config();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); /* additive */

    glUseProgram(s_sd_program);
    glUniform1f(s_sd_loc_time,       frame->time_sec);
    glUniform1f(s_sd_loc_density,    cfg.density);
    glUniform1f(s_sd_loc_brightness, cfg.brightness);
    glUniform1f(s_sd_loc_zoom_alpha, alpha);

    glBindVertexArray(s_sd_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void stardust_pass_destroy(void) {
    if (s_sd_program) glDeleteProgram(s_sd_program);
    if (s_sd_vao)     glDeleteVertexArrays(1, &s_sd_vao);
    if (s_sd_vbo)     glDeleteBuffers(1, &s_sd_vbo);
    s_sd_program = 0;
    s_sd_vao = 0;
    s_sd_vbo = 0;
}

#endif /* __EMSCRIPTEN__ */
