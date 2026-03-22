#include "../unity/unity.h"
#include "../../src/ui/daily_aztec_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Tonalpohualli --- */

void test_day_sign_in_range(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.tonal.day_sign >= 0 &&
                     layout.tonal.day_sign <= 19);
}

void test_day_number_in_range(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.tonal.day_number >= 1 &&
                     layout.tonal.day_number <= 13);
}

void test_day_index_in_range(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.tonal.day_index >= 1 &&
                     layout.tonal.day_index <= 260);
}

void test_sign_name_not_null(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.sign_name);
    TEST_ASSERT_TRUE(strlen(layout.sign_name) > 0);
}

/* --- Calendar Round --- */

void test_year_bearer_in_range(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.round.year_bearer >= 0 &&
                     layout.round.year_bearer <= 3);
}

void test_year_number_in_range(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.round.year_number >= 1 &&
                     layout.round.year_number <= 13);
}

void test_round_year_in_range(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.round.round_year >= 1 &&
                     layout.round.round_year <= 52);
}

void test_bearer_name_not_null(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.bearer_name);
    TEST_ASSERT_TRUE(strlen(layout.bearer_name) > 0);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_tonal_slot(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.tonal_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.tonal_slot.y);
}

void test_sign_slot(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.sign_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.sign_slot.y);
}

void test_round_slot(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.round_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.round_slot.y);
}

void test_bearer_slot(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.bearer_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.bearer_slot.y);
}

void test_cycle_slot(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.cycle_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.cycle_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.tonal_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.sign_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.round_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.bearer_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.cycle_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_sign_name_accessor(void)
{
    daily_aztec_layout_t layout = daily_aztec_compute(TEST_JD);
    const char *name = daily_aztec_sign_name(&layout);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_sign_name_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_aztec_sign_name(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_aztec_layout_t a = daily_aztec_compute(TEST_JD);
    daily_aztec_layout_t b = daily_aztec_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.tonal.day_sign, b.tonal.day_sign);
    TEST_ASSERT_EQUAL_INT(a.tonal.day_number, b.tonal.day_number);
    TEST_ASSERT_EQUAL_INT(a.round.year_bearer, b.round.year_bearer);
    TEST_ASSERT_EQUAL_STRING(a.sign_name, b.sign_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_aztec_layout_t a = daily_aztec_compute(2460676.5);
    daily_aztec_layout_t b = daily_aztec_compute(2460706.5);
    TEST_ASSERT_TRUE(a.tonal.day_sign != b.tonal.day_sign ||
                     a.tonal.day_number != b.tonal.day_number);
}

int main(void)
{
    UNITY_BEGIN();
    /* Tonalpohualli */
    RUN_TEST(test_day_sign_in_range);
    RUN_TEST(test_day_number_in_range);
    RUN_TEST(test_day_index_in_range);
    RUN_TEST(test_sign_name_not_null);
    /* Calendar Round */
    RUN_TEST(test_year_bearer_in_range);
    RUN_TEST(test_year_number_in_range);
    RUN_TEST(test_round_year_in_range);
    RUN_TEST(test_bearer_name_not_null);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_tonal_slot);
    RUN_TEST(test_sign_slot);
    RUN_TEST(test_round_slot);
    RUN_TEST(test_bearer_slot);
    RUN_TEST(test_cycle_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_sign_name_accessor);
    RUN_TEST(test_sign_name_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
