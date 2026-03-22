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
#include "../../systems/unified/today_summary.h"
#include "../../ui/kin_oracle_layout.h"
#include "../../ui/kin_wavespell_layout.h"
#include "../../ui/kin_cell.h"
#include "../../ui/focus_mode.h"
#include "../../ui/earth_timeline_layout.h"
#include "../constellation_label.h"
#include "../zoom_fade.h"
#include "../../systems/tzolkin/dreamspell.h"
#include "../../systems/iching/iching.h"
#include "../../ui/hexagram_layout.h"
#include "../../ui/daily_hd_layout.h"
#include "../../ui/zodiac_wheel_layout.h"
#include "../../ui/zodiac_animals_layout.h"
#include "../../ui/bagua_layout.h"
#include "../../ui/dignity_table_layout.h"
#include "../../ui/prayer_times_layout.h"
#include "../../ui/nakshatra_wheel_layout.h"
#include "../../ui/daily_transit_layout.h"
#include "../../systems/astronomy/lunar.h"
#include "../msdf_text.h"
#include "../zoom_fade.h"
#include "../constellation_label.h"

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

/* --- MSDF text rendering (2D card/oracle text) --- */
static GLuint s_msdf_program;
static GLint  s_msdf_loc_mvp;
static GLint  s_msdf_loc_atlas;
static GLint  s_msdf_loc_px_range;
static GLuint s_msdf_atlas_tex;

/* MSDF frame buffer: accumulates quads across multiple msdf_text_layout calls */
#define MSDF_FRAME_MAX_QUADS 512
static float    s_msdf_verts[MSDF_FRAME_MAX_QUADS * 4 * GLYPH_VERTEX_STRIDE];
static unsigned int s_msdf_indices[MSDF_FRAME_MAX_QUADS * 6];
static int      s_msdf_vert_count;
static int      s_msdf_idx_count;

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
    "    float alpha = smoothstep(0.25, 0.75, a);\n"
    "    if (alpha < 0.01) discard;\n"
    "    frag_color = vec4(v_color.rgb, v_color.a * alpha);\n"
    "}\n";

/* --- MSDF shader: 3-channel median distance for crispy text --- */

static const char *s_msdf_frag_source =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "in vec4 v_color;\n"
    "uniform sampler2D u_atlas;\n"
    "uniform float u_px_range;\n"
    "out vec4 frag_color;\n"
    "float median(float r, float g, float b) {\n"
    "    return max(min(r, g), min(max(r, g), b));\n"
    "}\n"
    "void main() {\n"
    "    vec3 msd = texture(u_atlas, v_uv).rgb;\n"
    "    float sd = median(msd.r, msd.g, msd.b);\n"
    "    vec2 sz = vec2(textureSize(u_atlas, 0));\n"
    "    vec2 dx = dFdx(v_uv) * sz;\n"
    "    vec2 dy = dFdy(v_uv) * sz;\n"
    "    float to_px = u_px_range * inversesqrt(dot(dx,dx) + dot(dy,dy));\n"
    "    float opacity = clamp((sd - 0.5) * to_px + 0.5, 0.0, 1.0);\n"
    "    if (opacity < 0.01) discard;\n"
    "    frag_color = vec4(v_color.rgb, v_color.a * opacity);\n"
    "}\n";

/* --- MSDF frame helpers --- */

static void msdf_begin(void) {
    s_msdf_vert_count = 0;
    s_msdf_idx_count = 0;
}

static void msdf_add_text(const char *text, float x, float y,
                           float font_size, float r, float g, float b, float a)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, text, x, y, font_size);
    for (int i = 0; i < layout.count && s_msdf_vert_count < MSDF_FRAME_MAX_QUADS * 4; i++) {
        const msdf_quad_t *q = &layout.quads[i];
        unsigned int base = (unsigned int)s_msdf_vert_count;
        float *v = &s_msdf_verts[s_msdf_vert_count * GLYPH_VERTEX_STRIDE];

        /* v0: top-left */
        v[0]=q->x;      v[1]=q->y;      v[2]=0; v[3]=q->u0; v[4]=q->v0;
        v[5]=r; v[6]=g; v[7]=b; v[8]=a;
        v += GLYPH_VERTEX_STRIDE;
        /* v1: top-right */
        v[0]=q->x+q->w; v[1]=q->y;      v[2]=0; v[3]=q->u1; v[4]=q->v0;
        v[5]=r; v[6]=g; v[7]=b; v[8]=a;
        v += GLYPH_VERTEX_STRIDE;
        /* v2: bottom-right */
        v[0]=q->x+q->w; v[1]=q->y+q->h; v[2]=0; v[3]=q->u1; v[4]=q->v1;
        v[5]=r; v[6]=g; v[7]=b; v[8]=a;
        v += GLYPH_VERTEX_STRIDE;
        /* v3: bottom-left */
        v[0]=q->x;      v[1]=q->y+q->h; v[2]=0; v[3]=q->u0; v[4]=q->v1;
        v[5]=r; v[6]=g; v[7]=b; v[8]=a;

        unsigned int *idx = &s_msdf_indices[s_msdf_idx_count];
        idx[0]=base; idx[1]=base+1; idx[2]=base+2;
        idx[3]=base; idx[4]=base+2; idx[5]=base+3;

        s_msdf_vert_count += 4;
        s_msdf_idx_count += 6;
    }
}

static void msdf_flush(int vw, int vh) {
    if (s_msdf_idx_count == 0) return;

    /* Orthographic projection: pixel coords -> NDC */
    mat4_t ortho;
    memset(&ortho, 0, sizeof(ortho));
    ortho.m[0]  =  2.0f / (float)vw;
    ortho.m[5]  = -2.0f / (float)vh;
    ortho.m[10] = -1.0f;
    ortho.m[12] = -1.0f;
    ortho.m[13] =  1.0f;
    ortho.m[14] =  0.0f;
    ortho.m[15] =  1.0f;

    glBindVertexArray(s_text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_text_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    s_msdf_vert_count * GLYPH_VERTEX_STRIDE * (GLsizeiptr)sizeof(float),
                    s_msdf_verts);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_text_ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                    s_msdf_idx_count * (GLsizeiptr)sizeof(unsigned int),
                    s_msdf_indices);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(s_msdf_program);
    glUniformMatrix4fv(s_msdf_loc_mvp, 1, GL_FALSE, ortho.m);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_msdf_atlas_tex);
    glUniform1i(s_msdf_loc_atlas, 0);
    glUniform1f(s_msdf_loc_px_range, 2.0f);

    glDrawElements(GL_TRIANGLES, s_msdf_idx_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

    /* --- MSDF shader + atlas for 2D card/oracle text --- */
    s_msdf_program = shader_create_program(s_text_vert_source, s_msdf_frag_source);
    if (s_msdf_program == 0) {
        printf("Failed to create MSDF text shader\n");
        return 1;
    }
    s_msdf_loc_mvp      = glGetUniformLocation(s_msdf_program, "u_mvp");
    s_msdf_loc_atlas    = glGetUniformLocation(s_msdf_program, "u_atlas");
    s_msdf_loc_px_range = glGetUniformLocation(s_msdf_program, "u_px_range");

    /* Upload MSDF atlas as RGB8 texture */
    int msdf_w = msdf_text_atlas_width(MSDF_FONT_MONO);
    int msdf_h = msdf_text_atlas_height(MSDF_FONT_MONO);
    const unsigned char *msdf_pixels = msdf_text_atlas_pixels(MSDF_FONT_MONO);

    glGenTextures(1, &s_msdf_atlas_tex);
    glBindTexture(GL_TEXTURE_2D, s_msdf_atlas_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                 msdf_w, msdf_h, 0, GL_RGB, GL_UNSIGNED_BYTE, msdf_pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    printf("MSDF: %dx%d atlas, %d glyphs, shader compiled\n",
           msdf_w, msdf_h, msdf_text_glyph_count(MSDF_FONT_MONO));

    return 0;
}

/* --- Card text helpers --- */

/* Compute approximate sun ecliptic longitude from JD (Meeus low-accuracy). */
static double approx_sun_longitude(double jd)
{
    double T = (jd - 2451545.0) / 36525.0;
    double lon = 280.46646 + 36000.76983 * T;
    lon = lon - 360.0 * (int)(lon / 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

/* Draw Kin Maya oracle cross text — MSDF rendering.
 * Position names, seal+tone, kin numbers, wavespell strip, headline. */
static void draw_oracle_text(const render_frame_t *frame)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2];
    int vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    tzolkin_day_t today = tzolkin_from_jd(frame->simulation_jd);
    kin_oracle_layout_t oracle = kin_oracle_compute(today.kin);

    theme_t th = theme_get((theme_id_t)frame->theme_id);
    float title_fs = 24.0f;
    float body_fs  = 18.0f;
    float margin_x = 8.0f;
    float margin_y = 10.0f;
    float line_h   = 22.0f;

    msdf_begin();

    for (int i = 0; i < KIN_ORACLE_POSITIONS; i++) {
        kin_cell_t cell = kin_oracle_cell_at(&oracle, i);
        float card_x = (cell.x - cell.w * 0.5f) * (float)vw + margin_x;
        float card_y = (cell.y - cell.h * 0.5f) * (float)vh + margin_y;

        /* Title: position name in directional color */
        float rgba[4];
        kin_cell_rgba(cell.color, rgba);
        msdf_add_text(kin_oracle_position_name(i),
                      card_x, card_y, title_fs, rgba[0], rgba[1], rgba[2], 1.0f);

        /* Line 1: Color + Seal name */
        card_y += line_h + 4.0f;
        dreamspell_color_t dc = dreamspell_color(cell.seal);
        char seal_line[64];
        snprintf(seal_line, sizeof(seal_line), "%s %s",
                 dc.name, tzolkin_seal_name(cell.seal));
        msdf_add_text(seal_line, card_x, card_y, body_fs,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);

        /* Line 2: Tone name + number */
        card_y += line_h;
        dreamspell_tone_t dt = dreamspell_tone(cell.tone);
        char tone_line[64];
        snprintf(tone_line, sizeof(tone_line), "%s %d", dt.name, cell.tone);
        msdf_add_text(tone_line, card_x, card_y, body_fs,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);

        /* Line 3: Kin number */
        card_y += line_h;
        char kin_line[32];
        snprintf(kin_line, sizeof(kin_line), "Kin %d", cell.kin);
        msdf_add_text(kin_line, card_x, card_y, body_fs,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, th.text_secondary.a);
    }

    /* --- Wavespell strip text --- */
    {
        kin_wavespell_layout_t ws = kin_wavespell_compute(today.kin);
        float ws_y_center = 0.675f;
        float ws_h = 0.08f;
        float ws_fs = 14.0f;

        char ws_title[64];
        snprintf(ws_title, sizeof(ws_title), "Wavespell %d: %s",
                 ws.wavespell_number, ws.purpose ? ws.purpose : "");
        float ws_title_y = (ws_y_center - ws_h * 0.5f) * (float)vh - 18.0f;
        msdf_add_text(ws_title, 0.04f * (float)vw, ws_title_y, body_fs,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);

        for (int i = 0; i < KIN_WS_CELLS; i++) {
            kin_cell_t cell = ws.cells[i];
            float cx = cell.x * (float)vw - 4.0f;
            float cy = ws_y_center * (float)vh - 5.0f;
            char tone_str[4];
            snprintf(tone_str, sizeof(tone_str), "%d", cell.tone);
            float rgba[4];
            kin_cell_rgba(cell.color, rgba);
            float a = cell.highlighted ? 1.0f : 0.6f;
            msdf_add_text(tone_str, cx, cy, ws_fs, rgba[0], rgba[1], rgba[2], a);
        }
    }

    /* Headline at bottom center */
    if (frame->headline[0] != '\0') {
        float hl_fs = 20.0f;
        float hl_w = msdf_text_width(MSDF_FONT_MONO, frame->headline, hl_fs);
        float hl_x = ((float)vw - hl_w) * 0.5f;
        float hl_y = (float)vh - 40.0f;
        msdf_add_text(frame->headline, hl_x, hl_y, hl_fs,
                      th.brand_secondary.r, th.brand_secondary.g,
                      th.brand_secondary.b, 0.85f);
    }

    msdf_flush(vw, vh);
}

/* Draw I Ching hexagram text — MSDF rendering.
 * Name, number, trigrams, keywords, judgment. */
static void draw_iching_text(const render_frame_t *frame)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2], vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    hexagram_t hex = iching_from_jd(frame->simulation_jd);
    hexagram_layout_t hl = hexagram_layout_compute(hex.king_wen);
    theme_t th = theme_get((theme_id_t)frame->theme_id);

    msdf_begin();

    /* Hexagram number + name (positioned from layout) */
    char num_str[16];
    snprintf(num_str, sizeof(num_str), "Hexagram %d", hl.king_wen);
    msdf_add_text(num_str, hl.number_x * (float)vw, hl.number_y * (float)vh, 20.0f,
                  th.text_secondary.r, th.text_secondary.g,
                  th.text_secondary.b, th.text_secondary.a);

    if (hl.name)
        msdf_add_text(hl.name, hl.name_x * (float)vw, hl.name_y * (float)vh, 28.0f,
                      th.brand_primary.r, th.brand_primary.g,
                      th.brand_primary.b, 1.0f);

    /* Trigram labels */
    if (hl.upper_name) {
        char upper[48];
        snprintf(upper, sizeof(upper), "Upper: %s", hl.upper_name);
        msdf_add_text(upper, hl.upper_label_x * (float)vw,
                      hl.upper_label_y * (float)vh, 16.0f,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);
    }
    if (hl.lower_name) {
        char lower[48];
        snprintf(lower, sizeof(lower), "Lower: %s", hl.lower_name);
        msdf_add_text(lower, hl.lower_label_x * (float)vw,
                      hl.lower_label_y * (float)vh, 16.0f,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);
    }

    /* Keywords on info card (right side) */
    float info_x = 0.57f * (float)vw;
    float info_y = 0.15f * (float)vh;
    if (hl.keywords)
        msdf_add_text(hl.keywords, info_x, info_y, 18.0f,
                      th.brand_secondary.r, th.brand_secondary.g,
                      th.brand_secondary.b, 0.9f);

    /* Judgment text (wrapped) */
    if (hl.judgment) {
        msdf_add_text("Judgment:", info_x, info_y + 30.0f, 16.0f,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, th.text_secondary.a);
        msdf_add_text(hl.judgment, info_x, info_y + 52.0f, 14.0f,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);
    }

    /* Image text */
    if (hl.image) {
        msdf_add_text("Image:", info_x, info_y + 100.0f, 16.0f,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, th.text_secondary.a);
        msdf_add_text(hl.image, info_x, info_y + 122.0f, 14.0f,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);
    }

    /* Line labels (yang/yin per position) */
    for (int i = 0; i < 6; i++) {
        hex_line_t line = hl.lines[i];
        char line_label[8];
        snprintf(line_label, sizeof(line_label), "%d", i + 1);
        float lx = (line.x - 0.03f) * (float)vw;
        float ly = (line.y + line.h * 0.3f) * (float)vh;
        msdf_add_text(line_label, lx, ly, 14.0f,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, 0.6f);
    }

    /* Bagua trigrams — upper and lower with element/direction */
    {
        bagua_layout_t bagua = bagua_layout_compute(hl.upper_trigram);
        float by = info_y + 160.0f;
        msdf_add_text("Trigrams:", info_x, by, 16.0f,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, th.text_secondary.a);
        for (int t = 0; t < 2; t++) {
            int tri = (t == 0) ? hl.upper_trigram : hl.lower_trigram;
            bagua_trigram_t bt = bagua_trigram_at(&bagua, -1);
            /* Find the trigram by index */
            for (int k = 0; k < BAGUA_TRIGRAM_COUNT; k++) {
                if (bagua.trigrams[k].trigram_index == tri) {
                    bt = bagua.trigrams[k];
                    break;
                }
            }
            char tri_line[64];
            snprintf(tri_line, sizeof(tri_line), "%s: %s (%s, %s)",
                     (t == 0) ? "Upper" : "Lower",
                     bt.name ? bt.name : "?",
                     bt.element ? bt.element : "?",
                     bt.direction ? bt.direction : "?");
            by += 20.0f;
            msdf_add_text(tri_line, info_x, by, 14.0f,
                          th.text_primary.r, th.text_primary.g,
                          th.text_primary.b, th.text_primary.a);
        }
    }

    /* Headline */
    if (frame->headline[0] != '\0') {
        float hl_w = msdf_text_width(MSDF_FONT_MONO, frame->headline, 20.0f);
        msdf_add_text(frame->headline,
                      ((float)vw - hl_w) * 0.5f, (float)vh - 40.0f, 20.0f,
                      th.brand_secondary.r, th.brand_secondary.g,
                      th.brand_secondary.b, 0.85f);
    }

    msdf_flush(vw, vh);
}

/* Draw Human Design focus text — Sun/Earth gate info via MSDF. */
static void draw_hd_text(const render_frame_t *frame)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2], vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    double T = (frame->simulation_jd - 2451545.0) / 36525.0;
    double sun_lon = 280.46646 + 36000.76983 * T;
    sun_lon = sun_lon - 360.0 * (int)(sun_lon / 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    daily_hd_layout_t dl = daily_hd_compute(sun_lon);
    theme_t th = theme_get((theme_id_t)frame->theme_id);

    msdf_begin();

    /* Title */
    msdf_add_text("Human Design", 0.30f * (float)vw, 0.12f * (float)vh, 28.0f,
                  th.brand_primary.r, th.brand_primary.g,
                  th.brand_primary.b, 1.0f);

    /* Sun gate card (left) */
    float sx = 0.08f * (float)vw, sy = 0.28f * (float)vh;
    msdf_add_text("Sun Gate", sx, sy, 20.0f,
                  th.brand_primary.r, th.brand_primary.g,
                  th.brand_primary.b, 0.9f);

    char sun_info[64];
    snprintf(sun_info, sizeof(sun_info), "Gate %d.%d",
             dl.sun_gate.gate, dl.sun_gate.line);
    msdf_add_text(sun_info, sx, sy + 28.0f, 24.0f,
                  th.text_primary.r, th.text_primary.g,
                  th.text_primary.b, th.text_primary.a);

    if (dl.sun_gate.gate_name)
        msdf_add_text(dl.sun_gate.gate_name, sx, sy + 58.0f, 18.0f,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);
    if (dl.sun_gate.keyword)
        msdf_add_text(dl.sun_gate.keyword, sx, sy + 82.0f, 16.0f,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, th.text_secondary.a);

    /* Earth gate card (right) */
    float ex = 0.58f * (float)vw, ey = 0.28f * (float)vh;
    msdf_add_text("Earth Gate", ex, ey, 20.0f,
                  th.brand_secondary.r, th.brand_secondary.g,
                  th.brand_secondary.b, 0.9f);

    char earth_info[64];
    snprintf(earth_info, sizeof(earth_info), "Gate %d.%d",
             dl.earth_gate.gate, dl.earth_gate.line);
    msdf_add_text(earth_info, ex, ey + 28.0f, 24.0f,
                  th.text_primary.r, th.text_primary.g,
                  th.text_primary.b, th.text_primary.a);

    if (dl.earth_gate.gate_name)
        msdf_add_text(dl.earth_gate.gate_name, ex, ey + 58.0f, 18.0f,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, th.text_primary.a);
    if (dl.earth_gate.keyword)
        msdf_add_text(dl.earth_gate.keyword, ex, ey + 82.0f, 16.0f,
                      th.text_secondary.r, th.text_secondary.g,
                      th.text_secondary.b, th.text_secondary.a);

    /* Headline */
    if (frame->headline[0] != '\0') {
        float hl_w = msdf_text_width(MSDF_FONT_MONO, frame->headline, 20.0f);
        msdf_add_text(frame->headline,
                      ((float)vw - hl_w) * 0.5f, (float)vh - 40.0f, 20.0f,
                      th.brand_secondary.r, th.brand_secondary.g,
                      th.brand_secondary.b, 0.85f);
    }

    msdf_flush(vw, vh);
}

/* Draw Astrology zodiac wheel text — 12 sign names + Sun position via MSDF. */
static void draw_astrology_text(const render_frame_t *frame)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2], vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    double T = (frame->simulation_jd - 2451545.0) / 36525.0;
    double sun_lon = 280.46646 + 36000.76983 * T;
    sun_lon = sun_lon - 360.0 * (int)(sun_lon / 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    zodiac_wheel_layout_t wl = zodiac_wheel_compute(sun_lon);
    theme_t th = theme_get((theme_id_t)frame->theme_id);

    msdf_begin();

    /* Title */
    msdf_add_text("Zodiac Wheel", 0.35f * (float)vw, 0.07f * (float)vh, 26.0f,
                  th.brand_primary.r, th.brand_primary.g,
                  th.brand_primary.b, 1.0f);

    /* 12 sign labels at wheel positions */
    for (int i = 0; i < 12; i++) {
        zw_sign_t s = wl.signs[i];
        float lx = s.label_x * (float)vw;
        float ly = s.label_y * (float)vh;
        float alpha = (s.sign == wl.sun_sign) ? 1.0f : 0.7f;
        float fs = (s.sign == wl.sun_sign) ? 18.0f : 14.0f;
        msdf_add_text(s.name, lx - 20.0f, ly - 6.0f, fs,
                      th.text_primary.r, th.text_primary.g,
                      th.text_primary.b, alpha);
    }

    /* Sun position label */
    char sun_label[48];
    snprintf(sun_label, sizeof(sun_label), "Sun: %.1f deg", wl.sun_degree);
    msdf_add_text(sun_label, wl.sun_x * (float)vw - 30.0f,
                  wl.sun_y * (float)vh + 14.0f, 14.0f,
                  th.brand_primary.r, th.brand_primary.g,
                  th.brand_primary.b, 0.9f);

    /* Dignity summary for Sun's current sign */
    {
        dignity_table_layout_t dt = dignity_table_compute();
        /* Find Sun row (planet index 8 in dignity convention) */
        for (int r = 0; r < DT_PLANET_COUNT; r++) {
            if (dt.rows[r].planet == 8) { /* Sun */
                char dig[64];
                if (dt.rows[r].domicile_sign == wl.sun_sign)
                    snprintf(dig, sizeof(dig), "Sun in domicile (rules this sign)");
                else if (dt.rows[r].exaltation_sign == wl.sun_sign)
                    snprintf(dig, sizeof(dig), "Sun exalted in this sign");
                else if (dt.rows[r].detriment_sign == wl.sun_sign)
                    snprintf(dig, sizeof(dig), "Sun in detriment");
                else if (dt.rows[r].fall_sign == wl.sun_sign)
                    snprintf(dig, sizeof(dig), "Sun in fall");
                else
                    snprintf(dig, sizeof(dig), "Sun: peregrine");
                msdf_add_text(dig, 0.08f * (float)vw, 0.92f * (float)vh, 14.0f,
                              th.text_secondary.r, th.text_secondary.g,
                              th.text_secondary.b, 0.7f);
                break;
            }
        }
    }

    /* Transit aspects summary (consumes daily_transit_layout) */
    {
        double lons[10];
        for (int i = 0; i < 8; i++) lons[i] = frame->planet_lon[i];
        lons[8] = sun_lon;
        lunar_info_t moon_info = lunar_phase(frame->simulation_jd);
        lons[9] = moon_info.moon_longitude;
        daily_transit_layout_t dtl = daily_transit_compute(lons, 8.0);
        if (dtl.aspect_count > 0) {
            char asp[48];
            snprintf(asp, sizeof(asp), "%d active aspects", dtl.aspect_count);
            msdf_add_text(asp, 0.08f * (float)vw, 0.88f * (float)vh, 14.0f,
                          th.text_secondary.r, th.text_secondary.g,
                          th.text_secondary.b, 0.6f);
        }
    }

    /* Headline */
    if (frame->headline[0] != '\0') {
        float hl_w = msdf_text_width(MSDF_FONT_MONO, frame->headline, 20.0f);
        msdf_add_text(frame->headline,
                      ((float)vw - hl_w) * 0.5f, (float)vh - 40.0f, 20.0f,
                      th.brand_secondary.r, th.brand_secondary.g,
                      th.brand_secondary.b, 0.85f);
    }

    msdf_flush(vw, vh);
}

/* Draw Chinese zodiac animals text — 12 animal names in circle via MSDF. */
static void draw_chinese_text(const render_frame_t *frame)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2], vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    zodiac_animals_layout_t zal = zodiac_animals_compute(frame->simulation_jd);
    theme_t th = theme_get((theme_id_t)frame->theme_id);

    msdf_begin();

    /* Title */
    char title[64];
    snprintf(title, sizeof(title), "Chinese Zodiac — %s %s",
             zal.current_element_name ? zal.current_element_name : "",
             zal.animals[zal.current_animal].name ? zal.animals[zal.current_animal].name : "");
    msdf_add_text(title, 0.22f * (float)vw, 0.07f * (float)vh, 24.0f,
                  th.brand_primary.r, th.brand_primary.g,
                  th.brand_primary.b, 1.0f);

    /* 12 animal labels at circle positions */
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        za_animal_t a = zal.animals[i];
        float lx = a.x * (float)vw;
        float ly = a.y * (float)vh;
        float alpha = a.highlighted ? 1.0f : 0.6f;
        float fs = a.highlighted ? 20.0f : 14.0f;
        if (a.name)
            msdf_add_text(a.name, lx - 16.0f, ly - 6.0f, fs,
                          th.text_primary.r, th.text_primary.g,
                          th.text_primary.b, alpha);
    }

    /* Headline */
    if (frame->headline[0] != '\0') {
        float hl_w = msdf_text_width(MSDF_FONT_MONO, frame->headline, 20.0f);
        msdf_add_text(frame->headline,
                      ((float)vw - hl_w) * 0.5f, (float)vh - 40.0f, 20.0f,
                      th.brand_secondary.r, th.brand_secondary.g,
                      th.brand_secondary.b, 0.85f);
    }

    msdf_flush(vw, vh);
}

/* Draw card text as a 2D screen-space overlay.
 * Called after the 3D planet label draw; reuses same VAO/VBO/EBO. */
static void draw_card_text(const render_frame_t *frame)
{
    if (!layer_is_visible(frame->layers, LAYER_CARDS))
        return;

    /* System-specific focus mode overlays */
    if (frame->focus_mode == FOCUS_MODE_KIN) {
        draw_oracle_text(frame);
        return;
    }
    if (frame->focus_mode == FOCUS_MODE_ICHING) {
        draw_iching_text(frame);
        return;
    }
    if (frame->focus_mode == FOCUS_MODE_HD) {
        draw_hd_text(frame);
        return;
    }
    if (frame->focus_mode == FOCUS_MODE_ASTROLOGY) {
        draw_astrology_text(frame);
        return;
    }
    if (frame->focus_mode == FOCUS_MODE_CHINESE) {
        draw_chinese_text(frame);
        return;
    }

    /* Get viewport dimensions */
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int vw = viewport[2];
    int vh = viewport[3];
    if (vw < 1) vw = 1920;
    if (vh < 1) vh = 1080;

    /* Compute card layout */
    float aspect = (float)vw / (float)vh;
    card_layout_t layout = card_layout_compute(card_all_mask(), aspect);

    double jd = frame->simulation_jd;
    double sun_lon = approx_sun_longitude(jd);
    lunar_info_t moon = lunar_phase(jd);
    double moon_lon = moon.moon_longitude;

    cs_selection_t sel = cs_select(frame->log_zoom, aspect);
    int focus_sys = card_style_focus_system(frame->focus_mode);
    if (focus_sys >= 0) {
        sel.filled_count = 1;
        sel.slots[0].system_id = focus_sys;
        sel.slots[0].opacity = frame->card_slide;
    }
    /* Per-word text reveal: multiply all card text alpha during focus transitions */
    float text_alpha = frame->card_text_reveal;

    card_content_t card_contents[CARD_TYPE_COUNT];
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        int sys_id = (i < sel.filled_count) ? sel.slots[i].system_id : -1;
        card_contents[i] = (sys_id >= 0)
            ? today_card_for_system(sys_id, jd, sun_lon, moon_lon,
                                       frame->observer_lat,
                                       frame->observer_lon)
            : (card_content_t){0};
        layout.cards[i].visible = (sys_id >= 0);
        layout.cards[i].opacity = (i < sel.filled_count) ? sel.slots[i].opacity : 0.0f;
    }

    /* MSDF text rendering — Krug/Refactoring UI tuned sizes */
    float title_fs  = 21.0f;
    float body_fs   = 14.0f;
    float detail_fs = 11.0f;
    float margin_x  = 13.0f;
    float margin_y  = 13.0f;
    float line_h    = 21.0f;
    float title_gap = 8.0f;  /* fibonacci sp-3 between title and body */

    msdf_begin();

    /* "Time" wordmark — top-left, subtle brand presence */
    {
        theme_cosmos_t wm = theme_cosmos_constant();
        msdf_add_text("Time", 16.0f, 20.0f, 11.0f,
                      wm.brand_primary.r, wm.brand_primary.g,
                      wm.brand_primary.b, 0.38f);
        /* View mode indicator below wordmark */
        const char *view_label = (frame->focus_mode == 0)
            ? "Space View" : "Focus View";
        msdf_add_text(view_label, 16.0f, 37.0f, 9.0f,
                      wm.brand_secondary.r, wm.brand_secondary.g,
                      wm.brand_secondary.b, 0.30f);
    }

    for (int c = 0; c < CARD_TYPE_COUNT; c++) {
        const card_rect_t *r = &layout.cards[c];
        if (!r->visible) continue;

        const card_content_t *content = &card_contents[c];
        int sys_id = (c < sel.filled_count) ? sel.slots[c].system_id : -1;
        card_style_t style = (sys_id >= 0)
            ? card_style_for_system(sys_id, r->opacity, (theme_id_t)frame->theme_id)
            : card_style_default(r->opacity, (theme_id_t)frame->theme_id);

        float px = r->x * (float)vw + margin_x;
        float py = r->y * (float)vh + margin_y;

        /* Title — alpha modulated by text reveal during focus transitions */
        float ta = text_alpha * r->opacity;
        msdf_add_text(content->title, px, py, title_fs,
                      style.title.r, style.title.g, style.title.b, style.title.a * ta);

        /* Focus key hint — top-right of card, subtle */
        {
            const char *key = NULL;
            if (sys_id == TS_SYS_ASTROLOGY)     key = "A";
            else if (sys_id == TS_SYS_TZOLKIN)   key = "K";
            else if (sys_id == TS_SYS_ICHING)    key = "I";
            else if (sys_id == TS_SYS_CHINESE)   key = "C";
            else if (sys_id == TS_SYS_HUMAN_DESIGN) key = "D";
            else if (sys_id == TS_SYS_KABBALAH)  key = "T";
            if (key) {
                float kx = (r->x + r->w * 0.5f) * (float)vw - margin_x - 8.0f;
                msdf_add_text(key, kx, py, 11.0f,
                              style.muted.r, style.muted.g, style.muted.b, 0.30f * ta);
            }
        }

        /* Line 1 */
        py += title_gap + title_fs;
        msdf_add_text(content->line1, px, py, body_fs,
                      style.body.r, style.body.g, style.body.b, style.body.a * ta);

        /* Line 2 */
        py += line_h;
        msdf_add_text(content->line2, px, py, body_fs,
                      style.body.r, style.body.g, style.body.b, style.body.a * ta);

        /* Line 3 (detail — smaller, muted) */
        if (content->line3[0] != '\0') {
            py += line_h;
            msdf_add_text(content->line3, px, py, detail_fs,
                          style.muted.r, style.muted.g, style.muted.b, style.muted.a * ta);
        }
    }

    /* Earth timeline — geological era labels when in Earth View */
    if (frame->view_id == 1) {
        etl_layout_t tl = etl_compute(0.0f, 500.0f);
        theme_cosmos_t tc = theme_cosmos_constant();
        for (int t = 0; t < tl.count; t++) {
            const etl_card_t *ec = &tl.cards[t];
            if (!ec->title || ec->opacity < 0.05f) continue;
            float tx = ec->x * (float)vw;
            float ty = tl.timeline_y * (float)vh;
            float fs = 10.0f + ec->drama * 2.0f;
            msdf_add_text(ec->title, tx, ty, fs,
                          tc.brand_primary.r, tc.brand_primary.g,
                          tc.brand_primary.b, ec->opacity * 0.6f);
        }
    }

    /* Bottom text stack: headline → percentile → wisdom → author
     * Build from bottom up to avoid overlap. */
    {
        theme_cosmos_t btc = theme_cosmos_constant();
        float bot_y = (float)vh;  /* current bottom cursor */

        /* Author attribution — absolute bottom */
        if (frame->author_line[0] != '\0') {
            bot_y -= 12.0f;
            float al = msdf_text_width(MSDF_FONT_MONO, frame->author_line, 8.0f);
            msdf_add_text(frame->author_line,
                          ((float)vw - al) * 0.5f, bot_y, 8.0f,
                          btc.brand_primary.r, btc.brand_primary.g,
                          btc.brand_primary.b, 0.20f);
        }

        /* Wisdom author */
        if (frame->wisdom_text[0] != '\0' && frame->wisdom_author[0] != '\0') {
            bot_y -= 14.0f;
            float aw = msdf_text_width(MSDF_FONT_MONO, frame->wisdom_author, 11.0f);
            msdf_add_text(frame->wisdom_author,
                          ((float)vw - aw) * 0.5f, bot_y, 11.0f,
                          btc.brand_primary.r, btc.brand_primary.g,
                          btc.brand_primary.b, 0.35f);
        }

        /* Wisdom quote */
        if (frame->wisdom_text[0] != '\0') {
            bot_y -= 16.0f;
            float ww = msdf_text_width(MSDF_FONT_MONO, frame->wisdom_text, 13.0f);
            msdf_add_text(frame->wisdom_text,
                          ((float)vw - ww) * 0.5f, bot_y, 13.0f,
                          btc.brand_primary.r, btc.brand_primary.g,
                          btc.brand_primary.b, 0.5f);
        }

        /* Time entropy — "Time entropy: 0.87 — complex (12/21 active)" */
        if (frame->entropy_text[0] != '\0') {
            bot_y -= 14.0f;
            float ew = msdf_text_width(MSDF_FONT_MONO, frame->entropy_text, 10.0f);
            msdf_add_text(frame->entropy_text,
                          ((float)vw - ew) * 0.5f, bot_y, 10.0f,
                          btc.brand_secondary.r, btc.brand_secondary.g,
                          btc.brand_secondary.b, 0.30f);
        }

        /* Top resonance — "Strongest resonance: Hebrew ↔ Islamic (42%)" */
        if (frame->top_resonance[0] != '\0') {
            bot_y -= 14.0f;
            float rw = msdf_text_width(MSDF_FONT_MONO, frame->top_resonance, 10.0f);
            msdf_add_text(frame->top_resonance,
                          ((float)vw - rw) * 0.5f, bot_y, 10.0f,
                          btc.brand_primary.r, btc.brand_primary.g,
                          btc.brand_primary.b, 0.30f);
        }

        /* Percentile ranking — "Day #47 of 365 — top 12.9%" */
        if (frame->percentile_text[0] != '\0') {
            bot_y -= 16.0f;
            float pw = msdf_text_width(MSDF_FONT_MONO, frame->percentile_text, 11.0f);
            msdf_add_text(frame->percentile_text,
                          ((float)vw - pw) * 0.5f, bot_y, 11.0f,
                          btc.brand_secondary.r, btc.brand_secondary.g,
                          btc.brand_secondary.b, 0.45f);
        }

        /* Last similar — "Last similar convergence: X days ago" */
        if (frame->last_similar[0] != '\0') {
            bot_y -= 14.0f;
            float lw = msdf_text_width(MSDF_FONT_MONO, frame->last_similar, 10.0f);
            msdf_add_text(frame->last_similar,
                          ((float)vw - lw) * 0.5f, bot_y, 10.0f,
                          btc.brand_secondary.r, btc.brand_secondary.g,
                          btc.brand_secondary.b, 0.35f);
        }

        /* Red thread — convergence narrative (only when systems align) */
        if (frame->red_thread[0] != '\0') {
            bot_y -= 16.0f;
            float rw = msdf_text_width(MSDF_FONT_MONO, frame->red_thread, 11.0f);
            float rx = ((float)vw - rw) * 0.5f;
            if (rx < 13.0f) rx = 13.0f;  /* left margin if too wide */
            msdf_add_text(frame->red_thread,
                          rx, bot_y, 11.0f,
                          btc.brand_primary.r, btc.brand_primary.g,
                          btc.brand_primary.b, 0.40f);
        }

        /* Headline — top of bottom stack */
        if (frame->headline[0] != '\0') {
            bot_y -= 24.0f;
            float hl_w = msdf_text_width(MSDF_FONT_MONO, frame->headline, 20.0f);
            msdf_add_text(frame->headline,
                          ((float)vw - hl_w) * 0.5f, bot_y, 20.0f,
                          btc.brand_secondary.r, btc.brand_secondary.g,
                          btc.brand_secondary.b, 0.85f);
        }

        /* Grand cycle — cosmic perspective, very subtle */
        if (frame->grand_cycle[0] != '\0') {
            bot_y -= 14.0f;
            float gw = msdf_text_width(MSDF_FONT_MONO, frame->grand_cycle, 9.0f);
            msdf_add_text(frame->grand_cycle,
                          ((float)vw - gw) * 0.5f, bot_y, 9.0f,
                          btc.brand_secondary.r, btc.brand_secondary.g,
                          btc.brand_secondary.b, 0.25f);
        }
    }

    msdf_flush(vw, vh);
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

    /* Zoom-based label opacity: smooth fade at zoom boundaries */
    float label_alpha = zf_opacity(ZF_TEXT_LABEL, frame->log_zoom);
    if (label_alpha < 0.01f) {
        /* Labels invisible at this zoom — skip 3D labels, still draw card text */
        draw_card_text(frame);
        return;
    }

    /* Cosmos-scope label colors — astronomical labels never change with theme.
     * Sun = solar gold (brand_primary), planets = warm off-white. */
    theme_cosmos_t cosmos = theme_cosmos_constant();
    glyph_color_t sun_label_color = {cosmos.brand_primary.r,
                                     cosmos.brand_primary.g,
                                     cosmos.brand_primary.b, label_alpha};
    glyph_color_t planet_label_color = {cosmos.planet_label.r,
                                        cosmos.planet_label.g,
                                        cosmos.planet_label.b,
                                        cosmos.planet_label.a * label_alpha};

    /* Get camera basis vectors FIRST — needed for text placement */
    vec3_t cam_right, cam_up;
    billboard_camera_vectors(&frame->view, &cam_right, &cam_up);

    /* Zoom-adaptive label sizing — labels maintain readable screen size
     * across all zoom levels. Camera distance = exp(log_zoom).
     * At default zoom (log_zoom~3.2, dist~25): scale factor = 1.0.
     * Close-up: labels shrink. Galaxy view: labels grow proportionally. */
    float cam_dist = expf(frame->log_zoom);
    float zoom_scale = cam_dist / 25.0f;
    if (zoom_scale < 0.15f) zoom_scale = 0.15f;
    if (zoom_scale > 4.0f) zoom_scale = 4.0f;

    /* Sun label at origin — place chars along cam_right so text always faces camera */
    {
        static const char *sun_name = "Sun";
        int slen = (int)strlen(sun_name);
        float spacing = 0.2f * zoom_scale;
        float sx = -spacing * (float)(slen - 1) * 0.5f;
        float label_y = 0.6f * zoom_scale;
        vec3_t base = vec3_create(0.0f, label_y, 0.0f);
        for (int i = 0; i < slen && len < GLYPH_BATCH_MAX; i++) {
            vec3_t offset = vec3_scale(cam_right, sx + (float)i * spacing);
            instances[len].glyph_id = (int)sun_name[i];
            instances[len].position = vec3_add(base, offset);
            instances[len].scale    = 0.35f * zoom_scale;
            instances[len].color    = sun_label_color;
            len++;
        }
    }

    /* Planet labels at orbital positions — placed along cam_right */
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
        ly += 0.3f * zoom_scale;

        /* Place chars along cam_right so text reads correctly from any angle */
        vec3_t base = vec3_create(lx, ly, lz);
        float spacing = 0.1f * zoom_scale;
        float sx = -spacing * (float)(nlen - 1) * 0.5f;
        for (int i = 0; i < nlen && len < GLYPH_BATCH_MAX; i++) {
            vec3_t offset = vec3_scale(cam_right, sx + (float)i * spacing);
            instances[len].glyph_id = (int)name[i];
            instances[len].position = vec3_add(base, offset);
            instances[len].scale    = 0.3f * zoom_scale;
            instances[len].color    = planet_label_color;
            len++;
        }
    }

    /* Constellation name labels — 88 IAU names on celestial sphere */
    {
        float const_alpha = zf_opacity(ZF_CONSTELLATION, frame->log_zoom);
        if (const_alpha > 0.01f) {
            cl_label_t labels[CL_COUNT];
            int nlabels = cl_compute(100.0f, labels);
            /* Dimmer than planet labels, zodiac constellations slightly brighter */
            glyph_color_t const_color = {0.5f, 0.55f, 0.65f,
                                          const_alpha * 0.6f};
            glyph_color_t zodiac_color = {0.7f, 0.75f, 0.8f,
                                           const_alpha * 0.8f};
            float const_scale = 0.25f * zoom_scale;
            for (int c = 0; c < nlabels && len < GLYPH_BATCH_MAX - 20; c++) {
                const char *name = labels[c].abbr; /* 3-letter IAU abbreviation */
                int nlen = (int)strlen(name);
                vec3_t base = vec3_create(labels[c].x,
                                           labels[c].y + 0.5f * zoom_scale,
                                           labels[c].z);
                float spacing = 0.15f * zoom_scale;
                float sx = -spacing * (float)(nlen - 1) * 0.5f;
                glyph_color_t col = labels[c].is_zodiac ? zodiac_color : const_color;
                for (int i = 0; i < nlen && len < GLYPH_BATCH_MAX; i++) {
                    vec3_t offset = vec3_scale(cam_right, sx + (float)i * spacing);
                    instances[len].glyph_id = (int)name[i];
                    instances[len].position = vec3_add(base, offset);
                    instances[len].scale    = const_scale;
                    instances[len].color    = col;
                    len++;
                }
            }
        }
    }

    /* Constellation labels — 88 IAU names on celestial sphere */
    {
        float con_alpha = zf_opacity(ZF_CONSTELLATION, frame->log_zoom);
        if (con_alpha > 0.02f && len < GLYPH_BATCH_MAX - 400) {
            cl_label_t labels[CL_COUNT];
            int lcount = cl_compute(50.0f, labels);
            float con_scale = 0.25f * zoom_scale;
            glyph_color_t con_color = {
                cosmos.brand_secondary.r, cosmos.brand_secondary.g,
                cosmos.brand_secondary.b, con_alpha * 0.5f
            };
            for (int ci = 0; ci < lcount && len < GLYPH_BATCH_MAX - 10; ci++) {
                const char *abbr = labels[ci].abbr;
                if (!abbr) continue;
                int alen = (int)strlen(abbr);
                vec3_t base = vec3_create(labels[ci].x, labels[ci].y, labels[ci].z);
                float spacing = 0.08f * zoom_scale;
                float sx = -spacing * (float)(alen - 1) * 0.5f;
                for (int ai = 0; ai < alen && len < GLYPH_BATCH_MAX; ai++) {
                    vec3_t offset = vec3_scale(cam_right, sx + (float)ai * spacing);
                    instances[len].glyph_id = (int)abbr[ai];
                    instances[len].position = vec3_add(base, offset);
                    instances[len].scale    = con_scale;
                    instances[len].color    = con_color;
                    len++;
                }
            }
        }
    }

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
    glDeleteProgram(s_msdf_program);
    glDeleteTextures(1, &s_msdf_atlas_tex);
    glDeleteBuffers(1, &s_text_vbo);
    glDeleteBuffers(1, &s_text_ebo);
    glDeleteVertexArrays(1, &s_text_vao);
}

#endif /* __EMSCRIPTEN__ */
