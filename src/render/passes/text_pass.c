/*
 * text_pass.c — Text render pass (font atlas + glyph batches)
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Loads the font bitmap atlas texture, batches glyph quads, draws text.
 */

#ifdef __EMSCRIPTEN__

#include "text_pass.h"
#include <stdio.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../font_bitmap.h"
#include "../font_atlas.h"
#include "../glyph_batch.h"
#include "../billboard.h"

/* --- Module-static GL handles --- */
static GLuint s_text_program;
static GLint  s_text_loc_mvp;
static GLint  s_text_loc_atlas;
static GLuint s_text_vao;
static GLuint s_text_vbo;
static GLuint s_text_ebo;
static GLuint s_text_atlas_tex;
static font_atlas_t s_text_font;

/* --- Static scratch buffer for atlas pixels (BSS, zero stack cost) --- */
static uint8_t s_atlas_pixels[FONT_BITMAP_ATLAS_W * FONT_BITMAP_ATLAS_H];

/* --- GLSL shader sources --- */

static const char *s_text_vert_source =
    "#version 300 es\n"
    "precision highp float;\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec4 a_color;\n"
    "uniform mat4 u_mvp;\n"
    "out vec2 v_uv;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "}\n";

static const char *s_text_frag_source =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "in vec4 v_color;\n"
    "uniform sampler2D u_atlas;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    float a = texture(u_atlas, v_uv).r;\n"
    "    if (a < 0.5) discard;\n"
    "    frag_color = vec4(v_color.rgb, v_color.a * a);\n"
    "}\n";

int text_pass_init(void) {
    /* Compile shader program */
    s_text_program = shader_create_program(s_text_vert_source, s_text_frag_source);
    if (s_text_program == 0) {
        printf("Failed to create text shader\n");
        return 1;
    }
    s_text_loc_mvp   = glGetUniformLocation(s_text_program, "u_mvp");
    s_text_loc_atlas = glGetUniformLocation(s_text_program, "u_atlas");

    /* Generate font atlas pixels */
    font_bitmap_generate(s_atlas_pixels);

    /* Create GL texture from atlas */
    glGenTextures(1, &s_text_atlas_tex);
    glBindTexture(GL_TEXTURE_2D, s_text_atlas_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                 FONT_BITMAP_ATLAS_W, FONT_BITMAP_ATLAS_H,
                 0, GL_RED, GL_UNSIGNED_BYTE, s_atlas_pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Create VAO/VBO/EBO for dynamic glyph batches */
    glGenVertexArrays(1, &s_text_vao);
    glGenBuffers(1, &s_text_vbo);
    glGenBuffers(1, &s_text_ebo);

    glBindVertexArray(s_text_vao);

    glBindBuffer(GL_ARRAY_BUFFER, s_text_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(GLYPH_BATCH_MAX * 4 * GLYPH_VERTEX_STRIDE
                              * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Attribute 0: position (vec3, offset 0) */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          GLYPH_VERTEX_STRIDE * (int)sizeof(float),
                          (void*)0);
    /* Attribute 1: uv (vec2, offset 12 bytes) */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          GLYPH_VERTEX_STRIDE * (int)sizeof(float),
                          (void*)(3 * sizeof(float)));
    /* Attribute 2: color (vec4, offset 20 bytes) */
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          GLYPH_VERTEX_STRIDE * (int)sizeof(float),
                          (void*)(5 * sizeof(float)));

    /* Index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_text_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(GLYPH_BATCH_MAX * 6 * (int)sizeof(unsigned int)),
                 NULL, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);

    /* Create font atlas descriptor */
    s_text_font = font_atlas_create(
        FONT_BITMAP_COLS, FONT_BITMAP_ROWS,
        FONT_BITMAP_FIRST, FONT_BITMAP_LAST,
        (float)FONT_BITMAP_CELL_W, (float)FONT_BITMAP_CELL_H);

    printf("Text: atlas %dx%d, shader compiled\n",
           FONT_BITMAP_ATLAS_W, FONT_BITMAP_ATLAS_H);
    return 0;
}

void text_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_LABELS))
        return;

    /* Build demo text: "Time" above ecliptic plane */
    static const char *demo_text = "Time";
    int len = (int)strlen(demo_text);
    if (len > GLYPH_BATCH_MAX)
        len = GLYPH_BATCH_MAX;

    /* Create glyph instances for each character */
    glyph_instance_t instances[GLYPH_BATCH_MAX];
    float spacing = 0.35f;
    float start_x = -spacing * (float)(len - 1) * 0.5f;

    for (int i = 0; i < len; i++) {
        instances[i].glyph_id  = (int)demo_text[i];
        instances[i].position  = vec3_create(start_x + (float)i * spacing,
                                             5.0f, 0.0f);
        instances[i].scale     = 1.0f;
        instances[i].color     = (glyph_color_t){1.0f, 0.85f, 0.55f, 1.0f};
    }

    /* Get camera basis vectors for billboarding */
    vec3_t cam_right, cam_up;
    billboard_camera_vectors(&frame->view, &cam_right, &cam_up);

    /* Build glyph atlas descriptor matching glyph_batch API */
    glyph_atlas_t batch_atlas = {
        .cols     = FONT_BITMAP_COLS,
        .rows     = FONT_BITMAP_ROWS,
        .first_id = FONT_BITMAP_FIRST,
        .last_id  = FONT_BITMAP_LAST
    };

    /* Generate batched vertex data */
    glyph_batch_t batch = glyph_batch_create(
        instances, len, batch_atlas,
        cam_right, cam_up,
        0.3f, 0.6f);

    if (batch.glyph_count == 0)
        return;

    /* Upload vertices + indices to GPU */
    glBindVertexArray(s_text_vao);

    glBindBuffer(GL_ARRAY_BUFFER, s_text_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)glyph_batch_vertex_bytes(&batch),
                    batch.vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_text_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    (GLsizeiptr)glyph_batch_index_bytes(&batch),
                    batch.indices);

    /* Set GL state */
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Bind shader + uniforms */
    glUseProgram(s_text_program);
    glUniformMatrix4fv(s_text_loc_mvp, 1, GL_FALSE, frame->view_proj.m);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_text_atlas_tex);
    glUniform1i(s_text_loc_atlas, 0);

    /* Draw */
    glDrawElements(GL_TRIANGLES, batch.index_count, GL_UNSIGNED_INT, 0);

    /* Restore GL state */
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void text_pass_destroy(void) {
    glDeleteProgram(s_text_program);
    glDeleteTextures(1, &s_text_atlas_tex);
    glDeleteBuffers(1, &s_text_vbo);
    glDeleteBuffers(1, &s_text_ebo);
    glDeleteVertexArrays(1, &s_text_vao);
}

#endif /* __EMSCRIPTEN__ */
