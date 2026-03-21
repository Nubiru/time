#include "../unity/unity.h"
#include "../../src/ui/daily_geology_layout.h"
#include "../../src/systems/earth/earth_fraction.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460755.5 = 2025-03-21 */
#define TEST_JD 2460755.5

/* J2000.0 epoch */
#define J2000_JD 2451545.0

/* --- Eon data --- */

void test_eon_name(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Phanerozoic", layout.eon_name);
}

void test_eon_meaning(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.eon_meaning);
    TEST_ASSERT_TRUE(strlen(layout.eon_meaning) > 0);
}

void test_eon_brief(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.eon_brief);
    TEST_ASSERT_TRUE(strlen(layout.eon_brief) > 0);
}

/* --- Earth fraction fact --- */

void test_fact_name_not_null(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.fact_name);
    TEST_ASSERT_TRUE(strlen(layout.fact_name) > 0);
}

void test_fact_description_not_null(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.fact_description);
    TEST_ASSERT_TRUE(strlen(layout.fact_description) > 0);
}

void test_fact_analogy_not_null(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.fact_analogy);
    TEST_ASSERT_TRUE(strlen(layout.fact_analogy) > 0);
}

void test_fact_rotates_by_day(void)
{
    /* Consecutive days should get different (or same, if count wraps) facts.
     * But JDs that differ by fact_count should get the same fact. */
    int count = ef_fact_count();
    daily_geology_layout_t a = daily_geology_compute(TEST_JD);
    daily_geology_layout_t b = daily_geology_compute(TEST_JD + (double)count);
    TEST_ASSERT_EQUAL_STRING(a.fact_name, b.fact_name);
}

void test_fact_different_day_differs(void)
{
    /* Two adjacent days should typically get different facts */
    daily_geology_layout_t a = daily_geology_compute(TEST_JD);
    daily_geology_layout_t b = daily_geology_compute(TEST_JD + 1.0);
    /* They should differ (unless count is 1, which it is not) */
    TEST_ASSERT_TRUE(strcmp(a.fact_name, b.fact_name) != 0);
}

void test_fact_index_valid(void)
{
    /* Verify the fact index stays within bounds for a range of JDs */
    for (int i = 0; i < 20; i++) {
        daily_geology_layout_t layout = daily_geology_compute(TEST_JD + (double)i);
        TEST_ASSERT_NOT_NULL(layout.fact_name);
    }
}

/* --- Human fraction --- */

void test_human_fraction(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    /* Human fraction should be around 0.0066% */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0066, layout.human_fraction_pct);
}

void test_human_fraction_positive(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.human_fraction_pct > 0.0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_eon_slot(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.eon_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.eon_slot.y);
}

void test_meaning_slot(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meaning_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.meaning_slot.y);
}

void test_fact_slot(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.fact_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.fact_slot.y);
}

void test_analogy_slot(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.analogy_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.analogy_slot.y);
}

void test_fraction_slot(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.fraction_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.fraction_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.eon_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meaning_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.fact_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.analogy_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.fraction_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_eon_name_accessor(void)
{
    daily_geology_layout_t layout = daily_geology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Phanerozoic", daily_geology_eon_name(&layout));
}

void test_eon_name_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_geology_eon_name(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_geology_layout_t a = daily_geology_compute(TEST_JD);
    daily_geology_layout_t b = daily_geology_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING(a.eon_name, b.eon_name);
    TEST_ASSERT_EQUAL_STRING(a.fact_name, b.fact_name);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, a.human_fraction_pct, b.human_fraction_pct);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_w, b.card_w);
}

void test_purity_different_dates(void)
{
    daily_geology_layout_t a = daily_geology_compute(2460755.5);
    daily_geology_layout_t b = daily_geology_compute(2460756.5);
    /* Eon stays the same, but fact may differ */
    TEST_ASSERT_EQUAL_STRING(a.eon_name, b.eon_name);
}

int main(void)
{
    UNITY_BEGIN();
    /* Eon data */
    RUN_TEST(test_eon_name);
    RUN_TEST(test_eon_meaning);
    RUN_TEST(test_eon_brief);
    /* Earth fraction fact */
    RUN_TEST(test_fact_name_not_null);
    RUN_TEST(test_fact_description_not_null);
    RUN_TEST(test_fact_analogy_not_null);
    RUN_TEST(test_fact_rotates_by_day);
    RUN_TEST(test_fact_different_day_differs);
    RUN_TEST(test_fact_index_valid);
    /* Human fraction */
    RUN_TEST(test_human_fraction);
    RUN_TEST(test_human_fraction_positive);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_eon_slot);
    RUN_TEST(test_meaning_slot);
    RUN_TEST(test_fact_slot);
    RUN_TEST(test_analogy_slot);
    RUN_TEST(test_fraction_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_eon_name_accessor);
    RUN_TEST(test_eon_name_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
