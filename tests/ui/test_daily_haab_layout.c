#include "../unity/unity.h"
#include "../../src/ui/daily_haab_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_month_in_range(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.month >= 0 && layout.date.month <= 18);
}

void test_day_in_range(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    if (layout.date.month < 18) {
        TEST_ASSERT_TRUE(layout.date.day >= 0 && layout.date.day <= 19);
    } else {
        /* Wayeb: 0-4 */
        TEST_ASSERT_TRUE(layout.date.day >= 0 && layout.date.day <= 4);
    }
}

void test_month_name_not_null(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

void test_month_meaning_not_null(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_meaning);
    TEST_ASSERT_TRUE(strlen(layout.month_meaning) > 0);
}

void test_day_of_year_in_range(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.day_of_year >= 0 && layout.day_of_year <= 364);
}

/* --- Vinal wisdom --- */

void test_vinal_number_valid(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.vinal_number >= 1 && layout.vinal_number <= 19);
}

void test_vinal_wisdom_not_null(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.vinal_wisdom);
    TEST_ASSERT_TRUE(strlen(layout.vinal_wisdom) > 0);
}

void test_wayeb_field_valid(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_wayeb == 0 || layout.is_wayeb == 1);
}

void test_wayeb_consistent(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    if (layout.date.month == 18) {
        TEST_ASSERT_EQUAL_INT(1, layout.is_wayeb);
    } else {
        TEST_ASSERT_EQUAL_INT(0, layout.is_wayeb);
    }
}

/* --- Calendar Round --- */

void test_round_kin_valid(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.cal_round.kin >= 1 && layout.cal_round.kin <= 260);
}

void test_round_tone_valid(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.cal_round.tone >= 1 && layout.cal_round.tone <= 13);
}

void test_round_seal_valid(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.cal_round.seal >= 0 && layout.cal_round.seal <= 19);
}

void test_round_fmt_not_empty(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(strlen(layout.round_fmt) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.04f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_date_slot(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.14f, layout.date_slot.y);
}

void test_meaning_slot(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meaning_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.26f, layout.meaning_slot.y);
}

void test_wisdom_slot(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.wisdom_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.40f, layout.wisdom_slot.y);
}

void test_round_slot(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.round_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.56f, layout.round_slot.y);
}

void test_progress_slot(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.progress_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.70f, layout.progress_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meaning_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.wisdom_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.round_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.progress_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_wisdom_accessor(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    const char *w = daily_haab_wisdom(&layout);
    TEST_ASSERT_NOT_NULL(w);
    TEST_ASSERT_TRUE(strlen(w) > 0);
}

void test_wisdom_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_haab_wisdom(NULL));
}

void test_wayeb_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_haab_is_wayeb(NULL));
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_haab_layout_t layout = daily_haab_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_haab_layout_t a = daily_haab_compute(TEST_JD);
    daily_haab_layout_t b = daily_haab_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.month, b.date.month);
    TEST_ASSERT_EQUAL_INT(a.date.day, b.date.day);
    TEST_ASSERT_EQUAL_INT(a.vinal_number, b.vinal_number);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_EQUAL_STRING(a.round_fmt, b.round_fmt);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_haab_layout_t a = daily_haab_compute(2460676.5);
    daily_haab_layout_t b = daily_haab_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_month_in_range);
    RUN_TEST(test_day_in_range);
    RUN_TEST(test_month_name_not_null);
    RUN_TEST(test_month_meaning_not_null);
    RUN_TEST(test_day_of_year_in_range);
    /* Vinal wisdom */
    RUN_TEST(test_vinal_number_valid);
    RUN_TEST(test_vinal_wisdom_not_null);
    RUN_TEST(test_wayeb_field_valid);
    RUN_TEST(test_wayeb_consistent);
    /* Calendar Round */
    RUN_TEST(test_round_kin_valid);
    RUN_TEST(test_round_tone_valid);
    RUN_TEST(test_round_seal_valid);
    RUN_TEST(test_round_fmt_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_meaning_slot);
    RUN_TEST(test_wisdom_slot);
    RUN_TEST(test_round_slot);
    RUN_TEST(test_progress_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_wisdom_accessor);
    RUN_TEST(test_wisdom_accessor_null);
    RUN_TEST(test_wayeb_accessor_null);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
