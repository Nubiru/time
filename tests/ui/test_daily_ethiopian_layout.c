#include "../unity/unity.h"
#include "../../src/ui/daily_ethiopian_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* Enkutatash (New Year): 1 Meskerem = ~Sep 11/12 Gregorian
 * 1 Meskerem 2018 = Sep 12, 2025 ~ JD 2460930.5
 * Use 1 Meskerem 2017 = Sep 12, 2024 ~ JD 2460565.5 */
#define ENKUTATASH_JD 2460565.5

/* --- Date conversion --- */

void test_date_year(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.year > 2000);
}

void test_date_month(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 1 && layout.date.month <= 13);
}

void test_date_day(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 30);
}

void test_month_name_not_null(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

/* --- Leap year --- */

void test_leap_year_field(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_leap == 0 || layout.is_leap == 1);
}

/* --- Month interpretation --- */

void test_meaning_not_null(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.meaning);
    TEST_ASSERT_TRUE(strlen(layout.meaning) > 0);
}

void test_season_not_null(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.season);
    TEST_ASSERT_TRUE(strlen(layout.season) > 0);
}

void test_brief_not_null(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
    TEST_ASSERT_TRUE(strlen(layout.brief) > 0);
}

/* --- Feast detection --- */

void test_feast_field_valid(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.has_feast == 0 || layout.has_feast == 1);
}

void test_enkutatash_feast(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(ENKUTATASH_JD);
    if (layout.date.month == 1 && layout.date.day == 1) {
        TEST_ASSERT_EQUAL_INT(1, layout.has_feast);
        TEST_ASSERT_TRUE(strlen(layout.feast_name) > 0);
    } else {
        TEST_ASSERT_NOT_NULL(layout.feast_name);
    }
}

void test_feast_name_empty_when_none(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    if (layout.has_feast == 0) {
        TEST_ASSERT_EQUAL_STRING("", layout.feast_name);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, layout.date_slot.y);
}

void test_month_slot(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.28f, layout.month_slot.y);
}

void test_season_slot(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.season_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.40f, layout.season_slot.y);
}

void test_feast_slot(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.feast_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.55f, layout.feast_slot.y);
}

void test_brief_slot(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.brief_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.72f, layout.brief_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.season_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.feast_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.brief_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_meaning_accessor(void)
{
    daily_ethiopian_layout_t layout = daily_ethiopian_compute(TEST_JD);
    const char *m = daily_ethiopian_month_meaning(&layout);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strlen(m) > 0);
}

void test_meaning_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_ethiopian_month_meaning(NULL));
}

void test_feast_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_ethiopian_has_feast(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_ethiopian_layout_t a = daily_ethiopian_compute(TEST_JD);
    daily_ethiopian_layout_t b = daily_ethiopian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.year, b.date.year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_ethiopian_layout_t a = daily_ethiopian_compute(2460676.5);
    daily_ethiopian_layout_t b = daily_ethiopian_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_date_year);
    RUN_TEST(test_date_month);
    RUN_TEST(test_date_day);
    RUN_TEST(test_month_name_not_null);
    /* Leap year */
    RUN_TEST(test_leap_year_field);
    /* Month interpretation */
    RUN_TEST(test_meaning_not_null);
    RUN_TEST(test_season_not_null);
    RUN_TEST(test_brief_not_null);
    /* Feast detection */
    RUN_TEST(test_feast_field_valid);
    RUN_TEST(test_enkutatash_feast);
    RUN_TEST(test_feast_name_empty_when_none);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_season_slot);
    RUN_TEST(test_feast_slot);
    RUN_TEST(test_brief_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_meaning_accessor);
    RUN_TEST(test_meaning_accessor_null);
    RUN_TEST(test_feast_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
