/* test_ps_dashboard.c — Tests for personal space dashboard layout.
 * TDD: verify golden rectangle subdivision produces correct layout. */

#include "unity.h"
#include "systems/earth/ps_dashboard.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Helper: standard viewport and spacing */
static gl_rect_t full_viewport(void) {
    gl_rect_t r = {0.0f, 0.0f, 1.0f, 1.0f};
    return r;
}

static const float BASE_SPACING = 0.02f;

/* ===== Layout Computation ===== */

void test_compute_returns_four_sections(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    /* All four sections should have non-zero area */
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        TEST_ASSERT_TRUE(d.sections[i].bounds.w > 0.0f);
        TEST_ASSERT_TRUE(d.sections[i].bounds.h > 0.0f);
    }
}

void test_compute_gap_is_phi_derived(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float expected_gap = BASE_SPACING * (float)PHI_INV;
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_gap, d.gap);
}

void test_compute_container_matches_viewport(void) {
    gl_rect_t vp = {0.1f, 0.2f, 0.8f, 0.6f};
    ps_dashboard_t d = ps_compute(vp, BASE_SPACING);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.1f, d.container.x);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.2f, d.container.y);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.8f, d.container.w);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.6f, d.container.h);
}

void test_top_row_taller_than_bottom(void) {
    /* Golden subdivision: top (major) should be taller than bottom (minor) */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float tl_h = d.sections[PS_ACTIVE_CYCLES].bounds.h;
    float bl_h = d.sections[PS_FINGERPRINT].bounds.h;
    TEST_ASSERT_TRUE(tl_h > bl_h);
}

void test_left_column_wider_than_right(void) {
    /* Golden subdivision: left (major) should be wider than right (minor) */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float tl_w = d.sections[PS_ACTIVE_CYCLES].bounds.w;
    float tr_w = d.sections[PS_TODAY_FOR_YOU].bounds.w;
    TEST_ASSERT_TRUE(tl_w > tr_w);
}

void test_top_left_is_largest_section(void) {
    /* TL (Active Cycles) = major x major = largest quadrant */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float tl_area = d.sections[PS_ACTIVE_CYCLES].bounds.w *
                    d.sections[PS_ACTIVE_CYCLES].bounds.h;
    for (int i = 1; i < PS_SECTION_COUNT; i++) {
        float area = d.sections[i].bounds.w * d.sections[i].bounds.h;
        TEST_ASSERT_TRUE(tl_area > area);
    }
}

void test_bottom_right_is_smallest_section(void) {
    /* BR (Progress) = minor x minor = smallest quadrant */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float br_area = d.sections[PS_PROGRESS].bounds.w *
                    d.sections[PS_PROGRESS].bounds.h;
    for (int i = 0; i < PS_SECTION_COUNT - 1; i++) {
        float area = d.sections[i].bounds.w * d.sections[i].bounds.h;
        TEST_ASSERT_TRUE(br_area < area);
    }
}

void test_sections_no_overlap(void) {
    /* No two sections should overlap */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        for (int j = i + 1; j < PS_SECTION_COUNT; j++) {
            gl_rect_t a = d.sections[i].bounds;
            gl_rect_t b = d.sections[j].bounds;
            /* Check non-overlap: at least one of these must be true */
            int no_overlap = (a.x + a.w <= b.x + 0.001f) ||
                             (b.x + b.w <= a.x + 0.001f) ||
                             (a.y + a.h <= b.y + 0.001f) ||
                             (b.y + b.h <= a.y + 0.001f);
            TEST_ASSERT_TRUE(no_overlap);
        }
    }
}

void test_sections_within_viewport(void) {
    gl_rect_t vp = {0.1f, 0.1f, 0.8f, 0.8f};
    ps_dashboard_t d = ps_compute(vp, BASE_SPACING);
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        gl_rect_t b = d.sections[i].bounds;
        TEST_ASSERT_TRUE(b.x >= vp.x - 0.001f);
        TEST_ASSERT_TRUE(b.y >= vp.y - 0.001f);
        TEST_ASSERT_TRUE(b.x + b.w <= vp.x + vp.w + 0.001f);
        TEST_ASSERT_TRUE(b.y + b.h <= vp.y + vp.h + 0.001f);
    }
}

void test_golden_ratio_height_split(void) {
    /* Top row height should be approximately phi^-1 of total (minus gap) */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float top_h = d.sections[PS_ACTIVE_CYCLES].bounds.h;
    float bot_h = d.sections[PS_FINGERPRINT].bounds.h;
    float total = top_h + bot_h + d.gap;
    float ratio = top_h / total;
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)PHI_INV, ratio);
}

void test_golden_ratio_width_split(void) {
    /* Left column width should be approximately phi^-1 of total (minus gap) */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    float left_w = d.sections[PS_ACTIVE_CYCLES].bounds.w;
    float right_w = d.sections[PS_TODAY_FOR_YOU].bounds.w;
    float total = left_w + right_w + d.gap;
    float ratio = left_w / total;
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)PHI_INV, ratio);
}

/* ===== Section Titles ===== */

void test_section_titles_set(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    TEST_ASSERT_EQUAL_STRING("Active Cycles", d.sections[PS_ACTIVE_CYCLES].title);
    TEST_ASSERT_EQUAL_STRING("Today for You", d.sections[PS_TODAY_FOR_YOU].title);
    TEST_ASSERT_EQUAL_STRING("Cosmic Fingerprint", d.sections[PS_FINGERPRINT].title);
    TEST_ASSERT_EQUAL_STRING("Exploration Progress", d.sections[PS_PROGRESS].title);
}

void test_section_title_by_id(void) {
    TEST_ASSERT_EQUAL_STRING("Active Cycles", ps_section_title(PS_ACTIVE_CYCLES));
    TEST_ASSERT_EQUAL_STRING("Today for You", ps_section_title(PS_TODAY_FOR_YOU));
    TEST_ASSERT_EQUAL_STRING("Cosmic Fingerprint", ps_section_title(PS_FINGERPRINT));
    TEST_ASSERT_EQUAL_STRING("Exploration Progress", ps_section_title(PS_PROGRESS));
}

void test_section_title_invalid_id(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", ps_section_title(-1));
    TEST_ASSERT_EQUAL_STRING("Unknown", ps_section_title(PS_SECTION_COUNT));
}

/* ===== Section Lookup ===== */

void test_ps_section_valid(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    const ps_section_t *s = ps_section(&d, PS_ACTIVE_CYCLES);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_STRING("Active Cycles", s->title);
}

void test_ps_section_invalid(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    const ps_section_t *s = ps_section(&d, -1);
    TEST_ASSERT_NULL(s);
    s = ps_section(&d, PS_SECTION_COUNT);
    TEST_ASSERT_NULL(s);
}

/* ===== Hit Testing ===== */

void test_hit_test_top_left(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    /* Center of TL section */
    gl_rect_t tl = d.sections[PS_ACTIVE_CYCLES].bounds;
    float cx = tl.x + tl.w * 0.5f;
    float cy = tl.y + tl.h * 0.5f;
    TEST_ASSERT_EQUAL_INT(PS_ACTIVE_CYCLES, ps_hit_test(&d, cx, cy));
}

void test_hit_test_top_right(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    gl_rect_t tr = d.sections[PS_TODAY_FOR_YOU].bounds;
    float cx = tr.x + tr.w * 0.5f;
    float cy = tr.y + tr.h * 0.5f;
    TEST_ASSERT_EQUAL_INT(PS_TODAY_FOR_YOU, ps_hit_test(&d, cx, cy));
}

void test_hit_test_bottom_left(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    gl_rect_t bl = d.sections[PS_FINGERPRINT].bounds;
    float cx = bl.x + bl.w * 0.5f;
    float cy = bl.y + bl.h * 0.5f;
    TEST_ASSERT_EQUAL_INT(PS_FINGERPRINT, ps_hit_test(&d, cx, cy));
}

void test_hit_test_bottom_right(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    gl_rect_t br = d.sections[PS_PROGRESS].bounds;
    float cx = br.x + br.w * 0.5f;
    float cy = br.y + br.h * 0.5f;
    TEST_ASSERT_EQUAL_INT(PS_PROGRESS, ps_hit_test(&d, cx, cy));
}

void test_hit_test_miss(void) {
    /* Point well outside viewport */
    ps_dashboard_t d = ps_compute(full_viewport(), BASE_SPACING);
    TEST_ASSERT_EQUAL_INT(-1, ps_hit_test(&d, -1.0f, -1.0f));
    TEST_ASSERT_EQUAL_INT(-1, ps_hit_test(&d, 2.0f, 2.0f));
}

void test_hit_test_gap_area(void) {
    /* Point in the gap between sections should miss */
    ps_dashboard_t d = ps_compute(full_viewport(), 0.1f); /* large gap */
    gl_rect_t tl = d.sections[PS_ACTIVE_CYCLES].bounds;
    gl_rect_t tr = d.sections[PS_TODAY_FOR_YOU].bounds;
    /* Midpoint of the gap between TL right edge and TR left edge */
    float gap_x = (tl.x + tl.w + tr.x) * 0.5f;
    float gap_y = tl.y + tl.h * 0.5f;
    TEST_ASSERT_EQUAL_INT(-1, ps_hit_test(&d, gap_x, gap_y));
}

/* ===== Edge Cases ===== */

void test_zero_spacing(void) {
    ps_dashboard_t d = ps_compute(full_viewport(), 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, d.gap);
    /* Sections should still have valid bounds */
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        TEST_ASSERT_TRUE(d.sections[i].bounds.w > 0.0f);
        TEST_ASSERT_TRUE(d.sections[i].bounds.h > 0.0f);
    }
}

void test_small_viewport(void) {
    gl_rect_t small = {0.4f, 0.4f, 0.2f, 0.2f};
    ps_dashboard_t d = ps_compute(small, 0.005f);
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        TEST_ASSERT_TRUE(d.sections[i].bounds.w > 0.0f);
        TEST_ASSERT_TRUE(d.sections[i].bounds.h > 0.0f);
    }
}

void test_custom_viewport_offset(void) {
    gl_rect_t vp = {0.2f, 0.3f, 0.6f, 0.4f};
    ps_dashboard_t d = ps_compute(vp, BASE_SPACING);
    /* All sections should be within the viewport */
    for (int i = 0; i < PS_SECTION_COUNT; i++) {
        gl_rect_t b = d.sections[i].bounds;
        TEST_ASSERT_TRUE(b.x >= 0.2f - 0.001f);
        TEST_ASSERT_TRUE(b.y >= 0.3f - 0.001f);
    }
}

void test_section_count_is_four(void) {
    TEST_ASSERT_EQUAL_INT(4, PS_SECTION_COUNT);
}

int main(void) {
    UNITY_BEGIN();

    /* Layout Computation (11) */
    RUN_TEST(test_compute_returns_four_sections);
    RUN_TEST(test_compute_gap_is_phi_derived);
    RUN_TEST(test_compute_container_matches_viewport);
    RUN_TEST(test_top_row_taller_than_bottom);
    RUN_TEST(test_left_column_wider_than_right);
    RUN_TEST(test_top_left_is_largest_section);
    RUN_TEST(test_bottom_right_is_smallest_section);
    RUN_TEST(test_sections_no_overlap);
    RUN_TEST(test_sections_within_viewport);
    RUN_TEST(test_golden_ratio_height_split);
    RUN_TEST(test_golden_ratio_width_split);

    /* Section Titles (3) */
    RUN_TEST(test_section_titles_set);
    RUN_TEST(test_section_title_by_id);
    RUN_TEST(test_section_title_invalid_id);

    /* Section Lookup (2) */
    RUN_TEST(test_ps_section_valid);
    RUN_TEST(test_ps_section_invalid);

    /* Hit Testing (5) */
    RUN_TEST(test_hit_test_top_left);
    RUN_TEST(test_hit_test_top_right);
    RUN_TEST(test_hit_test_bottom_left);
    RUN_TEST(test_hit_test_bottom_right);
    RUN_TEST(test_hit_test_miss);
    RUN_TEST(test_hit_test_gap_area);

    /* Edge Cases (4) */
    RUN_TEST(test_zero_spacing);
    RUN_TEST(test_small_viewport);
    RUN_TEST(test_custom_viewport_offset);
    RUN_TEST(test_section_count_is_four);

    return UNITY_END();
}
