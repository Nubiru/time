/* tarot_interpret.c — Tarot interpretation data (Thoth/Crowley tradition)
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "tarot_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Roman numeral helper (0-21)
 * ================================================================ */

static const char *ROMAN[22] = {
    "0", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX",
    "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII",
    "XIX", "XX", "XXI"
};

/* ================================================================
 * Static Major Arcana data (22 cards, Thoth tradition)
 * ================================================================ */

static const trt_major_t MAJORS[22] = {
    {  0, "The Fool",
       "The Divine Child",
       "Potential",
       "Innocence, spontaneity, trust in the unknown, beginner's mind",
       "Foolishness, recklessness, denial of consequences",
       "Pure potential before the first step \xe2\x80\x94 the breath before creation" },

    {  1, "The Magus",
       "The Trickster / Communicator",
       "Skill",
       "Eloquence, dexterity, mastery of tools, transparent communication",
       "Manipulation, deception, using skill without wisdom",
       "The power of the word made conscious \xe2\x80\x94 skill as the bridge between will and world" },

    {  2, "The Priestess",
       "The Anima / Inner Oracle",
       "Intuition",
       "Deep knowing, receptivity, the wisdom of silence and gestation",
       "Withdrawal, secrecy, disconnection from instinct",
       "The moon's reflection on still water \xe2\x80\x94 truth known without being spoken" },

    {  3, "The Empress",
       "The Great Mother",
       "Love",
       "Abundance, nurture, creative fertility, sensual delight in existence",
       "Smothering, overindulgence, creative stagnation",
       "The garden that grows because it is loved \xe2\x80\x94 nature's generosity made personal" },

    {  4, "The Emperor",
       "The Father / Lawgiver",
       "Authority",
       "Structure, vision, protective power, the courage to lead",
       "Tyranny, rigidity, authority without compassion",
       "The mountain that does not move \xe2\x80\x94 order as the foundation of freedom" },

    {  5, "The Hierophant",
       "The Teacher / Bridge",
       "Teaching",
       "Transmission of wisdom, sacred tradition, inner hearing",
       "Dogma, blind obedience, spiritual materialism",
       "The voice that teaches not what to think but how to listen" },

    {  6, "The Lovers",
       "The Syzygy / Sacred Union",
       "Choice",
       "Authentic relationship, integration of opposites, moral discernment",
       "Indecision, projection, loss of self in the other",
       "The sword that divides also unites \xe2\x80\x94 every true choice is a marriage" },

    {  7, "The Chariot",
       "The Hero / Triumphant Will",
       "Victory",
       "Directed will, triumph through alignment, carrying the grail forward",
       "Aggression, empty conquest, force without purpose",
       "The cup held steady in a moving vehicle \xe2\x80\x94 victory through focused will" },

    {  8, "Adjustment",
       "The Judge / Equilibrium",
       "Balance",
       "Perfect equilibrium, karma understood, justice as cosmic harmony",
       "Harsh judgment, imbalance disguised as fairness, cold logic",
       "The feather and the heart on the same scale \xe2\x80\x94 truth weighs nothing" },

    {  9, "The Hermit",
       "The Wise Old Man / Seeker",
       "Wisdom",
       "Solitary illumination, inner guidance, the lantern held for others",
       "Isolation, misanthropy, withholding wisdom out of fear",
       "The lamp that shines brightest in the deepest dark" },

    { 10, "Fortune",
       "The Wheel / Fate",
       "Change",
       "Acceptance of cycles, luck as alignment, the hub of stillness",
       "Attachment to outcomes, gambling, dizzy from change",
       "The wheel turns \xe2\x80\x94 only the center is still" },

    { 11, "Lust",
       "The Animal Soul / Sacred Power",
       "Strength",
       "Raw vitality channeled with joy, passion as spiritual fuel",
       "Domination, lust for power, uncontrolled appetite",
       "The woman rides the beast with delight \xe2\x80\x94 strength is joy, not effort" },

    { 12, "The Hanged Man",
       "The Sacrifice / Suspended One",
       "Surrender",
       "Voluntary sacrifice, seeing from a new angle, the water of redemption",
       "Martyrdom, stagnation, sacrifice without purpose",
       "The world inverts \xe2\x80\x94 what was hidden is revealed by letting go" },

    { 13, "Death",
       "The Transformer",
       "Transformation",
       "Necessary ending, the composting that feeds new life, liberation",
       "Resistance to change, fear of loss, clinging to the dead",
       "The scythe does not judge \xe2\x80\x94 it simply makes room for what comes next" },

    { 14, "Art",
       "The Alchemist",
       "Integration",
       "Creative synthesis, the marriage of opposites, gold from lead",
       "Dilution, half-measures, mixing without transforming",
       "Fire and water in one vessel \xe2\x80\x94 the impossible union that creates something new" },

    { 15, "The Devil",
       "The Shadow / Pan",
       "Bondage",
       "Humor about one's chains, creative mischief, the laughing god",
       "Addiction, materialism, self-deception, blaming external forces",
       "The chains are loose \xe2\x80\x94 the goat laughs because we could leave anytime" },

    { 16, "The Tower",
       "The Lightning Flash",
       "Revelation",
       "Sudden liberation, the destruction that clears for truth, awakening",
       "Catastrophe without growth, destruction for its own sake",
       "Lightning strikes the false structure \xe2\x80\x94 what remains is what was always true" },

    { 17, "The Star",
       "The Anima Mundi / Hope",
       "Hope",
       "Naked truth, meditation, the universe reflecting itself through you",
       "Despair, disconnection from meaning, beautiful emptiness",
       "The water poured on earth and water \xe2\x80\x94 nature healing itself in stillness" },

    { 18, "The Moon",
       "The Guardian of the Threshold",
       "Illusion",
       "Navigating the unconscious, dream wisdom, facing primal fear",
       "Deception, confusion, being lost in fantasy, lunar madness",
       "The path between the towers \xe2\x80\x94 only by walking through illusion do we reach dawn" },

    { 19, "The Sun",
       "The Divine Child Reborn",
       "Joy",
       "Radiance, innocence reclaimed, the warmth that makes all things grow",
       "Burnout, arrogance, blinding others with one's light",
       "The child dances in the garden \xe2\x80\x94 all complexity resolved into simple joy" },

    { 20, "The Aeon",
       "The Awakening / Judgment",
       "Rebirth",
       "Evolutionary leap, answering the call, the fire letter transforms the age",
       "Stagnation, refusal of the call, apocalyptic thinking",
       "The old aeon dissolves \xe2\x80\x94 a new child opens its eyes in a new world" },

    { 21, "The Universe",
       "The World Dancer / Completion",
       "Completion",
       "Wholeness, integration of all paths, the dance of matter and spirit",
       "Feeling trapped in materiality, refusing to begin again",
       "The dancer inside Saturn's ring \xe2\x80\x94 the end that is also the beginning" }
};

/* ================================================================
 * Static suit data (4 suits)
 * ================================================================ */

static const trt_suit_t SUITS[4] = {
    { 0, "Wands", "Fire",
      "Will, creativity, career, spiritual purpose",
      "The spark that initiates \xe2\x80\x94 active, creative, transformative",
      "The suit of will and fire \xe2\x80\x94 what drives you to create and act" },

    { 1, "Cups", "Water",
      "Emotion, relationships, intuition, the unconscious",
      "The vessel that receives \xe2\x80\x94 receptive, flowing, nurturing",
      "The suit of feeling and water \xe2\x80\x94 what moves you beneath the surface" },

    { 2, "Swords", "Air",
      "Mind, conflict, truth, communication, analysis",
      "The blade that divides \xe2\x80\x94 analytical, discriminating, clarifying",
      "The suit of thought and air \xe2\x80\x94 what you think and how you communicate it" },

    { 3, "Disks", "Earth",
      "Body, material world, health, finances, nature",
      "The ground that sustains \xe2\x80\x94 practical, grounding, manifesting",
      "The suit of matter and earth \xe2\x80\x94 what you build and what sustains you" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const trt_major_t INVALID_MAJOR = {
    -1, "?", "?", "?", "?", "?", "?"
};

static const trt_suit_t INVALID_SUIT = {
    -1, "?", "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

trt_major_t trt_major_data(int number)
{
    if (number < 0 || number > 21) return INVALID_MAJOR;
    return MAJORS[number];
}

trt_suit_t trt_suit_data(int suit)
{
    if (suit < 0 || suit > 3) return INVALID_SUIT;
    return SUITS[suit];
}

tarot_interp_t trt_interpret(int major_number, int decan_suit, int decan_rank)
{
    tarot_interp_t r;
    memset(&r, 0, sizeof(r));

    trt_major_t m = trt_major_data(major_number);

    /* Glyph: Roman numeral */
    if (m.number < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
    } else {
        snprintf(r.glyph, sizeof(r.glyph), "%s", ROMAN[major_number]);
    }

    /* Glance */
    if (m.number < 0) {
        snprintf(r.glance, sizeof(r.glance), "?");
    } else if (decan_suit >= 0 && decan_suit <= 3 && decan_rank >= 2 && decan_rank <= 10) {
        trt_suit_t s = trt_suit_data(decan_suit);
        snprintf(r.glance, sizeof(r.glance),
                 "%s \xe2\x80\x94 %s + %d of %s (%s)",
                 m.name, m.keyword, decan_rank, s.name, s.element);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s \xe2\x80\x94 %s",
                 m.name, m.keyword);
    }

    /* Detail: full narrative */
    if (m.number < 0) {
        snprintf(r.detail, sizeof(r.detail), "?");
    } else {
        int pos = 0;
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        "%s (%s): %s. Archetype: %s. "
                        "Light: %s. Shadow: %s.",
                        ROMAN[major_number], m.name, m.keyword, m.archetype,
                        m.light, m.shadow);

        if (decan_suit >= 0 && decan_suit <= 3 && decan_rank >= 2 && decan_rank <= 10) {
            trt_suit_t s = trt_suit_data(decan_suit);
            snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                     " Decan: %d of %s (%s). Domain: %s. %s",
                     decan_rank, s.name, s.element, s.domain, s.quality);
        }
    }

    return r;
}

int trt_major_count(void)
{
    return 22;
}

int trt_suit_count(void)
{
    return 4;
}
