/*
 * star_pass.c — Star field + constellation line render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Extracted from main.c.
 */

#ifdef __EMSCRIPTEN__

#include "star_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../star_field.h"
#include "../zoom_fade.h"

/* --- Module-static GL handles --- */
static GLuint s_star_program;
static GLint  s_star_loc_mvp;
static GLint  s_star_loc_scale;
static GLint  s_star_loc_time;
static GLuint s_star_vao;
static GLuint s_star_vbo;
static int    s_star_count;

static GLuint s_cline_program;
static GLint  s_cline_loc_mvp;
static GLuint s_cline_vao;
static GLuint s_cline_vbo;
static int    s_cline_vertex_count;

/* --- Static scratch buffers (BSS, zero stack cost) --- */
static float s_star_init_verts[600 * STAR_VERTEX_FLOATS];
static float s_cline_init_verts[500 * 2 * CLINE_VERTEX_FLOATS];

int star_pass_init(void) {
    /* --- Star field shader + data --- */
    s_star_program = shader_create_program(
        star_field_vert_source(), star_field_frag_source());
    if (s_star_program == 0) {
        printf("Failed to create star shader\n");
        return 1;
    }
    s_star_loc_mvp = glGetUniformLocation(s_star_program, "u_mvp");
    s_star_loc_scale = glGetUniformLocation(s_star_program, "u_scale_factor");
    s_star_loc_time = glGetUniformLocation(s_star_program, "u_time");

    /* Pack star vertex data */
    s_star_count = star_field_pack(s_star_init_verts, 600, 8.0f, 100.0f);

    /* Upload to GPU */
    glGenVertexArrays(1, &s_star_vao);
    glBindVertexArray(s_star_vao);

    glGenBuffers(1, &s_star_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_star_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(s_star_count * STAR_VERTEX_STRIDE),
                 s_star_init_verts, GL_STATIC_DRAW);

    /* Interleaved: pos(3) + color(3) + size(1) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STAR_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STAR_VERTEX_STRIDE, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, STAR_VERTEX_STRIDE, (void*)24);

    glBindVertexArray(0);
    printf("Stars: %d packed, shaders compiled\n", s_star_count);

    /* --- Constellation line shader + data --- */
    s_cline_program = shader_create_program(
        constellation_line_vert_source(), constellation_line_frag_source());
    if (s_cline_program == 0) {
        printf("Failed to create constellation shader\n");
        return 1;
    }
    s_cline_loc_mvp = glGetUniformLocation(s_cline_program, "u_mvp");

    int line_count = constellation_lines_pack(s_cline_init_verts, 500, 0.25f, 0.4f, 100.0f);
    s_cline_vertex_count = line_count * 2;

    glGenVertexArrays(1, &s_cline_vao);
    glBindVertexArray(s_cline_vao);

    glGenBuffers(1, &s_cline_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_cline_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(s_cline_vertex_count * CLINE_VERTEX_STRIDE),
                 s_cline_init_verts, GL_STATIC_DRAW);

    /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, CLINE_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, CLINE_VERTEX_STRIDE, (void*)12);

    glBindVertexArray(0);
    printf("Constellations: %d lines packed, shaders compiled\n", line_count);

    return 0;
}

void star_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_STARS))
        return;

    float star_opacity = frame->layers.opacity[LAYER_STARS];
    float zoom_alpha = zf_opacity(ZF_STARS, frame->log_zoom);
    if (zoom_alpha < 0.01f) return;
    star_opacity *= zoom_alpha;
    star_opacity *= frame->scene_brightness; /* dim during enter_zoom */

    /* Stars: point sprites with additive blending */
    glUseProgram(s_star_program);
    glUniformMatrix4fv(s_star_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
    glUniform1f(s_star_loc_scale, star_opacity);
    glUniform1f(s_star_loc_time, frame->time_sec);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(s_star_vao);
    glDrawArrays(GL_POINTS, 0, s_star_count);
    glBindVertexArray(0);

    /* Constellation lines */
    glUseProgram(s_cline_program);
    glUniformMatrix4fv(s_cline_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(s_cline_vao);
    glDrawArrays(GL_LINES, 0, s_cline_vertex_count);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void star_pass_destroy(void) {
    glDeleteProgram(s_star_program);
    glDeleteBuffers(1, &s_star_vbo);
    glDeleteVertexArrays(1, &s_star_vao);
    glDeleteProgram(s_cline_program);
    glDeleteBuffers(1, &s_cline_vbo);
    glDeleteVertexArrays(1, &s_cline_vao);
}

#endif /* __EMSCRIPTEN__ */
