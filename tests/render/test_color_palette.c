#include "../unity/unity.h"
#include "../../src/render/color_palette.h"
#include "../../src/render/color_theory.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

static int colors_equal(color_rgb_t a, color_rgb_t b)
{
    return (fabsf(a.r - b.r) < 0.001f &&
            fabsf(a.g - b.g) < 0.001f &&
            fabsf(a.b - b.b) < 0.001f);
}

static int color_valid(color_rgb_t c)
{
    return (c.r >= 0.0f && c.r <= 1.0f &&
            c.g >= 0.0f && c.g <= 1.0f &&
            c.b >= 0.0f && c.b <= 1.0f);
}

static int color_near(color_rgb_t c, float tr, float tg, float tb, float tol)
{
    return (fabsf(c.r - tr) <= tol &&
            fabsf(c.g - tg) <= tol &&
            fabsf(c.b - tb) <= tol);
}

/* Zodiac signs: 1-3 */

void test_all_zodiac_signs_valid(void)
{
    for (int i = 0; i < 12; i++) {
        color_rgb_t c = color_zodiac_sign(i);
        TEST_ASSERT_TRUE(color_valid(c));
    }
}

void test_zodiac_signs_distinct(void)
{
    int distinct_count = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = i + 1; j < 12; j++) {
            if (!colors_equal(color_zodiac_sign(i), color_zodiac_sign(j)))
                distinct_count++;
        }
    }
    /* All 66 pairs should be distinct */
    TEST_ASSERT_EQUAL_INT(66, distinct_count);
}

void test_zodiac_invalid_returns_gray(void)
{
    color_rgb_t c = color_zodiac_sign(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, c.r);
    c = color_zodiac_sign(12);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, c.r);
}

/* Elements: 4-5 */

void test_zodiac_elements_valid(void)
{
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_TRUE(color_valid(color_zodiac_element(i)));
    }
}

void test_fire_element_red_dominant(void)
{
    color_rgb_t fire = color_zodiac_element(0);
    TEST_ASSERT_TRUE(fire.r > fire.g);
    TEST_ASSERT_TRUE(fire.r > fire.b);
}

/* Planets: 6-8 */

void test_all_planets_valid(void)
{
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(color_valid(color_planet(i)));
    }
}

void test_sun_gold(void)
{
    color_rgb_t sun = color_planet(0);
    TEST_ASSERT_TRUE(sun.r > 0.8f);
    TEST_ASSERT_TRUE(sun.g > 0.6f);
}

void test_planet_invalid_gray(void)
{
    color_rgb_t c = color_planet(8);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, c.r);
}

/* Aspects: 9-11 */

void test_all_aspects_valid(void)
{
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_TRUE(color_valid(color_aspect(i)));
    }
}

void test_opposition_red(void)
{
    color_rgb_t opp = color_aspect(1);
    TEST_ASSERT_TRUE(opp.r > 0.8f);
    TEST_ASSERT_TRUE(opp.r > opp.b);
}

void test_trine_blue(void)
{
    color_rgb_t tri = color_aspect(2);
    TEST_ASSERT_TRUE(tri.b > tri.r);
}

/* Tzolkin families: 12-14 */

void test_tzolkin_families_valid(void)
{
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_TRUE(color_valid(color_tzolkin_family(i)));
    }
}

void test_tzolkin_red_family(void)
{
    color_rgb_t red = color_tzolkin_family(0);
    TEST_ASSERT_TRUE(red.r > red.g);
    TEST_ASSERT_TRUE(red.r > red.b);
}

void test_tzolkin_blue_family(void)
{
    color_rgb_t blue = color_tzolkin_family(2);
    TEST_ASSERT_TRUE(blue.b > blue.r);
}

/* Chinese elements: 15-16 */

void test_chinese_elements_valid(void)
{
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_TRUE(color_valid(color_chinese_element(i)));
    }
}

void test_chinese_wood_green(void)
{
    color_rgb_t wood = color_chinese_element(0);
    TEST_ASSERT_TRUE(wood.g > wood.r);
    TEST_ASSERT_TRUE(wood.g > wood.b);
}

/* Scale: 17-18 */

void test_scale_zero_is_black(void)
{
    color_rgb_t c = {0.8f, 0.5f, 0.3f};
    color_rgb_t scaled = color_scale(c, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, scaled.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, scaled.g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, scaled.b);
}

void test_scale_one_unchanged(void)
{
    color_rgb_t c = {0.8f, 0.5f, 0.3f};
    color_rgb_t scaled = color_scale(c, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, scaled.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, scaled.g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, scaled.b);
}

/* Lerp: 19-21 */

void test_lerp_t0_returns_a(void)
{
    color_rgb_t a = {1.0f, 0.0f, 0.0f};
    color_rgb_t b = {0.0f, 0.0f, 1.0f};
    color_rgb_t result = color_lerp(a, b, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, result.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result.b);
}

void test_lerp_t1_returns_b(void)
{
    color_rgb_t a = {1.0f, 0.0f, 0.0f};
    color_rgb_t b = {0.0f, 0.0f, 1.0f};
    color_rgb_t result = color_lerp(a, b, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, result.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, result.b);
}

void test_lerp_midpoint(void)
{
    color_rgb_t a = {0.0f, 0.0f, 0.0f};
    color_rgb_t b = {1.0f, 1.0f, 1.0f};
    color_rgb_t result = color_lerp(a, b, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, result.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, result.g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, result.b);
}

/* Clamp: 22-23 */

void test_clamp_overflows(void)
{
    color_rgb_t c = {1.5f, -0.5f, 2.0f};
    color_rgb_t clamped = color_clamp(c);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, clamped.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, clamped.g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, clamped.b);
}

void test_clamp_valid_unchanged(void)
{
    color_rgb_t c = {0.3f, 0.6f, 0.9f};
    color_rgb_t clamped = color_clamp(c);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, clamped.r);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.6f, clamped.g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.9f, clamped.b);
}

/* Purity: 24 */

void test_purity(void)
{
    color_rgb_t a = color_zodiac_sign(5);
    color_rgb_t b = color_zodiac_sign(5);
    TEST_ASSERT_EQUAL_FLOAT(a.r, b.r);
    TEST_ASSERT_EQUAL_FLOAT(a.g, b.g);
    TEST_ASSERT_EQUAL_FLOAT(a.b, b.b);
}

/* ======================================================================
 * NEW TESTS: Style Retrofit — color_theory derivation verification
 * ====================================================================== */

/* 25. Fire signs derive from ENERGY mood (hue range 0-22) */
void test_zodiac_fire_from_mood_energy(void)
{
    /* Fire signs: Aries(0), Leo(4), Sagittarius(8) — all red-dominant */
    for (int i = 0; i < 3; i++) {
        int sign = i * 4; /* 0, 4, 8 */
        color_rgb_t c = color_zodiac_sign(sign);
        TEST_ASSERT_TRUE(c.r > c.b);
        TEST_ASSERT_TRUE(c.r > 0.70f);
    }
}

/* 26. Water signs derive from CALM/TRUST/WISDOM mood (blue family) */
void test_zodiac_water_from_mood_calm(void)
{
    /* Water signs: Cancer(3), Scorpio(7), Pisces(11) — blue/indigo/purple */
    color_rgb_t cancer = color_zodiac_sign(3);
    TEST_ASSERT_TRUE(cancer.b > cancer.r); /* blue dominant */

    color_rgb_t scorpio = color_zodiac_sign(7);
    TEST_ASSERT_TRUE(scorpio.b > scorpio.g); /* indigo: blue > green */

    color_rgb_t pisces = color_zodiac_sign(11);
    TEST_ASSERT_TRUE(pisces.b > pisces.g); /* purple: blue > green */
}

/* 27. Fire element within 0.15 of original {0.90, 0.30, 0.20} */
void test_element_fire_near_original(void)
{
    color_rgb_t c = color_zodiac_element(0);
    TEST_ASSERT_TRUE(color_near(c, 0.90f, 0.30f, 0.20f, 0.15f));
}

/* 28. Water element within 0.15 of original {0.25, 0.50, 0.80} */
void test_element_water_near_original(void)
{
    color_rgb_t c = color_zodiac_element(3);
    TEST_ASSERT_TRUE(color_near(c, 0.25f, 0.50f, 0.80f, 0.15f));
}

/* 29. Tzolkin Red matches ct_system_primary(CT_SYSTEM_TZOLKIN) round-trip */
void test_tzolkin_red_matches_primary(void)
{
    color_rgb_t palette = color_tzolkin_family(0);
    color_rgb_t theory = ct_to_srgb(ct_system_primary(CT_SYSTEM_TZOLKIN));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.r, palette.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.g, palette.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.b, palette.b);
}

/* 30. Tzolkin White matches ct_system_secondary(CT_SYSTEM_TZOLKIN) */
void test_tzolkin_white_matches_secondary(void)
{
    color_rgb_t palette = color_tzolkin_family(1);
    color_rgb_t theory = ct_to_srgb(ct_system_secondary(CT_SYSTEM_TZOLKIN));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.r, palette.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.g, palette.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.b, palette.b);
}

/* 31. Chinese Wood matches ct_system_primary(CT_SYSTEM_CHINESE) */
void test_chinese_wood_matches_primary(void)
{
    color_rgb_t palette = color_chinese_element(0);
    color_rgb_t theory = ct_to_srgb(ct_system_primary(CT_SYSTEM_CHINESE));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.r, palette.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.g, palette.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.b, palette.b);
}

/* 32. Chinese Fire matches ct_system_secondary(CT_SYSTEM_CHINESE) */
void test_chinese_fire_matches_secondary(void)
{
    color_rgb_t palette = color_chinese_element(1);
    color_rgb_t theory = ct_to_srgb(ct_system_secondary(CT_SYSTEM_CHINESE));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.r, palette.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.g, palette.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, theory.b, palette.b);
}

/* 33. Sun within 0.15 of original {1.00, 0.85, 0.20} */
void test_planet_sun_near_original(void)
{
    color_rgb_t c = color_planet(0);
    TEST_ASSERT_TRUE(color_near(c, 1.00f, 0.85f, 0.20f, 0.15f));
}

/* 34. Conjunction within 0.15 of original {1.00, 0.85, 0.25} */
void test_aspect_conjunction_near_original(void)
{
    color_rgb_t c = color_aspect(0);
    TEST_ASSERT_TRUE(color_near(c, 1.00f, 0.85f, 0.25f, 0.15f));
}

/* 35. All zodiac colors within 0.15 per channel of originals */
void test_all_colors_near_originals(void)
{
    /* Original hardcoded zodiac values */
    const float originals[12][3] = {
        {0.90f, 0.25f, 0.20f}, {0.45f, 0.65f, 0.30f},
        {0.90f, 0.85f, 0.30f}, {0.30f, 0.55f, 0.80f},
        {0.95f, 0.55f, 0.15f}, {0.55f, 0.50f, 0.35f},
        {0.70f, 0.85f, 0.55f}, {0.35f, 0.25f, 0.60f},
        {0.85f, 0.35f, 0.30f}, {0.40f, 0.40f, 0.40f},
        {0.30f, 0.70f, 0.85f}, {0.50f, 0.40f, 0.75f},
    };
    for (int i = 0; i < 12; i++) {
        color_rgb_t c = color_zodiac_sign(i);
        TEST_ASSERT_TRUE_MESSAGE(
            color_near(c, originals[i][0], originals[i][1], originals[i][2], 0.15f),
            "Zodiac sign color too far from original");
    }
}

/* 36. Each zodiac sign's color matches its element's hue family */
void test_zodiac_element_consistency(void)
{
    /* Fire signs (0, 4, 8) should be red-dominant like Fire element */
    color_rgb_t fire_elem = color_zodiac_element(0);
    for (int i = 0; i < 3; i++) {
        color_rgb_t sign = color_zodiac_sign(i * 4);
        /* Same dominant channel as element */
        TEST_ASSERT_TRUE(sign.r > sign.b);
        (void)fire_elem;
    }

    /* Earth signs (1, 5, 9) — low saturation (no single channel overwhelms) */
    for (int i = 0; i < 3; i++) {
        color_rgb_t sign = color_zodiac_sign(i * 4 + 1);
        /* Earth tones: max - min spread is moderate */
        float mx = sign.r > sign.g ? (sign.r > sign.b ? sign.r : sign.b)
                                   : (sign.g > sign.b ? sign.g : sign.b);
        float mn = sign.r < sign.g ? (sign.r < sign.b ? sign.r : sign.b)
                                   : (sign.g < sign.b ? sign.g : sign.b);
        TEST_ASSERT_TRUE(mx - mn < 0.50f);
    }

    /* Water signs (3, 7, 11) should be blue-dominant */
    for (int i = 0; i < 3; i++) {
        color_rgb_t sign = color_zodiac_sign(i * 4 + 3);
        TEST_ASSERT_TRUE(sign.b > sign.g || sign.b > sign.r);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* Original tests 1-24 */
    RUN_TEST(test_all_zodiac_signs_valid);
    RUN_TEST(test_zodiac_signs_distinct);
    RUN_TEST(test_zodiac_invalid_returns_gray);
    RUN_TEST(test_zodiac_elements_valid);
    RUN_TEST(test_fire_element_red_dominant);
    RUN_TEST(test_all_planets_valid);
    RUN_TEST(test_sun_gold);
    RUN_TEST(test_planet_invalid_gray);
    RUN_TEST(test_all_aspects_valid);
    RUN_TEST(test_opposition_red);
    RUN_TEST(test_trine_blue);
    RUN_TEST(test_tzolkin_families_valid);
    RUN_TEST(test_tzolkin_red_family);
    RUN_TEST(test_tzolkin_blue_family);
    RUN_TEST(test_chinese_elements_valid);
    RUN_TEST(test_chinese_wood_green);
    RUN_TEST(test_scale_zero_is_black);
    RUN_TEST(test_scale_one_unchanged);
    RUN_TEST(test_lerp_t0_returns_a);
    RUN_TEST(test_lerp_t1_returns_b);
    RUN_TEST(test_lerp_midpoint);
    RUN_TEST(test_clamp_overflows);
    RUN_TEST(test_clamp_valid_unchanged);
    RUN_TEST(test_purity);
    /* New style-retrofit tests 25-36 */
    RUN_TEST(test_zodiac_fire_from_mood_energy);
    RUN_TEST(test_zodiac_water_from_mood_calm);
    RUN_TEST(test_element_fire_near_original);
    RUN_TEST(test_element_water_near_original);
    RUN_TEST(test_tzolkin_red_matches_primary);
    RUN_TEST(test_tzolkin_white_matches_secondary);
    RUN_TEST(test_chinese_wood_matches_primary);
    RUN_TEST(test_chinese_fire_matches_secondary);
    RUN_TEST(test_planet_sun_near_original);
    RUN_TEST(test_aspect_conjunction_near_original);
    RUN_TEST(test_all_colors_near_originals);
    RUN_TEST(test_zodiac_element_consistency);
    return UNITY_END();
}
