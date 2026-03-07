/* test_wheel_of_year.c — Wheel of the Year tests
 * TDD RED phase: tests written before implementation.
 * Celtic/Pagan 8-festival solar cycle.
 * 4 solar events (solstices, equinoxes) at 0/90/180/270 degrees.
 * 4 cross-quarter days (fire festivals) at 45/135/225/315 degrees. */

#include "../../unity/unity.h"
#include "../../../src/systems/celtic/wheel_of_year.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Festival count ===== */

void test_festival_count(void)
{
    TEST_ASSERT_EQUAL_INT(8, wheel_festival_count());
}

void test_festival_count_matches_enum(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_FESTIVAL_COUNT, wheel_festival_count());
}

/* ===== Festival names ===== */

void test_name_samhain(void)
{
    TEST_ASSERT_EQUAL_STRING("Samhain", wheel_festival_name(WHEEL_SAMHAIN));
}

void test_name_yule(void)
{
    TEST_ASSERT_EQUAL_STRING("Yule", wheel_festival_name(WHEEL_YULE));
}

void test_name_imbolc(void)
{
    TEST_ASSERT_EQUAL_STRING("Imbolc", wheel_festival_name(WHEEL_IMBOLC));
}

void test_name_ostara(void)
{
    TEST_ASSERT_EQUAL_STRING("Ostara", wheel_festival_name(WHEEL_OSTARA));
}

void test_name_beltane(void)
{
    TEST_ASSERT_EQUAL_STRING("Beltane", wheel_festival_name(WHEEL_BELTANE));
}

void test_name_litha(void)
{
    TEST_ASSERT_EQUAL_STRING("Litha", wheel_festival_name(WHEEL_LITHA));
}

void test_name_lughnasadh(void)
{
    TEST_ASSERT_EQUAL_STRING("Lughnasadh", wheel_festival_name(WHEEL_LUGHNASADH));
}

void test_name_mabon(void)
{
    TEST_ASSERT_EQUAL_STRING("Mabon", wheel_festival_name(WHEEL_MABON));
}

void test_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", wheel_festival_name((wheel_festival_t)99));
}

void test_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", wheel_festival_name((wheel_festival_t)-1));
}

/* ===== Celtic names ===== */

void test_celtic_name_samhain(void)
{
    TEST_ASSERT_EQUAL_STRING("Samhain", wheel_celtic_name(WHEEL_SAMHAIN));
}

void test_celtic_name_yule(void)
{
    TEST_ASSERT_EQUAL_STRING("Alban Arthan", wheel_celtic_name(WHEEL_YULE));
}

void test_celtic_name_imbolc(void)
{
    TEST_ASSERT_EQUAL_STRING("Imbolc", wheel_celtic_name(WHEEL_IMBOLC));
}

void test_celtic_name_ostara(void)
{
    TEST_ASSERT_EQUAL_STRING("Alban Eiler", wheel_celtic_name(WHEEL_OSTARA));
}

void test_celtic_name_beltane(void)
{
    TEST_ASSERT_EQUAL_STRING("Beltane", wheel_celtic_name(WHEEL_BELTANE));
}

void test_celtic_name_litha(void)
{
    TEST_ASSERT_EQUAL_STRING("Alban Hefin", wheel_celtic_name(WHEEL_LITHA));
}

void test_celtic_name_lughnasadh(void)
{
    TEST_ASSERT_EQUAL_STRING("Lughnasadh", wheel_celtic_name(WHEEL_LUGHNASADH));
}

void test_celtic_name_mabon(void)
{
    TEST_ASSERT_EQUAL_STRING("Alban Elfed", wheel_celtic_name(WHEEL_MABON));
}

void test_celtic_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", wheel_celtic_name((wheel_festival_t)99));
}

/* ===== Solar longitudes ===== */

void test_solar_longitude_samhain(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_SAMHAIN) - 225.0) < 0.001);
}

void test_solar_longitude_yule(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_YULE) - 270.0) < 0.001);
}

void test_solar_longitude_imbolc(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_IMBOLC) - 315.0) < 0.001);
}

void test_solar_longitude_ostara(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_OSTARA) - 0.0) < 0.001);
}

void test_solar_longitude_beltane(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_BELTANE) - 45.0) < 0.001);
}

void test_solar_longitude_litha(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_LITHA) - 90.0) < 0.001);
}

void test_solar_longitude_lughnasadh(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_LUGHNASADH) - 135.0) < 0.001);
}

void test_solar_longitude_mabon(void)
{
    TEST_ASSERT_TRUE(fabs(wheel_solar_longitude(WHEEL_MABON) - 180.0) < 0.001);
}

void test_solar_longitude_invalid(void)
{
    /* Invalid festival returns -1 */
    TEST_ASSERT_TRUE(wheel_solar_longitude((wheel_festival_t)99) < 0.0);
}

/* ===== Elements ===== */

void test_element_samhain(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_EARTH, wheel_festival_element(WHEEL_SAMHAIN));
}

void test_element_yule(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_EARTH, wheel_festival_element(WHEEL_YULE));
}

void test_element_imbolc(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_AIR, wheel_festival_element(WHEEL_IMBOLC));
}

void test_element_ostara(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_AIR, wheel_festival_element(WHEEL_OSTARA));
}

void test_element_beltane(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_FIRE, wheel_festival_element(WHEEL_BELTANE));
}

void test_element_litha(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_FIRE, wheel_festival_element(WHEEL_LITHA));
}

void test_element_lughnasadh(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_WATER, wheel_festival_element(WHEEL_LUGHNASADH));
}

void test_element_mabon(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_WATER, wheel_festival_element(WHEEL_MABON));
}

/* ===== Element names ===== */

void test_element_name_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", wheel_element_name(WHEEL_ELEMENT_EARTH));
}

void test_element_name_air(void)
{
    TEST_ASSERT_EQUAL_STRING("Air", wheel_element_name(WHEEL_ELEMENT_AIR));
}

void test_element_name_fire(void)
{
    TEST_ASSERT_EQUAL_STRING("Fire", wheel_element_name(WHEEL_ELEMENT_FIRE));
}

void test_element_name_water(void)
{
    TEST_ASSERT_EQUAL_STRING("Water", wheel_element_name(WHEEL_ELEMENT_WATER));
}

void test_element_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", wheel_element_name((wheel_element_t)99));
}

/* ===== Directions ===== */

void test_direction_samhain(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_NORTH, wheel_festival_direction(WHEEL_SAMHAIN));
}

void test_direction_ostara(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_EAST, wheel_festival_direction(WHEEL_OSTARA));
}

void test_direction_beltane(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_SOUTH, wheel_festival_direction(WHEEL_BELTANE));
}

void test_direction_lughnasadh(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_WEST, wheel_festival_direction(WHEEL_LUGHNASADH));
}

/* ===== Direction names ===== */

void test_direction_name_north(void)
{
    TEST_ASSERT_EQUAL_STRING("North", wheel_direction_name(WHEEL_DIR_NORTH));
}

void test_direction_name_east(void)
{
    TEST_ASSERT_EQUAL_STRING("East", wheel_direction_name(WHEEL_DIR_EAST));
}

void test_direction_name_south(void)
{
    TEST_ASSERT_EQUAL_STRING("South", wheel_direction_name(WHEEL_DIR_SOUTH));
}

void test_direction_name_west(void)
{
    TEST_ASSERT_EQUAL_STRING("West", wheel_direction_name(WHEEL_DIR_WEST));
}

void test_direction_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", wheel_direction_name((wheel_direction_t)99));
}

/* ===== Solar event flag ===== */

void test_is_solar_event_yule(void)
{
    TEST_ASSERT_TRUE(wheel_is_solar_event(WHEEL_YULE));
}

void test_is_solar_event_ostara(void)
{
    TEST_ASSERT_TRUE(wheel_is_solar_event(WHEEL_OSTARA));
}

void test_is_solar_event_litha(void)
{
    TEST_ASSERT_TRUE(wheel_is_solar_event(WHEEL_LITHA));
}

void test_is_solar_event_mabon(void)
{
    TEST_ASSERT_TRUE(wheel_is_solar_event(WHEEL_MABON));
}

void test_is_not_solar_event_samhain(void)
{
    TEST_ASSERT_FALSE(wheel_is_solar_event(WHEEL_SAMHAIN));
}

void test_is_not_solar_event_imbolc(void)
{
    TEST_ASSERT_FALSE(wheel_is_solar_event(WHEEL_IMBOLC));
}

void test_is_not_solar_event_beltane(void)
{
    TEST_ASSERT_FALSE(wheel_is_solar_event(WHEEL_BELTANE));
}

void test_is_not_solar_event_lughnasadh(void)
{
    TEST_ASSERT_FALSE(wheel_is_solar_event(WHEEL_LUGHNASADH));
}

void test_is_solar_event_invalid(void)
{
    TEST_ASSERT_FALSE(wheel_is_solar_event((wheel_festival_t)99));
}

/* ===== Festival info struct ===== */

void test_info_samhain(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_SAMHAIN);
    TEST_ASSERT_EQUAL_INT(WHEEL_SAMHAIN, info.id);
    TEST_ASSERT_EQUAL_STRING("Samhain", info.name);
    TEST_ASSERT_EQUAL_STRING("Samhain", info.celtic_name);
    TEST_ASSERT_TRUE(fabs(info.solar_longitude - 225.0) < 0.001);
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_EARTH, info.element);
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_NORTH, info.direction);
    TEST_ASSERT_FALSE(info.is_solar_event);
    TEST_ASSERT_EQUAL_INT(11, info.approx_month);
    TEST_ASSERT_EQUAL_INT(1, info.approx_day);
}

void test_info_ostara(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_OSTARA);
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, info.id);
    TEST_ASSERT_EQUAL_STRING("Ostara", info.name);
    TEST_ASSERT_EQUAL_STRING("Alban Eiler", info.celtic_name);
    TEST_ASSERT_TRUE(fabs(info.solar_longitude - 0.0) < 0.001);
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_AIR, info.element);
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_EAST, info.direction);
    TEST_ASSERT_TRUE(info.is_solar_event);
    TEST_ASSERT_EQUAL_INT(3, info.approx_month);
    TEST_ASSERT_EQUAL_INT(20, info.approx_day);
}

void test_info_litha(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_LITHA);
    TEST_ASSERT_EQUAL_INT(WHEEL_LITHA, info.id);
    TEST_ASSERT_EQUAL_STRING("Litha", info.name);
    TEST_ASSERT_EQUAL_STRING("Alban Hefin", info.celtic_name);
    TEST_ASSERT_TRUE(fabs(info.solar_longitude - 90.0) < 0.001);
    TEST_ASSERT_EQUAL_INT(WHEEL_ELEMENT_FIRE, info.element);
    TEST_ASSERT_EQUAL_INT(WHEEL_DIR_SOUTH, info.direction);
    TEST_ASSERT_TRUE(info.is_solar_event);
    TEST_ASSERT_EQUAL_INT(6, info.approx_month);
    TEST_ASSERT_EQUAL_INT(21, info.approx_day);
}

void test_info_meaning_not_null(void)
{
    /* Every festival should have a meaning string */
    for (int i = 0; i < WHEEL_FESTIVAL_COUNT; i++) {
        wheel_festival_info_t info = wheel_festival_info((wheel_festival_t)i);
        TEST_ASSERT_NOT_NULL(info.meaning);
        TEST_ASSERT_TRUE(strlen(info.meaning) > 0);
    }
}

/* ===== Current festival (nearest) ===== */

void test_current_festival_at_exact_samhain(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_SAMHAIN, wheel_current_festival(225.0));
}

void test_current_festival_at_exact_ostara(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_current_festival(0.0));
}

void test_current_festival_at_exact_litha(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_LITHA, wheel_current_festival(90.0));
}

void test_current_festival_at_exact_mabon(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_MABON, wheel_current_festival(180.0));
}

void test_current_festival_near_beltane(void)
{
    /* 47 degrees: closest to Beltane (45) */
    TEST_ASSERT_EQUAL_INT(WHEEL_BELTANE, wheel_current_festival(47.0));
}

void test_current_festival_near_yule(void)
{
    /* 268 degrees: closest to Yule (270) */
    TEST_ASSERT_EQUAL_INT(WHEEL_YULE, wheel_current_festival(268.0));
}

void test_current_festival_near_imbolc(void)
{
    /* 314 degrees: closest to Imbolc (315) */
    TEST_ASSERT_EQUAL_INT(WHEEL_IMBOLC, wheel_current_festival(314.0));
}

void test_current_festival_near_lughnasadh(void)
{
    /* 133 degrees: closest to Lughnasadh (135) */
    TEST_ASSERT_EQUAL_INT(WHEEL_LUGHNASADH, wheel_current_festival(133.0));
}

void test_current_festival_wrap_around_high(void)
{
    /* 358 degrees: closer to Ostara (0) than to Imbolc (315).
     * Distance to Ostara = 2, distance to Imbolc = 43 */
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_current_festival(358.0));
}

void test_current_festival_wrap_around_low(void)
{
    /* 1 degree: closer to Ostara (0) than to Beltane (45) */
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_current_festival(1.0));
}

void test_current_festival_midpoint_samhain_yule(void)
{
    /* Exactly between Samhain (225) and Yule (270) at 247.5 degrees.
     * At midpoint, either is valid. We accept either. */
    wheel_festival_t f = wheel_current_festival(247.5);
    TEST_ASSERT_TRUE(f == WHEEL_SAMHAIN || f == WHEEL_YULE);
}

/* ===== Next festival ===== */

void test_next_festival_from_zero(void)
{
    /* At Ostara (0), next is Beltane */
    TEST_ASSERT_EQUAL_INT(WHEEL_BELTANE, wheel_next_festival(0.0));
}

void test_next_festival_from_beltane(void)
{
    /* At Beltane (45), next is Litha */
    TEST_ASSERT_EQUAL_INT(WHEEL_LITHA, wheel_next_festival(45.0));
}

void test_next_festival_from_litha(void)
{
    /* At Litha (90), next is Lughnasadh */
    TEST_ASSERT_EQUAL_INT(WHEEL_LUGHNASADH, wheel_next_festival(90.0));
}

void test_next_festival_from_mabon(void)
{
    /* At Mabon (180), next is Samhain */
    TEST_ASSERT_EQUAL_INT(WHEEL_SAMHAIN, wheel_next_festival(180.0));
}

void test_next_festival_from_samhain(void)
{
    /* At Samhain (225), next is Yule */
    TEST_ASSERT_EQUAL_INT(WHEEL_YULE, wheel_next_festival(225.0));
}

void test_next_festival_from_yule(void)
{
    /* At Yule (270), next is Imbolc */
    TEST_ASSERT_EQUAL_INT(WHEEL_IMBOLC, wheel_next_festival(270.0));
}

void test_next_festival_from_imbolc(void)
{
    /* At Imbolc (315), next is Ostara */
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_next_festival(315.0));
}

void test_next_festival_wrap_from_high(void)
{
    /* At 350 degrees, next is Ostara (0) */
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_next_festival(350.0));
}

void test_next_festival_between_festivals(void)
{
    /* At 50 degrees (between Beltane=45 and Litha=90), next is Litha */
    TEST_ASSERT_EQUAL_INT(WHEEL_LITHA, wheel_next_festival(50.0));
}

void test_next_festival_just_past_lughnasadh(void)
{
    /* At 136 degrees (just past Lughnasadh=135), next is Mabon */
    TEST_ASSERT_EQUAL_INT(WHEEL_MABON, wheel_next_festival(136.0));
}

/* ===== Degrees until next ===== */

void test_degrees_until_next_from_ostara(void)
{
    /* From 0 (Ostara) to 45 (Beltane) = 45 degrees */
    double d = wheel_degrees_until_next(0.0);
    TEST_ASSERT_TRUE(fabs(d - 45.0) < 0.001);
}

void test_degrees_until_next_from_mabon(void)
{
    /* From 180 (Mabon) to 225 (Samhain) = 45 degrees */
    double d = wheel_degrees_until_next(180.0);
    TEST_ASSERT_TRUE(fabs(d - 45.0) < 0.001);
}

void test_degrees_until_next_from_imbolc(void)
{
    /* From 315 (Imbolc) to 0/360 (Ostara) = 45 degrees */
    double d = wheel_degrees_until_next(315.0);
    TEST_ASSERT_TRUE(fabs(d - 45.0) < 0.001);
}

void test_degrees_until_next_mid_sector(void)
{
    /* From 10 degrees, next is Beltane (45), distance = 35 */
    double d = wheel_degrees_until_next(10.0);
    TEST_ASSERT_TRUE(fabs(d - 35.0) < 0.001);
}

void test_degrees_until_next_near_wrap(void)
{
    /* From 350 degrees, next is Ostara (0/360), distance = 10 */
    double d = wheel_degrees_until_next(350.0);
    TEST_ASSERT_TRUE(fabs(d - 10.0) < 0.001);
}

void test_degrees_until_next_just_past(void)
{
    /* From 46 degrees, next is Litha (90), distance = 44 */
    double d = wheel_degrees_until_next(46.0);
    TEST_ASSERT_TRUE(fabs(d - 44.0) < 0.001);
}

/* ===== Festival active (within threshold) ===== */

void test_is_festival_active_exact(void)
{
    /* Exactly at Ostara (0), threshold 5: active */
    TEST_ASSERT_TRUE(wheel_is_festival_active(0.0, 5.0));
}

void test_is_festival_active_within(void)
{
    /* 3 degrees from Beltane (45), threshold 5: active */
    TEST_ASSERT_TRUE(wheel_is_festival_active(48.0, 5.0));
}

void test_is_festival_active_outside(void)
{
    /* 10 degrees from any festival, threshold 5: not active */
    TEST_ASSERT_FALSE(wheel_is_festival_active(55.0, 5.0));
}

void test_is_festival_active_wrap(void)
{
    /* 358 degrees, 3 degrees from Ostara (0), threshold 5: active */
    TEST_ASSERT_TRUE(wheel_is_festival_active(358.0, 5.0));
}

void test_is_festival_active_zero_threshold(void)
{
    /* Exactly at a festival with threshold 0 should still be active */
    TEST_ASSERT_TRUE(wheel_is_festival_active(90.0, 0.0));
}

/* ===== Active festival index ===== */

void test_active_festival_at_samhain(void)
{
    TEST_ASSERT_EQUAL_INT(WHEEL_SAMHAIN, wheel_active_festival(225.0, 5.0));
}

void test_active_festival_near_mabon(void)
{
    /* 178 degrees, 2 from Mabon (180), threshold 5 */
    TEST_ASSERT_EQUAL_INT(WHEEL_MABON, wheel_active_festival(178.0, 5.0));
}

void test_active_festival_none(void)
{
    /* 55 degrees: 10 from Beltane (45), 35 from Litha (90) */
    TEST_ASSERT_EQUAL_INT(-1, wheel_active_festival(55.0, 5.0));
}

void test_active_festival_wrap_near_ostara(void)
{
    /* 359 degrees, 1 from Ostara (0), threshold 5 */
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_active_festival(359.0, 5.0));
}

/* ===== Info for all 8 festivals ===== */

void test_all_festivals_have_unique_longitudes(void)
{
    double longs[8];
    for (int i = 0; i < 8; i++) {
        longs[i] = wheel_solar_longitude((wheel_festival_t)i);
    }
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            TEST_ASSERT_TRUE(fabs(longs[i] - longs[j]) > 0.001);
        }
    }
}

void test_all_festivals_45_degree_spacing(void)
{
    /* Festivals ordered by solar longitude: 0, 45, 90, 135, 180, 225, 270, 315.
     * The enum order is Samhain(225), Yule(270), Imbolc(315), Ostara(0),
     * Beltane(45), Litha(90), Lughnasadh(135), Mabon(180).
     * Each consecutive pair in ecliptic order is 45 degrees apart. */
    double sorted[8];
    for (int i = 0; i < 8; i++) {
        sorted[i] = wheel_solar_longitude((wheel_festival_t)i);
    }
    /* Sort ascending */
    for (int i = 0; i < 7; i++) {
        for (int j = i + 1; j < 8; j++) {
            if (sorted[j] < sorted[i]) {
                double tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    for (int i = 0; i < 7; i++) {
        double gap = sorted[i + 1] - sorted[i];
        TEST_ASSERT_TRUE(fabs(gap - 45.0) < 0.001);
    }
    /* Wrap gap: 360 - sorted[7] + sorted[0] = 45 */
    double wrap_gap = 360.0 - sorted[7] + sorted[0];
    TEST_ASSERT_TRUE(fabs(wrap_gap - 45.0) < 0.001);
}

void test_exactly_4_solar_events(void)
{
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (wheel_is_solar_event((wheel_festival_t)i)) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(4, count);
}

void test_exactly_4_elements_used(void)
{
    /* Each element should appear exactly twice */
    int counts[WHEEL_ELEMENT_COUNT];
    for (int i = 0; i < WHEEL_ELEMENT_COUNT; i++) counts[i] = 0;
    for (int i = 0; i < 8; i++) {
        wheel_element_t e = wheel_festival_element((wheel_festival_t)i);
        TEST_ASSERT_TRUE(e >= 0 && e < WHEEL_ELEMENT_COUNT);
        counts[e]++;
    }
    for (int i = 0; i < WHEEL_ELEMENT_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(2, counts[i]);
    }
}

void test_exactly_4_directions_used(void)
{
    /* Each direction should appear exactly twice */
    int counts[WHEEL_DIR_COUNT];
    for (int i = 0; i < WHEEL_DIR_COUNT; i++) counts[i] = 0;
    for (int i = 0; i < 8; i++) {
        wheel_direction_t d = wheel_festival_direction((wheel_festival_t)i);
        TEST_ASSERT_TRUE(d >= 0 && d < WHEEL_DIR_COUNT);
        counts[d]++;
    }
    for (int i = 0; i < WHEEL_DIR_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(2, counts[i]);
    }
}

/* ===== Approximate dates ===== */

void test_approx_date_yule(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_YULE);
    TEST_ASSERT_EQUAL_INT(12, info.approx_month);
    TEST_ASSERT_EQUAL_INT(21, info.approx_day);
}

void test_approx_date_beltane(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_BELTANE);
    TEST_ASSERT_EQUAL_INT(5, info.approx_month);
    TEST_ASSERT_EQUAL_INT(1, info.approx_day);
}

void test_approx_date_mabon(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_MABON);
    TEST_ASSERT_EQUAL_INT(9, info.approx_month);
    TEST_ASSERT_EQUAL_INT(22, info.approx_day);
}

void test_approx_date_imbolc(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_IMBOLC);
    TEST_ASSERT_EQUAL_INT(2, info.approx_month);
    TEST_ASSERT_EQUAL_INT(1, info.approx_day);
}

void test_approx_date_lughnasadh(void)
{
    wheel_festival_info_t info = wheel_festival_info(WHEEL_LUGHNASADH);
    TEST_ASSERT_EQUAL_INT(8, info.approx_month);
    TEST_ASSERT_EQUAL_INT(1, info.approx_day);
}

/* ===== Edge cases for longitude normalization ===== */

void test_current_festival_360(void)
{
    /* 360 degrees should normalize to 0, nearest is Ostara */
    TEST_ASSERT_EQUAL_INT(WHEEL_OSTARA, wheel_current_festival(360.0));
}

void test_next_festival_360(void)
{
    /* 360 degrees normalizes to 0, next after Ostara is Beltane */
    TEST_ASSERT_EQUAL_INT(WHEEL_BELTANE, wheel_next_festival(360.0));
}

void test_degrees_until_next_360(void)
{
    /* 360 normalizes to 0, same as Ostara, next is Beltane at 45 */
    double d = wheel_degrees_until_next(360.0);
    TEST_ASSERT_TRUE(fabs(d - 45.0) < 0.001);
}

/* ===== Main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Festival count */
    RUN_TEST(test_festival_count);
    RUN_TEST(test_festival_count_matches_enum);

    /* Festival names */
    RUN_TEST(test_name_samhain);
    RUN_TEST(test_name_yule);
    RUN_TEST(test_name_imbolc);
    RUN_TEST(test_name_ostara);
    RUN_TEST(test_name_beltane);
    RUN_TEST(test_name_litha);
    RUN_TEST(test_name_lughnasadh);
    RUN_TEST(test_name_mabon);
    RUN_TEST(test_name_invalid);
    RUN_TEST(test_name_negative);

    /* Celtic names */
    RUN_TEST(test_celtic_name_samhain);
    RUN_TEST(test_celtic_name_yule);
    RUN_TEST(test_celtic_name_imbolc);
    RUN_TEST(test_celtic_name_ostara);
    RUN_TEST(test_celtic_name_beltane);
    RUN_TEST(test_celtic_name_litha);
    RUN_TEST(test_celtic_name_lughnasadh);
    RUN_TEST(test_celtic_name_mabon);
    RUN_TEST(test_celtic_name_invalid);

    /* Solar longitudes */
    RUN_TEST(test_solar_longitude_samhain);
    RUN_TEST(test_solar_longitude_yule);
    RUN_TEST(test_solar_longitude_imbolc);
    RUN_TEST(test_solar_longitude_ostara);
    RUN_TEST(test_solar_longitude_beltane);
    RUN_TEST(test_solar_longitude_litha);
    RUN_TEST(test_solar_longitude_lughnasadh);
    RUN_TEST(test_solar_longitude_mabon);
    RUN_TEST(test_solar_longitude_invalid);

    /* Elements */
    RUN_TEST(test_element_samhain);
    RUN_TEST(test_element_yule);
    RUN_TEST(test_element_imbolc);
    RUN_TEST(test_element_ostara);
    RUN_TEST(test_element_beltane);
    RUN_TEST(test_element_litha);
    RUN_TEST(test_element_lughnasadh);
    RUN_TEST(test_element_mabon);

    /* Element names */
    RUN_TEST(test_element_name_earth);
    RUN_TEST(test_element_name_air);
    RUN_TEST(test_element_name_fire);
    RUN_TEST(test_element_name_water);
    RUN_TEST(test_element_name_invalid);

    /* Directions */
    RUN_TEST(test_direction_samhain);
    RUN_TEST(test_direction_ostara);
    RUN_TEST(test_direction_beltane);
    RUN_TEST(test_direction_lughnasadh);

    /* Direction names */
    RUN_TEST(test_direction_name_north);
    RUN_TEST(test_direction_name_east);
    RUN_TEST(test_direction_name_south);
    RUN_TEST(test_direction_name_west);
    RUN_TEST(test_direction_name_invalid);

    /* Solar event flag */
    RUN_TEST(test_is_solar_event_yule);
    RUN_TEST(test_is_solar_event_ostara);
    RUN_TEST(test_is_solar_event_litha);
    RUN_TEST(test_is_solar_event_mabon);
    RUN_TEST(test_is_not_solar_event_samhain);
    RUN_TEST(test_is_not_solar_event_imbolc);
    RUN_TEST(test_is_not_solar_event_beltane);
    RUN_TEST(test_is_not_solar_event_lughnasadh);
    RUN_TEST(test_is_solar_event_invalid);

    /* Festival info struct */
    RUN_TEST(test_info_samhain);
    RUN_TEST(test_info_ostara);
    RUN_TEST(test_info_litha);
    RUN_TEST(test_info_meaning_not_null);

    /* Current festival (nearest) */
    RUN_TEST(test_current_festival_at_exact_samhain);
    RUN_TEST(test_current_festival_at_exact_ostara);
    RUN_TEST(test_current_festival_at_exact_litha);
    RUN_TEST(test_current_festival_at_exact_mabon);
    RUN_TEST(test_current_festival_near_beltane);
    RUN_TEST(test_current_festival_near_yule);
    RUN_TEST(test_current_festival_near_imbolc);
    RUN_TEST(test_current_festival_near_lughnasadh);
    RUN_TEST(test_current_festival_wrap_around_high);
    RUN_TEST(test_current_festival_wrap_around_low);
    RUN_TEST(test_current_festival_midpoint_samhain_yule);

    /* Next festival */
    RUN_TEST(test_next_festival_from_zero);
    RUN_TEST(test_next_festival_from_beltane);
    RUN_TEST(test_next_festival_from_litha);
    RUN_TEST(test_next_festival_from_mabon);
    RUN_TEST(test_next_festival_from_samhain);
    RUN_TEST(test_next_festival_from_yule);
    RUN_TEST(test_next_festival_from_imbolc);
    RUN_TEST(test_next_festival_wrap_from_high);
    RUN_TEST(test_next_festival_between_festivals);
    RUN_TEST(test_next_festival_just_past_lughnasadh);

    /* Degrees until next */
    RUN_TEST(test_degrees_until_next_from_ostara);
    RUN_TEST(test_degrees_until_next_from_mabon);
    RUN_TEST(test_degrees_until_next_from_imbolc);
    RUN_TEST(test_degrees_until_next_mid_sector);
    RUN_TEST(test_degrees_until_next_near_wrap);
    RUN_TEST(test_degrees_until_next_just_past);

    /* Festival active */
    RUN_TEST(test_is_festival_active_exact);
    RUN_TEST(test_is_festival_active_within);
    RUN_TEST(test_is_festival_active_outside);
    RUN_TEST(test_is_festival_active_wrap);
    RUN_TEST(test_is_festival_active_zero_threshold);

    /* Active festival index */
    RUN_TEST(test_active_festival_at_samhain);
    RUN_TEST(test_active_festival_near_mabon);
    RUN_TEST(test_active_festival_none);
    RUN_TEST(test_active_festival_wrap_near_ostara);

    /* Structural integrity */
    RUN_TEST(test_all_festivals_have_unique_longitudes);
    RUN_TEST(test_all_festivals_45_degree_spacing);
    RUN_TEST(test_exactly_4_solar_events);
    RUN_TEST(test_exactly_4_elements_used);
    RUN_TEST(test_exactly_4_directions_used);

    /* Approximate dates */
    RUN_TEST(test_approx_date_yule);
    RUN_TEST(test_approx_date_beltane);
    RUN_TEST(test_approx_date_mabon);
    RUN_TEST(test_approx_date_imbolc);
    RUN_TEST(test_approx_date_lughnasadh);

    /* Edge cases */
    RUN_TEST(test_current_festival_360);
    RUN_TEST(test_next_festival_360);
    RUN_TEST(test_degrees_until_next_360);

    return UNITY_END();
}
