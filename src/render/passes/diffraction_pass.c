/*
 * diffraction_pass.c — Diffraction spike render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Packs diffraction spike data for bright stars on init, uploads to GPU,
 * draws each frame as GL_LINES with additive blending.
 */

#ifdef __EMSCRIPTEN__

#include "diffraction_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../diffraction.h"
#include "../star_catalog.h"

/* Sphere radius must match star_pass (100.0 scene units) */
#define SPHERE_RADIUS 100.0f
/* Earth's mean obliquity for RA/Dec -> ecliptic conversion */
#define OBLIQUITY_DEG 23.44f

/* --- Module-static GL handles --- */
static GLuint s_diff_program;
static GLint  s_diff_loc_mvp;
static GLuint s_diff_vao;
static GLuint s_diff_vbo;
static int    s_diff_vertex_count;

/* --- Static scratch buffers (BSS, zero stack cost) --- */
static float s_diff_positions[600 * 3];
static float s_diff_colors[600 * 3];
static float s_diff_magnitudes[600];
static float s_diff_spike_verts[600 * DIFFRACTION_VERTS_PER_STAR * DIFFRACTION_VERTEX_FLOATS];

int diffraction_pass_init(void) {
    /* Compile diffraction shaders */
    s_diff_program = shader_create_program(
        diffraction_vert_source(), diffraction_frag_source());
    if (s_diff_program == 0) {
        printf("Failed to create diffraction shader\n");
        return 1;
    }
    s_diff_loc_mvp = glGetUniformLocation(s_diff_program, "u_mvp");

    /* Build star position/color/magnitude arrays from catalog */
    int count = star_catalog_count();

    int n = count < 600 ? count : 600;
    for (int i = 0; i < n; i++) {
        star_entry_t s = star_catalog_entry(i);
        star_xyz_t p = star_to_ecliptic_xyz(s.ra_hours, s.dec_deg, OBLIQUITY_DEG);
        s_diff_positions[i * 3 + 0] = p.x * SPHERE_RADIUS;
        s_diff_positions[i * 3 + 1] = p.y * SPHERE_RADIUS;
        s_diff_positions[i * 3 + 2] = p.z * SPHERE_RADIUS;

        float r, g, b;
        star_bv_to_rgb(s.bv, &r, &g, &b);
        s_diff_colors[i * 3 + 0] = r;
        s_diff_colors[i * 3 + 1] = g;
        s_diff_colors[i * 3 + 2] = b;

        s_diff_magnitudes[i] = s.magnitude;
    }

    /* Pack diffraction spike vertices */
    diffraction_config_t config = diffraction_default_config();
    int spiked_stars = diffraction_pack(
        s_diff_positions, s_diff_colors, s_diff_magnitudes, n, &config,
        s_diff_spike_verts);

    diffraction_info_t info = diffraction_info(spiked_stars, config.spike_count);
    s_diff_vertex_count = info.vertex_count;

    if (s_diff_vertex_count == 0) {
        printf("Diffraction: no stars bright enough for spikes\n");
        return 0;
    }

    /* Upload to GPU */
    glGenVertexArrays(1, &s_diff_vao);
    glBindVertexArray(s_diff_vao);

    glGenBuffers(1, &s_diff_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_diff_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(s_diff_vertex_count * DIFFRACTION_VERTEX_STRIDE),
                 s_diff_spike_verts, GL_STATIC_DRAW);

    /* Interleaved: position(vec3) + color(vec4) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          DIFFRACTION_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          DIFFRACTION_VERTEX_STRIDE, (void*)12);

    glBindVertexArray(0);
    printf("Diffraction: %d stars, %d spike lines, shaders compiled\n",
           spiked_stars, info.line_count);

    return 0;
}

void diffraction_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_STARS))
        return;
    if (s_diff_vertex_count == 0)
        return;

    glUseProgram(s_diff_program);
    glUniformMatrix4fv(s_diff_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    /* Additive blending for glow effect */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glBindVertexArray(s_diff_vao);
    glDrawArrays(GL_LINES, 0, s_diff_vertex_count);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void diffraction_pass_destroy(void) {
    glDeleteProgram(s_diff_program);
    glDeleteBuffers(1, &s_diff_vbo);
    glDeleteVertexArrays(1, &s_diff_vao);
}

#endif /* __EMSCRIPTEN__ */
