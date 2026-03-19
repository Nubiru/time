#include "../../unity/unity.h"
#include "../../../src/systems/gregorian/gregorian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * gi_month_data — 12 months
 * ============================================================ */

void test_month_january(void) {
    gi_month_t m = gi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("January", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Janus"));
}

void test_month_march(void) {
    gi_month_t m = gi_month_data(3);
    TEST_ASSERT_EQUAL_INT(3, m.month);
    TEST_ASSERT_EQUAL_STRING("March", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Mars"));
}

void test_month_june(void) {
    gi_month_t m = gi_month_data(6);
    TEST_ASSERT_EQUAL_INT(6, m.month);
    TEST_ASSERT_EQUAL_STRING("June", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Juno"));
}

void test_month_december(void) {
    gi_month_t m = gi_month_data(12);
    TEST_ASSERT_EQUAL_INT(12, m.month);
    TEST_ASSERT_EQUAL_STRING("December", m.name);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Decem"));
}

void test_month_all_12_valid(void) {
    for (int i = 1; i <= 12; i++) {
        gi_month_t m = gi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.name);
        TEST_ASSERT_NOT_NULL(m.origin);
        TEST_ASSERT_NOT_NULL(m.quality);
        TEST_ASSERT_NOT_NULL(m.brief);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
    }
}

void test_month_invalid_0(void) {
    gi_month_t m = gi_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_month_invalid_13(void) {
    gi_month_t m = gi_month_data(13);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ============================================================
 * gi_season_data — 4 seasons
 * ============================================================ */

void test_season_spring(void) {
    gi_season_t s = gi_season_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.season);
    TEST_ASSERT_EQUAL_STRING("Spring", s.name);
}

void test_season_summer(void) {
    gi_season_t s = gi_season_data(1);
    TEST_ASSERT_EQUAL_INT(1, s.season);
    TEST_ASSERT_EQUAL_STRING("Summer", s.name);
}

void test_season_autumn(void) {
    gi_season_t s = gi_season_data(2);
    TEST_ASSERT_EQUAL_INT(2, s.season);
    TEST_ASSERT_EQUAL_STRING("Autumn", s.name);
}

void test_season_winter(void) {
    gi_season_t s = gi_season_data(3);
    TEST_ASSERT_EQUAL_INT(3, s.season);
    TEST_ASSERT_EQUAL_STRING("Winter", s.name);
}

void test_season_all_4_valid(void) {
    for (int i = 0; i < 4; i++) {
        gi_season_t s = gi_season_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.season);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.theme);
        TEST_ASSERT_NOT_NULL(s.quality);
        TEST_ASSERT_NOT_NULL(s.brief);
    }
}

void test_season_invalid(void) {
    gi_season_t s = gi_season_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.season);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* ============================================================
 * gi_day_data — 7 days
 * ============================================================ */

void test_day_monday(void) {
    gi_day_t d = gi_day_data(0);
    TEST_ASSERT_EQUAL_INT(0, d.day);
    TEST_ASSERT_EQUAL_STRING("Monday", d.name);
    TEST_ASSERT_EQUAL_STRING("Moon", d.planet);
}

void test_day_wednesday(void) {
    gi_day_t d = gi_day_data(2);
    TEST_ASSERT_EQUAL_INT(2, d.day);
    TEST_ASSERT_EQUAL_STRING("Wednesday", d.name);
    TEST_ASSERT_EQUAL_STRING("Mercury", d.planet);
}

void test_day_sunday(void) {
    gi_day_t d = gi_day_data(6);
    TEST_ASSERT_EQUAL_INT(6, d.day);
    TEST_ASSERT_EQUAL_STRING("Sunday", d.name);
    TEST_ASSERT_EQUAL_STRING("Sun", d.planet);
}

void test_day_all_7_valid(void) {
    for (int i = 0; i < 7; i++) {
        gi_day_t d = gi_day_data(i);
        TEST_ASSERT_EQUAL_INT(i, d.day);
        TEST_ASSERT_NOT_NULL(d.name);
        TEST_ASSERT_NOT_NULL(d.planet);
        TEST_ASSERT_NOT_NULL(d.origin);
        TEST_ASSERT_NOT_NULL(d.brief);
    }
}

void test_day_invalid(void) {
    gi_day_t d = gi_day_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, d.day);
    TEST_ASSERT_EQUAL_STRING("?", d.name);
}

void test_day_invalid_7(void) {
    gi_day_t d = gi_day_data(7);
    TEST_ASSERT_EQUAL_INT(-1, d.day);
    TEST_ASSERT_EQUAL_STRING("?", d.name);
}

/* ============================================================
 * gi_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_jan(void) {
    gregorian_interp_t r = gi_interpret(1, 15, 2);
    TEST_ASSERT_EQUAL_STRING("Jan", r.glyph);
}

void test_interpret_glyph_dec(void) {
    gregorian_interp_t r = gi_interpret(12, 25, 3);
    TEST_ASSERT_EQUAL_STRING("Dec", r.glyph);
}

void test_interpret_glance_format(void) {
    gregorian_interp_t r = gi_interpret(3, 19, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "March"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "19"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Wednesday"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Spring"));
}

void test_interpret_detail_contains_origin(void) {
    gregorian_interp_t r = gi_interpret(3, 19, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Mars"));
}

void test_interpret_detail_contains_season(void) {
    gregorian_interp_t r = gi_interpret(3, 19, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Spring"));
}

void test_interpret_detail_contains_planet(void) {
    gregorian_interp_t r = gi_interpret(3, 19, 2);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Mercury"));
}

void test_interpret_summer_season(void) {
    gregorian_interp_t r = gi_interpret(7, 4, 4);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Summer"));
}

void test_interpret_autumn_season(void) {
    gregorian_interp_t r = gi_interpret(10, 15, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Autumn"));
}

void test_interpret_winter_season(void) {
    gregorian_interp_t r = gi_interpret(1, 1, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Winter"));
}

void test_interpret_dec_is_winter(void) {
    gregorian_interp_t r = gi_interpret(12, 21, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Winter"));
}

void test_interpret_invalid_month(void) {
    gregorian_interp_t r = gi_interpret(0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_month_count(void) {
    TEST_ASSERT_EQUAL_INT(12, gi_month_count());
}

void test_season_count(void) {
    TEST_ASSERT_EQUAL_INT(4, gi_season_count());
}

void test_day_count(void) {
    TEST_ASSERT_EQUAL_INT(7, gi_day_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* gi_month_data */
    RUN_TEST(test_month_january);
    RUN_TEST(test_month_march);
    RUN_TEST(test_month_june);
    RUN_TEST(test_month_december);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid_0);
    RUN_TEST(test_month_invalid_13);

    /* gi_season_data */
    RUN_TEST(test_season_spring);
    RUN_TEST(test_season_summer);
    RUN_TEST(test_season_autumn);
    RUN_TEST(test_season_winter);
    RUN_TEST(test_season_all_4_valid);
    RUN_TEST(test_season_invalid);

    /* gi_day_data */
    RUN_TEST(test_day_monday);
    RUN_TEST(test_day_wednesday);
    RUN_TEST(test_day_sunday);
    RUN_TEST(test_day_all_7_valid);
    RUN_TEST(test_day_invalid);
    RUN_TEST(test_day_invalid_7);

    /* gi_interpret */
    RUN_TEST(test_interpret_glyph_jan);
    RUN_TEST(test_interpret_glyph_dec);
    RUN_TEST(test_interpret_glance_format);
    RUN_TEST(test_interpret_detail_contains_origin);
    RUN_TEST(test_interpret_detail_contains_season);
    RUN_TEST(test_interpret_detail_contains_planet);
    RUN_TEST(test_interpret_summer_season);
    RUN_TEST(test_interpret_autumn_season);
    RUN_TEST(test_interpret_winter_season);
    RUN_TEST(test_interpret_dec_is_winter);
    RUN_TEST(test_interpret_invalid_month);

    /* counts */
    RUN_TEST(test_month_count);
    RUN_TEST(test_season_count);
    RUN_TEST(test_day_count);

    return UNITY_END();
}
