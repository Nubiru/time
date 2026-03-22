#include "../unity/unity.h"
#include "../../src/ui/daily_greeting.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ==== dg_period: boundary tests ==== */

void test_period_midnight(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, dg_period(0));
}

void test_period_predawn(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, dg_period(4));
}

void test_period_dawn_start(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_DAWN, dg_period(5));
}

void test_period_dawn_end(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_DAWN, dg_period(7));
}

void test_period_morning_start(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_MORNING, dg_period(8));
}

void test_period_morning_end(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_MORNING, dg_period(11));
}

void test_period_midday_start(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_MIDDAY, dg_period(12));
}

void test_period_midday_end(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_MIDDAY, dg_period(13));
}

void test_period_afternoon_start(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_AFTERNOON, dg_period(14));
}

void test_period_afternoon_end(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_AFTERNOON, dg_period(17));
}

void test_period_evening_start(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_EVENING, dg_period(18));
}

void test_period_evening_end(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_EVENING, dg_period(20));
}

void test_period_night_start(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, dg_period(21));
}

void test_period_late_night(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, dg_period(23));
}

void test_period_negative_hour(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, dg_period(-1));
}

void test_period_over_23(void)
{
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, dg_period(25));
}

/* ==== dg_compose: greeting text ==== */

void test_compose_dawn_greeting(void)
{
    dg_input_t in = { .hour = 6, .locale = I18N_LOCALE_EN };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("The light returns", out.greeting);
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_DAWN, out.period);
}

void test_compose_morning_greeting(void)
{
    dg_input_t in = { .hour = 9, .locale = I18N_LOCALE_EN };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Good morning", out.greeting);
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_MORNING, out.period);
}

void test_compose_evening_greeting(void)
{
    dg_input_t in = { .hour = 19, .locale = I18N_LOCALE_EN };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Good evening", out.greeting);
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_EVENING, out.period);
}

void test_compose_night_greeting(void)
{
    dg_input_t in = { .hour = 23, .locale = I18N_LOCALE_EN };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Good night", out.greeting);
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_NIGHT, out.period);
}

void test_compose_midday_greeting(void)
{
    dg_input_t in = { .hour = 12, .locale = I18N_LOCALE_EN };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Good afternoon", out.greeting);
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_MIDDAY, out.period);
}

/* ==== dg_compose: summary construction ==== */

void test_compose_summary_weekday_and_date(void)
{
    dg_input_t in = {
        .hour = 10,
        .weekday = "Monday",
        .date_str = "March 22, 2026",
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_TRUE(strstr(out.summary, "Monday") != NULL);
    TEST_ASSERT_TRUE(strstr(out.summary, "March 22, 2026") != NULL);
}

void test_compose_summary_with_kin(void)
{
    dg_input_t in = {
        .hour = 10,
        .weekday = "Tuesday",
        .date_str = "March 22, 2026",
        .kin_str = "Kin 187 Blue Cosmic Hand",
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_TRUE(strstr(out.summary, "Kin 187 Blue Cosmic Hand") != NULL);
    TEST_ASSERT_TRUE(strstr(out.summary, "Tuesday") != NULL);
}

void test_compose_summary_only_kin(void)
{
    dg_input_t in = {
        .hour = 10,
        .kin_str = "Kin 1 Red Magnetic Dragon",
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Kin 1 Red Magnetic Dragon.", out.summary);
}

void test_compose_summary_empty_when_all_null(void)
{
    dg_input_t in = { .hour = 10, .locale = I18N_LOCALE_EN };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("", out.summary);
}

void test_compose_summary_weekday_only(void)
{
    dg_input_t in = {
        .hour = 14,
        .weekday = "Friday",
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Friday.", out.summary);
}

void test_compose_summary_date_only(void)
{
    dg_input_t in = {
        .hour = 14,
        .date_str = "March 22, 2026",
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("March 22, 2026.", out.summary);
}

/* ==== dg_compose: teaser ==== */

void test_compose_teaser_with_convergence(void)
{
    dg_input_t in = {
        .hour = 10,
        .convergence = "3 systems converge today",
        .convergence_count = 3,
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("3 systems converge today.", out.teaser);
}

void test_compose_teaser_count_only(void)
{
    dg_input_t in = {
        .hour = 10,
        .convergence_count = 5,
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("5 systems converge today.", out.teaser);
}

void test_compose_teaser_empty_when_no_convergence(void)
{
    dg_input_t in = {
        .hour = 10,
        .convergence_count = 0,
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("", out.teaser);
}

/* ==== dg_compose: error handling ==== */

void test_compose_null_input(void)
{
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(0, dg_compose(NULL, &out));
}

void test_compose_null_output(void)
{
    dg_input_t in = { .hour = 10, .locale = I18N_LOCALE_EN };
    TEST_ASSERT_EQUAL_INT(0, dg_compose(&in, NULL));
}

/* ==== dg_compose: default locale ==== */

void test_compose_default_locale_produces_english(void)
{
    dg_input_t in = { .hour = 9, .locale = 0 }; /* 0 = EN */
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_STRING("Good morning", out.greeting);
}

/* ==== dg_compose: full scenario ==== */

void test_compose_full_scenario(void)
{
    dg_input_t in = {
        .hour = 7,
        .minute = 30,
        .weekday = "Saturday",
        .date_str = "March 22, 2026",
        .kin_str = "Kin 187 Blue Cosmic Hand",
        .convergence = "3 systems converge today",
        .convergence_count = 3,
        .locale = I18N_LOCALE_EN,
    };
    dg_output_t out;
    TEST_ASSERT_EQUAL_INT(1, dg_compose(&in, &out));
    TEST_ASSERT_EQUAL_INT(DG_PERIOD_DAWN, out.period);
    TEST_ASSERT_EQUAL_STRING("The light returns", out.greeting);
    TEST_ASSERT_TRUE(strstr(out.summary, "Saturday") != NULL);
    TEST_ASSERT_TRUE(strstr(out.summary, "Kin 187") != NULL);
    TEST_ASSERT_TRUE(strlen(out.teaser) > 0);
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* dg_period: boundaries */
    RUN_TEST(test_period_midnight);
    RUN_TEST(test_period_predawn);
    RUN_TEST(test_period_dawn_start);
    RUN_TEST(test_period_dawn_end);
    RUN_TEST(test_period_morning_start);
    RUN_TEST(test_period_morning_end);
    RUN_TEST(test_period_midday_start);
    RUN_TEST(test_period_midday_end);
    RUN_TEST(test_period_afternoon_start);
    RUN_TEST(test_period_afternoon_end);
    RUN_TEST(test_period_evening_start);
    RUN_TEST(test_period_evening_end);
    RUN_TEST(test_period_night_start);
    RUN_TEST(test_period_late_night);
    RUN_TEST(test_period_negative_hour);
    RUN_TEST(test_period_over_23);

    /* dg_compose: greeting text */
    RUN_TEST(test_compose_dawn_greeting);
    RUN_TEST(test_compose_morning_greeting);
    RUN_TEST(test_compose_evening_greeting);
    RUN_TEST(test_compose_night_greeting);
    RUN_TEST(test_compose_midday_greeting);

    /* dg_compose: summary */
    RUN_TEST(test_compose_summary_weekday_and_date);
    RUN_TEST(test_compose_summary_with_kin);
    RUN_TEST(test_compose_summary_only_kin);
    RUN_TEST(test_compose_summary_empty_when_all_null);
    RUN_TEST(test_compose_summary_weekday_only);
    RUN_TEST(test_compose_summary_date_only);

    /* dg_compose: teaser */
    RUN_TEST(test_compose_teaser_with_convergence);
    RUN_TEST(test_compose_teaser_count_only);
    RUN_TEST(test_compose_teaser_empty_when_no_convergence);

    /* dg_compose: error handling */
    RUN_TEST(test_compose_null_input);
    RUN_TEST(test_compose_null_output);

    /* dg_compose: locale */
    RUN_TEST(test_compose_default_locale_produces_english);

    /* dg_compose: full scenario */
    RUN_TEST(test_compose_full_scenario);

    return UNITY_END();
}
