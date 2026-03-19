#include "../../unity/unity.h"
#include "../../../src/systems/zoroastrian/zoroastrian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * zi_amesha_data — 7 Amesha Spentas
 * ============================================================ */

void test_amesha_ahura_mazda(void) {
    zi_amesha_t a = zi_amesha_data(0);
    TEST_ASSERT_EQUAL_INT(0, a.index);
    TEST_ASSERT_NOT_NULL(strstr(a.avestan, "Ahura Mazda"));
    TEST_ASSERT_NOT_NULL(strstr(a.meaning, "Wise Lord"));
    TEST_ASSERT_NOT_NULL(strstr(a.virtue, "Wisdom"));
}

void test_amesha_vohu_manah(void) {
    zi_amesha_t a = zi_amesha_data(1);
    TEST_ASSERT_EQUAL_INT(1, a.index);
    TEST_ASSERT_NOT_NULL(strstr(a.avestan, "Vohu Manah"));
    TEST_ASSERT_NOT_NULL(strstr(a.meaning, "Good Mind"));
}

void test_amesha_asha(void) {
    zi_amesha_t a = zi_amesha_data(2);
    TEST_ASSERT_EQUAL_INT(2, a.index);
    TEST_ASSERT_NOT_NULL(strstr(a.avestan, "Asha"));
    TEST_ASSERT_NOT_NULL(strstr(a.element, "Fire"));
}

void test_amesha_ameretat(void) {
    zi_amesha_t a = zi_amesha_data(6);
    TEST_ASSERT_EQUAL_INT(6, a.index);
    TEST_ASSERT_NOT_NULL(strstr(a.avestan, "Ameretat"));
    TEST_ASSERT_NOT_NULL(strstr(a.meaning, "Immortality"));
    TEST_ASSERT_NOT_NULL(strstr(a.element, "Plants"));
}

void test_amesha_all_7_valid(void) {
    for (int i = 0; i < 7; i++) {
        zi_amesha_t a = zi_amesha_data(i);
        TEST_ASSERT_EQUAL_INT(i, a.index);
        TEST_ASSERT_NOT_NULL(a.avestan);
        TEST_ASSERT_NOT_NULL(a.meaning);
        TEST_ASSERT_NOT_NULL(a.domain);
        TEST_ASSERT_NOT_NULL(a.element);
        TEST_ASSERT_NOT_NULL(a.virtue);
        TEST_ASSERT_NOT_NULL(a.brief);
        TEST_ASSERT_TRUE(strlen(a.avestan) > 0);
    }
}

void test_amesha_invalid_neg1(void) {
    zi_amesha_t a = zi_amesha_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
    TEST_ASSERT_EQUAL_STRING("?", a.avestan);
}

void test_amesha_invalid_7(void) {
    zi_amesha_t a = zi_amesha_data(7);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
    TEST_ASSERT_EQUAL_STRING("?", a.avestan);
}

/* ============================================================
 * zi_gatha_data — 5 Gatha days
 * ============================================================ */

void test_gatha_ahunavad(void) {
    zi_gatha_t g = zi_gatha_data(1);
    TEST_ASSERT_EQUAL_INT(1, g.day);
    TEST_ASSERT_EQUAL_STRING("Ahunavad", g.name);
    TEST_ASSERT_NOT_NULL(strstr(g.theme, "will"));
}

void test_gatha_vahishtoisht(void) {
    zi_gatha_t g = zi_gatha_data(5);
    TEST_ASSERT_EQUAL_INT(5, g.day);
    TEST_ASSERT_EQUAL_STRING("Vahishtoisht", g.name);
}

void test_gatha_all_5_valid(void) {
    for (int i = 1; i <= 5; i++) {
        zi_gatha_t g = zi_gatha_data(i);
        TEST_ASSERT_EQUAL_INT(i, g.day);
        TEST_ASSERT_NOT_NULL(g.name);
        TEST_ASSERT_NOT_NULL(g.theme);
        TEST_ASSERT_NOT_NULL(g.practice);
        TEST_ASSERT_NOT_NULL(g.brief);
        TEST_ASSERT_TRUE(strlen(g.name) > 0);
    }
}

void test_gatha_invalid_0(void) {
    zi_gatha_t g = zi_gatha_data(0);
    TEST_ASSERT_EQUAL_INT(-1, g.day);
    TEST_ASSERT_EQUAL_STRING("?", g.name);
}

void test_gatha_invalid_6(void) {
    zi_gatha_t g = zi_gatha_data(6);
    TEST_ASSERT_EQUAL_INT(-1, g.day);
    TEST_ASSERT_EQUAL_STRING("?", g.name);
}

/* ============================================================
 * zi_festival_data — 9 festivals
 * ============================================================ */

void test_festival_nowruz(void) {
    zi_festival_t f = zi_festival_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.festival);
    TEST_ASSERT_EQUAL_STRING("Nowruz", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "rebirth"));
}

void test_festival_yalda(void) {
    zi_festival_t f = zi_festival_data(8);
    TEST_ASSERT_EQUAL_INT(8, f.festival);
    TEST_ASSERT_EQUAL_STRING("Yalda", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "light"));
}

void test_festival_sadeh(void) {
    zi_festival_t f = zi_festival_data(9);
    TEST_ASSERT_EQUAL_INT(9, f.festival);
    TEST_ASSERT_EQUAL_STRING("Sadeh", f.name);
}

void test_festival_all_9_valid(void) {
    for (int i = 1; i <= 9; i++) {
        zi_festival_t f = zi_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
        TEST_ASSERT_TRUE(strlen(f.name) > 0);
    }
}

void test_festival_invalid_0(void) {
    zi_festival_t f = zi_festival_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_festival_invalid_10(void) {
    zi_festival_t f = zi_festival_data(10);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* ============================================================
 * zi_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_farvardin(void) {
    zoroastrian_interp_t r = zi_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING("Far", r.glyph);
}

void test_interpret_glyph_esfand(void) {
    zoroastrian_interp_t r = zi_interpret(12, 15, 0);
    TEST_ASSERT_EQUAL_STRING("Esf", r.glyph);
}

void test_interpret_glyph_gatha(void) {
    zoroastrian_interp_t r = zi_interpret(13, 1, 0);
    TEST_ASSERT_EQUAL_STRING("Gth", r.glyph);
}

void test_interpret_glance_with_festival(void) {
    zoroastrian_interp_t r = zi_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Hormozd"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nowruz"));
}

void test_interpret_glance_no_festival(void) {
    zoroastrian_interp_t r = zi_interpret(1, 5, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
    TEST_ASSERT_NULL(strstr(r.glance, "Nowruz"));
}

void test_interpret_glance_gatha(void) {
    zoroastrian_interp_t r = zi_interpret(13, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Gatha"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Ahunavad"));
}

void test_interpret_detail_amesha_day1(void) {
    zoroastrian_interp_t r = zi_interpret(1, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Amesha Spenta"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ahura Mazda"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Wise Lord"));
}

void test_interpret_detail_amesha_day7(void) {
    zoroastrian_interp_t r = zi_interpret(3, 7, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Amesha Spenta"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ameretat"));
}

void test_interpret_detail_no_amesha_day8(void) {
    zoroastrian_interp_t r = zi_interpret(1, 8, 0);
    TEST_ASSERT_NULL(strstr(r.detail, "Amesha Spenta"));
}

void test_interpret_detail_festival(void) {
    zoroastrian_interp_t r = zi_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Festival: Nowruz"));
}

void test_interpret_gatha_detail(void) {
    zoroastrian_interp_t r = zi_interpret(13, 3, 7);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Spentamad"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Festival: Hamaspathmaidyem"));
}

void test_interpret_invalid_month(void) {
    zoroastrian_interp_t r = zi_interpret(0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

void test_interpret_invalid_month_99(void) {
    zoroastrian_interp_t r = zi_interpret(99, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_amesha_count(void) {
    TEST_ASSERT_EQUAL_INT(7, zi_amesha_count());
}

void test_gatha_count(void) {
    TEST_ASSERT_EQUAL_INT(5, zi_gatha_count());
}

void test_festival_count(void) {
    TEST_ASSERT_EQUAL_INT(9, zi_festival_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* zi_amesha_data */
    RUN_TEST(test_amesha_ahura_mazda);
    RUN_TEST(test_amesha_vohu_manah);
    RUN_TEST(test_amesha_asha);
    RUN_TEST(test_amesha_ameretat);
    RUN_TEST(test_amesha_all_7_valid);
    RUN_TEST(test_amesha_invalid_neg1);
    RUN_TEST(test_amesha_invalid_7);

    /* zi_gatha_data */
    RUN_TEST(test_gatha_ahunavad);
    RUN_TEST(test_gatha_vahishtoisht);
    RUN_TEST(test_gatha_all_5_valid);
    RUN_TEST(test_gatha_invalid_0);
    RUN_TEST(test_gatha_invalid_6);

    /* zi_festival_data */
    RUN_TEST(test_festival_nowruz);
    RUN_TEST(test_festival_yalda);
    RUN_TEST(test_festival_sadeh);
    RUN_TEST(test_festival_all_9_valid);
    RUN_TEST(test_festival_invalid_0);
    RUN_TEST(test_festival_invalid_10);

    /* zi_interpret */
    RUN_TEST(test_interpret_glyph_farvardin);
    RUN_TEST(test_interpret_glyph_esfand);
    RUN_TEST(test_interpret_glyph_gatha);
    RUN_TEST(test_interpret_glance_with_festival);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_glance_gatha);
    RUN_TEST(test_interpret_detail_amesha_day1);
    RUN_TEST(test_interpret_detail_amesha_day7);
    RUN_TEST(test_interpret_detail_no_amesha_day8);
    RUN_TEST(test_interpret_detail_festival);
    RUN_TEST(test_interpret_gatha_detail);
    RUN_TEST(test_interpret_invalid_month);
    RUN_TEST(test_interpret_invalid_month_99);

    /* counts */
    RUN_TEST(test_amesha_count);
    RUN_TEST(test_gatha_count);
    RUN_TEST(test_festival_count);

    return UNITY_END();
}
