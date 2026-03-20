/* tzolkin_today.h — Full Tzolkin "Today" page builder.
 *
 * Assembles a rich daily page from the Dreamspell/Arguelles system:
 * kin identity, oracle relationships, wavespell context, castle position,
 * earth family, Calendar Round (Tzolkin + Haab), and 52-year cycle.
 *
 * Pure functions: no globals, no malloc, no GL, no side effects.
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 */

#ifndef TIME_TZOLKIN_TODAY_H
#define TIME_TZOLKIN_TODAY_H

#define ZT_SECTION_MAX 512
#define ZT_TITLE_MAX   128

typedef struct {
    /* Page header */
    char page_title[ZT_TITLE_MAX];     /* "Kin 207 — Blue Crystal Hand" */
    int kin;                            /* 1-260 */
    int is_portal;                      /* Galactic Activation Portal day */

    /* Sections (each is a self-contained display block) */
    char kin_section[ZT_SECTION_MAX];       /* Seal, Tone, Color, keywords */
    char oracle_section[ZT_SECTION_MAX];    /* Guide, Analog, Antipode, Occult */
    char wavespell_section[ZT_SECTION_MAX]; /* Wavespell name, day position, purpose */
    char castle_section[ZT_SECTION_MAX];    /* Castle name, color, theme */
    char family_section[ZT_SECTION_MAX];    /* Earth family, direction, members */
    char round_section[ZT_SECTION_MAX];     /* Calendar Round: Tzolkin + Haab */
    char cycle_section[ZT_SECTION_MAX];     /* 52-year position, remaining days */

    int section_count;                  /* always 7 */
    int has_data;
} zt_page_t;

/* Build complete Tzolkin "Today" page for a Julian Day.
 * Returns has_data=1 always (Tzolkin is defined for all JD). */
zt_page_t zt_today(double jd);

/* Format the full page as multi-line display text.
 * Returns chars written (excluding NUL), 0 on error. */
int zt_format(const zt_page_t *page, char *buf, int buf_size);

#endif /* TIME_TZOLKIN_TODAY_H */
