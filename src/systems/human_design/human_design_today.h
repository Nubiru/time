/* human_design_today.h — Human Design "Today" transit page
 *
 * Shows the current transit: which gates the Sun and Earth activate
 * today based on the Sun's ecliptic longitude.  Not a birth chart —
 * this is the collective energy field for the day.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_HUMAN_DESIGN_TODAY_H
#define TIME_HUMAN_DESIGN_TODAY_H

#define HD_SECTION_MAX 512
#define HD_TITLE_MAX   128

typedef struct {
    char page_title[HD_TITLE_MAX];          /* "Human Design — Gate 1: The Creative" */
    int  sun_gate;                          /* 1-64 */
    int  earth_gate;                        /* 1-64 (opposite) */

    char gate_section[HD_SECTION_MAX];      /* Sun gate + Earth gate with names/keywords */
    char line_section[HD_SECTION_MAX];      /* Line detail (1-6) for sun/earth */
    char type_section[HD_SECTION_MAX];      /* Today's energy type context */
    char center_section[HD_SECTION_MAX];    /* Which centers the sun gate activates */
    char cross_section[HD_SECTION_MAX];     /* I Ching connection (gate = hexagram) */

    int  section_count;                     /* always 5 */
    int  has_data;                          /* 1 if valid */
} hdt_page_t;

/* Build HD "Today" page from Sun's ecliptic longitude.
 * sun_lon: tropical ecliptic longitude in degrees (0-360). */
hdt_page_t hdt_today(double sun_lon);

/* Format the page into a single text buffer.
 * Returns number of characters written (excluding NUL), or 0 on error. */
int hdt_format(const hdt_page_t *page, char *buf, int buf_size);

#endif /* TIME_HUMAN_DESIGN_TODAY_H */
