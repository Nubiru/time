/* calendar_convert.c — Calendar Conversion Engine
 * Unified JD-pivot conversion across 21 calendar systems.
 * Pure functions only. */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "calendar_convert.h"
#include "../../math/julian.h"
#include "../tzolkin/tzolkin.h"
#include "../tzolkin/haab.h"
#include "../tzolkin/cr_cycle.h"
#include "../chinese/chinese.h"
#include "../iching/iching.h"
#include "../hebrew/hebrew.h"
#include "../islamic/hijri.h"
#include "../persian/persian.h"
#include "../coptic/coptic.h"
#include "../ethiopian/ethiopian.h"
#include "../egyptian/egyptian.h"
#include "../french_republican/french_republican.h"
#include "../japanese/japanese.h"
#include "../bahai/bahai.h"
#include "../korean/korean_calendar.h"
#include "../thai/thai_calendar.h"
#include "../tamil/tamil_calendar.h"
#include "../myanmar/myanmar.h"
#include "../zoroastrian/zoroastrian.h"
#include "../celtic/celtic_tree.h"

/* ========== Display names ========== */

static const char *s_system_names[CAL_COUNT] = {
    "Gregorian",
    "Tzolkin",
    "Haab",
    "Chinese",
    "I Ching",
    "Hebrew",
    "Islamic",
    "Persian",
    "Coptic",
    "Ethiopian",
    "Egyptian",
    "French Republican",
    "Japanese",
    "Baha'i",
    "Korean",
    "Thai",
    "Tamil",
    "Myanmar",
    "Zoroastrian",
    "Celtic Tree",
    "Calendar Round"
};

/* Gregorian month names (no existing function in the codebase) */
static const char *s_gregorian_months[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

/* ========== Internal formatting helpers ========== */

static void fmt_gregorian(double jd, char *buf, int sz)
{
    int year, month;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)floor(day_frac);
    const char *mname = (month >= 1 && month <= 12)
                            ? s_gregorian_months[month - 1] : "?";
    snprintf(buf, (size_t)sz, "%d %s %d", year, mname, day);
}

static void fmt_tzolkin(double jd, char *buf, int sz)
{
    tzolkin_day_t t = tzolkin_from_jd(jd);
    snprintf(buf, (size_t)sz, "Kin %d: %s %s",
             t.kin, tzolkin_tone_name(t.tone), tzolkin_seal_name(t.seal));
}

static void fmt_haab(double jd, char *buf, int sz)
{
    haab_date_t h = haab_from_jd(jd);
    snprintf(buf, (size_t)sz, "%d %s", h.day, haab_month_name(h.month));
}

static void fmt_chinese(double jd, char *buf, int sz)
{
    chinese_year_t c = chinese_year_from_jd(jd);
    snprintf(buf, (size_t)sz, "Year of the %s (%s-%s)",
             chinese_animal_name(c.animal),
             chinese_stem_name(c.stem),
             chinese_branch_name(c.branch));
}

static void fmt_iching(double jd, char *buf, int sz)
{
    hexagram_t h = iching_from_jd(jd);
    snprintf(buf, (size_t)sz, "Hexagram %d: %s",
             h.king_wen, iching_hexagram_name(h.king_wen));
}

static void fmt_hebrew(double jd, char *buf, int sz)
{
    hebrew_date_t h = hebrew_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, %d",
             hebrew_month_name(h.month, hebrew_is_leap(h.year)),
             h.day, h.year);
}

static void fmt_islamic(double jd, char *buf, int sz)
{
    hijri_date_t h = hijri_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, %d",
             hijri_month_name(h.month), h.day, h.year);
}

static void fmt_persian(double jd, char *buf, int sz)
{
    persian_date_t p = persian_from_jd(jd);
    snprintf(buf, (size_t)sz, "%d %s %d",
             p.day, persian_month_name(p.month), p.year);
}

static void fmt_coptic(double jd, char *buf, int sz)
{
    coptic_date_t c = coptic_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, %d",
             coptic_month_name(c.month), c.day, c.year);
}

static void fmt_ethiopian(double jd, char *buf, int sz)
{
    ethiopian_date_t e = ethiopian_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, %d",
             ethiopian_month_name(e.month), e.day, e.year);
}

static void fmt_egyptian(double jd, char *buf, int sz)
{
    egypt_date_t e = egypt_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, Year %d",
             egypt_month_name(e.month), e.day, e.year);
}

static void fmt_french_republican(double jd, char *buf, int sz)
{
    fr_date_t f = fr_from_jd(jd);
    snprintf(buf, (size_t)sz, "%d %s An %d",
             f.day, fr_month_name(f.month), f.year);
}

static void fmt_japanese(double jd, char *buf, int sz)
{
    japanese_date_t j = japanese_from_jd(jd);
    if (j.era_index >= 0) {
        snprintf(buf, (size_t)sz, "%s %d, %d/%d",
                 japanese_era_name(j.era_index), j.era_year,
                 j.month, j.day);
    } else {
        snprintf(buf, (size_t)sz, "%d/%d/%d", j.greg_year, j.month, j.day);
    }
}

static void fmt_bahai(double jd, char *buf, int sz)
{
    bahai_date_t b = bahai_from_jd(jd);
    snprintf(buf, (size_t)sz, "%d %s %d BE",
             b.day, bahai_month_name(b.month), b.year);
}

static void fmt_korean(double jd, char *buf, int sz)
{
    korean_date_t k = korean_from_jd(jd);
    snprintf(buf, (size_t)sz, "Dangun %d (%s-%s)",
             k.dangun_year,
             korean_stem_name(k.stem),
             korean_branch_name(k.branch));
}

static void fmt_thai(double jd, char *buf, int sz)
{
    thai_date_t t = thai_from_jd(jd);
    snprintf(buf, (size_t)sz, "%d %s %d BE",
             t.day, thai_month_name(t.month), t.be_year);
}

static void fmt_tamil(double jd, char *buf, int sz)
{
    tamil_date_t t = tamil_from_jd(jd);
    snprintf(buf, (size_t)sz, "%d %s %d",
             t.day, tamil_month_name(t.month), t.year);
}

static void fmt_myanmar(double jd, char *buf, int sz)
{
    myanmar_date_t m = myanmar_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, ME %d",
             myanmar_month_name(m.month), m.day, m.year);
}

static void fmt_zoroastrian(double jd, char *buf, int sz)
{
    zoro_date_t z = zoro_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s %d, Year %d",
             zoro_month_name(z.month), z.day, z.year);
}

static void fmt_celtic_tree(double jd, char *buf, int sz)
{
    celtic_tree_date_t c = celtic_tree_from_jd(jd);
    snprintf(buf, (size_t)sz, "%s, Day %d",
             celtic_tree_name(c.month), c.day);
}

static void fmt_calendar_round(double jd, char *buf, int sz)
{
    calendar_round_t cr = calendar_round_from_jd(jd);
    calendar_round_fmt(cr, buf, (size_t)sz);
}

/* ========== Public API ========== */

const char *cal_system_name(cal_system_t sys)
{
    if (sys < 0 || sys >= CAL_COUNT) {
        return "?";
    }
    return s_system_names[sys];
}

int cal_system_count(void)
{
    return CAL_COUNT;
}

int cal_system_valid(cal_system_t sys)
{
    return sys >= 0 && sys < CAL_COUNT;
}

static cal_date_t make_invalid(void)
{
    cal_date_t d;
    memset(&d, 0, sizeof(d));
    snprintf(d.formatted, CAL_FMT_SIZE, "?");
    return d;
}

cal_date_t cal_convert(cal_system_t sys, double jd)
{
    cal_date_t d;
    memset(&d, 0, sizeof(d));
    d.system = sys;

    switch (sys) {
    case CAL_GREGORIAN: {
        int year, month;
        double day_frac = jd_to_gregorian(jd, &year, &month);
        d.year = year;
        d.month = month;
        d.day = (int)floor(day_frac);
        fmt_gregorian(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_TZOLKIN: {
        tzolkin_day_t t = tzolkin_from_jd(jd);
        d.month = t.tone;
        d.day = t.seal;
        d.extra = t.kin;
        fmt_tzolkin(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_HAAB: {
        haab_date_t h = haab_from_jd(jd);
        d.month = h.month;
        d.day = h.day;
        fmt_haab(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_CHINESE: {
        chinese_year_t c = chinese_year_from_jd(jd);
        d.year = c.cycle_year;
        d.month = c.element;
        d.day = c.animal;
        d.extra = c.cycle_year;
        fmt_chinese(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_ICHING: {
        hexagram_t h = iching_from_jd(jd);
        d.extra = h.king_wen;
        d.month = h.lower_trigram;
        d.day = h.upper_trigram;
        fmt_iching(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_HEBREW: {
        hebrew_date_t h = hebrew_from_jd(jd);
        d.year = h.year;
        d.month = h.month;
        d.day = h.day;
        fmt_hebrew(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_ISLAMIC: {
        hijri_date_t h = hijri_from_jd(jd);
        d.year = h.year;
        d.month = h.month;
        d.day = h.day;
        fmt_islamic(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_PERSIAN: {
        persian_date_t p = persian_from_jd(jd);
        d.year = p.year;
        d.month = p.month;
        d.day = p.day;
        fmt_persian(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_COPTIC: {
        coptic_date_t c = coptic_from_jd(jd);
        d.year = c.year;
        d.month = c.month;
        d.day = c.day;
        fmt_coptic(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_ETHIOPIAN: {
        ethiopian_date_t e = ethiopian_from_jd(jd);
        d.year = e.year;
        d.month = e.month;
        d.day = e.day;
        fmt_ethiopian(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_EGYPTIAN: {
        egypt_date_t e = egypt_from_jd(jd);
        d.year = e.year;
        d.month = e.month;
        d.day = e.day;
        fmt_egyptian(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_FRENCH_REPUBLICAN: {
        fr_date_t f = fr_from_jd(jd);
        d.year = f.year;
        d.month = f.month;
        d.day = f.day;
        fmt_french_republican(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_JAPANESE: {
        japanese_date_t j = japanese_from_jd(jd);
        d.year = j.greg_year;
        d.month = j.month;
        d.day = j.day;
        d.extra = j.era_year;
        fmt_japanese(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_BAHAI: {
        bahai_date_t b = bahai_from_jd(jd);
        d.year = b.year;
        d.month = b.month;
        d.day = b.day;
        fmt_bahai(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_KOREAN: {
        korean_date_t k = korean_from_jd(jd);
        d.year = k.dangun_year;
        d.month = k.lunar_month;
        d.day = k.lunar_day;
        d.extra = k.cycle_year;
        fmt_korean(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_THAI: {
        thai_date_t t = thai_from_jd(jd);
        d.year = t.be_year;
        d.month = t.month;
        d.day = t.day;
        fmt_thai(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_TAMIL: {
        tamil_date_t t = tamil_from_jd(jd);
        d.year = t.year;
        d.month = t.month;
        d.day = t.day;
        d.extra = t.jovian;
        fmt_tamil(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_MYANMAR: {
        myanmar_date_t m = myanmar_from_jd(jd);
        d.year = m.year;
        d.month = (int)m.month;
        d.day = m.day;
        fmt_myanmar(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_ZOROASTRIAN: {
        zoro_date_t z = zoro_from_jd(jd);
        d.year = z.year;
        d.month = z.month;
        d.day = z.day;
        fmt_zoroastrian(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_CELTIC_TREE: {
        celtic_tree_date_t c = celtic_tree_from_jd(jd);
        d.month = c.month;
        d.day = c.day;
        fmt_celtic_tree(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    case CAL_CALENDAR_ROUND: {
        cr_cycle_t cr = cr_cycle_from_jd(jd);
        d.day = cr.day_in_year;
        d.month = cr.year_in_round;
        d.extra = cr.day_in_round;
        fmt_calendar_round(jd, d.formatted, CAL_FMT_SIZE);
        break;
    }
    default:
        return make_invalid();
    }

    return d;
}

int cal_format(cal_system_t sys, double jd, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }
    if (!cal_system_valid(sys)) {
        buf[0] = '\0';
        return 0;
    }

    cal_date_t d = cal_convert(sys, jd);
    int n = snprintf(buf, (size_t)buf_size, "%s", d.formatted);
    if (n < 0) {
        buf[0] = '\0';
        return 0;
    }
    return n;
}

cal_date_t cal_convert_between(cal_system_t from, int year, int month, int day,
                               cal_system_t to)
{
    /* Only Gregorian source is supported (gregorian_to_jd always available) */
    if (from != CAL_GREGORIAN) {
        return make_invalid();
    }
    if (!cal_system_valid(to)) {
        return make_invalid();
    }

    double jd = gregorian_to_jd(year, month, (double)day);
    return cal_convert(to, jd);
}
