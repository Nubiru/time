#include "../../unity/unity.h"
#include "../../../src/systems/geology/geology_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_eon_hadean(void) {
    gli_eon_t e = gli_eon_data(0);
    TEST_ASSERT_EQUAL_INT(0, e.eon);
    TEST_ASSERT_EQUAL_STRING("Hadean", e.name);
    TEST_ASSERT_NOT_NULL(strstr(e.meaning, "Hades"));
}

void test_eon_phanerozoic(void) {
    gli_eon_t e = gli_eon_data(3);
    TEST_ASSERT_EQUAL_INT(3, e.eon);
    TEST_ASSERT_EQUAL_STRING("Phanerozoic", e.name);
    TEST_ASSERT_NOT_NULL(strstr(e.life, "Cambrian"));
}

void test_eon_all_4_valid(void) {
    for (int i = 0; i < 4; i++) {
        gli_eon_t e = gli_eon_data(i);
        TEST_ASSERT_EQUAL_INT(i, e.eon);
        TEST_ASSERT_NOT_NULL(e.name);
        TEST_ASSERT_NOT_NULL(e.meaning);
        TEST_ASSERT_NOT_NULL(e.character);
        TEST_ASSERT_NOT_NULL(e.life);
        TEST_ASSERT_NOT_NULL(e.brief);
    }
}

void test_eon_invalid(void) {
    gli_eon_t e = gli_eon_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.eon);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
}

void test_extinction_great_dying(void) {
    gli_extinction_t x = gli_extinction_data(2);
    TEST_ASSERT_EQUAL_INT(2, x.extinction);
    TEST_ASSERT_EQUAL_STRING("End-Permian", x.name);
    TEST_ASSERT_NOT_NULL(strstr(x.cause, "Siberian"));
}

void test_extinction_chicxulub(void) {
    gli_extinction_t x = gli_extinction_data(4);
    TEST_ASSERT_EQUAL_INT(4, x.extinction);
    TEST_ASSERT_EQUAL_STRING("End-Cretaceous", x.name);
    TEST_ASSERT_NOT_NULL(strstr(x.cause, "asteroid"));
}

void test_extinction_all_5_valid(void) {
    for (int i = 0; i < 5; i++) {
        gli_extinction_t x = gli_extinction_data(i);
        TEST_ASSERT_EQUAL_INT(i, x.extinction);
        TEST_ASSERT_NOT_NULL(x.name);
        TEST_ASSERT_NOT_NULL(x.cause);
        TEST_ASSERT_NOT_NULL(x.aftermath);
    }
}

void test_extinction_invalid(void) {
    gli_extinction_t x = gli_extinction_data(5);
    TEST_ASSERT_EQUAL_INT(-1, x.extinction);
    TEST_ASSERT_EQUAL_STRING("?", x.name);
}

void test_supercontinent_pangaea(void) {
    gli_supercontinent_t s = gli_supercontinent_data(5);
    TEST_ASSERT_EQUAL_INT(5, s.index);
    TEST_ASSERT_EQUAL_STRING("Pangaea", s.name);
}

void test_supercontinent_all_7_valid(void) {
    for (int i = 0; i < 7; i++) {
        gli_supercontinent_t s = gli_supercontinent_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.index);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.age);
        TEST_ASSERT_NOT_NULL(s.significance);
    }
}

void test_supercontinent_invalid(void) {
    gli_supercontinent_t s = gli_supercontinent_data(7);
    TEST_ASSERT_EQUAL_INT(-1, s.index);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

void test_interpret_glyph(void) {
    geology_interp_t r = gli_interpret(3, -1, -1);
    TEST_ASSERT_EQUAL_STRING("Pha", r.glyph);
}

void test_interpret_glance(void) {
    geology_interp_t r = gli_interpret(3, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Phanerozoic"));
}

void test_interpret_detail_eon(void) {
    geology_interp_t r = gli_interpret(3, -1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Phanerozoic"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Cambrian"));
}

void test_interpret_detail_extinction(void) {
    geology_interp_t r = gli_interpret(3, 4, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "End-Cretaceous"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "asteroid"));
}

void test_interpret_detail_supercontinent(void) {
    geology_interp_t r = gli_interpret(3, -1, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Pangaea"));
}

void test_interpret_invalid(void) {
    geology_interp_t r = gli_interpret(-1, -1, -1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_eon_count(void) { TEST_ASSERT_EQUAL_INT(4, gli_eon_count()); }
void test_extinction_count(void) { TEST_ASSERT_EQUAL_INT(5, gli_extinction_count()); }
void test_supercontinent_count(void) { TEST_ASSERT_EQUAL_INT(7, gli_supercontinent_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_eon_hadean);
    RUN_TEST(test_eon_phanerozoic);
    RUN_TEST(test_eon_all_4_valid);
    RUN_TEST(test_eon_invalid);
    RUN_TEST(test_extinction_great_dying);
    RUN_TEST(test_extinction_chicxulub);
    RUN_TEST(test_extinction_all_5_valid);
    RUN_TEST(test_extinction_invalid);
    RUN_TEST(test_supercontinent_pangaea);
    RUN_TEST(test_supercontinent_all_7_valid);
    RUN_TEST(test_supercontinent_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance);
    RUN_TEST(test_interpret_detail_eon);
    RUN_TEST(test_interpret_detail_extinction);
    RUN_TEST(test_interpret_detail_supercontinent);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_eon_count);
    RUN_TEST(test_extinction_count);
    RUN_TEST(test_supercontinent_count);
    return UNITY_END();
}
