#include "haab.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/*
 * Haab: 18 months of 20 days + 5 Wayeb days = 365 days.
 * Using GMT correlation 584283.
 *
 * The Haab epoch anchor: Long Count 0.0.0.0.0 = JD 584283 (Julian Day Number).
 * On that date, the Haab was 8 Kumk'u (month 17, day 8).
 *
 * From any JD, compute day offset from correlation, then find Haab position.
 */

/* Haab offset at Long Count epoch: 8 Kumk'u = month 17, day 8
 * = 17*20 + 8 = 348 days into the Haab year */
#define HAAB_EPOCH_OFFSET 348

static const char *MONTH_NAMES[19] = {
    "Pop",      "Wo",       "Sip",      "Sotz'",    "Sek",
    "Xul",      "Yaxk'in",  "Mol",      "Ch'en",    "Yax",
    "Sak",      "Keh",      "Mak",      "K'ank'in", "Muwan",
    "Pax",      "K'ayab",   "Kumk'u",   "Wayeb"
};

static const char *MONTH_MEANINGS[19] = {
    "Mat",              "Black conjunction",  "Red conjunction",
    "Bat",              "Death",              "Dog",
    "New Sun",          "Water",              "Black storm",
    "Green storm",      "White storm",        "Red storm",
    "Enclosure",        "Yellow Sun",         "Owl",
    "Planting time",    "Turtle",             "Granary",
    "Nameless days"
};

haab_date_t haab_from_jd(double jd)
{
    haab_date_t h;
    long day_num = (long)floor(jd + 0.5);

    /* Days since Long Count epoch */
    long offset = day_num - 584283L;

    /* Add Haab epoch offset and normalize to 0..364 */
    long haab_day = (offset + HAAB_EPOCH_OFFSET) % 365;
    if (haab_day < 0) haab_day += 365;

    if (haab_day < 360) {
        /* Regular months: 18 months x 20 days */
        h.month = (int)(haab_day / 20);
        h.day = (int)(haab_day % 20);
    } else {
        /* Wayeb: days 360-364 */
        h.month = 18;
        h.day = (int)(haab_day - 360);
    }

    return h;
}

const char *haab_month_name(int month)
{
    if (month < 0 || month > 18) return "?";
    return MONTH_NAMES[month];
}

const char *haab_month_meaning(int month)
{
    if (month < 0 || month > 18) return "?";
    return MONTH_MEANINGS[month];
}

char *haab_format(haab_date_t date, char *buf, int max)
{
    if (!buf) return NULL;
    if (max <= 0) return buf;

    const char *name = haab_month_name(date.month);
    snprintf(buf, (unsigned long)max, "%d %s", date.day, name);
    return buf;
}

calendar_round_t calendar_round_from_jd(double jd)
{
    calendar_round_t cr;
    haab_date_t h = haab_from_jd(jd);
    cr.haab_month = h.month;
    cr.haab_day = h.day;

    /* Compute Tzolkin inline (avoid dependency on tzolkin.c).
     * Tzolkin uses the same GMT correlation 584283.
     * At Long Count 0.0.0.0.0: Tzolkin = 4 Ahau (tone 4, seal 19).
     * Tone: (day_offset + 4 - 1) % 13 + 1 = (day_offset + 3) % 13 + 1
     * Seal: (day_offset + 19) % 20 */
    long day_num = (long)floor(jd + 0.5);
    long offset = day_num - 584283L;

    long tone_raw = (offset + 3) % 13;
    if (tone_raw < 0) tone_raw += 13;
    cr.tzolkin_tone = (int)tone_raw + 1;

    long seal_raw = (offset + 19) % 20;
    if (seal_raw < 0) seal_raw += 20;
    cr.tzolkin_seal = (int)seal_raw;

    return cr;
}

int calendar_round_cycle(void)
{
    /* LCM(260, 365) = 18980 */
    return 18980;
}

int calendar_round_next(double jd, calendar_round_t target)
{
    /* Brute force search up to 18980 days. For a pure module this is acceptable. */
    for (int d = 0; d < 18980; d++) {
        calendar_round_t cr = calendar_round_from_jd(jd + (double)d);
        if (cr.tzolkin_tone == target.tzolkin_tone &&
            cr.tzolkin_seal == target.tzolkin_seal &&
            cr.haab_month == target.haab_month &&
            cr.haab_day == target.haab_day) {
            return d;
        }
    }
    return -1; /* should not happen */
}

int haab_month_count(void)
{
    return HAAB_MONTH_COUNT;
}
