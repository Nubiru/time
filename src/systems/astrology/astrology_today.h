/* astrology_today.h — Full Astrology "Today" page
 *
 * Rich daily astrology page from Jungian archetypal perspective:
 * sun sign + decan, planetary hour, active aspects, essential dignity.
 * Takes pre-computed planetary longitudes (caller computes from Kepler).
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ASTROLOGY_TODAY_H
#define TIME_ASTROLOGY_TODAY_H

#define AT_SECTION_MAX  512
#define AT_TITLE_MAX    128
#define AT_PLANET_COUNT 7

/* Pre-computed planetary data (caller provides). */
typedef struct {
    double longitudes[AT_PLANET_COUNT]; /* ecliptic degrees: Sun,Moon,Merc,Venus,Mars,Jup,Sat */
} at_planets_t;

/* Complete Astrology "Today" page. */
typedef struct {
    /* Page header */
    char page_title[AT_TITLE_MAX];       /* "Astrology — Sun in Pisces 29°" */

    /* Sections */
    char sun_section[AT_SECTION_MAX];    /* Sun sign archetype, element, modality, decan */
    char moon_section[AT_SECTION_MAX];   /* Moon sign, emotional tone */
    char hour_section[AT_SECTION_MAX];   /* Planetary hour + day ruler */
    char aspects_section[AT_SECTION_MAX]; /* Active aspects (up to 5 tightest) */
    char dignity_section[AT_SECTION_MAX]; /* Dignity of each planet in its current sign */

    int section_count;  /* always 5 */
    int has_data;
} at_page_t;

/* Build complete Astrology "Today" page.
 * jd: Julian Day (for planetary hour).
 * planets: pre-computed ecliptic longitudes.
 * If planets is NULL, returns has_data=0. */
at_page_t at_today(double jd, const at_planets_t *planets);

/* Format full page as multi-line display text.
 * Returns chars written (excluding NUL), 0 on error. */
int at_format(const at_page_t *page, char *buf, int buf_size);

/* Return default planets struct (all zeros). */
at_planets_t at_default_planets(void);

#endif /* TIME_ASTROLOGY_TODAY_H */
