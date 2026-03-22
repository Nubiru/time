#include "../unity/unity.h"
#include "../../src/ui/daily_balinese_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Pawukon data --- */

void test_wuku_in_range(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.pawukon.wuku >= 0 && layout.pawukon.wuku <= 29);
}

void test_day_in_cycle_range(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.pawukon.day_in_cycle >= 0 &&
                     layout.pawukon.day_in_cycle <= 209);
}

void test_panca_wara_range(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.pawukon.panca_wara >= 0 &&
                     layout.pawukon.panca_wara <= 4);
}

void test_sapta_wara_range(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.pawukon.sapta_wara >= 0 &&
                     layout.pawukon.sapta_wara <= 6);
}

void test_wuku_name_not_null(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.wuku_name);
    TEST_ASSERT_TRUE(strlen(layout.wuku_name) > 0);
}

/* --- Urip --- */

void test_urip_positive(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.urip >= 0);
}

/* --- Special days --- */

void test_is_tumpek_bool(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_tumpek == 0 || layout.is_tumpek == 1);
}

void test_is_kajeng_kliwon_bool(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_kajeng_kliwon == 0 ||
                     layout.is_kajeng_kliwon == 1);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_wuku_slot(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.wuku_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.wuku_slot.y);
}

void test_cycle_slot(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.cycle_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.cycle_slot.y);
}

void test_urip_slot(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.urip_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.urip_slot.y);
}

void test_ceremony_slot(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.ceremony_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.ceremony_slot.y);
}

void test_special_slot(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.special_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.special_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.wuku_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.cycle_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.urip_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.ceremony_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.special_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_has_ceremony_accessor(void)
{
    daily_balinese_layout_t layout = daily_balinese_compute(TEST_JD);
    int c = daily_balinese_has_ceremony(&layout);
    TEST_ASSERT_TRUE(c == 0 || c == 1);
}

void test_has_ceremony_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_balinese_has_ceremony(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_balinese_layout_t a = daily_balinese_compute(TEST_JD);
    daily_balinese_layout_t b = daily_balinese_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.pawukon.wuku, b.pawukon.wuku);
    TEST_ASSERT_EQUAL_INT(a.pawukon.day_in_cycle, b.pawukon.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(a.urip, b.urip);
    TEST_ASSERT_EQUAL_STRING(a.wuku_name, b.wuku_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_balinese_layout_t a = daily_balinese_compute(2460676.5);
    daily_balinese_layout_t b = daily_balinese_compute(2460706.5);
    TEST_ASSERT_TRUE(a.pawukon.day_in_cycle != b.pawukon.day_in_cycle);
}

int main(void)
{
    UNITY_BEGIN();
    /* Pawukon data */
    RUN_TEST(test_wuku_in_range);
    RUN_TEST(test_day_in_cycle_range);
    RUN_TEST(test_panca_wara_range);
    RUN_TEST(test_sapta_wara_range);
    RUN_TEST(test_wuku_name_not_null);
    /* Urip */
    RUN_TEST(test_urip_positive);
    /* Special days */
    RUN_TEST(test_is_tumpek_bool);
    RUN_TEST(test_is_kajeng_kliwon_bool);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_wuku_slot);
    RUN_TEST(test_cycle_slot);
    RUN_TEST(test_urip_slot);
    RUN_TEST(test_ceremony_slot);
    RUN_TEST(test_special_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_has_ceremony_accessor);
    RUN_TEST(test_has_ceremony_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
