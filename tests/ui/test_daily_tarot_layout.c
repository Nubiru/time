#include "../unity/unity.h"
#include "../../src/ui/daily_tarot_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Card number --- */

void test_card_number_range(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.card_number >= 0 && layout.card_number <= 21);
}

/* --- Card name --- */

void test_card_name_not_null(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.card_name);
    TEST_ASSERT_TRUE(strlen(layout.card_name) > 0);
}

/* --- Hebrew letter --- */

void test_hebrew_letter_not_null(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.hebrew_letter);
    TEST_ASSERT_TRUE(strlen(layout.hebrew_letter) > 0);
}

/* --- Tree path --- */

void test_tree_path_range(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.tree_path >= 0 && layout.tree_path <= 21);
}

/* --- Interpretation --- */

void test_glance_not_empty(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.glance[0] != '\0');
}

void test_detail_not_empty(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.detail[0] != '\0');
}

/* --- Different JDs give different cards --- */

void test_different_dates_different_cards(void)
{
    daily_tarot_layout_t a = daily_tarot_compute(2460676.5);
    daily_tarot_layout_t b = daily_tarot_compute(2460677.5);
    /* Adjacent days should hash to different cards (extremely likely) */
    TEST_ASSERT_TRUE(a.card_number != b.card_number);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_card_slot(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.card_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.19f, layout.card_slot.y);
}

void test_arcana_slot(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.arcana_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.arcana_slot.y);
}

void test_hebrew_slot(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.hebrew_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.47f, layout.hebrew_slot.y);
}

void test_path_slot(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.path_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.61f, layout.path_slot.y);
}

void test_meaning_slot(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meaning_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, layout.meaning_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.card_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.arcana_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.hebrew_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.path_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meaning_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_tarot_layout_t layout = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_tarot_layout_t a = daily_tarot_compute(TEST_JD);
    daily_tarot_layout_t b = daily_tarot_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.card_number, b.card_number);
    TEST_ASSERT_EQUAL_STRING(a.card_name, b.card_name);
    TEST_ASSERT_EQUAL_STRING(a.hebrew_letter, b.hebrew_letter);
    TEST_ASSERT_EQUAL_INT(a.tree_path, b.tree_path);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_deterministic_hash(void)
{
    /* Same JD always produces same card number */
    daily_tarot_layout_t a = daily_tarot_compute(2460700.5);
    daily_tarot_layout_t b = daily_tarot_compute(2460700.5);
    TEST_ASSERT_EQUAL_INT(a.card_number, b.card_number);
}

int main(void)
{
    UNITY_BEGIN();
    /* Card number */
    RUN_TEST(test_card_number_range);
    /* Card name */
    RUN_TEST(test_card_name_not_null);
    /* Hebrew letter */
    RUN_TEST(test_hebrew_letter_not_null);
    /* Tree path */
    RUN_TEST(test_tree_path_range);
    /* Interpretation */
    RUN_TEST(test_glance_not_empty);
    RUN_TEST(test_detail_not_empty);
    /* Different dates */
    RUN_TEST(test_different_dates_different_cards);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_card_slot);
    RUN_TEST(test_arcana_slot);
    RUN_TEST(test_hebrew_slot);
    RUN_TEST(test_path_slot);
    RUN_TEST(test_meaning_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_deterministic_hash);
    return UNITY_END();
}
