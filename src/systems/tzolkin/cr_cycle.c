#include "cr_cycle.h"
#include "haab.h"
#include <math.h>

/*
 * Calendar Round: the 52-year cycle formed by interlocking the
 * 260-day Tzolkin and 365-day Haab. LCM(260,365) = 18,980 days.
 *
 * Uses Classic Maya day count (GMT correlation 584283) for ALL
 * computations. Tzolkin values are Classic Maya (every day counts,
 * no Dreamspell leap-day skipping). Haab from haab_from_jd (also
 * day-count-based). This ensures exact 18,980-day periodicity.
 *
 * Year-bearer: the Tzolkin day that falls on 0 Pop (first day of Haab).
 * Since 365 mod 20 = 5, the seal advances by 5 each year -> 4 bearer seals.
 * Since 365 mod 13 = 1, the tone advances by 1 each year -> 13 tones.
 * Full cycle: LCM(4,13) = 52 years.
 *
 * Source: Arguelles, Dreamspell (1990); GMT correlation 584283.
 */

#define GMT_CORRELATION  584283L

static long jd_to_day_count(double jd)
{
    return (long)floor(jd + 0.5) - GMT_CORRELATION;
}

/* Classic Maya Tzolkin from Long Count day count.
 * Anchor: LC 0.0.0.0.0 = 4 Ahau (tone 4, seal 19). */
static int classic_seal(long dc)
{
    return (int)(((dc % 20) + 20 + 19) % 20);
}

static int classic_tone(long dc)
{
    return (int)(((dc % 13) + 13 + 3) % 13) + 1;
}

static int compute_cr_position(double jd)
{
    long dc = jd_to_day_count(jd);
    return (int)(((dc % CR_CYCLE_DAYS) + CR_CYCLE_DAYS) % CR_CYCLE_DAYS);
}

int cr_cycle_day(double jd)
{
    return compute_cr_position(jd);
}

int cr_cycle_year(double jd)
{
    return compute_cr_position(jd) / CR_YEAR_DAYS;
}

cr_cycle_t cr_cycle_from_jd(double jd)
{
    cr_cycle_t cr;
    cr.day_in_round = compute_cr_position(jd);
    cr.year_in_round = cr.day_in_round / CR_YEAR_DAYS;
    cr.day_in_year = cr.day_in_round % CR_YEAR_DAYS;

    /* Year bearer: Classic Tzolkin at 0 Pop of this year */
    long dc = jd_to_day_count(jd);
    long dc_year_start = dc - cr.day_in_year;
    cr.year_bearer_seal = classic_seal(dc_year_start);
    cr.year_bearer_tone = classic_tone(dc_year_start);

    return cr;
}

int cr_cycle_year_bearer_seal(double jd, int year_in_round)
{
    if (year_in_round < 0 || year_in_round >= CR_CYCLE_YEARS)
        return -1;

    int current_pos = compute_cr_position(jd);
    long dc = jd_to_day_count(jd);
    long round_start_dc = dc - current_pos;

    return classic_seal(round_start_dc + (long)year_in_round * CR_YEAR_DAYS);
}

int cr_cycle_year_bearer_tone(double jd, int year_in_round)
{
    if (year_in_round < 0 || year_in_round >= CR_CYCLE_YEARS)
        return 0;

    int current_pos = compute_cr_position(jd);
    long dc = jd_to_day_count(jd);
    long round_start_dc = dc - current_pos;

    return classic_tone(round_start_dc + (long)year_in_round * CR_YEAR_DAYS);
}

bool cr_cycle_is_new(double jd)
{
    return compute_cr_position(jd) == 0;
}

double cr_cycle_next_start(double jd)
{
    int pos = compute_cr_position(jd);
    int remaining = (pos == 0) ? CR_CYCLE_DAYS : CR_CYCLE_DAYS - pos;
    return floor(jd + 0.5) + (double)remaining;
}

int cr_cycle_days_remaining(double jd)
{
    int pos = compute_cr_position(jd);
    return CR_CYCLE_DAYS - 1 - pos;
}

double cr_cycle_flower_angle(double jd)
{
    int pos = compute_cr_position(jd);
    return (pos / (double)CR_CYCLE_DAYS) * 360.0;
}

int cr_cycle_flower_petal(double jd)
{
    return compute_cr_position(jd) / CR_YEAR_DAYS;
}

double cr_cycle_petal_progress(double jd)
{
    int day_in_year = compute_cr_position(jd) % CR_YEAR_DAYS;
    return day_in_year / (double)CR_YEAR_DAYS;
}

double cr_cycle_next_occurrence(double jd, int tone, int seal,
                                 int haab_month, int haab_day)
{
    if (tone < 1 || tone > 13 || seal < 0 || seal > 19)
        return 0.0;
    if (haab_month < 0 || haab_month > 18)
        return 0.0;
    if (haab_month == 18 && (haab_day < 0 || haab_day > 4))
        return 0.0;
    if (haab_month < 18 && (haab_day < 0 || haab_day > 19))
        return 0.0;

    long jd_int = (long)floor(jd + 0.5);

    for (int d = 1; d <= CR_CYCLE_DAYS; d++) {
        double test_jd = (double)(jd_int + d);
        long dc = jd_to_day_count(test_jd);
        int t = classic_tone(dc);
        int s = classic_seal(dc);
        haab_date_t h = haab_from_jd(test_jd);
        if (t == tone && s == seal &&
            h.month == haab_month && h.day == haab_day) {
            return test_jd;
        }
    }
    return 0.0;
}
