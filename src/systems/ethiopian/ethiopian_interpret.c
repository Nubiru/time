/* ethiopian_interpret.c — Ethiopian calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "ethiopian_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static month data (13 months)
 * ================================================================ */

static const eti_month_t MONTHS[13] = {
    {  1, "Meskerem",  "New Year, beginning",
       "End of rainy season (Kiremt), flowers bloom across the highlands",
       "First month \xe2\x80\x94 the rains recede and Ethiopia carpets itself in yellow daisies" },
    {  2, "Tikimt",    "Planting, sowing",
       "Post-rain planting season, the land is rich and ready",
       "Second month \xe2\x80\x94 the fertile earth receives its seeds after the great rains" },
    {  3, "Hidar",     "Gathering, darkening",
       "Short rains possible, days shorten, harvest preparation",
       "Third month \xe2\x80\x94 the gathering in, as autumn deepens on the highland plateau" },
    {  4, "Tahsas",    "Joy, celebration",
       "Dry season begins, Christmas (Genna) preparations",
       "Fourth month \xe2\x80\x94 the month of joy that builds toward Genna" },
    {  5, "Tir",       "Sowing seed, the sun",
       "Peak dry season, Timkat (Epiphany) celebrations",
       "Fifth month \xe2\x80\x94 the sun blazes and Timkat processions fill the streets with color" },
    {  6, "Yekatit",   "Germination",
       "Dry season, seeds germinating from earlier planting",
       "Sixth month \xe2\x80\x94 what was planted begins to stir beneath the dry earth" },
    {  7, "Megabit",   "Harvest approach",
       "Small rains (Belg) may begin in some regions",
       "Seventh month \xe2\x80\x94 the small rains arrive, the first green returns" },
    {  8, "Miyazia",   "The blessing",
       "Belg rains, Easter season, the earth greens",
       "Eighth month \xe2\x80\x94 the blessing of rain and the joy of Fasika (Easter)" },
    {  9, "Ginbot",    "Strong growth",
       "Main rains approaching, strong vegetative growth",
       "Ninth month \xe2\x80\x94 everything grows with force as the big rains draw near" },
    { 10, "Sene",      "The great rain",
       "Kiremt (main rainy season) begins, heavy monsoon",
       "Tenth month \xe2\x80\x94 the great rains arrive, rivers swell, the world goes green" },
    { 11, "Hamle",     "Peak rain, flooding",
       "Heaviest rains, some flooding, lush green highlands",
       "Eleventh month \xe2\x80\x94 the rains at their most fierce, the highlands draped in mist" },
    { 12, "Nehase",    "End of rain, drying",
       "Rains begin to taper, preparing for the new year",
       "Twelfth month \xe2\x80\x94 the rains slow their drumming, Meskerem approaches" },
    { 13, "Pagume",    "Epagomenal days, the little month",
       "Five or six extra days between years, sacred transition",
       "The little month \xe2\x80\x94 the days outside time, when the old year surrenders to the new" }
};

/* ================================================================
 * Static feast data (4 fixed feasts)
 * ================================================================ */

static const eti_feast_t FEASTS[4] = {
    { 1, "Enkutatash",
      "New Year \xe2\x80\x94 gift of jewels, the return of the Queen of Sheba",
      "Yellow daisies (adey abeba), bonfires (chibo), singing door to door, new clothes, feasting",
      "Ethiopian New Year \xe2\x80\x94 1 Meskerem, when the land blooms gold and children sing for blessings" },

    { 2, "Meskel",
      "Finding of the True Cross \xe2\x80\x94 light triumphant over darkness",
      "Demera bonfire ceremony, yellow daisies laid in cross pattern, processions with priests and drums",
      "The Cross revealed \xe2\x80\x94 17 Meskerem, when the great bonfire falls toward the buried relic" },

    { 3, "Genna",
      "Christmas \xe2\x80\x94 the Nativity, preceded by 43 days of fasting",
      "All-night church service, breaking the fast, genna (field hockey), white shammas, injera feast",
      "Ethiopian Christmas \xe2\x80\x94 29 Tahsas, the dawn service after the longest fast" },

    { 4, "Timkat",
      "Epiphany \xe2\x80\x94 the Baptism of Christ, the holiest celebration",
      "Three-day festival, tabot (Ark replica) processions, blessing of waters, joyful crowds in white",
      "Timkat \xe2\x80\x94 11 Tir, when the tabots emerge from the churches and the waters are blessed" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const eti_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?"
};

static const eti_feast_t INVALID_FEAST = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

eti_month_t eti_month_data(int month)
{
    if (month < 1 || month > 13) return INVALID_MONTH;
    return MONTHS[month - 1];
}

eti_feast_t eti_feast_data(int feast)
{
    if (feast < 1 || feast > 4) return INVALID_FEAST;
    return FEASTS[feast - 1];
}

ethiopian_interp_t eti_interpret(int month, int day, int feast)
{
    ethiopian_interp_t r;
    memset(&r, 0, sizeof(r));

    eti_month_t m = eti_month_data(month);

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
    if (feast >= 1 && feast <= 4) {
        eti_feast_t f = eti_feast_data(feast);
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
                    "Month: %s. Meaning: %s. %s.",
                    m.name, m.meaning, m.season);

    if (feast >= 1 && feast <= 4) {
        eti_feast_t f = eti_feast_data(feast);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Feast: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int eti_month_count(void)
{
    return 13;
}

int eti_feast_count(void)
{
    return 4;
}
