/* buddhist_today.c — Buddhist "Today" page implementation
 *
 * Composes a rich daily page from buddhist.h, kalpa.h, and
 * buddhist_interpret.h data. All functions are pure.
 *
 * No globals, no malloc, no GL, no side effects. */

#include "buddhist_today.h"
#include "buddhist.h"
#include "kalpa.h"
#include "buddhist_interpret.h"
#include "../../math/julian.h"

#include <stdio.h>
#include <string.h>

/* ===== Season from month number ===== */

static const char *season_from_month(int month)
{
    /* Months 1-4 = Gimha (Hot), 5-8 = Vassana (Rainy), 9-12 = Hemanta (Cool) */
    if (month >= 1 && month <= 4)  return "Gimha (Hot)";
    if (month >= 5 && month <= 8)  return "Vassana (Rainy)";
    if (month >= 9 && month <= 12) return "Hemanta (Cool)";
    return "Unknown";
}

/* ===== Build sections ===== */

static void build_era_section(char *buf, int size, int be_year, int ce_year)
{
    snprintf(buf, (size_t)size,
             "Buddhist Era: %d\n"
             "Common Era: %d\n"
             "Difference: 543 years",
             be_year, ce_year);
}

static void build_uposatha_section(char *buf, int size, double jd,
                                    int *is_uposatha_out)
{
    uposatha_t u = buddhist_uposatha(jd);

    if (u.type != UPOSATHA_NONE) {
        *is_uposatha_out = 1;
        snprintf(buf, (size_t)size,
                 "Uposatha Day: %s\n"
                 "%s\n"
                 "Moon Age: %.1f days\n"
                 "Illumination: %.0f%%",
                 u.name,
                 u.description,
                 u.moon_age_days,
                 u.illumination * 100.0);
    } else {
        *is_uposatha_out = 0;
        double next_jd = buddhist_next_uposatha(jd);
        uposatha_t next_u = buddhist_uposatha(next_jd);
        snprintf(buf, (size_t)size,
                 "No observance today\n"
                 "Next Uposatha: %s\n"
                 "Moon Age: %.1f days",
                 next_u.name,
                 u.moon_age_days);
    }
}

static void build_month_section(char *buf, int size, buddhist_month_t month,
                                 int is_vassa)
{
    const char *season = season_from_month(month.month);
    snprintf(buf, (size_t)size,
             "Month: %d — %s\n"
             "Vassa (Rains Retreat): %s\n"
             "Season: %s",
             month.month,
             month.name,
             is_vassa ? "Yes" : "No",
             season);
}

static void build_kalpa_section(char *buf, int size)
{
    kalpa_position_t kp = kalpa_from_geological(0.0);
    const char *phase = kalpa_phase_name(kp.phase);

    snprintf(buf, (size_t)size,
             "Cosmic Phase: %s\n"
             "Maha-Kalpa: %d\n"
             "Antarakalpa: %d\n"
             "Cycle Fraction: %.6f",
             phase,
             kp.maha_kalpa_number,
             kp.antarakalpa_number,
             kp.maha_kalpa_fraction);
}

static void build_practice_section(char *buf, int size,
                                    int be_year, int month,
                                    uposatha_type_t uposatha,
                                    kalpa_phase_t kalpa_phase)
{
    buddhist_interp_t interp = bi_interpret(be_year, month,
                                            uposatha, kalpa_phase);
    snprintf(buf, (size_t)size, "%s", interp.detail);
}

/* ===== Public API ===== */

bt_page_t bt_today(double jd)
{
    bt_page_t page;
    memset(&page, 0, sizeof(page));

    /* Extract CE year from JD */
    int ce_year = 0;
    int ce_month = 0;
    jd_to_gregorian(jd, &ce_year, &ce_month);

    int be_year = buddhist_era(ce_year);

    /* Page title */
    snprintf(page.page_title, BT_TITLE_MAX,
             "Buddhist — BE %d", be_year);

    /* Era section */
    build_era_section(page.era_section, BT_SECTION_MAX, be_year, ce_year);

    /* Uposatha section */
    int is_uposatha = 0;
    build_uposatha_section(page.uposatha_section, BT_SECTION_MAX,
                            jd, &is_uposatha);

    /* Month section */
    buddhist_month_t bm = buddhist_month(jd);
    int is_vassa = buddhist_is_vassa(jd) ? 1 : 0;
    build_month_section(page.month_section, BT_SECTION_MAX, bm, is_vassa);

    /* Kalpa section */
    build_kalpa_section(page.kalpa_section, BT_SECTION_MAX);

    /* Practice section */
    uposatha_t u = buddhist_uposatha(jd);
    kalpa_position_t kp = kalpa_from_geological(0.0);
    build_practice_section(page.practice_section, BT_SECTION_MAX,
                            be_year, bm.month, u.type, kp.phase);

    page.section_count = 5;
    page.is_uposatha = is_uposatha;
    page.is_vassa = is_vassa;
    page.has_data = 1;

    return page;
}

int bt_format(const bt_page_t *page, char *buf, int buf_size)
{
    if (page == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
                           "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
                           page->page_title,
                           page->era_section,
                           page->uposatha_section,
                           page->month_section,
                           page->kalpa_section,
                           page->practice_section);

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
