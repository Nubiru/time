#include "../unity/unity.h"
#include "../../src/ui/accessibility.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- aria_planet_desc: tests 1-10 ---- */

void test_planet_desc_basic(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 15.3, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "15.3"));
    TEST_ASSERT_TRUE(n > 0);
}

void test_planet_desc_retrograde(void)
{
    aria_planet_t p = { .name = "Mars", .sign = "Scorpio", .degree = 8.7, .is_retrograde = true };
    char buf[ARIA_MAX_LEN];
    aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "retrograde"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mars"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Scorpio"));
}

void test_planet_desc_not_retrograde_no_tag(void)
{
    aria_planet_t p = { .name = "Venus", .sign = "Pisces", .degree = 1.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NULL(strstr(buf, "retrograde"));
}

void test_planet_desc_null_planet(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_planet_desc(NULL, buf, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_planet_desc_null_buf(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 0.0, .is_retrograde = false };
    int n = aria_planet_desc(&p, NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_planet_desc_zero_len(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 0.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_planet_desc(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_planet_desc_null_name(void)
{
    aria_planet_t p = { .name = NULL, .sign = "Aries", .degree = 5.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_planet_desc_null_sign(void)
{
    aria_planet_t p = { .name = "Moon", .sign = NULL, .degree = 12.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_planet_desc_small_buf(void)
{
    aria_planet_t p = { .name = "Mercury", .sign = "Gemini", .degree = 20.5, .is_retrograde = false };
    char buf[16];
    int n = aria_planet_desc(&p, buf, 16);
    /* Should truncate but not overflow */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n < 16);
    TEST_ASSERT_TRUE(strlen(buf) < 16);
}

void test_planet_desc_degree_zero(void)
{
    aria_planet_t p = { .name = "Jupiter", .sign = "Leo", .degree = 0.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "0.0"));
}

/* ---- aria_sky_desc: tests 11-20 ---- */

void test_sky_desc_single_planet(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 15.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_sky_desc(&p, 1, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Aries"));
}

void test_sky_desc_two_planets(void)
{
    aria_planet_t ps[2] = {
        { .name = "Sun", .sign = "Aries", .degree = 15.0, .is_retrograde = false },
        { .name = "Moon", .sign = "Cancer", .degree = 22.0, .is_retrograde = false }
    };
    char buf[ARIA_MAX_LEN];
    int n = aria_sky_desc(ps, 2, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon"));
}

void test_sky_desc_comma_separator(void)
{
    aria_planet_t ps[2] = {
        { .name = "Sun", .sign = "Aries", .degree = 15.0, .is_retrograde = false },
        { .name = "Moon", .sign = "Cancer", .degree = 22.0, .is_retrograde = false }
    };
    char buf[ARIA_MAX_LEN];
    aria_sky_desc(ps, 2, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, ", "));
}

void test_sky_desc_zero_count(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 0.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_sky_desc(&p, 0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_sky_desc_null_planets(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_sky_desc(NULL, 3, buf, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_sky_desc_null_buf(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 0.0, .is_retrograde = false };
    int n = aria_sky_desc(&p, 1, NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_sky_desc_zero_len(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 0.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_sky_desc(&p, 1, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_sky_desc_retrograde_planet(void)
{
    aria_planet_t ps[2] = {
        { .name = "Sun", .sign = "Aries", .degree = 10.0, .is_retrograde = false },
        { .name = "Saturn", .sign = "Aquarius", .degree = 5.0, .is_retrograde = true }
    };
    char buf[512];
    aria_sky_desc(ps, 2, buf, 512);
    TEST_ASSERT_NOT_NULL(strstr(buf, "retrograde"));
}

void test_sky_desc_negative_count(void)
{
    aria_planet_t p = { .name = "Sun", .sign = "Aries", .degree = 0.0, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    int n = aria_sky_desc(&p, -1, buf, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_sky_desc_small_buf_truncates(void)
{
    aria_planet_t ps[3] = {
        { .name = "Sun", .sign = "Aries", .degree = 15.0, .is_retrograde = false },
        { .name = "Moon", .sign = "Cancer", .degree = 22.0, .is_retrograde = false },
        { .name = "Mars", .sign = "Scorpio", .degree = 8.0, .is_retrograde = true }
    };
    char buf[32];
    int n = aria_sky_desc(ps, 3, buf, 32);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ---- aria_datetime_desc: tests 21-28 ---- */

void test_datetime_desc_basic(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_datetime_desc(2024, 1, 15, 14, 30, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "January"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "15"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2024"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "14:30"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "UTC"));
}

void test_datetime_desc_december(void)
{
    char buf[ARIA_MAX_LEN];
    aria_datetime_desc(2025, 12, 25, 0, 0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "December"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "25"));
}

void test_datetime_desc_null_buf(void)
{
    int n = aria_datetime_desc(2024, 1, 1, 0, 0, NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_datetime_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_datetime_desc(2024, 1, 1, 0, 0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_datetime_desc_invalid_month_low(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_datetime_desc(2024, 0, 1, 0, 0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    /* Should fall back gracefully */
}

void test_datetime_desc_invalid_month_high(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_datetime_desc(2024, 13, 1, 0, 0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
}

void test_datetime_desc_all_months(void)
{
    const char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    for (int m = 1; m <= 12; m++) {
        char buf[ARIA_MAX_LEN];
        aria_datetime_desc(2024, m, 1, 12, 0, buf, ARIA_MAX_LEN);
        TEST_ASSERT_NOT_NULL(strstr(buf, months[m - 1]));
    }
}

void test_datetime_desc_midnight(void)
{
    char buf[ARIA_MAX_LEN];
    aria_datetime_desc(2024, 6, 21, 0, 0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "0:00"));
}

/* ---- aria_calendar_desc: tests 29-34 ---- */

void test_calendar_desc_basic(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_calendar_desc("Hebrew", "5 Shevat 5784", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hebrew"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "5 Shevat 5784"));
}

void test_calendar_desc_chinese(void)
{
    char buf[ARIA_MAX_LEN];
    aria_calendar_desc("Chinese", "Year of the Dragon", buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Chinese"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Year of the Dragon"));
}

void test_calendar_desc_null_system(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_calendar_desc(NULL, "some date", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_calendar_desc_null_date(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_calendar_desc("Islamic", NULL, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_calendar_desc_null_buf(void)
{
    int n = aria_calendar_desc("Hebrew", "date", NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_calendar_desc_format(void)
{
    char buf[ARIA_MAX_LEN];
    aria_calendar_desc("Buddhist", "2568 BE", buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "calendar"));
}

/* ---- aria_festival_desc: tests 35-40 ---- */

void test_festival_desc_basic(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_festival_desc("Coptic", "Nayrouz, Coptic New Year", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Nayrouz"));
}

void test_festival_desc_contains_alert(void)
{
    char buf[ARIA_MAX_LEN];
    aria_festival_desc("Hindu", "Diwali", buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Festival"));
}

void test_festival_desc_null_system(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_festival_desc(NULL, "Easter", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
}

void test_festival_desc_null_name(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_festival_desc("Hebrew", NULL, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_festival_desc_null_buf(void)
{
    int n = aria_festival_desc("Hindu", "Diwali", NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_festival_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_festival_desc("Hindu", "Diwali", buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- aria_scale_desc: tests 41-44 ---- */

void test_scale_desc_basic(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_scale_desc("Solar System", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Solar System"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Zoom"));
}

void test_scale_desc_null_name(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_scale_desc(NULL, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_scale_desc_null_buf(void)
{
    int n = aria_scale_desc("Galaxy", NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_scale_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_scale_desc("Galaxy", buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- aria_view_desc: tests 45-48 ---- */

void test_view_desc_basic(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_view_desc("Earth surface", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "View"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth surface"));
}

void test_view_desc_null_name(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_view_desc(NULL, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_view_desc_null_buf(void)
{
    int n = aria_view_desc("Space", NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_view_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_view_desc("Space", buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- aria_event_desc: tests 49-55 ---- */

void test_event_desc_scale_change(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_event_desc(ARIA_EVENT_SCALE_CHANGE, "zoomed to inner planets", buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "zoomed to inner planets"));
}

void test_event_desc_festival(void)
{
    char buf[ARIA_MAX_LEN];
    aria_event_desc(ARIA_EVENT_FESTIVAL, "Passover begins", buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Passover"));
}

void test_event_desc_none(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_event_desc(ARIA_EVENT_NONE, "nothing", buf, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_event_desc_null_detail(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_event_desc(ARIA_EVENT_VIEW_CHANGE, NULL, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
}

void test_event_desc_null_buf(void)
{
    int n = aria_event_desc(ARIA_EVENT_ASPECT, "conjunction", NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_event_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_event_desc(ARIA_EVENT_ASPECT, "conjunction", buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_event_desc_invalid_event(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_event_desc(ARIA_EVENT_COUNT, "invalid", buf, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ---- aria_event_name: tests 56-60 ---- */

void test_event_name_scale(void)
{
    const char *name = aria_event_name(ARIA_EVENT_SCALE_CHANGE);
    TEST_ASSERT_EQUAL_STRING("Scale Change", name);
}

void test_event_name_system_focus(void)
{
    const char *name = aria_event_name(ARIA_EVENT_SYSTEM_FOCUS);
    TEST_ASSERT_EQUAL_STRING("System Focus", name);
}

void test_event_name_festival(void)
{
    const char *name = aria_event_name(ARIA_EVENT_FESTIVAL);
    TEST_ASSERT_EQUAL_STRING("Festival", name);
}

void test_event_name_none(void)
{
    const char *name = aria_event_name(ARIA_EVENT_NONE);
    TEST_ASSERT_EQUAL_STRING("None", name);
}

void test_event_name_invalid(void)
{
    const char *name = aria_event_name(ARIA_EVENT_COUNT);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

/* ---- aria_event_count: test 61 ---- */

void test_event_count(void)
{
    TEST_ASSERT_EQUAL_INT(ARIA_EVENT_COUNT, aria_event_count());
}

/* ---- aria_retrograde_desc: tests 62-67 ---- */

void test_retrograde_desc_is_retro(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_retrograde_desc("Mars", true, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mars"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "retrograde"));
}

void test_retrograde_desc_is_direct(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_retrograde_desc("Venus", false, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Venus"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "direct"));
}

void test_retrograde_desc_null_name(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_retrograde_desc(NULL, true, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_retrograde_desc_null_buf(void)
{
    int n = aria_retrograde_desc("Mars", true, NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_retrograde_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_retrograde_desc("Mars", true, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_retrograde_desc_small_buf(void)
{
    char buf[10];
    int n = aria_retrograde_desc("Mercury", true, buf, 10);
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 10);
}

/* ---- aria_aspect_desc: tests 68-76 ---- */

void test_aspect_desc_conjunction(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_aspect_desc("Sun", "Moon", "conjunction", 0.5, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "conjunction"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "0.5"));
}

void test_aspect_desc_opposition(void)
{
    char buf[ARIA_MAX_LEN];
    aria_aspect_desc("Mars", "Jupiter", "opposition", 179.3, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "opposition"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "179.3"));
}

void test_aspect_desc_trine(void)
{
    char buf[ARIA_MAX_LEN];
    aria_aspect_desc("Venus", "Saturn", "trine", 120.1, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "trine"));
}

void test_aspect_desc_null_planet_a(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_aspect_desc(NULL, "Moon", "conjunction", 0.0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_aspect_desc_null_planet_b(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_aspect_desc("Sun", NULL, "sextile", 60.0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_aspect_desc_null_aspect_name(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_aspect_desc("Sun", "Moon", NULL, 0.0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "unknown"));
}

void test_aspect_desc_null_buf(void)
{
    int n = aria_aspect_desc("Sun", "Moon", "conjunction", 0.0, NULL, ARIA_MAX_LEN);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_aspect_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_aspect_desc("Sun", "Moon", "conjunction", 0.0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_aspect_desc_separation_format(void)
{
    char buf[ARIA_MAX_LEN];
    aria_aspect_desc("Sun", "Moon", "conjunction", 0.0, buf, ARIA_MAX_LEN);
    TEST_ASSERT_NOT_NULL(strstr(buf, "separation"));
}

/* ---- Edge cases and integration: tests 77-82 ---- */

void test_planet_desc_degree_precision(void)
{
    aria_planet_t p = { .name = "Pluto", .sign = "Capricorn", .degree = 29.99, .is_retrograde = false };
    char buf[ARIA_MAX_LEN];
    aria_planet_desc(&p, buf, ARIA_MAX_LEN);
    /* Should show 1 decimal place: 30.0 (rounded) */
    TEST_ASSERT_TRUE(strstr(buf, "30.0") != NULL || strstr(buf, "29.9") != NULL);
    /* Check it wrote something reasonable */
    TEST_ASSERT_TRUE(strlen(buf) > 10);
}

void test_sky_desc_many_planets(void)
{
    aria_planet_t ps[5] = {
        { .name = "Sun", .sign = "Aries", .degree = 1.0, .is_retrograde = false },
        { .name = "Moon", .sign = "Taurus", .degree = 2.0, .is_retrograde = false },
        { .name = "Mercury", .sign = "Gemini", .degree = 3.0, .is_retrograde = true },
        { .name = "Venus", .sign = "Cancer", .degree = 4.0, .is_retrograde = false },
        { .name = "Mars", .sign = "Leo", .degree = 5.0, .is_retrograde = false }
    };
    char buf[512];
    int n = aria_sky_desc(ps, 5, buf, 512);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mars"));
}

void test_calendar_desc_zero_len(void)
{
    char buf[ARIA_MAX_LEN];
    int n = aria_calendar_desc("Hebrew", "date", buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_event_name_aspect(void)
{
    const char *name = aria_event_name(ARIA_EVENT_ASPECT);
    TEST_ASSERT_EQUAL_STRING("Aspect", name);
}

void test_event_name_time_jump(void)
{
    const char *name = aria_event_name(ARIA_EVENT_TIME_JUMP);
    TEST_ASSERT_EQUAL_STRING("Time Jump", name);
}

void test_event_name_view_change(void)
{
    const char *name = aria_event_name(ARIA_EVENT_VIEW_CHANGE);
    TEST_ASSERT_EQUAL_STRING("View Change", name);
}

/* ---- Main runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* aria_planet_desc: 1-10 */
    RUN_TEST(test_planet_desc_basic);
    RUN_TEST(test_planet_desc_retrograde);
    RUN_TEST(test_planet_desc_not_retrograde_no_tag);
    RUN_TEST(test_planet_desc_null_planet);
    RUN_TEST(test_planet_desc_null_buf);
    RUN_TEST(test_planet_desc_zero_len);
    RUN_TEST(test_planet_desc_null_name);
    RUN_TEST(test_planet_desc_null_sign);
    RUN_TEST(test_planet_desc_small_buf);
    RUN_TEST(test_planet_desc_degree_zero);

    /* aria_sky_desc: 11-20 */
    RUN_TEST(test_sky_desc_single_planet);
    RUN_TEST(test_sky_desc_two_planets);
    RUN_TEST(test_sky_desc_comma_separator);
    RUN_TEST(test_sky_desc_zero_count);
    RUN_TEST(test_sky_desc_null_planets);
    RUN_TEST(test_sky_desc_null_buf);
    RUN_TEST(test_sky_desc_zero_len);
    RUN_TEST(test_sky_desc_retrograde_planet);
    RUN_TEST(test_sky_desc_negative_count);
    RUN_TEST(test_sky_desc_small_buf_truncates);

    /* aria_datetime_desc: 21-28 */
    RUN_TEST(test_datetime_desc_basic);
    RUN_TEST(test_datetime_desc_december);
    RUN_TEST(test_datetime_desc_null_buf);
    RUN_TEST(test_datetime_desc_zero_len);
    RUN_TEST(test_datetime_desc_invalid_month_low);
    RUN_TEST(test_datetime_desc_invalid_month_high);
    RUN_TEST(test_datetime_desc_all_months);
    RUN_TEST(test_datetime_desc_midnight);

    /* aria_calendar_desc: 29-34 */
    RUN_TEST(test_calendar_desc_basic);
    RUN_TEST(test_calendar_desc_chinese);
    RUN_TEST(test_calendar_desc_null_system);
    RUN_TEST(test_calendar_desc_null_date);
    RUN_TEST(test_calendar_desc_null_buf);
    RUN_TEST(test_calendar_desc_format);

    /* aria_festival_desc: 35-40 */
    RUN_TEST(test_festival_desc_basic);
    RUN_TEST(test_festival_desc_contains_alert);
    RUN_TEST(test_festival_desc_null_system);
    RUN_TEST(test_festival_desc_null_name);
    RUN_TEST(test_festival_desc_null_buf);
    RUN_TEST(test_festival_desc_zero_len);

    /* aria_scale_desc: 41-44 */
    RUN_TEST(test_scale_desc_basic);
    RUN_TEST(test_scale_desc_null_name);
    RUN_TEST(test_scale_desc_null_buf);
    RUN_TEST(test_scale_desc_zero_len);

    /* aria_view_desc: 45-48 */
    RUN_TEST(test_view_desc_basic);
    RUN_TEST(test_view_desc_null_name);
    RUN_TEST(test_view_desc_null_buf);
    RUN_TEST(test_view_desc_zero_len);

    /* aria_event_desc: 49-55 */
    RUN_TEST(test_event_desc_scale_change);
    RUN_TEST(test_event_desc_festival);
    RUN_TEST(test_event_desc_none);
    RUN_TEST(test_event_desc_null_detail);
    RUN_TEST(test_event_desc_null_buf);
    RUN_TEST(test_event_desc_zero_len);
    RUN_TEST(test_event_desc_invalid_event);

    /* aria_event_name: 56-60 */
    RUN_TEST(test_event_name_scale);
    RUN_TEST(test_event_name_system_focus);
    RUN_TEST(test_event_name_festival);
    RUN_TEST(test_event_name_none);
    RUN_TEST(test_event_name_invalid);

    /* aria_event_count: 61 */
    RUN_TEST(test_event_count);

    /* aria_retrograde_desc: 62-67 */
    RUN_TEST(test_retrograde_desc_is_retro);
    RUN_TEST(test_retrograde_desc_is_direct);
    RUN_TEST(test_retrograde_desc_null_name);
    RUN_TEST(test_retrograde_desc_null_buf);
    RUN_TEST(test_retrograde_desc_zero_len);
    RUN_TEST(test_retrograde_desc_small_buf);

    /* aria_aspect_desc: 68-76 */
    RUN_TEST(test_aspect_desc_conjunction);
    RUN_TEST(test_aspect_desc_opposition);
    RUN_TEST(test_aspect_desc_trine);
    RUN_TEST(test_aspect_desc_null_planet_a);
    RUN_TEST(test_aspect_desc_null_planet_b);
    RUN_TEST(test_aspect_desc_null_aspect_name);
    RUN_TEST(test_aspect_desc_null_buf);
    RUN_TEST(test_aspect_desc_zero_len);
    RUN_TEST(test_aspect_desc_separation_format);

    /* Edge cases: 77-82 */
    RUN_TEST(test_planet_desc_degree_precision);
    RUN_TEST(test_sky_desc_many_planets);
    RUN_TEST(test_calendar_desc_zero_len);
    RUN_TEST(test_event_name_aspect);
    RUN_TEST(test_event_name_time_jump);
    RUN_TEST(test_event_name_view_change);

    return UNITY_END();
}
