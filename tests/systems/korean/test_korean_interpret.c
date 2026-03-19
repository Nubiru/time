#include "../../unity/unity.h"
#include "../../../src/systems/korean/korean_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * ki_festival_data — 7 festivals
 * ============================================================ */

void test_festival_seollal(void) {
    ki_festival_t f = ki_festival_data(0);
    TEST_ASSERT_EQUAL_INT(0, f.festival);
    TEST_ASSERT_EQUAL_STRING("Seollal", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "ancestors"));
}

void test_festival_chuseok(void) {
    ki_festival_t f = ki_festival_data(5);
    TEST_ASSERT_EQUAL_INT(5, f.festival);
    TEST_ASSERT_EQUAL_STRING("Chuseok", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "harvest"));
}

void test_festival_dano(void) {
    ki_festival_t f = ki_festival_data(3);
    TEST_ASSERT_EQUAL_INT(3, f.festival);
    TEST_ASSERT_EQUAL_STRING("Dano", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "yang"));
}

void test_festival_all_7_valid(void) {
    for (int i = 0; i < 7; i++) {
        ki_festival_t f = ki_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.korean);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
    }
}

void test_festival_invalid(void) {
    ki_festival_t f = ki_festival_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

void test_festival_invalid_7(void) {
    ki_festival_t f = ki_festival_data(7);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* ============================================================
 * ki_element_data — 5 elements
 * ============================================================ */

void test_element_wood(void) {
    ki_element_t e = ki_element_data(0);
    TEST_ASSERT_EQUAL_INT(0, e.element);
    TEST_ASSERT_EQUAL_STRING("Mok", e.korean);
    TEST_ASSERT_EQUAL_STRING("Spring", e.season);
}

void test_element_water(void) {
    ki_element_t e = ki_element_data(4);
    TEST_ASSERT_EQUAL_INT(4, e.element);
    TEST_ASSERT_EQUAL_STRING("Su", e.korean);
    TEST_ASSERT_EQUAL_STRING("Winter", e.season);
}

void test_element_all_5_valid(void) {
    for (int i = 0; i < 5; i++) {
        ki_element_t e = ki_element_data(i);
        TEST_ASSERT_EQUAL_INT(i, e.element);
        TEST_ASSERT_NOT_NULL(e.korean);
        TEST_ASSERT_NOT_NULL(e.meaning);
        TEST_ASSERT_NOT_NULL(e.season);
        TEST_ASSERT_NOT_NULL(e.brief);
    }
}

void test_element_invalid(void) {
    ki_element_t e = ki_element_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.element);
    TEST_ASSERT_EQUAL_STRING("?", e.korean);
}

/* ============================================================
 * ki_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_dragon(void) {
    korean_interp_t r = ki_interpret(4359, 4, 0, 0, -1);
    TEST_ASSERT_EQUAL_STRING("Yon", r.glyph);
}

void test_interpret_glance_no_festival(void) {
    korean_interp_t r = ki_interpret(4359, 4, 0, 0, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dangun 4359"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mok"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yang"));
}

void test_interpret_glance_with_festival(void) {
    korean_interp_t r = ki_interpret(4359, 4, 0, 0, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Seollal"));
}

void test_interpret_detail_contains_animal(void) {
    korean_interp_t r = ki_interpret(4359, 4, 0, 0, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Yong"));
}

void test_interpret_detail_contains_element(void) {
    korean_interp_t r = ki_interpret(4359, 4, 0, 0, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Mok"));
}

void test_interpret_detail_festival(void) {
    korean_interp_t r = ki_interpret(4359, 4, 0, 0, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Chuseok"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Songpyeon"));
}

void test_interpret_invalid_animal(void) {
    korean_interp_t r = ki_interpret(4359, 99, 0, 0, -1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_festival_count(void) {
    TEST_ASSERT_EQUAL_INT(7, ki_festival_count());
}

void test_element_count(void) {
    TEST_ASSERT_EQUAL_INT(5, ki_element_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_festival_seollal);
    RUN_TEST(test_festival_chuseok);
    RUN_TEST(test_festival_dano);
    RUN_TEST(test_festival_all_7_valid);
    RUN_TEST(test_festival_invalid);
    RUN_TEST(test_festival_invalid_7);

    RUN_TEST(test_element_wood);
    RUN_TEST(test_element_water);
    RUN_TEST(test_element_all_5_valid);
    RUN_TEST(test_element_invalid);

    RUN_TEST(test_interpret_glyph_dragon);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_glance_with_festival);
    RUN_TEST(test_interpret_detail_contains_animal);
    RUN_TEST(test_interpret_detail_contains_element);
    RUN_TEST(test_interpret_detail_festival);
    RUN_TEST(test_interpret_invalid_animal);

    RUN_TEST(test_festival_count);
    RUN_TEST(test_element_count);

    return UNITY_END();
}
