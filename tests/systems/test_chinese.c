#include "../unity/unity.h"
#include "../../src/systems/chinese/chinese.h"
#include "../../src/math/julian.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Known years ===== */

void test_2024_wood_dragon(void) {
    chinese_year_t cy = chinese_year_from_gregorian(2024);
    TEST_ASSERT_EQUAL_INT(0, cy.stem);       /* Jia */
    TEST_ASSERT_EQUAL_INT(4, cy.branch);     /* Chen */
    TEST_ASSERT_EQUAL_INT(4, cy.animal);     /* Dragon */
    TEST_ASSERT_EQUAL_INT(0, cy.element);    /* Wood */
    TEST_ASSERT_EQUAL_INT(0, cy.polarity);   /* Yang */
    TEST_ASSERT_EQUAL_INT(41, cy.cycle_year);
}

void test_2025_wood_snake(void) {
    chinese_year_t cy = chinese_year_from_gregorian(2025);
    TEST_ASSERT_EQUAL_INT(1, cy.stem);       /* Yi */
    TEST_ASSERT_EQUAL_INT(5, cy.branch);     /* Si */
    TEST_ASSERT_EQUAL_INT(5, cy.animal);     /* Snake */
    TEST_ASSERT_EQUAL_INT(0, cy.element);    /* Wood */
    TEST_ASSERT_EQUAL_INT(1, cy.polarity);   /* Yin */
    TEST_ASSERT_EQUAL_INT(42, cy.cycle_year);
}

void test_2026_fire_horse(void) {
    chinese_year_t cy = chinese_year_from_gregorian(2026);
    TEST_ASSERT_EQUAL_INT(2, cy.stem);       /* Bing */
    TEST_ASSERT_EQUAL_INT(6, cy.branch);     /* Wu */
    TEST_ASSERT_EQUAL_INT(6, cy.animal);     /* Horse */
    TEST_ASSERT_EQUAL_INT(1, cy.element);    /* Fire */
    TEST_ASSERT_EQUAL_INT(0, cy.polarity);   /* Yang */
    TEST_ASSERT_EQUAL_INT(43, cy.cycle_year);
}

void test_2000_metal_dragon(void) {
    chinese_year_t cy = chinese_year_from_gregorian(2000);
    TEST_ASSERT_EQUAL_INT(6, cy.stem);       /* Geng */
    TEST_ASSERT_EQUAL_INT(4, cy.branch);     /* Chen */
    TEST_ASSERT_EQUAL_INT(4, cy.animal);     /* Dragon */
    TEST_ASSERT_EQUAL_INT(3, cy.element);    /* Metal */
    TEST_ASSERT_EQUAL_INT(0, cy.polarity);   /* Yang */
    TEST_ASSERT_EQUAL_INT(17, cy.cycle_year);
}

void test_1984_wood_rat_cycle_start(void) {
    chinese_year_t cy = chinese_year_from_gregorian(1984);
    TEST_ASSERT_EQUAL_INT(0, cy.stem);       /* Jia */
    TEST_ASSERT_EQUAL_INT(0, cy.branch);     /* Zi */
    TEST_ASSERT_EQUAL_INT(0, cy.animal);     /* Rat */
    TEST_ASSERT_EQUAL_INT(0, cy.element);    /* Wood */
    TEST_ASSERT_EQUAL_INT(0, cy.polarity);   /* Yang */
    TEST_ASSERT_EQUAL_INT(1, cy.cycle_year);
}

void test_1900_metal_rat(void) {
    chinese_year_t cy = chinese_year_from_gregorian(1900);
    TEST_ASSERT_EQUAL_INT(6, cy.stem);       /* Geng */
    TEST_ASSERT_EQUAL_INT(0, cy.branch);     /* Zi */
    TEST_ASSERT_EQUAL_INT(0, cy.animal);     /* Rat */
    TEST_ASSERT_EQUAL_INT(3, cy.element);    /* Metal */
    TEST_ASSERT_EQUAL_INT(0, cy.polarity);   /* Yang */
    TEST_ASSERT_EQUAL_INT(37, cy.cycle_year);
}

/* ===== Cycle tests ===== */

void test_cycle_year_range(void) {
    for (int y = 1900; y <= 2100; y++) {
        chinese_year_t cy = chinese_year_from_gregorian(y);
        TEST_ASSERT_TRUE(cy.cycle_year >= 1 && cy.cycle_year <= 60);
    }
}

void test_60_year_cycle_repeats(void) {
    chinese_year_t a = chinese_year_from_gregorian(1984);
    chinese_year_t b = chinese_year_from_gregorian(1984 + 60);
    TEST_ASSERT_EQUAL_INT(a.stem, b.stem);
    TEST_ASSERT_EQUAL_INT(a.branch, b.branch);
    TEST_ASSERT_EQUAL_INT(a.cycle_year, b.cycle_year);
}

void test_12_year_animal_repeats(void) {
    chinese_year_t a = chinese_year_from_gregorian(2000);
    chinese_year_t b = chinese_year_from_gregorian(2012);
    TEST_ASSERT_EQUAL_INT(a.animal, b.animal);
}

void test_10_year_stem_repeats(void) {
    chinese_year_t a = chinese_year_from_gregorian(2000);
    chinese_year_t b = chinese_year_from_gregorian(2010);
    TEST_ASSERT_EQUAL_INT(a.stem, b.stem);
}

void test_stem_branch_ranges(void) {
    for (int y = 1800; y <= 2100; y++) {
        chinese_year_t cy = chinese_year_from_gregorian(y);
        TEST_ASSERT_TRUE(cy.stem >= 0 && cy.stem <= 9);
        TEST_ASSERT_TRUE(cy.branch >= 0 && cy.branch <= 11);
    }
}

/* ===== Name tests ===== */

void test_stem_name_jia(void) {
    TEST_ASSERT_EQUAL_STRING("Jia", chinese_stem_name(0));
}

void test_animal_name_rat(void) {
    TEST_ASSERT_EQUAL_STRING("Rat", chinese_animal_name(0));
}

void test_element_name_wood(void) {
    TEST_ASSERT_EQUAL_STRING("Wood", chinese_element_name(0));
}

void test_polarity_name_yang(void) {
    TEST_ASSERT_EQUAL_STRING("Yang", chinese_polarity_name(0));
}

void test_invalid_inputs(void) {
    TEST_ASSERT_EQUAL_STRING("?", chinese_stem_name(10));
    TEST_ASSERT_EQUAL_STRING("?", chinese_stem_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", chinese_branch_name(12));
    TEST_ASSERT_EQUAL_STRING("?", chinese_animal_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", chinese_element_name(5));
    TEST_ASSERT_EQUAL_STRING("?", chinese_polarity_name(2));
    TEST_ASSERT_EQUAL_STRING("?", chinese_animal_symbol(-1));
}

/* ===== JD test ===== */

void test_from_jd_j2000(void) {
    /* JD 2451545.0 = 2000-01-01 noon = Metal Dragon */
    chinese_year_t cy = chinese_year_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(6, cy.stem);    /* Geng */
    TEST_ASSERT_EQUAL_INT(4, cy.animal);  /* Dragon */
    TEST_ASSERT_EQUAL_INT(3, cy.element); /* Metal */
}

/* ===== Purity ===== */

void test_purity(void) {
    chinese_year_t a = chinese_year_from_gregorian(2024);
    chinese_year_t b = chinese_year_from_gregorian(2024);
    TEST_ASSERT_EQUAL_INT(a.stem, b.stem);
    TEST_ASSERT_EQUAL_INT(a.branch, b.branch);
    TEST_ASSERT_EQUAL_INT(a.animal, b.animal);
    TEST_ASSERT_EQUAL_INT(a.element, b.element);
    TEST_ASSERT_EQUAL_INT(a.polarity, b.polarity);
    TEST_ASSERT_EQUAL_INT(a.cycle_year, b.cycle_year);
}

int main(void) {
    UNITY_BEGIN();
    /* Known years */
    RUN_TEST(test_2024_wood_dragon);
    RUN_TEST(test_2025_wood_snake);
    RUN_TEST(test_2026_fire_horse);
    RUN_TEST(test_2000_metal_dragon);
    RUN_TEST(test_1984_wood_rat_cycle_start);
    RUN_TEST(test_1900_metal_rat);
    /* Cycle tests */
    RUN_TEST(test_cycle_year_range);
    RUN_TEST(test_60_year_cycle_repeats);
    RUN_TEST(test_12_year_animal_repeats);
    RUN_TEST(test_10_year_stem_repeats);
    RUN_TEST(test_stem_branch_ranges);
    /* Name tests */
    RUN_TEST(test_stem_name_jia);
    RUN_TEST(test_animal_name_rat);
    RUN_TEST(test_element_name_wood);
    RUN_TEST(test_polarity_name_yang);
    RUN_TEST(test_invalid_inputs);
    /* JD */
    RUN_TEST(test_from_jd_j2000);
    /* Purity */
    RUN_TEST(test_purity);
    return UNITY_END();
}
