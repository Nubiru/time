#include "../unity/unity.h"
#include "../../src/ui/daily_hebrew_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 = 1 Tevet 5785 */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_date_year(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(5785, layout.date.year);
}

void test_date_month_tevet(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    /* Tevet is month 10 in the Hebrew calendar */
    TEST_ASSERT_EQUAL_INT(10, layout.date.month);
}

void test_date_day(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, layout.date.day);
}

void test_month_name_tevet(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Tevet", layout.month_name);
}

/* --- Leap year --- */

void test_not_leap_year(void)
{
    /* 5785 is not a leap year (position 8 in Metonic cycle, not a leap) */
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, layout.is_leap);
}

/* --- Month interpretation --- */

void test_hebrew_letter(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    /* Tevet (month 10) has letter "Ayin" */
    TEST_ASSERT_NOT_NULL(layout.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Ayin", layout.hebrew_letter);
}

void test_tribe(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    /* Tevet (month 10) has tribe "Dan" */
    TEST_ASSERT_NOT_NULL(layout.tribe);
    TEST_ASSERT_EQUAL_STRING("Dan", layout.tribe);
}

void test_sense(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    /* Tevet (month 10) sense = "Anger (righteous)" */
    TEST_ASSERT_NOT_NULL(layout.sense);
    TEST_ASSERT_TRUE(strlen(layout.sense) > 0);
}

void test_quality(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    /* Tevet (month 10) quality = "Transformation" */
    TEST_ASSERT_NOT_NULL(layout.quality);
    TEST_ASSERT_EQUAL_STRING("Transformation", layout.quality);
}

void test_brief(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
    TEST_ASSERT_TRUE(strlen(layout.brief) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_date_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, layout.date_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.h);
}

void test_month_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.28f, layout.month_slot.y);
}

void test_letter_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.letter_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.40f, layout.letter_slot.y);
}

void test_tribe_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.tribe_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.52f, layout.tribe_slot.y);
}

void test_quality_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.quality_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.65f, layout.quality_slot.y);
}

void test_brief_slot(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.brief_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.78f, layout.brief_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.letter_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.tribe_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.quality_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.brief_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Shabbat detection --- */

void test_shabbat_wednesday(void)
{
    /* 2025-01-15 (JD 2460676.5) is a Wednesday, not Shabbat */
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, daily_hebrew_is_shabbat(&layout));
}

void test_shabbat_saturday(void)
{
    /* 2025-01-18 (JD 2460679.5) is a Saturday = Shabbat */
    daily_hebrew_layout_t layout = daily_hebrew_compute(2460679.5);
    TEST_ASSERT_EQUAL_INT(1, daily_hebrew_is_shabbat(&layout));
}

void test_shabbat_null_layout(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_hebrew_is_shabbat(NULL));
}

/* --- Accessor: daily_hebrew_month_quality --- */

void test_quality_accessor(void)
{
    daily_hebrew_layout_t layout = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Transformation",
                             daily_hebrew_month_quality(&layout));
}

void test_quality_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_hebrew_month_quality(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_hebrew_layout_t a = daily_hebrew_compute(TEST_JD);
    daily_hebrew_layout_t b = daily_hebrew_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.year, b.date.year);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_EQUAL_STRING(a.quality, b.quality);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_w, b.card_w);
}

void test_purity_different_dates(void)
{
    daily_hebrew_layout_t a = daily_hebrew_compute(2460676.5);
    daily_hebrew_layout_t b = daily_hebrew_compute(2460706.5);  /* 30 days later */
    /* Different dates should yield different day */
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

/* --- Different month test (Nisan) --- */

void test_nisan_month(void)
{
    /* 2025-04-15 ~ JD 2460766.5, should be around Nisan 5785 */
    daily_hebrew_layout_t layout = daily_hebrew_compute(2460780.5);
    /* Just verify we get a valid layout with a different month */
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
    TEST_ASSERT_NOT_NULL(layout.hebrew_letter);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_date_year);
    RUN_TEST(test_date_month_tevet);
    RUN_TEST(test_date_day);
    RUN_TEST(test_month_name_tevet);
    /* Leap year */
    RUN_TEST(test_not_leap_year);
    /* Month interpretation */
    RUN_TEST(test_hebrew_letter);
    RUN_TEST(test_tribe);
    RUN_TEST(test_sense);
    RUN_TEST(test_quality);
    RUN_TEST(test_brief);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_letter_slot);
    RUN_TEST(test_tribe_slot);
    RUN_TEST(test_quality_slot);
    RUN_TEST(test_brief_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Shabbat detection */
    RUN_TEST(test_shabbat_wednesday);
    RUN_TEST(test_shabbat_saturday);
    RUN_TEST(test_shabbat_null_layout);
    /* Accessor */
    RUN_TEST(test_quality_accessor);
    RUN_TEST(test_quality_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    /* Different month */
    RUN_TEST(test_nisan_month);
    return UNITY_END();
}
