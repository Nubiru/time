/* thai_interpret.c — Thai calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "thai_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static month data (12 months)
 * ================================================================ */

static const ti_thai_month_t MONTHS[12] = {
    {  1, "Mokarakhom",       "Makara (Capricorn)",
       "Cool season peak \xe2\x80\x94 the sea-goat month, harvest of cool-season crops",
       "January \xe2\x80\x94 named from Capricorn, the cool season reaches its depth" },
    {  2, "Kumphaphan",       "Kumbha (Aquarius)",
       "Cool season fading \xe2\x80\x94 the water-bearer month, Chinese New Year festivities",
       "February \xe2\x80\x94 named from Aquarius, the last cool breath before heat" },
    {  3, "Minakhom",         "Mina (Pisces)",
       "Hot season begins \xe2\x80\x94 the fish month, dry heat building",
       "March \xe2\x80\x94 named from Pisces, the transition from cool to scorching" },
    {  4, "Mesayon",          "Mesha (Aries)",
       "Hot season peak \xe2\x80\x94 the ram month, Songkran water festival",
       "April \xe2\x80\x94 named from Aries, the hottest month cooled by Songkran's water" },
    {  5, "Phruetsaphakhom",  "Vrishabha (Taurus)",
       "Hot season end \xe2\x80\x94 the bull month, monsoon anticipation",
       "May \xe2\x80\x94 named from Taurus, the earth waits for rain" },
    {  6, "Mithunayon",       "Mithuna (Gemini)",
       "Rainy season begins \xe2\x80\x94 the twins month, first monsoon rains",
       "June \xe2\x80\x94 named from Gemini, the monsoon announces itself" },
    {  7, "Karakadakhom",     "Karkata (Cancer)",
       "Rainy season \xe2\x80\x94 the crab month, Buddhist Lent (Khao Phansa) begins",
       "July \xe2\x80\x94 named from Cancer, monks enter the rains retreat" },
    {  8, "Singhakhom",       "Simha (Leo)",
       "Rainy season peak \xe2\x80\x94 the lion month, Mother's Day (Queen's birthday)",
       "August \xe2\x80\x94 named from Leo, the rains intensify" },
    {  9, "Kanyayon",         "Kanya (Virgo)",
       "Late rainy season \xe2\x80\x94 the maiden month, rice fields green",
       "September \xe2\x80\x94 named from Virgo, the rice paddies glow emerald" },
    { 10, "Tulakhom",         "Tula (Libra)",
       "Rainy season ending \xe2\x80\x94 the scales month, Lent ends (Ok Phansa)",
       "October \xe2\x80\x94 named from Libra, balance returns as the rains recede" },
    { 11, "Phruetsachikayon", "Vrischika (Scorpio)",
       "Cool season begins \xe2\x80\x94 the scorpion month, Loy Krathong festival",
       "November \xe2\x80\x94 named from Scorpio, floating lanterns honor the river goddess" },
    { 12, "Thanwakhom",       "Dhanus (Sagittarius)",
       "Cool season \xe2\x80\x94 the archer month, Father's Day (King's birthday)",
       "December \xe2\x80\x94 named from Sagittarius, the cool season's gift" }
};

/* ================================================================
 * Static festival data (3 festivals)
 * ================================================================ */

static const ti_thai_festival_t FESTIVALS[3] = {
    { 1, "Songkran",
      "Water, renewal, respect \xe2\x80\x94 the Thai New Year cleansing",
      "Pouring water over Buddha images and elders' hands, water fights in the streets, building sand stupas at temples",
      "The water festival \xe2\x80\x94 three days when Thailand washes away the old year with joy and reverence" },

    { 2, "Chakri Day",
      "Dynasty, gratitude \xe2\x80\x94 honoring the founding of the Chakri royal house",
      "Ceremony at the Grand Palace, paying respects to past kings, national holiday",
      "Chakri Memorial Day \xe2\x80\x94 honoring King Rama I who founded Bangkok and the dynasty" },

    { 3, "Constitution Day",
      "Democracy, nationhood \xe2\x80\x94 commemorating Thailand's first constitution",
      "National ceremonies, public holiday, reflection on democratic principles",
      "Constitution Day \xe2\x80\x94 remembering the transition from absolute to constitutional monarchy" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const ti_thai_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?"
};

static const ti_thai_festival_t INVALID_FESTIVAL = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ti_thai_month_t tti_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

ti_thai_festival_t tti_festival_data(int festival)
{
    if (festival < 1 || festival > 3) return INVALID_FESTIVAL;
    return FESTIVALS[festival - 1];
}

thai_interp_t tti_interpret(int be_year, int month, int day, int festival)
{
    thai_interp_t r;
    memset(&r, 0, sizeof(r));

    ti_thai_month_t m = tti_month_data(month);

    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of Thai month name */
    size_t len = strlen(m.thai);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.thai, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (festival >= 1 && festival <= 3) {
        ti_thai_festival_t f = tti_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "BE %d %s %d \xe2\x80\x94 %s",
                 be_year, m.thai, day, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "BE %d %s %d",
                 be_year, m.thai, day);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "BE %d. Month: %s (from Sanskrit %s). %s.",
                    be_year, m.thai, m.origin, m.season);

    if (festival >= 1 && festival <= 3) {
        ti_thai_festival_t f = tti_festival_data(festival);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Festival: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int tti_month_count(void)
{
    return 12;
}

int tti_festival_count(void)
{
    return 3;
}
