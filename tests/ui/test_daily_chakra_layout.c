#include "../unity/unity.h"
#include "../../src/ui/daily_chakra_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Chakra index --- */

void test_chakra_index_range(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.chakra_index >= 0 && layout.chakra_index <= 6);
}

/* --- Day of week --- */

void test_day_of_week_range(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.day_of_week >= 0 && layout.day_of_week <= 6);
}

/* --- Names --- */

void test_sanskrit_name_not_null(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.sanskrit_name);
    TEST_ASSERT_TRUE(strlen(layout.sanskrit_name) > 0);
}

void test_english_name_not_null(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.english_name);
    TEST_ASSERT_TRUE(strlen(layout.english_name) > 0);
}

/* --- Theme and question --- */

void test_theme_not_null(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.theme);
    TEST_ASSERT_TRUE(strlen(layout.theme) > 0);
}

void test_question_not_null(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.question);
    TEST_ASSERT_TRUE(strlen(layout.question) > 0);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Seven consecutive days cover all 7 chakras --- */

void test_seven_days_all_chakras(void)
{
    int seen[7] = {0};
    for (int i = 0; i < 7; i++) {
        daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD + (double)i);
        TEST_ASSERT_TRUE(layout.chakra_index >= 0 && layout.chakra_index <= 6);
        seen[layout.chakra_index] = 1;
    }
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_chakra_name_slot(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.chakra_name_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.chakra_name_slot.y);
}

void test_theme_slot(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.theme_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.theme_slot.y);
}

void test_question_slot(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.question_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.question_slot.y);
}

void test_energy_slot(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.energy_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.energy_slot.y);
}

void test_meditation_slot(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meditation_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.meditation_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.chakra_name_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.theme_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.question_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.energy_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meditation_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_chakra_layout_t layout = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_chakra_layout_t a = daily_chakra_compute(TEST_JD);
    daily_chakra_layout_t b = daily_chakra_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.chakra_index, b.chakra_index);
    TEST_ASSERT_EQUAL_INT(a.day_of_week, b.day_of_week);
    TEST_ASSERT_EQUAL_STRING(a.sanskrit_name, b.sanskrit_name);
    TEST_ASSERT_EQUAL_STRING(a.english_name, b.english_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_chakra_layout_t a = daily_chakra_compute(2460676.5);
    daily_chakra_layout_t b = daily_chakra_compute(2460677.5);
    /* Adjacent days must have different chakra index (different dow) */
    TEST_ASSERT_TRUE(a.chakra_index != b.chakra_index);
}

int main(void)
{
    UNITY_BEGIN();
    /* Chakra index */
    RUN_TEST(test_chakra_index_range);
    /* Day of week */
    RUN_TEST(test_day_of_week_range);
    /* Names */
    RUN_TEST(test_sanskrit_name_not_null);
    RUN_TEST(test_english_name_not_null);
    /* Theme and question */
    RUN_TEST(test_theme_not_null);
    RUN_TEST(test_question_not_null);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Seven days coverage */
    RUN_TEST(test_seven_days_all_chakras);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_chakra_name_slot);
    RUN_TEST(test_theme_slot);
    RUN_TEST(test_question_slot);
    RUN_TEST(test_energy_slot);
    RUN_TEST(test_meditation_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
