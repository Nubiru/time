/* chakra_interpret.c — Chakra system interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "chakra_interpret.h"
#include "chakra.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

static const chi_chakra_t CHAKRAS[7] = {
    { 0, "Muladhara", "Root",
      "The Foundation",
      "Grounding, safety, survival, physical vitality, belonging to the earth",
      "Fear, anxiety, instability, hoarding, disconnection from the body",
      "Am I safe? Do I belong here?",
      "The root \xe2\x80\x94 LAM \xe2\x80\x94 the foundation upon which everything else is built" },

    { 1, "Svadhisthana", "Sacral",
      "The Creator",
      "Pleasure, creativity, emotional flow, sensuality, healthy desire",
      "Guilt, emotional numbness, addiction, creative block, boundary issues",
      "What do I feel? What do I desire?",
      "The sacral \xe2\x80\x94 VAM \xe2\x80\x94 the water center where emotion and creation flow" },

    { 2, "Manipura", "Solar Plexus",
      "The Warrior",
      "Personal power, will, confidence, self-discipline, inner fire",
      "Shame, powerlessness, domination, anger, digestive distress",
      "Who am I? What is my purpose?",
      "The solar plexus \xe2\x80\x94 RAM \xe2\x80\x94 the fire of personal will and self-determination" },

    { 3, "Anahata", "Heart",
      "The Healer",
      "Unconditional love, compassion, forgiveness, connection, balance",
      "Grief, jealousy, codependency, isolation, inability to forgive",
      "Do I love? Am I loved?",
      "The heart \xe2\x80\x94 YAM \xe2\x80\x94 the unstruck sound, where human love meets the divine" },

    { 4, "Vishuddha", "Throat",
      "The Communicator",
      "Authentic expression, truth, creativity through voice, listening",
      "Lies, silence, over-talking, fear of judgment, creative suppression",
      "What is my truth? Can I speak it?",
      "The throat \xe2\x80\x94 HAM \xe2\x80\x94 the purification center where inner truth becomes outer expression" },

    { 5, "Ajna", "Third Eye",
      "The Seer",
      "Intuition, insight, imagination, clarity, inner vision, wisdom",
      "Illusion, delusion, inability to see patterns, headaches, rigidity",
      "What do I see beyond the visible?",
      "The third eye \xe2\x80\x94 OM \xe2\x80\x94 the seat of intuition where duality dissolves into seeing" },

    { 6, "Sahasrara", "Crown",
      "The Sage",
      "Transcendence, unity, spiritual connection, pure consciousness",
      "Spiritual bypassing, disconnection from embodiment, nihilism, isolation",
      "Am I connected to something greater?",
      "The crown \xe2\x80\x94 silence \xe2\x80\x94 the thousand-petaled lotus where self dissolves into the infinite" }
};

static const chi_chakra_t INVALID = {
    -1, "?", "?", "?", "?", "?", "?", "?"
};

chi_chakra_t chi_chakra_data(int chakra)
{
    if (chakra < 0 || chakra > 6) return INVALID;
    return CHAKRAS[chakra];
}

chakra_interp_t chi_interpret(int chakra)
{
    chakra_interp_t r;
    memset(&r, 0, sizeof(r));

    chi_chakra_t c = chi_chakra_data(chakra);
    if (c.chakra < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: bija mantra from base module */
    const char *bija = chakra_bija((chakra_t)chakra);
    snprintf(r.glyph, sizeof(r.glyph), "%s", bija);

    /* Glance */
    snprintf(r.glance, sizeof(r.glance),
             "%s (%s) \xe2\x80\x94 %s",
             c.sanskrit, c.english, c.theme);

    /* Detail */
    snprintf(r.detail, sizeof(r.detail),
             "Chakra: %s (%s). %s. "
             "Theme: %s. Shadow: %s. "
             "Question: %s",
             c.sanskrit, c.english, c.archetype,
             c.theme, c.shadow, c.question);

    return r;
}

int chi_chakra_count(void) { return 7; }

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

chakra_interp_t chi_interpret_locale(int chakra, i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return chi_interpret(chakra);
    }

    chakra_interp_t r;
    memset(&r, 0, sizeof(r));

    if (chakra < 0 || chakra > 6) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    char key[64];

    /* Fetch translated fields */
    snprintf(key, sizeof(key), "chakra.%d.sanskrit", chakra);
    const char *sanskrit = content_get(key, locale);

    snprintf(key, sizeof(key), "chakra.%d.english", chakra);
    const char *english = content_get(key, locale);

    snprintf(key, sizeof(key), "chakra.%d.theme", chakra);
    const char *theme = content_get(key, locale);

    snprintf(key, sizeof(key), "chakra.%d.archetype", chakra);
    const char *archetype_str = content_get(key, locale);

    snprintf(key, sizeof(key), "chakra.%d.shadow", chakra);
    const char *shadow = content_get(key, locale);

    snprintf(key, sizeof(key), "chakra.%d.question", chakra);
    const char *question = content_get(key, locale);

    /* T0: Glyph — bija mantra is universal (Sanskrit), no translation */
    const char *bija = chakra_bija((chakra_t)chakra);
    snprintf(r.glyph, sizeof(r.glyph), "%s", bija);

    /* T1: Glance */
    const char *tpl_glance = content_get("chakra.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             sanskrit, english, theme);

    /* T3: Detail */
    const char *tpl_detail = content_get("chakra.tpl.detail", locale);
    snprintf(r.detail, sizeof(r.detail), tpl_detail,
             sanskrit, english, archetype_str,
             theme, shadow, question);

    return r;
}
