/*
 * tree_pass.c — Kabbalah Tree of Life render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Renders 10 Sefirot nodes as quads, 22 paths as lines, and 3 pillars
 * as structural lines. All geometry in screen-space 2D.
 */

#ifdef __EMSCRIPTEN__

#include "tree_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../tree_pack.h"

/* --- Module-static GL handles --- */
static GLuint s_trp_program;
static GLint  s_trp_loc_projection;
static GLuint s_trp_vao;
static GLuint s_trp_vbo;

/* --- Static scratch buffer (BSS, zero stack cost) --- */
static float s_trp_scratch[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];

int tree_pass_init(void) {
    s_trp_program = shader_create_program(
        trp_vert_source(), trp_frag_source());
    if (s_trp_program == 0) {
        printf("Failed to create tree shader\n");
        return 1;
    }
    s_trp_loc_projection = glGetUniformLocation(s_trp_program, "u_projection");

    /* Pre-allocate VBO */
    glGenVertexArrays(1, &s_trp_vao);
    glBindVertexArray(s_trp_vao);

    glGenBuffers(1, &s_trp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_trp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(TRP_MAX_VERTICES * TRP_VERTEX_STRIDE),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(2) + color(4) = 6 floats = 24 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, TRP_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, TRP_VERTEX_STRIDE, (void*)8);

    glBindVertexArray(0);
    printf("Tree: 10 Sefirot + 22 paths + 3 pillars, shaders compiled\n");

    return 0;
}

void tree_pass_draw(const render_frame_t *frame) {
    /* Tree view — only draw when LAYER_CARDS visible */
    if (!layer_is_visible(frame->layers, LAYER_CARDS))
        return;

    /* For now, all nodes active and all paths active (full tree lit) */
    int active_nodes[11] = {1,1,1,1,1,1,1,1,1,1,0}; /* 10 Sefirot on, Da'at off */
    int active_paths[22];
    for (int i = 0; i < 22; i++) active_paths[i] = 1;

    trp_config_t config = trp_default_config();
    int total_verts = trp_pack(active_nodes, active_paths, &config, s_trp_scratch);

    if (total_verts == 0) return;

    trp_info_t info = trp_info(active_nodes, active_paths, &config);

    /* Build ortho projection */
    mat4_t ortho = mat4_identity();
    ortho.m[0]  =  2.0f / config.width;
    ortho.m[5]  =  2.0f / config.height;
    ortho.m[10] = -1.0f;
    ortho.m[12] = -1.0f + 2.0f * config.offset_x / config.width;
    ortho.m[13] = -1.0f + 2.0f * config.offset_y / config.height;

    glUseProgram(s_trp_program);
    glUniformMatrix4fv(s_trp_loc_projection, 1, GL_FALSE, ortho.m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(s_trp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_trp_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(total_verts * TRP_VERTEX_STRIDE),
                    s_trp_scratch);

    /* Node quads as triangles */
    if (info.node_verts > 0) {
        glDrawArrays(GL_TRIANGLES, 0, info.node_verts);
    }

    /* Path lines + pillar lines */
    int line_start = info.node_verts;
    int line_count = info.path_verts + info.pillar_verts;
    if (line_count > 0) {
        glDrawArrays(GL_LINES, line_start, line_count);
    }

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void tree_pass_destroy(void) {
    glDeleteProgram(s_trp_program);
    glDeleteBuffers(1, &s_trp_vbo);
    glDeleteVertexArrays(1, &s_trp_vao);
}

#endif /* __EMSCRIPTEN__ */
