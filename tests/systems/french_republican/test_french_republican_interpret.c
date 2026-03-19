#include "../../unity/unity.h"
#include "../../../src/systems/french_republican/french_republican_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_month_germinal(void) {
    fri_month_t m = fri_month_data(7);
    TEST_ASSERT_EQUAL_INT(7, m.month);
    TEST_ASSERT_EQUAL_STRING("Germinal", m.french);
    TEST_ASSERT_EQUAL_STRING("Germination", m.english);
}

void test_month_thermidor(void) {
    fri_month_t m = fri_month_data(11);
    TEST_ASSERT_EQUAL_INT(11, m.month);
    TEST_ASSERT_EQUAL_STRING("Thermidor", m.french);
    TEST_ASSERT_NOT_NULL(strstr(m.brief, "Robespierre"));
}

void test_month_all_12_valid(void) {
    for (int i = 1; i <= 12; i++) {
        fri_month_t m = fri_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.french);
        TEST_ASSERT_NOT_NULL(m.english);
        TEST_ASSERT_NOT_NULL(m.nature);
    }
}

void test_month_invalid(void) {
    fri_month_t m = fri_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.french);
}

void test_sans_virtue(void) {
    fri_sans_t s = fri_sans_data(1);
    TEST_ASSERT_EQUAL_INT(1, s.day);
    TEST_ASSERT_EQUAL_STRING("Virtue", s.virtue);
}

void test_sans_revolution(void) {
    fri_sans_t s = fri_sans_data(6);
    TEST_ASSERT_EQUAL_INT(6, s.day);
    TEST_ASSERT_EQUAL_STRING("Revolution", s.virtue);
}

void test_sans_all_6_valid(void) {
    for (int i = 1; i <= 6; i++) {
        fri_sans_t s = fri_sans_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.day);
        TEST_ASSERT_NOT_NULL(s.french);
        TEST_ASSERT_NOT_NULL(s.virtue);
        TEST_ASSERT_NOT_NULL(s.brief);
    }
}

void test_sans_invalid(void) {
    fri_sans_t s = fri_sans_data(0);
    TEST_ASSERT_EQUAL_INT(-1, s.day);
    TEST_ASSERT_EQUAL_STRING("?", s.french);
}

void test_interpret_glyph(void) {
    french_republican_interp_t r = fri_interpret(234, 7, 14);
    TEST_ASSERT_EQUAL_STRING("Ger", r.glyph);
}

void test_interpret_glance(void) {
    french_republican_interp_t r = fri_interpret(234, 7, 14);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "An 234"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Germinal"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Germination"));
}

void test_interpret_sansculottide(void) {
    french_republican_interp_t r = fri_interpret(234, 0, 3);
    TEST_ASSERT_EQUAL_STRING("San", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Labor"));
}

void test_interpret_invalid(void) {
    french_republican_interp_t r = fri_interpret(234, 13, 1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_month_count(void) { TEST_ASSERT_EQUAL_INT(12, fri_month_count()); }
void test_sans_count(void) { TEST_ASSERT_EQUAL_INT(6, fri_sans_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_month_germinal);
    RUN_TEST(test_month_thermidor);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid);
    RUN_TEST(test_sans_virtue);
    RUN_TEST(test_sans_revolution);
    RUN_TEST(test_sans_all_6_valid);
    RUN_TEST(test_sans_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance);
    RUN_TEST(test_interpret_sansculottide);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_month_count);
    RUN_TEST(test_sans_count);
    return UNITY_END();
}
