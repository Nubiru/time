#include "theme.h"

#include <stdio.h>

#define THEME_BASE_FONT    13.0f
#define THEME_BASE_SPACE   8.0f
#define THEME_BASE_TIMING  1.0f  /* 1 second */
#define THEME_SURFACE_TINT 0.05f /* subtle system tint */

/* --- Internal helpers --- */

static float clampf(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static color_rgba_t rgba_lerp(color_rgba_t a, color_rgba_t b, float t)
{
    color_rgba_t c;
    c.r = a.r + (b.r - a.r) * t;
    c.g = a.g + (b.g - a.g) * t;
    c.b = a.b + (b.b - a.b) * t;
    c.a = a.a + (b.a - a.a) * t;
    return c;
}

static float lerpf(float a, float b, float t)
{
    return a + (b - a) * t;
}

/* --- Shared layout construction --- */

static void theme_fill_layout(theme_t *t, float base_font)
{
    t->typography = gl_type_scale(base_font);
    t->spacing = gl_spacing_scale(THEME_BASE_SPACE);
    t->timing = gl_timing_scale(THEME_BASE_TIMING);
    t->opacity = gl_opacity_cascade();

    /* Derived metrics from phi proportions */
    t->corner_radius = THEME_BASE_SPACE * (float)PHI_INV2;
    t->border_width = 1.0f;
    t->shadow_blur = THEME_BASE_SPACE * (float)PHI_RATIO;
    t->icon_size = t->typography.sizes[3] * (float)PHI_RATIO;
}

/* --- Cosmos Theme (Dark) --- */

static theme_t theme_build_cosmos(void)
{
    theme_t t;
    t.id = THEME_COSMOS;
    t.name = "Cosmos";

    /* Background: deep space blue-black */
    t.bg_space = color_rgba(0.024f, 0.027f, 0.036f, 1.0f);
    /* Surface: card/panel — slightly lighter */
    t.bg_surface = color_rgba(0.067f, 0.067f, 0.090f, 1.0f);
    /* Elevated: floating elements */
    t.bg_elevated = color_rgba(0.09f, 0.09f, 0.12f, 1.0f);

    /* Text: warm off-white, cascading alpha for hierarchy */
    t.text_primary = color_rgba(0.90f, 0.90f, 0.92f, 1.0f);
    t.text_secondary = color_rgba(0.90f, 0.90f, 0.92f,
                                  (float)PHI_INV);
    t.text_muted = color_rgba(0.90f, 0.90f, 0.92f,
                              (float)PHI_INV2);

    /* Brand: solar gold + celestial teal */
    t.brand_primary = color_rgba(1.0f, 0.85f, 0.55f, 1.0f);
    t.brand_secondary = color_rgba(0.2f, 0.75f, 0.8f, 1.0f);

    /* Accent: gold (same as brand primary) */
    t.accent = color_rgba(1.0f, 0.85f, 0.55f, 1.0f);

    /* Semantic: danger, success, warning */
    t.danger = color_rgba(0.85f, 0.25f, 0.25f, 1.0f);
    t.success = color_rgba(0.25f, 0.75f, 0.45f, 1.0f);
    t.warning = color_rgba(0.90f, 0.70f, 0.20f, 1.0f);

    /* Border: subtle divider */
    t.border = color_rgba(0.20f, 0.20f, 0.25f, 1.0f);

    /* Star glow: warm white-gold */
    t.star_glow = color_rgba(1.00f, 0.95f, 0.85f, 1.0f);

    /* Layout */
    theme_fill_layout(&t, THEME_BASE_FONT);

    return t;
}

/* --- Dawn Theme (Light) --- */

static theme_t theme_build_dawn(void)
{
    theme_t t;
    t.id = THEME_DAWN;
    t.name = "Dawn";

    /* Background: pale warm white */
    t.bg_space = color_rgba(0.96f, 0.94f, 0.91f, 1.0f);
    /* Surface: slightly cooler white */
    t.bg_surface = color_rgba(0.98f, 0.97f, 0.95f, 1.0f);
    /* Elevated: pure white */
    t.bg_elevated = color_rgba(1.0f, 1.0f, 0.99f, 1.0f);

    /* Text: dark charcoal, cascading alpha */
    t.text_primary = color_rgba(0.12f, 0.12f, 0.14f, 1.0f);
    t.text_secondary = color_rgba(0.12f, 0.12f, 0.14f,
                                  (float)PHI_INV);
    t.text_muted = color_rgba(0.12f, 0.12f, 0.14f,
                              (float)PHI_INV2);

    /* Brand: same solar gold + celestial teal */
    t.brand_primary = color_rgba(1.0f, 0.85f, 0.55f, 1.0f);
    t.brand_secondary = color_rgba(0.2f, 0.75f, 0.8f, 1.0f);

    /* Accent: gold */
    t.accent = color_rgba(0.90f, 0.72f, 0.20f, 1.0f);

    /* Semantic: slightly deeper for light bg contrast */
    t.danger = color_rgba(0.80f, 0.20f, 0.20f, 1.0f);
    t.success = color_rgba(0.15f, 0.60f, 0.35f, 1.0f);
    t.warning = color_rgba(0.80f, 0.60f, 0.10f, 1.0f);

    /* Border: light gray */
    t.border = color_rgba(0.80f, 0.80f, 0.82f, 1.0f);

    /* Star glow: warm gold on light background */
    t.star_glow = color_rgba(0.95f, 0.85f, 0.55f, 1.0f);

    /* Layout */
    theme_fill_layout(&t, THEME_BASE_FONT);

    return t;
}

/* --- Theme Construction --- */

theme_t theme_get(theme_id_t id)
{
    switch (id) {
    case THEME_DAWN:
        return theme_build_dawn();
    case THEME_COSMOS: /* fall through */
    default:
        return theme_build_cosmos();
    }
}

theme_t theme_default(void)
{
    return theme_get(THEME_COSMOS);
}

/* --- User Preferences --- */

theme_prefs_t theme_default_prefs(void)
{
    theme_prefs_t p;
    p.font_scale = 1.0f;
    p.opacity_scale = 1.0f;
    p.animation_speed = 1.0f;
    p.accent_hue = color_hsl(45.0f, 0.85f, 0.55f); /* gold default */
    p.use_custom_accent = 0;
    return p;
}

theme_prefs_t theme_validate_prefs(theme_prefs_t prefs)
{
    prefs.font_scale = clampf(prefs.font_scale, 0.5f, 2.0f);
    prefs.opacity_scale = clampf(prefs.opacity_scale, 0.5f, 1.0f);
    prefs.animation_speed = clampf(prefs.animation_speed, 0.5f, 2.0f);
    prefs.accent_hue.s = clampf(prefs.accent_hue.s, 0.0f, 1.0f);
    prefs.accent_hue.l = clampf(prefs.accent_hue.l, 0.0f, 1.0f);
    return prefs;
}

theme_t theme_apply_prefs(theme_t base, theme_prefs_t prefs)
{
    prefs = theme_validate_prefs(prefs);

    /* Font scale: recompute typography with scaled base */
    float scaled_font = THEME_BASE_FONT * prefs.font_scale;
    base.typography = gl_type_scale(scaled_font);
    base.icon_size = base.typography.sizes[3] * (float)PHI_RATIO;

    /* Opacity scale: multiply each level */
    for (int i = 0; i < PHI_OPACITY_LEVELS; i++) {
        base.opacity.values[i] *= prefs.opacity_scale;
        if (base.opacity.values[i] > 1.0f)
            base.opacity.values[i] = 1.0f;
    }

    /* Animation speed: multiply timing values */
    base.timing.micro *= prefs.animation_speed;
    base.timing.fast *= prefs.animation_speed;
    base.timing.normal *= prefs.animation_speed;
    base.timing.medium *= prefs.animation_speed;
    base.timing.slow *= prefs.animation_speed;
    base.timing.base *= prefs.animation_speed;
    base.timing.dramatic *= prefs.animation_speed;
    base.timing.epic *= prefs.animation_speed;

    /* Custom accent color */
    if (prefs.use_custom_accent) {
        color_rgb_t rgb = color_hsl_to_rgb(prefs.accent_hue);
        base.accent = color_rgba(rgb.r, rgb.g, rgb.b, 1.0f);
    }

    return base;
}

/* --- System-Aware Colors --- */

color_rgba_t theme_system_accent(const theme_t *theme, ct_system_t system)
{
    if (system < 0 || system >= CT_SYSTEM_COUNT) {
        return theme->accent;
    }
    color_rgb_t sys = ct_system_primary(system);
    return color_rgba(sys.r, sys.g, sys.b, 1.0f);
}

color_rgba_t theme_system_surface(const theme_t *theme, ct_system_t system)
{
    if (system < 0 || system >= CT_SYSTEM_COUNT) {
        return theme->bg_surface;
    }
    color_rgb_t sys = ct_system_primary(system);
    /* Blend surface with system primary at subtle tint level */
    color_rgba_t tinted;
    tinted.r = theme->bg_surface.r + (sys.r - theme->bg_surface.r) * THEME_SURFACE_TINT;
    tinted.g = theme->bg_surface.g + (sys.g - theme->bg_surface.g) * THEME_SURFACE_TINT;
    tinted.b = theme->bg_surface.b + (sys.b - theme->bg_surface.b) * THEME_SURFACE_TINT;
    tinted.a = theme->bg_surface.a;
    return tinted;
}

/* --- Theme Interpolation --- */

theme_t theme_lerp(theme_t a, theme_t b, float t)
{
    theme_t out = a; /* start with a's layout/identity */

    /* Interpolate all color fields */
    out.bg_space = rgba_lerp(a.bg_space, b.bg_space, t);
    out.bg_surface = rgba_lerp(a.bg_surface, b.bg_surface, t);
    out.bg_elevated = rgba_lerp(a.bg_elevated, b.bg_elevated, t);
    out.text_primary = rgba_lerp(a.text_primary, b.text_primary, t);
    out.text_secondary = rgba_lerp(a.text_secondary, b.text_secondary, t);
    out.text_muted = rgba_lerp(a.text_muted, b.text_muted, t);
    out.brand_primary = rgba_lerp(a.brand_primary, b.brand_primary, t);
    out.brand_secondary = rgba_lerp(a.brand_secondary, b.brand_secondary, t);
    out.accent = rgba_lerp(a.accent, b.accent, t);
    out.danger = rgba_lerp(a.danger, b.danger, t);
    out.success = rgba_lerp(a.success, b.success, t);
    out.warning = rgba_lerp(a.warning, b.warning, t);
    out.border = rgba_lerp(a.border, b.border, t);
    out.star_glow = rgba_lerp(a.star_glow, b.star_glow, t);

    /* Interpolate derived metrics */
    out.corner_radius = lerpf(a.corner_radius, b.corner_radius, t);
    out.border_width = lerpf(a.border_width, b.border_width, t);
    out.shadow_blur = lerpf(a.shadow_blur, b.shadow_blur, t);
    out.icon_size = lerpf(a.icon_size, b.icon_size, t);

    return out;
}

/* --- CSS Export --- */

int theme_to_css(const theme_t *theme, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;

    int written = 0;
    int remaining = buf_size - 1; /* reserve for null terminator */

    /* Macro to write one CSS custom property */
#define WRITE_CSS_VAR(name, c) do { \
    int n = snprintf(buf + written, (size_t)(remaining > 0 ? remaining : 0), \
        "--" name ": %.3f, %.3f, %.3f;\n", \
        (double)(c).r, (double)(c).g, (double)(c).b); \
    if (n > 0 && n < remaining) { written += n; remaining -= n; } \
    else if (n > 0) { written += remaining; remaining = 0; } \
} while (0)

    WRITE_CSS_VAR("bg-space", theme->bg_space);
    WRITE_CSS_VAR("bg-surface", theme->bg_surface);
    WRITE_CSS_VAR("bg-elevated", theme->bg_elevated);
    WRITE_CSS_VAR("text-primary", theme->text_primary);
    WRITE_CSS_VAR("text-secondary", theme->text_secondary);
    WRITE_CSS_VAR("text-muted", theme->text_muted);
    WRITE_CSS_VAR("brand-primary", theme->brand_primary);
    WRITE_CSS_VAR("brand-secondary", theme->brand_secondary);
    WRITE_CSS_VAR("accent", theme->accent);
    WRITE_CSS_VAR("danger", theme->danger);
    WRITE_CSS_VAR("success", theme->success);
    WRITE_CSS_VAR("warning", theme->warning);
    WRITE_CSS_VAR("border", theme->border);
    WRITE_CSS_VAR("star-glow", theme->star_glow);

#undef WRITE_CSS_VAR

    buf[written] = '\0';
    return written;
}

/* --- Theme Naming --- */

static const char *THEME_NAMES[THEME_COUNT] = {
    "Cosmos",
    "Dawn"
};

const char *theme_name(theme_id_t id)
{
    if (id < 0 || id >= THEME_COUNT) return "?";
    return THEME_NAMES[id];
}
