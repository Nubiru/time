#include "golden_layout.h"

#include <math.h>

/* --- Typography Scale --- */

gl_type_scale_t gl_type_scale(float base_size)
{
    gl_type_scale_t s;
    /* Level 3 = body = base_size. Build up and down from there. */
    s.sizes[3] = base_size;
    /* Going up (larger): multiply by phi each level */
    s.sizes[2] = base_size * (float)PHI_RATIO;
    s.sizes[1] = s.sizes[2] * (float)PHI_RATIO;
    s.sizes[0] = s.sizes[1] * (float)PHI_RATIO;
    /* Going down (smaller): multiply by phi^-1 each level */
    s.sizes[4] = base_size * (float)PHI_INV;
    s.sizes[5] = s.sizes[4] * (float)PHI_INV;
    s.sizes[6] = s.sizes[5] * (float)PHI_INV;
    s.sizes[7] = s.sizes[6] * (float)PHI_INV;

    s.line_height = (float)PHI_RATIO;
    s.paragraph_spacing = (float)PHI_RATIO * (float)PHI_RATIO;
    return s;
}

float gl_font_size(gl_type_scale_t scale, int level)
{
    if (level < 0) level = 0;
    if (level >= PHI_TYPE_LEVELS) level = PHI_TYPE_LEVELS - 1;
    return scale.sizes[level];
}

/* --- Spacing Scale --- */

gl_spacing_t gl_spacing_scale(float base_unit)
{
    gl_spacing_t s;
    /* Level 4 = base. Build out from center. */
    s.values[4] = base_unit;
    s.values[5] = base_unit * (float)PHI_RATIO;
    s.values[6] = s.values[5] * (float)PHI_RATIO;
    s.values[7] = s.values[6] * (float)PHI_RATIO;
    s.values[8] = s.values[7] * (float)PHI_RATIO;
    s.values[9] = s.values[8] * (float)PHI_RATIO;
    s.values[3] = base_unit * (float)PHI_INV;
    s.values[2] = s.values[3] * (float)PHI_INV;
    s.values[1] = s.values[2] * (float)PHI_INV;
    s.values[0] = s.values[1] * (float)PHI_INV;
    return s;
}

float gl_space(gl_spacing_t spacing, int level)
{
    if (level < 0) level = 0;
    if (level >= PHI_SPACE_LEVELS) level = PHI_SPACE_LEVELS - 1;
    return spacing.values[level];
}

/* --- Screen Layout --- */

gl_screen_t gl_screen_layout(float aspect_ratio)
{
    gl_screen_t s;

    float margin = (float)PHI_INV5; /* ~0.09 of screen */
    s.margin = margin;

    float content_w = 1.0f - 2.0f * margin;
    float content_h = 1.0f - 2.0f * margin;

    /* Main area: golden major portion of width */
    float split = content_w * (float)PHI_INV;
    s.main_area.x = margin;
    s.main_area.y = margin;
    s.main_area.w = split;
    s.main_area.h = content_h;

    /* Sidebar: golden minor portion */
    s.sidebar.x = margin + split;
    s.sidebar.y = margin;
    s.sidebar.w = content_w - split;
    s.sidebar.h = content_h;

    /* HUD anchors at golden section points */
    s.hud_top_left.x = margin;
    s.hud_top_left.y = margin;
    s.hud_top_left.w = content_w * (float)PHI_INV2;
    s.hud_top_left.h = content_h * (float)PHI_INV3;

    s.hud_top_right.x = 1.0f - margin - content_w * (float)PHI_INV2;
    s.hud_top_right.y = margin;
    s.hud_top_right.w = content_w * (float)PHI_INV2;
    s.hud_top_right.h = content_h * (float)PHI_INV3;

    s.hud_bottom.x = margin;
    s.hud_bottom.y = 1.0f - margin - content_h * (float)PHI_INV3;
    s.hud_bottom.w = content_w;
    s.hud_bottom.h = content_h * (float)PHI_INV3;

    (void)aspect_ratio; /* reserved for future aspect-aware adjustments */
    return s;
}

gl_rect_t gl_rect_major(gl_rect_t r, int horizontal)
{
    gl_rect_t m = r;
    if (horizontal) {
        m.w = r.w * (float)PHI_INV;
    } else {
        m.h = r.h * (float)PHI_INV;
    }
    return m;
}

gl_rect_t gl_rect_minor(gl_rect_t r, int horizontal)
{
    gl_rect_t m = r;
    if (horizontal) {
        float major_w = r.w * (float)PHI_INV;
        m.x = r.x + major_w;
        m.w = r.w - major_w;
    } else {
        float major_h = r.h * (float)PHI_INV;
        m.y = r.y + major_h;
        m.h = r.h - major_h;
    }
    return m;
}

gl_rect_t gl_rect_golden(float center_x, float center_y, float width)
{
    gl_rect_t r;
    float height = width / (float)PHI_RATIO;
    r.x = center_x - width / 2.0f;
    r.y = center_y - height / 2.0f;
    r.w = width;
    r.h = height;
    return r;
}

void gl_distribute(float start, float end, int n, float *out)
{
    if (n <= 0) return;
    if (n == 1) {
        /* Single item at golden section point */
        out[0] = start + (end - start) * (float)PHI_INV;
        return;
    }

    float range = end - start;
    for (int i = 0; i < n; i++) {
        /* Distribute using golden angle (137.508 degrees) for optimal spacing */
        float t = (float)i / (float)(n - 1);
        out[i] = start + range * t;
    }
}

/* --- Card Dimensions --- */

gl_card_t gl_card_dimensions(float width, float base_spacing)
{
    gl_card_t c;
    c.width = width;
    c.height = width / (float)PHI_RATIO;
    c.padding = base_spacing * (float)PHI_INV;
    c.corner_radius = base_spacing * (float)PHI_INV2;

    float inner_h = c.height - 2.0f * c.padding;
    c.header_height = inner_h * (float)PHI_INV2;
    c.body_height = inner_h * (float)PHI_INV;
    c.footer_height = inner_h - c.header_height - c.body_height;
    return c;
}

/* --- Animation Timing --- */

gl_timing_t gl_timing_scale(float base_seconds)
{
    gl_timing_t t;
    t.base = base_seconds;
    t.slow = base_seconds * (float)PHI_INV;
    t.medium = t.slow * (float)PHI_INV;
    t.normal = t.medium * (float)PHI_INV;
    t.fast = t.normal * (float)PHI_INV;
    t.micro = t.fast * (float)PHI_INV;
    t.dramatic = base_seconds * (float)PHI_RATIO;
    t.epic = base_seconds * (float)PHI_SQ;
    return t;
}

float gl_stagger_delay(gl_timing_t timing, int index, int total)
{
    if (total <= 1 || index <= 0) return 0.0f;

    /* Each successive item delayed by fibonacci-growing intervals */
    float delay = 0.0f;
    float interval = timing.micro;
    for (int i = 1; i <= index; i++) {
        delay += interval;
        interval *= (float)PHI_INV; /* diminishing returns — feels natural */
    }
    (void)total;
    return delay;
}

/* --- Opacity Cascade --- */

gl_opacity_t gl_opacity_cascade(void)
{
    gl_opacity_t o;
    o.values[0] = 1.0f;
    for (int i = 1; i < PHI_OPACITY_LEVELS; i++) {
        o.values[i] = o.values[i - 1] * (float)PHI_INV;
    }
    return o;
}

float gl_opacity_at(int level)
{
    if (level <= 0) return 1.0f;
    if (level >= PHI_OPACITY_LEVELS) level = PHI_OPACITY_LEVELS - 1;

    float o = 1.0f;
    for (int i = 0; i < level; i++) {
        o *= (float)PHI_INV;
    }
    return o;
}

/* --- Proportional Sizes --- */

float gl_ring_outer(float inner_radius)
{
    return inner_radius * (float)PHI_RATIO;
}

float gl_ring_inner(float outer_radius)
{
    return outer_radius * (float)PHI_INV;
}

void gl_concentric_radii(float innermost, int n, float *out)
{
    if (n <= 0) return;
    out[0] = innermost;
    for (int i = 1; i < n; i++) {
        out[i] = out[i - 1] * (float)PHI_RATIO;
    }
}

float gl_aspect_ratio(void)
{
    return (float)PHI_RATIO;
}

/* --- Scale System --- */

float gl_zoom_factor(int level)
{
    float result = 1.0f;
    for (int i = 0; i < level; i++) {
        result *= (float)PHI_RATIO;
    }
    return result;
}

float gl_scale_fade(float value, int level, int total_levels)
{
    if (total_levels <= 1) return 1.0f;

    float center = (float)level / (float)(total_levels - 1);
    float half_width = (float)PHI_INV / (float)(total_levels - 1);
    float dist = fabsf(value - center);

    if (dist >= half_width) return 0.0f;
    /* Smooth fade using cosine (phi-width window) */
    float t = dist / half_width;
    return 0.5f * (1.0f + cosf(t * 3.14159265358979323846f));
}
