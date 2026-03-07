#include "../unity/unity.h"
#include "../../src/render/earth_atmosphere.h"

#include <math.h>

#define TOL 0.001f
#define FTOL 0.01f

/* Helper: check double is within tolerance of expected */
#define ASSERT_NEAR_D(expected, actual, tol) \
    TEST_ASSERT_TRUE((actual) >= (expected) - (tol) && (actual) <= (expected) + (tol))

void setUp(void) {}
void tearDown(void) {}

/* =========================================================
 *  Atmospheric density (5 tests)
 * ========================================================= */

/* 1. Sea level density = 1.0 */
void test_density_sea_level(void)
{
    ASSERT_NEAR_D(1.0, earth_atmo_density(0.0), 0.001);
}

/* 2. One scale height (8.5 km) => 1/e ~ 0.3679 */
void test_density_one_scale_height(void)
{
    double expected = exp(-1.0); /* 0.36788 */
    ASSERT_NEAR_D(expected, earth_atmo_density(8.5), 0.01);
}

/* 3. 50 km altitude => very low */
void test_density_50km(void)
{
    double d = earth_atmo_density(50.0);
    double expected = exp(-50.0 / 8.5);
    ASSERT_NEAR_D(expected, d, 0.001);
    TEST_ASSERT_TRUE(d < 0.01);
}

/* 4. 100 km (Karman line) => nearly zero */
void test_density_100km(void)
{
    TEST_ASSERT_TRUE(earth_atmo_density(100.0) < 0.0001);
}

/* 5. Negative altitude clamped to 1.0 (sea level) */
void test_density_negative_altitude(void)
{
    ASSERT_NEAR_D(1.0, earth_atmo_density(-10.0), 0.001);
}

/* =========================================================
 *  Air mass (5 tests)
 * ========================================================= */

/* 6. Air mass at zenith (90 degrees) = 1.0 */
void test_air_mass_zenith(void)
{
    ASSERT_NEAR_D(1.0, earth_atmo_air_mass(90.0), 0.01);
}

/* 7. Air mass at 30 degrees elevation ~ 2.0 */
void test_air_mass_30deg(void)
{
    ASSERT_NEAR_D(2.0, earth_atmo_air_mass(30.0), 0.1);
}

/* 8. Air mass near horizon is large (>10) */
void test_air_mass_horizon(void)
{
    TEST_ASSERT_TRUE(earth_atmo_air_mass(1.0) > 10.0);
}

/* 9. Air mass at 45 degrees ~ 1.41 */
void test_air_mass_45deg(void)
{
    ASSERT_NEAR_D(1.41, earth_atmo_air_mass(45.0), 0.1);
}

/* 10. Air mass at 0 or negative elevation is clamped to large value */
void test_air_mass_zero_elevation(void)
{
    double am = earth_atmo_air_mass(0.0);
    TEST_ASSERT_TRUE(am > 30.0);
    TEST_ASSERT_TRUE(am < 50.0);
}

/* =========================================================
 *  Optical depth (3 tests)
 * ========================================================= */

/* 11. Optical depth at zenith (0 degrees) = 1.0 */
void test_optical_depth_zenith(void)
{
    ASSERT_NEAR_D(1.0, earth_atmo_optical_depth(0.0), 0.01);
}

/* 12. Optical depth at 60 degrees zenith > 1.0 */
void test_optical_depth_60deg(void)
{
    TEST_ASSERT_TRUE(earth_atmo_optical_depth(60.0) > 1.5);
}

/* 13. Optical depth at 85 degrees zenith is large */
void test_optical_depth_85deg(void)
{
    TEST_ASSERT_TRUE(earth_atmo_optical_depth(85.0) > 5.0);
}

/* =========================================================
 *  Rayleigh scattering sky color (5 tests)
 * ========================================================= */

/* 14. Sea level, overhead sun: blue dominant */
void test_sky_color_overhead_blue(void)
{
    atmo_color_t c = earth_atmo_color(0.0, 90.0);
    TEST_ASSERT_TRUE(c.b > c.r);
    TEST_ASSERT_TRUE(c.b > c.g);
    TEST_ASSERT_TRUE(c.b > 0.5f);
}

/* 15. High altitude (80 km): darker than sea level */
void test_sky_color_high_altitude_dark(void)
{
    atmo_color_t lo = earth_atmo_color(0.0, 90.0);
    atmo_color_t hi = earth_atmo_color(80.0, 90.0);
    TEST_ASSERT_TRUE(hi.r < lo.r);
    TEST_ASSERT_TRUE(hi.g < lo.g);
    TEST_ASSERT_TRUE(hi.b < lo.b);
}

/* 16. Sun below horizon => dimmer */
void test_sky_color_night_dim(void)
{
    atmo_color_t day = earth_atmo_color(0.0, 45.0);
    atmo_color_t night = earth_atmo_color(0.0, -30.0);
    float day_lum = day.r + day.g + day.b;
    float night_lum = night.r + night.g + night.b;
    TEST_ASSERT_TRUE(night_lum < day_lum);
}

/* 17. All components in [0,1] */
void test_sky_color_range(void)
{
    atmo_color_t c = earth_atmo_color(0.0, 45.0);
    TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
    TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
}

/* 18. Low sun elevation: more red/orange relative to blue */
void test_sky_color_low_sun_warm(void)
{
    atmo_color_t c = earth_atmo_color(0.0, 5.0);
    atmo_color_t hi = earth_atmo_color(0.0, 90.0);
    /* ratio of red/blue should be higher at low sun */
    double ratio_low = (c.b > 0.001f) ? (double)c.r / (double)c.b : 100.0;
    double ratio_hi = (hi.b > 0.001f) ? (double)hi.r / (double)hi.b : 100.0;
    TEST_ASSERT_TRUE(ratio_low > ratio_hi);
}

/* =========================================================
 *  Sunset color (3 tests)
 * ========================================================= */

/* 19. Sunset color at 0 degrees: warm tones (r > b) */
void test_sunset_color_warm(void)
{
    atmo_color_t c = earth_atmo_sunset_color(0.0);
    TEST_ASSERT_TRUE(c.r > c.b);
    TEST_ASSERT_TRUE(c.r > 0.3f);
}

/* 20. Sunset color at -5 (below horizon): twilight purple */
void test_sunset_color_twilight(void)
{
    atmo_color_t c = earth_atmo_sunset_color(-5.0);
    /* Twilight should have some blue/purple component */
    TEST_ASSERT_TRUE(c.b > 0.0f);
}

/* 21. Sunset color components in [0,1] */
void test_sunset_color_range(void)
{
    atmo_color_t c = earth_atmo_sunset_color(3.0);
    TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
    TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
    TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
}

/* =========================================================
 *  Limb glow (3 tests)
 * ========================================================= */

/* 22. Edge-on (dot=0) => max glow */
void test_limb_glow_edge(void)
{
    float g = earth_atmo_limb_glow(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, g);
}

/* 23. Face-on (dot=1) => no glow */
void test_limb_glow_face(void)
{
    float g = earth_atmo_limb_glow(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, g);
}

/* 24. Mid angle (dot=0.5) => partial glow */
void test_limb_glow_mid(void)
{
    float g = earth_atmo_limb_glow(0.5f);
    TEST_ASSERT_TRUE(g > 0.0f && g < 1.0f);
}

/* =========================================================
 *  Cloud bands (6 tests)
 * ========================================================= */

/* 25. Cloud band count is 7 */
void test_cloud_band_count(void)
{
    TEST_ASSERT_EQUAL_INT(EARTH_CLOUD_BAND_COUNT, earth_cloud_band_count());
}

/* 26. ITCZ band is first, centered near equator */
void test_cloud_band_itcz(void)
{
    cloud_band_t b = earth_cloud_band_get(0);
    TEST_ASSERT_TRUE(b.lat_center >= 0.0f && b.lat_center <= 15.0f);
    TEST_ASSERT_TRUE(b.density > 0.5f);
    TEST_ASSERT_NOT_NULL(b.name);
}

/* 27. Subtropical highs are clear (low density) */
void test_cloud_band_subtropical_clear(void)
{
    cloud_band_t n = earth_cloud_band_get(1);
    cloud_band_t s = earth_cloud_band_get(2);
    TEST_ASSERT_TRUE(n.density < 0.4f);
    TEST_ASSERT_TRUE(s.density < 0.4f);
}

/* 28. Storm tracks have moderate density */
void test_cloud_band_storm_tracks(void)
{
    cloud_band_t n = earth_cloud_band_get(3);
    cloud_band_t s = earth_cloud_band_get(4);
    TEST_ASSERT_TRUE(n.density >= 0.5f);
    TEST_ASSERT_TRUE(s.density >= 0.5f);
    /* Southern storm track stronger */
    TEST_ASSERT_TRUE(s.density >= n.density);
}

/* 29. Out-of-range band returns zero density */
void test_cloud_band_out_of_range(void)
{
    cloud_band_t b = earth_cloud_band_get(-1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, b.density);
    b = earth_cloud_band_get(99);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, b.density);
}

/* 30. Each band has a non-NULL name */
void test_cloud_band_names(void)
{
    for (int i = 0; i < EARTH_CLOUD_BAND_COUNT; i++) {
        cloud_band_t b = earth_cloud_band_get(i);
        TEST_ASSERT_NOT_NULL(b.name);
    }
}

/* =========================================================
 *  Cloud density at latitude (3 tests)
 * ========================================================= */

/* 31. Equatorial latitude has ITCZ cloud density */
void test_cloud_density_equator(void)
{
    TEST_ASSERT_TRUE(earth_cloud_density_at(5.0) > 0.3);
}

/* 32. Subtropical high (~30N) has low density */
void test_cloud_density_subtropical(void)
{
    TEST_ASSERT_TRUE(earth_cloud_density_at(30.0) < 0.5);
}

/* 33. Cloud density always in [0,1] */
void test_cloud_density_range(void)
{
    for (int lat = -90; lat <= 90; lat += 10) {
        double d = earth_cloud_density_at((double)lat);
        TEST_ASSERT_TRUE(d >= 0.0);
        TEST_ASSERT_TRUE(d <= 1.0);
    }
}

/* =========================================================
 *  ITCZ seasonal migration (3 tests)
 * ========================================================= */

/* 34. January ITCZ offset is negative (further south) */
void test_itcz_january(void)
{
    double offset = earth_cloud_itcz_offset(1);
    TEST_ASSERT_TRUE(offset >= -6.0 && offset <= 1.0);
}

/* 35. July ITCZ migrates north (positive offset > january) */
void test_itcz_july(void)
{
    double jan_off = earth_cloud_itcz_offset(1);
    double jul_off = earth_cloud_itcz_offset(7);
    TEST_ASSERT_TRUE(jul_off > jan_off);
}

/* 36. Invalid month clamped */
void test_itcz_invalid_month(void)
{
    double d0 = earth_cloud_itcz_offset(0);
    double d1 = earth_cloud_itcz_offset(1);
    /* month 0 should clamp to month 1 */
    ASSERT_NEAR_D(d1, d0, 0.001);

    double d13 = earth_cloud_itcz_offset(13);
    double d12 = earth_cloud_itcz_offset(12);
    ASSERT_NEAR_D(d12, d13, 0.001);
}

/* =========================================================
 *  Atmosphere shell mesh (5 tests)
 * ========================================================= */

/* 37. Shell vertex count */
void test_shell_vertex_count(void)
{
    TEST_ASSERT_EQUAL_INT(EARTH_ATMO_MAX_VERTICES, earth_atmo_shell_vertex_count());
}

/* 38. Shell index count */
void test_shell_index_count(void)
{
    TEST_ASSERT_EQUAL_INT(EARTH_ATMO_MAX_INDICES, earth_atmo_shell_index_count());
}

/* 39. Generate mesh returns correct counts */
void test_shell_generate_counts(void)
{
    atmo_vertex_t verts[EARTH_ATMO_MAX_VERTICES];
    unsigned int idxs[EARTH_ATMO_MAX_INDICES];
    atmo_mesh_info_t info = earth_atmo_shell_generate(EARTH_ATMO_RADIUS_RATIO,
                                                       verts, idxs);
    TEST_ASSERT_EQUAL_INT(EARTH_ATMO_MAX_VERTICES, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(EARTH_ATMO_MAX_INDICES, info.index_count);
}

/* 40. Generated vertices are on the atmosphere shell radius */
void test_shell_generate_radius(void)
{
    atmo_vertex_t verts[EARTH_ATMO_MAX_VERTICES];
    unsigned int idxs[EARTH_ATMO_MAX_INDICES];
    earth_atmo_shell_generate(EARTH_ATMO_RADIUS_RATIO, verts, idxs);

    for (int i = 0; i < 10; i++) {
        float r = sqrtf(verts[i].x * verts[i].x +
                         verts[i].y * verts[i].y +
                         verts[i].z * verts[i].z);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)EARTH_ATMO_RADIUS_RATIO, r);
    }
}

/* 41. Normals are unit length */
void test_shell_normals_unit(void)
{
    atmo_vertex_t verts[EARTH_ATMO_MAX_VERTICES];
    unsigned int idxs[EARTH_ATMO_MAX_INDICES];
    earth_atmo_shell_generate(EARTH_ATMO_RADIUS_RATIO, verts, idxs);

    for (int i = 0; i < 20; i++) {
        float len = sqrtf(verts[i].nx * verts[i].nx +
                           verts[i].ny * verts[i].ny +
                           verts[i].nz * verts[i].nz);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, len);
    }
}

/* =========================================================
 *  Constants sanity checks (2 tests)
 * ========================================================= */

/* 42. Scale height constant */
void test_constant_scale_height(void)
{
    ASSERT_NEAR_D(8.5, EARTH_ATMO_SCALE_HEIGHT_KM, 0.001);
}

/* 43. Radius ratio consistent with Earth radius + 100km */
void test_constant_radius_ratio(void)
{
    double expected = (EARTH_RADIUS_KM + EARTH_ATMO_THICKNESS_KM) / EARTH_RADIUS_KM;
    ASSERT_NEAR_D(expected, EARTH_ATMO_RADIUS_RATIO, 0.001);
}

/* =========================================================
 *  Edge cases (2 tests)
 * ========================================================= */

/* 44. Density at exactly 0 altitude */
void test_density_exactly_zero(void)
{
    ASSERT_NEAR_D(1.0, earth_atmo_density(0.0), 0.001);
}

/* 45. Shell indices are within bounds */
void test_shell_indices_bounds(void)
{
    atmo_vertex_t verts[EARTH_ATMO_MAX_VERTICES];
    unsigned int idxs[EARTH_ATMO_MAX_INDICES];
    atmo_mesh_info_t info = earth_atmo_shell_generate(EARTH_ATMO_RADIUS_RATIO,
                                                       verts, idxs);
    for (int i = 0; i < info.index_count; i++) {
        TEST_ASSERT_TRUE(idxs[i] < (unsigned int)info.vertex_count);
    }
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Density */
    RUN_TEST(test_density_sea_level);
    RUN_TEST(test_density_one_scale_height);
    RUN_TEST(test_density_50km);
    RUN_TEST(test_density_100km);
    RUN_TEST(test_density_negative_altitude);

    /* Air mass */
    RUN_TEST(test_air_mass_zenith);
    RUN_TEST(test_air_mass_30deg);
    RUN_TEST(test_air_mass_horizon);
    RUN_TEST(test_air_mass_45deg);
    RUN_TEST(test_air_mass_zero_elevation);

    /* Optical depth */
    RUN_TEST(test_optical_depth_zenith);
    RUN_TEST(test_optical_depth_60deg);
    RUN_TEST(test_optical_depth_85deg);

    /* Sky color */
    RUN_TEST(test_sky_color_overhead_blue);
    RUN_TEST(test_sky_color_high_altitude_dark);
    RUN_TEST(test_sky_color_night_dim);
    RUN_TEST(test_sky_color_range);
    RUN_TEST(test_sky_color_low_sun_warm);

    /* Sunset color */
    RUN_TEST(test_sunset_color_warm);
    RUN_TEST(test_sunset_color_twilight);
    RUN_TEST(test_sunset_color_range);

    /* Limb glow */
    RUN_TEST(test_limb_glow_edge);
    RUN_TEST(test_limb_glow_face);
    RUN_TEST(test_limb_glow_mid);

    /* Cloud bands */
    RUN_TEST(test_cloud_band_count);
    RUN_TEST(test_cloud_band_itcz);
    RUN_TEST(test_cloud_band_subtropical_clear);
    RUN_TEST(test_cloud_band_storm_tracks);
    RUN_TEST(test_cloud_band_out_of_range);
    RUN_TEST(test_cloud_band_names);

    /* Cloud density at latitude */
    RUN_TEST(test_cloud_density_equator);
    RUN_TEST(test_cloud_density_subtropical);
    RUN_TEST(test_cloud_density_range);

    /* ITCZ migration */
    RUN_TEST(test_itcz_january);
    RUN_TEST(test_itcz_july);
    RUN_TEST(test_itcz_invalid_month);

    /* Atmosphere shell */
    RUN_TEST(test_shell_vertex_count);
    RUN_TEST(test_shell_index_count);
    RUN_TEST(test_shell_generate_counts);
    RUN_TEST(test_shell_generate_radius);
    RUN_TEST(test_shell_normals_unit);

    /* Constants */
    RUN_TEST(test_constant_scale_height);
    RUN_TEST(test_constant_radius_ratio);

    /* Edge cases */
    RUN_TEST(test_density_exactly_zero);
    RUN_TEST(test_shell_indices_bounds);

    return UNITY_END();
}
