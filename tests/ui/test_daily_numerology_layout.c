#include "../unity/unity.h"
#include "../../src/ui/daily_numerology_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Daily number --- */

void test_daily_number_range(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    /* Must be 1-9 or master number 11, 22, 33 */
    int n = layout.daily_number;
    TEST_ASSERT_TRUE((n >= 1 && n <= 9) || n == 11 || n == 22 || n == 33);
}

void test_is_master_bool(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_master == 0 || layout.is_master == 1);
}

/* --- Universal year --- */

void test_universal_year_range(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    int n = layout.universal_year;
    TEST_ASSERT_TRUE((n >= 1 && n <= 9) || n == 11 || n == 22 || n == 33);
}

/* --- Date extraction --- */

void test_year_extracted(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(2025, layout.year);
}

void test_month_extracted(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, layout.month);
}

void test_day_extracted(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, layout.day);
}

/* --- Meaning --- */

void test_number_meaning_not_null(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.number_meaning);
    TEST_ASSERT_TRUE(strlen(layout.number_meaning) > 0);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_number_slot(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.number_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.number_slot.y);
}

void test_meaning_slot(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meaning_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.meaning_slot.y);
}

void test_universal_slot(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.universal_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.universal_slot.y);
}

void test_year_energy_slot(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.year_energy_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.year_energy_slot.y);
}

void test_detail_slot(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.detail_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.detail_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.number_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meaning_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.universal_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.year_energy_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.detail_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_numerology_layout_t layout = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_numerology_layout_t a = daily_numerology_compute(TEST_JD);
    daily_numerology_layout_t b = daily_numerology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.daily_number, b.daily_number);
    TEST_ASSERT_EQUAL_INT(a.is_master, b.is_master);
    TEST_ASSERT_EQUAL_INT(a.universal_year, b.universal_year);
    TEST_ASSERT_EQUAL_STRING(a.number_meaning, b.number_meaning);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_numerology_layout_t a = daily_numerology_compute(2460676.5);
    daily_numerology_layout_t b = daily_numerology_compute(2460677.5);
    /* Adjacent days: day changes, so at least day field differs */
    TEST_ASSERT_TRUE(a.day != b.day ||
                     a.daily_number != b.daily_number);
}

int main(void)
{
    UNITY_BEGIN();
    /* Daily number */
    RUN_TEST(test_daily_number_range);
    RUN_TEST(test_is_master_bool);
    /* Universal year */
    RUN_TEST(test_universal_year_range);
    /* Date extraction */
    RUN_TEST(test_year_extracted);
    RUN_TEST(test_month_extracted);
    RUN_TEST(test_day_extracted);
    /* Meaning */
    RUN_TEST(test_number_meaning_not_null);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_number_slot);
    RUN_TEST(test_meaning_slot);
    RUN_TEST(test_universal_slot);
    RUN_TEST(test_year_energy_slot);
    RUN_TEST(test_detail_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
