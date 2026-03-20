/* geology_today.h — Current geological moment page.
 *
 * Composes geo_time + geology_interpret into a display-ready page
 * showing the full geological hierarchy, nearest extinction event,
 * deep time perspective, and interpretive narrative.
 *
 * Pure functions: no globals, no malloc, no GL, no side effects. */

#ifndef TIME_GEOLOGY_TODAY_H
#define TIME_GEOLOGY_TODAY_H

#define GL_SECTION_MAX 512
#define GL_TITLE_MAX  128

typedef struct {
    char page_title[GL_TITLE_MAX];           /* "Geology — Phanerozoic: Age of Visible Life" */

    char hierarchy_section[GL_SECTION_MAX];  /* Eon → Era → Period → Epoch */
    char eon_section[GL_SECTION_MAX];        /* Eon meaning, character, life state */
    char extinction_section[GL_SECTION_MAX]; /* Nearest mass extinction + aftermath */
    char deep_time_section[GL_SECTION_MAX];  /* Earth age context, timeline position */
    char perspective_section[GL_SECTION_MAX]; /* Meaning from interpret */

    int section_count;  /* always 5 */
    int has_data;       /* 1 if valid, 0 if lookup failed */
} gl_page_t;

/* Build the geology page for a given Julian Day.
 * Converts JD to Ma, clamps to 0.0 for present, looks up all layers. */
gl_page_t gl_today(double jd);

/* Format the entire page into a single buffer.
 * Returns number of chars written (excluding NUL), or 0 on error. */
int gl_format(const gl_page_t *page, char *buf, int buf_size);

#endif /* TIME_GEOLOGY_TODAY_H */
