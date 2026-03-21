/*
 * msdf_text.c — MSDF text layout (pure module, P1)
 *
 * Computes glyph quad positions and UV coordinates from MSDF atlas metrics.
 * No GL, no state — pure math.
 */

#include "msdf_text.h"
#include "msdf_mono_data.h"
#include "msdf_sans_data.h"
#include <string.h>

/* Line height multiplier relative to font_size */
#define LINE_HEIGHT_FACTOR 1.25f

/* --- Internal: decode one UTF-8 codepoint --- */

static int utf8_decode(const char *s, int *codepoint)
{
    const unsigned char *p = (const unsigned char *)s;
    if (p[0] < 0x80) {
        *codepoint = p[0];
        return 1;
    }
    if ((p[0] & 0xE0) == 0xC0 && (p[1] & 0xC0) == 0x80) {
        *codepoint = ((p[0] & 0x1F) << 6) | (p[1] & 0x3F);
        return 2;
    }
    if ((p[0] & 0xF0) == 0xE0 && (p[1] & 0xC0) == 0x80 &&
        (p[2] & 0xC0) == 0x80) {
        *codepoint = ((p[0] & 0x0F) << 12) | ((p[1] & 0x3F) << 6) |
                     (p[2] & 0x3F);
        return 3;
    }
    if ((p[0] & 0xF8) == 0xF0 && (p[1] & 0xC0) == 0x80 &&
        (p[2] & 0xC0) == 0x80 && (p[3] & 0xC0) == 0x80) {
        *codepoint = ((p[0] & 0x07) << 18) | ((p[1] & 0x3F) << 12) |
                     ((p[2] & 0x3F) << 6) | (p[3] & 0x3F);
        return 4;
    }
    /* Invalid: skip one byte */
    *codepoint = -1;
    return 1;
}

/* --- Internal: font-agnostic glyph lookup --- */

typedef struct {
    float advance;
    float plane_left, plane_bottom, plane_right, plane_top;
    float atlas_left, atlas_bottom, atlas_right, atlas_top;
    int found;
} glyph_info_t;

static glyph_info_t lookup_glyph(msdf_font_id_t font, int codepoint)
{
    glyph_info_t info = {0};

    if (font == MSDF_FONT_MONO) {
        const msdf_mono_glyph_t *g = msdf_mono_find(codepoint);
        if (!g) return info;
        info.found = 1;
        info.advance = g->advance;
        info.plane_left   = g->plane.left;
        info.plane_bottom = g->plane.bottom;
        info.plane_right  = g->plane.right;
        info.plane_top    = g->plane.top;
        info.atlas_left   = g->atlas.left;
        info.atlas_bottom = g->atlas.bottom;
        info.atlas_right  = g->atlas.right;
        info.atlas_top    = g->atlas.top;
    } else {
        const msdf_sans_glyph_t *g = msdf_sans_find(codepoint);
        if (!g) return info;
        info.found = 1;
        info.advance = g->advance;
        info.plane_left   = g->plane.left;
        info.plane_bottom = g->plane.bottom;
        info.plane_right  = g->plane.right;
        info.plane_top    = g->plane.top;
        info.atlas_left   = g->atlas.left;
        info.atlas_bottom = g->atlas.bottom;
        info.atlas_right  = g->atlas.right;
        info.atlas_top    = g->atlas.top;
    }
    return info;
}

static float atlas_w(msdf_font_id_t font)
{
    return (font == MSDF_FONT_MONO)
        ? (float)MSDF_MONO_ATLAS_W
        : (float)MSDF_SANS_ATLAS_W;
}

static float atlas_h(msdf_font_id_t font)
{
    return (font == MSDF_FONT_MONO)
        ? (float)MSDF_MONO_ATLAS_H
        : (float)MSDF_SANS_ATLAS_H;
}

/* --- Internal: emit one quad --- */

static int emit_quad(msdf_layout_t *layout, msdf_font_id_t font,
                     const glyph_info_t *g, int codepoint,
                     float cursor_x, float cursor_y, float scale)
{
    if (layout->count >= MSDF_TEXT_MAX_QUADS) return 0;

    /* Check for zero-area glyph (space) */
    float pw = g->plane_right - g->plane_left;
    float ph = g->plane_top - g->plane_bottom;
    if (pw <= 0.0f || ph <= 0.0f) return 0;

    float aw = atlas_w(font);
    float ah = atlas_h(font);

    msdf_quad_t *q = &layout->quads[layout->count];
    q->codepoint = codepoint;

    /* Position: cursor + plane offset * scale */
    q->x = cursor_x + g->plane_left * scale;
    q->y = cursor_y - g->plane_top * scale;  /* y grows down */
    q->w = pw * scale;
    q->h = ph * scale;

    /* UV: atlas pixel bounds -> normalized [0,1]
     * atlas bounds are in pixel coords with 0.5 offset (center of texel) */
    q->u0 = g->atlas_left   / aw;
    q->v0 = g->atlas_top    / ah;  /* top of glyph in atlas */
    q->u1 = g->atlas_right  / aw;
    q->v1 = g->atlas_bottom / ah;  /* bottom of glyph in atlas */

    /* Ensure v0 < v1 (normalized top < bottom) */
    if (q->v0 > q->v1) {
        float tmp = q->v0;
        q->v0 = q->v1;
        q->v1 = tmp;
    }

    layout->count++;
    return 1;
}

/* --- Internal: compute advance width of text --- */

static float compute_width(msdf_font_id_t font, const char *text, float scale)
{
    float cursor_x = 0.0f;
    const char *p = text;
    while (*p) {
        int cp;
        int bytes = utf8_decode(p, &cp);
        if (cp > 0) {
            glyph_info_t g = lookup_glyph(font, cp);
            if (g.found) {
                cursor_x += g.advance * scale;
            }
        }
        p += bytes;
    }
    return cursor_x;
}

/* --- Public API --- */

msdf_layout_t msdf_text_layout(msdf_font_id_t font, const char *text,
                               float x, float y, float font_size)
{
    msdf_layout_t layout;
    memset(&layout, 0, sizeof(layout));
    layout.line_height = font_size * LINE_HEIGHT_FACTOR;

    if (!text || *text == '\0') return layout;

    float scale = font_size;  /* atlas metrics are normalized to ~1.0 em */
    float cursor_x = x;
    const char *p = text;

    while (*p && layout.count < MSDF_TEXT_MAX_QUADS) {
        int cp;
        int bytes = utf8_decode(p, &cp);
        if (cp > 0) {
            glyph_info_t g = lookup_glyph(font, cp);
            if (g.found) {
                emit_quad(&layout, font, &g, cp, cursor_x, y, scale);
                cursor_x += g.advance * scale;
            }
        }
        p += bytes;
    }

    layout.total_width = cursor_x - x;
    return layout;
}

float msdf_text_width(msdf_font_id_t font, const char *text, float font_size)
{
    if (!text || *text == '\0') return 0.0f;
    return compute_width(font, text, font_size);
}

msdf_layout_t msdf_text_centered(msdf_font_id_t font, const char *text,
                                 float center_x, float y, float font_size)
{
    float w = msdf_text_width(font, text, font_size);
    float start_x = center_x - w * 0.5f;
    return msdf_text_layout(font, text, start_x, y, font_size);
}

msdf_layout_t msdf_text_wrapped(msdf_font_id_t font, const char *text,
                                float x, float y, float font_size,
                                float max_width, float line_spacing)
{
    msdf_layout_t layout;
    memset(&layout, 0, sizeof(layout));
    layout.line_height = font_size * LINE_HEIGHT_FACTOR;

    if (!text || *text == '\0') return layout;
    if (max_width <= 0.0f) return layout;

    float scale = font_size;
    float cursor_x = x;
    float cursor_y = y;
    float line_advance = layout.line_height * line_spacing;

    /* Word-wrap: scan for word boundaries */
    const char *p = text;
    const char *word_start = text;
    float word_width = 0.0f;
    float line_width = 0.0f;

    while (*p) {
        int cp;
        int bytes = utf8_decode(p, &cp);

        if (cp == ' ' || cp == '\n' || *(p + bytes) == '\0') {
            /* Include this character in the word measurement */
            if (cp > 0 && *(p + bytes) != '\0') {
                glyph_info_t g = lookup_glyph(font, cp);
                if (g.found) word_width += g.advance * scale;
            } else if (*(p + bytes) == '\0' && cp > 0) {
                /* Last character — include it */
                glyph_info_t g = lookup_glyph(font, cp);
                if (g.found) word_width += g.advance * scale;
            }

            /* Check if word fits on current line */
            if (line_width + word_width > max_width && line_width > 0.0f) {
                /* Wrap: move to next line */
                cursor_x = x;
                cursor_y += line_advance;
                line_width = 0.0f;
            }

            /* Emit the word from word_start to p */
            const char *wp = word_start;
            while (wp <= p && *wp) {
                int wcp;
                int wbytes = utf8_decode(wp, &wcp);
                if (wcp > 0 && layout.count < MSDF_TEXT_MAX_QUADS) {
                    glyph_info_t g = lookup_glyph(font, wcp);
                    if (g.found) {
                        emit_quad(&layout, font, &g, wcp,
                                  cursor_x, cursor_y, scale);
                        cursor_x += g.advance * scale;
                        line_width += g.advance * scale;
                    }
                }
                wp += wbytes;
                if (wp > p) break;
            }

            if (cp == '\n') {
                cursor_x = x;
                cursor_y += line_advance;
                line_width = 0.0f;
            }

            word_start = p + bytes;
            word_width = 0.0f;
        } else {
            /* Accumulate word width */
            if (cp > 0) {
                glyph_info_t g = lookup_glyph(font, cp);
                if (g.found) word_width += g.advance * scale;
            }
        }

        p += bytes;
    }

    layout.total_width = cursor_x - x;
    return layout;
}

int msdf_text_atlas_width(msdf_font_id_t font)
{
    return (font == MSDF_FONT_MONO) ? MSDF_MONO_ATLAS_W : MSDF_SANS_ATLAS_W;
}

int msdf_text_atlas_height(msdf_font_id_t font)
{
    return (font == MSDF_FONT_MONO) ? MSDF_MONO_ATLAS_H : MSDF_SANS_ATLAS_H;
}

int msdf_text_glyph_count(msdf_font_id_t font)
{
    return (font == MSDF_FONT_MONO) ? MSDF_MONO_GLYPH_COUNT : MSDF_SANS_GLYPH_COUNT;
}

const unsigned char *msdf_text_atlas_pixels(msdf_font_id_t font)
{
    return (font == MSDF_FONT_MONO) ? MSDF_MONO_PIXELS : MSDF_SANS_PIXELS;
}
