/* tamil_calendar.h — Tamil sidereal solar calendar
 * Pure functions for bidirectional conversion between Julian Day
 * and the Tamil calendar (Thiruvalluvar era).
 *
 * 12 months aligned to sidereal zodiac signs (Chittirai=Aries through
 * Panguni=Pisces). Solar month lengths vary ~29-32 days due to elliptical
 * orbit. 60-year Jovian (Brihaspati) cycle.
 *
 * Epoch: Thiruvalluvar year 1 = 31 BCE. CE offset: TV = CE + 31.
 * Puthandu (Tamil New Year): Sun enters sidereal Aries (~April 14).
 *
 * Sidereal longitude uses Lahiri ayanamsa approximation.
 *
 * Sources: traditional Tamil panchangam, Lahiri ayanamsa formula,
 *          Meeus "Astronomical Algorithms" for mean solar longitude. */

#ifndef TIME_TAMIL_CALENDAR_H
#define TIME_TAMIL_CALENDAR_H

#include <stdbool.h>

#define TAMIL_MONTHS       12
#define TAMIL_JOVIAN_CYCLE 60

typedef struct {
    int year;       /* Tamil Thiruvalluvar year (year 1 = 31 BCE) */
    int month;      /* 1-12 (Chittirai=1 through Panguni=12) */
    int day;        /* 1-32 (solar month lengths vary ~29-32 days) */
    int jovian;     /* Position in 60-year cycle (1-60) */
} tamil_date_t;

typedef enum {
    TAMIL_FEST_NONE = 0,
    TAMIL_FEST_PUTHANDU,       /* Tamil New Year (Sun enters sidereal Aries) */
    TAMIL_FEST_PONGAL,         /* Thai Pongal (Sun enters sidereal Capricorn, ~Jan 14) */
    TAMIL_FEST_CHITHIRAI_FEST, /* Chithirai Thiruvizha (full moon in Chittirai) */
    TAMIL_FEST_DEEPAVALI,      /* Festival of lights (new moon in Aippasi, ~Oct/Nov) */
    TAMIL_FEST_COUNT
} tamil_festival_t;

/* Convert Julian Day to Tamil date. */
tamil_date_t tamil_from_jd(double jd);

/* Convert Tamil date to Julian Day (midnight UT). */
double tamil_to_jd(tamil_date_t date);

/* Month name string (1-12). Returns "?" for invalid. */
const char *tamil_month_name(int month);

/* Month meaning string (1-12). Returns "?" for invalid. */
const char *tamil_month_meaning(int month);

/* Average number of days in a month (1-12). Returns 0 for invalid. */
int tamil_month_days(int month);

/* Jovian cycle year name (1-60). Returns "?" for invalid. */
const char *tamil_jovian_name(int jovian);

/* Compute Jovian year position (1-60) from Thiruvalluvar year.
 * Reference: TV 2018 (1987 CE) = Prabhava (year 1). */
int tamil_jovian_year(int thiruvalluvar_year);

/* Convert CE year to Thiruvalluvar year. TV = CE + 31. */
int tamil_thiruvalluvar_from_ce(int ce_year);

/* Convert Thiruvalluvar year to CE year. CE = TV - 31. */
int tamil_ce_from_thiruvalluvar(int tv_year);

/* Check if a given Tamil date falls on a festival. */
tamil_festival_t tamil_festival(tamil_date_t date);

/* Festival name string. Returns "" for NONE, "?" for invalid. */
const char *tamil_festival_name(tamil_festival_t fest);

/* Compute sidereal solar longitude (0-360 degrees) for a given JD.
 * Uses mean tropical longitude with Lahiri ayanamsa correction.
 * Self-contained: no external dependencies. */
double tamil_sidereal_sun(double jd);

#endif /* TIME_TAMIL_CALENDAR_H */
