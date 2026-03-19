#include "../../unity/unity.h"
#include "../../../src/systems/ethiopian/ethiopian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_month_meskerem(void) {
    eti_month_t m = eti_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Meskerem", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.season, "flowers"));
}

void test_month_pagume(void) {
    eti_month_t m = eti_month_data(13);
    TEST_ASSERT_EQUAL_INT(13, m.month);
    TEST_ASSERT_EQUAL_STRING("Pagume", m.name);
}

void test_month_all_13_valid(void) {
    for (int i = 1; i <= 13; i++) {
        eti_month_t m = eti_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.meaning);
        TEST_ASSERT_NOT_NULL(m.season);
        TEST_ASSERT_NOT_NULL(m.brief);
    }
}

void test_month_invalid(void) {
    eti_month_t m = eti_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_feast_enkutatash(void) {
    eti_feast_t f = eti_feast_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.feast);
    TEST_ASSERT_EQUAL_STRING("Enkutatash", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "New Year"));
}

void test_feast_timkat(void) {
    eti_feast_t f = eti_feast_data(4);
    TEST_ASSERT_EQUAL_INT(4, f.feast);
    TEST_ASSERT_EQUAL_STRING("Timkat", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Epiphany"));
}

void test_feast_all_4_valid(void) {
    for (int i = 1; i <= 4; i++) {
        eti_feast_t f = eti_feast_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.feast);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
    }
}

void test_feast_invalid(void) {
    eti_feast_t f = eti_feast_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.feast);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_interpret_glyph(void) {
    ethiopian_interp_t r = eti_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING("Mes", r.glyph);
}

void test_interpret_glance_feast(void) {
    ethiopian_interp_t r = eti_interpret(1, 1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Meskerem"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Enkutatash"));
}

void test_interpret_glance_no_feast(void) {
    ethiopian_interp_t r = eti_interpret(6, 15, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yekatit"));
}

void test_interpret_detail_feast(void) {
    ethiopian_interp_t r = eti_interpret(1, 17, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Meskel"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Cross"));
}

void test_interpret_detail_season(void) {
    ethiopian_interp_t r = eti_interpret(10, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "rainy"));
}

void test_interpret_invalid(void) {
    ethiopian_interp_t r = eti_interpret(0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_month_count(void) {
    TEST_ASSERT_EQUAL_INT(13, eti_month_count());
}

void test_feast_count(void) {
    TEST_ASSERT_EQUAL_INT(4, eti_feast_count());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_meskerem);
    RUN_TEST(test_month_pagume);
    RUN_TEST(test_month_all_13_valid);
    RUN_TEST(test_month_invalid);
    RUN_TEST(test_feast_enkutatash);
    RUN_TEST(test_feast_timkat);
    RUN_TEST(test_feast_all_4_valid);
    RUN_TEST(test_feast_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance_feast);
    RUN_TEST(test_interpret_glance_no_feast);
    RUN_TEST(test_interpret_detail_feast);
    RUN_TEST(test_interpret_detail_season);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_month_count);
    RUN_TEST(test_feast_count);
    return UNITY_END();
}
