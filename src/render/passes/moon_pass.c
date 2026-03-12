/*
 * moon_pass.c — Major moons render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs 9 major moon positions per frame, uploads to GPU, draws as point
 * sprites with orbit trails. Uses sqrt distance scaling to match planet_pass.
 */

#ifdef __EMSCRIPTEN__

#include "moon_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../moon_pack.h"

/* Must match planet_pass.c scaling */
#define ORBIT_SCALE 3.0f

/* --- Module-static GL handles --- */

/* Moon point sprites */
static GLuint s_moon_program;
static GLint  s_moon_loc_mvp;
static GLint  s_moon_loc_scale;
static GLuint s_moon_vao;
static GLuint s_moon_vbo;

/* Moon orbit trails */
static GLuint s_trail_program;
static GLint  s_trail_loc_mvp;
static GLuint s_trail_vao;
static GLuint s_trail_vbo;

/* Apply sqrt distance scaling to moon vertices (same formula as planet_pass) */
static void sqrt_scale_moon_vertices(float *verts, int count) {
    for (int i = 0; i < count; i++) {
        int base = i * MP_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float r_linear = sqrtf(x * x + y * y + z * z);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
            float s = r_sqrt / r_linear;
            verts[base + 0] *= s;
            verts[base + 1] *= s;
            verts[base + 2] *= s;
        }
    }
}

static void sqrt_scale_trail_vertices(float *verts, int count) {
    for (int i = 0; i < count; i++) {
        int base = i * MP_TRAIL_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float r_linear = sqrtf(x * x + y * y + z * z);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
            float s = r_sqrt / r_linear;
            verts[base + 0] *= s;
            verts[base + 1] *= s;
            verts[base + 2] *= s;
        }
    }
}

int moon_pass_init(void) {
    /* Moon billboard shader */
    s_moon_program = shader_create_program(
        mp_moon_vert_source(), mp_moon_frag_source());
    if (s_moon_program == 0) {
        printf("Failed to create moon shader\n");
        return 1;
    }
    s_moon_loc_mvp   = glGetUniformLocation(s_moon_program, "u_mvp");
    s_moon_loc_scale = glGetUniformLocation(s_moon_program, "u_scale");

    /* Pre-allocate moon VBO: 9 moons x 7 floats */
    glGenVertexArrays(1, &s_moon_vao);
    glBindVertexArray(s_moon_vao);

    glGenBuffers(1, &s_moon_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_moon_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(MP_MAX_MOONS * MP_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(3) + radius(1) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 28, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 28, (void*)24);

    glBindVertexArray(0);

    /* Moon trail shader */
    s_trail_program = shader_create_program(
        mp_trail_vert_source(), mp_trail_frag_source());
    if (s_trail_program == 0) {
        printf("Failed to create moon trail shader\n");
        return 1;
    }
    s_trail_loc_mvp = glGetUniformLocation(s_trail_program, "u_mvp");

    /* Pre-allocate trail VBO: 9 moons x 60 samples x 7 floats */
    glGenVertexArrays(1, &s_trail_vao);
    glBindVertexArray(s_trail_vao);

    glGenBuffers(1, &s_trail_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_trail_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(MP_TRAIL_MAX_TOTAL * MP_TRAIL_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, (void*)12);

    glBindVertexArray(0);

    printf("Moon pass: 9 moons + trails, shaders compiled\n");
    return 0;
}

void moon_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_PLANETS))
        return;

    /* Pack moon positions for current time */
    mp_moon_data_t mdata = mp_pack_moons(
        frame->simulation_jd, 1.0f, 5.0f, 150.0f);
    sqrt_scale_moon_vertices(mdata.vertices, mdata.moon_count);

    /* Draw moon sprites */
    glUseProgram(s_moon_program);
    glUniformMatrix4fv(s_moon_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
    glUniform1f(s_moon_loc_scale, 800.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(s_moon_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_moon_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)mp_moon_vertex_bytes(&mdata),
                    mdata.vertices);
    glDrawArrays(GL_POINTS, 0, mdata.moon_count);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

    /* Draw moon orbit trails (when trails enabled) */
    if (frame->show_trails &&
        layer_is_visible(frame->layers, LAYER_ORBITS)) {
        mp_trail_data_t tdata = mp_pack_trails(
            frame->simulation_jd, 1.0f, 5.0f, 40);
        sqrt_scale_trail_vertices(tdata.vertices, tdata.total_vertex_count);

        glUseProgram(s_trail_program);
        glUniformMatrix4fv(s_trail_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(s_trail_vao);
        glBindBuffer(GL_ARRAY_BUFFER, s_trail_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        (GLsizeiptr)mp_trail_vertex_bytes(&tdata),
                        tdata.vertices);

        for (int i = 0; i < MP_MAX_MOONS; i++) {
            if (tdata.trail_counts[i] > 0) {
                glDrawArrays(GL_LINE_STRIP,
                             tdata.trail_offsets[i],
                             tdata.trail_counts[i]);
            }
        }

        glBindVertexArray(0);
        glDisable(GL_BLEND);
    }
}

void moon_pass_destroy(void) {
    glDeleteProgram(s_moon_program);
    glDeleteBuffers(1, &s_moon_vbo);
    glDeleteVertexArrays(1, &s_moon_vao);
    glDeleteProgram(s_trail_program);
    glDeleteBuffers(1, &s_trail_vbo);
    glDeleteVertexArrays(1, &s_trail_vao);
}

#endif /* __EMSCRIPTEN__ */
