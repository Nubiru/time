#include "../unity/unity.h"
#include "../../src/ui/kin_wavespell_journey.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Phase assignment --- */

void test_tone_1_phase_initiate(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(1, j.today_tone);
    TEST_ASSERT_EQUAL_INT(0, j.phase);
}

void test_tone_4_phase_initiate(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(4);
    TEST_ASSERT_EQUAL_INT(0, j.phase);
}

void test_tone_5_phase_extend(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(5);
    TEST_ASSERT_EQUAL_INT(1, j.phase);
}

void test_tone_7_phase_extend(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(7);
    TEST_ASSERT_EQUAL_INT(1, j.phase);
}

void test_tone_9_phase_convert(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(9);
    TEST_ASSERT_EQUAL_INT(2, j.phase);
}

void test_tone_10_phase_convert(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(10);
    TEST_ASSERT_EQUAL_INT(2, j.phase);
}

void test_tone_13_phase_complete(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(13);
    TEST_ASSERT_EQUAL_INT(13, j.today_tone);
    TEST_ASSERT_EQUAL_INT(3, j.phase);
}

/* --- Highlighting --- */

void test_tone_1_highlights_step_0(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(1, j.steps[0].highlighted);
}

void test_tone_13_highlights_step_12(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(13);
    TEST_ASSERT_EQUAL_INT(1, j.steps[12].highlighted);
}

void test_only_one_step_highlighted(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(7);
    int count = 0;
    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        count += j.steps[i].highlighted;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Gate tones --- */

void test_gate_tone_1(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(1, j.steps[0].is_gate);   /* tone 1 = Magnetic */
}

void test_gate_tone_5(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(1, j.steps[4].is_gate);   /* tone 5 = Overtone */
}

void test_gate_tone_9(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(1, j.steps[8].is_gate);   /* tone 9 = Solar */
}

void test_gate_tone_13(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(1, j.steps[12].is_gate);  /* tone 13 = Cosmic */
}

void test_non_gate_tone_2(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(0, j.steps[1].is_gate);   /* tone 2 = Lunar */
}

void test_non_gate_tone_6(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_INT(0, j.steps[5].is_gate);   /* tone 6 = Rhythmic */
}

void test_exactly_four_gates(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    int count = 0;
    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        count += j.steps[i].is_gate;
    }
    TEST_ASSERT_EQUAL_INT(4, count);
}

/* --- Tone data validity --- */

void test_all_steps_have_valid_names(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(7);
    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        TEST_ASSERT_NOT_NULL(j.steps[i].name);
        TEST_ASSERT_TRUE(strcmp(j.steps[i].name, "?") != 0);
    }
}

void test_all_steps_have_valid_actions(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(3);
    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        TEST_ASSERT_NOT_NULL(j.steps[i].action);
        TEST_ASSERT_TRUE(strcmp(j.steps[i].action, "?") != 0);
    }
}

void test_all_steps_have_valid_powers(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(11);
    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        TEST_ASSERT_NOT_NULL(j.steps[i].power);
        TEST_ASSERT_TRUE(strcmp(j.steps[i].power, "?") != 0);
    }
}

void test_step_tone_values_sequential(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, j.steps[i].tone);
    }
}

/* --- Arc positions --- */

void test_step_0_x_position(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, j.steps[0].x);
}

void test_step_12_x_position(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.95f, j.steps[12].x);
}

void test_x_positions_increase_left_to_right(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(5);
    for (int i = 1; i < KIN_JOURNEY_STEPS; i++) {
        TEST_ASSERT_TRUE(j.steps[i].x > j.steps[i - 1].x);
    }
}

void test_arc_center_higher_than_ends(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    /* y decreases toward top, so center steps have lower y than ends */
    float y_left = j.steps[0].y;
    float y_right = j.steps[12].y;
    float y_center = j.steps[6].y;
    TEST_ASSERT_TRUE(y_center < y_left);
    TEST_ASSERT_TRUE(y_center < y_right);
}

void test_endpoints_y_near_0_5(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    /* At step 0: sin(0) = 0, so y = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, j.steps[0].y);
    /* At step 12: sin(PI) = 0, so y = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, j.steps[12].y);
}

/* --- Invalid input --- */

void test_tone_0_returns_zeroed(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(0);
    TEST_ASSERT_EQUAL_INT(0, j.today_tone);
    TEST_ASSERT_EQUAL_INT(0, j.phase);
    TEST_ASSERT_NULL(j.steps[0].name);
}

void test_tone_14_returns_zeroed(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(14);
    TEST_ASSERT_EQUAL_INT(0, j.today_tone);
    TEST_ASSERT_EQUAL_INT(0, j.phase);
    TEST_ASSERT_NULL(j.steps[0].name);
}

void test_tone_negative_returns_zeroed(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(-5);
    TEST_ASSERT_EQUAL_INT(0, j.today_tone);
}

/* --- Phase names --- */

void test_phase_name_initiate(void)
{
    TEST_ASSERT_EQUAL_STRING("Initiate", kin_journey_phase_name(0));
}

void test_phase_name_extend(void)
{
    TEST_ASSERT_EQUAL_STRING("Extend", kin_journey_phase_name(1));
}

void test_phase_name_convert(void)
{
    TEST_ASSERT_EQUAL_STRING("Convert", kin_journey_phase_name(2));
}

void test_phase_name_complete(void)
{
    TEST_ASSERT_EQUAL_STRING("Complete", kin_journey_phase_name(3));
}

void test_phase_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kin_journey_phase_name(4));
    TEST_ASSERT_EQUAL_STRING("?", kin_journey_phase_name(-1));
}

/* --- Specific tone data spot checks --- */

void test_step_0_is_magnetic(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(1);
    TEST_ASSERT_EQUAL_STRING("Magnetic", j.steps[0].name);
    TEST_ASSERT_EQUAL_STRING("Unify", j.steps[0].action);
    TEST_ASSERT_EQUAL_STRING("Purpose", j.steps[0].power);
}

void test_step_12_is_cosmic(void)
{
    kin_wavespell_journey_t j = kin_journey_compute(13);
    TEST_ASSERT_EQUAL_STRING("Cosmic", j.steps[12].name);
    TEST_ASSERT_EQUAL_STRING("Endure", j.steps[12].action);
    TEST_ASSERT_EQUAL_STRING("Presence", j.steps[12].power);
}

int main(void)
{
    UNITY_BEGIN();

    /* Phase assignment */
    RUN_TEST(test_tone_1_phase_initiate);
    RUN_TEST(test_tone_4_phase_initiate);
    RUN_TEST(test_tone_5_phase_extend);
    RUN_TEST(test_tone_7_phase_extend);
    RUN_TEST(test_tone_9_phase_convert);
    RUN_TEST(test_tone_10_phase_convert);
    RUN_TEST(test_tone_13_phase_complete);

    /* Highlighting */
    RUN_TEST(test_tone_1_highlights_step_0);
    RUN_TEST(test_tone_13_highlights_step_12);
    RUN_TEST(test_only_one_step_highlighted);

    /* Gate tones */
    RUN_TEST(test_gate_tone_1);
    RUN_TEST(test_gate_tone_5);
    RUN_TEST(test_gate_tone_9);
    RUN_TEST(test_gate_tone_13);
    RUN_TEST(test_non_gate_tone_2);
    RUN_TEST(test_non_gate_tone_6);
    RUN_TEST(test_exactly_four_gates);

    /* Tone data validity */
    RUN_TEST(test_all_steps_have_valid_names);
    RUN_TEST(test_all_steps_have_valid_actions);
    RUN_TEST(test_all_steps_have_valid_powers);
    RUN_TEST(test_step_tone_values_sequential);

    /* Arc positions */
    RUN_TEST(test_step_0_x_position);
    RUN_TEST(test_step_12_x_position);
    RUN_TEST(test_x_positions_increase_left_to_right);
    RUN_TEST(test_arc_center_higher_than_ends);
    RUN_TEST(test_endpoints_y_near_0_5);

    /* Invalid input */
    RUN_TEST(test_tone_0_returns_zeroed);
    RUN_TEST(test_tone_14_returns_zeroed);
    RUN_TEST(test_tone_negative_returns_zeroed);

    /* Phase names */
    RUN_TEST(test_phase_name_initiate);
    RUN_TEST(test_phase_name_extend);
    RUN_TEST(test_phase_name_convert);
    RUN_TEST(test_phase_name_complete);
    RUN_TEST(test_phase_name_invalid);

    /* Specific tone data */
    RUN_TEST(test_step_0_is_magnetic);
    RUN_TEST(test_step_12_is_cosmic);

    return UNITY_END();
}
