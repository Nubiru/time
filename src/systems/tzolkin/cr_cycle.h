#ifndef TIME_CR_CYCLE_H
#define TIME_CR_CYCLE_H

#include <stdbool.h>

#define CR_CYCLE_DAYS  18980  /* LCM(260, 365) = 52 Haab years = 73 Tzolkin cycles */
#define CR_CYCLE_YEARS 52
#define CR_YEAR_DAYS   365

/* 52-year Calendar Round cycle position */
typedef struct {
    int day_in_round;      /* 0-18979 */
    int year_in_round;     /* 0-51 */
    int day_in_year;       /* 0-364 */
    int year_bearer_seal;  /* 0-19 (seal of Tzolkin day at 0 Pop of this year) */
    int year_bearer_tone;  /* 1-13 (tone of Tzolkin day at 0 Pop of this year) */
} cr_cycle_t;

/* Compute 52-year cycle position from Julian Day. */
cr_cycle_t cr_cycle_from_jd(double jd);

/* Day position within 18,980-day cycle (0-18979). */
int cr_cycle_day(double jd);

/* Year within 52-year cycle (0-51). */
int cr_cycle_year(double jd);

/* Year-bearer seal for a given year in the round (0-51).
 * Uses the Tzolkin seal that falls on 0 Pop of that year. */
int cr_cycle_year_bearer_seal(double jd, int year_in_round);

/* Year-bearer tone for a given year in the round (0-51). */
int cr_cycle_year_bearer_tone(double jd, int year_in_round);

/* Is this JD the first day of a new Calendar Round? */
bool cr_cycle_is_new(double jd);

/* JD of the next Calendar Round start after given JD. */
double cr_cycle_next_start(double jd);

/* Days remaining in current Calendar Round. */
int cr_cycle_days_remaining(double jd);

/* Flower angle: position mapped to 0.0-360.0 degrees. */
double cr_cycle_flower_angle(double jd);

/* Which petal (0-51) of the 52-petal flower. */
int cr_cycle_flower_petal(double jd);

/* Progress within current petal (0.0-1.0). */
double cr_cycle_petal_progress(double jd);

/* Find next occurrence of a specific Tzolkin+Haab combination.
 * Searches up to 18,980 days. Returns JD, or 0.0 if invalid input. */
double cr_cycle_next_occurrence(double jd, int tone, int seal,
                                 int haab_month, int haab_day);

#endif
