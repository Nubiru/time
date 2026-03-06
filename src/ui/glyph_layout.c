#include "glyph_layout.h"

glyph_rect_t glyph_text_rect(glyph_style_t style, int num_chars)
{
    glyph_rect_t r;
    float text_w = (float)num_chars * style.char_width;
    r.width = 2.0f * style.padding + text_w;
    r.height = 2.0f * style.padding + style.char_height;
    r.y = style.y;

    if (style.align == ALIGN_CENTER) {
        r.x = style.x - text_w / 2.0f - style.padding;
    } else if (style.align == ALIGN_RIGHT) {
        r.x = style.x - r.width;
    } else {
        r.x = style.x;
    }
    return r;
}

glyph_rect_t glyph_block_rect(glyph_style_t style, int num_lines, int max_chars)
{
    glyph_rect_t r;
    float text_w = (float)max_chars * style.char_width;
    float text_h = (float)num_lines * style.char_height;
    if (num_lines > 1)
        text_h += (float)(num_lines - 1) * style.line_spacing;

    r.width = 2.0f * style.padding + text_w;
    r.height = 2.0f * style.padding + text_h;
    r.y = style.y;

    if (style.align == ALIGN_CENTER) {
        r.x = style.x - text_w / 2.0f - style.padding;
    } else if (style.align == ALIGN_RIGHT) {
        r.x = style.x - r.width;
    } else {
        r.x = style.x;
    }
    return r;
}

float glyph_char_x(glyph_style_t style, int char_index, int line_length)
{
    float offset = (float)char_index * style.char_width;

    if (style.align == ALIGN_CENTER) {
        float half_line = (float)line_length * style.char_width / 2.0f;
        return style.x - half_line + offset;
    } else if (style.align == ALIGN_RIGHT) {
        float full_line = (float)line_length * style.char_width;
        return style.x - style.padding - full_line + offset;
    }
    /* ALIGN_LEFT */
    return style.x + style.padding + offset;
}

float glyph_line_y(glyph_style_t style, int line_index)
{
    return style.y + style.padding
         + (float)line_index * (style.char_height + style.line_spacing);
}

glyph_rect_t glyph_label_above(float px, float py, float offset_y,
                                glyph_style_t style, int num_chars)
{
    glyph_rect_t r;
    float text_w = (float)num_chars * style.char_width;
    r.width = 2.0f * style.padding + text_w;
    r.height = 2.0f * style.padding + style.char_height;
    r.x = px - r.width / 2.0f;
    r.y = py - offset_y - r.height;
    return r;
}

glyph_rect_t glyph_label_below(float px, float py, float offset_y,
                                glyph_style_t style, int num_chars)
{
    glyph_rect_t r;
    float text_w = (float)num_chars * style.char_width;
    r.width = 2.0f * style.padding + text_w;
    r.height = 2.0f * style.padding + style.char_height;
    r.x = px - r.width / 2.0f;
    r.y = py + offset_y;
    return r;
}

int glyph_rects_overlap(glyph_rect_t a, glyph_rect_t b)
{
    if (a.x >= b.x + b.width) return 0;
    if (a.x + a.width <= b.x) return 0;
    if (a.y >= b.y + b.height) return 0;
    if (a.y + a.height <= b.y) return 0;
    return 1;
}

glyph_rect_t glyph_clamp_to_screen(glyph_rect_t r, float screen_w, float screen_h)
{
    if (r.x < 0.0f) r.x = 0.0f;
    if (r.y < 0.0f) r.y = 0.0f;
    if (r.x + r.width > screen_w) r.x = screen_w - r.width;
    if (r.y + r.height > screen_h) r.y = screen_h - r.height;
    return r;
}

glyph_style_t glyph_style_hud(void)
{
    glyph_style_t s;
    s.x = 0.0f;
    s.y = 0.0f;
    s.char_width = 8.0f;
    s.char_height = 14.0f;
    s.line_spacing = 2.0f;
    s.padding = 4.0f;
    s.align = ALIGN_LEFT;
    return s;
}

glyph_style_t glyph_style_card(void)
{
    glyph_style_t s;
    s.x = 0.0f;
    s.y = 0.0f;
    s.char_width = 10.0f;
    s.char_height = 18.0f;
    s.line_spacing = 4.0f;
    s.padding = 8.0f;
    s.align = ALIGN_CENTER;
    return s;
}
