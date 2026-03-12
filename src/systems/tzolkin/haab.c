#include "haab.h"
#include "tzolkin.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/*
 * Haab: 18 months of 20 days + 5 Wayeb days = 365 days.
 * Using GMT correlation 584283.
 *
 * The Haab epoch anchor: Long Count 0.0.0.0.0 = JD 584283.
 * On that date, the Haab was 8 Kumk'u (month 17, day 8).
 *
 * From any JD, compute day offset from correlation, then find Haab position.
 */

#define GMT_CORRELATION  584283L
#define HAAB_EPOCH_OFFSET 348  /* 17 * 20 + 8 = day-of-year for 8 Kumk'u */

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
    long day_count = (long)floor(jd + 0.5) - GMT_CORRELATION;
    int position = (int)(((day_count % 365) + HAAB_EPOCH_OFFSET + 365 * 2) % 365);

    if (position < 360) {
        h.month = position / 20;
        h.day = position % 20;
    } else {
        h.month = 18; /* Wayeb */
        h.day = position - 360;
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

int haab_month_length(int month)
{
    if (month < 0 || month > 18) return 0;
    if (month == 18) return 5;
    return 20;
}

int haab_month_count(void)
{
    return HAAB_MONTH_COUNT;
}

int haab_day_of_year(haab_date_t h)
{
    if (h.month < 18) {
        return h.month * 20 + h.day;
    }
    return 360 + h.day;
}

void haab_fmt(haab_date_t h, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "%d %s", h.day, haab_month_name(h.month));
}

calendar_round_t calendar_round_from_jd(double jd)
{
    calendar_round_t cr;
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    haab_date_t h = haab_from_jd(jd);

    cr.kin = tz.kin;
    cr.tone = tz.tone;
    cr.seal = tz.seal;
    cr.haab_month = h.month;
    cr.haab_day = h.day;
    return cr;
}

void calendar_round_fmt(calendar_round_t cr, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *seal_name = tzolkin_seal_name(cr.seal);
    const char *month_name = haab_month_name(cr.haab_month);
    snprintf(buf, sz, "%d %s %d %s",
             cr.tone, seal_name, cr.haab_day, month_name);
}

int calendar_round_cycle(void)
{
    /* LCM(260, 365) = 18980 */
    return 18980;
}
