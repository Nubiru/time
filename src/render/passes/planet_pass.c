/*
 * planet_pass.c — Sun + planet sprites + orbit trail render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Extracted from main.c.
 */

#ifdef __EMSCRIPTEN__

#include "planet_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../mesh.h"
#include "../mesh_shader.h"
#include "../planet_pack.h"

/* sqrt scale: compresses outer planets while keeping inner planets visible.
 * Mercury ~1.9, Earth ~3.0, Jupiter ~6.8, Neptune ~16.4 scene units. */
static const float ORBIT_SCALE = 3.0f;

/* --- Module-static GL handles --- */

/* Lit mesh shader (Sun sphere) */
static GLuint s_mesh_program;
static GLint  s_loc_model;
static GLint  s_loc_view;
static GLint  s_loc_proj;
static GLint  s_loc_color;
static GLint  s_loc_light_dir;
static GLint  s_loc_emissive;
static GLint  s_loc_opacity;
static mesh_t s_sun_mesh;

/* Planet pack (point sprites) */
static GLuint s_pp_program;
static GLint  s_pp_loc_mvp;
static GLint  s_pp_loc_scale;
static GLuint s_pp_vao;
static GLuint s_pp_vbo;

/* Trail (line strips) */
static GLuint s_trail_program;
static GLint  s_trail_loc_mvp;
static GLuint s_trail_vao;
static GLuint s_trail_vbo;

/* Planet pack uses linear AU scaling. Post-process to sqrt for visual compression. */
static void sqrt_scale_planet_vertices(float *verts, int count) {
    for (int i = 0; i < count; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float r_linear = sqrtf(x * x + y * y + z * z);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
            float scale = r_sqrt / r_linear;
            verts[base + 0] = x * scale;
            verts[base + 1] = y * scale;
            verts[base + 2] = z * scale;
        }
    }
}

/* Same sqrt scaling for trail vertices (7 floats per vertex: pos3 + color4) */
static void sqrt_scale_trail_vertices(float *verts, int count) {
    for (int i = 0; i < count; i++) {
        int base = i * PP_TRAIL_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float r_linear = sqrtf(x * x + y * y + z * z);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
            float scale = r_sqrt / r_linear;
            verts[base + 0] = x * scale;
            verts[base + 1] = y * scale;
            verts[base + 2] = z * scale;
        }
    }
}

int planet_pass_init(void) {
    /* --- Lit mesh shader (Sun) --- */
    s_mesh_program = shader_create_program(
        mesh_shader_vert_source(), mesh_shader_frag_source());
    if (s_mesh_program == 0) {
        printf("Failed to create mesh shader\n");
        return 1;
    }
    s_loc_model     = glGetUniformLocation(s_mesh_program, "u_model");
    s_loc_view      = glGetUniformLocation(s_mesh_program, "u_view");
    s_loc_proj      = glGetUniformLocation(s_mesh_program, "u_proj");
    s_loc_color     = glGetUniformLocation(s_mesh_program, "u_color");
    s_loc_light_dir = glGetUniformLocation(s_mesh_program, "u_light_dir");
    s_loc_emissive  = glGetUniformLocation(s_mesh_program, "u_emissive");
    s_loc_opacity   = glGetUniformLocation(s_mesh_program, "u_opacity");

    s_sun_mesh = mesh_create_sphere(16, 32);

    /* --- Planet pack shader (point sprites with atmosphere glow) --- */
    s_pp_program = shader_create_program(
        pp_planet_vert_source(), pp_planet_frag_source());
    if (s_pp_program == 0) {
        printf("Failed to create planet pack shader\n");
        return 1;
    }
    s_pp_loc_mvp   = glGetUniformLocation(s_pp_program, "u_mvp");
    s_pp_loc_scale = glGetUniformLocation(s_pp_program, "u_scale_factor");

    /* Pre-allocate planet VBO: 8 planets x 8 floats */
    glGenVertexArrays(1, &s_pp_vao);
    glBindVertexArray(s_pp_vao);

    glGenBuffers(1, &s_pp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_pp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(PP_PLANET_COUNT * PP_PLANET_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(3) + radius(1) + atmo(1) = 8 floats = 32 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 32, (void*)24);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 32, (void*)28);

    glBindVertexArray(0);

    /* --- Trail shader (alpha-fading line strips) --- */
    s_trail_program = shader_create_program(
        pp_trail_vert_source(), pp_trail_frag_source());
    if (s_trail_program == 0) {
        printf("Failed to create planet trail shader\n");
        return 1;
    }
    s_trail_loc_mvp = glGetUniformLocation(s_trail_program, "u_mvp");

    /* Pre-allocate trail VBO: 8 planets x 180 samples x 7 floats */
    glGenVertexArrays(1, &s_trail_vao);
    glBindVertexArray(s_trail_vao);

    glGenBuffers(1, &s_trail_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_trail_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(PP_PLANET_COUNT * 180 * PP_TRAIL_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, (void*)12);

    glBindVertexArray(0);

    printf("Planet pack: 8 planets + trails, shaders compiled\n");
    return 0;
}

void planet_pass_draw(const render_frame_t *frame) {
    /* --- Sun + Planets (only when LAYER_PLANETS visible) --- */
    if (layer_is_visible(frame->layers, LAYER_PLANETS)) {
        glUseProgram(s_mesh_program);
        glUniformMatrix4fv(s_loc_view, 1, GL_FALSE, frame->view.m);
        glUniformMatrix4fv(s_loc_proj, 1, GL_FALSE, frame->proj.m);
        glUniform1f(s_loc_opacity, 1.0f);

        /* Sun: emissive yellow sphere at origin */
        glUniform1f(s_loc_emissive, 1.0f);
        mat4_t sun_model = mat4_scale(0.4f, 0.4f, 0.4f);
        glUniformMatrix4fv(s_loc_model, 1, GL_FALSE, sun_model.m);
        glUniform3f(s_loc_color, 1.0f, 0.85f, 0.2f);
        glUniform3f(s_loc_light_dir, 0.0f, 0.0f, 1.0f);
        mesh_draw(&s_sun_mesh);

        /* Planets: packed point sprites with atmosphere glow */
        {
            pp_planet_data_t pdata = pp_pack_planets(frame->simulation_jd,
                                                      1.0f, 150.0f);
            sqrt_scale_planet_vertices(pdata.vertices, pdata.planet_count);

            glUseProgram(s_pp_program);
            glUniformMatrix4fv(s_pp_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
            glUniform1f(s_pp_loc_scale, 800.0f);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindVertexArray(s_pp_vao);
            glBindBuffer(GL_ARRAY_BUFFER, s_pp_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            (GLsizeiptr)pp_planet_vertex_bytes(&pdata),
                            pdata.vertices);
            glDrawArrays(GL_POINTS, 0, pdata.planet_count);

            glBindVertexArray(0);
            glDisable(GL_BLEND);
        }
    }

    /* --- Orbit trails (only when LAYER_ORBITS visible) --- */
    if (frame->show_trails &&
        layer_is_visible(frame->layers, LAYER_ORBITS)) {
        pp_trail_data_t tdata = pp_pack_trails(frame->simulation_jd,
                                                1.0f, 180);
        sqrt_scale_trail_vertices(tdata.vertices, tdata.total_vertex_count);

        glUseProgram(s_trail_program);
        glUniformMatrix4fv(s_trail_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(s_trail_vao);
        glBindBuffer(GL_ARRAY_BUFFER, s_trail_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        (GLsizeiptr)pp_trail_vertex_bytes(&tdata),
                        tdata.vertices);

        for (int i = 0; i < PP_PLANET_COUNT; i++) {
            glDrawArrays(GL_LINE_STRIP,
                         tdata.trail_offsets[i],
                         tdata.trail_counts[i]);
        }

        glBindVertexArray(0);
        glDisable(GL_BLEND);
    }
}

void planet_pass_destroy(void) {
    glDeleteProgram(s_mesh_program);
    mesh_destroy(&s_sun_mesh);
    glDeleteProgram(s_pp_program);
    glDeleteBuffers(1, &s_pp_vbo);
    glDeleteVertexArrays(1, &s_pp_vao);
    glDeleteProgram(s_trail_program);
    glDeleteBuffers(1, &s_trail_vbo);
    glDeleteVertexArrays(1, &s_trail_vao);
}

#endif /* __EMSCRIPTEN__ */
