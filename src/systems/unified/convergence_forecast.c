/* convergence_forecast.c — Predicts upcoming convergence events
 *
 * Uses cycle-based approximation: each system has a known period between
 * significant moments. fmod-based phase detection identifies which days
 * multiple systems align. No full calendar computation needed.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "convergence_forecast.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

/* ===== Schedule table ===== */

/* Fixed epoch offsets per system — arbitrary phase shifts that produce
 * different alignment patterns. Values don't need astronomical accuracy;
 * they just ensure systems don't all fire on the same day. */
static const double EPOCH_OFFSETS[TS_SYS_COUNT] = {
    /* GREGORIAN  */  0.0,
    /* TZOLKIN    */  3.0,
    /* HAAB       */  7.0,
    /* CHINESE    */  5.0,
    /* HEBREW     */  11.0,
    /* ISLAMIC    */  17.0,
    /* BUDDHIST   */  2.0,
    /* HINDU      */  9.0,
    /* ICHING     */  22.0,
    /* ASTROLOGY  */  14.0,
    /* HUMAN_DES  */  1.5,
    /* KABBALAH   */  19.0,
    /* COPTIC     */  8.0,
    /* ETHIOPIAN  */  10.0,
    /* PERSIAN    */  6.0,
    /* JAPANESE   */  13.0,
    /* KOREAN     */  16.0,
    /* THAI       */  4.0,
    /* GEOLOGICAL */  0.0,
    /* COSMIC     */  0.0,
};

/* Period in days and event name for each system */
static const double PERIODS[TS_SYS_COUNT] = {
    /* GREGORIAN  */ 30.44,
    /* TZOLKIN    */ 13.0,
    /* HAAB       */ 20.0,
    /* CHINESE    */ 29.53,
    /* HEBREW     */ 29.53,
    /* ISLAMIC    */ 29.53,
    /* BUDDHIST   */  7.38,
    /* HINDU      */ 14.77,
    /* ICHING     */ 64.0,
    /* ASTROLOGY  */ 30.44,
    /* HUMAN_DES  */  5.7,
    /* KABBALAH   */ 30.44,
    /* COPTIC     */ 30.0,
    /* ETHIOPIAN  */ 30.0,
    /* PERSIAN    */ 30.0,
    /* JAPANESE   */ 30.44,
    /* KOREAN     */ 30.44,
    /* THAI       */ 30.44,
    /* GEOLOGICAL */  0.0,
    /* COSMIC     */  0.0,
};

static const char *EVENT_NAMES[TS_SYS_COUNT] = {
    /* GREGORIAN  */ "Month boundary",
    /* TZOLKIN    */ "Wavespell start",
    /* HAAB       */ "Month boundary",
    /* CHINESE    */ "New moon",
    /* HEBREW     */ "Rosh Chodesh",
    /* ISLAMIC    */ "New crescent",
    /* BUDDHIST   */ "Uposatha",
    /* HINDU      */ "Paksha boundary",
    /* ICHING     */ "Hexagram 1",
    /* ASTROLOGY  */ "Sign ingress",
    /* HUMAN_DES  */ "Gate transition",
    /* KABBALAH   */ "Sefirah shift",
    /* COPTIC     */ "Month boundary",
    /* ETHIOPIAN  */ "Month boundary",
    /* PERSIAN    */ "Month boundary",
    /* JAPANESE   */ "Month boundary",
    /* KOREAN     */ "Month boundary",
    /* THAI       */ "Month boundary",
    /* GEOLOGICAL */ "",
    /* COSMIC     */ "",
};

/* Short names for headline formatting */
static const char *SHORT_NAMES[TS_SYS_COUNT] = {
    "Gregorian", "Tzolkin", "Haab", "Chinese",
    "Hebrew", "Islamic", "Buddhist", "Hindu",
    "I Ching", "Astrology", "Human Design", "Kabbalah",
    "Coptic", "Ethiopian", "Persian", "Japanese",
    "Korean", "Thai", "Geological", "Cosmic",
};

/* ===== cf_schedule ===== */

cf_schedule_t cf_schedule(int system_id)
{
    cf_schedule_t s;
    memset(&s, 0, sizeof(s));

    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        s.system_id = -1;
        s.period_days = 0.0;
        s.event_name = "";
        return s;
    }

    s.system_id = system_id;
    s.period_days = PERIODS[system_id];
    s.event_name = EVENT_NAMES[system_id];
    return s;
}

int cf_schedule_count(void)
{
    return TS_SYS_COUNT;
}

/* ===== cf_strength_name ===== */

const char *cf_strength_name(int strength)
{
    switch (strength) {
    case 1: return "Minor";
    case 2: return "Notable";
    case 3: return "Major";
    case 4: return "Rare";
    default: return "?";
    }
}

/* ===== Internal: check if system has event on a day ===== */

static int system_has_event(int sys, double day_jd)
{
    double period = PERIODS[sys];
    if (period <= 0.0) return 0;

    double offset = EPOCH_OFFSETS[sys];
    double phase = fmod(day_jd - offset, period);
    if (phase < 0.0) phase += period;

    /* Event fires if phase is within 1 day of cycle start */
    return (phase < 1.0) ? 1 : 0;
}

/* ===== Internal: compute strength from system count ===== */

static int strength_from_count(int sys_count)
{
    if (sys_count >= 5) return 4;  /* Rare */
    if (sys_count >= 4) return 3;  /* Major */
    if (sys_count >= 3) return 2;  /* Notable */
    if (sys_count >= 2) return 1;  /* Minor */
    return 0;
}

/* ===== Internal: format headline ===== */

static void format_headline(cf_event_t *ev)
{
    char buf[128];
    int pos = 0;
    int remaining = (int)sizeof(buf);

    for (int i = 0; i < ev->system_count && i < CF_MAX_SYSTEMS_PER; i++) {
        int sys = ev->systems[i];
        const char *name = (sys >= 0 && sys < TS_SYS_COUNT)
                           ? SHORT_NAMES[sys] : "?";

        int written;
        if (i == 0) {
            written = snprintf(buf + pos, (size_t)remaining, "%s", name);
        } else {
            written = snprintf(buf + pos, (size_t)remaining, ", %s", name);
        }
        if (written < 0 || written >= remaining) break;
        pos += written;
        remaining -= written;
    }

    snprintf(ev->headline, sizeof(ev->headline), "%s", buf);
}

/* ===== Internal: format detail ===== */

static void format_detail(cf_event_t *ev)
{
    char buf[256];
    int pos = 0;
    int remaining = (int)sizeof(buf);

    int written = snprintf(buf, (size_t)remaining,
                           "Day %d: %d systems align - ",
                           ev->days_from_now, ev->system_count);
    if (written > 0 && written < remaining) {
        pos += written;
        remaining -= written;
    }

    for (int i = 0; i < ev->system_count && i < CF_MAX_SYSTEMS_PER; i++) {
        int sys = ev->systems[i];
        const char *ename = (sys >= 0 && sys < TS_SYS_COUNT)
                            ? EVENT_NAMES[sys] : "?";
        const char *sname = (sys >= 0 && sys < TS_SYS_COUNT)
                            ? SHORT_NAMES[sys] : "?";

        if (i > 0) {
            written = snprintf(buf + pos, (size_t)remaining, "; ");
            if (written > 0 && written < remaining) {
                pos += written;
                remaining -= written;
            }
        }
        written = snprintf(buf + pos, (size_t)remaining, "%s (%s)", sname, ename);
        if (written < 0 || written >= remaining) break;
        pos += written;
        remaining -= written;
    }

    snprintf(ev->detail, sizeof(ev->detail), "%s", buf);
}

/* ===== cf_forecast ===== */

cf_forecast_t cf_forecast(double start_jd, int max_days)
{
    cf_forecast_t result;
    memset(&result, 0, sizeof(result));

    /* Validate inputs */
    if (start_jd <= 0.0 || max_days <= 0) {
        return result;
    }

    /* Clamp max_days to 365 */
    if (max_days > 365) {
        max_days = 365;
    }

    /* Scan each day in the range */
    for (int d = 0; d < max_days; d++) {
        double day_jd = start_jd + (double)d;

        /* Count how many systems have events on this day */
        int sys_hits[CF_MAX_SYSTEMS_PER];
        int hit_count = 0;

        for (int s = 0; s < TS_SYS_COUNT && hit_count < CF_MAX_SYSTEMS_PER; s++) {
            if (system_has_event(s, day_jd)) {
                sys_hits[hit_count++] = s;
            }
        }

        /* Need at least 2 systems for a convergence */
        if (hit_count < 2) continue;

        /* Create event */
        if (result.count >= CF_MAX_FORECASTS) break;

        cf_event_t *ev = &result.events[result.count];
        memset(ev, 0, sizeof(*ev));
        ev->jd = day_jd;
        ev->days_from_now = d;
        ev->system_count = hit_count;
        for (int i = 0; i < hit_count; i++) {
            ev->systems[i] = sys_hits[i];
        }
        ev->strength = strength_from_count(hit_count);

        format_headline(ev);
        format_detail(ev);

        if (ev->strength > result.strongest) {
            result.strongest = ev->strength;
        }

        result.count++;
    }

    return result;
}
