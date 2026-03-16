#include "../unity/unity.h"
#include "../../src/ui/timeline_data.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Known reference: JD 2451545.0 = Jan 1.5 2000 (noon UTC) */
static const double JD_J2000 = 2451545.0;

/* ---- 1-10: tl_auto_granularity — all 10 levels ---- */

void test_auto_gran_hour(void)
{
    /* < 2 days -> HOUR */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_HOUR, tl_auto_granularity(1.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_HOUR, tl_auto_granularity(1.99));
}

void test_auto_gran_day(void)
{
    /* >= 2, < 14 days -> DAY */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_DAY, tl_auto_granularity(2.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_DAY, tl_auto_granularity(13.9));
}

void test_auto_gran_week(void)
{
    /* >= 14, < 60 days -> WEEK */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_WEEK, tl_auto_granularity(14.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_WEEK, tl_auto_granularity(59.0));
}

void test_auto_gran_month(void)
{
    /* >= 60, < 400 days -> MONTH */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_MONTH, tl_auto_granularity(60.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_MONTH, tl_auto_granularity(399.0));
}

void test_auto_gran_year(void)
{
    /* >= 400, < 4000 days -> YEAR */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_YEAR, tl_auto_granularity(400.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_YEAR, tl_auto_granularity(3999.0));
}

void test_auto_gran_decade(void)
{
    /* >= 4000, < 40000 days -> DECADE */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_DECADE, tl_auto_granularity(4000.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_DECADE, tl_auto_granularity(39999.0));
}

void test_auto_gran_century(void)
{
    /* >= 40000, < 400000 days -> CENTURY */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_CENTURY, tl_auto_granularity(40000.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_CENTURY, tl_auto_granularity(399999.0));
}

void test_auto_gran_millennium(void)
{
    /* >= 400000, < 4000000 days -> MILLENNIUM */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_MILLENNIUM, tl_auto_granularity(400000.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_MILLENNIUM, tl_auto_granularity(3999999.0));
}

void test_auto_gran_megayear(void)
{
    /* >= 4000000, < 4e9 days -> MEGAYEAR */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_MEGAYEAR, tl_auto_granularity(4000000.0));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_MEGAYEAR, tl_auto_granularity(3.9e9));
}

void test_auto_gran_gigayear(void)
{
    /* >= 4e9 -> GIGAYEAR */
    TEST_ASSERT_EQUAL_INT(TL_GRAN_GIGAYEAR, tl_auto_granularity(4.0e9));
    TEST_ASSERT_EQUAL_INT(TL_GRAN_GIGAYEAR, tl_auto_granularity(1.0e12));
}

/* ---- 11: tl_granularity_days — step sizes ---- */

void test_granularity_days_values(void)
{
    /* HOUR = 1/24 day */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0 / 24.0, tl_granularity_days(TL_GRAN_HOUR));
    /* DAY = 1 */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, tl_granularity_days(TL_GRAN_DAY));
    /* WEEK = 7 */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 7.0, tl_granularity_days(TL_GRAN_WEEK));
    /* MONTH ~ 30.4375 */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 30.0, tl_granularity_days(TL_GRAN_MONTH));
    /* YEAR ~ 365.25 */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 365.25, tl_granularity_days(TL_GRAN_YEAR));
    /* DECADE ~ 3652.5 */
    TEST_ASSERT_FLOAT_WITHIN(10.0, 3652.5, tl_granularity_days(TL_GRAN_DECADE));
}

/* ---- 12: tl_granularity_name ---- */

void test_granularity_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Hour", tl_granularity_name(TL_GRAN_HOUR));
    TEST_ASSERT_EQUAL_STRING("Day", tl_granularity_name(TL_GRAN_DAY));
    TEST_ASSERT_EQUAL_STRING("Week", tl_granularity_name(TL_GRAN_WEEK));
    TEST_ASSERT_EQUAL_STRING("Month", tl_granularity_name(TL_GRAN_MONTH));
    TEST_ASSERT_EQUAL_STRING("Year", tl_granularity_name(TL_GRAN_YEAR));
    TEST_ASSERT_EQUAL_STRING("Decade", tl_granularity_name(TL_GRAN_DECADE));
    TEST_ASSERT_EQUAL_STRING("Century", tl_granularity_name(TL_GRAN_CENTURY));
    TEST_ASSERT_EQUAL_STRING("Millennium", tl_granularity_name(TL_GRAN_MILLENNIUM));
    TEST_ASSERT_EQUAL_STRING("Megayear", tl_granularity_name(TL_GRAN_MEGAYEAR));
    TEST_ASSERT_EQUAL_STRING("Gigayear", tl_granularity_name(TL_GRAN_GIGAYEAR));
}

/* ---- 13: tl_granularity_name invalid returns Unknown ---- */

void test_granularity_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tl_granularity_name(TL_GRAN_COUNT));
    TEST_ASSERT_EQUAL_STRING("Unknown", tl_granularity_name((tl_granularity_t)-1));
}

/* ---- 14: tl_generate produces ticks for a 1-day range ---- */

void test_generate_day_range_produces_hour_ticks(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 1.0;
    double center = JD_J2000 + 0.5;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_HOUR, tl.granularity);
    TEST_ASSERT_TRUE(tl.tick_count > 0);
    TEST_ASSERT_TRUE(tl.tick_count <= TL_MAX_TICKS);
}

/* ---- 15: tl_generate — tick positions are in [0, 1] and sorted ---- */

void test_generate_tick_positions_sorted(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 7.0;
    double center = JD_J2000 + 3.5;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_TRUE(tl.tick_count > 1);
    for (int i = 0; i < tl.tick_count; i++) {
        TEST_ASSERT_TRUE(tl.ticks[i].position >= 0.0);
        TEST_ASSERT_TRUE(tl.ticks[i].position <= 1.0);
        if (i > 0) {
            TEST_ASSERT_TRUE(tl.ticks[i].position >= tl.ticks[i - 1].position);
        }
    }
}

/* ---- 16: tl_generate year range produces month ticks ---- */

void test_generate_year_range_produces_month_ticks(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 365.25;
    double center = JD_J2000 + 180.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_MONTH, tl.granularity);
    /* A year should produce roughly 12 month ticks */
    TEST_ASSERT_TRUE(tl.tick_count >= 10);
    TEST_ASSERT_TRUE(tl.tick_count <= 15);
}

/* ---- 17: tl_generate_at with explicit granularity ---- */

void test_generate_at_explicit_granularity(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 365.0;
    double center = JD_J2000 + 180.0;
    tl_timeline_t tl = tl_generate_at(start, end, center, TL_GRAN_DAY);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_DAY, tl.granularity);
    /* With TL_MAX_TICKS=128, we get at most 128 day ticks for a year */
    TEST_ASSERT_TRUE(tl.tick_count > 0);
    TEST_ASSERT_TRUE(tl.tick_count <= TL_MAX_TICKS);
}

/* ---- 18: tl_generate millennium range ---- */

void test_generate_millennium_range(void)
{
    /* 730500 days span -> 400000 < 730500 < 4000000 -> MILLENNIUM */
    double start = JD_J2000 - 365250.0;  /* ~1000 years before */
    double end = JD_J2000 + 365250.0;    /* ~1000 years after */
    double center = JD_J2000;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_MILLENNIUM, tl.granularity);
    TEST_ASSERT_TRUE(tl.tick_count > 0);
}

/* ---- 19: tl_zoom in shrinks range ---- */

void test_zoom_in_shrinks_range(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 100.0;
    double center = JD_J2000 + 50.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    double orig_span = tl.jd_end - tl.jd_start;
    tl_timeline_t zoomed = tl_zoom(tl, 0.5); /* zoom in */
    double new_span = zoomed.jd_end - zoomed.jd_start;

    TEST_ASSERT_FLOAT_WITHIN(0.01, orig_span * 0.5, new_span);
    /* Center should remain the same */
    TEST_ASSERT_FLOAT_WITHIN(0.01, center, zoomed.jd_center);
}

/* ---- 20: tl_zoom out expands range ---- */

void test_zoom_out_expands_range(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 100.0;
    double center = JD_J2000 + 50.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    double orig_span = tl.jd_end - tl.jd_start;
    tl_timeline_t zoomed = tl_zoom(tl, 2.0); /* zoom out */
    double new_span = zoomed.jd_end - zoomed.jd_start;

    TEST_ASSERT_FLOAT_WITHIN(0.01, orig_span * 2.0, new_span);
    TEST_ASSERT_FLOAT_WITHIN(0.01, center, zoomed.jd_center);
}

/* ---- 21: tl_pan right moves forward in time ---- */

void test_pan_right_shifts_forward(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 100.0;
    double center = JD_J2000 + 50.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    double span = tl.jd_end - tl.jd_start;
    tl_timeline_t panned = tl_pan(tl, 0.1);

    double shift = span * 0.1;
    TEST_ASSERT_FLOAT_WITHIN(0.01, start + shift, panned.jd_start);
    TEST_ASSERT_FLOAT_WITHIN(0.01, end + shift, panned.jd_end);
    TEST_ASSERT_FLOAT_WITHIN(0.01, center + shift, panned.jd_center);
}

/* ---- 22: tl_pan left shifts backward ---- */

void test_pan_left_shifts_backward(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 100.0;
    double center = JD_J2000 + 50.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    double span = tl.jd_end - tl.jd_start;
    tl_timeline_t panned = tl_pan(tl, -0.25);

    double shift = span * -0.25;
    TEST_ASSERT_FLOAT_WITHIN(0.01, start + shift, panned.jd_start);
    TEST_ASSERT_FLOAT_WITHIN(0.01, end + shift, panned.jd_end);
}

/* ---- 23: tl_nearest_tick ---- */

void test_nearest_tick_finds_closest(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 7.0;
    double center = JD_J2000 + 3.5;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_TRUE(tl.tick_count > 0);

    /* Position 0.5 should find a tick near the middle */
    int idx = tl_nearest_tick(&tl, 0.5);
    TEST_ASSERT_TRUE(idx >= 0);
    TEST_ASSERT_TRUE(idx < tl.tick_count);
    TEST_ASSERT_FLOAT_WITHIN(0.3, 0.5, tl.ticks[idx].position);
}

/* ---- 24: tl_nearest_tick on empty timeline ---- */

void test_nearest_tick_empty_returns_neg1(void)
{
    tl_timeline_t tl;
    memset(&tl, 0, sizeof(tl));
    tl.tick_count = 0;

    TEST_ASSERT_EQUAL_INT(-1, tl_nearest_tick(&tl, 0.5));
}

/* ---- 25: tl_range_label produces non-empty string ---- */

void test_range_label_non_empty(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 30.0;
    double center = JD_J2000 + 15.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    char buf[128];
    int n = tl_range_label(&tl, buf, (int)sizeof(buf));

    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

/* ---- 26: tl_count_by_type ---- */

void test_count_by_type_returns_valid(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 365.0;
    double center = JD_J2000 + 180.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    int minor = tl_count_by_type(&tl, TL_TICK_MINOR);
    int major = tl_count_by_type(&tl, TL_TICK_MAJOR);
    int anchor = tl_count_by_type(&tl, TL_TICK_ANCHOR);

    /* Total should equal tick_count */
    int special = tl_count_by_type(&tl, TL_TICK_SPECIAL);
    TEST_ASSERT_EQUAL_INT(tl.tick_count, minor + major + anchor + special);
}

/* ---- 27: edge case — zero span ---- */

void test_generate_zero_span(void)
{
    tl_timeline_t tl = tl_generate(JD_J2000, JD_J2000, JD_J2000);
    /* Should not crash, tick_count 0 or 1 is acceptable */
    TEST_ASSERT_TRUE(tl.tick_count >= 0);
    TEST_ASSERT_TRUE(tl.tick_count <= TL_MAX_TICKS);
}

/* ---- 28: edge case — huge span (deep time) ---- */

void test_generate_huge_span_deep_time(void)
{
    /* 4.6 billion years in days ~ 1.68e12 */
    double span = 1.68e12;
    double start = JD_J2000 - span;
    double end = JD_J2000;
    tl_timeline_t tl = tl_generate(start, end, JD_J2000 - span / 2.0);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_GIGAYEAR, tl.granularity);
    TEST_ASSERT_TRUE(tl.tick_count > 0);
    TEST_ASSERT_TRUE(tl.tick_count <= TL_MAX_TICKS);
}

/* ---- 29: tl_generate has an anchor tick when center is in range ---- */

void test_generate_has_anchor_tick(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 365.0;
    double center = JD_J2000 + 180.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    int anchor_count = tl_count_by_type(&tl, TL_TICK_ANCHOR);
    TEST_ASSERT_TRUE(anchor_count >= 1);
}

/* ---- 30: tick labels are non-empty for major and anchor ticks ---- */

void test_tick_labels_non_empty(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 365.0;
    double center = JD_J2000 + 180.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    for (int i = 0; i < tl.tick_count; i++) {
        if (tl.ticks[i].type == TL_TICK_MAJOR ||
            tl.ticks[i].type == TL_TICK_ANCHOR) {
            TEST_ASSERT_TRUE(strlen(tl.ticks[i].label) > 0);
        }
    }
}

/* ---- 31: jd_start and jd_end are stored correctly ---- */

void test_generate_stores_range(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 100.0;
    double center = JD_J2000 + 50.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_FLOAT_WITHIN(0.001, start, tl.jd_start);
    TEST_ASSERT_FLOAT_WITHIN(0.001, end, tl.jd_end);
    TEST_ASSERT_FLOAT_WITHIN(0.001, center, tl.jd_center);
}

/* ---- 32: zoom preserves center ---- */

void test_zoom_preserves_center(void)
{
    double start = JD_J2000 - 50.0;
    double end = JD_J2000 + 50.0;
    double center = JD_J2000;
    tl_timeline_t tl = tl_generate(start, end, center);

    tl_timeline_t z = tl_zoom(tl, 0.25);
    TEST_ASSERT_FLOAT_WITHIN(0.001, center, z.jd_center);
    /* New span should be 100 * 0.25 = 25 */
    double new_span = z.jd_end - z.jd_start;
    TEST_ASSERT_FLOAT_WITHIN(0.1, 25.0, new_span);
}

/* ---- 33: pan preserves span ---- */

void test_pan_preserves_span(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 100.0;
    double center = JD_J2000 + 50.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    double orig_span = tl.jd_end - tl.jd_start;
    tl_timeline_t p = tl_pan(tl, 0.5);
    double new_span = p.jd_end - p.jd_start;

    TEST_ASSERT_FLOAT_WITHIN(0.001, orig_span, new_span);
}

/* ---- 34: generate with decade range ---- */

void test_generate_decade_range(void)
{
    /* 50 years ~ 18262 days -> DECADE granularity (4000 < 18262 < 40000) */
    double start = JD_J2000;
    double end = JD_J2000 + 18262.0;
    double center = JD_J2000 + 9131.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_DECADE, tl.granularity);
    TEST_ASSERT_TRUE(tl.tick_count > 0);
}

/* ---- 35: megayear label contains "Ma" ---- */

void test_megayear_labels_contain_ma(void)
{
    /* ~5 million years in days: 5e6 * 365.25 = 1.826e9 (< 4e9, MEGAYEAR) */
    double span = 5.0e6 * 365.25;
    double start = JD_J2000 - span;
    double end = JD_J2000;
    tl_timeline_t tl = tl_generate(start, end, JD_J2000 - span / 2.0);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_MEGAYEAR, tl.granularity);
    /* At least one tick should have "Ma" in the label */
    int found_ma = 0;
    for (int i = 0; i < tl.tick_count; i++) {
        if (strstr(tl.ticks[i].label, "Ma") != NULL) {
            found_ma = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_ma);
}

/* ---- 36: gigayear label contains "Ga" ---- */

void test_gigayear_labels_contain_ga(void)
{
    double span = 5.0e9 * 365.25;
    double start = JD_J2000 - span;
    double end = JD_J2000;
    tl_timeline_t tl = tl_generate(start, end, JD_J2000 - span / 2.0);

    TEST_ASSERT_EQUAL_INT(TL_GRAN_GIGAYEAR, tl.granularity);
    int found_ga = 0;
    for (int i = 0; i < tl.tick_count; i++) {
        if (strstr(tl.ticks[i].label, "Ga") != NULL) {
            found_ga = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found_ga);
}

/* ---- 37: swapped start/end handled gracefully ---- */

void test_generate_swapped_range(void)
{
    /* start > end: should handle gracefully (swap or zero ticks) */
    tl_timeline_t tl = tl_generate(JD_J2000 + 100.0, JD_J2000, JD_J2000 + 50.0);
    TEST_ASSERT_TRUE(tl.tick_count >= 0);
    TEST_ASSERT_TRUE(tl.tick_count <= TL_MAX_TICKS);
}

/* ---- 38: all tick JDs are within range ---- */

void test_tick_jds_within_range(void)
{
    double start = JD_J2000;
    double end = JD_J2000 + 30.0;
    double center = JD_J2000 + 15.0;
    tl_timeline_t tl = tl_generate(start, end, center);

    for (int i = 0; i < tl.tick_count; i++) {
        /* JDs should be at or very near the range */
        TEST_ASSERT_TRUE(tl.ticks[i].jd >= start - 1.0);
        TEST_ASSERT_TRUE(tl.ticks[i].jd <= end + 1.0);
    }
}

/* ---- Main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Auto granularity: 1-10 */
    RUN_TEST(test_auto_gran_hour);
    RUN_TEST(test_auto_gran_day);
    RUN_TEST(test_auto_gran_week);
    RUN_TEST(test_auto_gran_month);
    RUN_TEST(test_auto_gran_year);
    RUN_TEST(test_auto_gran_decade);
    RUN_TEST(test_auto_gran_century);
    RUN_TEST(test_auto_gran_millennium);
    RUN_TEST(test_auto_gran_megayear);
    RUN_TEST(test_auto_gran_gigayear);

    /* Granularity helpers: 11-13 */
    RUN_TEST(test_granularity_days_values);
    RUN_TEST(test_granularity_names);
    RUN_TEST(test_granularity_name_invalid);

    /* Generate: 14-18 */
    RUN_TEST(test_generate_day_range_produces_hour_ticks);
    RUN_TEST(test_generate_tick_positions_sorted);
    RUN_TEST(test_generate_year_range_produces_month_ticks);
    RUN_TEST(test_generate_at_explicit_granularity);
    RUN_TEST(test_generate_millennium_range);

    /* Zoom: 19-20 */
    RUN_TEST(test_zoom_in_shrinks_range);
    RUN_TEST(test_zoom_out_expands_range);

    /* Pan: 21-22 */
    RUN_TEST(test_pan_right_shifts_forward);
    RUN_TEST(test_pan_left_shifts_backward);

    /* Nearest tick: 23-24 */
    RUN_TEST(test_nearest_tick_finds_closest);
    RUN_TEST(test_nearest_tick_empty_returns_neg1);

    /* Range label: 25 */
    RUN_TEST(test_range_label_non_empty);

    /* Count by type: 26 */
    RUN_TEST(test_count_by_type_returns_valid);

    /* Edge cases: 27-28 */
    RUN_TEST(test_generate_zero_span);
    RUN_TEST(test_generate_huge_span_deep_time);

    /* Anchor: 29-30 */
    RUN_TEST(test_generate_has_anchor_tick);
    RUN_TEST(test_tick_labels_non_empty);

    /* Range storage: 31 */
    RUN_TEST(test_generate_stores_range);

    /* Zoom/pan details: 32-33 */
    RUN_TEST(test_zoom_preserves_center);
    RUN_TEST(test_pan_preserves_span);

    /* Additional coverage: 34-38 */
    RUN_TEST(test_generate_decade_range);
    RUN_TEST(test_megayear_labels_contain_ma);
    RUN_TEST(test_gigayear_labels_contain_ga);
    RUN_TEST(test_generate_swapped_range);
    RUN_TEST(test_tick_jds_within_range);

    return UNITY_END();
}
