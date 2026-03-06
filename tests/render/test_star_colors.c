#include "../unity/unity.h"
#include "../../src/render/star_colors.h"

#include <math.h>
#include <string.h>

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)
#define WIDE_TOL 0.15f
#define WIDE_NEAR(exp, act) ((act) >= (exp) - WIDE_TOL && (act) <= (exp) + WIDE_TOL)

void setUp(void) {}
void tearDown(void) {}

/* ===== B-V to Temperature (Ballesteros 2012) ===== */

/* 1. Sun (B-V = 0.65) should give ~5700-5800 K */
void test_bv_to_temp_sun(void)
{
    float t = star_bv_to_temperature(0.65f);
    TEST_ASSERT_TRUE(t > 5500.0f);
    TEST_ASSERT_TRUE(t < 6100.0f);
}

/* 2. A-type star (B-V = 0.00) should give ~9500-10500 K */
void test_bv_to_temp_a_type(void)
{
    float t = star_bv_to_temperature(0.00f);
    TEST_ASSERT_TRUE(t > 9000.0f);
    TEST_ASSERT_TRUE(t < 11000.0f);
}

/* 3. M-type star (B-V = 1.50) should give ~3000-3500 K */
void test_bv_to_temp_m_type(void)
{
    float t = star_bv_to_temperature(1.50f);
    TEST_ASSERT_TRUE(t > 2800.0f);
    TEST_ASSERT_TRUE(t < 3800.0f);
}

/* 4. O-type star (B-V = -0.33) should give >15000 K
 *    (Ballesteros formula gives ~17833 K; less accurate at blue extreme) */
void test_bv_to_temp_o_type(void)
{
    float t = star_bv_to_temperature(-0.33f);
    TEST_ASSERT_TRUE(t > 15000.0f);
}

/* 5. Monotonicity: lower B-V -> higher temperature */
void test_bv_to_temp_monotonic(void)
{
    float bv_vals[] = {-0.33f, -0.17f, 0.00f, 0.35f, 0.65f, 1.00f, 1.50f};
    int n = (int)(sizeof(bv_vals) / sizeof(bv_vals[0]));
    for (int i = 0; i < n - 1; i++) {
        float t1 = star_bv_to_temperature(bv_vals[i]);
        float t2 = star_bv_to_temperature(bv_vals[i + 1]);
        TEST_ASSERT_TRUE(t1 > t2);
    }
}

/* ===== Temperature to RGB ===== */

/* 6. Very hot star (30000 K) should be blue-ish: B >= R */
void test_temp_to_rgb_hot(void)
{
    star_rgb_t c = star_color_from_temperature(30000.0f);
    TEST_ASSERT_TRUE(c.b >= c.r);
    TEST_ASSERT_TRUE(c.r > 0.0f);
    TEST_ASSERT_TRUE(c.g > 0.0f);
    TEST_ASSERT_TRUE(c.b > 0.0f);
}

/* 7. Sun temperature (~5800 K) should be yellowish: R > G > B */
void test_temp_to_rgb_sun(void)
{
    star_rgb_t c = star_color_from_temperature(5800.0f);
    TEST_ASSERT_TRUE(c.r >= c.g);
    TEST_ASSERT_TRUE(c.g > c.b);
}

/* 8. Cool star (3200 K) should be reddish: R >> G >> B */
void test_temp_to_rgb_cool(void)
{
    star_rgb_t c = star_color_from_temperature(3200.0f);
    TEST_ASSERT_TRUE(c.r > c.g);
    TEST_ASSERT_TRUE(c.g > c.b);
    TEST_ASSERT_TRUE(c.r > 0.8f); /* red channel near max */
}

/* 9. All RGB values clamped 0-1 */
void test_temp_to_rgb_clamped(void)
{
    float temps[] = {1000.0f, 3000.0f, 6500.0f, 10000.0f, 40000.0f};
    int n = (int)(sizeof(temps) / sizeof(temps[0]));
    for (int i = 0; i < n; i++) {
        star_rgb_t c = star_color_from_temperature(temps[i]);
        TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
        TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
        TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
    }
}

/* 10. Very hot (6600+ K) should have B = 1.0 (maximum blue) */
void test_temp_to_rgb_hot_blue_max(void)
{
    star_rgb_t c = star_color_from_temperature(10000.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, c.b);
}

/* 11. RGB monotonicity: hotter stars have relatively more blue */
void test_temp_to_rgb_blue_increases_with_temp(void)
{
    star_rgb_t c_cool = star_color_from_temperature(3500.0f);
    star_rgb_t c_hot = star_color_from_temperature(15000.0f);
    float ratio_cool = c_cool.b / (c_cool.r + 0.001f);
    float ratio_hot = c_hot.b / (c_hot.r + 0.001f);
    TEST_ASSERT_TRUE(ratio_hot > ratio_cool);
}

/* ===== B-V to RGB (convenience pipeline) ===== */

/* 12. Rigel (B-V = -0.03): blue-ish white */
void test_bv_to_rgb_rigel(void)
{
    star_rgb_t c = star_color_from_bv(-0.03f);
    TEST_ASSERT_TRUE(c.b >= c.r * 0.85f); /* B comparable or greater than R */
    TEST_ASSERT_TRUE(c.r > 0.5f);
    TEST_ASSERT_TRUE(c.g > 0.5f);
    TEST_ASSERT_TRUE(c.b > 0.5f);
}

/* 13. Sirius (B-V = 0.00): nearly white, all channels high */
void test_bv_to_rgb_sirius(void)
{
    star_rgb_t c = star_color_from_bv(0.00f);
    TEST_ASSERT_TRUE(c.r > 0.7f);
    TEST_ASSERT_TRUE(c.g > 0.7f);
    TEST_ASSERT_TRUE(c.b > 0.7f);
}

/* 14. Sun (B-V = 0.65): yellowish, R > G > B */
void test_bv_to_rgb_sun(void)
{
    star_rgb_t c = star_color_from_bv(0.65f);
    TEST_ASSERT_TRUE(c.r >= c.g);
    TEST_ASSERT_TRUE(c.g > c.b);
}

/* 15. Arcturus (B-V = 1.23): orange, R >> G >> B */
void test_bv_to_rgb_arcturus(void)
{
    star_rgb_t c = star_color_from_bv(1.23f);
    TEST_ASSERT_TRUE(c.r > c.g);
    TEST_ASSERT_TRUE(c.g > c.b);
    TEST_ASSERT_TRUE(c.r > 0.8f);
}

/* 16. Betelgeuse (B-V = 1.85): red-orange, R >> G, B low
 *     Helland approx gives R=1.0, G~0.74, B~0.51 at ~3333K */
void test_bv_to_rgb_betelgeuse(void)
{
    star_rgb_t c = star_color_from_bv(1.85f);
    TEST_ASSERT_TRUE(c.r > c.g * 1.2f);
    TEST_ASSERT_TRUE(c.b < c.g);
    TEST_ASSERT_TRUE(c.r > 0.8f);
}

/* ===== Spectral Type Functions ===== */

/* 17. Spectral type temperatures match expected values */
void test_spectral_type_temperature(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1000.0f, 33000.0f, spectral_type_temperature(SPECTRAL_O));
    TEST_ASSERT_FLOAT_WITHIN(1000.0f, 20000.0f, spectral_type_temperature(SPECTRAL_B));
    TEST_ASSERT_FLOAT_WITHIN(500.0f, 9500.0f, spectral_type_temperature(SPECTRAL_A));
    TEST_ASSERT_FLOAT_WITHIN(500.0f, 7200.0f, spectral_type_temperature(SPECTRAL_F));
    TEST_ASSERT_FLOAT_WITHIN(500.0f, 5900.0f, spectral_type_temperature(SPECTRAL_G));
    TEST_ASSERT_FLOAT_WITHIN(500.0f, 4400.0f, spectral_type_temperature(SPECTRAL_K));
    TEST_ASSERT_FLOAT_WITHIN(500.0f, 3200.0f, spectral_type_temperature(SPECTRAL_M));
}

/* 18. Spectral type B-V values match expected */
void test_spectral_type_bv(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.05f, -0.33f, spectral_type_bv(SPECTRAL_O));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, -0.17f, spectral_type_bv(SPECTRAL_B));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.00f, spectral_type_bv(SPECTRAL_A));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.35f, spectral_type_bv(SPECTRAL_F));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.65f, spectral_type_bv(SPECTRAL_G));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.00f, spectral_type_bv(SPECTRAL_K));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.50f, spectral_type_bv(SPECTRAL_M));
}

/* 19. Spectral type names are correct */
void test_spectral_type_name(void)
{
    TEST_ASSERT_EQUAL_STRING("O", spectral_type_name(SPECTRAL_O));
    TEST_ASSERT_EQUAL_STRING("B", spectral_type_name(SPECTRAL_B));
    TEST_ASSERT_EQUAL_STRING("A", spectral_type_name(SPECTRAL_A));
    TEST_ASSERT_EQUAL_STRING("F", spectral_type_name(SPECTRAL_F));
    TEST_ASSERT_EQUAL_STRING("G", spectral_type_name(SPECTRAL_G));
    TEST_ASSERT_EQUAL_STRING("K", spectral_type_name(SPECTRAL_K));
    TEST_ASSERT_EQUAL_STRING("M", spectral_type_name(SPECTRAL_M));
}

/* 20. Spectral type from B-V classifies correctly */
void test_spectral_type_from_bv(void)
{
    TEST_ASSERT_EQUAL_INT(SPECTRAL_O, spectral_type_from_bv(-0.33f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_A, spectral_type_from_bv(0.00f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_G, spectral_type_from_bv(0.65f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_K, spectral_type_from_bv(1.00f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_M, spectral_type_from_bv(1.50f));
}

/* 21. Spectral type from temperature classifies correctly */
void test_spectral_type_from_temperature(void)
{
    TEST_ASSERT_EQUAL_INT(SPECTRAL_O, spectral_type_from_temperature(35000.0f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_B, spectral_type_from_temperature(15000.0f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_A, spectral_type_from_temperature(9000.0f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_F, spectral_type_from_temperature(7000.0f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_G, spectral_type_from_temperature(5800.0f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_K, spectral_type_from_temperature(4400.0f));
    TEST_ASSERT_EQUAL_INT(SPECTRAL_M, spectral_type_from_temperature(3000.0f));
}

/* ===== Reference Stars ===== */

/* 22. Reference star Rigel returns blue-ish white */
void test_reference_rigel(void)
{
    star_rgb_t c = star_reference_color("Rigel");
    TEST_ASSERT_TRUE(c.b >= c.r * 0.85f);
    TEST_ASSERT_TRUE(c.r > 0.5f);
}

/* 23. Reference star Sirius returns near-white */
void test_reference_sirius(void)
{
    star_rgb_t c = star_reference_color("Sirius");
    TEST_ASSERT_TRUE(c.r > 0.7f);
    TEST_ASSERT_TRUE(c.g > 0.7f);
    TEST_ASSERT_TRUE(c.b > 0.7f);
}

/* 24. Reference star Sun returns yellow */
void test_reference_sun(void)
{
    star_rgb_t c = star_reference_color("Sun");
    TEST_ASSERT_TRUE(c.r >= c.g);
    TEST_ASSERT_TRUE(c.g > c.b);
}

/* 25. Reference star Arcturus returns orange */
void test_reference_arcturus(void)
{
    star_rgb_t c = star_reference_color("Arcturus");
    TEST_ASSERT_TRUE(c.r > c.g);
    TEST_ASSERT_TRUE(c.g > c.b);
}

/* 26. Reference star Betelgeuse returns red-orange */
void test_reference_betelgeuse(void)
{
    star_rgb_t c = star_reference_color("Betelgeuse");
    TEST_ASSERT_TRUE(c.r > c.g * 1.2f);
    TEST_ASSERT_TRUE(c.b < c.g);
}

/* 27. Reference star Procyon returns warm white */
void test_reference_procyon(void)
{
    star_rgb_t c = star_reference_color("Procyon");
    TEST_ASSERT_TRUE(c.r > 0.7f);
    TEST_ASSERT_TRUE(c.g > 0.6f);
}

/* 28. Unknown star returns white (1,1,1) */
void test_reference_unknown(void)
{
    star_rgb_t c = star_reference_color("FakeStar");
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, c.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, c.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, c.b);
}

/* ===== Edge Cases ===== */

/* 29. Extreme negative B-V returns valid RGB */
void test_edge_extreme_blue(void)
{
    star_rgb_t c = star_color_from_bv(-0.40f);
    TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
    TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
}

/* 30. Extreme positive B-V returns valid RGB */
void test_edge_extreme_red(void)
{
    star_rgb_t c = star_color_from_bv(2.0f);
    TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
    TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
    TEST_ASSERT_TRUE(c.r > c.b); /* still reddish */
}

/* 31. Very low temperature (1000 K) */
void test_edge_very_low_temp(void)
{
    star_rgb_t c = star_color_from_temperature(1000.0f);
    TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
    TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
    TEST_ASSERT_TRUE(c.r > c.g); /* very red */
}

/* 32. Very high temperature (40000 K) */
void test_edge_very_high_temp(void)
{
    star_rgb_t c = star_color_from_temperature(40000.0f);
    TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
    TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= c.r); /* blue dominant */
}

/* 33. Invalid spectral type name returns "?" */
void test_spectral_type_name_invalid(void)
{
    const char *name = spectral_type_name(SPECTRAL_COUNT);
    TEST_ASSERT_EQUAL_STRING("?", name);
}

/* 34. Spectral SPECTRAL_COUNT as sentinel for out-of-range */
void test_spectral_type_temperature_invalid(void)
{
    float t = spectral_type_temperature(SPECTRAL_COUNT);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, t);
}

int main(void)
{
    UNITY_BEGIN();
    /* B-V to Temperature */
    RUN_TEST(test_bv_to_temp_sun);
    RUN_TEST(test_bv_to_temp_a_type);
    RUN_TEST(test_bv_to_temp_m_type);
    RUN_TEST(test_bv_to_temp_o_type);
    RUN_TEST(test_bv_to_temp_monotonic);
    /* Temperature to RGB */
    RUN_TEST(test_temp_to_rgb_hot);
    RUN_TEST(test_temp_to_rgb_sun);
    RUN_TEST(test_temp_to_rgb_cool);
    RUN_TEST(test_temp_to_rgb_clamped);
    RUN_TEST(test_temp_to_rgb_hot_blue_max);
    RUN_TEST(test_temp_to_rgb_blue_increases_with_temp);
    /* B-V to RGB */
    RUN_TEST(test_bv_to_rgb_rigel);
    RUN_TEST(test_bv_to_rgb_sirius);
    RUN_TEST(test_bv_to_rgb_sun);
    RUN_TEST(test_bv_to_rgb_arcturus);
    RUN_TEST(test_bv_to_rgb_betelgeuse);
    /* Spectral Types */
    RUN_TEST(test_spectral_type_temperature);
    RUN_TEST(test_spectral_type_bv);
    RUN_TEST(test_spectral_type_name);
    RUN_TEST(test_spectral_type_from_bv);
    RUN_TEST(test_spectral_type_from_temperature);
    /* Reference Stars */
    RUN_TEST(test_reference_rigel);
    RUN_TEST(test_reference_sirius);
    RUN_TEST(test_reference_sun);
    RUN_TEST(test_reference_arcturus);
    RUN_TEST(test_reference_betelgeuse);
    RUN_TEST(test_reference_procyon);
    RUN_TEST(test_reference_unknown);
    /* Edge Cases */
    RUN_TEST(test_edge_extreme_blue);
    RUN_TEST(test_edge_extreme_red);
    RUN_TEST(test_edge_very_low_temp);
    RUN_TEST(test_edge_very_high_temp);
    RUN_TEST(test_spectral_type_name_invalid);
    RUN_TEST(test_spectral_type_temperature_invalid);
    return UNITY_END();
}
