#include "../../unity/unity.h"
#include "../../../src/systems/balinese/pawukon_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_wuku_sinta(void) {
    pwi_wuku_t w = pwi_wuku_data(0);
    TEST_ASSERT_EQUAL_INT(0, w.wuku);
    TEST_ASSERT_EQUAL_STRING("Sinta", w.name);
    TEST_ASSERT_EQUAL_STRING("Shiva", w.deity);
}

void test_wuku_watugunung(void) {
    pwi_wuku_t w = pwi_wuku_data(29);
    TEST_ASSERT_EQUAL_INT(29, w.wuku);
    TEST_ASSERT_EQUAL_STRING("Watugunung", w.name);
}

void test_wuku_all_30_valid(void) {
    for (int i = 0; i < 30; i++) {
        pwi_wuku_t w = pwi_wuku_data(i);
        TEST_ASSERT_EQUAL_INT(i, w.wuku);
        TEST_ASSERT_NOT_NULL(w.name);
        TEST_ASSERT_NOT_NULL(w.deity);
        TEST_ASSERT_NOT_NULL(w.quality);
        TEST_ASSERT_NOT_NULL(w.brief);
    }
}

void test_wuku_invalid(void) {
    pwi_wuku_t w = pwi_wuku_data(30);
    TEST_ASSERT_EQUAL_INT(-1, w.wuku);
    TEST_ASSERT_EQUAL_STRING("?", w.name);
}

void test_interpret_glyph(void) {
    pawukon_interp_t r = pwi_interpret(0, 3, 12, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Sin", r.glyph);
}

void test_interpret_kajeng_kliwon(void) {
    pawukon_interp_t r = pwi_interpret(5, 3, 15, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kajeng Kliwon"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "spirit world"));
}

void test_interpret_tumpek(void) {
    pawukon_interp_t r = pwi_interpret(10, 7, 18, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tumpek"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sacred Saturday"));
}

void test_interpret_normal(void) {
    pawukon_interp_t r = pwi_interpret(8, 2, 10, 0, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Julungwangi"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Urip"));
}

void test_interpret_invalid(void) {
    pawukon_interp_t r = pwi_interpret(-1, 1, 0, 0, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_wuku_count(void) { TEST_ASSERT_EQUAL_INT(30, pwi_wuku_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_wuku_sinta);
    RUN_TEST(test_wuku_watugunung);
    RUN_TEST(test_wuku_all_30_valid);
    RUN_TEST(test_wuku_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_kajeng_kliwon);
    RUN_TEST(test_interpret_tumpek);
    RUN_TEST(test_interpret_normal);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_wuku_count);
    return UNITY_END();
}
