/* numerology_interpret.c — Numerology interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "numerology_interpret.h"
#include <stdio.h>
#include <string.h>

static const ni_number_t NUMBERS[12] = {
    { 1, "The Pioneer",
      "Independence, originality, leadership, initiative",
      "Self-reliance, courage, innovation, the ability to start from nothing",
      "Isolation, selfishness, aggression, inability to cooperate",
      "Number 1 \xe2\x80\x94 the spark of creation, the individual who walks first into the unknown" },

    { 2, "The Peacemaker",
      "Cooperation, sensitivity, diplomacy, balance",
      "Empathy, patience, mediation, the gift of seeing both sides",
      "Codependency, over-sensitivity, indecision, self-erasure",
      "Number 2 \xe2\x80\x94 the bridge between opposites, harmony through understanding" },

    { 3, "The Creator",
      "Expression, joy, creativity, communication",
      "Artistic talent, optimism, charm, the ability to inspire",
      "Scattered energy, superficiality, gossip, emotional avoidance through humor",
      "Number 3 \xe2\x80\x94 the child of 1 and 2, joy made manifest through creative expression" },

    { 4, "The Builder",
      "Structure, stability, discipline, hard work",
      "Reliability, endurance, practical skill, creating lasting foundations",
      "Rigidity, workaholism, narrow-mindedness, resistance to change",
      "Number 4 \xe2\x80\x94 the four walls, the four seasons, order imposed on chaos" },

    { 5, "The Adventurer",
      "Freedom, change, versatility, sensory experience",
      "Adaptability, resourcefulness, magnetism, love of the new",
      "Restlessness, addiction, irresponsibility, fear of commitment",
      "Number 5 \xe2\x80\x94 the five senses alive, the human who needs to taste everything" },

    { 6, "The Nurturer",
      "Responsibility, love, service, domestic harmony",
      "Compassion, healing, artistic beauty, devotion to family",
      "Control, martyrdom, perfectionism, meddling in others' lives",
      "Number 6 \xe2\x80\x94 the hexagram of balance, love expressed through responsibility" },

    { 7, "The Seeker",
      "Analysis, introspection, spiritual depth, wisdom",
      "Profound thought, research ability, intuition, inner knowing",
      "Isolation, cynicism, secretiveness, intellectual arrogance",
      "Number 7 \xe2\x80\x94 the sacred number, the mind that questions until it touches mystery" },

    { 8, "The Powerhouse",
      "Authority, abundance, manifestation, karmic balance",
      "Business acumen, leadership, material mastery, the courage to wield power",
      "Greed, domination, workaholism, abuse of authority",
      "Number 8 \xe2\x80\x94 the infinity loop, as above so below, material power as spiritual test" },

    { 9, "The Humanitarian",
      "Completion, wisdom, compassion, universal love",
      "Selflessness, artistic vision, global awareness, the ability to let go",
      "Martyrdom, resentment, emotional distance, difficulty receiving",
      "Number 9 \xe2\x80\x94 the final digit, completion that opens into new beginning" },

    { 11, "The Illuminator",
      "Spiritual insight, inspiration, visionary leadership, heightened intuition",
      "Channel for higher wisdom, catalytic presence, inspiring others",
      "Anxiety, nervous tension, impracticality, unrealized potential",
      "Master 11 \xe2\x80\x94 the double pillar, a gateway between the mundane and the divine" },

    { 22, "The Master Builder",
      "Turning vision into reality, large-scale manifestation, practical idealism",
      "Ability to build lasting institutions, global vision with practical skill",
      "Overwhelm, self-doubt, giving up on the dream, settling for 4",
      "Master 22 \xe2\x80\x94 the architect of the impossible, vision grounded in brick and mortar" },

    { 33, "The Master Teacher",
      "Selfless service, healing through love, spiritual upliftment",
      "Compassion without limit, ability to heal by presence alone",
      "Martyrdom, self-sacrifice to the point of destruction, cosmic guilt",
      "Master 33 \xe2\x80\x94 the Christ number, love so vast it transforms everything it touches" }
};

static const ni_number_t INVALID = {
    -1, "?", "?", "?", "?", "?"
};

/* Lookup helper: find index into NUMBERS array */
static int number_index(int number)
{
    if (number >= 1 && number <= 9) return number - 1;
    if (number == 11) return 9;
    if (number == 22) return 10;
    if (number == 33) return 11;
    return -1;
}

ni_number_t ni_number_data(int number)
{
    int idx = number_index(number);
    if (idx < 0) return INVALID;
    return NUMBERS[idx];
}

numerology_interp_t ni_interpret(int life_path, int personal_year)
{
    numerology_interp_t r;
    memset(&r, 0, sizeof(r));

    ni_number_t lp = ni_number_data(life_path);
    if (lp.number < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: the number itself */
    snprintf(r.glyph, sizeof(r.glyph), "%d", life_path);

    /* Glance */
    if (personal_year > 0) {
        ni_number_t py = ni_number_data(personal_year);
        if (py.number > 0) {
            snprintf(r.glance, sizeof(r.glance),
                     "Life Path %d (%s) \xe2\x80\x94 Year %d (%s)",
                     life_path, lp.name, personal_year, py.name);
        } else {
            snprintf(r.glance, sizeof(r.glance),
                     "Life Path %d \xe2\x80\x94 %s",
                     life_path, lp.name);
        }
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "Life Path %d \xe2\x80\x94 %s",
                 life_path, lp.name);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Life Path %d: %s. %s. "
                    "Strength: %s. Challenge: %s.",
                    life_path, lp.name, lp.quality,
                    lp.strength, lp.challenge);

    if (personal_year > 0) {
        ni_number_t py = ni_number_data(personal_year);
        if (py.number > 0) {
            snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                     " Personal Year %d: %s \xe2\x80\x94 %s.",
                     personal_year, py.name, py.quality);
        }
    }

    return r;
}

int ni_number_count(void) { return 12; }
