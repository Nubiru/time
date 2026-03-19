#include "../../unity/unity.h"
#include "../../../src/systems/egyptian/egyptian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * ei_season_data — 4 season interpretations
 * ============================================================ */

void test_season_akhet(void) {
    ei_season_t s = ei_season_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.season);
    TEST_ASSERT_EQUAL_STRING("Akhet", s.name);
    TEST_ASSERT_EQUAL_STRING("Inundation", s.meaning);
    TEST_ASSERT_NOT_NULL(s.theme);
    TEST_ASSERT_NOT_NULL(s.reflection);
}

void test_season_peret(void) {
    ei_season_t s = ei_season_data(1);
    TEST_ASSERT_EQUAL_INT(1, s.season);
    TEST_ASSERT_EQUAL_STRING("Peret", s.name);
    TEST_ASSERT_EQUAL_STRING("Emergence", s.meaning);
}

void test_season_shemu(void) {
    ei_season_t s = ei_season_data(2);
    TEST_ASSERT_EQUAL_INT(2, s.season);
    TEST_ASSERT_EQUAL_STRING("Shemu", s.name);
    TEST_ASSERT_EQUAL_STRING("Harvest", s.meaning);
}

void test_season_epagomenal(void) {
    ei_season_t s = ei_season_data(3);
    TEST_ASSERT_EQUAL_INT(3, s.season);
    TEST_ASSERT_EQUAL_STRING("Epagomenal", s.name);
    TEST_ASSERT_NOT_NULL(strstr(s.theme, "between years"));
}

void test_season_all_4_valid(void) {
    for (int i = 0; i < 4; i++) {
        ei_season_t s = ei_season_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.season);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.meaning);
        TEST_ASSERT_NOT_NULL(s.theme);
        TEST_ASSERT_NOT_NULL(s.reflection);
        TEST_ASSERT_NOT_NULL(s.brief);
        TEST_ASSERT_TRUE(strlen(s.name) > 0);
    }
}

void test_season_invalid_neg1(void) {
    ei_season_t s = ei_season_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.season);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

void test_season_invalid_4(void) {
    ei_season_t s = ei_season_data(4);
    TEST_ASSERT_EQUAL_INT(-1, s.season);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* ============================================================
 * ei_month_data — 12 month interpretations
 * ============================================================ */

void test_month_thoth(void) {
    ei_month_t m = ei_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Thoth", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.deity, "Thoth"));
    TEST_ASSERT_NOT_NULL(strstr(m.domain, "Wisdom"));
}

void test_month_choiak(void) {
    ei_month_t m = ei_month_data(4);
    TEST_ASSERT_EQUAL_INT(4, m.month);
    TEST_ASSERT_EQUAL_STRING("Choiak", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.deity, "Osiris"));
}

void test_month_mesore(void) {
    ei_month_t m = ei_month_data(12);
    TEST_ASSERT_EQUAL_INT(12, m.month);
    TEST_ASSERT_EQUAL_STRING("Mesore", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.deity, "Ra"));
}

void test_month_all_12_valid(void) {
    for (int i = 1; i <= 12; i++) {
        ei_month_t m = ei_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.deity);
        TEST_ASSERT_NOT_NULL(m.domain);
        TEST_ASSERT_NOT_NULL(m.brief);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
        TEST_ASSERT_TRUE(strlen(m.deity) > 0);
        TEST_ASSERT_TRUE(strlen(m.domain) > 0);
        TEST_ASSERT_TRUE(strlen(m.brief) > 0);
    }
}

void test_month_invalid_0(void) {
    ei_month_t m = ei_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_month_invalid_13(void) {
    ei_month_t m = ei_month_data(13);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ============================================================
 * ei_epag_data — 5 epagomenal day deities
 * ============================================================ */

void test_epag_osiris(void) {
    ei_epag_t e = ei_epag_data(1);
    TEST_ASSERT_EQUAL_INT(1, e.day);
    TEST_ASSERT_EQUAL_STRING("Osiris", e.deity);
    TEST_ASSERT_EQUAL_STRING("The Dying King", e.archetype);
}

void test_epag_horus(void) {
    ei_epag_t e = ei_epag_data(2);
    TEST_ASSERT_EQUAL_INT(2, e.day);
    TEST_ASSERT_EQUAL_STRING("Horus", e.deity);
    TEST_ASSERT_EQUAL_STRING("The Avenger", e.archetype);
}

void test_epag_set(void) {
    ei_epag_t e = ei_epag_data(3);
    TEST_ASSERT_EQUAL_INT(3, e.day);
    TEST_ASSERT_EQUAL_STRING("Set", e.deity);
    TEST_ASSERT_EQUAL_STRING("The Disruptor", e.archetype);
}

void test_epag_isis(void) {
    ei_epag_t e = ei_epag_data(4);
    TEST_ASSERT_EQUAL_INT(4, e.day);
    TEST_ASSERT_EQUAL_STRING("Isis", e.deity);
    TEST_ASSERT_EQUAL_STRING("The Enchantress", e.archetype);
}

void test_epag_nephthys(void) {
    ei_epag_t e = ei_epag_data(5);
    TEST_ASSERT_EQUAL_INT(5, e.day);
    TEST_ASSERT_EQUAL_STRING("Nephthys", e.deity);
}

void test_epag_all_5_valid(void) {
    for (int i = 1; i <= 5; i++) {
        ei_epag_t e = ei_epag_data(i);
        TEST_ASSERT_EQUAL_INT(i, e.day);
        TEST_ASSERT_NOT_NULL(e.deity);
        TEST_ASSERT_NOT_NULL(e.archetype);
        TEST_ASSERT_NOT_NULL(e.theme);
        TEST_ASSERT_NOT_NULL(e.brief);
        TEST_ASSERT_TRUE(strlen(e.deity) > 0);
        TEST_ASSERT_TRUE(strlen(e.archetype) > 0);
    }
}

void test_epag_invalid_0(void) {
    ei_epag_t e = ei_epag_data(0);
    TEST_ASSERT_EQUAL_INT(-1, e.day);
    TEST_ASSERT_EQUAL_STRING("?", e.deity);
}

void test_epag_invalid_6(void) {
    ei_epag_t e = ei_epag_data(6);
    TEST_ASSERT_EQUAL_INT(-1, e.day);
    TEST_ASSERT_EQUAL_STRING("?", e.deity);
}

/* ============================================================
 * ei_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_thoth(void) {
    egyptian_interp_t r = ei_interpret(1, 14, 500);
    TEST_ASSERT_EQUAL_STRING("Tho", r.glyph);
}

void test_interpret_glyph_mesore(void) {
    egyptian_interp_t r = ei_interpret(12, 1, 100);
    TEST_ASSERT_EQUAL_STRING("Mes", r.glyph);
}

void test_interpret_glyph_epagomenal(void) {
    egyptian_interp_t r = ei_interpret(13, 3, 100);
    TEST_ASSERT_EQUAL_STRING("Epg", r.glyph);
}

void test_interpret_glance_regular(void) {
    egyptian_interp_t r = ei_interpret(1, 14, 500);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thoth"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "day 14"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Akhet"));
}

void test_interpret_glance_epag(void) {
    egyptian_interp_t r = ei_interpret(13, 1, 500);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Osiris"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dying King"));
}

void test_interpret_detail_contains_deity(void) {
    egyptian_interp_t r = ei_interpret(1, 14, 500);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Thoth"));
}

void test_interpret_detail_contains_season(void) {
    egyptian_interp_t r = ei_interpret(1, 14, 500);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Akhet"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Inundation"));
}

void test_interpret_detail_contains_sothic(void) {
    egyptian_interp_t r = ei_interpret(1, 14, 500);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "500 of 1461"));
}

void test_interpret_detail_peret_season(void) {
    egyptian_interp_t r = ei_interpret(5, 1, 200);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Peret"));
}

void test_interpret_detail_shemu_season(void) {
    egyptian_interp_t r = ei_interpret(9, 1, 200);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Shemu"));
}

void test_interpret_epag_detail_contains_theme(void) {
    egyptian_interp_t r = ei_interpret(13, 4, 300);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Isis"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "magic"));
}

void test_interpret_invalid_month(void) {
    egyptian_interp_t r = ei_interpret(0, 1, 100);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

void test_interpret_invalid_month_99(void) {
    egyptian_interp_t r = ei_interpret(99, 1, 100);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_season_count(void) {
    TEST_ASSERT_EQUAL_INT(4, ei_season_count());
}

void test_month_count(void) {
    TEST_ASSERT_EQUAL_INT(12, ei_month_count());
}

void test_epag_count(void) {
    TEST_ASSERT_EQUAL_INT(5, ei_epag_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* ei_season_data */
    RUN_TEST(test_season_akhet);
    RUN_TEST(test_season_peret);
    RUN_TEST(test_season_shemu);
    RUN_TEST(test_season_epagomenal);
    RUN_TEST(test_season_all_4_valid);
    RUN_TEST(test_season_invalid_neg1);
    RUN_TEST(test_season_invalid_4);

    /* ei_month_data */
    RUN_TEST(test_month_thoth);
    RUN_TEST(test_month_choiak);
    RUN_TEST(test_month_mesore);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid_0);
    RUN_TEST(test_month_invalid_13);

    /* ei_epag_data */
    RUN_TEST(test_epag_osiris);
    RUN_TEST(test_epag_horus);
    RUN_TEST(test_epag_set);
    RUN_TEST(test_epag_isis);
    RUN_TEST(test_epag_nephthys);
    RUN_TEST(test_epag_all_5_valid);
    RUN_TEST(test_epag_invalid_0);
    RUN_TEST(test_epag_invalid_6);

    /* ei_interpret */
    RUN_TEST(test_interpret_glyph_thoth);
    RUN_TEST(test_interpret_glyph_mesore);
    RUN_TEST(test_interpret_glyph_epagomenal);
    RUN_TEST(test_interpret_glance_regular);
    RUN_TEST(test_interpret_glance_epag);
    RUN_TEST(test_interpret_detail_contains_deity);
    RUN_TEST(test_interpret_detail_contains_season);
    RUN_TEST(test_interpret_detail_contains_sothic);
    RUN_TEST(test_interpret_detail_peret_season);
    RUN_TEST(test_interpret_detail_shemu_season);
    RUN_TEST(test_interpret_epag_detail_contains_theme);
    RUN_TEST(test_interpret_invalid_month);
    RUN_TEST(test_interpret_invalid_month_99);

    /* counts */
    RUN_TEST(test_season_count);
    RUN_TEST(test_month_count);
    RUN_TEST(test_epag_count);

    return UNITY_END();
}
