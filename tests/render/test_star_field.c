/*
 * test_star_field.c — tests for star field GPU data packing module
 *
 * Tests: vertex packing, tier offsets, deduplication, constellation lines,
 *        shader source strings, metadata consistency, edge cases.
 */

#include "../unity/unity.h"
#include "../../src/render/star_field.h"
#include "../../src/render/star_catalog.h"
#include "../../src/render/star_catalog_ext.h"
#include "../../src/render/constellation.h"

#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

/* Large enough buffer for all stars: 600 * 7 = 4200 floats */
static float star_buf[600 * STAR_VERTEX_FLOATS];
/* Large enough buffer for all constellation lines: 200 * 2 * 7 = 2800 floats */
static float line_buf[200 * 2 * CLINE_VERTEX_FLOATS];

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * 1. star_field_total_count
 * ═══════════════════════════════════════════════════════════════════════ */

void test_total_count_positive(void)
{
    int n = star_field_total_count();
    TEST_ASSERT_TRUE(n > 0);
}

void test_total_count_less_than_sum_of_catalogs(void)
{
    /* Deduplication means total < star_catalog_count + ext_star_count */
    int n = star_field_total_count();
    int sum = star_catalog_count() + ext_star_count();
    TEST_ASSERT_TRUE(n < sum);
}

void test_total_count_at_least_ext_count(void)
{
    /* ext catalog is larger and covers more stars */
    int n = star_field_total_count();
    TEST_ASSERT_TRUE(n >= ext_star_count());
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. star_field_pack — basic
 * ═══════════════════════════════════════════════════════════════════════ */

void test_pack_returns_positive_count(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    TEST_ASSERT_TRUE(n > 0);
}

void test_pack_returns_total_count(void)
{
    int total = star_field_total_count();
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(total, n);
}

void test_pack_zero_buffer_returns_zero(void)
{
    int n = star_field_pack(star_buf, 0, 8.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_pack_small_buffer_truncates(void)
{
    int n = star_field_pack(star_buf, 5, 8.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(5, n);
}

void test_pack_null_buffer_returns_zero(void)
{
    int n = star_field_pack(NULL, 600, 8.0f, 100.0f);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. Vertex data structure
 * ═══════════════════════════════════════════════════════════════════════ */

void test_vertex_stride_is_28_bytes(void)
{
    TEST_ASSERT_EQUAL_INT(28, STAR_VERTEX_STRIDE);
}

void test_vertex_floats_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, STAR_VERTEX_FLOATS);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. First star is brightest (Sirius)
 * ═══════════════════════════════════════════════════════════════════════ */

void test_first_star_is_brightest(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    TEST_ASSERT_TRUE(n >= 2);
    /* First star size should be >= second star size (sorted by magnitude) */
    float size_0 = star_buf[6];  /* offset 6 = size of first star */
    float size_1 = star_buf[6 + STAR_VERTEX_FLOATS]; /* second star */
    TEST_ASSERT_TRUE(size_0 >= size_1);
}

void test_first_star_has_largest_size(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    float max_size = star_buf[6];
    for (int i = 1; i < n; i++) {
        float sz = star_buf[i * STAR_VERTEX_FLOATS + 6];
        TEST_ASSERT_TRUE(max_size >= sz);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 5. Star positions on sphere
 * ═══════════════════════════════════════════════════════════════════════ */

void test_star_positions_on_sphere(void)
{
    float radius = 100.0f;
    int n = star_field_pack(star_buf, 600, 8.0f, radius);
    TEST_ASSERT_TRUE(n > 0);
    /* Check first 10 stars are on the sphere */
    int check = (n < 10) ? n : 10;
    for (int i = 0; i < check; i++) {
        int off = i * STAR_VERTEX_FLOATS;
        float x = star_buf[off + 0];
        float y = star_buf[off + 1];
        float z = star_buf[off + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_FLOAT_WITHIN(0.5f, radius, dist);
    }
}

void test_star_positions_different_radius(void)
{
    float radius = 50.0f;
    int n = star_field_pack(star_buf, 10, 8.0f, radius);
    TEST_ASSERT_TRUE(n > 0);
    float x = star_buf[0];
    float y = star_buf[1];
    float z = star_buf[2];
    float dist = sqrtf(x * x + y * y + z * z);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, radius, dist);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 6. Star colors valid range
 * ═══════════════════════════════════════════════════════════════════════ */

void test_star_colors_valid_range(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    int check = (n < 20) ? n : 20;
    for (int i = 0; i < check; i++) {
        int off = i * STAR_VERTEX_FLOATS;
        float r = star_buf[off + 3];
        float g = star_buf[off + 4];
        float b = star_buf[off + 5];
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 7. Star sizes positive and ordered
 * ═══════════════════════════════════════════════════════════════════════ */

void test_star_sizes_positive(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    for (int i = 0; i < n; i++) {
        float sz = star_buf[i * STAR_VERTEX_FLOATS + 6];
        TEST_ASSERT_TRUE(sz > 0.0f);
    }
}

void test_star_sizes_non_increasing(void)
{
    /* Since sorted by magnitude (brightest first), sizes should be non-increasing */
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    for (int i = 1; i < n; i++) {
        float prev = star_buf[(i - 1) * STAR_VERTEX_FLOATS + 6];
        float curr = star_buf[i * STAR_VERTEX_FLOATS + 6];
        TEST_ASSERT_TRUE(prev >= curr - 0.001f);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 8. Tier offsets and counts
 * ═══════════════════════════════════════════════════════════════════════ */

void test_tier_offsets_monotonically_increasing(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    star_field_info_t info = star_field_info(n);
    for (int i = 1; i < 4; i++) {
        TEST_ASSERT_TRUE(info.tier_offsets[i] >= info.tier_offsets[i - 1]);
    }
}

void test_tier_counts_sum_to_total(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    star_field_info_t info = star_field_info(n);
    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += info.tier_counts[i];
    }
    TEST_ASSERT_EQUAL_INT(n, sum);
}

void test_tier_offset_0_is_zero(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    star_field_info_t info = star_field_info(n);
    TEST_ASSERT_EQUAL_INT(0, info.tier_offsets[0]);
}

void test_tier_counts_non_negative(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    star_field_info_t info = star_field_info(n);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_TRUE(info.tier_counts[i] >= 0);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 9. star_field_info metadata matches pack result
 * ═══════════════════════════════════════════════════════════════════════ */

void test_info_vertex_count_matches(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    star_field_info_t info = star_field_info(n);
    TEST_ASSERT_EQUAL_INT(n, info.vertex_count);
}

void test_info_float_count_matches(void)
{
    int n = star_field_pack(star_buf, 600, 8.0f, 100.0f);
    star_field_info_t info = star_field_info(n);
    TEST_ASSERT_EQUAL_INT(n * STAR_VERTEX_FLOATS, info.float_count);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 10. Deduplication
 * ═══════════════════════════════════════════════════════════════════════ */

void test_deduplication_reduces_total(void)
{
    int n = star_field_total_count();
    int sum = star_catalog_count() + ext_star_count();
    /* 595 total raw, should be less after dedup */
    TEST_ASSERT_TRUE(n < sum);
}

void test_deduplication_count_reasonable(void)
{
    /* ext has 400 unique, star_catalog has 195.
     * Many overlap, so total should be around 400 (ext is superset).
     * Allow range 395-500 */
    int n = star_field_total_count();
    TEST_ASSERT_TRUE(n >= 395);
    TEST_ASSERT_TRUE(n <= 500);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 11. Constellation lines pack — basic
 * ═══════════════════════════════════════════════════════════════════════ */

void test_constellation_lines_returns_positive(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_TRUE(n > 0);
}

void test_constellation_lines_matches_total(void)
{
    int expected = constellation_total_lines();
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_EQUAL_INT(expected, n);
}

void test_constellation_lines_zero_buffer(void)
{
    int n = constellation_lines_pack(line_buf, 0, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_constellation_lines_null_buffer(void)
{
    int n = constellation_lines_pack(NULL, 200, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 12. Zodiac constellations packed first
 * ═══════════════════════════════════════════════════════════════════════ */

void test_zodiac_lines_packed_first(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_TRUE(n > 0);
    constellation_line_info_t info = constellation_lines_info(n);
    /* Zodiac lines should be first in the buffer with higher alpha */
    TEST_ASSERT_TRUE(info.zodiac_line_count > 0);
    /* First zodiac line endpoint alpha = 0.4, non-zodiac = 0.25 */
    if (info.zodiac_line_count > 0) {
        float alpha_first = line_buf[6]; /* r,g,b,a at offset 3,4,5,6 */
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.4f, alpha_first);
    }
}

void test_zodiac_alpha_higher_than_base(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    constellation_line_info_t info = constellation_lines_info(n);
    if (info.zodiac_line_count > 0 && n > info.zodiac_line_count) {
        /* First zodiac line alpha */
        float zodiac_a = line_buf[6];
        /* First non-zodiac line alpha */
        int non_zodiac_off = info.zodiac_line_count * 2 * CLINE_VERTEX_FLOATS;
        float base_a = line_buf[non_zodiac_off + 6];
        TEST_ASSERT_TRUE(zodiac_a > base_a);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 13. Line vertex positions on sphere
 * ═══════════════════════════════════════════════════════════════════════ */

void test_line_positions_on_sphere(void)
{
    float radius = 100.0f;
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, radius);
    TEST_ASSERT_TRUE(n > 0);
    /* Check first 4 endpoints */
    int check = (n * 2 < 4) ? n * 2 : 4;
    for (int i = 0; i < check; i++) {
        int off = i * CLINE_VERTEX_FLOATS;
        float x = line_buf[off + 0];
        float y = line_buf[off + 1];
        float z = line_buf[off + 2];
        float dist = sqrtf(x * x + y * y + z * z);
        TEST_ASSERT_FLOAT_WITHIN(0.5f, radius, dist);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * 14. Line colors blue-white
 * ═══════════════════════════════════════════════════════════════════════ */

void test_line_colors_blue_white(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_TRUE(n > 0);
    /* First endpoint color should be cool blue-white (0.6, 0.7, 0.9) */
    float r = line_buf[3];
    float g = line_buf[4];
    float b = line_buf[5];
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.6f, r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.7f, g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.9f, b);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 15. constellation_lines_info metadata
 * ═══════════════════════════════════════════════════════════════════════ */

void test_cline_info_vertex_count(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    constellation_line_info_t info = constellation_lines_info(n);
    TEST_ASSERT_EQUAL_INT(n * 2, info.vertex_count);
}

void test_cline_info_float_count(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    constellation_line_info_t info = constellation_lines_info(n);
    TEST_ASSERT_EQUAL_INT(n * 2 * CLINE_VERTEX_FLOATS, info.float_count);
}

void test_cline_info_line_count(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    constellation_line_info_t info = constellation_lines_info(n);
    TEST_ASSERT_EQUAL_INT(n, info.line_count);
}

void test_cline_info_zodiac_count_is_12_zodiac_lines(void)
{
    int n = constellation_lines_pack(line_buf, 200, 0.25f, 0.4f, 100.0f);
    constellation_line_info_t info = constellation_lines_info(n);
    /* 12 zodiac constellations with varying line counts */
    TEST_ASSERT_TRUE(info.zodiac_line_count > 0);
    TEST_ASSERT_TRUE(info.zodiac_line_count <= n);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 16. Shader sources — non-NULL and non-empty
 * ═══════════════════════════════════════════════════════════════════════ */

void test_star_vert_source_not_null(void)
{
    const char *src = star_field_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_star_frag_source_not_null(void)
{
    const char *src = star_field_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_line_vert_source_not_null(void)
{
    const char *src = constellation_line_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

void test_line_frag_source_not_null(void)
{
    const char *src = constellation_line_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 0);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 17. Shader sources contain required tokens
 * ═══════════════════════════════════════════════════════════════════════ */

void test_star_vert_has_version(void)
{
    const char *src = star_field_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_star_frag_has_version(void)
{
    const char *src = star_field_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_line_vert_has_version(void)
{
    const char *src = constellation_line_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_line_frag_has_version(void)
{
    const char *src = constellation_line_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

void test_star_vert_has_gl_pointsize(void)
{
    const char *src = star_field_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "gl_PointSize"));
}

void test_star_frag_has_gl_pointcoord(void)
{
    const char *src = star_field_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "gl_PointCoord"));
}

void test_line_vert_has_u_mvp(void)
{
    const char *src = constellation_line_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

void test_star_vert_has_u_mvp(void)
{
    const char *src = star_field_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

void test_star_vert_has_u_scale_factor(void)
{
    const char *src = star_field_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_scale_factor"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 18. CLINE vertex stride
 * ═══════════════════════════════════════════════════════════════════════ */

void test_cline_vertex_stride(void)
{
    TEST_ASSERT_EQUAL_INT(28, CLINE_VERTEX_STRIDE);
}

void test_cline_vertex_floats(void)
{
    TEST_ASSERT_EQUAL_INT(7, CLINE_VERTEX_FLOATS);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 19. Small buffer truncation for constellation lines
 * ═══════════════════════════════════════════════════════════════════════ */

void test_constellation_lines_small_buffer(void)
{
    int n = constellation_lines_pack(line_buf, 3, 0.25f, 0.4f, 100.0f);
    TEST_ASSERT_EQUAL_INT(3, n);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Runner
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Total count */
    RUN_TEST(test_total_count_positive);
    RUN_TEST(test_total_count_less_than_sum_of_catalogs);
    RUN_TEST(test_total_count_at_least_ext_count);

    /* Pack basic */
    RUN_TEST(test_pack_returns_positive_count);
    RUN_TEST(test_pack_returns_total_count);
    RUN_TEST(test_pack_zero_buffer_returns_zero);
    RUN_TEST(test_pack_small_buffer_truncates);
    RUN_TEST(test_pack_null_buffer_returns_zero);

    /* Vertex structure */
    RUN_TEST(test_vertex_stride_is_28_bytes);
    RUN_TEST(test_vertex_floats_is_7);

    /* Brightest first */
    RUN_TEST(test_first_star_is_brightest);
    RUN_TEST(test_first_star_has_largest_size);

    /* Positions */
    RUN_TEST(test_star_positions_on_sphere);
    RUN_TEST(test_star_positions_different_radius);

    /* Colors */
    RUN_TEST(test_star_colors_valid_range);

    /* Sizes */
    RUN_TEST(test_star_sizes_positive);
    RUN_TEST(test_star_sizes_non_increasing);

    /* Tier offsets */
    RUN_TEST(test_tier_offsets_monotonically_increasing);
    RUN_TEST(test_tier_counts_sum_to_total);
    RUN_TEST(test_tier_offset_0_is_zero);
    RUN_TEST(test_tier_counts_non_negative);

    /* Info metadata */
    RUN_TEST(test_info_vertex_count_matches);
    RUN_TEST(test_info_float_count_matches);

    /* Deduplication */
    RUN_TEST(test_deduplication_reduces_total);
    RUN_TEST(test_deduplication_count_reasonable);

    /* Constellation lines basic */
    RUN_TEST(test_constellation_lines_returns_positive);
    RUN_TEST(test_constellation_lines_matches_total);
    RUN_TEST(test_constellation_lines_zero_buffer);
    RUN_TEST(test_constellation_lines_null_buffer);

    /* Zodiac first */
    RUN_TEST(test_zodiac_lines_packed_first);
    RUN_TEST(test_zodiac_alpha_higher_than_base);

    /* Line positions */
    RUN_TEST(test_line_positions_on_sphere);

    /* Line colors */
    RUN_TEST(test_line_colors_blue_white);

    /* Constellation info metadata */
    RUN_TEST(test_cline_info_vertex_count);
    RUN_TEST(test_cline_info_float_count);
    RUN_TEST(test_cline_info_line_count);
    RUN_TEST(test_cline_info_zodiac_count_is_12_zodiac_lines);

    /* Shader sources */
    RUN_TEST(test_star_vert_source_not_null);
    RUN_TEST(test_star_frag_source_not_null);
    RUN_TEST(test_line_vert_source_not_null);
    RUN_TEST(test_line_frag_source_not_null);

    /* Shader tokens */
    RUN_TEST(test_star_vert_has_version);
    RUN_TEST(test_star_frag_has_version);
    RUN_TEST(test_line_vert_has_version);
    RUN_TEST(test_line_frag_has_version);
    RUN_TEST(test_star_vert_has_gl_pointsize);
    RUN_TEST(test_star_frag_has_gl_pointcoord);
    RUN_TEST(test_line_vert_has_u_mvp);
    RUN_TEST(test_star_vert_has_u_mvp);
    RUN_TEST(test_star_vert_has_u_scale_factor);

    /* CLINE stride */
    RUN_TEST(test_cline_vertex_stride);
    RUN_TEST(test_cline_vertex_floats);

    /* Small buffer truncation for lines */
    RUN_TEST(test_constellation_lines_small_buffer);

    return UNITY_END();
}
