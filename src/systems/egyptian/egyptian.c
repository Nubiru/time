/* egyptian.c — Egyptian civil calendar implementation
 *
 * Pure arithmetic calendar: 12 x 30 + 5 epagomenal = 365 days, no leap year.
 * Epoch: JD 1448638.0 = 26 Feb 747 BCE (Julian) = Year 1, 1 Thoth.
 *
 * Sources: Ptolemy's Almagest (Nabonassar epoch), Reingold & Dershowitz
 * "Calendrical Calculations" (4th ed., ch. 1). */

#include "egyptian.h"
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
    int days_since = (int)floor(jd - EGYPT_EPOCH_JD);

    int year;
    int day_of_year;

    if (days_since >= 0) {
        year = days_since / EGYPT_YEAR_DAYS + 1;
        day_of_year = days_since % EGYPT_YEAR_DAYS + 1;
    } else {
        /* For negative days_since, C integer division truncates toward zero.
         * We need floor division. */
        year = (days_since + 1) / EGYPT_YEAR_DAYS;
        day_of_year = days_since - (year - 1) * EGYPT_YEAR_DAYS + 1;
        if (day_of_year <= 0) {
            year--;
            day_of_year += EGYPT_YEAR_DAYS;
        }
    }

    result.year = year;

    if (day_of_year <= EGYPT_REGULAR_TOTAL) {
        result.month = (day_of_year - 1) / EGYPT_MONTH_DAYS_REGULAR + 1;
        result.day = (day_of_year - 1) % EGYPT_MONTH_DAYS_REGULAR + 1;
    } else {
        result.month = 13;
        result.day = day_of_year - EGYPT_REGULAR_TOTAL;
    }

    return result;
}

double egypt_to_jd(egypt_date_t date)
{
    int day_of_year;

    if (date.month <= EGYPT_MONTHS_REGULAR) {
        day_of_year = (date.month - 1) * EGYPT_MONTH_DAYS_REGULAR + date.day;
    } else {
        day_of_year = EGYPT_REGULAR_TOTAL + date.day;
    }

    return EGYPT_EPOCH_JD + (double)(date.year - 1) * EGYPT_YEAR_DAYS
           + (double)(day_of_year - 1);
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
    if (date.month <= EGYPT_MONTHS_REGULAR) {
        return (date.month - 1) * EGYPT_MONTH_DAYS_REGULAR + date.day;
    }
    return EGYPT_REGULAR_TOTAL + date.day;
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
