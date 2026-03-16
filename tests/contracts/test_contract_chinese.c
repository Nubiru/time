/* test_contract_chinese.c — Exhaustive contract tests for chinese.h */

#include "../unity/unity.h"
#include "../../src/systems/chinese/chinese.h"
#include "../../src/math/julian.h"
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

void test_stem_range_sweep(void) {
    int f = 0;
    for (int y = 1900; y <= 2100; y++) { if (chinese_year_from_gregorian(y).stem < 0 || chinese_year_from_gregorian(y).stem > 9) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Stem out of [0,9]");
}

void test_branch_range_sweep(void) {
    int f = 0;
    for (int y = 1900; y <= 2100; y++) { if (chinese_year_from_gregorian(y).branch < 0 || chinese_year_from_gregorian(y).branch > 11) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Branch out of [0,11]");
}

void test_element_range_sweep(void) {
    int f = 0;
    for (int y = 1900; y <= 2100; y++) { if (chinese_year_from_gregorian(y).element < 0 || chinese_year_from_gregorian(y).element > 4) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Element out of [0,4]");
}

void test_polarity_range_sweep(void) {
    int f = 0;
    for (int y = 1900; y <= 2100; y++) { if (chinese_year_from_gregorian(y).polarity < 0 || chinese_year_from_gregorian(y).polarity > 1) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Polarity out of [0,1]");
}

void test_cycle_year_range_sweep(void) {
    int f = 0;
    for (int y = 1900; y <= 2100; y++) { if (chinese_year_from_gregorian(y).cycle_year < 1 || chinese_year_from_gregorian(y).cycle_year > 60) f++; }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Cycle year out of [1,60]");
}

void test_60_year_cycle(void) {
    int f = 0;
    for (int y = 1900; y <= 2040; y++) {
        chinese_year_t a = chinese_year_from_gregorian(y);
        chinese_year_t b = chinese_year_from_gregorian(y + 60);
        if (a.cycle_year != b.cycle_year || a.stem != b.stem || a.branch != b.branch) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "60-year cycle did not repeat");
}

void test_anchor_2024_dragon(void) {
    TEST_ASSERT_EQUAL_INT(4, chinese_year_from_gregorian(2024).branch);
}

void test_animal_equals_branch(void) {
    int f = 0;
    for (int y = 1900; y <= 2100; y++) {
        chinese_year_t cy = chinese_year_from_gregorian(y);
        if (cy.animal != cy.branch) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "Animal should equal branch");
}

void test_stem_name_valid(void) {
    for (int i = 0; i < 10; i++) { const char *n = chinese_stem_name(i); TEST_ASSERT_NOT_NULL(n); TEST_ASSERT_TRUE(n[0] != '?'); }
}

void test_stem_name_invalid(void) {
    TEST_ASSERT_NOT_NULL(chinese_stem_name(-1));
    TEST_ASSERT_NOT_NULL(chinese_stem_name(10));
}

void test_branch_name_valid(void) {
    for (int i = 0; i < 12; i++) { const char *n = chinese_branch_name(i); TEST_ASSERT_NOT_NULL(n); TEST_ASSERT_TRUE(n[0] != '?'); }
}

void test_branch_name_invalid(void) {
    TEST_ASSERT_NOT_NULL(chinese_branch_name(-1));
    TEST_ASSERT_NOT_NULL(chinese_branch_name(12));
}

void test_animal_name_valid(void) {
    for (int i = 0; i < 12; i++) { const char *n = chinese_animal_name(i); TEST_ASSERT_NOT_NULL(n); TEST_ASSERT_TRUE(n[0] != '?'); }
}

void test_element_name_valid(void) {
    for (int i = 0; i < 5; i++) { const char *n = chinese_element_name(i); TEST_ASSERT_NOT_NULL(n); TEST_ASSERT_TRUE(n[0] != '?'); }
}

void test_polarity_name_valid(void) {
    for (int i = 0; i < 2; i++) { const char *n = chinese_polarity_name(i); TEST_ASSERT_NOT_NULL(n); TEST_ASSERT_TRUE(n[0] != '?'); }
}

void test_animal_symbol_valid(void) {
    for (int i = 0; i < 12; i++) TEST_ASSERT_NOT_NULL(chinese_animal_symbol(i));
}

void test_from_jd_matches_from_gregorian(void) {
    int f = 0;
    for (int y = 1950; y <= 2050; y++) {
        chinese_year_t a = chinese_year_from_jd(gregorian_to_jd(y, 7, 1.0));
        chinese_year_t b = chinese_year_from_gregorian(y);
        if (a.stem != b.stem || a.branch != b.branch || a.cycle_year != b.cycle_year) f++;
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, f, "from_jd and from_gregorian disagree");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stem_range_sweep);
    RUN_TEST(test_branch_range_sweep);
    RUN_TEST(test_element_range_sweep);
    RUN_TEST(test_polarity_range_sweep);
    RUN_TEST(test_cycle_year_range_sweep);
    RUN_TEST(test_60_year_cycle);
    RUN_TEST(test_anchor_2024_dragon);
    RUN_TEST(test_animal_equals_branch);
    RUN_TEST(test_stem_name_valid);
    RUN_TEST(test_stem_name_invalid);
    RUN_TEST(test_branch_name_valid);
    RUN_TEST(test_branch_name_invalid);
    RUN_TEST(test_animal_name_valid);
    RUN_TEST(test_element_name_valid);
    RUN_TEST(test_polarity_name_valid);
    RUN_TEST(test_animal_symbol_valid);
    RUN_TEST(test_from_jd_matches_from_gregorian);
    return UNITY_END();
}
