/* tamil_interpret.c — Tamil calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "tamil_interpret.h"
#include <stdio.h>
#include <string.h>

static const tmi_month_t MONTHS[12] = {
    {  1, "Chittirai",  "Mesha (Aries)",       "Hot season, Tamil New Year",
       "First month \xe2\x80\x94 the Sun enters sidereal Aries and the Tamil year begins" },
    {  2, "Vaikasi",    "Rishabha (Taurus)",    "Peak heat, temple festivals",
       "Month of temple celebrations \xe2\x80\x94 the bull rasi, steadfast under the blazing sun" },
    {  3, "Aani",       "Mithuna (Gemini)",     "Onset of southwest monsoon",
       "The twins month \xe2\x80\x94 the monsoon arrives and the parched earth drinks" },
    {  4, "Aadi",       "Kataka (Cancer)",      "Monsoon, auspicious month",
       "Month of Aadi \xe2\x80\x94 the crab rasi, monsoon fullness, Aadi Perukku (river festival)" },
    {  5, "Avani",      "Simha (Leo)",          "Late monsoon, harvest preparations",
       "Month of the lion \xe2\x80\x94 Avani Avittam (sacred thread renewal) and monsoon bounty" },
    {  6, "Purattasi",  "Kanya (Virgo)",        "Post-monsoon, Navaratri season",
       "Month of Navaratri \xe2\x80\x94 nine nights of the goddess, the maiden rasi" },
    {  7, "Aippasi",    "Thula (Libra)",        "Northeast monsoon begins",
       "Month of the scales \xe2\x80\x94 Deepavali season, the balance between light and dark" },
    {  8, "Karthigai",  "Vrischika (Scorpio)",  "Festival of lights, cool season begins",
       "Month of Karthigai \xe2\x80\x94 the lamp festival, the Pleiades at zenith" },
    {  9, "Margazhi",   "Dhanus (Sagittarius)", "Cool season, dawn devotions",
       "Holiest month \xe2\x80\x94 Margazhi kolams (rice flour art) at dawn, the archer rasi" },
    { 10, "Thai",       "Makara (Capricorn)",   "Pongal harvest festival",
       "Month of Pongal \xe2\x80\x94 the harvest thanksgiving, the Sun's northward journey begins" },
    { 11, "Maasi",      "Kumbha (Aquarius)",    "Late cool season",
       "Month of the water-bearer \xe2\x80\x94 Maha Shivaratri, the great night of Shiva" },
    { 12, "Panguni",    "Meena (Pisces)",       "Pre-summer, temple marriages",
       "Last month \xe2\x80\x94 Panguni Uthiram (divine weddings), the fish rasi closes the year" }
};

static const tmi_festival_t FESTIVALS[4] = {
    { 1, "Puthandu",
      "New Year \xe2\x80\x94 the sidereal Sun enters Aries, auspicious beginnings",
      "Viewing the kanni (auspicious tray), mango pachadi (sweet-sour-bitter dish symbolizing life's tastes), visiting temples",
      "Tamil New Year \xe2\x80\x94 1 Chittirai, when the sidereal Sun crosses into Mesha" },

    { 2, "Pongal",
      "Harvest thanksgiving \xe2\x80\x94 gratitude to the Sun, cows, and the earth",
      "Boiling rice in new pots until it overflows (pongal = overflowing), decorating cows, kolam art, bonfires",
      "The harvest festival \xe2\x80\x94 four days in Thai month when abundance is celebrated with overflowing pots" },

    { 3, "Chithirai Festival",
      "Divine marriage \xe2\x80\x94 Meenakshi and Sundareswarar's cosmic wedding",
      "Ten-day festival in Madurai, chariot procession, re-enactment of the divine marriage",
      "The celestial wedding \xe2\x80\x94 Madurai's great festival celebrating the goddess who married Shiva" },

    { 4, "Deepavali",
      "Victory of light \xe2\x80\x94 triumph over darkness, the return of righteousness",
      "Oil lamps, firecrackers, new clothes, sweets, oil bath at dawn, family gatherings",
      "The festival of lights \xe2\x80\x94 every lamp declares that darkness cannot endure" }
};

static const tmi_month_t INVALID_MONTH = { -1, "?", "?", "?", "?" };
static const tmi_festival_t INVALID_FESTIVAL = { -1, "?", "?", "?", "?" };

tmi_month_t tmi_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

tmi_festival_t tmi_festival_data(int festival)
{
    if (festival < 1 || festival > 4) return INVALID_FESTIVAL;
    return FESTIVALS[festival - 1];
}

tamil_interp_t tmi_interpret(int month, int day, int jovian_year,
                             int festival)
{
    tamil_interp_t r;
    memset(&r, 0, sizeof(r));

    tmi_month_t m = tmi_month_data(month);
    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    size_t len = strlen(m.tamil);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.tamil, copy);
    r.glyph[copy] = '\0';

    if (festival >= 1 && festival <= 4) {
        tmi_festival_t f = tmi_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s",
                 m.tamil, day, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d (%s)",
                 m.tamil, day, m.rasi);
    }

    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Month: %s (Rasi: %s). %s. Jovian year %d of 60.",
                    m.tamil, m.rasi, m.season, jovian_year);

    if (festival >= 1 && festival <= 4) {
        tmi_festival_t f = tmi_festival_data(festival);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Festival: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int tmi_month_count(void) { return 12; }
int tmi_festival_count(void) { return 4; }
