#include "../unity/unity.h"
#include "../../src/math/arc_geometry.h"
#include <math.h>

#define EPSILON 1e-3f
#define PI_F 3.14159265f

static vec2_t origin = {0.0f, 0.0f};

static float dist(vec2_t a, vec2_t b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

void setUp(void) { }
void tearDown(void) { }

/* --- Circle/arc tests --- */

void test_circle_4_points_nsew(void) {
    vertex_buffer_t c = circle_points(origin, 1.0f, 4);
    TEST_ASSERT_EQUAL_INT(4, c.count);
    /* 0°=E, 120°=between, 240°=between, 360°=E again — wait, 4 points on 0-360:
     * t=0 -> 0°, t=1/3 -> 120°, t=2/3 -> 240°, t=1 -> 360°
     * Actually for circle_points (0 to 360), 4 points: 0, 120, 240, 360
     * Point 0: (1, 0)  Point 3: (1, 0) again (360=0) */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, c.vertices[0].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.vertices[0].position.y);
}

void test_circle_all_points_at_radius(void) {
    vertex_buffer_t c = circle_points(origin, 5.0f, 36);
    for (int i = 0; i < c.count; i++) {
        float d = dist(origin, c.vertices[i].position);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.0f, d);
    }
}

void test_arc_quarter(void) {
    vertex_buffer_t a = arc_points(origin, 1.0f, 0.0f, 90.0f, 3);
    TEST_ASSERT_EQUAL_INT(3, a.count);
    /* Point 0: 0° = (1,0), Point 1: 45° = (~0.707, ~0.707), Point 2: 90° = (0,1) */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, a.vertices[0].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, a.vertices[2].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, a.vertices[2].position.y);
}

void test_arc_full_matches_circle(void) {
    vertex_buffer_t a = arc_points(origin, 2.0f, 0.0f, 360.0f, 10);
    vertex_buffer_t c = circle_points(origin, 2.0f, 10);
    TEST_ASSERT_EQUAL_INT(a.count, c.count);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.vertices[i].position.x, a.vertices[i].position.x);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, c.vertices[i].position.y, a.vertices[i].position.y);
    }
}

void test_arc_wrap_350_to_10(void) {
    /* 350° to 10° should cover 20° (going through 0) */
    vertex_buffer_t a = arc_points(origin, 1.0f, 350.0f, 10.0f, 3);
    TEST_ASSERT_EQUAL_INT(3, a.count);
    /* All points should be at radius distance */
    for (int i = 0; i < a.count; i++) {
        float d = dist(origin, a.vertices[i].position);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, d);
    }
}

/* --- Ring tests --- */

void test_ring_segment_produces_strip(void) {
    ring_buffer_t r = ring_segment(origin, 3.0f, 5.0f, 0.0f, 30.0f, 4);
    /* 4 segments = 5 pairs = 10 vertices */
    TEST_ASSERT_EQUAL_INT(10, r.count);
}

void test_ring_inner_outer_radii(void) {
    ring_buffer_t r = ring_segment(origin, 3.0f, 5.0f, 0.0f, 90.0f, 8);
    /* Even indices (0,2,4,...) are inner, odd (1,3,5,...) are outer */
    for (int i = 0; i < r.count; i += 2) {
        float di = dist(origin, r.vertices[i].position);
        float do_ = dist(origin, r.vertices[i + 1].position);
        TEST_ASSERT_FLOAT_WITHIN(0.05f, 3.0f, di);
        TEST_ASSERT_FLOAT_WITHIN(0.05f, 5.0f, do_);
    }
}

void test_ring_full_360(void) {
    ring_buffer_t r = ring_full(origin, 2.0f, 4.0f, 36);
    TEST_ASSERT_TRUE(r.count > 0);
    /* First and last pairs should be at similar positions (closed ring) */
    float d = dist(r.vertices[0].position, r.vertices[r.count - 2].position);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, d);
}

void test_ring_uv_mapping(void) {
    ring_buffer_t r = ring_segment(origin, 2.0f, 4.0f, 0.0f, 90.0f, 4);
    /* First pair: u=0 */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.vertices[0].uv.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.vertices[0].uv.y); /* inner v=0 */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, r.vertices[1].uv.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.vertices[1].uv.y); /* outer v=1 */
    /* Last pair: u=1 */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.vertices[r.count - 2].uv.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, r.vertices[r.count - 1].uv.x);
}

/* --- Zodiac ring tests --- */

void test_zodiac_ring_12_segments(void) {
    zodiac_ring_t zr = zodiac_ring(origin, 3.0f, 5.0f, 4, 0.5f);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(zr.segments[i].count > 0);
    }
}

void test_zodiac_segment_covers_30_minus_gap(void) {
    zodiac_ring_t zr = zodiac_ring(origin, 3.0f, 5.0f, 4, 1.0f);
    /* Each segment should span ~29 degrees. First segment: 0.5 to 29.5 degrees */
    /* Check first and last inner vertices are at expected angles */
    ring_buffer_t *s = &zr.segments[0];
    /* First inner vertex angle from center */
    float a0 = atan2f(s->vertices[0].position.y, s->vertices[0].position.x) * 180.0f / PI_F;
    if (a0 < 0.0f) a0 += 360.0f;
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 0.5f, a0); /* starts at gap/2 = 0.5° */
}

void test_zodiac_segments_no_overlap(void) {
    zodiac_ring_t zr = zodiac_ring(origin, 3.0f, 5.0f, 4, 1.0f);
    /* End angle of segment i should be < start angle of segment i+1 */
    for (int i = 0; i < 11; i++) {
        ring_buffer_t *s0 = &zr.segments[i];
        ring_buffer_t *s1 = &zr.segments[i + 1];
        /* Last inner vertex of segment i */
        float end_angle = atan2f(s0->vertices[s0->count - 2].position.y,
                                  s0->vertices[s0->count - 2].position.x) * 180.0f / PI_F;
        if (end_angle < 0.0f) end_angle += 360.0f;
        /* First inner vertex of segment i+1 */
        float start_angle = atan2f(s1->vertices[0].position.y,
                                    s1->vertices[0].position.x) * 180.0f / PI_F;
        if (start_angle < 0.0f) start_angle += 360.0f;
        TEST_ASSERT_TRUE(start_angle > end_angle);
    }
}

/* --- Cusp line tests --- */

void test_cusp_line_at_0(void) {
    vertex_buffer_t cl = cusp_line(origin, 2.0f, 4.0f, 0.0f);
    TEST_ASSERT_EQUAL_INT(2, cl.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, cl.vertices[0].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cl.vertices[0].position.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 4.0f, cl.vertices[1].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cl.vertices[1].position.y);
}

void test_cusp_line_at_90(void) {
    vertex_buffer_t cl = cusp_line(origin, 2.0f, 4.0f, 90.0f);
    TEST_ASSERT_EQUAL_INT(2, cl.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cl.vertices[0].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, cl.vertices[0].position.y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, cl.vertices[1].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 4.0f, cl.vertices[1].position.y);
}

void test_house_cusp_lines_12(void) {
    float angles[12] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};
    cusp_lines_t cl = house_cusp_lines(origin, 2.0f, 4.0f, angles);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_EQUAL_INT(2, cl.lines[i].count);
    }
}

/* --- Chord tests --- */

void test_chord_diameter(void) {
    vertex_buffer_t ch = chord_line(origin, 5.0f, 0.0f, 180.0f);
    TEST_ASSERT_EQUAL_INT(2, ch.count);
    /* 0° = (5,0), 180° = (-5, 0) */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, ch.vertices[0].position.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, -5.0f, ch.vertices[1].position.x);
}

void test_chord_endpoints_on_circle(void) {
    vertex_buffer_t ch = chord_line(origin, 3.0f, 45.0f, 135.0f);
    float d0 = dist(origin, ch.vertices[0].position);
    float d1 = dist(origin, ch.vertices[1].position);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, d0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, d1);
}

/* --- Tick marks tests --- */

void test_tick_marks_12(void) {
    vertex_buffer_t t = tick_marks(origin, 4.0f, 5.0f, 12);
    TEST_ASSERT_EQUAL_INT(24, t.count); /* 12 ticks * 2 vertices */
}

void test_tick_inner_outer(void) {
    vertex_buffer_t t = tick_marks(origin, 4.0f, 5.0f, 4);
    /* Even indices inner, odd indices outer */
    for (int i = 0; i < t.count; i += 2) {
        float di = dist(origin, t.vertices[i].position);
        float do_ = dist(origin, t.vertices[i + 1].position);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, 4.0f, di);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, do_);
    }
}

/* --- Utility tests --- */

void test_point_on_circle_0_deg(void) {
    vec2_t p = point_on_circle(origin, 1.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, p.y);
}

void test_point_on_circle_90_deg(void) {
    vec2_t p = point_on_circle(origin, 1.0f, 90.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, p.x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f, p.y);
}

void test_arc_length_full_circle(void) {
    float len = arc_length(1.0f, 0.0f, 360.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f * PI_F, len);
}

/* --- Purity test --- */

void test_purity_same_input_same_output(void) {
    vertex_buffer_t a = arc_points(origin, 3.0f, 10.0f, 100.0f, 8);
    vertex_buffer_t b = arc_points(origin, 3.0f, 10.0f, 100.0f, 8);
    TEST_ASSERT_EQUAL_INT(a.count, b.count);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.vertices[i].position.x, b.vertices[i].position.x);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.vertices[i].position.y, b.vertices[i].position.y);
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Circle/arc */
    RUN_TEST(test_circle_4_points_nsew);
    RUN_TEST(test_circle_all_points_at_radius);
    RUN_TEST(test_arc_quarter);
    RUN_TEST(test_arc_full_matches_circle);
    RUN_TEST(test_arc_wrap_350_to_10);

    /* Ring */
    RUN_TEST(test_ring_segment_produces_strip);
    RUN_TEST(test_ring_inner_outer_radii);
    RUN_TEST(test_ring_full_360);
    RUN_TEST(test_ring_uv_mapping);

    /* Zodiac */
    RUN_TEST(test_zodiac_ring_12_segments);
    RUN_TEST(test_zodiac_segment_covers_30_minus_gap);
    RUN_TEST(test_zodiac_segments_no_overlap);

    /* Cusp lines */
    RUN_TEST(test_cusp_line_at_0);
    RUN_TEST(test_cusp_line_at_90);
    RUN_TEST(test_house_cusp_lines_12);

    /* Chord */
    RUN_TEST(test_chord_diameter);
    RUN_TEST(test_chord_endpoints_on_circle);

    /* Tick marks */
    RUN_TEST(test_tick_marks_12);
    RUN_TEST(test_tick_inner_outer);

    /* Utility */
    RUN_TEST(test_point_on_circle_0_deg);
    RUN_TEST(test_point_on_circle_90_deg);
    RUN_TEST(test_arc_length_full_circle);

    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);

    return UNITY_END();
}
