#include "../../unity/unity.h"
#include "../../../src/systems/myanmar/myanmar_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_month_tagu(void) {
    mmi_month_t m = mmi_month_data(0);
    TEST_ASSERT_EQUAL_INT(0, m.month);
    TEST_ASSERT_EQUAL_STRING("Tagu", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.festival, "Thingyan"));
}

void test_month_thadingyut(void) {
    mmi_month_t m = mmi_month_data(6);
    TEST_ASSERT_EQUAL_INT(6, m.month);
    TEST_ASSERT_EQUAL_STRING("Thadingyut", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.festival, "Lights"));
}

void test_month_all_12_valid(void) {
    for (int i = 0; i < 12; i++) {
        mmi_month_t m = mmi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.season);
        TEST_ASSERT_NOT_NULL(m.festival);
        TEST_ASSERT_NOT_NULL(m.brief);
    }
}

void test_month_invalid(void) {
    mmi_month_t m = mmi_month_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_interpret_glyph(void) {
    myanmar_interp_t r = mmi_interpret(0, 1, 1, 0);
    TEST_ASSERT_EQUAL_STRING("Tag", r.glyph);
}

void test_interpret_thingyan(void) {
    myanmar_interp_t r = mmi_interpret(0, 1, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thingyan"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "water festival"));
}

void test_interpret_normal(void) {
    myanmar_interp_t r = mmi_interpret(6, 15, 0, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Thadingyut"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Lights"));
}

void test_interpret_year_type(void) {
    myanmar_interp_t r = mmi_interpret(3, 1, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Small Watat"));
}

void test_interpret_invalid(void) {
    myanmar_interp_t r = mmi_interpret(12, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_month_count(void) { TEST_ASSERT_EQUAL_INT(12, mmi_month_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_tagu);
    RUN_TEST(test_month_thadingyut);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_thingyan);
    RUN_TEST(test_interpret_normal);
    RUN_TEST(test_interpret_year_type);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_month_count);
    return UNITY_END();
}
