#include "../unity/unity.h"
#include "../../src/ui/kin_round_layout.h"

void setUp(void) {}
void tearDown(void) {}

/* --- kin_round_year_bearer --- */

void test_year_bearer_0_seed(void)
{
    TEST_ASSERT_EQUAL_INT(3, kin_round_year_bearer(0));
}

void test_year_bearer_1_moon(void)
{
    TEST_ASSERT_EQUAL_INT(8, kin_round_year_bearer(1));
}

void test_year_bearer_2_wizard(void)
{
    TEST_ASSERT_EQUAL_INT(13, kin_round_year_bearer(2));
}

void test_year_bearer_3_storm(void)
{
    TEST_ASSERT_EQUAL_INT(18, kin_round_year_bearer(3));
}

void test_year_bearer_4_seed_again(void)
{
    /* Cycle of 4: year 4 wraps back to Seed */
    TEST_ASSERT_EQUAL_INT(3, kin_round_year_bearer(4));
}

void test_year_bearer_51_storm(void)
{
    /* 51 % 4 = 3 -> Storm(18) */
    TEST_ASSERT_EQUAL_INT(18, kin_round_year_bearer(51));
}

void test_year_bearer_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_round_year_bearer(-1));
}

void test_year_bearer_invalid_52(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_round_year_bearer(52));
}

/* --- kin_round_compute: valid JD --- */

void test_valid_jd_returns_nonzero(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    /* At least one petal should have a valid bearer_seal */
    TEST_ASSERT_TRUE(r.petals[0].bearer_seal >= 0);
}

void test_current_year_in_range(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    TEST_ASSERT_TRUE(r.current_year >= 0 && r.current_year <= 51);
}

void test_days_remaining_in_range(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    TEST_ASSERT_TRUE(r.days_remaining >= 0 && r.days_remaining < 18980);
}

void test_progress_in_range(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    TEST_ASSERT_TRUE(r.progress >= 0.0f && r.progress <= 1.0f);
}

void test_petal0_angle_0(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, r.petals[0].angle);
}

void test_petal0_position_near_top(void)
{
    /* angle=0: x = 0.5 + 0.4*sin(0) = 0.5, y = 0.5 - 0.4*cos(0) = 0.1 */
    kin_round_layout_t r = kin_round_compute(2460000.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, r.petals[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, r.petals[0].y);
}

void test_all_petals_valid_angle(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        TEST_ASSERT_TRUE(r.petals[i].angle >= 0.0f && r.petals[i].angle < 360.0f);
    }
}

void test_only_one_petal_highlighted(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    int count = 0;
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        if (r.petals[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_highlighted_petal_matches_current_year(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    TEST_ASSERT_EQUAL_INT(1, r.petals[r.current_year].highlighted);
}

void test_bearer_seals_are_valid(void)
{
    /* All bearer seals must be one of {3, 8, 13, 18} */
    kin_round_layout_t r = kin_round_compute(2460000.5);
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        int s = r.petals[i].bearer_seal;
        TEST_ASSERT_TRUE(s == 3 || s == 8 || s == 13 || s == 18);
    }
}

void test_bearer_colors_valid(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        TEST_ASSERT_TRUE(r.petals[i].color >= 0 && r.petals[i].color <= 3);
    }
}

void test_bearer_tones_valid(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        TEST_ASSERT_TRUE(r.petals[i].bearer_tone >= 1
                         && r.petals[i].bearer_tone <= 13);
    }
}

void test_petal_positions_in_unit_square(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        TEST_ASSERT_TRUE(r.petals[i].x >= 0.0f && r.petals[i].x <= 1.0f);
        TEST_ASSERT_TRUE(r.petals[i].y >= 0.0f && r.petals[i].y <= 1.0f);
    }
}

/* --- Invalid JD --- */

void test_invalid_jd_negative_returns_zeroed(void)
{
    kin_round_layout_t r = kin_round_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, r.current_year);
    TEST_ASSERT_EQUAL_INT(0, r.days_remaining);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, r.progress);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, r.petals[0].x);
}

/* --- Year in round values --- */

void test_petal_year_in_round_sequential(void)
{
    kin_round_layout_t r = kin_round_compute(2460000.5);
    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        TEST_ASSERT_EQUAL_INT(i, r.petals[i].year_in_round);
    }
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    kin_round_layout_t a = kin_round_compute(2460000.5);
    kin_round_layout_t b = kin_round_compute(2460000.5);
    TEST_ASSERT_EQUAL_INT(a.current_year, b.current_year);
    TEST_ASSERT_EQUAL_INT(a.days_remaining, b.days_remaining);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.progress, b.progress);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.petals[26].x, b.petals[26].x);
}

int main(void)
{
    UNITY_BEGIN();
    /* year bearer */
    RUN_TEST(test_year_bearer_0_seed);
    RUN_TEST(test_year_bearer_1_moon);
    RUN_TEST(test_year_bearer_2_wizard);
    RUN_TEST(test_year_bearer_3_storm);
    RUN_TEST(test_year_bearer_4_seed_again);
    RUN_TEST(test_year_bearer_51_storm);
    RUN_TEST(test_year_bearer_invalid_negative);
    RUN_TEST(test_year_bearer_invalid_52);
    /* valid JD */
    RUN_TEST(test_valid_jd_returns_nonzero);
    RUN_TEST(test_current_year_in_range);
    RUN_TEST(test_days_remaining_in_range);
    RUN_TEST(test_progress_in_range);
    RUN_TEST(test_petal0_angle_0);
    RUN_TEST(test_petal0_position_near_top);
    RUN_TEST(test_all_petals_valid_angle);
    RUN_TEST(test_only_one_petal_highlighted);
    RUN_TEST(test_highlighted_petal_matches_current_year);
    RUN_TEST(test_bearer_seals_are_valid);
    RUN_TEST(test_bearer_colors_valid);
    RUN_TEST(test_bearer_tones_valid);
    RUN_TEST(test_petal_positions_in_unit_square);
    /* invalid JD */
    RUN_TEST(test_invalid_jd_negative_returns_zeroed);
    /* year in round */
    RUN_TEST(test_petal_year_in_round_sequential);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
