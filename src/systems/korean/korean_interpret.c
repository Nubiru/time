/* korean_interpret.c — Korean calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "korean_interpret.h"
#include "korean_calendar.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static festival data (7 festivals)
 * ================================================================ */

static const ki_festival_t FESTIVALS[7] = {
    { 0, "Seollal", "\xec\x84\xa4\xeb\x82\xa0",
      "New beginnings \xe2\x80\x94 honoring ancestors, renewing family bonds",
      "Charye (ancestral rites), sebae (bowing to elders for blessings), tteokguk (rice cake soup for gaining a year)",
      "Lunar New Year \xe2\x80\x94 the most important holiday, when the whole nation returns home" },

    { 1, "Daeboreum", "\xeb\x8c\x80\xeb\xb3\xb4\xeb\xa6\x84",
      "Fullness \xe2\x80\x94 the first full moon, wishing for a bountiful year",
      "Cracking nuts (bureom), eating five-grain rice (ogokbap), moon-viewing, fire festivals (daljip burning)",
      "Great Full Moon \xe2\x80\x94 the first full moon of the year burns away bad luck" },

    { 2, "Samjinnal", "\xec\x82\xbc\xec\xa7\x84\xeb\x82\xa0",
      "Spring arrival \xe2\x80\x94 swallows return, nature awakens",
      "Flower pancakes (hwajeon), spring outings, welcoming the swallows",
      "Double Third \xe2\x80\x94 the swallows return from the south and spring declares itself" },

    { 3, "Dano", "\xeb\x8b\xa8\xec\x98\xa4",
      "Peak yang energy \xe2\x80\x94 the most yang day of the year (5th day, 5th month)",
      "Ssireum (wrestling), swing riding, washing hair in iris water, surichwitteok (herb rice cakes)",
      "Double Fifth \xe2\x80\x94 when heaven's yang energy is strongest and the earth celebrates with wrestling" },

    { 4, "Chilseok", "\xec\xb9\xa0\xec\x84\x9d",
      "Love and reunion \xe2\x80\x94 the Weaver and Cowherd cross the Milky Way",
      "Wishing upon stars, wheat flour noodles (milguksu), bathing in star-blessed rain",
      "Star Festival \xe2\x80\x94 the celestial lovers' once-a-year meeting across the bridge of magpies" },

    { 5, "Chuseok", "\xec\xb6\x94\xec\x84\x9d",
      "Gratitude \xe2\x80\x94 harvest thanksgiving, abundance shared with ancestors",
      "Songpyeon (half-moon rice cakes), charye (ancestral rites), visiting graves (seongmyo), folk games",
      "Harvest Moon \xe2\x80\x94 Korea's Thanksgiving, when the full autumn moon shines on shared abundance" },

    { 6, "Jungyangjeol", "\xec\xa4\x91\xec\x96\x91\xec\xa0\x88",
      "Longevity \xe2\x80\x94 ascending heights, chrysanthemum appreciation",
      "Climbing mountains, chrysanthemum wine (gukhwaju), chrysanthemum pancakes, poetry",
      "Double Ninth \xe2\x80\x94 ascending to high ground to see far, drinking the flower of longevity" }
};

/* ================================================================
 * Static element data (5 elements)
 * ================================================================ */

static const ki_element_t ELEMENTS[5] = {
    { 0, "Mok", "Growth, expansion, benevolence, the force that rises",
      "Spring", "Wood \xe2\x80\x94 the green force that pushes upward through all obstacles" },
    { 1, "Hwa", "Transformation, passion, propriety, the force that spreads",
      "Summer", "Fire \xe2\x80\x94 the radiance that illuminates and transforms" },
    { 2, "To",  "Stability, centering, trust, the force that holds",
      "Late summer", "Earth \xe2\x80\x94 the center that nourishes and grounds all things" },
    { 3, "Geum", "Precision, righteousness, clarity, the force that contracts",
      "Autumn", "Metal \xe2\x80\x94 the sharp edge that refines and lets go" },
    { 4, "Su",  "Wisdom, depth, adaptability, the force that descends",
      "Winter", "Water \xe2\x80\x94 the deep stillness that finds its way through everything" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const ki_festival_t INVALID_FESTIVAL = {
    -1, "?", "?", "?", "?", "?"
};

static const ki_element_t INVALID_ELEMENT = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ki_festival_t ki_festival_data(int festival)
{
    if (festival < 0 || festival > 6) return INVALID_FESTIVAL;
    return FESTIVALS[festival];
}

ki_element_t ki_element_data(int element)
{
    if (element < 0 || element > 4) return INVALID_ELEMENT;
    return ELEMENTS[element];
}

korean_interp_t ki_interpret(int dangun_year, int animal, int element,
                             int polarity, int festival)
{
    korean_interp_t r;
    memset(&r, 0, sizeof(r));

    const char *animal_name = korean_animal_name(animal);
    if (animal_name[0] == '?') {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    ki_element_t el = ki_element_data(element);
    const char *pol_str = (polarity == 0) ? "Yang" : "Yin";

    /* Glyph: first 3 chars of animal name */
    size_t len = strlen(animal_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, animal_name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (festival >= 0 && festival <= 6) {
        ki_festival_t f = ki_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "Dangun %d \xe2\x80\x94 %s %s (%s) \xe2\x80\x94 %s",
                 dangun_year, el.korean, animal_name, pol_str, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "Dangun %d \xe2\x80\x94 %s %s (%s)",
                 dangun_year, el.korean, animal_name, pol_str);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Dangun year %d. Animal: %s. Element: %s (%s). Polarity: %s.",
                    dangun_year, animal_name,
                    el.korean, el.meaning, pol_str);

    if (festival >= 0 && festival <= 6) {
        ki_festival_t f = ki_festival_data(festival);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Festival: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int ki_festival_count(void)
{
    return 7;
}

int ki_element_count(void)
{
    return 5;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

korean_interp_t ki_interpret_locale(int dangun_year, int animal, int element,
                                    int polarity, int festival,
                                    i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return ki_interpret(dangun_year, animal, element, polarity, festival);
    }

    korean_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Animal name from korean_calendar module (system-native, not translated) */
    const char *animal_name = korean_animal_name(animal);
    if (animal_name[0] == '?') {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    char key[64];

    /* Element data */
    snprintf(key, sizeof(key), "korean.element.%d.korean", element);
    const char *el_korean = content_get(key, locale);

    snprintf(key, sizeof(key), "korean.element.%d.meaning", element);
    const char *el_meaning = content_get(key, locale);

    const char *pol_str = (polarity == 0) ? "Yang" : "Yin";

    /* Glyph: first 3 chars of animal name */
    size_t len = strlen(animal_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, animal_name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (festival >= 0 && festival <= 6) {
        snprintf(key, sizeof(key), "korean.festival.%d.name", festival);
        const char *fest_name = content_get(key, locale);

        const char *tpl = content_get("korean.tpl.glance", locale);
        snprintf(r.glance, sizeof(r.glance), tpl,
                 dangun_year, el_korean, animal_name, pol_str, fest_name);
    } else {
        const char *tpl = content_get("korean.tpl.glance", locale);
        snprintf(r.glance, sizeof(r.glance), tpl,
                 dangun_year, el_korean, animal_name, pol_str, "");
    }

    /* Detail — two-part: base + optional festival append */
    const char *tpl_det = content_get("korean.tpl.detail", locale);
    int pos = snprintf(r.detail, sizeof(r.detail), tpl_det,
                       dangun_year, animal_name, el_korean, el_meaning, pol_str);

    if (festival >= 0 && festival <= 6 &&
        pos > 0 && (size_t)pos < sizeof(r.detail)) {
        snprintf(key, sizeof(key), "korean.festival.%d.name", festival);
        const char *fest_name = content_get(key, locale);

        snprintf(key, sizeof(key), "korean.festival.%d.theme", festival);
        const char *theme = content_get(key, locale);

        snprintf(key, sizeof(key), "korean.festival.%d.practice", festival);
        const char *practice = content_get(key, locale);

        const char *tpl_f = content_get("korean.tpl.detail_festival", locale);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 tpl_f, fest_name, theme, practice);
    }

    return r;
}
