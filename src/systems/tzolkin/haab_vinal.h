#ifndef TIME_HAAB_VINAL_H
#define TIME_HAAB_VINAL_H

/* Haab Vinales — the 18 named periods of the 365-day Haab solar calendar,
 * plus the 5-day Uayeb closing period.
 *
 * Each Vinal carries a name and a wisdom meaning that together form
 * a progressive spiritual journey through the solar year.
 *
 * Structure:
 *   18 vinales x 20 days = 360 days
 *    1 Uayeb   x  5 days =   5 days
 *   Total                 = 365 days (Haab year)
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995),
 *         Arguelles, "The Mayan Factor" (1987).
 */

#define HV_VINAL_COUNT    18  /* Regular vinales (not counting Uayeb) */
#define HV_TOTAL_PERIODS  19  /* 18 vinales + Uayeb */
#define HV_VINAL_DAYS     20  /* Days per regular vinal */
#define HV_UAYEB_DAYS      5  /* Days in Uayeb */
#define HV_HAAB_DAYS     365  /* Total days in Haab year */

typedef struct {
    int         number;    /* 1-18 for vinales, 19 for Uayeb */
    const char *name;      /* "Pop", "Uo", ... "Uayeb" */
    const char *meaning;   /* Wisdom text for the period */
    int         days;      /* 20 for vinales, 5 for Uayeb */
} haab_vinal_t;

/* Total period count including Uayeb (always 19). */
int hv_vinal_count(void);

/* Get full vinal data by number (1-19).
 * Returns zeroed struct for invalid number. */
haab_vinal_t hv_vinal_get(int num);

/* Vinal name by number (1-19). Returns NULL for invalid. */
const char *hv_vinal_name(int num);

/* Vinal meaning by number (1-19). Returns NULL for invalid. */
const char *hv_vinal_meaning(int num);

/* Is this the Uayeb period? Returns 1 if num == 19, else 0. */
int hv_is_uayeb(int vinal_num);

/* Total days in the Haab year (always 365). */
int hv_haab_days(void);

/* Map a day-of-year (0-364) to a vinal number (1-19).
 * Days 0-19 = vinal 1, 20-39 = vinal 2, ..., 360-364 = vinal 19 (Uayeb).
 * Returns 0 for invalid day. */
int hv_vinal_for_day_of_year(int day);

#endif /* TIME_HAAB_VINAL_H */
