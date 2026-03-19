#include "chinese_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static animal data (12 animals)
 * ================================================================ */

static const ci_animal_t ANIMALS[12] = {
    {  0, "Rat",     "The Strategist",  "Resourceful, quick-witted, adaptable",
       "Cunning, restless, manipulative",    "Dragon, Monkey, Ox",
       "First in the cycle \xe2\x80\x94 the clever mind that finds opportunity in chaos" },
    {  1, "Ox",      "The Steadfast",   "Patient, reliable, methodical",
       "Stubborn, rigid, slow to change",    "Rat, Snake, Rooster",
       "The strength that plows through \xe2\x80\x94 patient endurance conquers all" },
    {  2, "Tiger",   "The Brave",       "Courageous, passionate, confident",
       "Reckless, volatile, domineering",    "Horse, Dog, Pig",
       "The roar that awakens \xe2\x80\x94 fearless action in service of justice" },
    {  3, "Rabbit",  "The Diplomat",    "Elegant, gentle, compassionate",
       "Timid, superficial, escapist",       "Goat, Pig, Dog",
       "The grace that disarms \xe2\x80\x94 softness as a form of power" },
    {  4, "Dragon",  "The Emperor",     "Charismatic, ambitious, visionary",
       "Arrogant, demanding, impulsive",     "Rat, Monkey, Rooster",
       "The only mythical animal \xe2\x80\x94 the dream of transcendence made manifest" },
    {  5, "Snake",   "The Philosopher", "Wise, intuitive, graceful",
       "Secretive, suspicious, possessive",  "Ox, Rooster, Dragon",
       "The coil that strikes true \xe2\x80\x94 deep wisdom beneath a still surface" },
    {  6, "Horse",   "The Free Spirit", "Energetic, independent, adventurous",
       "Impatient, irresponsible, anxious",  "Tiger, Goat, Dog",
       "The gallop that liberates \xe2\x80\x94 freedom as a way of being" },
    {  7, "Goat",    "The Artist",      "Creative, gentle, empathetic",
       "Indecisive, pessimistic, dependent", "Rabbit, Horse, Pig",
       "The heart that creates \xe2\x80\x94 beauty born from sensitivity" },
    {  8, "Monkey",  "The Inventor",    "Clever, playful, versatile",
       "Tricky, arrogant, unreliable",       "Rat, Dragon, Snake",
       "The mind that plays \xe2\x80\x94 genius disguised as mischief" },
    {  9, "Rooster", "The Observer",    "Honest, diligent, perceptive",
       "Critical, abrasive, vain",           "Ox, Snake, Dragon",
       "The voice that calls truth \xe2\x80\x94 precision in all things" },
    { 10, "Dog",     "The Guardian",    "Loyal, honest, protective",
       "Anxious, stubborn, judgmental",      "Tiger, Rabbit, Horse",
       "The faithfulness that anchors \xe2\x80\x94 loyalty as the highest virtue" },
    { 11, "Pig",     "The Generous",    "Kind, generous, optimistic",
       "Naive, indulgent, gullible",         "Rabbit, Goat, Tiger",
       "The abundance that overflows \xe2\x80\x94 generosity without calculation" }
};

/* ================================================================
 * Static element data (5 elements)
 * ================================================================ */

static const ci_element_t ELEMENTS[5] = {
    { 0, "Wood",  "Growth, expansion, benevolence",      "Spring",
      "Through creativity, vision, and upward movement" },
    { 1, "Fire",  "Passion, transformation, brilliance", "Summer",
      "Through enthusiasm, warmth, and illumination" },
    { 2, "Earth", "Stability, nourishment, centering",   "Late Summer",
      "Through grounding, nurture, and practical care" },
    { 3, "Metal", "Precision, clarity, righteousness",   "Autumn",
      "Through refinement, letting go, and sharp focus" },
    { 4, "Water", "Wisdom, depth, adaptability",         "Winter",
      "Through flow, stillness, and the power of yielding" }
};

/* Invalid-return sentinels */
static const ci_animal_t INVALID_ANIMAL = {
    -1, "?", "?", "?", "?", "?", "?"
};

static const ci_element_t INVALID_ELEMENT = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ci_animal_t ci_animal_data(int animal) {
    if (animal < 0 || animal > 11) return INVALID_ANIMAL;
    return ANIMALS[animal];
}

ci_element_t ci_element_data(int element) {
    if (element < 0 || element > 4) return INVALID_ELEMENT;
    return ELEMENTS[element];
}

chinese_interp_t ci_interpret(int animal, int element,
                              int polarity, int cycle_year) {
    chinese_interp_t r;
    memset(&r, 0, sizeof(r));

    ci_animal_t  a = ci_animal_data(animal);
    ci_element_t e = ci_element_data(element);

    /* Glyph: first 3 chars of animal name */
    if (a.animal < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
    } else {
        /* Copy up to 3 characters */
        size_t len = strlen(a.name);
        size_t copy = len < 3 ? len : 3;
        memcpy(r.glyph, a.name, copy);
        r.glyph[copy] = '\0';
    }

    /* Polarity string */
    const char *pol_str = (polarity == 0) ? "Yang" : "Yin";

    /* Glance: "{element} {animal} ({polarity}) -- {brief}" */
    if (a.animal < 0 || e.element < 0) {
        snprintf(r.glance, sizeof(r.glance), "?");
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %s (%s) \xe2\x80\x94 %s",
                 e.name, a.name, pol_str, a.brief);
    }

    /* Detail: full narrative */
    if (a.animal < 0 || e.element < 0) {
        snprintf(r.detail, sizeof(r.detail), "?");
    } else {
        snprintf(r.detail, sizeof(r.detail),
                 "%s %s: %s. Strengths: %s. Challenges: %s. "
                 "Best with: %s. Element: %s (%s) \xe2\x80\x94 %s. "
                 "Year %d of 60.",
                 e.name, a.name, a.archetype,
                 a.strengths, a.challenges, a.compatibility,
                 e.name, e.nature, e.expression,
                 cycle_year);
    }

    return r;
}

int ci_animal_count(void) {
    return 12;
}

int ci_element_count(void) {
    return 5;
}
