/* today_card.c — Universal card content dispatcher.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "today_card.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

/* System computation headers */
#include "../math/julian.h"                        /* jd_to_gregorian */
#include "../systems/gregorian/gregorian.h"        /* gregorian_day_of_week */
#include "../systems/tzolkin/tzolkin.h"            /* tzolkin_from_jd */
#include "../systems/tzolkin/haab.h"               /* haab_from_jd */
#include "../systems/iching/iching.h"              /* iching_from_jd */
#include "../systems/chinese/chinese.h"            /* chinese_year_from_jd */
#include "../systems/hebrew/hebrew.h"              /* hebrew_from_jd */
#include "../systems/islamic/hijri.h"              /* hijri_from_jd */
#include "../systems/buddhist/buddhist.h"          /* buddhist_uposatha */
#include "../systems/hindu/nakshatra.h"            /* nakshatra_from_longitude */
#include "../systems/hindu/panchanga.h"            /* panchanga_compute */
#include "../systems/astrology/zodiac.h"           /* zodiac_sign */
#include "../systems/human_design/human_design.h"  /* hd_gate_from_longitude */
#include "../systems/kabbalah/sefirot.h"           /* sefirot_get */
#include "../systems/geology/geo_time.h"           /* geo_jd_to_ma */
#include "../systems/coptic/coptic.h"              /* coptic_from_jd */
#include "../systems/ethiopian/ethiopian.h"        /* ethiopian_from_jd */
#include "../systems/persian/persian.h"            /* persian_from_jd */
#include "../systems/japanese/japanese.h"          /* japanese_from_jd */
#include "../systems/korean/korean_calendar.h"     /* korean_from_jd */
#include "../systems/thai/thai_calendar.h"         /* thai_from_jd */

/* ------------------------------------------------------------------ */

static card_content_t empty_card(void)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));
    return c;
}

/* ------------------------------------------------------------------ */

static card_content_t make_gregorian(double jd)
{
    int year = 0, month = 0;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)floor(day_frac);
    int dow = gregorian_day_of_week(jd);
    return card_format_gregorian(year, month, day, dow, NULL);
}

static card_content_t make_tzolkin(double jd)
{
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    const char *seal_name = tzolkin_seal_name(tz.seal);
    return card_format_tzolkin(tz.seal, tz.tone, tz.kin, seal_name);
}

static card_content_t make_haab(double jd)
{
    haab_date_t h = haab_from_jd(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Haab");
    snprintf(c.line1, sizeof(c.line1), "%d %s",
             h.day, haab_month_name(h.month));
    snprintf(c.line2, sizeof(c.line2), "Month %d of 19", h.month + 1);
    return c;
}

static card_content_t make_iching(double jd)
{
    hexagram_t hex = iching_from_jd(jd);
    const char *name = iching_hexagram_name(hex.king_wen);
    const char *upper = iching_trigram_name(hex.upper_trigram);
    const char *lower = iching_trigram_name(hex.lower_trigram);
    return card_format_iching(hex.king_wen, name, upper, lower, hex.lines);
}

static card_content_t make_chinese(double jd)
{
    chinese_year_t cy = chinese_year_from_jd(jd);
    const char *symbol  = chinese_animal_symbol(cy.animal);
    const char *element = chinese_element_name(cy.element);
    const char *animal  = chinese_animal_name(cy.animal);
    const char *polarity = chinese_polarity_name(cy.polarity);
    return card_format_chinese(symbol, element, animal, polarity,
                               cy.stem, cy.branch);
}

static card_content_t make_hebrew(double jd)
{
    hebrew_date_t hd = hebrew_from_jd(jd);
    bool is_leap = hebrew_is_leap(hd.year);
    const char *mname = hebrew_month_name(hd.month, is_leap);
    return card_format_hebrew(hd.year, hd.month, hd.day, mname);
}

static card_content_t make_islamic(double jd)
{
    hijri_date_t hi = hijri_from_jd(jd);
    const char *mname = hijri_month_name(hi.month);
    return card_format_islamic(hi.year, hi.month, hi.day, mname);
}

static card_content_t make_buddhist(double jd)
{
    /* Extract CE year from JD for Buddhist Era */
    int year = 0, month_unused = 0;
    jd_to_gregorian(jd, &year, &month_unused);
    int be = buddhist_era(year);

    uposatha_t u = buddhist_uposatha(jd);
    return card_format_buddhist(be, (int)u.type, u.name, u.illumination);
}

static card_content_t make_hindu(double jd, double sun_lon, double moon_lon)
{
    panchanga_t p = panchanga_compute(jd, sun_lon, moon_lon);
    const char *nak_name = nakshatra_name(p.nakshatra_index);
    return card_format_hindu(p.nakshatra_index, nak_name,
                             p.tithi.number, p.vara.number,
                             p.yoga.name);
}

static card_content_t make_astrology(double sun_lon, double moon_lon)
{
    int sun_sign  = zodiac_sign(sun_lon);
    double sun_deg = zodiac_degree(sun_lon);
    int moon_sign = zodiac_sign(moon_lon);
    double moon_deg = zodiac_degree(moon_lon);
    /* No ascendant without observer latitude/LST — pass 0 */
    return card_format_astrology(sun_sign, sun_deg, moon_sign, moon_deg,
                                 0, 0.0);
}

static card_content_t make_human_design(double sun_lon)
{
    hd_incarnation_t inc = hd_incarnation(sun_lon);
    const char *name = hd_gate_name(inc.sun.gate);
    const char *keyword = hd_gate_keyword(inc.sun.gate);
    return card_format_human_design(inc.sun.gate, inc.sun.line,
                                    inc.earth.gate, inc.earth.line,
                                    name, keyword);
}

static card_content_t make_kabbalah(double jd)
{
    /* Cycle through 10 Sefirot based on day */
    int idx = ((int)floor(jd + 0.5)) % SEFIROT_COUNT;
    if (idx < 0) idx += SEFIROT_COUNT;
    sefirah_t s = sefirot_get(idx);
    /* No daily path — pass -1 */
    return card_format_kabbalah(s.id, s.name, s.meaning, -1, NULL);
}

static card_content_t make_geology(double jd)
{
    double ma = geo_jd_to_ma(jd);
    geo_unit_t eon    = geo_eon_at(ma);
    geo_unit_t era    = geo_era_at(ma);
    geo_unit_t period = geo_period_at(ma);
    geo_unit_t epoch  = geo_epoch_at(ma);
    return card_format_geology(eon.name, era.name, period.name, epoch.name);
}

static card_content_t make_coptic(double jd)
{
    coptic_date_t cd = coptic_from_jd(jd);
    const char *mname = coptic_month_name(cd.month);
    return card_format_coptic(cd.year, cd.month, cd.day, mname);
}

static card_content_t make_ethiopian(double jd)
{
    ethiopian_date_t ed = ethiopian_from_jd(jd);
    const char *mname = ethiopian_month_name(ed.month);
    return card_format_ethiopian(ed.year, ed.month, ed.day, mname);
}

static card_content_t make_persian(double jd)
{
    persian_date_t pd = persian_from_jd(jd);
    const char *mname = persian_month_name(pd.month);
    return card_format_persian(pd.year, pd.month, pd.day, mname);
}

static card_content_t make_japanese(double jd)
{
    japanese_date_t jd_date = japanese_from_jd(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Japanese");
    snprintf(c.line1, sizeof(c.line1), "%s %d",
             japanese_era_name(jd_date.era_index), jd_date.era_year);
    snprintf(c.line2, sizeof(c.line2), "%d-%02d-%02d",
             jd_date.greg_year, jd_date.month, jd_date.day);
    jp_rokuyo_t r = japanese_rokuyo(jd);
    snprintf(c.line3, sizeof(c.line3), "%s", japanese_rokuyo_name(r));
    snprintf(c.detail, sizeof(c.detail), "%s %d | %d-%02d-%02d | %s",
             japanese_era_name(jd_date.era_index), jd_date.era_year,
             jd_date.greg_year, jd_date.month, jd_date.day,
             japanese_rokuyo_name(r));
    return c;
}

static card_content_t make_korean(double jd)
{
    korean_date_t kd = korean_from_jd(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Korean");
    snprintf(c.line1, sizeof(c.line1), "Dangun %d", kd.dangun_year);
    snprintf(c.line2, sizeof(c.line2), "%s %s",
             korean_element_name(kd.element),
             korean_animal_name(kd.animal));
    snprintf(c.line3, sizeof(c.line3), "Lunar %d/%d",
             kd.lunar_month, kd.lunar_day);
    snprintf(c.detail, sizeof(c.detail),
             "Dangun %d | %s %s | Lunar %d/%d",
             kd.dangun_year,
             korean_element_name(kd.element),
             korean_animal_name(kd.animal),
             kd.lunar_month, kd.lunar_day);
    return c;
}

static card_content_t make_thai(double jd)
{
    thai_date_t td = thai_from_jd(jd);
    const char *mname = thai_month_name(td.month);
    return card_format_thai(td.be_year, td.month, td.day, mname);
}

/* ------------------------------------------------------------------ */

card_content_t today_card_for_system(int system_id, double jd,
                                      double sun_lon_deg,
                                      double moon_lon_deg)
{
    switch (system_id) {
    case TS_SYS_GREGORIAN:    return make_gregorian(jd);
    case TS_SYS_TZOLKIN:      return make_tzolkin(jd);
    case TS_SYS_HAAB:         return make_haab(jd);
    case TS_SYS_CHINESE:      return make_chinese(jd);
    case TS_SYS_HEBREW:       return make_hebrew(jd);
    case TS_SYS_ISLAMIC:      return make_islamic(jd);
    case TS_SYS_BUDDHIST:     return make_buddhist(jd);
    case TS_SYS_HINDU:        return make_hindu(jd, sun_lon_deg, moon_lon_deg);
    case TS_SYS_ICHING:       return make_iching(jd);
    case TS_SYS_ASTROLOGY:    return make_astrology(sun_lon_deg, moon_lon_deg);
    case TS_SYS_HUMAN_DESIGN: return make_human_design(sun_lon_deg);
    case TS_SYS_KABBALAH:     return make_kabbalah(jd);
    case TS_SYS_COPTIC:       return make_coptic(jd);
    case TS_SYS_ETHIOPIAN:    return make_ethiopian(jd);
    case TS_SYS_PERSIAN:      return make_persian(jd);
    case TS_SYS_JAPANESE:     return make_japanese(jd);
    case TS_SYS_KOREAN:       return make_korean(jd);
    case TS_SYS_THAI:         return make_thai(jd);
    case TS_SYS_GEOLOGICAL:   return make_geology(jd);
    case TS_SYS_COSMIC:       return empty_card();
    default:                  return empty_card();
    }
}

int today_card_supported(int system_id)
{
    /* Everything except COSMIC and out-of-range */
    if (system_id < 0 || system_id >= TS_SYS_COUNT) return 0;
    if (system_id == TS_SYS_COSMIC) return 0;
    return 1;
}

int today_card_supported_count(void)
{
    int count = 0;
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        if (today_card_supported(i)) count++;
    }
    return count;
}
