/*
 * test_milkyway_pack.c -- Tests for procedural Milky Way galaxy band
 *
 * 55 tests covering all public API functions:
 *   mw_default_config, mw_pack, mw_brightness_at, mw_region_count,
 *   mw_region_get, mw_galactic_to_equatorial, mw_vert_source, mw_frag_source
 */

#include "../unity/unity.h"
#include "../../src/render/milkyway_pack.h"

#include <math.h>
#include <string.h>

#define FTOL 0.02f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))
#define DEG2RAD(d) ((d) * PI / 180.0)

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * mw_default_config
 * ====================================================================== */

/* 1. Default config sphere_radius positive */
void test_default_config_sphere_radius(void)
{
    mw_config_t cfg = mw_default_config();
    TEST_ASSERT_TRUE(cfg.sphere_radius > 0.0f);
}

/* 2. Default config band_width_deg is 30 */
void test_default_config_band_width(void)
{
    mw_config_t cfg = mw_default_config();
    FCLOSE(30.0f, cfg.band_width_deg);
}

/* 3. Default config core_brightness is 1.5 */
void test_default_config_core_brightness(void)
{
    mw_config_t cfg = mw_default_config();
    FCLOSE(1.5f, cfg.core_brightness);
}

/* 4. Default config edge_falloff is 2.0 */
void test_default_config_edge_falloff(void)
{
    mw_config_t cfg = mw_default_config();
    FCLOSE(2.0f, cfg.edge_falloff);
}

/* 5. Default config longitude_segments is 72 */
void test_default_config_lon_segments(void)
{
    mw_config_t cfg = mw_default_config();
    TEST_ASSERT_EQUAL_INT(72, cfg.longitude_segments);
}

/* 6. Default config latitude_segments is 12 */
void test_default_config_lat_segments(void)
{
    mw_config_t cfg = mw_default_config();
    TEST_ASSERT_EQUAL_INT(12, cfg.latitude_segments);
}

/* ======================================================================
 * mw_region_count / mw_region_get
 * ====================================================================== */

/* 7. Region count is 13 */
void test_region_count(void)
{
    TEST_ASSERT_EQUAL_INT(13, mw_region_count());
}

/* 8. Region 0 is Galactic Center */
void test_region_0_is_galactic_center(void)
{
    mw_region_t r = mw_region_get(0);
    TEST_ASSERT_NOT_NULL(r.name);
    TEST_ASSERT_EQUAL_STRING("Galactic Center", r.name);
}

/* 9. Galactic Center at l=0, b=0 */
void test_region_galactic_center_position(void)
{
    mw_region_t r = mw_region_get(0);
    FCLOSE(0.0f, r.l_center_deg);
    FCLOSE(0.0f, r.b_center_deg);
}

/* 10. Galactic Center brightness is 2.0 */
void test_region_galactic_center_brightness(void)
{
    mw_region_t r = mw_region_get(0);
    FCLOSE(2.0f, r.brightness);
}

/* 11. Galactic Center angular radius is 15 */
void test_region_galactic_center_radius(void)
{
    mw_region_t r = mw_region_get(0);
    FCLOSE(15.0f, r.angular_radius);
}

/* 12. All regions have non-null names */
void test_all_regions_have_names(void)
{
    int count = mw_region_count();
    for (int i = 0; i < count; i++) {
        mw_region_t r = mw_region_get(i);
        TEST_ASSERT_NOT_NULL(r.name);
        TEST_ASSERT_TRUE(strlen(r.name) > 0);
    }
}

/* 13. All regions have positive angular radius */
void test_all_regions_positive_radius(void)
{
    int count = mw_region_count();
    for (int i = 0; i < count; i++) {
        mw_region_t r = mw_region_get(i);
        TEST_ASSERT_TRUE(r.angular_radius > 0.0f);
    }
}

/* 14. All regions have brightness > 0 */
void test_all_regions_positive_brightness(void)
{
    int count = mw_region_count();
    for (int i = 0; i < count; i++) {
        mw_region_t r = mw_region_get(i);
        TEST_ASSERT_TRUE(r.brightness > 0.0f);
    }
}

/* 15. Out-of-range index returns empty region */
void test_region_out_of_range(void)
{
    mw_region_t r = mw_region_get(99);
    TEST_ASSERT_NULL(r.name);
    FCLOSE(0.0f, r.brightness);
}

/* 16. Negative index returns empty region */
void test_region_negative_index(void)
{
    mw_region_t r = mw_region_get(-1);
    TEST_ASSERT_NULL(r.name);
}

/* 17. Bright regions have brightness > 1.0 */
void test_bright_regions(void)
{
    /* First 7 regions are bright clouds */
    for (int i = 0; i < 7; i++) {
        mw_region_t r = mw_region_get(i);
        TEST_ASSERT_TRUE(r.brightness >= 1.0f);
    }
}

/* 18. Dark rifts have brightness < 1.0 */
void test_dark_regions(void)
{
    /* Regions 7-12 are dark rifts */
    for (int i = 7; i < 13; i++) {
        mw_region_t r = mw_region_get(i);
        TEST_ASSERT_TRUE(r.brightness < 1.0f);
    }
}

/* 19. Coal Sack is darkest */
void test_coal_sack_darkest(void)
{
    float min_brightness = 999.0f;
    int min_idx = -1;
    int count = mw_region_count();
    for (int i = 0; i < count; i++) {
        mw_region_t r = mw_region_get(i);
        if (r.brightness < min_brightness) {
            min_brightness = r.brightness;
            min_idx = i;
        }
    }
    mw_region_t darkest = mw_region_get(min_idx);
    TEST_ASSERT_EQUAL_STRING("Coal Sack", darkest.name);
}

/* ======================================================================
 * mw_brightness_at
 * ====================================================================== */

/* 20. Brightness at galactic center is high (near 2.0) */
void test_brightness_at_galactic_center(void)
{
    float b = mw_brightness_at(0.0f, 0.0f);
    TEST_ASSERT_TRUE(b > 1.5f);
}

/* 21. Brightness at Coal Sack is low */
void test_brightness_at_coal_sack(void)
{
    float b = mw_brightness_at(300.0f, -1.0f);
    TEST_ASSERT_TRUE(b < 0.5f);
}

/* 22. Brightness at galactic plane is higher than off-plane */
void test_brightness_on_plane_vs_off(void)
{
    float b_on = mw_brightness_at(180.0f, 0.0f);
    float b_off = mw_brightness_at(180.0f, 14.0f);
    TEST_ASSERT_TRUE(b_on >= b_off);
}

/* 23. Brightness is always non-negative */
void test_brightness_non_negative(void)
{
    for (int l = 0; l < 360; l += 15) {
        for (int b = -15; b <= 15; b += 5) {
            float br = mw_brightness_at((float)l, (float)b);
            TEST_ASSERT_TRUE(br >= 0.0f);
        }
    }
}

/* 24. Cygnus Star Cloud is brighter than average */
void test_brightness_at_cygnus(void)
{
    float b = mw_brightness_at(80.0f, 0.0f);
    TEST_ASSERT_TRUE(b > 1.0f);
}

/* 25. Edge of band approaches zero */
void test_brightness_edge_approaches_zero(void)
{
    /* At latitude = half band width, brightness should be very low */
    float b = mw_brightness_at(180.0f, 14.9f);
    TEST_ASSERT_TRUE(b < 0.5f);
}

/* 26. Great Rift (l=30, b=0) has reduced brightness */
void test_brightness_great_rift(void)
{
    float b_rift = mw_brightness_at(30.0f, 0.0f);
    float b_clear = mw_brightness_at(180.0f, 0.0f);
    TEST_ASSERT_TRUE(b_rift < b_clear);
}

/* ======================================================================
 * mw_galactic_to_equatorial
 * ====================================================================== */

/* 27. Galactic center (l=0, b=0) -> RA ~ 17h45m, Dec ~ -29 */
void test_gal_to_eq_center(void)
{
    float ra, dec;
    mw_galactic_to_equatorial(0.0f, 0.0f, &ra, &dec);
    /* RA ~17.76 hours (266.4 / 15) */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 17.76f, ra);
    /* Dec ~ -28.94 */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -28.94f, dec);
}

/* 28. North galactic pole (b=90) -> Dec ~ +27.13 */
void test_gal_to_eq_north_pole(void)
{
    float ra, dec;
    mw_galactic_to_equatorial(0.0f, 90.0f, &ra, &dec);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 27.13f, dec);
}

/* 29. South galactic pole (b=-90) -> Dec ~ -27.13 */
void test_gal_to_eq_south_pole(void)
{
    float ra, dec;
    mw_galactic_to_equatorial(0.0f, -90.0f, &ra, &dec);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -27.13f, dec);
}

/* 30. RA is always in [0, 24) hours */
void test_gal_to_eq_ra_range(void)
{
    for (int l = 0; l < 360; l += 30) {
        float ra, dec;
        mw_galactic_to_equatorial((float)l, 0.0f, &ra, &dec);
        TEST_ASSERT_TRUE(ra >= 0.0f);
        TEST_ASSERT_TRUE(ra < 24.0f);
    }
}

/* 31. Dec is always in [-90, 90] */
void test_gal_to_eq_dec_range(void)
{
    for (int l = 0; l < 360; l += 30) {
        for (int b = -90; b <= 90; b += 30) {
            float ra, dec;
            mw_galactic_to_equatorial((float)l, (float)b, &ra, &dec);
            TEST_ASSERT_TRUE(dec >= -90.1f);
            TEST_ASSERT_TRUE(dec <= 90.1f);
        }
    }
}

/* 32. Cygnus region (l=80, b=0) maps to RA ~ 20h, Dec ~ 40 */
void test_gal_to_eq_cygnus(void)
{
    float ra, dec;
    mw_galactic_to_equatorial(80.0f, 0.0f, &ra, &dec);
    /* Cygnus is roughly RA 20h, Dec +40 */
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 20.5f, ra);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 40.0f, dec);
}

/* 33. Conversion is smooth (nearby inputs give nearby outputs) */
void test_gal_to_eq_smooth(void)
{
    float ra1, dec1, ra2, dec2;
    mw_galactic_to_equatorial(100.0f, 5.0f, &ra1, &dec1);
    mw_galactic_to_equatorial(100.1f, 5.0f, &ra2, &dec2);
    float ddec = fabsf(dec2 - dec1);
    TEST_ASSERT_TRUE(ddec < 1.0f);
}

/* ======================================================================
 * mw_pack
 * ====================================================================== */

/* 34. Pack returns positive vertex_count */
void test_pack_positive_vertex_count(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    TEST_ASSERT_TRUE(info.vertex_count > 0);
}

/* 35. Pack vertex_count <= MW_MAX_VERTICES */
void test_pack_within_max_vertices(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    TEST_ASSERT_TRUE(info.vertex_count <= MW_MAX_VERTICES);
}

/* 36. Pack returns positive index_count */
void test_pack_positive_index_count(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    TEST_ASSERT_TRUE(info.index_count > 0);
}

/* 37. index_count is divisible by 3 (triangles) */
void test_pack_index_count_divisible_by_3(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    TEST_ASSERT_EQUAL_INT(0, info.index_count % 3);
}

/* 38. triangle_count = index_count / 3 */
void test_pack_triangle_count(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    TEST_ASSERT_EQUAL_INT(info.index_count / 3, info.triangle_count);
}

/* 39. All indices are within valid range */
void test_pack_indices_in_range(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    for (int i = 0; i < info.index_count; i++) {
        TEST_ASSERT_TRUE(indices[i] < (unsigned short)info.vertex_count);
    }
}

/* 40. All vertex positions lie on the celestial sphere */
void test_pack_positions_on_sphere(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);

    for (int i = 0; i < info.vertex_count; i++) {
        int base = i * MW_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float mag = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_FLOAT_WITHIN(1.0f, cfg.sphere_radius, mag);
    }
}

/* 41. Galactic coordinates are normalized to [0,1] and [-1,1] */
void test_pack_galcoord_range(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);

    for (int i = 0; i < info.vertex_count; i++) {
        int base = i * MW_VERTEX_FLOATS;
        float l_norm = verts[base + 3]; /* l_normalized [0,1] */
        float b_norm = verts[base + 4]; /* b_normalized [-1,1] */
        TEST_ASSERT_FLOAT_WITHIN(0.01f + 1.0f, 0.5f, l_norm); /* roughly [0,1] */
        TEST_ASSERT_TRUE(b_norm >= -1.01f && b_norm <= 1.01f);
    }
}

/* 42. Brightness values are non-negative */
void test_pack_brightness_non_negative(void)
{
    mw_config_t cfg = mw_default_config();
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);

    for (int i = 0; i < info.vertex_count; i++) {
        int base = i * MW_VERTEX_FLOATS;
        float bright = verts[base + 5]; /* brightness */
        TEST_ASSERT_TRUE(bright >= 0.0f);
    }
}

/* 43. Small grid: 4 lon x 2 lat = 8 quads = 16 tris = expected geometry */
void test_pack_small_grid(void)
{
    mw_config_t cfg = mw_default_config();
    cfg.longitude_segments = 4;
    cfg.latitude_segments = 2;
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    mw_info_t info = mw_pack(cfg, verts, indices);
    /* (4+1) * (2+1) = 15 vertices */
    TEST_ASSERT_EQUAL_INT(15, info.vertex_count);
    /* 4 * 2 quads * 2 triangles * 3 indices = 48 */
    TEST_ASSERT_EQUAL_INT(48, info.index_count);
    TEST_ASSERT_EQUAL_INT(16, info.triangle_count);
}

/* 44. Different sphere radius changes vertex magnitudes */
void test_pack_radius_affects_positions(void)
{
    mw_config_t cfg1 = mw_default_config();
    cfg1.longitude_segments = 4;
    cfg1.latitude_segments = 2;
    cfg1.sphere_radius = 50.0f;
    mw_config_t cfg2 = cfg1;
    cfg2.sphere_radius = 200.0f;

    float v1[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    float v2[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short i1[MW_MAX_VERTICES * 6];
    unsigned short i2[MW_MAX_VERTICES * 6];
    mw_pack(cfg1, v1, i1);
    mw_pack(cfg2, v2, i2);

    /* First vertex magnitudes should differ */
    float mag1 = sqrtf(v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2]);
    float mag2 = sqrtf(v2[0]*v2[0] + v2[1]*v2[1] + v2[2]*v2[2]);
    TEST_ASSERT_TRUE(mag2 > mag1);
}

/* 45. MW_VERTEX_FLOATS is 6 */
void test_vertex_floats_is_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, MW_VERTEX_FLOATS);
}

/* 46. MW_MAX_VERTICES is 2000 */
void test_max_vertices_is_2000(void)
{
    TEST_ASSERT_EQUAL_INT(2000, MW_MAX_VERTICES);
}

/* 47. Pack produces non-zero vertex data */
void test_pack_nonzero_data(void)
{
    mw_config_t cfg = mw_default_config();
    cfg.longitude_segments = 8;
    cfg.latitude_segments = 4;
    float verts[MW_MAX_VERTICES * MW_VERTEX_FLOATS];
    unsigned short indices[MW_MAX_VERTICES * 6];
    memset(verts, 0, sizeof(verts));
    mw_pack(cfg, verts, indices);
    int non_zero = 0;
    for (int i = 0; i < 100; i++) {
        if (verts[i] != 0.0f) non_zero++;
    }
    TEST_ASSERT_TRUE(non_zero > 0);
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 48. Vertex shader starts with #version 300 es */
void test_vert_source_version(void)
{
    const char *src = mw_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 49. Fragment shader starts with #version 300 es */
void test_frag_source_version(void)
{
    const char *src = mw_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 50. Vertex shader has u_mvp uniform */
void test_vert_source_has_mvp(void)
{
    const char *src = mw_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* 51. Vertex shader has a_position attribute */
void test_vert_source_has_position(void)
{
    const char *src = mw_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

/* 52. Vertex shader has a_galcoord attribute */
void test_vert_source_has_galcoord(void)
{
    const char *src = mw_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_galcoord"));
}

/* 53. Vertex shader has a_brightness attribute */
void test_vert_source_has_brightness(void)
{
    const char *src = mw_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_brightness"));
}

/* 54. Fragment shader contains noise function (fbm2) */
void test_frag_source_has_noise(void)
{
    const char *src = mw_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "fbm2"));
}

/* 55. Fragment shader has frag_color output */
void test_frag_source_has_output(void)
{
    const char *src = mw_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Config */
    RUN_TEST(test_default_config_sphere_radius);
    RUN_TEST(test_default_config_band_width);
    RUN_TEST(test_default_config_core_brightness);
    RUN_TEST(test_default_config_edge_falloff);
    RUN_TEST(test_default_config_lon_segments);
    RUN_TEST(test_default_config_lat_segments);

    /* Regions */
    RUN_TEST(test_region_count);
    RUN_TEST(test_region_0_is_galactic_center);
    RUN_TEST(test_region_galactic_center_position);
    RUN_TEST(test_region_galactic_center_brightness);
    RUN_TEST(test_region_galactic_center_radius);
    RUN_TEST(test_all_regions_have_names);
    RUN_TEST(test_all_regions_positive_radius);
    RUN_TEST(test_all_regions_positive_brightness);
    RUN_TEST(test_region_out_of_range);
    RUN_TEST(test_region_negative_index);
    RUN_TEST(test_bright_regions);
    RUN_TEST(test_dark_regions);
    RUN_TEST(test_coal_sack_darkest);

    /* Brightness */
    RUN_TEST(test_brightness_at_galactic_center);
    RUN_TEST(test_brightness_at_coal_sack);
    RUN_TEST(test_brightness_on_plane_vs_off);
    RUN_TEST(test_brightness_non_negative);
    RUN_TEST(test_brightness_at_cygnus);
    RUN_TEST(test_brightness_edge_approaches_zero);
    RUN_TEST(test_brightness_great_rift);

    /* Galactic to equatorial */
    RUN_TEST(test_gal_to_eq_center);
    RUN_TEST(test_gal_to_eq_north_pole);
    RUN_TEST(test_gal_to_eq_south_pole);
    RUN_TEST(test_gal_to_eq_ra_range);
    RUN_TEST(test_gal_to_eq_dec_range);
    RUN_TEST(test_gal_to_eq_cygnus);
    RUN_TEST(test_gal_to_eq_smooth);

    /* Pack */
    RUN_TEST(test_pack_positive_vertex_count);
    RUN_TEST(test_pack_within_max_vertices);
    RUN_TEST(test_pack_positive_index_count);
    RUN_TEST(test_pack_index_count_divisible_by_3);
    RUN_TEST(test_pack_triangle_count);
    RUN_TEST(test_pack_indices_in_range);
    RUN_TEST(test_pack_positions_on_sphere);
    RUN_TEST(test_pack_galcoord_range);
    RUN_TEST(test_pack_brightness_non_negative);
    RUN_TEST(test_pack_small_grid);
    RUN_TEST(test_pack_radius_affects_positions);
    RUN_TEST(test_vertex_floats_is_6);
    RUN_TEST(test_max_vertices_is_2000);
    RUN_TEST(test_pack_nonzero_data);

    /* Shaders */
    RUN_TEST(test_vert_source_version);
    RUN_TEST(test_frag_source_version);
    RUN_TEST(test_vert_source_has_mvp);
    RUN_TEST(test_vert_source_has_position);
    RUN_TEST(test_vert_source_has_galcoord);
    RUN_TEST(test_vert_source_has_brightness);
    RUN_TEST(test_frag_source_has_noise);
    RUN_TEST(test_frag_source_has_output);

    return UNITY_END();
}
