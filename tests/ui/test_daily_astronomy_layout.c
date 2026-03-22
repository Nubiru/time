#include "../unity/unity.h"
#include "../../src/ui/daily_astronomy_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Moon phase --- */

void test_moon_phase_range(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.moon_phase >= 0 && layout.moon_phase <= 7);
}

void test_moon_phase_name_not_null(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.moon_phase_name);
    TEST_ASSERT_TRUE(strlen(layout.moon_phase_name) > 0);
}

/* --- Illumination --- */

void test_illumination_range(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.illumination >= 0.0 && layout.illumination <= 1.0);
}

/* --- Moon age --- */

void test_moon_age_non_negative(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.moon_age_days >= 0.0);
}

/* --- Zodiac sign --- */

void test_zodiac_sign_range(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.zodiac_sign >= 0 && layout.zodiac_sign <= 11);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_phase_slot(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.phase_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.phase_slot.y);
}

void test_illumination_slot(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.illumination_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.illumination_slot.y);
}

void test_age_slot(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.age_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.age_slot.y);
}

void test_zodiac_slot(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.zodiac_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.zodiac_slot.y);
}

void test_detail_slot(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.detail_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.detail_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.phase_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.illumination_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.age_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.zodiac_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.detail_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_astronomy_layout_t layout = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_astronomy_layout_t a = daily_astronomy_compute(TEST_JD);
    daily_astronomy_layout_t b = daily_astronomy_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.moon_phase, b.moon_phase);
    TEST_ASSERT_EQUAL_STRING(a.moon_phase_name, b.moon_phase_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)a.illumination, (float)b.illumination);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_astronomy_layout_t a = daily_astronomy_compute(2460676.5);
    daily_astronomy_layout_t b = daily_astronomy_compute(2460706.5);
    /* 30 days apart — moon phase or illumination must differ */
    TEST_ASSERT_TRUE(a.moon_phase != b.moon_phase ||
                     a.illumination != b.illumination);
}

int main(void)
{
    UNITY_BEGIN();
    /* Moon phase */
    RUN_TEST(test_moon_phase_range);
    RUN_TEST(test_moon_phase_name_not_null);
    /* Illumination */
    RUN_TEST(test_illumination_range);
    /* Moon age */
    RUN_TEST(test_moon_age_non_negative);
    /* Zodiac sign */
    RUN_TEST(test_zodiac_sign_range);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_phase_slot);
    RUN_TEST(test_illumination_slot);
    RUN_TEST(test_age_slot);
    RUN_TEST(test_zodiac_slot);
    RUN_TEST(test_detail_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
