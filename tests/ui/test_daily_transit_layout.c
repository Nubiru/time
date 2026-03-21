#include "../unity/unity.h"
#include "../../src/ui/daily_transit_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Test data: 10 longitudes (0-7=planets, 8=Sun, 9=Moon) --- */

/* Mercury=30, Venus=60, Earth=90, Mars=120, Jupiter=180,
 * Saturn=240, Uranus=300, Neptune=330, Sun=0, Moon=90 */
static const double TEST_LONS[10] = {
    30.0, 60.0, 90.0, 120.0, 180.0, 240.0, 300.0, 330.0, 0.0, 90.0
};

/* --- Card frame --- */

void test_card_frame_position(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_h);
}

/* --- Sun and Moon header slots --- */

void test_sun_slot_position(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.10f, layout.sun_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.08f, layout.sun_slot.y);
    TEST_ASSERT_TRUE(layout.sun_slot.w > 0.0f);
    TEST_ASSERT_TRUE(layout.sun_slot.h > 0.0f);
}

void test_moon_slot_position(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.10f, layout.moon_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.18f, layout.moon_slot.y);
}

void test_moon_slot_below_sun(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_TRUE(layout.moon_slot.y > layout.sun_slot.y);
}

/* --- Sun info --- */

void test_sun_sign_aries(void)
{
    /* Sun at 0 degrees = Aries (0) */
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(0, layout.sun.sign);  /* Aries */
    TEST_ASSERT_EQUAL_STRING("Aries", layout.sun.sign_name);
}

void test_sun_planet_index(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(8, layout.sun.planet_index);  /* DIGNITY_SUN */
}

void test_sun_degree(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, layout.sun.degree);
}

void test_sun_symbol(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_NOT_NULL(layout.sun.sign_symbol);
}

/* --- Moon info --- */

void test_moon_sign_cancer(void)
{
    /* Moon at 90 degrees = Cancer (3) */
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(3, layout.moon.sign);  /* Cancer */
    TEST_ASSERT_EQUAL_STRING("Cancer", layout.moon.sign_name);
}

void test_moon_planet_index(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(9, layout.moon.planet_index);  /* DIGNITY_MOON */
}

/* --- Planet list --- */

void test_planet_count(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(10, layout.planet_count);
}

void test_planet_slots_stacked_vertically(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 1; i < layout.planet_count; i++) {
        TEST_ASSERT_TRUE(layout.planet_slots[i].y > layout.planet_slots[i - 1].y);
    }
}

void test_planet_slot_dimensions(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 0; i < layout.planet_count; i++) {
        TEST_ASSERT_TRUE(layout.planet_slots[i].w > 0.0f);
        TEST_ASSERT_TRUE(layout.planet_slots[i].h > 0.0f);
    }
}

void test_planet_info_signs(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    /* Mercury at 30 deg = Taurus (1) */
    TEST_ASSERT_EQUAL_INT(1, layout.planets[0].sign);
    /* Venus at 60 deg = Gemini (2) */
    TEST_ASSERT_EQUAL_INT(2, layout.planets[1].sign);
    /* Sun at 0 deg = Aries (0) */
    TEST_ASSERT_EQUAL_INT(0, layout.planets[8].sign);
    /* Moon at 90 deg = Cancer (3) */
    TEST_ASSERT_EQUAL_INT(3, layout.planets[9].sign);
}

void test_planet_info_names_not_null(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 0; i < layout.planet_count; i++) {
        TEST_ASSERT_NOT_NULL(layout.planets[i].sign_name);
        TEST_ASSERT_NOT_NULL(layout.planets[i].sign_symbol);
    }
}

void test_planet_retrograde_placeholder(void)
{
    /* Retrograde is always 0 for now. */
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 0; i < layout.planet_count; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.planets[i].is_retrograde);
    }
}

/* --- Aspects --- */

void test_aspect_count_positive(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_TRUE(layout.aspect_count > 0);
}

void test_aspect_count_bounded(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_TRUE(layout.aspect_count <= TRANSIT_MAX_ASPECTS);
}

void test_aspect_info_names(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 0; i < layout.aspect_count; i++) {
        TEST_ASSERT_NOT_NULL(layout.aspects[i].aspect_name);
        TEST_ASSERT_NOT_NULL(layout.aspects[i].aspect_symbol);
    }
}

void test_aspect_slots_stacked(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 1; i < layout.aspect_count; i++) {
        TEST_ASSERT_TRUE(layout.aspect_slots[i].y > layout.aspect_slots[i - 1].y);
    }
}

void test_aspect_types_valid(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    for (int i = 0; i < layout.aspect_count; i++) {
        TEST_ASSERT_TRUE(layout.aspects[i].type >= 0);
        TEST_ASSERT_TRUE(layout.aspects[i].type <= 4);
    }
}

/* --- daily_transit_summary --- */

void test_summary_format(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    char buf[128];
    const char *result = daily_transit_summary(&layout, buf, 128);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Sun in Aries, Moon in Cancer", buf);
}

void test_summary_null_layout(void)
{
    char buf[128];
    const char *result = daily_transit_summary(NULL, buf, 128);
    TEST_ASSERT_NULL(result);
}

void test_summary_null_buf(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    const char *result = daily_transit_summary(&layout, NULL, 128);
    TEST_ASSERT_NULL(result);
}

void test_summary_zero_buf_size(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    char buf[128];
    const char *result = daily_transit_summary(&layout, buf, 0);
    TEST_ASSERT_NULL(result);
}

/* --- daily_transit_aspect_count --- */

void test_aspect_count_accessor(void)
{
    daily_transit_layout_t layout = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(layout.aspect_count, daily_transit_aspect_count(&layout));
}

void test_aspect_count_null_layout(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_transit_aspect_count(NULL));
}

/* --- Invalid input --- */

void test_null_lons(void)
{
    daily_transit_layout_t layout = daily_transit_compute(NULL, 8.0);
    TEST_ASSERT_EQUAL_INT(0, layout.planet_count);
    TEST_ASSERT_EQUAL_INT(0, layout.aspect_count);
    /* Card frame should still be set. */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_transit_layout_t a = daily_transit_compute(TEST_LONS, 8.0);
    daily_transit_layout_t b = daily_transit_compute(TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(a.planet_count, b.planet_count);
    TEST_ASSERT_EQUAL_INT(a.aspect_count, b.aspect_count);
    TEST_ASSERT_EQUAL_INT(a.sun.sign, b.sun.sign);
    TEST_ASSERT_EQUAL_INT(a.moon.sign, b.moon.sign);
    for (int i = 0; i < a.planet_count; i++) {
        TEST_ASSERT_EQUAL_INT(a.planets[i].sign, b.planets[i].sign);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.planets[i].degree, b.planets[i].degree);
    }
}

void test_purity_different_input_different_output(void)
{
    double lons_a[10] = {10.0, 40.0, 70.0, 100.0, 130.0, 160.0, 190.0, 220.0, 5.0, 45.0};
    double lons_b[10] = {20.0, 50.0, 80.0, 110.0, 140.0, 170.0, 200.0, 230.0, 15.0, 75.0};
    daily_transit_layout_t a = daily_transit_compute(lons_a, 8.0);
    daily_transit_layout_t b = daily_transit_compute(lons_b, 8.0);
    /* Different inputs should give different sun signs. */
    /* Sun at 5 = Aries(0), Sun at 15 = Aries(0) — same sign, but different degrees. */
    int any_diff = 0;
    for (int i = 0; i < 10; i++) {
        if (fabsf(a.planets[i].degree - b.planets[i].degree) > 0.001f)
            any_diff = 1;
    }
    TEST_ASSERT_EQUAL_INT(1, any_diff);
}

int main(void)
{
    UNITY_BEGIN();
    /* Card frame */
    RUN_TEST(test_card_frame_position);
    /* Sun and Moon header slots */
    RUN_TEST(test_sun_slot_position);
    RUN_TEST(test_moon_slot_position);
    RUN_TEST(test_moon_slot_below_sun);
    /* Sun info */
    RUN_TEST(test_sun_sign_aries);
    RUN_TEST(test_sun_planet_index);
    RUN_TEST(test_sun_degree);
    RUN_TEST(test_sun_symbol);
    /* Moon info */
    RUN_TEST(test_moon_sign_cancer);
    RUN_TEST(test_moon_planet_index);
    /* Planet list */
    RUN_TEST(test_planet_count);
    RUN_TEST(test_planet_slots_stacked_vertically);
    RUN_TEST(test_planet_slot_dimensions);
    RUN_TEST(test_planet_info_signs);
    RUN_TEST(test_planet_info_names_not_null);
    RUN_TEST(test_planet_retrograde_placeholder);
    /* Aspects */
    RUN_TEST(test_aspect_count_positive);
    RUN_TEST(test_aspect_count_bounded);
    RUN_TEST(test_aspect_info_names);
    RUN_TEST(test_aspect_slots_stacked);
    RUN_TEST(test_aspect_types_valid);
    /* Summary */
    RUN_TEST(test_summary_format);
    RUN_TEST(test_summary_null_layout);
    RUN_TEST(test_summary_null_buf);
    RUN_TEST(test_summary_zero_buf_size);
    /* Aspect count accessor */
    RUN_TEST(test_aspect_count_accessor);
    RUN_TEST(test_aspect_count_null_layout);
    /* Invalid input */
    RUN_TEST(test_null_lons);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_input_different_output);
    return UNITY_END();
}
