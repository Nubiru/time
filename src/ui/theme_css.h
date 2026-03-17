/* theme_css.h -- CSS custom property export with integer 0-255 values
 *
 * Converts theme_t color data to CSS custom property strings
 * with integer R,G,B values (0-255) suitable for rgb(var(--name)).
 *
 * Bridges the gap between theme_to_css() (float 0.0-1.0) and
 * the CSS convention of integer color components.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_THEME_CSS_H
#define TIME_THEME_CSS_H

#include "theme.h"

/* Generate CSS custom property declarations for a theme.
 * Colors output as 0-255 integers: "--bg-space: 6, 7, 9;\n"
 * Returns chars written (excluding null terminator). */
int theme_css_vars(const theme_t *theme, char *buf, int buf_size);

#endif /* TIME_THEME_CSS_H */
