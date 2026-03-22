#include "../unity/unity.h"
#include "../../src/ui/daily_celtic_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_date_month(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 0 && layout.date.month <= 13);
}

void test_date_day(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 28);
}

void test_tree_name_not_null(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.tree_name);
    TEST_ASSERT_TRUE(strlen(layout.tree_name) > 0);
}

void test_ogham_name_not_null(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.ogham_name);
    TEST_ASSERT_TRUE(strlen(layout.ogham_name) > 0);
}

/* --- Intercalary --- */

void test_is_intercalary_bool(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_intercalary == 0 ||
                     layout.is_intercalary == 1);
}

/* --- Festival --- */

void test_festival_valid_range(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.festival >= -1 && layout.festival <= 7);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.date_slot.y);
}

void test_tree_slot(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.tree_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.tree_slot.y);
}

void test_ogham_slot(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.ogham_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.ogham_slot.y);
}

void test_festival_slot(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.festival_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.festival_slot.y);
}

void test_meaning_slot(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meaning_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.meaning_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.tree_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.ogham_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.festival_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meaning_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_has_festival_accessor(void)
{
    daily_celtic_layout_t layout = daily_celtic_compute(TEST_JD);
    int f = daily_celtic_has_festival(&layout);
    TEST_ASSERT_TRUE(f == 0 || f == 1);
}

void test_has_festival_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_celtic_has_festival(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_celtic_layout_t a = daily_celtic_compute(TEST_JD);
    daily_celtic_layout_t b = daily_celtic_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.tree_name, b.tree_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_celtic_layout_t a = daily_celtic_compute(2460676.5);
    daily_celtic_layout_t b = daily_celtic_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_date_month);
    RUN_TEST(test_date_day);
    RUN_TEST(test_tree_name_not_null);
    RUN_TEST(test_ogham_name_not_null);
    /* Intercalary */
    RUN_TEST(test_is_intercalary_bool);
    /* Festival */
    RUN_TEST(test_festival_valid_range);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_tree_slot);
    RUN_TEST(test_ogham_slot);
    RUN_TEST(test_festival_slot);
    RUN_TEST(test_meaning_slot);
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
