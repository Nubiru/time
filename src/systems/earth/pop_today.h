/* pop_today.h — Today-page for human population display.
 *
 * Composes human_population data into a display-ready page
 * showing current population, historical fraction, milestones,
 * and deep time perspective.
 *
 * Pure functions: no globals, no malloc, no GL, no side effects. */

#ifndef TIME_POP_TODAY_H
#define TIME_POP_TODAY_H

#define PT_SECTION_MAX 512
#define PT_TITLE_MAX   128

typedef struct {
    char page_title[PT_TITLE_MAX];              /* "Population — 8.1 billion humans alive" */

    char current_section[PT_SECTION_MAX];       /* Current pop, growth context, era */
    char fraction_section[PT_SECTION_MAX];      /* Alive / ever born fraction */
    char milestone_section[PT_SECTION_MAX];     /* Nearest milestone, time since/until */
    char perspective_section[PT_SECTION_MAX];   /* Deep time perspective */

    int section_count;  /* always 5 (title + 4 sections) */
    int has_data;       /* 1 if valid, 0 if lookup failed */
} pt_page_t;

/* Build the population page for a given Julian Day.
 * Converts JD to approximate year, queries human_population. */
pt_page_t pt_today(double jd);

/* Format the entire page into a single buffer.
 * Returns number of chars written (excluding NUL), or 0 on error. */
int pt_format(const pt_page_t *page, char *buf, int buf_size);

#endif /* TIME_POP_TODAY_H */
