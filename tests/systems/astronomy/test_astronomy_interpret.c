#include "../../unity/unity.h"
#include "../../../src/systems/astronomy/astronomy_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_planet_mercury(void) {
    ai_planet_t p = ai_planet_data(0);
    TEST_ASSERT_EQUAL_INT(0, p.planet);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.name);
    TEST_ASSERT_NOT_NULL(strstr(p.deity, "Hermes"));
}

void test_planet_saturn(void) {
    ai_planet_t p = ai_planet_data(5);
    TEST_ASSERT_EQUAL_INT(5, p.planet);
    TEST_ASSERT_EQUAL_STRING("Saturn", p.name);
    TEST_ASSERT_NOT_NULL(strstr(p.archetype, "Elder"));
}

void test_planet_neptune(void) {
    ai_planet_t p = ai_planet_data(7);
    TEST_ASSERT_EQUAL_INT(7, p.planet);
    TEST_ASSERT_EQUAL_STRING("Neptune", p.name);
}

void test_planet_all_8_valid(void) {
    for (int i = 0; i < 8; i++) {
        ai_planet_t p = ai_planet_data(i);
        TEST_ASSERT_EQUAL_INT(i, p.planet);
        TEST_ASSERT_NOT_NULL(p.name);
        TEST_ASSERT_NOT_NULL(p.deity);
        TEST_ASSERT_NOT_NULL(p.domain);
        TEST_ASSERT_NOT_NULL(p.archetype);
        TEST_ASSERT_NOT_NULL(p.brief);
    }
}

void test_planet_invalid(void) {
    ai_planet_t p = ai_planet_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, p.planet);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
}

void test_phase_new(void) {
    ai_phase_t ph = ai_phase_data(0);
    TEST_ASSERT_EQUAL_INT(0, ph.phase);
    TEST_ASSERT_EQUAL_STRING("New Moon", ph.name);
    TEST_ASSERT_NOT_NULL(strstr(ph.theme, "Beginnings"));
}

void test_phase_full(void) {
    ai_phase_t ph = ai_phase_data(4);
    TEST_ASSERT_EQUAL_INT(4, ph.phase);
    TEST_ASSERT_EQUAL_STRING("Full Moon", ph.name);
}

void test_phase_all_8_valid(void) {
    for (int i = 0; i < 8; i++) {
        ai_phase_t ph = ai_phase_data(i);
        TEST_ASSERT_EQUAL_INT(i, ph.phase);
        TEST_ASSERT_NOT_NULL(ph.name);
        TEST_ASSERT_NOT_NULL(ph.theme);
        TEST_ASSERT_NOT_NULL(ph.guidance);
        TEST_ASSERT_NOT_NULL(ph.brief);
    }
}

void test_phase_invalid(void) {
    ai_phase_t ph = ai_phase_data(8);
    TEST_ASSERT_EQUAL_INT(-1, ph.phase);
    TEST_ASSERT_EQUAL_STRING("?", ph.name);
}

void test_interpret_glyph(void) {
    astronomy_interp_t r = ai_astro_interpret(3, 0, 4);
    TEST_ASSERT_EQUAL_STRING("Mar", r.glyph);
}

void test_interpret_glance_retrograde(void) {
    astronomy_interp_t r = ai_astro_interpret(3, 1, 4);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Mars"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "retrograde"));
}

void test_interpret_glance_with_phase(void) {
    astronomy_interp_t r = ai_astro_interpret(4, 0, 4);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Jupiter"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Full Moon"));
}

void test_interpret_detail_planet(void) {
    astronomy_interp_t r = ai_astro_interpret(5, 0, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Saturn"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Kronos"));
}

void test_interpret_detail_retrograde(void) {
    astronomy_interp_t r = ai_astro_interpret(0, 1, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "RETROGRADE"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "inward"));
}

void test_interpret_detail_moon_phase(void) {
    astronomy_interp_t r = ai_astro_interpret(2, 0, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "New Moon"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Beginnings"));
}

void test_interpret_invalid(void) {
    astronomy_interp_t r = ai_astro_interpret(-1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_planet_count(void) { TEST_ASSERT_EQUAL_INT(8, ai_planet_count()); }
void test_phase_count(void) { TEST_ASSERT_EQUAL_INT(8, ai_phase_count()); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_planet_mercury);
    RUN_TEST(test_planet_saturn);
    RUN_TEST(test_planet_neptune);
    RUN_TEST(test_planet_all_8_valid);
    RUN_TEST(test_planet_invalid);
    RUN_TEST(test_phase_new);
    RUN_TEST(test_phase_full);
    RUN_TEST(test_phase_all_8_valid);
    RUN_TEST(test_phase_invalid);
    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance_retrograde);
    RUN_TEST(test_interpret_glance_with_phase);
    RUN_TEST(test_interpret_detail_planet);
    RUN_TEST(test_interpret_detail_retrograde);
    RUN_TEST(test_interpret_detail_moon_phase);
    RUN_TEST(test_interpret_invalid);
    RUN_TEST(test_planet_count);
    RUN_TEST(test_phase_count);
    return UNITY_END();
}
