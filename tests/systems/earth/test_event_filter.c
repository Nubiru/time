/* test_event_filter.c — Tests for event filter engine module.
 * TDD RED phase: all tests written before implementation.
 * Covers default filter, presets, time windows, blacklisting, tag
 * requirements, ef_passes, ef_apply, rate limiter, name strings,
 * window durations, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/event_filter.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== ef_default ===== */

void test_default_filter_no_restrictions(void) {
    ef_filter_t f = ef_default();
    TEST_ASSERT_EQUAL_INT(0, f.type_blacklist_count);
    TEST_ASSERT_EQUAL_INT(0, f.required_tag_count);
    TEST_ASSERT_EQUAL_INT(0, f.require_all_tags);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.max_events_per_hour);
    TEST_ASSERT_EQUAL_INT(0, f.astronomical_only);
    /* Base filter should also be default */
    TEST_ASSERT_EQUAL_INT(0, f.base.type_mask);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.base.max_distance_km);
}

/* ===== ef_from_preset ===== */

void test_preset_all_no_filtering(void) {
    ef_filter_t f = ef_from_preset(EF_PRESET_ALL, 34.0, -118.0, 50.0);
    TEST_ASSERT_EQUAL_INT(0, f.type_blacklist_count);
    TEST_ASSERT_EQUAL_INT(0, f.required_tag_count);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 34.0f, (float)f.base.center_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -118.0f, (float)f.base.center_lon);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 50.0f, (float)f.base.max_distance_km);
}

void test_preset_nature_lover(void) {
    ef_filter_t f = ef_from_preset(EF_PRESET_NATURE_LOVER, 40.0, -74.0, 0.0);
    /* Nature lover: types NATURE + ASTRONOMICAL */
    int mask = f.base.type_mask;
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_NATURE));
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_ASTRONOMICAL));
    /* Should have a radius (100km default for nature lover) */
    TEST_ASSERT_TRUE(f.base.max_distance_km > 0.0);
}

void test_preset_night_owl(void) {
    ef_filter_t f = ef_from_preset(EF_PRESET_NIGHT_OWL, 40.0, -74.0, 0.0);
    int mask = f.base.type_mask;
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_ASTRONOMICAL));
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_CULTURE));
}

void test_preset_cultural(void) {
    ef_filter_t f = ef_from_preset(EF_PRESET_CULTURAL, 40.0, -74.0, 0.0);
    int mask = f.base.type_mask;
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_CULTURE));
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_SOCIAL));
}

void test_preset_social_butterfly(void) {
    ef_filter_t f = ef_from_preset(EF_PRESET_SOCIAL_BUTTERFLY, 40.0, -74.0, 0.0);
    int mask = f.base.type_mask;
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_SOCIAL));
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_CULTURE));
    TEST_ASSERT_TRUE(mask & (1 << LE_TYPE_NATURE));
}

void test_preset_uses_custom_radius_when_nonzero(void) {
    ef_filter_t f = ef_from_preset(EF_PRESET_NATURE_LOVER, 34.0, -118.0, 200.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 200.0f, (float)f.base.max_distance_km);
}

/* ===== ef_set_window ===== */

void test_window_next_hour(void) {
    double now_jd = 2460000.5;
    ef_filter_t f = ef_default();
    f = ef_set_window(f, EF_WINDOW_NEXT_HOUR, now_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)now_jd, (float)f.base.start_jd);
    /* 1 hour = 1/24 day */
    double expected_end = now_jd + (1.0 / 24.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected_end, (float)f.base.end_jd);
}

void test_window_today(void) {
    double now_jd = 2460000.5;
    ef_filter_t f = ef_default();
    f = ef_set_window(f, EF_WINDOW_TODAY, now_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)now_jd, (float)f.base.start_jd);
    double expected_end = now_jd + 1.0;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected_end, (float)f.base.end_jd);
}

void test_window_this_week(void) {
    double now_jd = 2460000.5;
    ef_filter_t f = ef_default();
    f = ef_set_window(f, EF_WINDOW_THIS_WEEK, now_jd);
    double expected_end = now_jd + 7.0;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected_end, (float)f.base.end_jd);
}

void test_window_this_month(void) {
    double now_jd = 2460000.5;
    ef_filter_t f = ef_default();
    f = ef_set_window(f, EF_WINDOW_THIS_MONTH, now_jd);
    double expected_end = now_jd + 30.0;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected_end, (float)f.base.end_jd);
}

/* ===== ef_blacklist_type ===== */

void test_blacklist_single_type(void) {
    ef_filter_t f = ef_default();
    f = ef_blacklist_type(f, LE_TYPE_ALERT);
    TEST_ASSERT_EQUAL_INT(1, f.type_blacklist_count);
    TEST_ASSERT_EQUAL_INT(LE_TYPE_ALERT, f.type_blacklist[0]);
}

void test_blacklist_multiple_types(void) {
    ef_filter_t f = ef_default();
    f = ef_blacklist_type(f, LE_TYPE_ALERT);
    f = ef_blacklist_type(f, LE_TYPE_SOCIAL);
    TEST_ASSERT_EQUAL_INT(2, f.type_blacklist_count);
    TEST_ASSERT_EQUAL_INT(LE_TYPE_ALERT, f.type_blacklist[0]);
    TEST_ASSERT_EQUAL_INT(LE_TYPE_SOCIAL, f.type_blacklist[1]);
}

void test_blacklist_overflow_ignored(void) {
    ef_filter_t f = ef_default();
    for (int i = 0; i < EF_MAX_TYPE_BLACKLIST + 3; i++) {
        f = ef_blacklist_type(f, LE_TYPE_ALERT);
    }
    TEST_ASSERT_EQUAL_INT(EF_MAX_TYPE_BLACKLIST, f.type_blacklist_count);
}

/* ===== ef_require_tag / ef_set_tag_mode ===== */

void test_require_single_tag(void) {
    ef_filter_t f = ef_default();
    f = ef_require_tag(f, 5);
    TEST_ASSERT_EQUAL_INT(1, f.required_tag_count);
    TEST_ASSERT_EQUAL_INT(5, f.required_tags[0]);
}

void test_require_tag_overflow_ignored(void) {
    ef_filter_t f = ef_default();
    for (int i = 0; i < EF_MAX_TAG_REQUIRE + 3; i++) {
        f = ef_require_tag(f, i);
    }
    TEST_ASSERT_EQUAL_INT(EF_MAX_TAG_REQUIRE, f.required_tag_count);
}

void test_tag_mode_and(void) {
    ef_filter_t f = ef_default();
    f = ef_set_tag_mode(f, 1);
    TEST_ASSERT_EQUAL_INT(1, f.require_all_tags);
}

void test_tag_mode_or(void) {
    ef_filter_t f = ef_default();
    f = ef_set_tag_mode(f, 0);
    TEST_ASSERT_EQUAL_INT(0, f.require_all_tags);
}

/* ===== ef_set_rate_limit ===== */

void test_set_rate_limit(void) {
    ef_filter_t f = ef_default();
    f = ef_set_rate_limit(f, 10.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, (float)f.max_events_per_hour);
}

/* ===== ef_passes ===== */

void test_passes_default_filter_all_pass(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    ef_filter_t f = ef_default();
    TEST_ASSERT_EQUAL_INT(1, ef_passes(&e, &f));
}

void test_passes_blacklisted_type_fails(void) {
    le_event_t e = le_create(LE_TYPE_ALERT, "Storm", "Warning",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    ef_filter_t f = ef_default();
    f = ef_blacklist_type(f, LE_TYPE_ALERT);
    TEST_ASSERT_EQUAL_INT(0, ef_passes(&e, &f));
}

void test_passes_non_blacklisted_type_passes(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    ef_filter_t f = ef_default();
    f = ef_blacklist_type(f, LE_TYPE_ALERT);
    TEST_ASSERT_EQUAL_INT(1, ef_passes(&e, &f));
}

void test_passes_astronomical_only_filters_non_astro(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    ef_filter_t f = ef_default();
    f.astronomical_only = 1;
    TEST_ASSERT_EQUAL_INT(0, ef_passes(&e, &f));
}

void test_passes_astronomical_only_allows_astro(void) {
    le_event_t e = le_create(LE_TYPE_ASTRONOMICAL, "Eclipse", "Total",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    ef_filter_t f = ef_default();
    f.astronomical_only = 1;
    TEST_ASSERT_EQUAL_INT(1, ef_passes(&e, &f));
}

void test_passes_required_tag_or_mode(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    e = le_add_tag(e, 3);
    ef_filter_t f = ef_default();
    f = ef_require_tag(f, 3);
    f = ef_require_tag(f, 7);
    f = ef_set_tag_mode(f, 0); /* OR mode */
    TEST_ASSERT_EQUAL_INT(1, ef_passes(&e, &f));
}

void test_passes_required_tag_or_mode_no_match(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    e = le_add_tag(e, 9);
    ef_filter_t f = ef_default();
    f = ef_require_tag(f, 3);
    f = ef_require_tag(f, 7);
    f = ef_set_tag_mode(f, 0); /* OR mode */
    TEST_ASSERT_EQUAL_INT(0, ef_passes(&e, &f));
}

void test_passes_required_tag_and_mode_all_present(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    e = le_add_tag(e, 3);
    e = le_add_tag(e, 7);
    ef_filter_t f = ef_default();
    f = ef_require_tag(f, 3);
    f = ef_require_tag(f, 7);
    f = ef_set_tag_mode(f, 1); /* AND mode */
    TEST_ASSERT_EQUAL_INT(1, ef_passes(&e, &f));
}

void test_passes_required_tag_and_mode_missing_one(void) {
    le_event_t e = le_create(LE_TYPE_NATURE, "Hike", "Trail",
                             34.0, -118.0, 5.0, 2460000.5, 0.0);
    e = le_add_tag(e, 3);
    ef_filter_t f = ef_default();
    f = ef_require_tag(f, 3);
    f = ef_require_tag(f, 7);
    f = ef_set_tag_mode(f, 1); /* AND mode */
    TEST_ASSERT_EQUAL_INT(0, ef_passes(&e, &f));
}

/* ===== ef_apply ===== */

void test_apply_empty_events(void) {
    ef_filter_t f = ef_default();
    le_result_t r = ef_apply(NULL, 0, &f);
    TEST_ASSERT_EQUAL_INT(0, r.count);
}

void test_apply_filters_blacklisted(void) {
    le_event_t events[3];
    events[0] = le_create(LE_TYPE_NATURE, "Hike", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);
    events[1] = le_create(LE_TYPE_ALERT, "Storm", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);
    events[2] = le_create(LE_TYPE_ASTRONOMICAL, "Eclipse", "",
                          34.0, -118.0, 5.0, 2460000.5, 0.0);

    ef_filter_t f = ef_default();
    f.base.start_jd = 2460000.5;
    f = ef_blacklist_type(f, LE_TYPE_ALERT);
    le_result_t r = ef_apply(events, 3, &f);
    /* ALERT event should be filtered out */
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(LE_TYPE_ALERT, r.items[i].event.type);
    }
}

void test_apply_sorted_by_relevance(void) {
    le_event_t events[3];
    events[0] = le_create(LE_TYPE_NATURE, "Near", "",
                          34.01, -118.01, 5.0, 2460000.5, 0.0);
    events[1] = le_create(LE_TYPE_NATURE, "Med", "",
                          34.1, -117.9, 5.0, 2460000.5, 0.0);
    events[2] = le_create(LE_TYPE_NATURE, "Far", "",
                          34.5, -117.5, 5.0, 2460000.5, 0.0);

    ef_filter_t f = ef_default();
    f.base = le_filter_nearby(34.0, -118.0, 200.0);
    f.base.start_jd = 2460000.5;
    le_result_t r = ef_apply(events, 3, &f);
    TEST_ASSERT_TRUE(r.count >= 2);
    TEST_ASSERT_TRUE(r.items[0].relevance >= r.items[1].relevance);
}

/* ===== Rate limiter ===== */

void test_rate_init(void) {
    ef_rate_t rate = ef_rate_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)rate.last_surface_jd);
    TEST_ASSERT_EQUAL_INT(0, rate.events_this_hour);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)rate.hour_start_jd);
}

void test_rate_allow_under_limit(void) {
    ef_rate_t rate = ef_rate_init();
    rate.hour_start_jd = 2460000.5;
    rate.events_this_hour = 3;
    /* now_jd within 1 hour of hour_start */
    double now_jd = 2460000.5 + (0.5 / 24.0);
    TEST_ASSERT_EQUAL_INT(1, ef_rate_allow(rate, now_jd, 10.0));
}

void test_rate_allow_at_limit(void) {
    ef_rate_t rate = ef_rate_init();
    rate.hour_start_jd = 2460000.5;
    rate.events_this_hour = 10;
    double now_jd = 2460000.5 + (0.5 / 24.0);
    TEST_ASSERT_EQUAL_INT(0, ef_rate_allow(rate, now_jd, 10.0));
}

void test_rate_allow_new_hour_resets(void) {
    ef_rate_t rate = ef_rate_init();
    rate.hour_start_jd = 2460000.5;
    rate.events_this_hour = 10;
    /* 2 hours later — new window */
    double now_jd = 2460000.5 + (2.0 / 24.0);
    TEST_ASSERT_EQUAL_INT(1, ef_rate_allow(rate, now_jd, 10.0));
}

void test_rate_allow_zero_limit_always_allows(void) {
    ef_rate_t rate = ef_rate_init();
    rate.events_this_hour = 999;
    TEST_ASSERT_EQUAL_INT(1, ef_rate_allow(rate, 2460000.5, 0.0));
}

void test_rate_record_increments(void) {
    ef_rate_t rate = ef_rate_init();
    rate.hour_start_jd = 2460000.5;
    double now_jd = 2460000.5 + (0.1 / 24.0);
    rate = ef_rate_record(rate, now_jd, 10.0);
    TEST_ASSERT_EQUAL_INT(1, rate.events_this_hour);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)now_jd, (float)rate.last_surface_jd);
}

void test_rate_record_new_hour_resets_counter(void) {
    ef_rate_t rate = ef_rate_init();
    rate.hour_start_jd = 2460000.5;
    rate.events_this_hour = 5;
    /* 2 hours later */
    double now_jd = 2460000.5 + (2.0 / 24.0);
    rate = ef_rate_record(rate, now_jd, 10.0);
    /* Counter should be 1 (reset + this record) */
    TEST_ASSERT_EQUAL_INT(1, rate.events_this_hour);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)now_jd, (float)rate.hour_start_jd);
}

/* ===== Preset name strings ===== */

void test_preset_name_all(void) {
    TEST_ASSERT_EQUAL_STRING("All", ef_preset_name(EF_PRESET_ALL));
}

void test_preset_name_nature_lover(void) {
    TEST_ASSERT_EQUAL_STRING("Nature Lover", ef_preset_name(EF_PRESET_NATURE_LOVER));
}

void test_preset_name_all_valid(void) {
    for (int i = 0; i < EF_PRESET_COUNT; i++) {
        const char *name = ef_preset_name((ef_preset_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_preset_name_invalid(void) {
    const char *name = ef_preset_name(EF_PRESET_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

/* ===== Window name strings + durations ===== */

void test_window_name_next_hour(void) {
    TEST_ASSERT_EQUAL_STRING("Next Hour", ef_window_name(EF_WINDOW_NEXT_HOUR));
}

void test_window_name_all_valid(void) {
    for (int i = 0; i < EF_WINDOW_COUNT; i++) {
        const char *name = ef_window_name((ef_window_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_window_name_invalid(void) {
    const char *name = ef_window_name(EF_WINDOW_COUNT);
    TEST_ASSERT_NOT_NULL(name);
}

void test_window_days_next_hour(void) {
    double d = ef_window_days(EF_WINDOW_NEXT_HOUR);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)(1.0 / 24.0), (float)d);
}

void test_window_days_today(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)ef_window_days(EF_WINDOW_TODAY));
}

void test_window_days_week(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 7.0f, (float)ef_window_days(EF_WINDOW_THIS_WEEK));
}

void test_window_days_month(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 30.0f, (float)ef_window_days(EF_WINDOW_THIS_MONTH));
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* Default filter (1 test) */
    RUN_TEST(test_default_filter_no_restrictions);

    /* Presets (6 tests) */
    RUN_TEST(test_preset_all_no_filtering);
    RUN_TEST(test_preset_nature_lover);
    RUN_TEST(test_preset_night_owl);
    RUN_TEST(test_preset_cultural);
    RUN_TEST(test_preset_social_butterfly);
    RUN_TEST(test_preset_uses_custom_radius_when_nonzero);

    /* Time windows (4 tests) */
    RUN_TEST(test_window_next_hour);
    RUN_TEST(test_window_today);
    RUN_TEST(test_window_this_week);
    RUN_TEST(test_window_this_month);

    /* Blacklisting (3 tests) */
    RUN_TEST(test_blacklist_single_type);
    RUN_TEST(test_blacklist_multiple_types);
    RUN_TEST(test_blacklist_overflow_ignored);

    /* Tag requirements (4 tests) */
    RUN_TEST(test_require_single_tag);
    RUN_TEST(test_require_tag_overflow_ignored);
    RUN_TEST(test_tag_mode_and);
    RUN_TEST(test_tag_mode_or);

    /* Rate limit setter (1 test) */
    RUN_TEST(test_set_rate_limit);

    /* ef_passes (9 tests) */
    RUN_TEST(test_passes_default_filter_all_pass);
    RUN_TEST(test_passes_blacklisted_type_fails);
    RUN_TEST(test_passes_non_blacklisted_type_passes);
    RUN_TEST(test_passes_astronomical_only_filters_non_astro);
    RUN_TEST(test_passes_astronomical_only_allows_astro);
    RUN_TEST(test_passes_required_tag_or_mode);
    RUN_TEST(test_passes_required_tag_or_mode_no_match);
    RUN_TEST(test_passes_required_tag_and_mode_all_present);
    RUN_TEST(test_passes_required_tag_and_mode_missing_one);

    /* ef_apply (3 tests) */
    RUN_TEST(test_apply_empty_events);
    RUN_TEST(test_apply_filters_blacklisted);
    RUN_TEST(test_apply_sorted_by_relevance);

    /* Rate limiter (6 tests) */
    RUN_TEST(test_rate_init);
    RUN_TEST(test_rate_allow_under_limit);
    RUN_TEST(test_rate_allow_at_limit);
    RUN_TEST(test_rate_allow_new_hour_resets);
    RUN_TEST(test_rate_allow_zero_limit_always_allows);
    RUN_TEST(test_rate_record_increments);
    RUN_TEST(test_rate_record_new_hour_resets_counter);

    /* Preset names (4 tests) */
    RUN_TEST(test_preset_name_all);
    RUN_TEST(test_preset_name_nature_lover);
    RUN_TEST(test_preset_name_all_valid);
    RUN_TEST(test_preset_name_invalid);

    /* Window names + durations (7 tests) */
    RUN_TEST(test_window_name_next_hour);
    RUN_TEST(test_window_name_all_valid);
    RUN_TEST(test_window_name_invalid);
    RUN_TEST(test_window_days_next_hour);
    RUN_TEST(test_window_days_today);
    RUN_TEST(test_window_days_week);
    RUN_TEST(test_window_days_month);

    return UNITY_END();
}
