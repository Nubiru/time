/* test_local_events.c — Tests for local event data module.
 * TDD RED phase: all tests written before implementation.
 * Covers event creation, tagging, haversine distance, filtering,
 * scoring, astronomical generation, type/source names, edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/local_events.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== le_create ===== */

void test_create_nature_event(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Beach Hike", "Coastal trail",
                             34.0, -118.0, 10.0, 2460000.5, 2460001.5);
    TEST_ASSERT_EQUAL_INT(LE_TYPE_NATURE, e.type);
    TEST_ASSERT_EQUAL_STRING("Beach Hike", e.title);
    TEST_ASSERT_EQUAL_STRING("Coastal trail", e.description);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 34.0f, (float)e.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -118.0f, (float)e.lon);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, (float)e.radius_km);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 2460000.5f, (float)e.start_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 2460001.5f, (float)e.end_jd);
    TEST_ASSERT_EQUAL_INT(0, e.tag_count);
}

void test_create_astronomical_event(void) {
    le_event_t e = le_create(LE_TYPE_ASTRONOMICAL, "Equinox", "Vernal equinox",
                             0.0, 0.0, 0.0, 2460000.5, 0.0);
    TEST_ASSERT_EQUAL_INT(LE_TYPE_ASTRONOMICAL, e.type);
    TEST_ASSERT_EQUAL_STRING("Equinox", e.title);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)e.end_jd);
}

void test_create_all_types(void) {
    const char *names[] = {"Nature", "Culture", "Social", "Alert", "Astro"};
    for (int i = 0; i < LE_TYPE_COUNT; i++) {
        le_event_t e = le_create((le_type_t)i, names[i], "desc",
                                 0.0, 0.0, 1.0, 2460000.5, 0.0);
        TEST_ASSERT_EQUAL_INT(i, e.type);
    }
}

void test_create_source_defaults_to_user(void) {
    le_event_t e = le_create(LE_TYPE_SOCIAL, "Meetup", "Desc",
                             0.0, 0.0, 5.0, 2460000.5, 0.0);
    TEST_ASSERT_EQUAL_INT(LE_SRC_USER, e.source);
}

void test_create_truncates_long_title(void) {
    char long_title[200];
    memset(long_title, 'A', 199);
    long_title[199] = '\0';
    le_event_t e = le_create(LE_TYPE_NATURE, long_title, "desc",
                             0.0, 0.0, 1.0, 2460000.5, 0.0);
    TEST_ASSERT_TRUE(strlen(e.title) < LE_TITLE_MAX);
}

void test_create_truncates_long_description(void) {
    char long_desc[500];
    memset(long_desc, 'B', 499);
    long_desc[499] = '\0';
    le_event_t e = le_create(LE_TYPE_NATURE, "Title", long_desc,
                             0.0, 0.0, 1.0, 2460000.5, 0.0);
    TEST_ASSERT_TRUE(strlen(e.description) < LE_DESC_MAX);
}

/* ===== le_add_tag ===== */

void test_add_single_tag(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             0.0, 0.0, 1.0, 2460000.5, 0.0);
    e = le_add_tag(e, 1);
    TEST_ASSERT_EQUAL_INT(1, e.tag_count);
    TEST_ASSERT_EQUAL_INT(1, e.tags[0]);
}

void test_add_multiple_tags(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             0.0, 0.0, 1.0, 2460000.5, 0.0);
    e = le_add_tag(e, 0);
    e = le_add_tag(e, 1);
    e = le_add_tag(e, 2);
    TEST_ASSERT_EQUAL_INT(3, e.tag_count);
    TEST_ASSERT_EQUAL_INT(0, e.tags[0]);
    TEST_ASSERT_EQUAL_INT(1, e.tags[1]);
    TEST_ASSERT_EQUAL_INT(2, e.tags[2]);
}

void test_add_tag_overflow(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Full", "Overflow test",
                             0.0, 0.0, 1.0, 2460000.5, 0.0);
    for (int i = 0; i < LE_MAX_TAGS + 3; i++) {
        e = le_add_tag(e, i);
    }
    TEST_ASSERT_EQUAL_INT(LE_MAX_TAGS, e.tag_count);
}

/* ===== le_distance_km (Haversine) ===== */

void test_distance_same_point(void) {
    double d = le_distance_km(51.5074, -0.1278, 51.5074, -0.1278);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)d);
}

void test_distance_london_to_paris(void) {
    /* London (51.5074, -0.1278) -> Paris (48.8566, 2.3522) ~343 km */
    double d = le_distance_km(51.5074, -0.1278, 48.8566, 2.3522);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 343.0f, (float)d);
}

void test_distance_nyc_to_la(void) {
    /* NYC (40.7128, -74.0060) -> LA (34.0522, -118.2437) ~3944 km */
    double d = le_distance_km(40.7128, -74.0060, 34.0522, -118.2437);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 3944.0f, (float)d);
}

void test_distance_poles(void) {
    /* North pole to south pole ~20015 km */
    double d = le_distance_km(90.0, 0.0, -90.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 20015.0f, (float)d);
}

void test_distance_equator_180_degrees(void) {
    /* (0, 0) to (0, 180) = half equatorial circumference ~20015 km */
    double d = le_distance_km(0.0, 0.0, 0.0, 180.0);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 20015.0f, (float)d);
}

void test_distance_symmetry(void) {
    double d1 = le_distance_km(40.0, -74.0, 34.0, -118.0);
    double d2 = le_distance_km(34.0, -118.0, 40.0, -74.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)d1, (float)d2);
}

/* ===== le_filter_default / le_filter_nearby ===== */

void test_filter_default_no_restrictions(void) {
    le_filter_t f = le_filter_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.max_distance_km);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.start_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.end_jd);
    TEST_ASSERT_EQUAL_INT(0, f.type_mask);
    TEST_ASSERT_NULL(f.profile);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.min_relevance);
}

void test_filter_nearby_sets_location(void) {
    le_filter_t f = le_filter_nearby(34.0, -118.0, 50.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 34.0f, (float)f.center_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -118.0f, (float)f.center_lon);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 50.0f, (float)f.max_distance_km);
}

/* ===== le_score ===== */

void test_score_event_at_same_location(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Near", "Nearby event",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    le_filter_t f = le_filter_nearby(34.0, -118.0, 50.0);
    f.start_jd = 2460000.5;
    double s = le_score(&e, &f);
    TEST_ASSERT_TRUE(s > 0.5);
}

void test_score_distant_event_low(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Far", "Distant event",
                             -33.0, 151.0, 5.0, 2460000.5, 0.0);
    le_filter_t f = le_filter_nearby(34.0, -118.0, 100.0);
    f.start_jd = 2460000.5;
    double s = le_score(&e, &f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s);
}

void test_score_no_distance_filter(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Any", "No distance constraint",
                             -33.0, 151.0, 5.0, 2460000.5, 0.0);
    le_filter_t f = le_filter_default();
    f.start_jd = 2460000.5;
    double s = le_score(&e, &f);
    TEST_ASSERT_TRUE(s > 0.0);
}

void test_score_with_interest_profile(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Stargazing", "Night sky",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    e = le_add_tag(e, 1);

    ip_profile_t profile = ip_default_profile();
    profile.weights[1] = 1.0;

    le_filter_t f = le_filter_nearby(34.0, -118.0, 50.0);
    f.start_jd = 2460000.5;
    f.profile = &profile;

    double s = le_score(&e, &f);
    TEST_ASSERT_TRUE(s > 0.5);
}

void test_score_no_matching_interests(void) {
    le_event_t e = le_create(LE_TYPE_SOCIAL, "Meetup", "Social event",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    e = le_add_tag(e, 8);

    ip_profile_t profile = ip_default_profile();
    profile.weights[1] = 1.0;

    le_filter_t f = le_filter_nearby(34.0, -118.0, 50.0);
    f.start_jd = 2460000.5;
    f.profile = &profile;

    double s = le_score(&e, &f);
    /* Tags don't match profile -> lower score than matching */
    le_event_t e2 = le_create(LE_TYPE_NATURE, "Stars", "Stars",
                              34.0, -118.0, 5.0, 2460000.5, 0.0);
    e2 = le_add_tag(e2, 1);
    double s2 = le_score(&e2, &f);
    TEST_ASSERT_TRUE(s2 > s);
}

void test_score_range_zero_to_one(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Test", "Desc",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    le_filter_t f = le_filter_nearby(34.0, -118.0, 50.0);
    f.start_jd = 2460000.5;
    double s = le_score(&e, &f);
    TEST_ASSERT_TRUE(s >= 0.0);
    TEST_ASSERT_TRUE(s <= 1.0);
}

/* ===== le_filter_events ===== */

void test_filter_empty_input(void) {
    le_filter_t f = le_filter_default();
    le_result_t r = le_filter_events(NULL, 0, &f);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

void test_filter_single_event(void) {
    le_event_t events[1];
    events[0] = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);
    le_filter_t f = le_filter_default();
    f.start_jd = 2460000.5;
    le_result_t r = le_filter_events(events, 1, &f);
    TEST_ASSERT_EQUAL_INT(1, r.count);
    TEST_ASSERT_EQUAL_STRING("Hike", r.items[0].event.title);
}

void test_filter_sorted_by_relevance_descending(void) {
    le_event_t events[3];
    events[0] = le_create(LE_TYPE_NATURE, "Near", "Close",
                          34.01, -118.01, 5.0, 2460000.5, 0.0);
    events[1] = le_create(LE_TYPE_NATURE, "Medium", "Mid",
                          34.1, -117.9, 5.0, 2460000.5, 0.0);
    events[2] = le_create(LE_TYPE_NATURE, "Far", "Distant",
                          34.5, -117.5, 5.0, 2460000.5, 0.0);

    le_filter_t f = le_filter_nearby(34.0, -118.0, 100.0);
    f.start_jd = 2460000.5;
    le_result_t r = le_filter_events(events, 3, &f);
    TEST_ASSERT_EQUAL_INT(3, r.count);
    TEST_ASSERT_TRUE(r.items[0].relevance >= r.items[1].relevance);
    TEST_ASSERT_TRUE(r.items[1].relevance >= r.items[2].relevance);
}

void test_filter_type_mask(void) {
    le_event_t events[3];
    events[0] = le_create(LE_TYPE_NATURE, "Hike", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);
    events[1] = le_create(LE_TYPE_SOCIAL, "Meetup", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);
    events[2] = le_create(LE_TYPE_ASTRONOMICAL, "Eclipse", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);

    le_filter_t f = le_filter_default();
    f.start_jd = 2460000.5;
    f.type_mask = (1 << LE_TYPE_NATURE) | (1 << LE_TYPE_ASTRONOMICAL);
    le_result_t r = le_filter_events(events, 3, &f);
    TEST_ASSERT_EQUAL_INT(2, r.count);
}

void test_filter_min_relevance_threshold(void) {
    le_event_t events[2];
    events[0] = le_create(LE_TYPE_NATURE, "Close", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);
    events[1] = le_create(LE_TYPE_NATURE, "Far", "",
                          35.0, -117.0, 5.0, 2460000.5, 0.0);

    le_filter_t f = le_filter_nearby(34.0, -118.0, 200.0);
    f.start_jd = 2460000.5;
    f.min_relevance = 0.5;
    le_result_t r = le_filter_events(events, 2, &f);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_TRUE(r.items[i].relevance >= 0.5);
    }
}

void test_filter_max_events_cap(void) {
    le_event_t events[LE_MAX_EVENTS + 10];
    for (int i = 0; i < LE_MAX_EVENTS + 10; i++) {
        events[i] = le_create(LE_TYPE_NATURE, "Evt", "",
                              34.0, -118.0, 5.0, 2460000.5, 0.0);
    }
    le_filter_t f = le_filter_default();
    f.start_jd = 2460000.5;
    le_result_t r = le_filter_events(events, LE_MAX_EVENTS + 10, &f);
    TEST_ASSERT_TRUE(r.count <= LE_MAX_EVENTS);
}

/* ===== le_generate_astronomical ===== */

void test_generate_astronomical_returns_events(void) {
    le_event_t out[16];
    int n = le_generate_astronomical(2460310.5, 2460676.5,
                                     40.0, -74.0, out, 16);
    TEST_ASSERT_TRUE(n > 0);
}

void test_generate_astronomical_events_are_typed(void) {
    le_event_t out[16];
    int n = le_generate_astronomical(2460310.5, 2460676.5,
                                     40.0, -74.0, out, 16);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(LE_TYPE_ASTRONOMICAL, out[i].type);
        TEST_ASSERT_EQUAL_INT(LE_SRC_AUTO, out[i].source);
    }
}

void test_generate_astronomical_within_jd_range(void) {
    le_event_t out[16];
    double jd_start = 2460310.5;
    double jd_end = 2460676.5;
    int n = le_generate_astronomical(jd_start, jd_end,
                                     40.0, -74.0, out, 16);
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(out[i].start_jd >= jd_start);
        TEST_ASSERT_TRUE(out[i].start_jd <= jd_end);
    }
}

void test_generate_astronomical_max_cap(void) {
    le_event_t out[2];
    int n = le_generate_astronomical(2460310.5, 2460676.5,
                                     40.0, -74.0, out, 2);
    TEST_ASSERT_TRUE(n <= 2);
}

void test_generate_astronomical_zero_range(void) {
    le_event_t out[16];
    int n = le_generate_astronomical(2460310.5, 2460310.5,
                                     40.0, -74.0, out, 16);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_generate_astronomical_has_stargazing_tag(void) {
    le_event_t out[16];
    int n = le_generate_astronomical(2460310.5, 2460676.5,
                                     40.0, -74.0, out, 16);
    TEST_ASSERT_TRUE(n > 0);
    int found_tag = 0;
    for (int i = 0; i < n; i++) {
        for (int t = 0; t < out[i].tag_count; t++) {
            if (out[i].tags[t] == 1) { found_tag = 1; break; }
        }
        if (found_tag) break;
    }
    TEST_ASSERT_TRUE(found_tag);
}

/* ===== le_type_name / le_source_name ===== */

void test_type_name_nature(void) {
    TEST_ASSERT_EQUAL_STRING("Nature", le_type_name(LE_TYPE_NATURE));
}

void test_type_name_astronomical(void) {
    TEST_ASSERT_EQUAL_STRING("Astronomical", le_type_name(LE_TYPE_ASTRONOMICAL));
}

void test_type_name_all_valid(void) {
    for (int i = 0; i < LE_TYPE_COUNT; i++) {
        const char *name = le_type_name((le_type_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_type_name_invalid(void) {
    const char *name = le_type_name(LE_TYPE_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

void test_source_name_auto(void) {
    TEST_ASSERT_EQUAL_STRING("Auto", le_source_name(LE_SRC_AUTO));
}

void test_source_name_user(void) {
    TEST_ASSERT_EQUAL_STRING("User", le_source_name(LE_SRC_USER));
}

void test_source_name_all_valid(void) {
    for (int i = 0; i < LE_SRC_COUNT; i++) {
        const char *name = le_source_name((le_source_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

/* ===== le_count_by_type ===== */

void test_count_by_type_empty(void) {
    le_result_t r;
    r.count = 0;
    int n = le_count_by_type(&r, LE_TYPE_NATURE);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_count_by_type_mixed(void) {
    le_result_t r;
    r.count = 3;
    r.items[0].event = le_create(LE_TYPE_NATURE, "A", "", 0, 0, 1, 2460000.5, 0);
    r.items[1].event = le_create(LE_TYPE_SOCIAL, "B", "", 0, 0, 1, 2460000.5, 0);
    r.items[2].event = le_create(LE_TYPE_NATURE, "C", "", 0, 0, 1, 2460000.5, 0);
    TEST_ASSERT_EQUAL_INT(2, le_count_by_type(&r, LE_TYPE_NATURE));
    TEST_ASSERT_EQUAL_INT(1, le_count_by_type(&r, LE_TYPE_SOCIAL));
    TEST_ASSERT_EQUAL_INT(0, le_count_by_type(&r, LE_TYPE_ASTRONOMICAL));
}

/* ===== Edge cases ===== */

void test_score_zero_distance_max(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Any", "",
                             -33.0, 151.0, 5.0, 2460000.5, 0.0);
    le_filter_t f = le_filter_default();
    f.start_jd = 2460000.5;
    double s = le_score(&e, &f);
    TEST_ASSERT_TRUE(s > 0.0);
}

void test_create_null_title(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, NULL, NULL,
                             0.0, 0.0, 1.0, 2460000.5, 0.0);
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(e.title));
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(e.description));
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* Event creation (7 tests) */
    RUN_TEST(test_create_nature_event);
    RUN_TEST(test_create_astronomical_event);
    RUN_TEST(test_create_all_types);
    RUN_TEST(test_create_source_defaults_to_user);
    RUN_TEST(test_create_truncates_long_title);
    RUN_TEST(test_create_truncates_long_description);
    RUN_TEST(test_create_null_title);

    /* Tag management (3 tests) */
    RUN_TEST(test_add_single_tag);
    RUN_TEST(test_add_multiple_tags);
    RUN_TEST(test_add_tag_overflow);

    /* Haversine distance (6 tests) */
    RUN_TEST(test_distance_same_point);
    RUN_TEST(test_distance_london_to_paris);
    RUN_TEST(test_distance_nyc_to_la);
    RUN_TEST(test_distance_poles);
    RUN_TEST(test_distance_equator_180_degrees);
    RUN_TEST(test_distance_symmetry);

    /* Filters (2 tests) */
    RUN_TEST(test_filter_default_no_restrictions);
    RUN_TEST(test_filter_nearby_sets_location);

    /* Scoring (7 tests) */
    RUN_TEST(test_score_event_at_same_location);
    RUN_TEST(test_score_distant_event_low);
    RUN_TEST(test_score_no_distance_filter);
    RUN_TEST(test_score_with_interest_profile);
    RUN_TEST(test_score_no_matching_interests);
    RUN_TEST(test_score_range_zero_to_one);
    RUN_TEST(test_score_zero_distance_max);

    /* Filter + sort (6 tests) */
    RUN_TEST(test_filter_empty_input);
    RUN_TEST(test_filter_single_event);
    RUN_TEST(test_filter_sorted_by_relevance_descending);
    RUN_TEST(test_filter_type_mask);
    RUN_TEST(test_filter_min_relevance_threshold);
    RUN_TEST(test_filter_max_events_cap);

    /* Astronomical event generation (6 tests) */
    RUN_TEST(test_generate_astronomical_returns_events);
    RUN_TEST(test_generate_astronomical_events_are_typed);
    RUN_TEST(test_generate_astronomical_within_jd_range);
    RUN_TEST(test_generate_astronomical_max_cap);
    RUN_TEST(test_generate_astronomical_zero_range);
    RUN_TEST(test_generate_astronomical_has_stargazing_tag);

    /* Type/source names (7 tests) */
    RUN_TEST(test_type_name_nature);
    RUN_TEST(test_type_name_astronomical);
    RUN_TEST(test_type_name_all_valid);
    RUN_TEST(test_type_name_invalid);
    RUN_TEST(test_source_name_auto);
    RUN_TEST(test_source_name_user);
    RUN_TEST(test_source_name_all_valid);

    /* Count by type (2 tests) */
    RUN_TEST(test_count_by_type_empty);
    RUN_TEST(test_count_by_type_mixed);

    return UNITY_END();
}
