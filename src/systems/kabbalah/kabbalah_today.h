/* kabbalah_today.h — Kabbalah "Today" page
 *
 * Builds a complete daily Kabbalah page: sefirah of the day, world,
 * path of the day, meditation focus, and tree position.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_KABBALAH_TODAY_H
#define TIME_KABBALAH_TODAY_H

#define KT_SECTION_MAX 512
#define KT_TITLE_MAX   128

typedef struct {
    /* Page header */
    char page_title[KT_TITLE_MAX];          /* "Kabbalah — Tiferet (Beauty)" */
    int  sefirah_index;                      /* 0-9 */
    int  path_index;                         /* 0-21 */

    /* Sections */
    char sefirah_section[KT_SECTION_MAX];   /* Name, meaning, pillar, triad, planet */
    char world_section[KT_SECTION_MAX];     /* Which of 4 worlds contains this sefirah */
    char path_section[KT_SECTION_MAX];      /* Today's path: from-to, Hebrew letter, Tarot */
    char meditation_section[KT_SECTION_MAX]; /* Meditation focus + contemplative question */
    char tree_section[KT_SECTION_MAX];      /* Middle pillar status, position on Tree */

    int  section_count;                      /* always 5 */
    int  is_middle_pillar;                   /* 1 if today's sefirah is on central axis */
    int  has_data;
} kt_page_t;

/* Build Kabbalah "Today" page for a Julian Day. */
kt_page_t kt_today(double jd);

/* Format page as multi-line text. Returns chars written, 0 on error. */
int kt_format(const kt_page_t *page, char *buf, int buf_size);

#endif /* TIME_KABBALAH_TODAY_H */
