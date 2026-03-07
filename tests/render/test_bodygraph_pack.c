/*
 * test_bodygraph_pack.c -- Tests for Human Design bodygraph vertex packing
 *
 * RED phase: 42 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/bodygraph_pack.h"
#include "../../src/systems/human_design/bodygraph.h"

#include <math.h>
#include <string.h>

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * bgp_default_config
 * ====================================================================== */

/* 1. Default config returns positive dimensions */
void test_default_config_positive_dimensions(void)
{
    bgp_config_t cfg = bgp_default_config();
    TEST_ASSERT_TRUE(cfg.width > 0.0f);
    TEST_ASSERT_TRUE(cfg.height > 0.0f);
}

/* 2. Default config center_radius is positive */
void test_default_config_center_radius(void)
{
    bgp_config_t cfg = bgp_default_config();
    TEST_ASSERT_TRUE(cfg.center_radius > 0.0f);
}

/* 3. Default config gate_radius is positive and smaller than center_radius */
void test_default_config_gate_radius(void)
{
    bgp_config_t cfg = bgp_default_config();
    TEST_ASSERT_TRUE(cfg.gate_radius > 0.0f);
    TEST_ASSERT_TRUE(cfg.gate_radius < cfg.center_radius);
}

/* 4. Default config alpha values in valid range */
void test_default_config_alpha_values(void)
{
    bgp_config_t cfg = bgp_default_config();
    TEST_ASSERT_TRUE(cfg.defined_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.defined_alpha <= 1.0f);
    TEST_ASSERT_TRUE(cfg.undefined_alpha > 0.0f);
    TEST_ASSERT_TRUE(cfg.undefined_alpha < cfg.defined_alpha);
}

/* 5. Default config offsets are reasonable */
void test_default_config_offsets(void)
{
    bgp_config_t cfg = bgp_default_config();
    /* Offsets can be zero or any value; just ensure they are finite */
    TEST_ASSERT_TRUE(cfg.offset_x == cfg.offset_x); /* not NaN */
    TEST_ASSERT_TRUE(cfg.offset_y == cfg.offset_y);
}

/* ======================================================================
 * bgp_defined_color
 * ====================================================================== */

/* 6. Defined center color has warm gold characteristics (R > G > B) */
void test_defined_color_warm_gold(void)
{
    float c[4];
    bgp_defined_color(BG_HEAD, 1.0f, c);
    TEST_ASSERT_TRUE(c[0] > 0.5f);  /* strong red component */
    TEST_ASSERT_TRUE(c[0] >= c[1]); /* red >= green (warm) */
    TEST_ASSERT_TRUE(c[1] > c[2]);  /* green > blue (golden) */
    FCLOSE(1.0f, c[3]);
}

/* 7. Defined color respects alpha parameter */
void test_defined_color_alpha(void)
{
    float c[4];
    bgp_defined_color(BG_SACRAL, 0.42f, c);
    FCLOSE(0.42f, c[3]);
}

/* 8. Different centers may have distinct hues */
void test_defined_color_varies_by_center(void)
{
    float c1[4], c2[4];
    bgp_defined_color(BG_HEAD, 1.0f, c1);
    bgp_defined_color(BG_ROOT, 1.0f, c2);
    /* Colors should be different (distinct per center) */
    int differs = (fabsf(c1[0] - c2[0]) > 0.01f) ||
                  (fabsf(c1[1] - c2[1]) > 0.01f) ||
                  (fabsf(c1[2] - c2[2]) > 0.01f);
    TEST_ASSERT_TRUE(differs);
}

/* 9. All defined colors have RGB in [0,1] */
void test_defined_color_range(void)
{
    for (int i = 0; i < BG_CENTER_COUNT; i++) {
        float c[4];
        bgp_defined_color((bg_center_id_t)i, 1.0f, c);
        for (int j = 0; j < 4; j++) {
            TEST_ASSERT_TRUE(c[j] >= 0.0f);
            TEST_ASSERT_TRUE(c[j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * bgp_undefined_color
 * ====================================================================== */

/* 10. Undefined center color is grey (R ~= G ~= B) */
void test_undefined_color_grey(void)
{
    float c[4];
    bgp_undefined_color(1.0f, c);
    FCLOSE(c[0], c[1]);
    FCLOSE(c[1], c[2]);
}

/* 11. Undefined color is muted (low saturation, low brightness) */
void test_undefined_color_muted(void)
{
    float c[4];
    bgp_undefined_color(1.0f, c);
    TEST_ASSERT_TRUE(c[0] < 0.6f);
    TEST_ASSERT_TRUE(c[0] > 0.1f);
}

/* 12. Undefined color respects alpha */
void test_undefined_color_alpha(void)
{
    float c[4];
    bgp_undefined_color(0.25f, c);
    FCLOSE(0.25f, c[3]);
}

/* ======================================================================
 * bgp_channel_color
 * ====================================================================== */

/* 13. Active channel color is bright */
void test_channel_color_active(void)
{
    float c[4];
    bgp_channel_color(1, 0.9f, 0.25f, c);
    /* Active channel should be visible: at least one component > 0.5 */
    TEST_ASSERT_TRUE(c[0] > 0.3f || c[1] > 0.3f || c[2] > 0.3f);
    FCLOSE(0.9f, c[3]);
}

/* 14. Inactive channel color is dim */
void test_channel_color_inactive(void)
{
    float c[4];
    bgp_channel_color(0, 0.9f, 0.25f, c);
    FCLOSE(0.25f, c[3]);
}

/* 15. Channel colors in valid range */
void test_channel_color_range(void)
{
    float c[4];
    bgp_channel_color(1, 0.9f, 0.25f, c);
    for (int j = 0; j < 4; j++) {
        TEST_ASSERT_TRUE(c[j] >= 0.0f);
        TEST_ASSERT_TRUE(c[j] <= 1.0f);
    }
    bgp_channel_color(0, 0.9f, 0.25f, c);
    for (int j = 0; j < 4; j++) {
        TEST_ASSERT_TRUE(c[j] >= 0.0f);
        TEST_ASSERT_TRUE(c[j] <= 1.0f);
    }
}

/* ======================================================================
 * bgp_pack_centers
 * ====================================================================== */

/* 16. Pack centers with no defined centers writes 9 quads */
void test_pack_centers_all_undefined(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_centers(defined, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(9 * BGP_CENTER_VERTS, n);
}

/* 17. Pack centers with all defined writes 9 quads */
void test_pack_centers_all_defined(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_centers(defined, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(9 * BGP_CENTER_VERTS, n);
}

/* 18. Defined center uses warm color (check first center when defined) */
void test_pack_centers_defined_color(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    bgp_pack_centers(defined, &cfg, buf);

    /* First vertex of first center: color at offset 2..5 (after vec2 pos) */
    float r = buf[2];
    float g = buf[3];
    float b = buf[4];
    float a = buf[5];
    TEST_ASSERT_TRUE(r > 0.5f);   /* warm gold red component */
    TEST_ASSERT_TRUE(a > 0.5f);   /* defined alpha */
    (void)g; (void)b;
}

/* 19. Undefined center uses grey color */
void test_pack_centers_undefined_color(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    bgp_pack_centers(defined, &cfg, buf);

    /* First vertex color of first (undefined) center */
    float r = buf[2];
    float g = buf[3];
    float a = buf[5];
    /* Grey: R ~= G */
    TEST_ASSERT_FLOAT_WITHIN(0.1f, r, g);
    TEST_ASSERT_TRUE(a < cfg.defined_alpha); /* lower alpha */
}

/* 20. Center positions are transformed to screen space */
void test_pack_centers_screen_coords(void)
{
    bgp_config_t cfg = bgp_default_config();
    cfg.width = 200.0f;
    cfg.height = 400.0f;
    cfg.offset_x = 50.0f;
    cfg.offset_y = 100.0f;
    cfg.center_radius = 10.0f;

    int defined[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    bgp_pack_centers(defined, &cfg, buf);

    /* Head center is at normalized (0.50, 0.05).
     * Screen: x = 0.50 * 200 + 50 = 150, y = 0.05 * 400 + 100 = 120.
     * The quad is centered around that point with radius offsets.
     * Average of all 6 vertex positions should be approx (150, 120). */
    float avg_x = 0.0f, avg_y = 0.0f;
    for (int v = 0; v < BGP_CENTER_VERTS; v++) {
        avg_x += buf[v * BGP_VERTEX_FLOATS + 0];
        avg_y += buf[v * BGP_VERTEX_FLOATS + 1];
    }
    avg_x /= (float)BGP_CENTER_VERTS;
    avg_y /= (float)BGP_CENTER_VERTS;
    FCLOSE(150.0f, avg_x);
    FCLOSE(120.0f, avg_y);
}

/* ======================================================================
 * bgp_pack_channels
 * ====================================================================== */

/* 21. No active gates -> all channels inactive, still 36*2 verts */
void test_pack_channels_none_active(void)
{
    bgp_config_t cfg = bgp_default_config();
    int gates[65];
    memset(gates, 0, sizeof(gates));
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_channels(gates, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(36 * BGP_CHANNEL_VERTS, n);
}

/* 22. Channel 64-47 active when both gates set */
void test_pack_channels_one_active(void)
{
    bgp_config_t cfg = bgp_default_config();
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[64] = 1;
    gates[47] = 1;
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_channels(gates, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(36 * BGP_CHANNEL_VERTS, n);

    /* First channel is 64-47 (Head-Ajna). If active, its alpha should be high */
    float a0 = buf[0 * BGP_VERTEX_FLOATS + 5]; /* first vertex alpha */
    TEST_ASSERT_TRUE(a0 > 0.5f); /* active channel: defined alpha */
}

/* 23. Channel line vertices connect two different center positions */
void test_pack_channels_endpoints_differ(void)
{
    bgp_config_t cfg = bgp_default_config();
    cfg.width = 100.0f;
    cfg.height = 100.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;

    int gates[65];
    memset(gates, 0, sizeof(gates));
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    bgp_pack_channels(gates, &cfg, buf);

    /* First channel (64-47, Head-Ajna): two endpoints */
    float y0 = buf[0 * BGP_VERTEX_FLOATS + 1];
    float y1 = buf[1 * BGP_VERTEX_FLOATS + 1];
    /* Head=(0.5, 0.05)*100=(50, 5), Ajna=(0.5, 0.20)*100=(50, 20) */
    /* y values should differ between the two centers */
    float dy = fabsf(y1 - y0);
    TEST_ASSERT_TRUE(dy > 1.0f); /* endpoints are not the same */
}

/* ======================================================================
 * bgp_pack_gates
 * ====================================================================== */

/* 24. No active gates -> 0 gate marker verts */
void test_pack_gates_none_active(void)
{
    bgp_config_t cfg = bgp_default_config();
    int gates[65];
    memset(gates, 0, sizeof(gates));
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_gates(gates, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* 25. One active gate -> 6 verts (one quad) */
void test_pack_gates_one_active(void)
{
    bgp_config_t cfg = bgp_default_config();
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[1] = 1; /* gate 1 on G Center */
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_gates(gates, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(BGP_GATE_VERTS, n);
}

/* 26. All 64 gates active -> 64 * 6 = 384 verts */
void test_pack_gates_all_active(void)
{
    bgp_config_t cfg = bgp_default_config();
    int gates[65];
    memset(gates, 0, sizeof(gates));
    for (int i = 1; i <= 64; i++) gates[i] = 1;
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_gates(gates, &cfg, buf);
    TEST_ASSERT_EQUAL_INT(64 * BGP_GATE_VERTS, n);
}

/* 27. Gate marker position is near its center */
void test_pack_gates_position_near_center(void)
{
    bgp_config_t cfg = bgp_default_config();
    cfg.width = 100.0f;
    cfg.height = 100.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;
    cfg.gate_radius = 1.0f;

    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[1] = 1; /* gate 1 belongs to G Center (0.50, 0.50) */

    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    bgp_pack_gates(gates, &cfg, buf);

    /* Average of 6 vertices should be near G Center at (50, 50) */
    float avg_x = 0.0f, avg_y = 0.0f;
    for (int v = 0; v < BGP_GATE_VERTS; v++) {
        avg_x += buf[v * BGP_VERTEX_FLOATS + 0];
        avg_y += buf[v * BGP_VERTEX_FLOATS + 1];
    }
    avg_x /= (float)BGP_GATE_VERTS;
    avg_y /= (float)BGP_GATE_VERTS;
    /* Should be within a few units of (50, 50) */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 50.0f, avg_x);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 50.0f, avg_y);
}

/* 28. Gate colors are in valid range */
void test_pack_gates_color_range(void)
{
    bgp_config_t cfg = bgp_default_config();
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[1] = 1;
    gates[64] = 1;
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack_gates(gates, &cfg, buf);
    for (int i = 0; i < n; i++) {
        int base = i * BGP_VERTEX_FLOATS;
        for (int j = 2; j <= 5; j++) { /* RGBA at offset 2-5 */
            TEST_ASSERT_TRUE(buf[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * bgp_pack (combined)
 * ====================================================================== */

/* 29. Empty chart: all undefined, no active gates */
void test_pack_empty_chart(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack(defined, gates, &cfg, buf);
    /* centers + channels + no gate markers */
    int expected = 9 * BGP_CENTER_VERTS + 36 * BGP_CHANNEL_VERTS + 0;
    TEST_ASSERT_EQUAL_INT(expected, n);
}

/* 30. Full chart: all defined, all gates active */
void test_pack_full_chart(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    for (int i = 1; i <= 64; i++) gates[i] = 1;
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack(defined, gates, &cfg, buf);
    int expected = 9 * BGP_CENTER_VERTS + 36 * BGP_CHANNEL_VERTS
                 + 64 * BGP_GATE_VERTS;
    TEST_ASSERT_EQUAL_INT(expected, n);
}

/* 31. Pack total never exceeds BGP_MAX_VERTICES */
void test_pack_within_max_vertices(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    for (int i = 1; i <= 64; i++) gates[i] = 1;
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack(defined, gates, &cfg, buf);
    TEST_ASSERT_TRUE(n <= BGP_MAX_VERTICES);
}

/* 32. All vertices in pack have colors in [0,1] */
void test_pack_all_colors_valid(void)
{
    bgp_config_t cfg = bgp_default_config();
    int defined[9] = {1, 0, 1, 0, 1, 0, 1, 0, 1};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[64] = 1; gates[47] = 1; /* activate channel 64-47 */
    gates[1] = 1;
    float buf[BGP_MAX_VERTICES * BGP_VERTEX_FLOATS];
    memset(buf, 0, sizeof(buf));
    int n = bgp_pack(defined, gates, &cfg, buf);
    for (int i = 0; i < n; i++) {
        int base = i * BGP_VERTEX_FLOATS;
        for (int j = 2; j <= 5; j++) {
            TEST_ASSERT_TRUE(buf[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(buf[base + j] <= 1.0f);
        }
    }
}

/* ======================================================================
 * bgp_info
 * ====================================================================== */

/* 33. Info with no gates: zero active channels and gates */
void test_info_empty(void)
{
    int defined[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    bgp_info_t info = bgp_info(defined, gates);
    TEST_ASSERT_EQUAL_INT(0, info.defined_centers);
    TEST_ASSERT_EQUAL_INT(0, info.active_channels);
    TEST_ASSERT_EQUAL_INT(0, info.active_gates);
}

/* 34. Info with some gates active */
void test_info_some_active(void)
{
    int defined[9] = {1, 1, 0, 0, 0, 0, 0, 0, 0};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[64] = 1; gates[47] = 1; /* channel 64-47 active */
    gates[1] = 1;
    bgp_info_t info = bgp_info(defined, gates);
    TEST_ASSERT_EQUAL_INT(2, info.defined_centers);
    TEST_ASSERT_EQUAL_INT(1, info.active_channels);
    TEST_ASSERT_EQUAL_INT(3, info.active_gates);
}

/* 35. Info vertex counts consistent */
void test_info_vertex_counts(void)
{
    int defined[9] = {1, 1, 0, 0, 0, 0, 0, 0, 0};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    gates[64] = 1; gates[47] = 1;
    gates[1] = 1;
    bgp_info_t info = bgp_info(defined, gates);
    TEST_ASSERT_EQUAL_INT(9 * BGP_CENTER_VERTS, info.center_verts);
    TEST_ASSERT_EQUAL_INT(36 * BGP_CHANNEL_VERTS, info.channel_verts);
    TEST_ASSERT_EQUAL_INT(3 * BGP_GATE_VERTS, info.gate_verts);
    TEST_ASSERT_EQUAL_INT(info.center_verts + info.channel_verts
                          + info.gate_verts, info.total_verts);
}

/* 36. Info total with full chart */
void test_info_full_chart(void)
{
    int defined[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int gates[65];
    memset(gates, 0, sizeof(gates));
    for (int i = 1; i <= 64; i++) gates[i] = 1;
    bgp_info_t info = bgp_info(defined, gates);
    TEST_ASSERT_EQUAL_INT(9, info.defined_centers);
    TEST_ASSERT_EQUAL_INT(64, info.active_gates);
    int expected_total = 9 * BGP_CENTER_VERTS + 36 * BGP_CHANNEL_VERTS
                       + 64 * BGP_GATE_VERTS;
    TEST_ASSERT_EQUAL_INT(expected_total, info.total_verts);
}

/* ======================================================================
 * Shader sources
 * ====================================================================== */

/* 37. Vertex shader starts with #version 300 es */
void test_vert_source_version(void)
{
    const char *src = bgp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 38. Fragment shader starts with #version 300 es */
void test_frag_source_version(void)
{
    const char *src = bgp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 39. Vertex shader has u_projection uniform */
void test_vert_source_has_projection(void)
{
    const char *src = bgp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_projection"));
}

/* 40. Vertex shader has a_position attribute */
void test_vert_source_has_position(void)
{
    const char *src = bgp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
}

/* 41. Vertex shader has a_color attribute */
void test_vert_source_has_color(void)
{
    const char *src = bgp_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "a_color"));
}

/* 42. Fragment shader has frag_color output */
void test_frag_source_has_output(void)
{
    const char *src = bgp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* ======================================================================
 * Constants
 * ====================================================================== */

/* 43. BGP_VERTEX_FLOATS is 6 */
void test_vertex_floats_is_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, BGP_VERTEX_FLOATS);
}

/* 44. BGP_VERTEX_STRIDE is 24 (6 * 4) */
void test_vertex_stride_is_24(void)
{
    TEST_ASSERT_EQUAL_INT(24, BGP_VERTEX_STRIDE);
}

/* 45. BGP_CENTER_VERTS is 6 */
void test_center_verts_is_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, BGP_CENTER_VERTS);
}

/* 46. BGP_CHANNEL_VERTS is 2 */
void test_channel_verts_is_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, BGP_CHANNEL_VERTS);
}

/* 47. BGP_MAX_VERTICES is 512 */
void test_max_vertices_is_512(void)
{
    TEST_ASSERT_EQUAL_INT(512, BGP_MAX_VERTICES);
}

/* ======================================================================
 * main
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Config */
    RUN_TEST(test_default_config_positive_dimensions);
    RUN_TEST(test_default_config_center_radius);
    RUN_TEST(test_default_config_gate_radius);
    RUN_TEST(test_default_config_alpha_values);
    RUN_TEST(test_default_config_offsets);

    /* Defined color */
    RUN_TEST(test_defined_color_warm_gold);
    RUN_TEST(test_defined_color_alpha);
    RUN_TEST(test_defined_color_varies_by_center);
    RUN_TEST(test_defined_color_range);

    /* Undefined color */
    RUN_TEST(test_undefined_color_grey);
    RUN_TEST(test_undefined_color_muted);
    RUN_TEST(test_undefined_color_alpha);

    /* Channel color */
    RUN_TEST(test_channel_color_active);
    RUN_TEST(test_channel_color_inactive);
    RUN_TEST(test_channel_color_range);

    /* Pack centers */
    RUN_TEST(test_pack_centers_all_undefined);
    RUN_TEST(test_pack_centers_all_defined);
    RUN_TEST(test_pack_centers_defined_color);
    RUN_TEST(test_pack_centers_undefined_color);
    RUN_TEST(test_pack_centers_screen_coords);

    /* Pack channels */
    RUN_TEST(test_pack_channels_none_active);
    RUN_TEST(test_pack_channels_one_active);
    RUN_TEST(test_pack_channels_endpoints_differ);

    /* Pack gates */
    RUN_TEST(test_pack_gates_none_active);
    RUN_TEST(test_pack_gates_one_active);
    RUN_TEST(test_pack_gates_all_active);
    RUN_TEST(test_pack_gates_position_near_center);
    RUN_TEST(test_pack_gates_color_range);

    /* Pack combined */
    RUN_TEST(test_pack_empty_chart);
    RUN_TEST(test_pack_full_chart);
    RUN_TEST(test_pack_within_max_vertices);
    RUN_TEST(test_pack_all_colors_valid);

    /* Info */
    RUN_TEST(test_info_empty);
    RUN_TEST(test_info_some_active);
    RUN_TEST(test_info_vertex_counts);
    RUN_TEST(test_info_full_chart);

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
    RUN_TEST(test_center_verts_is_6);
    RUN_TEST(test_channel_verts_is_2);
    RUN_TEST(test_max_vertices_is_512);

    return UNITY_END();
}
