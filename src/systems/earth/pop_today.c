/* pop_today.c — Today-page for human population display.
 *
 * Composes human_population data into a display-ready page struct
 * showing humanity's current moment in population history.
 *
 * Pure functions: no globals, no malloc, no GL, no side effects. */

#include "pop_today.h"
#include "human_population.h"

#include <stdio.h>
#include <string.h>

/* --- Internal helpers --- */

/* Convert Julian Day to approximate calendar year.
 * J2000.0 = JD 2451545.0 = 2000-01-01 12:00 TT. */
static int jd_to_year(double jd)
{
    return (int)(2000.0 + (jd - 2451545.0) / 365.25);
}

/* Find the milestone index whose billion count matches `billions`.
 * Returns 0..POP_MILESTONE_COUNT-1, or -1 if not found. */
static int find_milestone_index(int billions)
{
    int count = pop_milestone_count();
    for (int i = 0; i < count; i++) {
        pop_milestone_t m = pop_milestone_get(i);
        if (m.billions == billions) {
            return i;
        }
    }
    return -1;
}

/* --- Public API --- */

pt_page_t pt_today(double jd)
{
    pt_page_t p;
    memset(&p, 0, sizeof(p));

    int year = jd_to_year(jd);

    /* Get full summary from human_population */
    pop_summary_t sum = pop_summary(year);

    /* Format alive population for title */
    char alive_str[64];
    pop_format_number(sum.alive_millions, alive_str, (int)sizeof(alive_str));

    /* --- page_title --- */
    snprintf(p.page_title, PT_TITLE_MAX,
             "Population \xe2\x80\x94 %s humans alive", alive_str);

    /* --- current_section --- */
    snprintf(p.current_section, PT_SECTION_MAX,
             "Current Population: %s\n"
             "Era: %s\n"
             "Year: %d\n"
             "%s",
             alive_str,
             sum.era_name,
             year,
             sum.summary);

    /* --- fraction_section --- */
    double alive_billions = sum.alive_millions / 1000.0;
    double percent = 0.0;
    if (sum.ever_born_billions > 0.0) {
        percent = (alive_billions / sum.ever_born_billions) * 100.0;
    }

    char ever_born_str[64];
    snprintf(ever_born_str, sizeof(ever_born_str),
             "%.0f", sum.ever_born_billions);

    snprintf(p.fraction_section, PT_SECTION_MAX,
             "You are alive during the most populated moment. "
             "%.1f/%.0f billion = %.1f%% of all humans "
             "who ever lived are alive RIGHT NOW.",
             alive_billions, sum.ever_born_billions, percent);

    /* --- milestone_section --- */
    int ms_idx = find_milestone_index(sum.nearest_milestone_billions);
    if (ms_idx >= 0) {
        pop_milestone_t ms = pop_milestone_get(ms_idx);
        int years_since = year - ms.year;

        /* Look for next milestone */
        pop_milestone_t next = pop_milestone_get(ms_idx + 1);
        if (next.billions > 0) {
            int years_until = next.year - year;
            snprintf(p.milestone_section, PT_SECTION_MAX,
                     "Latest Milestone: %d billion (%d)\n"
                     "Context: %s\n"
                     "Time since: %d years\n"
                     "Next billion: ~%d (in %d years)",
                     ms.billions, ms.year,
                     ms.context,
                     years_since,
                     next.year, years_until);
        } else {
            /* At the last known milestone */
            snprintf(p.milestone_section, PT_SECTION_MAX,
                     "Latest Milestone: %d billion (%d)\n"
                     "Context: %s\n"
                     "Time since: %d years\n"
                     "This is the most recent billion milestone.",
                     ms.billions, ms.year,
                     ms.context,
                     years_since);
        }
    } else {
        /* Pre-billion era */
        int first_billion_year = pop_year_of_billion(1);
        int years_until = first_billion_year - year;
        snprintf(p.milestone_section, PT_SECTION_MAX,
                 "No billion milestone reached yet.\n"
                 "First billion: %d (in %d years)",
                 first_billion_year,
                 years_until > 0 ? years_until : 0);
    }

    /* --- perspective_section --- */
    double pre_ag = pop_alive_at_year(-10000);
    double year1  = pop_alive_at_year(1);
    double now    = sum.alive_millions;

    char pre_ag_str[64];
    char year1_str[64];
    char now_str[64];
    pop_format_number(pre_ag, pre_ag_str, (int)sizeof(pre_ag_str));
    pop_format_number(year1,  year1_str,  (int)sizeof(year1_str));
    pop_format_number(now,    now_str,    (int)sizeof(now_str));

    double growth_factor = 0.0;
    if (pre_ag > 0.0) {
        growth_factor = now / pre_ag;
    }

    snprintf(p.perspective_section, PT_SECTION_MAX,
             "Pre-agriculture: %s. "
             "Year 1 CE: %s. "
             "Now: %s. "
             "Growth factor: %.0fx.",
             pre_ag_str, year1_str, now_str, growth_factor);

    p.section_count = 5;
    p.has_data = 1;
    return p;
}

int pt_format(const pt_page_t *page, char *buf, int buf_size)
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
                     "%s",
                     page->page_title,
                     page->current_section,
                     page->fraction_section,
                     page->milestone_section,
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
