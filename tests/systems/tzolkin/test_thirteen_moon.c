#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/thirteen_moon.h"
#include "../../../src/math/julian.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- thirteen_moon_from_jd: known dates --- */

static void test_july_26_2026_is_moon1_day1(void)
{
    /* July 26, 2026 = first day of 13-Moon year 2026 */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, tm.moon);
    TEST_ASSERT_EQUAL_INT(1, tm.day);
    TEST_ASSERT_EQUAL_INT(1, tm.heptad);
    TEST_ASSERT_EQUAL_INT(1, tm.day_of_week);
    TEST_ASSERT_EQUAL_INT(0, tm.plasma);      /* Dali */
    TEST_ASSERT_EQUAL_INT(0, tm.day_of_year);
    TEST_ASSERT_EQUAL_INT(0, tm.is_day_out);
    TEST_ASSERT_EQUAL_INT(2026, tm.year);
}

static void test_july_27_2026_is_moon1_day2(void)
{
    double jd = gregorian_to_jd(2026, 7, 27.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, tm.moon);
    TEST_ASSERT_EQUAL_INT(2, tm.day);
    TEST_ASSERT_EQUAL_INT(1, tm.heptad);
    TEST_ASSERT_EQUAL_INT(2, tm.day_of_week);
    TEST_ASSERT_EQUAL_INT(1, tm.plasma);      /* Seli */
    TEST_ASSERT_EQUAL_INT(1, tm.day_of_year);
}

static void test_aug_22_2026_is_moon1_day28(void)
{
    /* Aug 22, 2026 = July 26 + 27 days = day_of_year 27, last day of Moon 1 */
    double jd = gregorian_to_jd(2026, 8, 22.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, tm.moon);
    TEST_ASSERT_EQUAL_INT(28, tm.day);
    TEST_ASSERT_EQUAL_INT(4, tm.heptad);
    TEST_ASSERT_EQUAL_INT(7, tm.day_of_week);
    TEST_ASSERT_EQUAL_INT(6, tm.plasma);      /* Silio */
    TEST_ASSERT_EQUAL_INT(27, tm.day_of_year);
}

static void test_aug_23_2026_is_moon2_day1(void)
{
    /* Aug 23, 2026 = first day of Moon 2 */
    double jd = gregorian_to_jd(2026, 8, 23.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2, tm.moon);
    TEST_ASSERT_EQUAL_INT(1, tm.day);
    TEST_ASSERT_EQUAL_INT(1, tm.heptad);
    TEST_ASSERT_EQUAL_INT(1, tm.day_of_week);
    TEST_ASSERT_EQUAL_INT(0, tm.plasma);      /* Dali */
    TEST_ASSERT_EQUAL_INT(28, tm.day_of_year);
}

static void test_july_25_2027_is_day_out_of_time(void)
{
    /* July 25, 2027 = Day Out of Time for 13-Moon year 2026 */
    double jd = gregorian_to_jd(2027, 7, 25.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, tm.is_day_out);
    TEST_ASSERT_EQUAL_INT(0, tm.moon);
    TEST_ASSERT_EQUAL_INT(0, tm.day);
    TEST_ASSERT_EQUAL_INT(364, tm.day_of_year);
    TEST_ASSERT_EQUAL_INT(2026, tm.year);
}

static void test_jan_1_2027_in_13moon_year_2026(void)
{
    /* Jan 1, 2027 is within 13-Moon year starting July 26, 2026.
     * Days from July 26 to Jan 1:
     *   July: 5 remaining (27-31), Aug: 31, Sep: 30, Oct: 31, Nov: 30, Dec: 31, Jan 1: 1
     *   = 5 + 31 + 30 + 31 + 30 + 31 + 1 = 159
     *   day_of_year = 159
     *   moon = 159 / 28 + 1 = 5 + 1 = 6
     *   day  = 159 % 28 + 1 = 19 + 1 = 20 */
    double jd = gregorian_to_jd(2027, 1, 1.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2026, tm.year);
    TEST_ASSERT_EQUAL_INT(0, tm.is_day_out);
    TEST_ASSERT_EQUAL_INT(159, tm.day_of_year);
    TEST_ASSERT_EQUAL_INT(6, tm.moon);
    TEST_ASSERT_EQUAL_INT(20, tm.day);
}

static void test_july_26_2027_is_next_year_moon1_day1(void)
{
    /* July 26, 2027 = first day of 13-Moon year 2027 */
    double jd = gregorian_to_jd(2027, 7, 26.0);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(2027, tm.year);
    TEST_ASSERT_EQUAL_INT(1, tm.moon);
    TEST_ASSERT_EQUAL_INT(1, tm.day);
    TEST_ASSERT_EQUAL_INT(0, tm.day_of_year);
}

/* --- Moon names --- */

static void test_moon_name_magnetic(void)
{
    TEST_ASSERT_EQUAL_STRING("Magnetic", thirteen_moon_name(1));
}

static void test_moon_name_resonant(void)
{
    TEST_ASSERT_EQUAL_STRING("Resonant", thirteen_moon_name(7));
}

static void test_moon_name_cosmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmic", thirteen_moon_name(13));
}

static void test_moon_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thirteen_moon_name(0));
}

static void test_moon_name_invalid_14(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thirteen_moon_name(14));
}

/* --- Plasma names --- */

static void test_plasma_name_dali(void)
{
    TEST_ASSERT_EQUAL_STRING("Dali", thirteen_moon_plasma_name(0));
}

static void test_plasma_name_silio(void)
{
    TEST_ASSERT_EQUAL_STRING("Silio", thirteen_moon_plasma_name(6));
}

static void test_plasma_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thirteen_moon_plasma_name(7));
}

/* --- Plasma chakras --- */

static void test_plasma_chakra_crown(void)
{
    TEST_ASSERT_EQUAL_STRING("Crown", thirteen_moon_plasma_chakra(0));
}

static void test_plasma_chakra_heart(void)
{
    TEST_ASSERT_EQUAL_STRING("Heart", thirteen_moon_plasma_chakra(6));
}

static void test_plasma_chakra_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thirteen_moon_plasma_chakra(-1));
}

/* --- Plasma actions --- */

static void test_plasma_action_target(void)
{
    TEST_ASSERT_EQUAL_STRING("Target", thirteen_moon_plasma_action(0));
}

static void test_plasma_action_discharge(void)
{
    TEST_ASSERT_EQUAL_STRING("Discharge", thirteen_moon_plasma_action(6));
}

static void test_plasma_action_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", thirteen_moon_plasma_action(7));
}

/* --- Range validation --- */

static void test_heptad_range_valid(void)
{
    /* Check a spread of dates: heptad must always be 1-4 */
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int i = 0; i < 364; i++) {
        thirteen_moon_t tm = thirteen_moon_from_jd(jd + i);
        if (!tm.is_day_out) {
            TEST_ASSERT_TRUE(tm.heptad >= 1 && tm.heptad <= 4);
        }
    }
}

static void test_day_of_week_range_valid(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int i = 0; i < 364; i++) {
        thirteen_moon_t tm = thirteen_moon_from_jd(jd + i);
        if (!tm.is_day_out) {
            TEST_ASSERT_TRUE(tm.day_of_week >= 1 && tm.day_of_week <= 7);
        }
    }
}

static void test_plasma_range_valid(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int i = 0; i < 364; i++) {
        thirteen_moon_t tm = thirteen_moon_from_jd(jd + i);
        if (!tm.is_day_out) {
            TEST_ASSERT_TRUE(tm.plasma >= 0 && tm.plasma <= 6);
        }
    }
}

static void test_moon_range_valid(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    for (int i = 0; i < 364; i++) {
        thirteen_moon_t tm = thirteen_moon_from_jd(jd + i);
        if (!tm.is_day_out) {
            TEST_ASSERT_TRUE(tm.moon >= 1 && tm.moon <= 13);
        }
    }
}

/* --- Invalid JD --- */

static void test_invalid_jd_negative(void)
{
    thirteen_moon_t tm = thirteen_moon_from_jd(-1.0);
    TEST_ASSERT_EQUAL_INT(0, tm.moon);
    TEST_ASSERT_EQUAL_INT(0, tm.day);
    TEST_ASSERT_EQUAL_INT(0, tm.year);
    TEST_ASSERT_EQUAL_INT(0, tm.is_day_out);
}

/* --- Purity --- */

static void test_purity_same_input_same_output(void)
{
    double jd = gregorian_to_jd(2026, 10, 15.0);
    thirteen_moon_t a = thirteen_moon_from_jd(jd);
    thirteen_moon_t b = thirteen_moon_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(a.moon, b.moon);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
    TEST_ASSERT_EQUAL_INT(a.heptad, b.heptad);
    TEST_ASSERT_EQUAL_INT(a.day_of_week, b.day_of_week);
    TEST_ASSERT_EQUAL_INT(a.plasma, b.plasma);
    TEST_ASSERT_EQUAL_INT(a.year, b.year);
}

int main(void)
{
    UNITY_BEGIN();
    /* known dates */
    RUN_TEST(test_july_26_2026_is_moon1_day1);
    RUN_TEST(test_july_27_2026_is_moon1_day2);
    RUN_TEST(test_aug_22_2026_is_moon1_day28);
    RUN_TEST(test_aug_23_2026_is_moon2_day1);
    RUN_TEST(test_july_25_2027_is_day_out_of_time);
    RUN_TEST(test_jan_1_2027_in_13moon_year_2026);
    RUN_TEST(test_july_26_2027_is_next_year_moon1_day1);
    /* moon names */
    RUN_TEST(test_moon_name_magnetic);
    RUN_TEST(test_moon_name_resonant);
    RUN_TEST(test_moon_name_cosmic);
    RUN_TEST(test_moon_name_invalid_zero);
    RUN_TEST(test_moon_name_invalid_14);
    /* plasma names */
    RUN_TEST(test_plasma_name_dali);
    RUN_TEST(test_plasma_name_silio);
    RUN_TEST(test_plasma_name_invalid);
    /* plasma chakras */
    RUN_TEST(test_plasma_chakra_crown);
    RUN_TEST(test_plasma_chakra_heart);
    RUN_TEST(test_plasma_chakra_invalid);
    /* plasma actions */
    RUN_TEST(test_plasma_action_target);
    RUN_TEST(test_plasma_action_discharge);
    RUN_TEST(test_plasma_action_invalid);
    /* range validation */
    RUN_TEST(test_heptad_range_valid);
    RUN_TEST(test_day_of_week_range_valid);
    RUN_TEST(test_plasma_range_valid);
    RUN_TEST(test_moon_range_valid);
    /* invalid jd */
    RUN_TEST(test_invalid_jd_negative);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
