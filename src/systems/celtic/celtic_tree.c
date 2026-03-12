/* celtic_tree.c — Celtic Tree Calendar (Robert Graves system)
 * 13 months x 28 days + 1 intercalary day = 365 days.
 * Year starts Dec 24 (Birch) and ends Dec 23 (Yew intercalary day).
 *
 * Sources: Robert Graves, "The White Goddess" (1948);
 * Ogham alphabet from the Book of Ballymote (1391). */

#include "celtic_tree.h"
#include <math.h>

/* ===== Static data ===== */

/* Tree month data table. Index 0 = Yew (intercalary), 1-13 = months.
 * start_day_of_year is the Gregorian day-of-year (non-leap) when
 * the month begins. */
static const celtic_tree_info_t TREE_DATA[14] = {
    { 0,  "Yew",      "Idho",   "I",  "Immortality, rebirth, passage between worlds",          357 },
    { 1,  "Birch",    "Beth",   "B",  "New beginnings, purification, rebirth",                  358 },
    { 2,  "Rowan",    "Luis",   "L",  "Protection, vision, psychic power",                       21 },
    { 3,  "Ash",      "Nion",   "N",  "World tree, connection, roots and sky",                   49 },
    { 4,  "Alder",    "Fearn",  "F",  "Foundation, endurance, shield",                           77 },
    { 5,  "Willow",   "Saille", "S",  "Moon, intuition, emotion, healing",                     105 },
    { 6,  "Hawthorn", "Huath",  "H",  "Cleansing, sacred space, fairy realm",                  133 },
    { 7,  "Oak",      "Duir",   "D",  "Strength, sovereignty, thunder god",                    161 },
    { 8,  "Holly",    "Tinne",  "T",  "Defense, challenge, warrior spirit",                    189 },
    { 9,  "Hazel",    "Coll",   "C",  "Wisdom, inspiration, sacred well",                      217 },
    { 10, "Vine",     "Muin",   "M",  "Harvest, prophecy, joy",                                245 },
    { 11, "Ivy",      "Gort",   "G",  "Persistence, spiral growth, search within",             273 },
    { 12, "Reed",     "Ngetal", "Ng", "Harmony, direction, threshold",                         301 },
    { 13, "Elder",    "Ruis",   "R",  "Transformation, death and rebirth, completion",          329 }
};

/* Wheel of the Year festival -> tree month mapping.
 * Index: WHEEL_SAMHAIN=0 through WHEEL_MABON=7. */
static const int FESTIVAL_TO_MONTH[8] = {
    12,  /* WHEEL_SAMHAIN (Oct 31/Nov 1) -> Reed (Oct 28 - Nov 24) */
    13,  /* WHEEL_YULE (Dec 21-22) -> Elder (Nov 25 - Dec 22) */
     2,  /* WHEEL_IMBOLC (Feb 1-2) -> Rowan (Jan 21 - Feb 17) */
     4,  /* WHEEL_OSTARA (Mar 20-21) -> Alder (Mar 18 - Apr 14) */
     5,  /* WHEEL_BELTANE (May 1) -> Willow (Apr 15 - May 12) */
     7,  /* WHEEL_LITHA (Jun 20-21) -> Oak (Jun 10 - Jul 7) */
     8,  /* WHEEL_LUGHNASADH (Aug 1) -> Holly (Jul 8 - Aug 4) */
    10   /* WHEEL_MABON (Sep 22-23) -> Vine (Sep 2 - Sep 29) */
};

/* Invalid sentinel for tree info queries */
static const celtic_tree_info_t INVALID_INFO = {
    -1, "?", "?", "?", "?", -1
};

/* ===== JD <-> Gregorian helpers (self-contained, static) ===== */

/* Convert JD to Gregorian year/month/day.
 * Standard algorithm (Richards / Meeus). */
static void jd_to_gregorian(double jd, int *year, int *month, int *day)
{
    int a = (int)floor(jd + 0.5) + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - (146097 * b) / 4;
    int d = (4 * c + 3) / 1461;
    int e = c - (1461 * d) / 4;
    int m = (5 * e + 2) / 153;

    *day   = e - (153 * m + 2) / 5 + 1;
    *month = m + 3 - 12 * (m / 10);
    *year  = 100 * b + d - 4800 + (m / 10);
}

/* Convert Gregorian year/month/day to JD (at midnight, 0h UT).
 * Standard algorithm. */
static double gregorian_to_jd(int year, int month, int day)
{
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    int jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    return (double)jdn - 0.5;
}

/* ===== Implementation ===== */

static bool valid_month(int month)
{
    return month >= 0 && month <= 13;
}

celtic_tree_date_t celtic_tree_from_jd(double jd)
{
    celtic_tree_date_t result = { .month = 0, .day = 0 };

    int year, month, day;
    jd_to_gregorian(jd, &year, &month, &day);

    /* Determine which Celtic year this date belongs to.
     * Celtic year starts Dec 24. If we're on or after Dec 24,
     * this is the start of a new Celtic year. Otherwise,
     * we're in the Celtic year that started Dec 24 of the previous year. */
    int celtic_start_year;
    if (month == 12 && day >= 24) {
        celtic_start_year = year;
    } else {
        celtic_start_year = year - 1;
    }

    /* Compute days since Dec 24 of celtic_start_year */
    double jd_start = gregorian_to_jd(celtic_start_year, 12, 24);
    int celtic_doy = (int)floor(jd - jd_start) + 1;

    /* celtic_doy should be 1-365 */
    if (celtic_doy == 365) {
        /* Intercalary Yew day (Dec 23) */
        result.month = 0;
        result.day = 1;
    } else if (celtic_doy >= 1 && celtic_doy <= 364) {
        result.month = (celtic_doy - 1) / 28 + 1;
        result.day = (celtic_doy - 1) % 28 + 1;
    }

    return result;
}

double celtic_tree_to_jd(celtic_tree_date_t date, int gregorian_year)
{
    /* gregorian_year is the year containing the Jan-Dec portion.
     * The Celtic year starting Dec 24 of (gregorian_year - 1)
     * runs through Dec 23 of gregorian_year. */
    int celtic_start_year = gregorian_year - 1;

    /* Compute celtic day of year */
    int celtic_doy;
    if (date.month == 0) {
        celtic_doy = 365;
    } else {
        celtic_doy = (date.month - 1) * 28 + date.day;
    }

    double jd_start = gregorian_to_jd(celtic_start_year, 12, 24);
    return jd_start + (double)(celtic_doy - 1);
}

celtic_tree_info_t celtic_tree_info(int month)
{
    if (!valid_month(month)) {
        return INVALID_INFO;
    }
    return TREE_DATA[month];
}

const char *celtic_tree_name(int month)
{
    if (!valid_month(month)) {
        return "?";
    }
    return TREE_DATA[month].tree;
}

const char *celtic_tree_ogham(int month)
{
    if (!valid_month(month)) {
        return "?";
    }
    return TREE_DATA[month].ogham;
}

const char *celtic_tree_letter(int month)
{
    if (!valid_month(month)) {
        return "?";
    }
    return TREE_DATA[month].letter;
}

const char *celtic_tree_meaning(int month)
{
    if (!valid_month(month)) {
        return "?";
    }
    return TREE_DATA[month].meaning;
}

bool celtic_tree_is_intercalary(celtic_tree_date_t date)
{
    return date.month == 0;
}

int celtic_tree_day_of_year(celtic_tree_date_t date)
{
    if (date.month == 0) {
        return 365;
    }
    return (date.month - 1) * 28 + date.day;
}

int celtic_tree_month_days(int month)
{
    if (month == 0) {
        return 1;
    }
    if (month >= 1 && month <= 13) {
        return 28;
    }
    return -1;
}

int celtic_tree_for_festival(int wheel_festival_index)
{
    if (wheel_festival_index < 0 || wheel_festival_index >= 8) {
        return -1;
    }
    return FESTIVAL_TO_MONTH[wheel_festival_index];
}
