#include "../../unity/unity.h"
#include "../../../src/systems/chinese/chinese_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * ci_animal_data — archetype data for 12 animals
 * ============================================================ */

void test_animal_data_rat(void) {
    ci_animal_t a = ci_animal_data(0);
    TEST_ASSERT_EQUAL_INT(0, a.animal);
    TEST_ASSERT_EQUAL_STRING("Rat", a.name);
    TEST_ASSERT_EQUAL_STRING("The Strategist", a.archetype);
    TEST_ASSERT_EQUAL_STRING("Resourceful, quick-witted, adaptable", a.strengths);
    TEST_ASSERT_EQUAL_STRING("Cunning, restless, manipulative", a.challenges);
    TEST_ASSERT_EQUAL_STRING("Dragon, Monkey, Ox", a.compatibility);
}

void test_animal_data_dragon(void) {
    ci_animal_t a = ci_animal_data(4);
    TEST_ASSERT_EQUAL_INT(4, a.animal);
    TEST_ASSERT_EQUAL_STRING("Dragon", a.name);
    TEST_ASSERT_EQUAL_STRING("The Emperor", a.archetype);
    TEST_ASSERT_EQUAL_STRING("Charismatic, ambitious, visionary", a.strengths);
    TEST_ASSERT_EQUAL_STRING("Rat, Monkey, Rooster", a.compatibility);
}

void test_animal_data_dog(void) {
    ci_animal_t a = ci_animal_data(10);
    TEST_ASSERT_EQUAL_INT(10, a.animal);
    TEST_ASSERT_EQUAL_STRING("Dog", a.name);
    TEST_ASSERT_EQUAL_STRING("The Guardian", a.archetype);
    TEST_ASSERT_EQUAL_STRING("Loyal, honest, protective", a.strengths);
    TEST_ASSERT_EQUAL_STRING("Anxious, stubborn, judgmental", a.challenges);
    TEST_ASSERT_EQUAL_STRING("Tiger, Rabbit, Horse", a.compatibility);
}

void test_animal_data_pig(void) {
    ci_animal_t a = ci_animal_data(11);
    TEST_ASSERT_EQUAL_INT(11, a.animal);
    TEST_ASSERT_EQUAL_STRING("Pig", a.name);
    TEST_ASSERT_EQUAL_STRING("The Generous", a.archetype);
    TEST_ASSERT_EQUAL_STRING("Kind, generous, optimistic", a.strengths);
    TEST_ASSERT_EQUAL_STRING("Naive, indulgent, gullible", a.challenges);
    TEST_ASSERT_EQUAL_STRING("Rabbit, Goat, Tiger", a.compatibility);
}

void test_animal_data_all_12_valid(void) {
    for (int i = 0; i < 12; i++) {
        ci_animal_t a = ci_animal_data(i);
        TEST_ASSERT_EQUAL_INT(i, a.animal);
        TEST_ASSERT_NOT_NULL(a.name);
        TEST_ASSERT_NOT_NULL(a.archetype);
        TEST_ASSERT_NOT_NULL(a.strengths);
        TEST_ASSERT_NOT_NULL(a.challenges);
        TEST_ASSERT_NOT_NULL(a.compatibility);
        TEST_ASSERT_NOT_NULL(a.brief);
        TEST_ASSERT_TRUE(strlen(a.name) > 0);
        TEST_ASSERT_TRUE(strlen(a.archetype) > 0);
        TEST_ASSERT_TRUE(strlen(a.strengths) > 0);
        TEST_ASSERT_TRUE(strlen(a.challenges) > 0);
        TEST_ASSERT_TRUE(strlen(a.compatibility) > 0);
        TEST_ASSERT_TRUE(strlen(a.brief) > 0);
    }
}

void test_animal_data_invalid_neg1(void) {
    ci_animal_t a = ci_animal_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, a.animal);
    TEST_ASSERT_EQUAL_STRING("?", a.name);
    TEST_ASSERT_EQUAL_STRING("?", a.archetype);
    TEST_ASSERT_EQUAL_STRING("?", a.strengths);
    TEST_ASSERT_EQUAL_STRING("?", a.challenges);
    TEST_ASSERT_EQUAL_STRING("?", a.compatibility);
    TEST_ASSERT_EQUAL_STRING("?", a.brief);
}

void test_animal_data_invalid_12(void) {
    ci_animal_t a = ci_animal_data(12);
    TEST_ASSERT_EQUAL_INT(-1, a.animal);
    TEST_ASSERT_EQUAL_STRING("?", a.name);
}

void test_rat_is_first(void) {
    ci_animal_t a = ci_animal_data(0);
    TEST_ASSERT_EQUAL_STRING("Rat", a.name);
}

void test_pig_is_last(void) {
    ci_animal_t a = ci_animal_data(11);
    TEST_ASSERT_EQUAL_STRING("Pig", a.name);
}

void test_dragon_archetype_mentions_mythical(void) {
    ci_animal_t a = ci_animal_data(4);
    TEST_ASSERT_NOT_NULL(strstr(a.brief, "mythical"));
}

/* ============================================================
 * ci_element_data — interpretation data for 5 elements
 * ============================================================ */

void test_element_data_all_5_valid(void) {
    for (int i = 0; i < 5; i++) {
        ci_element_t e = ci_element_data(i);
        TEST_ASSERT_EQUAL_INT(i, e.element);
        TEST_ASSERT_NOT_NULL(e.name);
        TEST_ASSERT_NOT_NULL(e.nature);
        TEST_ASSERT_NOT_NULL(e.season);
        TEST_ASSERT_NOT_NULL(e.expression);
        TEST_ASSERT_TRUE(strlen(e.name) > 0);
        TEST_ASSERT_TRUE(strlen(e.nature) > 0);
        TEST_ASSERT_TRUE(strlen(e.season) > 0);
        TEST_ASSERT_TRUE(strlen(e.expression) > 0);
    }
}

void test_element_data_wood(void) {
    ci_element_t e = ci_element_data(0);
    TEST_ASSERT_EQUAL_STRING("Wood", e.name);
    TEST_ASSERT_EQUAL_STRING("Growth, expansion, benevolence", e.nature);
    TEST_ASSERT_EQUAL_STRING("Spring", e.season);
    TEST_ASSERT_EQUAL_STRING("Through creativity, vision, and upward movement",
                             e.expression);
}

void test_element_data_fire(void) {
    ci_element_t e = ci_element_data(1);
    TEST_ASSERT_EQUAL_STRING("Fire", e.name);
    TEST_ASSERT_EQUAL_STRING("Passion, transformation, brilliance", e.nature);
    TEST_ASSERT_EQUAL_STRING("Summer", e.season);
}

void test_element_data_earth(void) {
    ci_element_t e = ci_element_data(2);
    TEST_ASSERT_EQUAL_STRING("Earth", e.name);
    TEST_ASSERT_EQUAL_STRING("Stability, nourishment, centering", e.nature);
    TEST_ASSERT_EQUAL_STRING("Late Summer", e.season);
}

void test_element_data_metal(void) {
    ci_element_t e = ci_element_data(3);
    TEST_ASSERT_EQUAL_STRING("Metal", e.name);
    TEST_ASSERT_EQUAL_STRING("Precision, clarity, righteousness", e.nature);
    TEST_ASSERT_EQUAL_STRING("Autumn", e.season);
}

void test_element_data_water(void) {
    ci_element_t e = ci_element_data(4);
    TEST_ASSERT_EQUAL_STRING("Water", e.name);
    TEST_ASSERT_EQUAL_STRING("Wisdom, depth, adaptability", e.nature);
    TEST_ASSERT_EQUAL_STRING("Winter", e.season);
}

void test_element_data_invalid_5(void) {
    ci_element_t e = ci_element_data(5);
    TEST_ASSERT_EQUAL_INT(-1, e.element);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
    TEST_ASSERT_EQUAL_STRING("?", e.nature);
    TEST_ASSERT_EQUAL_STRING("?", e.season);
    TEST_ASSERT_EQUAL_STRING("?", e.expression);
}

void test_element_data_invalid_neg1(void) {
    ci_element_t e = ci_element_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.element);
    TEST_ASSERT_EQUAL_STRING("?", e.name);
}

/* ============================================================
 * ci_interpret — tiered interpretation composition
 * ============================================================ */

void test_interpret_water_tiger_glance(void) {
    chinese_interp_t r = ci_interpret(2, 4, 0, 39);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Water"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tiger"));
}

void test_interpret_metal_rat_glance(void) {
    chinese_interp_t r = ci_interpret(0, 3, 0, 37);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Metal"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rat"));
}

void test_interpret_detail_contains_archetype(void) {
    chinese_interp_t r = ci_interpret(2, 4, 0, 39);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "The Brave"));
}

void test_interpret_detail_contains_strengths(void) {
    chinese_interp_t r = ci_interpret(2, 4, 0, 39);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Courageous, passionate, confident"));
}

void test_interpret_detail_contains_element_nature(void) {
    chinese_interp_t r = ci_interpret(2, 4, 0, 39);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Wisdom, depth, adaptability"));
}

void test_interpret_detail_contains_cycle_year(void) {
    chinese_interp_t r = ci_interpret(2, 4, 0, 39);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "39 of 60"));
}

void test_interpret_yang_polarity(void) {
    chinese_interp_t r = ci_interpret(0, 0, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yang"));
}

void test_interpret_yin_polarity(void) {
    chinese_interp_t r = ci_interpret(1, 1, 1, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yin"));
}

void test_interpret_glyph_rat(void) {
    chinese_interp_t r = ci_interpret(0, 0, 0, 1);
    TEST_ASSERT_EQUAL_STRING("Rat", r.glyph);
}

void test_interpret_glyph_tiger(void) {
    chinese_interp_t r = ci_interpret(2, 0, 0, 1);
    TEST_ASSERT_EQUAL_STRING("Tig", r.glyph);
}

void test_interpret_glyph_dragon(void) {
    chinese_interp_t r = ci_interpret(4, 0, 0, 1);
    TEST_ASSERT_EQUAL_STRING("Dra", r.glyph);
}

void test_interpret_invalid_animal(void) {
    chinese_interp_t r = ci_interpret(99, 0, 0, 1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

void test_interpret_invalid_element(void) {
    chinese_interp_t r = ci_interpret(0, 99, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ============================================================
 * ci_animal_count / ci_element_count
 * ============================================================ */

void test_animal_count(void) {
    TEST_ASSERT_EQUAL_INT(12, ci_animal_count());
}

void test_element_count(void) {
    TEST_ASSERT_EQUAL_INT(5, ci_element_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* ci_animal_data */
    RUN_TEST(test_animal_data_rat);
    RUN_TEST(test_animal_data_dragon);
    RUN_TEST(test_animal_data_dog);
    RUN_TEST(test_animal_data_pig);
    RUN_TEST(test_animal_data_all_12_valid);
    RUN_TEST(test_animal_data_invalid_neg1);
    RUN_TEST(test_animal_data_invalid_12);
    RUN_TEST(test_rat_is_first);
    RUN_TEST(test_pig_is_last);
    RUN_TEST(test_dragon_archetype_mentions_mythical);

    /* ci_element_data */
    RUN_TEST(test_element_data_all_5_valid);
    RUN_TEST(test_element_data_wood);
    RUN_TEST(test_element_data_fire);
    RUN_TEST(test_element_data_earth);
    RUN_TEST(test_element_data_metal);
    RUN_TEST(test_element_data_water);
    RUN_TEST(test_element_data_invalid_5);
    RUN_TEST(test_element_data_invalid_neg1);

    /* ci_interpret */
    RUN_TEST(test_interpret_water_tiger_glance);
    RUN_TEST(test_interpret_metal_rat_glance);
    RUN_TEST(test_interpret_detail_contains_archetype);
    RUN_TEST(test_interpret_detail_contains_strengths);
    RUN_TEST(test_interpret_detail_contains_element_nature);
    RUN_TEST(test_interpret_detail_contains_cycle_year);
    RUN_TEST(test_interpret_yang_polarity);
    RUN_TEST(test_interpret_yin_polarity);
    RUN_TEST(test_interpret_glyph_rat);
    RUN_TEST(test_interpret_glyph_tiger);
    RUN_TEST(test_interpret_glyph_dragon);
    RUN_TEST(test_interpret_invalid_animal);
    RUN_TEST(test_interpret_invalid_element);

    /* counts */
    RUN_TEST(test_animal_count);
    RUN_TEST(test_element_count);

    return UNITY_END();
}
