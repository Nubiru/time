/* astrology_today.c — Full Astrology "Today" page
 *
 * Assembles sun sign + decan, planetary hour, active aspects, essential
 * dignity into a single page struct.  Jungian archetypal perspective.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#include "astrology_today.h"
#include "zodiac.h"
#include "astrology_interpret.h"
#include "planetary_hours.h"
#include "aspects.h"
#include "dignity.h"
#include <stdio.h>
#include <string.h>

/* Ecliptic planet names (index 0=Sun .. 6=Saturn). */
static const char *PLANET_NAMES[AT_PLANET_COUNT] = {
    "Sun", "Moon", "Mercury", "Venus", "Mars", "Jupiter", "Saturn"
};

/* Map ecliptic index (0=Sun..6=Saturn) to dignity.h planet index.
 * dignity.h: 0=Mercury, 1=Venus, 2=Earth, 3=Mars, 4=Jupiter,
 *            5=Saturn, 6=Uranus, 7=Neptune, 8=Sun, 9=Moon */
static const int ECLIPTIC_TO_DIGNITY[AT_PLANET_COUNT] = {
    8,  /* Sun     -> DIGNITY_SUN     */
    9,  /* Moon    -> DIGNITY_MOON    */
    0,  /* Mercury -> DIGNITY_MERCURY */
    1,  /* Venus   -> DIGNITY_VENUS   */
    3,  /* Mars    -> DIGNITY_MARS    */
    4,  /* Jupiter -> DIGNITY_JUPITER */
    5   /* Saturn  -> DIGNITY_SATURN  */
};

/* Max aspects to show in the aspects section. */
#define AT_MAX_ASPECTS 5

/* Default orb for aspect search (degrees). */
#define AT_DEFAULT_ORB 8.0

/* ===== Decan ordinal suffix ===== */

static const char *decan_ordinal(int decan)
{
    switch (decan) {
        case 1: return "1st";
        case 2: return "2nd";
        case 3: return "3rd";
        default: return "?";
    }
}

/* ===== Build sun section ===== */

static void build_sun_section(char *buf, int size, double sun_lon)
{
    int sign = zodiac_sign(sun_lon);
    double deg = zodiac_degree(sun_lon);
    int decan = zodiac_decan(sun_lon);
    const char *sym = zodiac_sign_symbol(sign);
    const char *name = zodiac_sign_name(sign);
    ai_sign_t data = ai_sign_data(sign);

    snprintf(buf, (size_t)size,
        "Sun Sign: %s (%s) \xe2\x80\x94 %.1f\xc2\xb0\n"
        "Decan: %s (%.0f\xc2\xb0-%.0f\xc2\xb0)\n"
        "Archetype: %s\n"
        "Element: %s (%s)\n"
        "Ruler: %s\n"
        "Keywords: %s\n"
        "Shadow: %s",
        name, sym, deg,
        decan_ordinal(decan),
        (decan - 1) * 10.0, decan * 10.0,
        data.archetype,
        data.element, data.modality,
        data.ruler,
        data.keywords,
        data.shadow);
}

/* ===== Build moon section ===== */

static void build_moon_section(char *buf, int size, double moon_lon)
{
    int sign = zodiac_sign(moon_lon);
    double deg = zodiac_degree(moon_lon);
    const char *sym = zodiac_sign_symbol(sign);
    const char *name = zodiac_sign_name(sign);
    ai_sign_t data = ai_sign_data(sign);

    snprintf(buf, (size_t)size,
        "Moon Sign: %s (%s) \xe2\x80\x94 %.1f\xc2\xb0\n"
        "Emotional tone: %s\n"
        "Element: %s",
        name, sym, deg,
        data.keywords,
        data.element);
}

/* ===== Build hour section ===== */

static void build_hour_section(char *buf, int size, double jd)
{
    planetary_hour_t h = planetary_hour(jd);
    const char *pname = planet_chaldean_name(h.planet);
    const char *psym = planet_chaldean_symbol(h.planet);
    const char *dname = planet_chaldean_name(h.day_ruler);
    const char *dayname = planetary_day_name(h.day_of_week);

    snprintf(buf, (size_t)size,
        "Planetary Hour: %s (%s)\n"
        "Day Ruler: %s \xe2\x80\x94 %s\n"
        "Hour %d of 24",
        pname, psym,
        dname, dayname,
        h.hour_index + 1);
}

/* ===== Build aspects section ===== */

/* Simple insertion sort of aspects by orb (ascending). */
static void sort_aspects_by_orb(aspect_t *arr, int count)
{
    for (int i = 1; i < count; i++) {
        aspect_t key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].orb > key.orb) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

static void build_aspects_section(char *buf, int size,
                                  const double longitudes[AT_PLANET_COUNT])
{
    /* aspects_find_all needs 8 longitudes. Pad index 7 with 0.0. */
    double lons8[8];
    for (int i = 0; i < AT_PLANET_COUNT; i++) {
        lons8[i] = longitudes[i];
    }
    lons8[7] = 0.0;

    aspect_list_t list = aspects_find_all(lons8, AT_DEFAULT_ORB);

    /* Filter to only our 7 planets (exclude index 7). */
    aspect_t filtered[28];
    int fcount = 0;
    for (int i = 0; i < list.count; i++) {
        if (list.aspects[i].planet_a < AT_PLANET_COUNT &&
            list.aspects[i].planet_b < AT_PLANET_COUNT) {
            filtered[fcount++] = list.aspects[i];
        }
    }

    if (fcount == 0) {
        snprintf(buf, (size_t)size, "No significant aspects");
        return;
    }

    sort_aspects_by_orb(filtered, fcount);

    int show = fcount;
    if (show > AT_MAX_ASPECTS) show = AT_MAX_ASPECTS;

    int pos = 0;
    pos += snprintf(buf + pos, (size_t)(size - pos), "Active Aspects:");

    for (int i = 0; i < show && pos < size - 1; i++) {
        const aspect_t *a = &filtered[i];
        pos += snprintf(buf + pos, (size_t)(size - pos),
            "\n  %s %s %s (%s, orb %.1f\xc2\xb0)",
            PLANET_NAMES[a->planet_a],
            aspect_symbol(a->type),
            PLANET_NAMES[a->planet_b],
            aspect_name(a->type),
            a->orb);
    }
}

/* ===== Build dignity section ===== */

static void build_dignity_section(char *buf, int size,
                                  const double longitudes[AT_PLANET_COUNT])
{
    int pos = 0;
    pos += snprintf(buf + pos, (size_t)(size - pos), "Planetary Dignity:");

    for (int i = 0; i < AT_PLANET_COUNT && pos < size - 1; i++) {
        int sign = zodiac_sign(longitudes[i]);
        const char *sign_name = zodiac_sign_name(sign);
        int dignity_idx = ECLIPTIC_TO_DIGNITY[i];
        dignity_result_t dr = planet_dignity(dignity_idx, sign);

        pos += snprintf(buf + pos, (size_t)(size - pos),
            "\n  %s in %s \xe2\x80\x94 %s (%+d)",
            PLANET_NAMES[i],
            sign_name,
            dignity_name(dr.dignity),
            dr.score);
    }
}

/* ===== Public API ===== */

at_page_t at_today(double jd, const at_planets_t *planets)
{
    at_page_t page;
    memset(&page, 0, sizeof(page));

    if (!planets) {
        page.has_data = 0;
        return page;
    }

    page.has_data = 1;
    page.section_count = 5;

    /* Page title: "Astrology — Sun in {Sign} {degree}°" */
    int sun_sign = zodiac_sign(planets->longitudes[0]);
    double sun_deg = zodiac_degree(planets->longitudes[0]);
    snprintf(page.page_title, AT_TITLE_MAX,
        "Astrology \xe2\x80\x94 Sun in %s %.0f\xc2\xb0",
        zodiac_sign_name(sun_sign), sun_deg);

    build_sun_section(page.sun_section, AT_SECTION_MAX,
                      planets->longitudes[0]);
    build_moon_section(page.moon_section, AT_SECTION_MAX,
                       planets->longitudes[1]);
    build_hour_section(page.hour_section, AT_SECTION_MAX, jd);
    build_aspects_section(page.aspects_section, AT_SECTION_MAX,
                          planets->longitudes);
    build_dignity_section(page.dignity_section, AT_SECTION_MAX,
                          planets->longitudes);

    return page;
}

int at_format(const at_page_t *page, char *buf, int buf_size)
{
    if (!page || !buf || buf_size < 16) return 0;
    if (!page->has_data) return 0;

    int pos = 0;
    pos += snprintf(buf + pos, (size_t)(buf_size - pos),
        "%s\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s",
        page->page_title,
        page->sun_section,
        page->moon_section,
        page->hour_section,
        page->aspects_section,
        page->dignity_section);

    if (pos >= buf_size) return 0;
    return pos;
}

at_planets_t at_default_planets(void)
{
    at_planets_t p;
    memset(&p, 0, sizeof(p));
    return p;
}
