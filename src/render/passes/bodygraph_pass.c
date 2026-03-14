/*
 * bodygraph_pass.c — Human Design bodygraph render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Renders 9 energy centers as quads, 36 channels as lines, and 64 gates
 * as marker quads. Data computed per frame from current planetary positions.
 */

#ifdef __EMSCRIPTEN__

#include "bodygraph_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../bodygraph_pack.h"
#include "../../systems/human_design/human_design.h"
#include "../../systems/astronomy/planets.h"

/* --- Module-static GL handles --- */
static GLuint s_bgp_program;
static GLint  s_bgp_loc_projection;
static GLuint s_bgp_vao;
static GLuint s_bgp_vbo;

int bodygraph_pass_init(void) {
    s_bgp_program = shader_create_program(
        bgp_vert_source(), bgp_frag_source());
    if (s_bgp_program == 0) {
        printf("Failed to create bodygraph shader\n");
        return 1;
    }
    s_bgp_loc_projection = glGetUniformLocation(s_bgp_program, "u_projection");

    /* Pre-allocate VBO */
    glGenVertexArrays(1, &s_bgp_vao);
    glBindVertexArray(s_bgp_vao);

    glGenBuffers(1, &s_bgp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_bgp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(BGP_MAX_VERTICES * BGP_VERTEX_STRIDE),
                 NULL, GL_DYNAMIC_DRAW);

    /* pos(2) + color(4) = 6 floats = 24 bytes */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, BGP_VERTEX_STRIDE, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, BGP_VERTEX_STRIDE, (void*)8);

    glBindVertexArray(0);
    printf("Bodygraph: 9 centers + 36 channels + 64 gates, shaders compiled\n");

    return 0;
}

void bodygraph_pass_draw(const render_frame_t *frame) {
    /* Bodygraph is a specialized view — only draw when LAYER_CARDS visible */
    if (!layer_is_visible(frame->layers, LAYER_CARDS))
        return;

    /* Compute active gates from planetary positions */
    solar_system_t sys = planets_at(frame->simulation_jd);
    int active_gates[65] = {0};
    for (int p = 0; p < 8; p++) {
        int gate = hd_gate_at_degree(sys.positions[p].longitude);
        if (gate >= 1 && gate <= 64)
            active_gates[gate] = 1;
    }

    /* Compute defined centers from active gates */
    int defined_centers[9] = {0};
    bodygraph_defined_centers(active_gates, defined_centers);

    /* Pack all bodygraph geometry */
    bgp_config_t config = bgp_default_config();
    float verts[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    int total_verts = bgp_pack(defined_centers, active_gates, &config, verts);

    if (total_verts == 0) return;

    /* Build simple orthographic projection for screen-space */
    mat4_t ortho = mat4_identity();
    ortho.m[0]  =  2.0f / config.width;
    ortho.m[5]  =  2.0f / config.height;
    ortho.m[10] = -1.0f;
    ortho.m[12] = -1.0f + 2.0f * config.offset_x / config.width;
    ortho.m[13] = -1.0f + 2.0f * config.offset_y / config.height;

    glUseProgram(s_bgp_program);
    glUniformMatrix4fv(s_bgp_loc_projection, 1, GL_FALSE, ortho.m);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(s_bgp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_bgp_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)(total_verts * BGP_VERTEX_STRIDE),
                    verts);

    /* Centers + gates are triangles, channels are lines.
     * Pack order: centers first, then channels, then gates.
     * We need bgp_info to know the split. */
    bgp_info_t info = bgp_info(defined_centers, active_gates);

    /* Draw center quads + gate quads as triangles */
    int tri_verts = info.center_verts + info.gate_verts;
    if (tri_verts > 0) {
        glDrawArrays(GL_TRIANGLES, 0, info.center_verts);
        if (info.gate_verts > 0) {
            glDrawArrays(GL_TRIANGLES,
                         info.center_verts + info.channel_verts,
                         info.gate_verts);
        }
    }

    /* Draw channel lines */
    if (info.channel_verts > 0) {
        glDrawArrays(GL_LINES, info.center_verts, info.channel_verts);
    }

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void bodygraph_pass_destroy(void) {
    glDeleteProgram(s_bgp_program);
    glDeleteBuffers(1, &s_bgp_vbo);
    glDeleteVertexArrays(1, &s_bgp_vao);
}

#endif /* __EMSCRIPTEN__ */
