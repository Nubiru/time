#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/force_lines.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Vertical count
 * ================================================================ */

static void test_vertical_count_is_12(void)
{
    TEST_ASSERT_EQUAL_INT(12, force_line_vertical_count());
}

/* ================================================================
 * Vertical line 0: days 1-28, Tower of Spirit / Tower of Babel
 * ================================================================ */

static void test_vertical0_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(1, force_line_vertical_get(0).upper_day);
}

static void test_vertical0_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(28, force_line_vertical_get(0).lower_day);
}

static void test_vertical0_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Tower of Spirit / Tower of Babel",
                             force_line_vertical_get(0).meaning);
}

/* ================================================================
 * Vertical line 1: days 2-27, Telecosmic power of 9
 * ================================================================ */

static void test_vertical1_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(2, force_line_vertical_get(1).upper_day);
}

static void test_vertical1_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(27, force_line_vertical_get(1).lower_day);
}

static void test_vertical1_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Telecosmic power of 9",
                             force_line_vertical_get(1).meaning);
}

/* ================================================================
 * Vertical line 2: days 3-26, Telecosmic power of 13
 * ================================================================ */

static void test_vertical2_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(3, force_line_vertical_get(2).upper_day);
}

static void test_vertical2_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(26, force_line_vertical_get(2).lower_day);
}

static void test_vertical2_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Telecosmic power of 13",
                             force_line_vertical_get(2).meaning);
}

/* ================================================================
 * Vertical line 3: days 4-25, Telecosmic power of 5
 * ================================================================ */

static void test_vertical3_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(4, force_line_vertical_get(3).upper_day);
}

static void test_vertical3_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(25, force_line_vertical_get(3).lower_day);
}

static void test_vertical3_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Telecosmic power of 5",
                             force_line_vertical_get(3).meaning);
}

/* ================================================================
 * Vertical line 4: days 5-24, Telecosmic power of 4
 * ================================================================ */

static void test_vertical4_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(5, force_line_vertical_get(4).upper_day);
}

static void test_vertical4_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(24, force_line_vertical_get(4).lower_day);
}

static void test_vertical4_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Telecosmic power of 4",
                             force_line_vertical_get(4).meaning);
}

/* ================================================================
 * Vertical line 5: days 6-23, Tower of Navigation / Tower of Prophecy
 * ================================================================ */

static void test_vertical5_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(6, force_line_vertical_get(5).upper_day);
}

static void test_vertical5_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(23, force_line_vertical_get(5).lower_day);
}

static void test_vertical5_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Tower of Navigation / Tower of Prophecy",
                             force_line_vertical_get(5).meaning);
}

/* ================================================================
 * Vertical line 6: days 7-22, Buddha — New Jerusalem
 * ================================================================ */

static void test_vertical6_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(7, force_line_vertical_get(6).upper_day);
}

static void test_vertical6_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(22, force_line_vertical_get(6).lower_day);
}

static void test_vertical6_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddha — New Jerusalem",
                             force_line_vertical_get(6).meaning);
}

/* ================================================================
 * Vertical line 7: days 8-21, Christ — New Heaven, New Earth
 * ================================================================ */

static void test_vertical7_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(8, force_line_vertical_get(7).upper_day);
}

static void test_vertical7_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(21, force_line_vertical_get(7).lower_day);
}

static void test_vertical7_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Christ — New Heaven, New Earth",
                             force_line_vertical_get(7).meaning);
}

/* ================================================================
 * Vertical line 8: days 9-20, Lords of Red and Black — Prophecy 7
 * ================================================================ */

static void test_vertical8_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(9, force_line_vertical_get(8).upper_day);
}

static void test_vertical8_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(20, force_line_vertical_get(8).lower_day);
}

static void test_vertical8_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Lords of Red and Black — Prophecy 7",
                             force_line_vertical_get(8).meaning);
}

/* ================================================================
 * Vertical line 9: days 10-19, Muhammad, Pacal Votan — Prophecy 6
 * ================================================================ */

static void test_vertical9_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(10, force_line_vertical_get(9).upper_day);
}

static void test_vertical9_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(19, force_line_vertical_get(9).lower_day);
}

static void test_vertical9_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Muhammad, Pacal Votan — Prophecy 6",
                             force_line_vertical_get(9).meaning);
}

/* ================================================================
 * Vertical line 10: days 11-18, Quetzalcoatl — Prophecy 5
 * ================================================================ */

static void test_vertical10_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(11, force_line_vertical_get(10).upper_day);
}

static void test_vertical10_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(18, force_line_vertical_get(10).lower_day);
}

static void test_vertical10_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Quetzalcoatl — Prophecy 5",
                             force_line_vertical_get(10).meaning);
}

/* ================================================================
 * Vertical line 11: days 12-17, Baktun 12 — Prophecy 4
 * ================================================================ */

static void test_vertical11_upper_day(void)
{
    TEST_ASSERT_EQUAL_INT(12, force_line_vertical_get(11).upper_day);
}

static void test_vertical11_lower_day(void)
{
    TEST_ASSERT_EQUAL_INT(17, force_line_vertical_get(11).lower_day);
}

static void test_vertical11_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Baktun 12 — Prophecy 4 (Stairway to Heaven)",
                             force_line_vertical_get(11).meaning);
}

/* ================================================================
 * Vertical get — boundary: invalid indices return zeroed
 * ================================================================ */

static void test_vertical_get_neg1_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_get(-1);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_EQUAL_INT(0, v.lower_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_vertical_get_12_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_get(12);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_EQUAL_INT(0, v.lower_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_vertical_get_99_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_get(99);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_NULL(v.meaning);
}

/* ================================================================
 * Vertical — all pairs sum to 29
 * ================================================================ */

static void test_vertical_all_pairs_sum_29(void)
{
    for (int i = 0; i < 12; i++) {
        force_line_vertical_t v = force_line_vertical_get(i);
        TEST_ASSERT_EQUAL_INT(29, v.upper_day + v.lower_day);
    }
}

/* ================================================================
 * Vertical — upper days are 1-12 in order
 * ================================================================ */

static void test_vertical_upper_days_sequential(void)
{
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, force_line_vertical_get(i).upper_day);
    }
}

/* ================================================================
 * Vertical — lower days are 28-17 descending
 * ================================================================ */

static void test_vertical_lower_days_descending(void)
{
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT(28 - i, force_line_vertical_get(i).lower_day);
    }
}

/* ================================================================
 * Vertical — all meanings are non-null and non-empty
 * ================================================================ */

static void test_vertical_all_meanings_valid(void)
{
    for (int i = 0; i < 12; i++) {
        force_line_vertical_t v = force_line_vertical_get(i);
        TEST_ASSERT_NOT_NULL(v.meaning);
        TEST_ASSERT_TRUE(strlen(v.meaning) > 0);
    }
}

/* ================================================================
 * Paired day — basic pairs
 * ================================================================ */

static void test_paired_day_1_returns_28(void)
{
    TEST_ASSERT_EQUAL_INT(28, force_line_paired_day(1));
}

static void test_paired_day_28_returns_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, force_line_paired_day(28));
}

static void test_paired_day_7_returns_22(void)
{
    TEST_ASSERT_EQUAL_INT(22, force_line_paired_day(7));
}

static void test_paired_day_14_returns_15(void)
{
    TEST_ASSERT_EQUAL_INT(15, force_line_paired_day(14));
}

static void test_paired_day_15_returns_14(void)
{
    TEST_ASSERT_EQUAL_INT(14, force_line_paired_day(15));
}

static void test_paired_day_13_returns_16(void)
{
    TEST_ASSERT_EQUAL_INT(16, force_line_paired_day(13));
}

static void test_paired_day_16_returns_13(void)
{
    TEST_ASSERT_EQUAL_INT(13, force_line_paired_day(16));
}

static void test_paired_day_6_returns_23(void)
{
    TEST_ASSERT_EQUAL_INT(23, force_line_paired_day(6));
}

static void test_paired_day_12_returns_17(void)
{
    TEST_ASSERT_EQUAL_INT(17, force_line_paired_day(12));
}

/* ================================================================
 * Paired day — all 28 days: paired(paired(day)) == day
 * ================================================================ */

static void test_paired_day_double_inversion(void)
{
    for (int d = 1; d <= 28; d++) {
        int paired = force_line_paired_day(d);
        TEST_ASSERT_EQUAL_INT(d, force_line_paired_day(paired));
    }
}

/* ================================================================
 * Paired day — all pairs sum to 29
 * ================================================================ */

static void test_paired_day_all_sum_29(void)
{
    for (int d = 1; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(29, d + force_line_paired_day(d));
    }
}

/* ================================================================
 * Paired day — invalid inputs return 0
 * ================================================================ */

static void test_paired_day_0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, force_line_paired_day(0));
}

static void test_paired_day_29_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, force_line_paired_day(29));
}

static void test_paired_day_neg1_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, force_line_paired_day(-1));
}

static void test_paired_day_100_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, force_line_paired_day(100));
}

/* ================================================================
 * Vertical for day — upper days (1-12) return correct line
 * ================================================================ */

static void test_for_day_1_upper(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(1);
    TEST_ASSERT_EQUAL_INT(1, v.upper_day);
    TEST_ASSERT_EQUAL_INT(28, v.lower_day);
    TEST_ASSERT_EQUAL_STRING("Tower of Spirit / Tower of Babel", v.meaning);
}

static void test_for_day_6_upper(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(6);
    TEST_ASSERT_EQUAL_INT(6, v.upper_day);
    TEST_ASSERT_EQUAL_INT(23, v.lower_day);
    TEST_ASSERT_EQUAL_STRING("Tower of Navigation / Tower of Prophecy",
                             v.meaning);
}

static void test_for_day_12_upper(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(12);
    TEST_ASSERT_EQUAL_INT(12, v.upper_day);
    TEST_ASSERT_EQUAL_INT(17, v.lower_day);
}

/* ================================================================
 * Vertical for day — lower days (17-28) return correct line
 * ================================================================ */

static void test_for_day_28_lower(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(28);
    TEST_ASSERT_EQUAL_INT(1, v.upper_day);
    TEST_ASSERT_EQUAL_INT(28, v.lower_day);
    TEST_ASSERT_EQUAL_STRING("Tower of Spirit / Tower of Babel", v.meaning);
}

static void test_for_day_23_lower(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(23);
    TEST_ASSERT_EQUAL_INT(6, v.upper_day);
    TEST_ASSERT_EQUAL_INT(23, v.lower_day);
}

static void test_for_day_17_lower(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(17);
    TEST_ASSERT_EQUAL_INT(12, v.upper_day);
    TEST_ASSERT_EQUAL_INT(17, v.lower_day);
}

static void test_for_day_22_lower(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(22);
    TEST_ASSERT_EQUAL_INT(7, v.upper_day);
    TEST_ASSERT_EQUAL_INT(22, v.lower_day);
    TEST_ASSERT_EQUAL_STRING("Buddha — New Jerusalem", v.meaning);
}

static void test_for_day_19_lower(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(19);
    TEST_ASSERT_EQUAL_INT(10, v.upper_day);
    TEST_ASSERT_EQUAL_INT(19, v.lower_day);
}

/* ================================================================
 * Vertical for day — upper and lower return same line
 * ================================================================ */

static void test_for_day_upper_and_lower_match(void)
{
    for (int i = 0; i < 12; i++) {
        force_line_vertical_t from_upper = force_line_vertical_get(i);
        force_line_vertical_t via_upper =
            force_line_vertical_for_day(from_upper.upper_day);
        force_line_vertical_t via_lower =
            force_line_vertical_for_day(from_upper.lower_day);

        TEST_ASSERT_EQUAL_INT(via_upper.upper_day, via_lower.upper_day);
        TEST_ASSERT_EQUAL_INT(via_upper.lower_day, via_lower.lower_day);
        TEST_ASSERT_EQUAL_STRING(via_upper.meaning, via_lower.meaning);
    }
}

/* ================================================================
 * Vertical for day — days 13-16 return zeroed (center of cube)
 * ================================================================ */

static void test_for_day_13_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(13);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_EQUAL_INT(0, v.lower_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_for_day_14_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(14);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_EQUAL_INT(0, v.lower_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_for_day_15_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(15);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_for_day_16_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(16);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_NULL(v.meaning);
}

/* ================================================================
 * Vertical for day — invalid days return zeroed
 * ================================================================ */

static void test_for_day_0_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(0);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_for_day_29_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(29);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_NULL(v.meaning);
}

static void test_for_day_neg1_zeroed(void)
{
    force_line_vertical_t v = force_line_vertical_for_day(-1);
    TEST_ASSERT_EQUAL_INT(0, v.upper_day);
    TEST_ASSERT_NULL(v.meaning);
}

/* ================================================================
 * Horizontal count
 * ================================================================ */

static void test_horizontal_count_is_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, force_line_horizontal_count());
}

/* ================================================================
 * Horizontal 0: Circuit 1, Pluto-Mercury
 * ================================================================ */

static void test_horizontal0_circuit(void)
{
    TEST_ASSERT_EQUAL_INT(1, force_line_horizontal_get(0).circuit);
}

static void test_horizontal0_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Pluto", force_line_horizontal_get(0).planet_a);
}

static void test_horizontal0_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Mercury", force_line_horizontal_get(0).planet_b);
}

static void test_horizontal0_seal_codes(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(0);
    TEST_ASSERT_EQUAL_INT(0, h.seal_codes[0]);
    TEST_ASSERT_EQUAL_INT(19, h.seal_codes[1]);
    TEST_ASSERT_EQUAL_INT(9, h.seal_codes[2]);
    TEST_ASSERT_EQUAL_INT(10, h.seal_codes[3]);
}

static void test_horizontal0_powers(void)
{
    TEST_ASSERT_EQUAL_STRING(
        "Illumination, autogeneration, purification, love",
        force_line_horizontal_get(0).powers);
}

/* ================================================================
 * Horizontal 1: Circuit 2, Neptune-Venus
 * ================================================================ */

static void test_horizontal1_circuit(void)
{
    TEST_ASSERT_EQUAL_INT(2, force_line_horizontal_get(1).circuit);
}

static void test_horizontal1_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Neptune", force_line_horizontal_get(1).planet_a);
}

static void test_horizontal1_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Venus", force_line_horizontal_get(1).planet_b);
}

static void test_horizontal1_seal_codes(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(1);
    TEST_ASSERT_EQUAL_INT(1, h.seal_codes[0]);
    TEST_ASSERT_EQUAL_INT(18, h.seal_codes[1]);
    TEST_ASSERT_EQUAL_INT(8, h.seal_codes[2]);
    TEST_ASSERT_EQUAL_INT(11, h.seal_codes[3]);
}

static void test_horizontal1_powers(void)
{
    TEST_ASSERT_EQUAL_STRING("Memory, meditation, art, magic",
                             force_line_horizontal_get(1).powers);
}

/* ================================================================
 * Horizontal 2: Circuit 3, Uranus-Earth
 * ================================================================ */

static void test_horizontal2_circuit(void)
{
    TEST_ASSERT_EQUAL_INT(3, force_line_horizontal_get(2).circuit);
}

static void test_horizontal2_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Uranus", force_line_horizontal_get(2).planet_a);
}

static void test_horizontal2_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", force_line_horizontal_get(2).planet_b);
}

static void test_horizontal2_seal_codes(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(2);
    TEST_ASSERT_EQUAL_INT(2, h.seal_codes[0]);
    TEST_ASSERT_EQUAL_INT(17, h.seal_codes[1]);
    TEST_ASSERT_EQUAL_INT(7, h.seal_codes[2]);
    TEST_ASSERT_EQUAL_INT(12, h.seal_codes[3]);
}

static void test_horizontal2_powers(void)
{
    TEST_ASSERT_EQUAL_STRING("Spirit, navigation, realization, free will",
                             force_line_horizontal_get(2).powers);
}

/* ================================================================
 * Horizontal 3: Circuit 4, Saturn-Mars
 * ================================================================ */

static void test_horizontal3_circuit(void)
{
    TEST_ASSERT_EQUAL_INT(4, force_line_horizontal_get(3).circuit);
}

static void test_horizontal3_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Saturn", force_line_horizontal_get(3).planet_a);
}

static void test_horizontal3_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Mars", force_line_horizontal_get(3).planet_b);
}

static void test_horizontal3_seal_codes(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(3);
    TEST_ASSERT_EQUAL_INT(3, h.seal_codes[0]);
    TEST_ASSERT_EQUAL_INT(16, h.seal_codes[1]);
    TEST_ASSERT_EQUAL_INT(6, h.seal_codes[2]);
    TEST_ASSERT_EQUAL_INT(13, h.seal_codes[3]);
}

static void test_horizontal3_powers(void)
{
    TEST_ASSERT_EQUAL_STRING("Abundance, intelligence, death, prophecy",
                             force_line_horizontal_get(3).powers);
}

/* ================================================================
 * Horizontal 4: Circuit 5, Jupiter-Maldek
 * ================================================================ */

static void test_horizontal4_circuit(void)
{
    TEST_ASSERT_EQUAL_INT(5, force_line_horizontal_get(4).circuit);
}

static void test_horizontal4_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Jupiter", force_line_horizontal_get(4).planet_a);
}

static void test_horizontal4_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Maldek", force_line_horizontal_get(4).planet_b);
}

static void test_horizontal4_seal_codes(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(4);
    TEST_ASSERT_EQUAL_INT(4, h.seal_codes[0]);
    TEST_ASSERT_EQUAL_INT(15, h.seal_codes[1]);
    TEST_ASSERT_EQUAL_INT(5, h.seal_codes[2]);
    TEST_ASSERT_EQUAL_INT(14, h.seal_codes[3]);
}

static void test_horizontal4_powers(void)
{
    TEST_ASSERT_EQUAL_STRING("Flowering, vision, sex, atemporality",
                             force_line_horizontal_get(4).powers);
}

/* ================================================================
 * Horizontal get — boundary: invalid indices return zeroed
 * ================================================================ */

static void test_horizontal_get_neg1_zeroed(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(-1);
    TEST_ASSERT_EQUAL_INT(0, h.circuit);
    TEST_ASSERT_NULL(h.planet_a);
    TEST_ASSERT_NULL(h.planet_b);
    TEST_ASSERT_NULL(h.powers);
    TEST_ASSERT_EQUAL_INT(0, h.seal_codes[0]);
}

static void test_horizontal_get_5_zeroed(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(5);
    TEST_ASSERT_EQUAL_INT(0, h.circuit);
    TEST_ASSERT_NULL(h.planet_a);
    TEST_ASSERT_NULL(h.planet_b);
    TEST_ASSERT_NULL(h.powers);
}

static void test_horizontal_get_99_zeroed(void)
{
    force_line_horizontal_t h = force_line_horizontal_get(99);
    TEST_ASSERT_EQUAL_INT(0, h.circuit);
    TEST_ASSERT_NULL(h.planet_a);
}

/* ================================================================
 * Horizontal — circuits are 1-5 in order
 * ================================================================ */

static void test_horizontal_circuits_sequential(void)
{
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, force_line_horizontal_get(i).circuit);
    }
}

/* ================================================================
 * Horizontal — all have valid planet strings
 * ================================================================ */

static void test_horizontal_all_planets_valid(void)
{
    for (int i = 0; i < 5; i++) {
        force_line_horizontal_t h = force_line_horizontal_get(i);
        TEST_ASSERT_NOT_NULL(h.planet_a);
        TEST_ASSERT_NOT_NULL(h.planet_b);
        TEST_ASSERT_TRUE(strlen(h.planet_a) > 0);
        TEST_ASSERT_TRUE(strlen(h.planet_b) > 0);
    }
}

/* ================================================================
 * Horizontal — all have valid powers strings
 * ================================================================ */

static void test_horizontal_all_powers_valid(void)
{
    for (int i = 0; i < 5; i++) {
        force_line_horizontal_t h = force_line_horizontal_get(i);
        TEST_ASSERT_NOT_NULL(h.powers);
        TEST_ASSERT_TRUE(strlen(h.powers) > 0);
    }
}

/* ================================================================
 * Horizontal — seal codes pattern: first codes are 0,1,2,3,4
 * ================================================================ */

static void test_horizontal_first_seal_codes_ascending(void)
{
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(i, force_line_horizontal_get(i).seal_codes[0]);
    }
}

/* ================================================================
 * Horizontal — seal code pairs sum to 19 (codes[0]+codes[1])
 * ================================================================ */

static void test_horizontal_seal_code_pairs_sum_19(void)
{
    for (int i = 0; i < 5; i++) {
        force_line_horizontal_t h = force_line_horizontal_get(i);
        TEST_ASSERT_EQUAL_INT(19, h.seal_codes[0] + h.seal_codes[1]);
        TEST_ASSERT_EQUAL_INT(19, h.seal_codes[2] + h.seal_codes[3]);
    }
}

/* ================================================================
 * Horizontal — all 20 seal codes are unique (0-19 each once)
 * ================================================================ */

static void test_horizontal_all_seal_codes_unique(void)
{
    int seen[20];
    memset(seen, 0, sizeof(seen));
    for (int i = 0; i < 5; i++) {
        force_line_horizontal_t h = force_line_horizontal_get(i);
        for (int j = 0; j < 4; j++) {
            int code = h.seal_codes[j];
            TEST_ASSERT_TRUE(code >= 0 && code <= 19);
            TEST_ASSERT_EQUAL_INT(0, seen[code]);
            seen[code] = 1;
        }
    }
    /* All 20 codes should have been seen */
    for (int c = 0; c < 20; c++) {
        TEST_ASSERT_EQUAL_INT(1, seen[c]);
    }
}

/* ================================================================
 * Purity — same input, same output
 * ================================================================ */

static void test_purity_vertical_same_result(void)
{
    force_line_vertical_t a = force_line_vertical_get(5);
    force_line_vertical_t b = force_line_vertical_get(5);
    TEST_ASSERT_EQUAL_INT(a.upper_day, b.upper_day);
    TEST_ASSERT_EQUAL_INT(a.lower_day, b.lower_day);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
}

static void test_purity_horizontal_same_result(void)
{
    force_line_horizontal_t a = force_line_horizontal_get(2);
    force_line_horizontal_t b = force_line_horizontal_get(2);
    TEST_ASSERT_EQUAL_INT(a.circuit, b.circuit);
    TEST_ASSERT_EQUAL_STRING(a.planet_a, b.planet_a);
    TEST_ASSERT_EQUAL_STRING(a.planet_b, b.planet_b);
    TEST_ASSERT_EQUAL_STRING(a.powers, b.powers);
    for (int j = 0; j < 4; j++) {
        TEST_ASSERT_EQUAL_INT(a.seal_codes[j], b.seal_codes[j]);
    }
}

static void test_purity_paired_day_same_result(void)
{
    TEST_ASSERT_EQUAL_INT(force_line_paired_day(7), force_line_paired_day(7));
}

static void test_purity_for_day_same_result(void)
{
    force_line_vertical_t a = force_line_vertical_for_day(21);
    force_line_vertical_t b = force_line_vertical_for_day(21);
    TEST_ASSERT_EQUAL_INT(a.upper_day, b.upper_day);
    TEST_ASSERT_EQUAL_INT(a.lower_day, b.lower_day);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
}

/* ================================================================
 * Cross-check: for_day matches get for all upper days
 * ================================================================ */

static void test_for_day_matches_get_all_upper(void)
{
    for (int i = 0; i < 12; i++) {
        force_line_vertical_t from_get = force_line_vertical_get(i);
        force_line_vertical_t from_day =
            force_line_vertical_for_day(from_get.upper_day);
        TEST_ASSERT_EQUAL_INT(from_get.upper_day, from_day.upper_day);
        TEST_ASSERT_EQUAL_INT(from_get.lower_day, from_day.lower_day);
        TEST_ASSERT_EQUAL_STRING(from_get.meaning, from_day.meaning);
    }
}

/* ================================================================
 * Cross-check: for_day matches get for all lower days
 * ================================================================ */

static void test_for_day_matches_get_all_lower(void)
{
    for (int i = 0; i < 12; i++) {
        force_line_vertical_t from_get = force_line_vertical_get(i);
        force_line_vertical_t from_day =
            force_line_vertical_for_day(from_get.lower_day);
        TEST_ASSERT_EQUAL_INT(from_get.upper_day, from_day.upper_day);
        TEST_ASSERT_EQUAL_INT(from_get.lower_day, from_day.lower_day);
        TEST_ASSERT_EQUAL_STRING(from_get.meaning, from_day.meaning);
    }
}

/* ================================================================
 * main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Vertical count */
    RUN_TEST(test_vertical_count_is_12);

    /* Vertical line 0 — days 1-28 */
    RUN_TEST(test_vertical0_upper_day);
    RUN_TEST(test_vertical0_lower_day);
    RUN_TEST(test_vertical0_meaning);

    /* Vertical line 1 — days 2-27 */
    RUN_TEST(test_vertical1_upper_day);
    RUN_TEST(test_vertical1_lower_day);
    RUN_TEST(test_vertical1_meaning);

    /* Vertical line 2 — days 3-26 */
    RUN_TEST(test_vertical2_upper_day);
    RUN_TEST(test_vertical2_lower_day);
    RUN_TEST(test_vertical2_meaning);

    /* Vertical line 3 — days 4-25 */
    RUN_TEST(test_vertical3_upper_day);
    RUN_TEST(test_vertical3_lower_day);
    RUN_TEST(test_vertical3_meaning);

    /* Vertical line 4 — days 5-24 */
    RUN_TEST(test_vertical4_upper_day);
    RUN_TEST(test_vertical4_lower_day);
    RUN_TEST(test_vertical4_meaning);

    /* Vertical line 5 — days 6-23 */
    RUN_TEST(test_vertical5_upper_day);
    RUN_TEST(test_vertical5_lower_day);
    RUN_TEST(test_vertical5_meaning);

    /* Vertical line 6 — days 7-22 */
    RUN_TEST(test_vertical6_upper_day);
    RUN_TEST(test_vertical6_lower_day);
    RUN_TEST(test_vertical6_meaning);

    /* Vertical line 7 — days 8-21 */
    RUN_TEST(test_vertical7_upper_day);
    RUN_TEST(test_vertical7_lower_day);
    RUN_TEST(test_vertical7_meaning);

    /* Vertical line 8 — days 9-20 */
    RUN_TEST(test_vertical8_upper_day);
    RUN_TEST(test_vertical8_lower_day);
    RUN_TEST(test_vertical8_meaning);

    /* Vertical line 9 — days 10-19 */
    RUN_TEST(test_vertical9_upper_day);
    RUN_TEST(test_vertical9_lower_day);
    RUN_TEST(test_vertical9_meaning);

    /* Vertical line 10 — days 11-18 */
    RUN_TEST(test_vertical10_upper_day);
    RUN_TEST(test_vertical10_lower_day);
    RUN_TEST(test_vertical10_meaning);

    /* Vertical line 11 — days 12-17 */
    RUN_TEST(test_vertical11_upper_day);
    RUN_TEST(test_vertical11_lower_day);
    RUN_TEST(test_vertical11_meaning);

    /* Vertical get — boundaries */
    RUN_TEST(test_vertical_get_neg1_zeroed);
    RUN_TEST(test_vertical_get_12_zeroed);
    RUN_TEST(test_vertical_get_99_zeroed);

    /* Vertical sweeps */
    RUN_TEST(test_vertical_all_pairs_sum_29);
    RUN_TEST(test_vertical_upper_days_sequential);
    RUN_TEST(test_vertical_lower_days_descending);
    RUN_TEST(test_vertical_all_meanings_valid);

    /* Paired day — specific values */
    RUN_TEST(test_paired_day_1_returns_28);
    RUN_TEST(test_paired_day_28_returns_1);
    RUN_TEST(test_paired_day_7_returns_22);
    RUN_TEST(test_paired_day_14_returns_15);
    RUN_TEST(test_paired_day_15_returns_14);
    RUN_TEST(test_paired_day_13_returns_16);
    RUN_TEST(test_paired_day_16_returns_13);
    RUN_TEST(test_paired_day_6_returns_23);
    RUN_TEST(test_paired_day_12_returns_17);

    /* Paired day — sweeps */
    RUN_TEST(test_paired_day_double_inversion);
    RUN_TEST(test_paired_day_all_sum_29);

    /* Paired day — boundaries */
    RUN_TEST(test_paired_day_0_returns_0);
    RUN_TEST(test_paired_day_29_returns_0);
    RUN_TEST(test_paired_day_neg1_returns_0);
    RUN_TEST(test_paired_day_100_returns_0);

    /* Vertical for day — upper days */
    RUN_TEST(test_for_day_1_upper);
    RUN_TEST(test_for_day_6_upper);
    RUN_TEST(test_for_day_12_upper);

    /* Vertical for day — lower days */
    RUN_TEST(test_for_day_28_lower);
    RUN_TEST(test_for_day_23_lower);
    RUN_TEST(test_for_day_17_lower);
    RUN_TEST(test_for_day_22_lower);
    RUN_TEST(test_for_day_19_lower);

    /* Vertical for day — upper and lower match */
    RUN_TEST(test_for_day_upper_and_lower_match);

    /* Vertical for day — days 13-16 zeroed */
    RUN_TEST(test_for_day_13_zeroed);
    RUN_TEST(test_for_day_14_zeroed);
    RUN_TEST(test_for_day_15_zeroed);
    RUN_TEST(test_for_day_16_zeroed);

    /* Vertical for day — invalid days zeroed */
    RUN_TEST(test_for_day_0_zeroed);
    RUN_TEST(test_for_day_29_zeroed);
    RUN_TEST(test_for_day_neg1_zeroed);

    /* Horizontal count */
    RUN_TEST(test_horizontal_count_is_5);

    /* Horizontal 0 — Pluto-Mercury */
    RUN_TEST(test_horizontal0_circuit);
    RUN_TEST(test_horizontal0_planet_a);
    RUN_TEST(test_horizontal0_planet_b);
    RUN_TEST(test_horizontal0_seal_codes);
    RUN_TEST(test_horizontal0_powers);

    /* Horizontal 1 — Neptune-Venus */
    RUN_TEST(test_horizontal1_circuit);
    RUN_TEST(test_horizontal1_planet_a);
    RUN_TEST(test_horizontal1_planet_b);
    RUN_TEST(test_horizontal1_seal_codes);
    RUN_TEST(test_horizontal1_powers);

    /* Horizontal 2 — Uranus-Earth */
    RUN_TEST(test_horizontal2_circuit);
    RUN_TEST(test_horizontal2_planet_a);
    RUN_TEST(test_horizontal2_planet_b);
    RUN_TEST(test_horizontal2_seal_codes);
    RUN_TEST(test_horizontal2_powers);

    /* Horizontal 3 — Saturn-Mars */
    RUN_TEST(test_horizontal3_circuit);
    RUN_TEST(test_horizontal3_planet_a);
    RUN_TEST(test_horizontal3_planet_b);
    RUN_TEST(test_horizontal3_seal_codes);
    RUN_TEST(test_horizontal3_powers);

    /* Horizontal 4 — Jupiter-Maldek */
    RUN_TEST(test_horizontal4_circuit);
    RUN_TEST(test_horizontal4_planet_a);
    RUN_TEST(test_horizontal4_planet_b);
    RUN_TEST(test_horizontal4_seal_codes);
    RUN_TEST(test_horizontal4_powers);

    /* Horizontal boundaries */
    RUN_TEST(test_horizontal_get_neg1_zeroed);
    RUN_TEST(test_horizontal_get_5_zeroed);
    RUN_TEST(test_horizontal_get_99_zeroed);

    /* Horizontal sweeps */
    RUN_TEST(test_horizontal_circuits_sequential);
    RUN_TEST(test_horizontal_all_planets_valid);
    RUN_TEST(test_horizontal_all_powers_valid);
    RUN_TEST(test_horizontal_first_seal_codes_ascending);
    RUN_TEST(test_horizontal_seal_code_pairs_sum_19);
    RUN_TEST(test_horizontal_all_seal_codes_unique);

    /* Purity */
    RUN_TEST(test_purity_vertical_same_result);
    RUN_TEST(test_purity_horizontal_same_result);
    RUN_TEST(test_purity_paired_day_same_result);
    RUN_TEST(test_purity_for_day_same_result);

    /* Cross-checks */
    RUN_TEST(test_for_day_matches_get_all_upper);
    RUN_TEST(test_for_day_matches_get_all_lower);

    return UNITY_END();
}
