/* ring_today.c — Today's active segment index per knowledge system. */

#include "ring_today.h"
#include "system_rings.h"

#include "../systems/gregorian/gregorian.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/haab.h"
#include "../systems/chinese/chinese.h"
#include "../systems/hebrew/hebrew.h"
#include "../systems/islamic/hijri.h"
#include "../systems/buddhist/buddhist.h"
#include "../systems/hindu/nakshatra.h"
#include "../systems/iching/iching.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/human_design/human_design.h"
#include "../systems/coptic/coptic.h"
#include "../systems/ethiopian/ethiopian.h"
#include "../systems/persian/persian.h"
#include "../systems/japanese/japanese.h"
#include "../systems/korean/korean_calendar.h"
#include "../systems/thai/thai_calendar.h"
#include "../systems/geology/geo_time.h"

ring_today_t ring_today_compute(double jd, double sun_lon_deg,
                                 double moon_lon_deg)
{
    ring_today_t rt;

    /* Initialize all to -1 (unknown) */
    for (int i = 0; i < RT_MAX_SYSTEMS; i++) {
        rt.indices[i] = -1;
    }

    /* TS_SYS_GREGORIAN (0): day of week 0-6 */
    rt.indices[TS_SYS_GREGORIAN] = gregorian_day_of_week(jd);

    /* TS_SYS_TZOLKIN (1): kin 1-260 -> 0-259 */
    {
        tzolkin_day_t tz = tzolkin_from_jd(jd);
        rt.indices[TS_SYS_TZOLKIN] = tz.kin - 1;
    }

    /* TS_SYS_HAAB (2): month 0-18 */
    {
        haab_date_t hb = haab_from_jd(jd);
        rt.indices[TS_SYS_HAAB] = hb.month;
    }

    /* TS_SYS_CHINESE (3): cycle_year 1-60 -> 0-59 */
    {
        chinese_year_t cy = chinese_year_from_jd(jd);
        rt.indices[TS_SYS_CHINESE] = cy.cycle_year - 1;
    }

    /* TS_SYS_HEBREW (4): month 1-13 -> 0-12 */
    {
        hebrew_date_t hd = hebrew_from_jd(jd);
        rt.indices[TS_SYS_HEBREW] = hd.month - 1;
    }

    /* TS_SYS_ISLAMIC (5): month 1-12 -> 0-11 */
    {
        hijri_date_t hi = hijri_from_jd(jd);
        rt.indices[TS_SYS_ISLAMIC] = hi.month - 1;
    }

    /* TS_SYS_BUDDHIST (6): uposatha type enum 0-4 */
    {
        uposatha_t up = buddhist_uposatha(jd);
        rt.indices[TS_SYS_BUDDHIST] = (int)up.type;
    }

    /* TS_SYS_HINDU (7): nakshatra id 0-26 */
    {
        nakshatra_t nk = nakshatra_from_longitude(moon_lon_deg);
        rt.indices[TS_SYS_HINDU] = nk.id;
    }

    /* TS_SYS_ICHING (8): king_wen 1-64 -> 0-63 */
    {
        hexagram_t hex = iching_from_jd(jd);
        rt.indices[TS_SYS_ICHING] = hex.king_wen - 1;
    }

    /* TS_SYS_ASTROLOGY (9): zodiac sign 0-11 */
    rt.indices[TS_SYS_ASTROLOGY] = zodiac_sign(sun_lon_deg);

    /* TS_SYS_HUMAN_DESIGN (10): gate 1-64 -> 0-63 */
    {
        hd_gate_t gate = hd_gate_from_longitude(sun_lon_deg);
        rt.indices[TS_SYS_HUMAN_DESIGN] = gate.gate - 1;
    }

    /* TS_SYS_KABBALAH (11): no JD-driven mapping */
    rt.indices[TS_SYS_KABBALAH] = -1;

    /* TS_SYS_COPTIC (12): month 1-13 -> 0-12 */
    {
        coptic_date_t cop = coptic_from_jd(jd);
        rt.indices[TS_SYS_COPTIC] = cop.month - 1;
    }

    /* TS_SYS_ETHIOPIAN (13): month 1-13 -> 0-12 */
    {
        ethiopian_date_t eth = ethiopian_from_jd(jd);
        rt.indices[TS_SYS_ETHIOPIAN] = eth.month - 1;
    }

    /* TS_SYS_PERSIAN (14): month 1-12 -> 0-11 */
    {
        persian_date_t per = persian_from_jd(jd);
        rt.indices[TS_SYS_PERSIAN] = per.month - 1;
    }

    /* TS_SYS_JAPANESE (15): month 1-12 -> 0-11 */
    {
        japanese_date_t jap = japanese_from_jd(jd);
        rt.indices[TS_SYS_JAPANESE] = jap.month - 1;
    }

    /* TS_SYS_KOREAN (16): lunar_month 1-12 -> 0-11 */
    {
        korean_date_t kor = korean_from_jd(jd);
        rt.indices[TS_SYS_KOREAN] = kor.lunar_month - 1;
    }

    /* TS_SYS_THAI (17): month 1-12 -> 0-11 */
    {
        thai_date_t th = thai_from_jd(jd);
        rt.indices[TS_SYS_THAI] = th.month - 1;
    }

    /* TS_SYS_GEOLOGICAL (18): era index via Ma lookup.
     * Clamp negative Ma (future dates) to 0.0 (present). */
    {
        double ma = geo_jd_to_ma(jd);
        if (ma < 0.0) {
            ma = 0.0;
        }
        geo_unit_t era = geo_era_at(ma);
        int era_idx = -1;
        if (era.name) {
            for (int i = 0; i < geo_era_count(); i++) {
                geo_unit_t e = geo_era_by_index(i);
                if (e.start_ma == era.start_ma && e.end_ma == era.end_ma) {
                    era_idx = i;
                    break;
                }
            }
        }
        rt.indices[TS_SYS_GEOLOGICAL] = era_idx;
    }

    /* TS_SYS_COSMIC (19): scale too large */
    rt.indices[TS_SYS_COSMIC] = -1;

    /* TS_SYS_EARTH (20): daily cycle, map to Gregorian ring */
    rt.indices[TS_SYS_EARTH] = rt.indices[TS_SYS_GREGORIAN];

    return rt;
}

int ring_today_index(const ring_today_t *rt, int system_id)
{
    if (!rt || system_id < 0 || system_id >= RT_MAX_SYSTEMS) {
        return -1;
    }
    return rt->indices[system_id];
}

void ring_today_for_depth(const ring_today_t *rt, int out[6])
{
    if (!rt || !out) {
        return;
    }
    for (int i = 0; i < 6; i++) {
        int sys = sr_depth_representative(i);
        out[i] = ring_today_index(rt, sys);
    }
}
