#include "../../unity/unity.h"
#include "../../../src/systems/aztec/aztec_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_sign_cipactli(void) {
    azi_sign_t s = azi_sign_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.sign);
    TEST_ASSERT_EQUAL_STRING("Cipactli", s.nahuatl);
    TEST_ASSERT_EQUAL_STRING("Crocodile", s.english);
    TEST_ASSERT_EQUAL_STRING("East", s.direction);
}

void test_sign_ocelotl(void) {
    azi_sign_t s = azi_sign_data(13);
    TEST_ASSERT_EQUAL_INT(13, s.sign);
    TEST_ASSERT_EQUAL_STRING("Ocelotl", s.nahuatl);
    TEST_ASSERT_EQUAL_STRING("Jaguar", s.english);
}

void test_sign_xochitl(void) {
    azi_sign_t s = azi_sign_data(19);
    TEST_ASSERT_EQUAL_INT(19, s.sign);
    TEST_ASSERT_EQUAL_STRING("Xochitl", s.nahuatl);
    TEST_ASSERT_EQUAL_STRING("Flower", s.english);
}

void test_sign_all_20_valid(void) {
    for (int i = 0; i < 20; i++) {
        azi_sign_t s = azi_sign_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.sign);
        TEST_ASSERT_NOT_NULL(s.nahuatl);
        TEST_ASSERT_NOT_NULL(s.english);
        TEST_ASSERT_NOT_NULL(s.deity);
        TEST_ASSERT_NOT_NULL(s.direction);
        TEST_ASSERT_NOT_NULL(s.quality);
        TEST_ASSERT_NOT_NULL(s.brief);
    }
}

void test_sign_invalid(void) {
    azi_sign_t s = azi_sign_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.sign);
    TEST_ASSERT_EQUAL_STRING("?", s.nahuatl);
}

void test_bearer_all_4_valid(void) {
    for (int i = 0; i < 4; i++) {
        azi_bearer_t b = azi_bearer_data(i);
        TEST_ASSERT_EQUAL_INT(i, b.bearer);
        TEST_ASSERT_NOT_NULL(b.nahuatl);
        TEST_ASSERT_NOT_NULL(b.english);
        TEST_ASSERT_NOT_NULL(b.direction);
        TEST_ASSERT_NOT_NULL(b.quality);
    }
}

void test_bearer_invalid(void) {
    azi_bearer_t b = azi_bearer_data(4);
    TEST_ASSERT_EQUAL_INT(-1, b.bearer);
    TEST_ASSERT_EQUAL_STRING("?", b.nahuatl);
}

void test_interpret_glyph(void) {
    aztec_interp_t r = azi_interpret(13, 4, 1, 7);
    TEST_ASSERT_EQUAL_STRING("Oce", r.glyph);
}

void test_interpret_glance(void) {
    aztec_interp_t r = azi_interpret(13, 4, 1, 7);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "4 Jaguar"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "7 Reed"));
}

void test_interpret_detail_deity(void) {
    aztec_interp_t r = azi_interpret(4, 1, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Chalchiuhtlicue"));
}

void test_interpret_detail_year(void) {
    aztec_interp_t r = azi_interpret(0, 1, 2, 3);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Tecpatl"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Flint"));
}

void test_interpret_invalid(void) {
    aztec_interp_t r = azi_interpret(20, 1, 0, 1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_sign_count(void) { TEST_ASSERT_EQUAL_INT(20, azi_sign_count()); }
void test_bearer_count(void) { TEST_ASSERT_EQUAL_INT(4, azi_bearer_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sign_cipactli);
    RUN_TEST(test_sign_ocelotl);
    RUN_TEST(test_sign_xochitl);
    RUN_TEST(test_sign_all_20_valid);
    RUN_TEST(test_sign_invalid);
    RUN_TEST(test_bearer_all_4_valid);
    RUN_TEST(test_bearer_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance);
    RUN_TEST(test_interpret_detail_deity);
    RUN_TEST(test_interpret_detail_year);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_sign_count);
    RUN_TEST(test_bearer_count);
    return UNITY_END();
}
