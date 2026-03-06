#include "font_atlas.h"
#include <string.h>

font_atlas_t font_atlas_create(int cols, int rows, int first_char, int last_char,
                                float cell_width, float cell_height) {
    font_atlas_t a;
    a.cols = cols;
    a.rows = rows;
    a.first_char = first_char;
    a.last_char = last_char;
    a.cell_width = cell_width;
    a.cell_height = cell_height;
    return a;
}

glyph_uv_t font_glyph_uv(font_atlas_t atlas, char c) {
    glyph_uv_t uv = {0.0f, 0.0f, 0.0f, 0.0f};
    int code = (unsigned char)c;

    if (code < atlas.first_char || code > atlas.last_char) {
        return uv;
    }
    if (c == ' ') {
        return uv;
    }

    int idx = code - atlas.first_char;
    int col = idx % atlas.cols;
    int row = idx / atlas.cols;

    uv.u0 = (float)col / (float)atlas.cols;
    uv.v0 = (float)row / (float)atlas.rows;
    uv.u1 = (float)(col + 1) / (float)atlas.cols;
    uv.v1 = (float)(row + 1) / (float)atlas.rows;
    return uv;
}

text_layout_t font_layout_text(font_atlas_t atlas, const char *text,
                                float x, float y, float scale) {
    text_layout_t layout;
    layout.count = 0;
    layout.total_width = 0.0f;
    layout.line_height = atlas.cell_height * scale;

    if (text == NULL) return layout;

    float start_x = x;
    float cx = x;
    float cy = y;
    float w = atlas.cell_width * scale;
    float h = atlas.cell_height * scale;
    int len = (int)strlen(text);

    for (int i = 0; i < len && layout.count < 256; i++) {
        char c = text[i];
        int code = (unsigned char)c;

        if (c == '\n') {
            cx = start_x;
            cy += h;
            continue;
        }

        if (code < atlas.first_char || code > atlas.last_char) {
            continue;
        }

        glyph_quad_t *q = &layout.quads[layout.count];
        q->x = cx;
        q->y = cy;
        q->w = w;
        q->h = h;
        q->uv = font_glyph_uv(atlas, c);
        layout.count++;
        cx += w;
    }

    layout.total_width = cx - start_x;
    return layout;
}

text_layout_t font_layout_wrapped(font_atlas_t atlas, const char *text,
                                   float x, float y, float scale,
                                   float max_width, float line_spacing) {
    text_layout_t layout;
    layout.count = 0;
    layout.total_width = 0.0f;
    layout.line_height = atlas.cell_height * scale;

    if (text == NULL) return layout;

    float start_x = x;
    float cx = x;
    float cy = y;
    float w = atlas.cell_width * scale;
    float h = atlas.cell_height * scale;
    float line_advance = h * line_spacing;
    float current_line_width = 0.0f;
    int len = (int)strlen(text);

    for (int i = 0; i < len && layout.count < 256; i++) {
        char c = text[i];
        int code = (unsigned char)c;

        if (c == '\n') {
            if (current_line_width > layout.total_width) {
                layout.total_width = current_line_width;
            }
            cx = start_x;
            cy += line_advance;
            current_line_width = 0.0f;
            continue;
        }

        if (code < atlas.first_char || code > atlas.last_char) {
            continue;
        }

        /* Wrap if this character would exceed max_width */
        if (current_line_width + w > max_width && current_line_width > 0.0f) {
            if (current_line_width > layout.total_width) {
                layout.total_width = current_line_width;
            }
            cx = start_x;
            cy += line_advance;
            current_line_width = 0.0f;
        }

        glyph_quad_t *q = &layout.quads[layout.count];
        q->x = cx;
        q->y = cy;
        q->w = w;
        q->h = h;
        q->uv = font_glyph_uv(atlas, c);
        layout.count++;
        cx += w;
        current_line_width += w;
    }

    if (current_line_width > layout.total_width) {
        layout.total_width = current_line_width;
    }

    return layout;
}

float font_measure_width(font_atlas_t atlas, const char *text, float scale) {
    if (text == NULL) return 0.0f;

    float w = atlas.cell_width * scale;
    float max_line = 0.0f;
    float current = 0.0f;
    int len = (int)strlen(text);

    for (int i = 0; i < len; i++) {
        char c = text[i];
        int code = (unsigned char)c;

        if (c == '\n') {
            if (current > max_line) max_line = current;
            current = 0.0f;
            continue;
        }
        if (code >= atlas.first_char && code <= atlas.last_char) {
            current += w;
        }
    }

    if (current > max_line) max_line = current;
    return max_line;
}

float font_measure_height(font_atlas_t atlas, const char *text, float scale,
                           float max_width, float line_spacing) {
    if (text == NULL) return 0.0f;

    float w = atlas.cell_width * scale;
    float h = atlas.cell_height * scale;
    float line_advance = h * line_spacing;
    int lines = 1;
    float current_width = 0.0f;
    int len = (int)strlen(text);

    if (len == 0) return 0.0f;

    for (int i = 0; i < len; i++) {
        char c = text[i];
        int code = (unsigned char)c;

        if (c == '\n') {
            lines++;
            current_width = 0.0f;
            continue;
        }

        if (code < atlas.first_char || code > atlas.last_char) {
            continue;
        }

        if (current_width + w > max_width && current_width > 0.0f) {
            lines++;
            current_width = 0.0f;
        }
        current_width += w;
    }

    /* First line uses cell height, subsequent lines use line_advance */
    return h + (float)(lines - 1) * line_advance;
}

text_layout_t font_layout_centered(font_atlas_t atlas, const char *text,
                                    float center_x, float y, float scale) {
    float width = font_measure_width(atlas, text, scale);
    float start_x = center_x - width / 2.0f;
    return font_layout_text(atlas, text, start_x, y, scale);
}
