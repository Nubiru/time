/*
 * diamond_room_pass.c — Diamond Room wireframe render pass (E7)
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs icosahedron wireframe from diamond_room_pack, uploads GL_LINES,
 * draws with additive blending for glowing edges.
 */

#ifdef __EMSCRIPTEN__

#include "diamond_room_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../diamond_room_pack.h"
#include "../zoom_fade.h"
#include "../../systems/earth/diamond_room.h"

/* --- Module-static GL handles --- */
static GLuint s_dr_program;
static GLint  s_dr_loc_mvp;
static GLint  s_dr_loc_time;
static GLuint s_dr_vao;
static GLuint s_dr_vbo;

/* Line shader: per-vertex position + color */
static const char *s_dr_vert =
    "#version 300 es\n"
    "precision highp float;\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "uniform mat4 u_mvp;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "}\n";

static const char *s_dr_frag =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

int diamond_room_pass_init(void) {
    s_dr_program = shader_create_program(s_dr_vert, s_dr_frag);
    if (s_dr_program == 0) {
        printf("Failed to create diamond room shader\n");
        return 1;
    }
    s_dr_loc_mvp  = glGetUniformLocation(s_dr_program, "u_mvp");
    s_dr_loc_time = glGetUniformLocation(s_dr_program, "u_time");

    glGenVertexArrays(1, &s_dr_vao);
    glBindVertexArray(s_dr_vao);

    glGenBuffers(1, &s_dr_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_dr_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(DRP_LINE_VERTS * DRP_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(3) + color(4) = 7 floats = 28 bytes */
    int stride = DRP_VERTEX_FLOATS * (int)sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void *)12);

    glBindVertexArray(0);
    return 0;
}

void diamond_room_pass_draw(const render_frame_t *frame) {
    float alpha = zf_opacity(ZF_RING, frame->log_zoom);
    if (alpha < 0.01f) return;

    /* Compute diamond room state */
    dr_input_t input = dr_default_input();
    double day_frac = frame->simulation_jd - (int)frame->simulation_jd;
    if (day_frac < 0.0) day_frac += 1.0;
    input.hour_fraction = day_frac;
    dr_room_t room = dr_compute(&input);
    drp_data_t data = drp_pack(&room, 8.0f);
    if (data.edge_count == 0) return;

    /* Upload line vertices */
    glBindBuffer(GL_ARRAY_BUFFER, s_dr_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(data.vertex_count * DRP_VERTEX_FLOATS * (int)sizeof(float)),
                    data.vertices);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); /* additive glow */

    glUseProgram(s_dr_program);
    glUniformMatrix4fv(s_dr_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
    if (s_dr_loc_time >= 0)
        glUniform1f(s_dr_loc_time, frame->time_sec);

    glBindVertexArray(s_dr_vao);
    glDrawArrays(GL_LINES, 0, data.vertex_count);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void diamond_room_pass_destroy(void) {
    if (s_dr_program) glDeleteProgram(s_dr_program);
    if (s_dr_vao) glDeleteVertexArrays(1, &s_dr_vao);
    if (s_dr_vbo) glDeleteBuffers(1, &s_dr_vbo);
    s_dr_program = 0;
    s_dr_vao = 0;
    s_dr_vbo = 0;
}

#endif /* __EMSCRIPTEN__ */
