/* system_theme.c — Per-knowledge-system CSS custom properties
 *
 * Converts color_theory.h system palettes (linear float) to
 * integer sRGB CSS custom properties for DOM HUD usage.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "system_theme.h"

#include <math.h>
#include <stdio.h>

/* Slug lookup table — indexed by ct_system_t */
static const char *s_slugs[CT_SYSTEM_COUNT] = {
    "astronomy",      /* CT_SYSTEM_ASTRONOMY */
    "astrology",      /* CT_SYSTEM_ASTROLOGY */
    "tzolkin",        /* CT_SYSTEM_TZOLKIN */
    "iching",         /* CT_SYSTEM_ICHING */
    "chinese",        /* CT_SYSTEM_CHINESE */
    "human-design",   /* CT_SYSTEM_HUMAN_DESIGN */
    "gregorian",      /* CT_SYSTEM_GREGORIAN */
    "kabbalah",       /* CT_SYSTEM_KABBALAH */
    "hindu",          /* CT_SYSTEM_HINDU */
    "buddhist",       /* CT_SYSTEM_BUDDHIST */
    "hebrew",         /* CT_SYSTEM_HEBREW */
    "islamic",        /* CT_SYSTEM_ISLAMIC */
    "geology",        /* CT_SYSTEM_GEOLOGY */
    "coptic",         /* CT_SYSTEM_COPTIC */
    "ethiopian",      /* CT_SYSTEM_ETHIOPIAN */
    "persian",        /* CT_SYSTEM_PERSIAN */
    "japanese",       /* CT_SYSTEM_JAPANESE */
    "korean",         /* CT_SYSTEM_KOREAN */
    "thai"            /* CT_SYSTEM_THAI */
};

/* Convert a float channel (0.0-1.0) to integer (0-255) with rounding */
static int to_byte(float v)
{
    int i = (int)round((double)v * 255.0);
    if (i < 0) return 0;
    if (i > 255) return 255;
    return i;
}

/* Convert linear-space color_rgb_t to integer sRGB */
static st_rgb_t linear_to_int_srgb(color_rgb_t linear)
{
    color_rgb_t srgb = ct_to_srgb(linear);
    st_rgb_t out;
    out.r = to_byte(srgb.r);
    out.g = to_byte(srgb.g);
    out.b = to_byte(srgb.b);
    return out;
}

/* Convert color_rgba_t (theme surface blend) to integer RGB.
 * Theme surfaces are stored in sRGB-like space; the 5% linear
 * tint is negligible, so direct byte conversion matches theme_css. */
static st_rgb_t surface_to_int(color_rgba_t c)
{
    st_rgb_t out;
    out.r = to_byte(c.r);
    out.g = to_byte(c.g);
    out.b = to_byte(c.b);
    return out;
}

const char *st_system_slug(int system_id)
{
    if (system_id < 0 || system_id >= CT_SYSTEM_COUNT) {
        return "unknown";
    }
    return s_slugs[system_id];
}

int st_system_count(void)
{
    return CT_SYSTEM_COUNT;
}

st_system_colors_t st_system_colors(int system_id, int theme_id)
{
    st_system_colors_t result;
    result.slug = NULL;
    result.primary = (st_rgb_t){0, 0, 0};
    result.secondary = (st_rgb_t){0, 0, 0};
    result.accent = (st_rgb_t){0, 0, 0};
    result.surface = (st_rgb_t){0, 0, 0};

    if (system_id < 0 || system_id >= CT_SYSTEM_COUNT) {
        return result;
    }
    if (theme_id < 0 || theme_id >= THEME_COUNT) {
        return result;
    }

    ct_system_t sys = (ct_system_t)system_id;
    theme_t theme = theme_get((theme_id_t)theme_id);

    result.slug = s_slugs[system_id];
    result.primary = linear_to_int_srgb(ct_system_primary(sys));
    result.secondary = linear_to_int_srgb(ct_system_secondary(sys));
    result.accent = linear_to_int_srgb(ct_system_accent(sys));

    /* Surface: theme-aware tinted background */
    color_rgba_t surf = theme_system_surface(&theme, sys);
    result.surface = surface_to_int(surf);

    return result;
}

int st_css_vars(int theme_id, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;

    int written = 0;
    int remaining = buf_size - 1; /* reserve null terminator */

    for (int i = 0; i < CT_SYSTEM_COUNT; i++) {
        st_system_colors_t c = st_system_colors(i, theme_id);
        if (!c.slug) continue;

        const char *slug = c.slug;

        /* Write 4 properties per system */
        int n;

        n = snprintf(buf + written,
                     (size_t)(remaining > 0 ? remaining : 0),
                     "--sys-%s-primary: %d, %d, %d;\n",
                     slug, c.primary.r, c.primary.g, c.primary.b);
        if (n > 0 && n < remaining) {
            written += n; remaining -= n;
        } else if (n > 0) {
            written += remaining; remaining = 0;
        }

        n = snprintf(buf + written,
                     (size_t)(remaining > 0 ? remaining : 0),
                     "--sys-%s-secondary: %d, %d, %d;\n",
                     slug, c.secondary.r, c.secondary.g, c.secondary.b);
        if (n > 0 && n < remaining) {
            written += n; remaining -= n;
        } else if (n > 0) {
            written += remaining; remaining = 0;
        }

        n = snprintf(buf + written,
                     (size_t)(remaining > 0 ? remaining : 0),
                     "--sys-%s-accent: %d, %d, %d;\n",
                     slug, c.accent.r, c.accent.g, c.accent.b);
        if (n > 0 && n < remaining) {
            written += n; remaining -= n;
        } else if (n > 0) {
            written += remaining; remaining = 0;
        }

        n = snprintf(buf + written,
                     (size_t)(remaining > 0 ? remaining : 0),
                     "--sys-%s-surface: %d, %d, %d;\n",
                     slug, c.surface.r, c.surface.g, c.surface.b);
        if (n > 0 && n < remaining) {
            written += n; remaining -= n;
        } else if (n > 0) {
            written += remaining; remaining = 0;
        }
    }

    buf[written] = '\0';
    return written;
}
