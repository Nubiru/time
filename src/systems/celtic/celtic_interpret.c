/* celtic_interpret.c — Celtic calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "celtic_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static tree data (14 entries: intercalary Yew + 13 months)
 * ================================================================ */

static const cti_tree_t TREES[14] = {
    {  0, "Yew", "Idho",
       "The Gateway Between Worlds",
       "Ancient wisdom, transformation, transcendence of death and rebirth",
       "Endurance, regeneration, connection to ancestors",
       "Morbidity, stagnation, fear of change",
       "The oldest tree \xe2\x80\x94 the single intercalary day when time itself holds its breath between years" },

    {  1, "Birch", "Beith",
       "The Pioneer",
       "Initiative, new beginnings, courage to venture into the unknown",
       "Ambition, drive, resilience, purity",
       "Impatience, restlessness, intolerance of delay",
       "First tree to colonize bare ground \xe2\x80\x94 the courage to begin again in the darkest season" },

    {  2, "Rowan", "Luis",
       "The Protector",
       "Vision, intuition, protection against enchantment and illusion",
       "Discernment, psychic sensitivity, courage against deception",
       "Isolation, paranoia, over-vigilance",
       "The warding tree \xe2\x80\x94 planted at thresholds to guard against false magic and confusion" },

    {  3, "Ash", "Nion",
       "The World Tree",
       "Connection between inner and outer worlds, higher perspective",
       "Sensitivity, artistic expression, spiritual vision",
       "Moody, withdrawn, lost in inner worlds",
       "The cosmic axis \xe2\x80\x94 Yggdrasil's Celtic cousin, roots in water and crown in sky" },

    {  4, "Alder", "Fearn",
       "The Trailblazer",
       "Determination, confidence, willingness to make difficult choices",
       "Leadership, emotional courage, bridge-building",
       "Vanity, confrontation, burning bridges",
       "The tree of fire and water \xe2\x80\x94 it bleeds red when cut, thrives where others drown" },

    {  5, "Willow", "Saille",
       "The Enchantress",
       "Deep emotion, intuition, the power of the feminine and lunar cycles",
       "Flexibility, empathy, healing, dream wisdom",
       "Over-emotion, melancholy, passive aggression",
       "The moon tree \xe2\x80\x94 bends without breaking, drinks from the deepest water" },

    {  6, "Hawthorn", "Huath",
       "The Guardian of Mystery",
       "Paradox, restraint, the coexistence of beauty and thorns",
       "Patience, adaptability, inner strength under a gentle exterior",
       "Repression, explosive anger, hidden cruelty",
       "The fairy tree \xe2\x80\x94 sacred and untouchable, its beauty protected by thorns" },

    {  7, "Oak", "Duir",
       "The Sovereign",
       "Strength, endurance, generosity, protection of the vulnerable",
       "Stability, justice, hospitality, deep rootedness",
       "Stubbornness, over-responsibility, rigidity",
       "The king of trees \xe2\x80\x94 its door (duir) opens to the light half of the year at summer solstice" },

    {  8, "Holly", "Tinne",
       "The Warrior",
       "Challenge, directness, the courage to fight for what matters",
       "Physical energy, decisiveness, protective ferocity",
       "Aggression, competitiveness, inability to yield",
       "The dark twin of Oak \xe2\x80\x94 rules the waning year with fierce, evergreen determination" },

    {  9, "Hazel", "Coll",
       "The Wise One",
       "Wisdom, inspiration, the flash of poetic insight",
       "Intuition, knowledge, creative brilliance, concentration",
       "Pedantry, intellectualizing emotion, know-it-all tendency",
       "The salmon ate the hazelnuts of wisdom \xe2\x80\x94 the tree of concentrated knowing" },

    { 10, "Vine", "Muin",
       "The Equalizer",
       "Harvest of truth, the intoxication that reveals what is hidden",
       "Honesty, prophetic vision, ability to see all sides",
       "Indecision, excess, loss of boundaries",
       "In vino veritas \xe2\x80\x94 the vine strips pretense and reveals the authentic self" },

    { 11, "Ivy", "Gort",
       "The Survivor",
       "Tenacity, the ability to thrive in darkness and adversity",
       "Persistence, loyalty, finding light in any condition",
       "Parasitism, dependency, smothering attachment",
       "Grows toward light through any obstacle \xe2\x80\x94 the spiral of persistent life" },

    { 12, "Reed", "Ngetal",
       "The Seeker",
       "Depth of inquiry, fearless exploration of hidden truths",
       "Courage, directness, ability to penetrate to the heart of a matter",
       "Jealousy, manipulation, use of knowledge as weapon",
       "The arrow reed \xe2\x80\x94 shoots straight into the depths to find what lies beneath" },

    { 13, "Elder", "Ruis",
       "The Witch Tree",
       "Endings, transformation, the wisdom of completion and release",
       "Renewal, healing, acceptance of cycles, deep compassion",
       "Self-pity, martyrdom, fear of endings",
       "The tree of the Cailleach \xe2\x80\x94 last month before rebirth, the elder who has seen everything" }
};

/* ================================================================
 * Static festival data (8 festivals)
 * ================================================================ */

static const cti_festival_t FESTIVALS[8] = {
    { 0, "Samhain",
      "Death, ancestors, the thinning of the veil",
      "Honoring the dead, divination, releasing what has died",
      "What must I let die so that new life can come?",
      "The Celtic new year \xe2\x80\x94 when the veil between worlds is thinnest and the dead walk among the living" },

    { 1, "Yule",
      "Rebirth of light, hope in the darkest moment",
      "Vigil through the longest night, lighting the Yule log, welcoming the returning sun",
      "What spark of hope survives in my deepest darkness?",
      "The winter solstice \xe2\x80\x94 the longest night gives birth to the returning light" },

    { 2, "Imbolc",
      "Purification, first stirrings of spring, dedication",
      "Brigid's cross weaving, candle lighting, blessing of seeds",
      "What is quickening within me that needs tending?",
      "Brigid's fire \xe2\x80\x94 the first warmth that melts the frozen ground of winter" },

    { 3, "Ostara",
      "Balance, renewal, equal light and darkness",
      "Planting seeds, spring cleaning, celebrating fertility and growth",
      "Where in my life do I need to restore balance?",
      "The spring equinox \xe2\x80\x94 day and night in perfect balance, the world tips toward light" },

    { 4, "Beltane",
      "Fertility, passion, the sacred union of earth and sky",
      "Maypole dancing, jumping the Beltane fire, greenwood marriage",
      "What am I passionate about creating?",
      "The fire festival of union \xe2\x80\x94 when the earth and sky consummate the year's promise" },

    { 5, "Litha",
      "Abundance, power at its peak, the turn toward darkness",
      "Gathering herbs at peak potency, bonfires, honoring the Oak King",
      "What am I celebrating, and what begins to wane?",
      "The summer solstice \xe2\x80\x94 the longest day, when power peaks and the Holly King begins his reign" },

    { 6, "Lughnasadh",
      "First harvest, sacrifice, gratitude for abundance",
      "Grain harvest, bread baking, athletic games, craft fairs",
      "What have I harvested this year, and what sacrifice made it possible?",
      "Lugh's assembly \xe2\x80\x94 the first grain cut, honoring the god who sacrificed for the harvest" },

    { 7, "Mabon",
      "Second harvest, gratitude, preparation for darkness",
      "Wine making, feast of thanksgiving, equal sharing of the harvest",
      "What am I grateful for as the light fades?",
      "The autumn equinox \xe2\x80\x94 day and night equal again, the descent into the dark half begins" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const cti_tree_t INVALID_TREE = {
    -1, "?", "?", "?", "?", "?", "?", "?"
};

static const cti_festival_t INVALID_FESTIVAL = {
    -1, "?", "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

cti_tree_t cti_tree_data(int month)
{
    if (month < 0 || month > 13) return INVALID_TREE;
    return TREES[month];
}

cti_festival_t cti_festival_data(int festival)
{
    if (festival < 0 || festival > 7) return INVALID_FESTIVAL;
    return FESTIVALS[festival];
}

celtic_interp_t cti_interpret(int tree_month, int tree_day, int festival)
{
    celtic_interp_t r;
    memset(&r, 0, sizeof(r));

    cti_tree_t t = cti_tree_data(tree_month);

    /* Glyph: Ogham letter(s) */
    if (t.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
    } else {
        snprintf(r.glyph, sizeof(r.glyph), "%s", t.ogham);
    }

    /* Glance: "{Tree} — {archetype}" or with festival */
    if (t.month < 0) {
        snprintf(r.glance, sizeof(r.glance), "?");
    } else if (festival >= 0 && festival <= 7) {
        cti_festival_t f = cti_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "%s (%s) day %d \xe2\x80\x94 %s: %s",
                 t.tree, t.ogham, tree_day, f.name, f.theme);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s (%s) day %d \xe2\x80\x94 %s",
                 t.tree, t.ogham, tree_day, t.archetype);
    }

    /* Detail: full narrative */
    if (t.month < 0) {
        snprintf(r.detail, sizeof(r.detail), "?");
    } else {
        int pos = 0;
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        "Tree: %s (%s). %s. "
                        "Personality: %s. Strengths: %s. Challenges: %s.",
                        t.tree, t.ogham, t.archetype,
                        t.personality, t.strengths, t.challenges);

        if (festival >= 0 && festival <= 7) {
            cti_festival_t f = cti_festival_data(festival);
            snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                     " Festival: %s \xe2\x80\x94 %s. Practice: %s. "
                     "Reflection: %s",
                     f.name, f.theme, f.practice, f.reflection);
        }
    }

    return r;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

celtic_interp_t cti_interpret_locale(int tree_month, int tree_day,
                                     int festival,
                                     i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return cti_interpret(tree_month, tree_day, festival);
    }

    celtic_interp_t r;
    memset(&r, 0, sizeof(r));

    char key[64];

    /* Tree data lookups */
    snprintf(key, sizeof(key), "celtic.tree.%d.tree", tree_month);
    const char *tree = content_get(key, locale);

    snprintf(key, sizeof(key), "celtic.tree.%d.ogham", tree_month);
    const char *ogham = content_get(key, locale);

    snprintf(key, sizeof(key), "celtic.tree.%d.archetype", tree_month);
    const char *archetype = content_get(key, locale);

    snprintf(key, sizeof(key), "celtic.tree.%d.personality", tree_month);
    const char *personality = content_get(key, locale);

    snprintf(key, sizeof(key), "celtic.tree.%d.strengths", tree_month);
    const char *strengths = content_get(key, locale);

    snprintf(key, sizeof(key), "celtic.tree.%d.challenges", tree_month);
    const char *challenges = content_get(key, locale);

    /* T0: glyph — Ogham letter(s) */
    snprintf(r.glyph, sizeof(r.glyph), "%s", ogham);

    /* T1: glance */
    const char *tpl_glance = content_get("celtic.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             tree, ogham, tree_day, archetype);

    /* T3: detail — base tree narrative */
    const char *tpl_detail = content_get("celtic.tpl.detail", locale);
    int pos = snprintf(r.detail, sizeof(r.detail), tpl_detail,
                       tree, ogham, archetype,
                       personality, strengths, challenges);

    /* Append festival data if active */
    if (festival >= 0 && festival <= 7 &&
        pos > 0 && (size_t)pos < sizeof(r.detail)) {
        snprintf(key, sizeof(key), "celtic.festival.%d.name", festival);
        const char *fest_name = content_get(key, locale);

        snprintf(key, sizeof(key), "celtic.festival.%d.theme", festival);
        const char *fest_theme = content_get(key, locale);

        snprintf(key, sizeof(key), "celtic.festival.%d.practice", festival);
        const char *fest_practice = content_get(key, locale);

        const char *tpl_fest = content_get("celtic.tpl.detail_festival", locale);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 tpl_fest, fest_name, fest_theme, fest_practice);
    }

    return r;
}

int cti_tree_count(void)
{
    return 14;
}

int cti_festival_count(void)
{
    return 8;
}
