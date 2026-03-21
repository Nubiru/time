#include "../unity/unity.h"
#include "../../src/ui/tree_of_life_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Node count and path count --- */

void test_node_count(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(TOL_NODE_COUNT, layout.node_count);
    TEST_ASSERT_EQUAL_INT(11, layout.node_count);
}

void test_path_count(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(TOL_PATH_COUNT, layout.path_count);
    TEST_ASSERT_EQUAL_INT(22, layout.path_count);
}

/* --- Sefirot node data --- */

void test_keter_name(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_STRING("Keter", layout.nodes[0].name);
    TEST_ASSERT_EQUAL_STRING("Crown", layout.nodes[0].meaning);
}

void test_malkuth_name(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_STRING("Malkuth", layout.nodes[9].name);
    TEST_ASSERT_EQUAL_STRING("Kingdom", layout.nodes[9].meaning);
}

void test_daat_node(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(10, layout.nodes[10].id);
    TEST_ASSERT_EQUAL_STRING("Da'at", layout.nodes[10].name);
    TEST_ASSERT_EQUAL_STRING("Knowledge", layout.nodes[10].meaning);
}

void test_node_ids(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.nodes[i].id);
    }
    TEST_ASSERT_EQUAL_INT(10, layout.nodes[10].id);
}

/* --- Node positions --- */

void test_node_positions_in_bounds(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    for (int i = 0; i < TOL_NODE_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.nodes[i].x >= 0.0f);
        TEST_ASSERT_TRUE(layout.nodes[i].x <= 1.0f);
        TEST_ASSERT_TRUE(layout.nodes[i].y >= 0.0f);
        TEST_ASSERT_TRUE(layout.nodes[i].y <= 1.0f);
    }
}

void test_keter_at_top(void)
{
    /* Keter (id=0) has pos_y=0.0 in raw data, so it should be near the
     * top of the content area. */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.nodes[0].y);
}

void test_malkuth_at_bottom(void)
{
    /* Malkuth (id=9) has pos_y=1.0 in raw data, should be at bottom
     * of content area (0.05 + 1.0 * 0.85 = 0.90). */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.nodes[9].y);
}

void test_keter_centered(void)
{
    /* Keter has pos_x=0.0, maps to (0.0 + 0.5) * 1.0 = 0.5 in unit,
     * then scale_x: 0.10 + 0.5 * 0.80 = 0.50 */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.nodes[0].x);
}

void test_chokmah_right_pillar(void)
{
    /* Chokmah (id=1) has pos_x=0.5, maps to (0.5+0.5)=1.0 in unit,
     * scale_x: 0.10 + 1.0 * 0.80 = 0.90 */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.nodes[1].x);
}

void test_binah_left_pillar(void)
{
    /* Binah (id=2) has pos_x=-0.5, maps to (-0.5+0.5)=0.0 in unit,
     * scale_x: 0.10 + 0.0 * 0.80 = 0.10 */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.nodes[2].x);
}

/* --- Node colors --- */

void test_keter_white(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, layout.nodes[0].r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, layout.nodes[0].g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, layout.nodes[0].b);
}

void test_gevurah_red(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    /* Gevurah (id=4): red (0.8, 0.15, 0.15) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.80f, layout.nodes[4].r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, layout.nodes[4].g);
}

/* --- Pillar names --- */

void test_keter_pillar_balance(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_STRING("Balance", layout.nodes[0].pillar);
}

void test_chokmah_pillar_mercy(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_STRING("Mercy", layout.nodes[1].pillar);
}

void test_binah_pillar_severity(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_STRING("Severity", layout.nodes[2].pillar);
}

/* --- Path data --- */

void test_first_path_aleph(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(0, layout.paths[0].path_id);
    TEST_ASSERT_EQUAL_STRING("Aleph", layout.paths[0].hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("The Fool", layout.paths[0].tarot);
}

void test_last_path_tav(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(21, layout.paths[21].path_id);
    TEST_ASSERT_EQUAL_STRING("Tav", layout.paths[21].hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("The World", layout.paths[21].tarot);
}

void test_path_from_to(void)
{
    /* Path 0 (Aleph): Keter(0) to Chokmah(1) */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(0, layout.paths[0].from_node);
    TEST_ASSERT_EQUAL_INT(1, layout.paths[0].to_node);
}

void test_path_endpoints_in_bounds(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    for (int i = 0; i < TOL_PATH_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.paths[i].x0 >= 0.0f);
        TEST_ASSERT_TRUE(layout.paths[i].x0 <= 1.0f);
        TEST_ASSERT_TRUE(layout.paths[i].y0 >= 0.0f);
        TEST_ASSERT_TRUE(layout.paths[i].y0 <= 1.0f);
        TEST_ASSERT_TRUE(layout.paths[i].x1 >= 0.0f);
        TEST_ASSERT_TRUE(layout.paths[i].x1 <= 1.0f);
        TEST_ASSERT_TRUE(layout.paths[i].y1 >= 0.0f);
        TEST_ASSERT_TRUE(layout.paths[i].y1 <= 1.0f);
    }
}

void test_path_endpoints_match_nodes(void)
{
    /* Path 0 (Aleph) start should match Keter position,
     * end should match Chokmah position. */
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.nodes[0].x, layout.paths[0].x0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.nodes[0].y, layout.paths[0].y0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.nodes[1].x, layout.paths[0].x1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.nodes[1].y, layout.paths[0].y1);
}

/* --- Highlighting --- */

void test_no_highlight(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_EQUAL_INT(-1, layout.active_sefirah);
    TEST_ASSERT_EQUAL_INT(-1, layout.active_path);
    for (int i = 0; i < TOL_NODE_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.nodes[i].highlighted);
    }
    for (int i = 0; i < TOL_PATH_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.paths[i].highlighted);
    }
}

void test_highlight_sefirah(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(5, -1);
    TEST_ASSERT_EQUAL_INT(5, layout.active_sefirah);
    TEST_ASSERT_EQUAL_INT(1, layout.nodes[5].highlighted);
    /* Others not highlighted */
    TEST_ASSERT_EQUAL_INT(0, layout.nodes[0].highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.nodes[9].highlighted);
}

void test_highlight_daat(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(10, -1);
    TEST_ASSERT_EQUAL_INT(1, layout.nodes[10].highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.nodes[0].highlighted);
}

void test_highlight_path(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, 7);
    TEST_ASSERT_EQUAL_INT(7, layout.active_path);
    TEST_ASSERT_EQUAL_INT(1, layout.paths[7].highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.paths[0].highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.paths[21].highlighted);
}

void test_highlight_both(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(3, 12);
    TEST_ASSERT_EQUAL_INT(1, layout.nodes[3].highlighted);
    TEST_ASSERT_EQUAL_INT(1, layout.paths[12].highlighted);
}

/* --- Title and bounds --- */

void test_title_position(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.title_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.title_y);
}

void test_bounds(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.bounds_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.00f, layout.bounds_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.bounds_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.95f, layout.bounds_h);
}

/* --- Accessor: tree_of_life_node --- */

void test_accessor_node_keter(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_node_t node = tree_of_life_node(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, node.id);
    TEST_ASSERT_EQUAL_STRING("Keter", node.name);
}

void test_accessor_node_daat(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_node_t node = tree_of_life_node(&layout, 10);
    TEST_ASSERT_EQUAL_INT(10, node.id);
    TEST_ASSERT_EQUAL_STRING("Da'at", node.name);
}

void test_accessor_node_invalid_negative(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_node_t node = tree_of_life_node(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, node.id);
}

void test_accessor_node_invalid_high(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_node_t node = tree_of_life_node(&layout, 11);
    TEST_ASSERT_EQUAL_INT(-1, node.id);
}

void test_accessor_node_null_layout(void)
{
    tol_node_t node = tree_of_life_node(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, node.id);
}

/* --- Accessor: tree_of_life_path --- */

void test_accessor_path_valid(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_path_t path = tree_of_life_path(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, path.path_id);
    TEST_ASSERT_EQUAL_STRING("Aleph", path.hebrew_letter);
}

void test_accessor_path_last(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_path_t path = tree_of_life_path(&layout, 21);
    TEST_ASSERT_EQUAL_INT(21, path.path_id);
    TEST_ASSERT_EQUAL_STRING("Tav", path.hebrew_letter);
}

void test_accessor_path_invalid_negative(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_path_t path = tree_of_life_path(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, path.path_id);
}

void test_accessor_path_invalid_high(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, -1);
    tol_path_t path = tree_of_life_path(&layout, 22);
    TEST_ASSERT_EQUAL_INT(-1, path.path_id);
}

void test_accessor_path_null_layout(void)
{
    tol_path_t path = tree_of_life_path(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, path.path_id);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    tree_of_life_layout_t a = tree_of_life_compute(3, 5);
    tree_of_life_layout_t b = tree_of_life_compute(3, 5);
    TEST_ASSERT_EQUAL_INT(a.node_count, b.node_count);
    TEST_ASSERT_EQUAL_INT(a.path_count, b.path_count);
    for (int i = 0; i < TOL_NODE_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.nodes[i].x, b.nodes[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.nodes[i].y, b.nodes[i].y);
        TEST_ASSERT_EQUAL_INT(a.nodes[i].highlighted, b.nodes[i].highlighted);
    }
    for (int i = 0; i < TOL_PATH_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.paths[i].x0, b.paths[i].x0);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.paths[i].y0, b.paths[i].y0);
    }
}

void test_purity_different_highlight(void)
{
    tree_of_life_layout_t a = tree_of_life_compute(0, -1);
    tree_of_life_layout_t b = tree_of_life_compute(9, -1);
    TEST_ASSERT_EQUAL_INT(1, a.nodes[0].highlighted);
    TEST_ASSERT_EQUAL_INT(0, a.nodes[9].highlighted);
    TEST_ASSERT_EQUAL_INT(0, b.nodes[0].highlighted);
    TEST_ASSERT_EQUAL_INT(1, b.nodes[9].highlighted);
}

/* --- Invalid highlight range --- */

void test_invalid_sefirah_highlight_out_of_range(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(99, -1);
    /* No node should be highlighted */
    for (int i = 0; i < TOL_NODE_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.nodes[i].highlighted);
    }
}

void test_invalid_path_highlight_out_of_range(void)
{
    tree_of_life_layout_t layout = tree_of_life_compute(-1, 99);
    for (int i = 0; i < TOL_PATH_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.paths[i].highlighted);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* Node count and path count */
    RUN_TEST(test_node_count);
    RUN_TEST(test_path_count);
    /* Node data */
    RUN_TEST(test_keter_name);
    RUN_TEST(test_malkuth_name);
    RUN_TEST(test_daat_node);
    RUN_TEST(test_node_ids);
    /* Node positions */
    RUN_TEST(test_node_positions_in_bounds);
    RUN_TEST(test_keter_at_top);
    RUN_TEST(test_malkuth_at_bottom);
    RUN_TEST(test_keter_centered);
    RUN_TEST(test_chokmah_right_pillar);
    RUN_TEST(test_binah_left_pillar);
    /* Node colors */
    RUN_TEST(test_keter_white);
    RUN_TEST(test_gevurah_red);
    /* Pillar names */
    RUN_TEST(test_keter_pillar_balance);
    RUN_TEST(test_chokmah_pillar_mercy);
    RUN_TEST(test_binah_pillar_severity);
    /* Path data */
    RUN_TEST(test_first_path_aleph);
    RUN_TEST(test_last_path_tav);
    RUN_TEST(test_path_from_to);
    RUN_TEST(test_path_endpoints_in_bounds);
    RUN_TEST(test_path_endpoints_match_nodes);
    /* Highlighting */
    RUN_TEST(test_no_highlight);
    RUN_TEST(test_highlight_sefirah);
    RUN_TEST(test_highlight_daat);
    RUN_TEST(test_highlight_path);
    RUN_TEST(test_highlight_both);
    /* Title and bounds */
    RUN_TEST(test_title_position);
    RUN_TEST(test_bounds);
    /* Accessor: node */
    RUN_TEST(test_accessor_node_keter);
    RUN_TEST(test_accessor_node_daat);
    RUN_TEST(test_accessor_node_invalid_negative);
    RUN_TEST(test_accessor_node_invalid_high);
    RUN_TEST(test_accessor_node_null_layout);
    /* Accessor: path */
    RUN_TEST(test_accessor_path_valid);
    RUN_TEST(test_accessor_path_last);
    RUN_TEST(test_accessor_path_invalid_negative);
    RUN_TEST(test_accessor_path_invalid_high);
    RUN_TEST(test_accessor_path_null_layout);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_highlight);
    /* Invalid highlight */
    RUN_TEST(test_invalid_sefirah_highlight_out_of_range);
    RUN_TEST(test_invalid_path_highlight_out_of_range);
    return UNITY_END();
}
