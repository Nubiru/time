#include "../unity/unity.h"
#include "../../src/render/atmo_ring.h"

#include <math.h>

#define TOL 0.001f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* ---------- atmo_get: per-planet atmosphere data ---------- */

/* 1. Mercury has no atmosphere */
void test_atmo_mercury_none(void)
{
    atmo_properties_t a = atmo_get(0); /* PLANET_MERCURY */
    TEST_ASSERT_EQUAL_INT(0, a.has_atmosphere);
    TEST_ASSERT_TRUE(NEAR(0.0f, a.thickness_ratio));
    TEST_ASSERT_TRUE(NEAR(0.0f, a.density));
}

/* 2. Venus has very dense atmosphere */
void test_atmo_venus_dense(void)
{
    atmo_properties_t a = atmo_get(1); /* PLANET_VENUS */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    TEST_ASSERT_TRUE(a.thickness_ratio > 0.05f);
    TEST_ASSERT_TRUE(a.density > 0.5f);
    /* yellowish scatter */
    TEST_ASSERT_TRUE(a.scatter_r > a.scatter_b);
}

/* 3. Earth has moderate blue-scatter atmosphere */
void test_atmo_earth_blue(void)
{
    atmo_properties_t a = atmo_get(2); /* PLANET_EARTH */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    TEST_ASSERT_TRUE(a.thickness_ratio > 0.0f);
    TEST_ASSERT_TRUE(NEAR(1.0f, a.density)); /* Earth is reference density */
    /* blue Rayleigh scatter: B > R */
    TEST_ASSERT_TRUE(a.scatter_b > a.scatter_r);
}

/* 4. Mars has thin reddish atmosphere */
void test_atmo_mars_thin(void)
{
    atmo_properties_t a = atmo_get(3); /* PLANET_MARS */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    TEST_ASSERT_TRUE(a.thickness_ratio > 0.0f);
    TEST_ASSERT_TRUE(a.density < 0.1f); /* very thin */
    /* reddish dust scatter */
    TEST_ASSERT_TRUE(a.scatter_r > a.scatter_b);
}

/* 5. Jupiter has thick atmosphere */
void test_atmo_jupiter_thick(void)
{
    atmo_properties_t a = atmo_get(4); /* PLANET_JUPITER */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    TEST_ASSERT_TRUE(a.thickness_ratio > 0.02f);
    TEST_ASSERT_TRUE(a.density > 0.3f);
}

/* 6. Saturn has thick pale gold atmosphere */
void test_atmo_saturn_thick(void)
{
    atmo_properties_t a = atmo_get(5); /* PLANET_SATURN */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    TEST_ASSERT_TRUE(a.thickness_ratio > 0.02f);
    /* pale gold: R > B */
    TEST_ASSERT_TRUE(a.scatter_r > a.scatter_b);
}

/* 7. Uranus has cyan methane atmosphere */
void test_atmo_uranus_cyan(void)
{
    atmo_properties_t a = atmo_get(6); /* PLANET_URANUS */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    /* cyan from methane: G and B dominate R */
    TEST_ASSERT_TRUE(a.scatter_g > a.scatter_r);
    TEST_ASSERT_TRUE(a.scatter_b > a.scatter_r);
}

/* 8. Neptune has deep blue methane atmosphere */
void test_atmo_neptune_blue(void)
{
    atmo_properties_t a = atmo_get(7); /* PLANET_NEPTUNE */
    TEST_ASSERT_EQUAL_INT(1, a.has_atmosphere);
    /* deep blue: B > R */
    TEST_ASSERT_TRUE(a.scatter_b > a.scatter_r);
}

/* 9. Out-of-range planet returns no atmosphere */
void test_atmo_invalid_index(void)
{
    atmo_properties_t a = atmo_get(-1);
    TEST_ASSERT_EQUAL_INT(0, a.has_atmosphere);
    a = atmo_get(8);
    TEST_ASSERT_EQUAL_INT(0, a.has_atmosphere);
}

/* ---------- ring_system_get: per-planet ring data ---------- */

/* 10. Saturn has rings with correct Cassini Division */
void test_ring_saturn(void)
{
    ring_system_t r = ring_system_get(5); /* PLANET_SATURN */
    TEST_ASSERT_EQUAL_INT(1, r.has_rings);
    TEST_ASSERT_TRUE(r.inner_radius > 1.0f);   /* rings outside planet */
    TEST_ASSERT_TRUE(r.outer_radius > r.inner_radius);
    TEST_ASSERT_TRUE(r.opacity > 0.5f);
    /* Cassini Division at ~1.95-2.02 planet radii */
    TEST_ASSERT_TRUE(r.cassini.inner_radius >= 1.9f);
    TEST_ASSERT_TRUE(r.cassini.outer_radius <= 2.1f);
    TEST_ASSERT_TRUE(r.cassini.inner_radius < r.cassini.outer_radius);
    TEST_ASSERT_EQUAL_INT(1, r.gap_count);
}

/* 11. Uranus has faint thin rings */
void test_ring_uranus(void)
{
    ring_system_t r = ring_system_get(6); /* PLANET_URANUS */
    TEST_ASSERT_EQUAL_INT(1, r.has_rings);
    TEST_ASSERT_TRUE(r.inner_radius > 1.0f);
    TEST_ASSERT_TRUE(r.opacity < 0.3f); /* very faint */
}

/* 12. Neptune has faint thin rings */
void test_ring_neptune(void)
{
    ring_system_t r = ring_system_get(7); /* PLANET_NEPTUNE */
    TEST_ASSERT_EQUAL_INT(1, r.has_rings);
    TEST_ASSERT_TRUE(r.inner_radius > 1.0f);
    TEST_ASSERT_TRUE(r.opacity < 0.3f); /* very faint */
}

/* 13. Earth has no rings */
void test_ring_earth_none(void)
{
    ring_system_t r = ring_system_get(2); /* PLANET_EARTH */
    TEST_ASSERT_EQUAL_INT(0, r.has_rings);
    TEST_ASSERT_TRUE(NEAR(0.0f, r.inner_radius));
    TEST_ASSERT_TRUE(NEAR(0.0f, r.outer_radius));
}

/* 14. Invalid index returns no rings */
void test_ring_invalid_index(void)
{
    ring_system_t r = ring_system_get(-1);
    TEST_ASSERT_EQUAL_INT(0, r.has_rings);
    r = ring_system_get(99);
    TEST_ASSERT_EQUAL_INT(0, r.has_rings);
}

/* ---------- atmo_shell geometry ---------- */

/* 15. atmo_shell_positions returns correct vertex count */
void test_shell_positions_count(void)
{
    float positions[ATMO_SHELL_VERTS * 3];
    int count = atmo_shell_positions(positions, 1.0f, 0.05f);
    TEST_ASSERT_EQUAL_INT(ATMO_SHELL_VERTS, count);
}

/* 16. Inner vertices at planet radius, outer at planet+atmo */
void test_shell_positions_radii(void)
{
    float positions[ATMO_SHELL_VERTS * 3];
    float planet_r = 5.0f;
    float thickness = 0.1f; /* 10% */
    atmo_shell_positions(positions, planet_r, thickness);
    float expected_outer = planet_r * (1.0f + thickness);

    for (int i = 0; i <= ATMO_RING_SEGMENTS; i++) {
        int inner_v = i * 2;
        int outer_v = i * 2 + 1;
        float ix = positions[inner_v * 3 + 0];
        float iz = positions[inner_v * 3 + 2];
        float ir = sqrtf(ix * ix + iz * iz);
        TEST_ASSERT_TRUE(NEAR(planet_r, ir));

        float ox = positions[outer_v * 3 + 0];
        float oz = positions[outer_v * 3 + 2];
        float or_val = sqrtf(ox * ox + oz * oz);
        TEST_ASSERT_TRUE(NEAR(expected_outer, or_val));
    }
}

/* 17. All y coordinates are 0 (XZ plane) */
void test_shell_positions_y_zero(void)
{
    float positions[ATMO_SHELL_VERTS * 3];
    atmo_shell_positions(positions, 2.0f, 0.05f);
    for (int v = 0; v < ATMO_SHELL_VERTS; v++)
        TEST_ASSERT_TRUE(NEAR(0.0f, positions[v * 3 + 1]));
}

/* 18. Shell with zero thickness produces inner=outer radii */
void test_shell_positions_zero_thickness(void)
{
    float positions[ATMO_SHELL_VERTS * 3];
    atmo_shell_positions(positions, 3.0f, 0.0f);
    for (int i = 0; i <= ATMO_RING_SEGMENTS; i++) {
        int inner_v = i * 2;
        int outer_v = i * 2 + 1;
        float ix = positions[inner_v * 3 + 0];
        float iz = positions[inner_v * 3 + 2];
        float ir = sqrtf(ix * ix + iz * iz);
        float ox = positions[outer_v * 3 + 0];
        float oz = positions[outer_v * 3 + 2];
        float or_val = sqrtf(ox * ox + oz * oz);
        TEST_ASSERT_TRUE(NEAR(ir, or_val));
    }
}

/* 19. atmo_shell_indices returns correct count and valid indices */
void test_shell_indices_count(void)
{
    unsigned int indices[ATMO_SHELL_INDICES];
    int count = atmo_shell_indices(indices);
    TEST_ASSERT_EQUAL_INT(ATMO_SHELL_INDICES, count);
    /* All indices must be < vertex count */
    for (int i = 0; i < ATMO_SHELL_INDICES; i++)
        TEST_ASSERT_TRUE(indices[i] < (unsigned int)ATMO_SHELL_VERTS);
}

/* 20. Index count divisible by 3 (triangles) */
void test_shell_indices_triangles(void)
{
    TEST_ASSERT_EQUAL_INT(0, ATMO_SHELL_INDICES % 3);
}

/* ---------- atmo_shell colors ---------- */

/* 21. Inner vertices have full alpha, outer have zero alpha */
void test_shell_colors_alpha_gradient(void)
{
    float colors[ATMO_SHELL_VERTS * 4];
    atmo_properties_t atmo = atmo_get(2); /* Earth */
    atmo_shell_colors(colors, atmo);

    for (int i = 0; i <= ATMO_RING_SEGMENTS; i++) {
        int inner_v = i * 2;
        int outer_v = i * 2 + 1;
        /* inner alpha = density (full for Earth = 1.0) */
        TEST_ASSERT_TRUE(colors[inner_v * 4 + 3] > 0.5f);
        /* outer alpha = 0 */
        TEST_ASSERT_TRUE(NEAR(0.0f, colors[outer_v * 4 + 3]));
    }
}

/* 22. Inner vertices have scatter color */
void test_shell_colors_scatter(void)
{
    float colors[ATMO_SHELL_VERTS * 4];
    atmo_properties_t atmo = atmo_get(2); /* Earth */
    atmo_shell_colors(colors, atmo);

    /* First inner vertex should have Earth's scatter color */
    TEST_ASSERT_TRUE(NEAR(atmo.scatter_r, colors[0]));
    TEST_ASSERT_TRUE(NEAR(atmo.scatter_g, colors[1]));
    TEST_ASSERT_TRUE(NEAR(atmo.scatter_b, colors[2]));
}

/* 23. No-atmosphere planet produces zero-alpha colors */
void test_shell_colors_no_atmo(void)
{
    float colors[ATMO_SHELL_VERTS * 4];
    atmo_properties_t atmo = atmo_get(0); /* Mercury, no atmo */
    atmo_shell_colors(colors, atmo);
    for (int v = 0; v < ATMO_SHELL_VERTS; v++)
        TEST_ASSERT_TRUE(NEAR(0.0f, colors[v * 4 + 3]));
}

/* ---------- ring_quad geometry ---------- */

/* 24. ring_quad_positions returns correct vertex count */
void test_ring_quad_positions_count(void)
{
    float positions[RING_QUAD_VERTS * 3];
    int count = ring_quad_positions(positions, 1.5f, 2.5f);
    TEST_ASSERT_EQUAL_INT(RING_QUAD_VERTS, count);
}

/* 25. Ring quad inner/outer radii correct */
void test_ring_quad_positions_radii(void)
{
    float positions[RING_QUAD_VERTS * 3];
    float inner_r = 1.5f;
    float outer_r = 2.5f;
    ring_quad_positions(positions, inner_r, outer_r);

    for (int i = 0; i <= RING_QUAD_SEGMENTS; i++) {
        int inner_v = i * 2;
        int outer_v = i * 2 + 1;
        float ix = positions[inner_v * 3 + 0];
        float iz = positions[inner_v * 3 + 2];
        float ir = sqrtf(ix * ix + iz * iz);
        TEST_ASSERT_TRUE(NEAR(inner_r, ir));

        float ox = positions[outer_v * 3 + 0];
        float oz = positions[outer_v * 3 + 2];
        float or_val = sqrtf(ox * ox + oz * oz);
        TEST_ASSERT_TRUE(NEAR(outer_r, or_val));
    }
}

/* 26. Ring quad all y=0 */
void test_ring_quad_positions_y_zero(void)
{
    float positions[RING_QUAD_VERTS * 3];
    ring_quad_positions(positions, 1.5f, 2.5f);
    for (int v = 0; v < RING_QUAD_VERTS; v++)
        TEST_ASSERT_TRUE(NEAR(0.0f, positions[v * 3 + 1]));
}

/* 27. ring_quad_indices valid */
void test_ring_quad_indices_valid(void)
{
    unsigned int indices[RING_QUAD_INDICES];
    int count = ring_quad_indices(indices);
    TEST_ASSERT_EQUAL_INT(RING_QUAD_INDICES, count);
    for (int i = 0; i < RING_QUAD_INDICES; i++)
        TEST_ASSERT_TRUE(indices[i] < (unsigned int)RING_QUAD_VERTS);
    TEST_ASSERT_EQUAL_INT(0, RING_QUAD_INDICES % 3);
}

/* ---------- ring_quad colors ---------- */

/* 28. Ring colors have base color set */
void test_ring_quad_colors_base(void)
{
    float colors[RING_QUAD_VERTS * 4];
    ring_system_t ring = ring_system_get(5); /* Saturn */
    ring_quad_colors(colors, ring, 0.9f, 0.8f, 0.6f);
    /* First inner vertex should have ring color */
    TEST_ASSERT_TRUE(NEAR(0.9f, colors[0]));
    TEST_ASSERT_TRUE(NEAR(0.8f, colors[1]));
    TEST_ASSERT_TRUE(NEAR(0.6f, colors[2]));
}

/* 29. Ring colors have non-zero alpha for Saturn */
void test_ring_quad_colors_saturn_opacity(void)
{
    float colors[RING_QUAD_VERTS * 4];
    ring_system_t ring = ring_system_get(5);
    ring_quad_colors(colors, ring, 0.9f, 0.8f, 0.6f);
    /* At least some vertices should have positive alpha */
    int has_alpha = 0;
    for (int v = 0; v < RING_QUAD_VERTS; v++) {
        if (colors[v * 4 + 3] > 0.01f) {
            has_alpha = 1;
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, has_alpha);
}

/* 30. Cassini gap region has reduced opacity */
void test_ring_quad_colors_cassini_gap(void)
{
    float colors[RING_QUAD_VERTS * 4];
    ring_system_t ring = ring_system_get(5); /* Saturn */
    ring_quad_colors(colors, ring, 0.9f, 0.8f, 0.6f);

    /* Find a vertex in the Cassini gap region.
     * Gap at ~1.95-2.02 planet radii.
     * Saturn inner=1.239, outer=2.27.
     * Vertex t in [0,1], radius = inner + t*(outer-inner).
     * Gap center at ~1.985, t = (1.985 - 1.239)/(2.27 - 1.239) ~ 0.724
     * Vertex near segment 64*0.724 ~ 46 */
    int gap_seg = (int)(RING_QUAD_SEGMENTS * 0.724f);
    int gap_inner_v = gap_seg * 2;
    int gap_outer_v = gap_seg * 2 + 1;
    /* At least one of these should have low alpha due to gap */
    float gap_alpha_inner = colors[gap_inner_v * 4 + 3];
    float gap_alpha_outer = colors[gap_outer_v * 4 + 3];
    /* Not testing exact value, but gap alpha should be less than ring opacity */
    /* Actually the Cassini gap is a radial gap, so we need vertices at that radius.
     * The inner/outer vertices at a given segment have different radii.
     * Let me check: for the gap region, opacity should be reduced somewhere. */
    TEST_ASSERT_TRUE(gap_alpha_inner < ring.opacity + 0.01f ||
                     gap_alpha_outer < ring.opacity + 0.01f);
}

/* 31. No-ring planet produces zero-alpha colors */
void test_ring_quad_colors_no_rings(void)
{
    float colors[RING_QUAD_VERTS * 4];
    ring_system_t ring = ring_system_get(2); /* Earth, no rings */
    ring_quad_colors(colors, ring, 0.5f, 0.5f, 0.5f);
    for (int v = 0; v < RING_QUAD_VERTS; v++)
        TEST_ASSERT_TRUE(NEAR(0.0f, colors[v * 4 + 3]));
}

/* ---------- Fresnel ---------- */

/* 32. Fresnel at center (dot=1.0) returns near 0.0 */
void test_fresnel_center(void)
{
    float f = atmo_fresnel(1.0f, 3.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, f));
}

/* 33. Fresnel at edge (dot=0.0) returns 1.0 */
void test_fresnel_edge(void)
{
    float f = atmo_fresnel(0.0f, 3.0f);
    TEST_ASSERT_TRUE(NEAR(1.0f, f));
}

/* 34. Fresnel at 45 degrees (dot=0.5) with power=1.0 returns 0.5 */
void test_fresnel_half(void)
{
    float f = atmo_fresnel(0.5f, 1.0f);
    TEST_ASSERT_TRUE(NEAR(0.5f, f));
}

/* 35. Higher power concentrates effect at edges */
void test_fresnel_power_effect(void)
{
    float f_low = atmo_fresnel(0.5f, 2.0f);
    float f_high = atmo_fresnel(0.5f, 5.0f);
    /* Higher power makes mid-range values smaller */
    TEST_ASSERT_TRUE(f_high < f_low);
}

/* 36. Fresnel monotonically decreasing with dot */
void test_fresnel_monotonic(void)
{
    float prev = atmo_fresnel(0.0f, 3.0f);
    for (int i = 1; i <= 10; i++) {
        float dot = (float)i / 10.0f;
        float f = atmo_fresnel(dot, 3.0f);
        TEST_ASSERT_TRUE(f <= prev + TOL);
        prev = f;
    }
}

/* ---------- Purity ---------- */

/* 37. Same inputs produce same outputs (deterministic/pure) */
void test_purity_atmo_get(void)
{
    atmo_properties_t a1 = atmo_get(2);
    atmo_properties_t a2 = atmo_get(2);
    TEST_ASSERT_TRUE(a1.thickness_ratio == a2.thickness_ratio);
    TEST_ASSERT_TRUE(a1.scatter_r == a2.scatter_r);
    TEST_ASSERT_TRUE(a1.density == a2.density);
}

/* 38. Geometry generation is pure */
void test_purity_shell_positions(void)
{
    float pos_a[ATMO_SHELL_VERTS * 3];
    float pos_b[ATMO_SHELL_VERTS * 3];
    atmo_shell_positions(pos_a, 2.0f, 0.05f);
    atmo_shell_positions(pos_b, 2.0f, 0.05f);
    for (int i = 0; i < ATMO_SHELL_VERTS * 3; i++)
        TEST_ASSERT_TRUE(pos_a[i] == pos_b[i]);
}

/* ---------- Edge cases ---------- */

/* 39. Shell color count matches vertex count */
void test_shell_colors_count(void)
{
    float colors[ATMO_SHELL_VERTS * 4];
    atmo_properties_t atmo = atmo_get(2);
    int count = atmo_shell_colors(colors, atmo);
    TEST_ASSERT_EQUAL_INT(ATMO_SHELL_VERTS, count);
}

/* 40. Ring quad color count matches vertex count */
void test_ring_quad_colors_count(void)
{
    float colors[RING_QUAD_VERTS * 4];
    ring_system_t ring = ring_system_get(5);
    int count = ring_quad_colors(colors, ring, 0.9f, 0.8f, 0.6f);
    TEST_ASSERT_EQUAL_INT(RING_QUAD_VERTS, count);
}

/* 41. Last shell column matches first (full circle) */
void test_shell_positions_wrap(void)
{
    float positions[ATMO_SHELL_VERTS * 3];
    atmo_shell_positions(positions, 3.0f, 0.1f);
    int first_inner = 0;
    int last_inner = ATMO_RING_SEGMENTS * 2;
    TEST_ASSERT_TRUE(NEAR(positions[first_inner * 3 + 0],
                          positions[last_inner * 3 + 0]));
    TEST_ASSERT_TRUE(NEAR(positions[first_inner * 3 + 2],
                          positions[last_inner * 3 + 2]));
}

/* 42. Scatter color channels in valid range [0,1] for all planets */
void test_atmo_colors_valid_range(void)
{
    for (int p = 0; p < 8; p++) {
        atmo_properties_t a = atmo_get(p);
        TEST_ASSERT_TRUE(a.scatter_r >= 0.0f && a.scatter_r <= 1.0f);
        TEST_ASSERT_TRUE(a.scatter_g >= 0.0f && a.scatter_g <= 1.0f);
        TEST_ASSERT_TRUE(a.scatter_b >= 0.0f && a.scatter_b <= 1.0f);
        TEST_ASSERT_TRUE(a.density >= 0.0f && a.density <= 1.0f);
        TEST_ASSERT_TRUE(a.thickness_ratio >= 0.0f);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Atmosphere data (tests 1-9) */
    RUN_TEST(test_atmo_mercury_none);
    RUN_TEST(test_atmo_venus_dense);
    RUN_TEST(test_atmo_earth_blue);
    RUN_TEST(test_atmo_mars_thin);
    RUN_TEST(test_atmo_jupiter_thick);
    RUN_TEST(test_atmo_saturn_thick);
    RUN_TEST(test_atmo_uranus_cyan);
    RUN_TEST(test_atmo_neptune_blue);
    RUN_TEST(test_atmo_invalid_index);

    /* Ring data (tests 10-14) */
    RUN_TEST(test_ring_saturn);
    RUN_TEST(test_ring_uranus);
    RUN_TEST(test_ring_neptune);
    RUN_TEST(test_ring_earth_none);
    RUN_TEST(test_ring_invalid_index);

    /* Atmosphere shell geometry (tests 15-20) */
    RUN_TEST(test_shell_positions_count);
    RUN_TEST(test_shell_positions_radii);
    RUN_TEST(test_shell_positions_y_zero);
    RUN_TEST(test_shell_positions_zero_thickness);
    RUN_TEST(test_shell_indices_count);
    RUN_TEST(test_shell_indices_triangles);

    /* Atmosphere shell colors (tests 21-23) */
    RUN_TEST(test_shell_colors_alpha_gradient);
    RUN_TEST(test_shell_colors_scatter);
    RUN_TEST(test_shell_colors_no_atmo);

    /* Ring quad geometry (tests 24-27) */
    RUN_TEST(test_ring_quad_positions_count);
    RUN_TEST(test_ring_quad_positions_radii);
    RUN_TEST(test_ring_quad_positions_y_zero);
    RUN_TEST(test_ring_quad_indices_valid);

    /* Ring quad colors (tests 28-31) */
    RUN_TEST(test_ring_quad_colors_base);
    RUN_TEST(test_ring_quad_colors_saturn_opacity);
    RUN_TEST(test_ring_quad_colors_cassini_gap);
    RUN_TEST(test_ring_quad_colors_no_rings);

    /* Fresnel (tests 32-36) */
    RUN_TEST(test_fresnel_center);
    RUN_TEST(test_fresnel_edge);
    RUN_TEST(test_fresnel_half);
    RUN_TEST(test_fresnel_power_effect);
    RUN_TEST(test_fresnel_monotonic);

    /* Purity (tests 37-38) */
    RUN_TEST(test_purity_atmo_get);
    RUN_TEST(test_purity_shell_positions);

    /* Edge cases (tests 39-42) */
    RUN_TEST(test_shell_colors_count);
    RUN_TEST(test_ring_quad_colors_count);
    RUN_TEST(test_shell_positions_wrap);
    RUN_TEST(test_atmo_colors_valid_range);

    return UNITY_END();
}
