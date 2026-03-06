/* sabbatical.c — Sabbatical (Shemitah), Jubilee cycles & Hebrew holidays
 *
 * All functions are pure (no globals, no malloc, no side effects).
 * Depends on hebrew.h for leap year checks and JD conversion. */

#include "sabbatical.h"
#include "hebrew.h"
#include <string.h>
#include <stddef.h>

/* ===== Holiday table ===== */

#define HOLIDAY_COUNT 10

static const struct {
    const char *name;
    int month;
    int day;
} HOLIDAYS[HOLIDAY_COUNT] = {
    { "Rosh Hashanah",   7,  1 },
    { "Yom Kippur",      7, 10 },
    { "Sukkot",          7, 15 },
    { "Shemini Atzeret", 7, 22 },
    { "Hanukkah",        9, 25 },
    { "Tu BiShvat",     11, 15 },
    { "Purim",          12, 14 },  /* Adjusted to month 13 in leap years */
    { "Passover",        1, 15 },
    { "Shavuot",         3,  6 },
    { "Tisha B'Av",      5,  9 }
};

/* ===== Sabbatical (Shemitah) cycle ===== */

sabbatical_info_t sabbatical_info(int hebrew_year)
{
    sabbatical_info_t info;
    info.year_in_cycle = ((hebrew_year - 1) % 7) + 1;
    info.is_shemitah = (hebrew_year % 7 == 0);
    return info;
}

bool sabbatical_is_shemitah(int hebrew_year)
{
    return (hebrew_year % 7 == 0);
}

int sabbatical_next(int hebrew_year)
{
    int rem = hebrew_year % 7;
    if (rem == 0) {
        return hebrew_year;
    }
    return hebrew_year + (7 - rem);
}

/* ===== Jubilee cycle ===== */

jubilee_info_t jubilee_info(int hebrew_year)
{
    jubilee_info_t info;
    info.year_in_cycle = ((hebrew_year - 1) % 50) + 1;
    info.is_jubilee = (hebrew_year % 50 == 0);
    info.sabbatical_count = (info.year_in_cycle - 1) / 7;
    return info;
}

bool jubilee_is_jubilee(int hebrew_year)
{
    return (hebrew_year % 50 == 0);
}

int jubilee_next(int hebrew_year)
{
    int rem = hebrew_year % 50;
    if (rem == 0) {
        return hebrew_year;
    }
    return hebrew_year + (50 - rem);
}

/* ===== Hebrew holidays ===== */

int hebrew_holiday_count(void)
{
    return HOLIDAY_COUNT;
}

hebrew_holiday_t hebrew_holiday_get(int hebrew_year, int index)
{
    hebrew_holiday_t result;

    if (index < 0 || index >= HOLIDAY_COUNT) {
        result.name = NULL;
        result.month = 0;
        result.day = 0;
        result.jd = 0.0;
        return result;
    }

    result.name = HOLIDAYS[index].name;
    result.month = HOLIDAYS[index].month;
    result.day = HOLIDAYS[index].day;

    /* Purim special case: in leap years, move to Adar II (month 13) */
    if (index == 6 && hebrew_is_leap(hebrew_year)) {
        result.month = 13;
    }

    /* Compute Julian Day */
    hebrew_date_t hd;
    hd.year = hebrew_year;
    hd.month = result.month;
    hd.day = result.day;
    result.jd = hebrew_to_jd(hd);

    return result;
}

hebrew_holiday_t hebrew_holiday_find(int hebrew_year, const char *name)
{
    int i;
    for (i = 0; i < HOLIDAY_COUNT; i++) {
        if (strcmp(HOLIDAYS[i].name, name) == 0) {
            return hebrew_holiday_get(hebrew_year, i);
        }
    }

    /* Not found */
    hebrew_holiday_t result;
    result.name = NULL;
    result.month = 0;
    result.day = 0;
    result.jd = 0.0;
    return result;
}
