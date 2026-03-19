#include "../../unity/unity.h"
#include "../../../src/systems/coptic/coptic_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * cci_month_data — 13 months
 * ============================================================ */

void test_month_thout(void) {
    ci_coptic_month_t m = cci_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Thout", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Thoth"));
}

void test_month_koiak(void) {
    ci_coptic_month_t m = cci_month_data(4);
    TEST_ASSERT_EQUAL_INT(4, m.month);
    TEST_ASSERT_EQUAL_STRING("Koiak", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.liturgy, "Christmas"));
}

void test_month_nasie(void) {
    ci_coptic_month_t m = cci_month_data(13);
    TEST_ASSERT_EQUAL_INT(13, m.month);
    TEST_ASSERT_EQUAL_STRING("Nasie", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Epagomenal"));
}

void test_month_all_13_valid(void) {
    for (int i = 1; i <= 13; i++) {
        ci_coptic_month_t m = cci_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.origin);
        TEST_ASSERT_NOT_NULL(m.liturgy);
        TEST_ASSERT_NOT_NULL(m.brief);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
    }
}

void test_month_invalid_0(void) {
    ci_coptic_month_t m = cci_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_month_invalid_14(void) {
    ci_coptic_month_t m = cci_month_data(14);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ============================================================
 * cci_feast_data — 6 feasts
 * ============================================================ */

void test_feast_nayrouz(void) {
    ci_coptic_feast_t f = cci_feast_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.feast);
    TEST_ASSERT_EQUAL_STRING("Nayrouz", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Martyrs"));
}

void test_feast_christmas(void) {
    ci_coptic_feast_t f = cci_feast_data(3);
    TEST_ASSERT_EQUAL_INT(3, f.feast);
    TEST_ASSERT_EQUAL_STRING("Christmas", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Nativity"));
}

void test_feast_easter(void) {
    ci_coptic_feast_t f = cci_feast_data(6);
    TEST_ASSERT_EQUAL_INT(6, f.feast);
    TEST_ASSERT_EQUAL_STRING("Easter", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Resurrection"));
}

void test_feast_all_6_valid(void) {
    for (int i = 1; i <= 6; i++) {
        ci_coptic_feast_t f = cci_feast_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.feast);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
    }
}

void test_feast_invalid_0(void) {
    ci_coptic_feast_t f = cci_feast_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.feast);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_feast_invalid_7(void) {
    ci_coptic_feast_t f = cci_feast_data(7);
    TEST_ASSERT_EQUAL_INT(-1, f.feast);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* ============================================================
 * cci_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_thout(void) {
    coptic_interp_t r = cci_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING("Tho", r.glyph);
}

void test_interpret_glyph_nasie(void) {
    coptic_interp_t r = cci_interpret(13, 3, 0);
    TEST_ASSERT_EQUAL_STRING("Nas", r.glyph);
}

void test_interpret_glance_with_feast(void) {
    coptic_interp_t r = cci_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thout"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nayrouz"));
}

void test_interpret_glance_no_feast(void) {
    coptic_interp_t r = cci_interpret(6, 15, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Meshir"));
}

void test_interpret_detail_contains_origin(void) {
    coptic_interp_t r = cci_interpret(1, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Thoth"));
}

void test_interpret_detail_contains_liturgy(void) {
    coptic_interp_t r = cci_interpret(4, 29, 3);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Christmas"));
}

void test_interpret_detail_feast(void) {
    coptic_interp_t r = cci_interpret(4, 29, 3);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Feast: Christmas"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Nativity"));
}

void test_interpret_invalid_month(void) {
    coptic_interp_t r = cci_interpret(0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_month_count(void) {
    TEST_ASSERT_EQUAL_INT(13, cci_month_count());
}

void test_feast_count(void) {
    TEST_ASSERT_EQUAL_INT(6, cci_feast_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* cci_month_data */
    RUN_TEST(test_month_thout);
    RUN_TEST(test_month_koiak);
    RUN_TEST(test_month_nasie);
    RUN_TEST(test_month_all_13_valid);
    RUN_TEST(test_month_invalid_0);
    RUN_TEST(test_month_invalid_14);

    /* cci_feast_data */
    RUN_TEST(test_feast_nayrouz);
    RUN_TEST(test_feast_christmas);
    RUN_TEST(test_feast_easter);
    RUN_TEST(test_feast_all_6_valid);
    RUN_TEST(test_feast_invalid_0);
    RUN_TEST(test_feast_invalid_7);

    /* cci_interpret */
    RUN_TEST(test_interpret_glyph_thout);
    RUN_TEST(test_interpret_glyph_nasie);
    RUN_TEST(test_interpret_glance_with_feast);
    RUN_TEST(test_interpret_glance_no_feast);
    RUN_TEST(test_interpret_detail_contains_origin);
    RUN_TEST(test_interpret_detail_contains_liturgy);
    RUN_TEST(test_interpret_detail_feast);
    RUN_TEST(test_interpret_invalid_month);

    /* counts */
    RUN_TEST(test_month_count);
    RUN_TEST(test_feast_count);

    return UNITY_END();
}
