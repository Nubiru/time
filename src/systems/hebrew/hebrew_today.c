/* hebrew_today.c — Hebrew Calendar "Today" page implementation
 *
 * Composes a rich daily page from hebrew.h, sabbatical.h, and
 * hebrew_interpret.h data. All functions are pure.
 *
 * No globals, no malloc, no GL, no side effects. */

#include "hebrew_today.h"
#include "hebrew.h"
#include "sabbatical.h"
#include "hebrew_interpret.h"

#include <stdio.h>
#include <string.h>

/* Civil month walk order (same as hebrew.c) */
static const int CIVIL_COMMON[] = { 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6 };
static const int CIVIL_LEAP[]   = { 7, 8, 9, 10, 11, 12, 13, 1, 2, 3, 4, 5, 6 };

/* ===== Year type name ===== */

static const char *year_type_name(int year_days)
{
    if (year_days <= 353) return "Deficient";
    if (year_days <= 354) return "Regular";
    if (year_days <= 355) return "Abundant";
    if (year_days <= 383) return "Deficient Leap";
    if (year_days <= 384) return "Regular Leap";
    return "Abundant Leap";
}

/* ===== Day-of-year (civil: 1 Tishrei = day 1) ===== */

static int day_of_year(int year, int month, int day)
{
    int leap = hebrew_is_leap(year);
    int n_months = leap ? 13 : 12;
    const int *order = leap ? CIVIL_LEAP : CIVIL_COMMON;
    int total = 0;
    int i;

    for (i = 0; i < n_months; i++) {
        if (order[i] == month) {
            break;
        }
        total += hebrew_month_days(year, order[i]);
    }
    return total + day;
}

/* ===== Build sections ===== */

static void build_date_section(char *buf, int size, hebrew_date_t date)
{
    int leap = hebrew_is_leap(date.year);
    int ydays = hebrew_year_days(date.year);
    int months = hebrew_months_in_year(date.year);
    const char *mname = hebrew_month_name(date.month, leap);
    const char *ytype = year_type_name(ydays);
    int doy = day_of_year(date.year, date.month, date.day);

    snprintf(buf, (size_t)size,
             "Date: %d %s %d\n"
             "Year type: %s (%d days)\n"
             "Leap year: %s (%d months)\n"
             "Day %d of %d",
             date.day, mname, date.year,
             ytype, ydays,
             leap ? "Yes" : "No", months,
             doy, ydays);
}

static void build_month_section(char *buf, int size, int month)
{
    hi_month_t m = hi_month_data(month);

    snprintf(buf, (size_t)size,
             "Month: %s\n"
             "Hebrew Letter: %s\n"
             "Tribe: %s\n"
             "Sense: %s\n"
             "Quality: %s\n"
             "Meaning: %s",
             m.tribe != NULL ? hebrew_month_name(month,
                 month == 12 || month == 13) : "?",
             m.hebrew_letter,
             m.tribe,
             m.sense,
             m.quality,
             m.brief);
}

static void build_sabbatical_section(char *buf, int size, int year)
{
    sabbatical_info_t si = sabbatical_info(year);
    int next = sabbatical_next(year);
    int away = next - year;
    const char *status;

    if (si.is_shemitah) {
        status = "Shemitah (rest) year";
    } else {
        status = "Active cultivation year";
    }

    if (away == 0) {
        snprintf(buf, (size_t)size,
                 "Shemitah (Sabbatical) Cycle: Year %d of 7\n"
                 "Status: %s\n"
                 "This is a Shemitah year",
                 si.year_in_cycle,
                 status);
    } else {
        snprintf(buf, (size_t)size,
                 "Shemitah (Sabbatical) Cycle: Year %d of 7\n"
                 "Status: %s\n"
                 "Next Shemitah: %d (%d %s away)",
                 si.year_in_cycle,
                 status,
                 next, away,
                 away == 1 ? "year" : "years");
    }
}

static void build_jubilee_section(char *buf, int size, int year)
{
    jubilee_info_t ji = jubilee_info(year);
    int next = jubilee_next(year);
    int away = next - year;

    if (away == 0) {
        snprintf(buf, (size_t)size,
                 "Jubilee Cycle: Year %d of 50\n"
                 "Sabbatical count: %d within this Jubilee\n"
                 "This is a Jubilee year",
                 ji.year_in_cycle,
                 ji.sabbatical_count);
    } else {
        snprintf(buf, (size_t)size,
                 "Jubilee Cycle: Year %d of 50\n"
                 "Sabbatical count: %d within this Jubilee\n"
                 "Next Jubilee: %d (%d %s away)",
                 ji.year_in_cycle,
                 ji.sabbatical_count,
                 next, away,
                 away == 1 ? "year" : "years");
    }
}

static void build_holiday_section(char *buf, int size, double jd, int year)
{
    int count = hebrew_holiday_count();
    int written = 0;
    int found = 0;

    /* Collect holidays with JD >= today, pick the nearest 3 */
    typedef struct { const char *name; int month; int day; double jd; } hol_t;
    hol_t upcoming[10];
    int n_upcoming = 0;
    int i;

    /* Scan current year */
    for (i = 0; i < count && n_upcoming < 10; i++) {
        hebrew_holiday_t h = hebrew_holiday_get(year, i);
        if (h.name != NULL && h.jd >= jd) {
            upcoming[n_upcoming].name = h.name;
            upcoming[n_upcoming].month = h.month;
            upcoming[n_upcoming].day = h.day;
            upcoming[n_upcoming].jd = h.jd;
            n_upcoming++;
        }
    }

    /* Also scan next year for holidays that wrap around */
    for (i = 0; i < count && n_upcoming < 10; i++) {
        hebrew_holiday_t h = hebrew_holiday_get(year + 1, i);
        if (h.name != NULL && h.jd >= jd) {
            /* Avoid duplicates: only add if JD is after current year's last */
            int dup = 0;
            int j;
            for (j = 0; j < n_upcoming; j++) {
                if (upcoming[j].jd == h.jd) {
                    dup = 1;
                    break;
                }
            }
            if (!dup) {
                upcoming[n_upcoming].name = h.name;
                upcoming[n_upcoming].month = h.month;
                upcoming[n_upcoming].day = h.day;
                upcoming[n_upcoming].jd = h.jd;
                n_upcoming++;
            }
        }
    }

    /* Simple insertion sort by JD */
    {
        int a, b;
        for (a = 1; a < n_upcoming; a++) {
            hol_t tmp = upcoming[a];
            b = a - 1;
            while (b >= 0 && upcoming[b].jd > tmp.jd) {
                upcoming[b + 1] = upcoming[b];
                b--;
            }
            upcoming[b + 1] = tmp;
        }
    }

    written = snprintf(buf, (size_t)size, "Upcoming Holidays:");
    found = (n_upcoming > 3) ? 3 : n_upcoming;

    for (i = 0; i < found && written > 0 && written < size - 1; i++) {
        int days_away = (int)(upcoming[i].jd - jd + 0.5);
        const char *mname = hebrew_month_name(upcoming[i].month,
            hebrew_is_leap(year));
        int w = snprintf(buf + written, (size_t)(size - written),
                         "\n  %s -- %d %s (in %d %s)",
                         upcoming[i].name,
                         upcoming[i].day, mname,
                         days_away,
                         days_away == 1 ? "day" : "days");
        if (w > 0) {
            written += w;
        }
    }

    if (found == 0 && written > 0 && written < size - 1) {
        snprintf(buf + written, (size_t)(size - written),
                 "\n  (none remaining this year)");
    }
}

/* ===== Public API ===== */

ht_page_t ht_today(double jd)
{
    ht_page_t page;
    memset(&page, 0, sizeof(page));

    hebrew_date_t date = hebrew_from_jd(jd);
    int leap = hebrew_is_leap(date.year);
    const char *mname = hebrew_month_name(date.month, leap);

    /* Page title */
    snprintf(page.page_title, HT_TITLE_MAX,
             "Hebrew -- %d %s %d",
             date.day, mname, date.year);

    /* Build all 5 sections */
    build_date_section(page.date_section, HT_SECTION_MAX, date);
    build_month_section(page.month_section, HT_SECTION_MAX, date.month);
    build_sabbatical_section(page.sabbatical_section, HT_SECTION_MAX,
                             date.year);
    build_jubilee_section(page.jubilee_section, HT_SECTION_MAX, date.year);
    build_holiday_section(page.holiday_section, HT_SECTION_MAX,
                          jd, date.year);

    page.section_count = 5;
    page.has_data = 1;

    return page;
}

int ht_format(const ht_page_t *page, char *buf, int buf_size)
{
    if (page == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
                           "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
                           page->page_title,
                           page->date_section,
                           page->month_section,
                           page->sabbatical_section,
                           page->jubilee_section,
                           page->holiday_section);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }

    /* snprintf may have truncated */
    if (written >= buf_size) {
        written = buf_size - 1;
    }

    return written;
}
