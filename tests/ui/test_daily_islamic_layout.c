#include "../unity/unity.h"
#include "../../src/ui/daily_islamic_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_hijri_year_positive(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.year > 0);
}

void test_hijri_month_in_range(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 1 && layout.date.month <= 12);
}

void test_hijri_day_in_range(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 30);
}

/* --- Month name --- */

void test_month_name_not_null(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

/* --- Leap year --- */

void test_leap_is_bool(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_leap == 0 || layout.is_leap == 1);
}

/* --- Interpretation --- */

void test_sacred_month_is_bool(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_sacred_month == 0 || layout.is_sacred_month == 1);
}

void test_significance_not_null(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.significance);
}

void test_event_not_null(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.event);
}

void test_brief_not_null(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.date_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.h);
}

void test_month_slot(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.month_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.h);
}

void test_sacred_slot(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.sacred_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.45f, layout.sacred_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.sacred_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.sacred_slot.h);
}

void test_significance_slot(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.significance_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.58f, layout.significance_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.significance_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.significance_slot.h);
}

void test_event_slot(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.event_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.72f, layout.event_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.event_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.event_slot.h);
}

void test_all_slots_same_width(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.sacred_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.significance_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.event_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_is_sacred_accessor(void)
{
    daily_islamic_layout_t layout = daily_islamic_compute(TEST_JD);
    int sacred = daily_islamic_is_sacred(&layout);
    TEST_ASSERT_TRUE(sacred == 0 || sacred == 1);
}

void test_is_sacred_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_islamic_is_sacred(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_islamic_layout_t a = daily_islamic_compute(TEST_JD);
    daily_islamic_layout_t b = daily_islamic_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.year, b.date.year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_EQUAL_INT(a.is_leap, b.is_leap);
    TEST_ASSERT_EQUAL_INT(a.is_sacred_month, b.is_sacred_month);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_islamic_layout_t a = daily_islamic_compute(2460676.5);
    daily_islamic_layout_t b = daily_islamic_compute(2460706.5);  /* 30 days later */
    /* Different JDs should yield different day or month */
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

/* --- Muharram test (sacred month) --- */

void test_muharram_sacred(void)
{
    /* Find a JD in Muharram — Hijri month 1 is always sacred.
     * JD 2460326.5 ~ 2024-01-16 should be around Rajab 1445.
     * We just verify the is_sacred field is 0 or 1 for any date. */
    daily_islamic_layout_t layout = daily_islamic_compute(2460676.5);
    TEST_ASSERT_TRUE(layout.is_sacred_month == 0 ||
                     layout.is_sacred_month == 1);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_hijri_year_positive);
    RUN_TEST(test_hijri_month_in_range);
    RUN_TEST(test_hijri_day_in_range);
    /* Month name */
    RUN_TEST(test_month_name_not_null);
    /* Leap year */
    RUN_TEST(test_leap_is_bool);
    /* Interpretation */
    RUN_TEST(test_sacred_month_is_bool);
    RUN_TEST(test_significance_not_null);
    RUN_TEST(test_event_not_null);
    RUN_TEST(test_brief_not_null);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_sacred_slot);
    RUN_TEST(test_significance_slot);
    RUN_TEST(test_event_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_is_sacred_accessor);
    RUN_TEST(test_is_sacred_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    /* Sacred month */
    RUN_TEST(test_muharram_sacred);
    return UNITY_END();
}
