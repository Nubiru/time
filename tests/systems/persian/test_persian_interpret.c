#include "../../unity/unity.h"
#include "../../../src/systems/persian/persian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * pi_month_data — 12 months
 * ============================================================ */

void test_month_farvardin(void) {
    pi_month_t m = pi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Farvardin", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.avestan, "Fravashi"));
}

void test_month_mehr(void) {
    pi_month_t m = pi_month_data(7);
    TEST_ASSERT_EQUAL_INT(7, m.month);
    TEST_ASSERT_EQUAL_STRING("Mehr", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.avestan, "Mithra"));
}

void test_month_esfand(void) {
    pi_month_t m = pi_month_data(12);
    TEST_ASSERT_EQUAL_INT(12, m.month);
    TEST_ASSERT_EQUAL_STRING("Esfand", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.avestan, "Spenta Armaiti"));
}

void test_month_all_12_valid(void) {
    for (int i = 1; i <= 12; i++) {
        pi_month_t m = pi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.avestan);
        TEST_ASSERT_NOT_NULL(m.meaning);
        TEST_ASSERT_NOT_NULL(m.season);
        TEST_ASSERT_NOT_NULL(m.brief);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
    }
}

void test_month_invalid_0(void) {
    pi_month_t m = pi_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_month_invalid_13(void) {
    pi_month_t m = pi_month_data(13);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ============================================================
 * pi_festival_data — 4 festivals
 * ============================================================ */

void test_festival_nowruz(void) {
    pi_festival_t f = pi_festival_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.festival);
    TEST_ASSERT_EQUAL_STRING("Nowruz", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "New Day"));
}

void test_festival_sizdah_bedar(void) {
    pi_festival_t f = pi_festival_data(2);
    TEST_ASSERT_EQUAL_INT(2, f.festival);
    TEST_ASSERT_EQUAL_STRING("Sizdah Bedar", f.name);
}

void test_festival_yalda(void) {
    pi_festival_t f = pi_festival_data(3);
    TEST_ASSERT_EQUAL_INT(3, f.festival);
    TEST_ASSERT_EQUAL_STRING("Yalda", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "light"));
}

void test_festival_chaharshanbe_suri(void) {
    pi_festival_t f = pi_festival_data(4);
    TEST_ASSERT_EQUAL_INT(4, f.festival);
    TEST_ASSERT_EQUAL_STRING("Chaharshanbe Suri", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "fire"));
}

void test_festival_all_4_valid(void) {
    for (int i = 1; i <= 4; i++) {
        pi_festival_t f = pi_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
    }
}

void test_festival_invalid_0(void) {
    pi_festival_t f = pi_festival_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_festival_invalid_5(void) {
    pi_festival_t f = pi_festival_data(5);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* ============================================================
 * pi_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_farvardin(void) {
    persian_interp_t r = pi_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING("Far", r.glyph);
}

void test_interpret_glyph_esfand(void) {
    persian_interp_t r = pi_interpret(12, 15, 0);
    TEST_ASSERT_EQUAL_STRING("Esf", r.glyph);
}

void test_interpret_glance_with_festival(void) {
    persian_interp_t r = pi_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nowruz"));
}

void test_interpret_glance_no_festival(void) {
    persian_interp_t r = pi_interpret(7, 15, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mehr"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "oath"));
}

void test_interpret_detail_contains_avestan(void) {
    persian_interp_t r = pi_interpret(7, 15, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Mithra"));
}

void test_interpret_detail_contains_season(void) {
    persian_interp_t r = pi_interpret(1, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "spring"));
}

void test_interpret_detail_festival(void) {
    persian_interp_t r = pi_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Nowruz"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Haft Sin"));
}

void test_interpret_detail_yalda(void) {
    persian_interp_t r = pi_interpret(9, 30, 3);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Yalda"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "pomegranates"));
}

void test_interpret_invalid_month(void) {
    persian_interp_t r = pi_interpret(0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_month_count(void) {
    TEST_ASSERT_EQUAL_INT(12, pi_month_count());
}

void test_festival_count(void) {
    TEST_ASSERT_EQUAL_INT(4, pi_festival_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* pi_month_data */
    RUN_TEST(test_month_farvardin);
    RUN_TEST(test_month_mehr);
    RUN_TEST(test_month_esfand);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid_0);
    RUN_TEST(test_month_invalid_13);

    /* pi_festival_data */
    RUN_TEST(test_festival_nowruz);
    RUN_TEST(test_festival_sizdah_bedar);
    RUN_TEST(test_festival_yalda);
    RUN_TEST(test_festival_chaharshanbe_suri);
    RUN_TEST(test_festival_all_4_valid);
    RUN_TEST(test_festival_invalid_0);
    RUN_TEST(test_festival_invalid_5);

    /* pi_interpret */
    RUN_TEST(test_interpret_glyph_farvardin);
    RUN_TEST(test_interpret_glyph_esfand);
    RUN_TEST(test_interpret_glance_with_festival);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_detail_contains_avestan);
    RUN_TEST(test_interpret_detail_contains_season);
    RUN_TEST(test_interpret_detail_festival);
    RUN_TEST(test_interpret_detail_yalda);
    RUN_TEST(test_interpret_invalid_month);

    /* counts */
    RUN_TEST(test_month_count);
    RUN_TEST(test_festival_count);

    return UNITY_END();
}
