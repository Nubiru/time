#include "tzolkin_interpret.h"
#include "dreamspell.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/*
 * Dreamspell Interpretation Engine — Arguelles system.
 *
 * Composes archetype data with oracle/wavespell/castle/color/tone
 * relationships into tiered narrative text.
 *
 * All 20 seal archetypes from:
 *   Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 */

/* --- Bare seal names (without color prefix) --- */

static const char *SEAL_NAMES[20] = {
    "Dragon", "Wind", "Night", "Seed", "Serpent",
    "World-Bridger", "Hand", "Star", "Moon", "Dog",
    "Monkey", "Human", "Skywalker", "Wizard", "Eagle",
    "Warrior", "Earth", "Mirror", "Storm", "Sun"
};

/* --- Seal archetype data (20 seals, Arguelles Dreamspell) --- */

static const ti_seal_t SEAL_DATA[20] = {
    {  0, "Primal nurturance, the mother of creation",
          "What do I need to nurture into being?",
          "Birth", "Nurtures", "Being" },
    {  1, "Divine breath, the messenger of spirit",
          "What truth must I communicate?",
          "Spirit", "Communicates", "Breath" },
    {  2, "The dreamer, keeper of the void and abundance",
          "What dreams do I hold in the darkness?",
          "Abundance", "Dreams", "Intuition" },
    {  3, "Flowering awareness, the potential of new life",
          "What am I ready to plant?",
          "Flowering", "Targets", "Awareness" },
    {  4, "Life force, kundalini, the body's wisdom",
          "What instinct am I ignoring?",
          "Life Force", "Survives", "Instinct" },
    {  5, "Death and transformation, the bridge between worlds",
          "What must I release to cross over?",
          "Death", "Equalizes", "Opportunity" },
    {  6, "Healing, accomplishment, knowledge through doing",
          "What can I heal through action?",
          "Accomplishment", "Knows", "Healing" },
    {  7, "Beauty, art, elegance, the aesthetic principle",
          "Where must I bring beauty?",
          "Art", "Beautifies", "Elegance" },
    {  8, "Purification, flow, the universal water of feeling",
          "What feelings need to flow?",
          "Universal Water", "Purifies", "Flow" },
    {  9, "Heart, love, loyalty, unconditional devotion",
          "Who do I love without condition?",
          "Heart", "Loves", "Loyalty" },
    { 10, "Magic, play, illusion, the divine trickster",
          "Where am I too serious?",
          "Magic", "Plays", "Illusion" },
    { 11, "Free will, wisdom, the power of choice",
          "What choices define my path?",
          "Free Will", "Influences", "Wisdom" },
    { 12, "Space, exploration, wakefulness between worlds",
          "What new territory calls me?",
          "Space", "Explores", "Wakefulness" },
    { 13, "Timelessness, enchantment, receptive magic",
          "What appears when I stop forcing?",
          "Timelessness", "Enchants", "Receptivity" },
    { 14, "Vision, the mind's eye, seeing the whole pattern",
          "What can I see from above?",
          "Vision", "Creates", "Mind" },
    { 15, "Intelligence, fearlessness, questioning everything",
          "What question am I afraid to ask?",
          "Intelligence", "Questions", "Fearlessness" },
    { 16, "Navigation, synchronicity, the Earth's heartbeat",
          "What does the Earth want from me?",
          "Navigation", "Evolves", "Synchronicity" },
    { 17, "Endlessness, reflection, the hall of mirrors",
          "What am I reflecting back to others?",
          "Endlessness", "Reflects", "Order" },
    { 18, "Self-generation, transformation, catalytic energy",
          "What transformation am I resisting?",
          "Self-Generation", "Catalyzes", "Energy" },
    { 19, "Universal fire, enlightenment, wholeness, completion",
          "What must I illuminate?",
          "Universal Fire", "Enlightens", "Life" }
};

/* --- Color meaning strings --- */

static const char *COLOR_MEANINGS[4] = {
    "Initiates — the power of birth",
    "Refines — the power of death",
    "Transforms — the power of magic",
    "Ripens — the power of intelligence"
};

/* --- Tone meaning strings --- */

static const char *TONE_MEANINGS[13] = {
    "Magnetic — Unify Purpose",
    "Lunar — Polarize Challenge",
    "Electric — Activate Service",
    "Self-Existing — Define Form",
    "Overtone — Empower Radiance",
    "Rhythmic — Organize Equality",
    "Resonant — Channel Attunement",
    "Galactic — Harmonize Integrity",
    "Solar — Pulse Intention",
    "Planetary — Perfect Manifestation",
    "Spectral — Dissolve Liberation",
    "Crystal — Dedicate Cooperation",
    "Cosmic — Endure Transcendence"
};

/* --- Invalid result --- */

static const ti_seal_t INVALID_SEAL = {
    -1, "?", "?", "?", "?", "?"
};

/* --- Public API --- */

ti_seal_t ti_seal_data(int seal)
{
    if (seal < 0 || seal > 19)
        return INVALID_SEAL;
    return SEAL_DATA[seal];
}

int ti_seal_count(void)
{
    return 20;
}

const char *ti_color_meaning(int color)
{
    if (color < 0 || color > 3)
        return "?";
    return COLOR_MEANINGS[color];
}

const char *ti_tone_meaning(int tone)
{
    if (tone < 1 || tone > 13)
        return "?";
    return TONE_MEANINGS[tone - 1];
}

tzolkin_interp_t ti_interpret(int kin, int seal, int tone)
{
    tzolkin_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Validate inputs */
    if (kin < 1 || kin > 260 || seal < 0 || seal > 19 || tone < 1 || tone > 13) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Gather data from dreamspell engine */
    dreamspell_color_t color = dreamspell_color(seal);
    dreamspell_tone_t dt = dreamspell_tone(tone);
    dreamspell_oracle_t oracle = dreamspell_oracle(kin);
    dreamspell_wavespell_t ws = dreamspell_wavespell(kin);
    dreamspell_castle_t castle = dreamspell_castle(kin);
    ti_seal_t sd = SEAL_DATA[seal];

    const char *seal_name = SEAL_NAMES[seal];

    /* T0: Glyph */
    snprintf(r.glyph, sizeof(r.glyph), "KIN %d", kin);

    /* T1: Glance — "Kin {kin} {color} {tone_name} {seal_name} — {tone_action} {seal_power}" */
    snprintf(r.glance, sizeof(r.glance),
             "Kin %d %s %s %s — %s %s",
             kin, color.name, dt.name, seal_name,
             dt.action, sd.power);

    /* T3: Detail — full narrative */
    snprintf(r.detail, sizeof(r.detail),
             "%s %s %s: %s. "
             "Tone %d: %s — %s the power of %s. "
             "Seal: %s — %s "
             "Wavespell %d: %s wavespell — %s, %s. "
             "Castle: %s — %s. "
             "Oracle guide: Kin %d. Analog: Kin %d. Antipode: Kin %d. Occult: Kin %d.",
             color.name, dt.name, seal_name, sd.archetype,
             tone, dt.name, dt.action, dt.power,
             seal_name, sd.question,
             ws.number, ws.purpose, ws.action, ws.power,
             castle.name, castle.theme,
             oracle.guide, oracle.analog, oracle.antipode, oracle.occult);

    return r;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

tzolkin_interp_t ti_interpret_locale(int kin, int seal, int tone,
                                     i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return ti_interpret(kin, seal, tone);
    }

    tzolkin_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Validate inputs */
    if (kin < 1 || kin > 260 || seal < 0 || seal > 19 || tone < 1 || tone > 13) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    char key[64];

    /* Seal data */
    snprintf(key, sizeof(key), "tzolkin.seal.%d.name", seal);
    const char *seal_name = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.seal.%d.archetype", seal);
    const char *archetype = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.seal.%d.question", seal);
    const char *question = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.seal.%d.power", seal);
    const char *seal_power = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.seal.%d.action", seal);
    const char *seal_action = content_get(key, locale);
    (void)seal_action;  /* used indirectly via wavespell */

    /* Color data (seal % 4) */
    int color_idx = seal % 4;
    snprintf(key, sizeof(key), "tzolkin.color.%d.name", color_idx);
    const char *color_name = content_get(key, locale);

    /* Tone data */
    snprintf(key, sizeof(key), "tzolkin.tone.%d.name", tone);
    const char *tone_name = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.tone.%d.action", tone);
    const char *tone_action = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.tone.%d.power", tone);
    const char *tone_power = content_get(key, locale);

    /* Oracle — kin numbers, no translation needed */
    dreamspell_oracle_t oracle = dreamspell_oracle(kin);

    /* Wavespell data */
    dreamspell_wavespell_t ws = dreamspell_wavespell(kin);

    snprintf(key, sizeof(key), "tzolkin.seal.%d.name", ws.seal);
    const char *ws_purpose = content_get(key, locale);

    int ws_color_idx = ws.seal % 4;
    snprintf(key, sizeof(key), "tzolkin.color.%d.action", ws_color_idx);
    const char *ws_action = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.color.%d.quality", ws_color_idx);
    const char *ws_power = content_get(key, locale);

    /* Castle data */
    dreamspell_castle_t castle = dreamspell_castle(kin);

    snprintf(key, sizeof(key), "tzolkin.castle.%d.name", castle.number);
    const char *castle_name = content_get(key, locale);

    snprintf(key, sizeof(key), "tzolkin.castle.%d.theme", castle.number);
    const char *castle_theme = content_get(key, locale);

    /* T0: Glyph — same format, no translation */
    snprintf(r.glyph, sizeof(r.glyph), "KIN %d", kin);

    /* T1: Glance */
    const char *tpl_glance = content_get("tzolkin.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             kin, color_name, tone_name, seal_name,
             tone_action, seal_power);

    /* T3: Detail */
    const char *tpl_detail = content_get("tzolkin.tpl.detail", locale);
    snprintf(r.detail, sizeof(r.detail), tpl_detail,
             color_name, tone_name, seal_name, archetype,
             tone, tone_name, tone_action, tone_power,
             seal_name, question,
             ws.number, ws_purpose, ws_action, ws_power,
             castle_name, castle_theme,
             oracle.guide, oracle.analog, oracle.antipode, oracle.occult);

    return r;
}
