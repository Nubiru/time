#include "timeline_data.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../math/julian.h"
#include "../systems/gregorian/gregorian.h"

/* Days per unit for each granularity level */
static const double GRAN_DAYS[] = {
    1.0 / 24.0,          /* HOUR */
    1.0,                  /* DAY */
    7.0,                  /* WEEK */
    30.436875,            /* MONTH (365.2425 / 12) */
    365.2425,             /* YEAR */
    3652.425,             /* DECADE */
    36524.25,             /* CENTURY */
    365242.5,             /* MILLENNIUM */
    365242500.0,          /* MEGAYEAR (1e6 years) */
    365242500000.0        /* GIGAYEAR (1e9 years) */
};

static const char *GRAN_NAMES[] = {
    "Hour", "Day", "Week", "Month", "Year",
    "Decade", "Century", "Millennium", "Megayear", "Gigayear"
};

static const char *MONTH_ABBR[] = {
    "?", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* ---- Helpers ---- */

/* Snap a JD to the start of the next granularity boundary at or after jd. */
static double snap_to_boundary(double jd, tl_granularity_t gran)
{
    int year, month;
    double day;

    switch (gran) {
    case TL_GRAN_HOUR: {
        /* Snap to next whole hour. JD noon = .0 fraction. */
        double shifted = jd + 0.5;
        double frac = shifted - floor(shifted);
        double hours = frac * 24.0;
        double next_hour = ceil(hours);
        if (next_hour >= 24.0) {
            return floor(shifted) + 0.5; /* midnight of next day */
        }
        return floor(shifted) + next_hour / 24.0 - 0.5;
    }
    case TL_GRAN_DAY:
        /* Snap to noon (JD integer) */
        return ceil(jd - 0.5) + 0.5;
    case TL_GRAN_WEEK: {
        /* Snap to next Monday noon */
        double noon = ceil(jd - 0.5) + 0.5;
        int dow = (int)(floor(noon + 0.5)) % 7;
        if (dow < 0) dow += 7;
        /* Monday = 0 in gregorian_day_of_week scheme */
        int days_to_monday = (7 - dow) % 7;
        return noon + days_to_monday;
    }
    case TL_GRAN_MONTH:
        day = jd_to_gregorian(jd, &year, &month);
        if (day > 1.5) {
            month++;
            if (month > 12) { month = 1; year++; }
        }
        return gregorian_to_jd(year, month, 1.5);
    case TL_GRAN_YEAR:
        day = jd_to_gregorian(jd, &year, &month);
        if (month > 1 || day > 1.5) {
            year++;
        }
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_DECADE:
        day = jd_to_gregorian(jd, &year, &month);
        if (month > 1 || day > 1.5 || (year % 10) != 0) {
            year = ((year / 10) + 1) * 10;
        }
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_CENTURY:
        day = jd_to_gregorian(jd, &year, &month);
        if (month > 1 || day > 1.5 || (year % 100) != 0) {
            year = ((year / 100) + 1) * 100;
        }
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_MILLENNIUM:
        day = jd_to_gregorian(jd, &year, &month);
        if (month > 1 || day > 1.5 || (year % 1000) != 0) {
            year = ((year / 1000) + 1) * 1000;
        }
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_MEGAYEAR:
    case TL_GRAN_GIGAYEAR:
        /* For deep time, just return jd rounded up to step */
        {
            double step = GRAN_DAYS[gran];
            return ceil(jd / step) * step;
        }
    default:
        return jd;
    }
}

/* Advance a JD by one granularity step. */
static double advance_by_gran(double jd, tl_granularity_t gran)
{
    int year, month;
    double day;

    switch (gran) {
    case TL_GRAN_HOUR:
        return jd + 1.0 / 24.0;
    case TL_GRAN_DAY:
        return jd + 1.0;
    case TL_GRAN_WEEK:
        return jd + 7.0;
    case TL_GRAN_MONTH:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        month++;
        if (month > 12) { month = 1; year++; }
        return gregorian_to_jd(year, month, 1.5);
    case TL_GRAN_YEAR:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        year++;
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_DECADE:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        year += 10;
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_CENTURY:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        year += 100;
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_MILLENNIUM:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        year += 1000;
        return gregorian_to_jd(year, 1, 1.5);
    case TL_GRAN_MEGAYEAR:
    case TL_GRAN_GIGAYEAR:
        return jd + GRAN_DAYS[gran];
    default:
        return jd + 1.0;
    }
}

/* Format a tick label based on granularity. */
static void format_label(double jd, tl_granularity_t gran, char *buf, int buf_size)
{
    int year, month;
    double day;

    switch (gran) {
    case TL_GRAN_HOUR: {
        /* "12:00", "13:00" */
        double shifted = jd + 0.5;
        double frac = shifted - floor(shifted);
        int hour = (int)(frac * 24.0 + 0.5);
        if (hour >= 24) hour = 0;
        snprintf(buf, (size_t)buf_size, "%d:00", hour);
        break;
    }
    case TL_GRAN_DAY: {
        /* "Mar 15", "Mar 16" */
        day = jd_to_gregorian(jd, &year, &month);
        int d = (int)day;
        if (month >= 1 && month <= 12) {
            snprintf(buf, (size_t)buf_size, "%s %d", MONTH_ABBR[month], d);
        } else {
            snprintf(buf, (size_t)buf_size, "%d-%02d-%02d", year, month, d);
        }
        break;
    }
    case TL_GRAN_WEEK: {
        /* "Mar 15" (Monday of that week) */
        day = jd_to_gregorian(jd, &year, &month);
        int d = (int)day;
        if (month >= 1 && month <= 12) {
            snprintf(buf, (size_t)buf_size, "%s %d", MONTH_ABBR[month], d);
        } else {
            snprintf(buf, (size_t)buf_size, "%04d-%02d-%02d", year, month, d);
        }
        break;
    }
    case TL_GRAN_MONTH: {
        /* "Jan", "Feb" — year boundary shows year */
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        if (month >= 1 && month <= 12) {
            snprintf(buf, (size_t)buf_size, "%s", MONTH_ABBR[month]);
        } else {
            snprintf(buf, (size_t)buf_size, "%04d-%02d", year, month);
        }
        break;
    }
    case TL_GRAN_YEAR:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        snprintf(buf, (size_t)buf_size, "%d", year);
        break;
    case TL_GRAN_DECADE:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        snprintf(buf, (size_t)buf_size, "%ds", (year / 10) * 10);
        break;
    case TL_GRAN_CENTURY:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        snprintf(buf, (size_t)buf_size, "%d", year);
        break;
    case TL_GRAN_MILLENNIUM:
        day = jd_to_gregorian(jd, &year, &month);
        (void)day;
        snprintf(buf, (size_t)buf_size, "%d", year);
        break;
    case TL_GRAN_MEGAYEAR: {
        /* Convert JD offset from J2000 to Ma (millions of years ago).
         * Negative JD offset = past, positive = future. */
        static const double JD_J2000 = 2451545.0;
        static const double DAYS_PER_YEAR = 365.2425;
        double years_from_j2000 = (jd - JD_J2000) / DAYS_PER_YEAR;
        double ma = -years_from_j2000 / 1.0e6;
        if (fabs(ma) < 0.1) {
            snprintf(buf, (size_t)buf_size, "0 Ma");
        } else {
            snprintf(buf, (size_t)buf_size, "%.0f Ma", ma);
        }
        break;
    }
    case TL_GRAN_GIGAYEAR: {
        static const double JD_J2000_G = 2451545.0;
        static const double DAYS_PER_YEAR_G = 365.2425;
        double years_from_j2000 = (jd - JD_J2000_G) / DAYS_PER_YEAR_G;
        double ga = -years_from_j2000 / 1.0e9;
        if (fabs(ga) < 0.01) {
            snprintf(buf, (size_t)buf_size, "0 Ga");
        } else {
            snprintf(buf, (size_t)buf_size, "%.1f Ga", ga);
        }
        break;
    }
    default:
        buf[0] = '\0';
        break;
    }
}

/* Determine tick type (MINOR, MAJOR, ANCHOR). */
static tl_tick_type_t classify_tick(double jd, double jd_center,
                                     tl_granularity_t gran, double step)
{
    /* Is this tick closest to jd_center? */
    if (fabs(jd - jd_center) < step * 0.5) {
        return TL_TICK_ANCHOR;
    }

    int year, month;
    double day;

    switch (gran) {
    case TL_GRAN_HOUR: {
        /* Mark midnight (hour 0) as MAJOR */
        double shifted = jd + 0.5;
        double frac = shifted - floor(shifted);
        int hour = (int)(frac * 24.0 + 0.5);
        if (hour == 0 || hour == 12 || hour == 24) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    }
    case TL_GRAN_DAY: {
        /* First of month or Monday = MAJOR */
        day = jd_to_gregorian(jd, &year, &month);
        int d = (int)(day + 0.01);
        if (d == 1) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    }
    case TL_GRAN_WEEK:
        /* First week of month = MAJOR */
        day = jd_to_gregorian(jd, &year, &month);
        if ((int)(day + 0.01) <= 7) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    case TL_GRAN_MONTH:
        /* January = MAJOR (year boundary) */
        jd_to_gregorian(jd, &year, &month);
        if (month == 1) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    case TL_GRAN_YEAR:
        /* Decade boundary = MAJOR */
        jd_to_gregorian(jd, &year, &month);
        if ((year % 10) == 0) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    case TL_GRAN_DECADE:
        /* Century boundary = MAJOR */
        jd_to_gregorian(jd, &year, &month);
        if ((year % 100) == 0) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    case TL_GRAN_CENTURY:
        /* Millennium boundary = MAJOR */
        jd_to_gregorian(jd, &year, &month);
        if ((year % 1000) == 0) return TL_TICK_MAJOR;
        return TL_TICK_MINOR;
    case TL_GRAN_MILLENNIUM:
    case TL_GRAN_MEGAYEAR:
    case TL_GRAN_GIGAYEAR:
        /* All deep time ticks are MAJOR */
        return TL_TICK_MAJOR;
    default:
        return TL_TICK_MINOR;
    }
}

/* ---- Public API ---- */

tl_granularity_t tl_auto_granularity(double jd_span)
{
    if (jd_span < 0.0) jd_span = -jd_span;

    if (jd_span < 2.0)          return TL_GRAN_HOUR;
    if (jd_span < 14.0)         return TL_GRAN_DAY;
    if (jd_span < 60.0)         return TL_GRAN_WEEK;
    if (jd_span < 400.0)        return TL_GRAN_MONTH;
    if (jd_span < 4000.0)       return TL_GRAN_YEAR;
    if (jd_span < 40000.0)      return TL_GRAN_DECADE;
    if (jd_span < 400000.0)     return TL_GRAN_CENTURY;
    if (jd_span < 4000000.0)    return TL_GRAN_MILLENNIUM;
    if (jd_span < 4.0e9)        return TL_GRAN_MEGAYEAR;
    return TL_GRAN_GIGAYEAR;
}

tl_timeline_t tl_generate_at(double jd_start, double jd_end, double jd_center,
                              tl_granularity_t granularity)
{
    tl_timeline_t tl;
    memset(&tl, 0, sizeof(tl));

    /* Handle swapped range */
    if (jd_start > jd_end) {
        double tmp = jd_start;
        jd_start = jd_end;
        jd_end = tmp;
    }

    tl.jd_start = jd_start;
    tl.jd_end = jd_end;
    tl.jd_center = jd_center;
    tl.granularity = granularity;
    tl.tick_count = 0;

    double span = jd_end - jd_start;
    if (span < 1.0e-10) return tl;

    double step = GRAN_DAYS[granularity];
    double cursor = snap_to_boundary(jd_start, granularity);

    while (cursor <= jd_end && tl.tick_count < TL_MAX_TICKS) {
        double position = (cursor - jd_start) / span;
        if (position < -0.01 || position > 1.01) {
            cursor = advance_by_gran(cursor, granularity);
            continue;
        }

        /* Clamp position */
        if (position < 0.0) position = 0.0;
        if (position > 1.0) position = 1.0;

        tl_tick_t *tick = &tl.ticks[tl.tick_count];
        tick->jd = cursor;
        tick->position = position;
        tick->type = classify_tick(cursor, jd_center, granularity, step);
        format_label(cursor, granularity, tick->label, TL_LABEL_MAX);

        tl.tick_count++;
        cursor = advance_by_gran(cursor, granularity);
    }

    return tl;
}

tl_timeline_t tl_generate(double jd_start, double jd_end, double jd_center)
{
    double span = jd_end - jd_start;
    if (span < 0.0) span = -span;
    tl_granularity_t gran = tl_auto_granularity(span);
    return tl_generate_at(jd_start, jd_end, jd_center, gran);
}

tl_timeline_t tl_zoom(tl_timeline_t tl, double factor)
{
    if (factor <= 0.0) return tl;

    double span = tl.jd_end - tl.jd_start;
    double new_span = span * factor;
    double half = new_span / 2.0;

    double new_start = tl.jd_center - half;
    double new_end = tl.jd_center + half;

    return tl_generate(new_start, new_end, tl.jd_center);
}

tl_timeline_t tl_pan(tl_timeline_t tl, double fraction)
{
    double span = tl.jd_end - tl.jd_start;
    double shift = span * fraction;

    double new_start = tl.jd_start + shift;
    double new_end = tl.jd_end + shift;
    double new_center = tl.jd_center + shift;

    return tl_generate(new_start, new_end, new_center);
}

int tl_nearest_tick(const tl_timeline_t *tl, double position)
{
    if (tl->tick_count == 0) return -1;

    int best = 0;
    double best_dist = fabs(tl->ticks[0].position - position);

    for (int i = 1; i < tl->tick_count; i++) {
        double dist = fabs(tl->ticks[i].position - position);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }

    return best;
}

int tl_range_label(const tl_timeline_t *tl, char *buf, int buf_size)
{
    if (buf_size <= 0) return 0;
    buf[0] = '\0';

    if (tl->granularity >= TL_GRAN_MEGAYEAR) {
        /* Deep time label */
        static const double JD_J2000_R = 2451545.0;
        static const double DAYS_PER_YEAR_R = 365.2425;
        double start_years = (tl->jd_start - JD_J2000_R) / DAYS_PER_YEAR_R;
        double end_years = (tl->jd_end - JD_J2000_R) / DAYS_PER_YEAR_R;

        if (tl->granularity == TL_GRAN_GIGAYEAR) {
            double start_ga = -start_years / 1.0e9;
            double end_ga = -end_years / 1.0e9;
            return snprintf(buf, (size_t)buf_size, "%.1f - %.1f Ga", start_ga, end_ga);
        }
        double start_ma = -start_years / 1.0e6;
        double end_ma = -end_years / 1.0e6;
        return snprintf(buf, (size_t)buf_size, "%.0f - %.0f Ma", start_ma, end_ma);
    }

    /* Human-scale label using gregorian */
    int sy, sm, ey, em;
    double sd = jd_to_gregorian(tl->jd_start, &sy, &sm);
    double ed = jd_to_gregorian(tl->jd_end, &ey, &em);
    int sdi = (int)sd;
    int edi = (int)ed;

    if (sy == ey) {
        if (sm >= 1 && sm <= 12 && em >= 1 && em <= 12) {
            return snprintf(buf, (size_t)buf_size, "%s %d - %s %d, %d",
                            MONTH_ABBR[sm], sdi, MONTH_ABBR[em], edi, sy);
        }
    }

    if (sm >= 1 && sm <= 12 && em >= 1 && em <= 12) {
        return snprintf(buf, (size_t)buf_size, "%s %d, %d - %s %d, %d",
                        MONTH_ABBR[sm], sdi, sy, MONTH_ABBR[em], edi, ey);
    }

    return snprintf(buf, (size_t)buf_size, "%04d-%02d-%02d - %04d-%02d-%02d",
                    sy, sm, sdi, ey, em, edi);
}

double tl_granularity_days(tl_granularity_t gran)
{
    if (gran < 0 || gran >= TL_GRAN_COUNT) return 1.0;
    return GRAN_DAYS[gran];
}

const char *tl_granularity_name(tl_granularity_t gran)
{
    if (gran < 0 || gran >= TL_GRAN_COUNT) return "Unknown";
    return GRAN_NAMES[gran];
}

int tl_count_by_type(const tl_timeline_t *tl, tl_tick_type_t type)
{
    int count = 0;
    for (int i = 0; i < tl->tick_count; i++) {
        if (tl->ticks[i].type == type) count++;
    }
    return count;
}
