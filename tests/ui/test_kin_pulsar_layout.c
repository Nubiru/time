#include "../unity/unity.h"
#include "../../src/ui/kin_pulsar_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- kin_pulsar_compute: tone 1 (Magnetic — Time pulsar) --- */

void test_compute_tone_1_today_pulsar_is_0(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(0, p.today_pulsar);
}

void test_compute_tone_1_today_tone_is_1(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(1, p.today_tone);
}

void test_compute_tone_1_time_pulsar_contains_today(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(1, p.pulsars[0].contains_today);
}

/* --- tone 2 (Lunar — Sense pulsar) --- */

void test_compute_tone_2_today_pulsar_is_1(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(2);
    TEST_ASSERT_EQUAL_INT(1, p.today_pulsar);
}

void test_compute_tone_2_sense_pulsar_contains_today(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(2);
    TEST_ASSERT_EQUAL_INT(1, p.pulsars[1].contains_today);
}

/* --- tone 13 (Cosmic — Time pulsar, gate tone) --- */

void test_compute_tone_13_today_pulsar_is_0(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(13);
    TEST_ASSERT_EQUAL_INT(0, p.today_pulsar);
}

void test_compute_tone_13_today_tone_is_13(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(13);
    TEST_ASSERT_EQUAL_INT(13, p.today_tone);
}

/* --- Time pulsar data: 4 tones (1,5,9,13) --- */

void test_time_pulsar_name(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_NOT_NULL(p.pulsars[0].name);
    TEST_ASSERT_EQUAL_STRING("Time", p.pulsars[0].name);
}

void test_time_pulsar_dimension(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_STRING("1st", p.pulsars[0].dimension);
}

void test_time_pulsar_tone_count_is_4(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(4, p.pulsars[0].tone_count);
}

void test_time_pulsar_tones_are_1_5_9_13(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(1,  p.pulsars[0].tones[0]);
    TEST_ASSERT_EQUAL_INT(5,  p.pulsars[0].tones[1]);
    TEST_ASSERT_EQUAL_INT(9,  p.pulsars[0].tones[2]);
    TEST_ASSERT_EQUAL_INT(13, p.pulsars[0].tones[3]);
}

/* --- Sense pulsar: 3 tones (2,6,10) --- */

void test_sense_pulsar_name(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_STRING("Sense", p.pulsars[1].name);
}

void test_sense_pulsar_tone_count_is_3(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(3, p.pulsars[1].tone_count);
}

void test_sense_pulsar_tones_are_2_6_10(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(2,  p.pulsars[1].tones[0]);
    TEST_ASSERT_EQUAL_INT(6,  p.pulsars[1].tones[1]);
    TEST_ASSERT_EQUAL_INT(10, p.pulsars[1].tones[2]);
}

/* --- Mind pulsar: 3 tones (3,7,11) --- */

void test_mind_pulsar_tones_are_3_7_11(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(3,  p.pulsars[2].tones[0]);
    TEST_ASSERT_EQUAL_INT(7,  p.pulsars[2].tones[1]);
    TEST_ASSERT_EQUAL_INT(11, p.pulsars[2].tones[2]);
    TEST_ASSERT_EQUAL_INT(3,  p.pulsars[2].tone_count);
}

/* --- Self-Existing pulsar: 3 tones (4,8,12) --- */

void test_self_existing_pulsar_tones_are_4_8_12(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_INT(4,  p.pulsars[3].tones[0]);
    TEST_ASSERT_EQUAL_INT(8,  p.pulsars[3].tones[1]);
    TEST_ASSERT_EQUAL_INT(12, p.pulsars[3].tones[2]);
    TEST_ASSERT_EQUAL_INT(3,  p.pulsars[3].tone_count);
}

void test_self_existing_pulsar_name(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_EQUAL_STRING("Self-Existing", p.pulsars[3].name);
    TEST_ASSERT_EQUAL_STRING("4th", p.pulsars[3].dimension);
}

/* --- exactly 1 pulsar has contains_today=1 --- */

void test_exactly_one_pulsar_contains_today(void)
{
    for (int tone = 1; tone <= 13; tone++) {
        kin_pulsar_layout_t p = kin_pulsar_compute(tone);
        int count = 0;
        for (int i = 0; i < KIN_PULSAR_COUNT; i++) {
            if (p.pulsars[i].contains_today)
                count++;
        }
        TEST_ASSERT_EQUAL_INT(1, count);
    }
}

/* --- kin_pulsar_for_tone --- */

void test_pulsar_for_tone_1_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_pulsar_for_tone(1));
}

void test_pulsar_for_tone_2_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_pulsar_for_tone(2));
}

void test_pulsar_for_tone_3_is_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, kin_pulsar_for_tone(3));
}

void test_pulsar_for_tone_4_is_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, kin_pulsar_for_tone(4));
}

void test_pulsar_for_tone_5_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_pulsar_for_tone(5));
}

void test_pulsar_for_tone_13_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_pulsar_for_tone(13));
}

/* --- invalid tone returns -1 --- */

void test_pulsar_for_tone_0_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_pulsar_for_tone(0));
}

void test_pulsar_for_tone_14_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_pulsar_for_tone(14));
}

/* --- all pulsar colors have alpha=1.0 --- */

void test_all_pulsar_colors_alpha_1(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(7);
    for (int i = 0; i < KIN_PULSAR_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.pulsars[i].color[3]);
    }
}

/* --- Time pulsar color is gold-ish (r > 0.8) --- */

void test_time_pulsar_color_is_gold(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(1);
    TEST_ASSERT_TRUE(p.pulsars[0].color[0] > 0.8f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.pulsars[0].color[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, p.pulsars[0].color[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.55f, p.pulsars[0].color[2]);
}

/* --- invalid tone returns zeroed struct --- */

void test_compute_tone_0_returns_zeroed(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(0);
    TEST_ASSERT_EQUAL_INT(0, p.today_tone);
    TEST_ASSERT_EQUAL_INT(0, p.today_pulsar);
    TEST_ASSERT_NULL(p.pulsars[0].name);
}

void test_compute_tone_14_returns_zeroed(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(14);
    TEST_ASSERT_EQUAL_INT(0, p.today_tone);
    TEST_ASSERT_NULL(p.pulsars[0].name);
}

/* --- purity: same input, same output --- */

void test_purity_same_input_same_output(void)
{
    kin_pulsar_layout_t a = kin_pulsar_compute(7);
    kin_pulsar_layout_t b = kin_pulsar_compute(7);
    TEST_ASSERT_EQUAL_INT(a.today_tone, b.today_tone);
    TEST_ASSERT_EQUAL_INT(a.today_pulsar, b.today_pulsar);
    for (int i = 0; i < KIN_PULSAR_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.pulsars[i].tone_count, b.pulsars[i].tone_count);
        TEST_ASSERT_EQUAL_INT(a.pulsars[i].contains_today,
                              b.pulsars[i].contains_today);
    }
}

/* --- tone 3 in Mind pulsar (index 2) --- */

void test_compute_tone_3_today_pulsar_is_2(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(3);
    TEST_ASSERT_EQUAL_INT(2, p.today_pulsar);
    TEST_ASSERT_EQUAL_INT(1, p.pulsars[2].contains_today);
}

/* --- tone 4 in Self-Existing pulsar (index 3) --- */

void test_compute_tone_4_today_pulsar_is_3(void)
{
    kin_pulsar_layout_t p = kin_pulsar_compute(4);
    TEST_ASSERT_EQUAL_INT(3, p.today_pulsar);
    TEST_ASSERT_EQUAL_INT(1, p.pulsars[3].contains_today);
}

int main(void)
{
    UNITY_BEGIN();
    /* tone 1 */
    RUN_TEST(test_compute_tone_1_today_pulsar_is_0);
    RUN_TEST(test_compute_tone_1_today_tone_is_1);
    RUN_TEST(test_compute_tone_1_time_pulsar_contains_today);
    /* tone 2 */
    RUN_TEST(test_compute_tone_2_today_pulsar_is_1);
    RUN_TEST(test_compute_tone_2_sense_pulsar_contains_today);
    /* tone 13 */
    RUN_TEST(test_compute_tone_13_today_pulsar_is_0);
    RUN_TEST(test_compute_tone_13_today_tone_is_13);
    /* Time pulsar data */
    RUN_TEST(test_time_pulsar_name);
    RUN_TEST(test_time_pulsar_dimension);
    RUN_TEST(test_time_pulsar_tone_count_is_4);
    RUN_TEST(test_time_pulsar_tones_are_1_5_9_13);
    /* Sense pulsar data */
    RUN_TEST(test_sense_pulsar_name);
    RUN_TEST(test_sense_pulsar_tone_count_is_3);
    RUN_TEST(test_sense_pulsar_tones_are_2_6_10);
    /* Mind pulsar data */
    RUN_TEST(test_mind_pulsar_tones_are_3_7_11);
    /* Self-Existing pulsar data */
    RUN_TEST(test_self_existing_pulsar_tones_are_4_8_12);
    RUN_TEST(test_self_existing_pulsar_name);
    /* exactly 1 contains_today */
    RUN_TEST(test_exactly_one_pulsar_contains_today);
    /* kin_pulsar_for_tone */
    RUN_TEST(test_pulsar_for_tone_1_is_0);
    RUN_TEST(test_pulsar_for_tone_2_is_1);
    RUN_TEST(test_pulsar_for_tone_3_is_2);
    RUN_TEST(test_pulsar_for_tone_4_is_3);
    RUN_TEST(test_pulsar_for_tone_5_is_0);
    RUN_TEST(test_pulsar_for_tone_13_is_0);
    RUN_TEST(test_pulsar_for_tone_0_invalid);
    RUN_TEST(test_pulsar_for_tone_14_invalid);
    /* colors */
    RUN_TEST(test_all_pulsar_colors_alpha_1);
    RUN_TEST(test_time_pulsar_color_is_gold);
    /* invalid tone */
    RUN_TEST(test_compute_tone_0_returns_zeroed);
    RUN_TEST(test_compute_tone_14_returns_zeroed);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    /* tone 3 and 4 */
    RUN_TEST(test_compute_tone_3_today_pulsar_is_2);
    RUN_TEST(test_compute_tone_4_today_pulsar_is_3);
    return UNITY_END();
}
