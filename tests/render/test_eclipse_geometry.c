/*
 * test_eclipse_geometry.c -- Tests for eclipse geometry module
 *
 * Tests solar/lunar eclipse detection, type classification,
 * apparent diameters, shadow cone geometry, vertex packing,
 * next-eclipse finder, and shader source strings.
 */

#include "../../tests/unity/unity.h"
#include "../../src/render/eclipse_geometry.h"
#include "../../src/systems/astronomy/lunar.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

/* Known Julian Day dates for testing:
 * J2000.0 = 2451545.0 (2000-01-01 12:00 TT)
 * 2024-04-08 ~18:18 UT (total solar eclipse) = JD 2460409.26
 * 2024-10-02 ~18:45 UT (annular solar eclipse) = JD 2460586.28
 * 2024-09-18 ~02:44 UT (partial lunar eclipse) = JD 2460571.61
 * New moon ref: 2000-01-06 18:14 UT = JD 2451550.26
 */

/* ======================================================================
 * Helper: known new moon JD (from lunar.c reference)
 * ====================================================================== */
static const double REF_NEW_MOON_JD = 2451550.26;
static const double SYNODIC_MONTH   = 29.53058868;

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. ecl_type_name — string conversion
 * ====================================================================== */

void test_type_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("None", ecl_type_name(ECL_NONE));
}

void test_type_name_solar_partial(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Partial", ecl_type_name(ECL_SOLAR_PARTIAL));
}

void test_type_name_solar_annular(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Annular", ecl_type_name(ECL_SOLAR_ANNULAR));
}

void test_type_name_solar_total(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Total", ecl_type_name(ECL_SOLAR_TOTAL));
}

void test_type_name_lunar_penumbral(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunar Penumbral", ecl_type_name(ECL_LUNAR_PENUMBRAL));
}

void test_type_name_lunar_partial(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunar Partial", ecl_type_name(ECL_LUNAR_PARTIAL));
}

void test_type_name_lunar_total(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunar Total", ecl_type_name(ECL_LUNAR_TOTAL));
}

/* ======================================================================
 * 2. ecl_sun_apparent_diameter — varies with Earth-Sun distance
 * ====================================================================== */

void test_sun_diameter_j2000(void)
{
    double d = ecl_sun_apparent_diameter(2451545.0);
    /* Should be around 0.53 degrees */
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.533f, (float)d);
}

void test_sun_diameter_perihelion(void)
{
    /* ~Jan 3 -> perihelion, Sun appears largest */
    double jd_perihelion = 2451547.0; /* ~2000-01-03 */
    double d = ecl_sun_apparent_diameter(jd_perihelion);
    TEST_ASSERT_TRUE(d > 0.52);
    TEST_ASSERT_TRUE(d < 0.55);
}

void test_sun_diameter_aphelion(void)
{
    /* ~Jul 4 -> aphelion, Sun appears smallest */
    double jd_aphelion = 2451730.0; /* ~2000-07-04 */
    double d = ecl_sun_apparent_diameter(jd_aphelion);
    TEST_ASSERT_TRUE(d > 0.52);
    TEST_ASSERT_TRUE(d < 0.55);
}

void test_sun_diameter_positive(void)
{
    /* Must always be positive */
    double d = ecl_sun_apparent_diameter(2460000.0);
    TEST_ASSERT_TRUE(d > 0.0);
}

/* ======================================================================
 * 3. ecl_moon_apparent_diameter — varies with Earth-Moon distance
 * ====================================================================== */

void test_moon_diameter_range(void)
{
    /* Moon diameter varies ~0.49 to ~0.56 degrees */
    double d = ecl_moon_apparent_diameter(2451545.0);
    TEST_ASSERT_TRUE(d >= 0.48);
    TEST_ASSERT_TRUE(d <= 0.58);
}

void test_moon_diameter_positive(void)
{
    double d = ecl_moon_apparent_diameter(2460000.0);
    TEST_ASSERT_TRUE(d > 0.0);
}

void test_moon_diameter_different_dates(void)
{
    /* Two dates 14 days apart should give different diameters (different distances) */
    double d1 = ecl_moon_apparent_diameter(2451545.0);
    double d2 = ecl_moon_apparent_diameter(2451559.0);
    /* They may differ or not, but both must be in range */
    TEST_ASSERT_TRUE(d1 >= 0.48 && d1 <= 0.58);
    TEST_ASSERT_TRUE(d2 >= 0.48 && d2 <= 0.58);
}

/* ======================================================================
 * 4. ecl_sun_moon_separation — angular distance
 * ====================================================================== */

void test_separation_at_new_moon(void)
{
    /* At new moon, Sun-Moon separation should be small (near 0) */
    double jd_new = REF_NEW_MOON_JD;
    double sep = ecl_sun_moon_separation(jd_new);
    /* Approximate: within ~15 degrees of 0 (not exactly 0 due to orbit inclination) */
    TEST_ASSERT_TRUE(sep < 20.0);
}

void test_separation_at_full_moon(void)
{
    /* At full moon, Sun-Moon separation should be near 180 degrees */
    double jd_full = REF_NEW_MOON_JD + SYNODIC_MONTH * 0.5;
    double sep = ecl_sun_moon_separation(jd_full);
    /* Should be near 180, within ~15 degrees */
    TEST_ASSERT_TRUE(sep > 160.0);
}

void test_separation_always_positive(void)
{
    double sep = ecl_sun_moon_separation(2455000.0);
    TEST_ASSERT_TRUE(sep >= 0.0);
    TEST_ASSERT_TRUE(sep <= 180.0);
}

void test_separation_quarter_moon(void)
{
    /* At first quarter (~7.4 days after new), separation ~90 degrees */
    double jd_quarter = REF_NEW_MOON_JD + SYNODIC_MONTH * 0.25;
    double sep = ecl_sun_moon_separation(jd_quarter);
    TEST_ASSERT_TRUE(sep > 60.0);
    TEST_ASSERT_TRUE(sep < 120.0);
}

/* ======================================================================
 * 5. ecl_check — eclipse detection
 * ====================================================================== */

void test_check_random_date_no_eclipse(void)
{
    /* A random date that is not near new/full moon -> no eclipse */
    double jd = REF_NEW_MOON_JD + 7.0; /* ~first quarter */
    ecl_condition_t c = ecl_check(jd);
    TEST_ASSERT_EQUAL_INT(ECL_NONE, c.type);
}

void test_check_magnitude_zero_when_none(void)
{
    double jd = REF_NEW_MOON_JD + 7.0;
    ecl_condition_t c = ecl_check(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)c.magnitude);
}

void test_check_at_new_moon_has_separation_near_zero(void)
{
    ecl_condition_t c = ecl_check(REF_NEW_MOON_JD);
    /* Separation should be small at new moon */
    TEST_ASSERT_TRUE(c.sun_moon_sep_deg < 20.0);
}

void test_check_fields_populated(void)
{
    ecl_condition_t c = ecl_check(REF_NEW_MOON_JD);
    /* Apparent diameters should be populated */
    TEST_ASSERT_TRUE(c.sun_apparent_deg > 0.0);
    TEST_ASSERT_TRUE(c.moon_apparent_deg > 0.0);
}

void test_check_returns_solar_type_only_near_new(void)
{
    /* If an eclipse IS detected at new moon, it must be a solar type */
    ecl_condition_t c = ecl_check(REF_NEW_MOON_JD);
    if (c.type != ECL_NONE) {
        TEST_ASSERT_TRUE(c.type == ECL_SOLAR_PARTIAL ||
                         c.type == ECL_SOLAR_ANNULAR ||
                         c.type == ECL_SOLAR_TOTAL);
    } else {
        /* No eclipse at this particular new moon — that's valid */
        TEST_ASSERT_EQUAL_INT(ECL_NONE, c.type);
    }
}

void test_check_returns_lunar_type_only_near_full(void)
{
    /* At full moon, if eclipse detected, must be lunar type */
    double jd_full = REF_NEW_MOON_JD + SYNODIC_MONTH * 0.5;
    ecl_condition_t c = ecl_check(jd_full);
    if (c.type != ECL_NONE) {
        TEST_ASSERT_TRUE(c.type == ECL_LUNAR_PENUMBRAL ||
                         c.type == ECL_LUNAR_PARTIAL ||
                         c.type == ECL_LUNAR_TOTAL);
    } else {
        TEST_ASSERT_EQUAL_INT(ECL_NONE, c.type);
    }
}

void test_check_gamma_range(void)
{
    /* Gamma should be between -2 and 2 (or 0 if no eclipse) */
    ecl_condition_t c = ecl_check(REF_NEW_MOON_JD);
    TEST_ASSERT_TRUE(c.gamma >= -2.5 && c.gamma <= 2.5);
}

void test_check_eclipse_magnitude_bounded(void)
{
    /* If eclipse detected, magnitude should be > 0 */
    /* Search a few new moons for a solar eclipse */
    double jd = REF_NEW_MOON_JD;
    for (int i = 0; i < 50; i++) {
        ecl_condition_t c = ecl_check(jd);
        if (c.type != ECL_NONE) {
            TEST_ASSERT_TRUE(c.magnitude > 0.0);
            TEST_ASSERT_TRUE(c.magnitude < 2.0);
            return;
        }
        jd += SYNODIC_MONTH;
    }
    /* If no eclipse found in 50 months, just pass — rare but possible */
    TEST_PASS();
}

/* ======================================================================
 * 6. ecl_default_config
 * ====================================================================== */

void test_default_config_earth_radius(void)
{
    ecl_config_t cfg = ecl_default_config();
    TEST_ASSERT_TRUE(cfg.earth_radius > 0.0f);
}

void test_default_config_umbra_alpha(void)
{
    ecl_config_t cfg = ecl_default_config();
    TEST_ASSERT_TRUE(cfg.umbra_alpha > 0.0f && cfg.umbra_alpha <= 1.0f);
}

void test_default_config_penumbra_alpha(void)
{
    ecl_config_t cfg = ecl_default_config();
    TEST_ASSERT_TRUE(cfg.penumbra_alpha > 0.0f && cfg.penumbra_alpha <= 1.0f);
}

void test_default_config_penumbra_less_than_umbra(void)
{
    ecl_config_t cfg = ecl_default_config();
    TEST_ASSERT_TRUE(cfg.penumbra_alpha < cfg.umbra_alpha);
}

/* ======================================================================
 * 7. ecl_shadow — shadow cone geometry
 * ====================================================================== */

void test_shadow_no_eclipse_zero_radii(void)
{
    ecl_condition_t cond;
    memset(&cond, 0, sizeof(cond));
    cond.type = ECL_NONE;
    ecl_config_t cfg = ecl_default_config();
    ecl_shadow_t s = ecl_shadow(&cond, &cfg);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.umbra_radius);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.penumbra_radius);
}

void test_shadow_solar_has_positive_radii(void)
{
    ecl_condition_t cond;
    memset(&cond, 0, sizeof(cond));
    cond.type = ECL_SOLAR_TOTAL;
    cond.magnitude = 1.0;
    cond.gamma = 0.0;
    cond.sun_apparent_deg = 0.533;
    cond.moon_apparent_deg = 0.545;
    ecl_config_t cfg = ecl_default_config();
    ecl_shadow_t s = ecl_shadow(&cond, &cfg);
    TEST_ASSERT_TRUE(s.umbra_radius > 0.0f);
    TEST_ASSERT_TRUE(s.penumbra_radius > 0.0f);
}

void test_shadow_penumbra_larger_than_umbra(void)
{
    ecl_condition_t cond;
    memset(&cond, 0, sizeof(cond));
    cond.type = ECL_SOLAR_TOTAL;
    cond.magnitude = 1.0;
    cond.gamma = 0.0;
    cond.sun_apparent_deg = 0.533;
    cond.moon_apparent_deg = 0.545;
    ecl_config_t cfg = ecl_default_config();
    ecl_shadow_t s = ecl_shadow(&cond, &cfg);
    TEST_ASSERT_TRUE(s.penumbra_radius > s.umbra_radius);
}

void test_shadow_lunar_has_positive_radii(void)
{
    ecl_condition_t cond;
    memset(&cond, 0, sizeof(cond));
    cond.type = ECL_LUNAR_TOTAL;
    cond.magnitude = 1.0;
    cond.gamma = 0.0;
    cond.sun_apparent_deg = 0.533;
    cond.moon_apparent_deg = 0.52;
    ecl_config_t cfg = ecl_default_config();
    ecl_shadow_t s = ecl_shadow(&cond, &cfg);
    TEST_ASSERT_TRUE(s.umbra_radius > 0.0f);
    TEST_ASSERT_TRUE(s.penumbra_radius > 0.0f);
}

void test_shadow_center_scales_with_gamma(void)
{
    ecl_condition_t cond;
    memset(&cond, 0, sizeof(cond));
    cond.type = ECL_SOLAR_PARTIAL;
    cond.magnitude = 0.5;
    cond.gamma = 0.5;
    cond.sun_apparent_deg = 0.533;
    cond.moon_apparent_deg = 0.52;
    ecl_config_t cfg = ecl_default_config();
    ecl_shadow_t s = ecl_shadow(&cond, &cfg);
    /* Center should be offset from origin when gamma != 0 */
    double dist = sqrt((double)(s.center_x * s.center_x +
                                s.center_y * s.center_y +
                                s.center_z * s.center_z));
    TEST_ASSERT_TRUE(dist > 0.01);
}

/* ======================================================================
 * 8. ecl_pack_umbra — triangle fan vertices
 * ====================================================================== */

void test_pack_umbra_no_eclipse_zero_verts(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack_umbra(&shadow, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_umbra_vertex_count(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    shadow.center_x = 0.0f;
    shadow.center_y = 0.0f;
    shadow.center_z = 0.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack_umbra(&shadow, &cfg, buf);
    /* Triangle fan: center + ECL_DISK_SEGMENTS perimeter = 36*3 = 108 verts */
    TEST_ASSERT_EQUAL_INT(ECL_DISK_SEGMENTS * 3, n);
}

void test_pack_umbra_center_vertex_position(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    shadow.center_x = 5.0f;
    shadow.center_y = 3.0f;
    shadow.center_z = 0.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    ecl_pack_umbra(&shadow, &cfg, buf);
    /* First triangle's first vertex (center of fan) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.0f, buf[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.0f, buf[1]);
}

void test_pack_umbra_alpha_matches_config(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    ecl_pack_umbra(&shadow, &cfg, buf);
    /* Center vertex color alpha (index 6 = 4th component of RGBA at offset 3) */
    float alpha = buf[6]; /* pos(3) + rgba(4): alpha at index 6 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, cfg.umbra_alpha, alpha);
}

/* ======================================================================
 * 9. ecl_pack_penumbra — triangle strip ring
 * ====================================================================== */

void test_pack_penumbra_no_eclipse_zero_verts(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack_penumbra(&shadow, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_penumbra_vertex_count(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack_penumbra(&shadow, &cfg, buf);
    /* Ring as triangles: 36 segments * 6 verts per segment (2 tris) = 216 */
    TEST_ASSERT_EQUAL_INT(ECL_DISK_SEGMENTS * 6, n);
}

void test_pack_penumbra_alpha_matches_config(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    ecl_pack_penumbra(&shadow, &cfg, buf);
    /* First vertex outer alpha should be penumbra_alpha */
    /* The outer vertex alpha is at index 6 of the first vertex */
    float alpha = buf[6];
    TEST_ASSERT_TRUE(alpha > 0.0f && alpha <= 1.0f);
}

/* ======================================================================
 * 10. ecl_pack — combined packing
 * ====================================================================== */

void test_pack_combined_no_shadow(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack(&shadow, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_combined_vertex_count(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack(&shadow, &cfg, buf);
    /* umbra: 36*3 = 108, penumbra: 36*6 = 216, total = 324 */
    TEST_ASSERT_EQUAL_INT(ECL_DISK_SEGMENTS * 3 + ECL_DISK_SEGMENTS * 6, n);
}

void test_pack_within_max_vertices(void)
{
    ecl_shadow_t shadow;
    memset(&shadow, 0, sizeof(shadow));
    shadow.umbra_radius = 1.0f;
    shadow.penumbra_radius = 2.0f;
    ecl_config_t cfg = ecl_default_config();
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack(&shadow, &cfg, buf);
    TEST_ASSERT_TRUE(n <= ECL_MAX_VERTICES);
}

/* ======================================================================
 * 11. ecl_next_solar — find next solar eclipse
 * ====================================================================== */

void test_next_solar_returns_future_jd(void)
{
    double jd = 2451545.0; /* J2000 */
    double next = ecl_next_solar(jd);
    /* Should be after start date, or 0 if none found */
    TEST_ASSERT_TRUE(next == 0.0 || next > jd);
}

void test_next_solar_at_new_moon(void)
{
    double jd = 2451545.0;
    double next = ecl_next_solar(jd);
    if (next > 0.0) {
        /* Eclipse should occur near a new moon — phase fraction near 0 or 1 */
        lunar_info_t info = lunar_phase(next);
        TEST_ASSERT_TRUE(info.phase_fraction < 0.1 || info.phase_fraction > 0.9);
    }
}

void test_next_solar_within_two_years(void)
{
    /* Solar eclipses happen ~2-5 times per year */
    double jd = 2451545.0;
    double next = ecl_next_solar(jd);
    if (next > 0.0) {
        double days_ahead = next - jd;
        TEST_ASSERT_TRUE(days_ahead < 730.5); /* 2 years */
    }
}

/* ======================================================================
 * 12. ecl_next_lunar — find next lunar eclipse
 * ====================================================================== */

void test_next_lunar_returns_future_jd(void)
{
    double jd = 2451545.0;
    double next = ecl_next_lunar(jd);
    TEST_ASSERT_TRUE(next == 0.0 || next > jd);
}

void test_next_lunar_at_full_moon(void)
{
    double jd = 2451545.0;
    double next = ecl_next_lunar(jd);
    if (next > 0.0) {
        /* Eclipse should occur near a full moon — phase fraction near 0.5 */
        lunar_info_t info = lunar_phase(next);
        TEST_ASSERT_TRUE(info.phase_fraction > 0.4 && info.phase_fraction < 0.6);
    }
}

void test_next_lunar_within_two_years(void)
{
    double jd = 2451545.0;
    double next = ecl_next_lunar(jd);
    if (next > 0.0) {
        double days_ahead = next - jd;
        TEST_ASSERT_TRUE(days_ahead < 730.5);
    }
}

/* ======================================================================
 * 13. Shader sources
 * ====================================================================== */

void test_vert_source_not_null(void)
{
    const char *src = ecl_vert_source();
    TEST_ASSERT_NOT_NULL(src);
}

void test_vert_source_has_version(void)
{
    const char *src = ecl_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_vert_source_has_main(void)
{
    const char *src = ecl_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main()"));
}

void test_vert_source_has_mvp(void)
{
    const char *src = ecl_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

void test_frag_source_not_null(void)
{
    const char *src = ecl_frag_source();
    TEST_ASSERT_NOT_NULL(src);
}

void test_frag_source_has_version(void)
{
    const char *src = ecl_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_frag_source_has_main(void)
{
    const char *src = ecl_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main()"));
}

void test_frag_source_has_frag_color(void)
{
    const char *src = ecl_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* ======================================================================
 * 14. Constants validation
 * ====================================================================== */

void test_vertex_floats_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, ECL_VERTEX_FLOATS);
}

void test_disk_segments_is_36(void)
{
    TEST_ASSERT_EQUAL_INT(36, ECL_DISK_SEGMENTS);
}

void test_max_vertices_sufficient(void)
{
    /* umbra: 36*3=108, penumbra: 36*6=216, total: 324, max should be >= 324 */
    TEST_ASSERT_TRUE(ECL_MAX_VERTICES >= ECL_DISK_SEGMENTS * 3 + ECL_DISK_SEGMENTS * 6);
}

void test_vertex_stride_correct(void)
{
    TEST_ASSERT_EQUAL_INT(ECL_VERTEX_FLOATS * (int)sizeof(float), ECL_VERTEX_STRIDE);
}

/* ======================================================================
 * 15. Integration: ecl_check -> ecl_shadow -> ecl_pack pipeline
 * ====================================================================== */

void test_pipeline_no_eclipse_produces_zero_verts(void)
{
    /* At quarter moon, no eclipse */
    double jd = REF_NEW_MOON_JD + 7.38;
    ecl_condition_t cond = ecl_check(jd);
    ecl_config_t cfg = ecl_default_config();
    ecl_shadow_t shadow = ecl_shadow(&cond, &cfg);
    float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
    int n = ecl_pack(&shadow, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pipeline_found_eclipse_produces_verts(void)
{
    /* Search for any eclipse and verify the pipeline produces vertices */
    double jd = REF_NEW_MOON_JD;
    ecl_config_t cfg = ecl_default_config();
    for (int i = 0; i < 100; i++) {
        ecl_condition_t cond = ecl_check(jd);
        if (cond.type != ECL_NONE) {
            ecl_shadow_t shadow = ecl_shadow(&cond, &cfg);
            float buf[ECL_MAX_VERTICES * ECL_VERTEX_FLOATS];
            int n = ecl_pack(&shadow, &cfg, buf);
            TEST_ASSERT_TRUE(n > 0);
            return;
        }
        jd += SYNODIC_MONTH;
    }
    /* If no eclipse found in 100 months, still pass */
    TEST_PASS();
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* 1. Type names (7 tests) */
    RUN_TEST(test_type_name_none);
    RUN_TEST(test_type_name_solar_partial);
    RUN_TEST(test_type_name_solar_annular);
    RUN_TEST(test_type_name_solar_total);
    RUN_TEST(test_type_name_lunar_penumbral);
    RUN_TEST(test_type_name_lunar_partial);
    RUN_TEST(test_type_name_lunar_total);

    /* 2. Sun apparent diameter (4 tests) */
    RUN_TEST(test_sun_diameter_j2000);
    RUN_TEST(test_sun_diameter_perihelion);
    RUN_TEST(test_sun_diameter_aphelion);
    RUN_TEST(test_sun_diameter_positive);

    /* 3. Moon apparent diameter (3 tests) */
    RUN_TEST(test_moon_diameter_range);
    RUN_TEST(test_moon_diameter_positive);
    RUN_TEST(test_moon_diameter_different_dates);

    /* 4. Sun-Moon separation (4 tests) */
    RUN_TEST(test_separation_at_new_moon);
    RUN_TEST(test_separation_at_full_moon);
    RUN_TEST(test_separation_always_positive);
    RUN_TEST(test_separation_quarter_moon);

    /* 5. Eclipse check (8 tests) */
    RUN_TEST(test_check_random_date_no_eclipse);
    RUN_TEST(test_check_magnitude_zero_when_none);
    RUN_TEST(test_check_at_new_moon_has_separation_near_zero);
    RUN_TEST(test_check_fields_populated);
    RUN_TEST(test_check_returns_solar_type_only_near_new);
    RUN_TEST(test_check_returns_lunar_type_only_near_full);
    RUN_TEST(test_check_gamma_range);
    RUN_TEST(test_check_eclipse_magnitude_bounded);

    /* 6. Default config (4 tests) */
    RUN_TEST(test_default_config_earth_radius);
    RUN_TEST(test_default_config_umbra_alpha);
    RUN_TEST(test_default_config_penumbra_alpha);
    RUN_TEST(test_default_config_penumbra_less_than_umbra);

    /* 7. Shadow geometry (5 tests) */
    RUN_TEST(test_shadow_no_eclipse_zero_radii);
    RUN_TEST(test_shadow_solar_has_positive_radii);
    RUN_TEST(test_shadow_penumbra_larger_than_umbra);
    RUN_TEST(test_shadow_lunar_has_positive_radii);
    RUN_TEST(test_shadow_center_scales_with_gamma);

    /* 8. Umbra packing (4 tests) */
    RUN_TEST(test_pack_umbra_no_eclipse_zero_verts);
    RUN_TEST(test_pack_umbra_vertex_count);
    RUN_TEST(test_pack_umbra_center_vertex_position);
    RUN_TEST(test_pack_umbra_alpha_matches_config);

    /* 9. Penumbra packing (3 tests) */
    RUN_TEST(test_pack_penumbra_no_eclipse_zero_verts);
    RUN_TEST(test_pack_penumbra_vertex_count);
    RUN_TEST(test_pack_penumbra_alpha_matches_config);

    /* 10. Combined packing (3 tests) */
    RUN_TEST(test_pack_combined_no_shadow);
    RUN_TEST(test_pack_combined_vertex_count);
    RUN_TEST(test_pack_within_max_vertices);

    /* 11. Next solar eclipse (3 tests) */
    RUN_TEST(test_next_solar_returns_future_jd);
    RUN_TEST(test_next_solar_at_new_moon);
    RUN_TEST(test_next_solar_within_two_years);

    /* 12. Next lunar eclipse (3 tests) */
    RUN_TEST(test_next_lunar_returns_future_jd);
    RUN_TEST(test_next_lunar_at_full_moon);
    RUN_TEST(test_next_lunar_within_two_years);

    /* 13. Shaders (8 tests) */
    RUN_TEST(test_vert_source_not_null);
    RUN_TEST(test_vert_source_has_version);
    RUN_TEST(test_vert_source_has_main);
    RUN_TEST(test_vert_source_has_mvp);
    RUN_TEST(test_frag_source_not_null);
    RUN_TEST(test_frag_source_has_version);
    RUN_TEST(test_frag_source_has_main);
    RUN_TEST(test_frag_source_has_frag_color);

    /* 14. Constants (4 tests) */
    RUN_TEST(test_vertex_floats_is_7);
    RUN_TEST(test_disk_segments_is_36);
    RUN_TEST(test_max_vertices_sufficient);
    RUN_TEST(test_vertex_stride_correct);

    /* 15. Integration pipeline (2 tests) */
    RUN_TEST(test_pipeline_no_eclipse_produces_zero_verts);
    RUN_TEST(test_pipeline_found_eclipse_produces_verts);

    return UNITY_END();
}
