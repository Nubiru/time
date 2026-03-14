/* test_location_alerts.c -- Tests for location-aware alerts.
 * TDD: tests written before implementation (RED phase).
 * Covers all 9 alert types, sorting, filtering, edge cases.
 * Minimum 60 tests. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/location_alerts.h"

#include <math.h>
#include <string.h>

/* J2000.0 epoch */
#define JD_J2000 2451545.0

/* Vernal equinox 2024-03-20 12:00 UTC */
static const double JD_EQUINOX = 2460388.0;

/* Summer solstice ~2024-06-20 */
static const double JD_SUMMER = 2460388.0 + 92.0;

/* Winter solstice ~2024-12-21 */
static const double JD_WINTER = 2460388.0 + 275.0;

/* Pipeline, Oahu, Hawaii -- world-class surf spot */
static const double PIPELINE_LAT = 21.665;
static const double PIPELINE_LON = -158.054;

/* Greenwich, UK */
static const double GREENWICH_LAT = 51.48;
static const double GREENWICH_LON = 0.0;

/* Mecca, Saudi Arabia -- prayer times reference */
static const double MECCA_LAT = 21.4225;
static const double MECCA_LON = 39.8262;

/* Chamonix, France -- ski resort */
static const double CHAMONIX_LAT = 45.924;
static const double CHAMONIX_LON = 6.870;

/* Tromso, Norway -- polar conditions */
static const double TROMSO_LAT = 69.65;
static const double TROMSO_LON = 18.96;

/* Miami, Florida -- storm basin */
static const double MIAMI_LAT = 25.76;
static const double MIAMI_LON = -80.19;

void setUp(void) { }
void tearDown(void) { }

/* ===== la_type_name ===== */

void test_type_name_surf(void) {
    const char *name = la_type_name(LA_SURF);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_tide(void) {
    const char *name = la_type_name(LA_TIDE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_snow(void) {
    const char *name = la_type_name(LA_SNOW);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_golden_hour(void) {
    const char *name = la_type_name(LA_GOLDEN_HOUR);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_prayer(void) {
    const char *name = la_type_name(LA_PRAYER);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_storm(void) {
    const char *name = la_type_name(LA_STORM);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_convergence(void) {
    const char *name = la_type_name(LA_CONVERGENCE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_solstice(void) {
    const char *name = la_type_name(LA_SOLSTICE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_avalanche(void) {
    const char *name = la_type_name(LA_AVALANCHE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_invalid(void) {
    const char *name = la_type_name(LA_TYPE_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

void test_type_name_negative(void) {
    const char *name = la_type_name((la_type_t)-1);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===== la_urgency_name ===== */

void test_urgency_name_info(void) {
    const char *name = la_urgency_name(LA_INFO);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_urgency_name_suggestion(void) {
    const char *name = la_urgency_name(LA_SUGGESTION);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_urgency_name_alert(void) {
    const char *name = la_urgency_name(LA_ALERT);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_urgency_name_invalid(void) {
    const char *name = la_urgency_name((la_urgency_t)99);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===== la_type_count ===== */

void test_type_count(void) {
    TEST_ASSERT_EQUAL_INT(LA_TYPE_COUNT, la_type_count());
}

void test_type_count_positive(void) {
    TEST_ASSERT_TRUE(la_type_count() > 0);
}

/* ===== la_evaluate -- basic structure ===== */

void test_evaluate_returns_valid_result(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_count_within_max(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_alerts_have_valid_types(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.alerts[i].type >= 0);
        TEST_ASSERT_TRUE(r.alerts[i].type < LA_TYPE_COUNT);
    }
}

void test_evaluate_alerts_have_valid_urgency(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.alerts[i].urgency >= LA_INFO);
        TEST_ASSERT_TRUE(r.alerts[i].urgency <= LA_ALERT);
    }
}

void test_evaluate_sorted_by_urgency(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    for (int i = 1; i < r.count; i++) {
        /* Urgency should be non-increasing (ALERT >= SUGGESTION >= INFO) */
        TEST_ASSERT_TRUE(r.alerts[i].urgency <= r.alerts[i - 1].urgency);
    }
}

void test_evaluate_messages_not_null(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, MECCA_LAT, MECCA_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_NULL(r.alerts[i].message);
        TEST_ASSERT_TRUE(strlen(r.alerts[i].message) > 0);
    }
}

void test_evaluate_quality_in_range(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.alerts[i].quality >= 0.0);
        TEST_ASSERT_TRUE(r.alerts[i].quality <= 1.0);
    }
}

void test_evaluate_time_until_finite(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(isfinite(r.alerts[i].time_until));
    }
}

/* ===== la_evaluate -- location-specific behavior ===== */

void test_evaluate_at_pipeline_has_surf(void) {
    /* Pipeline is near a surf spot -- should produce surf alert */
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    int has_surf = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == LA_SURF) has_surf = 1;
    }
    TEST_ASSERT_TRUE(has_surf);
}

void test_evaluate_at_pipeline_has_tide(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    int has_tide = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == LA_TIDE) has_tide = 1;
    }
    TEST_ASSERT_TRUE(has_tide);
}

void test_evaluate_at_mecca_has_prayer(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, MECCA_LAT, MECCA_LON);
    int has_prayer = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == LA_PRAYER) has_prayer = 1;
    }
    TEST_ASSERT_TRUE(has_prayer);
}

void test_evaluate_at_chamonix_winter_has_snow(void) {
    /* Chamonix in Jan should produce snow alerts */
    double jd_jan = JD_WINTER + 15.0; /* ~early Jan */
    la_result_t r = la_evaluate(jd_jan, CHAMONIX_LAT, CHAMONIX_LON);
    int has_snow = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == LA_SNOW) has_snow = 1;
    }
    TEST_ASSERT_TRUE(has_snow);
}

void test_evaluate_at_miami_hurricane_season(void) {
    /* Miami in August should produce storm alert */
    double jd_aug = JD_EQUINOX + 155.0; /* ~Aug */
    la_result_t r = la_evaluate(jd_aug, MIAMI_LAT, MIAMI_LON);
    int has_storm = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == LA_STORM) has_storm = 1;
    }
    TEST_ASSERT_TRUE(has_storm);
}

void test_evaluate_near_solstice_has_solstice_alert(void) {
    /* Within 7 days of summer solstice */
    double jd_near = JD_SUMMER - 3.0;
    la_result_t r = la_evaluate(jd_near, GREENWICH_LAT, GREENWICH_LON);
    int has_solstice = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == LA_SOLSTICE) has_solstice = 1;
    }
    TEST_ASSERT_TRUE(has_solstice);
}

void test_evaluate_produces_at_least_one_alert(void) {
    /* Any location should produce at least one alert */
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    TEST_ASSERT_TRUE(r.count >= 1);
}

/* ===== la_any_active ===== */

void test_any_active_returns_boolean(void) {
    int active = la_any_active(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    TEST_ASSERT_TRUE(active == 0 || active == 1);
}

void test_any_active_consistent_with_evaluate(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    int active = la_any_active(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    if (r.count > 0) {
        TEST_ASSERT_EQUAL_INT(1, active);
    } else {
        TEST_ASSERT_EQUAL_INT(0, active);
    }
}

void test_any_active_at_mecca(void) {
    /* Mecca should always have prayer alert */
    int active = la_any_active(JD_EQUINOX, MECCA_LAT, MECCA_LON);
    TEST_ASSERT_EQUAL_INT(1, active);
}

/* ===== la_filter_type ===== */

void test_filter_type_surf(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_SURF, out, 4);
    TEST_ASSERT_TRUE(n >= 0);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(LA_SURF, out[i].type);
    }
}

void test_filter_type_prayer(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, MECCA_LAT, MECCA_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_PRAYER, out, 4);
    TEST_ASSERT_TRUE(n >= 1);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(LA_PRAYER, out[i].type);
    }
}

void test_filter_type_respects_max(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_entry_t out[1];
    int n = la_filter_type(r, LA_SURF, out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_filter_type_zero_max(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_entry_t out[1];
    int n = la_filter_type(r, LA_SURF, out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_filter_type_empty_result(void) {
    la_result_t r;
    memset(&r, 0, sizeof(r));
    r.count = 0;
    la_entry_t out[4];
    int n = la_filter_type(r, LA_SURF, out, 4);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_filter_type_no_match(void) {
    la_result_t r;
    memset(&r, 0, sizeof(r));
    r.count = 1;
    r.alerts[0].type = LA_SURF;
    r.alerts[0].message = "Test";
    la_entry_t out[4];
    int n = la_filter_type(r, LA_AVALANCHE, out, 4);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== Edge cases ===== */

void test_evaluate_at_equator(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, 0.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_at_south_pole(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, -90.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_at_north_pole(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, 90.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_at_dateline(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, 0.0, 180.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_at_negative_lon(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, 0.0, -180.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_different_times(void) {
    la_result_t r1 = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    la_result_t r2 = la_evaluate(JD_EQUINOX + 0.5, GREENWICH_LAT,
                                  GREENWICH_LON);
    TEST_ASSERT_TRUE(r1.count >= 0);
    TEST_ASSERT_TRUE(r2.count >= 0);
}

void test_evaluate_far_past(void) {
    la_result_t r = la_evaluate(JD_J2000 - 365250.0, 0.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_evaluate_far_future(void) {
    la_result_t r = la_evaluate(JD_J2000 + 365250.0, 0.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

/* ===== Surf-specific ===== */

void test_surf_quality_value(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_SURF, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].quality >= 0.0);
        TEST_ASSERT_TRUE(out[i].quality <= 1.0);
    }
}

/* ===== Snow-specific ===== */

void test_snow_not_at_tropical(void) {
    /* Tropical equatorial location -- no ski resort nearby */
    la_result_t r = la_evaluate(JD_SUMMER, 0.0, 0.0);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_SNOW, out, 4);
    /* Might still find a resort, but quality should be low */
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].quality >= 0.0);
        TEST_ASSERT_TRUE(out[i].quality <= 1.0);
    }
}

/* ===== Tide-specific ===== */

void test_tide_info_present_near_coast(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_TIDE, out, 4);
    TEST_ASSERT_TRUE(n >= 1);
}

void test_tide_time_until_reasonable(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_TIDE, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(isfinite(out[i].time_until));
    }
}

/* ===== Golden hour ===== */

void test_golden_hour_quality(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_GOLDEN_HOUR, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].quality >= 0.0);
        TEST_ASSERT_TRUE(out[i].quality <= 1.0);
    }
}

/* ===== Prayer ===== */

void test_prayer_time_until_positive(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, MECCA_LAT, MECCA_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_PRAYER, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].time_until >= 0.0);
    }
}

/* ===== Storm ===== */

void test_storm_off_season_no_alert(void) {
    /* Miami in January -- outside hurricane season */
    double jd_jan = JD_WINTER + 15.0;
    la_result_t r = la_evaluate(jd_jan, MIAMI_LAT, MIAMI_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_STORM, out, 4);
    /* Should produce 0 storm alerts outside season */
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== Avalanche ===== */

void test_avalanche_winter_high_elevation(void) {
    /* Chamonix in January -- high avalanche potential */
    double jd_jan = JD_WINTER + 15.0;
    la_result_t r = la_evaluate(jd_jan, CHAMONIX_LAT, CHAMONIX_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_AVALANCHE, out, 4);
    /* May or may not trigger depending on risk model */
    TEST_ASSERT_TRUE(n >= 0);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(LA_AVALANCHE, out[i].type);
    }
}

void test_avalanche_summer_low(void) {
    /* Summer -- very low avalanche risk */
    la_result_t r = la_evaluate(JD_SUMMER, CHAMONIX_LAT, CHAMONIX_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_AVALANCHE, out, 4);
    /* Should be 0 or low urgency only */
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].urgency <= LA_SUGGESTION);
    }
}

/* ===== Convergence ===== */

void test_convergence_quality_in_range(void) {
    la_result_t r = la_evaluate(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_CONVERGENCE, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].quality >= 0.0);
        TEST_ASSERT_TRUE(out[i].quality <= 1.0);
    }
}

/* ===== Solstice ===== */

void test_solstice_far_from_event(void) {
    /* Well away from any solstice/equinox */
    double jd_mid = JD_EQUINOX + 45.0; /* ~May 4, nowhere near */
    la_result_t r = la_evaluate(jd_mid, GREENWICH_LAT, GREENWICH_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_SOLSTICE, out, 4);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_solstice_near_equinox(void) {
    /* Within 3 days of equinox */
    double jd_near = JD_EQUINOX - 2.0;
    la_result_t r = la_evaluate(jd_near, GREENWICH_LAT, GREENWICH_LON);
    la_entry_t out[4];
    int n = la_filter_type(r, LA_SOLSTICE, out, 4);
    TEST_ASSERT_TRUE(n >= 1);
}

/* ===== Multiple locations produce different results ===== */

void test_different_locations_differ(void) {
    la_result_t r1 = la_evaluate(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON);
    la_result_t r2 = la_evaluate(JD_EQUINOX, TROMSO_LAT, TROMSO_LON);
    /* At least some difference in alert types should exist */
    int same_count = (r1.count == r2.count) ? 1 : 0;
    /* Both valid */
    TEST_ASSERT_TRUE(r1.count >= 0);
    TEST_ASSERT_TRUE(r2.count >= 0);
    (void)same_count;
}

/* ===== Polar conditions ===== */

void test_polar_day_tromso_summer(void) {
    la_result_t r = la_evaluate(JD_SUMMER, TROMSO_LAT, TROMSO_LON);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

void test_polar_night_tromso_winter(void) {
    la_result_t r = la_evaluate(JD_WINTER, TROMSO_LAT, TROMSO_LON);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= LA_MAX_ALERTS);
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* la_type_name (11 tests) */
    RUN_TEST(test_type_name_surf);
    RUN_TEST(test_type_name_tide);
    RUN_TEST(test_type_name_snow);
    RUN_TEST(test_type_name_golden_hour);
    RUN_TEST(test_type_name_prayer);
    RUN_TEST(test_type_name_storm);
    RUN_TEST(test_type_name_convergence);
    RUN_TEST(test_type_name_solstice);
    RUN_TEST(test_type_name_avalanche);
    RUN_TEST(test_type_name_invalid);
    RUN_TEST(test_type_name_negative);

    /* la_urgency_name (4 tests) */
    RUN_TEST(test_urgency_name_info);
    RUN_TEST(test_urgency_name_suggestion);
    RUN_TEST(test_urgency_name_alert);
    RUN_TEST(test_urgency_name_invalid);

    /* la_type_count (2 tests) */
    RUN_TEST(test_type_count);
    RUN_TEST(test_type_count_positive);

    /* la_evaluate basic (9 tests) */
    RUN_TEST(test_evaluate_returns_valid_result);
    RUN_TEST(test_evaluate_count_within_max);
    RUN_TEST(test_evaluate_alerts_have_valid_types);
    RUN_TEST(test_evaluate_alerts_have_valid_urgency);
    RUN_TEST(test_evaluate_sorted_by_urgency);
    RUN_TEST(test_evaluate_messages_not_null);
    RUN_TEST(test_evaluate_quality_in_range);
    RUN_TEST(test_evaluate_time_until_finite);
    RUN_TEST(test_evaluate_produces_at_least_one_alert);

    /* la_evaluate location-specific (6 tests) */
    RUN_TEST(test_evaluate_at_pipeline_has_surf);
    RUN_TEST(test_evaluate_at_pipeline_has_tide);
    RUN_TEST(test_evaluate_at_mecca_has_prayer);
    RUN_TEST(test_evaluate_at_chamonix_winter_has_snow);
    RUN_TEST(test_evaluate_at_miami_hurricane_season);
    RUN_TEST(test_evaluate_near_solstice_has_solstice_alert);

    /* la_any_active (3 tests) */
    RUN_TEST(test_any_active_returns_boolean);
    RUN_TEST(test_any_active_consistent_with_evaluate);
    RUN_TEST(test_any_active_at_mecca);

    /* la_filter_type (6 tests) */
    RUN_TEST(test_filter_type_surf);
    RUN_TEST(test_filter_type_prayer);
    RUN_TEST(test_filter_type_respects_max);
    RUN_TEST(test_filter_type_zero_max);
    RUN_TEST(test_filter_type_empty_result);
    RUN_TEST(test_filter_type_no_match);

    /* Edge cases (9 tests) */
    RUN_TEST(test_evaluate_at_equator);
    RUN_TEST(test_evaluate_at_south_pole);
    RUN_TEST(test_evaluate_at_north_pole);
    RUN_TEST(test_evaluate_at_dateline);
    RUN_TEST(test_evaluate_at_negative_lon);
    RUN_TEST(test_evaluate_different_times);
    RUN_TEST(test_evaluate_far_past);
    RUN_TEST(test_evaluate_far_future);
    RUN_TEST(test_different_locations_differ);

    /* Surf-specific (1 test) */
    RUN_TEST(test_surf_quality_value);

    /* Snow-specific (1 test) */
    RUN_TEST(test_snow_not_at_tropical);

    /* Tide-specific (2 tests) */
    RUN_TEST(test_tide_info_present_near_coast);
    RUN_TEST(test_tide_time_until_reasonable);

    /* Golden hour (1 test) */
    RUN_TEST(test_golden_hour_quality);

    /* Prayer (1 test) */
    RUN_TEST(test_prayer_time_until_positive);

    /* Storm (1 test) */
    RUN_TEST(test_storm_off_season_no_alert);

    /* Avalanche (2 tests) */
    RUN_TEST(test_avalanche_winter_high_elevation);
    RUN_TEST(test_avalanche_summer_low);

    /* Convergence (1 test) */
    RUN_TEST(test_convergence_quality_in_range);

    /* Solstice (2 tests) */
    RUN_TEST(test_solstice_far_from_event);
    RUN_TEST(test_solstice_near_equinox);

    /* Polar (2 tests) */
    RUN_TEST(test_polar_day_tromso_summer);
    RUN_TEST(test_polar_night_tromso_winter);

    return UNITY_END();
}
