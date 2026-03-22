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
#include "../systems/astronomy/lunar.h"
#include "../systems/zoroastrian/zoroastrian.h"
#include "../systems/balinese/pawukon.h"
#include "../systems/french_republican/french_republican.h"
#include "../systems/aztec/aztec.h"
#include "../systems/egyptian/egyptian.h"
#include "../systems/celtic/celtic_tree.h"
#include "../systems/lao/lao_calendar.h"
#include "../systems/myanmar/myanmar.h"
#include "../systems/bahai/bahai.h"
#include "../systems/tamil/tamil_calendar.h"
#include "../systems/tarot/tarot.h"
#include "../systems/numerology/numerology.h"
#include "../systems/chakra/chakra.h"

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

    /* TS_SYS_ASTRONOMY (21): moon phase 0-7 */
    {
        lunar_info_t li = lunar_phase(jd);
        rt.indices[TS_SYS_ASTRONOMY] = li.phase;
    }

    /* TS_SYS_TAROT (22): major arcanum 0-21 from JD */
    {
        int card = ((int)(jd + 0.5)) % 22;
        if (card < 0) card += 22;
        rt.indices[TS_SYS_TAROT] = card;
    }

    /* TS_SYS_NUMEROLOGY (23): root number 1-9 -> 0-8 */
    {
        int d_sum = ((int)(jd + 0.5)) % 9;
        if (d_sum < 0) d_sum += 9;
        rt.indices[TS_SYS_NUMEROLOGY] = d_sum;
    }

    /* TS_SYS_CHAKRA (24): day of week 0-6 */
    {
        int dow = ((int)(jd + 0.5) + 1) % 7;
        if (dow < 0) dow += 7;
        rt.indices[TS_SYS_CHAKRA] = dow;
    }

    /* TS_SYS_ZOROASTRIAN (25): month 1-12 -> 0-11 */
    {
        zoro_date_t zd = zoro_from_jd(jd);
        rt.indices[TS_SYS_ZOROASTRIAN] = (zd.month >= 1 && zd.month <= 12) ? zd.month - 1 : 0;
    }

    /* TS_SYS_BALINESE (26): day in 210-day cycle */
    {
        pawukon_t pw = pawukon_from_jd(jd);
        rt.indices[TS_SYS_BALINESE] = pw.day_in_cycle;
    }

    /* TS_SYS_FRENCH_REPUBLICAN (27): month 0-12 -> 0-11 */
    {
        fr_date_t fd = fr_from_jd(jd);
        rt.indices[TS_SYS_FRENCH_REPUBLICAN] = (fd.month >= 1) ? fd.month - 1 : 11;
    }

    /* TS_SYS_AZTEC (28): Tonalpohualli day 1-260 -> 0-259 */
    {
        aztec_tonalpohualli_t at = aztec_tonalpohualli(jd);
        rt.indices[TS_SYS_AZTEC] = at.day_index;
    }

    /* TS_SYS_EGYPTIAN (29): month 1-13 -> 0-11 */
    {
        egypt_date_t ed = egypt_from_jd(jd);
        rt.indices[TS_SYS_EGYPTIAN] = (ed.month >= 1 && ed.month <= 12) ? ed.month - 1 : 0;
    }

    /* TS_SYS_CELTIC (30): tree month 0-13 */
    {
        celtic_tree_date_t cd = celtic_tree_from_jd(jd);
        rt.indices[TS_SYS_CELTIC] = (cd.month >= 1) ? cd.month - 1 : 12;
    }

    /* TS_SYS_LAO (31): month 1-12 -> 0-11 */
    {
        lao_date_t ld = lao_from_jd(jd);
        rt.indices[TS_SYS_LAO] = (ld.month >= 1 && ld.month <= 12) ? ld.month - 1 : 0;
    }

    /* TS_SYS_MYANMAR (32): month enum 0-12 */
    {
        myanmar_date_t md = myanmar_from_jd(jd);
        rt.indices[TS_SYS_MYANMAR] = (int)md.month;
    }

    /* TS_SYS_BAHAI (33): month 0-19 -> 0-18 */
    {
        bahai_date_t bd = bahai_from_jd(jd);
        rt.indices[TS_SYS_BAHAI] = (bd.month >= 1) ? bd.month - 1 : 18;
    }

    /* TS_SYS_TAMIL (34): month 1-12 -> 0-11 */
    {
        tamil_date_t td = tamil_from_jd(jd);
        rt.indices[TS_SYS_TAMIL] = (td.month >= 1 && td.month <= 12) ? td.month - 1 : 0;
    }

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
