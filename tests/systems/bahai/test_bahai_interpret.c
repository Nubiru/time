#include "../../unity/unity.h"
#include "../../../src/systems/bahai/bahai_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_month_baha(void) {
    bhi_month_t m = bhi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Baha", m.arabic);
    TEST_ASSERT_EQUAL_STRING("Splendour", m.english);
}

void test_month_ala(void) {
    bhi_month_t m = bhi_month_data(19);
    TEST_ASSERT_EQUAL_INT(19, m.month);
    TEST_ASSERT_EQUAL_STRING("Ala", m.arabic);
    TEST_ASSERT_EQUAL_STRING("Loftiness", m.english);
}

void test_month_intercalary(void) {
    bhi_month_t m = bhi_month_data(0);
    TEST_ASSERT_EQUAL_INT(0, m.month);
    TEST_ASSERT_NOT_NULL(strstr(m.arabic, "Ayyam"));
}

void test_month_all_20_valid(void) {
    for (int i = 0; i <= 19; i++) {
        bhi_month_t m = bhi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.arabic);
        TEST_ASSERT_NOT_NULL(m.english);
        TEST_ASSERT_NOT_NULL(m.quality);
        TEST_ASSERT_NOT_NULL(m.brief);
    }
}

void test_month_invalid(void) {
    bhi_month_t m = bhi_month_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.arabic);
}

void test_holy_day_nawruz(void) {
    bhi_holy_day_t h = bhi_holy_day_data(1);
    TEST_ASSERT_EQUAL_INT(1, h.holy_day);
    TEST_ASSERT_EQUAL_STRING("Naw-Ruz", h.name);
}

void test_holy_day_ridvan(void) {
    bhi_holy_day_t h = bhi_holy_day_data(2);
    TEST_ASSERT_EQUAL_INT(2, h.holy_day);
    TEST_ASSERT_NOT_NULL(strstr(h.name, "Ridvan"));
}

void test_holy_day_all_8_valid(void) {
    for (int i = 1; i <= 8; i++) {
        bhi_holy_day_t h = bhi_holy_day_data(i);
        TEST_ASSERT_EQUAL_INT(i, h.holy_day);
        TEST_ASSERT_NOT_NULL(h.name);
        TEST_ASSERT_NOT_NULL(h.theme);
        TEST_ASSERT_NOT_NULL(h.brief);
    }
}

void test_holy_day_invalid(void) {
    bhi_holy_day_t h = bhi_holy_day_data(0);
    TEST_ASSERT_EQUAL_INT(-1, h.holy_day);
    TEST_ASSERT_EQUAL_STRING("?", h.name);
}

void test_interpret_glyph(void) {
    bahai_interp_t r = bhi_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING("Bah", r.glyph);
}

void test_interpret_glance_holy(void) {
    bahai_interp_t r = bhi_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Baha"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Naw-Ruz"));
}

void test_interpret_glance_no_holy(void) {
    bahai_interp_t r = bhi_interpret(12, 10, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Ilm"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Knowledge"));
}

void test_interpret_detail_quality(void) {
    bahai_interp_t r = bhi_interpret(19, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Loftiness"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "fasting"));
}

void test_interpret_invalid(void) {
    bahai_interp_t r = bhi_interpret(-1, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_month_count(void) { TEST_ASSERT_EQUAL_INT(20, bhi_month_count()); }
void test_holy_day_count(void) { TEST_ASSERT_EQUAL_INT(8, bhi_holy_day_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_baha);
    RUN_TEST(test_month_ala);
    RUN_TEST(test_month_intercalary);
    RUN_TEST(test_month_all_20_valid);
    RUN_TEST(test_month_invalid);
    RUN_TEST(test_holy_day_nawruz);
    RUN_TEST(test_holy_day_ridvan);
    RUN_TEST(test_holy_day_all_8_valid);
    RUN_TEST(test_holy_day_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance_holy);
    RUN_TEST(test_interpret_glance_no_holy);
    RUN_TEST(test_interpret_detail_quality);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_month_count);
    RUN_TEST(test_holy_day_count);
    return UNITY_END();
}
