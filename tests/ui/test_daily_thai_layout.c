#include "../unity/unity.h"
#include "../../src/ui/daily_thai_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian = BE 2568 */
#define TEST_JD 2460676.5

/* Songkran: April 13-15, 2025 ~ JD 2460778.5 */
#define SONGKRAN_JD 2460778.5

/* --- Date conversion --- */

void test_be_year(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(2568, layout.date.be_year);
}

void test_month(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, layout.date.month); /* January */
}

void test_day(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, layout.date.day);
}

void test_month_name_not_null(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

void test_month_abbr_not_null(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_abbr);
    TEST_ASSERT_TRUE(strlen(layout.month_abbr) > 0);
}

/* --- Songkran detection --- */

void test_not_songkran_in_january(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, layout.is_songkran);
}

void test_songkran_in_april(void)
{
    daily_thai_layout_t layout = daily_thai_compute(SONGKRAN_JD);
    /* April 13 should be Songkran */
    if (layout.date.month == 4 && layout.date.day >= 13 &&
        layout.date.day <= 15) {
        TEST_ASSERT_EQUAL_INT(1, layout.is_songkran);
    }
}

/* --- Month interpretation --- */

void test_origin_not_null(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.origin);
    TEST_ASSERT_TRUE(strlen(layout.origin) > 0);
}

void test_season_not_null(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.season);
    TEST_ASSERT_TRUE(strlen(layout.season) > 0);
}

void test_brief_not_null(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
    TEST_ASSERT_TRUE(strlen(layout.brief) > 0);
}

/* --- Festival detection --- */

void test_festival_field_valid(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.has_festival == 0 || layout.has_festival == 1);
}

void test_festival_name_empty_when_none(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    if (layout.has_festival == 0) {
        TEST_ASSERT_EQUAL_STRING("", layout.festival_name);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_date_slot(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, layout.date_slot.y);
}

void test_month_slot(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.28f, layout.month_slot.y);
}

void test_season_slot(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.season_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.40f, layout.season_slot.y);
}

void test_festival_slot(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.festival_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.55f, layout.festival_slot.y);
}

void test_brief_slot(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.brief_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.72f, layout.brief_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.season_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.festival_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.brief_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_origin_accessor(void)
{
    daily_thai_layout_t layout = daily_thai_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(daily_thai_month_origin(&layout));
    TEST_ASSERT_TRUE(strlen(daily_thai_month_origin(&layout)) > 0);
}

void test_origin_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_thai_month_origin(NULL));
}

void test_festival_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_thai_has_festival(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_thai_layout_t a = daily_thai_compute(TEST_JD);
    daily_thai_layout_t b = daily_thai_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.be_year, b.date.be_year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_thai_layout_t a = daily_thai_compute(2460676.5);
    daily_thai_layout_t b = daily_thai_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_be_year);
    RUN_TEST(test_month);
    RUN_TEST(test_day);
    RUN_TEST(test_month_name_not_null);
    RUN_TEST(test_month_abbr_not_null);
    /* Songkran */
    RUN_TEST(test_not_songkran_in_january);
    RUN_TEST(test_songkran_in_april);
    /* Month interpretation */
    RUN_TEST(test_origin_not_null);
    RUN_TEST(test_season_not_null);
    RUN_TEST(test_brief_not_null);
    /* Festival detection */
    RUN_TEST(test_festival_field_valid);
    RUN_TEST(test_festival_name_empty_when_none);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_season_slot);
    RUN_TEST(test_festival_slot);
    RUN_TEST(test_brief_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_origin_accessor);
    RUN_TEST(test_origin_accessor_null);
    RUN_TEST(test_festival_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
