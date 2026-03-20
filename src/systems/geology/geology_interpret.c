/* geology_interpret.c — Geological time interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "geology_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static eon data (4 eons)
 * ================================================================ */

static const gli_eon_t EONS[4] = {
    { 0, "Hadean", "Hades (Greek underworld)",
      "Molten hell \xe2\x80\x94 the bombardment era when Earth was a ball of magma under constant asteroid rain",
      "No life \xe2\x80\x94 surface too hot for water, atmosphere of hydrogen and helium",
      "The age of fire \xe2\x80\x94 4.54 to 4.0 billion years ago, Earth forming from the solar nebula" },

    { 1, "Archean", "Arkhe (Greek: beginning)",
      "First oceans, first continents \xe2\x80\x94 the planet cools enough for liquid water and the first rocks",
      "First life appears \xe2\x80\x94 prokaryotes, stromatolites, the Great Oxidation Event begins",
      "The age of beginnings \xe2\x80\x94 4.0 to 2.5 Ga, when life invented itself in a world without oxygen" },

    { 2, "Proterozoic", "Proteros (Greek: earlier) + zoe (life)",
      "Snowball Earth episodes, oxygen rising \xe2\x80\x94 the long preparation for complex life",
      "Eukaryotes evolve, first multicellular organisms \xe2\x80\x94 the Ediacaran fauna at the end",
      "The age of hidden life \xe2\x80\x94 2.5 Ga to 538 Ma, the longest eon, oxygen slowly transforming everything" },

    { 3, "Phanerozoic", "Phaneros (Greek: visible) + zoe (life)",
      "The age we live in \xe2\x80\x94 all familiar life, all mass extinctions, all human history",
      "Cambrian explosion to now \xe2\x80\x94 trilobites, dinosaurs, mammals, humans, all in 538 million years",
      "The age of visible life \xe2\x80\x94 538 Ma to present, everything we recognize as life" }
};

/* ================================================================
 * Static extinction data (5 major mass extinctions, the Big Five)
 * ================================================================ */

static const gli_extinction_t EXTINCTIONS[5] = {
    { 0, "Late Ordovician", "End-Ordovician (~443 Ma)",
      "Glaciation of Gondwana, sea level drop, ocean anoxia",
      "Recovery took 5-10 million years; life diversified anew in the Silurian",
      "The ice extinction \xe2\x80\x94 85% of marine species lost when Gondwana froze at the South Pole" },

    { 1, "Late Devonian", "Late Devonian (~372 Ma)",
      "Multiple pulses over 20 My: ocean anoxia, possible volcanism, Appalachian weathering",
      "Armored fish disappeared; tetrapods (our ancestors) survived and walked onto land",
      "The slow extinction \xe2\x80\x94 75% of species lost in pulses over 20 million years" },

    { 2, "End-Permian", "End-Permian (~252 Ma)",
      "Siberian Traps volcanism, global warming, ocean acidification and anoxia",
      "Recovery took 10 million years; the emptied world was inherited by archosaurs leading to dinosaurs",
      "The Great Dying \xe2\x80\x94 96% of all species lost, the worst catastrophe life has ever faced" },

    { 3, "End-Triassic", "End-Triassic (~201 Ma)",
      "Central Atlantic Magmatic Province volcanism, CO2 spike, ocean acidification",
      "Dinosaurs inherited the emptied niches, beginning their 135-million-year reign",
      "The dinosaur's opportunity \xe2\x80\x94 80% of species lost, clearing the stage for the Jurassic giants" },

    { 4, "End-Cretaceous", "End-Cretaceous (~66 Ma)",
      "Chicxulub asteroid impact + Deccan Traps volcanism, impact winter, global fires",
      "Mammals diversified explosively; birds survived as the last dinosaurs; our lineage began",
      "The asteroid \xe2\x80\x94 76% of species including all non-avian dinosaurs, ending one world and beginning ours" }
};

/* ================================================================
 * Static supercontinent data (7 supercontinents)
 * ================================================================ */

static const gli_supercontinent_t SUPERCONTINENTS[7] = {
    { 0, "Vaalbara", "~3.6-2.8 Ga",
      "Oldest proposed supercontinent \xe2\x80\x94 evidence from South Africa (Kaapvaal) and Australia (Pilbara)",
      "The first gathering \xe2\x80\x94 Earth's earliest attempt to assemble its crust into one mass" },
    { 1, "Kenorland", "~2.7-2.1 Ga",
      "Archean supercontinent that preceded the Great Oxidation Event",
      "The continent that breathed \xe2\x80\x94 its breakup may have triggered the rise of atmospheric oxygen" },
    { 2, "Columbia", "~1.8-1.3 Ga",
      "Also called Nuna \xe2\x80\x94 first well-documented supercontinent, all major cratons joined",
      "The first proven unity \xe2\x80\x94 Earth's continents locked together for 500 million years" },
    { 3, "Rodinia", "~1.1-0.75 Ga",
      "Supercontinent whose breakup may have triggered Snowball Earth glaciations",
      "The continent that froze the world \xe2\x80\x94 its rifting led to the most extreme ice ages" },
    { 4, "Pannotia", "~0.63-0.55 Ga",
      "Brief assembly at the end of the Ediacaran, just before the Cambrian explosion",
      "The fleeting assembly \xe2\x80\x94 together and apart in 80 million years, bookending the first complex animals" },
    { 5, "Pangaea", "~0.34-0.17 Ga",
      "The most famous supercontinent \xe2\x80\x94 Permian to Jurassic, all land united",
      "The one we remember \xe2\x80\x94 the supercontinent that shaped the world we know" },
    { 6, "Pangaea Proxima", "~0.25 Ga future",
      "Predicted next supercontinent \xe2\x80\x94 Atlantic closing, new assembly in 250 million years",
      "The one to come \xe2\x80\x94 the Atlantic will close and the continents will embrace again" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const gli_eon_t INVALID_EON = {
    -1, "?", "?", "?", "?", "?"
};

static const gli_extinction_t INVALID_EXTINCTION = {
    -1, "?", "?", "?", "?", "?"
};

static const gli_supercontinent_t INVALID_SUPERCONTINENT = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

gli_eon_t gli_eon_data(int eon)
{
    if (eon < 0 || eon > 3) return INVALID_EON;
    return EONS[eon];
}

gli_extinction_t gli_extinction_data(int extinction)
{
    if (extinction < 0 || extinction > 4) return INVALID_EXTINCTION;
    return EXTINCTIONS[extinction];
}

gli_supercontinent_t gli_supercontinent_data(int index)
{
    if (index < 0 || index > 6) return INVALID_SUPERCONTINENT;
    return SUPERCONTINENTS[index];
}

geology_interp_t gli_interpret(int eon, int nearest_extinction,
                               int supercontinent)
{
    geology_interp_t r;
    memset(&r, 0, sizeof(r));

    gli_eon_t e = gli_eon_data(eon);

    if (e.eon < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of eon name */
    size_t len = strlen(e.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, e.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    snprintf(r.glance, sizeof(r.glance),
             "%s \xe2\x80\x94 %s",
             e.name, e.meaning);

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Eon: %s (%s). %s. Life: %s.",
                    e.name, e.meaning, e.character, e.life);

    if (nearest_extinction >= 0 && nearest_extinction <= 4) {
        gli_extinction_t x = gli_extinction_data(nearest_extinction);
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        " Nearest extinction: %s \xe2\x80\x94 %s.",
                        x.name, x.cause);
    }

    if (supercontinent >= 0 && supercontinent <= 6) {
        gli_supercontinent_t s = gli_supercontinent_data(supercontinent);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Supercontinent: %s (%s).",
                 s.name, s.age);
    }

    return r;
}

int gli_eon_count(void) { return 4; }
int gli_extinction_count(void) { return 5; }
int gli_supercontinent_count(void) { return 7; }

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

geology_interp_t gli_interpret_locale(int eon, int nearest_extinction,
                                       int supercontinent,
                                       i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return gli_interpret(eon, nearest_extinction, supercontinent);
    }

    geology_interp_t r;
    memset(&r, 0, sizeof(r));

    if (eon < 0 || eon > 3) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    char key[64];

    /* Eon data */
    snprintf(key, sizeof(key), "geology.eon.%d.name", eon);
    const char *eon_name = content_get(key, locale);

    snprintf(key, sizeof(key), "geology.eon.%d.meaning", eon);
    const char *eon_meaning = content_get(key, locale);

    snprintf(key, sizeof(key), "geology.eon.%d.character", eon);
    const char *eon_character = content_get(key, locale);

    snprintf(key, sizeof(key), "geology.eon.%d.life", eon);
    const char *eon_life = content_get(key, locale);

    /* T0: Glyph — first 3 chars of eon name */
    size_t len = strlen(eon_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, eon_name, copy);
    r.glyph[copy] = '\0';

    /* T1: Glance */
    const char *tpl_glance = content_get("geology.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             eon_name, eon_meaning);

    /* T3: Detail */
    const char *tpl_detail = content_get("geology.tpl.detail", locale);
    int pos = snprintf(r.detail, sizeof(r.detail), tpl_detail,
                       eon_name, eon_meaning, eon_character, eon_life);

    /* Append extinction if valid */
    if (nearest_extinction >= 0 && nearest_extinction <= 4 &&
        pos > 0 && (size_t)pos < sizeof(r.detail)) {
        snprintf(key, sizeof(key), "geology.extinction.%d.name",
                 nearest_extinction);
        const char *ext_name = content_get(key, locale);

        snprintf(key, sizeof(key), "geology.extinction.%d.cause",
                 nearest_extinction);
        const char *ext_cause = content_get(key, locale);

        const char *tpl_ext = content_get("geology.tpl.extinction", locale);
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        tpl_ext, ext_name, ext_cause);
    }

    /* Append supercontinent if valid */
    if (supercontinent >= 0 && supercontinent <= 6 &&
        pos > 0 && (size_t)pos < sizeof(r.detail)) {
        snprintf(key, sizeof(key), "geology.supercontinent.%d.name",
                 supercontinent);
        const char *sc_name = content_get(key, locale);

        snprintf(key, sizeof(key), "geology.supercontinent.%d.age",
                 supercontinent);
        const char *sc_age = content_get(key, locale);

        const char *tpl_sc = content_get("geology.tpl.supercontinent", locale);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 tpl_sc, sc_name, sc_age);
    }

    return r;
}
