#include "../unity/unity.h"
#include "../../src/math/color.h"

#include <math.h>

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Pure red HSL(0, 1, 0.5) -> RGB(1, 0, 0) */
void test_hsl_to_rgb_red(void)
{
    color_rgb_t c = color_hsl_to_rgb(color_hsl(0.0f, 1.0f, 0.5f));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.b));
}

/* 2. Pure green HSL(120, 1, 0.5) -> RGB(0, 1, 0) */
void test_hsl_to_rgb_green(void)
{
    color_rgb_t c = color_hsl_to_rgb(color_hsl(120.0f, 1.0f, 0.5f));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.b));
}

/* 3. Pure blue HSL(240, 1, 0.5) -> RGB(0, 0, 1) */
void test_hsl_to_rgb_blue(void)
{
    color_rgb_t c = color_hsl_to_rgb(color_hsl(240.0f, 1.0f, 0.5f));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.b));
}

/* 4. White HSL(0, 0, 1) -> RGB(1, 1, 1) */
void test_hsl_to_rgb_white(void)
{
    color_rgb_t c = color_hsl_to_rgb(color_hsl(0.0f, 0.0f, 1.0f));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.b));
}

/* 5. Black HSL(0, 0, 0) -> RGB(0, 0, 0) */
void test_hsl_to_rgb_black(void)
{
    color_rgb_t c = color_hsl_to_rgb(color_hsl(0.0f, 0.0f, 0.0f));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.b));
}

/* 6. Grey HSL(0, 0, 0.5) -> RGB(0.5, 0.5, 0.5) */
void test_hsl_to_rgb_grey(void)
{
    color_rgb_t c = color_hsl_to_rgb(color_hsl(0.0f, 0.0f, 0.5f));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.b));
}

/* 7. RGB(1, 0, 0) -> HSL(0, 1, 0.5) roundtrip */
void test_rgb_hsl_roundtrip(void)
{
    color_hsl_t hsl = color_rgb_to_hsl(color_rgb(1.0f, 0.0f, 0.0f));
    TEST_ASSERT_TRUE(NEAR(0.0f, hsl.h));
    TEST_ASSERT_TRUE(NEAR(1.0f, hsl.s));
    TEST_ASSERT_TRUE(NEAR(0.5f, hsl.l));

    color_rgb_t back = color_hsl_to_rgb(hsl);
    TEST_ASSERT_TRUE(NEAR(1.0f, back.r));
    TEST_ASSERT_TRUE(NEAR(0.0f, back.g));
    TEST_ASSERT_TRUE(NEAR(0.0f, back.b));
}

/* 8. color_from_hex(0xFF0000) = RGB(1, 0, 0) */
void test_from_hex_red(void)
{
    color_rgb_t c = color_from_hex(0xFF0000);
    TEST_ASSERT_TRUE(NEAR(1.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.b));
}

/* 9. color_from_hex(0x808080) ~ RGB(0.502, 0.502, 0.502) */
void test_from_hex_grey(void)
{
    color_rgb_t c = color_from_hex(0x808080);
    TEST_ASSERT_TRUE(NEAR(0.502f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.502f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.502f, c.b));
}

/* 10. color_lerp(black, white, 0.0) = black */
void test_lerp_t0(void)
{
    color_rgb_t c = color_lerp(color_rgb(0, 0, 0), color_rgb(1, 1, 1), 0.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.0f, c.b));
}

/* 11. color_lerp(black, white, 1.0) = white */
void test_lerp_t1(void)
{
    color_rgb_t c = color_lerp(color_rgb(0, 0, 0), color_rgb(1, 1, 1), 1.0f);
    TEST_ASSERT_TRUE(NEAR(1.0f, c.r));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.g));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.b));
}

/* 12. color_lerp(black, white, 0.5) = grey(0.5) */
void test_lerp_midpoint(void)
{
    color_rgb_t c = color_lerp(color_rgb(0, 0, 0), color_rgb(1, 1, 1), 0.5f);
    TEST_ASSERT_TRUE(NEAR(0.5f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.b));
}

/* 13. color_lerp_hsl: red to green at t=0.5 should be yellow (H=60) */
void test_lerp_hsl_red_to_green(void)
{
    color_hsl_t c = color_lerp_hsl(
        color_hsl(0.0f, 1.0f, 0.5f),
        color_hsl(120.0f, 1.0f, 0.5f),
        0.5f);
    TEST_ASSERT_TRUE(NEAR(60.0f, c.h));
    TEST_ASSERT_TRUE(NEAR(1.0f, c.s));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.l));
}

/* 14. color_rotate_hue(red, 120) = green hue area */
void test_rotate_hue(void)
{
    color_hsl_t c = color_rotate_hue(color_hsl(0.0f, 1.0f, 0.5f), 120.0f);
    TEST_ASSERT_TRUE(NEAR(120.0f, c.h));
}

/* 15. color_complementary(red_hsl) has hue ~180 */
void test_complementary(void)
{
    color_hsl_t c = color_complementary(color_hsl(0.0f, 1.0f, 0.5f));
    TEST_ASSERT_TRUE(NEAR(180.0f, c.h));
}

/* 16. color_triadic produces 3 colors 120 apart */
void test_triadic(void)
{
    color_hsl_t out[3];
    color_triadic(color_hsl(30.0f, 0.8f, 0.5f), out);
    TEST_ASSERT_TRUE(NEAR(30.0f, out[0].h));
    TEST_ASSERT_TRUE(NEAR(150.0f, out[1].h));
    TEST_ASSERT_TRUE(NEAR(270.0f, out[2].h));
}

/* 17. color_analogous with spread=30 produces 3 colors +/-30 apart */
void test_analogous(void)
{
    color_hsl_t out[3];
    color_analogous(color_hsl(60.0f, 0.8f, 0.5f), 30.0f, out);
    TEST_ASSERT_TRUE(NEAR(30.0f, out[0].h));
    TEST_ASSERT_TRUE(NEAR(60.0f, out[1].h));
    TEST_ASSERT_TRUE(NEAR(90.0f, out[2].h));
}

/* 18. color_luminance: white=1.0, black=0.0 */
void test_luminance(void)
{
    TEST_ASSERT_TRUE(NEAR(1.0f, color_luminance(color_rgb(1, 1, 1))));
    TEST_ASSERT_TRUE(NEAR(0.0f, color_luminance(color_rgb(0, 0, 0))));
}

/* 21. Purity: same input gives same output */
void test_purity(void)
{
    color_rgb_t a = color_hsl_to_rgb(color_hsl(200.0f, 0.7f, 0.4f));
    color_rgb_t b = color_hsl_to_rgb(color_hsl(200.0f, 0.7f, 0.4f));
    TEST_ASSERT_TRUE(a.r == b.r);
    TEST_ASSERT_TRUE(a.g == b.g);
    TEST_ASSERT_TRUE(a.b == b.b);
}

/* 22. HSL roundtrip for multiple colors */
void test_hsl_roundtrip_multiple(void)
{
    color_hsl_t inputs[] = {
        {0.0f, 1.0f, 0.5f},    /* red */
        {120.0f, 1.0f, 0.5f},  /* green */
        {240.0f, 1.0f, 0.5f},  /* blue */
        {60.0f, 0.8f, 0.6f},   /* yellow-ish */
        {300.0f, 0.5f, 0.3f},  /* magenta-ish */
    };
    for (int i = 0; i < 5; i++) {
        color_rgb_t rgb = color_hsl_to_rgb(inputs[i]);
        color_hsl_t back = color_rgb_to_hsl(rgb);
        /* Hue wraps: 0 and 360 are the same, use circular distance */
        float hd = fabsf(back.h - inputs[i].h);
        if (hd > 180.0f) hd = 360.0f - hd;
        TEST_ASSERT_TRUE(hd < TOL);
        TEST_ASSERT_TRUE(NEAR(inputs[i].s, back.s));
        TEST_ASSERT_TRUE(NEAR(inputs[i].l, back.l));
    }
}

/* 23. color_saturate clamps */
void test_saturate_clamp(void)
{
    color_hsl_t c = color_saturate(color_hsl(0, 0.8f, 0.5f), 0.5f);
    TEST_ASSERT_TRUE(NEAR(1.0f, c.s));

    c = color_saturate(color_hsl(0, 0.3f, 0.5f), -0.5f);
    TEST_ASSERT_TRUE(NEAR(0.0f, c.s));
}

/* 24. color_lighten clamps */
void test_lighten_clamp(void)
{
    color_hsl_t c = color_lighten(color_hsl(0, 0.5f, 0.8f), 0.5f);
    TEST_ASSERT_TRUE(NEAR(1.0f, c.l));

    c = color_lighten(color_hsl(0, 0.5f, 0.2f), -0.5f);
    TEST_ASSERT_TRUE(NEAR(0.0f, c.l));
}

/* 25. color_multiply */
void test_multiply(void)
{
    color_rgb_t c = color_multiply(color_rgb(0.5f, 0.5f, 1.0f), color_rgb(1.0f, 0.5f, 0.5f));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.r));
    TEST_ASSERT_TRUE(NEAR(0.25f, c.g));
    TEST_ASSERT_TRUE(NEAR(0.5f, c.b));
}

/* 26. Hue lerp wraps shortest path */
void test_lerp_hsl_wrap(void)
{
    color_hsl_t c = color_lerp_hsl(
        color_hsl(350.0f, 1.0f, 0.5f),
        color_hsl(10.0f, 1.0f, 0.5f),
        0.5f);
    TEST_ASSERT_TRUE(NEAR(0.0f, c.h) || NEAR(360.0f, c.h));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_hsl_to_rgb_red);
    RUN_TEST(test_hsl_to_rgb_green);
    RUN_TEST(test_hsl_to_rgb_blue);
    RUN_TEST(test_hsl_to_rgb_white);
    RUN_TEST(test_hsl_to_rgb_black);
    RUN_TEST(test_hsl_to_rgb_grey);
    RUN_TEST(test_rgb_hsl_roundtrip);
    RUN_TEST(test_from_hex_red);
    RUN_TEST(test_from_hex_grey);
    RUN_TEST(test_lerp_t0);
    RUN_TEST(test_lerp_t1);
    RUN_TEST(test_lerp_midpoint);
    RUN_TEST(test_lerp_hsl_red_to_green);
    RUN_TEST(test_rotate_hue);
    RUN_TEST(test_complementary);
    RUN_TEST(test_triadic);
    RUN_TEST(test_analogous);
    RUN_TEST(test_luminance);
    RUN_TEST(test_purity);
    RUN_TEST(test_hsl_roundtrip_multiple);
    RUN_TEST(test_saturate_clamp);
    RUN_TEST(test_lighten_clamp);
    RUN_TEST(test_multiply);
    RUN_TEST(test_lerp_hsl_wrap);
    return UNITY_END();
}
