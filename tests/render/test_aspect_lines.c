#include "../unity/unity.h"
#include "../../src/render/aspect_lines.h"

#include <math.h>

#define TOL 0.01f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* --- aspect_line_color tests ---
 * After style retrofit, aspect_line_color() delegates to color_aspect()
 * from color_palette.h, which derives colors from color_theory.h.
 * Tests use behavioral assertions (hue family) instead of exact values. */

/* 1. Conjunction = gold (R high, G high, B low) */
void test_color_conjunction(void)
{
    float r, g, b;
    aspect_line_color(0, &r, &g, &b);
    TEST_ASSERT_TRUE(r > 0.85f);
    TEST_ASSERT_TRUE(g > 0.70f);
    TEST_ASSERT_TRUE(b < 0.40f);
}

/* 2. Opposition = red (R high, G low, B low) */
void test_color_opposition(void)
{
    float r, g, b;
    aspect_line_color(1, &r, &g, &b);
    TEST_ASSERT_TRUE(r > 0.80f);
    TEST_ASSERT_TRUE(r > g);
    TEST_ASSERT_TRUE(r > b);
}

/* 3. Trine = blue (B high, R low) */
void test_color_trine(void)
{
    float r, g, b;
    aspect_line_color(2, &r, &g, &b);
    TEST_ASSERT_TRUE(b > r);
    TEST_ASSERT_TRUE(b > 0.70f);
}

/* 4. Square = orange (R high, G moderate, B low) */
void test_color_square(void)
{
    float r, g, b;
    aspect_line_color(3, &r, &g, &b);
    TEST_ASSERT_TRUE(r > 0.80f);
    TEST_ASSERT_TRUE(g > 0.40f);
    TEST_ASSERT_TRUE(r > b);
}

/* 5. Sextile = green (G high, G > R, G > B) */
void test_color_sextile(void)
{
    float r, g, b;
    aspect_line_color(4, &r, &g, &b);
    TEST_ASSERT_TRUE(g > r);
    TEST_ASSERT_TRUE(g > b);
    TEST_ASSERT_TRUE(g > 0.60f);
}

/* --- aspect_line_opacity tests --- */

/* 6. Zero orb = full opacity */
void test_opacity_zero_orb(void)
{
    float o = aspect_line_opacity(0.0, 8.0);
    TEST_ASSERT_TRUE(NEAR(1.0f, o));
}

/* 7. Max orb = minimum opacity (0.2) */
void test_opacity_max_orb(void)
{
    float o = aspect_line_opacity(8.0, 8.0);
    TEST_ASSERT_TRUE(NEAR(0.2f, o));
}

/* 8. Half orb = 0.6 */
void test_opacity_half_orb(void)
{
    float o = aspect_line_opacity(4.0, 8.0);
    TEST_ASSERT_TRUE(NEAR(0.6f, o));
}

/* --- aspect_lines_generate tests --- */

static void make_positions(float *out, int n)
{
    for (int i = 0; i < n; i++) {
        out[i * 3 + 0] = (float)(i + 1);
        out[i * 3 + 1] = 0.0f;
        out[i * 3 + 2] = 0.0f;
    }
}

/* 9. Conjunction detected at 0 degrees */
void test_generate_conjunction(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {45.0, 45.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_EQUAL_INT(0, set.lines[0].type);
}

/* 10. Opposition detected at 180 degrees */
void test_generate_opposition(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {10.0, 190.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_EQUAL_INT(1, set.lines[0].type);
}

/* 11. Trine detected at 120 degrees */
void test_generate_trine(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {0.0, 120.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_EQUAL_INT(2, set.lines[0].type);
}

/* 12. Square detected at 90 degrees */
void test_generate_square(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {0.0, 90.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_EQUAL_INT(3, set.lines[0].type);
}

/* 13. Sextile detected at 60 degrees */
void test_generate_sextile(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {0.0, 60.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_EQUAL_INT(4, set.lines[0].type);
}

/* 14. No self-aspects (only 1 planet) */
void test_generate_no_self(void)
{
    float pos[3];
    make_positions(pos, 1);
    double lon[1] = {0.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 1, 8.0);
    TEST_ASSERT_EQUAL_INT(0, set.count);
}

/* 15. Count within MAX_ASPECT_LINES */
void test_generate_count_bounded(void)
{
    float pos[24];
    make_positions(pos, 8);
    /* All at same longitude -> all conjunctions */
    double lon[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 8, 8.0);
    TEST_ASSERT_TRUE(set.count <= MAX_ASPECT_LINES);
    TEST_ASSERT_EQUAL_INT(28, set.count); /* 8 choose 2 */
}

/* 16. Positions copied correctly */
void test_generate_positions_copied(void)
{
    float pos[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    double lon[2] = {0.0, 0.0}; /* conjunction */
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_TRUE(NEAR(1.0f, set.lines[0].x1));
    TEST_ASSERT_TRUE(NEAR(2.0f, set.lines[0].y1));
    TEST_ASSERT_TRUE(NEAR(3.0f, set.lines[0].z1));
    TEST_ASSERT_TRUE(NEAR(4.0f, set.lines[0].x2));
    TEST_ASSERT_TRUE(NEAR(5.0f, set.lines[0].y2));
    TEST_ASSERT_TRUE(NEAR(6.0f, set.lines[0].z2));
}

/* 17. No aspect outside orb */
void test_generate_no_aspect_outside_orb(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {0.0, 45.0}; /* 45 degrees: no standard aspect */
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(0, set.count);
}

/* 18. Wrapping: 350 and 10 = 20 degrees (no aspect) */
void test_generate_wrapping_no_aspect(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {350.0, 10.0}; /* 20 degrees apart */
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(0, set.count);
}

/* 19. Wrapping: 355 and 5 = 10 degrees, within orb of conjunction */
void test_generate_wrapping_conjunction(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {355.0, 5.0}; /* 10 degrees apart */
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 12.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    TEST_ASSERT_EQUAL_INT(0, set.lines[0].type); /* conjunction */
}

/* 20. Opacity set on generated line */
void test_generate_opacity(void)
{
    float pos[6];
    make_positions(pos, 2);
    double lon[2] = {0.0, 4.0}; /* 4 degrees from exact conjunction */
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 2, 8.0);
    TEST_ASSERT_EQUAL_INT(1, set.count);
    /* orb=4 out of max=8 -> opacity = 1.0 - 0.8*(4/8) = 0.6 */
    TEST_ASSERT_TRUE(NEAR(0.6f, set.lines[0].opacity));
}

/* 21. Multiple aspects from 3 planets */
void test_generate_multiple(void)
{
    float pos[9];
    make_positions(pos, 3);
    /* 0 and 120 and 240: each pair is 120 degrees = trine */
    double lon[3] = {0.0, 120.0, 240.0};
    aspect_line_set_t set = aspect_lines_generate(pos, lon, 3, 8.0);
    TEST_ASSERT_EQUAL_INT(3, set.count);
    for (int i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL_INT(2, set.lines[i].type); /* all trines */
}

/* 22. Purity: same input produces same output */
void test_purity(void)
{
    float pos[9];
    make_positions(pos, 3);
    double lon[3] = {0.0, 90.0, 180.0};
    aspect_line_set_t a = aspect_lines_generate(pos, lon, 3, 8.0);
    aspect_line_set_t b = aspect_lines_generate(pos, lon, 3, 8.0);
    TEST_ASSERT_EQUAL_INT(a.count, b.count);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_EQUAL_INT(a.lines[i].type, b.lines[i].type);
        TEST_ASSERT_TRUE(a.lines[i].opacity == b.lines[i].opacity);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_color_conjunction);
    RUN_TEST(test_color_opposition);
    RUN_TEST(test_color_trine);
    RUN_TEST(test_color_square);
    RUN_TEST(test_color_sextile);
    RUN_TEST(test_opacity_zero_orb);
    RUN_TEST(test_opacity_max_orb);
    RUN_TEST(test_opacity_half_orb);
    RUN_TEST(test_generate_conjunction);
    RUN_TEST(test_generate_opposition);
    RUN_TEST(test_generate_trine);
    RUN_TEST(test_generate_square);
    RUN_TEST(test_generate_sextile);
    RUN_TEST(test_generate_no_self);
    RUN_TEST(test_generate_count_bounded);
    RUN_TEST(test_generate_positions_copied);
    RUN_TEST(test_generate_no_aspect_outside_orb);
    RUN_TEST(test_generate_wrapping_no_aspect);
    RUN_TEST(test_generate_wrapping_conjunction);
    RUN_TEST(test_generate_opacity);
    RUN_TEST(test_generate_multiple);
    RUN_TEST(test_purity);
    return UNITY_END();
}
