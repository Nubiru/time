/* chinese_today.c — Chinese Calendar "Today" page implementation
 *
 * Composes a rich yearly page from chinese.h and chinese_interpret.h data.
 * All functions are pure.
 *
 * No globals, no malloc, no GL, no side effects. */

#include "chinese_today.h"
#include "chinese.h"
#include "chinese_interpret.h"

#include <stdio.h>
#include <string.h>

/* ===== Cycle phase name ===== */

static const char *cycle_phase_name(int cycle_year)
{
    if (cycle_year <= 15) return "Early";
    if (cycle_year <= 30) return "Middle";
    if (cycle_year <= 45) return "Late";
    return "Final";
}

/* ===== Build sections ===== */

static void build_year_section(char *buf, int size, const chinese_year_t *cy)
{
    const char *elem = chinese_element_name(cy->element);
    const char *anim = chinese_animal_name(cy->animal);
    const char *pol  = chinese_polarity_name(cy->polarity);
    const char *stem = chinese_stem_name(cy->stem);
    const char *branch = chinese_branch_name(cy->branch);

    snprintf(buf, (size_t)size,
             "Year: %s %s (%s)\n"
             "Stem-Branch: %s-%s\n"
             "Sexagenary Cycle: Year %d of 60",
             elem, anim, pol,
             stem, branch,
             cy->cycle_year);
}

static void build_animal_section(char *buf, int size, const chinese_year_t *cy)
{
    const char *emoji = chinese_animal_symbol(cy->animal);
    ci_animal_t a = ci_animal_data(cy->animal);

    snprintf(buf, (size_t)size,
             "Animal: %s %s\n"
             "Archetype: %s\n"
             "Strengths: %s\n"
             "Challenges: %s",
             emoji, a.name,
             a.archetype,
             a.strengths,
             a.challenges);
}

static void build_element_section(char *buf, int size, const chinese_year_t *cy)
{
    ci_element_t e = ci_element_data(cy->element);

    snprintf(buf, (size_t)size,
             "Element: %s\n"
             "Nature: %s\n"
             "Season: %s\n"
             "Expression: %s",
             e.name,
             e.nature,
             e.season,
             e.expression);
}

static void build_compat_section(char *buf, int size, const chinese_year_t *cy)
{
    ci_animal_t a = ci_animal_data(cy->animal);

    snprintf(buf, (size_t)size,
             "Compatible Animals: %s",
             a.compatibility);
}

static void build_cycle_section(char *buf, int size, const chinese_year_t *cy)
{
    int remaining = 60 - cy->cycle_year;
    const char *phase = cycle_phase_name(cy->cycle_year);

    snprintf(buf, (size_t)size,
             "Cycle Position: Year %d of 60\n"
             "Years until next cycle: %d\n"
             "Current phase: %s",
             cy->cycle_year,
             remaining,
             phase);
}

/* ===== Public API ===== */

ct_page_t ct_today(double jd)
{
    ct_page_t page;
    memset(&page, 0, sizeof(page));

    chinese_year_t cy = chinese_year_from_jd(jd);

    const char *emoji = chinese_animal_symbol(cy.animal);
    const char *elem  = chinese_element_name(cy.element);
    const char *anim  = chinese_animal_name(cy.animal);

    /* Page title: "{emoji} Year of the {Element} {Animal}" */
    snprintf(page.page_title, CT_TITLE_MAX,
             "%s Year of the %s %s",
             emoji, elem, anim);

    /* Build all 5 sections */
    build_year_section(page.year_section, CT_SECTION_MAX, &cy);
    build_animal_section(page.animal_section, CT_SECTION_MAX, &cy);
    build_element_section(page.element_section, CT_SECTION_MAX, &cy);
    build_compat_section(page.compat_section, CT_SECTION_MAX, &cy);
    build_cycle_section(page.cycle_section, CT_SECTION_MAX, &cy);

    page.section_count = 5;
    page.has_data = 1;

    return page;
}

int ct_format(const ct_page_t *page, char *buf, int buf_size)
{
    if (page == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
                           "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
                           page->page_title,
                           page->year_section,
                           page->animal_section,
                           page->element_section,
                           page->compat_section,
                           page->cycle_section);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }

    /* snprintf may have truncated */
    if (written >= buf_size) {
        written = buf_size - 1;
    }

    return written;
}
