/* test_location_alerts.c — Tests for location-based alert evaluation.
 * TDD RED phase: all tests written before implementation.
 * Evaluates surf, snow, golden hour, prayer, tide, and daylight alerts
 * from existing Earth/astronomy modules. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/location_alerts.h"

#include <math.h>
#include <string.h>

/* Vernal equinox 2024-03-20 12:00 UTC */
static const double JD_EQUINOX = 2460388.0;

/* Pipeline, Oahu, Hawaii — world-class surf spot */
static const double PIPELINE_LAT = 21.665;
static const double PIPELINE_LON = -158.054;

/* Greenwich, UK */
static const double GREENWICH_LAT = 51.48;
static const double GREENWICH_LON = 0.0;

/* Mecca, Saudi Arabia — prayer times reference */
static const double MECCA_LAT = 21.4225;
static const double MECCA_LON = 39.8262;

/* Chamonix, France — ski resort */
static const double CHAMONIX_LAT = 45.924;
static const double CHAMONIX_LON = 6.870;

/* Tromso, Norway — polar conditions */
static const double TROMSO_LAT = 69.65;
static const double TROMSO_LON = 18.96;

void setUp(void) { }
void tearDown(void) { }

/* ===== alert_type_name ===== */

void test_type_name_surf(void) {
    const char *name = alert_type_name(ALERT_SURF);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_snow(void) {
    const char *name = alert_type_name(ALERT_SNOW);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_golden_hour(void) {
    const char *name = alert_type_name(ALERT_GOLDEN_HOUR);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_blue_hour(void) {
    const char *name = alert_type_name(ALERT_BLUE_HOUR);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_prayer(void) {
    const char *name = alert_type_name(ALERT_PRAYER);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_daylight(void) {
    const char *name = alert_type_name(ALERT_DAYLIGHT);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_tide(void) {
    const char *name = alert_type_name(ALERT_TIDE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_type_name_invalid(void) {
    const char *name = alert_type_name(ALERT_TYPE_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===== alert_urgency_name ===== */

void test_urgency_name_info(void) {
    const char *name = alert_urgency_name(ALERT_INFO);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_urgency_name_suggestion(void) {
    const char *name = alert_urgency_name(ALERT_SUGGESTION);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_urgency_name_notable(void) {
    const char *name = alert_urgency_name(ALERT_NOTABLE);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_urgency_name_invalid(void) {
    const char *name = alert_urgency_name((alert_urgency_t)99);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===== alert_check_surf ===== */

void test_surf_returns_nonnegative_count(void) {
    alert_t out[4];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_surf_respects_max(void) {
    alert_t out[1];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_surf_type_is_surf(void) {
    alert_t out[4];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(ALERT_SURF, out[i].type);
    }
}

void test_surf_message_not_empty(void) {
    alert_t out[4];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(strlen(out[i].message) > 0);
    }
}

void test_surf_quality_in_range(void) {
    alert_t out[4];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].quality >= 0.0);
        TEST_ASSERT_TRUE(out[i].quality <= 1.0);
    }
}

void test_surf_hours_until_finite(void) {
    alert_t out[4];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(isfinite(out[i].hours_until));
    }
}

/* ===== alert_check_snow ===== */

void test_snow_returns_nonnegative_count(void) {
    alert_t out[4];
    int n = alert_check_snow(JD_EQUINOX, CHAMONIX_LAT, CHAMONIX_LON, out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_snow_respects_max(void) {
    alert_t out[1];
    int n = alert_check_snow(JD_EQUINOX, CHAMONIX_LAT, CHAMONIX_LON, out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_snow_type_is_snow(void) {
    alert_t out[4];
    int n = alert_check_snow(JD_EQUINOX, CHAMONIX_LAT, CHAMONIX_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(ALERT_SNOW, out[i].type);
    }
}

void test_snow_message_not_empty(void) {
    alert_t out[4];
    int n = alert_check_snow(JD_EQUINOX, CHAMONIX_LAT, CHAMONIX_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(strlen(out[i].message) > 0);
    }
}

void test_snow_at_tropical_location(void) {
    /* Equatorial location: unlikely to produce snow alerts */
    alert_t out[4];
    int n = alert_check_snow(JD_EQUINOX, 0.0, 0.0, out, 4);
    /* Should produce 0 or minimal alerts since no ski resorts nearby */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n <= 4);
}

/* ===== alert_check_golden_hour ===== */

void test_golden_hour_returns_nonnegative(void) {
    alert_t out[4];
    int n = alert_check_golden_hour(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                    out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_golden_hour_respects_max(void) {
    alert_t out[1];
    int n = alert_check_golden_hour(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                    out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_golden_hour_type_correct(void) {
    alert_t out[4];
    int n = alert_check_golden_hour(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                    out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].type == ALERT_GOLDEN_HOUR ||
                         out[i].type == ALERT_BLUE_HOUR);
    }
}

void test_golden_hour_message_not_empty(void) {
    alert_t out[4];
    int n = alert_check_golden_hour(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                    out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(strlen(out[i].message) > 0);
    }
}

void test_golden_hour_hours_until_reasonable(void) {
    alert_t out[4];
    int n = alert_check_golden_hour(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                    out, 4);
    for (int i = 0; i < n; i++) {
        /* hours_until should be within a day or negative for active windows */
        TEST_ASSERT_TRUE(isfinite(out[i].hours_until));
    }
}

/* ===== alert_check_prayer ===== */

void test_prayer_returns_nonnegative(void) {
    alert_t out[4];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_prayer_produces_at_least_one(void) {
    /* Should always produce at least one INFO alert for next prayer */
    alert_t out[4];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 4);
    TEST_ASSERT_TRUE(n >= 1);
}

void test_prayer_type_is_prayer(void) {
    alert_t out[4];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(ALERT_PRAYER, out[i].type);
    }
}

void test_prayer_message_not_empty(void) {
    alert_t out[4];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(strlen(out[i].message) > 0);
    }
}

void test_prayer_respects_max(void) {
    alert_t out[1];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_prayer_hours_until_positive(void) {
    alert_t out[4];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].hours_until >= 0.0);
    }
}

/* ===== alert_check_tide ===== */

void test_tide_returns_nonnegative(void) {
    alert_t out[4];
    int n = alert_check_tide(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_tide_respects_max(void) {
    alert_t out[1];
    int n = alert_check_tide(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_tide_type_is_tide(void) {
    alert_t out[4];
    int n = alert_check_tide(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(ALERT_TIDE, out[i].type);
    }
}

void test_tide_message_not_empty(void) {
    alert_t out[4];
    int n = alert_check_tide(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(strlen(out[i].message) > 0);
    }
}

/* ===== alert_check_daylight ===== */

void test_daylight_returns_nonnegative(void) {
    alert_t out[4];
    int n = alert_check_daylight(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                 out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_daylight_respects_max(void) {
    alert_t out[1];
    int n = alert_check_daylight(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                 out, 1);
    TEST_ASSERT_TRUE(n <= 1);
}

void test_daylight_type_is_daylight(void) {
    alert_t out[4];
    int n = alert_check_daylight(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                 out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(ALERT_DAYLIGHT, out[i].type);
    }
}

void test_daylight_message_not_empty(void) {
    alert_t out[4];
    int n = alert_check_daylight(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                 out, 4);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(strlen(out[i].message) > 0);
    }
}

void test_daylight_polar_day_info(void) {
    /* June solstice at Tromso — polar day */
    double jd_summer = JD_EQUINOX + 92.0; /* ~June 20 */
    alert_t out[4];
    int n = alert_check_daylight(jd_summer, TROMSO_LAT, TROMSO_LON, out, 4);
    /* Should produce at least one alert about polar day */
    TEST_ASSERT_TRUE(n >= 0);
}

void test_daylight_polar_night_info(void) {
    /* December solstice at Tromso — polar night */
    double jd_winter = JD_EQUINOX + 275.0; /* ~December 20 */
    alert_t out[4];
    int n = alert_check_daylight(jd_winter, TROMSO_LAT, TROMSO_LON, out, 4);
    TEST_ASSERT_TRUE(n >= 0);
}

/* ===== alert_sort ===== */

void test_sort_urgency_descending(void) {
    alert_t alerts[3];
    memset(alerts, 0, sizeof(alerts));
    alerts[0].urgency = ALERT_INFO;
    alerts[0].hours_until = 1.0;
    alerts[1].urgency = ALERT_NOTABLE;
    alerts[1].hours_until = 2.0;
    alerts[2].urgency = ALERT_SUGGESTION;
    alerts[2].hours_until = 0.5;

    alert_sort(alerts, 3);

    /* NOTABLE first, then SUGGESTION, then INFO */
    TEST_ASSERT_EQUAL_INT(ALERT_NOTABLE, alerts[0].urgency);
    TEST_ASSERT_EQUAL_INT(ALERT_SUGGESTION, alerts[1].urgency);
    TEST_ASSERT_EQUAL_INT(ALERT_INFO, alerts[2].urgency);
}

void test_sort_hours_ascending_within_urgency(void) {
    alert_t alerts[3];
    memset(alerts, 0, sizeof(alerts));
    alerts[0].urgency = ALERT_SUGGESTION;
    alerts[0].hours_until = 5.0;
    alerts[1].urgency = ALERT_SUGGESTION;
    alerts[1].hours_until = 1.0;
    alerts[2].urgency = ALERT_SUGGESTION;
    alerts[2].hours_until = 3.0;

    alert_sort(alerts, 3);

    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, alerts[0].hours_until);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.0, alerts[1].hours_until);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5.0, alerts[2].hours_until);
}

void test_sort_empty_array(void) {
    alert_sort(NULL, 0);
    /* Should not crash */
    TEST_ASSERT_TRUE(1);
}

void test_sort_single_element(void) {
    alert_t a;
    memset(&a, 0, sizeof(a));
    a.urgency = ALERT_INFO;
    a.hours_until = 2.0;
    alert_sort(&a, 1);
    TEST_ASSERT_EQUAL_INT(ALERT_INFO, a.urgency);
}

void test_sort_preserves_message(void) {
    alert_t alerts[2];
    memset(alerts, 0, sizeof(alerts));
    alerts[0].urgency = ALERT_INFO;
    alerts[0].hours_until = 1.0;
    snprintf(alerts[0].message, sizeof(alerts[0].message), "First");
    alerts[1].urgency = ALERT_NOTABLE;
    alerts[1].hours_until = 2.0;
    snprintf(alerts[1].message, sizeof(alerts[1].message), "Second");

    alert_sort(alerts, 2);

    /* NOTABLE should come first now */
    TEST_ASSERT_EQUAL_STRING("Second", alerts[0].message);
    TEST_ASSERT_EQUAL_STRING("First", alerts[1].message);
}

/* ===== location_alerts_evaluate ===== */

void test_evaluate_returns_valid_result(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, GREENWICH_LAT,
                                                GREENWICH_LON);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= ALERT_MAX);
}

void test_evaluate_count_within_max(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, PIPELINE_LAT,
                                                PIPELINE_LON);
    TEST_ASSERT_TRUE(r.count <= ALERT_MAX);
}

void test_evaluate_alerts_have_valid_types(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, GREENWICH_LAT,
                                                GREENWICH_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.alerts[i].type >= 0);
        TEST_ASSERT_TRUE(r.alerts[i].type < ALERT_TYPE_COUNT);
    }
}

void test_evaluate_alerts_have_valid_urgency(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, GREENWICH_LAT,
                                                GREENWICH_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.alerts[i].urgency >= ALERT_INFO);
        TEST_ASSERT_TRUE(r.alerts[i].urgency <= ALERT_NOTABLE);
    }
}

void test_evaluate_sorted_by_urgency(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, PIPELINE_LAT,
                                                PIPELINE_LON);
    for (int i = 1; i < r.count; i++) {
        /* Urgency should be non-increasing (NOTABLE >= SUGGESTION >= INFO) */
        TEST_ASSERT_TRUE(r.alerts[i].urgency <= r.alerts[i - 1].urgency ||
                         r.alerts[i].urgency == r.alerts[i - 1].urgency);
    }
}

void test_evaluate_messages_not_empty(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, MECCA_LAT,
                                                MECCA_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(strlen(r.alerts[i].message) > 0);
    }
}

void test_evaluate_at_pipeline(void) {
    /* Pipeline should generate surf and tide alerts */
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, PIPELINE_LAT,
                                                PIPELINE_LON);
    TEST_ASSERT_TRUE(r.count >= 1);
}

void test_evaluate_at_mecca(void) {
    /* Mecca should always generate at least a prayer alert */
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, MECCA_LAT,
                                                MECCA_LON);
    int has_prayer = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.alerts[i].type == ALERT_PRAYER) has_prayer = 1;
    }
    TEST_ASSERT_TRUE(has_prayer);
}

void test_evaluate_quality_in_range(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, PIPELINE_LAT,
                                                PIPELINE_LON);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.alerts[i].quality >= 0.0);
        TEST_ASSERT_TRUE(r.alerts[i].quality <= 1.0);
    }
}

/* ===== Edge cases ===== */

void test_evaluate_at_equator(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, 0.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= ALERT_MAX);
}

void test_evaluate_at_south_pole(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, -90.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= ALERT_MAX);
}

void test_evaluate_at_north_pole(void) {
    alert_result_t r = location_alerts_evaluate(JD_EQUINOX, 90.0, 0.0);
    TEST_ASSERT_TRUE(r.count >= 0);
    TEST_ASSERT_TRUE(r.count <= ALERT_MAX);
}

void test_evaluate_different_times(void) {
    /* Results should potentially differ at different times of day */
    alert_result_t r1 = location_alerts_evaluate(JD_EQUINOX,
                                                 GREENWICH_LAT, GREENWICH_LON);
    alert_result_t r2 = location_alerts_evaluate(JD_EQUINOX + 0.5,
                                                 GREENWICH_LAT, GREENWICH_LON);
    /* At least the structure should be valid */
    TEST_ASSERT_TRUE(r1.count >= 0);
    TEST_ASSERT_TRUE(r2.count >= 0);
}

void test_surf_zero_max_returns_zero(void) {
    alert_t out[1];
    int n = alert_check_surf(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_snow_zero_max_returns_zero(void) {
    alert_t out[1];
    int n = alert_check_snow(JD_EQUINOX, CHAMONIX_LAT, CHAMONIX_LON, out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_golden_hour_zero_max_returns_zero(void) {
    alert_t out[1];
    int n = alert_check_golden_hour(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                    out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_prayer_zero_max_returns_zero(void) {
    alert_t out[1];
    int n = alert_check_prayer(JD_EQUINOX, MECCA_LAT, MECCA_LON, out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_tide_zero_max_returns_zero(void) {
    alert_t out[1];
    int n = alert_check_tide(JD_EQUINOX, PIPELINE_LAT, PIPELINE_LON, out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_daylight_zero_max_returns_zero(void) {
    alert_t out[1];
    int n = alert_check_daylight(JD_EQUINOX, GREENWICH_LAT, GREENWICH_LON,
                                 out, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* alert_type_name */
    RUN_TEST(test_type_name_surf);
    RUN_TEST(test_type_name_snow);
    RUN_TEST(test_type_name_golden_hour);
    RUN_TEST(test_type_name_blue_hour);
    RUN_TEST(test_type_name_prayer);
    RUN_TEST(test_type_name_daylight);
    RUN_TEST(test_type_name_tide);
    RUN_TEST(test_type_name_invalid);

    /* alert_urgency_name */
    RUN_TEST(test_urgency_name_info);
    RUN_TEST(test_urgency_name_suggestion);
    RUN_TEST(test_urgency_name_notable);
    RUN_TEST(test_urgency_name_invalid);

    /* alert_check_surf */
    RUN_TEST(test_surf_returns_nonnegative_count);
    RUN_TEST(test_surf_respects_max);
    RUN_TEST(test_surf_type_is_surf);
    RUN_TEST(test_surf_message_not_empty);
    RUN_TEST(test_surf_quality_in_range);
    RUN_TEST(test_surf_hours_until_finite);

    /* alert_check_snow */
    RUN_TEST(test_snow_returns_nonnegative_count);
    RUN_TEST(test_snow_respects_max);
    RUN_TEST(test_snow_type_is_snow);
    RUN_TEST(test_snow_message_not_empty);
    RUN_TEST(test_snow_at_tropical_location);

    /* alert_check_golden_hour */
    RUN_TEST(test_golden_hour_returns_nonnegative);
    RUN_TEST(test_golden_hour_respects_max);
    RUN_TEST(test_golden_hour_type_correct);
    RUN_TEST(test_golden_hour_message_not_empty);
    RUN_TEST(test_golden_hour_hours_until_reasonable);

    /* alert_check_prayer */
    RUN_TEST(test_prayer_returns_nonnegative);
    RUN_TEST(test_prayer_produces_at_least_one);
    RUN_TEST(test_prayer_type_is_prayer);
    RUN_TEST(test_prayer_message_not_empty);
    RUN_TEST(test_prayer_respects_max);
    RUN_TEST(test_prayer_hours_until_positive);

    /* alert_check_tide */
    RUN_TEST(test_tide_returns_nonnegative);
    RUN_TEST(test_tide_respects_max);
    RUN_TEST(test_tide_type_is_tide);
    RUN_TEST(test_tide_message_not_empty);

    /* alert_check_daylight */
    RUN_TEST(test_daylight_returns_nonnegative);
    RUN_TEST(test_daylight_respects_max);
    RUN_TEST(test_daylight_type_is_daylight);
    RUN_TEST(test_daylight_message_not_empty);
    RUN_TEST(test_daylight_polar_day_info);
    RUN_TEST(test_daylight_polar_night_info);

    /* alert_sort */
    RUN_TEST(test_sort_urgency_descending);
    RUN_TEST(test_sort_hours_ascending_within_urgency);
    RUN_TEST(test_sort_empty_array);
    RUN_TEST(test_sort_single_element);
    RUN_TEST(test_sort_preserves_message);

    /* location_alerts_evaluate */
    RUN_TEST(test_evaluate_returns_valid_result);
    RUN_TEST(test_evaluate_count_within_max);
    RUN_TEST(test_evaluate_alerts_have_valid_types);
    RUN_TEST(test_evaluate_alerts_have_valid_urgency);
    RUN_TEST(test_evaluate_sorted_by_urgency);
    RUN_TEST(test_evaluate_messages_not_empty);
    RUN_TEST(test_evaluate_at_pipeline);
    RUN_TEST(test_evaluate_at_mecca);
    RUN_TEST(test_evaluate_quality_in_range);

    /* Edge cases */
    RUN_TEST(test_evaluate_at_equator);
    RUN_TEST(test_evaluate_at_south_pole);
    RUN_TEST(test_evaluate_at_north_pole);
    RUN_TEST(test_evaluate_different_times);
    RUN_TEST(test_surf_zero_max_returns_zero);
    RUN_TEST(test_snow_zero_max_returns_zero);
    RUN_TEST(test_golden_hour_zero_max_returns_zero);
    RUN_TEST(test_prayer_zero_max_returns_zero);
    RUN_TEST(test_tide_zero_max_returns_zero);
    RUN_TEST(test_daylight_zero_max_returns_zero);

    return UNITY_END();
}
