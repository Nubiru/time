#ifndef TIME_HAAB_H
#define TIME_HAAB_H

#define HAAB_MONTH_COUNT 19  /* 18 regular months + Wayeb */
#define HAAB_YEAR_DAYS   365

typedef struct {
    int month;  /* 0-18 (0=Pop, 17=Kumk'u, 18=Wayeb) */
    int day;    /* 0-19 for regular months, 0-4 for Wayeb */
} haab_date_t;

/* Calendar Round: combined Tzolkin + Haab position */
typedef struct {
    int tzolkin_tone;  /* 1-13 */
    int tzolkin_seal;  /* 0-19 */
    int haab_month;    /* 0-18 */
    int haab_day;      /* 0-19 or 0-4 */
} calendar_round_t;

/* Compute Haab date from Julian Day. Uses GMT correlation 584283. */
haab_date_t haab_from_jd(double jd);

/* Get Haab month name (0-18). Returns "?" for invalid. */
const char *haab_month_name(int month);

/* Get Haab month meaning/association. Returns "?" for invalid. */
const char *haab_month_meaning(int month);

/* Format Haab date as string: "5 Pop" or "3 Wayeb".
 * Writes to buf, returns buf. Returns NULL if buf is NULL. */
char *haab_format(haab_date_t date, char *buf, int max);

/* Calendar Round from JD (combined Tzolkin + Haab). */
calendar_round_t calendar_round_from_jd(double jd);

/* Calendar Round cycle length in days (LCM of 260 and 365). */
int calendar_round_cycle(void);

/* Days until next occurrence of a Calendar Round date from given JD.
 * Returns 0 if already on that date, otherwise 1-18979. */
int calendar_round_next(double jd, calendar_round_t target);

/* Number of Haab months (including Wayeb). */
int haab_month_count(void);

#endif
