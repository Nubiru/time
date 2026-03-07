/* test_decan_stars.c — Tests for the 36 Egyptian decan star data module.
 * Sources: Neugebauer & Parker "Egyptian Astronomical Texts" tradition.
 * TDD: RED phase — tests written before implementation. */

#include "../unity/unity.h"
#include "../../src/render/decan_stars.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define TOL 0.001
#define NEAR_D(exp, act) (fabs((double)(act) - (double)(exp)) < TOL)

void setUp(void) {}
void tearDown(void) {}

/* --- Data integrity tests --- */

/* 1. Total decan count is 36 */
void test_decan_count_is_36(void)
{
    TEST_ASSERT_EQUAL_INT(36, decan_count());
}

/* 2. All 36 decans are retrievable with valid indices */
void test_all_decans_valid_index(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_EQUAL_INT(i, d.index);
    }
}

/* 3. Invalid index returns sentinel (index == -1) */
void test_invalid_index_returns_sentinel(void)
{
    decan_t d = decan_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, d.index);

    d = decan_get(36);
    TEST_ASSERT_EQUAL_INT(-1, d.index);

    d = decan_get(999);
    TEST_ASSERT_EQUAL_INT(-1, d.index);
}

/* 4. Every decan has a non-null, non-empty Egyptian name */
void test_all_decans_have_egyptian_name(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_NOT_NULL(d.egyptian_name);
        TEST_ASSERT_TRUE(strlen(d.egyptian_name) > 0);
    }
}

/* 5. Every decan has a non-null, non-empty principal star */
void test_all_decans_have_principal_star(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_NOT_NULL(d.principal_star);
        TEST_ASSERT_TRUE(strlen(d.principal_star) > 0);
    }
}

/* 6. Every decan has a non-null description */
void test_all_decans_have_description(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_NOT_NULL(d.description);
        TEST_ASSERT_TRUE(strlen(d.description) > 0);
    }
}

/* 7. Ecliptic longitude ranges cover 0-360 contiguously */
void test_ecliptic_longitude_contiguous(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_TRUE(NEAR_D(i * DECAN_DEGREES, d.ecl_lon_start));
        TEST_ASSERT_TRUE(NEAR_D((i + 1) * DECAN_DEGREES, d.ecl_lon_end));
    }
}

/* 8. Each decan spans exactly 10 degrees */
void test_each_decan_spans_10_degrees(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_TRUE(NEAR_D(DECAN_DEGREES, d.ecl_lon_end - d.ecl_lon_start));
    }
}

/* 9. Zodiac sign assignments: 3 decans per sign, 0-11 */
void test_zodiac_sign_valid_range(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_TRUE(d.zodiac_sign >= 0 && d.zodiac_sign <= 11);
    }
}

/* 10. Decan-in-sign cycles 1,2,3 for each sign */
void test_decan_in_sign_cycles(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        int expected_decan = (i % 3) + 1;
        TEST_ASSERT_EQUAL_INT(expected_decan, d.decan_in_sign);
    }
}

/* 11. Zodiac sign advances every 3 decans */
void test_zodiac_sign_advances_every_3(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        int expected_sign = i / 3;
        TEST_ASSERT_EQUAL_INT(expected_sign, d.zodiac_sign);
    }
}

/* 12. Element follows zodiac triplicity: Fire/Earth/Air/Water cycle */
void test_element_follows_triplicity(void)
{
    /* Aries=Fire, Taurus=Earth, Gemini=Air, Cancer=Water, ...repeat */
    decan_element_t sign_elements[12] = {
        DECAN_ELEM_FIRE,  DECAN_ELEM_EARTH, DECAN_ELEM_AIR,   DECAN_ELEM_WATER,
        DECAN_ELEM_FIRE,  DECAN_ELEM_EARTH, DECAN_ELEM_AIR,   DECAN_ELEM_WATER,
        DECAN_ELEM_FIRE,  DECAN_ELEM_EARTH, DECAN_ELEM_AIR,   DECAN_ELEM_WATER
    };
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t d = decan_get(i);
        TEST_ASSERT_EQUAL_INT(sign_elements[d.zodiac_sign], d.element);
    }
}

/* 13. First decan is Aries 1st decan (0-10 deg) */
void test_first_decan_is_aries(void)
{
    decan_t d = decan_get(0);
    TEST_ASSERT_EQUAL_INT(0, d.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(1, d.decan_in_sign);
    TEST_ASSERT_TRUE(NEAR_D(0.0, d.ecl_lon_start));
    TEST_ASSERT_TRUE(NEAR_D(10.0, d.ecl_lon_end));
}

/* 14. Last decan is Pisces 3rd decan (350-360 deg) */
void test_last_decan_is_pisces(void)
{
    decan_t d = decan_get(35);
    TEST_ASSERT_EQUAL_INT(11, d.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(3, d.decan_in_sign);
    TEST_ASSERT_TRUE(NEAR_D(350.0, d.ecl_lon_start));
    TEST_ASSERT_TRUE(NEAR_D(360.0, d.ecl_lon_end));
}

/* --- Longitude mapping tests --- */

/* 15. decan_for_ecliptic_longitude at exact boundaries */
void test_longitude_exact_boundaries(void)
{
    TEST_ASSERT_EQUAL_INT(0, decan_for_ecliptic_longitude(0.0));
    TEST_ASSERT_EQUAL_INT(0, decan_for_ecliptic_longitude(9.999));
    TEST_ASSERT_EQUAL_INT(1, decan_for_ecliptic_longitude(10.0));
    TEST_ASSERT_EQUAL_INT(35, decan_for_ecliptic_longitude(350.0));
    TEST_ASSERT_EQUAL_INT(35, decan_for_ecliptic_longitude(359.999));
}

/* 16. decan_for_ecliptic_longitude wraps at 360 */
void test_longitude_wraps_at_360(void)
{
    TEST_ASSERT_EQUAL_INT(0, decan_for_ecliptic_longitude(360.0));
    TEST_ASSERT_EQUAL_INT(0, decan_for_ecliptic_longitude(720.0));
}

/* 17. decan_for_ecliptic_longitude handles negative */
void test_longitude_negative(void)
{
    /* -10 deg should wrap to 350 deg = decan 35 */
    TEST_ASSERT_EQUAL_INT(35, decan_for_ecliptic_longitude(-10.0));
    /* -360 should wrap to 0 = decan 0 */
    TEST_ASSERT_EQUAL_INT(0, decan_for_ecliptic_longitude(-360.0));
}

/* 18. Midpoint of each decan via longitude mapping */
void test_longitude_midpoints(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        double mid = i * 10.0 + 5.0;
        TEST_ASSERT_EQUAL_INT(i, decan_for_ecliptic_longitude(mid));
    }
}

/* --- Sign mapping tests --- */

/* 19. decan_for_sign returns 3 decans for each valid sign */
void test_sign_returns_3_decans(void)
{
    int out[3];
    for (int sign = 0; sign < 12; sign++) {
        int count = decan_for_sign(sign, out, 3);
        TEST_ASSERT_EQUAL_INT(3, count);
    }
}

/* 20. decan_for_sign returns correct indices for Aries (sign 0) */
void test_sign_aries_decans(void)
{
    int out[3];
    decan_for_sign(0, out, 3);
    TEST_ASSERT_EQUAL_INT(0, out[0]);
    TEST_ASSERT_EQUAL_INT(1, out[1]);
    TEST_ASSERT_EQUAL_INT(2, out[2]);
}

/* 21. decan_for_sign returns correct indices for Pisces (sign 11) */
void test_sign_pisces_decans(void)
{
    int out[3];
    decan_for_sign(11, out, 3);
    TEST_ASSERT_EQUAL_INT(33, out[0]);
    TEST_ASSERT_EQUAL_INT(34, out[1]);
    TEST_ASSERT_EQUAL_INT(35, out[2]);
}

/* 22. decan_for_sign invalid sign returns 0 */
void test_sign_invalid_returns_0(void)
{
    int out[3];
    TEST_ASSERT_EQUAL_INT(0, decan_for_sign(-1, out, 3));
    TEST_ASSERT_EQUAL_INT(0, decan_for_sign(12, out, 3));
}

/* 23. decan_for_sign respects max_out */
void test_sign_max_out_respected(void)
{
    int out[1];
    int count = decan_for_sign(0, out, 1);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(0, out[0]);
}

/* --- Element name tests --- */

/* 24. Element name strings */
void test_element_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Fire", decan_element_name(DECAN_ELEM_FIRE));
    TEST_ASSERT_EQUAL_STRING("Earth", decan_element_name(DECAN_ELEM_EARTH));
    TEST_ASSERT_EQUAL_STRING("Air", decan_element_name(DECAN_ELEM_AIR));
    TEST_ASSERT_EQUAL_STRING("Water", decan_element_name(DECAN_ELEM_WATER));
}

/* 25. Invalid element returns "Unknown" */
void test_element_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", decan_element_name(DECAN_ELEM_COUNT));
    TEST_ASSERT_EQUAL_STRING("Unknown", decan_element_name((decan_element_t)99));
}

/* --- Star longitude tests --- */

/* 26. Star longitude returns midpoint of decan range */
void test_star_longitude_midpoint(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        double got = decan_star_longitude(i);
        TEST_ASSERT_TRUE(NEAR_D(i * 10.0 + 5.0, got));
    }
}

/* 27. Star longitude invalid index returns -1 */
void test_star_longitude_invalid(void)
{
    TEST_ASSERT_TRUE(NEAR_D(-1.0, decan_star_longitude(-1)));
    TEST_ASSERT_TRUE(NEAR_D(-1.0, decan_star_longitude(36)));
}

/* --- Rising decan tests --- */

/* 28. decan_rising_now at LST=0 returns valid decan */
void test_rising_at_lst_0(void)
{
    int idx = decan_rising_now(0.0, 0.0);
    TEST_ASSERT_TRUE(idx >= 0 && idx < DECAN_COUNT);
}

/* 29. decan_rising_now returns valid index for various LSTs */
void test_rising_valid_range(void)
{
    for (double lst = 0.0; lst < 360.0; lst += 30.0) {
        int idx = decan_rising_now(lst, 30.0);
        TEST_ASSERT_TRUE(idx >= 0 && idx < DECAN_COUNT);
    }
}

/* 30. decan_rising_now advances with LST */
void test_rising_advances_with_lst(void)
{
    /* Over a full 360 deg sweep we should see many different decans. */
    int seen[DECAN_COUNT];
    memset(seen, 0, sizeof(seen));
    for (double lst = 0.0; lst < 360.0; lst += 10.0) {
        int idx = decan_rising_now(lst, 30.0);
        seen[idx] = 1;
    }
    int unique = 0;
    for (int i = 0; i < DECAN_COUNT; i++) {
        if (seen[i]) unique++;
    }
    /* Should see most of the 36 decans over a full sidereal rotation */
    TEST_ASSERT_TRUE(unique >= 20);
}

/* --- Night hours tests --- */

/* 31. decan_night_hours fills up to 12 entries */
void test_night_hours_fills_12(void)
{
    decan_hour_t hours[12];
    int count = decan_night_hours(0.0, 30.0, hours, 12);
    TEST_ASSERT_EQUAL_INT(12, count);
}

/* 32. decan_night_hours respects max_out */
void test_night_hours_max_out(void)
{
    decan_hour_t hours[4];
    int count = decan_night_hours(0.0, 30.0, hours, 4);
    TEST_ASSERT_EQUAL_INT(4, count);
}

/* 33. Each night hour has a valid decan index */
void test_night_hours_valid_decans(void)
{
    decan_hour_t hours[12];
    decan_night_hours(0.0, 30.0, hours, 12);
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_TRUE(hours[i].decan_index >= 0);
        TEST_ASSERT_TRUE(hours[i].decan_index < DECAN_COUNT);
    }
}

/* 34. Night hours advance through successive decans */
void test_night_hours_advance(void)
{
    decan_hour_t hours[12];
    decan_night_hours(0.0, 30.0, hours, 12);
    /* Each successive hour: next decan = (prev + 1) % 36 */
    for (int i = 1; i < 12; i++) {
        int expected = (hours[i - 1].decan_index + 1) % DECAN_COUNT;
        TEST_ASSERT_EQUAL_INT(expected, hours[i].decan_index);
    }
}

/* 35. decan_for_night_hour returns valid decan for hour 0-11 */
void test_for_night_hour_valid(void)
{
    for (int h = 0; h < 12; h++) {
        int idx = decan_for_night_hour(0.0, 30.0, h);
        TEST_ASSERT_TRUE(idx >= 0 && idx < DECAN_COUNT);
    }
}

/* 36. decan_for_night_hour invalid hour returns -1 */
void test_for_night_hour_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, decan_for_night_hour(0.0, 30.0, -1));
    TEST_ASSERT_EQUAL_INT(-1, decan_for_night_hour(0.0, 30.0, 12));
}

/* 37. decan_for_night_hour matches decan_night_hours output */
void test_for_night_hour_matches_array(void)
{
    double lst = 45.0;
    double lat = 25.0;
    decan_hour_t hours[12];
    decan_night_hours(lst, lat, hours, 12);
    for (int h = 0; h < 12; h++) {
        int idx = decan_for_night_hour(lst, lat, h);
        TEST_ASSERT_EQUAL_INT(hours[h].decan_index, idx);
    }
}

/* --- Purity test --- */

/* 38. Same inputs always produce same outputs */
void test_purity(void)
{
    decan_t a = decan_get(15);
    decan_t b = decan_get(15);
    TEST_ASSERT_EQUAL_INT(a.index, b.index);
    TEST_ASSERT_EQUAL_STRING(a.egyptian_name, b.egyptian_name);
    TEST_ASSERT_TRUE(NEAR_D(a.ecl_lon_start, b.ecl_lon_start));

    int idx_a = decan_for_ecliptic_longitude(123.456);
    int idx_b = decan_for_ecliptic_longitude(123.456);
    TEST_ASSERT_EQUAL_INT(idx_a, idx_b);

    int rise_a = decan_rising_now(100.0, 30.0);
    int rise_b = decan_rising_now(100.0, 30.0);
    TEST_ASSERT_EQUAL_INT(rise_a, rise_b);
}

/* --- Specific decan data spot checks --- */

/* 39. Decan 0 (Aries 1st) is Fire element */
void test_decan_0_aries_first(void)
{
    decan_t d = decan_get(0);
    TEST_ASSERT_EQUAL_INT(DECAN_ELEM_FIRE, d.element);
    TEST_ASSERT_NOT_NULL(d.egyptian_name);
}

/* 40. Decan at Leo (sign 4) should be Fire element */
void test_leo_decans_fire(void)
{
    int out[3];
    decan_for_sign(4, out, 3);
    for (int i = 0; i < 3; i++) {
        decan_t d = decan_get(out[i]);
        TEST_ASSERT_EQUAL_INT(DECAN_ELEM_FIRE, d.element);
    }
}

/* 41. Cancer decans (sign 3) should be Water */
void test_cancer_decans_water(void)
{
    int out[3];
    decan_for_sign(3, out, 3);
    for (int i = 0; i < 3; i++) {
        decan_t d = decan_get(out[i]);
        TEST_ASSERT_EQUAL_INT(DECAN_ELEM_WATER, d.element);
    }
}

/* 42. No two decans share the same Egyptian name */
void test_unique_egyptian_names(void)
{
    for (int i = 0; i < DECAN_COUNT; i++) {
        decan_t di = decan_get(i);
        for (int j = i + 1; j < DECAN_COUNT; j++) {
            decan_t dj = decan_get(j);
            TEST_ASSERT_TRUE(strcmp(di.egyptian_name, dj.egyptian_name) != 0);
        }
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* Data integrity (1-14) */
    RUN_TEST(test_decan_count_is_36);
    RUN_TEST(test_all_decans_valid_index);
    RUN_TEST(test_invalid_index_returns_sentinel);
    RUN_TEST(test_all_decans_have_egyptian_name);
    RUN_TEST(test_all_decans_have_principal_star);
    RUN_TEST(test_all_decans_have_description);
    RUN_TEST(test_ecliptic_longitude_contiguous);
    RUN_TEST(test_each_decan_spans_10_degrees);
    RUN_TEST(test_zodiac_sign_valid_range);
    RUN_TEST(test_decan_in_sign_cycles);
    RUN_TEST(test_zodiac_sign_advances_every_3);
    RUN_TEST(test_element_follows_triplicity);
    RUN_TEST(test_first_decan_is_aries);
    RUN_TEST(test_last_decan_is_pisces);
    /* Longitude mapping (15-18) */
    RUN_TEST(test_longitude_exact_boundaries);
    RUN_TEST(test_longitude_wraps_at_360);
    RUN_TEST(test_longitude_negative);
    RUN_TEST(test_longitude_midpoints);
    /* Sign mapping (19-23) */
    RUN_TEST(test_sign_returns_3_decans);
    RUN_TEST(test_sign_aries_decans);
    RUN_TEST(test_sign_pisces_decans);
    RUN_TEST(test_sign_invalid_returns_0);
    RUN_TEST(test_sign_max_out_respected);
    /* Element names (24-25) */
    RUN_TEST(test_element_names);
    RUN_TEST(test_element_name_invalid);
    /* Star longitude (26-27) */
    RUN_TEST(test_star_longitude_midpoint);
    RUN_TEST(test_star_longitude_invalid);
    /* Rising decan (28-30) */
    RUN_TEST(test_rising_at_lst_0);
    RUN_TEST(test_rising_valid_range);
    RUN_TEST(test_rising_advances_with_lst);
    /* Night hours (31-37) */
    RUN_TEST(test_night_hours_fills_12);
    RUN_TEST(test_night_hours_max_out);
    RUN_TEST(test_night_hours_valid_decans);
    RUN_TEST(test_night_hours_advance);
    RUN_TEST(test_for_night_hour_valid);
    RUN_TEST(test_for_night_hour_invalid);
    RUN_TEST(test_for_night_hour_matches_array);
    /* Purity (38) */
    RUN_TEST(test_purity);
    /* Spot checks (39-42) */
    RUN_TEST(test_decan_0_aries_first);
    RUN_TEST(test_leo_decans_fire);
    RUN_TEST(test_cancer_decans_water);
    RUN_TEST(test_unique_egyptian_names);
    return UNITY_END();
}
