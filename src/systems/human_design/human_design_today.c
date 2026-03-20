/* human_design_today.c — Human Design "Today" transit page
 *
 * Pure data module: builds a transit page from the Sun's ecliptic
 * longitude using the Human Design gate wheel and interpretation data.
 * No globals, no malloc, no side effects. */

#include "human_design_today.h"
#include "human_design.h"
#include "human_design_interpret.h"

#include <stdio.h>
#include <string.h>

/* ================================================================
 * Internal section builders
 * ================================================================ */

static void build_page_title(hdt_page_t *page, int sun_gate)
{
    const char *name = hd_gate_name(sun_gate);
    snprintf(page->page_title, HD_TITLE_MAX,
             "Human Design \xe2\x80\x94 Gate %d: %s",
             sun_gate, name);
}

static void build_gate_section(hdt_page_t *page,
                               const hd_incarnation_t *ic)
{
    const char *sun_name   = hd_gate_name(ic->sun.gate);
    const char *sun_kw     = hd_gate_keyword(ic->sun.gate);
    const char *earth_name = hd_gate_name(ic->earth.gate);
    const char *earth_kw   = hd_gate_keyword(ic->earth.gate);

    snprintf(page->gate_section, HD_SECTION_MAX,
             "Sun Gate: %d \xe2\x80\x94 %s\n"
             "Keyword: %s\n"
             "Line: %d.%d\n"
             "Earth Gate: %d \xe2\x80\x94 %s\n"
             "Keyword: %s",
             ic->sun.gate, sun_name,
             sun_kw,
             ic->sun.gate, ic->sun.line,
             ic->earth.gate, earth_name,
             earth_kw);
}

static void build_line_section(hdt_page_t *page,
                               const hd_incarnation_t *ic)
{
    hdi_profile_t prof = hdi_profile_data(ic->sun.line, ic->earth.line);

    if (prof.line1 < 0) {
        /* Invalid profile combination — show lines only */
        snprintf(page->line_section, HD_SECTION_MAX,
                 "Sun Line: %d of 6\n"
                 "Earth Line: %d of 6\n"
                 "Profile: %d/%d",
                 ic->sun.line, ic->earth.line,
                 ic->sun.line, ic->earth.line);
    } else {
        snprintf(page->line_section, HD_SECTION_MAX,
                 "Sun Line: %d of 6\n"
                 "Earth Line: %d of 6\n"
                 "Profile: %s \xe2\x80\x94 %s",
                 ic->sun.line, ic->earth.line,
                 prof.name, prof.theme);
    }
}

static void build_type_section(hdt_page_t *page)
{
    hdi_type_t gen = hdi_type_data(HDI_GENERATOR);

    snprintf(page->type_section, HD_SECTION_MAX,
             "Transit Energy: %s\n"
             "Strategy: %s\n"
             "Signature: %s\n"
             "Not-Self: %s",
             gen.name, gen.strategy, gen.signature, gen.not_self);
}

static void build_center_section(hdt_page_t *page)
{
    hdi_center_theme_t c0 = hdi_center_data(0); /* Head */
    hdi_center_theme_t c1 = hdi_center_data(1); /* Ajna */
    hdi_center_theme_t c2 = hdi_center_data(2); /* Throat */

    snprintf(page->center_section, HD_SECTION_MAX,
             "Centers:\n"
             "  %s: %s\n"
             "  %s: %s\n"
             "  %s: %s",
             c0.name, c0.theme_defined,
             c1.name, c1.theme_defined,
             c2.name, c2.theme_defined);
}

static void build_cross_section(hdt_page_t *page, int sun_gate)
{
    snprintf(page->cross_section, HD_SECTION_MAX,
             "I Ching Connection: Hexagram %d\n"
             "Gate %d = Hexagram %d in the I Ching\n"
             "The 64 Human Design gates map directly to the 64 hexagrams.",
             sun_gate, sun_gate, sun_gate);
}

/* ================================================================
 * Public API
 * ================================================================ */

hdt_page_t hdt_today(double sun_lon)
{
    hdt_page_t page;
    memset(&page, 0, sizeof(page));

    hd_incarnation_t ic = hd_incarnation(sun_lon);

    page.sun_gate   = ic.sun.gate;
    page.earth_gate = ic.earth.gate;

    build_page_title(&page, ic.sun.gate);
    build_gate_section(&page, &ic);
    build_line_section(&page, &ic);
    build_type_section(&page);
    build_center_section(&page);
    build_cross_section(&page, ic.sun.gate);

    page.section_count = 5;
    page.has_data      = 1;

    return page;
}

int hdt_format(const hdt_page_t *page, char *buf, int buf_size)
{
    if (!page || !buf || buf_size <= 0)
        return 0;

    int n = snprintf(buf, (size_t)buf_size,
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s",
                     page->page_title,
                     page->gate_section,
                     page->line_section,
                     page->type_section,
                     page->center_section,
                     page->cross_section);

    if (n < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (n >= buf_size)
        n = buf_size - 1;

    return n;
}
