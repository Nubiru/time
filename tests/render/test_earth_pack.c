/*
 * test_earth_pack.c -- Tests for Earth render pack: globe, coastlines,
 * terminator, atmosphere shell packing into GPU-ready vertex arrays.
 *
 * 50 tests covering all public API functions + edge cases.
 */

#include "../unity/unity.h"
#include "../../src/render/earth_pack.h"
#include "../../src/render/earth_globe.h"
#include "../../src/render/earth_atmosphere.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

#define FTOL 0.01f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * erp_default_config
 * ====================================================================== */

/* 1. Default config returns positive globe_radius */
void test_default_config_globe_radius(void)
{
    erp_config_t cfg = erp_default_config();
    TEST_ASSERT_TRUE(cfg.globe_radius > 0.0f);
}

/* 2. Default config atmo_radius_ratio > 1.0 */
void test_default_config_atmo_ratio(void)
{
    erp_config_t cfg = erp_default_config();
    TEST_ASSERT_TRUE(cfg.atmo_radius_ratio > 1.0f);
}

/* 3. Default config lat_segments >= 4 */
void test_default_config_lat_segments(void)
{
    erp_config_t cfg = erp_default_config();
    TEST_ASSERT_TRUE(cfg.lat_segments >= 4);
}

/* 4. Default config lon_segments >= 4 */
void test_default_config_lon_segments(void)
{
    erp_config_t cfg = erp_default_config();
    TEST_ASSERT_TRUE(cfg.lon_segments >= 4);
}

/* 5. Default config coastline_alpha in (0, 1] */
void test_default_config_coastline_alpha(void)
{
    erp_config_t cfg = erp_default_config();
    TEST_ASSERT_TRUE(cfg.coastline_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.coastline_alpha <= 1.0f);
}

/* 6. Default config terminator_alpha in (0, 1] */
void test_default_config_terminator_alpha(void)
{
    erp_config_t cfg = erp_default_config();
    TEST_ASSERT_TRUE(cfg.terminator_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.terminator_alpha <= 1.0f);
}

/* ======================================================================
 * erp_info
 * ====================================================================== */

/* 7. Info returns correct globe vertex count */
void test_info_globe_verts(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    int expected = earth_globe_vertex_count(cfg.lat_segments, cfg.lon_segments);
    TEST_ASSERT_EQUAL_INT(expected, info.globe_verts);
}

/* 8. Info returns correct globe index count */
void test_info_globe_indices(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    int expected = earth_globe_index_count(cfg.lat_segments, cfg.lon_segments);
    TEST_ASSERT_EQUAL_INT(expected, info.globe_indices);
}

/* 9. Info returns correct coastline vertex count (2 per segment) */
void test_info_coastline_verts(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    int expected = earth_coastline_count() * 2;
    TEST_ASSERT_EQUAL_INT(expected, info.coastline_verts);
}

/* 10. Info returns correct terminator vertex count */
void test_info_terminator_verts(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    TEST_ASSERT_EQUAL_INT(ERP_TERMINATOR_POINTS, info.terminator_verts);
}

/* 11. Info returns correct atmosphere vertex count */
void test_info_atmo_verts(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    int expected = earth_atmo_shell_vertex_count();
    TEST_ASSERT_EQUAL_INT(expected, info.atmo_verts);
}

/* 12. Info returns correct atmosphere index count */
void test_info_atmo_indices(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    int expected = earth_atmo_shell_index_count();
    TEST_ASSERT_EQUAL_INT(expected, info.atmo_indices);
}

/* ======================================================================
 * erp_pack_globe
 * ====================================================================== */

/* 13. Pack globe returns correct vertex count */
void test_pack_globe_vertex_count(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);

    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    int n = erp_pack_globe(&cfg, verts, indices);
    TEST_ASSERT_EQUAL_INT(info.globe_verts, n);
}

/* 14. Pack globe first vertex has valid position data */
void test_pack_globe_first_vertex_position(void)
{
    erp_config_t cfg = erp_default_config();
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    erp_pack_globe(&cfg, verts, indices);

    /* First vertex is north pole: (0, radius, 0) */
    float r = cfg.globe_radius;
    FCLOSE(0.0f, verts[0]);  /* x */
    FCLOSE(r, verts[1]);     /* y */
    FCLOSE(0.0f, verts[2]);  /* z */
}

/* 15. Pack globe positions are scaled by globe_radius */
void test_pack_globe_radius_scaling(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 5.0f;
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    erp_pack_globe(&cfg, verts, indices);

    /* North pole vertex: position should be (0, 5, 0) */
    FCLOSE(0.0f, verts[0]);
    FCLOSE(5.0f, verts[1]);
    FCLOSE(0.0f, verts[2]);
}

/* 16. Pack globe normals are unit length (not scaled by radius) */
void test_pack_globe_normal_unit_length(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 10.0f;
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    int n = erp_pack_globe(&cfg, verts, indices);

    /* Check a few vertices have unit-length normals */
    for (int i = 0; i < n && i < 10; i++) {
        int base = i * ERP_GLOBE_VERTEX_FLOATS;
        float nx = verts[base + 3];
        float ny = verts[base + 4];
        float nz = verts[base + 5];
        float len = sqrtf(nx * nx + ny * ny + nz * nz);
        FCLOSE(1.0f, len);
    }
}

/* 17. Pack globe UVs are in [0, 1] */
void test_pack_globe_uv_range(void)
{
    erp_config_t cfg = erp_default_config();
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    int n = erp_pack_globe(&cfg, verts, indices);

    for (int i = 0; i < n; i++) {
        int base = i * ERP_GLOBE_VERTEX_FLOATS;
        float u = verts[base + 6];
        float v = verts[base + 7];
        TEST_ASSERT_TRUE(u >= 0.0f && u <= 1.0f);
        TEST_ASSERT_TRUE(v >= 0.0f && v <= 1.0f);
    }
}

/* 18. Pack globe stride matches expected */
void test_pack_globe_stride(void)
{
    TEST_ASSERT_EQUAL_INT(8 * (int)sizeof(float), ERP_GLOBE_VERTEX_STRIDE);
}

/* 19. Pack globe indices are within bounds */
void test_pack_globe_indices_in_bounds(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    int n = erp_pack_globe(&cfg, verts, indices);

    for (int i = 0; i < info.globe_indices; i++) {
        TEST_ASSERT_TRUE(indices[i] < (unsigned int)n);
    }
}

/* ======================================================================
 * erp_pack_coastlines
 * ====================================================================== */

/* 20. Pack coastlines returns correct vertex count */
void test_pack_coastlines_vertex_count(void)
{
    erp_config_t cfg = erp_default_config();
    int seg_count = earth_coastline_count();
    float out[EARTH_COASTLINE_POINT_COUNT * 2 * ERP_LINE_VERTEX_FLOATS];

    int n = erp_pack_coastlines(&cfg, out);
    TEST_ASSERT_EQUAL_INT(seg_count * 2, n);
}

/* 21. Pack coastlines positions are scaled by globe_radius */
void test_pack_coastlines_radius_scaling(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 3.0f;

    float out[EARTH_COASTLINE_POINT_COUNT * 2 * ERP_LINE_VERTEX_FLOATS];
    int n = erp_pack_coastlines(&cfg, out);
    TEST_ASSERT_TRUE(n > 0);

    /* Every vertex position should be approximately on a sphere of radius 3 */
    for (int i = 0; i < n && i < 10; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        float x = out[base + 0];
        float y = out[base + 1];
        float z = out[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        FCLOSE(3.0f, dist);
    }
}

/* 22. Pack coastlines color alpha matches config */
void test_pack_coastlines_color_alpha(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.coastline_alpha = 0.8f;

    float out[EARTH_COASTLINE_POINT_COUNT * 2 * ERP_LINE_VERTEX_FLOATS];
    erp_pack_coastlines(&cfg, out);

    /* Check alpha of first vertex: floats at offset 3,4,5,6 => r,g,b,a */
    float a = out[6]; /* position(3) + color(4), alpha at index 6 */
    FCLOSE(0.8f, a);
}

/* 23. Pack coastlines color RGB components are in [0, 1] */
void test_pack_coastlines_color_valid(void)
{
    erp_config_t cfg = erp_default_config();
    float out[EARTH_COASTLINE_POINT_COUNT * 2 * ERP_LINE_VERTEX_FLOATS];
    int n = erp_pack_coastlines(&cfg, out);

    for (int i = 0; i < n && i < 20; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        float r = out[base + 3];
        float g = out[base + 4];
        float b = out[base + 5];
        float a = out[base + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

/* 24. Pack coastlines line stride matches expected */
void test_pack_coastlines_stride(void)
{
    TEST_ASSERT_EQUAL_INT(7 * (int)sizeof(float), ERP_LINE_VERTEX_STRIDE);
}

/* ======================================================================
 * erp_pack_terminator
 * ====================================================================== */

/* 25. Pack terminator returns correct vertex count */
void test_pack_terminator_vertex_count(void)
{
    erp_config_t cfg = erp_default_config();
    float out[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];

    int n = erp_pack_terminator(0.0, 0.0, &cfg, out);
    TEST_ASSERT_EQUAL_INT(ERP_TERMINATOR_POINTS, n);
}

/* 26. Pack terminator positions are on sphere of globe_radius */
void test_pack_terminator_on_sphere(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 4.0f;

    float out[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];
    erp_pack_terminator(0.0, 0.0, &cfg, out);

    for (int i = 0; i < ERP_TERMINATOR_POINTS; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        float x = out[base + 0];
        float y = out[base + 1];
        float z = out[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        FCLOSE(4.0f, dist);
    }
}

/* 27. Pack terminator alpha matches config */
void test_pack_terminator_alpha(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.terminator_alpha = 0.6f;

    float out[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];
    erp_pack_terminator(0.0, 6.0, &cfg, out);

    float a = out[6]; /* first vertex alpha at offset 6 */
    FCLOSE(0.6f, a);
}

/* 28. Pack terminator color RGB are in [0, 1] */
void test_pack_terminator_color_valid(void)
{
    erp_config_t cfg = erp_default_config();
    float out[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];
    erp_pack_terminator(23.0, 12.0, &cfg, out);

    for (int i = 0; i < ERP_TERMINATOR_POINTS; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        float r = out[base + 3];
        float g = out[base + 4];
        float b = out[base + 5];
        float a = out[base + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

/* 29. Pack terminator at different sun positions gives different geometry */
void test_pack_terminator_sun_position_matters(void)
{
    erp_config_t cfg = erp_default_config();

    float out1[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];
    float out2[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];

    erp_pack_terminator(0.0, 0.0, &cfg, out1);
    erp_pack_terminator(23.0, 12.0, &cfg, out2);

    /* At least one position should differ */
    int differs = 0;
    for (int i = 0; i < ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS; i++) {
        if (fabsf(out1[i] - out2[i]) > 0.01f) {
            differs = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differs);
}

/* ======================================================================
 * erp_pack_atmosphere
 * ====================================================================== */

/* 30. Pack atmosphere returns correct vertex count */
void test_pack_atmosphere_vertex_count(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);

    float verts[EARTH_ATMO_MAX_VERTICES * 7]; /* 7 floats per atmo vertex */
    unsigned int indices[EARTH_ATMO_MAX_INDICES];

    int n = erp_pack_atmosphere(&cfg, verts, indices);
    TEST_ASSERT_EQUAL_INT(info.atmo_verts, n);
}

/* 31. Atmosphere positions are scaled by globe_radius * atmo_radius_ratio */
void test_pack_atmosphere_radius_scaling(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 2.0f;
    cfg.atmo_radius_ratio = 1.05f;

    float verts[EARTH_ATMO_MAX_VERTICES * 7];
    unsigned int indices[EARTH_ATMO_MAX_INDICES];

    erp_pack_atmosphere(&cfg, verts, indices);

    /* Atmo shell has its own radius_ratio built in from earth_atmo_shell_generate.
     * We then scale by globe_radius. Check a few vertices are at expected distance. */
    float expected_r = 2.0f * 1.05f;

    /* Check a mid-ring vertex (skip poles which may be at 0,0,0 in x,z) */
    /* Vertex in middle of the mesh: pick row at lat_seg/2 */
    int mid = (EARTH_ATMO_LAT_SEGMENTS / 2) * (EARTH_ATMO_LON_SEGMENTS + 1) + 1;
    int base = mid * 7;
    float x = verts[base + 0];
    float y = verts[base + 1];
    float z = verts[base + 2];
    float dist = sqrtf(x * x + y * y + z * z);
    FCLOSE(expected_r, dist);
}

/* 32. Atmosphere normals are unit length */
void test_pack_atmosphere_normals_unit(void)
{
    erp_config_t cfg = erp_default_config();
    float verts[EARTH_ATMO_MAX_VERTICES * 7];
    unsigned int indices[EARTH_ATMO_MAX_INDICES];

    int n = erp_pack_atmosphere(&cfg, verts, indices);

    for (int i = 0; i < n && i < 20; i++) {
        int base = i * 7;
        float nx = verts[base + 3];
        float ny = verts[base + 4];
        float nz = verts[base + 5];
        float len = sqrtf(nx * nx + ny * ny + nz * nz);
        FCLOSE(1.0f, len);
    }
}

/* 33. Atmosphere opacity values are in [0, 1] */
void test_pack_atmosphere_opacity_range(void)
{
    erp_config_t cfg = erp_default_config();
    float verts[EARTH_ATMO_MAX_VERTICES * 7];
    unsigned int indices[EARTH_ATMO_MAX_INDICES];

    int n = erp_pack_atmosphere(&cfg, verts, indices);

    for (int i = 0; i < n; i++) {
        int base = i * 7;
        float opacity = verts[base + 6];
        TEST_ASSERT_TRUE(opacity >= 0.0f && opacity <= 1.0f);
    }
}

/* 34. Atmosphere indices are within bounds */
void test_pack_atmosphere_indices_in_bounds(void)
{
    erp_config_t cfg = erp_default_config();
    erp_info_t info = erp_info(&cfg);
    float verts[EARTH_ATMO_MAX_VERTICES * 7];
    unsigned int indices[EARTH_ATMO_MAX_INDICES];

    int n = erp_pack_atmosphere(&cfg, verts, indices);

    for (int i = 0; i < info.atmo_indices; i++) {
        TEST_ASSERT_TRUE(indices[i] < (unsigned int)n);
    }
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 35. Globe vertex shader source is non-null and non-empty */
void test_globe_vert_source_exists(void)
{
    const char *src = erp_globe_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 36. Globe fragment shader source is non-null */
void test_globe_frag_source_exists(void)
{
    const char *src = erp_globe_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 37. Globe fragment shader contains u_sun_dir uniform */
void test_globe_frag_has_sun_dir(void)
{
    const char *src = erp_globe_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_sun_dir"));
}

/* 38. Atmosphere vertex shader source exists */
void test_atmo_vert_source_exists(void)
{
    const char *src = erp_atmo_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 39. Atmosphere fragment shader mentions limb glow (dot product) */
void test_atmo_frag_has_limb_glow(void)
{
    const char *src = erp_atmo_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "dot"));
}

/* 49. Atmosphere fragment shader contains u_sun_dir uniform */
void test_atmo_frag_has_sun_dir(void)
{
    const char *src = erp_atmo_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "u_sun_dir"));
}

/* 50. Atmosphere fragment shader contains Rayleigh scattering term */
void test_atmo_frag_has_rayleigh(void)
{
    const char *src = erp_atmo_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "rayleigh"));
}

/* 40. Line vertex shader source exists */
void test_line_vert_source_exists(void)
{
    const char *src = erp_line_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 41. Line fragment shader source exists */
void test_line_frag_source_exists(void)
{
    const char *src = erp_line_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 42. All shaders contain version 300 es */
void test_shaders_version_300_es(void)
{
    TEST_ASSERT_NOT_NULL(strstr(erp_globe_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(erp_globe_frag_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(erp_atmo_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(erp_atmo_frag_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(erp_line_vert_source(), "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(erp_line_frag_source(), "#version 300 es"));
}

/* ======================================================================
 * Edge cases
 * ====================================================================== */

/* 43. Pack globe with radius=0 produces zero-position vertices */
void test_pack_globe_radius_zero(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 0.0f;
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    int n = erp_pack_globe(&cfg, verts, indices);
    TEST_ASSERT_TRUE(n > 0);

    /* All positions should be at origin */
    for (int i = 0; i < n && i < 10; i++) {
        int base = i * ERP_GLOBE_VERTEX_FLOATS;
        FCLOSE(0.0f, verts[base + 0]);
        FCLOSE(0.0f, verts[base + 1]);
        FCLOSE(0.0f, verts[base + 2]);
    }
}

/* 44. Pack globe with very large radius produces scaled positions */
void test_pack_globe_large_radius(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 1000.0f;
    float verts[EARTH_GLOBE_MAX_VERTICES * ERP_GLOBE_VERTEX_FLOATS];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];

    erp_pack_globe(&cfg, verts, indices);

    /* North pole: (0, 1000, 0) */
    FCLOSE(0.0f, verts[0]);
    FCLOSE(1000.0f, verts[1]);
    FCLOSE(0.0f, verts[2]);
}

/* 45. Pack coastlines with radius=0 produces zero-distance vertices */
void test_pack_coastlines_radius_zero(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 0.0f;
    float out[EARTH_COASTLINE_POINT_COUNT * 2 * ERP_LINE_VERTEX_FLOATS];

    int n = erp_pack_coastlines(&cfg, out);
    TEST_ASSERT_TRUE(n > 0);

    for (int i = 0; i < n && i < 10; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        float x = out[base + 0];
        float y = out[base + 1];
        float z = out[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        FCLOSE(0.0f, dist);
    }
}

/* 46. All vertex shaders contain gl_Position */
void test_all_vert_shaders_have_gl_position(void)
{
    TEST_ASSERT_NOT_NULL(strstr(erp_globe_vert_source(), "gl_Position"));
    TEST_ASSERT_NOT_NULL(strstr(erp_atmo_vert_source(), "gl_Position"));
    TEST_ASSERT_NOT_NULL(strstr(erp_line_vert_source(), "gl_Position"));
}

/* 47. All shaders contain a main function */
void test_all_shaders_have_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(erp_globe_vert_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(erp_globe_frag_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(erp_atmo_vert_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(erp_atmo_frag_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(erp_line_vert_source(), "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(erp_line_frag_source(), "void main()"));
}

/* 48. Pack terminator with radius=0 produces zero-distance vertices */
void test_pack_terminator_radius_zero(void)
{
    erp_config_t cfg = erp_default_config();
    cfg.globe_radius = 0.0f;
    float out[ERP_TERMINATOR_POINTS * ERP_LINE_VERTEX_FLOATS];

    int n = erp_pack_terminator(0.0, 0.0, &cfg, out);
    TEST_ASSERT_EQUAL_INT(ERP_TERMINATOR_POINTS, n);

    for (int i = 0; i < n; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        float x = out[base + 0];
        float y = out[base + 1];
        float z = out[base + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        FCLOSE(0.0f, dist);
    }
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* erp_default_config */
    RUN_TEST(test_default_config_globe_radius);
    RUN_TEST(test_default_config_atmo_ratio);
    RUN_TEST(test_default_config_lat_segments);
    RUN_TEST(test_default_config_lon_segments);
    RUN_TEST(test_default_config_coastline_alpha);
    RUN_TEST(test_default_config_terminator_alpha);

    /* erp_info */
    RUN_TEST(test_info_globe_verts);
    RUN_TEST(test_info_globe_indices);
    RUN_TEST(test_info_coastline_verts);
    RUN_TEST(test_info_terminator_verts);
    RUN_TEST(test_info_atmo_verts);
    RUN_TEST(test_info_atmo_indices);

    /* erp_pack_globe */
    RUN_TEST(test_pack_globe_vertex_count);
    RUN_TEST(test_pack_globe_first_vertex_position);
    RUN_TEST(test_pack_globe_radius_scaling);
    RUN_TEST(test_pack_globe_normal_unit_length);
    RUN_TEST(test_pack_globe_uv_range);
    RUN_TEST(test_pack_globe_stride);
    RUN_TEST(test_pack_globe_indices_in_bounds);

    /* erp_pack_coastlines */
    RUN_TEST(test_pack_coastlines_vertex_count);
    RUN_TEST(test_pack_coastlines_radius_scaling);
    RUN_TEST(test_pack_coastlines_color_alpha);
    RUN_TEST(test_pack_coastlines_color_valid);
    RUN_TEST(test_pack_coastlines_stride);

    /* erp_pack_terminator */
    RUN_TEST(test_pack_terminator_vertex_count);
    RUN_TEST(test_pack_terminator_on_sphere);
    RUN_TEST(test_pack_terminator_alpha);
    RUN_TEST(test_pack_terminator_color_valid);
    RUN_TEST(test_pack_terminator_sun_position_matters);

    /* erp_pack_atmosphere */
    RUN_TEST(test_pack_atmosphere_vertex_count);
    RUN_TEST(test_pack_atmosphere_radius_scaling);
    RUN_TEST(test_pack_atmosphere_normals_unit);
    RUN_TEST(test_pack_atmosphere_opacity_range);
    RUN_TEST(test_pack_atmosphere_indices_in_bounds);

    /* Shaders */
    RUN_TEST(test_globe_vert_source_exists);
    RUN_TEST(test_globe_frag_source_exists);
    RUN_TEST(test_globe_frag_has_sun_dir);
    RUN_TEST(test_atmo_vert_source_exists);
    RUN_TEST(test_atmo_frag_has_limb_glow);
    RUN_TEST(test_atmo_frag_has_sun_dir);
    RUN_TEST(test_atmo_frag_has_rayleigh);
    RUN_TEST(test_line_vert_source_exists);
    RUN_TEST(test_line_frag_source_exists);
    RUN_TEST(test_shaders_version_300_es);

    /* Edge cases */
    RUN_TEST(test_pack_globe_radius_zero);
    RUN_TEST(test_pack_globe_large_radius);
    RUN_TEST(test_pack_coastlines_radius_zero);
    RUN_TEST(test_all_vert_shaders_have_gl_position);
    RUN_TEST(test_all_shaders_have_main);
    RUN_TEST(test_pack_terminator_radius_zero);

    return UNITY_END();
}
