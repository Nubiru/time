#include "unity.h"
#include "ui/seasonal_greeting.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Time-of-day greetings ---- */

void test_morning_early(void)
{
    TEST_ASSERT_EQUAL_STRING("Good morning", sg_time_greeting(6));
}

void test_morning_late(void)
{
    TEST_ASSERT_EQUAL_STRING("Good morning", sg_time_greeting(11));
}

void test_afternoon(void)
{
    TEST_ASSERT_EQUAL_STRING("Good afternoon", sg_time_greeting(12));
}

void test_afternoon_late(void)
{
    TEST_ASSERT_EQUAL_STRING("Good afternoon", sg_time_greeting(16));
}

void test_evening(void)
{
    TEST_ASSERT_EQUAL_STRING("Good evening", sg_time_greeting(17));
}

void test_evening_late(void)
{
    TEST_ASSERT_EQUAL_STRING("Good evening", sg_time_greeting(20));
}

void test_night(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", sg_time_greeting(21));
}

void test_night_midnight(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", sg_time_greeting(0));
}

void test_night_predawn(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", sg_time_greeting(5));
}

void test_negative_hour_clamps(void)
{
    const char *g = sg_time_greeting(-1);
    TEST_ASSERT_NOT_NULL(g);
}

void test_over_23_clamps(void)
{
    const char *g = sg_time_greeting(25);
    TEST_ASSERT_NOT_NULL(g);
}

/* ---- Hebrew Sabbath ---- */

void test_hebrew_friday_evening(void)
{
    /* Friday=5, after sunset (18+) */
    TEST_ASSERT_EQUAL_STRING("Shabbat Shalom", sg_hebrew_greeting(5, 18));
}

void test_hebrew_saturday_morning(void)
{
    TEST_ASSERT_EQUAL_STRING("Shabbat Shalom", sg_hebrew_greeting(6, 10));
}

void test_hebrew_saturday_evening(void)
{
    /* Saturday after sunset — Shabbat ends */
    TEST_ASSERT_NULL(sg_hebrew_greeting(6, 19));
}

void test_hebrew_weekday(void)
{
    TEST_ASSERT_NULL(sg_hebrew_greeting(2, 10));
}

/* ---- Islamic Ramadan ---- */

void test_islamic_ramadan_active(void)
{
    TEST_ASSERT_EQUAL_STRING("Ramadan Kareem", sg_islamic_greeting(1));
}

void test_islamic_no_ramadan(void)
{
    TEST_ASSERT_NULL(sg_islamic_greeting(0));
}

/* ---- Astronomical events ---- */

void test_spring_equinox_north(void)
{
    /* ~March 20 = day 79 */
    const char *g = sg_astro_greeting(80, 0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Equinox") != NULL);
}

void test_summer_solstice_north(void)
{
    /* ~June 21 = day 172 */
    const char *g = sg_astro_greeting(172, 0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Solstice") != NULL);
}

void test_autumn_equinox_north(void)
{
    /* ~Sep 22 = day 265 */
    const char *g = sg_astro_greeting(266, 0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Equinox") != NULL);
}

void test_winter_solstice_north(void)
{
    /* ~Dec 21 = day 355 */
    const char *g = sg_astro_greeting(355, 0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Solstice") != NULL);
}

void test_no_astro_event(void)
{
    TEST_ASSERT_NULL(sg_astro_greeting(100, 0));
}

void test_southern_hemisphere_reverses(void)
{
    /* Spring equinox in north = Autumn equinox in south */
    const char *g = sg_astro_greeting(80, 1);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Autumn") != NULL || strstr(g, "Equinox") != NULL);
}

/* ---- Chinese New Year ---- */

void test_chinese_new_year_active(void)
{
    TEST_ASSERT_EQUAL_STRING("Gong Xi Fa Cai", sg_chinese_greeting(1));
}

void test_chinese_not_new_year(void)
{
    TEST_ASSERT_NULL(sg_chinese_greeting(0));
}

/* ---- Buddhist Uposatha ---- */

void test_buddhist_uposatha_active(void)
{
    const char *g = sg_buddhist_greeting(1);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strlen(g) > 0);
}

void test_buddhist_not_uposatha(void)
{
    TEST_ASSERT_NULL(sg_buddhist_greeting(0));
}

/* ---- Generic new year ---- */

void test_new_year_greeting(void)
{
    const char *g = sg_new_year_greeting();
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strlen(g) > 0);
}

/* ---- Compose ---- */

void test_compose_time_only(void)
{
    char buf[128];
    const char *result = sg_compose(buf, 128, 9, 2, 100, 0, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Good morning", result);
}

void test_compose_with_sabbath(void)
{
    char buf[128];
    const char *result = sg_compose(buf, 128, 19, 5, 100, 0, 1, 0, 0, 0);
    TEST_ASSERT_TRUE(strstr(result, "Good evening") != NULL);
    TEST_ASSERT_TRUE(strstr(result, "Shabbat Shalom") != NULL);
}

void test_compose_with_solstice(void)
{
    char buf[128];
    const char *result = sg_compose(buf, 128, 10, 3, 172, 0, 0, 0, 0, 0);
    TEST_ASSERT_TRUE(strstr(result, "Good morning") != NULL);
    TEST_ASSERT_TRUE(strstr(result, "Solstice") != NULL);
}

void test_compose_null_buf(void)
{
    const char *result = sg_compose(NULL, 0, 9, 2, 100, 0, 0, 0, 0, 0);
    TEST_ASSERT_NULL(result);
}

void test_compose_small_buf(void)
{
    char buf[8];
    const char *result = sg_compose(buf, 8, 9, 2, 100, 0, 0, 0, 0, 0);
    TEST_ASSERT_NOT_NULL(result);
    /* Should not overflow */
    TEST_ASSERT_TRUE(strlen(result) < 8);
}

int main(void)
{
    UNITY_BEGIN();

    /* Time-of-day */
    RUN_TEST(test_morning_early);
    RUN_TEST(test_morning_late);
    RUN_TEST(test_afternoon);
    RUN_TEST(test_afternoon_late);
    RUN_TEST(test_evening);
    RUN_TEST(test_evening_late);
    RUN_TEST(test_night);
    RUN_TEST(test_night_midnight);
    RUN_TEST(test_night_predawn);
    RUN_TEST(test_negative_hour_clamps);
    RUN_TEST(test_over_23_clamps);

    /* Hebrew Sabbath */
    RUN_TEST(test_hebrew_friday_evening);
    RUN_TEST(test_hebrew_saturday_morning);
    RUN_TEST(test_hebrew_saturday_evening);
    RUN_TEST(test_hebrew_weekday);

    /* Islamic Ramadan */
    RUN_TEST(test_islamic_ramadan_active);
    RUN_TEST(test_islamic_no_ramadan);

    /* Astronomical events */
    RUN_TEST(test_spring_equinox_north);
    RUN_TEST(test_summer_solstice_north);
    RUN_TEST(test_autumn_equinox_north);
    RUN_TEST(test_winter_solstice_north);
    RUN_TEST(test_no_astro_event);
    RUN_TEST(test_southern_hemisphere_reverses);

    /* Chinese New Year */
    RUN_TEST(test_chinese_new_year_active);
    RUN_TEST(test_chinese_not_new_year);

    /* Buddhist Uposatha */
    RUN_TEST(test_buddhist_uposatha_active);
    RUN_TEST(test_buddhist_not_uposatha);

    /* Generic new year */
    RUN_TEST(test_new_year_greeting);

    /* Compose */
    RUN_TEST(test_compose_time_only);
    RUN_TEST(test_compose_with_sabbath);
    RUN_TEST(test_compose_with_solstice);
    RUN_TEST(test_compose_null_buf);
    RUN_TEST(test_compose_small_buf);

    return UNITY_END();
}
