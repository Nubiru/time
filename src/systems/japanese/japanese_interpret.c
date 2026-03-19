/* japanese_interpret.c — Japanese calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "japanese_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static era data (5 modern eras)
 * ================================================================ */

static const ji_era_t ERAS[5] = {
    { 0, "Meiji", "\xe6\x98\x8e\xe6\xb2\xbb",
      "Enlightened Rule",
      "Rapid modernization, opening to the world, imperial restoration",
      "The era of enlightened rule \xe2\x80\x94 Japan emerged from isolation into the modern age" },

    { 1, "Taisho", "\xe5\xa4\xa7\xe6\xad\xa3",
      "Great Righteousness",
      "Democratic movement, cultural liberalism, the brief flowering",
      "The era of great righteousness \xe2\x80\x94 a short season of democracy and artistic freedom" },

    { 2, "Showa", "\xe6\x98\xad\xe5\x92\x8c",
      "Enlightened Peace / Radiant Japan",
      "War, defeat, rebirth, economic miracle, longest reign",
      "The era of radiant peace \xe2\x80\x94 from devastation to the world's second economy" },

    { 3, "Heisei", "\xe5\xb9\xb3\xe6\x88\x90",
      "Achieving Peace",
      "The lost decades, natural disasters, quiet resilience",
      "The era of achieving peace \xe2\x80\x94 prosperity tempered by earthquakes and introspection" },

    { 4, "Reiwa", "\xe4\xbb\xa4\xe5\x92\x8c",
      "Beautiful Harmony",
      "New era, connection to Man'yoshu poetry, hope and harmony",
      "The era of beautiful harmony \xe2\x80\x94 named from the oldest Japanese poetry anthology" }
};

/* ================================================================
 * Static rokuyo data (6-day cycle)
 * ================================================================ */

static const ji_rokuyo_t ROKUYO[6] = {
    { 0, "Sensho", "Win First",
      "Morning is auspicious, afternoon unlucky \xe2\x80\x94 act early, avoid delay",
      "Win first \xe2\x80\x94 seize the morning, the early hours favor bold action" },

    { 1, "Tomobiki", "Draw Friend",
      "Midday is unlucky, morning and evening good \xe2\x80\x94 avoid funerals (the dead draw friends)",
      "Draw friend \xe2\x80\x94 what happens to you today may happen to those close to you" },

    { 2, "Senbu", "Lose First",
      "Morning is unlucky, afternoon improves \xe2\x80\x94 patience is rewarded, wait for the turn",
      "Lose first \xe2\x80\x94 the day begins poorly but reverses; persistence wins" },

    { 3, "Butsumetsu", "Buddha's Death",
      "The most inauspicious day \xe2\x80\x94 avoid celebrations, weddings, new ventures",
      "Buddha's death \xe2\x80\x94 the most unlucky day, yet some see humility as auspicious for weddings" },

    { 4, "Taian", "Great Peace",
      "The most auspicious day \xe2\x80\x94 ideal for weddings, openings, and all beginnings",
      "Great peace \xe2\x80\x94 the luckiest day, when everything begun has the best chance of flourishing" },

    { 5, "Shakko", "Red Mouth",
      "Midday is briefly auspicious, rest of day unlucky \xe2\x80\x94 guard your words, avoid arguments",
      "Red mouth \xe2\x80\x94 words can wound today; only the noon hour offers safe passage" }
};

/* ================================================================
 * Static sekki data (24 solar terms)
 * ================================================================ */

static const ji_sekki_t SEKKI[24] = {
    {  0, "Shunbun",   "\xe6\x98\xa5\xe5\x88\x86",   "Vernal Equinox",
       "Day and night equal, cherry preparations begin",
       "The spring division \xe2\x80\x94 light and dark in perfect balance, life stirs" },
    {  1, "Seimei",    "\xe6\xb8\x85\xe6\x98\x8e",   "Clear and Bright",
       "Skies clear, cherry blossoms peak, ancestral tombs swept",
       "Pure brightness \xe2\x80\x94 the world washed clean, hanami season" },
    {  2, "Kokuu",     "\xe7\xa9\x80\xe9\x9b\xa8",   "Grain Rain",
       "Spring rain nourishes planted fields, last frost passes",
       "Grain rain \xe2\x80\x94 the rain that feeds the rice seedlings" },
    {  3, "Rikka",     "\xe7\xab\x8b\xe5\xa4\x8f",   "Start of Summer",
       "Green deepens, frogs begin singing, warmth settles in",
       "Summer stands \xe2\x80\x94 the season of growth declares itself" },
    {  4, "Shoman",    "\xe5\xb0\x8f\xe6\xba\x80",   "Grain Fills",
       "Grain heads begin to fill, barley ripens",
       "Small fullness \xe2\x80\x94 the grain swells but is not yet ripe" },
    {  5, "Boshu",     "\xe8\x8a\x92\xe7\xa8\xae",   "Grain in Ear",
       "Rice planting season, praying mantises hatch",
       "Bearded grain \xe2\x80\x94 the ears of grain appear, planting must be done" },
    {  6, "Geshi",     "\xe5\xa4\x8f\xe8\x87\xb3",   "Summer Solstice",
       "Longest day, peak sunlight, rainy season (tsuyu) begins",
       "Summer arrives \xe2\x80\x94 the longest day, but the monsoon brings its own darkness" },
    {  7, "Shousho",   "\xe5\xb0\x8f\xe6\x9a\x91",   "Minor Heat",
       "Heat builds, hot winds blow, first lotus flowers open",
       "Small heat \xe2\x80\x94 the warmth intensifies, lotus ponds bloom" },
    {  8, "Taisho",    "\xe5\xa4\xa7\xe6\x9a\x91",   "Major Heat",
       "Hottest period, thunderstorms, Obon preparations",
       "Great heat \xe2\x80\x94 the peak of summer's furnace" },
    {  9, "Risshu",    "\xe7\xab\x8b\xe7\xa7\x8b",   "Start of Autumn",
       "First cool breezes, higurashi cicadas sing at dusk",
       "Autumn stands \xe2\x80\x94 invisible at first, but the light begins to shift" },
    { 10, "Shosho",    "\xe5\x87\xa6\xe6\x9a\x91",   "End of Heat",
       "Heat breaks, typhoon season, cotton flowers bloom",
       "Heat departs \xe2\x80\x94 the furnace door closes, the air remembers coolness" },
    { 11, "Hakuro",    "\xe7\x99\xbd\xe9\x9c\xb2",   "White Dew",
       "Morning dew on grass, geese begin to arrive, autumn deepens",
       "White dew \xe2\x80\x94 moisture condenses on cool mornings, the world turns" },
    { 12, "Shubun",    "\xe7\xa7\x8b\xe5\x88\x86",   "Autumnal Equinox",
       "Day and night equal again, higan (equinox festival), ancestors honored",
       "The autumn division \xe2\x80\x94 balance returns, the other shore is near" },
    { 13, "Kanro",     "\xe5\xaf\x92\xe9\x9c\xb2",   "Cold Dew",
       "Dew chills, chrysanthemums bloom, geese have arrived",
       "Cold dew \xe2\x80\x94 the dew bites, chrysanthemums open" },
    { 14, "Soko",      "\xe9\x9c\x9c\xe9\x99\x8d",   "Frost Descends",
       "First frost, maple leaves turn, harvest ends",
       "Frost falls \xe2\x80\x94 the first ice crystals, the last red leaves" },
    { 15, "Ritto",     "\xe7\xab\x8b\xe5\x86\xac",   "Start of Winter",
       "First signs of winter, camellias bloom, tea season begins",
       "Winter stands \xe2\x80\x94 the season of rest announces itself quietly" },
    { 16, "Shosetsu",  "\xe5\xb0\x8f\xe9\x9b\xaa",   "Minor Snow",
       "Light snow on high ground, citrus ripens, rainbows fade",
       "Small snow \xe2\x80\x94 the first flakes, not yet settling" },
    { 17, "Taisetsu",  "\xe5\xa4\xa7\xe9\x9b\xaa",   "Major Snow",
       "Heavy snow, bears hibernate, salmon run upstream",
       "Great snow \xe2\x80\x94 the world goes white and quiet" },
    { 18, "Toji",      "\xe5\x86\xac\xe8\x87\xb3",   "Winter Solstice",
       "Shortest day, yuzu baths, kabocha pumpkin eaten for health",
       "Winter arrives \xe2\x80\x94 the longest night, yuzu in the bath, light returns tomorrow" },
    { 19, "Shokan",    "\xe5\xb0\x8f\xe5\xaf\x92",   "Minor Cold",
       "Deep cold, New Year preparations, nanakusa (seven herbs) approaching",
       "Small cold \xe2\x80\x94 the cold deepens as the new year begins" },
    { 20, "Daikan",    "\xe5\xa4\xa7\xe5\xaf\x92",   "Major Cold",
       "Coldest period, miso making, the patience of deep winter",
       "Great cold \xe2\x80\x94 the deepest freeze, when miso is prepared and patience rewarded" },
    { 21, "Risshun",   "\xe7\xab\x8b\xe6\x98\xa5",   "Start of Spring",
       "First spring, plum blossoms, uguisu (warbler) sings",
       "Spring stands \xe2\x80\x94 the plum dares to bloom in the cold, the warbler calls" },
    { 22, "Usui",      "\xe9\x9b\xa8\xe6\xb0\xb4",   "Rainwater",
       "Snow turns to rain, ice melts, mist over the fields",
       "Rain water \xe2\x80\x94 the frozen world begins to flow again" },
    { 23, "Keichitsu", "\xe5\x95\x93\xe8\x9b\xb0",   "Awakening of Insects",
       "Thunder wakes hibernating creatures, peach trees blossom",
       "Insects awaken \xe2\x80\x94 the first thunder, the earth opens, everything stirs" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const ji_era_t INVALID_ERA = {
    -1, "?", "?", "?", "?", "?"
};

static const ji_rokuyo_t INVALID_ROKUYO = {
    -1, "?", "?", "?", "?"
};

static const ji_sekki_t INVALID_SEKKI = {
    -1, "?", "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ji_era_t ji_era_data(int era)
{
    if (era < 0 || era > 4) return INVALID_ERA;
    return ERAS[era];
}

ji_rokuyo_t ji_rokuyo_data(int rokuyo)
{
    if (rokuyo < 0 || rokuyo > 5) return INVALID_ROKUYO;
    return ROKUYO[rokuyo];
}

ji_sekki_t ji_sekki_data(int sekki)
{
    if (sekki < 0 || sekki > 23) return INVALID_SEKKI;
    return SEKKI[sekki];
}

japanese_interp_t ji_interpret(int era, int era_year, int rokuyo, int sekki)
{
    japanese_interp_t r;
    memset(&r, 0, sizeof(r));

    ji_era_t e = ji_era_data(era);

    if (e.era < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    ji_rokuyo_t ro = ji_rokuyo_data(rokuyo);
    ji_sekki_t  sk = ji_sekki_data(sekki);

    /* Glyph: first 3 chars of era name */
    size_t len = strlen(e.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, e.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (sk.sekki >= 0) {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s (%s)",
                 e.name, era_year, sk.name, sk.meaning);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d",
                 e.name, era_year);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Era: %s (%s) \xe2\x80\x94 %s. Year %d.",
                    e.name, e.meaning, e.spirit, era_year);

    if (ro.rokuyo >= 0) {
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        " Rokuyo: %s (%s) \xe2\x80\x94 %s.",
                        ro.name, ro.meaning, ro.guidance);
    }

    if (sk.sekki >= 0) {
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Sekki: %s (%s) \xe2\x80\x94 %s.",
                 sk.name, sk.meaning, sk.nature);
    }

    return r;
}

int ji_era_count(void)
{
    return 5;
}

int ji_rokuyo_count(void)
{
    return 6;
}

int ji_sekki_count(void)
{
    return 24;
}
