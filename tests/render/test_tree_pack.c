/*
 * test_tree_pack.c -- Tests for Kabbalah Tree of Life vertex packing
 *
 * RED phase: 48 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/tree_pack.h"
#include "../../src/systems/kabbalah/tree_geometry.h"
#include "../../src/systems/kabbalah/sefirot.h"

#include <math.h>
#include <string.h>

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * trp_default_config
 * ====================================================================== */

/* 1. Default config returns positive dimensions */
void test_default_config_positive_dimensions(void)
{
    trp_config_t cfg = trp_default_config();
    TEST_ASSERT_TRUE(cfg.width > 0.0f);
    TEST_ASSERT_TRUE(cfg.height > 0.0f);
}

/* 2. Default config node_radius is positive */
void test_default_config_node_radius(void)
{
    trp_config_t cfg = trp_default_config();
    TEST_ASSERT_TRUE(cfg.node_radius > 0.0f);
}

/* 3. Default config alpha values in valid range */
void test_default_config_alpha_values(void)
{
    trp_config_t cfg = trp_default_config();
    TEST_ASSERT_TRUE(cfg.active_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.active_alpha <= 1.0f);
    TEST_ASSERT_TRUE(cfg.inactive_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.inactive_alpha < cfg.active_alpha);
}

/* 4. Default config offsets are finite */
void test_default_config_offsets(void)
{
    trp_config_t cfg = trp_default_config();
    TEST_ASSERT_TRUE(cfg.offset_x == cfg.offset_x); /* not NaN */
    TEST_ASSERT_TRUE(cfg.offset_y == cfg.offset_y);
}

/* 5. Default config show_daat is 0 */
void test_default_config_show_daat(void)
{
    trp_config_t cfg = trp_default_config();
    TEST_ASSERT_EQUAL_INT(0, cfg.show_daat);
}

/* ======================================================================
 * trp_node_color
 * ====================================================================== */

/* 6. Keter (0) color is white-ish (high R,G,B) */
void test_node_color_keter_white(void)
{
    float c[4];
    trp_node_color(0, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.8f);
    TEST_ASSERT_TRUE(c[1] > 0.8f);
    TEST_ASSERT_TRUE(c[2] > 0.8f);
    FCLOSE(1.0f, c[3]);
}

/* 7. Gevurah (4) color is red-ish */
void test_node_color_gevurah_red(void)
{
    float c[4];
    trp_node_color(4, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.5f);  /* strong red */
    TEST_ASSERT_TRUE(c[0] > c[1]);  /* redder than green */
    TEST_ASSERT_TRUE(c[0] > c[2]);  /* redder than blue */
}

/* 8. Node color respects alpha */
void test_node_color_alpha(void)
{
    float c[4];
    trp_node_color(5, 0.42f, c);
    FCLOSE(0.42f, c[3]);
}

/* 9. All node colors have RGB in [0,1] */
void test_node_color_range(void)
{
    for (int i = 0; i < SEFIROT_COUNT; i++) {
        float c[4];
        trp_node_color(i, 1.0f, c);
        for (int j = 0; j < 4; j++) {
            TEST_ASSERT_TRUE(c[j] >= 0.0f);
            TEST_ASSERT_TRUE(c[j] <= 1.0f);
        }
    }
}

/* 10. Different Sefirot have distinct colors */
void test_node_color_varies_by_sefirah(void)
{
    float c1[4], c2[4];
    trp_node_color(0, 1.0f, c1); /* Keter = white */
    trp_node_color(4, 1.0f, c2); /* Gevurah = red */
    int differs = (fabsf(c1[0] - c2[0]) > 0.01f) ||
                  (fabsf(c1[1] - c2[1]) > 0.01f) ||
                  (fabsf(c1[2] - c2[2]) > 0.01f);
    TEST_ASSERT_TRUE(differs);
}

/* 11. Da'at (index 10) color is valid */
void test_node_color_daat(void)
{
    float c[4];
    trp_node_color(10, 0.8f, c);
    for (int j = 0; j < 3; j++) {
        TEST_ASSERT_TRUE(c[j] >= 0.0f);
        TEST_ASSERT_TRUE(c[j] <= 1.0f);
    }
    FCLOSE(0.8f, c[3]);
}

/* 12. Invalid index clamps to safe color */
void test_node_color_invalid_index(void)
{
    float c[4];
    trp_node_color(-1, 1.0f, c);
    /* Should still produce valid RGBA */
    for (int j = 0; j < 4; j++) {
        TEST_ASSERT_TRUE(c[j] >= 0.0f);
        TEST_ASSERT_TRUE(c[j] <= 1.0f);
    }
}

/* ======================================================================
 * trp_path_color
 * ====================================================================== */

/* 13. Mother path (active) color is golden */
void test_path_color_mother_active(void)
{
    float c[4];
    trp_path_color(TREE_LETTER_MOTHER, 1, 0.9f, c);
    TEST_ASSERT_TRUE(c[0] > 0.6f); /* warm golden */
    TEST_ASSERT_TRUE(c[3] > 0.5f);
}

/* 14. Double path (active) color is silver-ish */
void test_path_color_double_active(void)
{
    float c[4];
    trp_path_color(TREE_LETTER_DOUBLE, 1, 0.9f, c);
    /* Silver: R ~= G ~= B and moderately bright */
    TEST_ASSERT_FLOAT_WITHIN(0.15f, c[0], c[1]);
    TEST_ASSERT_TRUE(c[0] > 0.3f);
}

/* 15. Simple path (active) has color */
void test_path_color_simple_active(void)
{
    float c[4];
    trp_path_color(TREE_LETTER_SIMPLE, 1, 0.9f, c);
    /* At least one component should be noticeable */
    TEST_ASSERT_TRUE(c[0] > 0.1f || c[1] > 0.1f || c[2] > 0.1f);
}

/* 16. Inactive path is dimmer */
void test_path_color_inactive_dim(void)
{
    float c_active[4], c_inactive[4];
    trp_path_color(TREE_LETTER_MOTHER, 1, 0.9f, c_active);
    trp_path_color(TREE_LETTER_MOTHER, 0, 0.3f, c_inactive);
    TEST_ASSERT_TRUE(c_active[3] > c_inactive[3]);
}

/* 17. Path colors in valid range */
void test_path_color_range(void)
{
    for (int t = 0; t <= 2; t++) {
        for (int active = 0; active <= 1; active++) {
            float c[4];
            trp_path_color(t, active, 0.9f, c);
            for (int j = 0; j < 4; j++) {
                TEST_ASSERT_TRUE(c[j] >= 0.0f);
                TEST_ASSERT_TRUE(c[j] <= 1.0f);
            }
        }
    }
}

/* ======================================================================
 * trp_pack_nodes
 * ====================================================================== */

/* 18. Pack 10 nodes (no Da'at) -> 10 * 6 = 60 verts */
void test_pack_nodes_no_daat(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 0;
    int active[11] = {0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_nodes(active, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(10 * TRP_NODE_VERTS, n);
}

/* 19. Pack 11 nodes (with Da'at) -> 11 * 6 = 66 verts */
void test_pack_nodes_with_daat(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active[11] = {0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_nodes(active, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(11 * TRP_NODE_VERTS, n);
}

/* 20. Active node uses Queen Scale color with active alpha */
void test_pack_nodes_active_color(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 0;
    int active[11] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_nodes(active, &cfg, buf);

    /* First vertex of Keter (index 0): color at offset 2..5 */
    float r = buf[2];
    float a = buf[5];
    TEST_ASSERT_TRUE(r > 0.5f); /* Keter is white => R high */
    FCLOSE(cfg.active_alpha, a);
}

/* 21. Inactive node uses inactive alpha */
void test_pack_nodes_inactive_alpha(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 0;
    int active[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_nodes(active, &cfg, buf);

    /* First vertex alpha */
    float a = buf[5];
    FCLOSE(cfg.inactive_alpha, a);
}

/* 22. Node positions are within screen bounds */
void test_pack_nodes_screen_coords(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.width = 200.0f;
    cfg.height = 400.0f;
    cfg.offset_x = 10.0f;
    cfg.offset_y = 20.0f;
    cfg.node_radius = 8.0f;
    cfg.show_daat = 0;

    int active[11] = {0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_nodes(active, &cfg, buf);

    /* Keter is at pos (0.0, 0.0) => scaled: (0.5*200+10, 0.0*400+20)=(110,20)
     * Average of 6 quad vertices should be ~(110,20) */
    float avg_x = 0.0f, avg_y = 0.0f;
    for (int v = 0; v < TRP_NODE_VERTS; v++) {
        avg_x += buf[v * TRP_VERTEX_FLOATS + 0];
        avg_y += buf[v * TRP_VERTEX_FLOATS + 1];
    }
    avg_x /= (float)TRP_NODE_VERTS;
    avg_y /= (float)TRP_NODE_VERTS;
    FCLOSE(110.0f, avg_x);
    FCLOSE(20.0f, avg_y);
}

/* 23. All node vertex colors in [0,1] */
void test_pack_nodes_color_range(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active[11] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_nodes(active, &cfg, buf);
    for (int i = 0; i < n; i++) {
        int base = i * TRP_VERTEX_FLOATS;
        for (int j = 2; j <= 5; j++) {
            TEST_ASSERT_TRUE(buf[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * trp_pack_paths
 * ====================================================================== */

/* 24. Pack all 22 paths -> 22 * 2 = 44 verts */
void test_pack_paths_count(void)
{
    trp_config_t cfg = trp_default_config();
    int active[22] = {0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_paths(active, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(22 * TRP_PATH_VERTS, n);
}

/* 25. Active path has higher alpha than inactive */
void test_pack_paths_active_alpha(void)
{
    trp_config_t cfg = trp_default_config();
    int active[22] = {0};
    active[0] = 1; /* Aleph path active */
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_paths(active, &cfg, buf);

    /* First path (Aleph): vertex 0 alpha at offset 5 */
    float a0 = buf[5];
    FCLOSE(cfg.active_alpha, a0);

    /* Second path (Bet, inactive): vertex at offset 2*6 = 12, alpha at 12+5=17 */
    float a1 = buf[2 * TRP_VERTEX_FLOATS + 5];
    FCLOSE(cfg.inactive_alpha, a1);
}

/* 26. Path endpoints connect different Sefirot positions */
void test_pack_paths_endpoints_differ(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.width = 100.0f;
    cfg.height = 100.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;

    int active[22] = {0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_paths(active, &cfg, buf);

    /* Path 0 (Aleph): Keter(0) to Chokmah(1).
     * Keter at (0.0, 0.0) => (50, 0), Chokmah at (0.5, 0.17) => (100, 17).
     * Endpoints should differ. */
    float x0 = buf[0];
    float y0 = buf[1];
    float x1 = buf[TRP_VERTEX_FLOATS + 0];
    float y1 = buf[TRP_VERTEX_FLOATS + 1];
    float dist = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    TEST_ASSERT_TRUE(dist > 1.0f);
}

/* 27. Path colors vary by type */
void test_pack_paths_color_by_type(void)
{
    trp_config_t cfg = trp_default_config();
    int active[22];
    for (int i = 0; i < 22; i++) active[i] = 1;
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_paths(active, &cfg, buf);

    /* Path 0 = Mother (Aleph), Path 1 = Double (Bet).
     * Mother should be golden, Double should be silver.
     * At least the red channels should differ. */
    float r_mother = buf[2]; /* path 0, vert 0, R */
    float r_double = buf[2 * TRP_VERTEX_FLOATS + 2]; /* path 1, vert 0, R */
    int differs = (fabsf(r_mother - r_double) > 0.01f);
    /* Mother(gold) and Double(silver) should have different color profiles */
    TEST_ASSERT_TRUE(differs);
}

/* 28. All path vertex colors in [0,1] */
void test_pack_paths_color_range(void)
{
    trp_config_t cfg = trp_default_config();
    int active[22];
    for (int i = 0; i < 22; i++) active[i] = i % 2;
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_paths(active, &cfg, buf);
    for (int i = 0; i < n; i++) {
        int base = i * TRP_VERTEX_FLOATS;
        for (int j = 2; j <= 5; j++) {
            TEST_ASSERT_TRUE(buf[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * trp_pack_pillars
 * ====================================================================== */

/* 29. Pack 3 pillars -> 3 * 2 = 6 verts */
void test_pack_pillars_count(void)
{
    trp_config_t cfg = trp_default_config();
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_pillars(&cfg, buf);
    TEST_ASSERT_EQUAL_INT(3 * TRP_PILLAR_VERTS, n);
}

/* 30. Pillar lines are vertical (x0 ~= x1 for each) */
void test_pack_pillars_vertical(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.width = 200.0f;
    cfg.height = 400.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;

    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_pillars(&cfg, buf);

    /* Each pillar is 2 verts = 12 floats. Check x0 ~= x1 for each pillar. */
    for (int p = 0; p < 3; p++) {
        int base = p * 2 * TRP_VERTEX_FLOATS;
        float x0 = buf[base + 0];
        float x1 = buf[base + TRP_VERTEX_FLOATS + 0];
        FCLOSE(x0, x1);
    }
}

/* 31. Middle pillar is between left and right */
void test_pack_pillars_middle_between(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.width = 200.0f;
    cfg.height = 400.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;

    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    trp_pack_pillars(&cfg, buf);

    /* Collect x positions of each pillar's first vertex */
    float x[3];
    for (int p = 0; p < 3; p++) {
        x[p] = buf[p * 2 * TRP_VERTEX_FLOATS + 0];
    }
    /* Sort to find min, mid, max */
    float xmin = x[0], xmax = x[0], xmid = x[0];
    for (int i = 1; i < 3; i++) {
        if (x[i] < xmin) xmin = x[i];
        if (x[i] > xmax) xmax = x[i];
    }
    /* Find the one that's neither min nor max */
    for (int i = 0; i < 3; i++) {
        if (fabsf(x[i] - xmin) > 0.1f && fabsf(x[i] - xmax) > 0.1f) {
            xmid = x[i];
        }
    }
    TEST_ASSERT_TRUE(xmid > xmin);
    TEST_ASSERT_TRUE(xmid < xmax);
}

/* 32. Pillar colors in valid range */
void test_pack_pillars_color_range(void)
{
    trp_config_t cfg = trp_default_config();
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack_pillars(&cfg, buf);
    for (int i = 0; i < n; i++) {
        int base = i * TRP_VERTEX_FLOATS;
        for (int j = 2; j <= 5; j++) {
            TEST_ASSERT_TRUE(buf[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * trp_pack (combined)
 * ====================================================================== */

/* 33. Empty tree (no Da'at, no active): 10*6 + 22*2 + 3*2 = 110 verts */
void test_pack_empty_tree(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 0;
    int active_nodes[11] = {0};
    int active_paths[22] = {0};
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack(active_nodes, active_paths, &cfg, buf);
    int expected = 10 * TRP_NODE_VERTS + 22 * TRP_PATH_VERTS
                 + 3 * TRP_PILLAR_VERTS;
    TEST_ASSERT_EQUAL_INT(expected, n);
}

/* 34. Full tree with Da'at: 11*6 + 22*2 + 3*2 = 116 verts */
void test_pack_full_tree(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active_nodes[11];
    int active_paths[22];
    for (int i = 0; i < 11; i++) active_nodes[i] = 1;
    for (int i = 0; i < 22; i++) active_paths[i] = 1;
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack(active_nodes, active_paths, &cfg, buf);
    int expected = 11 * TRP_NODE_VERTS + 22 * TRP_PATH_VERTS
                 + 3 * TRP_PILLAR_VERTS;
    TEST_ASSERT_EQUAL_INT(expected, n);
}

/* 35. Pack total never exceeds TRP_MAX_VERTICES */
void test_pack_within_max_vertices(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active_nodes[11];
    int active_paths[22];
    for (int i = 0; i < 11; i++) active_nodes[i] = 1;
    for (int i = 0; i < 22; i++) active_paths[i] = 1;
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack(active_nodes, active_paths, &cfg, buf);
    TEST_ASSERT_TRUE(n <= TRP_MAX_VERTICES);
}

/* 36. All vertices in combined pack have valid colors */
void test_pack_all_colors_valid(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active_nodes[11] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    int active_paths[22];
    for (int i = 0; i < 22; i++) active_paths[i] = i % 3;
    float buf[TRP_MAX_VERTICES * TRP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = trp_pack(active_nodes, active_paths, &cfg, buf);
    for (int i = 0; i < n; i++) {
        int base = i * TRP_VERTEX_FLOATS;
        for (int j = 2; j <= 5; j++) {
            TEST_ASSERT_TRUE(buf[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * trp_info
 * ====================================================================== */

/* 37. Info with no active, no Da'at */
void test_info_empty(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 0;
    int active_nodes[11] = {0};
    int active_paths[22] = {0};
    trp_info_t info = trp_info(active_nodes, active_paths, &cfg);
    TEST_ASSERT_EQUAL_INT(0, info.active_nodes);
    TEST_ASSERT_EQUAL_INT(0, info.active_paths);
    TEST_ASSERT_EQUAL_INT(10 * TRP_NODE_VERTS, info.node_verts);
    TEST_ASSERT_EQUAL_INT(22 * TRP_PATH_VERTS, info.path_verts);
    TEST_ASSERT_EQUAL_INT(3 * TRP_PILLAR_VERTS, info.pillar_verts);
}

/* 38. Info with some active nodes and paths */
void test_info_some_active(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 0;
    int active_nodes[11] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int active_paths[22] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    trp_info_t info = trp_info(active_nodes, active_paths, &cfg);
    TEST_ASSERT_EQUAL_INT(2, info.active_nodes);
    TEST_ASSERT_EQUAL_INT(1, info.active_paths);
}

/* 39. Info total verts is sum of parts */
void test_info_total_is_sum(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active_nodes[11] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    int active_paths[22] = {0};
    trp_info_t info = trp_info(active_nodes, active_paths, &cfg);
    TEST_ASSERT_EQUAL_INT(info.node_verts + info.path_verts
                          + info.pillar_verts, info.total_verts);
}

/* 40. Info with Da'at shows 11 nodes worth of verts */
void test_info_daat_node_verts(void)
{
    trp_config_t cfg = trp_default_config();
    cfg.show_daat = 1;
    int active_nodes[11] = {0};
    int active_paths[22] = {0};
    trp_info_t info = trp_info(active_nodes, active_paths, &cfg);
    TEST_ASSERT_EQUAL_INT(11 * TRP_NODE_VERTS, info.node_verts);
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 41. Vertex shader starts with #version 300 es */
void test_vert_source_version(void)
{
    const char *src = trp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 42. Fragment shader starts with #version 300 es */
void test_frag_source_version(void)
{
    const char *src = trp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 43. Vertex shader has u_projection uniform */
void test_vert_source_has_projection(void)
{
    const char *src = trp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_projection"));
}

/* 44. Vertex shader has a_position attribute */
void test_vert_source_has_position(void)
{
    const char *src = trp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

/* 45. Vertex shader has a_color attribute */
void test_vert_source_has_color(void)
{
    const char *src = trp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_color"));
}

/* 46. Fragment shader has frag_color output */
void test_frag_source_has_output(void)
{
    const char *src = trp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* ======================================================================
 * Constants
 * ====================================================================== */

/* 47. TRP_VERTEX_FLOATS is 6 */
void test_vertex_floats_is_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, TRP_VERTEX_FLOATS);
}

/* 48. TRP_VERTEX_STRIDE is 24 (6 * 4) */
void test_vertex_stride_is_24(void)
{
    TEST_ASSERT_EQUAL_INT(24, TRP_VERTEX_STRIDE);
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Config */
    RUN_TEST(test_default_config_positive_dimensions);
    RUN_TEST(test_default_config_node_radius);
    RUN_TEST(test_default_config_alpha_values);
    RUN_TEST(test_default_config_offsets);
    RUN_TEST(test_default_config_show_daat);

    /* Node color */
    RUN_TEST(test_node_color_keter_white);
    RUN_TEST(test_node_color_gevurah_red);
    RUN_TEST(test_node_color_alpha);
    RUN_TEST(test_node_color_range);
    RUN_TEST(test_node_color_varies_by_sefirah);
    RUN_TEST(test_node_color_daat);
    RUN_TEST(test_node_color_invalid_index);

    /* Path color */
    RUN_TEST(test_path_color_mother_active);
    RUN_TEST(test_path_color_double_active);
    RUN_TEST(test_path_color_simple_active);
    RUN_TEST(test_path_color_inactive_dim);
    RUN_TEST(test_path_color_range);

    /* Pack nodes */
    RUN_TEST(test_pack_nodes_no_daat);
    RUN_TEST(test_pack_nodes_with_daat);
    RUN_TEST(test_pack_nodes_active_color);
    RUN_TEST(test_pack_nodes_inactive_alpha);
    RUN_TEST(test_pack_nodes_screen_coords);
    RUN_TEST(test_pack_nodes_color_range);

    /* Pack paths */
    RUN_TEST(test_pack_paths_count);
    RUN_TEST(test_pack_paths_active_alpha);
    RUN_TEST(test_pack_paths_endpoints_differ);
    RUN_TEST(test_pack_paths_color_by_type);
    RUN_TEST(test_pack_paths_color_range);

    /* Pack pillars */
    RUN_TEST(test_pack_pillars_count);
    RUN_TEST(test_pack_pillars_vertical);
    RUN_TEST(test_pack_pillars_middle_between);
    RUN_TEST(test_pack_pillars_color_range);

    /* Pack combined */
    RUN_TEST(test_pack_empty_tree);
    RUN_TEST(test_pack_full_tree);
    RUN_TEST(test_pack_within_max_vertices);
    RUN_TEST(test_pack_all_colors_valid);

    /* Info */
    RUN_TEST(test_info_empty);
    RUN_TEST(test_info_some_active);
    RUN_TEST(test_info_total_is_sum);
    RUN_TEST(test_info_daat_node_verts);

    /* Shaders */
    RUN_TEST(test_vert_source_version);
    RUN_TEST(test_frag_source_version);
    RUN_TEST(test_vert_source_has_projection);
    RUN_TEST(test_vert_source_has_position);
    RUN_TEST(test_vert_source_has_color);
    RUN_TEST(test_frag_source_has_output);

    /* Constants */
    RUN_TEST(test_vertex_floats_is_6);
    RUN_TEST(test_vertex_stride_is_24);

    return UNITY_END();
}
