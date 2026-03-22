/*
 * zodiac_pass.c — Zodiac ring + markers + aspects + cusps + glyphs render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Extracted from main.c.
 */

#ifdef __EMSCRIPTEN__

#include "zodiac_pass.h"
#include <stdio.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../mesh.h"
#include "../mesh_shader.h"
#include "../ring_geometry.h"
#include "../color_palette.h"
#include "../aspect_lines.h"
#include "../cusp_lines.h"
#include "../billboard.h"
#include "../../ui/zodiac_glyphs.h"
#include "../../systems/astronomy/planets.h"
#include "../../systems/astronomy/planet_data.h"
#include "../../systems/astrology/observer.h"
#include "../../systems/astrology/houses.h"
#include "../../math/sidereal.h"
#include "../../math/ecliptic.h"
#include "../zoom_fade.h"

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

#define GLYPH_ATLAS_W 96
#define GLYPH_ATLAS_H 12
#define SIGN_LABEL_SIZE 0.4f
#define SIGN_LABEL_HEIGHT 0.3f

/* Ring placement: between Mars (sqrt(1.52)*3 ~ 3.7) and Jupiter (sqrt(5.2)*3 ~ 6.8) */
#define RING_INNER_RADIUS 4.2f
#define RING_OUTER_RADIUS 4.8f
#define RING_MID_RADIUS   ((RING_INNER_RADIUS + RING_OUTER_RADIUS) / 2.0f)
#define RING_SEGMENTS_PER_SIGN 8

/* --- MAX constants for static ring buffers --- */
/* With 8 segments/sign: vertex_count=194, index_count=576. 2x headroom. */
#define ZP_MAX_RING_VERTS   512
#define ZP_MAX_RING_INDICES 1536

/* --- Module-static GL handles --- */

/* Lit mesh shader (zodiac ring markers) */
static GLuint s_mesh_program;
static GLint  s_loc_model;
static GLint  s_loc_view;
static GLint  s_loc_proj;
static GLint  s_loc_color;
static GLint  s_loc_light_dir;
static GLint  s_loc_emissive;
static GLint  s_loc_opacity;
static mesh_t s_marker_mesh;

/* Ring shader (vertex-colored ring) */
static GLuint s_ring_program;
static GLint  s_ring_loc_view;
static GLint  s_ring_loc_proj;
static GLint  s_ring_loc_opacity;
static GLuint s_ring_vao;
static GLuint s_ring_vbo_pos;
static GLuint s_ring_vbo_color;
static GLuint s_ring_ebo;
static int    s_ring_index_count;

/* Line shader (aspect + cusp lines, per-vertex RGBA) */
static GLuint s_line_program;
static GLint  s_line_loc_view;
static GLint  s_line_loc_proj;
static GLint  s_line_loc_opacity;
static GLuint s_line_vao;
static GLuint s_line_vbo;

/* Glyph shader (textured billboards for sign labels) */
static GLuint s_glyph_program;
static GLint  s_glyph_loc_view;
static GLint  s_glyph_loc_proj;
static GLint  s_glyph_loc_opacity;
static GLuint s_glyph_texture;
static GLuint s_glyph_vao;
static GLuint s_glyph_vbo;
static GLuint s_glyph_ebo;

/* --- Static scratch buffers (BSS, zero stack cost) --- */
static float        s_zp_ring_positions[ZP_MAX_RING_VERTS * 3];
static float        s_zp_ring_colors[ZP_MAX_RING_VERTS * 3];
static unsigned int s_zp_ring_indices[ZP_MAX_RING_INDICES];
static float        s_zp_aspect_verts[MAX_ASPECT_LINES * 14];
static float        s_zp_cusp_verts[MAX_CUSP_LINES * 14];
static float        s_zp_glyph_verts[12 * 4 * 8];
static float        s_zp_ring_pos[8 * 3];

/* --- Ring shader sources --- */

static const char *s_ring_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_color;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "out vec3 v_color;\n"
    "out vec3 v_world_pos;\n"
    "void main() {\n"
    "    gl_Position = u_proj * u_view * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "    v_world_pos = a_position;\n"
    "}\n";

static const char *s_ring_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 v_color;\n"
    "in vec3 v_world_pos;\n"
    "uniform float u_opacity;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    /* Radial glow from world position */\n"
    "    float r = length(v_world_pos.xz);\n"
    "    float inner = 4.2;\n"
    "    float outer = 4.8;\n"
    "    float radial = clamp((r - inner) / (outer - inner), 0.0, 1.0);\n"
    "    float dist_from_center = abs(radial - 0.5) / 0.5;\n"
    "    float glow = 1.0 - dist_from_center * dist_from_center;\n"
    "    glow = clamp(glow, 0.0, 1.0);\n"
    "    vec3 color = v_color * (0.5 + 0.5 * glow);\n"
    "    float alpha = u_opacity * (0.3 + 0.7 * glow);\n"
    "    frag_color = vec4(color, alpha);\n"
    "}\n";

/* --- Line shader sources --- */

static const char *s_line_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "uniform float u_opacity;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_proj * u_view * vec4(a_position, 1.0);\n"
    "    v_color = vec4(a_color.rgb, a_color.a * u_opacity);\n"
    "}\n";

static const char *s_line_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

/* --- Glyph shader sources --- */

static const char *s_glyph_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec3 a_color;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "out vec2 v_uv;\n"
    "out vec3 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_proj * u_view * vec4(a_position, 1.0);\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "}\n";

static const char *s_glyph_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec2 v_uv;\n"
    "in vec3 v_color;\n"
    "uniform sampler2D u_texture;\n"
    "uniform float u_opacity;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    float a = texture(u_texture, v_uv).r;\n"
    "    if (a < 0.5) discard;\n"
    "    frag_color = vec4(v_color, u_opacity);\n"
    "}\n";

/* Planet colors for zodiac ring markers (from planet_pack data) */
static void planet_marker_color(int planet_idx, float *r, float *g, float *b) {
    planet_color_t c = planet_data_color(planet_idx);
    *r = c.r; *g = c.g; *b = c.b;
}

/* --- Init helpers --- */

static int init_ring_and_mesh(void) {
    /* Lit mesh shader (ring markers) */
    s_mesh_program = shader_create_program(
        mesh_shader_vert_source(), mesh_shader_frag_source());
    if (s_mesh_program == 0) {
        printf("Failed to create zodiac mesh shader\n");
        return 1;
    }
    s_loc_model     = glGetUniformLocation(s_mesh_program, "u_model");
    s_loc_view      = glGetUniformLocation(s_mesh_program, "u_view");
    s_loc_proj      = glGetUniformLocation(s_mesh_program, "u_proj");
    s_loc_color     = glGetUniformLocation(s_mesh_program, "u_color");
    s_loc_light_dir = glGetUniformLocation(s_mesh_program, "u_light_dir");
    s_loc_emissive  = glGetUniformLocation(s_mesh_program, "u_emissive");
    s_loc_opacity   = glGetUniformLocation(s_mesh_program, "u_opacity");

    s_marker_mesh = mesh_create_sphere(12, 24);

    /* Ring shader */
    s_ring_program = shader_create_program(s_ring_vert_source, s_ring_frag_source);
    if (s_ring_program == 0) {
        printf("Failed to create ring shader\n");
        return 1;
    }
    s_ring_loc_view    = glGetUniformLocation(s_ring_program, "u_view");
    s_ring_loc_proj    = glGetUniformLocation(s_ring_program, "u_proj");
    s_ring_loc_opacity = glGetUniformLocation(s_ring_program, "u_opacity");

    /* Generate ring mesh data */
    ring_mesh_info_t info = ring_mesh_size(RING_SEGMENTS_PER_SIGN);

    ring_generate_positions(s_zp_ring_positions, RING_INNER_RADIUS, RING_OUTER_RADIUS,
                            RING_SEGMENTS_PER_SIGN);
    ring_generate_indices(s_zp_ring_indices, RING_SEGMENTS_PER_SIGN);

    for (int v = 0; v < info.vertex_count; v++) {
        int sign = ring_vertex_sign(v, RING_SEGMENTS_PER_SIGN);
        color_rgb_t c = color_zodiac_sign(sign);
        s_zp_ring_colors[v * 3 + 0] = c.r;
        s_zp_ring_colors[v * 3 + 1] = c.g;
        s_zp_ring_colors[v * 3 + 2] = c.b;
    }

    s_ring_index_count = info.index_count;

    glGenVertexArrays(1, &s_ring_vao);
    glBindVertexArray(s_ring_vao);

    glGenBuffers(1, &s_ring_vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, s_ring_vbo_pos);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(info.vertex_count * 3 * (int)sizeof(float)),
                 s_zp_ring_positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &s_ring_vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, s_ring_vbo_color);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(info.vertex_count * 3 * (int)sizeof(float)),
                 s_zp_ring_colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &s_ring_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ring_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(info.index_count * (int)sizeof(unsigned int)),
                 s_zp_ring_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    return 0;
}

static int init_lines(void) {
    s_line_program = shader_create_program(s_line_vert_source, s_line_frag_source);
    if (s_line_program == 0) {
        printf("Failed to create line shader\n");
        return 1;
    }
    s_line_loc_view    = glGetUniformLocation(s_line_program, "u_view");
    s_line_loc_proj    = glGetUniformLocation(s_line_program, "u_proj");
    s_line_loc_opacity = glGetUniformLocation(s_line_program, "u_opacity");

    glGenVertexArrays(1, &s_line_vao);
    glBindVertexArray(s_line_vao);

    glGenBuffers(1, &s_line_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_line_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(MAX_ASPECT_LINES * 14 * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes per vertex */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, (void*)12);

    glBindVertexArray(0);
    return 0;
}

static int init_glyphs(void) {
    s_glyph_program = shader_create_program(s_glyph_vert_source, s_glyph_frag_source);
    if (s_glyph_program == 0) {
        printf("Failed to create glyph shader\n");
        return 1;
    }
    s_glyph_loc_view    = glGetUniformLocation(s_glyph_program, "u_view");
    s_glyph_loc_proj    = glGetUniformLocation(s_glyph_program, "u_proj");
    s_glyph_loc_opacity = glGetUniformLocation(s_glyph_program, "u_opacity");

    /* Pack 12 zodiac bitmaps into a 96x12 R8 texture atlas */
    unsigned char atlas[GLYPH_ATLAS_W * GLYPH_ATLAS_H];
    for (int i = 0; i < GLYPH_ATLAS_W * GLYPH_ATLAS_H; i++)
        atlas[i] = 0;
    for (int s = 0; s < 12; s++) {
        const unsigned char *glyph = zodiac_glyph(s);
        if (!glyph) continue;
        for (int y = 0; y < GLYPH_HEIGHT; y++) {
            for (int x = 0; x < GLYPH_WIDTH; x++) {
                atlas[y * GLYPH_ATLAS_W + s * GLYPH_WIDTH + x] =
                    glyph[y * GLYPH_WIDTH + x] * 255;
            }
        }
    }
    glGenTextures(1, &s_glyph_texture);
    glBindTexture(GL_TEXTURE_2D, s_glyph_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, GLYPH_ATLAS_W, GLYPH_ATLAS_H,
                 0, GL_RED, GL_UNSIGNED_BYTE, atlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Pre-allocate glyph billboard VBO: 12 signs x 4 verts x 8 floats */
    glGenVertexArrays(1, &s_glyph_vao);
    glBindVertexArray(s_glyph_vao);

    glGenBuffers(1, &s_glyph_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_glyph_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(12 * 4 * 8 * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + uv(2) + color(3) = 8 floats = 32 bytes per vertex */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)20);

    /* Static index buffer: 12 signs x 6 indices (2 triangles each) */
    unsigned int glyph_indices[72];
    for (int s = 0; s < 12; s++) {
        int base_v = s * 4;
        int base_i = s * 6;
        glyph_indices[base_i + 0] = (unsigned int)base_v + 0;
        glyph_indices[base_i + 1] = (unsigned int)base_v + 1;
        glyph_indices[base_i + 2] = (unsigned int)base_v + 2;
        glyph_indices[base_i + 3] = (unsigned int)base_v + 0;
        glyph_indices[base_i + 4] = (unsigned int)base_v + 2;
        glyph_indices[base_i + 5] = (unsigned int)base_v + 3;
    }
    glGenBuffers(1, &s_glyph_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_glyph_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(72 * (int)sizeof(unsigned int)),
                 glyph_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    return 0;
}

int zodiac_pass_init(void) {
    if (init_ring_and_mesh() != 0) return 1;
    if (init_lines() != 0) return 1;
    if (init_glyphs() != 0) return 1;
    return 0;
}

/* --- Draw helpers --- */

static void draw_zodiac_ring(const render_frame_t *frame, float opacity) {
    glUseProgram(s_ring_program);
    glUniformMatrix4fv(s_ring_loc_view, 1, GL_FALSE, frame->view.m);
    glUniformMatrix4fv(s_ring_loc_proj, 1, GL_FALSE, frame->proj.m);
    glUniform1f(s_ring_loc_opacity, opacity);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(s_ring_vao);
    glDrawElements(GL_TRIANGLES, s_ring_index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

static void draw_planet_markers(const render_frame_t *frame, float opacity,
                                const float *ring_pos) {
    glUseProgram(s_mesh_program);
    glUniformMatrix4fv(s_loc_view, 1, GL_FALSE, frame->view.m);
    glUniformMatrix4fv(s_loc_proj, 1, GL_FALSE, frame->proj.m);
    glUniform1f(s_loc_emissive, 0.7f);
    glUniform1f(s_loc_opacity, opacity);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int p = 0; p < 8; p++) {
        float mx = ring_pos[p * 3 + 0];
        float my = ring_pos[p * 3 + 1];
        float mz = ring_pos[p * 3 + 2];

        mat4_t translate = mat4_translate(mx, my, mz);
        mat4_t scale = mat4_scale(0.12f, 0.12f, 0.12f);
        mat4_t model = mat4_multiply(translate, scale);

        glUniformMatrix4fv(s_loc_model, 1, GL_FALSE, model.m);

        if (p == PLANET_EARTH) {
            glUniform3f(s_loc_color, 1.0f, 0.85f, 0.55f); /* solar gold */
        } else {
            float pr, pg, pb;
            planet_marker_color(p, &pr, &pg, &pb);
            glUniform3f(s_loc_color, pr, pg, pb);
        }
        glUniform3f(s_loc_light_dir, 0.0f, 1.0f, 0.0f);

        mesh_draw(&s_marker_mesh);
    }

    glDisable(GL_BLEND);
}

static void draw_aspect_lines(const render_frame_t *frame, float opacity,
                              const float *ring_pos, const double *geo_lons) {
    aspect_line_set_t alines = aspect_lines_generate(
        ring_pos, geo_lons, 8, 8.0);

    if (alines.count == 0) return;

    for (int i = 0; i < alines.count; i++) {
        aspect_line_t *al = &alines.lines[i];
        int base = i * 14;
        s_zp_aspect_verts[base + 0]  = al->x1;
        s_zp_aspect_verts[base + 1]  = al->y1;
        s_zp_aspect_verts[base + 2]  = al->z1;
        s_zp_aspect_verts[base + 3]  = al->r;
        s_zp_aspect_verts[base + 4]  = al->g;
        s_zp_aspect_verts[base + 5]  = al->b;
        s_zp_aspect_verts[base + 6]  = al->opacity;
        s_zp_aspect_verts[base + 7]  = al->x2;
        s_zp_aspect_verts[base + 8]  = al->y2;
        s_zp_aspect_verts[base + 9]  = al->z2;
        s_zp_aspect_verts[base + 10] = al->r;
        s_zp_aspect_verts[base + 11] = al->g;
        s_zp_aspect_verts[base + 12] = al->b;
        s_zp_aspect_verts[base + 13] = al->opacity;
    }

    glUseProgram(s_line_program);
    glUniformMatrix4fv(s_line_loc_view, 1, GL_FALSE, frame->view.m);
    glUniformMatrix4fv(s_line_loc_proj, 1, GL_FALSE, frame->proj.m);
    glUniform1f(s_line_loc_opacity, opacity);

    glBindVertexArray(s_line_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_line_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(alines.count * 14 * (int)sizeof(float)),
                    s_zp_aspect_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_LINES, 0, alines.count * 2);
    glDisable(GL_BLEND);

    glBindVertexArray(0);
}

static void draw_cusp_lines(const render_frame_t *frame, float opacity) {
    double jd_0h = floor(frame->simulation_jd - 0.5) + 0.5;
    double gmst = gmst_degrees(jd_0h);
    double lst = lst_degrees(gmst, frame->observer_lon);
    double obliquity = mean_obliquity(frame->simulation_jd);
    double asc = ascendant_longitude(lst, obliquity, frame->observer_lat);
    house_system_t houses = houses_whole_sign(asc);

    cusp_line_set_t cusps = cusp_lines_generate(
        houses.cusps, RING_INNER_RADIUS, RING_OUTER_RADIUS);

    if (cusps.count == 0) return;

    for (int i = 0; i < cusps.count; i++) {
        cusp_line_t *cl = &cusps.lines[i];
        int base = i * 14;
        float alpha = cl->is_angular ? 0.8f : 0.35f;
        float gray = cl->is_angular ? 1.0f : 0.6f;
        s_zp_cusp_verts[base + 0]  = cl->x1;
        s_zp_cusp_verts[base + 1]  = cl->y1;
        s_zp_cusp_verts[base + 2]  = cl->z1;
        s_zp_cusp_verts[base + 3]  = gray;
        s_zp_cusp_verts[base + 4]  = gray;
        s_zp_cusp_verts[base + 5]  = gray;
        s_zp_cusp_verts[base + 6]  = alpha;
        s_zp_cusp_verts[base + 7]  = cl->x2;
        s_zp_cusp_verts[base + 8]  = cl->y2;
        s_zp_cusp_verts[base + 9]  = cl->z2;
        s_zp_cusp_verts[base + 10] = gray;
        s_zp_cusp_verts[base + 11] = gray;
        s_zp_cusp_verts[base + 12] = gray;
        s_zp_cusp_verts[base + 13] = alpha;
    }

    glUseProgram(s_line_program);
    glUniformMatrix4fv(s_line_loc_view, 1, GL_FALSE, frame->view.m);
    glUniformMatrix4fv(s_line_loc_proj, 1, GL_FALSE, frame->proj.m);
    glUniform1f(s_line_loc_opacity, opacity);

    glBindVertexArray(s_line_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_line_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(cusps.count * 14 * (int)sizeof(float)),
                    s_zp_cusp_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_LINES, 0, cusps.count * 2);
    glDisable(GL_BLEND);

    glBindVertexArray(0);
}

static void draw_sign_labels(const render_frame_t *frame, float opacity) {
    vec3_t cam_right, cam_up;
    billboard_camera_vectors(&frame->view, &cam_right, &cam_up);

    float gw = SIGN_LABEL_SIZE;
    float gh = gw * 1.5f;

    for (int s = 0; s < 12; s++) {
        float angle_deg = (float)(s * 30 + 15);
        float angle_rad = angle_deg * (float)DEG_TO_RAD;
        vec3_t center = {
            RING_MID_RADIUS * cosf(angle_rad),
            SIGN_LABEL_HEIGHT,
            RING_MID_RADIUS * sinf(angle_rad)
        };

        billboard_quad_t quad = billboard_create(
            center, cam_right, cam_up, gw, gh);

        float u0 = (float)s / 12.0f;
        float u1 = (float)(s + 1) / 12.0f;

        color_rgb_t sc = color_zodiac_sign(s);

        int base = s * 32;
        for (int v = 0; v < 4; v++) {
            int vi = base + v * 8;
            s_zp_glyph_verts[vi + 0] = quad.positions[v * 3 + 0];
            s_zp_glyph_verts[vi + 1] = quad.positions[v * 3 + 1];
            s_zp_glyph_verts[vi + 2] = quad.positions[v * 3 + 2];
            float bu = quad.uvs[v * 2 + 0];
            float bv = quad.uvs[v * 2 + 1];
            s_zp_glyph_verts[vi + 3] = u0 + bu * (u1 - u0);
            s_zp_glyph_verts[vi + 4] = bv;
            s_zp_glyph_verts[vi + 5] = sc.r;
            s_zp_glyph_verts[vi + 6] = sc.g;
            s_zp_glyph_verts[vi + 7] = sc.b;
        }
    }

    glUseProgram(s_glyph_program);
    glUniformMatrix4fv(s_glyph_loc_view, 1, GL_FALSE, frame->view.m);
    glUniformMatrix4fv(s_glyph_loc_proj, 1, GL_FALSE, frame->proj.m);
    glUniform1f(s_glyph_loc_opacity, opacity);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_glyph_texture);

    glBindVertexArray(s_glyph_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_glyph_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(12 * 32 * (int)sizeof(float)),
                    s_zp_glyph_verts);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glBindVertexArray(0);
}

void zodiac_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_ZODIAC_RING))
        return;

    float ring_opacity = frame->layers.opacity[LAYER_ZODIAC_RING];
    float zoom_alpha = zf_opacity(ZF_ZODIAC, frame->log_zoom);
    if (zoom_alpha < 0.01f) return;
    ring_opacity *= zoom_alpha;

    draw_zodiac_ring(frame, ring_opacity);

    /* Compute geocentric planet longitudes + ring positions */
    solar_system_t sys = planets_at(frame->simulation_jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double sun_geo_lon = fmod(earth_lon + 180.0, 360.0);
    double geo_lons[8];
    for (int p = 0; p < 8; p++) {
        double gl;
        if (p == PLANET_EARTH)
            gl = sun_geo_lon;
        else {
            gl = fmod(sys.positions[p].longitude - earth_lon + 180.0, 360.0);
            if (gl < 0.0) gl += 360.0;
        }
        geo_lons[p] = gl;
        float lon_rad = (float)(gl * DEG_TO_RAD);
        s_zp_ring_pos[p * 3 + 0] = RING_MID_RADIUS * cosf(lon_rad);
        s_zp_ring_pos[p * 3 + 1] = 0.0f;
        s_zp_ring_pos[p * 3 + 2] = RING_MID_RADIUS * sinf(lon_rad);
    }

    draw_planet_markers(frame, ring_opacity, s_zp_ring_pos);
    draw_aspect_lines(frame, ring_opacity, s_zp_ring_pos, geo_lons);
    draw_cusp_lines(frame, ring_opacity);
    draw_sign_labels(frame, ring_opacity);
}

void zodiac_pass_destroy(void) {
    glDeleteProgram(s_mesh_program);
    mesh_destroy(&s_marker_mesh);
    glDeleteProgram(s_ring_program);
    glDeleteBuffers(1, &s_ring_vbo_pos);
    glDeleteBuffers(1, &s_ring_vbo_color);
    glDeleteBuffers(1, &s_ring_ebo);
    glDeleteVertexArrays(1, &s_ring_vao);
    glDeleteProgram(s_line_program);
    glDeleteBuffers(1, &s_line_vbo);
    glDeleteVertexArrays(1, &s_line_vao);
    glDeleteProgram(s_glyph_program);
    glDeleteTextures(1, &s_glyph_texture);
    glDeleteBuffers(1, &s_glyph_vbo);
    glDeleteBuffers(1, &s_glyph_ebo);
    glDeleteVertexArrays(1, &s_glyph_vao);
}

#endif /* __EMSCRIPTEN__ */
