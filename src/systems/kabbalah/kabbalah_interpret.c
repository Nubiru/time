/* kabbalah_interpret.c — Kabbalah Tree of Life interpretation data
 *
 * Pure data module. All data is static const. No malloc, no globals,
 * no side effects.
 *
 * Sources: Traditional Kabbalistic meditations,
 *          Golden Dawn path attributions (Hebrew letter + Tarot). */

#include "kabbalah_interpret.h"
#include <stdio.h>
#include <string.h>

/* ===== Sefirah names (for glyph + compose) ===== */

static const char *SEFIRAH_NAMES[10] = {
    "Keter", "Chokmah", "Binah", "Chesed", "Gevurah",
    "Tiferet", "Netzach", "Hod", "Yesod", "Malkuth"
};

/* ===== 10 Sefirot interpretation data ===== */

static const ki_sefirah_t SEFIRAH_DATA[10] = {
    { 0,
      "Crown \xe2\x80\x94 the point before thought, pure will",
      "What exists before I think?",
      "The unknowable origin \xe2\x80\x94 divine will before manifestation" },
    { 1,
      "Wisdom \xe2\x80\x94 the flash of insight, the father principle",
      "What is the first spark of creation?",
      "Primal wisdom \xe2\x80\x94 the lightning flash that begins all things" },
    { 2,
      "Understanding \xe2\x80\x94 the womb that gives form to wisdom",
      "How does the infinite become finite?",
      "The great mother \xe2\x80\x94 understanding that shapes raw wisdom into form" },
    { 3,
      "Mercy \xe2\x80\x94 boundless giving, expansion without limit",
      "Where must I give without condition?",
      "Unconditional love \xe2\x80\x94 the outpouring of grace" },
    { 4,
      "Strength \xe2\x80\x94 necessary contraction, judgment, discipline",
      "Where must I set boundaries?",
      "Sacred restriction \xe2\x80\x94 strength that shapes through limitation" },
    { 5,
      "Beauty \xe2\x80\x94 the heart that balances mercy and judgment",
      "How do I harmonize opposites?",
      "The heart center \xe2\x80\x94 beauty born from the marriage of giving and restraint" },
    { 6,
      "Victory \xe2\x80\x94 endurance, artistic passion, desire",
      "What drives me to persist?",
      "The fire of eternity \xe2\x80\x94 passion that endures beyond reason" },
    { 7,
      "Splendor \xe2\x80\x94 intellect, communication, humility",
      "How do I serve the truth?",
      "The glory of surrender \xe2\x80\x94 mind bowing to what is greater" },
    { 8,
      "Foundation \xe2\x80\x94 the gateway between above and below",
      "What connects my inner world to outer reality?",
      "The foundation \xe2\x80\x94 where all upper forces gather before manifesting" },
    { 9,
      "Kingdom \xe2\x80\x94 the physical world, embodiment, presence",
      "Am I fully here?",
      "The kingdom of the present moment \xe2\x80\x94 divinity in matter" }
};

/* ===== 22 path interpretation data ===== */

static const ki_path_t PATH_DATA[22] = {
    { 0,
      "Stepping into the void with perfect trust",
      "Faith requires no ground to stand on" },
    { 1,
      "Channeling divine will into conscious creation",
      "The creator creates by becoming transparent to creation" },
    { 2,
      "The long descent from crown to heart",
      "Wisdom crosses the abyss only through silence" },
    { 3,
      "Wisdom meeting understanding \xe2\x80\x94 father and mother unite",
      "Creation begins where two principles embrace" },
    { 4,
      "Raw insight finding its center",
      "Vision must find a heart or it remains blinding light" },
    { 5,
      "Wisdom flowing into mercy",
      "True teaching is an act of unconditional giving" },
    { 6,
      "Understanding choosing beauty over severity",
      "The first choice: love or fear \xe2\x80\x94 always love" },
    { 7,
      "Form becoming discipline",
      "Structure without compassion becomes a prison; with it, a chariot" },
    { 8,
      "Mercy meeting judgment directly",
      "True strength is mercy that knows when to say no" },
    { 9,
      "Grace descending to the heart",
      "The lamp of mercy illuminates the path inward" },
    { 10,
      "Abundance turning into endurance",
      "Fortune changes but passion outlasts every turning" },
    { 11,
      "Judgment softened by beauty",
      "Balance is not compromise \xe2\x80\x94 it is precision of the heart" },
    { 12,
      "Severity becoming surrender",
      "The deepest discipline is to stop struggling" },
    { 13,
      "Heart releasing into passion",
      "Something must die for desire to become authentic" },
    { 14,
      "Beauty grounding into foundation",
      "Harmony manifests only when it touches the ground" },
    { 15,
      "Heart confronting the shadow",
      "Beauty must include darkness or it becomes a lie" },
    { 16,
      "Passion and intellect shattering illusion",
      "When desire and truth collide, every false structure falls" },
    { 17,
      "Passion entering dreams",
      "The unconscious speaks in the language of what you love" },
    { 18,
      "Desire becoming embodied",
      "Joy is passion that has found a body" },
    { 19,
      "Intellect awakening the foundation",
      "The mind\xe2\x80\x99s final gift: calling the soul to rise" },
    { 20,
      "Understanding completing itself in matter",
      "The scholar\xe2\x80\x99s path ends where the hands begin" },
    { 21,
      "Foundation becoming kingdom",
      "The last step: making the invisible visible" }
};

/* Invalid sentinel */
static const ki_sefirah_t INVALID_SEFIRAH = {
    -1, "?", "?", "?"
};

static const ki_path_t INVALID_PATH = {
    -1, "?", "?"
};

/* ===== Public API ===== */

ki_sefirah_t ki_sefirah_data(int index)
{
    if (index < 0 || index >= 10) {
        return INVALID_SEFIRAH;
    }
    return SEFIRAH_DATA[index];
}

ki_path_t ki_path_data(int index)
{
    if (index < 0 || index >= 22) {
        return INVALID_PATH;
    }
    return PATH_DATA[index];
}

kabbalah_interp_t ki_interpret_sefirah(int index, const char *planet_name)
{
    kabbalah_interp_t result;
    memset(&result, 0, sizeof(result));

    if (index < 0 || index >= 10) {
        snprintf(result.glyph, sizeof(result.glyph), "?");
        snprintf(result.glance, sizeof(result.glance), "?");
        snprintf(result.detail, sizeof(result.detail), "?");
        return result;
    }

    const char *name = SEFIRAH_NAMES[index];
    const ki_sefirah_t *s = &SEFIRAH_DATA[index];
    const char *planet = planet_name ? planet_name : "unassigned";

    /* Glyph: first 3 chars of name */
    snprintf(result.glyph, sizeof(result.glyph), "%.3s", name);

    /* Glance: "{name} — {brief}" */
    snprintf(result.glance, sizeof(result.glance),
             "%s \xe2\x80\x94 %s", name, s->brief);

    /* Detail: "{name}: {meditation}. The question: {question}. Planet: {planet}." */
    snprintf(result.detail, sizeof(result.detail),
             "%s: %s. The question: %s. Planet: %s.",
             name, s->meditation, s->question, planet);

    return result;
}

int ki_sefirah_count(void)
{
    return 10;
}

int ki_path_count(void)
{
    return 22;
}
