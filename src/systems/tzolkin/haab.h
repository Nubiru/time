#ifndef TIME_HAAB_H
#define TIME_HAAB_H

#include <stddef.h>

#define HAAB_MONTH_COUNT 19  /* 18 regular months + Wayeb */
#define HAAB_YEAR_DAYS   365

/* Haab date: month (0-18) + day within month */
typedef struct {
    int month;  /* 0-17 regular months, 18 = Wayeb */
    int day;    /* 0-19 for regular months, 0-4 for Wayeb */
} haab_date_t;

/* Calendar Round: Tzolkin + Haab together */
typedef struct {
    int kin;        /* Tzolkin kin 1-260 */
    int tone;       /* 1-13 */
    int seal;       /* 0-19 */
    int haab_month; /* 0-18 */
    int haab_day;   /* 0-19/0-4 */
} calendar_round_t;

/* Compute Haab date from Julian Day.
 * Uses GMT correlation 584283.
 * Anchor: JD 584283 (Long Count 0.0.0.0.0) = 8 Kumk'u. */
haab_date_t haab_from_jd(double jd);

/* Haab month name (0-18). Returns "?" for invalid. */
const char *haab_month_name(int month);

/* Haab month meaning/association (0-18). Returns "?" for invalid. */
const char *haab_month_meaning(int month);

/* Number of days in a Haab month: 20 for 0-17, 5 for Wayeb (18). */
int haab_month_length(int month);

/* Number of Haab months (including Wayeb). Always 19. */
int haab_month_count(void);

/* Day-of-year position in the 365-day Haab cycle (0-364). */
int haab_day_of_year(haab_date_t h);

/* Format Haab date: "3 Pop", "17 Yax", "2 Wayeb".
 * Writes to buf (up to sz bytes). */
void haab_fmt(haab_date_t h, char *buf, size_t sz);

/* Build Calendar Round from Julian Day.
 * Combines Dreamspell Tzolkin with GMT Haab. */
calendar_round_t calendar_round_from_jd(double jd);

/* Format Calendar Round: "4 Yellow Sun 8 Kumk'u" style.
 * Uses tone + seal name from Tzolkin, month + day from Haab. */
void calendar_round_fmt(calendar_round_t cr, char *buf, size_t sz);

/* Calendar Round cycle length in days (LCM of 260 and 365). */
int calendar_round_cycle(void);

/* Days until next occurrence of a Calendar Round date from given JD.
 * Returns 0 if already on that date, otherwise 1-18979.
 * Returns -1 if not found (should not happen for valid input). */
int calendar_round_next(double jd, calendar_round_t target);

#endif
