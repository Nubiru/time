#include "../unity/unity.h"
#include "../../src/ui/daily_myanmar_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_year_positive(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.year > 1300);
}

void test_month_valid(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE((int)layout.date.month >= 0 &&
                     (int)layout.date.month < MY_MONTH_COUNT);
}

void test_day_in_range(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 30);
}

void test_month_name_not_null(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

/* --- Year type --- */

void test_year_type_valid(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.year_type >= 0 &&
                     layout.year_type < MY_YEAR_TYPE_COUNT);
}

void test_year_type_name_not_null(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.year_type_name);
    TEST_ASSERT_TRUE(strlen(layout.year_type_name) > 0);
}

/* --- Thingyan --- */

void test_is_thingyan_bool(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_thingyan == 0 || layout.is_thingyan == 1);
}

void test_january_not_thingyan(void)
{
    /* Thingyan is in April, so January should not be Thingyan */
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, layout.is_thingyan);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.date_slot.y);
}

void test_month_slot(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.month_slot.y);
}

void test_year_type_slot(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.year_type_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.year_type_slot.y);
}

void test_thingyan_slot(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.thingyan_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.thingyan_slot.y);
}

void test_info_slot(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.info_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.info_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.year_type_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.thingyan_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.info_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_is_thingyan_accessor(void)
{
    daily_myanmar_layout_t layout = daily_myanmar_compute(TEST_JD);
    int t = daily_myanmar_is_thingyan(&layout);
    TEST_ASSERT_TRUE(t == 0 || t == 1);
}

void test_is_thingyan_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_myanmar_is_thingyan(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_myanmar_layout_t a = daily_myanmar_compute(TEST_JD);
    daily_myanmar_layout_t b = daily_myanmar_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.year, b.date.year);
    TEST_ASSERT_EQUAL_INT((int)a.date.month, (int)b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_myanmar_layout_t a = daily_myanmar_compute(2460676.5);
    daily_myanmar_layout_t b = daily_myanmar_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     (int)a.date.month != (int)b.date.month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_year_positive);
    RUN_TEST(test_month_valid);
    RUN_TEST(test_day_in_range);
    RUN_TEST(test_month_name_not_null);
    /* Year type */
    RUN_TEST(test_year_type_valid);
    RUN_TEST(test_year_type_name_not_null);
    /* Thingyan */
    RUN_TEST(test_is_thingyan_bool);
    RUN_TEST(test_january_not_thingyan);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_year_type_slot);
    RUN_TEST(test_thingyan_slot);
    RUN_TEST(test_info_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_is_thingyan_accessor);
    RUN_TEST(test_is_thingyan_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
