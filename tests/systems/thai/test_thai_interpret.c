#include "../../unity/unity.h"
#include "../../../src/systems/thai/thai_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_month_mesayon(void) {
    ti_thai_month_t m = tti_month_data(4);
    TEST_ASSERT_EQUAL_INT(4, m.month);
    TEST_ASSERT_EQUAL_STRING("Mesayon", m.thai);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Aries"));
}

void test_month_all_12_valid(void) {
    for (int i = 1; i <= 12; i++) {
        ti_thai_month_t m = tti_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.thai);
        TEST_ASSERT_NOT_NULL(m.origin);
        TEST_ASSERT_NOT_NULL(m.season);
        TEST_ASSERT_NOT_NULL(m.brief);
    }
}

void test_month_invalid(void) {
    ti_thai_month_t m = tti_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.thai);
}

void test_festival_songkran(void) {
    ti_thai_festival_t f = tti_festival_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.festival);
    TEST_ASSERT_EQUAL_STRING("Songkran", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Water"));
}

void test_festival_all_3_valid(void) {
    for (int i = 1; i <= 3; i++) {
        ti_thai_festival_t f = tti_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
    }
}

void test_festival_invalid(void) {
    ti_thai_festival_t f = tti_festival_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_interpret_glyph(void) {
    thai_interp_t r = tti_interpret(2569, 4, 13, 1);
    TEST_ASSERT_EQUAL_STRING("Mes", r.glyph);
}

void test_interpret_glance_festival(void) {
    thai_interp_t r = tti_interpret(2569, 4, 13, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "BE 2569"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Songkran"));
}

void test_interpret_glance_no_festival(void) {
    thai_interp_t r = tti_interpret(2569, 7, 15, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Karakadakhom"));
    TEST_ASSERT_NULL(strstr(r.glance, "Songkran"));
}

void test_interpret_detail_sanskrit(void) {
    thai_interp_t r = tti_interpret(2569, 4, 13, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Sanskrit"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Aries"));
}

void test_interpret_detail_festival(void) {
    thai_interp_t r = tti_interpret(2569, 4, 13, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Songkran"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "water"));
}

void test_interpret_invalid(void) {
    thai_interp_t r = tti_interpret(2569, 0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_month_count(void) {
    TEST_ASSERT_EQUAL_INT(12, tti_month_count());
}

void test_festival_count(void) {
    TEST_ASSERT_EQUAL_INT(3, tti_festival_count());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_mesayon);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid);
    RUN_TEST(test_festival_songkran);
    RUN_TEST(test_festival_all_3_valid);
    RUN_TEST(test_festival_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance_festival);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_detail_sanskrit);
    RUN_TEST(test_interpret_detail_festival);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_month_count);
    RUN_TEST(test_festival_count);
    return UNITY_END();
}
