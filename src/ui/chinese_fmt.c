#include "chinese_fmt.h"
#include "../systems/chinese/chinese.h"

#include <stdio.h>
#include <string.h>

void chinese_fmt_title(int animal, int element, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *sym = chinese_animal_symbol(animal);
    const char *elem = chinese_element_name(element);
    const char *anim = chinese_animal_name(animal);
    snprintf(buf, sz, "%s Year of the %s %s", sym, elem, anim);
}

void chinese_fmt_stem_branch(int stem, int branch, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *s = chinese_stem_name(stem);
    const char *b = chinese_branch_name(branch);
    snprintf(buf, sz, "%s-%s", s, b);
}

void chinese_fmt_element(int element, int polarity, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *elem = chinese_element_name(element);
    const char *pol = chinese_polarity_name(polarity);
    snprintf(buf, sz, "%s \xc2\xb7 %s", elem, pol);
}

void chinese_fmt_cycle(int cycle_year, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "Year %d of 60", cycle_year);
}

void chinese_fmt_animal(int animal, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *sym = chinese_animal_symbol(animal);
    const char *name = chinese_animal_name(animal);
    snprintf(buf, sz, "%s %s", sym, name);
}

static chinese_display_t build_display(chinese_year_t cy)
{
    chinese_display_t d;
    memset(&d, 0, sizeof(d));

    chinese_fmt_title(cy.animal, cy.element, d.title, sizeof(d.title));
    chinese_fmt_stem_branch(cy.stem, cy.branch,
                            d.stem_branch, sizeof(d.stem_branch));
    chinese_fmt_element(cy.element, cy.polarity,
                        d.element_line, sizeof(d.element_line));
    chinese_fmt_cycle(cy.cycle_year, d.cycle_line, sizeof(d.cycle_line));
    chinese_fmt_animal(cy.animal, d.animal_line, sizeof(d.animal_line));

    const char *sym = chinese_animal_symbol(cy.animal);
    const char *elem = chinese_element_name(cy.element);
    const char *anim = chinese_animal_name(cy.animal);
    const char *stem = chinese_stem_name(cy.stem);
    const char *branch = chinese_branch_name(cy.branch);
    const char *pol = chinese_polarity_name(cy.polarity);
    snprintf(d.summary, sizeof(d.summary), "%s %s %s \xc2\xb7 %s-%s \xc2\xb7 %s",
             sym, elem, anim, stem, branch, pol);

    return d;
}

chinese_display_t chinese_display_from_year(int year)
{
    chinese_year_t cy = chinese_year_from_gregorian(year);
    return build_display(cy);
}

chinese_display_t chinese_display_from_jd(double jd)
{
    chinese_year_t cy = chinese_year_from_jd(jd);
    return build_display(cy);
}
