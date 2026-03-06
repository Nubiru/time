/* sabbatical.h — Sabbatical (Shemitah) and Jubilee cycles + Hebrew holidays
 *
 * Pure functions for computing the 7-year Sabbatical cycle,
 * the 50-year Jubilee cycle, and major Hebrew holiday dates.
 *
 * Shemitah: year % 7 == 0 (known: 5782, 5789, 5796, ...).
 * Jubilee: year % 50 == 0 (simplified traditional reckoning).
 * Holidays: 10 major fixed-date holidays with Purim leap-year adjustment. */

#ifndef TIME_SABBATICAL_H
#define TIME_SABBATICAL_H

#include <stdbool.h>

typedef struct {
    int year_in_cycle;   /* 1-7 (7 = Shemitah year) */
    bool is_shemitah;
} sabbatical_info_t;

typedef struct {
    int year_in_cycle;   /* 1-50 (50 = Jubilee year) */
    bool is_jubilee;
    int sabbatical_count; /* which sabbatical within this jubilee (0-7) */
} jubilee_info_t;

typedef struct {
    const char *name;
    int month;
    int day;
    double jd;    /* Julian Day of this occurrence */
} hebrew_holiday_t;

/* Sabbatical (Shemitah) cycle — 7-year agricultural rest cycle. */
sabbatical_info_t sabbatical_info(int hebrew_year);
bool sabbatical_is_shemitah(int hebrew_year);
int sabbatical_next(int hebrew_year);

/* Jubilee cycle — 50-year cycle (7 sabbaticals + 1). */
jubilee_info_t jubilee_info(int hebrew_year);
bool jubilee_is_jubilee(int hebrew_year);
int jubilee_next(int hebrew_year);

/* Hebrew holidays — 10 major fixed-date holidays. */
int hebrew_holiday_count(void);
hebrew_holiday_t hebrew_holiday_get(int hebrew_year, int index);
hebrew_holiday_t hebrew_holiday_find(int hebrew_year, const char *name);

#endif /* TIME_SABBATICAL_H */
