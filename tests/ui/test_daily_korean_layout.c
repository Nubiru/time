#include "../unity/unity.h"
#include "../../src/ui/daily_korean_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian
 * Dangun year ~4358, Year of the Wood Snake */
#define TEST_JD 2460676.5

/* --- Date conversion --- */

void test_dangun_year(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.dangun_year > 4300);
}

void test_gregorian_year(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    /* Korean lunisolar year adjusts for lunar New Year —
     * Jan 1 Gregorian may fall before Korean NY, so year can be prior */
    TEST_ASSERT_TRUE(layout.date.gregorian_year == 2024 ||
                     layout.date.gregorian_year == 2025);
}

void test_animal_valid(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.animal >= 0 && layout.date.animal <= 11);
}

void test_element_valid(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.element >= 0 && layout.date.element <= 4);
}

void test_polarity_valid(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.polarity == 0 || layout.date.polarity == 1);
}

void test_cycle_year_valid(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.date.cycle_year >= 1 && layout.date.cycle_year <= 60);
}

/* --- Zodiac interpretation --- */

void test_animal_name_not_null(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.animal_name);
    TEST_ASSERT_TRUE(strlen(layout.animal_name) > 0);
}

void test_element_name_not_null(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.element_name);
    TEST_ASSERT_TRUE(strlen(layout.element_name) > 0);
}

void test_polarity_name_not_null(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.polarity_name);
    TEST_ASSERT_TRUE(strlen(layout.polarity_name) > 0);
}

void test_sexagenary_not_null(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.sexagenary);
    TEST_ASSERT_TRUE(strlen(layout.sexagenary) > 0);
}

/* --- Festival detection --- */

void test_festival_field_valid(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.has_festival == 0 || layout.has_festival == 1);
}

void test_festival_name_empty_when_none(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    if (layout.has_festival == 0) {
        TEST_ASSERT_EQUAL_STRING("", layout.festival_name);
        TEST_ASSERT_EQUAL_STRING("", layout.festival_korean);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_date_slot(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.date_slot.y);
}

void test_animal_slot(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.animal_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.34f, layout.animal_slot.y);
}

void test_cycle_slot(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.cycle_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.cycle_slot.y);
}

void test_festival_slot(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.festival_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.66f, layout.festival_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.animal_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.cycle_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.festival_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_animal_accessor(void)
{
    daily_korean_layout_t layout = daily_korean_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(daily_korean_animal(&layout));
    TEST_ASSERT_TRUE(strlen(daily_korean_animal(&layout)) > 0);
}

void test_animal_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_korean_animal(NULL));
}

void test_festival_accessor_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_korean_has_festival(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_korean_layout_t a = daily_korean_compute(TEST_JD);
    daily_korean_layout_t b = daily_korean_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.date.dangun_year, b.date.dangun_year);
    TEST_ASSERT_EQUAL_INT(a.date.animal, b.date.animal);
    TEST_ASSERT_EQUAL_INT(a.date.element, b.date.element);
    TEST_ASSERT_EQUAL_STRING(a.animal_name, b.animal_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_korean_layout_t a = daily_korean_compute(2460676.5);
    daily_korean_layout_t b = daily_korean_compute(2460706.5);
    TEST_ASSERT_TRUE(a.date.lunar_day != b.date.lunar_day ||
                     a.date.lunar_month != b.date.lunar_month);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_dangun_year);
    RUN_TEST(test_gregorian_year);
    RUN_TEST(test_animal_valid);
    RUN_TEST(test_element_valid);
    RUN_TEST(test_polarity_valid);
    RUN_TEST(test_cycle_year_valid);
    /* Zodiac interpretation */
    RUN_TEST(test_animal_name_not_null);
    RUN_TEST(test_element_name_not_null);
    RUN_TEST(test_polarity_name_not_null);
    RUN_TEST(test_sexagenary_not_null);
    /* Festival detection */
    RUN_TEST(test_festival_field_valid);
    RUN_TEST(test_festival_name_empty_when_none);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_animal_slot);
    RUN_TEST(test_cycle_slot);
    RUN_TEST(test_festival_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_animal_accessor);
    RUN_TEST(test_animal_accessor_null);
    RUN_TEST(test_festival_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
