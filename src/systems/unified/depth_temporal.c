/* depth_temporal.c — Temporal exploration for depth layers
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Known cycle lengths for each knowledge system. */

#include "depth_temporal.h"

#include <stdio.h>
#include <math.h>

#define DAYS_PER_YEAR 365.2425

/* Static cycle data table indexed by ts_system_t (0-19) */
static const dt_cycle_t s_cycles[TS_SYS_COUNT] = {
    [TS_SYS_GREGORIAN]    = { TS_SYS_GREGORIAN,    "Gregorian Year",         365.2425, 1.0,    "Solar year" },
    [TS_SYS_TZOLKIN]      = { TS_SYS_TZOLKIN,      "Tzolkin Sacred Cycle",   260.0,    0.712,  "260 sacred days — 13 tones x 20 seals" },
    [TS_SYS_HAAB]         = { TS_SYS_HAAB,         "Haab Solar Year",        365.0,    0.9993, "365-day Mayan solar year (no leap day)" },
    [TS_SYS_CHINESE]      = { TS_SYS_CHINESE,      "Sexagenary Cycle",       21915.0,  60.0,   "60-year stem-branch cycle" },
    [TS_SYS_HEBREW]       = { TS_SYS_HEBREW,       "Metonic Cycle",          6939.75,  19.0,   "19-year lunisolar cycle" },
    [TS_SYS_ISLAMIC]      = { TS_SYS_ISLAMIC,      "Islamic 30-Year Cycle",  10631.0,  30.0,   "30-year tabular cycle (360 lunations)" },
    [TS_SYS_BUDDHIST]     = { TS_SYS_BUDDHIST,     "Uposatha Cycle",         29.53,    0.0808, "Synodic month (lunar phase cycle)" },
    [TS_SYS_HINDU]        = { TS_SYS_HINDU,        "Tithi Cycle",            29.53,    0.0808, "30 tithis per synodic month" },
    [TS_SYS_ICHING]       = { TS_SYS_ICHING,       "Hexagram Cycle",         64.0,     0.175,  "64-day hexagram rotation" },
    [TS_SYS_ASTROLOGY]    = { TS_SYS_ASTROLOGY,    "Zodiac Year",            365.2425, 1.0,    "Sun through 12 signs" },
    [TS_SYS_HUMAN_DESIGN] = { TS_SYS_HUMAN_DESIGN, "Gate Cycle",             365.2425, 1.0,    "Sun through 64 gates" },
    [TS_SYS_KABBALAH]     = { TS_SYS_KABBALAH,     "Sefirot Cycle",          365.2425, 1.0,    "Annual Sefirot meditation cycle" },
    [TS_SYS_COPTIC]       = { TS_SYS_COPTIC,       "Coptic Year",            365.25,   1.0,    "Julian-based year" },
    [TS_SYS_ETHIOPIAN]    = { TS_SYS_ETHIOPIAN,     "Ethiopian Year",         365.25,   1.0,    "Julian-based year" },
    [TS_SYS_PERSIAN]      = { TS_SYS_PERSIAN,      "Persian Year",           365.2424, 1.0,    "Astronomical solar year" },
    [TS_SYS_JAPANESE]     = { TS_SYS_JAPANESE,      "Zodiac Year",            4383.0,   12.0,   "12-year animal cycle" },
    [TS_SYS_KOREAN]       = { TS_SYS_KOREAN,       "Dangun Cycle",           21915.0,  60.0,   "60-year sexagenary" },
    [TS_SYS_THAI]         = { TS_SYS_THAI,         "Buddhist Year",          365.2425, 1.0,    "Solar year (BE dating)" },
    [TS_SYS_GEOLOGICAL]   = { TS_SYS_GEOLOGICAL,   "Geological Epoch",       0.0,      0.0,    "No recurrence (linear deep time)" },
    [TS_SYS_COSMIC]       = { TS_SYS_COSMIC,       "Cosmic Age",             0.0,      0.0,    "No recurrence (linear cosmic time)" },
};

dt_cycle_t dt_cycle_info(int system_id)
{
    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        dt_cycle_t invalid = { -1, "?", 0.0, 0.0, "Unknown system" };
        return invalid;
    }
    return s_cycles[system_id];
}

dt_recurrence_t dt_recurrence(int system_id, double current_jd)
{
    dt_recurrence_t r = { 0.0, 0.0, 0.0, 0.0, 0 };

    if (current_jd <= 0.0) {
        return r;
    }

    dt_cycle_t c = dt_cycle_info(system_id);
    if (c.system_id < 0 || c.cycle_days <= 0.0) {
        return r;
    }

    r.next_jd = current_jd + c.cycle_days;
    r.prev_jd = current_jd - c.cycle_days;
    r.days_until = c.cycle_days;
    r.days_since = 0.0;
    r.occurrences_per_year = (int)(DAYS_PER_YEAR / c.cycle_days);

    return r;
}

/* Compute positive modulo (always returns 0..m-1) */
static int pos_mod(int val, int m)
{
    if (m <= 0) return 0;
    int result = val % m;
    if (result < 0) result += m;
    return result;
}

dt_snapshot_t dt_snapshot(int system_id, double current_jd, int years_offset)
{
    dt_snapshot_t s;
    s.years_offset = years_offset;
    s.state[0] = '\0';
    s.has_data = 0;

    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        return s;
    }

    s.has_data = 1;
    double offset_days = (double)years_offset * DAYS_PER_YEAR;

    /* Non-cyclical systems */
    if (system_id == TS_SYS_GEOLOGICAL || system_id == TS_SYS_COSMIC) {
        snprintf(s.state, sizeof(s.state), "Deep time — no cycle");
        return s;
    }

    dt_cycle_t c = s_cycles[system_id];

    if (system_id == TS_SYS_TZOLKIN) {
        /* Kin position: 1-260 */
        int offset_int = (int)floor(offset_days);
        int kin = pos_mod(offset_int, 260) + 1;
        snprintf(s.state, sizeof(s.state), "Kin %d", kin);
    } else if (system_id == TS_SYS_ICHING) {
        /* Hexagram position: 1-64 */
        int offset_int = (int)floor(offset_days);
        int hex = pos_mod(offset_int, 64) + 1;
        snprintf(s.state, sizeof(s.state), "Hexagram %d", hex);
    } else {
        /* Generic: cycle position */
        int cycle_int = (int)floor(c.cycle_days);
        if (cycle_int > 0) {
            int offset_int = (int)floor(offset_days);
            int pos = pos_mod(offset_int, cycle_int);
            snprintf(s.state, sizeof(s.state), "%s position %d", c.cycle_name, pos);
        } else {
            snprintf(s.state, sizeof(s.state), "%s", c.cycle_name);
        }
    }

    (void)current_jd;  /* used conceptually; cycle position is relative */

    return s;
}

int dt_cycle_count(void)
{
    return TS_SYS_COUNT;
}
