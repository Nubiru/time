#include "../../unity/unity.h"
#include "../../../src/systems/japanese/japanese_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * ji_era_data — 5 eras
 * ============================================================ */

void test_era_meiji(void) {
    ji_era_t e = ji_era_data(0);
    TEST_ASSERT_EQUAL_INT(0, e.era);
    TEST_ASSERT_EQUAL_STRING("Meiji", e.name);
    TEST_ASSERT_EQUAL_STRING("Enlightened Rule", e.meaning);
}

void test_era_reiwa(void) {
    ji_era_t e = ji_era_data(4);
    TEST_ASSERT_EQUAL_INT(4, e.era);
    TEST_ASSERT_EQUAL_STRING("Reiwa", e.name);
    TEST_ASSERT_EQUAL_STRING("Beautiful Harmony", e.meaning);
}

void test_era_all_5_valid(void) {
    for (int i = 0; i < 5; i++) {
        ji_era_t e = ji_era_data(i);
        TEST_ASSERT_EQUAL_INT(i, e.era);
        TEST_ASSERT_NOT_NULL(e.name);
        TEST_ASSERT_NOT_NULL(e.kanji);
        TEST_ASSERT_NOT_NULL(e.meaning);
        TEST_ASSERT_NOT_NULL(e.spirit);
        TEST_ASSERT_NOT_NULL(e.brief);
        TEST_ASSERT_TRUE(strlen(e.name) > 0);
    }
}

void test_era_invalid(void) {
    ji_era_t e = ji_era_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.era);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
}

void test_era_invalid_5(void) {
    ji_era_t e = ji_era_data(5);
    TEST_ASSERT_EQUAL_INT(-1, e.era);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
}

/* ============================================================
 * ji_rokuyo_data — 6 rokuyo
 * ============================================================ */

void test_rokuyo_taian(void) {
    ji_rokuyo_t r = ji_rokuyo_data(4);
    TEST_ASSERT_EQUAL_INT(4, r.rokuyo);
    TEST_ASSERT_EQUAL_STRING("Taian", r.name);
    TEST_ASSERT_EQUAL_STRING("Great Peace", r.meaning);
    TEST_ASSERT_NOT_NULL(strstr(r.guidance, "auspicious"));
}

void test_rokuyo_butsumetsu(void) {
    ji_rokuyo_t r = ji_rokuyo_data(3);
    TEST_ASSERT_EQUAL_INT(3, r.rokuyo);
    TEST_ASSERT_EQUAL_STRING("Butsumetsu", r.name);
    TEST_ASSERT_NOT_NULL(strstr(r.meaning, "Buddha"));
}

void test_rokuyo_all_6_valid(void) {
    for (int i = 0; i < 6; i++) {
        ji_rokuyo_t r = ji_rokuyo_data(i);
        TEST_ASSERT_EQUAL_INT(i, r.rokuyo);
        TEST_ASSERT_NOT_NULL(r.name);
        TEST_ASSERT_NOT_NULL(r.meaning);
        TEST_ASSERT_NOT_NULL(r.guidance);
        TEST_ASSERT_NOT_NULL(r.brief);
    }
}

void test_rokuyo_invalid(void) {
    ji_rokuyo_t r = ji_rokuyo_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, r.rokuyo);
    TEST_ASSERT_EQUAL_STRING("?", r.name);
}

void test_rokuyo_invalid_6(void) {
    ji_rokuyo_t r = ji_rokuyo_data(6);
    TEST_ASSERT_EQUAL_INT(-1, r.rokuyo);
    TEST_ASSERT_EQUAL_STRING("?", r.name);
}

/* ============================================================
 * ji_sekki_data — 24 solar terms
 * ============================================================ */

void test_sekki_shunbun(void) {
    ji_sekki_t s = ji_sekki_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.sekki);
    TEST_ASSERT_EQUAL_STRING("Shunbun", s.name);
    TEST_ASSERT_EQUAL_STRING("Vernal Equinox", s.meaning);
}

void test_sekki_geshi(void) {
    ji_sekki_t s = ji_sekki_data(6);
    TEST_ASSERT_EQUAL_INT(6, s.sekki);
    TEST_ASSERT_EQUAL_STRING("Geshi", s.name);
    TEST_ASSERT_EQUAL_STRING("Summer Solstice", s.meaning);
}

void test_sekki_toji(void) {
    ji_sekki_t s = ji_sekki_data(18);
    TEST_ASSERT_EQUAL_INT(18, s.sekki);
    TEST_ASSERT_EQUAL_STRING("Toji", s.name);
    TEST_ASSERT_EQUAL_STRING("Winter Solstice", s.meaning);
}

void test_sekki_all_24_valid(void) {
    for (int i = 0; i < 24; i++) {
        ji_sekki_t s = ji_sekki_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.sekki);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.kanji);
        TEST_ASSERT_NOT_NULL(s.meaning);
        TEST_ASSERT_NOT_NULL(s.nature);
        TEST_ASSERT_NOT_NULL(s.brief);
        TEST_ASSERT_TRUE(strlen(s.name) > 0);
    }
}

void test_sekki_invalid(void) {
    ji_sekki_t s = ji_sekki_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.sekki);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

void test_sekki_invalid_24(void) {
    ji_sekki_t s = ji_sekki_data(24);
    TEST_ASSERT_EQUAL_INT(-1, s.sekki);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* ============================================================
 * ji_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_reiwa(void) {
    japanese_interp_t r = ji_interpret(4, 8, 4, 0);
    TEST_ASSERT_EQUAL_STRING("Rei", r.glyph);
}

void test_interpret_glyph_meiji(void) {
    japanese_interp_t r = ji_interpret(0, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Mei", r.glyph);
}

void test_interpret_glance_with_sekki(void) {
    japanese_interp_t r = ji_interpret(4, 8, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Reiwa"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Shunbun"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Vernal Equinox"));
}

void test_interpret_detail_contains_era(void) {
    japanese_interp_t r = ji_interpret(4, 8, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Reiwa"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Beautiful Harmony"));
}

void test_interpret_detail_contains_rokuyo(void) {
    japanese_interp_t r = ji_interpret(4, 8, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Taian"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Great Peace"));
}

void test_interpret_detail_contains_sekki(void) {
    japanese_interp_t r = ji_interpret(4, 8, 4, 6);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Geshi"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Summer Solstice"));
}

void test_interpret_invalid_era(void) {
    japanese_interp_t r = ji_interpret(99, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_era_count(void) {
    TEST_ASSERT_EQUAL_INT(5, ji_era_count());
}

void test_rokuyo_count(void) {
    TEST_ASSERT_EQUAL_INT(6, ji_rokuyo_count());
}

void test_sekki_count(void) {
    TEST_ASSERT_EQUAL_INT(24, ji_sekki_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* ji_era_data */
    RUN_TEST(test_era_meiji);
    RUN_TEST(test_era_reiwa);
    RUN_TEST(test_era_all_5_valid);
    RUN_TEST(test_era_invalid);
    RUN_TEST(test_era_invalid_5);

    /* ji_rokuyo_data */
    RUN_TEST(test_rokuyo_taian);
    RUN_TEST(test_rokuyo_butsumetsu);
    RUN_TEST(test_rokuyo_all_6_valid);
    RUN_TEST(test_rokuyo_invalid);
    RUN_TEST(test_rokuyo_invalid_6);

    /* ji_sekki_data */
    RUN_TEST(test_sekki_shunbun);
    RUN_TEST(test_sekki_geshi);
    RUN_TEST(test_sekki_toji);
    RUN_TEST(test_sekki_all_24_valid);
    RUN_TEST(test_sekki_invalid);
    RUN_TEST(test_sekki_invalid_24);

    /* ji_interpret */
    RUN_TEST(test_interpret_glyph_reiwa);
    RUN_TEST(test_interpret_glyph_meiji);
    RUN_TEST(test_interpret_glance_with_sekki);
    RUN_TEST(test_interpret_detail_contains_era);
    RUN_TEST(test_interpret_detail_contains_rokuyo);
    RUN_TEST(test_interpret_detail_contains_sekki);
    RUN_TEST(test_interpret_invalid_era);

    /* counts */
    RUN_TEST(test_era_count);
    RUN_TEST(test_rokuyo_count);
    RUN_TEST(test_sekki_count);

    return UNITY_END();
}
