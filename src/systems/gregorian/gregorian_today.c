/* gregorian_today.c — Gregorian "today" page composition
 *
 * Pure module: no globals, no malloc, no GL, no side effects.
 * Depends on gregorian.h (format/day-of-week) and
 * gregorian_interpret.h (month/day/season archetype data). */

#include "gregorian_today.h"
#include "gregorian.h"
#include "gregorian_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Internal helpers
 * ================================================================ */

/* Days-in-month table (non-leap). Index 0 unused. */
static const int DAYS_IN_MONTH[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int gt_is_leap(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static int gt_days_in_year(int year)
{
    return gt_is_leap(year) ? 366 : 365;
}

static int gt_day_of_year(int year, int month, int day)
{
    int doy = 0;
    for (int m = 1; m < month; m++) {
        doy += DAYS_IN_MONTH[m];
        if (m == 2 && gt_is_leap(year)) {
            doy += 1;
        }
    }
    doy += day;
    return doy;
}

static int gt_season_from_month(int month)
{
    if (month >= 3 && month <= 5)  return 0; /* Spring */
    if (month >= 6 && month <= 8)  return 1; /* Summer */
    if (month >= 9 && month <= 11) return 2; /* Autumn */
    return 3;                                /* Winter (Dec, Jan, Feb) */
}

/* ================================================================
 * gt_today
 * ================================================================ */

gt_page_t gt_today(double jd)
{
    gt_page_t p;
    memset(&p, 0, sizeof(p));

    /* Parse year/month/day from formatted date string */
    char date_buf[16];
    gregorian_format_date(jd, date_buf, sizeof(date_buf));

    int y = 0, m = 0, d = 0;
    if (sscanf(date_buf, "%d-%d-%d", &y, &m, &d) != 3) {
        return p; /* has_data remains 0 */
    }

    p.year  = y;
    p.month = m;
    p.day   = d;
    p.day_of_week = gregorian_day_of_week(jd);

    /* Derived values */
    int doy        = gt_day_of_year(y, m, d);
    int total_days = gt_days_in_year(y);
    int week_num   = (doy - 1) / 7 + 1;
    int quarter    = (m - 1) / 3 + 1;
    int season_idx = gt_season_from_month(m);
    int leap       = gt_is_leap(y);

    /* Archetype data */
    gi_month_t  mo = gi_month_data(m);
    gi_day_t    da = gi_day_data(p.day_of_week);
    gi_season_t se = gi_season_data(season_idx);

    /* page_title: "Gregorian — March 20, 2026 (Friday)" */
    snprintf(p.page_title, GT_TITLE_MAX,
             "Gregorian \xe2\x80\x94 %s %d, %d (%s)",
             mo.name, d, y, da.name);

    /* date_section */
    snprintf(p.date_section, GT_SECTION_MAX,
             "Date: %s %d, %d\n"
             "Day of Year: %d of %d\n"
             "Week: %d\n"
             "Quarter: Q%d",
             mo.name, d, y,
             doy, total_days,
             week_num,
             quarter);

    /* month_section */
    snprintf(p.month_section, GT_SECTION_MAX,
             "Month: %s (month %d)\n"
             "Origin: %s\n"
             "Quality: %s\n"
             "%s",
             mo.name, m,
             mo.origin,
             mo.quality,
             mo.brief);

    /* day_section */
    snprintf(p.day_section, GT_SECTION_MAX,
             "Day: %s\n"
             "Ruling Planet: %s\n"
             "Origin: %s\n"
             "%s",
             da.name,
             da.planet,
             da.origin,
             da.brief);

    /* season_section */
    snprintf(p.season_section, GT_SECTION_MAX,
             "Season: %s\n"
             "Theme: %s\n"
             "Quality: %s\n"
             "%s",
             se.name,
             se.theme,
             se.quality,
             se.brief);

    /* calendar_section */
    double pct = (double)doy / (double)total_days * 100.0;
    snprintf(p.calendar_section, GT_SECTION_MAX,
             "Year Progress: %d/%d (%.1f%%)\n"
             "Days Remaining: %d\n"
             "Leap Year: %s",
             doy, total_days, pct,
             total_days - doy,
             leap ? "Yes" : "No");

    p.section_count = 5;
    p.has_data      = 1;
    return p;
}

/* ================================================================
 * gt_format
 * ================================================================ */

int gt_format(const gt_page_t *page, char *buf, int buf_size)
{
    if (!page || !buf || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
        "%s\n\n"
        "%s\n\n"
        "%s\n\n"
        "%s\n\n"
        "%s\n\n"
        "%s",
        page->page_title,
        page->date_section,
        page->month_section,
        page->day_section,
        page->season_section,
        page->calendar_section);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }

    /* If truncated, return what fits (excluding NUL) */
    if (written >= buf_size) {
        return buf_size - 1;
    }

    return written;
}
