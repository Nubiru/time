/*
 * convergence_pass.c — Convergence connection line render pass
 *
 * Stateful module (S1): renders lines connecting concentric knowledge
 * system rings when brain convergence data flows through render_frame_t.
 *
 * Uses brain_visual_bridge link data for system pairs and visual weight.
 * Uses convergence_visual for geometry (XZ positions on rings).
 * Reuses concentric_ring shader (position(3) + color(4) + MVP).
 *
 * Lines are rendered as quads (2 triangles per line) for thickness
 * control, since WebGL2 glLineWidth is typically clamped to 1.0.
 */

#ifdef __EMSCRIPTEN__

#include "convergence_pass.h"
#include <GLES3/gl3.h>
#include <math.h>
#include <string.h>

#include "../concentric_ring.h"
#include "../convergence_visual.h"
#include "../system_rings.h"
#include "../ring_today.h"
#include "../shader.h"
#include "../../systems/unified/depth_ring.h"
#include "../../systems/astronomy/planets.h"
#include "../../systems/astronomy/lunar.h"

#define CONV_MAX_LINES       32
#define CONV_VERTS_PER_LINE   4
#define CONV_INDICES_PER_LINE 6
#define CONV_MAX_VERTS   (CONV_MAX_LINES * CONV_VERTS_PER_LINE)
#define CONV_MAX_INDICES (CONV_MAX_LINES * CONV_INDICES_PER_LINE)
#define CONV_VERT_FLOATS      7   /* position(3) + color(4) */

/* Ring placement constants — must match ring_pass.c */
#define RING_INNERMOST  5.5f
#define RING_GAP        0.2f

/* Golden angle for deterministic system placement on rings (degrees) */
#define GOLDEN_ANGLE 137.508f

/* Module-static GL handles */
static GLuint s_program;
static GLint  s_loc_mvp;
static GLint  s_loc_opacity;
static GLuint s_vao;
static GLuint s_vbo;
static GLuint s_ebo;
static int    s_initialized;

/* Map system_id to angular position on its ring using golden angle.
 * Produces well-distributed, non-clustering angles for any ID. */
static float system_angle(int system_id) {
    float angle = (float)system_id * GOLDEN_ANGLE;
    angle = fmodf(angle, 360.0f);
    if (angle < 0.0f) angle += 360.0f;
    return angle;
}

/* Build a quad (2 triangles) for a line segment in the XZ plane.
 * Writes 4 vertices (28 floats) and 6 indices.
 * Returns 1 on success, 0 if line is degenerate. */
static int build_line_quad(
    float x1, float z1, float x2, float z2,
    float half_width, const float color[4],
    float *out_verts, unsigned int *out_indices, unsigned int base_vert)
{
    float dx = x2 - x1;
    float dz = z2 - z1;
    float len = sqrtf(dx * dx + dz * dz);
    if (len < 1e-6f) return 0;

    /* Perpendicular direction in XZ plane */
    float px = -dz / len * half_width;
    float pz =  dx / len * half_width;

    float *v = out_verts;

    /* v0: start + perp */
    v[0] = x1 + px; v[1] = 0.0f; v[2] = z1 + pz;
    v[3] = color[0]; v[4] = color[1]; v[5] = color[2]; v[6] = color[3];
    v += CONV_VERT_FLOATS;

    /* v1: start - perp */
    v[0] = x1 - px; v[1] = 0.0f; v[2] = z1 - pz;
    v[3] = color[0]; v[4] = color[1]; v[5] = color[2]; v[6] = color[3];
    v += CONV_VERT_FLOATS;

    /* v2: end - perp */
    v[0] = x2 - px; v[1] = 0.0f; v[2] = z2 - pz;
    v[3] = color[0]; v[4] = color[1]; v[5] = color[2]; v[6] = color[3];
    v += CONV_VERT_FLOATS;

    /* v3: end + perp */
    v[0] = x2 + px; v[1] = 0.0f; v[2] = z2 + pz;
    v[3] = color[0]; v[4] = color[1]; v[5] = color[2]; v[6] = color[3];

    /* Two triangles: 0-1-2, 0-2-3 */
    out_indices[0] = base_vert;
    out_indices[1] = base_vert + 1;
    out_indices[2] = base_vert + 2;
    out_indices[3] = base_vert;
    out_indices[4] = base_vert + 2;
    out_indices[5] = base_vert + 3;

    return 1;
}

int convergence_pass_init(void) {
    /* Reuse concentric ring shader (position(3) + color(4) + u_mvp + u_opacity) */
    const char *vert = cr_vert_source();
    const char *frag = cr_frag_source();
    if (!vert || !frag) return -1;

    s_program = shader_create_program(vert, frag);
    if (!s_program) return -1;

    s_loc_mvp = glGetUniformLocation(s_program, "u_mvp");
    s_loc_opacity = glGetUniformLocation(s_program, "u_opacity");

    /* Create VAO/VBO/EBO */
    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glGenBuffers(1, &s_ebo);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);

    int stride = CONV_VERT_FLOATS * (int)sizeof(float);
    /* position: location 0 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    /* color: location 1 */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride,
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    /* Pre-allocate GPU buffers */
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 CONV_MAX_VERTS * CONV_VERT_FLOATS * (GLsizeiptr)sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 CONV_MAX_INDICES * (GLsizeiptr)sizeof(unsigned int),
                 NULL, GL_DYNAMIC_DRAW);

    s_initialized = 1;
    return 0;
}

void convergence_pass_draw(const render_frame_t *frame) {
    if (!s_initialized) return;

    const br_visual_frame_t *bv = &frame->brain_visual;
    if (bv->link_count <= 0) return;

    /* Reconstruct ring layout (same parameters as ring_pass) */
    solar_system_t ss = planets_at(frame->simulation_jd);
    double sun_lon = fmod(ss.positions[PLANET_EARTH].longitude + 180.0, 360.0);
    double moon_lon = lunar_phase(frame->simulation_jd).moon_longitude;
    ring_today_t rt = ring_today_compute(frame->simulation_jd, sun_lon, moon_lon);

    int today_indices[6];
    ring_today_for_depth(&rt, today_indices);
    cr_layout_t layout = sr_depth_layout(today_indices, RING_INNERMOST, RING_GAP);

    /* Build line quads from brain visual links */
    float verts[CONV_MAX_VERTS * CONV_VERT_FLOATS];
    unsigned int indices[CONV_MAX_INDICES];
    int total_verts = 0;
    int total_indices = 0;

    for (int i = 0; i < bv->link_count && i < CONV_MAX_LINES; i++) {
        const br_visual_link_t *link = &bv->links[i];
        if (link->line_opacity < 0.01f) continue;

        /* Map systems to ring indices */
        int ring_a = (int)depth_ring_for_system(link->system_a);
        int ring_b = (int)depth_ring_for_system(link->system_b);
        if (ring_a >= layout.ring_count || ring_b >= layout.ring_count) continue;

        /* Golden angle placement per system */
        float angle_a = system_angle(link->system_a);
        float angle_b = system_angle(link->system_b);

        /* Compute line geometry between ring midpoints */
        cv_line_t line = cv_line_between_rings(
            &layout, ring_a, ring_b, angle_a, angle_b,
            link->line_opacity, link->line_thickness, link->color_warmth);

        /* Compose final color with opacity baked into alpha */
        float color[4] = {
            line.color[0], line.color[1], line.color[2],
            line.color[3] * line.opacity
        };

        /* Half-width in scene units (thickness 1-4 maps to 0.02-0.08) */
        float half_w = line.thickness * 0.02f;

        if (build_line_quad(line.x1, line.z1, line.x2, line.z2,
                           half_w, color,
                           &verts[total_verts * CONV_VERT_FLOATS],
                           &indices[total_indices],
                           (unsigned int)total_verts)) {
            total_verts += CONV_VERTS_PER_LINE;
            total_indices += CONV_INDICES_PER_LINE;
        }
    }

    if (total_indices <= 0) return;

    /* Upload to GPU */
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    total_verts * CONV_VERT_FLOATS * (GLsizeiptr)sizeof(float),
                    verts);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    total_indices * (GLsizeiptr)sizeof(unsigned int),
                    indices);

    /* Draw with blending */
    glUseProgram(s_program);
    glUniformMatrix4fv(s_loc_mvp, 1, GL_FALSE, frame->view_proj.m);

    /* Use overall_intensity as scene-level opacity multiplier */
    float opacity = bv->overall_intensity;
    if (opacity < 0.01f) opacity = 1.0f; /* default to full if not set */
    if (s_loc_opacity >= 0) glUniform1f(s_loc_opacity, opacity);

    glBindVertexArray(s_vao);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, total_indices, GL_UNSIGNED_INT, 0);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void convergence_pass_destroy(void) {
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    glDeleteBuffers(1, &s_ebo);
    s_initialized = 0;
}

#endif /* __EMSCRIPTEN__ */
