#include "../unity/unity.h"
#include "../../src/ui/daily_kabbalah_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian */
#define TEST_JD 2460676.5

/* --- Sefirah cycling --- */

void test_sefirah_index_valid(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.sefirah_index >= 0 &&
                     layout.sefirah_index <= 9);
}

void test_sefirah_name_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.sefirah_name);
    TEST_ASSERT_TRUE(strlen(layout.sefirah_name) > 0);
}

void test_sefirah_meaning_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.sefirah_meaning);
    TEST_ASSERT_TRUE(strlen(layout.sefirah_meaning) > 0);
}

void test_pillar_name_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.pillar_name);
    TEST_ASSERT_TRUE(strlen(layout.pillar_name) > 0);
}

void test_triad_name_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.triad_name);
    TEST_ASSERT_TRUE(strlen(layout.triad_name) > 0);
}

/* --- Sefirah interpretation --- */

void test_meditation_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.meditation);
    TEST_ASSERT_TRUE(strlen(layout.meditation) > 0);
}

void test_question_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.question);
    TEST_ASSERT_TRUE(strlen(layout.question) > 0);
}

void test_brief_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.brief);
    TEST_ASSERT_TRUE(strlen(layout.brief) > 0);
}

/* --- Path cycling --- */

void test_path_index_valid(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.path_index >= 0 &&
                     layout.path_index <= 21);
}

void test_hebrew_letter_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.hebrew_letter);
    TEST_ASSERT_TRUE(strlen(layout.hebrew_letter) > 0);
}

void test_tarot_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.tarot);
    TEST_ASSERT_TRUE(strlen(layout.tarot) > 0);
}

void test_path_journey_not_null(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.path_journey);
    TEST_ASSERT_TRUE(strlen(layout.path_journey) > 0);
}

/* --- Different days cycle differently --- */

void test_sefirah_cycles_10(void)
{
    /* 10 consecutive days should touch all 10 sefirot */
    int seen[10] = {0};
    for (int i = 0; i < 10; i++) {
        daily_kabbalah_layout_t l = daily_kabbalah_compute(TEST_JD + i);
        TEST_ASSERT_TRUE(l.sefirah_index >= 0 && l.sefirah_index <= 9);
        seen[l.sefirah_index] = 1;
    }
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

void test_path_cycles_22(void)
{
    /* 22 consecutive days should touch all 22 paths */
    int seen[22] = {0};
    for (int i = 0; i < 22; i++) {
        daily_kabbalah_layout_t l = daily_kabbalah_compute(TEST_JD + i);
        TEST_ASSERT_TRUE(l.path_index >= 0 && l.path_index <= 21);
        seen[l.path_index] = 1;
    }
    for (int i = 0; i < 22; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.03f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_sefirah_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.sefirah_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.12f, layout.sefirah_slot.y);
}

void test_pillar_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.pillar_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.22f, layout.pillar_slot.y);
}

void test_meditate_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.meditate_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.33f, layout.meditate_slot.y);
}

void test_question_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.question_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.question_slot.y);
}

void test_path_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.path_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.59f, layout.path_slot.y);
}

void test_journey_slot(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.journey_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.72f, layout.journey_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.sefirah_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.pillar_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.meditate_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.question_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.path_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.journey_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_meditation_accessor(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(daily_kabbalah_meditation(&layout));
    TEST_ASSERT_TRUE(strlen(daily_kabbalah_meditation(&layout)) > 0);
}

void test_meditation_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_kabbalah_meditation(NULL));
}

void test_letter_accessor(void)
{
    daily_kabbalah_layout_t layout = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(daily_kabbalah_letter(&layout));
    TEST_ASSERT_TRUE(strlen(daily_kabbalah_letter(&layout)) > 0);
}

void test_letter_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_kabbalah_letter(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_kabbalah_layout_t a = daily_kabbalah_compute(TEST_JD);
    daily_kabbalah_layout_t b = daily_kabbalah_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.sefirah_index, b.sefirah_index);
    TEST_ASSERT_EQUAL_INT(a.path_index, b.path_index);
    TEST_ASSERT_EQUAL_STRING(a.sefirah_name, b.sefirah_name);
    TEST_ASSERT_EQUAL_STRING(a.hebrew_letter, b.hebrew_letter);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_kabbalah_layout_t a = daily_kabbalah_compute(2460676.5);
    daily_kabbalah_layout_t b = daily_kabbalah_compute(2460677.5);
    /* Next day should have different sefirah (mod 10 shifts by 1) */
    TEST_ASSERT_TRUE(a.sefirah_index != b.sefirah_index ||
                     a.path_index != b.path_index);
}

int main(void)
{
    UNITY_BEGIN();
    /* Sefirah cycling */
    RUN_TEST(test_sefirah_index_valid);
    RUN_TEST(test_sefirah_name_not_null);
    RUN_TEST(test_sefirah_meaning_not_null);
    RUN_TEST(test_pillar_name_not_null);
    RUN_TEST(test_triad_name_not_null);
    /* Sefirah interpretation */
    RUN_TEST(test_meditation_not_null);
    RUN_TEST(test_question_not_null);
    RUN_TEST(test_brief_not_null);
    /* Path cycling */
    RUN_TEST(test_path_index_valid);
    RUN_TEST(test_hebrew_letter_not_null);
    RUN_TEST(test_tarot_not_null);
    RUN_TEST(test_path_journey_not_null);
    /* Cycling coverage */
    RUN_TEST(test_sefirah_cycles_10);
    RUN_TEST(test_path_cycles_22);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_sefirah_slot);
    RUN_TEST(test_pillar_slot);
    RUN_TEST(test_meditate_slot);
    RUN_TEST(test_question_slot);
    RUN_TEST(test_path_slot);
    RUN_TEST(test_journey_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_meditation_accessor);
    RUN_TEST(test_meditation_accessor_null);
    RUN_TEST(test_letter_accessor);
    RUN_TEST(test_letter_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
