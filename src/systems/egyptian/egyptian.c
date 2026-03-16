/* egyptian.c — Egyptian civil calendar implementation
 *
 * Pure arithmetic calendar: 12 x 30 + 5 epagomenal = 365 days, no leap year.
 * Epoch: JD 1448638.0 = 26 Feb 747 BCE (Julian) = Year 1, 1 Thoth.
 *
 * Sources: Ptolemy's Almagest (Nabonassar epoch), Reingold & Dershowitz
 * "Calendrical Calculations" (4th ed., ch. 1). */

#include "egyptian.h"
#include "../unified/calendar_fixed.h"
#include <math.h>

#define EGYPT_EPOCH_JD      1448638.0
#define EGYPT_YEAR_DAYS     365
#define EGYPT_MONTH_DAYS_REGULAR 30
#define EGYPT_MONTHS_REGULAR 12
#define EGYPT_REGULAR_TOTAL 360  /* 12 * 30 */
#define EGYPT_EPAG_DAYS     5
#define EGYPT_SEASONS       3
#define EGYPT_MONTHS_PER_SEASON 4

static const char *MONTH_NAMES[] = {
    "Thoth", "Phaophi", "Athyr", "Choiak",
    "Tybi", "Mechir", "Phamenoth", "Pharmuthi",
    "Pachons", "Payni", "Epiphi", "Mesore",
    "Epagomenal"
};

static const char *SEASON_NAMES[] = {
    "Akhet",       /* Inundation */
    "Peret",       /* Emergence/Growth */
    "Shemu",       /* Harvest/Low Water */
    "Epagomenal"   /* Days upon the year */
};

egypt_date_t egypt_from_jd(double jd)
{
    egypt_date_t result;
    cf_date_t cf = cf_fixed_from_jd(jd, EGYPT_EPOCH_JD);
    result.year = cf.year;
    result.month = cf.month;
    result.day = cf.day;
    return result;
}

double egypt_to_jd(egypt_date_t date)
{
    cf_date_t cf = { date.year, date.month, date.day };
    return cf_fixed_to_jd(cf, EGYPT_EPOCH_JD);
}

const char *egypt_month_name(int month)
{
    if (month < 1 || month > 13) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

egypt_season_t egypt_season(int month)
{
    if (month >= 1 && month <= 4) {
        return EGYPT_SEASON_AKHET;
    }
    if (month >= 5 && month <= 8) {
        return EGYPT_SEASON_PERET;
    }
    if (month >= 9 && month <= 12) {
        return EGYPT_SEASON_SHEMU;
    }
    if (month == 13) {
        return EGYPT_SEASON_EPAG;
    }
    return EGYPT_SEASON_EPAG;  /* fallback for invalid */
}

const char *egypt_season_name(egypt_season_t season)
{
    if (season < 0 || season >= EGYPT_SEASON_COUNT) {
        return "?";
    }
    return SEASON_NAMES[season];
}

int egypt_month_days(int month)
{
    if (month >= 1 && month <= EGYPT_MONTHS_REGULAR) {
        return EGYPT_MONTH_DAYS_REGULAR;
    }
    if (month == 13) {
        return EGYPT_EPAG_DAYS;
    }
    return 0;
}

int egypt_day_of_year(egypt_date_t date)
{
    return cf_day_of_year(date.month, date.day);
}

bool egypt_is_epagomenal(egypt_date_t date)
{
    return date.month == 13;
}

egypt_sothic_t egypt_sothic_position(double jd)
{
    egypt_sothic_t result;
    egypt_date_t d = egypt_from_jd(jd);

    /* Years elapsed since epoch (year 1 = 0 elapsed) */
    int years_elapsed = d.year - 1;

    /* Wrap into Sothic cycle of 1461 years */
    int cy = years_elapsed % EGYPT_SOTHIC_CYCLE_YEARS;
    if (cy < 0) {
        cy += EGYPT_SOTHIC_CYCLE_YEARS;
    }

    result.cycle_year = cy;
    result.progress = (double)cy / (double)EGYPT_SOTHIC_CYCLE_YEARS;
    result.drift_days = cy / 4;

    return result;
}

int egypt_season_month(int month)
{
    if (month < 1 || month > 12) {
        return 0;
    }
    return ((month - 1) % EGYPT_MONTHS_PER_SEASON) + 1;
}

int egypt_year(double jd)
{
    egypt_date_t d = egypt_from_jd(jd);
    return d.year;
}
