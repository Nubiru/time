/* today_summary.c — Unified "Today in All Systems" snapshot.
 *
 * Computes a structured summary of a Julian Day across 20 calendar/time
 * systems. Each entry provides a formatted date string, significance
 * level, and descriptive note.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "today_summary.h"

#include "../gregorian/gregorian.h"
#include "../tzolkin/tzolkin.h"
#include "../tzolkin/dreamspell.h"
#include "../tzolkin/haab.h"
#include "../chinese/chinese.h"
#include "../hebrew/hebrew.h"
#include "../islamic/hijri.h"
#include "../buddhist/buddhist.h"
#include "../hindu/panchanga.h"
#include "../hindu/nakshatra.h"
#include "../iching/iching.h"
#include "../astrology/zodiac.h"
#include "../human_design/human_design.h"
#include "../kabbalah/sefirot.h"
#include "../coptic/coptic.h"
#include "../ethiopian/ethiopian.h"
#include "../persian/persian.h"
#include "../japanese/japanese.h"
#include "../korean/korean_calendar.h"
#include "../thai/thai_calendar.h"
#include "../geology/geo_time.h"
#include "../astronomy/cosmic_time.h"
#include "../astronomy/lunar.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* System name table                                                    */
/* ------------------------------------------------------------------ */

static const char *SYSTEM_NAMES[TS_SYS_COUNT] = {
    "Gregorian",
    "Tzolkin",
    "Haab",
    "Chinese",
    "Hebrew",
    "Islamic",
    "Buddhist",
    "Hindu",
    "I Ching",
    "Astrology",
    "Human Design",
    "Kabbalah",
    "Coptic",
    "Ethiopian",
    "Persian",
    "Japanese",
    "Korean",
    "Thai",
    "Geological",
    "Cosmic"
};

/* ------------------------------------------------------------------ */
/* Internal: make a zeroed entry with system id                        */
/* ------------------------------------------------------------------ */

static ts_entry_t make_entry(ts_system_t sys)
{
    ts_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = sys;
    return e;
}

/* ------------------------------------------------------------------ */
/* Internal: Gregorian year/month/day extraction from JD               */
/* (Used by several systems that need a CE year from JD.)              */
/* Meeus, Astronomical Algorithms, Ch7.                                */
/* ------------------------------------------------------------------ */

static void jd_to_ymd(double jd, int *y, int *m, int *d)
{
    int z = (int)(jd + 0.5);
    int a;
    if (z < 2299161) {
        a = z;
    } else {
        int alpha = (int)((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - alpha / 4;
    }
    int b = a + 1524;
    int c = (int)((b - 122.1) / 365.25);
    int dd = (int)(365.25 * c);
    int e = (int)((b - dd) / 30.6001);

    *d = b - dd - (int)(30.6001 * e);
    *m = (e < 14) ? e - 1 : e - 13;
    *y = (*m > 2) ? c - 4716 : c - 4715;
}

/* ------------------------------------------------------------------ */
/* Per-system computation functions                                     */
/* ------------------------------------------------------------------ */

static ts_entry_t compute_gregorian(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_GREGORIAN);
    gregorian_format_date(jd, e.date_str, TS_DATE_MAX);

    int y, m, d;
    jd_to_ymd(jd, &y, &m, &d);

    /* Significance: New Year = major, equinox/solstice = major,
     * first of month = notable */
    if (m == 1 && d == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "New Year's Day");
    } else if (m == 3 && d >= 19 && d <= 21) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Spring Equinox");
    } else if (m == 6 && d >= 20 && d <= 22) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Summer Solstice");
    } else if (m == 9 && d >= 22 && d <= 24) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Autumn Equinox");
    } else if (m == 12 && d >= 20 && d <= 22) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Winter Solstice");
    } else if (d == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "First of month");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_tzolkin(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_TZOLKIN);
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    dreamspell_tone_t tone = dreamspell_tone(tz.tone);
    const char *seal = tzolkin_seal_name(tz.seal);
    dreamspell_color_t col = dreamspell_color(tz.seal);

    snprintf(e.date_str, TS_DATE_MAX, "Kin %d %s %s %s",
             tz.kin, col.name, tone.name, seal);

    /* Significance: Kin 1 (start of Tzolkin) = major, Tone 1 (wavespell start) = notable */
    if (tz.kin == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Tzolkin cycle begins");
    } else if (tz.tone == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Wavespell begins");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_haab(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_HAAB);
    haab_date_t h = haab_from_jd(jd);
    haab_fmt(h, e.date_str, TS_DATE_MAX);

    /* Significance: 0 Pop (Haab New Year) = major, Wayeb = notable */
    if (h.month == 0 && h.day == 0) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Haab New Year");
    } else if (h.month == 18) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Wayeb (nameless days)");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_chinese(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_CHINESE);
    chinese_year_t cy = chinese_year_from_jd(jd);

    snprintf(e.date_str, TS_DATE_MAX, "%s %s %s (%d/60)",
             chinese_element_name(cy.element),
             chinese_animal_name(cy.animal),
             chinese_polarity_name(cy.polarity),
             cy.cycle_year);

    /* Chinese New Year is hard to detect without lunar data;
     * mark cycle_year == 1 as notable (start of 60-year cycle). */
    if (cy.cycle_year == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Sexagenary cycle begins");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_hebrew(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_HEBREW);
    hebrew_date_t hd = hebrew_from_jd(jd);
    const char *month = hebrew_month_name(hd.month, hebrew_is_leap(hd.year));

    snprintf(e.date_str, TS_DATE_MAX, "%d %s %d", hd.day, month, hd.year);

    /* Significance: major holidays = 2, minor = 1 */
    if (hd.month == 7 && hd.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Rosh Hashana");
    } else if (hd.month == 7 && hd.day == 10) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Yom Kippur");
    } else if (hd.month == 7 && hd.day == 15) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Sukkot");
    } else if (hd.month == 1 && hd.day == 15) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Pesach (Passover)");
    } else if (hd.month == 3 && hd.day == 6) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Shavuot");
    } else if (hd.month == 9 && hd.day == 25) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Hanukkah begins");
    } else if (hd.month == 12 && hd.day == 14) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Purim");
    } else if (hd.day == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Rosh Chodesh");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_islamic(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_ISLAMIC);
    hijri_date_t hd = hijri_from_jd(jd);
    hijri_fmt(hd, e.date_str, TS_DATE_MAX);

    /* Significance: major holidays = 2, Ramadan days = 1, new month = 1 */
    if (hd.month == 1 && hd.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Islamic New Year");
    } else if (hd.month == 9 && hd.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Ramadan begins");
    } else if (hd.month == 10 && hd.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Eid al-Fitr");
    } else if (hd.month == 12 && hd.day == 10) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Eid al-Adha");
    } else if (hd.month == 3 && hd.day == 12) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Mawlid an-Nabi");
    } else if (hd.month == 9) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Ramadan");
    } else if (hd.day == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "New month");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_buddhist(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_BUDDHIST);

    /* Extract CE year for Buddhist Era */
    int y, m, d;
    jd_to_ymd(jd, &y, &m, &d);
    int be = buddhist_era(y);

    buddhist_month_t bm = buddhist_month(jd);
    uposatha_t up = buddhist_uposatha(jd);

    snprintf(e.date_str, TS_DATE_MAX, "BE %d, %s", be, bm.name);

    /* Significance: Uposatha full moon = notable, Vassa start = notable */
    if (up.type == UPOSATHA_FULL_MOON) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Uposatha (Full Moon)");
    } else if (up.type == UPOSATHA_NEW_MOON) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Uposatha (New Moon)");
    }
    if (bm.is_vassa) {
        if (e.note[0] != '\0') {
            /* Append to existing note */
            size_t len = strlen(e.note);
            snprintf(e.note + len, TS_NOTE_MAX - (int)len, " + Vassa");
        } else {
            snprintf(e.note, TS_NOTE_MAX, "Vassa (Rains Retreat)");
        }
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_hindu(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_HINDU);

    /* Panchanga needs sun/moon longitudes. Use lunar_phase for moon. */
    lunar_info_t li = lunar_phase(jd);

    /* Approximate sun longitude from JD:
     * T = centuries from J2000, L0 ≈ 280.46646 + 36000.76983 * T */
    double T = (jd - 2451545.0) / 36525.0;
    double sun_lon = 280.46646 + 36000.76983 * T;
    /* Normalize to [0, 360) */
    sun_lon = sun_lon - 360.0 * (int)(sun_lon / 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    double elongation = li.moon_longitude - sun_lon;
    if (elongation < 0.0) elongation += 360.0;

    tithi_t tithi = panchanga_tithi(elongation);
    vara_t vara = panchanga_vara(jd);

    /* Get nakshatra from moon sidereal longitude */
    double ayanamsa = nakshatra_lahiri_ayanamsa(jd);
    double moon_sid = nakshatra_tropical_to_sidereal(li.moon_longitude, ayanamsa);
    nakshatra_t nk = nakshatra_from_longitude(moon_sid);

    snprintf(e.date_str, TS_DATE_MAX, "%s %s, %s, %s",
             tithi.paksha, tithi.name, vara.name, nk.name);

    /* Significance: Purnima (tithi 15) = notable, Amavasya (tithi 30) = notable */
    if (tithi.number == 15) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Purnima (Full Moon)");
    } else if (tithi.number == 30) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Amavasya (New Moon)");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_iching(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_ICHING);
    hexagram_t hex = iching_from_jd(jd);
    const char *name = iching_hexagram_name(hex.king_wen);
    const char *upper = iching_trigram_name(hex.upper_trigram);
    const char *lower = iching_trigram_name(hex.lower_trigram);

    snprintf(e.date_str, TS_DATE_MAX, "%d. %s (%s/%s)",
             hex.king_wen, name, upper, lower);

    /* Significance: hexagram 1 (The Creative) or 2 (The Receptive) = notable */
    if (hex.king_wen == 1 || hex.king_wen == 2) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Primary hexagram");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_astrology(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_ASTROLOGY);

    /* Approximate sun longitude (same as in Hindu) */
    double T = (jd - 2451545.0) / 36525.0;
    double sun_lon = 280.46646 + 36000.76983 * T;
    sun_lon = sun_lon - 360.0 * (int)(sun_lon / 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    int sign = zodiac_sign(sun_lon);
    double deg = zodiac_degree(sun_lon);
    const char *sign_name = zodiac_sign_name(sign);

    snprintf(e.date_str, TS_DATE_MAX, "Sun in %s (%.1f)", sign_name, deg);

    /* Significance: sign change (degree < 1) = notable */
    if (deg < 1.0) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Sun enters %s", sign_name);
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_human_design(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_HUMAN_DESIGN);

    /* Approximate sun longitude */
    double T = (jd - 2451545.0) / 36525.0;
    double sun_lon = 280.46646 + 36000.76983 * T;
    sun_lon = sun_lon - 360.0 * (int)(sun_lon / 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    hd_gate_t gate = hd_gate_from_longitude(sun_lon);
    const char *name = hd_gate_name(gate.gate);
    const char *keyword = hd_gate_keyword(gate.gate);

    snprintf(e.date_str, TS_DATE_MAX, "Gate %d.%d %s", gate.gate, gate.line, name);

    if (keyword && keyword[0] != '?') {
        snprintf(e.note, TS_NOTE_MAX, "%s", keyword);
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_kabbalah(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_KABBALAH);

    /* Map day of week (0-6) to a Sefirah for daily meditation focus.
     * Traditional Omer count: Chesed(3)->Gevurah(4)->Tiferet(5)->
     * Netzach(6)->Hod(7)->Yesod(8)->Malkuth(9), cycling each week. */
    int dow = gregorian_day_of_week(jd); /* 0=Mon..6=Sun */
    int sefira_index = 3 + dow;  /* Chesed(3) on Monday through Malkuth(9) on Sunday */
    if (sefira_index > 9) sefira_index = 9;

    sefirah_t s = sefirot_get(sefira_index);

    snprintf(e.date_str, TS_DATE_MAX, "%s (%s)", s.name, s.meaning);

    /* The middle pillar days are notable (Tiferet=Wednesday, Yesod=Saturday, Malkuth=Sunday) */
    if (sefirot_is_middle_pillar(sefira_index)) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "Middle Pillar (%s)",
                 sefirot_pillar_name(s.pillar));
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_coptic(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_COPTIC);
    coptic_date_t cd = coptic_from_jd(jd);
    const char *month = coptic_month_name(cd.month);

    snprintf(e.date_str, TS_DATE_MAX, "%d %s %d AM", cd.day, month, cd.year);

    /* Check for feasts */
    coptic_feast_t feast = coptic_feast(cd);
    if (feast != COPTIC_FEAST_NONE) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "%s", coptic_feast_name(feast));
    } else if (cd.month == 1 && cd.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Nayrouz (Coptic New Year)");
    } else if (cd.day == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "New month");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_ethiopian(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_ETHIOPIAN);
    ethiopian_date_t ed = ethiopian_from_jd(jd);
    const char *month = ethiopian_month_name(ed.month);

    snprintf(e.date_str, TS_DATE_MAX, "%d %s %d", ed.day, month, ed.year);

    ethiopian_feast_t feast = ethiopian_feast(ed);
    if (feast != ETH_FEAST_NONE) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "%s", ethiopian_feast_name(feast));
    } else if (ed.month == 1 && ed.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Enkutatash (Ethiopian New Year)");
    } else if (ed.day == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "New month");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_persian(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_PERSIAN);
    persian_date_t pd = persian_from_jd(jd);
    const char *month = persian_month_name(pd.month);

    snprintf(e.date_str, TS_DATE_MAX, "%d %s %d SH", pd.day, month, pd.year);

    persian_festival_t fest = persian_festival(pd);
    if (fest != PERSIAN_FEST_NONE) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "%s", persian_festival_name(fest));
    } else if (pd.month == 1 && pd.day == 1) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "Nowruz");
    } else if (pd.day == 1) {
        e.significance = 1;
        snprintf(e.note, TS_NOTE_MAX, "New month");
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_japanese(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_JAPANESE);
    japanese_date_t jd_jp = japanese_from_jd(jd);

    if (jd_jp.era_index >= 0) {
        const char *era = japanese_era_name(jd_jp.era_index);
        snprintf(e.date_str, TS_DATE_MAX, "%s %d/%d/%d",
                 era, jd_jp.era_year, jd_jp.month, jd_jp.day);
    } else {
        snprintf(e.date_str, TS_DATE_MAX, "%d/%d/%d (pre-Meiji)",
                 jd_jp.greg_year, jd_jp.month, jd_jp.day);
    }

    /* Rokuyo fortune */
    jp_rokuyo_t rk = japanese_rokuyo(jd);
    const char *rk_name = japanese_rokuyo_name(rk);
    snprintf(e.note, TS_NOTE_MAX, "%s", rk_name);

    /* Taian (great peace) is the most auspicious rokuyo */
    if (rk == JP_ROKUYO_TAIAN) {
        e.significance = 1;
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_korean(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_KOREAN);
    korean_date_t kd = korean_from_jd(jd);

    snprintf(e.date_str, TS_DATE_MAX, "Dangun %d, %s %s",
             kd.dangun_year,
             korean_element_name(kd.element),
             korean_animal_name(kd.animal));

    /* Check for festivals */
    const korean_festival_t *fest = korean_festival(kd.lunar_month, kd.lunar_day);
    if (fest) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "%s", fest->name);
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_thai(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_THAI);
    thai_date_t td = thai_from_jd(jd);
    const char *month = thai_month_name(td.month);

    snprintf(e.date_str, TS_DATE_MAX, "BE %d, %d %s",
             td.be_year, td.day, month);

    thai_festival_t fest = thai_festival(td);
    if (fest != THAI_FEST_NONE) {
        e.significance = 2;
        snprintf(e.note, TS_NOTE_MAX, "%s", thai_festival_name(fest));
    }

    e.active = 1;
    return e;
}

static ts_entry_t compute_geological(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_GEOLOGICAL);

    double ma = geo_jd_to_ma(jd);
    geo_unit_t eon = geo_eon_at(ma);
    geo_unit_t epoch = geo_epoch_at(ma);

    snprintf(e.date_str, TS_DATE_MAX, "%s, %s", eon.name, epoch.name);

    e.active = 1;
    return e;
}

static ts_entry_t compute_cosmic(double jd)
{
    ts_entry_t e = make_entry(TS_SYS_COSMIC);

    cosmic_context_t cc = cosmic_context(jd);
    char age_buf[32];
    cosmic_format_age(cc.universe_age_years, age_buf, sizeof(age_buf));

    snprintf(e.date_str, TS_DATE_MAX, "Universe: %s", age_buf);
    snprintf(e.note, TS_NOTE_MAX, "Galactic orbit: %.4f",
             cc.sun_galactic_phase);

    e.active = 1;
    return e;
}

/* ------------------------------------------------------------------ */
/* Convergence and headline                                             */
/* ------------------------------------------------------------------ */

static int compute_convergence(const ts_summary_t *s)
{
    int count = 0;
    for (int i = 0; i < s->entry_count; i++) {
        if (s->entries[i].active && s->entries[i].significance >= 1) {
            count++;
        }
    }
    /* Cap at 10 */
    return (count > 10) ? 10 : count;
}

static void compute_headline(ts_summary_t *s)
{
    /* Find the most significant entry */
    int best_idx = -1;
    int best_sig = -1;
    int notable_count = 0;

    for (int i = 0; i < s->entry_count; i++) {
        if (!s->entries[i].active) continue;
        if (s->entries[i].significance > best_sig) {
            best_sig = s->entries[i].significance;
            best_idx = i;
        }
        if (s->entries[i].significance >= 1) {
            notable_count++;
        }
    }

    if (notable_count >= 3) {
        /* Multiple systems converge */
        snprintf(s->headline, TS_NOTE_MAX,
                 "%d systems align today", notable_count);
    } else if (best_idx >= 0 && best_sig >= 1) {
        /* Use the best entry's note or date_str. */
        if (s->entries[best_idx].note[0] != '\0') {
            const char *sys = SYSTEM_NAMES[s->entries[best_idx].system];
            const char *note = s->entries[best_idx].note;
            size_t sys_len = strlen(sys);
            size_t note_len = strlen(note);
            size_t total = sys_len + 2 + note_len; /* "name: note" */
            if (total >= TS_NOTE_MAX) {
                total = TS_NOTE_MAX - 1;
            }
            size_t pos = 0;
            /* Copy system name */
            size_t copy_len = sys_len;
            if (copy_len > total) copy_len = total;
            memcpy(s->headline, sys, copy_len);
            pos = copy_len;
            /* Separator ": " */
            if (pos + 2 <= total) {
                s->headline[pos++] = ':';
                s->headline[pos++] = ' ';
            }
            /* Copy note */
            size_t remaining = total - pos;
            if (remaining > note_len) remaining = note_len;
            memcpy(s->headline + pos, note, remaining);
            pos += remaining;
            s->headline[pos] = '\0';
        } else {
            strncpy(s->headline, s->entries[best_idx].date_str,
                    TS_NOTE_MAX - 1);
            s->headline[TS_NOTE_MAX - 1] = '\0';
        }
    } else {
        snprintf(s->headline, TS_NOTE_MAX, "A day in time");
    }
}

/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */

ts_summary_t ts_compute(double jd)
{
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.jd = jd;

    /* Compute each system */
    s.entries[TS_SYS_GREGORIAN]    = compute_gregorian(jd);
    s.entries[TS_SYS_TZOLKIN]      = compute_tzolkin(jd);
    s.entries[TS_SYS_HAAB]         = compute_haab(jd);
    s.entries[TS_SYS_CHINESE]      = compute_chinese(jd);
    s.entries[TS_SYS_HEBREW]       = compute_hebrew(jd);
    s.entries[TS_SYS_ISLAMIC]      = compute_islamic(jd);
    s.entries[TS_SYS_BUDDHIST]     = compute_buddhist(jd);
    s.entries[TS_SYS_HINDU]        = compute_hindu(jd);
    s.entries[TS_SYS_ICHING]       = compute_iching(jd);
    s.entries[TS_SYS_ASTROLOGY]    = compute_astrology(jd);
    s.entries[TS_SYS_HUMAN_DESIGN] = compute_human_design(jd);
    s.entries[TS_SYS_KABBALAH]     = compute_kabbalah(jd);
    s.entries[TS_SYS_COPTIC]       = compute_coptic(jd);
    s.entries[TS_SYS_ETHIOPIAN]    = compute_ethiopian(jd);
    s.entries[TS_SYS_PERSIAN]      = compute_persian(jd);
    s.entries[TS_SYS_JAPANESE]     = compute_japanese(jd);
    s.entries[TS_SYS_KOREAN]       = compute_korean(jd);
    s.entries[TS_SYS_THAI]         = compute_thai(jd);
    s.entries[TS_SYS_GEOLOGICAL]   = compute_geological(jd);
    s.entries[TS_SYS_COSMIC]       = compute_cosmic(jd);

    s.entry_count = TS_SYS_COUNT;

    /* Convergence + headline */
    s.convergence_score = compute_convergence(&s);
    compute_headline(&s);

    return s;
}

ts_entry_t ts_get_system(const ts_summary_t *summary, ts_system_t system)
{
    if ((int)system >= 0 && (int)system < summary->entry_count) {
        return summary->entries[(int)system];
    }
    /* Not found — return inactive entry */
    ts_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = system;
    e.active = 0;
    return e;
}

int ts_significant_count(const ts_summary_t *summary, int min_significance)
{
    int count = 0;
    for (int i = 0; i < summary->entry_count; i++) {
        if (summary->entries[i].active &&
            summary->entries[i].significance >= min_significance) {
            count++;
        }
    }
    return count;
}

int ts_format_all(const ts_summary_t *summary, char *buf, int buf_size)
{
    if (buf_size <= 0) return 0;
    buf[0] = '\0';

    int pos = 0;
    for (int i = 0; i < summary->entry_count; i++) {
        if (!summary->entries[i].active) continue;

        const char *name = ts_system_name(summary->entries[i].system);
        int remaining = buf_size - pos;
        if (remaining <= 1) break;

        int written;
        if (summary->entries[i].note[0] != '\0') {
            written = snprintf(buf + pos, (size_t)remaining,
                              "%s: %s (%s)\n",
                              name,
                              summary->entries[i].date_str,
                              summary->entries[i].note);
        } else {
            written = snprintf(buf + pos, (size_t)remaining,
                              "%s: %s\n",
                              name,
                              summary->entries[i].date_str);
        }

        if (written < 0) break;
        if (written >= remaining) {
            /* Truncated */
            pos = buf_size - 1;
            buf[pos] = '\0';
            break;
        }
        pos += written;
    }

    return pos;
}

int ts_format_brief(const ts_summary_t *summary, char *buf, int buf_size)
{
    if (buf_size <= 0) return 0;
    buf[0] = '\0';

    int pos = 0;
    int remaining = buf_size;

    /* Headline first */
    int written = snprintf(buf + pos, (size_t)remaining,
                          "%s\n", summary->headline);
    if (written < 0 || written >= remaining) {
        buf[buf_size - 1] = '\0';
        return buf_size - 1;
    }
    pos += written;
    remaining -= written;

    /* Top 5 most significant entries */
    int shown = 0;
    for (int sig = 3; sig >= 0 && shown < 5; sig--) {
        for (int i = 0; i < summary->entry_count && shown < 5; i++) {
            if (!summary->entries[i].active) continue;
            if (summary->entries[i].significance != sig) continue;

            const char *name = ts_system_name(summary->entries[i].system);
            written = snprintf(buf + pos, (size_t)remaining,
                              "  %s: %s\n", name,
                              summary->entries[i].date_str);
            if (written < 0 || written >= remaining) {
                buf[buf_size - 1] = '\0';
                return buf_size - 1;
            }
            pos += written;
            remaining -= written;
            shown++;
        }
    }

    return pos;
}

const char *ts_system_name(ts_system_t system)
{
    if ((int)system >= 0 && (int)system < TS_SYS_COUNT) {
        return SYSTEM_NAMES[(int)system];
    }
    return "?";
}

int ts_system_count(void)
{
    return TS_SYS_COUNT;
}
