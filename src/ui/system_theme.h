/* system_theme.h — Per-knowledge-system CSS custom properties
 *
 * Generates CSS custom properties for all 13 knowledge system
 * color palettes. Each system gets 4 properties: primary,
 * secondary, accent, surface. Colors are integer RGB (0-255)
 * in sRGB space, suitable for CSS rgb() usage.
 *
 * Dependencies:
 *   src/render/color_theory.h — system palettes (linear space)
 *   src/ui/theme.h            — theme-aware surface blending
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_SYSTEM_THEME_H
#define TIME_SYSTEM_THEME_H

#include "../render/color_theory.h"
#include "theme.h"

#define ST_SLUG_MAX 24
#define ST_CSS_BUF_SIZE 4096

/* Integer RGB for CSS output */
typedef struct {
    int r, g, b;
} st_rgb_t;

/* Complete color set for one knowledge system */
typedef struct {
    const char *slug;       /* "astronomy", "tzolkin", etc. */
    st_rgb_t primary;       /* system identity color */
    st_rgb_t secondary;     /* secondary color */
    st_rgb_t accent;        /* interactive accent */
    st_rgb_t surface;       /* theme-aware tinted surface */
} st_system_colors_t;

/* Get colors for one knowledge system.
 * system_id: ct_system_t enum value (0 to CT_SYSTEM_COUNT-1)
 * theme_id: theme_id_t enum value (THEME_COSMOS=0, THEME_DAWN=1)
 * Returns colors converted to 0-255 integer RGB (sRGB space).
 * Returns all-zero for invalid system_id. */
st_system_colors_t st_system_colors(int system_id, int theme_id);

/* Format CSS custom properties for ALL systems into buffer.
 * Format: "--sys-astronomy-primary: R, G, B;\n--sys-astronomy-secondary: R, G, B;\n..."
 * Each system gets 4 properties: primary, secondary, accent, surface.
 * 13 systems x 4 props = 52 CSS custom properties.
 * Returns chars written (excluding null terminator). */
int st_css_vars(int theme_id, char *buf, int buf_size);

/* Get system slug from ct_system_t index.
 * Returns "unknown" for invalid. */
const char *st_system_slug(int system_id);

/* Number of systems (CT_SYSTEM_COUNT). */
int st_system_count(void);

#endif /* TIME_SYSTEM_THEME_H */
