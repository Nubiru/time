/*
 * earth_pass.c — Earth globe render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Renders Earth globe with day/night shading, coastline overlays, terminator
 * line, and atmosphere limb glow. Globe mesh is static; terminator updates
 * per frame based on solar position.
 *
 * TODO: Gate on Earth View mode flag once render_frame_t supports it.
 * Currently draws when LAYER_PLANETS is visible.
 */

#ifdef __EMSCRIPTEN__

#include "earth_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../earth_pack.h"
#include "../camera.h"
#include "../../systems/astronomy/orbit.h"
#include "../../systems/astronomy/planets.h"
#include "../../math/mat4.h"
#include "../../math/ecliptic.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif
#define DEG_TO_RAD (PI / 180.0)

/* Must match planet_pass.c scaling */
#define ORBIT_SCALE 3.0f

/* --- MAX constants for static buffers (2x headroom over default config) --- */
/* Default config: lat=32, lon=64 -> globe_verts=2145, globe_indices=12288 */
#define ERP_MAX_GLOBE_VERTS   4096
#define ERP_MAX_GLOBE_INDICES 16384
#define ERP_MAX_COAST_VERTS   512    /* ~107 segments * 2 = ~214, 2x headroom */
#define ERP_MAX_ATMO_VERTS    4096   /* atmo shell same as globe */
#define ERP_MAX_ATMO_INDICES  16384

/* --- Module-static GL handles --- */

/* Globe (Earth surface with day/night) */
static GLuint s_globe_program;
static GLint  s_globe_loc_mvp;
static GLint  s_globe_loc_model;
static GLint  s_globe_loc_sun_dir;
static GLuint s_globe_vao;
static GLuint s_globe_vbo;
static GLuint s_globe_ebo;
static int    s_globe_index_count;

/* Coastline + terminator lines */
static GLuint s_line_program;
static GLint  s_line_loc_mvp;
static GLuint s_coast_vao;
static GLuint s_coast_vbo;
static int    s_coast_vertex_count;
static GLuint s_term_vao;
static GLuint s_term_vbo;

/* Atmosphere shell */
static GLuint s_atmo_program;
static GLint  s_atmo_loc_mvp;
static GLint  s_atmo_loc_model;
static GLint  s_atmo_loc_camera_pos;
static GLuint s_atmo_vao;
static GLuint s_atmo_vbo;
static GLuint s_atmo_ebo;
static int    s_atmo_index_count;

/* Config */
static erp_config_t s_erp_config;

/* --- Static scratch buffers (BSS, zero stack cost) --- */
static float        s_erp_globe_verts[ERP_MAX_GLOBE_VERTS * ERP_GLOBE_VERTEX_FLOATS];
static unsigned int s_erp_globe_indices[ERP_MAX_GLOBE_INDICES];
static float        s_erp_coast_verts[ERP_MAX_COAST_VERTS * ERP_LINE_VERTEX_FLOATS];
static float        s_erp_atmo_verts[ERP_MAX_ATMO_VERTS * 7]; /* pos3+normal3+opacity1 */
static unsigned int s_erp_atmo_indices[ERP_MAX_ATMO_INDICES];
static float        s_erp_term_verts[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];

int earth_pass_init(void) {
    s_erp_config = erp_default_config();
    erp_info_t info = erp_info(&s_erp_config);

    /* --- Globe shader --- */
    s_globe_program = shader_create_program(
        erp_globe_vert_source(), erp_globe_frag_source());
    if (s_globe_program == 0) {
        printf("Failed to create earth globe shader\n");
        return 1;
    }
    s_globe_loc_mvp     = glGetUniformLocation(s_globe_program, "u_mvp");
    s_globe_loc_model   = glGetUniformLocation(s_globe_program, "u_model");
    s_globe_loc_sun_dir = glGetUniformLocation(s_globe_program, "u_sun_dir");

    /* Pack globe mesh (static) */
    erp_pack_globe(&s_erp_config, s_erp_globe_verts, s_erp_globe_indices);

    s_globe_index_count = info.globe_indices;

    glGenVertexArrays(1, &s_globe_vao);
    glBindVertexArray(s_globe_vao);

    glGenBuffers(1, &s_globe_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_globe_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(info.globe_verts * ERP_GLOBE_VERTEX_STRIDE),
                 s_erp_globe_verts, GL_STATIC_DRAW);

    glGenBuffers(1, &s_globe_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_globe_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(info.globe_indices * (int)sizeof(unsigned int)),
                 s_erp_globe_indices, GL_STATIC_DRAW);

    /* pos(3) + normal(3) + uv(2) = 8 floats = 32 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ERP_GLOBE_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, ERP_GLOBE_VERTEX_STRIDE, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, ERP_GLOBE_VERTEX_STRIDE, (void*)24);

    glBindVertexArray(0);

    /* --- Line shader (coastlines + terminator) --- */
    s_line_program = shader_create_program(
        erp_line_vert_source(), erp_line_frag_source());
    if (s_line_program == 0) {
        printf("Failed to create earth line shader\n");
        return 1;
    }
    s_line_loc_mvp = glGetUniformLocation(s_line_program, "u_mvp");

    /* Pack coastlines (static) */
    s_coast_vertex_count = erp_pack_coastlines(&s_erp_config, s_erp_coast_verts);

    glGenVertexArrays(1, &s_coast_vao);
    glBindVertexArray(s_coast_vao);

    glGenBuffers(1, &s_coast_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_coast_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(s_coast_vertex_count * ERP_LINE_VERTEX_STRIDE),
                 s_erp_coast_verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ERP_LINE_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, ERP_LINE_VERTEX_STRIDE, (void*)12);

    glBindVertexArray(0);

    /* Pre-allocate terminator VBO (dynamic, updated per frame) */
    glGenVertexArrays(1, &s_term_vao);
    glBindVertexArray(s_term_vao);

    glGenBuffers(1, &s_term_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_term_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_STRIDE),
                 NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ERP_LINE_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, ERP_LINE_VERTEX_STRIDE, (void*)12);

    glBindVertexArray(0);

    /* --- Atmosphere shader --- */
    s_atmo_program = shader_create_program(
        erp_atmo_vert_source(), erp_atmo_frag_source());
    if (s_atmo_program == 0) {
        printf("Failed to create earth atmo shader\n");
        return 1;
    }
    s_atmo_loc_mvp        = glGetUniformLocation(s_atmo_program, "u_mvp");
    s_atmo_loc_model      = glGetUniformLocation(s_atmo_program, "u_model");
    s_atmo_loc_camera_pos = glGetUniformLocation(s_atmo_program, "u_camera_pos");

    /* Pack atmosphere shell (static) */
    erp_pack_atmosphere(&s_erp_config, s_erp_atmo_verts, s_erp_atmo_indices);

    s_atmo_index_count = info.atmo_indices;

    glGenVertexArrays(1, &s_atmo_vao);
    glBindVertexArray(s_atmo_vao);

    glGenBuffers(1, &s_atmo_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_atmo_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(info.atmo_verts * 7 * (int)sizeof(float)),
                 s_erp_atmo_verts, GL_STATIC_DRAW);

    glGenBuffers(1, &s_atmo_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_atmo_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(info.atmo_indices * (int)sizeof(unsigned int)),
                 s_erp_atmo_indices, GL_STATIC_DRAW);

    /* pos(3)+normal(3)+opacity(1) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 28, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 28, (void*)24);

    glBindVertexArray(0);

    printf("Earth: globe(%d verts, %d tris), %d coastline verts, atmo(%d verts), shaders compiled\n",
           info.globe_verts, info.globe_indices / 3,
           s_coast_vertex_count, info.atmo_verts);
    return 0;
}

void earth_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_PLANETS))
        return;

    /* Compute Earth's position from orbital elements */
    solar_system_t sys = planets_at(frame->simulation_jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double earth_lat = sys.positions[PLANET_EARTH].latitude;
    double earth_dist = sys.positions[PLANET_EARTH].distance;

    double lon_rad = earth_lon * DEG_TO_RAD;
    double lat_rad = earth_lat * DEG_TO_RAD;
    double cos_lat = cos(lat_rad);

    float ex = (float)(earth_dist * cos_lat * cos(lon_rad));
    float ez = (float)(earth_dist * cos_lat * sin(lon_rad));
    float ey = (float)(earth_dist * sin(lat_rad));

    /* Apply sqrt scaling to match planet_pass */
    float r_linear = sqrtf(ex * ex + ey * ey + ez * ez);
    if (r_linear > 0.001f) {
        float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
        float s = r_sqrt / r_linear;
        ex *= s; ey *= s; ez *= s;
    }

    /* Model matrix: translate to Earth position */
    float tilt_rad = (float)(23.44 * DEG_TO_RAD);
    mat4_t translate = mat4_translate(ex, ey, ez);
    mat4_t tilt = mat4_rotate_x(tilt_rad);
    mat4_t model = mat4_multiply(translate, tilt);
    mat4_t mvp = mat4_multiply(frame->view_proj, model);

    /* Sun direction: from Earth toward Sun (Sun at origin) */
    float sun_dx = -ex, sun_dy = -ey, sun_dz = -ez;
    float sun_len = sqrtf(sun_dx * sun_dx + sun_dy * sun_dy + sun_dz * sun_dz);
    if (sun_len > 0.001f) {
        sun_dx /= sun_len; sun_dy /= sun_len; sun_dz /= sun_len;
    }

    /* --- Draw globe --- */
    glUseProgram(s_globe_program);
    glUniformMatrix4fv(s_globe_loc_mvp, 1, GL_FALSE, mvp.m);
    glUniformMatrix4fv(s_globe_loc_model, 1, GL_FALSE, model.m);
    glUniform3f(s_globe_loc_sun_dir, sun_dx, sun_dy, sun_dz);

    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(s_globe_vao);
    glDrawElements(GL_TRIANGLES, s_globe_index_count, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    /* --- Draw coastlines --- */
    if (s_coast_vertex_count > 0) {
        glUseProgram(s_line_program);
        glUniformMatrix4fv(s_line_loc_mvp, 1, GL_FALSE, mvp.m);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(s_coast_vao);
        glDrawArrays(GL_LINES, 0, s_coast_vertex_count);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
    }

    /* --- Draw terminator (dynamic) --- */
    {
        /* Compute solar declination and RA for terminator position */
        double sun_geo_lon = fmod(earth_lon + 180.0, 360.0);
        double obliquity = mean_obliquity(frame->simulation_jd);
        double solar_dec = asin(sin(sun_geo_lon * DEG_TO_RAD) *
                                sin(obliquity * DEG_TO_RAD)) / DEG_TO_RAD;
        double solar_ra = sun_geo_lon / 15.0; /* approximate RA in hours */

        int term_count = erp_pack_terminator(
            solar_dec, solar_ra, &s_erp_config, s_erp_term_verts);

        if (term_count > 0) {
            glUseProgram(s_line_program);
            glUniformMatrix4fv(s_line_loc_mvp, 1, GL_FALSE, mvp.m);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindVertexArray(s_term_vao);
            glBindBuffer(GL_ARRAY_BUFFER, s_term_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            (GLsizeiptr)(term_count * ERP_LINE_VERTEX_STRIDE),
                            s_erp_term_verts);
            glDrawArrays(GL_LINE_STRIP, 0, term_count);
            glBindVertexArray(0);

            glDisable(GL_BLEND);
        }
    }

    /* --- Draw atmosphere shell --- */
    if (s_atmo_index_count > 0) {
        glUseProgram(s_atmo_program);
        glUniformMatrix4fv(s_atmo_loc_mvp, 1, GL_FALSE, mvp.m);
        glUniformMatrix4fv(s_atmo_loc_model, 1, GL_FALSE, model.m);

        /* Camera position in world space (approximate from view matrix inverse) */
        glUniform3f(s_atmo_loc_camera_pos, 0.0f, 0.0f,
                    frame->view.m[14] != 0.0f ? -frame->view.m[14] : 10.0f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(s_atmo_vao);
        glDrawElements(GL_TRIANGLES, s_atmo_index_count,
                        GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
}

void earth_pass_destroy(void) {
    glDeleteProgram(s_globe_program);
    glDeleteBuffers(1, &s_globe_vbo);
    glDeleteBuffers(1, &s_globe_ebo);
    glDeleteVertexArrays(1, &s_globe_vao);

    glDeleteProgram(s_line_program);
    glDeleteBuffers(1, &s_coast_vbo);
    glDeleteVertexArrays(1, &s_coast_vao);
    glDeleteBuffers(1, &s_term_vbo);
    glDeleteVertexArrays(1, &s_term_vao);

    glDeleteProgram(s_atmo_program);
    glDeleteBuffers(1, &s_atmo_vbo);
    glDeleteBuffers(1, &s_atmo_ebo);
    glDeleteVertexArrays(1, &s_atmo_vao);
}

#endif /* __EMSCRIPTEN__ */
