/* persian_interpret.c — Persian/Solar Hijri calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "persian_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static month data (12 months)
 * ================================================================ */

static const pi_month_t MONTHS[12] = {
    {  1, "Farvardin",   "Fravashi (guardian spirits)",
       "Guardian spirits of the righteous \xe2\x80\x94 the protective ancestors",
       "Early spring \xe2\x80\x94 Nowruz season, new year, rebirth of nature",
       "Month of the guardian spirits \xe2\x80\x94 the ancestors bless the new year's beginning" },
    {  2, "Ordibehesht", "Asha Vahishta (Best Truth)",
       "The Amesha Spenta of truth, righteousness, and holy fire",
       "Mid-spring \xe2\x80\x94 the earth warms, truth becomes visible in green",
       "Month of best truth \xe2\x80\x94 the cosmic order made visible in blossoming nature" },
    {  3, "Khordad",     "Haurvatat (Wholeness)",
       "The Amesha Spenta of water, health, and perfection",
       "Late spring \xe2\x80\x94 waters flowing, fullness of the season",
       "Month of wholeness \xe2\x80\x94 the perfection of the waters that sustain all life" },
    {  4, "Tir",         "Tishtrya (Sirius, the rain star)",
       "The yazata of rain and the star Sirius, bringer of the monsoon",
       "Early summer \xe2\x80\x94 heat begins, rain prayers",
       "Month of Sirius \xe2\x80\x94 the star whose heliacal rising brings the longed-for rain" },
    {  5, "Mordad",      "Ameretat (Immortality)",
       "The Amesha Spenta of plants, immortality, and deathlessness",
       "Midsummer \xe2\x80\x94 heat at peak, plants in full power",
       "Month of immortality \xe2\x80\x94 the deathless force that lives in every green thing" },
    {  6, "Shahrivar",   "Khshathra Vairya (Desirable Dominion)",
       "The Amesha Spenta of metals, sky, and righteous power",
       "Late summer \xe2\x80\x94 harvest approaching, the kingdom of light",
       "Month of desirable dominion \xe2\x80\x94 the just sovereignty that rules the sky" },
    {  7, "Mehr",        "Mithra (Covenant, Light)",
       "The yazata of oath, friendship, and the sun's justice",
       "Early autumn \xe2\x80\x94 equinox, balance of light and dark",
       "Month of Mithra \xe2\x80\x94 the lord of the covenant who watches over every promise" },
    {  8, "Aban",        "Apas (Waters)",
       "The yazata of the cosmic waters and purification",
       "Mid-autumn \xe2\x80\x94 rains return, waters honored",
       "Month of the waters \xe2\x80\x94 the cleansing rains that renew the parched earth" },
    {  9, "Azar",        "Atar (Fire)",
       "The yazata of fire, the sacred flame of Ahura Mazda",
       "Late autumn \xe2\x80\x94 Yalda approaches, fire against the growing dark",
       "Month of fire \xe2\x80\x94 the sacred flame that holds back the longest night" },
    { 10, "Dey",         "Dadar (Creator, Ahura Mazda)",
       "The Creator himself \xe2\x80\x94 three days of this month bear his name",
       "Early winter \xe2\x80\x94 solstice past, the creator's presence in stillness",
       "Month of the Creator \xe2\x80\x94 Ahura Mazda's own month, when light begins its return" },
    { 11, "Bahman",      "Vohu Manah (Good Mind)",
       "The Amesha Spenta of good thought, right thinking, and animals",
       "Midwinter \xe2\x80\x94 contemplation, the discipline of good thought",
       "Month of good mind \xe2\x80\x94 the righteous thought that discerns truth from lie" },
    { 12, "Esfand",      "Spenta Armaiti (Holy Devotion)",
       "The Amesha Spenta of the earth, devotion, and loving faith",
       "Late winter \xe2\x80\x94 Chaharshanbe Suri, fire leaping, spring anticipated",
       "Month of holy devotion \xe2\x80\x94 the sacred earth preparing to bloom once more" }
};

/* ================================================================
 * Static festival data (4 festivals)
 * ================================================================ */

static const pi_festival_t FESTIVALS[4] = {
    { 1, "Nowruz",
      "New Day \xe2\x80\x94 rebirth of the world at the vernal equinox",
      "Haft Sin table with seven symbolic items, visiting family, wearing new clothes, goldfish and sprouted wheat",
      "The oldest continuously celebrated holiday on Earth \xe2\x80\x94 3000+ years of welcoming spring" },

    { 2, "Sizdah Bedar",
      "Thirteenth Outside \xe2\x80\x94 spending the 13th day of the new year in nature",
      "Picnicking outdoors, tying grass knots for wishes, releasing the sprouted sabzeh into running water",
      "Nature Day \xe2\x80\x94 leaving bad luck behind by spending the day under the open sky" },

    { 3, "Yalda",
      "Birth of light \xe2\x80\x94 the longest night of the year conquered by togetherness",
      "Staying awake all night with family, eating pomegranates and watermelon, reading Hafez poetry (fal-e Hafez)",
      "The night of Yalda \xe2\x80\x94 red fruits against the darkness, poetry against silence" },

    { 4, "Chaharshanbe Suri",
      "Red Wednesday \xe2\x80\x94 fire festival, purification before the new year",
      "Jumping over bonfires chanting 'give me your red, take my yellow' (health for sickness), fireworks, spoon-banging",
      "The fire festival \xe2\x80\x94 leaping over flame to burn away the old year's sorrow" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const pi_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?", "?"
};

static const pi_festival_t INVALID_FESTIVAL = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

pi_month_t pi_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

pi_festival_t pi_festival_data(int festival)
{
    if (festival < 1 || festival > 4) return INVALID_FESTIVAL;
    return FESTIVALS[festival - 1];
}

persian_interp_t pi_interpret(int month, int day, int festival)
{
    persian_interp_t r;
    memset(&r, 0, sizeof(r));

    pi_month_t m = pi_month_data(month);

    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of month name */
    size_t len = strlen(m.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (festival >= 1 && festival <= 4) {
        pi_festival_t f = pi_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s",
                 m.name, day, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s",
                 m.name, day, m.meaning);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Month: %s (Avestan: %s). %s. %s.",
                    m.name, m.avestan, m.meaning, m.season);

    if (festival >= 1 && festival <= 4) {
        pi_festival_t f = pi_festival_data(festival);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Festival: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int pi_month_count(void)
{
    return 12;
}

int pi_festival_count(void)
{
    return 4;
}
