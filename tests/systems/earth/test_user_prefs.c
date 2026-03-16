/* test_user_prefs.c -- Tests for user preferences store module.
 * TDD RED phase: all tests written before implementation.
 * Track 47.3: User Preferences Store. */

#include "unity.h"
#include "systems/earth/user_prefs.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== up_default ===== */

void test_default_visual_theme_is_cosmos(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(THEME_COSMOS, p.visual.theme);
}

void test_default_visual_font_scale_is_one(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.visual.font_scale);
}

void test_default_visual_opacity_scale_is_one(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.visual.opacity_scale);
}

void test_default_visual_animation_speed_is_one(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.visual.animation_speed);
}

void test_default_visual_layers_all_visible(void) {
    up_prefs_t p = up_default();
    /* All bits set = 0xFFFF (16 layers) */
    TEST_ASSERT_EQUAL_INT(0xFFFF, p.visual.layers_visible);
}

void test_default_visual_scale_level_zero(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.visual.default_scale_level);
}

void test_default_temporal_utc_offset_zero(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.temporal.home_timezone_offset_min);
}

void test_default_temporal_24h_off(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.temporal.use_24h_time);
}

void test_default_temporal_week_start_sunday(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.temporal.week_start_day);
}

void test_default_temporal_no_preferred_calendars(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.temporal.preferred_calendar_count);
    TEST_ASSERT_EQUAL_INT(-1, p.temporal.preferred_calendars[0]);
}

void test_default_social_introvert(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(IP_ACT_INTROVERT, p.social.activity_level);
}

void test_default_social_anonymous(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(IP_VIS_ANONYMOUS, p.social.visibility);
}

void test_default_social_notifications_off(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.social.notification_frequency);
}

void test_default_social_roulette_off(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.social.roulette_opt_in);
}

void test_default_social_consent_none(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_NONE, p.social.consent);
}

void test_default_social_retention_session(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_SESSION, p.social.retention);
}

void test_default_location_not_set(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, p.location.location_set);
}

void test_default_location_coords_zero(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.location.home_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.location.home_lon);
}

void test_default_location_radius(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 50.0, p.location.exploration_radius_km);
}

void test_default_version_is_one(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(1, p.version);
}

/* ===== up_validate ===== */

void test_validate_clamps_font_scale_low(void) {
    up_prefs_t p = up_default();
    p.visual.font_scale = 0.1f;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, p.visual.font_scale);
}

void test_validate_clamps_font_scale_high(void) {
    up_prefs_t p = up_default();
    p.visual.font_scale = 5.0f;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, p.visual.font_scale);
}

void test_validate_clamps_opacity_scale_low(void) {
    up_prefs_t p = up_default();
    p.visual.opacity_scale = 0.1f;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, p.visual.opacity_scale);
}

void test_validate_clamps_opacity_scale_high(void) {
    up_prefs_t p = up_default();
    p.visual.opacity_scale = 1.5f;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p.visual.opacity_scale);
}

void test_validate_clamps_animation_speed_low(void) {
    up_prefs_t p = up_default();
    p.visual.animation_speed = 0.1f;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, p.visual.animation_speed);
}

void test_validate_clamps_animation_speed_high(void) {
    up_prefs_t p = up_default();
    p.visual.animation_speed = 10.0f;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, p.visual.animation_speed);
}

void test_validate_clamps_scale_level(void) {
    up_prefs_t p = up_default();
    p.visual.default_scale_level = 99;
    p = up_validate(p);
    TEST_ASSERT_EQUAL_INT(6, p.visual.default_scale_level);
}

void test_validate_clamps_timezone_low(void) {
    up_prefs_t p = up_default();
    p.temporal.home_timezone_offset_min = -9999;
    p = up_validate(p);
    TEST_ASSERT_EQUAL_INT(-720, p.temporal.home_timezone_offset_min);
}

void test_validate_clamps_timezone_high(void) {
    up_prefs_t p = up_default();
    p.temporal.home_timezone_offset_min = 9999;
    p = up_validate(p);
    TEST_ASSERT_EQUAL_INT(840, p.temporal.home_timezone_offset_min);
}

void test_validate_clamps_week_start(void) {
    up_prefs_t p = up_default();
    p.temporal.week_start_day = 5;
    p = up_validate(p);
    TEST_ASSERT_TRUE(p.temporal.week_start_day == 0 ||
                     p.temporal.week_start_day == 1);
}

void test_validate_clamps_notification_frequency(void) {
    up_prefs_t p = up_default();
    p.social.notification_frequency = 10;
    p = up_validate(p);
    TEST_ASSERT_EQUAL_INT(3, p.social.notification_frequency);
}

void test_validate_clamps_latitude(void) {
    up_prefs_t p = up_default();
    p.location.home_lat = 100.0;
    p.location.location_set = 1;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 90.0, p.location.home_lat);
}

void test_validate_clamps_longitude(void) {
    up_prefs_t p = up_default();
    p.location.home_lon = -200.0;
    p.location.location_set = 1;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001, -180.0, p.location.home_lon);
}

void test_validate_clamps_radius(void) {
    up_prefs_t p = up_default();
    p.location.exploration_radius_km = 999.0;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 500.0, p.location.exploration_radius_km);
}

void test_validate_clamps_radius_min(void) {
    up_prefs_t p = up_default();
    p.location.exploration_radius_km = 0.1;
    p = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, p.location.exploration_radius_km);
}

void test_validate_preserves_valid_prefs(void) {
    up_prefs_t p = up_default();
    p.visual.font_scale = 1.5f;
    p.temporal.home_timezone_offset_min = 330;
    up_prefs_t v = up_validate(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, v.visual.font_scale);
    TEST_ASSERT_EQUAL_INT(330, v.temporal.home_timezone_offset_min);
}

/* ===== up_serialize / up_deserialize round-trip ===== */

void test_serialize_roundtrip_defaults(void) {
    up_prefs_t p = up_default();
    char buf[UP_MAX_SERIAL_SIZE];
    up_serial_result_t r = up_serialize(&p, buf, UP_MAX_SERIAL_SIZE);
    TEST_ASSERT_TRUE(r.length > 0);
    TEST_ASSERT_EQUAL_INT(0, r.truncated);

    up_prefs_t d = up_deserialize(buf, r.length);
    int diff = up_diff(&p, &d);
    TEST_ASSERT_EQUAL_INT(0, diff);
}

void test_serialize_roundtrip_custom_values(void) {
    up_prefs_t p = up_default();
    p.visual.theme = THEME_DAWN;
    p.visual.font_scale = 1.5f;
    p.temporal.home_timezone_offset_min = 330;
    p.temporal.use_24h_time = 1;
    p.social.activity_level = IP_ACT_EXTROVERT;
    p.social.visibility = IP_VIS_PUBLIC;
    p.location.home_lat = 40.7128;
    p.location.home_lon = -74.0060;
    p.location.exploration_radius_km = 100.0;
    p.location.location_set = 1;

    char buf[UP_MAX_SERIAL_SIZE];
    up_serial_result_t r = up_serialize(&p, buf, UP_MAX_SERIAL_SIZE);
    TEST_ASSERT_TRUE(r.length > 0);

    up_prefs_t d = up_deserialize(buf, r.length);
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, d.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, d.visual.font_scale);
    TEST_ASSERT_EQUAL_INT(330, d.temporal.home_timezone_offset_min);
    TEST_ASSERT_EQUAL_INT(1, d.temporal.use_24h_time);
    TEST_ASSERT_EQUAL_INT(IP_ACT_EXTROVERT, d.social.activity_level);
    TEST_ASSERT_EQUAL_INT(IP_VIS_PUBLIC, d.social.visibility);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 40.7128, d.location.home_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, -74.0060, d.location.home_lon);
    TEST_ASSERT_EQUAL_INT(1, d.location.location_set);
}

/* ===== Partial deserialization ===== */

void test_deserialize_partial_missing_keys_get_defaults(void) {
    const char *buf = "visual.theme=1\ntemporal.use_24h=1\n";
    up_prefs_t d = up_deserialize(buf, (int)strlen(buf));
    /* Supplied keys */
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, d.visual.theme);
    TEST_ASSERT_EQUAL_INT(1, d.temporal.use_24h_time);
    /* Missing keys get defaults */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, d.visual.font_scale);
    TEST_ASSERT_EQUAL_INT(IP_ACT_INTROVERT, d.social.activity_level);
}

void test_deserialize_empty_string_returns_defaults(void) {
    up_prefs_t d = up_deserialize("", 0);
    up_prefs_t def = up_default();
    TEST_ASSERT_EQUAL_INT(0, up_diff(&d, &def));
}

/* ===== up_parse_kv ===== */

void test_parse_kv_valid_pair(void) {
    const char *buf = "visual.theme=0\n";
    up_kv_t kv;
    int consumed = up_parse_kv(buf, (int)strlen(buf), &kv);
    TEST_ASSERT_TRUE(consumed > 0);
    TEST_ASSERT_EQUAL_STRING("visual.theme", kv.key);
    TEST_ASSERT_EQUAL_STRING("0", kv.val);
}

void test_parse_kv_no_newline(void) {
    /* Last line without trailing newline */
    const char *buf = "visual.theme=0";
    up_kv_t kv;
    int consumed = up_parse_kv(buf, (int)strlen(buf), &kv);
    TEST_ASSERT_TRUE(consumed > 0);
    TEST_ASSERT_EQUAL_STRING("visual.theme", kv.key);
    TEST_ASSERT_EQUAL_STRING("0", kv.val);
}

void test_parse_kv_empty_returns_zero(void) {
    up_kv_t kv;
    int consumed = up_parse_kv("", 0, &kv);
    TEST_ASSERT_EQUAL_INT(0, consumed);
}

void test_parse_kv_malformed_no_equals(void) {
    const char *buf = "badline\n";
    up_kv_t kv;
    int consumed = up_parse_kv(buf, (int)strlen(buf), &kv);
    /* Should skip malformed line */
    TEST_ASSERT_TRUE(consumed > 0);
    TEST_ASSERT_EQUAL_STRING("", kv.key);
}

/* ===== up_count_keys ===== */

void test_count_keys_empty(void) {
    TEST_ASSERT_EQUAL_INT(0, up_count_keys("", 0));
}

void test_count_keys_three_lines(void) {
    const char *buf = "a=1\nb=2\nc=3\n";
    TEST_ASSERT_EQUAL_INT(3, up_count_keys(buf, (int)strlen(buf)));
}

/* ===== up_serialize_visual / temporal / social / location ===== */

void test_serialize_visual_section(void) {
    up_prefs_t p = up_default();
    char buf[512];
    up_serial_result_t r = up_serialize_visual(&p.visual, buf, 512);
    TEST_ASSERT_TRUE(r.length > 0);
    TEST_ASSERT_EQUAL_INT(0, r.truncated);
    /* Should contain visual.theme */
    TEST_ASSERT_NOT_NULL(strstr(buf, "visual.theme="));
}

void test_serialize_temporal_section(void) {
    up_prefs_t p = up_default();
    char buf[512];
    up_serial_result_t r = up_serialize_temporal(&p.temporal, buf, 512);
    TEST_ASSERT_TRUE(r.length > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "temporal.tz_offset="));
}

void test_serialize_social_section(void) {
    up_prefs_t p = up_default();
    char buf[512];
    up_serial_result_t r = up_serialize_social(&p.social, buf, 512);
    TEST_ASSERT_TRUE(r.length > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "social.activity="));
}

void test_serialize_location_section(void) {
    up_prefs_t p = up_default();
    p.location.home_lat = 51.5074;
    p.location.home_lon = -0.1278;
    p.location.location_set = 1;
    char buf[512];
    up_serial_result_t r = up_serialize_location(&p.location, buf, 512);
    TEST_ASSERT_TRUE(r.length > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "location.lat="));
}

/* ===== up_merge ===== */

void test_merge_patch_overrides_base(void) {
    up_prefs_t base = up_default();
    up_prefs_t patch = up_default();
    patch.visual.theme = THEME_DAWN;
    patch.temporal.use_24h_time = 1;

    up_prefs_t merged = up_merge(base, patch);
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, merged.visual.theme);
    TEST_ASSERT_EQUAL_INT(1, merged.temporal.use_24h_time);
}

void test_merge_default_patch_preserves_base(void) {
    up_prefs_t base = up_default();
    base.visual.theme = THEME_DAWN;
    base.visual.font_scale = 1.5f;

    up_prefs_t patch = up_default(); /* all defaults = no changes */
    up_prefs_t merged = up_merge(base, patch);
    /* Default patch should not override */
    TEST_ASSERT_EQUAL_INT(THEME_DAWN, merged.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, merged.visual.font_scale);
}

/* ===== up_diff ===== */

void test_diff_identical_returns_zero(void) {
    up_prefs_t a = up_default();
    up_prefs_t b = up_default();
    TEST_ASSERT_EQUAL_INT(0, up_diff(&a, &b));
}

void test_diff_visual_change(void) {
    up_prefs_t a = up_default();
    up_prefs_t b = up_default();
    b.visual.theme = THEME_DAWN;
    int d = up_diff(&a, &b);
    TEST_ASSERT_TRUE(d & UP_DIFF_VISUAL);
    TEST_ASSERT_FALSE(d & UP_DIFF_TEMPORAL);
    TEST_ASSERT_FALSE(d & UP_DIFF_SOCIAL);
    TEST_ASSERT_FALSE(d & UP_DIFF_LOCATION);
}

void test_diff_temporal_change(void) {
    up_prefs_t a = up_default();
    up_prefs_t b = up_default();
    b.temporal.use_24h_time = 1;
    int d = up_diff(&a, &b);
    TEST_ASSERT_TRUE(d & UP_DIFF_TEMPORAL);
    TEST_ASSERT_FALSE(d & UP_DIFF_VISUAL);
}

void test_diff_multiple_sections(void) {
    up_prefs_t a = up_default();
    up_prefs_t b = up_default();
    b.visual.font_scale = 1.5f;
    b.social.activity_level = IP_ACT_EXTROVERT;
    int d = up_diff(&a, &b);
    TEST_ASSERT_TRUE(d & UP_DIFF_VISUAL);
    TEST_ASSERT_TRUE(d & UP_DIFF_SOCIAL);
}

/* ===== up_to_theme_prefs ===== */

void test_to_theme_prefs_defaults(void) {
    up_prefs_t p = up_default();
    theme_prefs_t tp = up_to_theme_prefs(&p.visual);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, tp.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, tp.opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, tp.animation_speed);
    TEST_ASSERT_EQUAL_INT(0, tp.use_custom_accent);
}

void test_to_theme_prefs_custom_scales(void) {
    up_prefs_t p = up_default();
    p.visual.font_scale = 1.5f;
    p.visual.opacity_scale = 0.8f;
    p.visual.animation_speed = 1.3f;
    theme_prefs_t tp = up_to_theme_prefs(&p.visual);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, tp.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, tp.opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.3f, tp.animation_speed);
}

/* ===== up_to_privacy_settings ===== */

void test_to_privacy_settings_defaults(void) {
    up_prefs_t p = up_default();
    pf_settings_t pf = up_to_privacy_settings(&p.social);
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_NONE, pf.consent);
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_SESSION, pf.retention);
}

void test_to_privacy_settings_full_consent(void) {
    up_prefs_t p = up_default();
    p.social.consent = PF_CONSENT_FULL;
    p.social.retention = PF_RETAIN_LOCAL;
    pf_settings_t pf = up_to_privacy_settings(&p.social);
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_FULL, pf.consent);
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_LOCAL, pf.retention);
}

/* ===== up_has_location ===== */

void test_has_location_false_when_not_set(void) {
    up_prefs_t p = up_default();
    TEST_ASSERT_EQUAL_INT(0, up_has_location(&p.location));
}

void test_has_location_true_when_set(void) {
    up_location_t loc = {0};
    loc.home_lat = 48.8566;
    loc.home_lon = 2.3522;
    loc.exploration_radius_km = 25.0;
    loc.location_set = 1;
    TEST_ASSERT_EQUAL_INT(1, up_has_location(&loc));
}

/* ===== up_summary ===== */

void test_summary_writes_something(void) {
    up_prefs_t p = up_default();
    char buf[512];
    int len = up_summary(&p, buf, 512);
    TEST_ASSERT_TRUE(len > 0);
    /* Should mention theme name */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Cosmos"));
}

void test_summary_with_location(void) {
    up_prefs_t p = up_default();
    p.location.home_lat = 40.7128;
    p.location.home_lon = -74.0060;
    p.location.location_set = 1;
    char buf[512];
    int len = up_summary(&p, buf, 512);
    TEST_ASSERT_TRUE(len > 0);
    /* Should mention the location */
    TEST_ASSERT_NOT_NULL(strstr(buf, "40.71"));
}

/* ===== Edge cases ===== */

void test_serialize_truncation(void) {
    up_prefs_t p = up_default();
    char buf[32]; /* Too small */
    up_serial_result_t r = up_serialize(&p, buf, 32);
    TEST_ASSERT_EQUAL_INT(1, r.truncated);
}

void test_serialize_null_safe_zero_buffer(void) {
    up_prefs_t p = up_default();
    char buf[1];
    up_serial_result_t r = up_serialize(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, r.length);
    TEST_ASSERT_EQUAL_INT(1, r.truncated);
}

void test_deserialize_garbage_returns_defaults(void) {
    const char *buf = "!@#$%^&*()\nfoo\nbar\n";
    up_prefs_t d = up_deserialize(buf, (int)strlen(buf));
    up_prefs_t def = up_default();
    TEST_ASSERT_EQUAL_INT(0, up_diff(&d, &def));
}

void test_serialize_preferred_calendars(void) {
    up_prefs_t p = up_default();
    p.temporal.preferred_calendars[0] = 0;
    p.temporal.preferred_calendars[1] = 3;
    p.temporal.preferred_calendars[2] = 7;
    p.temporal.preferred_calendars[3] = -1;
    p.temporal.preferred_calendar_count = 3;

    char buf[UP_MAX_SERIAL_SIZE];
    up_serial_result_t r = up_serialize(&p, buf, UP_MAX_SERIAL_SIZE);
    TEST_ASSERT_TRUE(r.length > 0);

    up_prefs_t d = up_deserialize(buf, r.length);
    TEST_ASSERT_EQUAL_INT(3, d.temporal.preferred_calendar_count);
    TEST_ASSERT_EQUAL_INT(0, d.temporal.preferred_calendars[0]);
    TEST_ASSERT_EQUAL_INT(3, d.temporal.preferred_calendars[1]);
    TEST_ASSERT_EQUAL_INT(7, d.temporal.preferred_calendars[2]);
}

void test_validate_calendar_count_clamped(void) {
    up_prefs_t p = up_default();
    p.temporal.preferred_calendar_count = 99;
    p = up_validate(p);
    TEST_ASSERT_EQUAL_INT(UP_MAX_CALENDAR_SYSTEMS, p.temporal.preferred_calendar_count);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* Defaults (10) */
    RUN_TEST(test_default_visual_theme_is_cosmos);
    RUN_TEST(test_default_visual_font_scale_is_one);
    RUN_TEST(test_default_visual_opacity_scale_is_one);
    RUN_TEST(test_default_visual_animation_speed_is_one);
    RUN_TEST(test_default_visual_layers_all_visible);
    RUN_TEST(test_default_visual_scale_level_zero);
    RUN_TEST(test_default_temporal_utc_offset_zero);
    RUN_TEST(test_default_temporal_24h_off);
    RUN_TEST(test_default_temporal_week_start_sunday);
    RUN_TEST(test_default_temporal_no_preferred_calendars);
    RUN_TEST(test_default_social_introvert);
    RUN_TEST(test_default_social_anonymous);
    RUN_TEST(test_default_social_notifications_off);
    RUN_TEST(test_default_social_roulette_off);
    RUN_TEST(test_default_social_consent_none);
    RUN_TEST(test_default_social_retention_session);
    RUN_TEST(test_default_location_not_set);
    RUN_TEST(test_default_location_coords_zero);
    RUN_TEST(test_default_location_radius);
    RUN_TEST(test_default_version_is_one);

    /* Validation (15) */
    RUN_TEST(test_validate_clamps_font_scale_low);
    RUN_TEST(test_validate_clamps_font_scale_high);
    RUN_TEST(test_validate_clamps_opacity_scale_low);
    RUN_TEST(test_validate_clamps_opacity_scale_high);
    RUN_TEST(test_validate_clamps_animation_speed_low);
    RUN_TEST(test_validate_clamps_animation_speed_high);
    RUN_TEST(test_validate_clamps_scale_level);
    RUN_TEST(test_validate_clamps_timezone_low);
    RUN_TEST(test_validate_clamps_timezone_high);
    RUN_TEST(test_validate_clamps_week_start);
    RUN_TEST(test_validate_clamps_notification_frequency);
    RUN_TEST(test_validate_clamps_latitude);
    RUN_TEST(test_validate_clamps_longitude);
    RUN_TEST(test_validate_clamps_radius);
    RUN_TEST(test_validate_clamps_radius_min);
    RUN_TEST(test_validate_preserves_valid_prefs);
    RUN_TEST(test_validate_calendar_count_clamped);

    /* Serialization round-trip (2) */
    RUN_TEST(test_serialize_roundtrip_defaults);
    RUN_TEST(test_serialize_roundtrip_custom_values);

    /* Partial deserialization (2) */
    RUN_TEST(test_deserialize_partial_missing_keys_get_defaults);
    RUN_TEST(test_deserialize_empty_string_returns_defaults);

    /* Key-value parsing (4) */
    RUN_TEST(test_parse_kv_valid_pair);
    RUN_TEST(test_parse_kv_no_newline);
    RUN_TEST(test_parse_kv_empty_returns_zero);
    RUN_TEST(test_parse_kv_malformed_no_equals);

    /* Count keys (2) */
    RUN_TEST(test_count_keys_empty);
    RUN_TEST(test_count_keys_three_lines);

    /* Section serialization (4) */
    RUN_TEST(test_serialize_visual_section);
    RUN_TEST(test_serialize_temporal_section);
    RUN_TEST(test_serialize_social_section);
    RUN_TEST(test_serialize_location_section);

    /* Merge (2) */
    RUN_TEST(test_merge_patch_overrides_base);
    RUN_TEST(test_merge_default_patch_preserves_base);

    /* Diff (4) */
    RUN_TEST(test_diff_identical_returns_zero);
    RUN_TEST(test_diff_visual_change);
    RUN_TEST(test_diff_temporal_change);
    RUN_TEST(test_diff_multiple_sections);

    /* Theme prefs integration (2) */
    RUN_TEST(test_to_theme_prefs_defaults);
    RUN_TEST(test_to_theme_prefs_custom_scales);

    /* Privacy settings integration (2) */
    RUN_TEST(test_to_privacy_settings_defaults);
    RUN_TEST(test_to_privacy_settings_full_consent);

    /* Location validation (2) */
    RUN_TEST(test_has_location_false_when_not_set);
    RUN_TEST(test_has_location_true_when_set);

    /* Summary (2) */
    RUN_TEST(test_summary_writes_something);
    RUN_TEST(test_summary_with_location);

    /* Edge cases (4) */
    RUN_TEST(test_serialize_truncation);
    RUN_TEST(test_serialize_null_safe_zero_buffer);
    RUN_TEST(test_deserialize_garbage_returns_defaults);
    RUN_TEST(test_serialize_preferred_calendars);

    return UNITY_END();
}
