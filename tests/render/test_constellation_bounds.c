/*
 * test_constellation_bounds.c — tests for IAU constellation boundary pack
 *
 * Tests: data integrity (88 constellations), zodiac marking, RA/Dec ranges,
 *        lookup by abbreviation, total segments, point-in-constellation,
 *        GPU vertex packing, shader sources, edge cases.
 */

#include "../unity/unity.h"
#include "../../src/render/constellation_bounds.h"

#include <math.h>
#include <string.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Large enough buffer for all boundary segments: 1200 * 2 * 7 = 16800 floats */
static float pack_buf[CB_MAX_TOTAL_VERTICES * CB_VERTEX_FLOATS];

/* Smaller buffer for single-constellation packing */
static float one_buf[CB_MAX_BOUNDARY_VERTICES * 2 * CB_VERTEX_FLOATS];

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * 1. Constellation count and data integrity
 * ═══════════════════════════════════════════════════════════════════════ */

void test_constellation_count_is_88(void)
{
    /* IAU defines exactly 88 constellations */
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t c = cb_get(i);
        TEST_ASSERT_NOT_NULL(c.name);
        TEST_ASSERT_NOT_NULL(c.abbr);
    }
}

void test_all_have_names(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t c = cb_get(i);
        TEST_ASSERT_NOT_NULL(c.name);
        TEST_ASSERT_TRUE(strlen(c.name) > 0);
    }
}

void test_all_have_abbreviations(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t c = cb_get(i);
        TEST_ASSERT_NOT_NULL(c.abbr);
        TEST_ASSERT_EQUAL_INT(3, (int)strlen(c.abbr));
    }
}

void test_all_have_vertices(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t c = cb_get(i);
        TEST_ASSERT_TRUE(c.vertex_count >= 3);
    }
}

void test_invalid_index_returns_sentinel(void)
{
    cb_constellation_t c = cb_get(-1);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_NULL(c.abbr);
    TEST_ASSERT_EQUAL_INT(0, c.vertex_count);

    c = cb_get(88);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_NULL(c.abbr);
}

void test_abbreviations_unique(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t ci = cb_get(i);
        for (int j = i + 1; j < CB_CONSTELLATION_COUNT; j++) {
            cb_constellation_t cj = cb_get(j);
            TEST_ASSERT_TRUE(strcmp(ci.abbr, cj.abbr) != 0);
        }
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. Zodiac constellations
 * ═══════════════════════════════════════════════════════════════════════ */

void test_exactly_12_zodiac(void)
{
    int count = 0;
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t c = cb_get(i);
        if (c.is_zodiac) count++;
    }
    TEST_ASSERT_EQUAL_INT(12, count);
}

void test_aries_is_zodiac(void)
{
    int idx = cb_find("Ari");
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_TRUE(c.is_zodiac);
}

void test_taurus_is_zodiac(void)
{
    int idx = cb_find("Tau");
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_TRUE(c.is_zodiac);
}

void test_scorpius_is_zodiac(void)
{
    int idx = cb_find("Sco");
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_TRUE(c.is_zodiac);
}

void test_orion_is_not_zodiac(void)
{
    int idx = cb_find("Ori");
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_FALSE(c.is_zodiac);
}

void test_ursa_major_is_not_zodiac(void)
{
    int idx = cb_find("UMa");
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_FALSE(c.is_zodiac);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. Boundary vertex data ranges
 * ═══════════════════════════════════════════════════════════════════════ */

void test_all_ra_in_range(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        int nv = cb_vertex_count(i);
        for (int v = 0; v < nv; v++) {
            cb_point_t p = cb_vertex(i, v);
            TEST_ASSERT_TRUE(p.ra_hours >= 0.0f);
            TEST_ASSERT_TRUE(p.ra_hours <= 24.0f);
        }
    }
}

void test_all_dec_in_range(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        int nv = cb_vertex_count(i);
        for (int v = 0; v < nv; v++) {
            cb_point_t p = cb_vertex(i, v);
            TEST_ASSERT_TRUE(p.dec_deg >= -90.0f);
            TEST_ASSERT_TRUE(p.dec_deg <= 90.0f);
        }
    }
}

void test_vertex_count_matches_get(void)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        cb_constellation_t c = cb_get(i);
        int nv = cb_vertex_count(i);
        TEST_ASSERT_EQUAL_INT(c.vertex_count, nv);
    }
}

void test_invalid_vertex_returns_zero(void)
{
    cb_point_t p = cb_vertex(0, 9999);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p.dec_deg);
}

void test_invalid_constellation_vertex_count(void)
{
    TEST_ASSERT_EQUAL_INT(0, cb_vertex_count(-1));
    TEST_ASSERT_EQUAL_INT(0, cb_vertex_count(88));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. cb_find — lookup by abbreviation
 * ═══════════════════════════════════════════════════════════════════════ */

void test_find_orion(void)
{
    int idx = cb_find("Ori");
    TEST_ASSERT_TRUE(idx >= 0 && idx < CB_CONSTELLATION_COUNT);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_EQUAL_STRING("Ori", c.abbr);
}

void test_find_ursa_major(void)
{
    int idx = cb_find("UMa");
    TEST_ASSERT_TRUE(idx >= 0 && idx < CB_CONSTELLATION_COUNT);
}

void test_find_cassiopeia(void)
{
    int idx = cb_find("Cas");
    TEST_ASSERT_TRUE(idx >= 0 && idx < CB_CONSTELLATION_COUNT);
}

void test_find_cygnus(void)
{
    int idx = cb_find("Cyg");
    TEST_ASSERT_TRUE(idx >= 0 && idx < CB_CONSTELLATION_COUNT);
}

void test_find_crux(void)
{
    int idx = cb_find("Cru");
    TEST_ASSERT_TRUE(idx >= 0 && idx < CB_CONSTELLATION_COUNT);
}

void test_find_invalid_returns_neg1(void)
{
    TEST_ASSERT_EQUAL_INT(-1, cb_find("ZZZ"));
    TEST_ASSERT_EQUAL_INT(-1, cb_find(NULL));
    TEST_ASSERT_EQUAL_INT(-1, cb_find(""));
}

void test_find_all_zodiac_present(void)
{
    const char *zodiac[] = {
        "Ari", "Tau", "Gem", "Cnc", "Leo", "Vir",
        "Lib", "Sco", "Sgr", "Cap", "Aqr", "Psc"
    };
    for (int i = 0; i < 12; i++) {
        int idx = cb_find(zodiac[i]);
        TEST_ASSERT_TRUE_MESSAGE(idx >= 0, zodiac[i]);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 5. Total segments
 * ═══════════════════════════════════════════════════════════════════════ */

void test_total_segments_positive(void)
{
    int total = cb_total_segments();
    TEST_ASSERT_TRUE(total > 0);
}

void test_total_segments_reasonable_range(void)
{
    /* 88 constellations with 3-20+ vertices each, total ~600-1200 segments */
    int total = cb_total_segments();
    TEST_ASSERT_TRUE(total >= 400);
    TEST_ASSERT_TRUE(total <= CB_MAX_TOTAL_SEGMENTS);
}

void test_total_segments_equals_sum_of_vertex_counts(void)
{
    /* Each constellation is a closed polygon: segments = vertex_count */
    int sum = 0;
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        sum += cb_vertex_count(i);
    }
    TEST_ASSERT_EQUAL_INT(sum, cb_total_segments());
}

/* ═══════════════════════════════════════════════════════════════════════
 * 6. cb_containing — point-in-constellation
 * ═══════════════════════════════════════════════════════════════════════ */

void test_containing_polaris_in_umi(void)
{
    /* Polaris: RA ~2.53h, Dec ~89.26° → Ursa Minor */
    int idx = cb_containing(2.53f, 89.26f);
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_EQUAL_STRING("UMi", c.abbr);
}

void test_containing_orion_belt(void)
{
    /* Orion's belt area: RA ~5.6h, Dec ~-1.2° → Orion */
    int idx = cb_containing(5.6f, -1.2f);
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_EQUAL_STRING("Ori", c.abbr);
}

void test_containing_sirius_in_cma(void)
{
    /* Sirius: RA ~6.75h, Dec ~-16.72° → Canis Major */
    int idx = cb_containing(6.75f, -16.72f);
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_EQUAL_STRING("CMa", c.abbr);
}

void test_containing_vega_in_lyr(void)
{
    /* Vega: RA ~18.62h, Dec ~38.78° → Lyra */
    int idx = cb_containing(18.62f, 38.78f);
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_EQUAL_STRING("Lyr", c.abbr);
}

void test_containing_south_pole_in_oct(void)
{
    /* South celestial pole: Dec ~-90° → Octans */
    int idx = cb_containing(12.0f, -89.5f);
    TEST_ASSERT_TRUE(idx >= 0);
    cb_constellation_t c = cb_get(idx);
    TEST_ASSERT_EQUAL_STRING("Oct", c.abbr);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 7. cb_pack — full packing
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_returns_positive_count(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    TEST_ASSERT_TRUE(n > 0);
}

void test_pack_returns_total_segments(void)
{
    int expected = cb_total_segments();
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    TEST_ASSERT_EQUAL_INT(expected, n);
}

void test_pack_null_buffer_returns_zero(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, NULL);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_vertices_on_sphere(void)
{
    float radius = 100.0f;
    int n = cb_pack(radius, 0.15f, 0.3f, pack_buf);
    TEST_ASSERT_TRUE(n > 0);

    /* Check first 10 vertices are on the sphere */
    int check = (n * 2 < 10) ? n * 2 : 10;
    for (int i = 0; i < check; i++) {
        int off = i * CB_VERTEX_FLOATS;
        float x = pack_buf[off + 0];
        float y = pack_buf[off + 1];
        float z = pack_buf[off + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_FLOAT_WITHIN(0.5f, radius, dist);
    }
}

void test_pack_color_channels_valid(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    TEST_ASSERT_TRUE(n > 0);

    /* Check first 4 vertices have valid RGBA */
    int check = (n * 2 < 4) ? n * 2 : 4;
    for (int i = 0; i < check; i++) {
        int off = i * CB_VERTEX_FLOATS;
        float r = pack_buf[off + 3];
        float g = pack_buf[off + 4];
        float b = pack_buf[off + 5];
        float a = pack_buf[off + 6];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
        TEST_ASSERT_TRUE(a >= 0.0f && a <= 1.0f);
    }
}

void test_pack_different_radius(void)
{
    float radius = 50.0f;
    int n = cb_pack(radius, 0.15f, 0.3f, pack_buf);
    TEST_ASSERT_TRUE(n > 0);

    float x = pack_buf[0];
    float y = pack_buf[1];
    float z = pack_buf[2];
    float dist = sqrtf(x * x + y * y + z * z);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, radius, dist);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 8. cb_pack_info — metadata
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_info_total_segments(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    cb_pack_info_t info = cb_pack_info(n);
    TEST_ASSERT_EQUAL_INT(n, info.total_segments);
}

void test_pack_info_total_vertices(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    cb_pack_info_t info = cb_pack_info(n);
    TEST_ASSERT_EQUAL_INT(n * 2, info.total_vertices);
}

void test_pack_info_zodiac_segments_positive(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    cb_pack_info_t info = cb_pack_info(n);
    TEST_ASSERT_TRUE(info.zodiac_segments > 0);
}

void test_pack_info_zodiac_segments_less_than_total(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    cb_pack_info_t info = cb_pack_info(n);
    TEST_ASSERT_TRUE(info.zodiac_segments < info.total_segments);
}

void test_pack_info_offsets_sum_correctly(void)
{
    int n = cb_pack(100.0f, 0.15f, 0.3f, pack_buf);
    cb_pack_info_t info = cb_pack_info(n);

    int sum = 0;
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        sum += info.constellation_counts[i];
    }
    TEST_ASSERT_EQUAL_INT(n, sum);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 9. cb_pack_one — single constellation
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_one_returns_positive(void)
{
    int idx = cb_find("Ori");
    TEST_ASSERT_TRUE(idx >= 0);
    int n = cb_pack_one(idx, 100.0f, 0.25f, one_buf);
    TEST_ASSERT_TRUE(n > 0);
}

void test_pack_one_matches_vertex_count(void)
{
    int idx = cb_find("Ori");
    TEST_ASSERT_TRUE(idx >= 0);
    int expected = cb_vertex_count(idx);
    int n = cb_pack_one(idx, 100.0f, 0.25f, one_buf);
    TEST_ASSERT_EQUAL_INT(expected, n);
}

void test_pack_one_invalid_index(void)
{
    int n = cb_pack_one(-1, 100.0f, 0.25f, one_buf);
    TEST_ASSERT_EQUAL_INT(0, n);
    n = cb_pack_one(88, 100.0f, 0.25f, one_buf);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_one_null_buffer(void)
{
    int idx = cb_find("Ori");
    int n = cb_pack_one(idx, 100.0f, 0.25f, NULL);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_one_vertices_on_sphere(void)
{
    float radius = 100.0f;
    int idx = cb_find("UMa");
    TEST_ASSERT_TRUE(idx >= 0);
    int n = cb_pack_one(idx, radius, 0.25f, one_buf);
    TEST_ASSERT_TRUE(n > 0);

    for (int i = 0; i < n * 2 && i < 6; i++) {
        int off = i * CB_VERTEX_FLOATS;
        float x = one_buf[off + 0];
        float y = one_buf[off + 1];
        float z = one_buf[off + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_FLOAT_WITHIN(0.5f, radius, dist);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 10. Shader sources
 * ═══════════════════════════════════════════════════════════════════════ */

void test_vert_source_not_null(void)
{
    const char *src = cb_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_frag_source_not_null(void)
{
    const char *src = cb_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_vert_has_version(void)
{
    const char *src = cb_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_frag_has_version(void)
{
    const char *src = cb_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_vert_has_gl_position(void)
{
    const char *src = cb_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "gl_Position"));
}

void test_vert_has_main(void)
{
    const char *src = cb_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main"));
}

void test_frag_has_main(void)
{
    const char *src = cb_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "void main"));
}

void test_vert_has_u_mvp(void)
{
    const char *src = cb_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 11. Constants
 * ═══════════════════════════════════════════════════════════════════════ */

void test_vertex_floats_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, CB_VERTEX_FLOATS);
}

void test_vertex_stride_is_28(void)
{
    TEST_ASSERT_EQUAL_INT(28, CB_VERTEX_STRIDE);
}

void test_constellation_count_macro(void)
{
    TEST_ASSERT_EQUAL_INT(88, CB_CONSTELLATION_COUNT);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Runner
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Data integrity */
    RUN_TEST(test_constellation_count_is_88);
    RUN_TEST(test_all_have_names);
    RUN_TEST(test_all_have_abbreviations);
    RUN_TEST(test_all_have_vertices);
    RUN_TEST(test_invalid_index_returns_sentinel);
    RUN_TEST(test_abbreviations_unique);

    /* Zodiac */
    RUN_TEST(test_exactly_12_zodiac);
    RUN_TEST(test_aries_is_zodiac);
    RUN_TEST(test_taurus_is_zodiac);
    RUN_TEST(test_scorpius_is_zodiac);
    RUN_TEST(test_orion_is_not_zodiac);
    RUN_TEST(test_ursa_major_is_not_zodiac);

    /* Vertex ranges */
    RUN_TEST(test_all_ra_in_range);
    RUN_TEST(test_all_dec_in_range);
    RUN_TEST(test_vertex_count_matches_get);
    RUN_TEST(test_invalid_vertex_returns_zero);
    RUN_TEST(test_invalid_constellation_vertex_count);

    /* Find by abbreviation */
    RUN_TEST(test_find_orion);
    RUN_TEST(test_find_ursa_major);
    RUN_TEST(test_find_cassiopeia);
    RUN_TEST(test_find_cygnus);
    RUN_TEST(test_find_crux);
    RUN_TEST(test_find_invalid_returns_neg1);
    RUN_TEST(test_find_all_zodiac_present);

    /* Total segments */
    RUN_TEST(test_total_segments_positive);
    RUN_TEST(test_total_segments_reasonable_range);
    RUN_TEST(test_total_segments_equals_sum_of_vertex_counts);

    /* Point-in-constellation */
    RUN_TEST(test_containing_polaris_in_umi);
    RUN_TEST(test_containing_orion_belt);
    RUN_TEST(test_containing_sirius_in_cma);
    RUN_TEST(test_containing_vega_in_lyr);
    RUN_TEST(test_containing_south_pole_in_oct);

    /* Full packing */
    RUN_TEST(test_pack_returns_positive_count);
    RUN_TEST(test_pack_returns_total_segments);
    RUN_TEST(test_pack_null_buffer_returns_zero);
    RUN_TEST(test_pack_vertices_on_sphere);
    RUN_TEST(test_pack_color_channels_valid);
    RUN_TEST(test_pack_different_radius);

    /* Pack info */
    RUN_TEST(test_pack_info_total_segments);
    RUN_TEST(test_pack_info_total_vertices);
    RUN_TEST(test_pack_info_zodiac_segments_positive);
    RUN_TEST(test_pack_info_zodiac_segments_less_than_total);
    RUN_TEST(test_pack_info_offsets_sum_correctly);

    /* Pack one */
    RUN_TEST(test_pack_one_returns_positive);
    RUN_TEST(test_pack_one_matches_vertex_count);
    RUN_TEST(test_pack_one_invalid_index);
    RUN_TEST(test_pack_one_null_buffer);
    RUN_TEST(test_pack_one_vertices_on_sphere);

    /* Shader sources */
    RUN_TEST(test_vert_source_not_null);
    RUN_TEST(test_frag_source_not_null);
    RUN_TEST(test_vert_has_version);
    RUN_TEST(test_frag_has_version);
    RUN_TEST(test_vert_has_gl_position);
    RUN_TEST(test_vert_has_main);
    RUN_TEST(test_frag_has_main);
    RUN_TEST(test_vert_has_u_mvp);

    /* Constants */
    RUN_TEST(test_vertex_floats_is_7);
    RUN_TEST(test_vertex_stride_is_28);
    RUN_TEST(test_constellation_count_macro);

    return UNITY_END();
}
