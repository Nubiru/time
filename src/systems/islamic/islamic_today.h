/* islamic_today.h — Islamic "Today" page
 *
 * Builds a rich daily page with Hijri date, month significance,
 * sacred month status, prayer times, and next upcoming prayer.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_ISLAMIC_TODAY_H
#define TIME_ISLAMIC_TODAY_H

#define IT_SECTION_MAX 512
#define IT_TITLE_MAX   128

/* Location input for prayer times */
typedef struct {
    double lat;   /* degrees, negative for south */
    double lon;   /* degrees, negative for west */
} it_location_t;

typedef struct {
    /* Page header */
    char page_title[IT_TITLE_MAX];         /* "Islamic -- 15 Ramadan 1446" */

    /* Sections */
    char date_section[IT_SECTION_MAX];     /* Full date, day of year, leap status */
    char month_section[IT_SECTION_MAX];    /* Month significance, sacred status, events */
    char prayer_section[IT_SECTION_MAX];   /* All 6 prayer times formatted as HH:MM */
    char next_prayer_line[IT_SECTION_MAX]; /* "Next prayer: Maghrib at 18:32" */
    char sacred_section[IT_SECTION_MAX];   /* Sacred month context or general guidance */

    int section_count;   /* always 5 */
    int is_sacred_month;
    int has_data;
} it_page_t;

/* Build Islamic "Today" page. Uses MWL method and Shafi'i Asr by default.
 * If loc.lat and loc.lon are both 0.0, prayer section shows "Location required". */
it_page_t it_today(double jd, it_location_t loc);

/* Format page as multi-line text. Returns chars written, 0 on error. */
int it_format(const it_page_t *page, char *buf, int buf_size);

#endif /* TIME_ISLAMIC_TODAY_H */
