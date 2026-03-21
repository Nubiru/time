#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/telektonon_board.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Circuit count & total units
 * ================================================================ */

static void test_circuit_count_is_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, tk_circuit_count());
}

static void test_total_units_is_140(void)
{
    TEST_ASSERT_EQUAL_INT(140, tk_total_units());
}

static void test_tube_day_count_is_28(void)
{
    TEST_ASSERT_EQUAL_INT(28, tk_tube_day_count());
}

/* Verify sum of circuit units equals total */
static void test_circuit_units_sum_to_140(void)
{
    int sum = 0;
    for (int i = 1; i <= 5; i++) {
        sum += tk_circuit_get(i).units;
    }
    TEST_ASSERT_EQUAL_INT(140, sum);
}

/* ================================================================
 * Circuit 1: Pluto-Mercury, Alpha-Omega Recharge, 36 units
 * ================================================================ */

static void test_circuit1_number(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_circuit_get(1).number);
}

static void test_circuit1_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Pluto", tk_circuit_get(1).planet_a);
}

static void test_circuit1_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Mercury", tk_circuit_get(1).planet_b);
}

static void test_circuit1_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Alpha-Omega Recharge", tk_circuit_get(1).name);
}

static void test_circuit1_units(void)
{
    TEST_ASSERT_EQUAL_INT(36, tk_circuit_get(1).units);
}

/* ================================================================
 * Circuit 2: Neptune-Venus, Allied Memory-Instinct, 32 units
 * ================================================================ */

static void test_circuit2_number(void)
{
    TEST_ASSERT_EQUAL_INT(2, tk_circuit_get(2).number);
}

static void test_circuit2_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Neptune", tk_circuit_get(2).planet_a);
}

static void test_circuit2_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Venus", tk_circuit_get(2).planet_b);
}

static void test_circuit2_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Allied Memory-Instinct", tk_circuit_get(2).name);
}

static void test_circuit2_units(void)
{
    TEST_ASSERT_EQUAL_INT(32, tk_circuit_get(2).units);
}

/* ================================================================
 * Circuit 3: Uranus-Earth, Telektonon, 28 units
 * ================================================================ */

static void test_circuit3_number(void)
{
    TEST_ASSERT_EQUAL_INT(3, tk_circuit_get(3).number);
}

static void test_circuit3_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Uranus", tk_circuit_get(3).planet_a);
}

static void test_circuit3_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", tk_circuit_get(3).planet_b);
}

static void test_circuit3_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Telektonon", tk_circuit_get(3).name);
}

static void test_circuit3_units(void)
{
    TEST_ASSERT_EQUAL_INT(28, tk_circuit_get(3).units);
}

/* ================================================================
 * Circuit 4: Saturn-Mars, Externalized Intelligence, 24 units
 * ================================================================ */

static void test_circuit4_number(void)
{
    TEST_ASSERT_EQUAL_INT(4, tk_circuit_get(4).number);
}

static void test_circuit4_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Saturn", tk_circuit_get(4).planet_a);
}

static void test_circuit4_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Mars", tk_circuit_get(4).planet_b);
}

static void test_circuit4_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Externalized Intelligence", tk_circuit_get(4).name);
}

static void test_circuit4_units(void)
{
    TEST_ASSERT_EQUAL_INT(24, tk_circuit_get(4).units);
}

/* ================================================================
 * Circuit 5: Jupiter-Maldek, Internalized Intelligence, 20 units
 * ================================================================ */

static void test_circuit5_number(void)
{
    TEST_ASSERT_EQUAL_INT(5, tk_circuit_get(5).number);
}

static void test_circuit5_planet_a(void)
{
    TEST_ASSERT_EQUAL_STRING("Jupiter", tk_circuit_get(5).planet_a);
}

static void test_circuit5_planet_b(void)
{
    TEST_ASSERT_EQUAL_STRING("Maldek", tk_circuit_get(5).planet_b);
}

static void test_circuit5_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Internalized Intelligence", tk_circuit_get(5).name);
}

static void test_circuit5_units(void)
{
    TEST_ASSERT_EQUAL_INT(20, tk_circuit_get(5).units);
}

/* ================================================================
 * Circuit boundaries — invalid numbers
 * ================================================================ */

static void test_circuit0_returns_zeroed(void)
{
    tk_circuit_t c = tk_circuit_get(0);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet_a);
    TEST_ASSERT_NULL(c.planet_b);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

static void test_circuit6_returns_zeroed(void)
{
    tk_circuit_t c = tk_circuit_get(6);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet_a);
    TEST_ASSERT_NULL(c.planet_b);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

static void test_circuit_negative_returns_zeroed(void)
{
    tk_circuit_t c = tk_circuit_get(-1);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet_a);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

/* ================================================================
 * Phase classification — Earth Walk (days 1-6)
 * ================================================================ */

static void test_phase_day1_earth_walk(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_EARTH_WALK, tk_phase_for_day(1));
}

static void test_phase_day3_earth_walk(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_EARTH_WALK, tk_phase_for_day(3));
}

static void test_phase_day6_earth_walk(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_EARTH_WALK, tk_phase_for_day(6));
}

/* ================================================================
 * Phase classification — Warrior's Cube (days 7-22)
 * ================================================================ */

static void test_phase_day7_warriors_cube(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_WARRIORS_CUBE, tk_phase_for_day(7));
}

static void test_phase_day14_warriors_cube(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_WARRIORS_CUBE, tk_phase_for_day(14));
}

static void test_phase_day22_warriors_cube(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_WARRIORS_CUBE, tk_phase_for_day(22));
}

/* ================================================================
 * Phase classification — Heaven Walk (days 23-28)
 * ================================================================ */

static void test_phase_day23_heaven_walk(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_HEAVEN_WALK, tk_phase_for_day(23));
}

static void test_phase_day25_heaven_walk(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_HEAVEN_WALK, tk_phase_for_day(25));
}

static void test_phase_day28_heaven_walk(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_HEAVEN_WALK, tk_phase_for_day(28));
}

/* ================================================================
 * Phase classification — invalid days
 * ================================================================ */

static void test_phase_day0_none(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_NONE, tk_phase_for_day(0));
}

static void test_phase_day29_none(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_NONE, tk_phase_for_day(29));
}

static void test_phase_negative_none(void)
{
    TEST_ASSERT_EQUAL_INT(TK_PHASE_NONE, tk_phase_for_day(-1));
}

/* ================================================================
 * Phase sweep — all 28 days have correct phase
 * ================================================================ */

static void test_phase_sweep_earth_walk(void)
{
    for (int d = 1; d <= 6; d++) {
        TEST_ASSERT_EQUAL_INT(TK_PHASE_EARTH_WALK, tk_phase_for_day(d));
    }
}

static void test_phase_sweep_warriors_cube(void)
{
    for (int d = 7; d <= 22; d++) {
        TEST_ASSERT_EQUAL_INT(TK_PHASE_WARRIORS_CUBE, tk_phase_for_day(d));
    }
}

static void test_phase_sweep_heaven_walk(void)
{
    for (int d = 23; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(TK_PHASE_HEAVEN_WALK, tk_phase_for_day(d));
    }
}

/* ================================================================
 * Phase names
 * ================================================================ */

static void test_phase_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("None", tk_phase_name(TK_PHASE_NONE));
}

static void test_phase_name_earth_walk(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth Walk", tk_phase_name(TK_PHASE_EARTH_WALK));
}

static void test_phase_name_warriors_cube(void)
{
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", tk_phase_name(TK_PHASE_WARRIORS_CUBE));
}

static void test_phase_name_heaven_walk(void)
{
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", tk_phase_name(TK_PHASE_HEAVEN_WALK));
}

static void test_phase_name_invalid_returns_none(void)
{
    TEST_ASSERT_EQUAL_STRING("None", tk_phase_name((tk_phase_t)99));
}

/* ================================================================
 * Cube positions — days 7-22 map to 1-16
 * ================================================================ */

static void test_cube_day7_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_cube_position(7));
}

static void test_cube_day8_is_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, tk_cube_position(8));
}

static void test_cube_day14_is_8(void)
{
    TEST_ASSERT_EQUAL_INT(8, tk_cube_position(14));
}

static void test_cube_day22_is_16(void)
{
    TEST_ASSERT_EQUAL_INT(16, tk_cube_position(22));
}

static void test_cube_day6_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_cube_position(6));
}

static void test_cube_day23_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_cube_position(23));
}

static void test_cube_day0_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_cube_position(0));
}

static void test_cube_day_negative_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_cube_position(-5));
}

/* Sweep: all 16 cube positions */
static void test_cube_sweep_all_16(void)
{
    for (int d = 7; d <= 22; d++) {
        int expected = d - 6;
        TEST_ASSERT_EQUAL_INT(expected, tk_cube_position(d));
    }
}

/* Non-cube days all return 0 */
static void test_cube_sweep_non_cube_zero(void)
{
    for (int d = 1; d <= 6; d++) {
        TEST_ASSERT_EQUAL_INT(0, tk_cube_position(d));
    }
    for (int d = 23; d <= 28; d++) {
        TEST_ASSERT_EQUAL_INT(0, tk_cube_position(d));
    }
}

/* ================================================================
 * Tower days: 1, 6, 23, 28
 * ================================================================ */

static void test_tower_day1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_day(1));
}

static void test_tower_day6(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_day(6));
}

static void test_tower_day23(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_day(23));
}

static void test_tower_day28(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_day(28));
}

static void test_tower_day2_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_day(2));
}

static void test_tower_day7_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_day(7));
}

static void test_tower_day22_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_day(22));
}

static void test_tower_day0_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_day(0));
}

static void test_tower_day29_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_day(29));
}

static void test_tower_negative_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_day(-1));
}

/* Sweep: exactly 4 tower days */
static void test_tower_sweep_count(void)
{
    int count = 0;
    for (int d = 1; d <= 28; d++) {
        count += tk_is_tower_day(d);
    }
    TEST_ASSERT_EQUAL_INT(4, count);
}

/* ================================================================
 * Reunion day: day 23 only
 * ================================================================ */

static void test_reunion_day23(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_reunion_day(23));
}

static void test_reunion_day1_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_reunion_day(1));
}

static void test_reunion_day22_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_reunion_day(22));
}

static void test_reunion_day24_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_reunion_day(24));
}

static void test_reunion_day0_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_reunion_day(0));
}

static void test_reunion_negative_not(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_reunion_day(-1));
}

/* Sweep: exactly 1 reunion day */
static void test_reunion_sweep_count(void)
{
    int count = 0;
    for (int d = 1; d <= 28; d++) {
        count += tk_is_reunion_day(d);
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* ================================================================
 * Stairway to Heaven: days 13-16
 * ================================================================ */

static void test_stairway_day13(void)
{
    tk_day_info_t info = tk_day_info(13);
    TEST_ASSERT_EQUAL_INT(1, info.is_stairway);
}

static void test_stairway_day14(void)
{
    tk_day_info_t info = tk_day_info(14);
    TEST_ASSERT_EQUAL_INT(1, info.is_stairway);
}

static void test_stairway_day15(void)
{
    tk_day_info_t info = tk_day_info(15);
    TEST_ASSERT_EQUAL_INT(1, info.is_stairway);
}

static void test_stairway_day16(void)
{
    tk_day_info_t info = tk_day_info(16);
    TEST_ASSERT_EQUAL_INT(1, info.is_stairway);
}

static void test_stairway_day12_not(void)
{
    tk_day_info_t info = tk_day_info(12);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

static void test_stairway_day17_not(void)
{
    tk_day_info_t info = tk_day_info(17);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

/* Sweep: exactly 4 stairway days */
static void test_stairway_sweep_count(void)
{
    int count = 0;
    for (int d = 1; d <= 28; d++) {
        tk_day_info_t info = tk_day_info(d);
        count += info.is_stairway;
    }
    TEST_ASSERT_EQUAL_INT(4, count);
}

/* ================================================================
 * Day info — full struct verification
 * ================================================================ */

static void test_day_info_day1_full(void)
{
    tk_day_info_t info = tk_day_info(1);
    TEST_ASSERT_EQUAL_INT(1, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_EARTH_WALK, info.phase);
    TEST_ASSERT_EQUAL_INT(0, info.cube_position);
    TEST_ASSERT_EQUAL_INT(1, info.is_tower_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

static void test_day_info_day7_full(void)
{
    tk_day_info_t info = tk_day_info(7);
    TEST_ASSERT_EQUAL_INT(7, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_WARRIORS_CUBE, info.phase);
    TEST_ASSERT_EQUAL_INT(1, info.cube_position);
    TEST_ASSERT_EQUAL_INT(0, info.is_tower_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

static void test_day_info_day13_full(void)
{
    tk_day_info_t info = tk_day_info(13);
    TEST_ASSERT_EQUAL_INT(13, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_WARRIORS_CUBE, info.phase);
    TEST_ASSERT_EQUAL_INT(7, info.cube_position);
    TEST_ASSERT_EQUAL_INT(0, info.is_tower_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(1, info.is_stairway);
}

static void test_day_info_day23_full(void)
{
    tk_day_info_t info = tk_day_info(23);
    TEST_ASSERT_EQUAL_INT(23, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_HEAVEN_WALK, info.phase);
    TEST_ASSERT_EQUAL_INT(0, info.cube_position);
    TEST_ASSERT_EQUAL_INT(1, info.is_tower_day);
    TEST_ASSERT_EQUAL_INT(1, info.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

static void test_day_info_day28_full(void)
{
    tk_day_info_t info = tk_day_info(28);
    TEST_ASSERT_EQUAL_INT(28, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_HEAVEN_WALK, info.phase);
    TEST_ASSERT_EQUAL_INT(0, info.cube_position);
    TEST_ASSERT_EQUAL_INT(1, info.is_tower_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

/* ================================================================
 * Day info — boundaries
 * ================================================================ */

static void test_day_info_day0_zeroed(void)
{
    tk_day_info_t info = tk_day_info(0);
    TEST_ASSERT_EQUAL_INT(0, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_NONE, info.phase);
    TEST_ASSERT_EQUAL_INT(0, info.cube_position);
    TEST_ASSERT_EQUAL_INT(0, info.is_tower_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(0, info.is_stairway);
}

static void test_day_info_day29_zeroed(void)
{
    tk_day_info_t info = tk_day_info(29);
    TEST_ASSERT_EQUAL_INT(0, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_NONE, info.phase);
    TEST_ASSERT_EQUAL_INT(0, info.cube_position);
}

static void test_day_info_negative_zeroed(void)
{
    tk_day_info_t info = tk_day_info(-1);
    TEST_ASSERT_EQUAL_INT(0, info.day);
    TEST_ASSERT_EQUAL_INT(TK_PHASE_NONE, info.phase);
    TEST_ASSERT_EQUAL_INT(0, info.cube_position);
}

/* ================================================================
 * Purity — same input, same output
 * ================================================================ */

static void test_purity_circuit_same_result(void)
{
    tk_circuit_t a = tk_circuit_get(3);
    tk_circuit_t b = tk_circuit_get(3);
    TEST_ASSERT_EQUAL_INT(a.number, b.number);
    TEST_ASSERT_EQUAL_INT(a.units, b.units);
    TEST_ASSERT_EQUAL_STRING(a.planet_a, b.planet_a);
    TEST_ASSERT_EQUAL_STRING(a.planet_b, b.planet_b);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
}

static void test_purity_day_info_same_result(void)
{
    tk_day_info_t a = tk_day_info(15);
    tk_day_info_t b = tk_day_info(15);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
    TEST_ASSERT_EQUAL_INT(a.phase, b.phase);
    TEST_ASSERT_EQUAL_INT(a.cube_position, b.cube_position);
    TEST_ASSERT_EQUAL_INT(a.is_tower_day, b.is_tower_day);
    TEST_ASSERT_EQUAL_INT(a.is_reunion_day, b.is_reunion_day);
    TEST_ASSERT_EQUAL_INT(a.is_stairway, b.is_stairway);
}

/* ================================================================
 * Sweep: all circuits have valid strings
 * ================================================================ */

static void test_all_circuits_have_valid_data(void)
{
    for (int i = 1; i <= 5; i++) {
        tk_circuit_t c = tk_circuit_get(i);
        TEST_ASSERT_EQUAL_INT(i, c.number);
        TEST_ASSERT_NOT_NULL(c.planet_a);
        TEST_ASSERT_NOT_NULL(c.planet_b);
        TEST_ASSERT_NOT_NULL(c.name);
        TEST_ASSERT_TRUE(c.units > 0);
        TEST_ASSERT_TRUE(strlen(c.planet_a) > 0);
        TEST_ASSERT_TRUE(strlen(c.planet_b) > 0);
        TEST_ASSERT_TRUE(strlen(c.name) > 0);
    }
}

/* ================================================================
 * Sweep: all 28 days have valid info
 * ================================================================ */

static void test_all_days_have_valid_data(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_day_info_t info = tk_day_info(d);
        TEST_ASSERT_EQUAL_INT(d, info.day);
        TEST_ASSERT_TRUE(info.phase >= TK_PHASE_EARTH_WALK);
        TEST_ASSERT_TRUE(info.phase <= TK_PHASE_HEAVEN_WALK);
    }
}

/* ================================================================
 * Circuit units descend: 36, 32, 28, 24, 20 (step of 4)
 * ================================================================ */

static void test_circuit_units_descend_by_4(void)
{
    for (int i = 1; i < 5; i++) {
        int a = tk_circuit_get(i).units;
        int b = tk_circuit_get(i + 1).units;
        TEST_ASSERT_EQUAL_INT(4, a - b);
    }
}

/* ================================================================
 * Day 23 is both tower AND reunion (unique combination)
 * ================================================================ */

static void test_day23_tower_and_reunion(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_day(23));
    TEST_ASSERT_EQUAL_INT(1, tk_is_reunion_day(23));
}

/* ================================================================
 * Phase name round-trip via day
 * ================================================================ */

static void test_phase_name_round_trip_day5(void)
{
    tk_phase_t p = tk_phase_for_day(5);
    const char *name = tk_phase_name(p);
    TEST_ASSERT_EQUAL_STRING("Earth Walk", name);
}

static void test_phase_name_round_trip_day10(void)
{
    tk_phase_t p = tk_phase_for_day(10);
    const char *name = tk_phase_name(p);
    TEST_ASSERT_EQUAL_STRING("Warrior's Cube", name);
}

static void test_phase_name_round_trip_day26(void)
{
    tk_phase_t p = tk_phase_for_day(26);
    const char *name = tk_phase_name(p);
    TEST_ASSERT_EQUAL_STRING("Heaven Walk", name);
}

/* ================================================================
 * Day info phase matches tk_phase_for_day
 * ================================================================ */

static void test_day_info_phase_matches_phase_for_day(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_day_info_t info = tk_day_info(d);
        TEST_ASSERT_EQUAL_INT(tk_phase_for_day(d), info.phase);
    }
}

/* ================================================================
 * Day info cube_position matches tk_cube_position
 * ================================================================ */

static void test_day_info_cube_matches_cube_position(void)
{
    for (int d = 1; d <= 28; d++) {
        tk_day_info_t info = tk_day_info(d);
        TEST_ASSERT_EQUAL_INT(tk_cube_position(d), info.cube_position);
    }
}

/* ================================================================
 * main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Counts & totals */
    RUN_TEST(test_circuit_count_is_5);
    RUN_TEST(test_total_units_is_140);
    RUN_TEST(test_tube_day_count_is_28);
    RUN_TEST(test_circuit_units_sum_to_140);

    /* Circuit 1 — Pluto-Mercury */
    RUN_TEST(test_circuit1_number);
    RUN_TEST(test_circuit1_planet_a);
    RUN_TEST(test_circuit1_planet_b);
    RUN_TEST(test_circuit1_name);
    RUN_TEST(test_circuit1_units);

    /* Circuit 2 — Neptune-Venus */
    RUN_TEST(test_circuit2_number);
    RUN_TEST(test_circuit2_planet_a);
    RUN_TEST(test_circuit2_planet_b);
    RUN_TEST(test_circuit2_name);
    RUN_TEST(test_circuit2_units);

    /* Circuit 3 — Uranus-Earth */
    RUN_TEST(test_circuit3_number);
    RUN_TEST(test_circuit3_planet_a);
    RUN_TEST(test_circuit3_planet_b);
    RUN_TEST(test_circuit3_name);
    RUN_TEST(test_circuit3_units);

    /* Circuit 4 — Saturn-Mars */
    RUN_TEST(test_circuit4_number);
    RUN_TEST(test_circuit4_planet_a);
    RUN_TEST(test_circuit4_planet_b);
    RUN_TEST(test_circuit4_name);
    RUN_TEST(test_circuit4_units);

    /* Circuit 5 — Jupiter-Maldek */
    RUN_TEST(test_circuit5_number);
    RUN_TEST(test_circuit5_planet_a);
    RUN_TEST(test_circuit5_planet_b);
    RUN_TEST(test_circuit5_name);
    RUN_TEST(test_circuit5_units);

    /* Circuit boundaries */
    RUN_TEST(test_circuit0_returns_zeroed);
    RUN_TEST(test_circuit6_returns_zeroed);
    RUN_TEST(test_circuit_negative_returns_zeroed);

    /* Phase classification */
    RUN_TEST(test_phase_day1_earth_walk);
    RUN_TEST(test_phase_day3_earth_walk);
    RUN_TEST(test_phase_day6_earth_walk);
    RUN_TEST(test_phase_day7_warriors_cube);
    RUN_TEST(test_phase_day14_warriors_cube);
    RUN_TEST(test_phase_day22_warriors_cube);
    RUN_TEST(test_phase_day23_heaven_walk);
    RUN_TEST(test_phase_day25_heaven_walk);
    RUN_TEST(test_phase_day28_heaven_walk);
    RUN_TEST(test_phase_day0_none);
    RUN_TEST(test_phase_day29_none);
    RUN_TEST(test_phase_negative_none);

    /* Phase sweeps */
    RUN_TEST(test_phase_sweep_earth_walk);
    RUN_TEST(test_phase_sweep_warriors_cube);
    RUN_TEST(test_phase_sweep_heaven_walk);

    /* Phase names */
    RUN_TEST(test_phase_name_none);
    RUN_TEST(test_phase_name_earth_walk);
    RUN_TEST(test_phase_name_warriors_cube);
    RUN_TEST(test_phase_name_heaven_walk);
    RUN_TEST(test_phase_name_invalid_returns_none);

    /* Cube positions */
    RUN_TEST(test_cube_day7_is_1);
    RUN_TEST(test_cube_day8_is_2);
    RUN_TEST(test_cube_day14_is_8);
    RUN_TEST(test_cube_day22_is_16);
    RUN_TEST(test_cube_day6_is_0);
    RUN_TEST(test_cube_day23_is_0);
    RUN_TEST(test_cube_day0_is_0);
    RUN_TEST(test_cube_day_negative_is_0);
    RUN_TEST(test_cube_sweep_all_16);
    RUN_TEST(test_cube_sweep_non_cube_zero);

    /* Tower days */
    RUN_TEST(test_tower_day1);
    RUN_TEST(test_tower_day6);
    RUN_TEST(test_tower_day23);
    RUN_TEST(test_tower_day28);
    RUN_TEST(test_tower_day2_not);
    RUN_TEST(test_tower_day7_not);
    RUN_TEST(test_tower_day22_not);
    RUN_TEST(test_tower_day0_not);
    RUN_TEST(test_tower_day29_not);
    RUN_TEST(test_tower_negative_not);
    RUN_TEST(test_tower_sweep_count);

    /* Reunion */
    RUN_TEST(test_reunion_day23);
    RUN_TEST(test_reunion_day1_not);
    RUN_TEST(test_reunion_day22_not);
    RUN_TEST(test_reunion_day24_not);
    RUN_TEST(test_reunion_day0_not);
    RUN_TEST(test_reunion_negative_not);
    RUN_TEST(test_reunion_sweep_count);

    /* Stairway to Heaven */
    RUN_TEST(test_stairway_day13);
    RUN_TEST(test_stairway_day14);
    RUN_TEST(test_stairway_day15);
    RUN_TEST(test_stairway_day16);
    RUN_TEST(test_stairway_day12_not);
    RUN_TEST(test_stairway_day17_not);
    RUN_TEST(test_stairway_sweep_count);

    /* Full day info structs */
    RUN_TEST(test_day_info_day1_full);
    RUN_TEST(test_day_info_day7_full);
    RUN_TEST(test_day_info_day13_full);
    RUN_TEST(test_day_info_day23_full);
    RUN_TEST(test_day_info_day28_full);

    /* Day info boundaries */
    RUN_TEST(test_day_info_day0_zeroed);
    RUN_TEST(test_day_info_day29_zeroed);
    RUN_TEST(test_day_info_negative_zeroed);

    /* Purity */
    RUN_TEST(test_purity_circuit_same_result);
    RUN_TEST(test_purity_day_info_same_result);

    /* Sweeps */
    RUN_TEST(test_all_circuits_have_valid_data);
    RUN_TEST(test_all_days_have_valid_data);
    RUN_TEST(test_circuit_units_descend_by_4);

    /* Cross-checks */
    RUN_TEST(test_day23_tower_and_reunion);
    RUN_TEST(test_phase_name_round_trip_day5);
    RUN_TEST(test_phase_name_round_trip_day10);
    RUN_TEST(test_phase_name_round_trip_day26);
    RUN_TEST(test_day_info_phase_matches_phase_for_day);
    RUN_TEST(test_day_info_cube_matches_cube_position);

    return UNITY_END();
}
