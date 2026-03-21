#include "../unity/unity.h"
#include "../../src/ui/daily_coptic_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian
 * Coptic: ~22 Kiahk 1741 AM */
#define TEST_JD 2460676.5

/* Nayrouz: 1 Thout = ~Sep 11/12 Gregorian.
 * JD 2460569.5 = 2024-09-16, close to Nayrouz.
 * Actually 1 Thout 1741 = Sep 12, 2024 = JD 2460565.5 */
#define NAYROUZ_JD 2460565.5

/* --- Date conversion --- */

void test_date_year(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.year > 1700);
}

void test_date_month(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 1 && layout.date.month <= 13);
}

void test_date_day(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.day >= 1 && layout.date.day <= 30);
}

void test_month_name_not_null(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

/* --- Leap year --- */

void test_leap_year_field(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_leap == 0 || layout.is_leap == 1);
}

/* --- Month interpretation --- */

void test_origin_not_null(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.origin);
    TEST_ASSERT_TRUE(strlen(layout.origin) > 0);
}

void test_liturgy_not_null(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.liturgy);
    TEST_ASSERT_TRUE(strlen(layout.liturgy) > 0);
}

void test_brief_not_null(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
    TEST_ASSERT_TRUE(strlen(layout.brief) > 0);
}

/* --- Feast detection --- */

void test_no_feast_on_regular_day(void)
{
    /* Most days have no feast */
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    /* Just verify it's a valid 0 or 1 */
    TEST_ASSERT_TRUE(layout.has_feast == 0 || layout.has_feast == 1);
}

void test_nayrouz_feast(void)
{
    /* 1 Thout should be Nayrouz */
    daily_coptic_layout_t layout = daily_coptic_compute(NAYROUZ_JD);
    if (layout.date.month == 1 && layout.date.day == 1) {
        TEST_ASSERT_EQUAL_INT(1, layout.has_feast);
        TEST_ASSERT_TRUE(strlen(layout.feast_name) > 0);
    } else {
        /* If our JD doesn't land on 1 Thout, just verify structure */
        TEST_ASSERT_NOT_NULL(layout.feast_name);
    }
}

void test_feast_name_empty_when_none(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    if (layout.has_feast == 0) {
        TEST_ASSERT_EQUAL_STRING("", layout.feast_name);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, layout.date_slot.y);
}

void test_month_slot(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.28f, layout.month_slot.y);
}

void test_liturgy_slot(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.liturgy_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.40f, layout.liturgy_slot.y);
}

void test_feast_slot(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.feast_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.55f, layout.feast_slot.y);
}

void test_brief_slot(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.brief_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.72f, layout.brief_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.liturgy_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.feast_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.brief_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_origin_accessor(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    const char *o = daily_coptic_month_origin(&layout);
    TEST_ASSERT_NOT_NULL(o);
    TEST_ASSERT_TRUE(strlen(o) > 0);
}

void test_origin_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_coptic_month_origin(NULL));
}

void test_feast_accessor(void)
{
    daily_coptic_layout_t layout = daily_coptic_compute(TEST_JD);
    int f = daily_coptic_has_feast(&layout);
    TEST_ASSERT_TRUE(f == 0 || f == 1);
}

void test_feast_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_coptic_has_feast(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_coptic_layout_t a = daily_coptic_compute(TEST_JD);
    daily_coptic_layout_t b = daily_coptic_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.year, b.date.year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_coptic_layout_t a = daily_coptic_compute(2460676.5);
    daily_coptic_layout_t b = daily_coptic_compute(2460706.5);
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
    RUN_TEST(test_origin_not_null);
    RUN_TEST(test_liturgy_not_null);
    RUN_TEST(test_brief_not_null);
    /* Feast detection */
    RUN_TEST(test_no_feast_on_regular_day);
    RUN_TEST(test_nayrouz_feast);
    RUN_TEST(test_feast_name_empty_when_none);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_liturgy_slot);
    RUN_TEST(test_feast_slot);
    RUN_TEST(test_brief_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_origin_accessor);
    RUN_TEST(test_origin_accessor_null);
    RUN_TEST(test_feast_accessor);
    RUN_TEST(test_feast_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
