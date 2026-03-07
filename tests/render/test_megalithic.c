#include "../unity/unity.h"
#include "../../src/render/megalithic.h"
#include "../../src/systems/astronomy/solar_events.h"

#include <math.h>

#define TOL 0.1f
#define DTOL 1.0f  /* degree tolerance for azimuth checks */

void setUp(void) {}
void tearDown(void) {}

/* ---- Site data tests ---- */

/* 1. Site count returns expected number */
void test_site_count(void)
{
    int n = meg_site_count();
    TEST_ASSERT_TRUE(n >= 5);
    TEST_ASSERT_TRUE(n <= MEG_MAX_SITES);
}

/* 2. Stonehenge exists at index 1 */
void test_stonehenge_data(void)
{
    meg_site_t s = meg_site_get(1);
    TEST_ASSERT_NOT_NULL(s.name);
    TEST_ASSERT_EQUAL_STRING("Stonehenge", s.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 51.1789f, (float)s.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.8262f, (float)s.lon);
    TEST_ASSERT_EQUAL_INT(MEG_EVENT_SUMMER_SOLSTICE_SUNRISE, s.event);
}

/* 3. Newgrange exists at index 0 */
void test_newgrange_data(void)
{
    meg_site_t s = meg_site_get(0);
    TEST_ASSERT_NOT_NULL(s.name);
    TEST_ASSERT_EQUAL_STRING("Newgrange", s.name);
    TEST_ASSERT_EQUAL_INT(MEG_EVENT_WINTER_SOLSTICE_SUNRISE, s.event);
    TEST_ASSERT_TRUE(s.epoch_bce == 3200);
}

/* 4. Invalid index returns sentinel */
void test_invalid_site_index(void)
{
    meg_site_t s = meg_site_get(-1);
    TEST_ASSERT_NULL(s.name);
    s = meg_site_get(999);
    TEST_ASSERT_NULL(s.name);
}

/* 5. All sites have valid latitudes */
void test_all_sites_valid_lat(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_site_t s = meg_site_get(i);
        TEST_ASSERT_TRUE(s.lat >= -90.0 && s.lat <= 90.0);
    }
}

/* 6. All sites have valid longitudes */
void test_all_sites_valid_lon(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_site_t s = meg_site_get(i);
        TEST_ASSERT_TRUE(s.lon >= -180.0 && s.lon <= 180.0);
    }
}

/* 7. All sites have positive tolerance */
void test_all_sites_positive_tolerance(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_site_t s = meg_site_get(i);
        TEST_ASSERT_TRUE(s.tolerance_deg > 0.0);
    }
}

/* 8. All sites have names and locations */
void test_all_sites_have_names(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_site_t s = meg_site_get(i);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_NOT_NULL(s.location);
        TEST_ASSERT_TRUE(s.name[0] != '\0');
        TEST_ASSERT_TRUE(s.location[0] != '\0');
    }
}

/* ---- Event name tests ---- */

/* 9. Event names are valid strings */
void test_event_name_valid(void)
{
    for (int i = 0; i < MEG_EVENT_COUNT; i++) {
        const char *name = meg_event_name((meg_event_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

/* 10. Invalid event returns "Unknown" */
void test_event_name_invalid(void)
{
    const char *name = meg_event_name(MEG_EVENT_COUNT);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

/* ---- Azimuth formula tests ---- */

/* 11. Equinox sunrise at equator = 90 degrees */
void test_sunrise_az_equinox_equator(void)
{
    TEST_ASSERT_FLOAT_WITHIN(TOL, 90.0f, (float)meg_sunrise_azimuth(0.0, 0.0));
}

/* 12. Equinox sunset at equator = 270 degrees */
void test_sunset_az_equinox_equator(void)
{
    TEST_ASSERT_FLOAT_WITHIN(TOL, 270.0f, (float)meg_sunset_azimuth(0.0, 0.0));
}

/* 13. Stonehenge summer solstice sunrise ~ 50.6 degrees */
void test_sunrise_az_stonehenge_summer(void)
{
    TEST_ASSERT_FLOAT_WITHIN(DTOL, 50.6f, (float)meg_sunrise_azimuth(51.1789, 23.4393));
}

/* 14. Newgrange winter solstice sunrise ~ 132.2 degrees */
void test_sunrise_az_newgrange_winter(void)
{
    TEST_ASSERT_FLOAT_WITHIN(DTOL, 132.2f, (float)meg_sunrise_azimuth(53.6947, -23.4393));
}

/* 15. Sunset is always 360 - sunrise azimuth */
void test_sunset_is_complement(void)
{
    double sr = meg_sunrise_azimuth(45.0, 15.0);
    double ss = meg_sunset_azimuth(45.0, 15.0);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 360.0f, (float)(sr + ss));
}

/* 16. Sunrise azimuth increases for more negative declination */
void test_sunrise_az_increases_with_negative_dec(void)
{
    double az1 = meg_sunrise_azimuth(50.0, 0.0);
    double az2 = meg_sunrise_azimuth(50.0, -23.4);
    TEST_ASSERT_TRUE(az2 > az1);
}

/* 17. Sunrise azimuth decreases for positive declination */
void test_sunrise_az_decreases_with_positive_dec(void)
{
    double az1 = meg_sunrise_azimuth(50.0, 0.0);
    double az2 = meg_sunrise_azimuth(50.0, 23.4);
    TEST_ASSERT_TRUE(az2 < az1);
}

/* 18. Polar case: sunrise azimuth clamped when ratio > 1 */
void test_sunrise_az_polar_clamp(void)
{
    /* lat 70, dec 23.44: sin(23.44)/cos(70) > 1.0 -> midnight sun */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, (float)meg_sunrise_azimuth(70.0, 23.4393));
}

/* 19. Polar case: sunrise azimuth 180 when ratio < -1 */
void test_sunrise_az_polar_night(void)
{
    /* lat 70, dec -23.44: sin(-23.44)/cos(70) < -1.0 -> polar night */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 180.0f, (float)meg_sunrise_azimuth(70.0, -23.4393));
}

/* ---- Declination constant tests ---- */

/* 20. Summer solstice declination = +23.4393 */
void test_solstice_dec_summer(void)
{
    TEST_ASSERT_FLOAT_WITHIN(TOL, 23.4393f, (float)meg_solstice_declination(1));
}

/* 21. Winter solstice declination = -23.4393 */
void test_solstice_dec_winter(void)
{
    TEST_ASSERT_FLOAT_WITHIN(TOL, -23.4393f, (float)meg_solstice_declination(0));
}

/* 22. Equinox declination = 0.0 */
void test_equinox_dec(void)
{
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, (float)meg_equinox_declination());
}

/* ---- Lunar standstill tests ---- */

/* 23. Northern lunar standstill azimuth at Callanish lat */
void test_lunar_standstill_north(void)
{
    /* Moonrise at dec +28.5843 at lat 58.1976 -> ~24.8 degrees */
    TEST_ASSERT_FLOAT_WITHIN(DTOL, 24.8f, (float)meg_lunar_standstill_azimuth(58.1976, 1));
}

/* 24. Southern lunar standstill azimuth at Callanish lat */
void test_lunar_standstill_south(void)
{
    /* Moonset at dec -28.5843 at lat 58.1976 -> ~204.8 degrees */
    TEST_ASSERT_FLOAT_WITHIN(DTOL, 204.8f, (float)meg_lunar_standstill_azimuth(58.1976, 0));
}

/* 25. Lunar standstill azimuth at equator */
void test_lunar_standstill_equator(void)
{
    /* At equator, north moonrise at dec +28.58 -> acos(sin(28.58)) ~ 61.4 */
    TEST_ASSERT_FLOAT_WITHIN(DTOL, 61.4f, (float)meg_lunar_standstill_azimuth(0.0, 1));
}

/* ---- Alignment check tests ---- */

/* 26. Stonehenge alignment near summer solstice: delta is small */
void test_alignment_stonehenge_solstice(void)
{
    /* JD for ~June 21, 2024 12:00 UT */
    double jd = 2460483.0;
    meg_alignment_t a = meg_check_alignment(1, jd);
    /* Near solstice, delta should be small */
    TEST_ASSERT_TRUE(a.delta_deg < 10.0);
    /* site_az should be Stonehenge's alignment */
    meg_site_t s = meg_site_get(1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, (float)s.alignment_az, (float)a.site_az);
}

/* 27. Mnajdra alignment near equinox: delta is small */
void test_alignment_mnajdra_equinox(void)
{
    /* JD for ~March 20, 2024 12:00 UT */
    double jd = 2460389.0;
    meg_alignment_t a = meg_check_alignment(4, jd);
    /* Near equinox, sunrise az is close to 90, which matches Mnajdra */
    TEST_ASSERT_TRUE(a.delta_deg < 5.0);
}

/* 28. Invalid site index for alignment returns not aligned */
void test_alignment_invalid_index(void)
{
    meg_alignment_t a = meg_check_alignment(-1, 2460000.0);
    TEST_ASSERT_FALSE(a.aligned);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, (float)a.event_az);
}

/* 29. meg_check_alignment event_az is non-negative for valid sites */
void test_alignment_az_positive(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_alignment_t a = meg_check_alignment(i, 2460000.0);
        TEST_ASSERT_TRUE(a.event_az >= 0.0);
        TEST_ASSERT_TRUE(a.event_az <= 360.0);
    }
}

/* ---- meg_aligned_now tests ---- */

/* 30. meg_aligned_now returns count in valid range */
void test_aligned_now_valid_range(void)
{
    int out[MEG_MAX_SITES];
    int count = meg_aligned_now(2460000.0, out, MEG_MAX_SITES);
    TEST_ASSERT_TRUE(count >= 0);
    TEST_ASSERT_TRUE(count <= MEG_MAX_SITES);
}

/* 31. meg_aligned_now respects max_out limit */
void test_aligned_now_max_limit(void)
{
    int out[1];
    int count = meg_aligned_now(2460000.0, out, 1);
    TEST_ASSERT_TRUE(count <= 1);
}

/* 32. meg_aligned_now with zero max returns zero */
void test_aligned_now_zero_max(void)
{
    int out[1];
    int count = meg_aligned_now(2460000.0, out, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ---- meg_days_to_alignment tests ---- */

/* 33. Days to alignment for solstice site is in [0, 366] */
void test_days_to_alignment_range(void)
{
    /* Stonehenge: summer solstice event */
    int days = meg_days_to_alignment(1, 2460000.0);
    TEST_ASSERT_TRUE(days >= 0);
    TEST_ASSERT_TRUE(days <= 366);
}

/* 34. Days to alignment for equinox site is in [0, 366] */
void test_days_to_equinox_range(void)
{
    /* Mnajdra: equinox event */
    int days = meg_days_to_alignment(4, 2460000.0);
    TEST_ASSERT_TRUE(days >= 0);
    TEST_ASSERT_TRUE(days <= 366);
}

/* 35. Days to alignment returns -1 for lunar standstill */
void test_days_to_lunar_standstill_returns_negative(void)
{
    /* Callanish: lunar standstill event */
    int days = meg_days_to_alignment(2, 2460000.0);
    TEST_ASSERT_EQUAL_INT(-1, days);
}

/* 36. Days to alignment invalid index returns -1 */
void test_days_to_alignment_invalid(void)
{
    int days = meg_days_to_alignment(-1, 2460000.0);
    TEST_ASSERT_EQUAL_INT(-1, days);
    days = meg_days_to_alignment(999, 2460000.0);
    TEST_ASSERT_EQUAL_INT(-1, days);
}

/* ---- Purity tests ---- */

/* 37. Same inputs produce same outputs (purity) */
void test_purity_sunrise(void)
{
    double a1 = meg_sunrise_azimuth(51.0, 23.4);
    double a2 = meg_sunrise_azimuth(51.0, 23.4);
    TEST_ASSERT_TRUE(a1 == a2);
}

/* 38. Same inputs produce same outputs for alignment check */
void test_purity_alignment(void)
{
    meg_alignment_t a1 = meg_check_alignment(0, 2460000.0);
    meg_alignment_t a2 = meg_check_alignment(0, 2460000.0);
    TEST_ASSERT_TRUE(a1.event_az == a2.event_az);
    TEST_ASSERT_TRUE(a1.delta_deg == a2.delta_deg);
    TEST_ASSERT_EQUAL_INT(a1.aligned, a2.aligned);
}

/* ---- Edge case tests ---- */

/* 39. Sunrise azimuth at latitude 0, positive declination */
void test_sunrise_az_equator_positive_dec(void)
{
    /* acos(sin(23.44)) = acos(0.3978) ~ 66.6 degrees */
    TEST_ASSERT_FLOAT_WITHIN(DTOL, 66.6f, (float)meg_sunrise_azimuth(0.0, 23.4393));
}

/* 40. Carnac alignment check */
void test_carnac_site_data(void)
{
    meg_site_t s = meg_site_get(3);
    TEST_ASSERT_EQUAL_STRING("Carnac", s.name);
    TEST_ASSERT_EQUAL_STRING("France", s.location);
    TEST_ASSERT_EQUAL_INT(MEG_EVENT_SUMMER_SOLSTICE_SUNRISE, s.event);
    TEST_ASSERT_TRUE(s.epoch_bce == 4500);
}

/* 41. Callanish site data */
void test_callanish_site_data(void)
{
    meg_site_t s = meg_site_get(2);
    TEST_ASSERT_EQUAL_STRING("Callanish", s.name);
    TEST_ASSERT_EQUAL_INT(MEG_EVENT_LUNAR_STANDSTILL_SOUTH, s.event);
}

/* 42. Mnajdra site data */
void test_mnajdra_site_data(void)
{
    meg_site_t s = meg_site_get(4);
    TEST_ASSERT_EQUAL_STRING("Mnajdra", s.name);
    TEST_ASSERT_EQUAL_STRING("Malta", s.location);
    TEST_ASSERT_EQUAL_INT(MEG_EVENT_EQUINOX_SUNRISE, s.event);
}

/* 43. All alignment_az values in [0, 360) */
void test_all_alignment_az_valid(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_site_t s = meg_site_get(i);
        TEST_ASSERT_TRUE(s.alignment_az >= 0.0);
        TEST_ASSERT_TRUE(s.alignment_az < 360.0);
    }
}

/* 44. All epoch_bce values are positive */
void test_all_epochs_positive(void)
{
    int n = meg_site_count();
    for (int i = 0; i < n; i++) {
        meg_site_t s = meg_site_get(i);
        TEST_ASSERT_TRUE(s.epoch_bce > 0);
    }
}

/* 45. Mnajdra equinox sunrise exactly 90 degrees (dec=0) */
void test_mnajdra_computed_azimuth(void)
{
    double dec = meg_equinox_declination();
    TEST_ASSERT_FLOAT_WITHIN(TOL, 90.0f, (float)meg_sunrise_azimuth(35.8267, dec));
}

int main(void)
{
    UNITY_BEGIN();

    /* Site data (1-8) */
    RUN_TEST(test_site_count);
    RUN_TEST(test_stonehenge_data);
    RUN_TEST(test_newgrange_data);
    RUN_TEST(test_invalid_site_index);
    RUN_TEST(test_all_sites_valid_lat);
    RUN_TEST(test_all_sites_valid_lon);
    RUN_TEST(test_all_sites_positive_tolerance);
    RUN_TEST(test_all_sites_have_names);

    /* Event names (9-10) */
    RUN_TEST(test_event_name_valid);
    RUN_TEST(test_event_name_invalid);

    /* Azimuth formulas (11-19) */
    RUN_TEST(test_sunrise_az_equinox_equator);
    RUN_TEST(test_sunset_az_equinox_equator);
    RUN_TEST(test_sunrise_az_stonehenge_summer);
    RUN_TEST(test_sunrise_az_newgrange_winter);
    RUN_TEST(test_sunset_is_complement);
    RUN_TEST(test_sunrise_az_increases_with_negative_dec);
    RUN_TEST(test_sunrise_az_decreases_with_positive_dec);
    RUN_TEST(test_sunrise_az_polar_clamp);
    RUN_TEST(test_sunrise_az_polar_night);

    /* Declination constants (20-22) */
    RUN_TEST(test_solstice_dec_summer);
    RUN_TEST(test_solstice_dec_winter);
    RUN_TEST(test_equinox_dec);

    /* Lunar standstill (23-25) */
    RUN_TEST(test_lunar_standstill_north);
    RUN_TEST(test_lunar_standstill_south);
    RUN_TEST(test_lunar_standstill_equator);

    /* Alignment checks (26-29) */
    RUN_TEST(test_alignment_stonehenge_solstice);
    RUN_TEST(test_alignment_mnajdra_equinox);
    RUN_TEST(test_alignment_invalid_index);
    RUN_TEST(test_alignment_az_positive);

    /* meg_aligned_now (30-32) */
    RUN_TEST(test_aligned_now_valid_range);
    RUN_TEST(test_aligned_now_max_limit);
    RUN_TEST(test_aligned_now_zero_max);

    /* meg_days_to_alignment (33-36) */
    RUN_TEST(test_days_to_alignment_range);
    RUN_TEST(test_days_to_equinox_range);
    RUN_TEST(test_days_to_lunar_standstill_returns_negative);
    RUN_TEST(test_days_to_alignment_invalid);

    /* Purity (37-38) */
    RUN_TEST(test_purity_sunrise);
    RUN_TEST(test_purity_alignment);

    /* Edge cases & extra validation (39-45) */
    RUN_TEST(test_sunrise_az_equator_positive_dec);
    RUN_TEST(test_carnac_site_data);
    RUN_TEST(test_callanish_site_data);
    RUN_TEST(test_mnajdra_site_data);
    RUN_TEST(test_all_alignment_az_valid);
    RUN_TEST(test_all_epochs_positive);
    RUN_TEST(test_mnajdra_computed_azimuth);

    return UNITY_END();
}
