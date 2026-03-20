/* astronomy_interpret.c — Astronomical interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "astronomy_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static planet data (8 planets)
 * ================================================================ */

static const ai_planet_t PLANETS[8] = {
    { 0, "Mercury", "Hermes / Nabu / Thoth",
      "Communication, intellect, commerce, the messenger between worlds",
      "The Trickster-Messenger \xe2\x80\x94 quicksilver mind, the psychopomp who bridges realms",
      "The swift messenger \xe2\x80\x94 closest to the Sun, never far from the source of light" },

    { 1, "Venus", "Aphrodite / Ishtar / Freya",
      "Love, beauty, harmony, values, artistic creation",
      "The Beloved \xe2\x80\x94 the mirror of desire, beauty as a force that civilizes",
      "The morning and evening star \xe2\x80\x94 the brightest planet, whose light is borrowed" },

    { 2, "Earth", "Gaia / Terra / Jord",
      "Home, embodiment, the body, the ground we stand on",
      "The Mother \xe2\x80\x94 the living world that sustains all observation of the cosmos",
      "The pale blue dot \xe2\x80\x94 the only planet we know that looks back at the sky" },

    { 3, "Mars", "Ares / Nergal / Tyr",
      "Action, drive, conflict, courage, the will to assert",
      "The Warrior \xe2\x80\x94 the red force that fights for what matters",
      "The red planet \xe2\x80\x94 iron-rusted, blood-colored, named for the god of war" },

    { 4, "Jupiter", "Zeus / Marduk / Thor",
      "Expansion, wisdom, generosity, law, the great benefic",
      "The King \xe2\x80\x94 the largest body in the solar system after the Sun, protector and judge",
      "The great benefic \xe2\x80\x94 more massive than all other planets combined" },

    { 5, "Saturn", "Kronos / Ninurta / Loki",
      "Structure, limitation, time, discipline, the great teacher",
      "The Elder \xe2\x80\x94 the ringed boundary, the lesson that cannot be avoided",
      "The ringed teacher \xe2\x80\x94 beautiful and severe, the lord of time and consequence" },

    { 6, "Uranus", "Ouranos / sky father",
      "Revolution, originality, sudden insight, the unexpected",
      "The Awakener \xe2\x80\x94 the lightning bolt of change, the force that disrupts to liberate",
      "The tilted rebel \xe2\x80\x94 rolling on its side, the planet that breaks every rule" },

    { 7, "Neptune", "Poseidon / Ea / Njord",
      "Imagination, transcendence, illusion, the oceanic unconscious",
      "The Mystic \xe2\x80\x94 the dissolving ocean where boundaries cease to exist",
      "The blue dreamer \xe2\x80\x94 farthest major planet, where the solar system fades into mystery" }
};

/* ================================================================
 * Static moon phase data (8 phases)
 * ================================================================ */

static const ai_phase_t PHASES[8] = {
    { 0, "New Moon",
      "Beginnings, seed planting, intention setting, the dark of renewal",
      "Set intentions, start projects, rest and reflect in the darkness",
      "The dark seed \xe2\x80\x94 invisible, full of potential, everything begins here" },

    { 1, "Waxing Crescent",
      "Emerging intention, first commitment, the courage to begin",
      "Take the first step, gather resources, push through initial resistance",
      "The first sliver \xe2\x80\x94 intention made visible, the young light gathering strength" },

    { 2, "First Quarter",
      "Challenge, decision, the crisis of action",
      "Make decisions, overcome obstacles, commit to the direction chosen",
      "The half-lit choice \xe2\x80\x94 light and dark in equal measure, the crossroads" },

    { 3, "Waxing Gibbous",
      "Refinement, adjustment, patience before fulfillment",
      "Refine the plan, adjust based on feedback, trust the process",
      "Almost full \xe2\x80\x94 the final preparation, polishing before the peak" },

    { 4, "Full Moon",
      "Culmination, illumination, harvest, revelation",
      "Celebrate completion, see clearly, harvest what was planted at the new moon",
      "The full light \xe2\x80\x94 everything illuminated, nothing hidden, the peak" },

    { 5, "Waning Gibbous",
      "Gratitude, sharing, teaching, distribution",
      "Share what you've learned, give back, express gratitude for the harvest",
      "The generous decline \xe2\x80\x94 the light begins to give itself away" },

    { 6, "Last Quarter",
      "Release, forgiveness, clearing, the crisis of letting go",
      "Let go of what no longer serves, forgive, clear space for the new",
      "The half-lit release \xe2\x80\x94 light and dark again equal, but now the light is leaving" },

    { 7, "Waning Crescent",
      "Surrender, rest, preparation for the void, the balsamic phase",
      "Rest deeply, surrender control, prepare for the coming darkness",
      "The last whisper \xe2\x80\x94 the thinnest light before the dark, the wisdom of surrender" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const ai_planet_t INVALID_PLANET = {
    -1, "?", "?", "?", "?", "?"
};

static const ai_phase_t INVALID_PHASE = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ai_planet_t ai_planet_data(int planet)
{
    if (planet < 0 || planet > 7) return INVALID_PLANET;
    return PLANETS[planet];
}

ai_phase_t ai_phase_data(int phase)
{
    if (phase < 0 || phase > 7) return INVALID_PHASE;
    return PHASES[phase];
}

astronomy_interp_t ai_astro_interpret(int planet, int is_retrograde,
                                      int moon_phase)
{
    astronomy_interp_t r;
    memset(&r, 0, sizeof(r));

    ai_planet_t p = ai_planet_data(planet);

    if (p.planet < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of planet name */
    size_t len = strlen(p.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, p.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    ai_phase_t ph = ai_phase_data(moon_phase);
    if (is_retrograde) {
        snprintf(r.glance, sizeof(r.glance),
                 "%s (retrograde) \xe2\x80\x94 %s",
                 p.name, p.archetype);
    } else if (ph.phase >= 0) {
        snprintf(r.glance, sizeof(r.glance),
                 "%s \xe2\x80\x94 %s + %s",
                 p.name, p.archetype, ph.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s \xe2\x80\x94 %s",
                 p.name, p.archetype);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Planet: %s. Deity: %s. Domain: %s. %s.",
                    p.name, p.deity, p.domain, p.brief);

    if (is_retrograde) {
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        " RETROGRADE: apparent backward motion \xe2\x80\x94"
                        " the planet's themes turn inward for review.");
    }

    if (ph.phase >= 0) {
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Moon: %s \xe2\x80\x94 %s.",
                 ph.name, ph.theme);
    }

    return r;
}

int ai_planet_count(void) { return 8; }
int ai_phase_count(void) { return 8; }

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

astronomy_interp_t ai_astro_interpret_locale(int planet, int is_retrograde,
                                             int moon_phase,
                                             i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return ai_astro_interpret(planet, is_retrograde, moon_phase);
    }

    astronomy_interp_t r;
    memset(&r, 0, sizeof(r));

    if (planet < 0 || planet > 7) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    char key[64];

    snprintf(key, sizeof(key), "astronomy.planet.%d.name", planet);
    const char *p_name = content_get(key, locale);

    snprintf(key, sizeof(key), "astronomy.planet.%d.archetype", planet);
    const char *p_arch = content_get(key, locale);

    snprintf(key, sizeof(key), "astronomy.planet.%d.deity", planet);
    const char *p_deity = content_get(key, locale);

    snprintf(key, sizeof(key), "astronomy.planet.%d.domain", planet);
    const char *p_domain = content_get(key, locale);

    snprintf(key, sizeof(key), "astronomy.planet.%d.brief", planet);
    const char *p_brief = content_get(key, locale);

    /* Glyph: first 3 chars of localized planet name */
    size_t len = strlen(p_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, p_name, copy);
    r.glyph[copy] = '\0';

    /* Moon phase data */
    const char *ph_name = NULL;
    const char *ph_theme = NULL;
    if (moon_phase >= 0 && moon_phase <= 7) {
        snprintf(key, sizeof(key), "astronomy.phase.%d.name", moon_phase);
        ph_name = content_get(key, locale);
        snprintf(key, sizeof(key), "astronomy.phase.%d.theme", moon_phase);
        ph_theme = content_get(key, locale);
    }

    /* Glance */
    if (is_retrograde) {
        const char *tpl = content_get("astronomy.tpl.glance_retro", locale);
        snprintf(r.glance, sizeof(r.glance), tpl, p_name, p_arch);
    } else if (ph_name != NULL) {
        const char *tpl = content_get("astronomy.tpl.glance_moon", locale);
        snprintf(r.glance, sizeof(r.glance), tpl, p_name, p_arch, ph_name);
    } else {
        const char *tpl = content_get("astronomy.tpl.glance", locale);
        snprintf(r.glance, sizeof(r.glance), tpl, p_name, p_arch);
    }

    /* Detail */
    const char *tpl_planet = content_get("astronomy.tpl.detail_planet",
                                         locale);
    int pos = snprintf(r.detail, sizeof(r.detail), tpl_planet,
                       p_name, p_deity, p_domain, p_brief);

    if (is_retrograde && pos > 0 && (size_t)pos < sizeof(r.detail)) {
        const char *tpl_retro = content_get("astronomy.tpl.detail_retro",
                                            locale);
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        "%s", tpl_retro);
    }

    if (ph_name != NULL && pos > 0 && (size_t)pos < sizeof(r.detail)) {
        const char *tpl_moon = content_get("astronomy.tpl.detail_moon",
                                           locale);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 tpl_moon, ph_name, ph_theme);
    }

    return r;
}
