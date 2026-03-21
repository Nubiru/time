#include "../unity/unity.h"
#include "../../src/ui/kin_wheel_layout.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- kin_wheel_compute: basic identity --- */

void test_kin1_today_seal_0(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_EQUAL_INT(0, w.today_seal);
}

void test_kin1_today_tone_1(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_EQUAL_INT(1, w.today_tone);
}

void test_kin1_today_kin_1(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_EQUAL_INT(1, w.today_kin);
}

void test_kin131_seal_10_tone_1(void)
{
    /* kin 131: seal = (131-1)%20 = 10, tone = (131-1)%13+1 = 1 */
    kin_wheel_layout_t w = kin_wheel_compute(131);
    TEST_ASSERT_EQUAL_INT(10, w.today_seal);
    TEST_ASSERT_EQUAL_INT(1, w.today_tone);
    TEST_ASSERT_EQUAL_INT(131, w.today_kin);
}

/* --- Seal ring angles --- */

void test_seal0_angle_0(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, w.seals[0].angle);
}

void test_seal10_angle_180(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, w.seals[10].angle);
}

void test_seal5_angle_90(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, w.seals[5].angle);
}

/* --- Seal ring positions --- */

void test_seal0_position_top(void)
{
    /* angle=0 (top): x = 0.5 + 0.4*sin(0) = 0.5, y = 0.5 - 0.4*cos(0) = 0.1 */
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, w.seals[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, w.seals[0].y);
}

void test_seal5_position_right(void)
{
    /* angle=90: x = 0.5 + 0.4*sin(90) = 0.9, y = 0.5 - 0.4*cos(90) = 0.5 */
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.9f, w.seals[5].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, w.seals[5].y);
}

/* --- Tone ring --- */

void test_tone0_angle_0(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, w.tones[0].angle);
}

void test_tone0_position_top(void)
{
    /* angle=0: x = 0.5 + 0.25*sin(0) = 0.5, y = 0.5 - 0.25*cos(0) = 0.25 */
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, w.tones[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, w.tones[0].y);
}

void test_tone_index_is_tone_number(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    for (int i = 0; i < KIN_WHEEL_TONES; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, w.tones[i].index);
    }
}

/* --- Highlighting --- */

void test_today_seal_highlighted(void)
{
    /* kin 1 -> seal 0 highlighted */
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_EQUAL_INT(1, w.seals[0].highlighted);
}

void test_other_seals_not_highlighted(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    for (int i = 1; i < KIN_WHEEL_SEALS; i++) {
        TEST_ASSERT_EQUAL_INT(0, w.seals[i].highlighted);
    }
}

void test_today_tone_highlighted(void)
{
    /* kin 1 -> tone 1 -> tones[0] highlighted */
    kin_wheel_layout_t w = kin_wheel_compute(1);
    TEST_ASSERT_EQUAL_INT(1, w.tones[0].highlighted);
}

void test_other_tones_not_highlighted(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    for (int i = 1; i < KIN_WHEEL_TONES; i++) {
        TEST_ASSERT_EQUAL_INT(0, w.tones[i].highlighted);
    }
}

void test_kin131_seal10_highlighted(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(131);
    TEST_ASSERT_EQUAL_INT(1, w.seals[10].highlighted);
    TEST_ASSERT_EQUAL_INT(0, w.seals[0].highlighted);
}

/* --- Color validation --- */

void test_all_seals_valid_color(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    for (int i = 0; i < KIN_WHEEL_SEALS; i++) {
        TEST_ASSERT_TRUE(w.seals[i].color >= 0 && w.seals[i].color <= 3);
    }
}

void test_all_tones_color_minus1(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    for (int i = 0; i < KIN_WHEEL_TONES; i++) {
        TEST_ASSERT_EQUAL_INT(-1, w.tones[i].color);
    }
}

/* --- Invalid kin --- */

void test_invalid_kin_0_returns_zeroed(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(0);
    TEST_ASSERT_EQUAL_INT(0, w.today_kin);
    TEST_ASSERT_EQUAL_INT(0, w.today_seal);
    TEST_ASSERT_EQUAL_INT(0, w.today_tone);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, w.seals[0].x);
}

void test_invalid_kin_261_returns_zeroed(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(261);
    TEST_ASSERT_EQUAL_INT(0, w.today_kin);
}

void test_invalid_kin_negative_returns_zeroed(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(-5);
    TEST_ASSERT_EQUAL_INT(0, w.today_kin);
}

/* --- kin_wheel_from_jd --- */

void test_from_jd_valid_returns_nonzero(void)
{
    /* JD 2460000.5 is a valid date */
    kin_wheel_layout_t w = kin_wheel_from_jd(2460000.5);
    TEST_ASSERT_TRUE(w.today_kin >= 1 && w.today_kin <= 260);
}

void test_from_jd_seal_in_range(void)
{
    kin_wheel_layout_t w = kin_wheel_from_jd(2460000.5);
    TEST_ASSERT_TRUE(w.today_seal >= 0 && w.today_seal <= 19);
}

void test_from_jd_tone_in_range(void)
{
    kin_wheel_layout_t w = kin_wheel_from_jd(2460000.5);
    TEST_ASSERT_TRUE(w.today_tone >= 1 && w.today_tone <= 13);
}

/* --- Seal index values --- */

void test_seal_indices_0_to_19(void)
{
    kin_wheel_layout_t w = kin_wheel_compute(1);
    for (int i = 0; i < KIN_WHEEL_SEALS; i++) {
        TEST_ASSERT_EQUAL_INT(i, w.seals[i].index);
    }
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    kin_wheel_layout_t a = kin_wheel_compute(42);
    kin_wheel_layout_t b = kin_wheel_compute(42);
    TEST_ASSERT_EQUAL_INT(a.today_kin, b.today_kin);
    TEST_ASSERT_EQUAL_INT(a.today_seal, b.today_seal);
    TEST_ASSERT_EQUAL_INT(a.today_tone, b.today_tone);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.seals[5].x, b.seals[5].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.tones[3].y, b.tones[3].y);
}

int main(void)
{
    UNITY_BEGIN();
    /* basic identity */
    RUN_TEST(test_kin1_today_seal_0);
    RUN_TEST(test_kin1_today_tone_1);
    RUN_TEST(test_kin1_today_kin_1);
    RUN_TEST(test_kin131_seal_10_tone_1);
    /* seal ring angles */
    RUN_TEST(test_seal0_angle_0);
    RUN_TEST(test_seal10_angle_180);
    RUN_TEST(test_seal5_angle_90);
    /* seal ring positions */
    RUN_TEST(test_seal0_position_top);
    RUN_TEST(test_seal5_position_right);
    /* tone ring */
    RUN_TEST(test_tone0_angle_0);
    RUN_TEST(test_tone0_position_top);
    RUN_TEST(test_tone_index_is_tone_number);
    /* highlighting */
    RUN_TEST(test_today_seal_highlighted);
    RUN_TEST(test_other_seals_not_highlighted);
    RUN_TEST(test_today_tone_highlighted);
    RUN_TEST(test_other_tones_not_highlighted);
    RUN_TEST(test_kin131_seal10_highlighted);
    /* color validation */
    RUN_TEST(test_all_seals_valid_color);
    RUN_TEST(test_all_tones_color_minus1);
    /* invalid kin */
    RUN_TEST(test_invalid_kin_0_returns_zeroed);
    RUN_TEST(test_invalid_kin_261_returns_zeroed);
    RUN_TEST(test_invalid_kin_negative_returns_zeroed);
    /* from_jd */
    RUN_TEST(test_from_jd_valid_returns_nonzero);
    RUN_TEST(test_from_jd_seal_in_range);
    RUN_TEST(test_from_jd_tone_in_range);
    /* seal indices */
    RUN_TEST(test_seal_indices_0_to_19);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
