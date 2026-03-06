#include "../unity/unity.h"
#include "../../src/systems/astrology/planetary_hours.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Known JDs:
 * J2000 = JD 2451545.0 = Saturday 2000-01-01 12:00 UT
 * Sunday 6AM = JD 2451545.75
 * Monday 6AM = JD 2451546.75
 * 2024-01-01 noon = JD 2460311.0 (Monday)
 */
#define J2000      2451545.0
#define SUN_6AM    2451545.75
#define MON_6AM    2451546.75
#define JD_2024    2460311.0

/* 1. Sunday ruler = Sun (3) */
void test_sunday_ruler(void)
{
    TEST_ASSERT_EQUAL_INT(3, planetary_day_ruler(0));
}

/* 2. Monday ruler = Moon (6) */
void test_monday_ruler(void)
{
    TEST_ASSERT_EQUAL_INT(6, planetary_day_ruler(1));
}

/* 3. Saturday ruler = Saturn (0) */
void test_saturday_ruler(void)
{
    TEST_ASSERT_EQUAL_INT(0, planetary_day_ruler(6));
}

/* 4. All 7 day rulers are distinct */
void test_all_rulers_distinct(void)
{
    int seen[7] = {0};
    for (int i = 0; i < 7; i++) {
        int r = planetary_day_ruler(i);
        TEST_ASSERT_TRUE(r >= 0 && r <= 6);
        seen[r] = 1;
    }
    for (int i = 0; i < 7; i++)
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
}

/* 5. J2000 = Saturday, day_ruler = Saturn (0) */
void test_j2000_saturday(void)
{
    planetary_hour_t h = planetary_hour(J2000);
    TEST_ASSERT_EQUAL_INT(6, h.day_of_week);
    TEST_ASSERT_EQUAL_INT(0, h.day_ruler);
}

/* 6. 2024-01-01 = Monday, day_ruler = Moon (6) */
void test_2024_monday(void)
{
    planetary_hour_t h = planetary_hour(JD_2024);
    TEST_ASSERT_EQUAL_INT(1, h.day_of_week);
    TEST_ASSERT_EQUAL_INT(6, h.day_ruler);
}

/* 7. Noon (JD .0) = hour_index 6 (6 hours after 6AM sunrise) */
void test_noon_hour_index(void)
{
    planetary_hour_t h = planetary_hour(J2000);
    TEST_ASSERT_EQUAL_INT(6, h.hour_index);
}

/* 8. Sunday: hour 0=Sun(3), hour 1=Venus(4), hour 2=Mercury(5) */
void test_sunday_sequence(void)
{
    planetary_hour_t h0 = planetary_hour(SUN_6AM + 0.5 / 24.0);
    planetary_hour_t h1 = planetary_hour(SUN_6AM + 1.5 / 24.0);
    planetary_hour_t h2 = planetary_hour(SUN_6AM + 2.5 / 24.0);
    TEST_ASSERT_EQUAL_INT(3, h0.planet);
    TEST_ASSERT_EQUAL_INT(4, h1.planet);
    TEST_ASSERT_EQUAL_INT(5, h2.planet);
}

/* 9. Monday: hour 0=Moon(6), hour 1=Saturn(0) */
void test_monday_sequence(void)
{
    planetary_hour_t h0 = planetary_hour(MON_6AM + 0.5 / 24.0);
    planetary_hour_t h1 = planetary_hour(MON_6AM + 1.5 / 24.0);
    TEST_ASSERT_EQUAL_INT(6, h0.planet);
    TEST_ASSERT_EQUAL_INT(0, h1.planet);
}

/* 10. After 7 hours, ruler cycles back to same planet */
void test_cycle_after_7(void)
{
    planetary_hour_t h0 = planetary_hour(SUN_6AM + 0.5 / 24.0);
    planetary_hour_t h7 = planetary_hour(SUN_6AM + 7.5 / 24.0);
    TEST_ASSERT_EQUAL_INT(h0.planet, h7.planet);
}

/* 11. planet_chaldean_name(0) = "Saturn" */
void test_name_saturn(void)
{
    TEST_ASSERT_EQUAL_STRING("Saturn", planet_chaldean_name(0));
}

/* 12. planet_chaldean_name(3) = "Sun" */
void test_name_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("Sun", planet_chaldean_name(3));
}

/* 13. planet_chaldean_name(6) = "Moon" */
void test_name_moon(void)
{
    TEST_ASSERT_EQUAL_STRING("Moon", planet_chaldean_name(6));
}

/* 14. planetary_day_name(0) = "Sunday" */
void test_day_name_sunday(void)
{
    TEST_ASSERT_EQUAL_STRING("Sunday", planetary_day_name(0));
}

/* 15. All 7 symbols non-null and non-empty */
void test_all_symbols_nonnull(void)
{
    for (int i = 0; i < 7; i++) {
        const char *sym = planet_chaldean_symbol(i);
        TEST_ASSERT_NOT_NULL(sym);
        TEST_ASSERT_TRUE(strlen(sym) > 0);
    }
}

/* 16. planet always 0-6 across a full week */
void test_planet_range(void)
{
    for (double jd = J2000; jd < J2000 + 7.0; jd += 0.1) {
        planetary_hour_t h = planetary_hour(jd);
        TEST_ASSERT_TRUE(h.planet >= 0 && h.planet <= 6);
    }
}

/* 17. hour_index always 0-23 */
void test_hour_range(void)
{
    for (double jd = J2000; jd < J2000 + 7.0; jd += 0.1) {
        planetary_hour_t h = planetary_hour(jd);
        TEST_ASSERT_TRUE(h.hour_index >= 0 && h.hour_index <= 23);
    }
}

/* 18. day_of_week always 0-6 */
void test_dow_range(void)
{
    for (double jd = J2000; jd < J2000 + 7.0; jd += 0.1) {
        planetary_hour_t h = planetary_hour(jd);
        TEST_ASSERT_TRUE(h.day_of_week >= 0 && h.day_of_week <= 6);
    }
}

/* 19. Purity: same input gives same output */
void test_purity(void)
{
    planetary_hour_t a = planetary_hour(2451600.3);
    planetary_hour_t b = planetary_hour(2451600.3);
    TEST_ASSERT_EQUAL_INT(a.planet, b.planet);
    TEST_ASSERT_EQUAL_INT(a.hour_index, b.hour_index);
    TEST_ASSERT_EQUAL_INT(a.day_ruler, b.day_ruler);
    TEST_ASSERT_EQUAL_INT(a.day_of_week, b.day_of_week);
}

/* 20. Chaldean continuity: 24 hours from day ruler = next day ruler */
void test_chaldean_continuity(void)
{
    for (int dow = 0; dow < 7; dow++) {
        int ruler = planetary_day_ruler(dow);
        int after_24 = (ruler + 24) % 7;
        int next_dow = (dow + 1) % 7;
        TEST_ASSERT_EQUAL_INT(planetary_day_ruler(next_dow), after_24);
    }
}

/* 21. All 7 day names correct */
void test_all_day_names(void)
{
    const char *expected[7] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };
    for (int i = 0; i < 7; i++)
        TEST_ASSERT_EQUAL_STRING(expected[i], planetary_day_name(i));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sunday_ruler);
    RUN_TEST(test_monday_ruler);
    RUN_TEST(test_saturday_ruler);
    RUN_TEST(test_all_rulers_distinct);
    RUN_TEST(test_j2000_saturday);
    RUN_TEST(test_2024_monday);
    RUN_TEST(test_noon_hour_index);
    RUN_TEST(test_sunday_sequence);
    RUN_TEST(test_monday_sequence);
    RUN_TEST(test_cycle_after_7);
    RUN_TEST(test_name_saturn);
    RUN_TEST(test_name_sun);
    RUN_TEST(test_name_moon);
    RUN_TEST(test_day_name_sunday);
    RUN_TEST(test_all_symbols_nonnull);
    RUN_TEST(test_planet_range);
    RUN_TEST(test_hour_range);
    RUN_TEST(test_dow_range);
    RUN_TEST(test_purity);
    RUN_TEST(test_chaldean_continuity);
    RUN_TEST(test_all_day_names);
    return UNITY_END();
}
