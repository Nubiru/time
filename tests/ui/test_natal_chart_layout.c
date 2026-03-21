#include "../unity/unity.h"
#include "../../src/ui/natal_chart_layout.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Test data: Aries ascendant, planets spread across zodiac --- */

static const double ARIES_ASC = 0.0;  /* 0 degrees Aries */

/* Mercury=30, Venus=60, Earth=90, Mars=120, Jupiter=180,
 * Saturn=240, Uranus=300, Neptune=330 */
static const double TEST_LONS[8] = {
    30.0, 60.0, 90.0, 120.0, 180.0, 240.0, 300.0, 330.0
};

/* --- Wheel geometry constants --- */

void test_center_position(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, layout.center_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, layout.center_y);
}

void test_outer_radius(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.45f, layout.outer_radius);
}

void test_inner_radius(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.30f, layout.inner_radius);
}

void test_planet_radius(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.37f, layout.planet_radius);
}

/* --- Ascendant --- */

void test_ascendant_degree(void)
{
    natal_chart_layout_t layout = natal_chart_compute(45.0, TEST_LONS, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 45.0f, layout.ascendant_deg);
}

void test_ascendant_sign_aries(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(0, layout.asc_sign);  /* Aries */
}

void test_ascendant_sign_leo(void)
{
    natal_chart_layout_t layout = natal_chart_compute(130.0, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(4, layout.asc_sign);  /* Leo */
}

void test_ascendant_point_at_left(void)
{
    /* Ascendant should be at 180 degrees chart angle (left of wheel). */
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    /* cos(180) = -1, so x = 0.5 + 0.45*(-1) = 0.05 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, layout.asc_point.x);
    /* sin(180) = 0, so y = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.asc_point.y);
}

/* --- House cusps --- */

void test_cusp_signs_whole_sign(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    /* Whole sign: house 1 = Aries (0), house 2 = Taurus (1), etc. */
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.cusp_signs[i]);
    }
}

void test_cusp_signs_cancer_asc(void)
{
    /* Cancer asc (90 deg): house 1 = Cancer (3), house 2 = Leo (4), ... */
    natal_chart_layout_t layout = natal_chart_compute(90.0, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(3, layout.cusp_signs[0]);   /* Cancer */
    TEST_ASSERT_EQUAL_INT(4, layout.cusp_signs[1]);   /* Leo */
    TEST_ASSERT_EQUAL_INT(5, layout.cusp_signs[2]);   /* Virgo */
}

void test_cusp_inner_outer_different_radius(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    /* Inner and outer cusp points should be at different distances from center. */
    float dx_inner = layout.cusp_inner[0].x - 0.5f;
    float dy_inner = layout.cusp_inner[0].y - 0.5f;
    float r_inner = sqrtf(dx_inner * dx_inner + dy_inner * dy_inner);

    float dx_outer = layout.cusp_outer[0].x - 0.5f;
    float dy_outer = layout.cusp_outer[0].y - 0.5f;
    float r_outer = sqrtf(dx_outer * dx_outer + dy_outer * dy_outer);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.30f, r_inner);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.45f, r_outer);
}

/* --- Sign labels --- */

void test_sign_labels_on_label_ring(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    float label_r = (0.45f + 0.30f) / 2.0f;  /* 0.375 */
    for (int i = 0; i < 12; i++) {
        float dx = layout.sign_labels[i].x - 0.5f;
        float dy = layout.sign_labels[i].y - 0.5f;
        float r = sqrtf(dx * dx + dy * dy);
        TEST_ASSERT_FLOAT_WITHIN(0.02f, label_r, r);
    }
}

/* --- Planet positions --- */

void test_planet_count(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(NATAL_CHART_PLANET_COUNT, layout.planet_count);
}

void test_planet_signs(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    /* 30 deg = Taurus (1), 60 = Gemini (2), 90 = Cancer (3), 120 = Leo (4),
     * 180 = Libra (6), 240 = Sagittarius (8), 300 = Aquarius (10), 330 = Pisces (11) */
    TEST_ASSERT_EQUAL_INT(1, layout.planets[0].sign);   /* Mercury in Taurus */
    TEST_ASSERT_EQUAL_INT(2, layout.planets[1].sign);   /* Venus in Gemini */
    TEST_ASSERT_EQUAL_INT(3, layout.planets[2].sign);   /* Earth in Cancer */
    TEST_ASSERT_EQUAL_INT(4, layout.planets[3].sign);   /* Mars in Leo */
    TEST_ASSERT_EQUAL_INT(6, layout.planets[4].sign);   /* Jupiter in Libra */
    TEST_ASSERT_EQUAL_INT(8, layout.planets[5].sign);   /* Saturn in Sagittarius */
    TEST_ASSERT_EQUAL_INT(10, layout.planets[6].sign);  /* Uranus in Aquarius */
    TEST_ASSERT_EQUAL_INT(11, layout.planets[7].sign);  /* Neptune in Pisces */
}

void test_planet_degrees_at_sign_boundary(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    /* All test longitudes are exact multiples of 30, so degree = 0.0 within sign. */
    for (int i = 0; i < NATAL_CHART_PLANET_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, layout.planets[i].degree);
    }
}

void test_planet_positions_on_planet_ring(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    for (int i = 0; i < NATAL_CHART_PLANET_COUNT; i++) {
        float dx = layout.planets[i].pos.x - 0.5f;
        float dy = layout.planets[i].pos.y - 0.5f;
        float r = sqrtf(dx * dx + dy * dy);
        TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.37f, r);
    }
}

void test_planet_index_matches(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    for (int i = 0; i < NATAL_CHART_PLANET_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.planets[i].planet_index);
    }
}

/* --- Dignity --- */

void test_planet_dignity_venus_in_taurus(void)
{
    /* Venus (1) in Taurus (1) = Domicile (+5). But lons[1]=60 = Gemini. */
    /* Use custom lons with Venus at 30 = Taurus. */
    double lons[8] = {0.0, 30.0, 90.0, 120.0, 180.0, 240.0, 300.0, 330.0};
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, lons, 8.0);
    /* Venus index=1, sign=Taurus(1), Venus rules Taurus -> Domicile=5 */
    TEST_ASSERT_EQUAL_INT(5, layout.planets[1].dignity);
}

void test_planet_dignity_earth_always_zero(void)
{
    /* Earth (index 2) always peregrine (0 score). */
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(0, layout.planets[2].dignity);
}

/* --- Aspects --- */

void test_aspects_found(void)
{
    /* With planets every 30 degrees and orb 8, we should find sextiles (60),
     * squares (90), trines (120), and oppositions (180). */
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_TRUE(layout.aspect_count > 0);
}

void test_aspect_count_bounded(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_TRUE(layout.aspect_count <= NATAL_CHART_MAX_ASPECTS);
    TEST_ASSERT_TRUE(layout.aspect_count >= 0);
}

void test_aspect_types_valid(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    for (int i = 0; i < layout.aspect_count; i++) {
        TEST_ASSERT_TRUE(layout.aspects[i].type >= 0);
        TEST_ASSERT_TRUE(layout.aspects[i].type <= 4);
    }
}

void test_aspect_orbs_non_negative(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    for (int i = 0; i < layout.aspect_count; i++) {
        TEST_ASSERT_TRUE(layout.aspects[i].orb >= 0.0f);
    }
}

/* --- angle_to_point --- */

void test_angle_to_point_zero(void)
{
    /* 0 degrees = right (east). */
    chart_point_t p = natal_chart_angle_to_point(0.0f, 0.45f, 0.5f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.95f, p.x);  /* 0.5 + 0.45 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, p.y);
}

void test_angle_to_point_90(void)
{
    /* 90 degrees = top (north, y flipped). */
    chart_point_t p = natal_chart_angle_to_point(90.0f, 0.45f, 0.5f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, p.y);  /* 0.5 - 0.45 */
}

void test_angle_to_point_180(void)
{
    /* 180 degrees = left (west). */
    chart_point_t p = natal_chart_angle_to_point(180.0f, 0.45f, 0.5f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, p.x);  /* 0.5 - 0.45 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, p.y);
}

void test_angle_to_point_270(void)
{
    /* 270 degrees = bottom (south, y flipped). */
    chart_point_t p = natal_chart_angle_to_point(270.0f, 0.45f, 0.5f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.95f, p.y);  /* 0.5 + 0.45 */
}

void test_angle_to_point_stores_angle(void)
{
    chart_point_t p = natal_chart_angle_to_point(123.4f, 0.3f, 0.5f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 123.4f, p.angle_deg);
}

/* --- natal_chart_planet_sign_name --- */

void test_planet_sign_name_valid(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    /* Planet 0 at 30 deg = Taurus */
    TEST_ASSERT_EQUAL_STRING("Taurus", natal_chart_planet_sign_name(&layout, 0));
}

void test_planet_sign_name_invalid_index(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_STRING("?", natal_chart_planet_sign_name(&layout, -1));
    TEST_ASSERT_EQUAL_STRING("?", natal_chart_planet_sign_name(&layout, 8));
}

void test_planet_sign_name_null_layout(void)
{
    TEST_ASSERT_EQUAL_STRING("?", natal_chart_planet_sign_name(NULL, 0));
}

/* --- Invalid input --- */

void test_null_planet_lons(void)
{
    natal_chart_layout_t layout = natal_chart_compute(ARIES_ASC, NULL, 8.0);
    TEST_ASSERT_EQUAL_INT(0, layout.planet_count);
    TEST_ASSERT_EQUAL_INT(0, layout.aspect_count);
    /* Geometry should still be set. */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, layout.center_x);
}

void test_negative_ascendant_normalized(void)
{
    natal_chart_layout_t layout = natal_chart_compute(-90.0, TEST_LONS, 8.0);
    /* -90 mod 360 = 270 -> Capricorn (9) */
    TEST_ASSERT_EQUAL_INT(9, layout.asc_sign);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 270.0f, layout.ascendant_deg);
}

void test_ascendant_over_360_normalized(void)
{
    natal_chart_layout_t layout = natal_chart_compute(400.0, TEST_LONS, 8.0);
    /* 400 mod 360 = 40 -> Taurus (1) */
    TEST_ASSERT_EQUAL_INT(1, layout.asc_sign);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 40.0f, layout.ascendant_deg);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    natal_chart_layout_t a = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    natal_chart_layout_t b = natal_chart_compute(ARIES_ASC, TEST_LONS, 8.0);
    TEST_ASSERT_EQUAL_INT(a.planet_count, b.planet_count);
    TEST_ASSERT_EQUAL_INT(a.aspect_count, b.aspect_count);
    TEST_ASSERT_EQUAL_INT(a.asc_sign, b.asc_sign);
    for (int i = 0; i < NATAL_CHART_PLANET_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.planets[i].sign, b.planets[i].sign);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.planets[i].pos.x, b.planets[i].pos.x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.planets[i].pos.y, b.planets[i].pos.y);
    }
}

void test_purity_different_input_different_output(void)
{
    double lons_a[8] = {10.0, 40.0, 70.0, 100.0, 130.0, 160.0, 190.0, 220.0};
    double lons_b[8] = {20.0, 50.0, 80.0, 110.0, 140.0, 170.0, 200.0, 230.0};
    natal_chart_layout_t a = natal_chart_compute(0.0, lons_a, 8.0);
    natal_chart_layout_t b = natal_chart_compute(0.0, lons_b, 8.0);
    /* Different inputs should produce different planet positions. */
    int any_diff = 0;
    for (int i = 0; i < NATAL_CHART_PLANET_COUNT; i++) {
        if (fabsf(a.planets[i].pos.x - b.planets[i].pos.x) > 0.001f)
            any_diff = 1;
    }
    TEST_ASSERT_EQUAL_INT(1, any_diff);
}

int main(void)
{
    UNITY_BEGIN();
    /* Wheel geometry */
    RUN_TEST(test_center_position);
    RUN_TEST(test_outer_radius);
    RUN_TEST(test_inner_radius);
    RUN_TEST(test_planet_radius);
    /* Ascendant */
    RUN_TEST(test_ascendant_degree);
    RUN_TEST(test_ascendant_sign_aries);
    RUN_TEST(test_ascendant_sign_leo);
    RUN_TEST(test_ascendant_point_at_left);
    /* House cusps */
    RUN_TEST(test_cusp_signs_whole_sign);
    RUN_TEST(test_cusp_signs_cancer_asc);
    RUN_TEST(test_cusp_inner_outer_different_radius);
    /* Sign labels */
    RUN_TEST(test_sign_labels_on_label_ring);
    /* Planets */
    RUN_TEST(test_planet_count);
    RUN_TEST(test_planet_signs);
    RUN_TEST(test_planet_degrees_at_sign_boundary);
    RUN_TEST(test_planet_positions_on_planet_ring);
    RUN_TEST(test_planet_index_matches);
    /* Dignity */
    RUN_TEST(test_planet_dignity_venus_in_taurus);
    RUN_TEST(test_planet_dignity_earth_always_zero);
    /* Aspects */
    RUN_TEST(test_aspects_found);
    RUN_TEST(test_aspect_count_bounded);
    RUN_TEST(test_aspect_types_valid);
    RUN_TEST(test_aspect_orbs_non_negative);
    /* angle_to_point */
    RUN_TEST(test_angle_to_point_zero);
    RUN_TEST(test_angle_to_point_90);
    RUN_TEST(test_angle_to_point_180);
    RUN_TEST(test_angle_to_point_270);
    RUN_TEST(test_angle_to_point_stores_angle);
    /* Accessor: planet sign name */
    RUN_TEST(test_planet_sign_name_valid);
    RUN_TEST(test_planet_sign_name_invalid_index);
    RUN_TEST(test_planet_sign_name_null_layout);
    /* Invalid input */
    RUN_TEST(test_null_planet_lons);
    RUN_TEST(test_negative_ascendant_normalized);
    RUN_TEST(test_ascendant_over_360_normalized);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_input_different_output);
    return UNITY_END();
}
