#ifndef TIME_FONT_ATLAS_H
#define TIME_FONT_ATLAS_H

typedef struct {
    int cols;
    int rows;
    int first_char;
    int last_char;
    float cell_width;
    float cell_height;
} font_atlas_t;

typedef struct {
    float u0, v0;
    float u1, v1;
} glyph_uv_t;

typedef struct {
    float x, y;
    float w, h;
    glyph_uv_t uv;
} glyph_quad_t;

typedef struct {
    glyph_quad_t quads[256];
    int count;
    float total_width;
    float line_height;
} text_layout_t;

font_atlas_t font_atlas_create(int cols, int rows, int first_char, int last_char,
                                float cell_width, float cell_height);

glyph_uv_t font_glyph_uv(font_atlas_t atlas, char c);

text_layout_t font_layout_text(font_atlas_t atlas, const char *text,
                                float x, float y, float scale);

text_layout_t font_layout_wrapped(font_atlas_t atlas, const char *text,
                                   float x, float y, float scale,
                                   float max_width, float line_spacing);

float font_measure_width(font_atlas_t atlas, const char *text, float scale);

float font_measure_height(font_atlas_t atlas, const char *text, float scale,
                           float max_width, float line_spacing);

text_layout_t font_layout_centered(font_atlas_t atlas, const char *text,
                                    float center_x, float y, float scale);

#endif
