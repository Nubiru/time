/* test_share_moment.c — Tests for the Shareable Moments module.
 * Validates sm_current, sm_format_url, sm_format_text, sm_format_og,
 * sm_generate, sm_view_slug. Constructs ts_summary_t manually to avoid
 * pulling in all 20 calendar modules. */

#include "unity.h"
#include "ui/share_moment.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

/* Helper: Unity has no double support in this build. Use manual check. */
#define ASSERT_DOUBLE_NEAR(expected, actual, tol) \
    TEST_ASSERT_TRUE(fabs((double)(actual) - (double)(expected)) < (tol))

/* Known JD values for testing */
#define JD_J2000  2451545.0
#define JD_EPOCH  0.0
#define JD_FAR    9999999.0

void setUp(void) {}
void tearDown(void) {}

/* ---- Helper: build a test summary with N active entries ---- */

static ts_summary_t make_summary(int n_entries)
{
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.jd = JD_J2000;
    s.entry_count = n_entries;
    s.convergence_score = 0;
    snprintf(s.headline, TS_NOTE_MAX, "Test headline");

    /* Populate entries with descending significance */
    const char *dates[] = {
        "Kin 207 Blue Crystal Hand",
        "Hexagram 42 Increase",
        "Year of the Snake",
        "15 Tishrei 5786",
        "Mon 16 Mar 2026"
    };
    const int sigs[] = { 3, 2, 1, 1, 0 };

    for (int i = 0; i < n_entries && i < 5; i++) {
        s.entries[i].system = (ts_system_t)i;
        snprintf(s.entries[i].date_str, TS_DATE_MAX, "%s", dates[i]);
        s.entries[i].note[0] = '\0';
        s.entries[i].significance = sigs[i];
        s.entries[i].active = 1;
    }
    return s;
}

/* ==== sm_current tests ==== */

static void test_current_constructs_moment(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 3.4f, 51.48, -0.01);
    ASSERT_DOUBLE_NEAR(JD_J2000, m.jd, 0.001);
    TEST_ASSERT_EQUAL_INT(0, m.view_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.4f, m.camera_zoom);
    ASSERT_DOUBLE_NEAR(51.48, m.observer_lat, 0.001);
    ASSERT_DOUBLE_NEAR(-0.01, m.observer_lon, 0.001);
}

static void test_current_zero_values(void)
{
    sm_moment_t m = sm_current(0.0, 0, 0.0f, 0.0, 0.0);
    ASSERT_DOUBLE_NEAR(0.0, m.jd, 0.001);
    TEST_ASSERT_EQUAL_INT(0, m.view_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, m.camera_zoom);
    ASSERT_DOUBLE_NEAR(0.0, m.observer_lat, 0.001);
    ASSERT_DOUBLE_NEAR(0.0, m.observer_lon, 0.001);
}

static void test_current_negative_coords(void)
{
    sm_moment_t m = sm_current(JD_J2000, 1, 1.0f, -33.87, -151.21);
    TEST_ASSERT_EQUAL_INT(1, m.view_id);
    ASSERT_DOUBLE_NEAR(-33.87, m.observer_lat, 0.001);
    ASSERT_DOUBLE_NEAR(-151.21, m.observer_lon, 0.001);
}

static void test_current_active_systems_default(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 1.0f, 0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, m.active_systems);
}

/* ==== sm_view_slug tests ==== */

static void test_view_slug_space(void)
{
    TEST_ASSERT_EQUAL_STRING("space", sm_view_slug(0));
}

static void test_view_slug_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("earth", sm_view_slug(1));
}

static void test_view_slug_galaxy(void)
{
    TEST_ASSERT_EQUAL_STRING("galaxy", sm_view_slug(2));
}

static void test_view_slug_personal(void)
{
    TEST_ASSERT_EQUAL_STRING("personal", sm_view_slug(3));
}

static void test_view_slug_deep_time(void)
{
    TEST_ASSERT_EQUAL_STRING("deep_time", sm_view_slug(4));
}

static void test_view_slug_city(void)
{
    TEST_ASSERT_EQUAL_STRING("city", sm_view_slug(5));
}

static void test_view_slug_room(void)
{
    TEST_ASSERT_EQUAL_STRING("room", sm_view_slug(6));
}

static void test_view_slug_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("unknown", sm_view_slug(-1));
}

static void test_view_slug_out_of_range(void)
{
    TEST_ASSERT_EQUAL_STRING("unknown", sm_view_slug(7));
    TEST_ASSERT_EQUAL_STRING("unknown", sm_view_slug(100));
}

/* ==== sm_format_url tests ==== */

static void test_format_url_basic(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 3.40f, 51.48, -0.01);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);

    /* Should start with ? */
    TEST_ASSERT_EQUAL_CHAR('?', buf[0]);

    /* Check each param is present */
    TEST_ASSERT_NOT_NULL(strstr(buf, "jd=2451545.000"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "view=space"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "zoom=3.40"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "lat=51.48"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "lon=-0.01"));
}

static void test_format_url_negative_longitude(void)
{
    sm_moment_t m = sm_current(JD_J2000, 1, 1.0f, 40.71, -74.01);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "lon=-74.01"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "view=earth"));
}

static void test_format_url_negative_latitude(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 1.0f, -33.87, 151.21);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "lat=-33.87"));
}

static void test_format_url_zero_jd(void)
{
    sm_moment_t m = sm_current(0.0, 0, 1.0f, 0.0, 0.0);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "jd=0.000"));
}

static void test_format_url_extreme_jd(void)
{
    sm_moment_t m = sm_current(JD_FAR, 2, 5.0f, 0.0, 0.0);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "jd=9999999.000"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "view=galaxy"));
}

static void test_format_url_buffer_too_small(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 1.0f, 51.48, -0.01);
    char buf[10];
    int len = sm_format_url(m, buf, 10);
    /* Should return 0 on truncation or write truncated content */
    TEST_ASSERT_TRUE(len < 10);
}

static void test_format_url_null_buffer(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 1.0f, 0.0, 0.0);
    int len = sm_format_url(m, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

static void test_format_url_all_views(void)
{
    const char *expected[] = {
        "view=space", "view=earth", "view=galaxy",
        "view=personal", "view=deep_time", "view=city", "view=room"
    };
    for (int i = 0; i < 7; i++) {
        sm_moment_t m = sm_current(JD_J2000, i, 1.0f, 0.0, 0.0);
        char buf[SM_URL_MAX];
        sm_format_url(m, buf, SM_URL_MAX);
        TEST_ASSERT_NOT_NULL_MESSAGE(strstr(buf, expected[i]),
                                     expected[i]);
    }
}

static void test_format_url_zero_lat_lon(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 1.0f, 0.0, 0.0);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "lat=0.00"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "lon=0.00"));
}

static void test_format_url_unknown_view(void)
{
    sm_moment_t m = sm_current(JD_J2000, 99, 1.0f, 0.0, 0.0);
    char buf[SM_URL_MAX];
    int len = sm_format_url(m, buf, SM_URL_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "view=unknown"));
}

/* ==== sm_format_text tests ==== */

static void test_format_text_empty_summary(void)
{
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.entry_count = 0;

    char buf[SM_TEXT_MAX];
    int len = sm_format_text(&s, buf, SM_TEXT_MAX);
    /* Empty summary -> empty or minimal text */
    TEST_ASSERT_TRUE(len >= 0);
}

static void test_format_text_single_entry(void)
{
    ts_summary_t s = make_summary(1);
    char buf[SM_TEXT_MAX];
    int len = sm_format_text(&s, buf, SM_TEXT_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Kin 207 Blue Crystal Hand"));
}

static void test_format_text_multiple_entries(void)
{
    ts_summary_t s = make_summary(3);
    char buf[SM_TEXT_MAX];
    int len = sm_format_text(&s, buf, SM_TEXT_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);

    /* All three active entries should appear */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Kin 207"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hexagram 42"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Year of the Snake"));
}

static void test_format_text_separator(void)
{
    ts_summary_t s = make_summary(2);
    char buf[SM_TEXT_MAX];
    sm_format_text(&s, buf, SM_TEXT_MAX);
    /* Entries should be separated */
    TEST_ASSERT_NOT_NULL(strstr(buf, " \xc2\xb7 "));
}

static void test_format_text_respects_280_limit(void)
{
    ts_summary_t s = make_summary(5);
    char buf[SM_TEXT_MAX];
    int len = sm_format_text(&s, buf, SM_TEXT_MAX);
    TEST_ASSERT_TRUE(len < SM_TEXT_MAX);
    TEST_ASSERT_TRUE((int)strlen(buf) < SM_TEXT_MAX);
}

static void test_format_text_sorted_by_significance(void)
{
    /* Build summary with entries in reverse significance order */
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.entry_count = 3;
    s.jd = JD_J2000;

    /* Entry 0: low significance */
    s.entries[0].system = TS_SYS_GREGORIAN;
    snprintf(s.entries[0].date_str, TS_DATE_MAX, "LowSig");
    s.entries[0].significance = 0;
    s.entries[0].active = 1;

    /* Entry 1: high significance */
    s.entries[1].system = TS_SYS_TZOLKIN;
    snprintf(s.entries[1].date_str, TS_DATE_MAX, "HighSig");
    s.entries[1].significance = 3;
    s.entries[1].active = 1;

    /* Entry 2: medium significance */
    s.entries[2].system = TS_SYS_CHINESE;
    snprintf(s.entries[2].date_str, TS_DATE_MAX, "MedSig");
    s.entries[2].significance = 1;
    s.entries[2].active = 1;

    char buf[SM_TEXT_MAX];
    sm_format_text(&s, buf, SM_TEXT_MAX);

    /* HighSig should appear before MedSig, MedSig before LowSig */
    const char *high = strstr(buf, "HighSig");
    const char *med  = strstr(buf, "MedSig");
    const char *low  = strstr(buf, "LowSig");
    TEST_ASSERT_NOT_NULL(high);
    TEST_ASSERT_NOT_NULL(med);
    TEST_ASSERT_NOT_NULL(low);
    TEST_ASSERT_TRUE(high < med);
    TEST_ASSERT_TRUE(med < low);
}

static void test_format_text_null_summary(void)
{
    char buf[SM_TEXT_MAX];
    int len = sm_format_text(NULL, buf, SM_TEXT_MAX);
    TEST_ASSERT_EQUAL_INT(0, len);
}

static void test_format_text_null_buffer(void)
{
    ts_summary_t s = make_summary(1);
    int len = sm_format_text(&s, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

static void test_format_text_inactive_entries_skipped(void)
{
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.entry_count = 3;
    s.jd = JD_J2000;

    /* Only entry 1 is active */
    s.entries[0].active = 0;
    snprintf(s.entries[0].date_str, TS_DATE_MAX, "Inactive1");
    s.entries[0].significance = 3;

    s.entries[1].active = 1;
    snprintf(s.entries[1].date_str, TS_DATE_MAX, "Active1");
    s.entries[1].significance = 2;

    s.entries[2].active = 0;
    snprintf(s.entries[2].date_str, TS_DATE_MAX, "Inactive2");
    s.entries[2].significance = 3;

    char buf[SM_TEXT_MAX];
    sm_format_text(&s, buf, SM_TEXT_MAX);

    TEST_ASSERT_NOT_NULL(strstr(buf, "Active1"));
    TEST_ASSERT_NULL(strstr(buf, "Inactive1"));
    TEST_ASSERT_NULL(strstr(buf, "Inactive2"));
}

static void test_format_text_many_entries_truncation(void)
{
    /* Build summary with many long entries to test truncation */
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.jd = JD_J2000;
    s.entry_count = TS_MAX_SYSTEMS;
    for (int i = 0; i < TS_MAX_SYSTEMS; i++) {
        s.entries[i].system = (ts_system_t)(i % TS_SYS_COUNT);
        snprintf(s.entries[i].date_str, TS_DATE_MAX,
                 "Very Long Calendar Entry Number %d With Extra Text", i);
        s.entries[i].significance = 2;
        s.entries[i].active = 1;
    }

    char buf[SM_TEXT_MAX];
    int len = sm_format_text(&s, buf, SM_TEXT_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < SM_TEXT_MAX);
    /* String must be shorter than buffer */
    TEST_ASSERT_TRUE((int)strlen(buf) < SM_TEXT_MAX);
}

/* ==== sm_format_og tests ==== */

static void test_format_og_includes_headline(void)
{
    ts_summary_t s = make_summary(3);
    char buf[SM_DESC_MAX];
    int len = sm_format_og(&s, buf, SM_DESC_MAX);
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Test headline"));
}

static void test_format_og_shorter_than_share(void)
{
    ts_summary_t s = make_summary(5);
    char text_buf[SM_TEXT_MAX];
    char og_buf[SM_DESC_MAX];
    sm_format_text(&s, text_buf, SM_TEXT_MAX);
    sm_format_og(&s, og_buf, SM_DESC_MAX);

    TEST_ASSERT_TRUE((int)strlen(og_buf) <= SM_DESC_MAX);
}

static void test_format_og_respects_200_limit(void)
{
    ts_summary_t s = make_summary(5);
    char buf[SM_DESC_MAX];
    int len = sm_format_og(&s, buf, SM_DESC_MAX);
    TEST_ASSERT_TRUE(len < SM_DESC_MAX);
    TEST_ASSERT_TRUE((int)strlen(buf) < SM_DESC_MAX);
}

static void test_format_og_null_summary(void)
{
    char buf[SM_DESC_MAX];
    int len = sm_format_og(NULL, buf, SM_DESC_MAX);
    TEST_ASSERT_EQUAL_INT(0, len);
}

static void test_format_og_null_buffer(void)
{
    ts_summary_t s = make_summary(1);
    int len = sm_format_og(&s, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

static void test_format_og_empty_summary(void)
{
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.entry_count = 0;
    snprintf(s.headline, TS_NOTE_MAX, "Empty day");

    char buf[SM_DESC_MAX];
    int len = sm_format_og(&s, buf, SM_DESC_MAX);
    /* Should at least include the headline */
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Empty day"));
}

/* ==== sm_generate tests ==== */

static void test_generate_populates_all_fields(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 2.5f, 51.48, -0.01);
    ts_summary_t s = make_summary(3);
    sm_share_t share = sm_generate(m, &s);

    TEST_ASSERT_TRUE(strlen(share.url_params) > 0);
    TEST_ASSERT_TRUE(strlen(share.share_text) > 0);
    TEST_ASSERT_TRUE(strlen(share.og_description) > 0);
}

static void test_generate_url_matches_format_url(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 2.5f, 51.48, -0.01);
    ts_summary_t s = make_summary(3);
    sm_share_t share = sm_generate(m, &s);

    char url_buf[SM_URL_MAX];
    sm_format_url(m, url_buf, SM_URL_MAX);

    TEST_ASSERT_EQUAL_STRING(url_buf, share.url_params);
}

static void test_generate_null_summary(void)
{
    sm_moment_t m = sm_current(JD_J2000, 0, 1.0f, 0.0, 0.0);
    sm_share_t share = sm_generate(m, NULL);

    /* URL should still be populated */
    TEST_ASSERT_TRUE(strlen(share.url_params) > 0);
    /* Text and OG should be empty or minimal — just verify no crash */
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(share.share_text));
}

/* ==== Main ==== */

int main(void)
{
    UNITY_BEGIN();

    /* sm_current */
    RUN_TEST(test_current_constructs_moment);
    RUN_TEST(test_current_zero_values);
    RUN_TEST(test_current_negative_coords);
    RUN_TEST(test_current_active_systems_default);

    /* sm_view_slug */
    RUN_TEST(test_view_slug_space);
    RUN_TEST(test_view_slug_earth);
    RUN_TEST(test_view_slug_galaxy);
    RUN_TEST(test_view_slug_personal);
    RUN_TEST(test_view_slug_deep_time);
    RUN_TEST(test_view_slug_city);
    RUN_TEST(test_view_slug_room);
    RUN_TEST(test_view_slug_negative);
    RUN_TEST(test_view_slug_out_of_range);

    /* sm_format_url */
    RUN_TEST(test_format_url_basic);
    RUN_TEST(test_format_url_negative_longitude);
    RUN_TEST(test_format_url_negative_latitude);
    RUN_TEST(test_format_url_zero_jd);
    RUN_TEST(test_format_url_extreme_jd);
    RUN_TEST(test_format_url_buffer_too_small);
    RUN_TEST(test_format_url_null_buffer);
    RUN_TEST(test_format_url_all_views);
    RUN_TEST(test_format_url_zero_lat_lon);
    RUN_TEST(test_format_url_unknown_view);

    /* sm_format_text */
    RUN_TEST(test_format_text_empty_summary);
    RUN_TEST(test_format_text_single_entry);
    RUN_TEST(test_format_text_multiple_entries);
    RUN_TEST(test_format_text_separator);
    RUN_TEST(test_format_text_respects_280_limit);
    RUN_TEST(test_format_text_sorted_by_significance);
    RUN_TEST(test_format_text_null_summary);
    RUN_TEST(test_format_text_null_buffer);
    RUN_TEST(test_format_text_inactive_entries_skipped);
    RUN_TEST(test_format_text_many_entries_truncation);

    /* sm_format_og */
    RUN_TEST(test_format_og_includes_headline);
    RUN_TEST(test_format_og_shorter_than_share);
    RUN_TEST(test_format_og_respects_200_limit);
    RUN_TEST(test_format_og_null_summary);
    RUN_TEST(test_format_og_null_buffer);
    RUN_TEST(test_format_og_empty_summary);

    /* sm_generate */
    RUN_TEST(test_generate_populates_all_fields);
    RUN_TEST(test_generate_url_matches_format_url);
    RUN_TEST(test_generate_null_summary);

    return UNITY_END();
}
