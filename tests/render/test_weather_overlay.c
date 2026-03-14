/*
 * test_weather_overlay.c -- Tests for weather overlay geometry module.
 *
 * Covers: config, latlon_to_xyz, arrow_glyph, wind arrows, basin outlines,
 * ITCZ band, isobars, full pack, shaders, edge cases. 62 tests.
 */

#include "../unity/unity.h"
#include "../../src/render/weather_overlay.h"

#include <math.h>
#include <string.h>

#define FTOL 0.02f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* Helper: vector length */
static float vec_len(float x, float y, float z)
{
    return sqrtf(x * x + y * y + z * z);
}

/* ======================================================================
 * wov_default_config
 * ====================================================================== */

/* 1. Default config has positive globe_radius */
void test_default_config_globe_radius(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.globe_radius > 0.0f);
}

/* 2. Default config month in valid range */
void test_default_config_month(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.month >= 1 && cfg.month <= 12);
}

/* 3. Default config wind_arrows_per_band > 0 */
void test_default_config_arrows_per_band(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.wind_arrows_per_band > 0);
}

/* 4. Default config arrow_scale > 0 */
void test_default_config_arrow_scale(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.arrow_scale > 0.0f);
}

/* 5. Default config arrow_alpha in (0, 1] */
void test_default_config_arrow_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.arrow_alpha > 0.0f && cfg.arrow_alpha <= 1.0f);
}

/* 6. Default config basin_alpha in (0, 1] */
void test_default_config_basin_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.basin_alpha > 0.0f && cfg.basin_alpha <= 1.0f);
}

/* 7. Default config itcz_alpha in (0, 1] */
void test_default_config_itcz_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.itcz_alpha > 0.0f && cfg.itcz_alpha <= 1.0f);
}

/* 8. Default config isobar_count > 0 */
void test_default_config_isobar_count(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.isobar_count > 0);
}

/* 9. Default config isobar_alpha in (0, 1] */
void test_default_config_isobar_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    TEST_ASSERT_TRUE(cfg.isobar_alpha > 0.0f && cfg.isobar_alpha <= 1.0f);
}

/* ======================================================================
 * wov_latlon_to_xyz — must match earth_globe.h convention
 * ====================================================================== */

/* 10. Equator, prime meridian: (1, 0, 0) */
void test_latlon_equator_prime(void)
{
    float xyz[3];
    wov_latlon_to_xyz(0.0, 0.0, 1.0f, xyz);
    FCLOSE(1.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 11. North pole: (0, 1, 0) */
void test_latlon_north_pole(void)
{
    float xyz[3];
    wov_latlon_to_xyz(90.0, 0.0, 1.0f, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(1.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 12. South pole: (0, -1, 0) */
void test_latlon_south_pole(void)
{
    float xyz[3];
    wov_latlon_to_xyz(-90.0, 0.0, 1.0f, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(-1.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 13. Equator, 90E: (0, 0, 1) */
void test_latlon_equator_90e(void)
{
    float xyz[3];
    wov_latlon_to_xyz(0.0, 90.0, 1.0f, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(1.0f, xyz[2]);
}

/* 14. Equator, 180: (-1, 0, 0) */
void test_latlon_equator_180(void)
{
    float xyz[3];
    wov_latlon_to_xyz(0.0, 180.0, 1.0f, xyz);
    FCLOSE(-1.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 15. Equator, 270E (= -90): (0, 0, -1) */
void test_latlon_equator_270(void)
{
    float xyz[3];
    wov_latlon_to_xyz(0.0, -90.0, 1.0f, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(-1.0f, xyz[2]);
}

/* 16. Radius scaling */
void test_latlon_radius_scaling(void)
{
    float xyz[3];
    wov_latlon_to_xyz(0.0, 0.0, 2.5f, xyz);
    FCLOSE(2.5f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 17. Result lies on sphere of given radius */
void test_latlon_on_sphere(void)
{
    float xyz[3];
    wov_latlon_to_xyz(45.0, 60.0, 1.0f, xyz);
    float len = vec_len(xyz[0], xyz[1], xyz[2]);
    FCLOSE(1.0f, len);
}

/* 18. Match earth_globe: Y = sin(lat) */
void test_latlon_y_equals_sin_lat(void)
{
    float xyz[3];
    wov_latlon_to_xyz(30.0, 45.0, 1.0f, xyz);
    double expected_y = sin(30.0 * PI / 180.0);
    FCLOSE((float)expected_y, xyz[1]);
}

/* ======================================================================
 * wov_arrow_glyph
 * ====================================================================== */

/* 19. Arrow glyph returns vertices > 0 */
void test_arrow_glyph_returns_verts(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[WOV_ARROW_SEGMENTS * 2 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 0.0f, 0.7f, 0.05f, color, buf, WOV_ARROW_SEGMENTS * 2);
    TEST_ASSERT_TRUE(n > 0);
}

/* 20. Arrow glyph vertex count is even (GL_LINES pairs) */
void test_arrow_glyph_even_count(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[WOV_ARROW_SEGMENTS * 2 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 45.0f, 0.6f, 0.05f, color, buf, WOV_ARROW_SEGMENTS * 2);
    TEST_ASSERT_EQUAL_INT(0, n % 2);
}

/* 21. Arrow glyph respects max_verts limit */
void test_arrow_glyph_max_verts(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[4 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 0.0f, 0.7f, 0.05f, color, buf, 4);
    TEST_ASSERT_TRUE(n <= 4);
}

/* 22. Arrow glyph vertices contain the correct color */
void test_arrow_glyph_color(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[WOV_ARROW_SEGMENTS * 2 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 0.0f, 0.7f, 0.05f, color, buf, WOV_ARROW_SEGMENTS * 2);
    TEST_ASSERT_TRUE(n > 0);
    /* Check color of first vertex (floats 3,4,5,6) */
    FCLOSE(0.3f, buf[3]);
    FCLOSE(0.5f, buf[4]);
    FCLOSE(0.9f, buf[5]);
    FCLOSE(0.6f, buf[6]);
}

/* 23. Arrow glyph with zero strength produces zero vertices */
void test_arrow_glyph_zero_strength(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[WOV_ARROW_SEGMENTS * 2 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 0.0f, 0.0f, 0.05f, color, buf, WOV_ARROW_SEGMENTS * 2);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* 24. Arrow glyph with zero scale produces zero vertices */
void test_arrow_glyph_zero_scale(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[WOV_ARROW_SEGMENTS * 2 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 0.0f, 0.7f, 0.0f, color, buf, WOV_ARROW_SEGMENTS * 2);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* 25. Arrow glyph vertices near pos on globe surface */
void test_arrow_glyph_near_surface(void)
{
    float pos[3] = {1.0f, 0.0f, 0.0f};
    float color[4] = {0.3f, 0.5f, 0.9f, 0.6f};
    float buf[WOV_ARROW_SEGMENTS * 2 * WOV_VERTEX_FLOATS];
    int n = wov_arrow_glyph(pos, 0.0f, 0.7f, 0.05f, color, buf, WOV_ARROW_SEGMENTS * 2);
    TEST_ASSERT_TRUE(n > 0);
    /* All vertices should be near the globe surface radius ~1.0 */
    for (int i = 0; i < n; i++) {
        float vx = buf[i * WOV_VERTEX_FLOATS + 0];
        float vy = buf[i * WOV_VERTEX_FLOATS + 1];
        float vz = buf[i * WOV_VERTEX_FLOATS + 2];
        float len = vec_len(vx, vy, vz);
        TEST_ASSERT_FLOAT_WITHIN(0.15f, 1.0f, len);
    }
}

/* ======================================================================
 * wov_pack_wind_arrows
 * ====================================================================== */

/* 26. Wind arrows produce vertices */
void test_wind_arrows_produce_verts(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
}

/* 27. Wind arrow vertex count is even (GL_LINES) */
void test_wind_arrows_even_count(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_EQUAL_INT(0, n % 2);
}

/* 28. Wind arrows don't exceed max_verts */
void test_wind_arrows_max_verts(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[100 * WOV_VERTEX_FLOATS];
    int n = wov_pack_wind_arrows(cfg, buf, 100);
    TEST_ASSERT_TRUE(n <= 100);
}

/* 29. Wind arrows scale with arrows_per_band */
void test_wind_arrows_scale_with_count(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    cfg.wind_arrows_per_band = 6;
    int n6 = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    cfg.wind_arrows_per_band = 12;
    int n12 = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n12 > n6);
}

/* 30. Wind arrow vertices lie near sphere of globe_radius */
void test_wind_arrows_on_sphere(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.globe_radius = 1.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    for (int i = 0; i < n && i < 20; i++) {
        float vx = buf[i * WOV_VERTEX_FLOATS + 0];
        float vy = buf[i * WOV_VERTEX_FLOATS + 1];
        float vz = buf[i * WOV_VERTEX_FLOATS + 2];
        float len = vec_len(vx, vy, vz);
        TEST_ASSERT_FLOAT_WITHIN(0.15f, cfg.globe_radius, len);
    }
}

/* 31. Wind arrows with large radius scale positions */
void test_wind_arrows_large_radius(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.globe_radius = 5.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
    float vx = buf[0];
    float vy = buf[1];
    float vz = buf[2];
    float len = vec_len(vx, vy, vz);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 5.0f, len);
}

/* 32. Wind arrows alpha stored in vertex color */
void test_wind_arrows_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.arrow_alpha = 0.7f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_wind_arrows(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
    /* Alpha is at offset 6 in each vertex */
    FCLOSE(0.7f, buf[6]);
}

/* ======================================================================
 * wov_pack_basin_outlines
 * ====================================================================== */

/* 33. Basin outlines produce vertices in hurricane season (September) */
void test_basin_outlines_september(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_basin_outlines(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
}

/* 34. Basin outlines vertex count is even (GL_LINES) */
void test_basin_outlines_even(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_basin_outlines(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_EQUAL_INT(0, n % 2);
}

/* 35. Basin outlines also produced in February (SH season) */
void test_basin_outlines_february_active(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    cfg.month = 2;
    int n_feb = wov_pack_basin_outlines(cfg, buf, WOV_MAX_VERTICES);
    /* SH basins + West Pacific active in Feb */
    TEST_ASSERT_TRUE(n_feb > 0);
}

/* 36. Basin outlines don't exceed max_verts */
void test_basin_outlines_max_verts(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    float buf[50 * WOV_VERTEX_FLOATS];
    int n = wov_pack_basin_outlines(cfg, buf, 50);
    TEST_ASSERT_TRUE(n <= 50);
}

/* 37. Basin outlines with West Pacific active year-round */
void test_basin_outlines_west_pacific_always(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    /* West Pacific season is 1-12, always active */
    cfg.month = 3;
    int n = wov_pack_basin_outlines(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
}

/* 38. Basin outlines vertices near sphere surface */
void test_basin_outlines_on_sphere(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    cfg.globe_radius = 1.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_basin_outlines(cfg, buf, WOV_MAX_VERTICES);
    for (int i = 0; i < n && i < 20; i++) {
        float vx = buf[i * WOV_VERTEX_FLOATS + 0];
        float vy = buf[i * WOV_VERTEX_FLOATS + 1];
        float vz = buf[i * WOV_VERTEX_FLOATS + 2];
        float len = vec_len(vx, vy, vz);
        FCLOSE(1.0f, len);
    }
}

/* ======================================================================
 * wov_pack_itcz
 * ====================================================================== */

/* 39. ITCZ produces vertices */
void test_itcz_produce_verts(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
}

/* 40. ITCZ vertex count is even (GL_LINES) */
void test_itcz_even(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_EQUAL_INT(0, n % 2);
}

/* 41. ITCZ vertices at correct latitude for July (ITCZ ~15 deg N) */
void test_itcz_july_latitude(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 7;
    cfg.globe_radius = 1.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
    /* Y coordinate = sin(lat)*radius. For lat=15 deg, Y = sin(15) ~= 0.259 */
    float expected_y = (float)sin(15.0 * PI / 180.0);
    /* Check first vertex Y is near expected */
    FCLOSE(expected_y, buf[1]);
}

/* 42. ITCZ vertices at correct latitude for January (ITCZ ~5 deg N) */
void test_itcz_january_latitude(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 1;
    cfg.globe_radius = 1.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
    float expected_y = (float)sin(5.0 * PI / 180.0);
    FCLOSE(expected_y, buf[1]);
}

/* 43. ITCZ vertices on sphere */
void test_itcz_on_sphere(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.globe_radius = 1.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, WOV_MAX_VERTICES);
    for (int i = 0; i < n && i < 20; i++) {
        float vx = buf[i * WOV_VERTEX_FLOATS + 0];
        float vy = buf[i * WOV_VERTEX_FLOATS + 1];
        float vz = buf[i * WOV_VERTEX_FLOATS + 2];
        float len = vec_len(vx, vy, vz);
        FCLOSE(1.0f, len);
    }
}

/* 44. ITCZ alpha in color */
void test_itcz_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.itcz_alpha = 0.5f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
    FCLOSE(0.5f, buf[6]);
}

/* 45. ITCZ doesn't exceed max_verts */
void test_itcz_max_verts(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[10 * WOV_VERTEX_FLOATS];
    int n = wov_pack_itcz(cfg, buf, 10);
    TEST_ASSERT_TRUE(n <= 10);
}

/* ======================================================================
 * wov_pack_isobars
 * ====================================================================== */

/* 46. Isobars produce vertices */
void test_isobars_produce_verts(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
}

/* 47. Isobar vertex count is even (GL_LINES) */
void test_isobars_even(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_EQUAL_INT(0, n % 2);
}

/* 48. More isobars produce more vertices */
void test_isobars_scale_with_count(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    cfg.isobar_count = 3;
    int n3 = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    cfg.isobar_count = 6;
    int n6 = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n6 > n3);
}

/* 49. Isobar vertices on sphere */
void test_isobars_on_sphere(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.globe_radius = 1.0f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    for (int i = 0; i < n && i < 20; i++) {
        float vx = buf[i * WOV_VERTEX_FLOATS + 0];
        float vy = buf[i * WOV_VERTEX_FLOATS + 1];
        float vz = buf[i * WOV_VERTEX_FLOATS + 2];
        float len = vec_len(vx, vy, vz);
        FCLOSE(1.0f, len);
    }
}

/* 50. Isobar alpha stored in vertex color */
void test_isobars_alpha(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.isobar_alpha = 0.25f;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_TRUE(n > 0);
    FCLOSE(0.25f, buf[6]);
}

/* 51. Isobars don't exceed max_verts */
void test_isobars_max_verts(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[20 * WOV_VERTEX_FLOATS];
    int n = wov_pack_isobars(cfg, buf, 20);
    TEST_ASSERT_TRUE(n <= 20);
}

/* ======================================================================
 * wov_pack (full pack)
 * ====================================================================== */

/* 52. Full pack produces vertices */
void test_pack_produces_verts(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    wov_info_t info = wov_pack(cfg, buf);
    TEST_ASSERT_TRUE(info.total_verts > 0);
}

/* 53. Full pack total equals sum of parts */
void test_pack_total_equals_sum(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    wov_info_t info = wov_pack(cfg, buf);
    int expected = info.wind_arrow_verts + info.basin_outline_verts +
                   info.itcz_verts + info.isobar_verts;
    TEST_ASSERT_EQUAL_INT(expected, info.total_verts);
}

/* 54. Full pack doesn't exceed WOV_MAX_VERTICES */
void test_pack_max_vertices(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.month = 9;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    wov_info_t info = wov_pack(cfg, buf);
    TEST_ASSERT_TRUE(info.total_verts <= WOV_MAX_VERTICES);
}

/* 55. Full pack wind_arrow_verts > 0 */
void test_pack_has_wind_arrows(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    wov_info_t info = wov_pack(cfg, buf);
    TEST_ASSERT_TRUE(info.wind_arrow_verts > 0);
}

/* 56. Full pack itcz_verts > 0 */
void test_pack_has_itcz(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    wov_info_t info = wov_pack(cfg, buf);
    TEST_ASSERT_TRUE(info.itcz_verts > 0);
}

/* 57. Full pack isobar_verts > 0 */
void test_pack_has_isobars(void)
{
    wov_config_t cfg = wov_default_config();
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    wov_info_t info = wov_pack(cfg, buf);
    TEST_ASSERT_TRUE(info.isobar_verts > 0);
}

/* ======================================================================
 * Shaders
 * ====================================================================== */

/* 58. Vertex shader source is not NULL */
void test_vert_shader_not_null(void)
{
    TEST_ASSERT_NOT_NULL(wov_vert_source());
}

/* 59. Fragment shader source is not NULL */
void test_frag_shader_not_null(void)
{
    TEST_ASSERT_NOT_NULL(wov_frag_source());
}

/* 60. Vertex shader contains #version */
void test_vert_shader_version(void)
{
    const char *s = wov_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "#version 300 es"));
}

/* 61. Fragment shader contains frag_color */
void test_frag_shader_frag_color(void)
{
    const char *s = wov_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "frag_color"));
}

/* ======================================================================
 * Edge cases
 * ====================================================================== */

/* 62. Zero isobar_count produces zero isobar vertices */
void test_zero_isobars(void)
{
    wov_config_t cfg = wov_default_config();
    cfg.isobar_count = 0;
    float buf[WOV_MAX_VERTICES * WOV_VERTEX_FLOATS];
    int n = wov_pack_isobars(cfg, buf, WOV_MAX_VERTICES);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Config (1-9) */
    RUN_TEST(test_default_config_globe_radius);
    RUN_TEST(test_default_config_month);
    RUN_TEST(test_default_config_arrows_per_band);
    RUN_TEST(test_default_config_arrow_scale);
    RUN_TEST(test_default_config_arrow_alpha);
    RUN_TEST(test_default_config_basin_alpha);
    RUN_TEST(test_default_config_itcz_alpha);
    RUN_TEST(test_default_config_isobar_count);
    RUN_TEST(test_default_config_isobar_alpha);

    /* Lat/lon to XYZ (10-18) */
    RUN_TEST(test_latlon_equator_prime);
    RUN_TEST(test_latlon_north_pole);
    RUN_TEST(test_latlon_south_pole);
    RUN_TEST(test_latlon_equator_90e);
    RUN_TEST(test_latlon_equator_180);
    RUN_TEST(test_latlon_equator_270);
    RUN_TEST(test_latlon_radius_scaling);
    RUN_TEST(test_latlon_on_sphere);
    RUN_TEST(test_latlon_y_equals_sin_lat);

    /* Arrow glyph (19-25) */
    RUN_TEST(test_arrow_glyph_returns_verts);
    RUN_TEST(test_arrow_glyph_even_count);
    RUN_TEST(test_arrow_glyph_max_verts);
    RUN_TEST(test_arrow_glyph_color);
    RUN_TEST(test_arrow_glyph_zero_strength);
    RUN_TEST(test_arrow_glyph_zero_scale);
    RUN_TEST(test_arrow_glyph_near_surface);

    /* Wind arrows (26-32) */
    RUN_TEST(test_wind_arrows_produce_verts);
    RUN_TEST(test_wind_arrows_even_count);
    RUN_TEST(test_wind_arrows_max_verts);
    RUN_TEST(test_wind_arrows_scale_with_count);
    RUN_TEST(test_wind_arrows_on_sphere);
    RUN_TEST(test_wind_arrows_large_radius);
    RUN_TEST(test_wind_arrows_alpha);

    /* Basin outlines (33-38) */
    RUN_TEST(test_basin_outlines_september);
    RUN_TEST(test_basin_outlines_even);
    RUN_TEST(test_basin_outlines_february_active);
    RUN_TEST(test_basin_outlines_max_verts);
    RUN_TEST(test_basin_outlines_west_pacific_always);
    RUN_TEST(test_basin_outlines_on_sphere);

    /* ITCZ (39-45) */
    RUN_TEST(test_itcz_produce_verts);
    RUN_TEST(test_itcz_even);
    RUN_TEST(test_itcz_july_latitude);
    RUN_TEST(test_itcz_january_latitude);
    RUN_TEST(test_itcz_on_sphere);
    RUN_TEST(test_itcz_alpha);
    RUN_TEST(test_itcz_max_verts);

    /* Isobars (46-51) */
    RUN_TEST(test_isobars_produce_verts);
    RUN_TEST(test_isobars_even);
    RUN_TEST(test_isobars_scale_with_count);
    RUN_TEST(test_isobars_on_sphere);
    RUN_TEST(test_isobars_alpha);
    RUN_TEST(test_isobars_max_verts);

    /* Full pack (52-57) */
    RUN_TEST(test_pack_produces_verts);
    RUN_TEST(test_pack_total_equals_sum);
    RUN_TEST(test_pack_max_vertices);
    RUN_TEST(test_pack_has_wind_arrows);
    RUN_TEST(test_pack_has_itcz);
    RUN_TEST(test_pack_has_isobars);

    /* Shaders (58-61) */
    RUN_TEST(test_vert_shader_not_null);
    RUN_TEST(test_frag_shader_not_null);
    RUN_TEST(test_vert_shader_version);
    RUN_TEST(test_frag_shader_frag_color);

    /* Edge cases (62) */
    RUN_TEST(test_zero_isobars);

    return UNITY_END();
}
