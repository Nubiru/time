/*
 * test_deep_sky_pack.c -- Tests for deep sky object billboard vertex packing
 *
 * RED phase: 38 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/deep_sky_pack.h"
#include "../../src/render/deep_sky.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * dsp_default_config
 * ====================================================================== */

/* 1. Default config returns sane sphere_radius */
void test_default_config_sphere_radius(void)
{
    dsp_config_t cfg = dsp_default_config();
    TEST_ASSERT_TRUE(cfg.sphere_radius > 0.0f);
}

/* 2. Default config has positive base_size */
void test_default_config_base_size(void)
{
    dsp_config_t cfg = dsp_default_config();
    TEST_ASSERT_TRUE(cfg.base_size > 0.0f);
}

/* 3. Default config min_size <= max_size */
void test_default_config_size_range(void)
{
    dsp_config_t cfg = dsp_default_config();
    TEST_ASSERT_TRUE(cfg.min_size <= cfg.max_size);
}

/* 4. Default config alpha range valid */
void test_default_config_alpha_range(void)
{
    dsp_config_t cfg = dsp_default_config();
    TEST_ASSERT_TRUE(cfg.base_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.base_alpha <= 1.0f);
    TEST_ASSERT_TRUE(cfg.min_alpha >= 0.0f);
    TEST_ASSERT_TRUE(cfg.min_alpha < cfg.base_alpha);
}

/* 5. Default config magnitude range: bright < faint */
void test_default_config_magnitude_range(void)
{
    dsp_config_t cfg = dsp_default_config();
    TEST_ASSERT_TRUE(cfg.bright_mag < cfg.faint_mag);
}

/* ======================================================================
 * dsp_ra_dec_to_xyz
 * ====================================================================== */

/* 6. RA=0, Dec=0 -> on the x-axis (vernal equinox) */
void test_ra_dec_zero_zero(void)
{
    float out[3];
    dsp_ra_dec_to_xyz(0.0, 0.0, 1.0f, 0.0f, out);
    /* At obliquity=0, equatorial == ecliptic. RA=0 Dec=0 -> x=1, y=0, z=0 */
    FCLOSE(1.0f, out[0]);
    FCLOSE(0.0f, out[1]);
    FCLOSE(0.0f, out[2]);
}

/* 7. RA=6h, Dec=0 -> on the z-axis (90 deg ecliptic longitude) */
void test_ra_6h_dec_0(void)
{
    float out[3];
    dsp_ra_dec_to_xyz(6.0, 0.0, 1.0f, 0.0f, out);
    /* At obliquity=0: equatorial y -> ecliptic z (via convention swap) */
    FCLOSE(0.0f, out[0]);
    /* y = z_ecl = -eq.y*so + eq.z*co = eq.z at obl=0 = 0 */
    FCLOSE(0.0f, out[1]);
    /* z = y_ecl = eq.y*co + eq.z*so = eq.y at obl=0 = sin(90)=1 */
    FCLOSE(1.0f, out[2]);
}

/* 8. Dec=90 (north pole) -> on the y-axis (ecliptic north at obl=0) */
void test_north_pole(void)
{
    float out[3];
    dsp_ra_dec_to_xyz(0.0, 90.0, 1.0f, 0.0f, out);
    /* eq: x=cos(90)*cos(0)=0, y=cos(90)*sin(0)=0, z=sin(90)=1 */
    /* ecliptic at obl=0: x_ecl=0, y_ecl=0*1+1*0=0, z_ecl=-0*0+1*1=1 */
    /* convention: out[0]=x_ecl=0, out[1]=z_ecl=1, out[2]=y_ecl=0 */
    FCLOSE(0.0f, out[0]);
    FCLOSE(1.0f, out[1]);
    FCLOSE(0.0f, out[2]);
}

/* 9. Radius scaling */
void test_ra_dec_radius_scaling(void)
{
    float out1[3], out2[3];
    dsp_ra_dec_to_xyz(3.0, 45.0, 1.0f, 23.44f, out1);
    dsp_ra_dec_to_xyz(3.0, 45.0, 50.0f, 23.44f, out2);
    FCLOSE(out1[0] * 50.0f, out2[0]);
    FCLOSE(out1[1] * 50.0f, out2[1]);
    FCLOSE(out1[2] * 50.0f, out2[2]);
}

/* 10. Result on unit sphere has magnitude ~1 */
void test_ra_dec_unit_sphere(void)
{
    float out[3];
    dsp_ra_dec_to_xyz(12.5, -30.0, 1.0f, 23.44f, out);
    float mag = sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
    FCLOSE(1.0f, mag);
}

/* 11. South pole (Dec=-90) */
void test_south_pole(void)
{
    float out[3];
    dsp_ra_dec_to_xyz(0.0, -90.0, 1.0f, 0.0f, out);
    /* eq: x=0, y=0, z=-1. obl=0: x_ecl=0, y_ecl=0*1+(-1)*0=0, z_ecl=-0*0+(-1)*1=-1 */
    /* convention: out[0]=x_ecl=0, out[1]=z_ecl=-1, out[2]=y_ecl=0 */
    FCLOSE(0.0f, out[0]);
    FCLOSE(-1.0f, out[1]);
    FCLOSE(0.0f, out[2]);
}

/* ======================================================================
 * dsp_billboard_size
 * ====================================================================== */

/* 12. 1 arcmin -> base_size */
void test_billboard_size_1_arcmin(void)
{
    float s = dsp_billboard_size(1.0, 0.5f, 0.1f, 10.0f);
    FCLOSE(0.5f, s);
}

/* 13. Large angular size -> clamped to max */
void test_billboard_size_clamp_max(void)
{
    float s = dsp_billboard_size(200.0, 0.5f, 0.1f, 10.0f);
    FCLOSE(10.0f, s);
}

/* 14. Tiny angular size -> clamped to min */
void test_billboard_size_clamp_min(void)
{
    float s = dsp_billboard_size(0.001, 0.5f, 0.3f, 10.0f);
    FCLOSE(0.3f, s);
}

/* 15. Proportional scaling */
void test_billboard_size_proportional(void)
{
    float s = dsp_billboard_size(4.0, 0.5f, 0.1f, 100.0f);
    FCLOSE(2.0f, s);
}

/* 16. Zero angular size -> min */
void test_billboard_size_zero(void)
{
    float s = dsp_billboard_size(0.0, 0.5f, 0.2f, 10.0f);
    FCLOSE(0.2f, s);
}

/* ======================================================================
 * dsp_magnitude_alpha
 * ====================================================================== */

/* 17. Brightest magnitude -> base_alpha */
void test_magnitude_alpha_bright(void)
{
    float a = dsp_magnitude_alpha(3.0, 0.6f, 0.1f, 3.0f, 10.0f);
    FCLOSE(0.6f, a);
}

/* 18. Faintest magnitude -> min_alpha */
void test_magnitude_alpha_faint(void)
{
    float a = dsp_magnitude_alpha(10.0, 0.6f, 0.1f, 3.0f, 10.0f);
    FCLOSE(0.1f, a);
}

/* 19. Midpoint magnitude -> midpoint alpha */
void test_magnitude_alpha_midpoint(void)
{
    float a = dsp_magnitude_alpha(6.5, 0.6f, 0.1f, 3.0f, 10.0f);
    FCLOSE(0.35f, a);
}

/* 20. Brighter than bright_mag -> clamped to base_alpha */
void test_magnitude_alpha_brighter_than_bright(void)
{
    float a = dsp_magnitude_alpha(1.0, 0.6f, 0.1f, 3.0f, 10.0f);
    FCLOSE(0.6f, a);
}

/* 21. Fainter than faint_mag -> clamped to min_alpha */
void test_magnitude_alpha_fainter_than_faint(void)
{
    float a = dsp_magnitude_alpha(15.0, 0.6f, 0.1f, 3.0f, 10.0f);
    FCLOSE(0.1f, a);
}

/* ======================================================================
 * dsp_type_color
 * ====================================================================== */

/* 22. Emission nebula -> red/pink channel dominant */
void test_type_color_emission(void)
{
    float c[4];
    dsp_type_color(DSO_EMISSION_NEBULA, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.8f);  /* strong red */
    TEST_ASSERT_TRUE(c[0] > c[1]);   /* red > green */
    TEST_ASSERT_TRUE(c[0] > c[2]);   /* red > blue */
    FCLOSE(1.0f, c[3]);              /* alpha passed through */
}

/* 23. Reflection nebula -> blue dominant */
void test_type_color_reflection(void)
{
    float c[4];
    dsp_type_color(DSO_REFLECTION_NEBULA, 0.5f, c);
    TEST_ASSERT_TRUE(c[2] > 0.8f);  /* strong blue */
    TEST_ASSERT_TRUE(c[2] > c[0]);   /* blue > red */
    FCLOSE(0.5f, c[3]);
}

/* 24. Planetary nebula -> teal/green dominant */
void test_type_color_planetary(void)
{
    float c[4];
    dsp_type_color(DSO_PLANETARY_NEBULA, 1.0f, c);
    TEST_ASSERT_TRUE(c[1] > 0.7f);  /* green channel high */
    TEST_ASSERT_TRUE(c[1] > c[0]);   /* green > red */
}

/* 25. Open cluster -> warm white (all channels high) */
void test_type_color_open_cluster(void)
{
    float c[4];
    dsp_type_color(DSO_OPEN_CLUSTER, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.8f);
    TEST_ASSERT_TRUE(c[1] > 0.8f);
    TEST_ASSERT_TRUE(c[2] > 0.7f);
}

/* 26. Globular cluster -> yellowish-white */
void test_type_color_globular_cluster(void)
{
    float c[4];
    dsp_type_color(DSO_GLOBULAR_CLUSTER, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.8f);
    TEST_ASSERT_TRUE(c[1] > 0.8f);
}

/* 27. Dark nebula -> very dark */
void test_type_color_dark_nebula(void)
{
    float c[4];
    dsp_type_color(DSO_DARK_NEBULA, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] < 0.3f);
    TEST_ASSERT_TRUE(c[1] < 0.3f);
    TEST_ASSERT_TRUE(c[2] < 0.3f);
}

/* 28. Spiral galaxy -> pale blue-ish warm */
void test_type_color_spiral_galaxy(void)
{
    float c[4];
    dsp_type_color(DSO_SPIRAL_GALAXY, 1.0f, c);
    TEST_ASSERT_TRUE(c[2] > 0.8f);  /* blue channel high */
}

/* 29. Alpha is passed through for all types */
void test_type_color_alpha_passthrough(void)
{
    float c[4];
    dsp_type_color(DSO_ELLIPTICAL_GALAXY, 0.42f, c);
    FCLOSE(0.42f, c[3]);
}

/* 30. Supernova remnant -> bright pink/white */
void test_type_color_snr(void)
{
    float c[4];
    dsp_type_color(DSO_SUPERNOVA_REMNANT, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.8f);  /* strong red/pink */
    TEST_ASSERT_TRUE(c[1] > 0.4f);  /* some green */
}

/* 31. Irregular galaxy -> blue-white */
void test_type_color_irregular(void)
{
    float c[4];
    dsp_type_color(DSO_IRREGULAR_GALAXY, 1.0f, c);
    TEST_ASSERT_TRUE(c[2] > 0.9f);  /* strong blue */
}

/* ======================================================================
 * dsp_pack
 * ====================================================================== */

/* 32. Pack returns catalog count */
void test_pack_returns_catalog_count(void)
{
    dsp_config_t cfg = dsp_default_config();
    int n = dso_catalog_count();
    float buf[110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS];
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3]    = {0.0f, 1.0f, 0.0f};
    int packed = dsp_pack(cam_right, cam_up, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(n, packed);
}

/* 33. First object has 6 vertices, 9 floats each -> first 54 floats */
void test_pack_first_object_vertex_count(void)
{
    dsp_config_t cfg = dsp_default_config();
    float buf[110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3]    = {0.0f, 1.0f, 0.0f};
    dsp_pack(cam_right, cam_up, &cfg, buf);

    /* Vertices should be non-zero (positions on celestial sphere) */
    int non_zero = 0;
    for (int i = 0; i < DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS; i++) {
        if (buf[i] != 0.0f) non_zero++;
    }
    TEST_ASSERT_TRUE(non_zero > 0);
}

/* 34. UV coordinates of first quad form (0,0)(1,0)(1,1)(0,0)(1,1)(0,1) */
void test_pack_uv_coordinates(void)
{
    dsp_config_t cfg = dsp_default_config();
    float buf[110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS];
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3]    = {0.0f, 1.0f, 0.0f};
    dsp_pack(cam_right, cam_up, &cfg, buf);

    /* UV starts at offset 3 in each vertex (after vec3 pos) */
    /* Vertex 0 (BL): UV = (0,0) */
    FCLOSE(0.0f, buf[0 * DSP_VERTEX_FLOATS + 3]);
    FCLOSE(0.0f, buf[0 * DSP_VERTEX_FLOATS + 4]);
    /* Vertex 1 (BR): UV = (1,0) */
    FCLOSE(1.0f, buf[1 * DSP_VERTEX_FLOATS + 3]);
    FCLOSE(0.0f, buf[1 * DSP_VERTEX_FLOATS + 4]);
    /* Vertex 2 (TR): UV = (1,1) */
    FCLOSE(1.0f, buf[2 * DSP_VERTEX_FLOATS + 3]);
    FCLOSE(1.0f, buf[2 * DSP_VERTEX_FLOATS + 4]);
    /* Vertex 3 (BL again): UV = (0,0) */
    FCLOSE(0.0f, buf[3 * DSP_VERTEX_FLOATS + 3]);
    FCLOSE(0.0f, buf[3 * DSP_VERTEX_FLOATS + 4]);
    /* Vertex 4 (TR again): UV = (1,1) */
    FCLOSE(1.0f, buf[4 * DSP_VERTEX_FLOATS + 3]);
    FCLOSE(1.0f, buf[4 * DSP_VERTEX_FLOATS + 4]);
    /* Vertex 5 (TL): UV = (0,1) */
    FCLOSE(0.0f, buf[5 * DSP_VERTEX_FLOATS + 3]);
    FCLOSE(1.0f, buf[5 * DSP_VERTEX_FLOATS + 4]);
}

/* 35. Color channels are in range [0,1] */
void test_pack_colors_in_range(void)
{
    dsp_config_t cfg = dsp_default_config();
    float buf[110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS];
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3]    = {0.0f, 1.0f, 0.0f};
    int n = dsp_pack(cam_right, cam_up, &cfg, buf);

    for (int i = 0; i < n * DSP_VERTS_PER_OBJECT; i++) {
        int base = i * DSP_VERTEX_FLOATS;
        /* Color is at offset 5,6,7,8 (RGBA) */
        for (int c = 5; c <= 8; c++) {
            TEST_ASSERT_TRUE(buf[base + c] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + c] <= 1.0f);
        }
    }
}

/* 36. All positions on the celestial sphere at expected radius */
void test_pack_positions_on_sphere(void)
{
    dsp_config_t cfg = dsp_default_config();
    float buf[110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS];
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3]    = {0.0f, 1.0f, 0.0f};
    int n = dsp_pack(cam_right, cam_up, &cfg, buf);
    TEST_ASSERT_TRUE(n > 0);

    /* Positions are offset from the sphere center by billboard expansion.
     * The center of each quad should be approximately at sphere_radius distance.
     * Check the average of 6 vertices for the first object. */
    float avg[3] = {0.0f, 0.0f, 0.0f};
    for (int v = 0; v < DSP_VERTS_PER_OBJECT; v++) {
        int base = v * DSP_VERTEX_FLOATS;
        avg[0] += buf[base + 0];
        avg[1] += buf[base + 1];
        avg[2] += buf[base + 2];
    }
    avg[0] /= (float)DSP_VERTS_PER_OBJECT;
    avg[1] /= (float)DSP_VERTS_PER_OBJECT;
    avg[2] /= (float)DSP_VERTS_PER_OBJECT;
    float dist = sqrtf(avg[0] * avg[0] + avg[1] * avg[1] + avg[2] * avg[2]);
    /* Should be approximately sphere_radius */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, cfg.sphere_radius, dist);
}

/* ======================================================================
 * dsp_info
 * ====================================================================== */

/* 37. Info vertex count = object_count * 6 */
void test_info_vertex_count(void)
{
    dsp_info_t info = dsp_info(110);
    TEST_ASSERT_EQUAL_INT(110 * DSP_VERTS_PER_OBJECT, info.vertex_count);
}

/* 38. Info float count = vertex_count * 9 */
void test_info_float_count(void)
{
    dsp_info_t info = dsp_info(110);
    TEST_ASSERT_EQUAL_INT(110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS,
                          info.float_count);
}

/* 39. Info sub-counts sum to total */
void test_info_subcounts_sum(void)
{
    dsp_info_t info = dsp_info(110);
    TEST_ASSERT_EQUAL_INT(110, info.object_count);
    int sum = info.nebula_count + info.cluster_count + info.galaxy_count;
    TEST_ASSERT_EQUAL_INT(110, sum);
}

/* 40. Info nebula_count > 0 */
void test_info_has_nebulae(void)
{
    dsp_info_t info = dsp_info(110);
    TEST_ASSERT_TRUE(info.nebula_count > 0);
}

/* 41. Info cluster_count > 0 */
void test_info_has_clusters(void)
{
    dsp_info_t info = dsp_info(110);
    TEST_ASSERT_TRUE(info.cluster_count > 0);
}

/* 42. Info galaxy_count > 0 */
void test_info_has_galaxies(void)
{
    dsp_info_t info = dsp_info(110);
    TEST_ASSERT_TRUE(info.galaxy_count > 0);
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 43. Vertex shader source starts with #version 300 es */
void test_vert_source_version(void)
{
    const char *src = dsp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 44. Fragment shader source starts with #version 300 es */
void test_frag_source_version(void)
{
    const char *src = dsp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 45. Vertex shader contains u_mvp uniform */
void test_vert_source_has_mvp(void)
{
    const char *src = dsp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* 46. Fragment shader contains gaussian exp */
void test_frag_source_has_gaussian(void)
{
    const char *src = dsp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "exp("));
}

/* 47. Vertex shader has a_position attribute */
void test_vert_source_has_position(void)
{
    const char *src = dsp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

/* 48. Vertex shader has a_uv attribute */
void test_vert_source_has_uv(void)
{
    const char *src = dsp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_uv"));
}

/* 49. Vertex shader has a_color attribute */
void test_vert_source_has_color(void)
{
    const char *src = dsp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_color"));
}

/* 50. Fragment shader has frag_color output */
void test_frag_source_has_output(void)
{
    const char *src = dsp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* ======================================================================
 * Constants
 * ====================================================================== */

/* 51. DSP_VERTEX_FLOATS is 9 */
void test_vertex_floats_is_9(void)
{
    TEST_ASSERT_EQUAL_INT(9, DSP_VERTEX_FLOATS);
}

/* 52. DSP_VERTS_PER_OBJECT is 6 */
void test_verts_per_object_is_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, DSP_VERTS_PER_OBJECT);
}

/* 53. DSP_VERTEX_STRIDE is 36 */
void test_vertex_stride_is_36(void)
{
    TEST_ASSERT_EQUAL_INT(36, DSP_VERTEX_STRIDE);
}

/* ======================================================================
 * Edge cases
 * ====================================================================== */

/* 54. Billboard quads expand symmetrically from center */
void test_pack_quad_symmetry(void)
{
    dsp_config_t cfg = dsp_default_config();
    float buf[110 * DSP_VERTS_PER_OBJECT * DSP_VERTEX_FLOATS];
    float cam_right[3] = {1.0f, 0.0f, 0.0f};
    float cam_up[3]    = {0.0f, 1.0f, 0.0f};
    dsp_pack(cam_right, cam_up, &cfg, buf);

    /* For first object: center of quad = average of all 6 vertex positions.
     * BL and TR appear twice (in both triangles), TL and BR once each.
     * Avg = (BL+BR+TR+BL+TR+TL)/6 = (2*BL+BR+2*TR+TL)/6 = center.
     * Actually: (BL+BR+TR)/3 = center of first tri, (BL+TR+TL)/3 = center of second.
     * Both triangles share same center -> average of 6 vertices = center. */
    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    for (int v = 0; v < DSP_VERTS_PER_OBJECT; v++) {
        cx += buf[v * DSP_VERTEX_FLOATS + 0];
        cy += buf[v * DSP_VERTEX_FLOATS + 1];
        cz += buf[v * DSP_VERTEX_FLOATS + 2];
    }
    cx /= 6.0f;
    cy /= 6.0f;
    cz /= 6.0f;

    /* Center should be on the sphere */
    float dist = sqrtf(cx * cx + cy * cy + cz * cz);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, cfg.sphere_radius, dist);
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Config */
    RUN_TEST(test_default_config_sphere_radius);
    RUN_TEST(test_default_config_base_size);
    RUN_TEST(test_default_config_size_range);
    RUN_TEST(test_default_config_alpha_range);
    RUN_TEST(test_default_config_magnitude_range);

    /* RA/Dec to XYZ */
    RUN_TEST(test_ra_dec_zero_zero);
    RUN_TEST(test_ra_6h_dec_0);
    RUN_TEST(test_north_pole);
    RUN_TEST(test_ra_dec_radius_scaling);
    RUN_TEST(test_ra_dec_unit_sphere);
    RUN_TEST(test_south_pole);

    /* Billboard size */
    RUN_TEST(test_billboard_size_1_arcmin);
    RUN_TEST(test_billboard_size_clamp_max);
    RUN_TEST(test_billboard_size_clamp_min);
    RUN_TEST(test_billboard_size_proportional);
    RUN_TEST(test_billboard_size_zero);

    /* Magnitude alpha */
    RUN_TEST(test_magnitude_alpha_bright);
    RUN_TEST(test_magnitude_alpha_faint);
    RUN_TEST(test_magnitude_alpha_midpoint);
    RUN_TEST(test_magnitude_alpha_brighter_than_bright);
    RUN_TEST(test_magnitude_alpha_fainter_than_faint);

    /* Type color */
    RUN_TEST(test_type_color_emission);
    RUN_TEST(test_type_color_reflection);
    RUN_TEST(test_type_color_planetary);
    RUN_TEST(test_type_color_open_cluster);
    RUN_TEST(test_type_color_globular_cluster);
    RUN_TEST(test_type_color_dark_nebula);
    RUN_TEST(test_type_color_spiral_galaxy);
    RUN_TEST(test_type_color_alpha_passthrough);
    RUN_TEST(test_type_color_snr);
    RUN_TEST(test_type_color_irregular);

    /* Pack */
    RUN_TEST(test_pack_returns_catalog_count);
    RUN_TEST(test_pack_first_object_vertex_count);
    RUN_TEST(test_pack_uv_coordinates);
    RUN_TEST(test_pack_colors_in_range);
    RUN_TEST(test_pack_positions_on_sphere);

    /* Info */
    RUN_TEST(test_info_vertex_count);
    RUN_TEST(test_info_float_count);
    RUN_TEST(test_info_subcounts_sum);
    RUN_TEST(test_info_has_nebulae);
    RUN_TEST(test_info_has_clusters);
    RUN_TEST(test_info_has_galaxies);

    /* Shaders */
    RUN_TEST(test_vert_source_version);
    RUN_TEST(test_frag_source_version);
    RUN_TEST(test_vert_source_has_mvp);
    RUN_TEST(test_frag_source_has_gaussian);
    RUN_TEST(test_vert_source_has_position);
    RUN_TEST(test_vert_source_has_uv);
    RUN_TEST(test_vert_source_has_color);
    RUN_TEST(test_frag_source_has_output);

    /* Constants */
    RUN_TEST(test_vertex_floats_is_9);
    RUN_TEST(test_verts_per_object_is_6);
    RUN_TEST(test_vertex_stride_is_36);

    /* Edge cases */
    RUN_TEST(test_pack_quad_symmetry);

    return UNITY_END();
}
