#include "../../unity/unity.h"
#include "../../../src/systems/tarot/tarot_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * trt_major_data — 22 Major Arcana archetypes
 * ============================================================ */

void test_major_fool(void) {
    trt_major_t m = trt_major_data(0);
    TEST_ASSERT_EQUAL_INT(0, m.number);
    TEST_ASSERT_EQUAL_STRING("The Fool", m.name);
    TEST_ASSERT_EQUAL_STRING("Potential", m.keyword);
    TEST_ASSERT_EQUAL_STRING("The Divine Child", m.archetype);
}

void test_major_magus(void) {
    trt_major_t m = trt_major_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.number);
    TEST_ASSERT_EQUAL_STRING("The Magus", m.name);
    TEST_ASSERT_EQUAL_STRING("Skill", m.keyword);
}

void test_major_adjustment(void) {
    trt_major_t m = trt_major_data(8);
    TEST_ASSERT_EQUAL_INT(8, m.number);
    TEST_ASSERT_EQUAL_STRING("Adjustment", m.name);
    TEST_ASSERT_EQUAL_STRING("Balance", m.keyword);
}

void test_major_lust(void) {
    trt_major_t m = trt_major_data(11);
    TEST_ASSERT_EQUAL_INT(11, m.number);
    TEST_ASSERT_EQUAL_STRING("Lust", m.name);
    TEST_ASSERT_EQUAL_STRING("Strength", m.keyword);
}

void test_major_death(void) {
    trt_major_t m = trt_major_data(13);
    TEST_ASSERT_EQUAL_INT(13, m.number);
    TEST_ASSERT_EQUAL_STRING("Death", m.name);
    TEST_ASSERT_EQUAL_STRING("Transformation", m.keyword);
}

void test_major_art(void) {
    trt_major_t m = trt_major_data(14);
    TEST_ASSERT_EQUAL_INT(14, m.number);
    TEST_ASSERT_EQUAL_STRING("Art", m.name);
    TEST_ASSERT_EQUAL_STRING("Integration", m.keyword);
}

void test_major_universe(void) {
    trt_major_t m = trt_major_data(21);
    TEST_ASSERT_EQUAL_INT(21, m.number);
    TEST_ASSERT_EQUAL_STRING("The Universe", m.name);
    TEST_ASSERT_EQUAL_STRING("Completion", m.keyword);
}

void test_major_all_22_valid(void) {
    for (int i = 0; i < 22; i++) {
        trt_major_t m = trt_major_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.number);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.archetype);
        TEST_ASSERT_NOT_NULL(m.keyword);
        TEST_ASSERT_NOT_NULL(m.light);
        TEST_ASSERT_NOT_NULL(m.shadow);
        TEST_ASSERT_NOT_NULL(m.brief);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
        TEST_ASSERT_TRUE(strlen(m.archetype) > 0);
        TEST_ASSERT_TRUE(strlen(m.keyword) > 0);
        TEST_ASSERT_TRUE(strlen(m.light) > 0);
        TEST_ASSERT_TRUE(strlen(m.shadow) > 0);
        TEST_ASSERT_TRUE(strlen(m.brief) > 0);
    }
}

void test_major_invalid_neg1(void) {
    trt_major_t m = trt_major_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.number);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
    TEST_ASSERT_EQUAL_STRING("?", m.archetype);
    TEST_ASSERT_EQUAL_STRING("?", m.keyword);
    TEST_ASSERT_EQUAL_STRING("?", m.light);
    TEST_ASSERT_EQUAL_STRING("?", m.shadow);
    TEST_ASSERT_EQUAL_STRING("?", m.brief);
}

void test_major_invalid_22(void) {
    trt_major_t m = trt_major_data(22);
    TEST_ASSERT_EQUAL_INT(-1, m.number);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ============================================================
 * trt_suit_data — 4 elemental suits
 * ============================================================ */

void test_suit_wands(void) {
    trt_suit_t s = trt_suit_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.suit);
    TEST_ASSERT_EQUAL_STRING("Wands", s.name);
    TEST_ASSERT_EQUAL_STRING("Fire", s.element);
}

void test_suit_cups(void) {
    trt_suit_t s = trt_suit_data(1);
    TEST_ASSERT_EQUAL_INT(1, s.suit);
    TEST_ASSERT_EQUAL_STRING("Cups", s.name);
    TEST_ASSERT_EQUAL_STRING("Water", s.element);
}

void test_suit_swords(void) {
    trt_suit_t s = trt_suit_data(2);
    TEST_ASSERT_EQUAL_INT(2, s.suit);
    TEST_ASSERT_EQUAL_STRING("Swords", s.name);
    TEST_ASSERT_EQUAL_STRING("Air", s.element);
}

void test_suit_disks(void) {
    trt_suit_t s = trt_suit_data(3);
    TEST_ASSERT_EQUAL_INT(3, s.suit);
    TEST_ASSERT_EQUAL_STRING("Disks", s.name);
    TEST_ASSERT_EQUAL_STRING("Earth", s.element);
}

void test_suit_all_4_valid(void) {
    for (int i = 0; i < 4; i++) {
        trt_suit_t s = trt_suit_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.suit);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.element);
        TEST_ASSERT_NOT_NULL(s.domain);
        TEST_ASSERT_NOT_NULL(s.quality);
        TEST_ASSERT_NOT_NULL(s.brief);
        TEST_ASSERT_TRUE(strlen(s.name) > 0);
        TEST_ASSERT_TRUE(strlen(s.element) > 0);
        TEST_ASSERT_TRUE(strlen(s.domain) > 0);
        TEST_ASSERT_TRUE(strlen(s.quality) > 0);
        TEST_ASSERT_TRUE(strlen(s.brief) > 0);
    }
}

void test_suit_invalid_neg1(void) {
    trt_suit_t s = trt_suit_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.suit);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
    TEST_ASSERT_EQUAL_STRING("?", s.element);
    TEST_ASSERT_EQUAL_STRING("?", s.domain);
    TEST_ASSERT_EQUAL_STRING("?", s.quality);
    TEST_ASSERT_EQUAL_STRING("?", s.brief);
}

void test_suit_invalid_4(void) {
    trt_suit_t s = trt_suit_data(4);
    TEST_ASSERT_EQUAL_INT(-1, s.suit);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* ============================================================
 * trt_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_fool(void) {
    tarot_interp_t r = trt_interpret(0, -1, -1);
    TEST_ASSERT_EQUAL_STRING("0", r.glyph);
}

void test_interpret_glyph_magus(void) {
    tarot_interp_t r = trt_interpret(1, -1, -1);
    TEST_ASSERT_EQUAL_STRING("I", r.glyph);
}

void test_interpret_glyph_universe(void) {
    tarot_interp_t r = trt_interpret(21, -1, -1);
    TEST_ASSERT_EQUAL_STRING("XXI", r.glyph);
}

void test_interpret_glyph_fortune(void) {
    tarot_interp_t r = trt_interpret(10, -1, -1);
    TEST_ASSERT_EQUAL_STRING("X", r.glyph);
}

void test_interpret_glance_major_only(void) {
    tarot_interp_t r = trt_interpret(0, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Fool"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Potential"));
}

void test_interpret_glance_with_decan(void) {
    tarot_interp_t r = trt_interpret(4, 0, 3);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Emperor"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "3 of Wands"));
}

void test_interpret_glance_decan_element(void) {
    tarot_interp_t r = trt_interpret(4, 1, 7);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "7 of Cups"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Water"));
}

void test_interpret_detail_contains_name(void) {
    tarot_interp_t r = trt_interpret(13, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Death"));
}

void test_interpret_detail_contains_keyword(void) {
    tarot_interp_t r = trt_interpret(13, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Transformation"));
}

void test_interpret_detail_contains_archetype(void) {
    tarot_interp_t r = trt_interpret(13, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "The Transformer"));
}

void test_interpret_detail_contains_light(void) {
    tarot_interp_t r = trt_interpret(13, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Necessary ending"));
}

void test_interpret_detail_contains_shadow(void) {
    tarot_interp_t r = trt_interpret(13, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Resistance to change"));
}

void test_interpret_detail_with_decan(void) {
    tarot_interp_t r = trt_interpret(4, 2, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "5 of Swords"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Air"));
}

void test_interpret_detail_decan_domain(void) {
    tarot_interp_t r = trt_interpret(4, 3, 8);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Disks"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "material"));
}

void test_interpret_no_decan_no_suit_info(void) {
    tarot_interp_t r = trt_interpret(9, -1, -1);
    TEST_ASSERT_NULL(strstr(r.detail, "Decan:"));
}

void test_interpret_invalid_major(void) {
    tarot_interp_t r = trt_interpret(99, -1, -1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "?"));
}

void test_interpret_thoth_names_adjustment(void) {
    tarot_interp_t r = trt_interpret(8, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Adjustment"));
}

void test_interpret_thoth_names_lust(void) {
    tarot_interp_t r = trt_interpret(11, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Lust"));
}

void test_interpret_thoth_names_art(void) {
    tarot_interp_t r = trt_interpret(14, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Art"));
}

void test_interpret_thoth_names_aeon(void) {
    tarot_interp_t r = trt_interpret(20, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Aeon"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_major_count(void) {
    TEST_ASSERT_EQUAL_INT(22, trt_major_count());
}

void test_suit_count(void) {
    TEST_ASSERT_EQUAL_INT(4, trt_suit_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* trt_major_data */
    RUN_TEST(test_major_fool);
    RUN_TEST(test_major_magus);
    RUN_TEST(test_major_adjustment);
    RUN_TEST(test_major_lust);
    RUN_TEST(test_major_death);
    RUN_TEST(test_major_art);
    RUN_TEST(test_major_universe);
    RUN_TEST(test_major_all_22_valid);
    RUN_TEST(test_major_invalid_neg1);
    RUN_TEST(test_major_invalid_22);

    /* trt_suit_data */
    RUN_TEST(test_suit_wands);
    RUN_TEST(test_suit_cups);
    RUN_TEST(test_suit_swords);
    RUN_TEST(test_suit_disks);
    RUN_TEST(test_suit_all_4_valid);
    RUN_TEST(test_suit_invalid_neg1);
    RUN_TEST(test_suit_invalid_4);

    /* trt_interpret */
    RUN_TEST(test_interpret_glyph_fool);
    RUN_TEST(test_interpret_glyph_magus);
    RUN_TEST(test_interpret_glyph_universe);
    RUN_TEST(test_interpret_glyph_fortune);
    RUN_TEST(test_interpret_glance_major_only);
    RUN_TEST(test_interpret_glance_with_decan);
    RUN_TEST(test_interpret_glance_decan_element);
    RUN_TEST(test_interpret_detail_contains_name);
    RUN_TEST(test_interpret_detail_contains_keyword);
    RUN_TEST(test_interpret_detail_contains_archetype);
    RUN_TEST(test_interpret_detail_contains_light);
    RUN_TEST(test_interpret_detail_contains_shadow);
    RUN_TEST(test_interpret_detail_with_decan);
    RUN_TEST(test_interpret_detail_decan_domain);
    RUN_TEST(test_interpret_no_decan_no_suit_info);
    RUN_TEST(test_interpret_invalid_major);
    RUN_TEST(test_interpret_thoth_names_adjustment);
    RUN_TEST(test_interpret_thoth_names_lust);
    RUN_TEST(test_interpret_thoth_names_art);
    RUN_TEST(test_interpret_thoth_names_aeon);

    /* counts */
    RUN_TEST(test_major_count);
    RUN_TEST(test_suit_count);

    return UNITY_END();
}
