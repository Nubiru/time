#include "../unity/unity.h"
#include "../../src/ui/daily_chinese_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 */
#define TEST_JD 2460676.5

/* --- Year data populated --- */

void test_stem_in_range(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.stem >= 0 && layout.stem <= 9);
}

void test_branch_in_range(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.branch >= 0 && layout.branch <= 11);
}

void test_animal_in_range(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.animal >= 0 && layout.animal <= 11);
}

void test_element_in_range(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.element >= 0 && layout.element <= 4);
}

void test_polarity_in_range(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.polarity == 0 || layout.polarity == 1);
}

void test_cycle_year_in_range(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.cycle_year >= 1 && layout.cycle_year <= 60);
}

/* --- Name strings populated --- */

void test_animal_name_not_null(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.animal_name);
    TEST_ASSERT_TRUE(strlen(layout.animal_name) > 0);
}

void test_element_name_not_null(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.element_name);
    TEST_ASSERT_TRUE(strlen(layout.element_name) > 0);
}

void test_polarity_name_not_null(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.polarity_name);
    TEST_ASSERT_TRUE(strlen(layout.polarity_name) > 0);
}

void test_stem_name_not_null(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.stem_name);
    TEST_ASSERT_TRUE(strlen(layout.stem_name) > 0);
}

void test_branch_name_not_null(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.branch_name);
    TEST_ASSERT_TRUE(strlen(layout.branch_name) > 0);
}

void test_animal_symbol_not_null(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.animal_symbol);
    TEST_ASSERT_TRUE(strlen(layout.animal_symbol) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_animal_slot(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.animal_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.animal_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.animal_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.animal_slot.h);
}

void test_element_slot(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.element_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.element_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.element_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.element_slot.h);
}

void test_cycle_slot(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.cycle_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.cycle_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.cycle_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.cycle_slot.h);
}

void test_stem_slot(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.stem_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.stem_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.stem_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.stem_slot.h);
}

void test_branch_slot(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.branch_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.branch_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.branch_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.branch_slot.h);
}

void test_all_slots_same_width(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.animal_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.element_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.cycle_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.stem_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.branch_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_animal_name_accessor(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    const char *name = daily_chinese_animal_name(&layout);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_animal_name_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_chinese_animal_name(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_chinese_layout_t a = daily_chinese_compute(TEST_JD);
    daily_chinese_layout_t b = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.stem, b.stem);
    TEST_ASSERT_EQUAL_INT(a.branch, b.branch);
    TEST_ASSERT_EQUAL_INT(a.animal, b.animal);
    TEST_ASSERT_EQUAL_INT(a.element, b.element);
    TEST_ASSERT_EQUAL_INT(a.polarity, b.polarity);
    TEST_ASSERT_EQUAL_INT(a.cycle_year, b.cycle_year);
    TEST_ASSERT_EQUAL_STRING(a.animal_name, b.animal_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    /* 2025-01-01 vs 2026-02-01 — different years in Chinese cycle */
    daily_chinese_layout_t a = daily_chinese_compute(2460676.5);
    daily_chinese_layout_t b = daily_chinese_compute(2461076.5);
    /* A year apart should differ in at least stem or branch */
    TEST_ASSERT_TRUE(a.stem != b.stem || a.branch != b.branch);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_chinese_layout_t layout = daily_chinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Different date test --- */

void test_different_year(void)
{
    /* JD 2451545.0 = 2000-01-01 */
    daily_chinese_layout_t layout = daily_chinese_compute(2451545.0);
    TEST_ASSERT_NOT_NULL(layout.animal_name);
    TEST_ASSERT_TRUE(strlen(layout.animal_name) > 0);
    TEST_ASSERT_TRUE(layout.cycle_year >= 1 && layout.cycle_year <= 60);
}

int main(void)
{
    UNITY_BEGIN();
    /* Year data */
    RUN_TEST(test_stem_in_range);
    RUN_TEST(test_branch_in_range);
    RUN_TEST(test_animal_in_range);
    RUN_TEST(test_element_in_range);
    RUN_TEST(test_polarity_in_range);
    RUN_TEST(test_cycle_year_in_range);
    /* Name strings */
    RUN_TEST(test_animal_name_not_null);
    RUN_TEST(test_element_name_not_null);
    RUN_TEST(test_polarity_name_not_null);
    RUN_TEST(test_stem_name_not_null);
    RUN_TEST(test_branch_name_not_null);
    RUN_TEST(test_animal_symbol_not_null);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_animal_slot);
    RUN_TEST(test_element_slot);
    RUN_TEST(test_cycle_slot);
    RUN_TEST(test_stem_slot);
    RUN_TEST(test_branch_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_animal_name_accessor);
    RUN_TEST(test_animal_name_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Different date */
    RUN_TEST(test_different_year);
    return UNITY_END();
}
