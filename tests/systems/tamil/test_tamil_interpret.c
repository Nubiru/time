#include "../../unity/unity.h"
#include "../../../src/systems/tamil/tamil_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_month_chittirai(void) {
    tmi_month_t m = tmi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Chittirai", m.tamil);
    TEST_ASSERT_NOT_NULL(strstr(m.rasi, "Aries"));
}

void test_month_margazhi(void) {
    tmi_month_t m = tmi_month_data(9);
    TEST_ASSERT_EQUAL_INT(9, m.month);
    TEST_ASSERT_EQUAL_STRING("Margazhi", m.tamil);
    TEST_ASSERT_NOT_NULL(strstr(m.season, "dawn"));
}

void test_month_all_12_valid(void) {
    for (int i = 1; i <= 12; i++) {
        tmi_month_t m = tmi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.tamil);
        TEST_ASSERT_NOT_NULL(m.rasi);
        TEST_ASSERT_NOT_NULL(m.season);
        TEST_ASSERT_NOT_NULL(m.brief);
    }
}

void test_month_invalid(void) {
    tmi_month_t m = tmi_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.tamil);
}

void test_festival_pongal(void) {
    tmi_festival_t f = tmi_festival_data(2);
    TEST_ASSERT_EQUAL_INT(2, f.festival);
    TEST_ASSERT_EQUAL_STRING("Pongal", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Harvest"));
}

void test_festival_deepavali(void) {
    tmi_festival_t f = tmi_festival_data(4);
    TEST_ASSERT_EQUAL_INT(4, f.festival);
    TEST_ASSERT_EQUAL_STRING("Deepavali", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "light"));
}

void test_festival_all_4_valid(void) {
    for (int i = 1; i <= 4; i++) {
        tmi_festival_t f = tmi_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
    }
}

void test_festival_invalid(void) {
    tmi_festival_t f = tmi_festival_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_interpret_glyph(void) {
    tamil_interp_t r = tmi_interpret(10, 1, 30, 2);
    TEST_ASSERT_EQUAL_STRING("Tha", r.glyph);
}

void test_interpret_glance_festival(void) {
    tamil_interp_t r = tmi_interpret(10, 1, 30, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thai"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pongal"));
}

void test_interpret_glance_no_festival(void) {
    tamil_interp_t r = tmi_interpret(9, 15, 30, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Margazhi"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sagittarius"));
}

void test_interpret_detail_jovian(void) {
    tamil_interp_t r = tmi_interpret(1, 1, 42, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "42 of 60"));
}

void test_interpret_detail_festival(void) {
    tamil_interp_t r = tmi_interpret(1, 1, 30, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Puthandu"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sidereal"));
}

void test_interpret_invalid(void) {
    tamil_interp_t r = tmi_interpret(0, 1, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_month_count(void) { TEST_ASSERT_EQUAL_INT(12, tmi_month_count()); }
void test_festival_count(void) { TEST_ASSERT_EQUAL_INT(4, tmi_festival_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_chittirai);
    RUN_TEST(test_month_margazhi);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid);
    RUN_TEST(test_festival_pongal);
    RUN_TEST(test_festival_deepavali);
    RUN_TEST(test_festival_all_4_valid);
    RUN_TEST(test_festival_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance_festival);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_detail_jovian);
    RUN_TEST(test_interpret_detail_festival);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_month_count);
    RUN_TEST(test_festival_count);
    return UNITY_END();
}
