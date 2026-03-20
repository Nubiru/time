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
#include <math.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../font_bitmap.h"
#include "../font_atlas.h"
#include "../glyph_batch.h"
#include "../billboard.h"
#include "../../systems/astronomy/planets.h"
#include "../../systems/astronomy/lunar.h"
#include "../../systems/astrology/zodiac.h"
#include "../../systems/tzolkin/tzolkin.h"
#include "../../systems/iching/iching.h"
#include "../../systems/chinese/chinese.h"
#include "../../systems/human_design/human_design.h"
#include "../../ui/card_data.h"
#include "../../ui/card_layout.h"
#include "../../ui/card_selector.h"
#include "../../ui/card_style.h"
#include "../../ui/today_card.h"

/* Must match ORBIT_SCALE in planet_pass.c so labels align with planet sprites */
static const float TEXT_ORBIT_SCALE = 3.0f;

#ifndef PI
#define PI 3.14159265358979323846
#endif
#define TEXT_DEG_TO_RAD (PI / 180.0)

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

/* --- Card text helpers --- */

/* Maximum characters per card text line to keep within GLYPH_BATCH_MAX */
#define CARD_TEXT_LINE_MAX 20

/* Append characters from a string as glyph instances at a given pixel position.
 * Advances px_x by character spacing. Returns updated glyph count. */
static int append_text_glyphs(glyph_instance_t *inst, int len,
                              const char *text, int max_chars,
                              float px_x, float px_y,
                              float scale, glyph_color_t color)
{
    float spacing = 10.0f * scale;  /* pixels per character advance */
    int chars = (int)strlen(text);
    if (chars > max_chars) chars = max_chars;
    for (int i = 0; i < chars && len < GLYPH_BATCH_MAX; i++) {
        if (text[i] < 32) continue;  /* skip control chars */
        inst[len].glyph_id = (int)text[i];
        inst[len].position = vec3_create(
            px_x + (float)i * spacing, px_y, 0.0f);
        inst[len].scale    = scale;
        inst[len].color    = color;
        len++;
    }
    return len;
}

/* Compute approximate sun ecliptic longitude from JD (Meeus low-accuracy). */
static double approx_sun_longitude(double jd)
{
    double T = (jd - 2451545.0) / 36525.0;
    double lon = 280.46646 + 36000.76983 * T;
    lon = lon - 360.0 * (int)(lon / 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

/* Draw card text as a 2D screen-space overlay.
 * Called after the 3D planet label draw; reuses same VAO/VBO/EBO. */
static void draw_card_text(const render_frame_t *frame)
{
    if (!layer_is_visible(frame->layers, LAYER_CARDS))
        return;

    /* Get viewport dimensions */
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2];
    int vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    /* Compute card layout (all cards visible for demo) */
    float aspect = (float)vw / (float)vh;
    card_layout_t layout = card_layout_compute(card_all_mask(), aspect);

    /* Compute system data via zoom-aware card pipeline */
    double jd = frame->simulation_jd;
    double sun_lon = approx_sun_longitude(jd);
    lunar_info_t moon = lunar_phase(jd);
    double moon_lon = moon.moon_longitude;

    /* Select top 5 systems for current zoom depth */
    cs_selection_t sel = cs_select(frame->log_zoom, aspect);

    card_content_t card_contents[CARD_TYPE_COUNT];
    const card_content_t *contents[CARD_TYPE_COUNT];
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        int sys_id = (i < sel.filled_count) ? sel.slots[i].system_id : -1;
        card_contents[i] = (sys_id >= 0)
            ? today_card_for_system(sys_id, jd, sun_lon, moon_lon)
            : (card_content_t){0};
        contents[i] = &card_contents[i];
        /* Update layout visibility based on selection */
        layout.cards[i].visible = (sys_id >= 0);
        layout.cards[i].opacity = (i < sel.filled_count) ? sel.slots[i].opacity : 0.0f;
    }

    /* Build glyph instances for card text */
    glyph_instance_t instances[GLYPH_BATCH_MAX];
    int len = 0;

    /* Scale: title slightly larger than body */
    float title_scale = 1.8f;
    float body_scale  = 1.4f;

    /* Vertical offsets within card (in pixels from card top) */
    float margin_x = 8.0f;
    float margin_y_title = 6.0f;
    float line_height = 18.0f;

    for (int c = 0; c < CARD_TYPE_COUNT && len < GLYPH_BATCH_MAX - 20; c++) {
        const card_rect_t *r = &layout.cards[c];
        if (!r->visible) continue;

        const card_content_t *content = contents[c];

        /* Per-card themed colors from card_style */
        int sys_id = (c < sel.filled_count) ? sel.slots[c].system_id : -1;
        card_style_t style = (sys_id >= 0)
            ? card_style_for_system(sys_id, r->opacity, THEME_COSMOS)
            : card_style_default(r->opacity, THEME_COSMOS);
        glyph_color_t title_color = {style.title.r, style.title.g,
                                     style.title.b, style.title.a};
        glyph_color_t body_color  = {style.body.r, style.body.g,
                                     style.body.b, style.body.a};

        /* Convert normalized card position to pixel coordinates */
        float px = r->x * (float)vw + margin_x;
        float py = r->y * (float)vh + margin_y_title;

        /* Title */
        len = append_text_glyphs(instances, len, content->title,
                                 CARD_TEXT_LINE_MAX,
                                 px, py, title_scale, title_color);

        /* Line 1 */
        py += line_height * title_scale;
        len = append_text_glyphs(instances, len, content->line1,
                                 CARD_TEXT_LINE_MAX,
                                 px, py, body_scale, body_color);

        /* Line 2 */
        py += line_height * body_scale;
        len = append_text_glyphs(instances, len, content->line2,
                                 CARD_TEXT_LINE_MAX,
                                 px, py, body_scale, body_color);
    }

    if (len == 0)
        return;

    /* Build orthographic projection: pixel coords -> NDC.
     * (0,0) = top-left, (vw,vh) = bottom-right. */
    mat4_t ortho;
    memset(&ortho, 0, sizeof(ortho));
    ortho.m[0]  =  2.0f / (float)vw;
    ortho.m[5]  = -2.0f / (float)vh;  /* flip Y: top = 0 */
    ortho.m[10] = -1.0f;
    ortho.m[12] = -1.0f;
    ortho.m[13] =  1.0f;
    ortho.m[14] =  0.0f;
    ortho.m[15] =  1.0f;

    /* For screen-space text: fixed camera axes, no billboarding */
    vec3_t cam_right = vec3_create(1.0f, 0.0f, 0.0f);
    vec3_t cam_up    = vec3_create(0.0f, -1.0f, 0.0f);  /* -Y: screen Y grows down */

    glyph_atlas_t batch_atlas = {
        .cols     = FONT_BITMAP_COLS,
        .rows     = FONT_BITMAP_ROWS,
        .first_id = FONT_BITMAP_FIRST,
        .last_id  = FONT_BITMAP_LAST
    };

    /* Base size in pixels: each glyph quad is base_width x base_height pixels,
     * then multiplied by per-instance scale. */
    glyph_batch_t batch = glyph_batch_create(
        instances, len, batch_atlas,
        cam_right, cam_up,
        8.0f, 14.0f);

    if (batch.glyph_count == 0)
        return;

    /* Upload to same VAO/VBO/EBO (replaces 3D label data) */
    glBindVertexArray(s_text_vao);

    glBindBuffer(GL_ARRAY_BUFFER, s_text_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (GLsizeiptr)glyph_batch_vertex_bytes(&batch),
                    batch.vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_text_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    (GLsizeiptr)glyph_batch_index_bytes(&batch),
                    batch.indices);

    /* GL state: depth off, blend on (should still be set from labels draw) */
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Set ortho MVP for screen-space rendering */
    glUseProgram(s_text_program);
    glUniformMatrix4fv(s_text_loc_mvp, 1, GL_FALSE, ortho.m);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_text_atlas_tex);
    glUniform1i(s_text_loc_atlas, 0);

    /* Draw card text */
    glDrawElements(GL_TRIANGLES, batch.index_count, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void text_pass_draw(const render_frame_t *frame) {
    if (!layer_is_visible(frame->layers, LAYER_LABELS)) {
        /* Labels layer hidden but still draw card text if cards are visible */
        draw_card_text(frame);
        return;
    }

    /* Compute planet positions at current simulation time */
    solar_system_t sys = planets_at(frame->simulation_jd);

    /* Build glyph instances for planet name labels */
    glyph_instance_t instances[GLYPH_BATCH_MAX];
    int len = 0;

    /* Sun label at origin */
    {
        static const char *sun_name = "Sun";
        int slen = (int)strlen(sun_name);
        float spacing = 0.3f;
        float sx = -spacing * (float)(slen - 1) * 0.5f;
        for (int i = 0; i < slen && len < GLYPH_BATCH_MAX; i++) {
            instances[len].glyph_id = (int)sun_name[i];
            instances[len].position = vec3_create(
                sx + (float)i * spacing, 1.0f, 0.0f);
            instances[len].scale    = 1.0f;
            instances[len].color    = (glyph_color_t){1.0f, 0.85f, 0.55f, 1.0f};
            len++;
        }
    }

    /* Planet labels at orbital positions (same math as planet_pass.c) */
    for (int p = 0; p < 8 && len < GLYPH_BATCH_MAX - 10; p++) {
        const char *name = planet_name(p);
        if (!name) continue;
        int nlen = (int)strlen(name);

        /* Ecliptic spherical -> Cartesian (matches planet_pack.c) */
        double lon_rad = sys.positions[p].longitude * TEXT_DEG_TO_RAD;
        double lat_rad = sys.positions[p].latitude  * TEXT_DEG_TO_RAD;
        double dist    = sys.positions[p].distance;
        double cos_lat = cos(lat_rad);

        float lx = (float)(dist * cos_lat * cos(lon_rad));
        float lz = (float)(dist * cos_lat * sin(lon_rad));
        float ly = (float)(dist * sin(lat_rad));

        /* Apply sqrt distance scaling (matches planet_pass.c) */
        float r_linear = sqrtf(lx * lx + ly * ly + lz * lz);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * TEXT_ORBIT_SCALE;
            float s = r_sqrt / r_linear;
            lx *= s;
            ly *= s;
            lz *= s;
        }

        /* Offset label slightly above ecliptic plane */
        ly += 0.5f;

        /* Place label characters centered above planet */
        float spacing = 0.25f;
        float sx = -spacing * (float)(nlen - 1) * 0.5f;
        for (int i = 0; i < nlen && len < GLYPH_BATCH_MAX; i++) {
            instances[len].glyph_id = (int)name[i];
            instances[len].position = vec3_create(
                lx + sx + (float)i * spacing, ly, lz);
            instances[len].scale    = 0.8f;
            instances[len].color    = (glyph_color_t){0.8f, 0.85f, 0.9f, 0.9f};
            len++;
        }
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

    if (batch.glyph_count == 0) {
        /* No 3D labels but still draw card text */
        draw_card_text(frame);
        return;
    }

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

    /* Draw 3D planet labels */
    glDrawElements(GL_TRIANGLES, batch.index_count, GL_UNSIGNED_INT, 0);

    /* Restore GL state after 3D labels */
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    /* --- Second draw batch: 2D card text overlay --- */
    draw_card_text(frame);
}

void text_pass_destroy(void) {
    glDeleteProgram(s_text_program);
    glDeleteTextures(1, &s_text_atlas_tex);
    glDeleteBuffers(1, &s_text_vbo);
    glDeleteBuffers(1, &s_text_ebo);
    glDeleteVertexArrays(1, &s_text_vao);
}

#endif /* __EMSCRIPTEN__ */
