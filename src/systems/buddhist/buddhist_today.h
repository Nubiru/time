/* buddhist_today.h — Buddhist "Today" page
 *
 * Builds a rich daily page with Buddhist Era, Uposatha observance,
 * lunar month, kalpa cosmic cycle, and practice guidance.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BUDDHIST_TODAY_H
#define TIME_BUDDHIST_TODAY_H

#define BT_SECTION_MAX 512
#define BT_TITLE_MAX   128

typedef struct {
    char page_title[BT_TITLE_MAX];          /* "Buddhist — BE 2569" */
    char era_section[BT_SECTION_MAX];       /* BE year, CE equivalent */
    char uposatha_section[BT_SECTION_MAX];  /* Current observance status, moon phase */
    char month_section[BT_SECTION_MAX];     /* Lunar month, Pali name, Vassa status */
    char kalpa_section[BT_SECTION_MAX];     /* Cosmic cycle position */
    char practice_section[BT_SECTION_MAX];  /* Practice guidance from interpret */
    int section_count;  /* always 5 */
    int is_uposatha;    /* 1 if today is an observance day */
    int is_vassa;       /* 1 if in Rains Retreat */
    int has_data;
} bt_page_t;

/* Build Buddhist "Today" page for the given Julian Day. */
bt_page_t bt_today(double jd);

/* Format page as multi-line text. Returns chars written, 0 on error. */
int bt_format(const bt_page_t *page, char *buf, int buf_size);

#endif /* TIME_BUDDHIST_TODAY_H */
