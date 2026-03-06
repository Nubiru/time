/* tree_geometry.c — Kabbalah Tree of Life: 22 paths + 231 gates
 *
 * Pure data module. All data is static const. No malloc, no globals,
 * no side effects.
 *
 * Sources: Sefer Yetzirah (3 mothers, 7 doubles, 12 simples),
 *          Golden Dawn / Crowley "777" correspondence tables (public domain),
 *          Kircher Tree of Life path layout.
 *
 * Path numbering follows the Hebrew alphabet order (Aleph=0 through Tav=21),
 * matching the standard Golden Dawn attribution system. */

#include "tree_geometry.h"
#include "sefirot.h"
#include <string.h>

/* The 22 paths, indexed 0 (Aleph) through 21 (Tav).
 *
 * Letter classification per Sefer Yetzirah:
 *   3 Mothers (elements): Aleph=Air, Mem=Water, Shin=Fire
 *   7 Doubles (planets): Bet=Mercury, Gimel=Moon, Dalet=Venus,
 *                         Kaf=Jupiter, Pe=Mars, Resh=Sun, Tav=Saturn
 *   12 Simples (zodiac): He=Aries through Qof=Pisces
 *
 * Sefirot connections follow the Kircher Tree (Golden Dawn standard).
 * Tarot attributions follow the Golden Dawn / Crowley system. */
static const tree_path_t PATH_DATA[TREE_PATH_COUNT] = {
    /* === 3 Mother Letters (elements) === */
    { /* 0 */ .id = 0,
      .from_sefirah = 0, .to_sefirah = 1,
      .hebrew_letter = "Aleph", .hebrew_char = "\xD7\x90",
      .letter_value = 1, .letter_type = TREE_LETTER_MOTHER,
      .tarot = "The Fool", .tarot_number = 0,
      .attribution = "Air", .zodiac_sign = -1, .planet_index = -1 },

    /* === 7 Double Letters (planets) === */
    { /* 1 */ .id = 1,
      .from_sefirah = 0, .to_sefirah = 2,
      .hebrew_letter = "Bet", .hebrew_char = "\xD7\x91",
      .letter_value = 2, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "The Magician", .tarot_number = 1,
      .attribution = "Mercury", .zodiac_sign = -1, .planet_index = 0 },

    { /* 2 */ .id = 2,
      .from_sefirah = 0, .to_sefirah = 5,
      .hebrew_letter = "Gimel", .hebrew_char = "\xD7\x92",
      .letter_value = 3, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "The High Priestess", .tarot_number = 2,
      .attribution = "Moon", .zodiac_sign = -1, .planet_index = 9 },

    { /* 3 */ .id = 3,
      .from_sefirah = 1, .to_sefirah = 2,
      .hebrew_letter = "Dalet", .hebrew_char = "\xD7\x93",
      .letter_value = 4, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "The Empress", .tarot_number = 3,
      .attribution = "Venus", .zodiac_sign = -1, .planet_index = 1 },

    /* === 12 Simple Letters (zodiac) === */
    { /* 4 */ .id = 4,
      .from_sefirah = 1, .to_sefirah = 5,
      .hebrew_letter = "He", .hebrew_char = "\xD7\x94",
      .letter_value = 5, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Emperor", .tarot_number = 4,
      .attribution = "Aries", .zodiac_sign = 0, .planet_index = -1 },

    { /* 5 */ .id = 5,
      .from_sefirah = 1, .to_sefirah = 3,
      .hebrew_letter = "Vav", .hebrew_char = "\xD7\x95",
      .letter_value = 6, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Hierophant", .tarot_number = 5,
      .attribution = "Taurus", .zodiac_sign = 1, .planet_index = -1 },

    { /* 6 */ .id = 6,
      .from_sefirah = 2, .to_sefirah = 5,
      .hebrew_letter = "Zayin", .hebrew_char = "\xD7\x96",
      .letter_value = 7, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Lovers", .tarot_number = 6,
      .attribution = "Gemini", .zodiac_sign = 2, .planet_index = -1 },

    { /* 7 */ .id = 7,
      .from_sefirah = 2, .to_sefirah = 4,
      .hebrew_letter = "Chet", .hebrew_char = "\xD7\x97",
      .letter_value = 8, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Chariot", .tarot_number = 7,
      .attribution = "Cancer", .zodiac_sign = 3, .planet_index = -1 },

    { /* 8 */ .id = 8,
      .from_sefirah = 3, .to_sefirah = 4,
      .hebrew_letter = "Tet", .hebrew_char = "\xD7\x98",
      .letter_value = 9, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "Strength", .tarot_number = 8,
      .attribution = "Leo", .zodiac_sign = 4, .planet_index = -1 },

    { /* 9 */ .id = 9,
      .from_sefirah = 3, .to_sefirah = 5,
      .hebrew_letter = "Yod", .hebrew_char = "\xD7\x99",
      .letter_value = 10, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Hermit", .tarot_number = 9,
      .attribution = "Virgo", .zodiac_sign = 5, .planet_index = -1 },

    /* === Double Letter === */
    { /* 10 */ .id = 10,
      .from_sefirah = 3, .to_sefirah = 6,
      .hebrew_letter = "Kaf", .hebrew_char = "\xD7\x9B",
      .letter_value = 20, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "Wheel of Fortune", .tarot_number = 10,
      .attribution = "Jupiter", .zodiac_sign = -1, .planet_index = 4 },

    /* === Simple Letter === */
    { /* 11 */ .id = 11,
      .from_sefirah = 4, .to_sefirah = 5,
      .hebrew_letter = "Lamed", .hebrew_char = "\xD7\x9C",
      .letter_value = 30, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "Justice", .tarot_number = 11,
      .attribution = "Libra", .zodiac_sign = 6, .planet_index = -1 },

    /* === Mother Letter === */
    { /* 12 */ .id = 12,
      .from_sefirah = 4, .to_sefirah = 7,
      .hebrew_letter = "Mem", .hebrew_char = "\xD7\x9E",
      .letter_value = 40, .letter_type = TREE_LETTER_MOTHER,
      .tarot = "The Hanged Man", .tarot_number = 12,
      .attribution = "Water", .zodiac_sign = -1, .planet_index = -1 },

    /* === Simple Letters === */
    { /* 13 */ .id = 13,
      .from_sefirah = 5, .to_sefirah = 6,
      .hebrew_letter = "Nun", .hebrew_char = "\xD7\xA0",
      .letter_value = 50, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "Death", .tarot_number = 13,
      .attribution = "Scorpio", .zodiac_sign = 7, .planet_index = -1 },

    { /* 14 */ .id = 14,
      .from_sefirah = 5, .to_sefirah = 8,
      .hebrew_letter = "Samekh", .hebrew_char = "\xD7\xA1",
      .letter_value = 60, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "Temperance", .tarot_number = 14,
      .attribution = "Sagittarius", .zodiac_sign = 8, .planet_index = -1 },

    { /* 15 */ .id = 15,
      .from_sefirah = 5, .to_sefirah = 7,
      .hebrew_letter = "Ayin", .hebrew_char = "\xD7\xA2",
      .letter_value = 70, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Devil", .tarot_number = 15,
      .attribution = "Capricorn", .zodiac_sign = 9, .planet_index = -1 },

    /* === Double Letter === */
    { /* 16 */ .id = 16,
      .from_sefirah = 6, .to_sefirah = 7,
      .hebrew_letter = "Pe", .hebrew_char = "\xD7\xA4",
      .letter_value = 80, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "The Tower", .tarot_number = 16,
      .attribution = "Mars", .zodiac_sign = -1, .planet_index = 3 },

    /* === Simple Letters === */
    { /* 17 */ .id = 17,
      .from_sefirah = 6, .to_sefirah = 8,
      .hebrew_letter = "Tsade", .hebrew_char = "\xD7\xA6",
      .letter_value = 90, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Star", .tarot_number = 17,
      .attribution = "Aquarius", .zodiac_sign = 10, .planet_index = -1 },

    { /* 18 */ .id = 18,
      .from_sefirah = 8, .to_sefirah = 9,
      .hebrew_letter = "Qof", .hebrew_char = "\xD7\xA7",
      .letter_value = 100, .letter_type = TREE_LETTER_SIMPLE,
      .tarot = "The Moon", .tarot_number = 18,
      .attribution = "Pisces", .zodiac_sign = 11, .planet_index = -1 },

    /* === Double Letter === */
    { /* 19 */ .id = 19,
      .from_sefirah = 7, .to_sefirah = 8,
      .hebrew_letter = "Resh", .hebrew_char = "\xD7\xA8",
      .letter_value = 200, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "The Sun", .tarot_number = 19,
      .attribution = "Sun", .zodiac_sign = -1, .planet_index = 8 },

    /* === Mother Letter === */
    { /* 20 */ .id = 20,
      .from_sefirah = 7, .to_sefirah = 9,
      .hebrew_letter = "Shin", .hebrew_char = "\xD7\xA9",
      .letter_value = 300, .letter_type = TREE_LETTER_MOTHER,
      .tarot = "Judgement", .tarot_number = 20,
      .attribution = "Fire", .zodiac_sign = -1, .planet_index = -1 },

    /* === Double Letter === */
    { /* 21 */ .id = 21,
      .from_sefirah = 8, .to_sefirah = 9,
      .hebrew_letter = "Tav", .hebrew_char = "\xD7\xAA",
      .letter_value = 400, .letter_type = TREE_LETTER_DOUBLE,
      .tarot = "The World", .tarot_number = 21,
      .attribution = "Saturn", .zodiac_sign = -1, .planet_index = 5 }
};

tree_path_t tree_path_get(int index)
{
    if (index < 0 || index >= TREE_PATH_COUNT) {
        tree_path_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return PATH_DATA[index];
}

tree_path_t tree_path_for_zodiac(int sign)
{
    if (sign < 0 || sign > 11) {
        tree_path_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (PATH_DATA[i].zodiac_sign == sign) {
            return PATH_DATA[i];
        }
    }
    tree_path_t empty;
    memset(&empty, 0, sizeof(empty));
    return empty;
}

tree_path_t tree_path_for_planet(int planet)
{
    if (planet < 0 || planet > 9) {
        tree_path_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (PATH_DATA[i].planet_index == planet) {
            return PATH_DATA[i];
        }
    }
    tree_path_t empty;
    memset(&empty, 0, sizeof(empty));
    return empty;
}

tree_path_t tree_path_for_tarot(int tarot_number)
{
    if (tarot_number < 0 || tarot_number >= TREE_PATH_COUNT) {
        tree_path_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (PATH_DATA[i].tarot_number == tarot_number) {
            return PATH_DATA[i];
        }
    }
    tree_path_t empty;
    memset(&empty, 0, sizeof(empty));
    return empty;
}

int tree_path_count(void)
{
    return TREE_PATH_COUNT;
}

tree_layout_t tree_layout_generate(float width, float height)
{
    tree_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Sefirot positions: pos_x in [-0.5, 0.5], pos_y in [0.0, 1.0]
     * Scale: x maps [-0.5, 0.5] to [0, width] => x_pixel = (pos_x + 0.5) * width
     *        y maps [0.0, 1.0] to [0, height] => y_pixel = pos_y * height */

    /* 10 Sefirot */
    for (int i = 0; i < SEFIROT_COUNT; i++) {
        sefirah_t s = sefirot_get(i);
        layout.node_x[i] = (s.pos_x + 0.5f) * width;
        layout.node_y[i] = s.pos_y * height;
    }

    /* Da'at as node[10] */
    sefirah_t daat = sefirot_get_daat();
    layout.node_x[10] = (daat.pos_x + 0.5f) * width;
    layout.node_y[10] = daat.pos_y * height;

    layout.node_count = 11;

    /* Generate line segments from path data */
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        int from = PATH_DATA[i].from_sefirah;
        int to = PATH_DATA[i].to_sefirah;
        layout.lines[i].x0 = layout.node_x[from];
        layout.lines[i].y0 = layout.node_y[from];
        layout.lines[i].x1 = layout.node_x[to];
        layout.lines[i].y1 = layout.node_y[to];
    }
    layout.line_count = TREE_PATH_COUNT;

    return layout;
}

tree_gate_t tree_gate_get(int index)
{
    if (index < 0 || index >= TREE_GATE_COUNT) {
        tree_gate_t empty = { 0, 0 };
        return empty;
    }

    /* Generate C(22,2) combinations: for a from 0..20, b from a+1..21
     * Sequential index: sum of (21-a) for previous a values, plus offset */
    int idx = 0;
    for (int a = 0; a < 21; a++) {
        int remaining = 21 - a;
        if (idx + remaining > index) {
            tree_gate_t gate;
            gate.letter_a = a;
            gate.letter_b = a + 1 + (index - idx);
            return gate;
        }
        idx += remaining;
    }

    /* Should not reach here for valid index */
    tree_gate_t empty = { 0, 0 };
    return empty;
}

int tree_gate_count(void)
{
    return TREE_GATE_COUNT;
}

int tree_count_mother_paths(void)
{
    int count = 0;
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (PATH_DATA[i].letter_type == TREE_LETTER_MOTHER) {
            count++;
        }
    }
    return count;
}

int tree_count_double_paths(void)
{
    int count = 0;
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (PATH_DATA[i].letter_type == TREE_LETTER_DOUBLE) {
            count++;
        }
    }
    return count;
}

int tree_count_simple_paths(void)
{
    int count = 0;
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (PATH_DATA[i].letter_type == TREE_LETTER_SIMPLE) {
            count++;
        }
    }
    return count;
}
