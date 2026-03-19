/* test_convergence_forecast.c — Tests for convergence forecast module
 * Predicts upcoming convergence events using cycle-based approximation.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/convergence_forecast.h"

#include <string.h>
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== cf_schedule_count ========== */

static void test_schedule_count_returns_20(void)
{
    TEST_ASSERT_EQUAL_INT(20, cf_schedule_count());
}

/* ========== cf_schedule — specific systems ========== */

static void test_schedule_hebrew(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HEBREW, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 29.53f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Rosh Chodesh", s.event_name);
}

static void test_schedule_tzolkin(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_TZOLKIN, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 13.0f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Wavespell start", s.event_name);
}

static void test_schedule_celtic_like_coptic(void)
{
    /* COPTIC at enum pos 12 — period 30.0, "Month boundary" */
    cf_schedule_t s = cf_schedule(TS_SYS_COPTIC);
    TEST_ASSERT_EQUAL_INT(TS_SYS_COPTIC, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Month boundary", s.event_name);
}

static void test_schedule_gregorian(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GREGORIAN, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.44f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Month boundary", s.event_name);
}

static void test_schedule_haab(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_HAAB);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HAAB, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 20.0f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Month boundary", s.event_name);
}

static void test_schedule_chinese(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_CHINESE);
    TEST_ASSERT_EQUAL_INT(TS_SYS_CHINESE, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 29.53f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("New moon", s.event_name);
}

static void test_schedule_islamic(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_ISLAMIC);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ISLAMIC, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 29.53f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("New crescent", s.event_name);
}

static void test_schedule_buddhist(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_INT(TS_SYS_BUDDHIST, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 7.38f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Uposatha", s.event_name);
}

static void test_schedule_hindu(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_HINDU);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HINDU, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 14.77f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Paksha boundary", s.event_name);
}

static void test_schedule_iching(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_ICHING);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ICHING, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 64.0f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Hexagram 1", s.event_name);
}

static void test_schedule_astrology(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_ASTROLOGY);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ASTROLOGY, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.44f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Sign ingress", s.event_name);
}

static void test_schedule_human_design(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_HUMAN_DESIGN);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HUMAN_DESIGN, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.7f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Gate transition", s.event_name);
}

static void test_schedule_kabbalah(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_KABBALAH);
    TEST_ASSERT_EQUAL_INT(TS_SYS_KABBALAH, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.44f, (float)s.period_days);
    TEST_ASSERT_EQUAL_STRING("Sefirah shift", s.event_name);
}

static void test_schedule_geological_no_events(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_GEOLOGICAL);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GEOLOGICAL, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.period_days);
}

static void test_schedule_cosmic_no_events(void)
{
    cf_schedule_t s = cf_schedule(TS_SYS_COSMIC);
    TEST_ASSERT_EQUAL_INT(TS_SYS_COSMIC, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.period_days);
}

/* ========== cf_schedule — all 20 systems loop ========== */

static void test_schedule_all_nonempty_name_for_active(void)
{
    for (int i = 0; i < 20; i++) {
        cf_schedule_t s = cf_schedule(i);
        TEST_ASSERT_EQUAL_INT(i, s.system_id);
        if (s.period_days > 0.0) {
            TEST_ASSERT_NOT_NULL(s.event_name);
            TEST_ASSERT_TRUE(strlen(s.event_name) > 0);
        }
    }
}

/* ========== cf_schedule — invalid ========== */

static void test_schedule_invalid_positive(void)
{
    cf_schedule_t s = cf_schedule(20);
    TEST_ASSERT_EQUAL_INT(-1, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.period_days);
}

static void test_schedule_invalid_negative(void)
{
    cf_schedule_t s = cf_schedule(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.period_days);
}

/* ========== cf_strength_name ========== */

static void test_strength_name_minor(void)
{
    TEST_ASSERT_EQUAL_STRING("Minor", cf_strength_name(1));
}

static void test_strength_name_notable(void)
{
    TEST_ASSERT_EQUAL_STRING("Notable", cf_strength_name(2));
}

static void test_strength_name_major(void)
{
    TEST_ASSERT_EQUAL_STRING("Major", cf_strength_name(3));
}

static void test_strength_name_rare(void)
{
    TEST_ASSERT_EQUAL_STRING("Rare", cf_strength_name(4));
}

static void test_strength_name_invalid_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cf_strength_name(0));
}

static void test_strength_name_invalid_five(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cf_strength_name(5));
}

static void test_strength_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cf_strength_name(-1));
}

/* ========== cf_forecast — edge cases ========== */

static void test_forecast_jd_zero_empty(void)
{
    cf_forecast_t f = cf_forecast(0.0, 30);
    TEST_ASSERT_EQUAL_INT(0, f.count);
    TEST_ASSERT_EQUAL_INT(0, f.strongest);
}

static void test_forecast_jd_negative_empty(void)
{
    cf_forecast_t f = cf_forecast(-100.0, 30);
    TEST_ASSERT_EQUAL_INT(0, f.count);
    TEST_ASSERT_EQUAL_INT(0, f.strongest);
}

static void test_forecast_max_days_zero_empty(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 0);
    TEST_ASSERT_EQUAL_INT(0, f.count);
    TEST_ASSERT_EQUAL_INT(0, f.strongest);
}

static void test_forecast_max_days_negative_empty(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, -5);
    TEST_ASSERT_EQUAL_INT(0, f.count);
    TEST_ASSERT_EQUAL_INT(0, f.strongest);
}

/* ========== cf_forecast — basic operation ========== */

static void test_forecast_30_days_has_events(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 30);
    TEST_ASSERT_TRUE(f.count > 0);
}

static void test_forecast_1_day_may_be_empty(void)
{
    /* With only 1 day, convergences are unlikely but possible */
    cf_forecast_t f = cf_forecast(2460000.0, 1);
    TEST_ASSERT_TRUE(f.count >= 0);
    TEST_ASSERT_TRUE(f.count <= CF_MAX_FORECASTS);
}

static void test_forecast_events_sorted_by_jd(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 90);
    for (int i = 1; i < f.count; i++) {
        TEST_ASSERT_TRUE(f.events[i].jd >= f.events[i - 1].jd);
    }
}

static void test_forecast_strength_at_least_1(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 90);
    for (int i = 0; i < f.count; i++) {
        TEST_ASSERT_TRUE(f.events[i].strength >= 1);
    }
}

static void test_forecast_system_count_at_least_2(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 90);
    for (int i = 0; i < f.count; i++) {
        TEST_ASSERT_TRUE(f.events[i].system_count >= 2);
    }
}

static void test_forecast_headline_nonempty(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 90);
    for (int i = 0; i < f.count; i++) {
        TEST_ASSERT_TRUE(strlen(f.events[i].headline) > 0);
    }
}

static void test_forecast_strongest_is_max(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 90);
    int max_s = 0;
    for (int i = 0; i < f.count; i++) {
        if (f.events[i].strength > max_s) {
            max_s = f.events[i].strength;
        }
    }
    TEST_ASSERT_EQUAL_INT(max_s, f.strongest);
}

static void test_forecast_max_days_clamped_to_365(void)
{
    /* 1000 days should be clamped to 365 internally */
    cf_forecast_t f = cf_forecast(2460000.0, 1000);
    /* All events should be within 365 days */
    for (int i = 0; i < f.count; i++) {
        TEST_ASSERT_TRUE(f.events[i].days_from_now <= 365);
    }
}

static void test_forecast_days_from_now_positive(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 60);
    for (int i = 0; i < f.count; i++) {
        TEST_ASSERT_TRUE(f.events[i].days_from_now >= 0);
    }
}

static void test_forecast_jd_within_range(void)
{
    double start_jd = 2460000.0;
    int max_days = 60;
    cf_forecast_t f = cf_forecast(start_jd, max_days);
    for (int i = 0; i < f.count; i++) {
        TEST_ASSERT_TRUE(f.events[i].jd >= start_jd);
        TEST_ASSERT_TRUE(f.events[i].jd < start_jd + (double)max_days + 1.0);
    }
}

static void test_forecast_count_within_limit(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 365);
    TEST_ASSERT_TRUE(f.count <= CF_MAX_FORECASTS);
}

static void test_forecast_systems_array_valid(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 60);
    for (int i = 0; i < f.count; i++) {
        for (int j = 0; j < f.events[i].system_count; j++) {
            TEST_ASSERT_TRUE(f.events[i].systems[j] >= 0);
            TEST_ASSERT_TRUE(f.events[i].systems[j] < TS_SYS_COUNT);
        }
    }
}

static void test_forecast_detail_not_null(void)
{
    cf_forecast_t f = cf_forecast(2460000.0, 60);
    for (int i = 0; i < f.count; i++) {
        /* detail may be empty but should be null-terminated */
        TEST_ASSERT_TRUE(strlen(f.events[i].detail) < 256);
    }
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* schedule count */
    RUN_TEST(test_schedule_count_returns_20);

    /* schedule — specific systems */
    RUN_TEST(test_schedule_hebrew);
    RUN_TEST(test_schedule_tzolkin);
    RUN_TEST(test_schedule_celtic_like_coptic);
    RUN_TEST(test_schedule_gregorian);
    RUN_TEST(test_schedule_haab);
    RUN_TEST(test_schedule_chinese);
    RUN_TEST(test_schedule_islamic);
    RUN_TEST(test_schedule_buddhist);
    RUN_TEST(test_schedule_hindu);
    RUN_TEST(test_schedule_iching);
    RUN_TEST(test_schedule_astrology);
    RUN_TEST(test_schedule_human_design);
    RUN_TEST(test_schedule_kabbalah);
    RUN_TEST(test_schedule_geological_no_events);
    RUN_TEST(test_schedule_cosmic_no_events);

    /* schedule — all systems loop */
    RUN_TEST(test_schedule_all_nonempty_name_for_active);

    /* schedule — invalid */
    RUN_TEST(test_schedule_invalid_positive);
    RUN_TEST(test_schedule_invalid_negative);

    /* strength name */
    RUN_TEST(test_strength_name_minor);
    RUN_TEST(test_strength_name_notable);
    RUN_TEST(test_strength_name_major);
    RUN_TEST(test_strength_name_rare);
    RUN_TEST(test_strength_name_invalid_zero);
    RUN_TEST(test_strength_name_invalid_five);
    RUN_TEST(test_strength_name_invalid_negative);

    /* forecast — edge cases */
    RUN_TEST(test_forecast_jd_zero_empty);
    RUN_TEST(test_forecast_jd_negative_empty);
    RUN_TEST(test_forecast_max_days_zero_empty);
    RUN_TEST(test_forecast_max_days_negative_empty);

    /* forecast — basic operation */
    RUN_TEST(test_forecast_30_days_has_events);
    RUN_TEST(test_forecast_1_day_may_be_empty);
    RUN_TEST(test_forecast_events_sorted_by_jd);
    RUN_TEST(test_forecast_strength_at_least_1);
    RUN_TEST(test_forecast_system_count_at_least_2);
    RUN_TEST(test_forecast_headline_nonempty);
    RUN_TEST(test_forecast_strongest_is_max);
    RUN_TEST(test_forecast_max_days_clamped_to_365);
    RUN_TEST(test_forecast_days_from_now_positive);
    RUN_TEST(test_forecast_jd_within_range);
    RUN_TEST(test_forecast_count_within_limit);
    RUN_TEST(test_forecast_systems_array_valid);
    RUN_TEST(test_forecast_detail_not_null);

    return UNITY_END();
}
