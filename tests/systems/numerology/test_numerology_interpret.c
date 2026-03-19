#include "../../unity/unity.h"
#include "../../../src/systems/numerology/numerology_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_number_1(void) {
    ni_number_t n = ni_number_data(1);
    TEST_ASSERT_EQUAL_INT(1, n.number);
    TEST_ASSERT_EQUAL_STRING("The Pioneer", n.name);
}

void test_number_7(void) {
    ni_number_t n = ni_number_data(7);
    TEST_ASSERT_EQUAL_INT(7, n.number);
    TEST_ASSERT_EQUAL_STRING("The Seeker", n.name);
}

void test_number_9(void) {
    ni_number_t n = ni_number_data(9);
    TEST_ASSERT_EQUAL_INT(9, n.number);
    TEST_ASSERT_EQUAL_STRING("The Humanitarian", n.name);
}

void test_master_11(void) {
    ni_number_t n = ni_number_data(11);
    TEST_ASSERT_EQUAL_INT(11, n.number);
    TEST_ASSERT_EQUAL_STRING("The Illuminator", n.name);
}

void test_master_22(void) {
    ni_number_t n = ni_number_data(22);
    TEST_ASSERT_EQUAL_INT(22, n.number);
    TEST_ASSERT_EQUAL_STRING("The Master Builder", n.name);
}

void test_master_33(void) {
    ni_number_t n = ni_number_data(33);
    TEST_ASSERT_EQUAL_INT(33, n.number);
    TEST_ASSERT_EQUAL_STRING("The Master Teacher", n.name);
}

void test_all_9_roots_valid(void) {
    for (int i = 1; i <= 9; i++) {
        ni_number_t n = ni_number_data(i);
        TEST_ASSERT_EQUAL_INT(i, n.number);
        TEST_ASSERT_NOT_NULL(n.name);
        TEST_ASSERT_NOT_NULL(n.quality);
        TEST_ASSERT_NOT_NULL(n.strength);
        TEST_ASSERT_NOT_NULL(n.challenge);
        TEST_ASSERT_NOT_NULL(n.brief);
    }
}

void test_invalid_0(void) {
    ni_number_t n = ni_number_data(0);
    TEST_ASSERT_EQUAL_INT(-1, n.number);
    TEST_ASSERT_EQUAL_STRING("?", n.name);
}

void test_invalid_10(void) {
    ni_number_t n = ni_number_data(10);
    TEST_ASSERT_EQUAL_INT(-1, n.number);
    TEST_ASSERT_EQUAL_STRING("?", n.name);
}

void test_interpret_glyph(void) {
    numerology_interp_t r = ni_interpret(7, -1);
    TEST_ASSERT_EQUAL_STRING("7", r.glyph);
}

void test_interpret_glyph_master(void) {
    numerology_interp_t r = ni_interpret(11, -1);
    TEST_ASSERT_EQUAL_STRING("11", r.glyph);
}

void test_interpret_glance(void) {
    numerology_interp_t r = ni_interpret(7, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Life Path 7"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Seeker"));
}

void test_interpret_glance_with_year(void) {
    numerology_interp_t r = ni_interpret(7, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Year 1"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pioneer"));
}

void test_interpret_detail(void) {
    numerology_interp_t r = ni_interpret(4, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Builder"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Reliability"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Rigidity"));
}

void test_interpret_detail_personal_year(void) {
    numerology_interp_t r = ni_interpret(7, 9);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Personal Year 9"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Humanitarian"));
}

void test_interpret_invalid(void) {
    numerology_interp_t r = ni_interpret(0, -1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_count(void) { TEST_ASSERT_EQUAL_INT(12, ni_number_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_number_1);
    RUN_TEST(test_number_7);
    RUN_TEST(test_number_9);
    RUN_TEST(test_master_11);
    RUN_TEST(test_master_22);
    RUN_TEST(test_master_33);
    RUN_TEST(test_all_9_roots_valid);
    RUN_TEST(test_invalid_0);
    RUN_TEST(test_invalid_10);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glyph_master);
    RUN_TEST(test_interpret_glance);
    RUN_TEST(test_interpret_glance_with_year);
    RUN_TEST(test_interpret_detail);
    RUN_TEST(test_interpret_detail_personal_year);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_count);
    return UNITY_END();
}
