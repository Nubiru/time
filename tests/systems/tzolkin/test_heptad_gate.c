#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/heptad_gate.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Count --- */

static void test_heptad_gate_count_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, heptad_gate_count());
}

/* --- Day 1: DALI / Dragon / 108 / Crown --- */

static void test_day1_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("DALI", heptad_gate_plasma_name(1));
}

static void test_day1_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Dragon", heptad_gate_seal_name(1));
}

static void test_day1_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(108, heptad_gate_hk_number(1));
}

static void test_day1_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Crown", heptad_gate_chakra(1));
}

static void test_day1_mantra(void)
{
    TEST_ASSERT_EQUAL_STRING("OM", heptad_gate_mantra(1));
}

static void test_day1_struct_fields(void)
{
    heptad_gate_t g = heptad_gate_for_day(1);
    TEST_ASSERT_EQUAL_INT(1, g.day);
    TEST_ASSERT_EQUAL_INT(1, g.seal_code);
    TEST_ASSERT_EQUAL_STRING("Pre-conscious", g.consciousness);
    TEST_ASSERT_EQUAL_STRING("1st Sphere", g.mental_sphere);
    TEST_ASSERT_EQUAL_STRING("Opens Circuit 8", g.circuit);
}

/* --- Day 2: SELI / Hand / 291 / Root --- */

static void test_day2_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("SELI", heptad_gate_plasma_name(2));
}

static void test_day2_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Hand", heptad_gate_seal_name(2));
}

static void test_day2_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(291, heptad_gate_hk_number(2));
}

static void test_day2_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Root", heptad_gate_chakra(2));
}

static void test_day2_mantra(void)
{
    TEST_ASSERT_EQUAL_STRING("HRAM", heptad_gate_mantra(2));
}

/* --- Day 3: GAMMA / Wind / 144 / Third Eye --- */

static void test_day3_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("GAMMA", heptad_gate_plasma_name(3));
}

static void test_day3_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Wind", heptad_gate_seal_name(3));
}

static void test_day3_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(144, heptad_gate_hk_number(3));
}

static void test_day3_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Third Eye", heptad_gate_chakra(3));
}

static void test_day3_mantra(void)
{
    TEST_ASSERT_EQUAL_STRING("HRAHA", heptad_gate_mantra(3));
}

/* --- Day 4: KALI / Skywalker / 315 / Sacral --- */

static void test_day4_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("KALI", heptad_gate_plasma_name(4));
}

static void test_day4_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Skywalker", heptad_gate_seal_name(4));
}

static void test_day4_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(315, heptad_gate_hk_number(4));
}

static void test_day4_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Sacral", heptad_gate_chakra(4));
}

static void test_day4_struct_fields(void)
{
    heptad_gate_t g = heptad_gate_for_day(4);
    TEST_ASSERT_EQUAL_INT(4, g.day);
    TEST_ASSERT_EQUAL_INT(13, g.seal_code);
    TEST_ASSERT_EQUAL_STRING("HRIM", g.mantra);
    TEST_ASSERT_EQUAL_STRING("Continuous Conscious", g.consciousness);
    TEST_ASSERT_EQUAL_STRING("4th Sphere", g.mental_sphere);
    TEST_ASSERT_EQUAL_STRING("Closes Circuit 5", g.circuit);
}

/* --- Day 5: ALFA / Sun / 414 / Throat --- */

static void test_day5_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("ALFA", heptad_gate_plasma_name(5));
}

static void test_day5_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Sun", heptad_gate_seal_name(5));
}

static void test_day5_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(414, heptad_gate_hk_number(5));
}

static void test_day5_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Throat", heptad_gate_chakra(5));
}

static void test_day5_mantra(void)
{
    TEST_ASSERT_EQUAL_STRING("HRAUM", heptad_gate_mantra(5));
}

/* --- Day 6: LIMI / Mirror / 402 / Solar Plexus --- */

static void test_day6_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("LIMI", heptad_gate_plasma_name(6));
}

static void test_day6_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Mirror", heptad_gate_seal_name(6));
}

static void test_day6_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(402, heptad_gate_hk_number(6));
}

static void test_day6_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Plexus", heptad_gate_chakra(6));
}

static void test_day6_mantra(void)
{
    TEST_ASSERT_EQUAL_STRING("HRUM", heptad_gate_mantra(6));
}

/* --- Day 7: SILIO / Hunab Ku / 441 / Heart --- */

static void test_day7_plasma(void)
{
    TEST_ASSERT_EQUAL_STRING("SILIO", heptad_gate_plasma_name(7));
}

static void test_day7_seal(void)
{
    TEST_ASSERT_EQUAL_STRING("Hunab Ku", heptad_gate_seal_name(7));
}

static void test_day7_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(441, heptad_gate_hk_number(7));
}

static void test_day7_chakra(void)
{
    TEST_ASSERT_EQUAL_STRING("Heart", heptad_gate_chakra(7));
}

static void test_day7_struct_fields(void)
{
    heptad_gate_t g = heptad_gate_for_day(7);
    TEST_ASSERT_EQUAL_INT(7, g.day);
    TEST_ASSERT_EQUAL_INT(21, g.seal_code);
    TEST_ASSERT_EQUAL_STRING("HRAIM", g.mantra);
    TEST_ASSERT_EQUAL_STRING("Holomental Perceptor", g.consciousness);
    TEST_ASSERT_EQUAL_STRING("7th Sphere", g.mental_sphere);
    TEST_ASSERT_EQUAL_STRING("Circuit 11", g.circuit);
}

/* --- Boundary: invalid day 0 --- */

static void test_day0_returns_zeroed(void)
{
    heptad_gate_t g = heptad_gate_for_day(0);
    TEST_ASSERT_EQUAL_INT(0, g.day);
    TEST_ASSERT_NULL(g.plasma);
    TEST_ASSERT_NULL(g.seal_name);
    TEST_ASSERT_NULL(g.mantra);
    TEST_ASSERT_NULL(g.chakra);
    TEST_ASSERT_EQUAL_INT(0, g.hk_number);
}

static void test_day0_helpers_return_null(void)
{
    TEST_ASSERT_NULL(heptad_gate_plasma_name(0));
    TEST_ASSERT_NULL(heptad_gate_mantra(0));
    TEST_ASSERT_NULL(heptad_gate_chakra(0));
    TEST_ASSERT_NULL(heptad_gate_seal_name(0));
    TEST_ASSERT_EQUAL_INT(0, heptad_gate_hk_number(0));
}

/* --- Boundary: invalid day 8 --- */

static void test_day8_returns_zeroed(void)
{
    heptad_gate_t g = heptad_gate_for_day(8);
    TEST_ASSERT_EQUAL_INT(0, g.day);
    TEST_ASSERT_NULL(g.plasma);
    TEST_ASSERT_NULL(g.seal_name);
    TEST_ASSERT_NULL(g.mantra);
    TEST_ASSERT_NULL(g.chakra);
    TEST_ASSERT_EQUAL_INT(0, g.hk_number);
}

static void test_day8_helpers_return_null(void)
{
    TEST_ASSERT_NULL(heptad_gate_plasma_name(8));
    TEST_ASSERT_NULL(heptad_gate_mantra(8));
    TEST_ASSERT_NULL(heptad_gate_chakra(8));
    TEST_ASSERT_NULL(heptad_gate_seal_name(8));
    TEST_ASSERT_EQUAL_INT(0, heptad_gate_hk_number(8));
}

/* --- Boundary: negative day --- */

static void test_negative_day_returns_zeroed(void)
{
    heptad_gate_t g = heptad_gate_for_day(-1);
    TEST_ASSERT_EQUAL_INT(0, g.day);
    TEST_ASSERT_NULL(g.plasma);
    TEST_ASSERT_EQUAL_INT(0, g.hk_number);
}

/* --- Purity: same input gives same output --- */

static void test_purity_same_result(void)
{
    heptad_gate_t a = heptad_gate_for_day(3);
    heptad_gate_t b = heptad_gate_for_day(3);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
    TEST_ASSERT_EQUAL_INT(a.seal_code, b.seal_code);
    TEST_ASSERT_EQUAL_INT(a.hk_number, b.hk_number);
    TEST_ASSERT_EQUAL_STRING(a.plasma, b.plasma);
    TEST_ASSERT_EQUAL_STRING(a.chakra, b.chakra);
    TEST_ASSERT_EQUAL_STRING(a.mantra, b.mantra);
}

/* --- Sweep: all 7 days have non-null strings --- */

static void test_all_days_have_valid_data(void)
{
    for (int d = 1; d <= 7; d++) {
        heptad_gate_t g = heptad_gate_for_day(d);
        TEST_ASSERT_EQUAL_INT(d, g.day);
        TEST_ASSERT_NOT_NULL(g.plasma);
        TEST_ASSERT_NOT_NULL(g.seal_name);
        TEST_ASSERT_NOT_NULL(g.mantra);
        TEST_ASSERT_NOT_NULL(g.chakra);
        TEST_ASSERT_NOT_NULL(g.circuit);
        TEST_ASSERT_NOT_NULL(g.mental_sphere);
        TEST_ASSERT_NOT_NULL(g.consciousness);
        TEST_ASSERT_TRUE(g.seal_code >= 1);
        TEST_ASSERT_TRUE(g.hk_number > 0);
        TEST_ASSERT_TRUE(strlen(g.plasma) > 0);
        TEST_ASSERT_TRUE(strlen(g.seal_name) > 0);
        TEST_ASSERT_TRUE(strlen(g.mantra) > 0);
        TEST_ASSERT_TRUE(strlen(g.chakra) > 0);
    }
}

/* --- HK numbers are all unique --- */

static void test_hk_numbers_unique(void)
{
    int nums[7];
    for (int d = 1; d <= 7; d++) {
        nums[d - 1] = heptad_gate_hk_number(d);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = i + 1; j < 7; j++) {
            TEST_ASSERT_NOT_EQUAL(nums[i], nums[j]);
        }
    }
}

/* --- HK number 441 is day 7 (Hunab Ku = 21x21) --- */

static void test_hk_441_is_21_squared(void)
{
    TEST_ASSERT_EQUAL_INT(441, 21 * 21);
    TEST_ASSERT_EQUAL_INT(441, heptad_gate_hk_number(7));
}

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_heptad_gate_count_is_7);

    /* Day 1 — DALI / Dragon / 108 / Crown */
    RUN_TEST(test_day1_plasma);
    RUN_TEST(test_day1_seal);
    RUN_TEST(test_day1_hk_number);
    RUN_TEST(test_day1_chakra);
    RUN_TEST(test_day1_mantra);
    RUN_TEST(test_day1_struct_fields);

    /* Day 2 — SELI / Hand / 291 / Root */
    RUN_TEST(test_day2_plasma);
    RUN_TEST(test_day2_seal);
    RUN_TEST(test_day2_hk_number);
    RUN_TEST(test_day2_chakra);
    RUN_TEST(test_day2_mantra);

    /* Day 3 — GAMMA / Wind / 144 / Third Eye */
    RUN_TEST(test_day3_plasma);
    RUN_TEST(test_day3_seal);
    RUN_TEST(test_day3_hk_number);
    RUN_TEST(test_day3_chakra);
    RUN_TEST(test_day3_mantra);

    /* Day 4 — KALI / Skywalker / 315 / Sacral */
    RUN_TEST(test_day4_plasma);
    RUN_TEST(test_day4_seal);
    RUN_TEST(test_day4_hk_number);
    RUN_TEST(test_day4_chakra);
    RUN_TEST(test_day4_struct_fields);

    /* Day 5 — ALFA / Sun / 414 / Throat */
    RUN_TEST(test_day5_plasma);
    RUN_TEST(test_day5_seal);
    RUN_TEST(test_day5_hk_number);
    RUN_TEST(test_day5_chakra);
    RUN_TEST(test_day5_mantra);

    /* Day 6 — LIMI / Mirror / 402 / Solar Plexus */
    RUN_TEST(test_day6_plasma);
    RUN_TEST(test_day6_seal);
    RUN_TEST(test_day6_hk_number);
    RUN_TEST(test_day6_chakra);
    RUN_TEST(test_day6_mantra);

    /* Day 7 — SILIO / Hunab Ku / 441 / Heart */
    RUN_TEST(test_day7_plasma);
    RUN_TEST(test_day7_seal);
    RUN_TEST(test_day7_hk_number);
    RUN_TEST(test_day7_chakra);
    RUN_TEST(test_day7_struct_fields);

    /* Boundary — invalid days */
    RUN_TEST(test_day0_returns_zeroed);
    RUN_TEST(test_day0_helpers_return_null);
    RUN_TEST(test_day8_returns_zeroed);
    RUN_TEST(test_day8_helpers_return_null);
    RUN_TEST(test_negative_day_returns_zeroed);

    /* Purity */
    RUN_TEST(test_purity_same_result);

    /* Sweep */
    RUN_TEST(test_all_days_have_valid_data);
    RUN_TEST(test_hk_numbers_unique);
    RUN_TEST(test_hk_441_is_21_squared);

    return UNITY_END();
}
