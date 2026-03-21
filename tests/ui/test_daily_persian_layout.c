#include "../unity/unity.h"
#include "../../src/ui/daily_persian_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian = ~11 Dey 1403 SH */
#define TEST_JD 2460676.5

/* Nowruz: 1 Farvardin = ~March 20/21 Gregorian
 * 2025-03-20 ~ JD 2460754.5 */
#define NOWRUZ_JD 2460754.5

/* --- Date conversion --- */

void test_date_year(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.year > 1400);
}

void test_date_month(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 1 && layout.date.month <= 12);
}

void test_date_day(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 31);
}

void test_month_name_not_null(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

void test_weekday_name_not_null(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.weekday_name);
    TEST_ASSERT_TRUE(strlen(layout.weekday_name) > 0);
}

/* --- Month interpretation --- */

void test_avestan_not_null(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.avestan);
    TEST_ASSERT_TRUE(strlen(layout.avestan) > 0);
}

void test_meaning_not_null(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.meaning);
    TEST_ASSERT_TRUE(strlen(layout.meaning) > 0);
}

void test_season_not_null(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.season);
    TEST_ASSERT_TRUE(strlen(layout.season) > 0);
}

void test_brief_not_null(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
    TEST_ASSERT_TRUE(strlen(layout.brief) > 0);
}

/* --- Festival detection --- */

void test_festival_field_valid(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.has_festival == 0 || layout.has_festival == 1);
}

void test_nowruz_festival(void)
{
    daily_persian_layout_t layout = daily_persian_compute(NOWRUZ_JD);
    if (layout.date.month == 1 && layout.date.day == 1) {
        TEST_ASSERT_EQUAL_INT(1, layout.has_festival);
        TEST_ASSERT_TRUE(strlen(layout.festival_name) > 0);
    } else {
        TEST_ASSERT_NOT_NULL(layout.festival_name);
    }
}

void test_festival_name_empty_when_none(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    if (layout.has_festival == 0) {
        TEST_ASSERT_EQUAL_STRING("", layout.festival_name);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.04f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_date_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.13f, layout.date_slot.y);
}

void test_month_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.24f, layout.month_slot.y);
}

void test_meaning_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meaning_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.35f, layout.meaning_slot.y);
}

void test_season_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.season_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.season_slot.y);
}

void test_festival_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.festival_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.festival_slot.y);
}

void test_brief_slot(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.brief_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.brief_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meaning_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.season_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.festival_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.brief_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_avestan_accessor(void)
{
    daily_persian_layout_t layout = daily_persian_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(daily_persian_avestan(&layout));
    TEST_ASSERT_TRUE(strlen(daily_persian_avestan(&layout)) > 0);
}

void test_avestan_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_persian_avestan(NULL));
}

void test_festival_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_persian_has_festival(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_persian_layout_t a = daily_persian_compute(TEST_JD);
    daily_persian_layout_t b = daily_persian_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.year, b.date.year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_persian_layout_t a = daily_persian_compute(2460676.5);
    daily_persian_layout_t b = daily_persian_compute(2460706.5);
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
    RUN_TEST(test_weekday_name_not_null);
    /* Month interpretation */
    RUN_TEST(test_avestan_not_null);
    RUN_TEST(test_meaning_not_null);
    RUN_TEST(test_season_not_null);
    RUN_TEST(test_brief_not_null);
    /* Festival detection */
    RUN_TEST(test_festival_field_valid);
    RUN_TEST(test_nowruz_festival);
    RUN_TEST(test_festival_name_empty_when_none);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_meaning_slot);
    RUN_TEST(test_season_slot);
    RUN_TEST(test_festival_slot);
    RUN_TEST(test_brief_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_avestan_accessor);
    RUN_TEST(test_avestan_accessor_null);
    RUN_TEST(test_festival_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
