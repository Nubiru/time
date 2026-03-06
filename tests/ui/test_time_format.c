#include "../unity/unity.h"
#include "../../src/ui/time_format.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* JD grouped: 1-3 */

void test_jd_grouped_j2000(void)
{
    char buf[48];
    fmt_jd_grouped(2451545.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2,451,545"));
}

void test_jd_grouped_small(void)
{
    char buf[48];
    fmt_jd_grouped(100.5, buf, sizeof(buf));
    /* No comma needed for < 1000 */
    TEST_ASSERT_NOT_NULL(strstr(buf, "100"));
}

void test_jd_grouped_large(void)
{
    char buf[48];
    fmt_jd_grouped(12345678.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "12,345,678"));
}

/* Adaptive time: 4-9 */

void test_adaptive_seconds(void)
{
    char buf[32];
    fmt_time_adaptive(0.5, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "s"));
}

void test_adaptive_minutes(void)
{
    char buf[32];
    fmt_time_adaptive(150.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "m"));
}

void test_adaptive_hours(void)
{
    char buf[32];
    fmt_time_adaptive(7200.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "h"));
}

void test_adaptive_days(void)
{
    char buf[32];
    fmt_time_adaptive(172800.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "d"));
}

void test_adaptive_years(void)
{
    char buf[32];
    fmt_time_adaptive(3.16e7, buf, sizeof(buf)); /* > 365.25 * 86400 */
    TEST_ASSERT_NOT_NULL(strstr(buf, "yr"));
}

void test_adaptive_gigayears(void)
{
    char buf[32];
    fmt_time_adaptive(4.35e17, buf, sizeof(buf)); /* ~13.8 Gyr */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gyr"));
}

/* Planetary hours: 10-14 */

void test_planetary_hour_noon_sunday(void)
{
    /* Sunday day ruler = Sun (3). At noon = ~hour 7 of day. */
    tf_planetary_hour_t ph = tf_planetary_hour(6, 0.5); /* 6=Sunday */
    TEST_ASSERT_EQUAL_INT(0, ph.is_night);
    TEST_ASSERT_TRUE(ph.hour_number >= 1 && ph.hour_number <= 12);
}

void test_planetary_hour_midnight(void)
{
    /* Midnight = night hour */
    tf_planetary_hour_t ph = tf_planetary_hour(0, 0.0); /* Monday midnight */
    TEST_ASSERT_EQUAL_INT(1, ph.is_night);
}

void test_planetary_hour_chaldean_order(void)
{
    /* Day rulers by weekday (Mon=0..Sun=6):
     * Mon=Moon, Tue=Mars, Wed=Mercury, Thu=Jupiter, Fri=Venus, Sat=Saturn, Sun=Sun
     * First day hour ruler = day ruler */
    tf_planetary_hour_t mon = tf_planetary_hour(0, 0.25); /* Monday 6am = first day hour */
    TEST_ASSERT_EQUAL_INT(6, mon.planet_ruler); /* Moon */

    tf_planetary_hour_t sat = tf_planetary_hour(5, 0.25);
    TEST_ASSERT_EQUAL_INT(0, sat.planet_ruler); /* Saturn */

    tf_planetary_hour_t sun = tf_planetary_hour(6, 0.25);
    TEST_ASSERT_EQUAL_INT(3, sun.planet_ruler); /* Sun */
}

void test_planetary_hour_range(void)
{
    for (int d = 0; d < 7; d++) {
        for (double f = 0.0; f < 1.0; f += 0.1) {
            tf_planetary_hour_t ph = tf_planetary_hour(d, f);
            TEST_ASSERT_TRUE(ph.hour_number >= 1 && ph.hour_number <= 24);
            TEST_ASSERT_TRUE(ph.planet_ruler >= 0 && ph.planet_ruler <= 6);
            TEST_ASSERT_TRUE(ph.is_night == 0 || ph.is_night == 1);
        }
    }
}

void test_planetary_hour_day_night_boundary(void)
{
    /* At 0.25 (6am) = day. At 0.75 (6pm) = night (equinox model). */
    tf_planetary_hour_t day = tf_planetary_hour(3, 0.3);
    TEST_ASSERT_EQUAL_INT(0, day.is_night);

    tf_planetary_hour_t night = tf_planetary_hour(3, 0.8);
    TEST_ASSERT_EQUAL_INT(1, night.is_night);
}

/* Ruler names and symbols: 15-16 */

void test_ruler_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Saturn", tf_planetary_hour_ruler_name(0));
    TEST_ASSERT_EQUAL_STRING("Jupiter", tf_planetary_hour_ruler_name(1));
    TEST_ASSERT_EQUAL_STRING("Mars", tf_planetary_hour_ruler_name(2));
    TEST_ASSERT_EQUAL_STRING("Sun", tf_planetary_hour_ruler_name(3));
    TEST_ASSERT_EQUAL_STRING("Venus", tf_planetary_hour_ruler_name(4));
    TEST_ASSERT_EQUAL_STRING("Mercury", tf_planetary_hour_ruler_name(5));
    TEST_ASSERT_EQUAL_STRING("Moon", tf_planetary_hour_ruler_name(6));
}

void test_ruler_symbols(void)
{
    for (int i = 0; i < 7; i++) {
        const char *sym = tf_planetary_hour_ruler_symbol(i);
        TEST_ASSERT_NOT_NULL(sym);
        TEST_ASSERT_TRUE(strlen(sym) > 0);
        TEST_ASSERT_TRUE(strcmp(sym, "?") != 0);
    }
    TEST_ASSERT_EQUAL_STRING("?", tf_planetary_hour_ruler_symbol(-1));
    TEST_ASSERT_EQUAL_STRING("?", tf_planetary_hour_ruler_symbol(7));
}

/* Aspect pair: 17-18 */

void test_aspect_pair(void)
{
    char buf[64];
    fmt_aspect_pair("Sun", "\xe2\x98\x8d", "Moon", buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon"));
}

void test_aspect_pair_truncation(void)
{
    char buf[8];
    fmt_aspect_pair("Sun", "-", "Moon", buf, sizeof(buf));
    /* Should not overflow */
    TEST_ASSERT_TRUE(strlen(buf) < 8);
}

/* Elapsed compact: 19-21 */

void test_elapsed_compact_seconds(void)
{
    char buf[32];
    fmt_elapsed_compact(45.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "45"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "s"));
}

void test_elapsed_compact_hours(void)
{
    char buf[32];
    fmt_elapsed_compact(3661.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1h"));
}

void test_elapsed_compact_days(void)
{
    char buf[32];
    fmt_elapsed_compact(90061.0, buf, sizeof(buf)); /* 1d 1h 1m 1s */
    TEST_ASSERT_NOT_NULL(strstr(buf, "1d"));
}

/* Purity: 22 */

void test_purity(void)
{
    char buf1[48], buf2[48];
    fmt_jd_grouped(2451545.0, buf1, sizeof(buf1));
    fmt_jd_grouped(2451545.0, buf2, sizeof(buf2));
    TEST_ASSERT_EQUAL_STRING(buf1, buf2);

    tf_planetary_hour_t a = tf_planetary_hour(3, 0.5);
    tf_planetary_hour_t b = tf_planetary_hour(3, 0.5);
    TEST_ASSERT_EQUAL_INT(a.planet_ruler, b.planet_ruler);
    TEST_ASSERT_EQUAL_INT(a.hour_number, b.hour_number);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_jd_grouped_j2000);
    RUN_TEST(test_jd_grouped_small);
    RUN_TEST(test_jd_grouped_large);
    RUN_TEST(test_adaptive_seconds);
    RUN_TEST(test_adaptive_minutes);
    RUN_TEST(test_adaptive_hours);
    RUN_TEST(test_adaptive_days);
    RUN_TEST(test_adaptive_years);
    RUN_TEST(test_adaptive_gigayears);
    RUN_TEST(test_planetary_hour_noon_sunday);
    RUN_TEST(test_planetary_hour_midnight);
    RUN_TEST(test_planetary_hour_chaldean_order);
    RUN_TEST(test_planetary_hour_range);
    RUN_TEST(test_planetary_hour_day_night_boundary);
    RUN_TEST(test_ruler_names);
    RUN_TEST(test_ruler_symbols);
    RUN_TEST(test_aspect_pair);
    RUN_TEST(test_aspect_pair_truncation);
    RUN_TEST(test_elapsed_compact_seconds);
    RUN_TEST(test_elapsed_compact_hours);
    RUN_TEST(test_elapsed_compact_days);
    RUN_TEST(test_purity);
    return UNITY_END();
}
