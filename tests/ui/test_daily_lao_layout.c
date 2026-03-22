#include "../unity/unity.h"
#include "../../src/ui/daily_lao_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_be_year_positive(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.be_year > 2500);
}

void test_month_in_range(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 1 && layout.date.month <= 12);
}

void test_day_in_range(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 31);
}

void test_month_name_not_null(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

void test_day_name_not_null(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.day_name);
    TEST_ASSERT_TRUE(strlen(layout.day_name) > 0);
}

/* --- Festival --- */

void test_festival_valid(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.festival >= 0 && layout.festival < LAO_FEST_COUNT);
}

void test_festival_name_not_null(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.festival_name);
}

/* --- Zodiac --- */

void test_zodiac_not_null(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.zodiac_animal);
    TEST_ASSERT_TRUE(strlen(layout.zodiac_animal) > 0);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.date_slot.y);
}

void test_month_slot(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.month_slot.y);
}

void test_festival_slot(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.festival_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.festival_slot.y);
}

void test_zodiac_slot(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.zodiac_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.zodiac_slot.y);
}

void test_day_name_slot(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.day_name_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.day_name_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.festival_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.zodiac_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.day_name_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_has_festival_accessor(void)
{
    daily_lao_layout_t layout = daily_lao_compute(TEST_JD);
    int f = daily_lao_has_festival(&layout);
    TEST_ASSERT_TRUE(f == 0 || f == 1);
}

void test_has_festival_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_lao_has_festival(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_lao_layout_t a = daily_lao_compute(TEST_JD);
    daily_lao_layout_t b = daily_lao_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.be_year, b.date.be_year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_lao_layout_t a = daily_lao_compute(2460676.5);
    daily_lao_layout_t b = daily_lao_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_be_year_positive);
    RUN_TEST(test_month_in_range);
    RUN_TEST(test_day_in_range);
    RUN_TEST(test_month_name_not_null);
    RUN_TEST(test_day_name_not_null);
    /* Festival */
    RUN_TEST(test_festival_valid);
    RUN_TEST(test_festival_name_not_null);
    /* Zodiac */
    RUN_TEST(test_zodiac_not_null);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_festival_slot);
    RUN_TEST(test_zodiac_slot);
    RUN_TEST(test_day_name_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_has_festival_accessor);
    RUN_TEST(test_has_festival_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
