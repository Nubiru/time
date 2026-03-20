/* egyptian_interpret.c — Egyptian calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "egyptian_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static season data (4 entries: 3 seasons + epagomenal)
 * ================================================================ */

static const ei_season_t SEASONS[4] = {
    { 0, "Akhet", "Inundation",
      "The Nile floods, covering the land with fertile black silt",
      "Surrender to forces beyond control \xe2\x80\x94 what must be submerged so new life can grow?",
      "The great flooding \xe2\x80\x94 destruction and renewal united in the same dark water" },

    { 1, "Peret", "Emergence",
      "The waters recede, seeds are planted in the rich new soil",
      "The ground has been prepared \xe2\x80\x94 what seeds are you planting in the fertile aftermath?",
      "The coming forth \xe2\x80\x94 life pushes through the wet earth after the flood" },

    { 2, "Shemu", "Harvest",
      "The crops are gathered, the land dries under the blazing sun",
      "What has ripened in you that is ready to be gathered and shared?",
      "The low water \xe2\x80\x94 the sun burns away everything but what has taken root" },

    { 3, "Epagomenal", "Days Outside Time",
      "Five days between years, sacred and dangerous, belonging to no month",
      "Between endings and beginnings \xe2\x80\x94 the liminal space where gods are born",
      "The five days upon the year \xe2\x80\x94 when time pauses and the divine breaks through" }
};

/* ================================================================
 * Static month data (12 months)
 * ================================================================ */

static const ei_month_t MONTHS[12] = {
    {  1, "Thoth",     "Thoth (Djehuty)",     "Wisdom, writing, measurement, the moon",
       "First month \xe2\x80\x94 the ibis-headed god who invented writing and measured time itself" },
    {  2, "Phaophi",   "Hathor",              "Love, beauty, music, motherhood",
       "Month of the golden goddess \xe2\x80\x94 joy and celebration as the flood settles" },
    {  3, "Athyr",     "Sekhmet/Hathor",      "Healing, war, solar fire, protection",
       "Month of the lioness \xe2\x80\x94 the fierce protector who can both destroy and heal" },
    {  4, "Choiak",    "Osiris",              "Death, resurrection, the underworld, grain",
       "Month of the slain king \xe2\x80\x94 the mystery of death that feeds all life" },
    {  5, "Tybi",      "Min/Horus",           "Fertility, virility, the young falcon",
       "Month of emergence \xe2\x80\x94 the potent seed that breaks through the dark earth" },
    {  6, "Mechir",    "Rekeh-wer",           "Fire, purification, the burning sun",
       "Month of great burning \xe2\x80\x94 the fire that purifies and the light that reveals" },
    {  7, "Phamenoth", "Amenhotep (Imhotep)", "Architecture, medicine, divine craftsmanship",
       "Month of the builder \xe2\x80\x94 the deified architect who measured heaven and earth" },
    {  8, "Pharmuthi", "Renenutet",           "Harvest, abundance, the cobra guardian",
       "Month of the harvest goddess \xe2\x80\x94 the serpent who guards the granary" },
    {  9, "Pachons",   "Khonsu",              "The moon, healing, passage of time",
       "Month of the moon child \xe2\x80\x94 the wanderer who traverses the sky" },
    { 10, "Payni",     "Mut",                 "The great mother, queenship, the vulture",
       "Month of the divine mother \xe2\x80\x94 she who consumes death and births eternity" },
    { 11, "Epiphi",    "Wadjet",              "Protection, sovereignty, the green serpent",
       "Month of the cobra crown \xe2\x80\x94 the fierce eye of Ra that guards the throne" },
    { 12, "Mesore",    "Ra-Horakhty",         "The sun at zenith, creation, sovereignty",
       "Month of the birth of Ra \xe2\x80\x94 the sun reborn at the peak of its power" }
};

/* ================================================================
 * Static epagomenal day data (5 days)
 * ================================================================ */

static const ei_epag_t EPAGS[5] = {
    { 1, "Osiris",   "The Dying King",
      "Sacrifice, regeneration, the grain that dies to feed the world",
      "Day of Osiris \xe2\x80\x94 the king who was dismembered and made whole, ruling the dead from his green throne" },
    { 2, "Horus",    "The Avenger",
      "Justice, restoration, the falcon eye that sees all",
      "Day of Horus \xe2\x80\x94 the son who fought for his father's throne and restored Ma'at to the world" },
    { 3, "Set",      "The Disruptor",
      "Chaos, storms, the necessary opposition that defines order",
      "Day of Set \xe2\x80\x94 the red god of the desert whose violence is the other face of creation" },
    { 4, "Isis",     "The Enchantress",
      "Magic, healing, the love that reassembles what was scattered",
      "Day of Isis \xe2\x80\x94 the great of magic who gathered the pieces and remembered the dead back to life" },
    { 5, "Nephthys", "The Companion of Death",
      "The hidden, mourning, the beauty at the boundary between worlds",
      "Day of Nephthys \xe2\x80\x94 the mistress of the house who stands at the edge and guides souls across" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const ei_season_t INVALID_SEASON = {
    -1, "?", "?", "?", "?", "?"
};

static const ei_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?"
};

static const ei_epag_t INVALID_EPAG = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ei_season_t ei_season_data(int season)
{
    if (season < 0 || season > 3) return INVALID_SEASON;
    return SEASONS[season];
}

ei_month_t ei_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

ei_epag_t ei_epag_data(int day)
{
    if (day < 1 || day > 5) return INVALID_EPAG;
    return EPAGS[day - 1];
}

egyptian_interp_t ei_interpret(int month, int day, int sothic_year)
{
    egyptian_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Epagomenal days (month 13) */
    if (month == 13) {
        ei_epag_t e = ei_epag_data(day);
        ei_season_t s = ei_season_data(3);

        if (e.day < 0) {
            snprintf(r.glyph, sizeof(r.glyph), "?");
            snprintf(r.glance, sizeof(r.glance), "?");
            snprintf(r.detail, sizeof(r.detail), "?");
        } else {
            snprintf(r.glyph, sizeof(r.glyph), "Epg");
            snprintf(r.glance, sizeof(r.glance),
                     "Epagomenal day %d \xe2\x80\x94 %s: %s",
                     day, e.deity, e.archetype);
            snprintf(r.detail, sizeof(r.detail),
                     "%s. %s: %s. Theme: %s. "
                     "Season: %s \xe2\x80\x94 %s. Sothic cycle year %d of 1461.",
                     e.brief, e.deity, e.archetype, e.theme,
                     s.name, s.theme, sothic_year);
        }
        return r;
    }

    /* Regular months (1-12) */
    ei_month_t m = ei_month_data(month);

    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Season: months 1-4 = Akhet, 5-8 = Peret, 9-12 = Shemu */
    int season_idx = (month - 1) / 4;
    ei_season_t s = ei_season_data(season_idx);

    /* Glyph: first 3 chars of month name */
    size_t len = strlen(m.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    snprintf(r.glance, sizeof(r.glance),
             "%s day %d \xe2\x80\x94 %s (%s)",
             m.name, day, s.name, s.meaning);

    /* Detail */
    snprintf(r.detail, sizeof(r.detail),
             "Month: %s (deity: %s). Domain: %s. %s. "
             "Season: %s (%s) \xe2\x80\x94 %s. Reflection: %s. "
             "Sothic cycle year %d of 1461.",
             m.name, m.deity, m.domain, m.brief,
             s.name, s.meaning, s.theme, s.reflection,
             sothic_year);

    return r;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

egyptian_interp_t ei_interpret_locale(int month, int day, int sothic_year,
                                      i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return ei_interpret(month, day, sothic_year);
    }

    egyptian_interp_t r;
    memset(&r, 0, sizeof(r));

    char key[64];

    /* Epagomenal days (month 13) */
    if (month == 13) {
        int epag_idx = day - 1;
        if (epag_idx < 0 || epag_idx > 4) {
            snprintf(r.glyph, sizeof(r.glyph), "?");
            snprintf(r.glance, sizeof(r.glance), "?");
            snprintf(r.detail, sizeof(r.detail), "?");
            return r;
        }

        snprintf(key, sizeof(key), "egyptian.epag.%d.deity", epag_idx);
        const char *deity = content_get(key, locale);

        snprintf(key, sizeof(key), "egyptian.epag.%d.archetype", epag_idx);
        const char *archetype = content_get(key, locale);

        snprintf(key, sizeof(key), "egyptian.epag.%d.theme", epag_idx);
        const char *theme = content_get(key, locale);

        snprintf(r.glyph, sizeof(r.glyph), "Epg");

        const char *tpl_glance = content_get("egyptian.tpl.glance", locale);
        snprintf(r.glance, sizeof(r.glance), tpl_glance,
                 "Epagomenal", day, deity, archetype);

        const char *tpl_detail = content_get("egyptian.tpl.detail", locale);
        snprintf(r.detail, sizeof(r.detail), tpl_detail,
                 deity, archetype, theme, sothic_year);

        return r;
    }

    /* Regular months (1-12) */
    if (month < 1 || month > 12) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    snprintf(key, sizeof(key), "egyptian.month.%d.name", month);
    const char *month_name = content_get(key, locale);

    snprintf(key, sizeof(key), "egyptian.month.%d.deity", month);
    const char *deity = content_get(key, locale);

    snprintf(key, sizeof(key), "egyptian.month.%d.domain", month);
    const char *domain = content_get(key, locale);

    /* Season: months 1-4 = Akhet(0), 5-8 = Peret(1), 9-12 = Shemu(2) */
    int season_idx = (month - 1) / 4;

    snprintf(key, sizeof(key), "egyptian.season.%d.name", season_idx);
    const char *season_name = content_get(key, locale);

    snprintf(key, sizeof(key), "egyptian.season.%d.meaning", season_idx);
    const char *season_meaning = content_get(key, locale);

    /* Glyph: first 3 chars of month name */
    size_t len = strlen(month_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, month_name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    const char *tpl_glance = content_get("egyptian.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             month_name, day, season_name, season_meaning);

    /* Detail — base month narrative */
    const char *tpl_detail = content_get("egyptian.tpl.detail", locale);
    int pos = snprintf(r.detail, sizeof(r.detail), tpl_detail,
                       month_name, deity, domain, sothic_year);

    /* Append season data */
    if (pos > 0 && (size_t)pos < sizeof(r.detail)) {
        const char *tpl_season = content_get("egyptian.tpl.detail_season", locale);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 tpl_season, season_name, season_meaning);
    }

    return r;
}

int ei_season_count(void)
{
    return 4;
}

int ei_month_count(void)
{
    return 12;
}

int ei_epag_count(void)
{
    return 5;
}
