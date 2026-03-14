/* festival_detector.c — Cross-System Festival Detector
 * Aggregates festival detection from 10 calendar systems.
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Systems: Korean, Hebrew, Islamic, Buddhist, Celtic, Persian,
 *          Thai, Tamil, Myanmar, Zoroastrian. */

#include <math.h>
#include <stddef.h>
#include <string.h>

#include "festival_detector.h"
#include "../korean/korean_calendar.h"
#include "../hebrew/hebrew.h"
#include "../hebrew/sabbatical.h"
#include "../islamic/hijri.h"
#include "../buddhist/buddhist.h"
#include "../celtic/wheel_of_year.h"
#include "../thai/thai_calendar.h"
#include "../tamil/tamil_calendar.h"
#include "../myanmar/myanmar.h"
#include "../persian/persian.h"
#include "../zoroastrian/zoroastrian.h"

#define PI 3.14159265358979323846

/* ===== System names ===== */

static const char *SYSTEM_NAMES[FEST_SYS_COUNT] = {
    "Korean",
    "Hebrew",
    "Islamic",
    "Buddhist",
    "Celtic",
    "Persian",
    "Thai",
    "Tamil",
    "Myanmar",
    "Zoroastrian"
};

/* ===== Islamic festival table (hijri.h has no festival function) ===== */

typedef struct {
    int month;
    int day;
    fest_priority_t priority;
    const char *name;
    const char *description;
} islamic_fest_def_t;

#define ISLAMIC_FEST_DEF_COUNT 5

static const islamic_fest_def_t ISLAMIC_FESTS[ISLAMIC_FEST_DEF_COUNT] = {
    { 9,  1,  FEST_MAJOR, "Ramadan",         "Start of the month of fasting" },
    { 10, 1,  FEST_MAJOR, "Eid al-Fitr",     "Festival of breaking the fast" },
    { 12, 10, FEST_MAJOR, "Eid al-Adha",     "Festival of sacrifice" },
    { 1,  1,  FEST_MINOR, "Islamic New Year", "1 Muharram, start of new Hijri year" },
    { 3,  12, FEST_MINOR, "Mawlid",          "Birth of the Prophet Muhammad" }
};

/* ===== Descriptions for each system's festivals ===== */

/* Korean festival descriptions (mapped from korean_festival_t->meaning) */
static const char *KOREAN_DESC_MAJOR = "Major Korean traditional festival";
static const char *KOREAN_DESC_MINOR = "Korean traditional celebration";

/* Hebrew holiday description */
static const char *HEBREW_DESC = "Major Hebrew holiday";

/* Buddhist uposatha descriptions */
static const char *BUDDHIST_DESC_FULL  = "Full moon observance day (Purnima)";
static const char *BUDDHIST_DESC_NEW   = "New moon observance day (Amavasya)";
static const char *BUDDHIST_DESC_QTR   = "Quarter moon observance day";

/* Celtic wheel description */
static const char *CELTIC_DESC_SOLAR = "Solar event on the Wheel of the Year";
static const char *CELTIC_DESC_FIRE  = "Fire festival on the Wheel of the Year";

/* Persian festival descriptions */
static const char *PERSIAN_DESC_NOWRUZ  = "Persian New Year, spring equinox celebration";
static const char *PERSIAN_DESC_SIZDAH  = "Nature Day, 13th day of Farvardin";
static const char *PERSIAN_DESC_YALDA   = "Longest night celebration";
static const char *PERSIAN_DESC_CHAHAR  = "Fire-jumping festival, last Wednesday before Nowruz";

/* Thai festival descriptions */
static const char *THAI_DESC_SONGKRAN   = "Thai New Year water festival";
static const char *THAI_DESC_CHAKRI     = "Chakri Memorial Day";
static const char *THAI_DESC_CONST      = "Thai Constitution Day";

/* Tamil festival descriptions */
static const char *TAMIL_DESC_PUTHANDU  = "Tamil New Year";
static const char *TAMIL_DESC_PONGAL    = "Harvest festival, Thai Pongal";
static const char *TAMIL_DESC_CHITHIRAI = "Chithirai Thiruvizha temple festival";
static const char *TAMIL_DESC_DEEPAVALI = "Festival of lights";

/* Myanmar festival descriptions */
static const char *MYANMAR_DESC_THINGYAN = "Myanmar water festival (Thingyan)";
static const char *MYANMAR_DESC_NEWYEAR  = "Myanmar New Year (1 Tagu)";

/* Zoroastrian festival descriptions */
static const char *ZORO_DESC_NOWRUZ     = "Zoroastrian New Year";
static const char *ZORO_DESC_GAHANBAR   = "Zoroastrian seasonal Gahanbar festival";
static const char *ZORO_DESC_YALDA      = "Longest night celebration";
static const char *ZORO_DESC_SADEH      = "Mid-winter fire festival";

/* ===== Helper: add entry to result if room ===== */

static void add_entry(fest_result_t *r, fest_system_t sys,
                       fest_priority_t pri, const char *name,
                       const char *desc)
{
    if (r->count >= FEST_MAX_ACTIVE) return;
    r->entries[r->count].system = sys;
    r->entries[r->count].priority = pri;
    r->entries[r->count].name = name;
    r->entries[r->count].description = desc;
    r->count++;
}

/* ===== Approximate solar longitude from JD ===== */

static double approx_solar_longitude(double jd)
{
    /* Mean solar longitude approximation (Meeus simplified).
     * T = centuries from J2000.0 */
    double t = (jd - 2451545.0) / 36525.0;
    double l0 = 280.46646 + 36000.76983 * t + 0.0003032 * t * t;
    /* Reduce to 0-360 */
    l0 = fmod(l0, 360.0);
    if (l0 < 0.0) l0 += 360.0;
    return l0;
}

/* ===== Per-system detection functions ===== */

static void detect_korean(double jd, fest_result_t *r)
{
    korean_date_t kd = korean_from_jd(jd);
    const korean_festival_t *kf = korean_festival(kd.lunar_month, kd.lunar_day);
    if (kf != NULL) {
        /* Seollal (id=0) and Chuseok (id=5) are major */
        fest_priority_t pri = (kf->id == 0 || kf->id == 5)
                              ? FEST_MAJOR : FEST_MINOR;
        const char *desc = (pri == FEST_MAJOR)
                           ? KOREAN_DESC_MAJOR : KOREAN_DESC_MINOR;
        add_entry(r, FEST_SYS_KOREAN, pri, kf->name, desc);
    }
}

static void detect_hebrew(double jd, fest_result_t *r)
{
    hebrew_date_t hd = hebrew_from_jd(jd);
    int count = hebrew_holiday_count();
    for (int i = 0; i < count; i++) {
        hebrew_holiday_t hol = hebrew_holiday_get(hd.year, i);
        if (hol.month == hd.month && hol.day == hd.day) {
            add_entry(r, FEST_SYS_HEBREW, FEST_MAJOR, hol.name, HEBREW_DESC);
        }
    }
}

static void detect_islamic(double jd, fest_result_t *r)
{
    hijri_date_t hd = hijri_from_jd(jd);
    for (int i = 0; i < ISLAMIC_FEST_DEF_COUNT; i++) {
        if (hd.month == ISLAMIC_FESTS[i].month &&
            hd.day == ISLAMIC_FESTS[i].day) {
            add_entry(r, FEST_SYS_ISLAMIC, ISLAMIC_FESTS[i].priority,
                      ISLAMIC_FESTS[i].name, ISLAMIC_FESTS[i].description);
        }
    }
}

static void detect_buddhist(double jd, fest_result_t *r)
{
    uposatha_t u = buddhist_uposatha(jd);
    if (u.type != UPOSATHA_NONE) {
        const char *desc;
        switch (u.type) {
        case UPOSATHA_FULL_MOON:    desc = BUDDHIST_DESC_FULL; break;
        case UPOSATHA_NEW_MOON:     desc = BUDDHIST_DESC_NEW;  break;
        default:                    desc = BUDDHIST_DESC_QTR;   break;
        }
        add_entry(r, FEST_SYS_BUDDHIST, FEST_ASTRO, u.name, desc);
    }
}

static void detect_celtic(double jd, fest_result_t *r)
{
    double sol_long = approx_solar_longitude(jd);
    /* Use 3-degree threshold for matching a festival */
    int idx = wheel_active_festival(sol_long, 3.0);
    if (idx >= 0) {
        wheel_festival_info_t info = wheel_festival_info((wheel_festival_t)idx);
        const char *desc = info.is_solar_event ? CELTIC_DESC_SOLAR : CELTIC_DESC_FIRE;
        fest_priority_t pri = info.is_solar_event ? FEST_ASTRO : FEST_MAJOR;
        add_entry(r, FEST_SYS_CELTIC, pri, info.name, desc);
    }
}

static void detect_persian(double jd, fest_result_t *r)
{
    persian_date_t pd = persian_from_jd(jd);
    persian_festival_t pf = persian_festival(pd);
    if (pf != PERSIAN_FEST_NONE) {
        const char *name = persian_festival_name(pf);
        const char *desc;
        switch (pf) {
        case PERSIAN_FEST_NOWRUZ:            desc = PERSIAN_DESC_NOWRUZ;  break;
        case PERSIAN_FEST_SIZDAH_BEDAR:      desc = PERSIAN_DESC_SIZDAH;  break;
        case PERSIAN_FEST_YALDA:             desc = PERSIAN_DESC_YALDA;   break;
        case PERSIAN_FEST_CHAHARSHANBE_SURI: desc = PERSIAN_DESC_CHAHAR;  break;
        default:                             desc = "";                    break;
        }
        fest_priority_t pri = (pf == PERSIAN_FEST_NOWRUZ) ? FEST_MAJOR : FEST_MINOR;
        add_entry(r, FEST_SYS_PERSIAN, pri, name, desc);
    }
}

static void detect_thai(double jd, fest_result_t *r)
{
    thai_date_t td = thai_from_jd(jd);
    thai_festival_t tf = thai_festival(td);
    if (tf != THAI_FEST_NONE) {
        const char *name = thai_festival_name(tf);
        const char *desc;
        switch (tf) {
        case THAI_FEST_SONGKRAN:      desc = THAI_DESC_SONGKRAN; break;
        case THAI_FEST_CHAKRI_DAY:    desc = THAI_DESC_CHAKRI;   break;
        case THAI_FEST_CONSTITUTION:  desc = THAI_DESC_CONST;    break;
        default:                      desc = "";                  break;
        }
        fest_priority_t pri = (tf == THAI_FEST_SONGKRAN) ? FEST_MAJOR : FEST_MINOR;
        add_entry(r, FEST_SYS_THAI, pri, name, desc);
    }
}

static void detect_tamil(double jd, fest_result_t *r)
{
    tamil_date_t td = tamil_from_jd(jd);
    tamil_festival_t tf = tamil_festival(td);
    if (tf != TAMIL_FEST_NONE) {
        const char *name = tamil_festival_name(tf);
        const char *desc;
        switch (tf) {
        case TAMIL_FEST_PUTHANDU:       desc = TAMIL_DESC_PUTHANDU;  break;
        case TAMIL_FEST_PONGAL:         desc = TAMIL_DESC_PONGAL;    break;
        case TAMIL_FEST_CHITHIRAI_FEST: desc = TAMIL_DESC_CHITHIRAI; break;
        case TAMIL_FEST_DEEPAVALI:      desc = TAMIL_DESC_DEEPAVALI; break;
        default:                        desc = "";                    break;
        }
        fest_priority_t pri = (tf == TAMIL_FEST_PUTHANDU || tf == TAMIL_FEST_PONGAL)
                              ? FEST_MAJOR : FEST_MINOR;
        add_entry(r, FEST_SYS_TAMIL, pri, name, desc);
    }
}

static int myanmar_check_thingyan(double jd, double jd_floor,
                                   int me_year, fest_result_t *r)
{
    myanmar_thingyan_t tk = myanmar_thingyan(me_year);
    double akyo_floor = floor(tk.akyo_jd - 0.5) + 0.5;
    double akya_floor = floor(tk.akya_jd - 0.5) + 0.5;
    double ny_floor = floor(tk.new_year_jd - 0.5) + 0.5;

    (void)jd; /* suppress unused warning */

    if (jd_floor >= akyo_floor && jd_floor <= akya_floor) {
        add_entry(r, FEST_SYS_MYANMAR, FEST_MAJOR,
                  "Thingyan", MYANMAR_DESC_THINGYAN);
        return 1;
    }
    if (fabs(jd_floor - ny_floor) < 0.5) {
        add_entry(r, FEST_SYS_MYANMAR, FEST_MAJOR,
                  "Myanmar New Year", MYANMAR_DESC_NEWYEAR);
        return 1;
    }
    return 0;
}

static void detect_myanmar(double jd, fest_result_t *r)
{
    myanmar_date_t md = myanmar_from_jd(jd);
    double jd_floor = floor(jd - 0.5) + 0.5; /* normalize to midnight */

    /* Thingyan falls at year boundary — check current year and next year */
    if (myanmar_check_thingyan(jd, jd_floor, md.year, r)) return;
    if (myanmar_check_thingyan(jd, jd_floor, md.year + 1, r)) return;
}

static void detect_zoroastrian(double jd, fest_result_t *r)
{
    zoro_date_t zd = zoro_from_jd(jd);
    zoro_festival_t zf = zoro_festival(zd);
    if (zf != ZORO_FEST_NONE) {
        const char *name = zoro_festival_name(zf);
        const char *desc;
        switch (zf) {
        case ZORO_FEST_NOWRUZ:  desc = ZORO_DESC_NOWRUZ;   break;
        case ZORO_FEST_YALDA:   desc = ZORO_DESC_YALDA;    break;
        case ZORO_FEST_SADEH:   desc = ZORO_DESC_SADEH;    break;
        default:                desc = ZORO_DESC_GAHANBAR;  break;
        }
        fest_priority_t pri = (zf == ZORO_FEST_NOWRUZ) ? FEST_MAJOR : FEST_MINOR;
        add_entry(r, FEST_SYS_ZOROASTRIAN, pri, name, desc);
    }
}

/* ===== Public API ===== */

fest_result_t festival_detect(double jd)
{
    fest_result_t r;
    memset(&r, 0, sizeof(r));

    detect_korean(jd, &r);
    detect_hebrew(jd, &r);
    detect_islamic(jd, &r);
    detect_buddhist(jd, &r);
    detect_celtic(jd, &r);
    detect_persian(jd, &r);
    detect_thai(jd, &r);
    detect_tamil(jd, &r);
    detect_myanmar(jd, &r);
    detect_zoroastrian(jd, &r);

    return r;
}

int festival_count_for_system(fest_result_t result, fest_system_t sys)
{
    int n = 0;
    for (int i = 0; i < result.count; i++) {
        if (result.entries[i].system == sys) {
            n++;
        }
    }
    return n;
}

const char *festival_system_name(fest_system_t sys)
{
    if (sys < 0 || sys >= FEST_SYS_COUNT) return "?";
    return SYSTEM_NAMES[sys];
}

int festival_system_count(void)
{
    return FEST_SYS_COUNT;
}

int festival_any_active(double jd)
{
    fest_result_t r = festival_detect(jd);
    return r.count;
}

const fest_entry_t *festival_highest_priority(const fest_result_t *result)
{
    if (result == NULL || result->count == 0) return NULL;

    int best = 0;
    for (int i = 1; i < result->count; i++) {
        /* Lower enum value = higher priority (MAJOR=0 > MINOR=1 > ASTRO=2) */
        if (result->entries[i].priority < result->entries[best].priority) {
            best = i;
        }
    }
    return &result->entries[best];
}

int festival_total_defined(void)
{
    /* Korean: 7 festivals (KOREAN_FESTIVAL_COUNT)
     * Hebrew: 10 holidays (hebrew_holiday_count())
     * Islamic: 5 (internal table)
     * Buddhist: 4 uposatha types (FULL, NEW, FIRST_QUARTER, LAST_QUARTER)
     * Celtic: 8 wheel festivals (WHEEL_FESTIVAL_COUNT)
     * Persian: 4 festivals (PERSIAN_FEST_COUNT - 1, excluding NONE)
     * Thai: 3 festivals (THAI_FEST_COUNT - 1, excluding NONE)
     * Tamil: 4 festivals (TAMIL_FEST_COUNT - 1, excluding NONE)
     * Myanmar: 2 (Thingyan + New Year)
     * Zoroastrian: 9 (ZORO_FEST_COUNT - 1, excluding NONE) */
    return KOREAN_FESTIVAL_COUNT
         + hebrew_holiday_count()
         + ISLAMIC_FEST_DEF_COUNT
         + 4  /* Buddhist uposatha types */
         + WHEEL_FESTIVAL_COUNT
         + (PERSIAN_FEST_COUNT - 1)
         + (THAI_FEST_COUNT - 1)
         + (TAMIL_FEST_COUNT - 1)
         + 2  /* Myanmar: Thingyan + New Year */
         + (ZORO_FEST_COUNT - 1);
}
