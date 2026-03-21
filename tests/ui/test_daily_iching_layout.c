#include "../unity/unity.h"
#include "../../src/ui/daily_iching_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2451545.0 = 2000-01-01T12:00 (J2000.0 epoch)
 * iching_from_jd: raw = 2451544, idx = 24, king_wen = 25
 * Hexagram 25: Wu Wang (Innocence)
 * Lower: Zhen (Thunder, trigram 3), Upper: Qian (Heaven, trigram 0)
 * Lines: [1,0,0,1,1,1] (bottom to top) */
#define TEST_JD 2451545.0

/* --- Hexagram identity --- */

void test_king_wen_number(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(25, layout.king_wen);
}

void test_hexagram_name(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.name);
    /* ii_hexagram_data(25).name = "Innocence" (paraphrase of Wu Wang) */
    TEST_ASSERT_TRUE(strlen(layout.name) > 0);
}

void test_judgment_not_null(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.judgment);
    TEST_ASSERT_TRUE(strlen(layout.judgment) > 0);
}

void test_image_not_null(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.image);
    TEST_ASSERT_TRUE(strlen(layout.image) > 0);
}

void test_keywords_not_null(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.keywords);
    TEST_ASSERT_TRUE(strlen(layout.keywords) > 0);
}

/* --- Lines --- */

void test_lines_bottom(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    /* Hexagram 25, lower trigram Zhen (3): lines = [1,0,0] */
    TEST_ASSERT_EQUAL_INT(1, layout.lines[0]);  /* bottom */
    TEST_ASSERT_EQUAL_INT(0, layout.lines[1]);
    TEST_ASSERT_EQUAL_INT(0, layout.lines[2]);
}

void test_lines_top(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    /* Upper trigram Qian (0): lines = [1,1,1] */
    TEST_ASSERT_EQUAL_INT(1, layout.lines[3]);
    TEST_ASSERT_EQUAL_INT(1, layout.lines[4]);
    TEST_ASSERT_EQUAL_INT(1, layout.lines[5]);  /* top */
}

void test_lines_all_valid(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_TRUE(layout.lines[i] == 0 || layout.lines[i] == 1);
    }
}

/* --- Trigrams --- */

void test_lower_trigram(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(3, layout.lower_trigram);  /* Zhen (Thunder) */
}

void test_upper_trigram(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, layout.upper_trigram);  /* Qian (Heaven) */
}

void test_lower_name(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Zhen", layout.lower_name);
}

void test_upper_name(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Qian", layout.upper_name);
}

void test_lower_symbol_not_empty(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.lower_symbol);
    TEST_ASSERT_TRUE(strlen(layout.lower_symbol) > 0);
}

void test_upper_symbol_not_empty(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.upper_symbol);
    TEST_ASSERT_TRUE(strlen(layout.upper_symbol) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.03f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_number_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.number_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.12f, layout.number_slot.y);
}

void test_name_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.name_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.22f, layout.name_slot.y);
}

void test_lines_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.lines_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.30f, layout.lines_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.30f, layout.lines_slot.h);
}

void test_upper_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.upper_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.62f, layout.upper_slot.y);
}

void test_lower_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.lower_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.70f, layout.lower_slot.y);
}

void test_judgment_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.judgment_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.78f, layout.judgment_slot.y);
}

void test_image_slot(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.image_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.88f, layout.image_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.number_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.name_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.lines_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.upper_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.lower_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.judgment_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.image_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Name accessor --- */

void test_name_accessor(void)
{
    daily_iching_layout_t layout = daily_iching_compute(TEST_JD);
    const char *name = daily_iching_hexagram_name(&layout);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_name_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_iching_hexagram_name(NULL));
}

/* --- Different JD gives different hexagram --- */

void test_different_jd(void)
{
    /* JD 2460676.5: king_wen = 5 (Xu / Waiting) */
    daily_iching_layout_t layout = daily_iching_compute(2460676.5);
    TEST_ASSERT_EQUAL_INT(5, layout.king_wen);
    TEST_ASSERT_NOT_NULL(layout.name);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_iching_layout_t a = daily_iching_compute(TEST_JD);
    daily_iching_layout_t b = daily_iching_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.king_wen, b.king_wen);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_INT(a.upper_trigram, b.upper_trigram);
    TEST_ASSERT_EQUAL_INT(a.lower_trigram, b.lower_trigram);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(a.lines[i], b.lines[i]);
    }
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_days_different_hexagram(void)
{
    daily_iching_layout_t a = daily_iching_compute(2451545.0);
    daily_iching_layout_t b = daily_iching_compute(2451546.0);
    /* Consecutive days give consecutive hexagrams */
    TEST_ASSERT_TRUE(a.king_wen != b.king_wen);
}

int main(void)
{
    UNITY_BEGIN();
    /* Hexagram identity */
    RUN_TEST(test_king_wen_number);
    RUN_TEST(test_hexagram_name);
    RUN_TEST(test_judgment_not_null);
    RUN_TEST(test_image_not_null);
    RUN_TEST(test_keywords_not_null);
    /* Lines */
    RUN_TEST(test_lines_bottom);
    RUN_TEST(test_lines_top);
    RUN_TEST(test_lines_all_valid);
    /* Trigrams */
    RUN_TEST(test_lower_trigram);
    RUN_TEST(test_upper_trigram);
    RUN_TEST(test_lower_name);
    RUN_TEST(test_upper_name);
    RUN_TEST(test_lower_symbol_not_empty);
    RUN_TEST(test_upper_symbol_not_empty);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_number_slot);
    RUN_TEST(test_name_slot);
    RUN_TEST(test_lines_slot);
    RUN_TEST(test_upper_slot);
    RUN_TEST(test_lower_slot);
    RUN_TEST(test_judgment_slot);
    RUN_TEST(test_image_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Name accessor */
    RUN_TEST(test_name_accessor);
    RUN_TEST(test_name_accessor_null);
    /* Different JD */
    RUN_TEST(test_different_jd);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_days_different_hexagram);
    return UNITY_END();
}
