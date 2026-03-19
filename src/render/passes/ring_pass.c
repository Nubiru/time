/*
 * ring_pass.c — Concentric knowledge system ring render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Uses system_rings (sr_depth_layout) for data and concentric_ring (cr_generate)
 * for geometry. Renders 6 concentric rings in the ecliptic plane (XZ, y=0).
 */

#ifdef __EMSCRIPTEN__

#include "ring_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../concentric_ring.h"
#include "../system_rings.h"
#include "../shader.h"

/* Ring placement: outside the zodiac ring (which is at ~4.2-4.8).
 * Concentric rings start at radius 5.5, extending outward. */
#define RING_INNERMOST  5.5f
#define RING_GAP        0.2f

/* Max vertex/index buffer sizes */
#define MAX_VERTS   8192
#define MAX_INDICES 16384

/* Module-static GL handles */
static GLuint s_program;
static GLint  s_loc_mvp;
static GLint  s_loc_opacity;
static GLuint s_vao;
static GLuint s_vbo;
static GLuint s_ebo;
static int    s_index_count;
static int    s_initialized;

int ring_pass_init(void) {
    /* Compile shaders from concentric_ring GLSL source */
    const char *vert = cr_vert_source();
    const char *frag = cr_frag_source();
    if (!vert || !frag) {
        printf("ring_pass: missing shader source\n");
        return -1;
    }

    s_program = shader_create_program(vert, frag);
    if (!s_program) {
        printf("ring_pass: shader compilation failed\n");
        return -1;
    }

    s_loc_mvp = glGetUniformLocation(s_program, "u_mvp");
    s_loc_opacity = glGetUniformLocation(s_program, "u_opacity");

    /* Create VAO/VBO/EBO */
    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    glGenBuffers(1, &s_ebo);

    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);

    /* Vertex layout: position(3) + color(4) = 7 floats, stride = 28 bytes */
    int stride = CR_VERT_FLOATS * (int)sizeof(float);
    /* position: location 0 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    /* color: location 1 */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride,
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    /* Allocate GPU buffers (data uploaded each frame) */
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 MAX_VERTS * CR_VERT_FLOATS * (GLsizeiptr)sizeof(float),
                 NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 MAX_INDICES * (GLsizeiptr)sizeof(unsigned int),
                 NULL, GL_DYNAMIC_DRAW);

    s_initialized = 1;
    return 0;
}

void ring_pass_draw(const render_frame_t *frame) {
    if (!s_initialized) return;
    if (!layer_is_visible(frame->layers, LAYER_ZODIAC_RING)) return;

    /* Build ring layout from system data (no "today" highlighting for now) */
    cr_layout_t layout = sr_depth_layout(NULL, RING_INNERMOST, RING_GAP);

    /* Check buffer capacity */
    int needed_verts = cr_total_verts(&layout);
    int needed_indices = cr_total_indices(&layout);
    if (needed_verts > MAX_VERTS || needed_indices > MAX_INDICES) return;

    /* Generate geometry */
    float verts[MAX_VERTS * CR_VERT_FLOATS];
    unsigned int indices[MAX_INDICES];
    cr_info_t info = cr_generate(&layout, verts, indices);

    s_index_count = info.total_indices;
    if (s_index_count <= 0) return;

    /* Upload to GPU */
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    info.total_vertices * CR_VERT_FLOATS * (GLsizeiptr)sizeof(float),
                    verts);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    info.total_indices * (GLsizeiptr)sizeof(unsigned int),
                    indices);

    /* Draw */
    glUseProgram(s_program);

    mat4_t mvp = frame->view_proj;
    glUniformMatrix4fv(s_loc_mvp, 1, GL_FALSE, mvp.m);

    float opacity = frame->layers.opacity[LAYER_ZODIAC_RING];
    if (s_loc_opacity >= 0) glUniform1f(s_loc_opacity, opacity);

    glBindVertexArray(s_vao);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, s_index_count, GL_UNSIGNED_INT, 0);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void ring_pass_destroy(void) {
    if (!s_initialized) return;
    glDeleteProgram(s_program);
    glDeleteVertexArrays(1, &s_vao);
    glDeleteBuffers(1, &s_vbo);
    glDeleteBuffers(1, &s_ebo);
    s_initialized = 0;
}

#endif /* __EMSCRIPTEN__ */
