#include "tzolkin.h"
#include "../../math/julian.h"
#include <math.h>

/* Dreamspell reference: "dreamspell day" value for a known Kin 1 date.
 * Derived from anchor: 2012-12-21 = Kin 207 (Blue Crystal Hand).
 * Kin 1 falls on 2013-02-13 (day_num 2456337, feb29_count 488).
 * dreamspell_day = 2456337 - 488 = 2455849. */
#define DREAMSPELL_REF 2455849L

static const char *SEAL_NAMES[] = {
    "Red Dragon",       "White Wind",        "Blue Night",       "Yellow Seed",
    "Red Serpent",       "White Worldbridger","Blue Hand",        "Yellow Star",
    "Red Moon",          "White Dog",         "Blue Monkey",      "Yellow Human",
    "Red Skywalker",     "White Wizard",      "Blue Eagle",       "Yellow Warrior",
    "Red Earth",         "White Mirror",      "Blue Storm",       "Yellow Sun"
};

static const char *TONE_NAMES[] = {
    "Magnetic",  "Lunar",     "Electric",  "Self-Existing",
    "Overtone",  "Rhythmic",  "Resonant",  "Galactic",
    "Solar",     "Planetary", "Spectral",  "Crystal",
    "Cosmic"
};

static int is_leap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

/* Count Feb 29s from year 1 through date (y, m, d).
 * Valid for years >= 1 (proleptic Gregorian). */
static int feb29_count(int y, int m, int d) {
    int count = (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;
    if (is_leap(y) && (m > 2 || (m == 2 && d >= 29)))
        count++;
    return count;
}

tzolkin_day_t tzolkin_from_jd(double jd) {
    tzolkin_day_t result;
    long day_num = (long)floor(jd + 0.5);

    int year, month;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)day_frac;

    /* Dreamspell: Feb 29 ("0.0 Hunab Ku") gets same kin as Feb 28. */
    if (month == 2 && day == 29) {
        day_num--;
        day = 28;
    }

    long dreamspell_day = day_num - (long)feb29_count(year, month, day);
    long offset = dreamspell_day - DREAMSPELL_REF;
    int kin = (int)(((offset % 260) + 260) % 260) + 1;

    result.kin  = kin;
    result.seal = (kin - 1) % 20;
    result.tone = (kin - 1) % 13 + 1;
    return result;
}

const char *tzolkin_seal_name(int seal) {
    if (seal < 0 || seal > 19) return "?";
    return SEAL_NAMES[seal];
}

const char *tzolkin_tone_name(int tone) {
    if (tone < 1 || tone > 13) return "?";
    return TONE_NAMES[tone - 1];
}
