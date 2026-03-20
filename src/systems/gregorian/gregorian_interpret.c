/* gregorian_interpret.c — Gregorian calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "gregorian_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static month data (12 months)
 * ================================================================ */

static const gi_month_t MONTHS[12] = {
    {  1, "January",   "Janus \xe2\x80\x94 two-faced god of beginnings, gates, and transitions",
       "Deep winter, reflection, fresh starts",
       "The month of doorways \xe2\x80\x94 one face looks back, one looks forward" },
    {  2, "February",  "Februa \xe2\x80\x94 Roman purification festival (Lupercalia)",
       "Late winter, purification, preparation for spring",
       "The month of cleansing \xe2\x80\x94 the last fires of purification before new growth" },
    {  3, "March",     "Mars \xe2\x80\x94 god of war and agriculture, the Roman new year",
       "Spring equinox, awakening, aggressive new growth",
       "The month of Mars \xe2\x80\x94 the old Roman year began here, with war and planting alike" },
    {  4, "April",     "Aperire (to open) \xe2\x80\x94 the earth opens, buds unfold",
       "Full spring, opening, blossoming",
       "The month of opening \xe2\x80\x94 everything that was closed begins to unfold" },
    {  5, "May",       "Maia \xe2\x80\x94 goddess of growth, mother of Mercury",
       "Late spring, fertility, abundance beginning",
       "The month of growth \xe2\x80\x94 the earth goddess whose name means 'she who is great'" },
    {  6, "June",      "Juno \xe2\x80\x94 queen of the gods, patron of marriage",
       "Summer solstice, union, peak daylight",
       "The month of marriage \xe2\x80\x94 the queen of heaven blesses the longest days" },
    {  7, "July",      "Julius Caesar \xe2\x80\x94 reformed the calendar, deified after death",
       "High summer, power, radiance",
       "The month of Caesar \xe2\x80\x94 named for the man who remade time itself" },
    {  8, "August",    "Augustus Caesar \xe2\x80\x94 first Roman emperor, 'the revered one'",
       "Late summer, harvest begins, empire at zenith",
       "The month of Augustus \xe2\x80\x94 the emperor who demanded a month as grand as July" },
    {  9, "September", "Septem (seven) \xe2\x80\x94 the seventh month of the old Roman calendar",
       "Autumn equinox, balance, gathering in",
       "The seventh month \xe2\x80\x94 still carrying its old number though it is now the ninth" },
    { 10, "October",   "Octo (eight) \xe2\x80\x94 the eighth month of the old Roman calendar",
       "Mid-autumn, deepening, transformation",
       "The eighth month \xe2\x80\x94 the world turns inward as the light contracts" },
    { 11, "November",  "Novem (nine) \xe2\x80\x94 the ninth month of the old Roman calendar",
       "Late autumn, remembrance, preparation for dark",
       "The ninth month \xe2\x80\x94 the dead are honored as the year grows old" },
    { 12, "December",  "Decem (ten) \xe2\x80\x94 the tenth month of the old Roman calendar",
       "Winter solstice, Saturnalia, the return of light",
       "The tenth month \xe2\x80\x94 in the deepest dark, the ancient Romans feasted and the sun was reborn" }
};

/* ================================================================
 * Static season data (4 seasons, Northern Hemisphere)
 * ================================================================ */

static const gi_season_t SEASONS[4] = {
    { 0, "Spring",
      "Rebirth, growth, potential becoming actual",
      "Expansive \xe2\x80\x94 energy rises, seeds germinate, days lengthen",
      "The season of becoming \xe2\x80\x94 everything that slept now wakes" },
    { 1, "Summer",
      "Fullness, radiance, maximum expression",
      "Radiant \xe2\x80\x94 energy at peak, light dominates, life in full bloom",
      "The season of being \xe2\x80\x94 the world at maximum brilliance" },
    { 2, "Autumn",
      "Harvest, release, gratitude for what grew",
      "Contracting \xe2\x80\x94 energy turns inward, fruit ripens, leaves fall",
      "The season of letting go \xe2\x80\x94 beauty in decay, richness in release" },
    { 3, "Winter",
      "Rest, depth, the hidden seed, essential stillness",
      "Still \xe2\x80\x94 energy withdrawn to roots, silence, gestation in darkness",
      "The season of rest \xe2\x80\x94 what appears dead is dreaming" }
};

/* ================================================================
 * Static day data (7 days, planetary hours tradition)
 * ================================================================ */

static const gi_day_t DAYS[7] = {
    { 0, "Monday",    "Moon",    "Old English Monandaeg (Moon's day)",
      "The Moon's day \xe2\x80\x94 intuition, emotion, the tides within" },
    { 1, "Tuesday",   "Mars",    "Old English Tiwesdaeg (Tyr's day, Norse Mars)",
      "Mars' day \xe2\x80\x94 courage, action, the warrior's resolve" },
    { 2, "Wednesday", "Mercury", "Old English Wodnesdaeg (Woden's day, Norse Mercury)",
      "Mercury's day \xe2\x80\x94 communication, craft, the trickster's wit" },
    { 3, "Thursday",  "Jupiter", "Old English Thunresdaeg (Thor's day, Norse Jupiter)",
      "Jupiter's day \xe2\x80\x94 expansion, abundance, the thunder god's generosity" },
    { 4, "Friday",    "Venus",   "Old English Frigedaeg (Freya's day, Norse Venus)",
      "Venus' day \xe2\x80\x94 love, beauty, the goddess of desire" },
    { 5, "Saturday",  "Saturn",  "Latin Saturni dies (Saturn's day)",
      "Saturn's day \xe2\x80\x94 discipline, structure, the taskmaster's lesson" },
    { 6, "Sunday",    "Sun",     "Old English Sunnandaeg (Sun's day)",
      "The Sun's day \xe2\x80\x94 vitality, sovereignty, the center of all orbits" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const gi_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?"
};

static const gi_season_t INVALID_SEASON = {
    -1, "?", "?", "?", "?"
};

static const gi_day_t INVALID_DAY = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

gi_month_t gi_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

gi_season_t gi_season_data(int season)
{
    if (season < 0 || season > 3) return INVALID_SEASON;
    return SEASONS[season];
}

gi_day_t gi_day_data(int day)
{
    if (day < 0 || day > 6) return INVALID_DAY;
    return DAYS[day];
}

gregorian_interp_t gi_interpret(int month, int day_of_month, int day_of_week)
{
    gregorian_interp_t r;
    memset(&r, 0, sizeof(r));

    gi_month_t m = gi_month_data(month);

    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    gi_day_t d = gi_day_data(day_of_week);

    /* Season: Mar-May=Spring, Jun-Aug=Summer, Sep-Nov=Autumn, Dec-Feb=Winter */
    int season_idx;
    if (month >= 3 && month <= 5) season_idx = 0;
    else if (month >= 6 && month <= 8) season_idx = 1;
    else if (month >= 9 && month <= 11) season_idx = 2;
    else season_idx = 3;
    gi_season_t s = gi_season_data(season_idx);

    /* Glyph: first 3 chars of month name */
    size_t len = strlen(m.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    snprintf(r.glance, sizeof(r.glance),
             "%s %d (%s) \xe2\x80\x94 %s",
             m.name, day_of_month, d.name, s.name);

    /* Detail */
    snprintf(r.detail, sizeof(r.detail),
             "%s: %s. %s. Season: %s \xe2\x80\x94 %s. "
             "%s (%s): %s.",
             m.name, m.origin, m.quality,
             s.name, s.theme,
             d.name, d.planet, d.brief);

    return r;
}

int gi_month_count(void)
{
    return 12;
}

int gi_season_count(void)
{
    return 4;
}

int gi_day_count(void)
{
    return 7;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

gregorian_interp_t gi_interpret_locale(int month, int day_of_month,
                                       int day_of_week,
                                       i18n_locale_t locale)
{
    /* English fast path — use existing static data */
    if (locale == I18N_LOCALE_EN) {
        return gi_interpret(month, day_of_month, day_of_week);
    }

    gregorian_interp_t r;
    memset(&r, 0, sizeof(r));

    if (month < 1 || month > 12) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Season: Mar-May=0, Jun-Aug=1, Sep-Nov=2, Dec-Feb=3 */
    int season_idx;
    if (month >= 3 && month <= 5) season_idx = 0;
    else if (month >= 6 && month <= 8) season_idx = 1;
    else if (month >= 9 && month <= 11) season_idx = 2;
    else season_idx = 3;

    /* Build keys and fetch locale-aware strings */
    char key[64];

    snprintf(key, sizeof(key), "gregorian.month.%d.name", month);
    const char *month_name = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.month.%d.origin", month);
    const char *month_origin = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.month.%d.quality", month);
    const char *month_quality = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.season.%d.name", season_idx);
    const char *season_name = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.season.%d.theme", season_idx);
    const char *season_theme = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.day.%d.name", day_of_week);
    const char *day_name = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.day.%d.planet", day_of_week);
    const char *day_planet = content_get(key, locale);

    snprintf(key, sizeof(key), "gregorian.day.%d.brief", day_of_week);
    const char *day_brief = content_get(key, locale);

    /* Glyph: first 3 chars of localized month name */
    size_t len = strlen(month_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, month_name, copy);
    r.glyph[copy] = '\0';

    /* Glance with positional format specifiers */
    const char *tpl_glance = content_get("gregorian.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             month_name, day_of_month, day_name, season_name);

    /* Detail with positional format specifiers */
    const char *tpl_detail = content_get("gregorian.tpl.detail", locale);
    snprintf(r.detail, sizeof(r.detail), tpl_detail,
             month_name, month_origin, month_quality,
             season_name, season_theme,
             day_name, day_planet, day_brief);

    return r;
}
