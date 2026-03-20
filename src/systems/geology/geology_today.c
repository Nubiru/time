/* geology_today.c — Current geological moment page.
 *
 * Composes geo_time + geology_interpret data into a display-ready
 * page struct showing Earth's present position in deep time.
 *
 * Pure functions: no globals, no malloc, no GL, no side effects. */

#include "geology_today.h"
#include "geo_time.h"
#include "geology_interpret.h"

#include <stdio.h>
#include <string.h>

/* --- Internal helpers --- */

/* Find extinction index by matching name from geo_extinction_nearest.
 * Returns 0-4, or -1 if not found. */
static int find_extinction_index(const char *name)
{
    int count = geo_extinction_count();
    for (int i = 0; i < count; i++) {
        geo_extinction_t ext = geo_extinction_by_index(i);
        if (ext.name != NULL && strcmp(ext.name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* --- Public API --- */

gl_page_t gl_today(double jd)
{
    gl_page_t p;
    memset(&p, 0, sizeof(p));

    /* Convert JD to Ma, clamp negative to 0.0 (geologically "now") */
    double ma = geo_jd_to_ma(jd);
    if (ma < 0.0) {
        ma = 0.0;
    }

    /* Look up all hierarchy levels */
    geo_unit_t eon    = geo_eon_at(ma);
    geo_unit_t era    = geo_era_at(ma);
    geo_unit_t period = geo_period_at(ma);
    geo_unit_t epoch  = geo_epoch_at(ma);

    if (eon.name == NULL) {
        p.has_data = 0;
        return p;
    }

    /* Get interpretation data — Phanerozoic is eon index 3 */
    gli_eon_t eon_interp = gli_eon_data(3);

    /* Nearest extinction */
    geo_extinction_t nearest = geo_extinction_nearest(ma);
    int ext_idx = find_extinction_index(nearest.name);
    gli_extinction_t ext_interp = gli_extinction_data(ext_idx);

    /* Compose interpretation */
    geology_interp_t interp = gli_interpret(3, ext_idx, -1);

    /* --- page_title --- */
    snprintf(p.page_title, GL_TITLE_MAX,
             "Geology \xe2\x80\x94 %s: Age of Visible Life",
             eon.name);

    /* --- hierarchy_section --- */
    snprintf(p.hierarchy_section, GL_SECTION_MAX,
             "Geological Position:\n"
             "  Eon: %s (%.0f \xe2\x80\x94 %.0f Ma)\n"
             "  Era: %s (%.1f \xe2\x80\x94 %.1f Ma)\n"
             "  Period: %s (%.1f \xe2\x80\x94 %.1f Ma)\n"
             "  Epoch: %s (%.4f \xe2\x80\x94 %.4f Ma)",
             eon.name, eon.start_ma, eon.end_ma,
             era.name  ? era.name  : "?", era.start_ma, era.end_ma,
             period.name ? period.name : "?", period.start_ma, period.end_ma,
             epoch.name  ? epoch.name  : "?", epoch.start_ma, epoch.end_ma);

    /* --- eon_section --- */
    snprintf(p.eon_section, GL_SECTION_MAX,
             "Eon: %s \xe2\x80\x94 %s\n"
             "Character: %s\n"
             "Life: %s\n"
             "%s",
             eon_interp.name, eon_interp.meaning,
             eon_interp.character,
             eon_interp.life,
             eon_interp.brief);

    /* --- extinction_section --- */
    snprintf(p.extinction_section, GL_SECTION_MAX,
             "Nearest Mass Extinction: %s\n"
             "Time: %.1f million years ago\n"
             "Cause: %s\n"
             "Aftermath: %s",
             ext_interp.name,
             nearest.ma,
             ext_interp.cause,
             ext_interp.aftermath);

    /* --- deep_time_section --- */
    snprintf(p.deep_time_section, GL_SECTION_MAX,
             "Earth Age: 4,540 million years\n"
             "Time Since Last Extinction: %.0f million years\n"
             "Perspective: If Earth's history were 24 hours, "
             "humans appeared in the last 4 seconds.",
             nearest.ma);

    /* --- perspective_section --- */
    snprintf(p.perspective_section, GL_SECTION_MAX, "%s", interp.detail);

    p.section_count = 5;
    p.has_data = 1;
    return p;
}

int gl_format(const gl_page_t *page, char *buf, int buf_size)
{
    if (page == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    if (!page->has_data) {
        return 0;
    }

    int n = snprintf(buf, (size_t)buf_size,
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s",
                     page->page_title,
                     page->hierarchy_section,
                     page->eon_section,
                     page->extinction_section,
                     page->deep_time_section,
                     page->perspective_section);

    if (n < 0) {
        buf[0] = '\0';
        return 0;
    }

    /* If truncated, return actual written count */
    if (n >= buf_size) {
        return buf_size - 1;
    }

    return n;
}
