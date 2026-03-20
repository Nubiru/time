/* hindu_today.h -- Hindu "Today" page
 *
 * Builds a rich daily page with Panchanga (five limbs), tithi detail,
 * nakshatra detail, Yuga position, and practice guidance.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_HINDU_TODAY_H
#define TIME_HINDU_TODAY_H

#define HN_SECTION_MAX 512
#define HN_TITLE_MAX   128

/* Pre-computed planetary data (caller provides tropical longitudes) */
typedef struct {
    double sun_lon;   /* tropical ecliptic longitude degrees */
    double moon_lon;  /* tropical ecliptic longitude degrees */
} hn_planets_t;

typedef struct {
    char page_title[HN_TITLE_MAX];          /* "Hindu -- Shukla Tritiya" */
    char panchanga_section[HN_SECTION_MAX]; /* All 5 limbs: tithi, vara, nakshatra, yoga, karana */
    char tithi_section[HN_SECTION_MAX];     /* Tithi detail: deity, quality, recommended activity */
    char nakshatra_section[HN_SECTION_MAX]; /* Nakshatra: name, meaning, deity, symbol, ruling planet */
    char yuga_section[HN_SECTION_MAX];      /* Kali Yuga position, dharma fraction */
    char guidance_section[HN_SECTION_MAX];  /* Practice guidance from interpret */
    int section_count;  /* always 5 (panchanga + tithi + nakshatra + yuga + guidance) */
    int has_data;
} hn_page_t;

/* Build Hindu "Today" page. planets provides sun/moon tropical longitudes.
 * If planets is NULL, returns has_data=0. ce_year for yuga calculation. */
hn_page_t hn_today(double jd, int ce_year, const hn_planets_t *planets);

/* Format page as multi-line text. Returns chars written, 0 on error. */
int hn_format(const hn_page_t *page, char *buf, int buf_size);

/* Returns an hn_planets_t with both longitudes set to 0.0. */
hn_planets_t hn_default_planets(void);

#endif /* TIME_HINDU_TODAY_H */
