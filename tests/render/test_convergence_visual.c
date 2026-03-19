#include "../unity/unity.h"
#include "../../src/render/convergence_visual.h"
#include "../../src/render/concentric_ring.h"

#include <math.h>

#define FLOAT_TOL 0.001f
#define PI 3.14159265358979323846

void setUp(void) {}
void tearDown(void) {}

/* Helper: build a simple 3-ring layout for testing */
static cr_layout_t make_test_layout(void)
{
    cr_layout_t layout;
    layout.innermost_radius = 10.0f;
    layout.ring_gap = 1.0f;
    layout.ring_count = 3;

    for (int i = 0; i < CR_MAX_RINGS; i++) {
        layout.rings[i].segment_count = 12;
        layout.rings[i].highlight_index = -1;
        layout.rings[i].highlight_boost = 1.0f;
        layout.rings[i].segment_colors = NULL;
        layout.rings[i].uniform_color[0] = 0.5f;
        layout.rings[i].uniform_color[1] = 0.5f;
        layout.rings[i].uniform_color[2] = 0.5f;
        layout.rings[i].base_alpha = 0.5f;
    }

    return layout;
}

/* 1. cv_frame_init returns zeroed frame */
void test_cv_frame_init(void)
{
    cv_frame_t frame = cv_frame_init();
    TEST_ASSERT_EQUAL_INT(0, frame.line_count);
    TEST_ASSERT_EQUAL_INT(0, frame.glow_count);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, frame.atmosphere_warmth);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, frame.scene_intensity);

    /* All lines should be zeroed */
    for (int i = 0; i < CV_MAX_LINES; i++) {
        TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, frame.lines[i].opacity);
    }
    /* All glows should be zeroed */
    for (int i = 0; i < CV_MAX_GLOWS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, frame.glows[i].intensity);
    }
}

/* 2. cv_ring_midpoint computes (inner + outer) / 2 */
void test_cv_ring_midpoint(void)
{
    cr_layout_t layout = make_test_layout();
    /* Ring width = innermost_radius * 0.15 = 10.0 * 0.15 = 1.5 */
    /* Ring 0: inner=10.0, outer=11.5, mid=10.75 */
    float mid0 = cv_ring_midpoint(&layout, 0);
    float expected0 = (cr_ring_inner(&layout, 0) + cr_ring_outer(&layout, 0)) / 2.0f;
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, expected0, mid0);

    /* Ring 1: inner=10.0 + 1*(1.5+1.0) = 12.5, outer=14.0, mid=13.25 */
    float mid1 = cv_ring_midpoint(&layout, 1);
    float expected1 = (cr_ring_inner(&layout, 1) + cr_ring_outer(&layout, 1)) / 2.0f;
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, expected1, mid1);

    /* Ring 2 */
    float mid2 = cv_ring_midpoint(&layout, 2);
    float expected2 = (cr_ring_inner(&layout, 2) + cr_ring_outer(&layout, 2)) / 2.0f;
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, expected2, mid2);
}

/* 3. cv_line_between_rings produces endpoints at correct radii */
void test_cv_line_between_rings_positions(void)
{
    cr_layout_t layout = make_test_layout();
    float mid0 = cv_ring_midpoint(&layout, 0);
    float mid1 = cv_ring_midpoint(&layout, 1);

    cv_line_t line = cv_line_between_rings(&layout, 0, 1, 0.0f, 0.0f,
                                            0.8f, 2.0f, 0.5f);

    /* At angle 0, both points should be on +X axis (z=0) */
    float r1 = sqrtf(line.x1 * line.x1 + line.z1 * line.z1);
    float r2 = sqrtf(line.x2 * line.x2 + line.z2 * line.z2);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, mid0, r1);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, mid1, r2);
}

/* 4. Angle 0 = +X axis, angle 90 = +Z axis */
void test_cv_line_between_rings_angle(void)
{
    cr_layout_t layout = make_test_layout();
    float mid0 = cv_ring_midpoint(&layout, 0);

    /* Angle 0: point on +X axis */
    cv_line_t line0 = cv_line_between_rings(&layout, 0, 1, 0.0f, 0.0f,
                                             0.5f, 1.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, mid0, line0.x1);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, line0.z1);

    /* Angle 90: point on +Z axis */
    cv_line_t line90 = cv_line_between_rings(&layout, 0, 1, 90.0f, 0.0f,
                                              0.5f, 1.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, line90.x1);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, mid0, line90.z1);
}

/* 5. Opacity is preserved from parameters */
void test_cv_line_opacity_passthrough(void)
{
    cr_layout_t layout = make_test_layout();
    cv_line_t line = cv_line_between_rings(&layout, 0, 1, 0.0f, 0.0f,
                                            0.73f, 2.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.73f, line.opacity);
}

/* 6. Thickness is preserved from parameters */
void test_cv_line_thickness_passthrough(void)
{
    cr_layout_t layout = make_test_layout();
    cv_line_t line = cv_line_between_rings(&layout, 0, 1, 0.0f, 0.0f,
                                            0.5f, 3.5f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 3.5f, line.thickness);
}

/* 7. warmth=0 produces cool teal-ish (blue component > red) */
void test_cv_warmth_to_color_cool(void)
{
    float rgba[4];
    cv_warmth_to_color(0.0f, rgba);
    /* Celestial teal: {0.2, 0.75, 0.8} */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.2f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.75f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.8f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 1.0f, rgba[3]);
    /* Blue > red */
    TEST_ASSERT_TRUE(rgba[2] > rgba[0]);
}

/* 8. warmth=1 produces warm gold-ish (red > blue) */
void test_cv_warmth_to_color_warm(void)
{
    float rgba[4];
    cv_warmth_to_color(1.0f, rgba);
    /* Solar gold: {1.0, 0.85, 0.55} */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 1.0f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.85f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.55f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 1.0f, rgba[3]);
    /* Red > blue */
    TEST_ASSERT_TRUE(rgba[0] > rgba[2]);
}

/* 9. warmth=0.5 produces intermediate values */
void test_cv_warmth_to_color_mid(void)
{
    float rgba[4];
    cv_warmth_to_color(0.5f, rgba);
    /* Lerp: R = 0.2 + 0.5*(1.0-0.2) = 0.6 */
    /* Lerp: G = 0.75 + 0.5*(0.85-0.75) = 0.8 */
    /* Lerp: B = 0.8 + 0.5*(0.55-0.8) = 0.675 */
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.6f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.8f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.675f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 1.0f, rgba[3]);
}

/* 10. warmth clamped to [0, 1] */
void test_cv_warmth_to_color_clamped(void)
{
    float rgba_lo[4], rgba_hi[4], rgba_zero[4], rgba_one[4];
    cv_warmth_to_color(-0.5f, rgba_lo);
    cv_warmth_to_color(0.0f, rgba_zero);
    cv_warmth_to_color(1.5f, rgba_hi);
    cv_warmth_to_color(1.0f, rgba_one);

    /* Negative warmth clamps to 0 → same as warmth=0 */
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, rgba_zero[i], rgba_lo[i]);
    }
    /* Warmth > 1 clamps to 1 → same as warmth=1 */
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, rgba_one[i], rgba_hi[i]);
    }
}

/* 11. cv_line_between_rings color uses warmth mapping */
void test_cv_line_color_from_warmth(void)
{
    cr_layout_t layout = make_test_layout();

    /* warmth=0 should give teal color */
    cv_line_t cool = cv_line_between_rings(&layout, 0, 1, 0.0f, 0.0f,
                                            0.5f, 1.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.2f, cool.color[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.8f, cool.color[2]);

    /* warmth=1 should give gold color */
    cv_line_t warm = cv_line_between_rings(&layout, 0, 1, 0.0f, 0.0f,
                                            0.5f, 1.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 1.0f, warm.color[0]);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.55f, warm.color[2]);
}

/* 12. cv_frame_add_line adds line to frame */
void test_cv_frame_add_line(void)
{
    cv_frame_t frame = cv_frame_init();
    cv_line_t line;
    line.x1 = 1.0f; line.z1 = 2.0f;
    line.x2 = 3.0f; line.z2 = 4.0f;
    line.opacity = 0.9f;
    line.thickness = 2.5f;
    line.color[0] = 1.0f; line.color[1] = 0.8f;
    line.color[2] = 0.5f; line.color[3] = 1.0f;

    int ok = cv_frame_add_line(&frame, &line);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_EQUAL_INT(1, frame.line_count);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 1.0f, frame.lines[0].x1);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.9f, frame.lines[0].opacity);
}

/* 13. cv_frame_add_glow adds glow to frame */
void test_cv_frame_add_glow(void)
{
    cv_frame_t frame = cv_frame_init();
    cv_glow_t glow;
    glow.ring_index = 2;
    glow.intensity = 0.7f;
    glow.color[0] = 0.3f; glow.color[1] = 0.6f; glow.color[2] = 0.9f;

    int ok = cv_frame_add_glow(&frame, &glow);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_EQUAL_INT(1, frame.glow_count);
    TEST_ASSERT_EQUAL_INT(2, frame.glows[0].ring_index);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.7f, frame.glows[0].intensity);
}

/* 14. Line count capped at CV_MAX_LINES */
void test_cv_line_count_clamped(void)
{
    cv_frame_t frame = cv_frame_init();
    cv_line_t line;
    line.x1 = 0; line.z1 = 0; line.x2 = 1; line.z2 = 1;
    line.opacity = 0.5f; line.thickness = 1.0f;
    line.color[0] = 1; line.color[1] = 1; line.color[2] = 1; line.color[3] = 1;

    for (int i = 0; i < CV_MAX_LINES + 5; i++) {
        cv_frame_add_line(&frame, &line);
    }
    TEST_ASSERT_EQUAL_INT(CV_MAX_LINES, frame.line_count);
}

/* 15. Glow count capped at CV_MAX_GLOWS */
void test_cv_glow_count_clamped(void)
{
    cv_frame_t frame = cv_frame_init();
    cv_glow_t glow;
    glow.ring_index = 0; glow.intensity = 0.5f;
    glow.color[0] = 1; glow.color[1] = 1; glow.color[2] = 1;

    for (int i = 0; i < CV_MAX_GLOWS + 5; i++) {
        cv_frame_add_glow(&frame, &glow);
    }
    TEST_ASSERT_EQUAL_INT(CV_MAX_GLOWS, frame.glow_count);
}

/* 16. cv_frame_add_line returns 0 when full */
void test_cv_frame_add_line_returns_zero_when_full(void)
{
    cv_frame_t frame = cv_frame_init();
    cv_line_t line;
    line.x1 = 0; line.z1 = 0; line.x2 = 1; line.z2 = 1;
    line.opacity = 0.5f; line.thickness = 1.0f;
    line.color[0] = 1; line.color[1] = 1; line.color[2] = 1; line.color[3] = 1;

    for (int i = 0; i < CV_MAX_LINES; i++) {
        int ok = cv_frame_add_line(&frame, &line);
        TEST_ASSERT_EQUAL_INT(1, ok);
    }
    /* One more should fail */
    int fail = cv_frame_add_line(&frame, &line);
    TEST_ASSERT_EQUAL_INT(0, fail);
}

/* 17. cv_frame_add_glow returns 0 when full */
void test_cv_frame_add_glow_returns_zero_when_full(void)
{
    cv_frame_t frame = cv_frame_init();
    cv_glow_t glow;
    glow.ring_index = 0; glow.intensity = 0.5f;
    glow.color[0] = 1; glow.color[1] = 1; glow.color[2] = 1;

    for (int i = 0; i < CV_MAX_GLOWS; i++) {
        int ok = cv_frame_add_glow(&frame, &glow);
        TEST_ASSERT_EQUAL_INT(1, ok);
    }
    int fail = cv_frame_add_glow(&frame, &glow);
    TEST_ASSERT_EQUAL_INT(0, fail);
}

/* 18. Line at angle 180 is on -X axis */
void test_cv_line_angle_180(void)
{
    cr_layout_t layout = make_test_layout();
    float mid0 = cv_ring_midpoint(&layout, 0);

    cv_line_t line = cv_line_between_rings(&layout, 0, 1, 180.0f, 0.0f,
                                            0.5f, 1.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, -mid0, line.x1);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, line.z1);
}

/* 19. Ring midpoint with NULL layout pointer returns 0 */
void test_cv_ring_midpoint_null_safety(void)
{
    float mid = cv_ring_midpoint(NULL, 0);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, 0.0f, mid);
}

/* 20. Line between same ring index */
void test_cv_line_same_ring(void)
{
    cr_layout_t layout = make_test_layout();
    float mid0 = cv_ring_midpoint(&layout, 0);

    cv_line_t line = cv_line_between_rings(&layout, 0, 0, 0.0f, 90.0f,
                                            0.5f, 1.0f, 0.5f);
    /* Both points on ring 0, different angles */
    float r1 = sqrtf(line.x1 * line.x1 + line.z1 * line.z1);
    float r2 = sqrtf(line.x2 * line.x2 + line.z2 * line.z2);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, mid0, r1);
    TEST_ASSERT_FLOAT_WITHIN(FLOAT_TOL, mid0, r2);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cv_frame_init);
    RUN_TEST(test_cv_ring_midpoint);
    RUN_TEST(test_cv_line_between_rings_positions);
    RUN_TEST(test_cv_line_between_rings_angle);
    RUN_TEST(test_cv_line_opacity_passthrough);
    RUN_TEST(test_cv_line_thickness_passthrough);
    RUN_TEST(test_cv_warmth_to_color_cool);
    RUN_TEST(test_cv_warmth_to_color_warm);
    RUN_TEST(test_cv_warmth_to_color_mid);
    RUN_TEST(test_cv_warmth_to_color_clamped);
    RUN_TEST(test_cv_line_color_from_warmth);
    RUN_TEST(test_cv_frame_add_line);
    RUN_TEST(test_cv_frame_add_glow);
    RUN_TEST(test_cv_line_count_clamped);
    RUN_TEST(test_cv_glow_count_clamped);
    RUN_TEST(test_cv_frame_add_line_returns_zero_when_full);
    RUN_TEST(test_cv_frame_add_glow_returns_zero_when_full);
    RUN_TEST(test_cv_line_angle_180);
    RUN_TEST(test_cv_ring_midpoint_null_safety);
    RUN_TEST(test_cv_line_same_ring);
    return UNITY_END();
}
