/*
 * test_card_pack.c -- tests for card quad pack module
 *
 * Tests: quad packing, line packing, visibility counting, byte sizes,
 *        shader sources, edge cases, per-card offsets.
 */

#include "../unity/unity.h"
#include "../../src/render/card_pack.h"

#include <string.h>

#define TOL 0.001f

void setUp(void) {}
void tearDown(void) {}

/* Helper: build a layout with all cards visible */
static card_layout_t all_visible_layout(void)
{
    return card_layout_compute(card_all_mask(), 1.777f);
}

/* Helper: build a layout with no cards visible */
static card_layout_t none_visible_layout(void)
{
    return card_layout_compute(0, 1.777f);
}

/* Helper: build a layout with one card visible */
static card_layout_t single_visible_layout(card_type_t type)
{
    return card_layout_compute(1 << type, 1.777f);
}

/* ═══════════════════════════════════════════════════════════════════════
 * Quad packing tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 1. All cards visible: vertex_count = 5 * 4 = 20 */
void test_pack_quads_all_visible_vertex_count(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(5 * CP_VERTS_PER_QUAD, q.vertex_count);
}

/* 2. All cards visible: index_count = 5 * 6 = 30 */
void test_pack_quads_all_visible_index_count(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(5 * CP_INDICES_PER_QUAD, q.index_count);
}

/* 3. All cards visible: card_count = 5 */
void test_pack_quads_all_visible_card_count(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(5, q.card_count);
}

/* 4. No cards visible: vertex_count = 0 */
void test_pack_quads_no_visible_vertex_count(void)
{
    card_layout_t layout = none_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(0, q.vertex_count);
}

/* 5. No cards visible: card_count = 0 */
void test_pack_quads_no_visible_card_count(void)
{
    card_layout_t layout = none_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(0, q.card_count);
}

/* 6. Single card visible: vertex_count = 4 */
void test_pack_quads_single_card_vertex_count(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(4, q.vertex_count);
}

/* 7. Single card visible: index_count = 6 */
void test_pack_quads_single_card_index_count(void)
{
    card_layout_t layout = single_visible_layout(CARD_ICHING);
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(6, q.index_count);
}

/* 8. Per-card offsets are non-overlapping for all visible */
void test_pack_quads_offsets_non_overlapping(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    for (int i = 0; i < q.card_count; i++) {
        TEST_ASSERT_EQUAL_INT(CP_INDICES_PER_QUAD, q.card_counts[i]);
        TEST_ASSERT_EQUAL_INT(i * CP_INDICES_PER_QUAD, q.card_offsets[i]);
    }
}

/* 9. UV coordinates: bottom-left (0,0) and top-right (1,1) */
void test_pack_quads_uv_coordinates(void)
{
    card_layout_t layout = single_visible_layout(CARD_CHINESE);
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);

    /* Vertex layout: x, y, u, v, r, g, b, a — 8 floats per vertex
     * Quad vertices: BL(0), BR(1), TR(2), TL(3)
     * BL uv = (0,0), BR uv = (1,0), TR uv = (1,1), TL uv = (0,1) */
    float *v = q.vertices;

    /* BL: u=0, v=0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v[0 * CP_VERTEX_FLOATS + 2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v[0 * CP_VERTEX_FLOATS + 3]);

    /* BR: u=1, v=0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, v[1 * CP_VERTEX_FLOATS + 2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v[1 * CP_VERTEX_FLOATS + 3]);

    /* TR: u=1, v=1 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, v[2 * CP_VERTEX_FLOATS + 2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, v[2 * CP_VERTEX_FLOATS + 3]);

    /* TL: u=0, v=1 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, v[3 * CP_VERTEX_FLOATS + 2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, v[3 * CP_VERTEX_FLOATS + 3]);
}

/* 10. Positions are in pixel space within viewport */
void test_pack_quads_positions_in_pixel_space(void)
{
    float vw = 1920.0f, vh = 1080.0f;
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_quad_data_t q = cp_pack_quads(&layout, vw, vh,
                                      0.1f, 0.1f, 0.15f, 0.85f);

    /* All position values should be within [0, viewport] */
    for (int i = 0; i < q.vertex_count; i++) {
        float px = q.vertices[i * CP_VERTEX_FLOATS + 0];
        float py = q.vertices[i * CP_VERTEX_FLOATS + 1];
        TEST_ASSERT_TRUE(px >= 0.0f && px <= vw);
        TEST_ASSERT_TRUE(py >= 0.0f && py <= vh);
    }
}

/* 11. Color values match input background color */
void test_pack_quads_color_values(void)
{
    card_layout_t layout = single_visible_layout(CARD_HD);
    float r = 0.12f, g = 0.34f, b = 0.56f, a = 0.78f;
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f, r, g, b, a);

    for (int i = 0; i < q.vertex_count; i++) {
        int base = i * CP_VERTEX_FLOATS;
        TEST_ASSERT_FLOAT_WITHIN(TOL, r, q.vertices[base + 4]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, g, q.vertices[base + 5]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, b, q.vertices[base + 6]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, a, q.vertices[base + 7]);
    }
}

/* 12. Triangle indices use [0,1,2, 2,3,0] pattern */
void test_pack_quads_index_pattern(void)
{
    card_layout_t layout = single_visible_layout(CARD_ASTROLOGY);
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);

    /* First card: base vertex = 0, indices = [0,1,2, 2,3,0] */
    TEST_ASSERT_EQUAL_UINT(0, q.indices[0]);
    TEST_ASSERT_EQUAL_UINT(1, q.indices[1]);
    TEST_ASSERT_EQUAL_UINT(2, q.indices[2]);
    TEST_ASSERT_EQUAL_UINT(2, q.indices[3]);
    TEST_ASSERT_EQUAL_UINT(3, q.indices[4]);
    TEST_ASSERT_EQUAL_UINT(0, q.indices[5]);
}

/* 13. Multi-card index pattern references correct base vertices */
void test_pack_quads_multi_card_index_bases(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);

    for (int c = 0; c < q.card_count; c++) {
        unsigned int base = (unsigned int)(c * CP_VERTS_PER_QUAD);
        int idx_start = c * CP_INDICES_PER_QUAD;
        TEST_ASSERT_EQUAL_UINT(base + 0, q.indices[idx_start + 0]);
        TEST_ASSERT_EQUAL_UINT(base + 1, q.indices[idx_start + 1]);
        TEST_ASSERT_EQUAL_UINT(base + 2, q.indices[idx_start + 2]);
        TEST_ASSERT_EQUAL_UINT(base + 2, q.indices[idx_start + 3]);
        TEST_ASSERT_EQUAL_UINT(base + 3, q.indices[idx_start + 4]);
        TEST_ASSERT_EQUAL_UINT(base + 0, q.indices[idx_start + 5]);
    }
}

/* 14. Index values reference valid vertex indices */
void test_pack_quads_indices_within_bounds(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);

    for (int i = 0; i < q.index_count; i++) {
        TEST_ASSERT_TRUE(q.indices[i] < (unsigned int)q.vertex_count);
    }
}

/* 15. Different viewport sizes produce different positions */
void test_pack_quads_different_viewports(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_quad_data_t q1 = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                       0.1f, 0.1f, 0.15f, 0.85f);
    cp_quad_data_t q2 = cp_pack_quads(&layout, 800.0f, 600.0f,
                                       0.1f, 0.1f, 0.15f, 0.85f);

    /* Same card, different viewport -> different pixel positions */
    int differs = 0;
    for (int i = 0; i < q1.vertex_count; i++) {
        float px1 = q1.vertices[i * CP_VERTEX_FLOATS + 0];
        float px2 = q2.vertices[i * CP_VERTEX_FLOATS + 0];
        if (px1 != px2) differs = 1;
    }
    TEST_ASSERT_TRUE(differs);
}

/* 16. Opacity 0 cards still generate geometry if visible=1 */
void test_pack_quads_zero_opacity_still_packed(void)
{
    /* card_layout_compute sets opacity=1.0 for visible cards.
     * Build a custom layout with visible=1 but opacity=0. */
    card_layout_t layout = none_visible_layout();
    layout.cards[0].visible = 1;
    layout.cards[0].opacity = 0.0f;
    layout.cards[0].x = 0.1f;
    layout.cards[0].y = 0.1f;
    layout.cards[0].w = 0.2f;
    layout.cards[0].h = 0.3f;

    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(1, q.card_count);
    TEST_ASSERT_EQUAL_INT(4, q.vertex_count);
}

/* 17. Edge case: viewport 0x0 produces zero-area quads */
void test_pack_quads_zero_viewport(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_quad_data_t q = cp_pack_quads(&layout, 0.0f, 0.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    /* Still produces geometry (4 verts), just at position 0,0 */
    TEST_ASSERT_EQUAL_INT(4, q.vertex_count);
    for (int i = 0; i < q.vertex_count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, q.vertices[i * CP_VERTEX_FLOATS + 0]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, q.vertices[i * CP_VERTEX_FLOATS + 1]);
    }
}

/* 18. Each of the 5 card types produces correct geometry when solo */
void test_pack_quads_each_type_produces_geometry(void)
{
    for (int t = 0; t < CARD_TYPE_COUNT; t++) {
        card_layout_t layout = single_visible_layout((card_type_t)t);
        cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                          0.1f, 0.1f, 0.15f, 0.85f);
        TEST_ASSERT_EQUAL_INT(1, q.card_count);
        TEST_ASSERT_EQUAL_INT(4, q.vertex_count);
        TEST_ASSERT_EQUAL_INT(6, q.index_count);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * Line packing tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 19. Borders only (0 dividers): 4 lines per card = 8 verts per card */
void test_pack_lines_borders_only_single(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 0);
    TEST_ASSERT_EQUAL_INT(8, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(4, l.line_count);
    TEST_ASSERT_EQUAL_INT(4, l.border_line_count);
    TEST_ASSERT_EQUAL_INT(0, l.divider_line_count);
}

/* 20. Borders for all visible: 5 * 8 = 40 verts */
void test_pack_lines_borders_all_visible(void)
{
    card_layout_t layout = all_visible_layout();
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 0);
    TEST_ASSERT_EQUAL_INT(40, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(20, l.line_count);
    TEST_ASSERT_EQUAL_INT(20, l.border_line_count);
}

/* 21. 1 divider per card: +2 verts per card */
void test_pack_lines_one_divider(void)
{
    card_layout_t layout = single_visible_layout(CARD_ICHING);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 1);
    /* 8 border verts + 2 divider verts = 10 */
    TEST_ASSERT_EQUAL_INT(10, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(5, l.line_count);
    TEST_ASSERT_EQUAL_INT(4, l.border_line_count);
    TEST_ASSERT_EQUAL_INT(1, l.divider_line_count);
}

/* 22. 2 dividers per card */
void test_pack_lines_two_dividers(void)
{
    card_layout_t layout = single_visible_layout(CARD_CHINESE);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 2);
    /* 8 border + 4 divider = 12 */
    TEST_ASSERT_EQUAL_INT(12, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(6, l.line_count);
    TEST_ASSERT_EQUAL_INT(2, l.divider_line_count);
}

/* 23. 3 dividers per card */
void test_pack_lines_three_dividers(void)
{
    card_layout_t layout = single_visible_layout(CARD_HD);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 3);
    /* 8 border + 6 divider = 14 */
    TEST_ASSERT_EQUAL_INT(14, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(7, l.line_count);
    TEST_ASSERT_EQUAL_INT(3, l.divider_line_count);
}

/* 24. 4 dividers per card (maximum) */
void test_pack_lines_four_dividers(void)
{
    card_layout_t layout = single_visible_layout(CARD_ASTROLOGY);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 4);
    /* 8 border + 8 divider = 16 */
    TEST_ASSERT_EQUAL_INT(16, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(8, l.line_count);
    TEST_ASSERT_EQUAL_INT(4, l.divider_line_count);
}

/* 25. All visible + 4 dividers: 5*(8+8) = 80 verts */
void test_pack_lines_all_visible_with_dividers(void)
{
    card_layout_t layout = all_visible_layout();
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 4);
    TEST_ASSERT_EQUAL_INT(80, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(40, l.line_count);
    TEST_ASSERT_EQUAL_INT(20, l.border_line_count);
    TEST_ASSERT_EQUAL_INT(20, l.divider_line_count);
}

/* 26. No cards visible: 0 line verts */
void test_pack_lines_no_visible(void)
{
    card_layout_t layout = none_visible_layout();
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 2);
    TEST_ASSERT_EQUAL_INT(0, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, l.line_count);
}

/* 27. Line vertex color matches input border color */
void test_pack_lines_color_values(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    float r = 0.8f, g = 0.6f, b = 0.4f, a = 0.9f;
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      r, g, b, a, 0);
    for (int i = 0; i < l.vertex_count; i++) {
        int base = i * CP_LINE_VERTEX_FLOATS;
        TEST_ASSERT_FLOAT_WITHIN(TOL, r, l.vertices[base + 2]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, g, l.vertices[base + 3]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, b, l.vertices[base + 4]);
        TEST_ASSERT_FLOAT_WITHIN(TOL, a, l.vertices[base + 5]);
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 * Visibility count tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 28. cp_visible_count with all visible */
void test_visible_count_all(void)
{
    card_layout_t layout = all_visible_layout();
    TEST_ASSERT_EQUAL_INT(5, cp_visible_count(&layout));
}

/* 29. cp_visible_count with none visible */
void test_visible_count_none(void)
{
    card_layout_t layout = none_visible_layout();
    TEST_ASSERT_EQUAL_INT(0, cp_visible_count(&layout));
}

/* 30. cp_visible_count with 2 visible */
void test_visible_count_partial(void)
{
    int mask = (1 << CARD_TZOLKIN) | (1 << CARD_ASTROLOGY);
    card_layout_t layout = card_layout_compute(mask, 1.777f);
    TEST_ASSERT_EQUAL_INT(2, cp_visible_count(&layout));
}

/* ═══════════════════════════════════════════════════════════════════════
 * Byte size tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 31. Quad vertex bytes */
void test_quad_vertex_bytes(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    int expected = q.vertex_count * CP_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, cp_quad_vertex_bytes(&q));
}

/* 32. Quad index bytes */
void test_quad_index_bytes(void)
{
    card_layout_t layout = all_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    int expected = q.index_count * (int)sizeof(unsigned int);
    TEST_ASSERT_EQUAL_INT(expected, cp_quad_index_bytes(&q));
}

/* 33. Line vertex bytes */
void test_line_vertex_bytes(void)
{
    card_layout_t layout = all_visible_layout();
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 2);
    int expected = l.vertex_count * CP_LINE_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, cp_line_vertex_bytes(&l));
}

/* 34. Zero vertex bytes for empty data */
void test_quad_vertex_bytes_empty(void)
{
    card_layout_t layout = none_visible_layout();
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(0, cp_quad_vertex_bytes(&q));
    TEST_ASSERT_EQUAL_INT(0, cp_quad_index_bytes(&q));
}

/* ═══════════════════════════════════════════════════════════════════════
 * Shader source tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 35. Quad vertex shader source non-null */
void test_quad_vert_source_non_null(void)
{
    TEST_ASSERT_NOT_NULL(cp_quad_vert_source());
}

/* 36. Quad fragment shader source non-null */
void test_quad_frag_source_non_null(void)
{
    TEST_ASSERT_NOT_NULL(cp_quad_frag_source());
}

/* 37. Line vertex shader source non-null */
void test_line_vert_source_non_null(void)
{
    TEST_ASSERT_NOT_NULL(cp_line_vert_source());
}

/* 38. Line fragment shader source non-null */
void test_line_frag_source_non_null(void)
{
    TEST_ASSERT_NOT_NULL(cp_line_frag_source());
}

/* 39. Quad vert shader contains GLSL keywords */
void test_quad_vert_contains_glsl(void)
{
    const char *s = cp_quad_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(s, "gl_Position"));
    TEST_ASSERT_NOT_NULL(strstr(s, "void main"));
}

/* 40. Quad frag shader contains GLSL keywords */
void test_quad_frag_contains_glsl(void)
{
    const char *s = cp_quad_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(s, "out_color"));
    TEST_ASSERT_NOT_NULL(strstr(s, "void main"));
}

/* 41. Line vert shader contains GLSL keywords */
void test_line_vert_contains_glsl(void)
{
    const char *s = cp_line_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(s, "gl_Position"));
}

/* 42. Line frag shader contains GLSL keywords */
void test_line_frag_contains_glsl(void)
{
    const char *s = cp_line_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(s, "out_color"));
}

/* 43. Quad vert shader has a_position, a_uv, a_color inputs */
void test_quad_vert_has_attributes(void)
{
    const char *s = cp_quad_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "a_position"));
    TEST_ASSERT_NOT_NULL(strstr(s, "a_uv"));
    TEST_ASSERT_NOT_NULL(strstr(s, "a_color"));
}

/* 44. Quad vert shader has u_resolution uniform */
void test_quad_vert_has_resolution_uniform(void)
{
    const char *s = cp_quad_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "u_resolution"));
}

/* 45. Quad frag shader has u_corner_radius uniform */
void test_quad_frag_has_corner_radius(void)
{
    const char *s = cp_quad_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(s, "u_corner_radius"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * Additional edge case & geometry tests
 * ═══════════════════════════════════════════════════════════════════════ */

/* 46. Divider sections clamped to max 4 */
void test_pack_lines_dividers_clamped(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 10);
    /* Should clamp to 4 max */
    TEST_ASSERT_EQUAL_INT(4, l.divider_line_count);
}

/* 47. Negative divider sections treated as 0 */
void test_pack_lines_negative_dividers(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, -3);
    TEST_ASSERT_EQUAL_INT(0, l.divider_line_count);
    TEST_ASSERT_EQUAL_INT(4, l.border_line_count);
}

/* 48. Quad positions: BL.x < BR.x and BL.y > TL.y (screen coords) */
void test_pack_quads_vertex_ordering(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    float *v = q.vertices;

    /* BL.x < BR.x */
    float bl_x = v[0 * CP_VERTEX_FLOATS + 0];
    float br_x = v[1 * CP_VERTEX_FLOATS + 0];
    TEST_ASSERT_TRUE(bl_x < br_x);

    /* BL.y > TL.y (screen Y: 0=top, increases downward) */
    float bl_y = v[0 * CP_VERTEX_FLOATS + 1];
    float tl_y = v[3 * CP_VERTEX_FLOATS + 1];
    TEST_ASSERT_TRUE(bl_y > tl_y);
}

/* 49. Two visible cards: vertex count = 8 */
void test_pack_quads_two_visible(void)
{
    int mask = (1 << CARD_TZOLKIN) | (1 << CARD_ICHING);
    card_layout_t layout = card_layout_compute(mask, 1.777f);
    cp_quad_data_t q = cp_pack_quads(&layout, 1920.0f, 1080.0f,
                                      0.1f, 0.1f, 0.15f, 0.85f);
    TEST_ASSERT_EQUAL_INT(2, q.card_count);
    TEST_ASSERT_EQUAL_INT(8, q.vertex_count);
    TEST_ASSERT_EQUAL_INT(12, q.index_count);
}

/* 50. Line border forms closed rectangle: first and last lines connect corners */
void test_pack_lines_border_forms_rectangle(void)
{
    card_layout_t layout = single_visible_layout(CARD_TZOLKIN);
    cp_line_data_t l = cp_pack_lines(&layout, 1920.0f, 1080.0f,
                                      0.5f, 0.5f, 0.5f, 1.0f, 0);

    /* 4 border lines = 8 vertices. Lines are: top, right, bottom, left.
     * Each line is 2 vertices. Verify we have 4 distinct corner points. */
    TEST_ASSERT_EQUAL_INT(8, l.vertex_count);
    TEST_ASSERT_EQUAL_INT(4, l.line_count);
}

int main(void)
{
    UNITY_BEGIN();

    /* Quad packing */
    RUN_TEST(test_pack_quads_all_visible_vertex_count);
    RUN_TEST(test_pack_quads_all_visible_index_count);
    RUN_TEST(test_pack_quads_all_visible_card_count);
    RUN_TEST(test_pack_quads_no_visible_vertex_count);
    RUN_TEST(test_pack_quads_no_visible_card_count);
    RUN_TEST(test_pack_quads_single_card_vertex_count);
    RUN_TEST(test_pack_quads_single_card_index_count);
    RUN_TEST(test_pack_quads_offsets_non_overlapping);
    RUN_TEST(test_pack_quads_uv_coordinates);
    RUN_TEST(test_pack_quads_positions_in_pixel_space);
    RUN_TEST(test_pack_quads_color_values);
    RUN_TEST(test_pack_quads_index_pattern);
    RUN_TEST(test_pack_quads_multi_card_index_bases);
    RUN_TEST(test_pack_quads_indices_within_bounds);
    RUN_TEST(test_pack_quads_different_viewports);
    RUN_TEST(test_pack_quads_zero_opacity_still_packed);
    RUN_TEST(test_pack_quads_zero_viewport);
    RUN_TEST(test_pack_quads_each_type_produces_geometry);

    /* Line packing */
    RUN_TEST(test_pack_lines_borders_only_single);
    RUN_TEST(test_pack_lines_borders_all_visible);
    RUN_TEST(test_pack_lines_one_divider);
    RUN_TEST(test_pack_lines_two_dividers);
    RUN_TEST(test_pack_lines_three_dividers);
    RUN_TEST(test_pack_lines_four_dividers);
    RUN_TEST(test_pack_lines_all_visible_with_dividers);
    RUN_TEST(test_pack_lines_no_visible);
    RUN_TEST(test_pack_lines_color_values);

    /* Visibility count */
    RUN_TEST(test_visible_count_all);
    RUN_TEST(test_visible_count_none);
    RUN_TEST(test_visible_count_partial);

    /* Byte sizes */
    RUN_TEST(test_quad_vertex_bytes);
    RUN_TEST(test_quad_index_bytes);
    RUN_TEST(test_line_vertex_bytes);
    RUN_TEST(test_quad_vertex_bytes_empty);

    /* Shader sources */
    RUN_TEST(test_quad_vert_source_non_null);
    RUN_TEST(test_quad_frag_source_non_null);
    RUN_TEST(test_line_vert_source_non_null);
    RUN_TEST(test_line_frag_source_non_null);
    RUN_TEST(test_quad_vert_contains_glsl);
    RUN_TEST(test_quad_frag_contains_glsl);
    RUN_TEST(test_line_vert_contains_glsl);
    RUN_TEST(test_line_frag_contains_glsl);
    RUN_TEST(test_quad_vert_has_attributes);
    RUN_TEST(test_quad_vert_has_resolution_uniform);
    RUN_TEST(test_quad_frag_has_corner_radius);

    /* Edge cases */
    RUN_TEST(test_pack_lines_dividers_clamped);
    RUN_TEST(test_pack_lines_negative_dividers);
    RUN_TEST(test_pack_quads_vertex_ordering);
    RUN_TEST(test_pack_quads_two_visible);
    RUN_TEST(test_pack_lines_border_forms_rectangle);

    return UNITY_END();
}
