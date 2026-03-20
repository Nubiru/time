/*
 * card_pass.c — Knowledge system info card render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Renders card background quads and border lines as 2D screen-space overlay.
 * Card content (text) is handled separately by the HUD/text system.
 */

#ifdef __EMSCRIPTEN__

#include "card_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../card_pack.h"
#include "../../ui/card_layout.h"
#include "../../ui/card_selector.h"
#include "../../ui/card_style.h"

/* --- Module-static GL handles --- */

/* Card background quads */
static GLuint s_quad_program;
static GLint  s_quad_loc_resolution;
static GLint  s_quad_loc_corner_radius;
static GLuint s_quad_vao;
static GLuint s_quad_vbo;
static GLuint s_quad_ebo;

/* Card border lines */
static GLuint s_line_program;
static GLint  s_line_loc_resolution;
static GLuint s_line_vao;
static GLuint s_line_vbo;

int card_pass_init(void) {
    /* Quad shader */
    s_quad_program = shader_create_program(
        cp_quad_vert_source(), cp_quad_frag_source());
    if (s_quad_program == 0) {
        printf("Failed to create card quad shader\n");
        return 1;
    }
    s_quad_loc_resolution    = glGetUniformLocation(s_quad_program, "u_resolution");
    s_quad_loc_corner_radius = glGetUniformLocation(s_quad_program, "u_corner_radius");

    /* Pre-allocate quad VBO + EBO */
    glGenVertexArrays(1, &s_quad_vao);
    glBindVertexArray(s_quad_vao);

    glGenBuffers(1, &s_quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(CP_MAX_CARDS * CP_VERTS_PER_QUAD * CP_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(2) + uv(2) + color(4) = 8 floats = 32 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)8);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 32, (void*)16);

    glGenBuffers(1, &s_quad_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_quad_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(CP_MAX_CARDS * CP_INDICES_PER_QUAD * (int)sizeof(unsigned int)),
                 NULL, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);

    /* Line shader */
    s_line_program = shader_create_program(
        cp_line_vert_source(), cp_line_frag_source());
    if (s_line_program == 0) {
        printf("Failed to create card line shader\n");
        return 1;
    }
    s_line_loc_resolution = glGetUniformLocation(s_line_program, "u_resolution");

    /* Pre-allocate line VBO */
    glGenVertexArrays(1, &s_line_vao);
    glBindVertexArray(s_line_vao);

    glGenBuffers(1, &s_line_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_line_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)((CP_MAX_CARDS * 8 + CP_MAX_DIVIDERS * 2) *
                              CP_LINE_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(2) + color(4) = 6 floats = 24 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (void*)8);

    glBindVertexArray(0);
    printf("Cards: quad + line shaders compiled\n");

    return 0;
}

void card_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_CARDS))
        return;

    /* Query actual viewport dimensions from GL */
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float vw = (float)viewport[2];
    float vh = (float)viewport[3];
    if (vw < 1.0f) vw = 1920.0f;  /* fallback */
    if (vh < 1.0f) vh = 1080.0f;
    float aspect = vw / vh;

    /* Select top cards for current zoom depth (same as text_pass) */
    cs_selection_t sel = cs_select(frame->log_zoom, aspect);
    if (sel.filled_count == 0) return;

    /* Build layout with selected cards visible */
    card_layout_t layout = card_layout_compute(card_all_mask(), aspect);
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        layout.cards[i].visible = (i < sel.filled_count);
        layout.cards[i].opacity = (i < sel.filled_count)
            ? sel.slots[i].opacity : 0.0f;
    }

    /* Pack card background quads (dummy color — overwritten below) */
    cp_quad_data_t qdata = cp_pack_quads(&layout, vw, vh,
                                          0.0f, 0.0f, 0.0f, 0.0f);

    /* Overwrite per-card vertex colors from card_style */
    for (int i = 0; i < qdata.card_count && i < sel.filled_count; i++) {
        card_style_t style = card_style_for_system(
            sel.slots[i].system_id, sel.slots[i].opacity, THEME_COSMOS);
        for (int v = 0; v < CP_VERTS_PER_QUAD; v++) {
            int base = (i * CP_VERTS_PER_QUAD + v) * CP_VERTEX_FLOATS;
            qdata.vertices[base + 4] = style.background.r;
            qdata.vertices[base + 5] = style.background.g;
            qdata.vertices[base + 6] = style.background.b;
            qdata.vertices[base + 7] = style.background.a;
        }
    }

    if (qdata.vertex_count > 0) {
        glUseProgram(s_quad_program);
        glUniform2f(s_quad_loc_resolution, vw, vh);
        glUniform1f(s_quad_loc_corner_radius, 8.0f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        glBindVertexArray(s_quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, s_quad_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        (GLsizeiptr)cp_quad_vertex_bytes(&qdata),
                        qdata.vertices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_quad_ebo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                        (GLsizeiptr)cp_quad_index_bytes(&qdata),
                        qdata.indices);

        glDrawElements(GL_TRIANGLES, qdata.index_count,
                        GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

    /* Pack card border lines with theme border color */
    card_style_t border_style = card_style_default(1.0f, THEME_COSMOS);
    cp_line_data_t ldata = cp_pack_lines(&layout, vw, vh,
                                          border_style.border.r,
                                          border_style.border.g,
                                          border_style.border.b,
                                          border_style.border.a, 2);

    if (ldata.vertex_count > 0) {
        glUseProgram(s_line_program);
        glUniform2f(s_line_loc_resolution, vw, vh);

        glBindVertexArray(s_line_vao);
        glBindBuffer(GL_ARRAY_BUFFER, s_line_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        (GLsizeiptr)cp_line_vertex_bytes(&ldata),
                        ldata.vertices);

        glDrawArrays(GL_LINES, 0, ldata.vertex_count);
        glBindVertexArray(0);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void card_pass_destroy(void) {
    glDeleteProgram(s_quad_program);
    glDeleteBuffers(1, &s_quad_vbo);
    glDeleteBuffers(1, &s_quad_ebo);
    glDeleteVertexArrays(1, &s_quad_vao);
    glDeleteProgram(s_line_program);
    glDeleteBuffers(1, &s_line_vbo);
    glDeleteVertexArrays(1, &s_line_vao);
}

#endif /* __EMSCRIPTEN__ */
