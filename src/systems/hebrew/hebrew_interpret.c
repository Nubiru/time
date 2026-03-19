/* hebrew_interpret.c — Hebrew month interpretation (Sefer Yetzirah tradition)
 *
 * All functions are pure (no globals, no malloc, no side effects).
 * Month data from Sefer Yetzirah / Kabbalistic tradition. */

#include "hebrew_interpret.h"
#include <stdio.h>
#include <string.h>

#define MONTH_COUNT 13

static const hi_month_t MONTHS[MONTH_COUNT] = {
    { 1,  "Hei",    "Judah",    "Speech",
      "Redemption",
      "Month of miracles and liberation -- Passover" },
    { 2,  "Vav",    "Issachar", "Thought",
      "Healing",
      "Month of natural healing -- counting the Omer" },
    { 3,  "Zayin",  "Zebulun",  "Motion",
      "Revelation",
      "Month of Torah giving -- Shavuot" },
    { 4,  "Chet",   "Reuben",   "Sight",
      "Vision through darkness",
      "Month of the broken tablets -- seeing beyond" },
    { 5,  "Tet",    "Simeon",   "Hearing",
      "Transformation of sorrow",
      "Month of destruction and consolation" },
    { 6,  "Yod",    "Gad",      "Action",
      "Repentance",
      "Month of returning -- preparation for the new year" },
    { 7,  "Lamed",  "Ephraim",  "Touch",
      "Awe and judgment",
      "Month of the High Holidays -- Rosh Hashanah, Yom Kippur, Sukkot" },
    { 8,  "Nun",    "Manasseh", "Smell",
      "Quiet depth",
      "The bitter month -- no holidays, only rain prayers" },
    { 9,  "Samekh", "Benjamin", "Sleep",
      "Trust and miracles",
      "Month of dreams and lights -- Hanukkah" },
    { 10, "Ayin",   "Dan",      "Anger (righteous)",
      "Transformation",
      "The darkest month -- righteous indignation" },
    { 11, "Tzadi",  "Asher",    "Taste",
      "Renewal",
      "New Year of Trees -- Tu B'Shevat" },
    { 12, "Qof",    "Naphtali", "Laughter",
      "Joy and reversal",
      "Month of Purim -- hidden miracles revealed" },
    { 13, "Qof",    "Naphtali", "Laughter",
      "Joy doubled",
      "Leap month -- Purim celebrated here in leap years" }
};

hi_month_t hi_month_data(int month)
{
    if (month < 1 || month > MONTH_COUNT) {
        hi_month_t invalid = { -1, "?", "?", "?", "?", "?" };
        return invalid;
    }
    return MONTHS[month - 1];
}

int hi_month_count(void)
{
    return MONTH_COUNT;
}

hebrew_interp_t hi_interpret(hebrew_date_t date, int sabbatical_pos,
                             int is_leap_year)
{
    hebrew_interp_t result;
    memset(&result, 0, sizeof(result));

    hi_month_t m = hi_month_data(date.month);

    /* T0: glyph — the Hebrew letter */
    snprintf(result.glyph, sizeof(result.glyph), "%s", m.hebrew_letter);

    /* Get month name from hebrew.h */
    const char *month_name = hebrew_month_name(date.month,
                                               is_leap_year != 0);

    /* T1: glance — "{year} {month_name} {day} — {quality}" */
    snprintf(result.glance, sizeof(result.glance),
             "%d %s %d -- %s",
             date.year, month_name, date.day, m.quality);

    /* T3: detail — full narrative paragraph */
    int written = snprintf(result.detail, sizeof(result.detail),
             "%s is the month of %s. "
             "Its letter is %s, its tribe is %s, its sense is %s. "
             "%s.",
             month_name, m.quality,
             m.hebrew_letter, m.tribe, m.sense,
             m.brief);

    /* Append sabbatical position if provided */
    if (sabbatical_pos >= 1 && sabbatical_pos <= 7 &&
        written > 0 && (size_t)written < sizeof(result.detail)) {
        snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 " Shemitah year %d of 7.", sabbatical_pos);
    }

    return result;
}
