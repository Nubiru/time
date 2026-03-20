/* islamic_today.c — Islamic "Today" page implementation
 *
 * Composes a rich daily page from hijri.h, prayer_times.h, and
 * islamic_interpret.h data. All functions are pure.
 *
 * No globals, no malloc, no GL, no side effects. */

#include "islamic_today.h"
#include "hijri.h"
#include "prayer_times.h"
#include "islamic_interpret.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* ===== JD to HH:MM conversion ===== */

static void jd_to_hhmm(double jd, int *h, int *m)
{
    double frac = jd - floor(jd);
    double hours = (frac - 0.5) * 24.0;
    if (hours < 0.0) {
        hours += 24.0;
    }
    *h = (int)hours;
    *m = (int)((hours - (double)*h) * 60.0);
    if (*h >= 24) {
        *h -= 24;
    }
    if (*m < 0) {
        *m = 0;
    }
}

/* ===== Build sections ===== */

static void build_date_section(char *buf, int size, hijri_date_t date)
{
    const char *mname = hijri_month_name(date.month);
    int mdays = hijri_month_days(date.year, date.month);
    int leap = hijri_is_leap(date.year);
    int doy = hijri_day_of_year(date);
    int ydays = hijri_year_days(date.year);

    snprintf(buf, (size_t)size,
             "Date: %d %s %d AH\n"
             "Day %d of %d | %s year\n"
             "Day of year: %d of %d",
             date.day, mname, date.year,
             date.day, mdays, leap ? "Leap" : "Common",
             doy, ydays);
}

static void build_month_section(char *buf, int size, int month)
{
    isi_month_t m = isi_month_data(month);

    snprintf(buf, (size_t)size,
             "Month: %s\n"
             "Sacred: %s\n"
             "Significance: %s\n"
             "Observance: %s",
             m.name,
             m.is_sacred ? "Yes" : "No",
             m.significance,
             (m.event != NULL && strlen(m.event) > 0) ? m.event : "None");
}

static void build_prayer_section(char *buf, int size, double jd,
                                  it_location_t loc)
{
    if (loc.lat == 0.0 && loc.lon == 0.0) {
        snprintf(buf, (size_t)size,
                 "Prayer Times: Location required for calculation");
        return;
    }

    /* Compute noon JD for this day */
    double jd_noon = floor(jd) + 0.5;
    prayer_times_t pt = prayer_times_compute(jd_noon, loc.lat, loc.lon,
                                              PRAYER_METHOD_MWL, ASR_SHAFII);

    int fh, fm, srh, srm, dh, dm, ah, am, mh, mm, ih, im;
    jd_to_hhmm(pt.fajr, &fh, &fm);
    jd_to_hhmm(pt.sunrise, &srh, &srm);
    jd_to_hhmm(pt.dhuhr, &dh, &dm);
    jd_to_hhmm(pt.asr, &ah, &am);
    jd_to_hhmm(pt.maghrib, &mh, &mm);
    jd_to_hhmm(pt.isha, &ih, &im);

    snprintf(buf, (size_t)size,
             "Prayer Times (MWL):\n"
             "  Fajr:    %02d:%02d\n"
             "  Sunrise: %02d:%02d\n"
             "  Dhuhr:   %02d:%02d\n"
             "  Asr:     %02d:%02d\n"
             "  Maghrib: %02d:%02d\n"
             "  Isha:    %02d:%02d",
             fh, fm, srh, srm, dh, dm, ah, am, mh, mm, ih, im);
}

static void build_next_prayer_line(char *buf, int size, double jd,
                                    it_location_t loc)
{
    if (loc.lat == 0.0 && loc.lon == 0.0) {
        snprintf(buf, (size_t)size,
                 "Next Prayer: Provide location for times");
        return;
    }

    double jd_noon = floor(jd) + 0.5;
    prayer_times_t pt = prayer_times_compute(jd_noon, loc.lat, loc.lon,
                                              PRAYER_METHOD_MWL, ASR_SHAFII);
    next_prayer_t np = prayer_next(pt, jd);

    int h, m;
    jd_to_hhmm(np.jd, &h, &m);

    snprintf(buf, (size_t)size,
             "Next Prayer: %s at %02d:%02d",
             np.name, h, m);
}

static void build_sacred_section(char *buf, int size, int month)
{
    isi_month_t m = isi_month_data(month);

    if (m.is_sacred) {
        snprintf(buf, (size_t)size,
                 "This is one of the four sacred months "
                 "(al-ashhur al-hurum).\n%s",
                 m.brief);
    } else {
        snprintf(buf, (size_t)size, "%s", m.brief);
    }
}

/* ===== Public API ===== */

it_page_t it_today(double jd, it_location_t loc)
{
    it_page_t page;
    memset(&page, 0, sizeof(page));

    hijri_date_t date = hijri_from_jd(jd);
    const char *mname = hijri_month_name(date.month);
    isi_month_t month_data = isi_month_data(date.month);

    /* Page title */
    snprintf(page.page_title, IT_TITLE_MAX,
             "Islamic -- %d %s %d",
             date.day, mname, date.year);

    /* Build all 5 sections */
    build_date_section(page.date_section, IT_SECTION_MAX, date);
    build_month_section(page.month_section, IT_SECTION_MAX, date.month);
    build_prayer_section(page.prayer_section, IT_SECTION_MAX, jd, loc);
    build_next_prayer_line(page.next_prayer_line, IT_SECTION_MAX, jd, loc);
    build_sacred_section(page.sacred_section, IT_SECTION_MAX, date.month);

    page.section_count = 5;
    page.is_sacred_month = month_data.is_sacred;
    page.has_data = 1;

    return page;
}

int it_format(const it_page_t *page, char *buf, int buf_size)
{
    if (page == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
                           "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
                           page->page_title,
                           page->date_section,
                           page->month_section,
                           page->prayer_section,
                           page->next_prayer_line,
                           page->sacred_section);

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
