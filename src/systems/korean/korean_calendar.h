/* korean_calendar.h -- Korean Traditional Calendar
 * Dangun era (year = Gregorian + 2333), Korean sexagenary cycle,
 * Korean zodiac animals, lunisolar month/day approximation,
 * and traditional Korean festivals.
 * Self-contained: implements its own JD math and new moon computation. */

#ifndef TIME_KOREAN_CALENDAR_H
#define TIME_KOREAN_CALENDAR_H

#include <stdbool.h>

#define KOREAN_DANGUN_OFFSET     2333
#define KOREAN_STEMS             10
#define KOREAN_BRANCHES          12
#define KOREAN_ANIMALS           12
#define KOREAN_ELEMENTS          5
#define KOREAN_FESTIVAL_COUNT    7

typedef struct {
    int dangun_year;     /* Dangun era year (Gregorian + 2333, adjusted for lunar NY) */
    int gregorian_year;  /* Gregorian year */
    int stem;            /* 0-9 Heavenly Stem index */
    int branch;          /* 0-11 Earthly Branch index */
    int animal;          /* 0-11 zodiac animal (same as branch) */
    int element;         /* 0-4 (Wood, Fire, Earth, Metal, Water) */
    int polarity;        /* 0=Yang, 1=Yin */
    int cycle_year;      /* 1-60 in sexagenary cycle */
    int lunar_month;     /* 1-12 (approximate lunar month) */
    int lunar_day;       /* 1-30 (day within lunar month) */
} korean_date_t;

typedef struct {
    int id;              /* 0-6 festival index */
    const char *name;    /* Romanized Korean name */
    const char *korean;  /* Korean script name */
    const char *meaning; /* Festival description */
    int lunar_month;     /* Lunar month (1-12) */
    int lunar_day;       /* Lunar day (1-30) */
} korean_festival_t;

/* Convert Julian Day to Korean date. */
korean_date_t korean_from_jd(double jd);

/* Convert Korean lunar date + Gregorian year to approximate Julian Day. */
double korean_to_jd(int gregorian_year, int lunar_month, int lunar_day);

/* Dangun year from Gregorian year (simple: year + 2333). */
int korean_dangun_year(int gregorian_year);

/* Check if a lunar month/day matches a Korean festival. Returns festival info or NULL. */
const korean_festival_t *korean_festival(int lunar_month, int lunar_day);

/* Heavenly Stem name in Korean romanization (0-9). Returns "?" for invalid. */
const char *korean_stem_name(int stem);

/* Earthly Branch name in Korean romanization (0-11). Returns "?" for invalid. */
const char *korean_branch_name(int branch);

/* Korean zodiac animal name (0-11). Returns "?" for invalid. */
const char *korean_animal_name(int animal);

/* Element name in Korean romanization (0-4). Returns "?" for invalid. */
const char *korean_element_name(int element);

/* Polarity name (0=Yang, 1=Yin). Returns "?" for invalid. */
const char *korean_polarity_name(int polarity);

/* Full sexagenary name (e.g., "Gap-Ja" for cycle_year 1). */
const char *korean_sexagenary_name(int cycle_year);

/* Number of festivals defined. */
int korean_festival_count(void);

/* Get festival info by index (0 to KOREAN_FESTIVAL_COUNT-1). */
korean_festival_t korean_festival_by_index(int index);

/* Approximate JD of Lunar New Year (Seollal) for a given Gregorian year. */
double korean_seollal_jd(int gregorian_year);

#endif /* TIME_KOREAN_CALENDAR_H */
