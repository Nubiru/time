/* hindu_today.c -- Hindu "Today" page implementation
 *
 * Composes a rich daily page from panchanga.h, nakshatra.h, yuga.h,
 * and hindu_interpret.h data. All functions are pure.
 *
 * No globals, no malloc, no GL, no side effects. */

#include "hindu_today.h"
#include "panchanga.h"
#include "nakshatra.h"
#include "yuga.h"
#include "hindu_interpret.h"

#include <stdio.h>
#include <string.h>

/* ===== Build sections ===== */

static void build_panchanga_section(char *buf, int size, panchanga_t p,
                                     const char *nakshatra_name)
{
    snprintf(buf, (size_t)size,
             "Panchanga (Five Limbs):\n"
             "  Tithi: %s %s (%d/30)\n"
             "  Vara: %s (%s, %s)\n"
             "  Nakshatra: %s\n"
             "  Yoga: %s (%s)\n"
             "  Karana: %s",
             p.tithi.paksha, p.tithi.name, p.tithi.number,
             p.vara.name, p.vara.deity, p.vara.planet,
             nakshatra_name,
             p.yoga.name, p.yoga.quality,
             p.karana.name);
}

static void build_tithi_section(char *buf, int size, panchanga_t p,
                                 hndi_tithi_t td)
{
    snprintf(buf, (size_t)size,
             "Tithi: %s\n"
             "Paksha: %s (day %d of 15)\n"
             "Deity: %s\n"
             "Quality: %s\n"
             "Activity: %s",
             p.tithi.name,
             p.tithi.paksha, p.tithi.paksha_day,
             td.deity,
             td.quality,
             td.activity);
}

static void build_nakshatra_section(char *buf, int size, nakshatra_t n)
{
    const char *ruler = nakshatra_ruler_name(n.ruling_planet);

    snprintf(buf, (size_t)size,
             "Nakshatra: %s -- %s\n"
             "Deity: %s\n"
             "Symbol: %s\n"
             "Ruling Planet: %s\n"
             "Quality: %s",
             n.name, n.meaning,
             n.deity,
             n.symbol,
             ruler,
             n.quality);
}

static void build_yuga_section(char *buf, int size, int ce_year)
{
    yuga_position_t pos = yuga_for_year(ce_year);
    yuga_t y = yuga_get(pos.yuga_index);
    double dharma_pct = y.dharma_fraction * 100.0;

    snprintf(buf, (size_t)size,
             "Current Age: %s -- %s\n"
             "%s\n"
             "Years Elapsed: %lld\n"
             "Years Remaining: %lld\n"
             "Dharma: %.0f%%\n"
             "Maha-Yuga: %d",
             y.name, y.meaning,
             y.description,
             pos.years_elapsed,
             pos.years_remaining,
             dharma_pct,
             pos.maha_yuga_number);
}

static void build_guidance_section(char *buf, int size, int tithi_number,
                                    const char *nakshatra_name,
                                    const char *yoga_name)
{
    hindu_interp_t interp = hndi_interpret(tithi_number, nakshatra_name,
                                           yoga_name);

    snprintf(buf, (size_t)size, "%s", interp.detail);
}

/* ===== Public API ===== */

hn_page_t hn_today(double jd, int ce_year, const hn_planets_t *planets)
{
    hn_page_t page;
    memset(&page, 0, sizeof(page));

    if (planets == NULL) {
        return page;
    }

    /* Compute panchanga */
    panchanga_t p = panchanga_compute(jd, planets->sun_lon, planets->moon_lon);

    /* Get nakshatra detail */
    nakshatra_t nak = nakshatra_get(p.nakshatra_index);

    /* Get tithi interpretation */
    hndi_tithi_t td = hndi_tithi_data(p.tithi.number);

    /* Page title: "Hindu -- {Paksha} {TithiName}" */
    snprintf(page.page_title, HN_TITLE_MAX,
             "Hindu -- %s %s",
             p.tithi.paksha, p.tithi.name);

    /* Build all 5 sections */
    build_panchanga_section(page.panchanga_section, HN_SECTION_MAX, p,
                            nak.name);
    build_tithi_section(page.tithi_section, HN_SECTION_MAX, p, td);
    build_nakshatra_section(page.nakshatra_section, HN_SECTION_MAX, nak);
    build_yuga_section(page.yuga_section, HN_SECTION_MAX, ce_year);
    build_guidance_section(page.guidance_section, HN_SECTION_MAX,
                           p.tithi.number, nak.name, p.yoga.name);

    page.section_count = 5;
    page.has_data = 1;

    return page;
}

int hn_format(const hn_page_t *page, char *buf, int buf_size)
{
    if (page == NULL || buf == NULL || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
                           "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
                           page->page_title,
                           page->panchanga_section,
                           page->tithi_section,
                           page->nakshatra_section,
                           page->yuga_section,
                           page->guidance_section);

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

hn_planets_t hn_default_planets(void)
{
    hn_planets_t p;
    p.sun_lon = 0.0;
    p.moon_lon = 0.0;
    return p;
}
