#include "../unity/unity.h"
#include "../../src/ui/greeting.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ==== Time of day greetings ==== */

void test_time_morning_early(void)
{
    TEST_ASSERT_EQUAL_STRING("Good morning", greeting_time_text(6));
}

void test_time_morning_late(void)
{
    TEST_ASSERT_EQUAL_STRING("Good morning", greeting_time_text(11));
}

void test_time_afternoon(void)
{
    TEST_ASSERT_EQUAL_STRING("Good afternoon", greeting_time_text(12));
}

void test_time_afternoon_late(void)
{
    TEST_ASSERT_EQUAL_STRING("Good afternoon", greeting_time_text(16));
}

void test_time_evening(void)
{
    TEST_ASSERT_EQUAL_STRING("Good evening", greeting_time_text(17));
}

void test_time_evening_late(void)
{
    TEST_ASSERT_EQUAL_STRING("Good evening", greeting_time_text(20));
}

void test_time_night(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", greeting_time_text(21));
}

void test_time_midnight(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", greeting_time_text(0));
}

void test_time_predawn(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", greeting_time_text(5));
}

void test_time_negative_returns_night(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", greeting_time_text(-1));
}

void test_time_over_23_returns_night(void)
{
    TEST_ASSERT_EQUAL_STRING("Good night", greeting_time_text(25));
}

/* ==== Cardinal date greetings (solstice/equinox) ==== */

void test_cardinal_vernal_equinox(void)
{
    const char *g = greeting_cardinal_text(80, 45.0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "equinox") != NULL || strstr(g, "Equinox") != NULL);
}

void test_cardinal_summer_solstice(void)
{
    const char *g = greeting_cardinal_text(172, 45.0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "solstice") != NULL || strstr(g, "Solstice") != NULL);
}

void test_cardinal_autumnal_equinox(void)
{
    const char *g = greeting_cardinal_text(265, 45.0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "equinox") != NULL || strstr(g, "Equinox") != NULL);
}

void test_cardinal_winter_solstice(void)
{
    const char *g = greeting_cardinal_text(355, 45.0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "solstice") != NULL || strstr(g, "Solstice") != NULL);
}

void test_cardinal_near_equinox(void)
{
    /* Within 1 day should still trigger */
    const char *g = greeting_cardinal_text(81, 45.0);
    TEST_ASSERT_NOT_NULL(g);
}

void test_cardinal_not_near(void)
{
    /* Day 100 is not near any cardinal date */
    const char *g = greeting_cardinal_text(100, 45.0);
    TEST_ASSERT_NULL(g);
}

void test_cardinal_southern_summer_solstice(void)
{
    /* Dec solstice in southern hemisphere is summer */
    const char *g = greeting_cardinal_text(355, -35.0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Summer") != NULL || strstr(g, "summer") != NULL);
}

void test_cardinal_southern_winter_solstice(void)
{
    /* Jun solstice in southern hemisphere is winter */
    const char *g = greeting_cardinal_text(172, -35.0);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Winter") != NULL || strstr(g, "winter") != NULL);
}

/* ==== Sabbath greetings ==== */

void test_sabbath_friday_evening(void)
{
    /* weekday=5 (Friday), hour=18 */
    const char *g = greeting_sabbath_text(5, 18);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Shabbat") != NULL);
}

void test_sabbath_friday_before_evening(void)
{
    /* Friday noon — not yet Shabbat */
    const char *g = greeting_sabbath_text(5, 12);
    TEST_ASSERT_NULL(g);
}

void test_sabbath_saturday(void)
{
    /* Saturday morning */
    const char *g = greeting_sabbath_text(6, 10);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_TRUE(strstr(g, "Shabbat") != NULL);
}

void test_sabbath_saturday_evening(void)
{
    /* Saturday evening — Shabbat ending */
    const char *g = greeting_sabbath_text(6, 20);
    TEST_ASSERT_NULL(g);
}

void test_sabbath_weekday(void)
{
    /* Wednesday — no Shabbat */
    const char *g = greeting_sabbath_text(3, 18);
    TEST_ASSERT_NULL(g);
}

/* ==== Generate aggregate ==== */

void test_generate_has_time_greeting(void)
{
    greeting_ctx_t ctx = { 10, 100, 3, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    TEST_ASSERT_TRUE(r.count >= 1);
    TEST_ASSERT_EQUAL_INT(GREET_TIME_OF_DAY, r.items[0].type);
}

void test_generate_morning_text(void)
{
    greeting_ctx_t ctx = { 8, 100, 1, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    TEST_ASSERT_EQUAL_STRING("Good morning", r.items[0].text);
}

void test_generate_solstice_adds_cardinal(void)
{
    /* Summer solstice */
    greeting_ctx_t ctx = { 10, 172, 3, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    TEST_ASSERT_TRUE(r.count >= 2);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.items[i].type == GREET_SOLSTICE) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_generate_equinox_adds_cardinal(void)
{
    greeting_ctx_t ctx = { 14, 80, 2, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.items[i].type == GREET_EQUINOX) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_generate_sabbath_friday_evening(void)
{
    /* Friday 19:00 */
    greeting_ctx_t ctx = { 19, 100, 5, 32.0 };
    greeting_result_t r = greeting_generate(ctx);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.items[i].type == GREET_SABBATH) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_generate_no_sabbath_weekday(void)
{
    /* Tuesday 19:00 */
    greeting_ctx_t ctx = { 19, 100, 2, 32.0 };
    greeting_result_t r = greeting_generate(ctx);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.items[i].type == GREET_SABBATH) found = 1;
    }
    TEST_ASSERT_FALSE(found);
}

void test_generate_count_max(void)
{
    greeting_ctx_t ctx = { 10, 100, 1, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    TEST_ASSERT_TRUE(r.count <= GREETING_MAX);
    TEST_ASSERT_TRUE(r.count >= 1);
}

void test_greeting_count(void)
{
    greeting_ctx_t ctx = { 10, 100, 1, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    TEST_ASSERT_EQUAL_INT(r.count, greeting_count(&r));
}

void test_greeting_count_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, greeting_count(NULL));
}

void test_generate_all_have_text(void)
{
    greeting_ctx_t ctx = { 19, 172, 5, 45.0 };
    greeting_result_t r = greeting_generate(ctx);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_NULL(r.items[i].text);
        TEST_ASSERT_TRUE(strlen(r.items[i].text) > 0);
    }
}

void test_generate_all_have_source(void)
{
    greeting_ctx_t ctx = { 19, 80, 5, -35.0 };
    greeting_result_t r = greeting_generate(ctx);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_NULL(r.items[i].source);
    }
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* Time of day */
    RUN_TEST(test_time_morning_early);
    RUN_TEST(test_time_morning_late);
    RUN_TEST(test_time_afternoon);
    RUN_TEST(test_time_afternoon_late);
    RUN_TEST(test_time_evening);
    RUN_TEST(test_time_evening_late);
    RUN_TEST(test_time_night);
    RUN_TEST(test_time_midnight);
    RUN_TEST(test_time_predawn);
    RUN_TEST(test_time_negative_returns_night);
    RUN_TEST(test_time_over_23_returns_night);

    /* Cardinal dates */
    RUN_TEST(test_cardinal_vernal_equinox);
    RUN_TEST(test_cardinal_summer_solstice);
    RUN_TEST(test_cardinal_autumnal_equinox);
    RUN_TEST(test_cardinal_winter_solstice);
    RUN_TEST(test_cardinal_near_equinox);
    RUN_TEST(test_cardinal_not_near);
    RUN_TEST(test_cardinal_southern_summer_solstice);
    RUN_TEST(test_cardinal_southern_winter_solstice);

    /* Sabbath */
    RUN_TEST(test_sabbath_friday_evening);
    RUN_TEST(test_sabbath_friday_before_evening);
    RUN_TEST(test_sabbath_saturday);
    RUN_TEST(test_sabbath_saturday_evening);
    RUN_TEST(test_sabbath_weekday);

    /* Generate aggregate */
    RUN_TEST(test_generate_has_time_greeting);
    RUN_TEST(test_generate_morning_text);
    RUN_TEST(test_generate_solstice_adds_cardinal);
    RUN_TEST(test_generate_equinox_adds_cardinal);
    RUN_TEST(test_generate_sabbath_friday_evening);
    RUN_TEST(test_generate_no_sabbath_weekday);
    RUN_TEST(test_generate_count_max);
    RUN_TEST(test_greeting_count);
    RUN_TEST(test_greeting_count_null);
    RUN_TEST(test_generate_all_have_text);
    RUN_TEST(test_generate_all_have_source);

    return UNITY_END();
}
