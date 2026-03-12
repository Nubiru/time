/*
 * test_horizon_geometry.c -- Tests for horizon line geometry module.
 *
 * Covers: config, zenith, pack, cardinals, altaz, angular distance,
 * visibility, shaders, edge cases. 55 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/horizon_geometry.h"

#include <math.h>
#include <string.h>

#define FTOL 0.01f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* Helper: vector length */
static float vec_len(float x, float y, float z)
{
    return sqrtf(x * x + y * y + z * z);
}

/* Helper: dot product */
static float vec_dot(const float a[3], const float b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/* ======================================================================
 * hzg_default_config
 * ====================================================================== */

/* 1. Default config has positive globe_radius */
void test_default_config_globe_radius(void)
{
    hzg_config_t cfg = hzg_default_config();
    TEST_ASSERT_TRUE(cfg.globe_radius > 0.0f);
}

/* 2. Default config observer at lat/lon 0,0 */
void test_default_config_observer_location(void)
{
    hzg_config_t cfg = hzg_default_config();
    FCLOSE(0.0f, (float)cfg.observer_lat_deg);
    FCLOSE(0.0f, (float)cfg.observer_lon_deg);
}

/* 3. Default config horizon_segments > 0 */
void test_default_config_horizon_segments(void)
{
    hzg_config_t cfg = hzg_default_config();
    TEST_ASSERT_TRUE(cfg.horizon_segments > 0);
}

/* 4. Default config altitude_steps > 0 */
void test_default_config_altitude_steps(void)
{
    hzg_config_t cfg = hzg_default_config();
    TEST_ASSERT_TRUE(cfg.altitude_steps > 0);
}

/* 5. Default config azimuth_lines > 0 */
void test_default_config_azimuth_lines(void)
{
    hzg_config_t cfg = hzg_default_config();
    TEST_ASSERT_TRUE(cfg.azimuth_lines > 0);
}

/* ======================================================================
 * hzg_zenith — must match earth_latlon_to_xyz
 * ====================================================================== */

/* 6. Zenith at equator/prime meridian: (1, 0, 0) */
void test_zenith_equator_prime_meridian(void)
{
    float xyz[3];
    hzg_zenith(0.0, 0.0, xyz);
    FCLOSE(1.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 7. Zenith at north pole: (0, 1, 0) */
void test_zenith_north_pole(void)
{
    float xyz[3];
    hzg_zenith(90.0, 0.0, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(1.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 8. Zenith at south pole: (0, -1, 0) */
void test_zenith_south_pole(void)
{
    float xyz[3];
    hzg_zenith(-90.0, 0.0, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(-1.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 9. Zenith is always unit length */
void test_zenith_unit_length(void)
{
    float xyz[3];
    double lats[] = {0, 45, -45, 90, -90, 33.5, -72.1};
    double lons[] = {0, 90, -90, 180, -180, 123.4, -45.6};
    for (int i = 0; i < 7; i++) {
        hzg_zenith(lats[i], lons[i], xyz);
        float len = vec_len(xyz[0], xyz[1], xyz[2]);
        FCLOSE(1.0f, len);
    }
}

/* 10. Zenith at (0, 90) points along +Z (matching earth_globe convention) */
void test_zenith_equator_90_east(void)
{
    float xyz[3];
    hzg_zenith(0.0, 90.0, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(1.0f, xyz[2]);
}

/* 11. Zenith at (0, -90) points along -Z */
void test_zenith_equator_90_west(void)
{
    float xyz[3];
    hzg_zenith(0.0, -90.0, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(-1.0f, xyz[2]);
}

/* 12. Zenith at (45, 0) — cos45 * cos0 = 0.7071, sin45 = 0.7071 */
void test_zenith_45_lat(void)
{
    float xyz[3];
    hzg_zenith(45.0, 0.0, xyz);
    float c45 = (float)cos(45.0 * PI / 180.0);
    float s45 = (float)sin(45.0 * PI / 180.0);
    FCLOSE(c45, xyz[0]);
    FCLOSE(s45, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* ======================================================================
 * hzg_angular_distance — Haversine
 * ====================================================================== */

/* 13. Same point = 0 degrees */
void test_angular_distance_same_point(void)
{
    float d = hzg_angular_distance(40.0, -74.0, 40.0, -74.0);
    FCLOSE(0.0f, d);
}

/* 14. Antipodal points = 180 degrees */
void test_angular_distance_antipodal(void)
{
    float d = hzg_angular_distance(0.0, 0.0, 0.0, 180.0);
    FCLOSE(180.0f, d);
}

/* 15. North pole to south pole = 180 degrees */
void test_angular_distance_poles(void)
{
    float d = hzg_angular_distance(90.0, 0.0, -90.0, 0.0);
    FCLOSE(180.0f, d);
}

/* 16. Equator, 90 degrees apart */
void test_angular_distance_quarter_circle(void)
{
    float d = hzg_angular_distance(0.0, 0.0, 0.0, 90.0);
    FCLOSE(90.0f, d);
}

/* 17. Known distance: New York to London ~ 50.1 degrees */
void test_angular_distance_nyc_london(void)
{
    /* NYC: 40.7128 N, 74.0060 W. London: 51.5074 N, 0.1278 W */
    float d = hzg_angular_distance(40.7128, -74.006, 51.5074, -0.1278);
    /* Great circle distance ~50.1 degrees (5570 km / 111.19 km per degree) */
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 50.1f, d);
}

/* 18. Distance is symmetric */
void test_angular_distance_symmetric(void)
{
    float d1 = hzg_angular_distance(30.0, 45.0, -20.0, 120.0);
    float d2 = hzg_angular_distance(-20.0, 120.0, 30.0, 45.0);
    FCLOSE(d1, d2);
}

/* 19. Distance is non-negative */
void test_angular_distance_non_negative(void)
{
    float d = hzg_angular_distance(-45.0, -90.0, 45.0, 90.0);
    TEST_ASSERT_TRUE(d >= 0.0f);
}

/* ======================================================================
 * hzg_is_visible
 * ====================================================================== */

/* 20. Point directly above observer is visible */
void test_is_visible_directly_above(void)
{
    TEST_ASSERT_EQUAL_INT(1, hzg_is_visible(0.0, 0.0, 0.0, 0.0));
}

/* 21. Antipodal point is not visible */
void test_is_visible_antipodal(void)
{
    TEST_ASSERT_EQUAL_INT(0, hzg_is_visible(0.0, 0.0, 0.0, 180.0));
}

/* 22. Point exactly 89 degrees away is visible */
void test_is_visible_just_under_horizon(void)
{
    TEST_ASSERT_EQUAL_INT(1, hzg_is_visible(0.0, 0.0, 0.0, 89.0));
}

/* 23. Point exactly 91 degrees away is not visible */
void test_is_visible_just_over_horizon(void)
{
    TEST_ASSERT_EQUAL_INT(0, hzg_is_visible(0.0, 0.0, 0.0, 91.0));
}

/* 24. Nearby point is visible */
void test_is_visible_nearby(void)
{
    TEST_ASSERT_EQUAL_INT(1, hzg_is_visible(40.0, -74.0, 42.0, -71.0));
}

/* ======================================================================
 * hzg_altaz_to_xyz
 * ====================================================================== */

/* 25. Altitude 90 (zenith) should match zenith direction scaled by radius */
void test_altaz_zenith(void)
{
    float xyz[3], zen[3];
    hzg_zenith(0.0, 0.0, zen);
    hzg_altaz_to_xyz(0.0, 0.0, 2.0f, 90.0, 0.0, xyz);
    FCLOSE(zen[0] * 2.0f, xyz[0]);
    FCLOSE(zen[1] * 2.0f, xyz[1]);
    FCLOSE(zen[2] * 2.0f, xyz[2]);
}

/* 26. Altitude 0 should be on the horizon (perpendicular to zenith) */
void test_altaz_horizon_perpendicular(void)
{
    float xyz[3], zen[3];
    hzg_zenith(30.0, 45.0, zen);
    hzg_altaz_to_xyz(30.0, 45.0, 1.0f, 0.0, 0.0, xyz);
    /* dot(xyz, zenith) should be ~0 (perpendicular) */
    float dot = vec_dot(xyz, zen);
    FCLOSE(0.0f, dot);
}

/* 27. Any altaz point should be at globe_radius distance from origin */
void test_altaz_on_sphere(void)
{
    float xyz[3];
    hzg_altaz_to_xyz(40.0, -74.0, 3.0f, 30.0, 120.0, xyz);
    float len = vec_len(xyz[0], xyz[1], xyz[2]);
    FCLOSE(3.0f, len);
}

/* 28. Altitude 0, azimuth 0 (north) should point towards geographic north */
void test_altaz_north_direction(void)
{
    /* At equator (lat=0, lon=0), north is +Y direction */
    float xyz[3];
    hzg_altaz_to_xyz(0.0, 0.0, 1.0f, 0.0, 0.0, xyz);
    /* The point should have positive Y component (towards north pole) */
    TEST_ASSERT_TRUE(xyz[1] > 0.0f);
}

/* 29. Altitude 0, azimuth 180 (south) should point towards geographic south */
void test_altaz_south_direction(void)
{
    /* At equator (lat=0, lon=0), south is -Y direction */
    float xyz[3];
    hzg_altaz_to_xyz(0.0, 0.0, 1.0f, 0.0, 180.0, xyz);
    /* The point should have negative Y component (towards south pole) */
    TEST_ASSERT_TRUE(xyz[1] < 0.0f);
}

/* 30. Different azimuths at same altitude produce different points */
void test_altaz_different_azimuths(void)
{
    float xyz1[3], xyz2[3];
    hzg_altaz_to_xyz(30.0, 60.0, 1.0f, 45.0, 0.0, xyz1);
    hzg_altaz_to_xyz(30.0, 60.0, 1.0f, 45.0, 90.0, xyz2);
    int differs = (fabsf(xyz1[0] - xyz2[0]) > 0.01f ||
                   fabsf(xyz1[1] - xyz2[1]) > 0.01f ||
                   fabsf(xyz1[2] - xyz2[2]) > 0.01f);
    TEST_ASSERT_TRUE(differs);
}

/* 31. Altitude 0, azimuth 90 (east) at equator should point along +Z */
void test_altaz_east_at_equator(void)
{
    float xyz[3];
    hzg_altaz_to_xyz(0.0, 0.0, 1.0f, 0.0, 90.0, xyz);
    /* At equator (0,0), east is roughly +Z */
    TEST_ASSERT_TRUE(xyz[2] > 0.5f);
}

/* ======================================================================
 * hzg_cardinals
 * ====================================================================== */

/* 32. Always returns 8 cardinal/intercardinal points */
void test_cardinals_count(void)
{
    hzg_config_t cfg = hzg_default_config();
    hzg_cardinal_t pts[8];
    int n = hzg_cardinals(cfg, pts);
    TEST_ASSERT_EQUAL_INT(8, n);
}

/* 33. Cardinal azimuths are 0, 45, 90, 135, 180, 225, 270, 315 */
void test_cardinals_azimuths(void)
{
    hzg_config_t cfg = hzg_default_config();
    hzg_cardinal_t pts[8];
    hzg_cardinals(cfg, pts);
    float expected[] = {0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f};
    for (int i = 0; i < 8; i++) {
        FCLOSE(expected[i], pts[i].azimuth_deg);
    }
}

/* 34. Cardinal labels are correct */
void test_cardinals_labels(void)
{
    hzg_config_t cfg = hzg_default_config();
    hzg_cardinal_t pts[8];
    hzg_cardinals(cfg, pts);
    TEST_ASSERT_EQUAL_STRING("N", pts[0].label);
    TEST_ASSERT_EQUAL_STRING("NE", pts[1].label);
    TEST_ASSERT_EQUAL_STRING("E", pts[2].label);
    TEST_ASSERT_EQUAL_STRING("SE", pts[3].label);
    TEST_ASSERT_EQUAL_STRING("S", pts[4].label);
    TEST_ASSERT_EQUAL_STRING("SW", pts[5].label);
    TEST_ASSERT_EQUAL_STRING("W", pts[6].label);
    TEST_ASSERT_EQUAL_STRING("NW", pts[7].label);
}

/* 35. Cardinal points are on the sphere at globe_radius */
void test_cardinals_on_sphere(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.globe_radius = 5.0f;
    hzg_cardinal_t pts[8];
    hzg_cardinals(cfg, pts);
    for (int i = 0; i < 8; i++) {
        float len = vec_len(pts[i].x, pts[i].y, pts[i].z);
        FCLOSE(5.0f, len);
    }
}

/* 36. Cardinal points are perpendicular to zenith (on horizon) */
void test_cardinals_perpendicular_to_zenith(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.observer_lat_deg = 30.0;
    cfg.observer_lon_deg = 60.0;
    cfg.globe_radius = 1.0f;
    hzg_cardinal_t pts[8];
    hzg_cardinals(cfg, pts);

    float zen[3];
    hzg_zenith(30.0, 60.0, zen);

    for (int i = 0; i < 8; i++) {
        float pt[3] = {pts[i].x, pts[i].y, pts[i].z};
        float dot = vec_dot(pt, zen);
        FCLOSE(0.0f, dot);
    }
}

/* 37. N cardinal at north pole: should still produce valid points */
void test_cardinals_at_north_pole(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.observer_lat_deg = 90.0;
    cfg.observer_lon_deg = 0.0;
    hzg_cardinal_t pts[8];
    int n = hzg_cardinals(cfg, pts);
    TEST_ASSERT_EQUAL_INT(8, n);
    /* All points should be on the horizon (equator for north pole) */
    for (int i = 0; i < 8; i++) {
        float len = vec_len(pts[i].x, pts[i].y, pts[i].z);
        FCLOSE(cfg.globe_radius, len);
    }
}

/* ======================================================================
 * hzg_pack
 * ====================================================================== */

/* 38. Pack returns positive total vertex count */
void test_pack_total_verts_positive(void)
{
    hzg_config_t cfg = hzg_default_config();
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);
    TEST_ASSERT_TRUE(info.total_verts > 0);
}

/* 39. Pack horizon verts matches config segments + 1 (closed loop) */
void test_pack_horizon_verts(void)
{
    hzg_config_t cfg = hzg_default_config();
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);
    /* Horizon is a closed loop: segments + 1 vertices */
    TEST_ASSERT_EQUAL_INT(cfg.horizon_segments + 1, info.horizon_verts);
}

/* 40. Pack total_verts equals sum of components */
void test_pack_total_verts_sum(void)
{
    hzg_config_t cfg = hzg_default_config();
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);
    int sum = info.horizon_verts + info.altitude_verts + info.azimuth_verts;
    TEST_ASSERT_EQUAL_INT(sum, info.total_verts);
}

/* 41. Pack cardinal count is 8 */
void test_pack_cardinal_count(void)
{
    hzg_config_t cfg = hzg_default_config();
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);
    TEST_ASSERT_EQUAL_INT(8, info.cardinal_count);
}

/* 42. Pack horizon vertices are on the sphere at globe_radius */
void test_pack_horizon_on_sphere(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.globe_radius = 4.0f;
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);

    for (int i = 0; i < info.horizon_verts; i++) {
        int base = i * HZG_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float len = vec_len(x, y, z);
        FCLOSE(4.0f, len);
    }
}

/* 43. Pack horizon vertices are perpendicular to zenith */
void test_pack_horizon_perpendicular_to_zenith(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.observer_lat_deg = 45.0;
    cfg.observer_lon_deg = -30.0;
    cfg.globe_radius = 1.0f;
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);

    float zen[3];
    hzg_zenith(45.0, -30.0, zen);

    for (int i = 0; i < info.horizon_verts; i++) {
        int base = i * HZG_VERTEX_FLOATS;
        float pt[3] = {verts[base], verts[base + 1], verts[base + 2]};
        float dot = vec_dot(pt, zen);
        FCLOSE(0.0f, dot);
    }
}

/* 44. Pack vertex colors are in valid range [0, 1] */
void test_pack_colors_valid_range(void)
{
    hzg_config_t cfg = hzg_default_config();
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);

    for (int i = 0; i < info.total_verts; i++) {
        int base = i * HZG_VERTEX_FLOATS;
        float r = verts[base + 3];
        float g = verts[base + 4];
        float b = verts[base + 5];
        float a = verts[base + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

/* 45. Pack does not exceed HZG_MAX_VERTICES */
void test_pack_within_max_vertices(void)
{
    hzg_config_t cfg = hzg_default_config();
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);
    TEST_ASSERT_TRUE(info.total_verts <= HZG_MAX_VERTICES);
}

/* 46. Pack with different observer locations produces different geometry */
void test_pack_different_observers(void)
{
    hzg_config_t cfg1 = hzg_default_config();
    cfg1.observer_lat_deg = 0.0;
    cfg1.observer_lon_deg = 0.0;

    hzg_config_t cfg2 = hzg_default_config();
    cfg2.observer_lat_deg = 45.0;
    cfg2.observer_lon_deg = 90.0;

    float verts1[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    float verts2[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_pack(cfg1, verts1);
    hzg_pack(cfg2, verts2);

    int differs = 0;
    for (int i = 0; i < 50; i++) {
        if (fabsf(verts1[i] - verts2[i]) > 0.01f) {
            differs = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differs);
}

/* 47. Pack altitude grid vertices are also on the sphere */
void test_pack_altitude_grid_on_sphere(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.globe_radius = 2.0f;
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);

    /* Altitude verts start after horizon verts */
    int offset = info.horizon_verts;
    for (int i = 0; i < info.altitude_verts; i++) {
        int base = (offset + i) * HZG_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float len = vec_len(x, y, z);
        FCLOSE(2.0f, len);
    }
}

/* 48. Pack with radius=0 produces zero-distance vertices */
void test_pack_radius_zero(void)
{
    hzg_config_t cfg = hzg_default_config();
    cfg.globe_radius = 0.0f;
    float verts[HZG_MAX_VERTICES * HZG_VERTEX_FLOATS];
    hzg_info_t info = hzg_pack(cfg, verts);

    TEST_ASSERT_TRUE(info.total_verts > 0);
    for (int i = 0; i < info.total_verts && i < 20; i++) {
        int base = i * HZG_VERTEX_FLOATS;
        float len = vec_len(verts[base], verts[base + 1], verts[base + 2]);
        FCLOSE(0.0f, len);
    }
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 49. Vertex shader source is non-null and non-empty */
void test_vert_source_exists(void)
{
    const char *src = hzg_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 50. Fragment shader source is non-null and non-empty */
void test_frag_source_exists(void)
{
    const char *src = hzg_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 51. Shaders contain version 300 es */
void test_shaders_version_300_es(void)
{
    TEST_ASSERT_NOT_NULL(strstr(hzg_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(hzg_frag_source(), "#version 300 es"));
}

/* 52. Vertex shader contains gl_Position */
void test_vert_shader_has_gl_position(void)
{
    TEST_ASSERT_NOT_NULL(strstr(hzg_vert_source(), "gl_Position"));
}

/* 53. Shaders contain void main() */
void test_shaders_have_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(hzg_vert_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(hzg_frag_source(), "void main()"));
}

/* 54. Vertex shader has u_mvp uniform */
void test_vert_shader_has_mvp(void)
{
    TEST_ASSERT_NOT_NULL(strstr(hzg_vert_source(), "u_mvp"));
}

/* 55. Vertex shader has a_position attribute */
void test_vert_shader_has_position_attrib(void)
{
    TEST_ASSERT_NOT_NULL(strstr(hzg_vert_source(), "a_position"));
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* hzg_default_config */
    RUN_TEST(test_default_config_globe_radius);
    RUN_TEST(test_default_config_observer_location);
    RUN_TEST(test_default_config_horizon_segments);
    RUN_TEST(test_default_config_altitude_steps);
    RUN_TEST(test_default_config_azimuth_lines);

    /* hzg_zenith */
    RUN_TEST(test_zenith_equator_prime_meridian);
    RUN_TEST(test_zenith_north_pole);
    RUN_TEST(test_zenith_south_pole);
    RUN_TEST(test_zenith_unit_length);
    RUN_TEST(test_zenith_equator_90_east);
    RUN_TEST(test_zenith_equator_90_west);
    RUN_TEST(test_zenith_45_lat);

    /* hzg_angular_distance */
    RUN_TEST(test_angular_distance_same_point);
    RUN_TEST(test_angular_distance_antipodal);
    RUN_TEST(test_angular_distance_poles);
    RUN_TEST(test_angular_distance_quarter_circle);
    RUN_TEST(test_angular_distance_nyc_london);
    RUN_TEST(test_angular_distance_symmetric);
    RUN_TEST(test_angular_distance_non_negative);

    /* hzg_is_visible */
    RUN_TEST(test_is_visible_directly_above);
    RUN_TEST(test_is_visible_antipodal);
    RUN_TEST(test_is_visible_just_under_horizon);
    RUN_TEST(test_is_visible_just_over_horizon);
    RUN_TEST(test_is_visible_nearby);

    /* hzg_altaz_to_xyz */
    RUN_TEST(test_altaz_zenith);
    RUN_TEST(test_altaz_horizon_perpendicular);
    RUN_TEST(test_altaz_on_sphere);
    RUN_TEST(test_altaz_north_direction);
    RUN_TEST(test_altaz_south_direction);
    RUN_TEST(test_altaz_different_azimuths);
    RUN_TEST(test_altaz_east_at_equator);

    /* hzg_cardinals */
    RUN_TEST(test_cardinals_count);
    RUN_TEST(test_cardinals_azimuths);
    RUN_TEST(test_cardinals_labels);
    RUN_TEST(test_cardinals_on_sphere);
    RUN_TEST(test_cardinals_perpendicular_to_zenith);
    RUN_TEST(test_cardinals_at_north_pole);

    /* hzg_pack */
    RUN_TEST(test_pack_total_verts_positive);
    RUN_TEST(test_pack_horizon_verts);
    RUN_TEST(test_pack_total_verts_sum);
    RUN_TEST(test_pack_cardinal_count);
    RUN_TEST(test_pack_horizon_on_sphere);
    RUN_TEST(test_pack_horizon_perpendicular_to_zenith);
    RUN_TEST(test_pack_colors_valid_range);
    RUN_TEST(test_pack_within_max_vertices);
    RUN_TEST(test_pack_different_observers);
    RUN_TEST(test_pack_altitude_grid_on_sphere);
    RUN_TEST(test_pack_radius_zero);

    /* Shaders */
    RUN_TEST(test_vert_source_exists);
    RUN_TEST(test_frag_source_exists);
    RUN_TEST(test_shaders_version_300_es);
    RUN_TEST(test_vert_shader_has_gl_position);
    RUN_TEST(test_shaders_have_main);
    RUN_TEST(test_vert_shader_has_mvp);
    RUN_TEST(test_vert_shader_has_position_attrib);

    return UNITY_END();
}
