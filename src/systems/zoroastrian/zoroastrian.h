/* zoroastrian.h — Yazdgerdi (Zoroastrian) calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Zoroastrian calendar.
 *
 * 12 months x 30 days + 5 Gatha epagomenal days = 365 days, no leap year.
 * Epoch: JD 1952063.5 = 16 June 632 CE (Julian) = Year 1, 1 Farvardin. */

#ifndef TIME_ZOROASTRIAN_H
#define TIME_ZOROASTRIAN_H

#include <stdbool.h>

typedef struct {
    int year;    /* Yazdgerdi year (year 1 = 632 CE) */
    int month;   /* 1-12 (Farvardin=1 through Esfand=12), 13=Gatha days */
    int day;     /* 1-30 for months 1-12, 1-5 for month 13 (Gatha days) */
} zoro_date_t;

typedef enum {
    ZORO_FEST_NONE = 0,
    ZORO_FEST_NOWRUZ,
    ZORO_FEST_GAHANBAR_1,
    ZORO_FEST_GAHANBAR_2,
    ZORO_FEST_GAHANBAR_3,
    ZORO_FEST_GAHANBAR_4,
    ZORO_FEST_GAHANBAR_5,
    ZORO_FEST_GAHANBAR_6,
    ZORO_FEST_YALDA,
    ZORO_FEST_SADEH,
    ZORO_FEST_COUNT
} zoro_festival_t;

/* Convert Julian Day to Zoroastrian date. */
zoro_date_t zoro_from_jd(double jd);

/* Convert Zoroastrian date to Julian Day (midnight UT). */
double zoro_to_jd(zoro_date_t date);

/* Month name string (1-12 or 13 for Gatha). Returns "?" for invalid. */
const char *zoro_month_name(int month);

/* Day name from the 30 Avestan day names (1-30). Returns "?" for invalid. */
const char *zoro_day_name(int day);

/* Gatha epagomenal day name (1-5). Returns "?" for invalid. */
const char *zoro_gatha_name(int gatha_day);

/* Check if a given date falls on a festival. */
zoro_festival_t zoro_festival(zoro_date_t date);

/* Festival name string. Returns "" for NONE, "?" for invalid. */
const char *zoro_festival_name(zoro_festival_t fest);

/* Number of days in a month (30 for 1-12, 5 for 13, 0 for invalid). */
int zoro_month_days(int month);

/* Convenience: extract Yazdgerdi year from a Julian Day. */
int zoro_year(double jd);

/* True if date is in the Gatha epagomenal period (month 13). */
bool zoro_is_gatha(zoro_date_t date);

/* Day of year (1-365) for a given Zoroastrian date. */
int zoro_day_of_year(zoro_date_t date);

#endif /* TIME_ZOROASTRIAN_H */
