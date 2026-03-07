/* japanese.h -- Japanese Calendar System
 * Imperial era dating (Meiji/Taisho/Showa/Heisei/Reiwa), rokuyo six-day
 * divination cycle, 24 sekki solar term divisions, Japanese zodiac animals.
 * Self-contained: implements its own Gregorian/JD math internally.
 *
 * Attribution: Imperial era system codified by the Meiji government (1868).
 * Rokuyo originates from Chinese fortune-telling, adopted in Japan's
 * Muromachi period. 24 Sekki (solar terms) from Chinese agricultural
 * calendar, adapted to Japanese pronunciation. */

#ifndef TIME_JAPANESE_H
#define TIME_JAPANESE_H

#include <stdbool.h>

/* ===== Imperial Era Types ===== */

typedef enum {
    JP_ERA_MEIJI = 0,
    JP_ERA_TAISHO,
    JP_ERA_SHOWA,
    JP_ERA_HEISEI,
    JP_ERA_REIWA,
    JP_ERA_COUNT
} jp_era_t;

typedef struct {
    int era_index;    /* 0=Meiji,1=Taisho,2=Showa,3=Heisei,4=Reiwa,-1=pre-Meiji */
    int era_year;     /* year within era (1 = gannen) */
    int greg_year;    /* Gregorian year */
    int month;        /* 1-12 */
    int day;          /* 1-31 */
} japanese_date_t;

/* ===== Rokuyo Types ===== */

typedef enum {
    JP_ROKUYO_SENSHO = 0,    /* Win first */
    JP_ROKUYO_TOMOBIKI,      /* Draw friend */
    JP_ROKUYO_SENBU,         /* Lose first */
    JP_ROKUYO_BUTSUMETSU,    /* Buddha's death */
    JP_ROKUYO_TAIAN,         /* Great peace */
    JP_ROKUYO_SHAKKO,        /* Red mouth */
    JP_ROKUYO_COUNT
} jp_rokuyo_t;

/* ===== Imperial Era Functions ===== */

/* Convert Julian Day to Japanese date with imperial era. */
japanese_date_t japanese_from_jd(double jd);

/* Convert Japanese date to Julian Day (midnight UT). */
double japanese_to_jd(japanese_date_t date);

/* Era name string (0-4). Returns "?" for invalid. */
const char *japanese_era_name(int era_index);

/* Era kanji string (0-4). Returns "?" for invalid. */
const char *japanese_era_kanji(int era_index);

/* Gregorian start year of an era (0-4). Returns -1 for invalid. */
int japanese_era_start_year(int era_index);

/* Is the given JD on or after Meiji era start (1868-01-25)? */
bool japanese_is_meiji_or_later(double jd);

/* ===== Rokuyo Functions ===== */

/* Compute rokuyo for a given Julian Day. Approximate: uses synodic month
 * to estimate lunar month/day, then (lunar_month + lunar_day) % 6. */
jp_rokuyo_t japanese_rokuyo(double jd);

/* Rokuyo name string (0-5). Returns "?" for invalid. */
const char *japanese_rokuyo_name(jp_rokuyo_t r);

/* Rokuyo meaning string (0-5). Returns "?" for invalid. */
const char *japanese_rokuyo_meaning(jp_rokuyo_t r);

/* ===== 24 Sekki Functions ===== */

/* Sekki index (0-23) from solar ecliptic longitude in degrees.
 * Longitude is normalized to [0, 360). Each sekki spans 15 degrees. */
int japanese_sekki(double solar_longitude_deg);

/* Sekki name string (0-23). Returns "?" for invalid. */
const char *japanese_sekki_name(int index);

/* Sekki kanji string (0-23). Returns "?" for invalid. */
const char *japanese_sekki_kanji(int index);

/* Starting solar longitude for a sekki index (0-23). Returns -1 for invalid. */
double japanese_sekki_longitude(int index);

/* ===== Japanese Zodiac Functions ===== */

/* Zodiac animal index (0-11) from Gregorian year. Same formula as Chinese. */
int japanese_zodiac_animal(int year);

/* Japanese zodiac animal name (0-11). Returns "?" for invalid. */
const char *japanese_zodiac_name(int animal);

/* Japanese zodiac animal kanji (0-11). Returns "?" for invalid. */
const char *japanese_zodiac_kanji(int animal);

#endif /* TIME_JAPANESE_H */
