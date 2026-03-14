/*
 * saturn_pass.c — Saturn ring system render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs Saturn ring mesh on init, uploads to GPU, computes Saturn's orbital
 * position each frame to place the ring mesh with correct orientation and lighting.
 */

#ifdef __EMSCRIPTEN__

#include "saturn_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../saturn_ring_pack.h"
#include "../../systems/astronomy/orbit.h"
#include "../../systems/astronomy/planets.h"
#include "../../math/mat4.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif
#define DEG_TO_RAD (PI / 180.0)

/* Must match planet_pass.c scaling */
#define ORBIT_SCALE 3.0f

/* Saturn body radius in scene units (visual, not physical) */
#define SATURN_SCENE_RADIUS 0.3f

/* --- Module-static GL handles --- */
static GLuint s_srp_program;
static GLint  s_srp_loc_mvp;
static GLint  s_srp_loc_model;
static GLint  s_srp_loc_sun_dir;
static GLint  s_srp_loc_saturn_pos;
static GLint  s_srp_loc_saturn_radius;
static GLuint s_srp_vao;
static GLuint s_srp_vbo;
static GLuint s_srp_ebo;
static int    s_srp_index_count;

int saturn_pass_init(void) {
    /* Compile ring shaders */
    s_srp_program = shader_create_program(
        srp_vert_source(), srp_frag_source());
    if (s_srp_program == 0) {
        printf("Failed to create Saturn ring shader\n");
        return 1;
    }
    s_srp_loc_mvp          = glGetUniformLocation(s_srp_program, "u_mvp");
    s_srp_loc_model        = glGetUniformLocation(s_srp_program, "u_model");
    s_srp_loc_sun_dir      = glGetUniformLocation(s_srp_program, "u_sun_dir");
    s_srp_loc_saturn_pos   = glGetUniformLocation(s_srp_program, "u_saturn_pos");
    s_srp_loc_saturn_radius = glGetUniformLocation(s_srp_program, "u_saturn_radius");

    /* Pack ring mesh */
    srp_config_t config = srp_default_config();
    config.saturn_scene_radius = SATURN_SCENE_RADIUS;

    float verts[SRP_MAX_VERTICES * SRP_VERTEX_FLOATS];
    unsigned short indices[SRP_MAX_VERTICES * 6];
    srp_info_t info = srp_pack(config, verts, indices);

    s_srp_index_count = info.index_count;
    if (s_srp_index_count == 0) {
        printf("Saturn rings: no geometry packed\n");
        return 0;
    }

    /* Upload to GPU */
    glGenVertexArrays(1, &s_srp_vao);
    glBindVertexArray(s_srp_vao);

    glGenBuffers(1, &s_srp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_srp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(info.vertex_count * SRP_VERTEX_FLOATS * (int)sizeof(float)),
                 verts, GL_STATIC_DRAW);

    glGenBuffers(1, &s_srp_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_srp_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(info.index_count * (int)sizeof(unsigned short)),
                 indices, GL_STATIC_DRAW);

    /* Interleaved: pos(3) + normal(3) + uv(2) + opacity(1) = 9 floats = 36 bytes */
    int stride = SRP_VERTEX_FLOATS * (int)sizeof(float);

    glEnableVertexAttribArray(0); /* a_position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1); /* a_normal */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)12);

    glEnableVertexAttribArray(2); /* a_texcoord */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)24);

    glEnableVertexAttribArray(3); /* a_opacity */
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)32);

    glBindVertexArray(0);
    printf("Saturn rings: %d vertices, %d triangles, %d ring bands, shaders compiled\n",
           info.vertex_count, info.triangle_count, info.ring_count);

    return 0;
}

void saturn_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_PLANETS))
        return;
    if (s_srp_index_count == 0)
        return;

    /* Compute Saturn's position from orbital elements */
    orbital_elements_t elem = orbit_elements_at(&ORBIT_SATURN, frame->simulation_jd);
    heliocentric_pos_t hpos = orbit_heliocentric(&elem);

    double lon_rad = hpos.longitude * DEG_TO_RAD;
    double lat_rad = hpos.latitude * DEG_TO_RAD;
    double cos_lat = cos(lat_rad);

    /* Ecliptic spherical -> Cartesian (same as planet_pack.c) */
    float sx = (float)(hpos.distance * cos_lat * cos(lon_rad));
    float sz = (float)(hpos.distance * cos_lat * sin(lon_rad));
    float sy = (float)(hpos.distance * sin(lat_rad));

    /* Apply sqrt scaling to match planet_pass */
    float r_linear = sqrtf(sx * sx + sy * sy + sz * sz);
    if (r_linear > 0.001f) {
        float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
        float scale = r_sqrt / r_linear;
        sx *= scale;
        sy *= scale;
        sz *= scale;
    }

    /* Model matrix: translate to Saturn + tilt ring plane */
    float tilt_rad = (float)(26.73 * DEG_TO_RAD);
    mat4_t translate = mat4_translate(sx, sy, sz);
    mat4_t tilt = mat4_rotate_x(tilt_rad);
    mat4_t model = mat4_multiply(translate, tilt);

    /* Sun direction: from Saturn toward Sun (Sun at origin) */
    float sun_dx = -sx, sun_dy = -sy, sun_dz = -sz;
    float sun_len = sqrtf(sun_dx * sun_dx + sun_dy * sun_dy + sun_dz * sun_dz);
    if (sun_len > 0.001f) {
        sun_dx /= sun_len;
        sun_dy /= sun_len;
        sun_dz /= sun_len;
    }

    glUseProgram(s_srp_program);
    glUniformMatrix4fv(s_srp_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
    glUniformMatrix4fv(s_srp_loc_model, 1, GL_FALSE, model.m);
    glUniform3f(s_srp_loc_sun_dir, sun_dx, sun_dy, sun_dz);
    glUniform3f(s_srp_loc_saturn_pos, sx, sy, sz);
    glUniform1f(s_srp_loc_saturn_radius, SATURN_SCENE_RADIUS);

    /* Alpha blending for ring transparency */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(s_srp_vao);
    glDrawElements(GL_TRIANGLES, s_srp_index_count, GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void saturn_pass_destroy(void) {
    glDeleteProgram(s_srp_program);
    glDeleteBuffers(1, &s_srp_vbo);
    glDeleteBuffers(1, &s_srp_ebo);
    glDeleteVertexArrays(1, &s_srp_vao);
}

#endif /* __EMSCRIPTEN__ */
