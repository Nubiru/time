#include "../../unity/unity.h"
#include "../../../src/systems/chakra/chakra_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_root(void) {
    chi_chakra_t c = chi_chakra_data(0);
    TEST_ASSERT_EQUAL_INT(0, c.chakra);
    TEST_ASSERT_EQUAL_STRING("Muladhara", c.sanskrit);
    TEST_ASSERT_EQUAL_STRING("Root", c.english);
    TEST_ASSERT_NOT_NULL(strstr(c.theme, "Grounding"));
}

void test_heart(void) {
    chi_chakra_t c = chi_chakra_data(3);
    TEST_ASSERT_EQUAL_INT(3, c.chakra);
    TEST_ASSERT_EQUAL_STRING("Anahata", c.sanskrit);
    TEST_ASSERT_EQUAL_STRING("Heart", c.english);
    TEST_ASSERT_NOT_NULL(strstr(c.theme, "love"));
}

void test_crown(void) {
    chi_chakra_t c = chi_chakra_data(6);
    TEST_ASSERT_EQUAL_INT(6, c.chakra);
    TEST_ASSERT_EQUAL_STRING("Sahasrara", c.sanskrit);
    TEST_ASSERT_EQUAL_STRING("Crown", c.english);
}

void test_all_7_valid(void) {
    for (int i = 0; i < 7; i++) {
        chi_chakra_t c = chi_chakra_data(i);
        TEST_ASSERT_EQUAL_INT(i, c.chakra);
        TEST_ASSERT_NOT_NULL(c.sanskrit);
        TEST_ASSERT_NOT_NULL(c.english);
        TEST_ASSERT_NOT_NULL(c.archetype);
        TEST_ASSERT_NOT_NULL(c.theme);
        TEST_ASSERT_NOT_NULL(c.shadow);
        TEST_ASSERT_NOT_NULL(c.question);
        TEST_ASSERT_NOT_NULL(c.brief);
    }
}

void test_invalid(void) {
    chi_chakra_t c = chi_chakra_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, c.chakra);
    TEST_ASSERT_EQUAL_STRING("?", c.sanskrit);
}

void test_interpret_glyph(void) {
    chakra_interp_t r = chi_interpret(0);
    TEST_ASSERT_EQUAL_STRING("LAM", r.glyph);
}

void test_interpret_glyph_heart(void) {
    chakra_interp_t r = chi_interpret(3);
    TEST_ASSERT_EQUAL_STRING("YAM", r.glyph);
}

void test_interpret_glance(void) {
    chakra_interp_t r = chi_interpret(3);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Anahata"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Heart"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "love"));
}

void test_interpret_detail(void) {
    chakra_interp_t r = chi_interpret(2);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Manipura"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Warrior"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Shame"));
}

void test_interpret_invalid(void) {
    chakra_interp_t r = chi_interpret(7);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_count(void) { TEST_ASSERT_EQUAL_INT(7, chi_chakra_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_root);
    RUN_TEST(test_heart);
    RUN_TEST(test_crown);
    RUN_TEST(test_all_7_valid);
    RUN_TEST(test_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glyph_heart);
    RUN_TEST(test_interpret_glance);
    RUN_TEST(test_interpret_detail);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_count);
    return UNITY_END();
}
