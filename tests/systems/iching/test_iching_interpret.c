/* test_iching_interpret.c — I Ching hexagram interpretation data + tiered text
 * TDD RED phase: tests written before implementation.
 * 64 hexagrams: name, judgment, image, keywords, element.
 * Composition: glyph (T0), glance (T1), detail (T3). */

#include "../../unity/unity.h"
#include "../../../src/systems/iching/iching_interpret.h"
#include "../../../src/systems/iching/iching.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ii_hexagram_count ===== */

void test_hexagram_count_returns_64(void)
{
    TEST_ASSERT_EQUAL_INT(64, ii_hexagram_count());
}

/* ===== ii_hexagram_data — valid hexagrams ===== */

void test_hexagram_data_1_creative(void)
{
    ii_hexagram_t h = ii_hexagram_data(1);
    TEST_ASSERT_EQUAL_INT(1, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("The Creative", h.name);
    TEST_ASSERT_TRUE(strlen(h.judgment) > 0);
    TEST_ASSERT_TRUE(strlen(h.image) > 0);
    TEST_ASSERT_TRUE(strlen(h.keywords) > 0);
    TEST_ASSERT_EQUAL_STRING("Heaven", h.element);
}

void test_hexagram_data_2_receptive(void)
{
    ii_hexagram_t h = ii_hexagram_data(2);
    TEST_ASSERT_EQUAL_INT(2, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("The Receptive", h.name);
    TEST_ASSERT_TRUE(strlen(h.judgment) > 0);
    TEST_ASSERT_TRUE(strlen(h.image) > 0);
    TEST_ASSERT_TRUE(strlen(h.keywords) > 0);
    TEST_ASSERT_EQUAL_STRING("Earth", h.element);
}

void test_hexagram_data_23_splitting(void)
{
    ii_hexagram_t h = ii_hexagram_data(23);
    TEST_ASSERT_EQUAL_INT(23, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("Splitting Apart", h.name);
    TEST_ASSERT_TRUE(strlen(h.judgment) > 0);
    TEST_ASSERT_TRUE(strlen(h.image) > 0);
    TEST_ASSERT_TRUE(strlen(h.keywords) > 0);
}

void test_hexagram_data_50_cauldron(void)
{
    ii_hexagram_t h = ii_hexagram_data(50);
    TEST_ASSERT_EQUAL_INT(50, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("The Cauldron", h.name);
    TEST_ASSERT_TRUE(strlen(h.judgment) > 0);
    TEST_ASSERT_TRUE(strlen(h.image) > 0);
    TEST_ASSERT_TRUE(strlen(h.keywords) > 0);
}

void test_hexagram_data_64_before_completion(void)
{
    ii_hexagram_t h = ii_hexagram_data(64);
    TEST_ASSERT_EQUAL_INT(64, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("Before Completion", h.name);
    TEST_ASSERT_TRUE(strlen(h.judgment) > 0);
    TEST_ASSERT_TRUE(strlen(h.image) > 0);
    TEST_ASSERT_TRUE(strlen(h.keywords) > 0);
}

/* ===== ii_hexagram_data — invalid inputs ===== */

void test_hexagram_data_invalid_zero(void)
{
    ii_hexagram_t h = ii_hexagram_data(0);
    TEST_ASSERT_EQUAL_INT(-1, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("?", h.name);
    TEST_ASSERT_EQUAL_STRING("?", h.judgment);
    TEST_ASSERT_EQUAL_STRING("?", h.image);
    TEST_ASSERT_EQUAL_STRING("?", h.keywords);
    TEST_ASSERT_EQUAL_STRING("?", h.element);
}

void test_hexagram_data_invalid_65(void)
{
    ii_hexagram_t h = ii_hexagram_data(65);
    TEST_ASSERT_EQUAL_INT(-1, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("?", h.name);
}

void test_hexagram_data_invalid_negative(void)
{
    ii_hexagram_t h = ii_hexagram_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, h.king_wen);
    TEST_ASSERT_EQUAL_STRING("?", h.name);
}

/* ===== ii_interpret — glyph ===== */

void test_interpret_glyph_is_number_string(void)
{
    iching_interp_t r = ii_interpret(23, "Gen", "Kun");
    TEST_ASSERT_EQUAL_STRING("23", r.glyph);
}

void test_interpret_glyph_single_digit(void)
{
    iching_interp_t r = ii_interpret(1, "Qian", "Qian");
    TEST_ASSERT_EQUAL_STRING("1", r.glyph);
}

void test_interpret_glyph_64(void)
{
    iching_interp_t r = ii_interpret(64, "Li", "Kan");
    TEST_ASSERT_EQUAL_STRING("64", r.glyph);
}

/* ===== ii_interpret — glance ===== */

void test_interpret_glance_contains_number(void)
{
    iching_interp_t r = ii_interpret(23, "Gen", "Kun");
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "23"));
}

void test_interpret_glance_contains_name(void)
{
    iching_interp_t r = ii_interpret(23, "Gen", "Kun");
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Splitting Apart"));
}

void test_interpret_glance_contains_upper_trigram(void)
{
    iching_interp_t r = ii_interpret(23, "Gen", "Kun");
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Gen"));
}

void test_interpret_glance_contains_lower_trigram(void)
{
    iching_interp_t r = ii_interpret(23, "Gen", "Kun");
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kun"));
}

void test_interpret_glance_format(void)
{
    iching_interp_t r = ii_interpret(1, "Qian", "Qian");
    /* "Hexagram 1 The Creative — Qian over Qian" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Hexagram 1"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Creative"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Qian over Qian"));
}

/* ===== ii_interpret — detail ===== */

void test_interpret_detail_contains_judgment(void)
{
    ii_hexagram_t data = ii_hexagram_data(1);
    iching_interp_t r = ii_interpret(1, "Qian", "Qian");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, data.judgment));
}

void test_interpret_detail_contains_image(void)
{
    ii_hexagram_t data = ii_hexagram_data(1);
    iching_interp_t r = ii_interpret(1, "Qian", "Qian");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, data.image));
}

void test_interpret_detail_contains_keywords(void)
{
    ii_hexagram_t data = ii_hexagram_data(1);
    iching_interp_t r = ii_interpret(1, "Qian", "Qian");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, data.keywords));
}

void test_interpret_detail_contains_trigrams(void)
{
    iching_interp_t r = ii_interpret(23, "Gen", "Kun");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Gen"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Kun"));
}

/* ===== ii_interpret — invalid ===== */

void test_interpret_invalid_king_wen(void)
{
    iching_interp_t r = ii_interpret(0, "Qian", "Qian");
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_EQUAL_STRING("?", r.glance);
    TEST_ASSERT_EQUAL_STRING("?", r.detail);
}

void test_interpret_invalid_negative(void)
{
    iching_interp_t r = ii_interpret(-5, "Qian", "Qian");
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_interpret_invalid_65(void)
{
    iching_interp_t r = ii_interpret(65, "Qian", "Qian");
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ===== ii_interpret — NULL trigram names ===== */

void test_interpret_null_upper_trigram(void)
{
    iching_interp_t r = ii_interpret(1, NULL, "Qian");
    /* Should not crash; uses "?" for missing trigram */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

void test_interpret_null_lower_trigram(void)
{
    iching_interp_t r = ii_interpret(1, "Qian", NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

void test_interpret_both_null_trigrams(void)
{
    iching_interp_t r = ii_interpret(1, NULL, NULL);
    /* Should not crash */
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
}

/* ===== All 64 hexagrams have non-NULL data (loop test) ===== */

void test_all_64_hexagrams_have_valid_data(void)
{
    for (int i = 1; i <= 64; i++) {
        ii_hexagram_t h = ii_hexagram_data(i);
        TEST_ASSERT_EQUAL_INT_MESSAGE(i, h.king_wen,
            "king_wen mismatch");
        TEST_ASSERT_NOT_NULL_MESSAGE(h.name,
            "name is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(h.name) > 0,
            "name is empty");
        TEST_ASSERT_NOT_NULL_MESSAGE(h.judgment,
            "judgment is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(h.judgment) > 0,
            "judgment is empty");
        TEST_ASSERT_NOT_NULL_MESSAGE(h.image,
            "image is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(h.image) > 0,
            "image is empty");
        TEST_ASSERT_NOT_NULL_MESSAGE(h.keywords,
            "keywords is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(h.keywords) > 0,
            "keywords is empty");
        TEST_ASSERT_NOT_NULL_MESSAGE(h.element,
            "element is NULL");
        TEST_ASSERT_TRUE_MESSAGE(strlen(h.element) > 0,
            "element is empty");
    }
}

/* ===== No duplicate names ===== */

void test_no_duplicate_names(void)
{
    const char *names[64];
    for (int i = 0; i < 64; i++) {
        ii_hexagram_t h = ii_hexagram_data(i + 1);
        names[i] = h.name;
    }
    for (int i = 0; i < 64; i++) {
        for (int j = i + 1; j < 64; j++) {
            TEST_ASSERT_FALSE_MESSAGE(
                strcmp(names[i], names[j]) == 0,
                "Duplicate hexagram name found");
        }
    }
}

/* ===== Spot-check specific hexagrams ===== */

void test_hexagram_data_11_peace(void)
{
    ii_hexagram_t h = ii_hexagram_data(11);
    TEST_ASSERT_EQUAL_STRING("Peace", h.name);
}

void test_hexagram_data_29_abysmal(void)
{
    ii_hexagram_t h = ii_hexagram_data(29);
    TEST_ASSERT_EQUAL_STRING("The Abysmal", h.name);
}

void test_hexagram_data_30_clinging(void)
{
    ii_hexagram_t h = ii_hexagram_data(30);
    TEST_ASSERT_EQUAL_STRING("The Clinging", h.name);
}

void test_hexagram_data_63_after_completion(void)
{
    ii_hexagram_t h = ii_hexagram_data(63);
    TEST_ASSERT_EQUAL_STRING("After Completion", h.name);
}

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_hexagram_count_returns_64);

    /* Data — valid hexagrams */
    RUN_TEST(test_hexagram_data_1_creative);
    RUN_TEST(test_hexagram_data_2_receptive);
    RUN_TEST(test_hexagram_data_23_splitting);
    RUN_TEST(test_hexagram_data_50_cauldron);
    RUN_TEST(test_hexagram_data_64_before_completion);

    /* Data — invalid */
    RUN_TEST(test_hexagram_data_invalid_zero);
    RUN_TEST(test_hexagram_data_invalid_65);
    RUN_TEST(test_hexagram_data_invalid_negative);

    /* Interpret — glyph */
    RUN_TEST(test_interpret_glyph_is_number_string);
    RUN_TEST(test_interpret_glyph_single_digit);
    RUN_TEST(test_interpret_glyph_64);

    /* Interpret — glance */
    RUN_TEST(test_interpret_glance_contains_number);
    RUN_TEST(test_interpret_glance_contains_name);
    RUN_TEST(test_interpret_glance_contains_upper_trigram);
    RUN_TEST(test_interpret_glance_contains_lower_trigram);
    RUN_TEST(test_interpret_glance_format);

    /* Interpret — detail */
    RUN_TEST(test_interpret_detail_contains_judgment);
    RUN_TEST(test_interpret_detail_contains_image);
    RUN_TEST(test_interpret_detail_contains_keywords);
    RUN_TEST(test_interpret_detail_contains_trigrams);

    /* Interpret — invalid */
    RUN_TEST(test_interpret_invalid_king_wen);
    RUN_TEST(test_interpret_invalid_negative);
    RUN_TEST(test_interpret_invalid_65);

    /* Interpret — NULL trigrams */
    RUN_TEST(test_interpret_null_upper_trigram);
    RUN_TEST(test_interpret_null_lower_trigram);
    RUN_TEST(test_interpret_both_null_trigrams);

    /* Comprehensive checks */
    RUN_TEST(test_all_64_hexagrams_have_valid_data);
    RUN_TEST(test_no_duplicate_names);

    /* Spot checks */
    RUN_TEST(test_hexagram_data_11_peace);
    RUN_TEST(test_hexagram_data_29_abysmal);
    RUN_TEST(test_hexagram_data_30_clinging);
    RUN_TEST(test_hexagram_data_63_after_completion);

    return UNITY_END();
}
