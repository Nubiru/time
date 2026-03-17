/* theme_css.c -- CSS custom property export with integer 0-255 values
 *
 * Converts each color_rgba_t field in theme_t to an integer RGB triplet
 * and formats as CSS custom property declarations.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "theme_css.h"

#include <math.h>
#include <stdio.h>

/* Convert a float channel (0.0-1.0) to integer (0-255) with rounding */
static int to_byte(float v)
{
    int i = (int)round((double)v * 255.0);
    if (i < 0) return 0;
    if (i > 255) return 255;
    return i;
}

int theme_css_vars(const theme_t *theme, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;

    int written = 0;
    int remaining = buf_size - 1; /* reserve null terminator */

#define WRITE_INT_VAR(name, c) do { \
    int _r = to_byte((c).r); \
    int _g = to_byte((c).g); \
    int _b = to_byte((c).b); \
    int _n = snprintf(buf + written, \
        (size_t)(remaining > 0 ? remaining : 0), \
        "--" name ": %d, %d, %d;\n", _r, _g, _b); \
    if (_n > 0 && _n < remaining) { \
        written += _n; remaining -= _n; \
    } else if (_n > 0) { \
        written += remaining; remaining = 0; \
    } \
} while (0)

    WRITE_INT_VAR("bg-space",         theme->bg_space);
    WRITE_INT_VAR("bg-surface",       theme->bg_surface);
    WRITE_INT_VAR("bg-elevated",      theme->bg_elevated);
    WRITE_INT_VAR("text-primary",     theme->text_primary);
    WRITE_INT_VAR("text-secondary",   theme->text_secondary);
    WRITE_INT_VAR("text-muted",       theme->text_muted);
    WRITE_INT_VAR("brand-primary",    theme->brand_primary);
    WRITE_INT_VAR("brand-secondary",  theme->brand_secondary);
    WRITE_INT_VAR("accent",           theme->accent);
    WRITE_INT_VAR("danger",           theme->danger);
    WRITE_INT_VAR("success",          theme->success);
    WRITE_INT_VAR("warning",          theme->warning);
    WRITE_INT_VAR("border",           theme->border);
    WRITE_INT_VAR("star-glow",        theme->star_glow);

#undef WRITE_INT_VAR

    buf[written] = '\0';
    return written;
}
