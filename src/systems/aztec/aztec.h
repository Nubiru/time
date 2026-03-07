#ifndef TIME_AZTEC_H
#define TIME_AZTEC_H

#include <stdbool.h>

/* Aztec Tonalpohualli — 260-day sacred calendar (20 day signs x 13 numbers).
 * Structurally identical to the Mayan Tzolkin with different day sign names.
 * Uses GMT correlation 584283 (continuous day count). */

typedef struct {
    int day_sign;    /* 0-19 (Cipactli to Xochitl) */
    int day_number;  /* 1-13 */
    int day_index;   /* 1-260 (position in 260-day cycle) */
} aztec_tonalpohualli_t;

/* Aztec Xiuhpohualli — 365-day solar calendar.
 * 18 months of 20 days + 5 nemontemi ("useless" days). */

typedef struct {
    int month;       /* 1-18 for regular months, 0 for Nemontemi */
    int day;         /* 1-20 for months, 1-5 for Nemontemi */
    int year_day;    /* 1-365 (day of year) */
} aztec_xiuhpohualli_t;

/* Aztec Calendar Round — 52-year cycle (LCM of 260, 365 = 18,980 days).
 * Four year bearers: Tochtli, Acatl, Tecpatl, Calli, each cycling with
 * numbers 1-13 for 52 unique year designations. */

typedef struct {
    int year_bearer;  /* 0-3 (Tochtli=0, Acatl=1, Tecpatl=2, Calli=3) */
    int year_number;  /* 1-13 */
    int round_year;   /* 1-52 (position in Calendar Round) */
} aztec_calendar_round_t;

/* Tonalpohualli from Julian Day. */
aztec_tonalpohualli_t aztec_tonalpohualli(double jd);

/* Day sign name from index 0-19. Returns "?" for invalid. */
const char *aztec_day_sign_name(int sign);

/* Day sign English meaning from index 0-19. Returns "?" for invalid. */
const char *aztec_day_sign_meaning(int sign);

/* Xiuhpohualli from Julian Day. */
aztec_xiuhpohualli_t aztec_xiuhpohualli(double jd);

/* Month name from index 0-18. 0 = Nemontemi. Returns "?" for invalid. */
const char *aztec_month_name(int month);

/* Month meaning from index 0-18. Returns "?" for invalid. */
const char *aztec_month_meaning(int month);

/* Number of days in a month. 1-18 return 20, 0 returns 5. Invalid returns 0. */
int aztec_month_days(int month);

/* Calendar Round from Julian Day. */
aztec_calendar_round_t aztec_calendar_round(double jd);

/* Year bearer name from index 0-3. Returns "?" for invalid. */
const char *aztec_year_bearer_name(int bearer);

/* True if jd falls on a New Fire ceremony day (Calendar Round completion). */
bool aztec_is_new_fire(double jd);

/* Days until the next New Fire ceremony. 0 if today is a New Fire day. */
int aztec_days_to_new_fire(double jd);

/* Calendar Round length in days (always 18980). */
int aztec_calendar_round_days(void);

#endif /* TIME_AZTEC_H */
