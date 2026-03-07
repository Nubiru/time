/* text_render.c -- SDF text rendering data implementation
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "text_render.h"

/* Default slide distance in pixels for slide animations */
static const float TEXT_SLIDE_DISTANCE = 30.0f;

/* ===== Internal helpers ===== */

static float clampf(float x, float lo, float hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static float smoothstep(float edge0, float edge1, float x) {
    if (edge1 <= edge0) {
        /* Degenerate: hard cutoff at midpoint */
        return x < (edge0 + edge1) * 0.5f ? 0.0f : 1.0f;
    }
    float t = clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

/* ===== SDF Functions ===== */

float sdf_alpha(float distance, float edge, float smoothing) {
    /* Inside = low distance => high alpha. Invert smoothstep. */
    return 1.0f - smoothstep(edge - smoothing, edge + smoothing, distance);
}

float sdf_outline_alpha(float distance, float edge, float outline_width,
                        float smoothing) {
    if (outline_width <= 0.0f) return 0.0f;
    /* Outline lives between edge and edge + outline_width.
     * inner_alpha = how much we're outside the glyph body
     * outer_alpha = how much we're inside the outline outer bound */
    float inner = smoothstep(edge - smoothing, edge + smoothing, distance);
    float outer = 1.0f - smoothstep(edge + outline_width - smoothing,
                                     edge + outline_width + smoothing,
                                     distance);
    return inner * outer;
}

sdf_params_t sdf_params_default(void) {
    sdf_params_t p;
    p.edge_threshold = 0.5f;
    p.smoothing = 0.1f;
    p.outline_width = 0.0f;
    p.outline_r = 0.0f;
    p.outline_g = 0.0f;
    p.outline_b = 0.0f;
    p.outline_a = 1.0f;
    p.shadow_offset_x = 0.0f;
    p.shadow_offset_y = 0.0f;
    p.shadow_softness = 0.0f;
    p.shadow_r = 0.0f;
    p.shadow_g = 0.0f;
    p.shadow_b = 0.0f;
    p.shadow_a = 0.5f;
    p.glow_radius = 0.0f;
    p.glow_r = 1.0f;
    p.glow_g = 1.0f;
    p.glow_b = 1.0f;
    p.glow_a = 0.0f;
    return p;
}

sdf_params_t sdf_params_outlined(float width, float r, float g, float b) {
    sdf_params_t p = sdf_params_default();
    p.outline_width = width;
    p.outline_r = r;
    p.outline_g = g;
    p.outline_b = b;
    p.outline_a = 1.0f;
    return p;
}

sdf_params_t sdf_params_shadowed(float ox, float oy, float softness) {
    sdf_params_t p = sdf_params_default();
    p.shadow_offset_x = ox;
    p.shadow_offset_y = oy;
    p.shadow_softness = softness;
    p.shadow_r = 0.0f;
    p.shadow_g = 0.0f;
    p.shadow_b = 0.0f;
    p.shadow_a = 0.5f;
    return p;
}

sdf_params_t sdf_params_glowing(float radius, float r, float g, float b) {
    sdf_params_t p = sdf_params_default();
    p.glow_radius = radius;
    p.glow_r = r;
    p.glow_g = g;
    p.glow_b = b;
    p.glow_a = 1.0f;
    return p;
}

/* ===== Text Size Scale ===== */

static const float SIZE_SCALES[TEXT_SIZE_COUNT] = {
    0.5f,  /* TINY */
    0.7f,  /* SMALL */
    1.0f,  /* BODY */
    1.4f,  /* LARGE */
    2.0f,  /* TITLE */
    3.0f   /* DISPLAY */
};

float text_size_scale(text_size_t size) {
    if (size < 0 || size >= TEXT_SIZE_COUNT) return 1.0f;
    return SIZE_SCALES[size];
}

/* ===== Text Style Presets ===== */

text_style_t text_style_label(void) {
    text_style_t s;
    s.size = TEXT_SIZE_SMALL;
    s.align = TEXT_ALIGN_LEFT;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.2f;
    s.letter_spacing = 0.0f;
    s.opacity = 1.0f;
    return s;
}

text_style_t text_style_body(void) {
    text_style_t s;
    s.size = TEXT_SIZE_BODY;
    s.align = TEXT_ALIGN_LEFT;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.5f;
    s.letter_spacing = 0.0f;
    s.opacity = 1.0f;
    return s;
}

text_style_t text_style_title(void) {
    text_style_t s;
    s.size = TEXT_SIZE_TITLE;
    s.align = TEXT_ALIGN_CENTER;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.2f;
    s.letter_spacing = 0.02f;
    s.opacity = 1.0f;
    return s;
}

text_style_t text_style_subtitle(void) {
    text_style_t s;
    s.size = TEXT_SIZE_LARGE;
    s.align = TEXT_ALIGN_CENTER;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.3f;
    s.letter_spacing = 0.01f;
    s.opacity = 0.85f;
    return s;
}

text_style_t text_style_annotation(void) {
    text_style_t s;
    s.size = TEXT_SIZE_TINY;
    s.align = TEXT_ALIGN_LEFT;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.1f;
    s.letter_spacing = 0.0f;
    s.opacity = 0.7f;
    return s;
}

text_style_t text_style_data(void) {
    text_style_t s;
    s.size = TEXT_SIZE_BODY;
    s.align = TEXT_ALIGN_RIGHT;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.4f;
    s.letter_spacing = 0.02f;
    s.opacity = 1.0f;
    return s;
}

text_style_t text_style_accent(void) {
    text_style_t s;
    s.size = TEXT_SIZE_LARGE;
    s.align = TEXT_ALIGN_CENTER;
    s.sdf = sdf_params_glowing(0.12f, 1.0f, 0.85f, 0.55f);
    s.line_spacing = 1.3f;
    s.letter_spacing = 0.03f;
    s.opacity = 1.0f;
    return s;
}

text_style_t text_style_hud(void) {
    text_style_t s;
    s.size = TEXT_SIZE_SMALL;
    s.align = TEXT_ALIGN_LEFT;
    s.sdf = sdf_params_default();
    s.line_spacing = 1.2f;
    s.letter_spacing = 0.01f;
    s.opacity = 0.9f;
    return s;
}

/* ===== Text Animation ===== */

text_anim_t text_anim_create(text_anim_type_t type, float duration) {
    text_anim_t a;
    a.type = type;
    a.duration = duration;
    a.elapsed = 0.0f;
    a.delay = 0.0f;
    a.char_stagger = 0.0f;
    a.char_count = 0;
    a.active = 1;
    a.finished = (type == TEXT_ANIM_NONE) ? 1 : 0;
    return a;
}

text_anim_t text_anim_typewriter(float char_delay, int char_count) {
    text_anim_t a;
    a.type = TEXT_ANIM_TYPEWRITER;
    a.duration = char_delay * (float)char_count;
    a.elapsed = 0.0f;
    a.delay = 0.0f;
    a.char_stagger = char_delay;
    a.char_count = char_count;
    a.active = 1;
    a.finished = 0;
    return a;
}

text_anim_t text_anim_delayed(text_anim_t anim, float delay) {
    text_anim_t result = anim;
    result.delay = delay;
    return result;
}

text_anim_t text_anim_update(text_anim_t anim, float dt) {
    text_anim_t result = anim;
    result.elapsed = anim.elapsed + dt;
    if (result.type == TEXT_ANIM_NONE) {
        result.finished = 1;
    } else if (result.elapsed >= result.delay + result.duration) {
        result.finished = 1;
    }
    return result;
}

float text_anim_progress(text_anim_t anim) {
    if (anim.type == TEXT_ANIM_NONE) return 1.0f;
    if (anim.duration <= 0.0f) return 1.0f;
    float t = (anim.elapsed - anim.delay) / anim.duration;
    return clampf(t, 0.0f, 1.0f);
}

float text_anim_opacity(text_anim_t anim) {
    float p = text_anim_progress(anim);
    switch (anim.type) {
        case TEXT_ANIM_FADE_IN:
            return p;
        case TEXT_ANIM_FADE_OUT:
            return 1.0f - p;
        case TEXT_ANIM_NONE:
        case TEXT_ANIM_SLIDE_UP:
        case TEXT_ANIM_SLIDE_DOWN:
        case TEXT_ANIM_SLIDE_LEFT:
        case TEXT_ANIM_SLIDE_RIGHT:
        case TEXT_ANIM_SCALE_UP:
        case TEXT_ANIM_SCALE_DOWN:
        case TEXT_ANIM_TYPEWRITER:
        case TEXT_ANIM_COUNT:
        default:
            return 1.0f;
    }
}

void text_anim_offset(text_anim_t anim, float *dx, float *dy) {
    float p = text_anim_progress(anim);
    float remain = 1.0f - p;
    *dx = 0.0f;
    *dy = 0.0f;
    switch (anim.type) {
        case TEXT_ANIM_SLIDE_UP:
            *dy = -remain * TEXT_SLIDE_DISTANCE;
            break;
        case TEXT_ANIM_SLIDE_DOWN:
            *dy = remain * TEXT_SLIDE_DISTANCE;
            break;
        case TEXT_ANIM_SLIDE_LEFT:
            *dx = -remain * TEXT_SLIDE_DISTANCE;
            break;
        case TEXT_ANIM_SLIDE_RIGHT:
            *dx = remain * TEXT_SLIDE_DISTANCE;
            break;
        default:
            break;
    }
}

float text_anim_scale(text_anim_t anim) {
    float p = text_anim_progress(anim);
    switch (anim.type) {
        case TEXT_ANIM_SCALE_UP:
            return p;
        case TEXT_ANIM_SCALE_DOWN:
            return 1.0f - p;
        default:
            return 1.0f;
    }
}

float text_anim_char_opacity(text_anim_t anim, int char_index) {
    if (anim.type == TEXT_ANIM_TYPEWRITER) {
        float effective = anim.elapsed - anim.delay;
        float threshold = (float)char_index * anim.char_stagger;
        return effective >= threshold ? 1.0f : 0.0f;
    }
    /* Non-typewriter: all chars share the same opacity */
    return text_anim_opacity(anim);
}

/* ===== Unicode Script Ranges ===== */

#define SCRIPT_RANGE_TOTAL 8

static const script_range_t SCRIPT_RANGES[SCRIPT_RANGE_TOTAL] = {
    { "Basic Latin",     0x0020, 0x007E, 0, 0 },
    { "Latin Extended",  0x00C0, 0x024F, 0, 0 },
    { "Hebrew",          0x0590, 0x05FF, 1, 1 },
    { "Arabic",          0x0600, 0x06FF, 1, 2 },
    { "Devanagari",      0x0900, 0x097F, 0, 3 },
    { "CJK Unified",     0x4E00, 0x9FFF, 0, 4 },
    { "Zodiac Symbols",  0x2648, 0x2653, 0, 5 },
    { "Planet Symbols",  0x2609, 0x2647, 0, 5 }
};

int script_range_count(void) {
    return SCRIPT_RANGE_TOTAL;
}

script_range_t script_range_get(int index) {
    if (index < 0 || index >= SCRIPT_RANGE_TOTAL) {
        script_range_t empty = { "Unknown", 0, 0, 0, -1 };
        return empty;
    }
    return SCRIPT_RANGES[index];
}

int script_range_find(int codepoint) {
    for (int i = 0; i < SCRIPT_RANGE_TOTAL; i++) {
        if (codepoint >= SCRIPT_RANGES[i].start_codepoint &&
            codepoint <= SCRIPT_RANGES[i].end_codepoint) {
            return i;
        }
    }
    return -1;
}

int script_is_rtl(int codepoint) {
    int idx = script_range_find(codepoint);
    if (idx < 0) return 0;
    return SCRIPT_RANGES[idx].rtl;
}

int script_atlas_for(int codepoint) {
    int idx = script_range_find(codepoint);
    if (idx < 0) return -1;
    return SCRIPT_RANGES[idx].atlas_index;
}
