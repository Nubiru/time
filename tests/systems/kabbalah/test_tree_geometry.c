/* test_tree_geometry.c — Tree of Life geometry module tests
 * TDD RED phase: tests written before implementation.
 * Source: Sefer Yetzirah (3 mothers, 7 doubles, 12 simples),
 *         Golden Dawn / Crowley 777 correspondence tables. */

#include "../../unity/unity.h"
#include "../../../src/systems/kabbalah/tree_geometry.h"
#include "../../../src/systems/kabbalah/sefirot.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants ===== */

void test_path_count_constant(void)
{
    TEST_ASSERT_EQUAL_INT(22, TREE_PATH_COUNT);
}

void test_gate_count_constant(void)
{
    TEST_ASSERT_EQUAL_INT(231, TREE_GATE_COUNT);
}

void test_letter_type_constants(void)
{
    TEST_ASSERT_EQUAL_INT(0, TREE_LETTER_MOTHER);
    TEST_ASSERT_EQUAL_INT(1, TREE_LETTER_DOUBLE);
    TEST_ASSERT_EQUAL_INT(2, TREE_LETTER_SIMPLE);
}

/* ===== Path count function ===== */

void test_path_count_function(void)
{
    TEST_ASSERT_EQUAL_INT(22, tree_path_count());
}

/* ===== Letter type counts ===== */

void test_mother_count(void)
{
    TEST_ASSERT_EQUAL_INT(3, tree_count_mother_paths());
}

void test_double_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, tree_count_double_paths());
}

void test_simple_count(void)
{
    TEST_ASSERT_EQUAL_INT(12, tree_count_simple_paths());
}

void test_total_3_7_12(void)
{
    int total = tree_count_mother_paths()
              + tree_count_double_paths()
              + tree_count_simple_paths();
    TEST_ASSERT_EQUAL_INT(22, total);
}

/* ===== Mother letters ===== */

void test_path_aleph(void)
{
    tree_path_t p = tree_path_get(0);
    TEST_ASSERT_EQUAL_INT(0, p.id);
    TEST_ASSERT_EQUAL_STRING("Aleph", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(1, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_MOTHER, p.letter_type);
    TEST_ASSERT_EQUAL_INT(0, p.from_sefirah);  /* Keter */
    TEST_ASSERT_EQUAL_INT(1, p.to_sefirah);    /* Chokmah */
    TEST_ASSERT_EQUAL_INT(0, p.tarot_number);
    TEST_ASSERT_EQUAL_STRING("The Fool", p.tarot);
    TEST_ASSERT_EQUAL_STRING("Air", p.attribution);
    TEST_ASSERT_EQUAL_INT(-1, p.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(-1, p.planet_index);
}

void test_path_mem(void)
{
    tree_path_t p = tree_path_get(12);
    TEST_ASSERT_EQUAL_INT(12, p.id);
    TEST_ASSERT_EQUAL_STRING("Mem", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(40, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_MOTHER, p.letter_type);
    TEST_ASSERT_EQUAL_INT(4, p.from_sefirah);  /* Gevurah */
    TEST_ASSERT_EQUAL_INT(7, p.to_sefirah);    /* Hod */
    TEST_ASSERT_EQUAL_INT(12, p.tarot_number);
    TEST_ASSERT_EQUAL_STRING("The Hanged Man", p.tarot);
    TEST_ASSERT_EQUAL_STRING("Water", p.attribution);
}

void test_path_shin(void)
{
    tree_path_t p = tree_path_get(20);
    TEST_ASSERT_EQUAL_INT(20, p.id);
    TEST_ASSERT_EQUAL_STRING("Shin", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(300, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_MOTHER, p.letter_type);
    TEST_ASSERT_EQUAL_INT(7, p.from_sefirah);  /* Hod */
    TEST_ASSERT_EQUAL_INT(9, p.to_sefirah);    /* Malkuth */
    TEST_ASSERT_EQUAL_INT(20, p.tarot_number);
    TEST_ASSERT_EQUAL_STRING("Judgement", p.tarot);
    TEST_ASSERT_EQUAL_STRING("Fire", p.attribution);
}

/* ===== Double letters (planets) ===== */

void test_path_bet(void)
{
    tree_path_t p = tree_path_get(1);
    TEST_ASSERT_EQUAL_STRING("Bet", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(2, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(0, p.from_sefirah);  /* Keter */
    TEST_ASSERT_EQUAL_INT(2, p.to_sefirah);    /* Binah */
    TEST_ASSERT_EQUAL_INT(1, p.tarot_number);
    TEST_ASSERT_EQUAL_STRING("The Magician", p.tarot);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.attribution);
    TEST_ASSERT_EQUAL_INT(-1, p.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(0, p.planet_index);  /* Mercury=0 */
}

void test_path_gimel(void)
{
    tree_path_t p = tree_path_get(2);
    TEST_ASSERT_EQUAL_STRING("Gimel", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(3, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(0, p.from_sefirah);  /* Keter */
    TEST_ASSERT_EQUAL_INT(5, p.to_sefirah);    /* Tiferet */
    TEST_ASSERT_EQUAL_INT(2, p.tarot_number);
    TEST_ASSERT_EQUAL_STRING("The High Priestess", p.tarot);
    TEST_ASSERT_EQUAL_INT(9, p.planet_index);  /* Moon=9 */
}

void test_path_dalet(void)
{
    tree_path_t p = tree_path_get(3);
    TEST_ASSERT_EQUAL_STRING("Dalet", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(1, p.from_sefirah);  /* Chokmah */
    TEST_ASSERT_EQUAL_INT(2, p.to_sefirah);    /* Binah */
    TEST_ASSERT_EQUAL_STRING("The Empress", p.tarot);
    TEST_ASSERT_EQUAL_INT(1, p.planet_index);  /* Venus=1 */
}

void test_path_kaf(void)
{
    tree_path_t p = tree_path_get(10);
    TEST_ASSERT_EQUAL_STRING("Kaf", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(20, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(3, p.from_sefirah);  /* Chesed */
    TEST_ASSERT_EQUAL_INT(6, p.to_sefirah);    /* Netzach */
    TEST_ASSERT_EQUAL_STRING("Wheel of Fortune", p.tarot);
    TEST_ASSERT_EQUAL_INT(4, p.planet_index);  /* Jupiter=4 */
}

void test_path_pe(void)
{
    tree_path_t p = tree_path_get(16);
    TEST_ASSERT_EQUAL_STRING("Pe", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(80, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(6, p.from_sefirah);  /* Netzach */
    TEST_ASSERT_EQUAL_INT(7, p.to_sefirah);    /* Hod */
    TEST_ASSERT_EQUAL_STRING("The Tower", p.tarot);
    TEST_ASSERT_EQUAL_INT(3, p.planet_index);  /* Mars=3 */
}

void test_path_resh(void)
{
    tree_path_t p = tree_path_get(19);
    TEST_ASSERT_EQUAL_STRING("Resh", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(200, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(7, p.from_sefirah);  /* Hod */
    TEST_ASSERT_EQUAL_INT(8, p.to_sefirah);    /* Yesod */
    TEST_ASSERT_EQUAL_STRING("The Sun", p.tarot);
    TEST_ASSERT_EQUAL_INT(8, p.planet_index);  /* Sun=8 */
}

void test_path_tav(void)
{
    tree_path_t p = tree_path_get(21);
    TEST_ASSERT_EQUAL_STRING("Tav", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(400, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_DOUBLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(8, p.from_sefirah);  /* Yesod */
    TEST_ASSERT_EQUAL_INT(9, p.to_sefirah);    /* Malkuth */
    TEST_ASSERT_EQUAL_STRING("The World", p.tarot);
    TEST_ASSERT_EQUAL_INT(5, p.planet_index);  /* Saturn=5 */
}

/* ===== Simple letters (zodiac) ===== */

void test_path_he_aries(void)
{
    tree_path_t p = tree_path_get(4);
    TEST_ASSERT_EQUAL_STRING("He", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(5, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_SIMPLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(1, p.from_sefirah);  /* Chokmah */
    TEST_ASSERT_EQUAL_INT(5, p.to_sefirah);    /* Tiferet */
    TEST_ASSERT_EQUAL_STRING("The Emperor", p.tarot);
    TEST_ASSERT_EQUAL_INT(0, p.zodiac_sign);   /* Aries=0 */
    TEST_ASSERT_EQUAL_INT(-1, p.planet_index);
}

void test_path_vav_taurus(void)
{
    tree_path_t p = tree_path_get(5);
    TEST_ASSERT_EQUAL_STRING("Vav", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_SIMPLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(1, p.from_sefirah);  /* Chokmah */
    TEST_ASSERT_EQUAL_INT(3, p.to_sefirah);    /* Chesed */
    TEST_ASSERT_EQUAL_STRING("The Hierophant", p.tarot);
    TEST_ASSERT_EQUAL_INT(1, p.zodiac_sign);   /* Taurus=1 */
}

void test_path_lamed_libra(void)
{
    tree_path_t p = tree_path_get(11);
    TEST_ASSERT_EQUAL_STRING("Lamed", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(30, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_SIMPLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(4, p.from_sefirah);  /* Gevurah */
    TEST_ASSERT_EQUAL_INT(5, p.to_sefirah);    /* Tiferet */
    TEST_ASSERT_EQUAL_STRING("Justice", p.tarot);
    TEST_ASSERT_EQUAL_INT(6, p.zodiac_sign);   /* Libra=6 */
}

void test_path_qof_pisces(void)
{
    tree_path_t p = tree_path_get(18);
    TEST_ASSERT_EQUAL_STRING("Qof", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(100, p.letter_value);
    TEST_ASSERT_EQUAL_INT(TREE_LETTER_SIMPLE, p.letter_type);
    TEST_ASSERT_EQUAL_INT(8, p.from_sefirah);  /* Yesod */
    TEST_ASSERT_EQUAL_INT(9, p.to_sefirah);    /* Malkuth */
    TEST_ASSERT_EQUAL_STRING("The Moon", p.tarot);
    TEST_ASSERT_EQUAL_INT(11, p.zodiac_sign);  /* Pisces=11 */
}

/* ===== Hebrew chars (UTF-8) ===== */

void test_hebrew_char_aleph(void)
{
    tree_path_t p = tree_path_get(0);
    TEST_ASSERT_NOT_NULL(p.hebrew_char);
    /* Aleph should be a non-empty UTF-8 string */
    TEST_ASSERT_TRUE(strlen(p.hebrew_char) > 0);
}

void test_hebrew_chars_all_present(void)
{
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        TEST_ASSERT_NOT_NULL(p.hebrew_char);
        TEST_ASSERT_TRUE(strlen(p.hebrew_char) > 0);
    }
}

/* ===== Zodiac lookup ===== */

void test_zodiac_lookup_aries(void)
{
    tree_path_t p = tree_path_for_zodiac(0);
    TEST_ASSERT_EQUAL_STRING("He", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(4, p.id);
}

void test_zodiac_lookup_pisces(void)
{
    tree_path_t p = tree_path_for_zodiac(11);
    TEST_ASSERT_EQUAL_STRING("Qof", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(18, p.id);
}

void test_zodiac_lookup_gemini(void)
{
    tree_path_t p = tree_path_for_zodiac(2);
    TEST_ASSERT_EQUAL_STRING("Zayin", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(6, p.id);
}

void test_zodiac_lookup_invalid(void)
{
    tree_path_t p = tree_path_for_zodiac(12);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

void test_zodiac_lookup_negative(void)
{
    tree_path_t p = tree_path_for_zodiac(-1);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

/* ===== Planet lookup ===== */

void test_planet_lookup_mercury(void)
{
    tree_path_t p = tree_path_for_planet(0);
    TEST_ASSERT_EQUAL_STRING("Bet", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(1, p.id);
}

void test_planet_lookup_moon(void)
{
    tree_path_t p = tree_path_for_planet(9);
    TEST_ASSERT_EQUAL_STRING("Gimel", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(2, p.id);
}

void test_planet_lookup_sun(void)
{
    tree_path_t p = tree_path_for_planet(8);
    TEST_ASSERT_EQUAL_STRING("Resh", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(19, p.id);
}

void test_planet_lookup_venus(void)
{
    tree_path_t p = tree_path_for_planet(1);
    TEST_ASSERT_EQUAL_STRING("Dalet", p.hebrew_letter);
}

void test_planet_lookup_mars(void)
{
    tree_path_t p = tree_path_for_planet(3);
    TEST_ASSERT_EQUAL_STRING("Pe", p.hebrew_letter);
}

void test_planet_lookup_jupiter(void)
{
    tree_path_t p = tree_path_for_planet(4);
    TEST_ASSERT_EQUAL_STRING("Kaf", p.hebrew_letter);
}

void test_planet_lookup_saturn(void)
{
    tree_path_t p = tree_path_for_planet(5);
    TEST_ASSERT_EQUAL_STRING("Tav", p.hebrew_letter);
}

void test_planet_lookup_earth_none(void)
{
    /* Earth(2) has no path (maps to Sefirah Malkuth instead) */
    tree_path_t p = tree_path_for_planet(2);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

void test_planet_lookup_uranus_none(void)
{
    /* Uranus(6) has no path */
    tree_path_t p = tree_path_for_planet(6);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

void test_planet_lookup_neptune_none(void)
{
    /* Neptune(7) has no path */
    tree_path_t p = tree_path_for_planet(7);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

/* ===== Tarot lookup ===== */

void test_tarot_lookup_fool(void)
{
    tree_path_t p = tree_path_for_tarot(0);
    TEST_ASSERT_EQUAL_STRING("Aleph", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(0, p.id);
}

void test_tarot_lookup_world(void)
{
    tree_path_t p = tree_path_for_tarot(21);
    TEST_ASSERT_EQUAL_STRING("Tav", p.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(21, p.id);
}

void test_tarot_lookup_death(void)
{
    tree_path_t p = tree_path_for_tarot(13);
    TEST_ASSERT_EQUAL_STRING("Nun", p.hebrew_letter);
}

void test_tarot_lookup_invalid(void)
{
    tree_path_t p = tree_path_for_tarot(22);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

/* ===== Invalid path index ===== */

void test_path_invalid_high(void)
{
    tree_path_t p = tree_path_get(22);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

void test_path_invalid_negative(void)
{
    tree_path_t p = tree_path_get(-1);
    TEST_ASSERT_NULL(p.hebrew_letter);
}

/* ===== Layout generation ===== */

void test_layout_line_count(void)
{
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    TEST_ASSERT_EQUAL_INT(22, layout.line_count);
}

void test_layout_node_count(void)
{
    /* 10 Sefirot + Da'at = 11 nodes */
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    TEST_ASSERT_EQUAL_INT(11, layout.node_count);
}

void test_layout_nodes_scaled(void)
{
    /* Keter at center-top, Malkuth at center-bottom */
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    /* Keter: pos_x=0.0 => center of width=100 => 50.0
     * Keter: pos_y=0.0 => top of height=200 => 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 50.0f, layout.node_x[0]);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 0.0f, layout.node_y[0]);
}

void test_layout_malkuth_position(void)
{
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    /* Malkuth: pos_x=0.0 => 50.0, pos_y=1.0 => 200.0 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 50.0f, layout.node_x[9]);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 200.0f, layout.node_y[9]);
}

void test_layout_chokmah_right(void)
{
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    /* Chokmah: pos_x=0.5 => (0.5+0.5)*100 = 100.0, pos_y=0.17 => 34.0 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 100.0f, layout.node_x[1]);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 34.0f, layout.node_y[1]);
}

void test_layout_daat_node(void)
{
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    /* Da'at is node[10]: pos_x=0.0 => 50.0, pos_y=0.33 => 66.0 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 50.0f, layout.node_x[10]);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 66.0f, layout.node_y[10]);
}

void test_layout_lines_match_paths(void)
{
    /* First line should connect Keter(0) to Chokmah(1) — path 0 (Aleph) */
    tree_layout_t layout = tree_layout_generate(100.0f, 200.0f);
    tree_path_t p0 = tree_path_get(0);
    /* Line endpoints should match the node positions for from/to */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, layout.node_x[p0.from_sefirah], layout.lines[0].x0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, layout.node_y[p0.from_sefirah], layout.lines[0].y0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, layout.node_x[p0.to_sefirah], layout.lines[0].x1);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, layout.node_y[p0.to_sefirah], layout.lines[0].y1);
}

/* ===== 231 Gates ===== */

void test_gate_count_function(void)
{
    TEST_ASSERT_EQUAL_INT(231, tree_gate_count());
}

void test_gate_first(void)
{
    tree_gate_t g = tree_gate_get(0);
    TEST_ASSERT_EQUAL_INT(0, g.letter_a);
    TEST_ASSERT_EQUAL_INT(1, g.letter_b);
}

void test_gate_last(void)
{
    tree_gate_t g = tree_gate_get(230);
    TEST_ASSERT_EQUAL_INT(20, g.letter_a);
    TEST_ASSERT_EQUAL_INT(21, g.letter_b);
}

void test_gate_ordering(void)
{
    /* letter_a should always be less than letter_b */
    for (int i = 0; i < TREE_GATE_COUNT; i++) {
        tree_gate_t g = tree_gate_get(i);
        TEST_ASSERT_TRUE(g.letter_a < g.letter_b);
    }
}

void test_gate_invalid(void)
{
    tree_gate_t g = tree_gate_get(231);
    TEST_ASSERT_EQUAL_INT(0, g.letter_a);
    TEST_ASSERT_EQUAL_INT(0, g.letter_b);
}

void test_gate_invalid_negative(void)
{
    tree_gate_t g = tree_gate_get(-1);
    TEST_ASSERT_EQUAL_INT(0, g.letter_a);
    TEST_ASSERT_EQUAL_INT(0, g.letter_b);
}

/* ===== All paths completeness ===== */

void test_all_paths_have_letter_names(void)
{
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        TEST_ASSERT_NOT_NULL(p.hebrew_letter);
        TEST_ASSERT_TRUE(strlen(p.hebrew_letter) > 0);
    }
}

void test_all_paths_have_tarot(void)
{
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        TEST_ASSERT_NOT_NULL(p.tarot);
        TEST_ASSERT_TRUE(strlen(p.tarot) > 0);
    }
}

void test_all_paths_have_attribution(void)
{
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        TEST_ASSERT_NOT_NULL(p.attribution);
        TEST_ASSERT_TRUE(strlen(p.attribution) > 0);
    }
}

void test_all_paths_valid_sefirot(void)
{
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        TEST_ASSERT_TRUE(p.from_sefirah >= 0);
        TEST_ASSERT_TRUE(p.from_sefirah <= 9);
        TEST_ASSERT_TRUE(p.to_sefirah >= 0);
        TEST_ASSERT_TRUE(p.to_sefirah <= 9);
        TEST_ASSERT_TRUE(p.from_sefirah != p.to_sefirah);
    }
}

void test_letter_values_ascending(void)
{
    /* Hebrew letter values should be in ascending order across the 22 paths */
    int prev = 0;
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        TEST_ASSERT_TRUE(p.letter_value > prev);
        prev = p.letter_value;
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Constants (3) */
    RUN_TEST(test_path_count_constant);
    RUN_TEST(test_gate_count_constant);
    RUN_TEST(test_letter_type_constants);

    /* Path count function (1) */
    RUN_TEST(test_path_count_function);

    /* Letter type counts (4) */
    RUN_TEST(test_mother_count);
    RUN_TEST(test_double_count);
    RUN_TEST(test_simple_count);
    RUN_TEST(test_total_3_7_12);

    /* Mother letters (3) */
    RUN_TEST(test_path_aleph);
    RUN_TEST(test_path_mem);
    RUN_TEST(test_path_shin);

    /* Double letters (7) */
    RUN_TEST(test_path_bet);
    RUN_TEST(test_path_gimel);
    RUN_TEST(test_path_dalet);
    RUN_TEST(test_path_kaf);
    RUN_TEST(test_path_pe);
    RUN_TEST(test_path_resh);
    RUN_TEST(test_path_tav);

    /* Simple letters (4) */
    RUN_TEST(test_path_he_aries);
    RUN_TEST(test_path_vav_taurus);
    RUN_TEST(test_path_lamed_libra);
    RUN_TEST(test_path_qof_pisces);

    /* Hebrew chars (2) */
    RUN_TEST(test_hebrew_char_aleph);
    RUN_TEST(test_hebrew_chars_all_present);

    /* Zodiac lookup (5) */
    RUN_TEST(test_zodiac_lookup_aries);
    RUN_TEST(test_zodiac_lookup_pisces);
    RUN_TEST(test_zodiac_lookup_gemini);
    RUN_TEST(test_zodiac_lookup_invalid);
    RUN_TEST(test_zodiac_lookup_negative);

    /* Planet lookup (10) */
    RUN_TEST(test_planet_lookup_mercury);
    RUN_TEST(test_planet_lookup_moon);
    RUN_TEST(test_planet_lookup_sun);
    RUN_TEST(test_planet_lookup_venus);
    RUN_TEST(test_planet_lookup_mars);
    RUN_TEST(test_planet_lookup_jupiter);
    RUN_TEST(test_planet_lookup_saturn);
    RUN_TEST(test_planet_lookup_earth_none);
    RUN_TEST(test_planet_lookup_uranus_none);
    RUN_TEST(test_planet_lookup_neptune_none);

    /* Tarot lookup (4) */
    RUN_TEST(test_tarot_lookup_fool);
    RUN_TEST(test_tarot_lookup_world);
    RUN_TEST(test_tarot_lookup_death);
    RUN_TEST(test_tarot_lookup_invalid);

    /* Invalid path indices (2) */
    RUN_TEST(test_path_invalid_high);
    RUN_TEST(test_path_invalid_negative);

    /* Layout (7) */
    RUN_TEST(test_layout_line_count);
    RUN_TEST(test_layout_node_count);
    RUN_TEST(test_layout_nodes_scaled);
    RUN_TEST(test_layout_malkuth_position);
    RUN_TEST(test_layout_chokmah_right);
    RUN_TEST(test_layout_daat_node);
    RUN_TEST(test_layout_lines_match_paths);

    /* Gates (5) */
    RUN_TEST(test_gate_count_function);
    RUN_TEST(test_gate_first);
    RUN_TEST(test_gate_last);
    RUN_TEST(test_gate_ordering);
    RUN_TEST(test_gate_invalid);
    RUN_TEST(test_gate_invalid_negative);

    /* Completeness (5) */
    RUN_TEST(test_all_paths_have_letter_names);
    RUN_TEST(test_all_paths_have_tarot);
    RUN_TEST(test_all_paths_have_attribution);
    RUN_TEST(test_all_paths_valid_sefirot);
    RUN_TEST(test_letter_values_ascending);

    return UNITY_END();
}
