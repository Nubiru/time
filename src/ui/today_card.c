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

/* Daily layout modules — richer card content */
#include "daily_hebrew_layout.h"
#include "daily_islamic_layout.h"
#include "daily_buddhist_layout.h"
#include "daily_hindu_layout.h"
#include "daily_chinese_layout.h"
#include "daily_iching_layout.h"
#include "daily_hd_layout.h"
#include "daily_transit_layout.h"
#include "nakshatra_wheel_layout.h"
#include "daily_gregorian_layout.h"
#include "daily_geology_layout.h"
#include "daily_earth_layout.h"
#include "../systems/earth/pop_today.h"
#include "../systems/earth/earth_fraction.h"
#include "../systems/earth/heartbeat_counter.h"
#include "../systems/earth/pop_counter.h"
#include "daily_coptic_layout.h"
#include "daily_ethiopian_layout.h"
#include "daily_japanese_layout.h"
#include "daily_korean_layout.h"
#include "daily_persian_layout.h"
#include "daily_thai_layout.h"
#include "daily_haab_layout.h"
#include "daily_kabbalah_layout.h"

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
    daily_gregorian_layout_t dg = daily_gregorian_compute(jd);
    int year = 0, month = 0;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)floor(day_frac);
    int dow = gregorian_day_of_week(jd);
    card_content_t c = card_format_gregorian(year, month, day, dow,
                                              dg.season_name);
    if (dg.day_planet && dg.day_planet[0])
        snprintf(c.line3, sizeof(c.line3), "%s day — %s",
                 dg.day_planet, dg.day_origin ? dg.day_origin : "");
    if (dg.month_quality && dg.month_quality[0])
        snprintf(c.detail, sizeof(c.detail), "%s: %s",
                 dg.month_name ? dg.month_name : "",
                 dg.month_quality);
    return c;
}

static card_content_t make_tzolkin(double jd)
{
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    const char *seal_name = tzolkin_seal_name(tz.seal);
    return card_format_tzolkin(tz.seal, tz.tone, tz.kin, seal_name);
}

static card_content_t make_haab(double jd)
{
    daily_haab_layout_t dhl = daily_haab_compute(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Haab");
    snprintf(c.line1, sizeof(c.line1), "%d %s",
             dhl.date.day, dhl.month_name ? dhl.month_name : "");
    if (dhl.month_meaning && dhl.month_meaning[0])
        snprintf(c.line2, sizeof(c.line2), "%s", dhl.month_meaning);
    if (dhl.is_wayeb)
        snprintf(c.line3, sizeof(c.line3), "Wayeb — 5 nameless days");
    else if (dhl.vinal_wisdom && dhl.vinal_wisdom[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dhl.vinal_wisdom);
    if (dhl.round_fmt[0])
        snprintf(c.detail, sizeof(c.detail), "Round: %s | Day %d/365",
                 dhl.round_fmt, dhl.day_of_year + 1);
    return c;
}

static card_content_t make_iching(double jd)
{
    daily_iching_layout_t dl = daily_iching_compute(jd);
    card_content_t c = card_format_iching(dl.king_wen, dl.name,
                                           dl.upper_name, dl.lower_name,
                                           dl.lines);
    /* Enrich with judgment and keywords from daily layout */
    if (dl.keywords && dl.keywords[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dl.keywords);
    if (dl.judgment && dl.judgment[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dl.judgment);
    return c;
}

static card_content_t make_chinese(double jd)
{
    daily_chinese_layout_t dl = daily_chinese_compute(jd);
    card_content_t c = card_format_chinese(
        dl.animal_symbol, dl.element_name, dl.animal_name,
        dl.polarity_name, dl.stem, dl.branch);
    /* Enrich with stem/branch names and cycle position */
    if (dl.stem_name && dl.branch_name)
        snprintf(c.line3, sizeof(c.line3), "%s-%s (Year %d of 60)",
                 dl.stem_name, dl.branch_name, dl.cycle_year);
    return c;
}

static card_content_t make_hebrew(double jd)
{
    daily_hebrew_layout_t dl = daily_hebrew_compute(jd);
    card_content_t c = card_format_hebrew(dl.date.year, dl.date.month,
                                           dl.date.day, dl.month_name);
    /* Enrich with daily layout fields */
    if (dl.quality && dl.quality[0])
        snprintf(c.line3, sizeof(c.line3), "%s — %s",
                 dl.quality, dl.hebrew_letter ? dl.hebrew_letter : "");
    if (dl.brief && dl.brief[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dl.brief);
    return c;
}

static card_content_t make_islamic(double jd)
{
    daily_islamic_layout_t dl = daily_islamic_compute(jd);
    card_content_t c = card_format_islamic(dl.date.year, dl.date.month,
                                            dl.date.day, dl.month_name);
    if (dl.significance && dl.significance[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dl.significance);
    if (dl.brief && dl.brief[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dl.brief);
    return c;
}

static card_content_t make_buddhist(double jd)
{
    daily_buddhist_layout_t dl = daily_buddhist_compute(jd);
    card_content_t c = card_format_buddhist(dl.be_year, dl.uposatha_type,
                                             dl.uposatha_name,
                                             dl.illumination);
    if (dl.uposatha_desc && dl.uposatha_desc[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dl.uposatha_desc);
    return c;
}

static card_content_t make_hindu(double jd, double sun_lon, double moon_lon)
{
    daily_hindu_layout_t dl = daily_hindu_compute(jd, sun_lon, moon_lon);
    card_content_t c = card_format_hindu(dl.nakshatra_index, dl.tithi_name,
                                          dl.tithi_number, dl.vara_number,
                                          dl.yoga_name);
    if (dl.vara_deity && dl.vara_deity[0])
        snprintf(c.line3, sizeof(c.line3), "%s — %s",
                 dl.vara_name ? dl.vara_name : "", dl.vara_deity);

    /* Enrich with nakshatra deity from wheel layout */
    nakshatra_wheel_layout_t nw = nakshatra_wheel_compute(moon_lon, jd);
    int ni = dl.nakshatra_index;
    if (ni >= 0 && ni < 27) {
        nw_nakshatra_t nak = nakshatra_wheel_at(&nw, ni);
        if (nak.deity && nak.deity[0] && dl.yoga_quality && dl.yoga_quality[0])
            snprintf(c.detail, sizeof(c.detail), "%s (deity: %s) | Yoga: %s (%s)",
                     nak.name ? nak.name : "", nak.deity,
                     dl.yoga_name ? dl.yoga_name : "", dl.yoga_quality);
        else if (dl.yoga_quality && dl.yoga_quality[0])
            snprintf(c.detail, sizeof(c.detail), "Yoga: %s (%s)",
                     dl.yoga_name ? dl.yoga_name : "", dl.yoga_quality);
    }
    return c;
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
    daily_hd_layout_t dl = daily_hd_compute(sun_lon);
    card_content_t c = card_format_human_design(
        dl.sun_gate.gate, dl.sun_gate.line,
        dl.earth_gate.gate, dl.earth_gate.line,
        dl.sun_gate.gate_name, dl.sun_gate.keyword);
    /* Enrich with Earth gate info */
    if (dl.earth_gate.gate_name && dl.earth_gate.keyword)
        snprintf(c.line3, sizeof(c.line3), "Earth: Gate %d — %s",
                 dl.earth_gate.gate, dl.earth_gate.keyword);
    return c;
}

static card_content_t make_kabbalah(double jd)
{
    daily_kabbalah_layout_t dkl = daily_kabbalah_compute(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Kabbalah");
    if (dkl.sefirah_name && dkl.sefirah_meaning)
        snprintf(c.line1, sizeof(c.line1), "%s — %s",
                 dkl.sefirah_name, dkl.sefirah_meaning);
    if (dkl.pillar_name && dkl.triad_name)
        snprintf(c.line2, sizeof(c.line2), "%s Pillar, %s Triad",
                 dkl.pillar_name, dkl.triad_name);
    if (dkl.hebrew_letter && dkl.tarot)
        snprintf(c.line3, sizeof(c.line3), "Path: %s — %s",
                 dkl.hebrew_letter, dkl.tarot);
    if (dkl.meditation && dkl.meditation[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dkl.meditation);
    return c;
}

static card_content_t make_geology(double jd)
{
    daily_geology_layout_t dgl = daily_geology_compute(jd);
    double ma = geo_jd_to_ma(jd);
    geo_unit_t eon    = geo_eon_at(ma);
    geo_unit_t era    = geo_era_at(ma);
    geo_unit_t period = geo_period_at(ma);
    geo_unit_t epoch  = geo_epoch_at(ma);
    card_content_t c = card_format_geology(eon.name, era.name,
                                            period.name, epoch.name);
    if (dgl.fact_name && dgl.fact_name[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dgl.fact_name);
    if (dgl.fact_description && dgl.fact_description[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dgl.fact_description);
    return c;
}

static card_content_t make_coptic(double jd)
{
    daily_coptic_layout_t dcl = daily_coptic_compute(jd);
    coptic_date_t cd = coptic_from_jd(jd);
    const char *mname = coptic_month_name(cd.month);
    card_content_t c = card_format_coptic(cd.year, cd.month, cd.day, mname);
    if (dcl.liturgy && dcl.liturgy[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dcl.liturgy);
    if (dcl.feast_name && dcl.feast_name[0])
        snprintf(c.detail, sizeof(c.detail), "%s — %s",
                 dcl.origin ? dcl.origin : "", dcl.feast_name);
    else if (dcl.origin && dcl.origin[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dcl.origin);
    return c;
}

static card_content_t make_ethiopian(double jd)
{
    daily_ethiopian_layout_t del = daily_ethiopian_compute(jd);
    ethiopian_date_t ed = ethiopian_from_jd(jd);
    const char *mname = ethiopian_month_name(ed.month);
    card_content_t c = card_format_ethiopian(ed.year, ed.month, ed.day, mname);
    if (del.season && del.season[0])
        snprintf(c.line3, sizeof(c.line3), "%s", del.season);
    if (del.feast_name && del.feast_name[0])
        snprintf(c.detail, sizeof(c.detail), "%s — %s",
                 del.meaning ? del.meaning : "", del.feast_name);
    else if (del.meaning && del.meaning[0])
        snprintf(c.detail, sizeof(c.detail), "%s", del.meaning);
    return c;
}

static card_content_t make_persian(double jd)
{
    daily_persian_layout_t dpl = daily_persian_compute(jd);
    persian_date_t pd = persian_from_jd(jd);
    const char *mname = persian_month_name(pd.month);
    card_content_t c = card_format_persian(pd.year, pd.month, pd.day, mname);
    if (dpl.weekday_name && dpl.weekday_name[0])
        snprintf(c.line3, sizeof(c.line3), "%s — %s",
                 dpl.weekday_name, dpl.season ? dpl.season : "");
    if (dpl.festival_name && dpl.festival_name[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dpl.festival_name);
    else if (dpl.meaning && dpl.meaning[0])
        snprintf(c.detail, sizeof(c.detail), "%s: %s",
                 mname, dpl.meaning);
    return c;
}

static card_content_t make_japanese(double jd, double sun_lon)
{
    daily_japanese_layout_t djl = daily_japanese_compute(jd, sun_lon);
    japanese_date_t jd_date = japanese_from_jd(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Japanese");
    snprintf(c.line1, sizeof(c.line1), "%s %d",
             japanese_era_name(jd_date.era_index), jd_date.era_year);
    if (djl.rokuyo_name && djl.rokuyo_name[0])
        snprintf(c.line2, sizeof(c.line2), "%s — %s",
                 djl.rokuyo_name, djl.rokuyo_meaning ? djl.rokuyo_meaning : "");
    else
        snprintf(c.line2, sizeof(c.line2), "%d-%02d-%02d",
                 jd_date.greg_year, jd_date.month, jd_date.day);
    if (djl.sekki_name && djl.sekki_name[0])
        snprintf(c.line3, sizeof(c.line3), "%s %s",
                 djl.sekki_kanji ? djl.sekki_kanji : "", djl.sekki_name);
    if (djl.animal_name && djl.animal_name[0])
        snprintf(c.detail, sizeof(c.detail), "%s %s | %s %d",
                 djl.animal_kanji ? djl.animal_kanji : "", djl.animal_name,
                 japanese_era_name(jd_date.era_index), jd_date.era_year);
    return c;
}

static card_content_t make_korean(double jd)
{
    daily_korean_layout_t dkl = daily_korean_compute(jd);
    korean_date_t kd = korean_from_jd(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));
    snprintf(c.title, sizeof(c.title), "Korean");
    snprintf(c.line1, sizeof(c.line1), "Dangun %d", kd.dangun_year);
    if (dkl.sexagenary && dkl.sexagenary[0])
        snprintf(c.line2, sizeof(c.line2), "%s %s — %s",
                 dkl.element_name ? dkl.element_name : "",
                 dkl.animal_name ? dkl.animal_name : "",
                 dkl.sexagenary);
    else
        snprintf(c.line2, sizeof(c.line2), "%s %s",
                 korean_element_name(kd.element),
                 korean_animal_name(kd.animal));
    snprintf(c.line3, sizeof(c.line3), "Lunar %d/%d",
             kd.lunar_month, kd.lunar_day);
    if (dkl.festival_name && dkl.festival_name[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dkl.festival_name);
    else
        snprintf(c.detail, sizeof(c.detail),
                 "Dangun %d | %s %s",
                 kd.dangun_year,
                 korean_element_name(kd.element),
                 korean_animal_name(kd.animal));
    return c;
}

static card_content_t make_thai(double jd)
{
    daily_thai_layout_t dtl = daily_thai_compute(jd);
    thai_date_t td = thai_from_jd(jd);
    const char *mname = thai_month_name(td.month);
    card_content_t c = card_format_thai(td.be_year, td.month, td.day, mname);
    if (dtl.season && dtl.season[0])
        snprintf(c.line3, sizeof(c.line3), "%s", dtl.season);
    if (dtl.festival_name && dtl.festival_name[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dtl.festival_name);
    else if (dtl.origin && dtl.origin[0])
        snprintf(c.detail, sizeof(c.detail), "%s", dtl.origin);
    return c;
}

static card_content_t make_earth(double jd, double lat, double lon)
{
    daily_earth_layout_t el = daily_earth_compute(jd, lat, lon);
    pt_page_t pop = pt_today(jd);
    card_content_t c;
    memset(&c, 0, sizeof(c));

    /* Title: include population count if available */
    if (pop.has_data)
        snprintf(c.title, sizeof(c.title), "Earth — %.44s", pop.page_title);
    else
        snprintf(c.title, sizeof(c.title), "Earth");

    if (el.season_name)
        snprintf(c.line1, sizeof(c.line1), "%s (%.0f%%) | Daylight: %.1fh",
                 el.season_name, el.season_progress * 100.0,
                 el.day_length_hours);
    else
        snprintf(c.line1, sizeof(c.line1), "Daylight: %.1f hours",
                 el.day_length_hours);

    snprintf(c.line2, sizeof(c.line2), "Sun: %.1f\xc2\xb0 %s",
             el.sun_elevation_deg,
             el.sun_elevation_deg >= 0.0 ? "above" : "below");

    /* Line3: earth_fraction awe fact — rotates daily */
    {
        int fcount = ef_fact_count();
        if (fcount > 0) {
            int fi = (int)jd % fcount;
            if (fi < 0) fi = -fi;
            earth_fraction_t ef = ef_fact_get(fi);
            if (ef.visual_analogy)
                snprintf(c.line3, sizeof(c.line3), "%.127s", ef.visual_analogy);
            else if (ef.description)
                snprintf(c.line3, sizeof(c.line3), "%.127s", ef.description);
        }
    }

    /* Detail: heartbeat + pop counter or polar day/night */
    {
        int year = (int)(2000.0 + (jd - 2451545.0) / 365.25);
        hb_counter_t hb = hb_compute(year);
        pc_counter_t pc = pc_compute(year, 0.0);

        if (el.polar_day)
            snprintf(c.detail, sizeof(c.detail), "Polar day — %.127s", hb.poetic_text);
        else if (el.polar_night)
            snprintf(c.detail, sizeof(c.detail), "Polar night — %.127s", hb.poetic_text);
        else if (hb.poetic_text[0] != '\0' && pc.rate_text[0] != '\0')
            snprintf(c.detail, sizeof(c.detail), "%.120s | %.120s",
                     hb.poetic_text, pc.rate_text);
        else if (hb.poetic_text[0] != '\0')
            snprintf(c.detail, sizeof(c.detail), "%.255s", hb.poetic_text);
        else if (pop.has_data && pop.fraction_section[0] != '\0')
            snprintf(c.detail, sizeof(c.detail), "%.255s", pop.fraction_section);
        else
            snprintf(c.detail, sizeof(c.detail), "%s | %.1fh daylight",
                     el.season_name ? el.season_name : "", el.day_length_hours);
    }
    return c;
}

/* ------------------------------------------------------------------ */

card_content_t today_card_for_system(int system_id, double jd,
                                      double sun_lon_deg,
                                      double moon_lon_deg,
                                      double observer_lat,
                                      double observer_lon)
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
    case TS_SYS_JAPANESE:     return make_japanese(jd, sun_lon_deg);
    case TS_SYS_KOREAN:       return make_korean(jd);
    case TS_SYS_THAI:         return make_thai(jd);
    case TS_SYS_GEOLOGICAL:   return make_geology(jd);
    case TS_SYS_COSMIC:       return empty_card();
    case TS_SYS_EARTH:        return make_earth(jd, observer_lat, observer_lon);
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
