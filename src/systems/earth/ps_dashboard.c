/* ps_dashboard.c — Personal space dashboard layout.
 * Pure functions. No GL, no malloc, no globals, no side effects. */

#include "ps_dashboard.h"

#include <string.h>

/* --- Section titles --- */

static const char *s_titles[PS_SECTION_COUNT] = {
    "Active Cycles",
    "Today for You",
    "Cosmic Fingerprint",
    "Exploration Progress"
};

/* --- Helpers --- */

/* Check if point is inside rect */
static int rect_contains(gl_rect_t r, float x, float y) {
    return x >= r.x && x <= r.x + r.w &&
           y >= r.y && y <= r.y + r.h;
}

/* --- Layout computation --- */

ps_dashboard_t ps_compute(gl_rect_t viewport, float base_spacing) {
    ps_dashboard_t dash;
    dash.container = viewport;
    dash.gap = base_spacing * (float)PHI_INV;

    float half_gap = dash.gap * 0.5f;

    /* Step 1: Split viewport horizontally at golden ratio → top (major) + bottom (minor) */
    gl_rect_t top_row = gl_rect_major(viewport, 0);    /* height = h * phi^-1 */
    gl_rect_t bot_row = gl_rect_minor(viewport, 0);     /* height = h * (1 - phi^-1) */

    /* Apply half-gap between rows */
    top_row.h -= half_gap;
    bot_row.y += half_gap;
    bot_row.h -= half_gap;

    /* Step 2: Split each row vertically at golden ratio → left (major) + right (minor) */
    gl_rect_t tl = gl_rect_major(top_row, 1);
    gl_rect_t tr = gl_rect_minor(top_row, 1);
    gl_rect_t bl = gl_rect_major(bot_row, 1);
    gl_rect_t br = gl_rect_minor(bot_row, 1);

    /* Apply half-gap between columns */
    tl.w -= half_gap;
    tr.x += half_gap;
    tr.w -= half_gap;
    bl.w -= half_gap;
    br.x += half_gap;
    br.w -= half_gap;

    /* Assign sections */
    strncpy(dash.sections[PS_ACTIVE_CYCLES].title, s_titles[PS_ACTIVE_CYCLES],
            PS_TITLE_MAX - 1);
    dash.sections[PS_ACTIVE_CYCLES].title[PS_TITLE_MAX - 1] = '\0';
    dash.sections[PS_ACTIVE_CYCLES].bounds = tl;

    strncpy(dash.sections[PS_TODAY_FOR_YOU].title, s_titles[PS_TODAY_FOR_YOU],
            PS_TITLE_MAX - 1);
    dash.sections[PS_TODAY_FOR_YOU].title[PS_TITLE_MAX - 1] = '\0';
    dash.sections[PS_TODAY_FOR_YOU].bounds = tr;

    strncpy(dash.sections[PS_FINGERPRINT].title, s_titles[PS_FINGERPRINT],
            PS_TITLE_MAX - 1);
    dash.sections[PS_FINGERPRINT].title[PS_TITLE_MAX - 1] = '\0';
    dash.sections[PS_FINGERPRINT].bounds = bl;

    strncpy(dash.sections[PS_PROGRESS].title, s_titles[PS_PROGRESS],
            PS_TITLE_MAX - 1);
    dash.sections[PS_PROGRESS].title[PS_TITLE_MAX - 1] = '\0';
    dash.sections[PS_PROGRESS].bounds = br;

    return dash;
}

const ps_section_t *ps_section(const ps_dashboard_t *dash, ps_section_id_t id) {
    if (id < 0 || id >= PS_SECTION_COUNT) {
        return (const ps_section_t *)0;
    }
    return &dash->sections[id];
}

const char *ps_section_title(ps_section_id_t id) {
    if (id < 0 || id >= PS_SECTION_COUNT) {
        return "Unknown";
    }
    return s_titles[id];
}

int ps_hit_test(const ps_dashboard_t *dash, float x, float y) {
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        if (rect_contains(dash->sections[i].bounds, x, y)) {
            return i;
        }
    }
    return -1;
}
