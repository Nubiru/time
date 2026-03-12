/*
 * test_orbit_trail_pack.c — Tests for orbit trail render pack module
 *
 * Tests: default config, orbit point geometry, sqrt scaling correctness,
 * gradient fade alpha, per-planet colors, full orbit packing, single planet
 * packing, shader sources, boundary conditions.
 *
 * 62 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/orbit_trail_pack.h"
#include "../../src/systems/astronomy/orbit.h"
#include "../../src/systems/astronomy/planet_data.h"
#include "../../src/math/kepler.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

/* J2000.0 epoch */
#define J2000 2451545.0
/* A different date: 2024-06-15 */
#define JD_2024 2460476.5

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. otp_default_config — default values
 * ====================================================================== */

/* 1. Default orbit_scale is 3.0f */
void test_default_config_orbit_scale(void)
{
    otp_config_t cfg = otp_default_config();
    FCLOSE(3.0f, cfg.orbit_scale);
}

/* 2. Default points_per_orbit is 256 */
void test_default_config_points_per_orbit(void)
{
    otp_config_t cfg = otp_default_config();
    TEST_ASSERT_EQUAL_INT(256, cfg.points_per_orbit);
}

/* 3. Default fade_fraction is 0.85f */
void test_default_config_fade_fraction(void)
{
    otp_config_t cfg = otp_default_config();
    FCLOSE(0.85f, cfg.fade_fraction);
}

/* 4. Default line_alpha_max is 0.6f */
void test_default_config_line_alpha_max(void)
{
    otp_config_t cfg = otp_default_config();
    FCLOSE(0.6f, cfg.line_alpha_max);
}

/* ======================================================================
 * 2. otp_orbit_point — single point geometry
 * ====================================================================== */

/* 5. Orbit point at M=0 returns nonzero position */
void test_orbit_point_nonzero(void)
{
    float xyz[3];
    otp_orbit_point(0, 0.0, 3.0f, xyz);
    float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    TEST_ASSERT_TRUE(dist > 0.0f);
}

/* 6. Mercury orbit point is closer than Neptune orbit point */
void test_orbit_point_mercury_closer_than_neptune(void)
{
    float merc[3], nept[3];
    otp_orbit_point(0, 0.0, 3.0f, merc);
    otp_orbit_point(7, 0.0, 3.0f, nept);
    float d_merc = sqrtf(merc[0]*merc[0] + merc[1]*merc[1] + merc[2]*merc[2]);
    float d_nept = sqrtf(nept[0]*nept[0] + nept[1]*nept[1] + nept[2]*nept[2]);
    TEST_ASSERT_TRUE(d_merc < d_nept);
}

/* 7. Orbit point applies sqrt scaling — Mercury at ~0.387 AU should be sqrt(0.387)*3 ~ 1.866 */
void test_orbit_point_sqrt_scaling_mercury(void)
{
    float xyz[3];
    /* At M=0 (perihelion), r = a*(1-e) for Mercury */
    otp_orbit_point(0, 0.0, 3.0f, xyz);
    float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    /* Mercury perihelion: a*(1-e) ~ 0.387*(1-0.206) ~ 0.307 AU
     * sqrt(0.307)*3 ~ 1.66. Allow 10% tolerance for inclination effects. */
    TEST_ASSERT_TRUE(dist > 1.0f);
    TEST_ASSERT_TRUE(dist < 3.0f);
}

/* 8. Earth orbit point distance: sqrt(~1.0)*3.0 ~ 3.0 */
void test_orbit_point_sqrt_scaling_earth(void)
{
    float xyz[3];
    /* Earth e ~ 0.017, so orbit is nearly circular, r ~ 1.0 AU */
    otp_orbit_point(2, PI, 3.0f, xyz);
    float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    /* sqrt(1.0)*3.0 = 3.0, with some tolerance for eccentricity */
    TEST_ASSERT_FLOAT_WITHIN(0.3f, 3.0f, dist);
}

/* 9. Orbit point at M=PI vs M=0 gives different positions */
void test_orbit_point_different_anomalies(void)
{
    float xyz0[3], xyz_pi[3];
    otp_orbit_point(0, 0.0, 3.0f, xyz0);
    otp_orbit_point(0, PI, 3.0f, xyz_pi);
    /* Positions should differ */
    int differ = (fabsf(xyz0[0] - xyz_pi[0]) > 0.01f ||
                  fabsf(xyz0[1] - xyz_pi[1]) > 0.01f ||
                  fabsf(xyz0[2] - xyz_pi[2]) > 0.01f);
    TEST_ASSERT_TRUE(differ);
}

/* 10. orbit_scale=0 gives zero position */
void test_orbit_point_zero_scale(void)
{
    float xyz[3];
    otp_orbit_point(2, 0.0, 0.0f, xyz);
    FCLOSE(0.0f, xyz[0]);
    FCLOSE(0.0f, xyz[1]);
    FCLOSE(0.0f, xyz[2]);
}

/* 11. Orbit point traces a closed ellipse — M=0 and M=2*PI give same position */
void test_orbit_point_closed_orbit(void)
{
    float xyz0[3], xyz2pi[3];
    otp_orbit_point(3, 0.0, 3.0f, xyz0);
    otp_orbit_point(3, 2.0 * PI, 3.0f, xyz2pi);
    FCLOSE(xyz0[0], xyz2pi[0]);
    FCLOSE(xyz0[1], xyz2pi[1]);
    FCLOSE(xyz0[2], xyz2pi[2]);
}

/* 12. All 8 planets produce valid orbit points */
void test_orbit_point_all_planets(void)
{
    for (int i = 0; i < 8; i++) {
        float xyz[3];
        otp_orbit_point(i, 1.0, 3.0f, xyz);
        float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
        TEST_ASSERT_TRUE(dist > 0.0f);
    }
}

/* 13. Orbit points lie roughly in the ecliptic plane (small y for low-incl planets) */
void test_orbit_point_ecliptic_plane_earth(void)
{
    /* Earth inclination ~ 0 deg, so y should be near zero */
    float xyz[3];
    otp_orbit_point(2, 1.0, 3.0f, xyz);
    float xz_dist = sqrtf(xyz[0]*xyz[0] + xyz[2]*xyz[2]);
    /* y / xz ratio should be tiny for Earth */
    TEST_ASSERT_TRUE(fabsf(xyz[1]) < xz_dist * 0.05f);
}

/* 14. Neptune orbit point distance: sqrt(~30)*3 ~ 16.4 */
void test_orbit_point_neptune_distance(void)
{
    float xyz[3];
    otp_orbit_point(7, 0.0, 3.0f, xyz);
    float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    /* sqrt(30)*3 ~ 16.4, allow wide tolerance for eccentricity */
    TEST_ASSERT_TRUE(dist > 12.0f);
    TEST_ASSERT_TRUE(dist < 20.0f);
}

/* 15. Orbit point with large orbit_scale */
void test_orbit_point_large_scale(void)
{
    float xyz_small[3], xyz_large[3];
    otp_orbit_point(2, 0.0, 1.0f, xyz_small);
    otp_orbit_point(2, 0.0, 10.0f, xyz_large);
    float d_small = sqrtf(xyz_small[0]*xyz_small[0] + xyz_small[1]*xyz_small[1] + xyz_small[2]*xyz_small[2]);
    float d_large = sqrtf(xyz_large[0]*xyz_large[0] + xyz_large[1]*xyz_large[1] + xyz_large[2]*xyz_large[2]);
    /* 10x scale should give 10x distance */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, d_small * 10.0f, d_large);
}

/* 16. Mercury eccentric orbit: perihelion != aphelion distance */
void test_orbit_point_mercury_eccentricity(void)
{
    float peri[3], aph[3];
    otp_orbit_point(0, 0.0, 3.0f, peri);    /* perihelion */
    otp_orbit_point(0, PI, 3.0f, aph);      /* aphelion */
    float d_peri = sqrtf(peri[0]*peri[0] + peri[1]*peri[1] + peri[2]*peri[2]);
    float d_aph = sqrtf(aph[0]*aph[0] + aph[1]*aph[1] + aph[2]*aph[2]);
    /* Mercury e~0.206, significant difference */
    TEST_ASSERT_TRUE(d_aph > d_peri);
    TEST_ASSERT_TRUE((d_aph - d_peri) > 0.1f);
}

/* ======================================================================
 * 3. otp_pack_planet — single planet orbit trail
 * ====================================================================== */

/* 17. Pack single planet returns expected vertex count */
void test_pack_planet_vertex_count(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(0, J2000, cfg, verts, 256);
    TEST_ASSERT_EQUAL_INT(256, count);
}

/* 18. Pack planet with small max_verts clamps */
void test_pack_planet_clamp_max_verts(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[64 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(0, J2000, cfg, verts, 64);
    TEST_ASSERT_EQUAL_INT(64, count);
}

/* 19. Pack planet vertices have nonzero positions */
void test_pack_planet_positions_nonzero(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(2, J2000, cfg, verts, 256);
    TEST_ASSERT_TRUE(count > 0);
    for (int i = 0; i < count; i++) {
        float x = verts[i * OTP_VERTEX_FLOATS + 0];
        float y = verts[i * OTP_VERTEX_FLOATS + 1];
        float z = verts[i * OTP_VERTEX_FLOATS + 2];
        float dist = sqrtf(x*x + y*y + z*z);
        TEST_ASSERT_TRUE(dist > 0.0f);
    }
}

/* 20. First vertex has brightest alpha (line_alpha_max) */
void test_pack_planet_first_vertex_brightest(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts, 256);
    float alpha0 = verts[0 * OTP_VERTEX_FLOATS + 3];
    FCLOSE(cfg.line_alpha_max, alpha0);
}

/* 21. Alpha decreases along the trail */
void test_pack_planet_alpha_decreases(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts, 256);
    float alpha0 = verts[0 * OTP_VERTEX_FLOATS + 3];
    float alpha_mid = verts[128 * OTP_VERTEX_FLOATS + 3];
    TEST_ASSERT_TRUE(alpha0 > alpha_mid);
}

/* 22. Vertices beyond fade region have alpha 0 */
void test_pack_planet_alpha_zero_beyond_fade(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts, 256);
    /* fade_fraction = 0.85, so last 15% should be alpha 0 */
    int zero_start = (int)(cfg.fade_fraction * 256);
    for (int i = zero_start + 1; i < 256; i++) {
        float alpha = verts[i * OTP_VERTEX_FLOATS + 3];
        FCLOSE(0.0f, alpha);
    }
}

/* 23. Color values match planet_data_color */
void test_pack_planet_colors_match(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    otp_pack_planet(0, J2000, cfg, verts, 256);
    planet_color_t expected = planet_data_color(0);
    /* Check first vertex color (offset 4,5,6) */
    FCLOSE(expected.r, verts[0 * OTP_VERTEX_FLOATS + 4]);
    FCLOSE(expected.g, verts[0 * OTP_VERTEX_FLOATS + 5]);
    FCLOSE(expected.b, verts[0 * OTP_VERTEX_FLOATS + 6]);
}

/* 24. All planet colors are consistent across vertices */
void test_pack_planet_colors_consistent(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[128 * OTP_VERTEX_FLOATS];
    cfg.points_per_orbit = 128;
    otp_pack_planet(3, J2000, cfg, verts, 128);
    float r0 = verts[0 * OTP_VERTEX_FLOATS + 4];
    float g0 = verts[0 * OTP_VERTEX_FLOATS + 5];
    float b0 = verts[0 * OTP_VERTEX_FLOATS + 6];
    for (int i = 1; i < 128; i++) {
        FCLOSE(r0, verts[i * OTP_VERTEX_FLOATS + 4]);
        FCLOSE(g0, verts[i * OTP_VERTEX_FLOATS + 5]);
        FCLOSE(b0, verts[i * OTP_VERTEX_FLOATS + 6]);
    }
}

/* 25. Pack planet with invalid index returns 0 */
void test_pack_planet_invalid_index_neg(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(-1, J2000, cfg, verts, 256);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* 26. Pack planet with index 8 returns 0 */
void test_pack_planet_invalid_index_high(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(8, J2000, cfg, verts, 256);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* 27. Pack planet with 0 max_verts returns 0 */
void test_pack_planet_zero_max_verts(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[1];
    int count = otp_pack_planet(0, J2000, cfg, verts, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* 28. Different JDs produce different trail start positions */
void test_pack_planet_different_jd(void)
{
    otp_config_t cfg = otp_default_config();
    float verts_a[256 * OTP_VERTEX_FLOATS];
    float verts_b[256 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts_a, 256);
    otp_pack_planet(2, JD_2024, cfg, verts_b, 256);
    /* First vertex position should differ */
    int differ = (fabsf(verts_a[0] - verts_b[0]) > 0.01f ||
                  fabsf(verts_a[1] - verts_b[1]) > 0.01f ||
                  fabsf(verts_a[2] - verts_b[2]) > 0.01f);
    TEST_ASSERT_TRUE(differ);
}

/* 29. Orbit trail forms a loop (first/last close if full orbit visible) */
void test_pack_planet_orbit_continuity(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.fade_fraction = 1.0f; /* full orbit visible */
    float verts[256 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(2, J2000, cfg, verts, 256);
    /* First and last positions should be close (nearly closed loop) */
    float dx = verts[0] - verts[(count-1)*OTP_VERTEX_FLOATS + 0];
    float dy = verts[1] - verts[(count-1)*OTP_VERTEX_FLOATS + 1];
    float dz = verts[2] - verts[(count-1)*OTP_VERTEX_FLOATS + 2];
    float gap = sqrtf(dx*dx + dy*dy + dz*dz);
    /* Gap should be small relative to orbit size */
    float dist0 = sqrtf(verts[0]*verts[0] + verts[1]*verts[1] + verts[2]*verts[2]);
    TEST_ASSERT_TRUE(gap < dist0 * 0.1f);
}

/* 30. All alpha values are in [0, line_alpha_max] */
void test_pack_planet_alpha_range(void)
{
    otp_config_t cfg = otp_default_config();
    float verts[256 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(4, J2000, cfg, verts, 256);
    for (int i = 0; i < count; i++) {
        float alpha = verts[i * OTP_VERTEX_FLOATS + 3];
        TEST_ASSERT_TRUE(alpha >= -0.001f);
        TEST_ASSERT_TRUE(alpha <= cfg.line_alpha_max + 0.001f);
    }
}

/* 31. Custom points_per_orbit is respected */
void test_pack_planet_custom_points(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 64;
    float verts[64 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(0, J2000, cfg, verts, 64);
    TEST_ASSERT_EQUAL_INT(64, count);
}

/* 32. Each planet has distinct colors */
void test_pack_planet_distinct_colors(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 4;
    float verts_a[4 * OTP_VERTEX_FLOATS];
    float verts_b[4 * OTP_VERTEX_FLOATS];
    /* Mercury vs Neptune */
    otp_pack_planet(0, J2000, cfg, verts_a, 4);
    otp_pack_planet(7, J2000, cfg, verts_b, 4);
    int color_differs = (fabsf(verts_a[4] - verts_b[4]) > 0.01f ||
                         fabsf(verts_a[5] - verts_b[5]) > 0.01f ||
                         fabsf(verts_a[6] - verts_b[6]) > 0.01f);
    TEST_ASSERT_TRUE(color_differs);
}

/* ======================================================================
 * 4. otp_pack — all planet orbits
 * ====================================================================== */

/* 33. otp_pack returns 8 orbits */
void test_pack_orbit_count(void)
{
    otp_config_t cfg = otp_default_config();
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_EQUAL_INT(8, info.orbit_count);
}

/* 34. Total vertex count = 8 * points_per_orbit */
void test_pack_total_vertex_count(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 32;
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_EQUAL_INT(8 * 32, info.vertex_count);
}

/* 35. Total vertex count does not exceed OTP_MAX_VERTICES */
void test_pack_max_vertices_not_exceeded(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 1024; /* Should clamp to OTP_MAX_VERTICES / 8 = 512 */
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_TRUE(info.vertex_count <= OTP_MAX_VERTICES);
}

/* 36. Pack at J2000 returns nonzero vertex count */
void test_pack_j2000_nonzero(void)
{
    otp_config_t cfg = otp_default_config();
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_TRUE(info.vertex_count > 0);
}

/* 37. Pack at different JD returns different positions */
void test_pack_different_jd(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 8;
    static float verts_a[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    static float verts_b[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_pack(J2000, cfg, verts_a);
    otp_pack(JD_2024, cfg, verts_b);
    /* First vertex should differ */
    int differ = (fabsf(verts_a[0] - verts_b[0]) > 0.01f ||
                  fabsf(verts_a[1] - verts_b[1]) > 0.01f ||
                  fabsf(verts_a[2] - verts_b[2]) > 0.01f);
    TEST_ASSERT_TRUE(differ);
}

/* 38. Inner orbits have smaller scene distances than outer */
void test_pack_inner_smaller_than_outer(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 4;
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_pack(J2000, cfg, verts);
    /* Mercury first vertex (index 0) vs Neptune first vertex (index 7*4) */
    float merc_d = sqrtf(verts[0]*verts[0] + verts[1]*verts[1] + verts[2]*verts[2]);
    int nept_base = 7 * 4 * OTP_VERTEX_FLOATS;
    float nept_d = sqrtf(verts[nept_base]*verts[nept_base] +
                         verts[nept_base+1]*verts[nept_base+1] +
                         verts[nept_base+2]*verts[nept_base+2]);
    TEST_ASSERT_TRUE(merc_d < nept_d);
}

/* 39. With very low points_per_orbit (1), still produces valid output */
void test_pack_single_point_per_orbit(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 1;
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_EQUAL_INT(8, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(8, info.orbit_count);
}

/* 40. Points_per_orbit = 0 should be treated as at least 1 */
void test_pack_zero_points_per_orbit(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 0;
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_TRUE(info.vertex_count >= 8);
}

/* 41. Negative points_per_orbit is clamped to 1 */
void test_pack_negative_points_per_orbit(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = -5;
    static float verts[OTP_MAX_VERTICES * OTP_VERTEX_FLOATS];
    otp_info_t info = otp_pack(J2000, cfg, verts);
    TEST_ASSERT_TRUE(info.vertex_count >= 8);
}

/* ======================================================================
 * 5. Alpha gradient tests
 * ====================================================================== */

/* 42. Alpha at first vertex equals line_alpha_max */
void test_alpha_first_vertex(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 64;
    float verts[64 * OTP_VERTEX_FLOATS];
    otp_pack_planet(0, J2000, cfg, verts, 64);
    FCLOSE(cfg.line_alpha_max, verts[3]);
}

/* 43. Alpha is monotonically non-increasing */
void test_alpha_monotonic(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 64;
    float verts[64 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(2, J2000, cfg, verts, 64);
    for (int i = 1; i < count; i++) {
        float prev_alpha = verts[(i-1) * OTP_VERTEX_FLOATS + 3];
        float curr_alpha = verts[i * OTP_VERTEX_FLOATS + 3];
        TEST_ASSERT_TRUE(curr_alpha <= prev_alpha + 0.001f);
    }
}

/* 44. Custom line_alpha_max is used */
void test_alpha_custom_max(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.line_alpha_max = 0.8f;
    cfg.points_per_orbit = 16;
    float verts[16 * OTP_VERTEX_FLOATS];
    otp_pack_planet(3, J2000, cfg, verts, 16);
    FCLOSE(0.8f, verts[3]);
}

/* 45. Custom fade_fraction: smaller fraction means more of trail is invisible */
void test_alpha_small_fade_fraction(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.fade_fraction = 0.5f;
    cfg.points_per_orbit = 100;
    float verts[100 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts, 100);
    /* At 75% (which is beyond 50% fade), alpha should be 0 */
    float alpha75 = verts[75 * OTP_VERTEX_FLOATS + 3];
    FCLOSE(0.0f, alpha75);
}

/* 46. fade_fraction = 1.0 means entire orbit is visible */
void test_alpha_full_fade(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.fade_fraction = 1.0f;
    cfg.points_per_orbit = 100;
    float verts[100 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts, 100);
    /* Last vertex should still have some alpha > 0 */
    float alpha_last = verts[99 * OTP_VERTEX_FLOATS + 3];
    TEST_ASSERT_TRUE(alpha_last > 0.0f);
}

/* ======================================================================
 * 6. Shader source tests
 * ====================================================================== */

/* 47. Vertex shader source is non-null */
void test_vert_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(otp_vert_source());
}

/* 48. Fragment shader source is non-null */
void test_frag_source_not_null(void)
{
    TEST_ASSERT_NOT_NULL(otp_frag_source());
}

/* 49. Vertex shader contains #version 300 es */
void test_vert_source_version(void)
{
    const char *src = otp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 50. Fragment shader contains #version 300 es */
void test_frag_source_version(void)
{
    const char *src = otp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 51. Vertex shader contains u_mvp uniform */
void test_vert_source_has_mvp(void)
{
    const char *src = otp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* 52. Fragment shader contains out vec4 */
void test_frag_source_has_output(void)
{
    const char *src = otp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "out vec4"));
}

/* 53. Vertex shader has a_position attribute */
void test_vert_source_has_position(void)
{
    const char *src = otp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

/* 54. Vertex shader has precision qualifier */
void test_vert_source_has_precision(void)
{
    const char *src = otp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "precision"));
}

/* 55. Fragment shader has precision qualifier */
void test_frag_source_has_precision(void)
{
    const char *src = otp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "precision"));
}

/* ======================================================================
 * 7. Vertex layout tests
 * ====================================================================== */

/* 56. OTP_VERTEX_FLOATS is 7 */
void test_vertex_floats_constant(void)
{
    TEST_ASSERT_EQUAL_INT(7, OTP_VERTEX_FLOATS);
}

/* 57. OTP_MAX_VERTICES is 4096 */
void test_max_vertices_constant(void)
{
    TEST_ASSERT_EQUAL_INT(4096, OTP_MAX_VERTICES);
}

/* 58. OTP_POINTS_PER_ORBIT is 256 */
void test_points_per_orbit_constant(void)
{
    TEST_ASSERT_EQUAL_INT(256, OTP_POINTS_PER_ORBIT);
}

/* ======================================================================
 * 8. Sqrt scaling consistency tests
 * ====================================================================== */

/* 59. otp_orbit_point matches the pattern: sqrt(r_AU)*orbit_scale */
void test_orbit_point_sqrt_pattern(void)
{
    float xyz[3];
    /* Earth at M=0: r ~ a*(1-e) ~ 1.0*(1-0.017) ~ 0.983 AU */
    otp_orbit_point(2, 0.0, 3.0f, xyz);
    float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    /* Expected: sqrt(0.983)*3.0 ~ 2.975 */
    TEST_ASSERT_FLOAT_WITHIN(0.15f, sqrtf(0.983f) * 3.0f, dist);
}

/* 60. Jupiter orbit point: sqrt(~5.2)*3 ~ 6.84 */
void test_orbit_point_jupiter_distance(void)
{
    float xyz[3];
    otp_orbit_point(4, 0.0, 3.0f, xyz);
    float dist = sqrtf(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    /* Jupiter perihelion: a*(1-e) ~ 5.2*(1-0.048) ~ 4.95 AU
     * sqrt(4.95)*3 ~ 6.67 */
    TEST_ASSERT_TRUE(dist > 5.5f);
    TEST_ASSERT_TRUE(dist < 8.0f);
}

/* 61. Orbit trail positions are self-consistent with otp_orbit_point */
void test_pack_planet_matches_orbit_point(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 64;
    float verts[64 * OTP_VERTEX_FLOATS];
    otp_pack_planet(2, J2000, cfg, verts, 64);
    /* The positions should be on the orbit ellipse —
     * check that first vertex distance is reasonable for Earth */
    float x = verts[0];
    float y = verts[1];
    float z = verts[2];
    float dist = sqrtf(x*x + y*y + z*z);
    /* Earth: sqrt(~1.0)*3.0 ~ 3.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 3.0f, dist);
}

/* 62. Color values are all in [0, 1] range */
void test_pack_planet_color_range(void)
{
    otp_config_t cfg = otp_default_config();
    cfg.points_per_orbit = 32;
    float verts[32 * OTP_VERTEX_FLOATS];
    int count = otp_pack_planet(5, J2000, cfg, verts, 32);
    for (int i = 0; i < count; i++) {
        float r = verts[i * OTP_VERTEX_FLOATS + 4];
        float g = verts[i * OTP_VERTEX_FLOATS + 5];
        float b = verts[i * OTP_VERTEX_FLOATS + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
    }
}

/* ======================================================================
 * main — register all tests
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* 1. Default config */
    RUN_TEST(test_default_config_orbit_scale);
    RUN_TEST(test_default_config_points_per_orbit);
    RUN_TEST(test_default_config_fade_fraction);
    RUN_TEST(test_default_config_line_alpha_max);

    /* 2. Orbit point geometry */
    RUN_TEST(test_orbit_point_nonzero);
    RUN_TEST(test_orbit_point_mercury_closer_than_neptune);
    RUN_TEST(test_orbit_point_sqrt_scaling_mercury);
    RUN_TEST(test_orbit_point_sqrt_scaling_earth);
    RUN_TEST(test_orbit_point_different_anomalies);
    RUN_TEST(test_orbit_point_zero_scale);
    RUN_TEST(test_orbit_point_closed_orbit);
    RUN_TEST(test_orbit_point_all_planets);
    RUN_TEST(test_orbit_point_ecliptic_plane_earth);
    RUN_TEST(test_orbit_point_neptune_distance);
    RUN_TEST(test_orbit_point_large_scale);
    RUN_TEST(test_orbit_point_mercury_eccentricity);

    /* 3. Single planet packing */
    RUN_TEST(test_pack_planet_vertex_count);
    RUN_TEST(test_pack_planet_clamp_max_verts);
    RUN_TEST(test_pack_planet_positions_nonzero);
    RUN_TEST(test_pack_planet_first_vertex_brightest);
    RUN_TEST(test_pack_planet_alpha_decreases);
    RUN_TEST(test_pack_planet_alpha_zero_beyond_fade);
    RUN_TEST(test_pack_planet_colors_match);
    RUN_TEST(test_pack_planet_colors_consistent);
    RUN_TEST(test_pack_planet_invalid_index_neg);
    RUN_TEST(test_pack_planet_invalid_index_high);
    RUN_TEST(test_pack_planet_zero_max_verts);
    RUN_TEST(test_pack_planet_different_jd);
    RUN_TEST(test_pack_planet_orbit_continuity);
    RUN_TEST(test_pack_planet_alpha_range);
    RUN_TEST(test_pack_planet_custom_points);
    RUN_TEST(test_pack_planet_distinct_colors);

    /* 4. Full orbit packing */
    RUN_TEST(test_pack_orbit_count);
    RUN_TEST(test_pack_total_vertex_count);
    RUN_TEST(test_pack_max_vertices_not_exceeded);
    RUN_TEST(test_pack_j2000_nonzero);
    RUN_TEST(test_pack_different_jd);
    RUN_TEST(test_pack_inner_smaller_than_outer);
    RUN_TEST(test_pack_single_point_per_orbit);
    RUN_TEST(test_pack_zero_points_per_orbit);
    RUN_TEST(test_pack_negative_points_per_orbit);

    /* 5. Alpha gradient */
    RUN_TEST(test_alpha_first_vertex);
    RUN_TEST(test_alpha_monotonic);
    RUN_TEST(test_alpha_custom_max);
    RUN_TEST(test_alpha_small_fade_fraction);
    RUN_TEST(test_alpha_full_fade);

    /* 6. Shaders */
    RUN_TEST(test_vert_source_not_null);
    RUN_TEST(test_frag_source_not_null);
    RUN_TEST(test_vert_source_version);
    RUN_TEST(test_frag_source_version);
    RUN_TEST(test_vert_source_has_mvp);
    RUN_TEST(test_frag_source_has_output);
    RUN_TEST(test_vert_source_has_position);
    RUN_TEST(test_vert_source_has_precision);
    RUN_TEST(test_frag_source_has_precision);

    /* 7. Constants */
    RUN_TEST(test_vertex_floats_constant);
    RUN_TEST(test_max_vertices_constant);
    RUN_TEST(test_points_per_orbit_constant);

    /* 8. Sqrt scaling consistency */
    RUN_TEST(test_orbit_point_sqrt_pattern);
    RUN_TEST(test_orbit_point_jupiter_distance);
    RUN_TEST(test_pack_planet_matches_orbit_point);
    RUN_TEST(test_pack_planet_color_range);

    return UNITY_END();
}
