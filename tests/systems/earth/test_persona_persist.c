/* test_persona_persist.c -- Tests for persona data serialization module.
 * TDD: tests written first, implementation follows. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/persona_persist.h"

#include <string.h>
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Core key-value store ===== */

void test_create_empty_store(void) {
    pp_store_t s = pp_create();
    TEST_ASSERT_EQUAL_INT(0, s.count);
}

void test_set_and_get(void) {
    pp_store_t s = pp_create();
    s = pp_set(s, "hello", "world");
    const char *v = pp_get(&s, "hello");
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("world", v);
}

void test_has_existing_key(void) {
    pp_store_t s = pp_create();
    s = pp_set(s, "alpha", "1");
    TEST_ASSERT_EQUAL_INT(1, pp_has(&s, "alpha"));
}

void test_has_missing_key(void) {
    pp_store_t s = pp_create();
    TEST_ASSERT_EQUAL_INT(0, pp_has(&s, "nope"));
}

void test_get_missing_key(void) {
    pp_store_t s = pp_create();
    const char *v = pp_get(&s, "missing");
    TEST_ASSERT_NULL(v);
}

void test_overwrite_key(void) {
    pp_store_t s = pp_create();
    s = pp_set(s, "key", "first");
    s = pp_set(s, "key", "second");
    const char *v = pp_get(&s, "key");
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("second", v);
    TEST_ASSERT_EQUAL_INT(1, s.count); /* only one entry */
}

void test_remove_key(void) {
    pp_store_t s = pp_create();
    s = pp_set(s, "a", "1");
    s = pp_set(s, "b", "2");
    s = pp_remove(s, "a");
    TEST_ASSERT_EQUAL_INT(0, pp_has(&s, "a"));
    TEST_ASSERT_EQUAL_INT(1, pp_has(&s, "b"));
    TEST_ASSERT_EQUAL_INT(1, s.count);
}

void test_remove_prefix(void) {
    pp_store_t s = pp_create();
    s = pp_set(s, "time.visual.theme", "0");
    s = pp_set(s, "time.visual.opacity", "1.0");
    s = pp_set(s, "time.temporal.tz", "120");
    s = pp_set(s, "other.key", "val");
    s = pp_remove_prefix(s, "time.visual.");
    TEST_ASSERT_EQUAL_INT(0, pp_has(&s, "time.visual.theme"));
    TEST_ASSERT_EQUAL_INT(0, pp_has(&s, "time.visual.opacity"));
    TEST_ASSERT_EQUAL_INT(1, pp_has(&s, "time.temporal.tz"));
    TEST_ASSERT_EQUAL_INT(1, pp_has(&s, "other.key"));
    TEST_ASSERT_EQUAL_INT(2, s.count);
}

void test_count(void) {
    pp_store_t s = pp_create();
    TEST_ASSERT_EQUAL_INT(0, pp_count(&s));
    s = pp_set(s, "a", "1");
    s = pp_set(s, "b", "2");
    s = pp_set(s, "c", "3");
    TEST_ASSERT_EQUAL_INT(3, pp_count(&s));
}

void test_max_capacity(void) {
    pp_store_t s = pp_create();
    char key[PP_MAX_KEY];
    /* Fill to capacity */
    for (int i = 0; i < PP_MAX_PAIRS; i++) {
        snprintf(key, PP_MAX_KEY, "key_%d", i);
        s = pp_set(s, key, "v");
    }
    TEST_ASSERT_EQUAL_INT(PP_MAX_PAIRS, s.count);
    /* Try one more -- should not overflow */
    s = pp_set(s, "overflow", "nope");
    TEST_ASSERT_EQUAL_INT(PP_MAX_PAIRS, s.count);
    TEST_ASSERT_EQUAL_INT(0, pp_has(&s, "overflow"));
}

/* ===== Prefs roundtrip ===== */

void test_roundtrip_prefs_default(void) {
    up_prefs_t orig = up_default();
    pp_store_t s = pp_create();
    s = pp_save_prefs(s, &orig);
    up_prefs_t loaded = pp_load_prefs(&s);

    /* Visual */
    TEST_ASSERT_EQUAL_INT((int)orig.visual.theme, (int)loaded.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, orig.visual.font_scale,
                             loaded.visual.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, orig.visual.opacity_scale,
                             loaded.visual.opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, orig.visual.animation_speed,
                             loaded.visual.animation_speed);
    TEST_ASSERT_EQUAL_INT(orig.visual.layers_visible,
                          loaded.visual.layers_visible);
    TEST_ASSERT_EQUAL_INT(orig.visual.default_scale_level,
                          loaded.visual.default_scale_level);

    /* Temporal */
    TEST_ASSERT_EQUAL_INT(orig.temporal.home_timezone_offset_min,
                          loaded.temporal.home_timezone_offset_min);
    TEST_ASSERT_EQUAL_INT(orig.temporal.use_24h_time,
                          loaded.temporal.use_24h_time);
    TEST_ASSERT_EQUAL_INT(orig.temporal.week_start_day,
                          loaded.temporal.week_start_day);

    /* Social */
    TEST_ASSERT_EQUAL_INT((int)orig.social.activity_level,
                          (int)loaded.social.activity_level);
    TEST_ASSERT_EQUAL_INT((int)orig.social.visibility,
                          (int)loaded.social.visibility);
    TEST_ASSERT_EQUAL_INT(orig.social.notification_frequency,
                          loaded.social.notification_frequency);
    TEST_ASSERT_EQUAL_INT(orig.social.roulette_opt_in,
                          loaded.social.roulette_opt_in);

    /* Location */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, orig.location.home_lat,
                              loaded.location.home_lat);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, orig.location.home_lon,
                              loaded.location.home_lon);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, orig.location.exploration_radius_km,
                              loaded.location.exploration_radius_km);
    TEST_ASSERT_EQUAL_INT(orig.location.location_set,
                          loaded.location.location_set);
}

void test_roundtrip_prefs_custom(void) {
    up_prefs_t orig = up_default();
    orig.visual.theme = THEME_DAWN;
    orig.visual.font_scale = 1.5f;
    orig.visual.opacity_scale = 0.75f;
    orig.visual.animation_speed = 0.8f;
    orig.visual.layers_visible = 0x00FF;
    orig.visual.default_scale_level = 3;

    orig.temporal.home_timezone_offset_min = -300;
    orig.temporal.preferred_calendars[0] = 1;
    orig.temporal.preferred_calendars[1] = 4;
    orig.temporal.preferred_calendars[2] = 7;
    orig.temporal.preferred_calendar_count = 3;
    orig.temporal.use_24h_time = 1;
    orig.temporal.week_start_day = 1;

    orig.social.activity_level = IP_ACT_EXTROVERT;
    orig.social.visibility = IP_VIS_PUBLIC;
    orig.social.notification_frequency = 2;
    orig.social.roulette_opt_in = 1;

    orig.location.home_lat = 40.7128;
    orig.location.home_lon = -74.0060;
    orig.location.exploration_radius_km = 100.0;
    orig.location.location_set = 1;

    pp_store_t s = pp_create();
    s = pp_save_prefs(s, &orig);
    up_prefs_t loaded = pp_load_prefs(&s);

    TEST_ASSERT_EQUAL_INT(THEME_DAWN, (int)loaded.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.5f, loaded.visual.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.75f, loaded.visual.opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.8f, loaded.visual.animation_speed);
    TEST_ASSERT_EQUAL_INT(0x00FF, loaded.visual.layers_visible);
    TEST_ASSERT_EQUAL_INT(3, loaded.visual.default_scale_level);

    TEST_ASSERT_EQUAL_INT(-300, loaded.temporal.home_timezone_offset_min);
    TEST_ASSERT_EQUAL_INT(3, loaded.temporal.preferred_calendar_count);
    TEST_ASSERT_EQUAL_INT(1, loaded.temporal.preferred_calendars[0]);
    TEST_ASSERT_EQUAL_INT(4, loaded.temporal.preferred_calendars[1]);
    TEST_ASSERT_EQUAL_INT(7, loaded.temporal.preferred_calendars[2]);
    TEST_ASSERT_EQUAL_INT(1, loaded.temporal.use_24h_time);
    TEST_ASSERT_EQUAL_INT(1, loaded.temporal.week_start_day);

    TEST_ASSERT_EQUAL_INT(IP_ACT_EXTROVERT,
                          (int)loaded.social.activity_level);
    TEST_ASSERT_EQUAL_INT(IP_VIS_PUBLIC, (int)loaded.social.visibility);
    TEST_ASSERT_EQUAL_INT(2, loaded.social.notification_frequency);
    TEST_ASSERT_EQUAL_INT(1, loaded.social.roulette_opt_in);

    TEST_ASSERT_FLOAT_WITHIN(1e-4, 40.7128, loaded.location.home_lat);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, -74.0060, loaded.location.home_lon);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 100.0,
                              loaded.location.exploration_radius_km);
    TEST_ASSERT_EQUAL_INT(1, loaded.location.location_set);
}

/* ===== Birth data roundtrip ===== */

void test_roundtrip_birth_with_location(void) {
    pp_store_t s = pp_create();
    s = pp_save_birth(s, 1990, 6, 15, 1, 34.0522, -118.2437);

    int y = 0, m = 0, d = 0, hl = 0;
    double lat = 0.0, lon = 0.0;
    int found = pp_load_birth(&s, &y, &m, &d, &hl, &lat, &lon);

    TEST_ASSERT_EQUAL_INT(1, found);
    TEST_ASSERT_EQUAL_INT(1990, y);
    TEST_ASSERT_EQUAL_INT(6, m);
    TEST_ASSERT_EQUAL_INT(15, d);
    TEST_ASSERT_EQUAL_INT(1, hl);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, 34.0522, lat);
    TEST_ASSERT_FLOAT_WITHIN(1e-4, -118.2437, lon);
}

void test_roundtrip_birth_no_location(void) {
    pp_store_t s = pp_create();
    s = pp_save_birth(s, 1985, 12, 25, 0, 0.0, 0.0);

    int y = 0, m = 0, d = 0, hl = 0;
    double lat = 99.0, lon = 99.0;
    int found = pp_load_birth(&s, &y, &m, &d, &hl, &lat, &lon);

    TEST_ASSERT_EQUAL_INT(1, found);
    TEST_ASSERT_EQUAL_INT(1985, y);
    TEST_ASSERT_EQUAL_INT(12, m);
    TEST_ASSERT_EQUAL_INT(25, d);
    TEST_ASSERT_EQUAL_INT(0, hl);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, lat);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, lon);
}

void test_roundtrip_birth_missing(void) {
    pp_store_t s = pp_create();
    int y = 0, m = 0, d = 0, hl = 0;
    double lat = 0.0, lon = 0.0;
    int found = pp_load_birth(&s, &y, &m, &d, &hl, &lat, &lon);
    TEST_ASSERT_EQUAL_INT(0, found);
}

/* ===== Interest profile roundtrip ===== */

void test_roundtrip_interests(void) {
    ip_profile_t orig = ip_default_profile();
    /* Set some custom weights */
    orig.weights[0] = 0.9;
    orig.weights[5] = 0.3;
    orig.weights[12] = 0.75;
    orig.weights[19] = 1.0;
    orig.visibility = IP_VIS_PSEUDONYM;
    orig.activity_level = IP_ACT_EXTROVERT;
    orig.location_radius_km = 200.0;

    pp_store_t s = pp_create();
    s = pp_save_interests(s, &orig);
    ip_profile_t loaded = pp_load_interests(&s);

    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(1e-6, orig.weights[i], loaded.weights[i]);
    }
    TEST_ASSERT_EQUAL_INT(IP_VIS_PSEUDONYM, (int)loaded.visibility);
    TEST_ASSERT_EQUAL_INT(IP_ACT_EXTROVERT, (int)loaded.activity_level);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 200.0, loaded.location_radius_km);
}

/* ===== Personal space roundtrip ===== */

void test_roundtrip_space_favorites(void) {
    ps_space_t orig = ps_create();
    orig = ps_add_favorite(orig, 2);
    orig = ps_add_favorite(orig, 7);
    orig = ps_add_favorite(orig, 11);

    pp_store_t s = pp_create();
    s = pp_save_space(s, &orig);
    ps_space_t loaded = pp_load_space(&s);

    TEST_ASSERT_EQUAL_INT(3, loaded.favorite_count);
    TEST_ASSERT_EQUAL_INT(2, loaded.favorite_systems[0]);
    TEST_ASSERT_EQUAL_INT(7, loaded.favorite_systems[1]);
    TEST_ASSERT_EQUAL_INT(11, loaded.favorite_systems[2]);
}

void test_roundtrip_space_cycles(void) {
    ps_space_t orig = ps_create();
    orig = ps_add_cycle(orig, "Saturn Return", 5, 2460000.5, 2460365.5);
    orig = ps_add_cycle(orig, "Next Eclipse", -1, 2460100.0, 2460200.0);

    pp_store_t s = pp_create();
    s = pp_save_space(s, &orig);
    ps_space_t loaded = pp_load_space(&s);

    TEST_ASSERT_EQUAL_INT(2, loaded.cycle_count);
    TEST_ASSERT_EQUAL_STRING("Saturn Return", loaded.cycles[0].name);
    TEST_ASSERT_EQUAL_INT(5, loaded.cycles[0].system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460000.5, loaded.cycles[0].start_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460365.5,
                              loaded.cycles[0].next_event_jd);
    TEST_ASSERT_EQUAL_INT(1, loaded.cycles[0].active);

    TEST_ASSERT_EQUAL_STRING("Next Eclipse", loaded.cycles[1].name);
    TEST_ASSERT_EQUAL_INT(-1, loaded.cycles[1].system_id);
}

void test_roundtrip_space_bookmarks(void) {
    ps_space_t orig = ps_create();
    orig = ps_bookmark(orig, 2460100.5, "Solstice meditation", 3);
    orig = ps_bookmark(orig, 2460200.5, "Full moon", 0);

    pp_store_t s = pp_create();
    s = pp_save_space(s, &orig);
    ps_space_t loaded = pp_load_space(&s);

    TEST_ASSERT_EQUAL_INT(2, loaded.bookmark_count);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460100.5, loaded.bookmarks[0].jd);
    TEST_ASSERT_EQUAL_STRING("Solstice meditation",
                             loaded.bookmarks[0].note);
    TEST_ASSERT_EQUAL_INT(3, loaded.bookmarks[0].system_id);

    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460200.5, loaded.bookmarks[1].jd);
    TEST_ASSERT_EQUAL_STRING("Full moon", loaded.bookmarks[1].note);
    TEST_ASSERT_EQUAL_INT(0, loaded.bookmarks[1].system_id);
}

/* ===== Privacy roundtrip ===== */

void test_roundtrip_privacy(void) {
    pf_settings_t orig = pf_default_settings();
    orig.consent = PF_CONSENT_FULL;
    orig.retention = PF_RETAIN_LOCAL;
    orig.fields_shared[0] = 1;
    orig.fields_shared[3] = 1;
    orig.fields_shared[7] = 1;

    pp_store_t s = pp_create();
    s = pp_save_privacy(s, &orig);
    pf_settings_t loaded = pp_load_privacy(&s);

    TEST_ASSERT_EQUAL_INT(PF_CONSENT_FULL, (int)loaded.consent);
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_LOCAL, (int)loaded.retention);
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(orig.fields_shared[i], loaded.fields_shared[i]);
    }
}

/* ===== Serialize / Deserialize text ===== */

void test_serialize_deserialize(void) {
    pp_store_t orig = pp_create();
    orig = pp_set(orig, "alpha", "100");
    orig = pp_set(orig, "beta", "hello");
    orig = pp_set(orig, "gamma", "3.14");

    char buf[4096];
    int len = pp_serialize(&orig, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);

    pp_store_t loaded = pp_deserialize(buf, len);
    TEST_ASSERT_EQUAL_INT(orig.count, loaded.count);

    for (int i = 0; i < orig.count; i++) {
        const char *v = pp_get(&loaded, orig.pairs[i].key);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_STRING(orig.pairs[i].val, v);
    }
}

/* ===== JSON export ===== */

void test_export_json(void) {
    pp_store_t s = pp_create();
    s = pp_set(s, "a", "1");
    s = pp_set(s, "b", "hello");

    char buf[2048];
    int len = pp_export_json(&s, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);

    /* Check structure: starts with {, ends with }\n */
    TEST_ASSERT_EQUAL_CHAR('{', buf[0]);
    /* Find closing brace */
    char *close = strrchr(buf, '}');
    TEST_ASSERT_NOT_NULL(close);
    /* Contains the keys as JSON strings */
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"a\""));
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"b\""));
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"1\""));
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"hello\""));
}

/* ===== Empty store defaults ===== */

void test_load_prefs_empty_store(void) {
    pp_store_t s = pp_create();
    up_prefs_t p = pp_load_prefs(&s);
    up_prefs_t def = up_default();

    /* Should match defaults exactly */
    TEST_ASSERT_EQUAL_INT((int)def.visual.theme, (int)p.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, def.visual.font_scale,
                             p.visual.font_scale);
    TEST_ASSERT_EQUAL_INT(def.temporal.home_timezone_offset_min,
                          p.temporal.home_timezone_offset_min);
    TEST_ASSERT_EQUAL_INT((int)def.social.activity_level,
                          (int)p.social.activity_level);
    TEST_ASSERT_EQUAL_INT(def.location.location_set,
                          p.location.location_set);
}

void test_load_interests_empty_store(void) {
    pp_store_t s = pp_create();
    ip_profile_t p = pp_load_interests(&s);
    ip_profile_t def = ip_default_profile();

    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(1e-10, def.weights[i], p.weights[i]);
    }
    TEST_ASSERT_EQUAL_INT((int)def.visibility, (int)p.visibility);
    TEST_ASSERT_EQUAL_INT((int)def.activity_level, (int)p.activity_level);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, def.location_radius_km,
                              p.location_radius_km);
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* Core store */
    RUN_TEST(test_create_empty_store);
    RUN_TEST(test_set_and_get);
    RUN_TEST(test_has_existing_key);
    RUN_TEST(test_has_missing_key);
    RUN_TEST(test_get_missing_key);
    RUN_TEST(test_overwrite_key);
    RUN_TEST(test_remove_key);
    RUN_TEST(test_remove_prefix);
    RUN_TEST(test_count);
    RUN_TEST(test_max_capacity);

    /* Prefs roundtrip */
    RUN_TEST(test_roundtrip_prefs_default);
    RUN_TEST(test_roundtrip_prefs_custom);

    /* Birth data roundtrip */
    RUN_TEST(test_roundtrip_birth_with_location);
    RUN_TEST(test_roundtrip_birth_no_location);
    RUN_TEST(test_roundtrip_birth_missing);

    /* Interest profile */
    RUN_TEST(test_roundtrip_interests);

    /* Personal space */
    RUN_TEST(test_roundtrip_space_favorites);
    RUN_TEST(test_roundtrip_space_cycles);
    RUN_TEST(test_roundtrip_space_bookmarks);

    /* Privacy */
    RUN_TEST(test_roundtrip_privacy);

    /* Bulk operations */
    RUN_TEST(test_serialize_deserialize);
    RUN_TEST(test_export_json);

    /* Empty store defaults */
    RUN_TEST(test_load_prefs_empty_store);
    RUN_TEST(test_load_interests_empty_store);

    return UNITY_END();
}
