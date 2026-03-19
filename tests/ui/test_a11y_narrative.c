/* test_a11y_narrative.c — Tests for a11y_narrative (LANGUAGE L2.3)
 *
 * TDD: rich accessibility narrative for screen readers.
 * Upgrades ARIA labels to human-readable narrative descriptions. */

#include "../../tests/unity/unity.h"
#include "../../src/ui/a11y_narrative.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Enum counts
 * ================================================================ */

void test_sky_position_count(void)
{
    TEST_ASSERT_EQUAL_INT(SKY_POS_COUNT, a11y_sky_position_count());
}

void test_brightness_count(void)
{
    TEST_ASSERT_EQUAL_INT(SKY_BRIGHT_COUNT, a11y_brightness_count());
}

void test_transition_type_count(void)
{
    TEST_ASSERT_EQUAL_INT(A11Y_TRANS_COUNT, a11y_transition_type_count());
}

/* ================================================================
 * Sky position descriptions
 * ================================================================ */

void test_position_rising(void)
{
    const char *d = a11y_sky_position_desc(SKY_POS_RISING);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_NOT_NULL(strstr(d, "east"));
}

void test_position_overhead(void)
{
    const char *d = a11y_sky_position_desc(SKY_POS_OVERHEAD);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_NOT_NULL(strstr(d, "overhead"));
}

void test_position_setting(void)
{
    const char *d = a11y_sky_position_desc(SKY_POS_SETTING);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_NOT_NULL(strstr(d, "west"));
}

void test_position_below(void)
{
    const char *d = a11y_sky_position_desc(SKY_POS_BELOW);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_NOT_NULL(strstr(d, "below"));
}

void test_position_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", a11y_sky_position_desc(SKY_POS_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", a11y_sky_position_desc((sky_position_t)-1));
}

/* ================================================================
 * Brightness descriptions
 * ================================================================ */

void test_brightness_brilliant(void)
{
    const char *d = a11y_brightness_desc(SKY_BRIGHT_BRILLIANT);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_TRUE(strlen(d) > 5);
}

void test_brightness_invisible(void)
{
    const char *d = a11y_brightness_desc(SKY_BRIGHT_INVISIBLE);
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_NOT_NULL(strstr(d, "not visible"));
}

void test_brightness_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", a11y_brightness_desc(SKY_BRIGHT_COUNT));
}

/* ================================================================
 * Transition names
 * ================================================================ */

void test_transition_zoom_in(void)
{
    TEST_ASSERT_EQUAL_STRING("Zoom In", a11y_transition_name(A11Y_TRANS_ZOOM_IN));
}

void test_transition_view_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth View", a11y_transition_name(A11Y_TRANS_VIEW_EARTH));
}

void test_transition_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", a11y_transition_name(A11Y_TRANS_COUNT));
}

/* ================================================================
 * Single planet narrative
 * ================================================================ */

void test_planet_basic(void)
{
    a11y_planet_t p = {
        .name = "Venus",
        .sign = "Taurus",
        .degree = 15.3,
        .is_retrograde = false,
        .position = SKY_POS_HIGH,
        .brightness = SKY_BRIGHT_BRILLIANT,
        .near_object = NULL
    };
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_planet(&p, buf, A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Venus"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Taurus"));
}

void test_planet_with_retrograde(void)
{
    a11y_planet_t p = {
        .name = "Mars",
        .sign = "Scorpio",
        .degree = 8.7,
        .is_retrograde = true,
        .position = SKY_POS_LOW_EAST,
        .brightness = SKY_BRIGHT_BRIGHT,
        .near_object = NULL
    };
    char buf[A11Y_NARR_MAX];
    a11y_narr_planet(&p, buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "retrograde"));
}

void test_planet_with_near_object(void)
{
    a11y_planet_t p = {
        .name = "Venus",
        .sign = "Taurus",
        .degree = 22.0,
        .is_retrograde = false,
        .position = SKY_POS_LOW_WEST,
        .brightness = SKY_BRIGHT_BRILLIANT,
        .near_object = "the Pleiades"
    };
    char buf[A11Y_NARR_MAX];
    a11y_narr_planet(&p, buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pleiades"));
}

void test_planet_below_horizon_says_not_visible(void)
{
    a11y_planet_t p = {
        .name = "Saturn",
        .sign = "Pisces",
        .degree = 5.0,
        .is_retrograde = false,
        .position = SKY_POS_BELOW,
        .brightness = SKY_BRIGHT_INVISIBLE,
        .near_object = NULL
    };
    char buf[A11Y_NARR_MAX];
    a11y_narr_planet(&p, buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "below"));
}

void test_planet_null_returns_zero(void)
{
    char buf[64];
    TEST_ASSERT_EQUAL_INT(0, a11y_narr_planet(NULL, buf, 64));
}

void test_planet_null_buffer_returns_zero(void)
{
    a11y_planet_t p = {
        .name = "Sun", .sign = "Aries", .degree = 0,
        .is_retrograde = false, .position = SKY_POS_HIGH,
        .brightness = SKY_BRIGHT_BRILLIANT, .near_object = NULL
    };
    TEST_ASSERT_EQUAL_INT(0, a11y_narr_planet(&p, NULL, 64));
}

/* ================================================================
 * Sky narrative (multiple planets)
 * ================================================================ */

void test_sky_two_planets(void)
{
    a11y_planet_t planets[2] = {
        {
            .name = "Sun", .sign = "Aries", .degree = 10.0,
            .is_retrograde = false, .position = SKY_POS_HIGH,
            .brightness = SKY_BRIGHT_BRILLIANT, .near_object = NULL
        },
        {
            .name = "Moon", .sign = "Cancer", .degree = 22.0,
            .is_retrograde = false, .position = SKY_POS_LOW_EAST,
            .brightness = SKY_BRIGHT_BRIGHT, .near_object = NULL
        }
    };
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_sky(planets, 2, buf, A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon"));
}

void test_sky_skips_below_horizon(void)
{
    a11y_planet_t planets[2] = {
        {
            .name = "Venus", .sign = "Taurus", .degree = 15.0,
            .is_retrograde = false, .position = SKY_POS_HIGH,
            .brightness = SKY_BRIGHT_BRILLIANT, .near_object = NULL
        },
        {
            .name = "Saturn", .sign = "Pisces", .degree = 5.0,
            .is_retrograde = false, .position = SKY_POS_BELOW,
            .brightness = SKY_BRIGHT_INVISIBLE, .near_object = NULL
        }
    };
    char buf[A11Y_NARR_MAX];
    a11y_narr_sky(planets, 2, buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Venus"));
    /* Saturn below horizon should be mentioned but differently */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Saturn"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "below"));
}

void test_sky_null_returns_zero(void)
{
    char buf[64];
    TEST_ASSERT_EQUAL_INT(0, a11y_narr_sky(NULL, 2, buf, 64));
}

void test_sky_zero_count_returns_zero(void)
{
    a11y_planet_t p = {
        .name = "Sun", .sign = "Aries", .degree = 0,
        .is_retrograde = false, .position = SKY_POS_HIGH,
        .brightness = SKY_BRIGHT_BRILLIANT, .near_object = NULL
    };
    char buf[64];
    TEST_ASSERT_EQUAL_INT(0, a11y_narr_sky(&p, 0, buf, 64));
}

/* ================================================================
 * Calendar narrative
 * ================================================================ */

void test_calendar_basic(void)
{
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_calendar("Hebrew", "5 Shevat 5784", NULL,
                               buf, A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hebrew"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "5 Shevat 5784"));
}

void test_calendar_with_meaning(void)
{
    char buf[A11Y_NARR_MAX];
    a11y_narr_calendar("Hebrew", "15 Shevat 5784",
                       "Tu BiShvat, the new year for trees",
                       buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Tu BiShvat"));
}

void test_calendar_null_system(void)
{
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_calendar(NULL, "day 1", NULL, buf, A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
}

void test_calendar_null_buffer_returns_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0,
        a11y_narr_calendar("Hebrew", "1 Tishrei", NULL, NULL, 64));
}

/* ================================================================
 * Transition narrative
 * ================================================================ */

void test_transition_zoom_in_narrative(void)
{
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_transition(A11Y_TRANS_ZOOM_IN, "Mars", buf,
                                 A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_TRUE(strstr(buf, "closer") != NULL ||
                      strstr(buf, "Zoom") != NULL);
}

void test_transition_view_space(void)
{
    char buf[A11Y_NARR_MAX];
    a11y_narr_transition(A11Y_TRANS_VIEW_SPACE, NULL, buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "space"));
}

void test_transition_system_enter(void)
{
    char buf[A11Y_NARR_MAX];
    a11y_narr_transition(A11Y_TRANS_SYSTEM_ENTER, "Astrology", buf,
                         A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Astrology"));
}

void test_transition_time_forward(void)
{
    char buf[A11Y_NARR_MAX];
    a11y_narr_transition(A11Y_TRANS_TIME_FORWARD, "1 year", buf,
                         A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "forward"));
}

void test_transition_invalid_returns_zero(void)
{
    char buf[64];
    TEST_ASSERT_EQUAL_INT(0,
        a11y_narr_transition(A11Y_TRANS_COUNT, NULL, buf, 64));
}

void test_transition_null_buffer_returns_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0,
        a11y_narr_transition(A11Y_TRANS_ZOOM_IN, NULL, NULL, 64));
}

/* ================================================================
 * Convergence narrative
 * ================================================================ */

void test_convergence_two_systems(void)
{
    const char *systems[] = {"Tzolkin", "I Ching"};
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_convergence(systems, 2, "notable", buf,
                                  A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Tzolkin"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "I Ching"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "notable"));
}

void test_convergence_three_systems(void)
{
    const char *systems[] = {"Tzolkin", "I Ching", "Hebrew"};
    char buf[A11Y_NARR_MAX];
    a11y_narr_convergence(systems, 3, "significant", buf, A11Y_NARR_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "3"));
}

void test_convergence_single_system(void)
{
    const char *systems[] = {"Astrology"};
    char buf[A11Y_NARR_MAX];
    int n = a11y_narr_convergence(systems, 1, "ordinary", buf,
                                  A11Y_NARR_MAX);
    TEST_ASSERT_GREATER_THAN(0, n);
}

void test_convergence_null_returns_zero(void)
{
    char buf[64];
    TEST_ASSERT_EQUAL_INT(0,
        a11y_narr_convergence(NULL, 2, "rare", buf, 64));
}

void test_convergence_zero_count_returns_zero(void)
{
    const char *systems[] = {"A"};
    char buf[64];
    TEST_ASSERT_EQUAL_INT(0,
        a11y_narr_convergence(systems, 0, "rare", buf, 64));
}

void test_convergence_null_buffer_returns_zero(void)
{
    const char *systems[] = {"A"};
    TEST_ASSERT_EQUAL_INT(0,
        a11y_narr_convergence(systems, 1, "rare", NULL, 64));
}

/* ================================================================
 * Small buffer handling (truncation, not crash)
 * ================================================================ */

void test_planet_small_buffer(void)
{
    a11y_planet_t p = {
        .name = "Venus", .sign = "Taurus", .degree = 15.0,
        .is_retrograde = false, .position = SKY_POS_HIGH,
        .brightness = SKY_BRIGHT_BRILLIANT, .near_object = NULL
    };
    char buf[16];
    int n = a11y_narr_planet(&p, buf, 16);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_TRUE(n < 16);
}

void test_transition_small_buffer(void)
{
    char buf[16];
    int n = a11y_narr_transition(A11Y_TRANS_ZOOM_IN, "Mars", buf, 16);
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_TRUE(n < 16);
}

/* ================================================================
 * Runner
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Enum counts */
    RUN_TEST(test_sky_position_count);
    RUN_TEST(test_brightness_count);
    RUN_TEST(test_transition_type_count);

    /* Sky positions */
    RUN_TEST(test_position_rising);
    RUN_TEST(test_position_overhead);
    RUN_TEST(test_position_setting);
    RUN_TEST(test_position_below);
    RUN_TEST(test_position_invalid);

    /* Brightness */
    RUN_TEST(test_brightness_brilliant);
    RUN_TEST(test_brightness_invisible);
    RUN_TEST(test_brightness_invalid);

    /* Transition names */
    RUN_TEST(test_transition_zoom_in);
    RUN_TEST(test_transition_view_earth);
    RUN_TEST(test_transition_invalid);

    /* Planet narrative */
    RUN_TEST(test_planet_basic);
    RUN_TEST(test_planet_with_retrograde);
    RUN_TEST(test_planet_with_near_object);
    RUN_TEST(test_planet_below_horizon_says_not_visible);
    RUN_TEST(test_planet_null_returns_zero);
    RUN_TEST(test_planet_null_buffer_returns_zero);

    /* Sky narrative */
    RUN_TEST(test_sky_two_planets);
    RUN_TEST(test_sky_skips_below_horizon);
    RUN_TEST(test_sky_null_returns_zero);
    RUN_TEST(test_sky_zero_count_returns_zero);

    /* Calendar narrative */
    RUN_TEST(test_calendar_basic);
    RUN_TEST(test_calendar_with_meaning);
    RUN_TEST(test_calendar_null_system);
    RUN_TEST(test_calendar_null_buffer_returns_zero);

    /* Transition narrative */
    RUN_TEST(test_transition_zoom_in_narrative);
    RUN_TEST(test_transition_view_space);
    RUN_TEST(test_transition_system_enter);
    RUN_TEST(test_transition_time_forward);
    RUN_TEST(test_transition_invalid_returns_zero);
    RUN_TEST(test_transition_null_buffer_returns_zero);

    /* Convergence narrative */
    RUN_TEST(test_convergence_two_systems);
    RUN_TEST(test_convergence_three_systems);
    RUN_TEST(test_convergence_single_system);
    RUN_TEST(test_convergence_null_returns_zero);
    RUN_TEST(test_convergence_zero_count_returns_zero);
    RUN_TEST(test_convergence_null_buffer_returns_zero);

    /* Buffer safety */
    RUN_TEST(test_planet_small_buffer);
    RUN_TEST(test_transition_small_buffer);

    return UNITY_END();
}
