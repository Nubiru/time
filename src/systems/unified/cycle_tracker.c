/* cycle_tracker.c — Personal cycle status from birth data + current date.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "cycle_tracker.h"
#include "../../math/julian.h"
#include "../tzolkin/tzolkin.h"
#include "../tzolkin/dreamspell.h"
#include "../astrology/zodiac.h"
#include "../astronomy/planets.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* ---- cycle names ---- */

static const char *CYCLE_NAMES[CT_CYCLE_COUNT] = {
    "Wavespell",
    "Castle",
    "Tzolkin Round",
    "Zodiac Transit",
    "Personal Year"
};

/* ---- helpers ---- */

static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static int days_in_year(int y)
{
    return is_leap(y) ? 366 : 365;
}

/* Day of year (1-366) for a Gregorian date. */
static int day_of_year(int year, int month, int day)
{
    static const int cum[] = {0,31,59,90,120,151,181,212,243,273,304,334};
    if (month < 1 || month > 12) return 0;
    int doy = cum[month - 1] + day;
    if (month > 2 && is_leap(year))
        doy++;
    return doy;
}

/* ---- cycle builders ---- */

static ct_cycle_t build_wavespell(double current_jd)
{
    ct_cycle_t c;
    memset(&c, 0, sizeof(c));
    c.id = CT_WAVESPELL;
    snprintf(c.name, CT_NAME_MAX, "%s", CYCLE_NAMES[CT_WAVESPELL]);
    c.cycle_length = 13;

    tzolkin_day_t tz = tzolkin_from_jd(current_jd);
    dreamspell_wavespell_t ws = dreamspell_wavespell(tz.kin);

    /* Tone tells position within wavespell (tone 1 = day 0, tone 13 = day 12) */
    c.days_elapsed = tz.tone - 1;
    c.days_remaining = 13 - tz.tone + 1;
    c.progress = (double)c.days_elapsed / 13.0;

    /* Value: wavespell purpose (seal name of the initiating seal) */
    const char *seal_name = tzolkin_seal_name(ws.seal);
    snprintf(c.value, CT_VALUE_MAX, "%s Wavespell", seal_name);

    return c;
}

static ct_cycle_t build_castle(double current_jd)
{
    ct_cycle_t c;
    memset(&c, 0, sizeof(c));
    c.id = CT_CASTLE;
    snprintf(c.name, CT_NAME_MAX, "%s", CYCLE_NAMES[CT_CASTLE]);
    c.cycle_length = 52;

    tzolkin_day_t tz = tzolkin_from_jd(current_jd);
    dreamspell_castle_t castle = dreamspell_castle(tz.kin);

    /* Position within castle: kin relative to castle start */
    c.days_elapsed = tz.kin - castle.start_kin;
    c.days_remaining = castle.end_kin - tz.kin + 1;
    c.progress = (double)c.days_elapsed / 52.0;

    snprintf(c.value, CT_VALUE_MAX, "%s", castle.name);

    return c;
}

static ct_cycle_t build_tzolkin_round(double current_jd)
{
    ct_cycle_t c;
    memset(&c, 0, sizeof(c));
    c.id = CT_TZOLKIN_ROUND;
    snprintf(c.name, CT_NAME_MAX, "%s", CYCLE_NAMES[CT_TZOLKIN_ROUND]);
    c.cycle_length = 260;

    tzolkin_day_t tz = tzolkin_from_jd(current_jd);

    /* Kin 1 = day 0, kin 260 = day 259 */
    c.days_elapsed = tz.kin - 1;
    c.days_remaining = 260 - tz.kin + 1;
    c.progress = (double)c.days_elapsed / 260.0;

    const char *tone = tzolkin_tone_name(tz.tone);
    const char *seal = tzolkin_seal_name(tz.seal);
    snprintf(c.value, CT_VALUE_MAX, "Kin %d: %s %s", tz.kin, tone, seal);

    return c;
}

static ct_cycle_t build_zodiac_transit(double current_jd)
{
    ct_cycle_t c;
    memset(&c, 0, sizeof(c));
    c.id = CT_ZODIAC_TRANSIT;
    snprintf(c.name, CT_NAME_MAX, "%s", CYCLE_NAMES[CT_ZODIAC_TRANSIT]);
    c.cycle_length = 30; /* approximate; signs span ~30 degrees ≈ 30 days */

    /* Compute current sun longitude */
    solar_system_t sys = planets_at(current_jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double sun_lon = fmod(earth_lon + 180.0, 360.0);
    if (sun_lon < 0.0) sun_lon += 360.0;

    int sign = zodiac_sign(sun_lon);
    double degree = zodiac_degree(sun_lon);

    c.days_elapsed = (int)degree;
    c.days_remaining = 30 - c.days_elapsed;
    if (c.days_remaining < 0) c.days_remaining = 0;
    c.progress = degree / 30.0;
    if (c.progress > 1.0) c.progress = 1.0;

    snprintf(c.value, CT_VALUE_MAX, "%s", zodiac_sign_name(sign));

    return c;
}

static ct_cycle_t build_personal_year(const birth_profile_t *profile,
                                       double current_jd)
{
    ct_cycle_t c;
    memset(&c, 0, sizeof(c));
    c.id = CT_PERSONAL_YEAR;
    snprintf(c.name, CT_NAME_MAX, "%s", CYCLE_NAMES[CT_PERSONAL_YEAR]);

    /* Get current Gregorian date */
    int cur_year, cur_month;
    double cur_day_frac = jd_to_gregorian(current_jd, &cur_year, &cur_month);
    int cur_day = (int)cur_day_frac;

    int cur_doy = day_of_year(cur_year, cur_month, cur_day);
    int birth_doy = day_of_year(profile->year, profile->month, profile->day);
    int year_len = days_in_year(cur_year);

    c.cycle_length = year_len;

    /* Days until next birthday */
    if (birth_doy > cur_doy) {
        c.days_remaining = birth_doy - cur_doy;
    } else if (birth_doy < cur_doy) {
        c.days_remaining = year_len - cur_doy + birth_doy;
    } else {
        /* It's your birthday — next one is a full year away */
        c.days_remaining = year_len;
    }

    c.days_elapsed = year_len - c.days_remaining;
    if (c.days_elapsed >= year_len) c.days_elapsed = 0;
    c.progress = (double)c.days_elapsed / (double)year_len;

    snprintf(c.value, CT_VALUE_MAX, "Age %d", cur_year - profile->year -
             (cur_doy < birth_doy ? 1 : 0));

    return c;
}

/* ---- public API ---- */

ct_tracker_t ct_compute(const birth_profile_t *profile, double current_jd)
{
    ct_tracker_t t;
    memset(&t, 0, sizeof(t));

    if (!profile) return t;

    /* Days alive */
    double birth_jd = gregorian_to_jd(profile->year, profile->month,
                                       (double)profile->day);
    t.days_alive = (int)(current_jd - birth_jd);

    /* Approximate age */
    int cur_year, cur_month;
    double cur_day_frac = jd_to_gregorian(current_jd, &cur_year, &cur_month);
    int cur_day = (int)cur_day_frac;
    int cur_doy = day_of_year(cur_year, cur_month, cur_day);
    int birth_doy = day_of_year(profile->year, profile->month, profile->day);

    t.age_years = cur_year - profile->year;
    if (cur_doy < birth_doy) t.age_years--;

    /* Build cycles */
    t.cycles[CT_WAVESPELL]      = build_wavespell(current_jd);
    t.cycles[CT_CASTLE]         = build_castle(current_jd);
    t.cycles[CT_TZOLKIN_ROUND]  = build_tzolkin_round(current_jd);
    t.cycles[CT_ZODIAC_TRANSIT] = build_zodiac_transit(current_jd);
    t.cycles[CT_PERSONAL_YEAR]  = build_personal_year(profile, current_jd);

    t.count = CT_CYCLE_COUNT;

    /* Days until birthday from personal year cycle */
    t.days_until_birthday = t.cycles[CT_PERSONAL_YEAR].days_remaining;

    return t;
}

const ct_cycle_t *ct_cycle(const ct_tracker_t *tracker, ct_cycle_id_t id)
{
    if (!tracker || (int)id < 0 || id >= CT_CYCLE_COUNT)
        return NULL;
    return &tracker->cycles[id];
}

const char *ct_cycle_name(ct_cycle_id_t id)
{
    if ((int)id < 0 || id >= CT_CYCLE_COUNT) return NULL;
    return CYCLE_NAMES[id];
}

int ct_any_transition_soon(const ct_tracker_t *tracker, int within_days)
{
    return ct_transitions_soon(tracker, within_days) > 0;
}

int ct_transitions_soon(const ct_tracker_t *tracker, int within_days)
{
    if (!tracker) return 0;
    int n = 0;
    for (int i = 0; i < CT_CYCLE_COUNT; i++) {
        if (tracker->cycles[i].days_remaining <= within_days) n++;
    }
    return n;
}
