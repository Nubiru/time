#include "../unity/unity.h"
#include "../../src/render/hexagram_geometry.h"
#include "../../src/systems/iching/iching.h"
#include "../../src/render/color_theory.h"
#include "../../src/math/color.h"

#include <math.h>
#include <string.h>

#define TOL 0.001f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* --- Helper: count yang/yin lines for a hexagram --- */
static int count_yang(int king_wen)
{
    hexagram_t h = iching_hexagram(king_wen);
    int n = 0;
    for (int i = 0; i < 6; i++) {
        if (h.lines[i] == 1) n++;
    }
    return n;
}

static int count_yin(int king_wen)
{
    return 6 - count_yang(king_wen);
}

/* Expected vertex count: yang lines produce 2 verts, yin lines produce 4 verts */
static int expected_vertex_count(int king_wen)
{
    int ny = count_yang(king_wen);
    int ni = count_yin(king_wen);
    return ny * 2 + ni * 4;
}

/* Expected line (segment) count: yang=1 segment, yin=2 segments */
static int expected_line_count(int king_wen)
{
    int ny = count_yang(king_wen);
    int ni = count_yin(king_wen);
    return ny * 1 + ni * 2;
}

/* ---- 1. All-yang hexagram (1, Qian): 6 solid lines ---- */
void test_pack_all_yang(void)
{
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, NULL);
    /* 6 yang lines: each 2 verts -> 12 total */
    TEST_ASSERT_EQUAL_INT(12, d.vertex_count);
    /* 6 yang lines: each 1 segment -> 6 segments */
    TEST_ASSERT_EQUAL_INT(6, d.line_count);
}

/* ---- 2. All-yin hexagram (2, Kun): 6 broken lines ---- */
void test_pack_all_yin(void)
{
    hg_data_t d = hg_pack(2, 1.0f, 1.0f, NULL);
    /* 6 yin lines: each 4 verts -> 24 total */
    TEST_ASSERT_EQUAL_INT(24, d.vertex_count);
    /* 6 yin lines: each 2 segments -> 12 segments */
    TEST_ASSERT_EQUAL_INT(12, d.line_count);
}

/* ---- 3. Mixed hexagram (3, Zhun): Zhen(1,0,0) + Kan(0,1,0) ---- */
void test_pack_mixed_zhun(void)
{
    hg_data_t d = hg_pack(3, 1.0f, 1.0f, NULL);
    /* lines: {1,0,0,0,1,0} -> 2 yang, 4 yin */
    int exp_v = 2 * 2 + 4 * 4; /* 4 + 16 = 20 */
    TEST_ASSERT_EQUAL_INT(exp_v, d.vertex_count);
    int exp_l = 2 * 1 + 4 * 2; /* 2 + 8 = 10 */
    TEST_ASSERT_EQUAL_INT(exp_l, d.line_count);
}

/* ---- 4. Yang line produces 2 vertices ---- */
void test_yang_line_two_vertices(void)
{
    /* Hexagram 1: all yang. 6 lines, each 2 verts = 12 */
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(12, d.vertex_count);
}

/* ---- 5. Yin line produces 4 vertices ---- */
void test_yin_line_four_vertices(void)
{
    /* Hexagram 2: all yin. 6 lines, each 4 verts = 24 */
    hg_data_t d = hg_pack(2, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(24, d.vertex_count);
}

/* ---- 6. Vertex count correctness for hexagram 29 (Kan/Kan) ---- */
void test_vertex_count_hex29(void)
{
    /* Kan = {0,1,0}, so full lines = {0,1,0,0,1,0}: 2 yang, 4 yin */
    hg_data_t d = hg_pack(29, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(expected_vertex_count(29), d.vertex_count);
}

/* ---- 7. Vertex count correctness for hexagram 30 (Li/Li) ---- */
void test_vertex_count_hex30(void)
{
    /* Li = {1,0,1}, so full lines = {1,0,1,1,0,1}: 4 yang, 2 yin */
    hg_data_t d = hg_pack(30, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(expected_vertex_count(30), d.vertex_count);
}

/* ---- 8. Line count correctness for hexagram 63 (Ji Ji) ---- */
void test_line_count_hex63(void)
{
    /* Li/Kan: {1,0,1,0,1,0}: 3 yang, 3 yin -> 3+6=9 segments */
    hg_data_t d = hg_pack(63, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(expected_line_count(63), d.line_count);
}

/* ---- 9. Changing line color differs from normal ---- */
void test_changing_line_color_differs(void)
{
    /* Pack hex 1 without changing */
    hg_data_t d_normal = hg_pack(1, 1.0f, 1.0f, NULL);

    /* Pack hex 1 with line 0 changing */
    hg_changing_t ch = {{0}};
    ch.changing[0] = 1;
    hg_data_t d_changing = hg_pack(1, 1.0f, 1.0f, &ch);

    /* Line 0 vertex colors: offset into vertices for first line.
     * Each vertex = 6 floats (x,y,r,g,b,a). First vertex color at index 2. */
    float nr = d_normal.vertices[2];
    float ng = d_normal.vertices[3];
    float nb = d_normal.vertices[4];
    float cr = d_changing.vertices[2];
    float cg = d_changing.vertices[3];
    float cb = d_changing.vertices[4];

    /* At least one channel must differ */
    int differs = (fabsf(nr - cr) > TOL) ||
                  (fabsf(ng - cg) > TOL) ||
                  (fabsf(nb - cb) > TOL);
    TEST_ASSERT_TRUE(differs);
}

/* ---- 10. No changing lines (NULL parameter) works ---- */
void test_no_changing_null(void)
{
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(12, d.vertex_count);
}

/* ---- 11. All 6 lines changing ---- */
void test_all_lines_changing(void)
{
    hg_changing_t ch = {{1, 1, 1, 1, 1, 1}};
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, &ch);
    /* Still same vertex count — changing only affects color */
    TEST_ASSERT_EQUAL_INT(12, d.vertex_count);

    /* Verify all lines have accent color */
    color_rgb_t accent = ct_system_accent(CT_SYSTEM_ICHING);
    for (int v = 0; v < d.vertex_count; v++) {
        int base = v * HG_VERTEX_FLOATS;
        TEST_ASSERT_TRUE(NEAR(accent.r, d.vertices[base + 2]));
        TEST_ASSERT_TRUE(NEAR(accent.g, d.vertices[base + 3]));
        TEST_ASSERT_TRUE(NEAR(accent.b, d.vertices[base + 4]));
    }
}

/* ---- 12. Single changing line (line 3) ---- */
void test_single_changing_line(void)
{
    hg_changing_t ch = {{0}};
    ch.changing[3] = 1;
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, &ch);

    color_rgb_t primary = ct_system_primary(CT_SYSTEM_ICHING);
    color_rgb_t accent = ct_system_accent(CT_SYSTEM_ICHING);

    /* Line 3 is the 4th line from bottom. For all-yang, each line = 2 verts.
     * Line 3 starts at vertex 6 (2 verts/line * 3 lines before). */
    int line3_start = 6;
    int base = line3_start * HG_VERTEX_FLOATS;
    TEST_ASSERT_TRUE(NEAR(accent.r, d.vertices[base + 2]));
    TEST_ASSERT_TRUE(NEAR(accent.g, d.vertices[base + 3]));
    TEST_ASSERT_TRUE(NEAR(accent.b, d.vertices[base + 4]));

    /* Line 0 should still be primary color */
    TEST_ASSERT_TRUE(NEAR(primary.r, d.vertices[2]));
    TEST_ASSERT_TRUE(NEAR(primary.g, d.vertices[3]));
    TEST_ASSERT_TRUE(NEAR(primary.b, d.vertices[4]));
}

/* ---- 13. hg_pack_with_nuclear produces main + nuclear vertices ---- */
void test_pack_with_nuclear_counts(void)
{
    hg_data_t d = hg_pack_with_nuclear(1, 1.0f, 1.0f, NULL, 1.5f, 0.5f);
    TEST_ASSERT_TRUE(d.main_vertex_count > 0);
    TEST_ASSERT_TRUE(d.nuclear_vertex_count > 0);
    TEST_ASSERT_EQUAL_INT(d.main_vertex_count + d.nuclear_vertex_count,
                          d.vertex_count);
}

/* ---- 14. Nuclear hexagram derivation: hex 1 (all yang) -> nuclear = 1 ---- */
void test_nuclear_hex1(void)
{
    TEST_ASSERT_EQUAL_INT(1, hg_nuclear_lower(1));
    TEST_ASSERT_EQUAL_INT(1, hg_nuclear_upper(1));
}

/* ---- 15. Nuclear hexagram derivation: hex 2 (all yin) -> nuclear = 2 ---- */
void test_nuclear_hex2(void)
{
    TEST_ASSERT_EQUAL_INT(2, hg_nuclear_lower(2));
    TEST_ASSERT_EQUAL_INT(2, hg_nuclear_upper(2));
}

/* ---- 16. Nuclear of hex 3 (Zhun): lines {1,0,0,0,1,0} ---- */
void test_nuclear_hex3(void)
{
    /* Nuclear lower: lines[1,2,3]={0,0,0} lower, lines[2,3,4]={0,0,1} upper
     * = Kun/Gen = hex 23 (Bo) */
    int nuc = hg_nuclear_lower(3);
    TEST_ASSERT_EQUAL_INT(23, nuc);
}

/* ---- 17. Nuclear upper of hex 3 ---- */
void test_nuclear_upper_hex3(void)
{
    /* Nuclear upper: lines[2,3,4]={0,0,1} lower, lines[3,4,5]={0,1,0} upper
     * = Gen/Kan = hex 39 (Jian) */
    int nuc = hg_nuclear_upper(3);
    TEST_ASSERT_EQUAL_INT(39, nuc);
}

/* ---- 18. Nuclear of hex 63 (Ji Ji): {1,0,1,0,1,0} ---- */
void test_nuclear_hex63(void)
{
    /* Nuclear lower: lines[1,2,3]={0,1,0} lower, lines[2,3,4]={1,0,1} upper
     * = Kan/Li = hex 64 (Wei Ji) */
    TEST_ASSERT_EQUAL_INT(64, hg_nuclear_lower(63));
}

/* ---- 19. Nuclear upper of hex 63 ---- */
void test_nuclear_upper_hex63(void)
{
    /* Nuclear upper: lines[2,3,4]={1,0,1} lower, lines[3,4,5]={0,1,0} upper
     * = Li/Kan = hex 63 (Ji Ji) */
    TEST_ASSERT_EQUAL_INT(63, hg_nuclear_upper(63));
}

/* ---- 20. Trigram pack produces correct vertex count with gap ---- */
void test_pack_trigrams_vertex_count(void)
{
    hg_data_t d = hg_pack_trigrams(1, 1.0f, 1.0f, 0.15f);
    /* Same line types as regular hex — gap only affects Y positions */
    TEST_ASSERT_EQUAL_INT(expected_vertex_count(1), d.vertex_count);
}

/* ---- 21. Trigram gap is reflected in Y positions ---- */
void test_trigram_gap_y_positions(void)
{
    hg_data_t d_nogap = hg_pack(1, 1.0f, 1.0f, NULL);
    hg_data_t d_gap = hg_pack_trigrams(1, 1.0f, 1.0f, 0.15f);

    /* Upper trigram lines should be shifted up relative to no-gap version */
    /* Line 3 (first of upper trigram), first vertex Y.
     * For all-yang hex: line 3 starts at vertex 6, Y at index 6*6+1 */
    float y_nogap = d_nogap.vertices[6 * HG_VERTEX_FLOATS + 1];
    float y_gap = d_gap.vertices[6 * HG_VERTEX_FLOATS + 1];
    /* The gap pushes upper lines higher */
    TEST_ASSERT_TRUE(y_gap > y_nogap);
}

/* ---- 22. hg_line_count returns 6 for valid ---- */
void test_line_count_valid(void)
{
    TEST_ASSERT_EQUAL_INT(6, hg_line_count(1));
    TEST_ASSERT_EQUAL_INT(6, hg_line_count(32));
    TEST_ASSERT_EQUAL_INT(6, hg_line_count(64));
}

/* ---- 23. hg_line_count returns 0 for invalid ---- */
void test_line_count_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, hg_line_count(0));
    TEST_ASSERT_EQUAL_INT(0, hg_line_count(65));
    TEST_ASSERT_EQUAL_INT(0, hg_line_count(-1));
}

/* ---- 24. hg_vertex_bytes calculation ---- */
void test_vertex_bytes(void)
{
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, NULL);
    int expected = d.vertex_count * HG_VERTEX_FLOATS * (int)sizeof(float);
    TEST_ASSERT_EQUAL_INT(expected, hg_vertex_bytes(&d));
}

/* ---- 25. Invalid input: king_wen 0 ---- */
void test_invalid_zero(void)
{
    hg_data_t d = hg_pack(0, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(0, d.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, d.line_count);
}

/* ---- 26. Invalid input: king_wen 65 ---- */
void test_invalid_65(void)
{
    hg_data_t d = hg_pack(65, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(0, d.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, d.line_count);
}

/* ---- 27. Invalid input: king_wen -1 ---- */
void test_invalid_negative(void)
{
    hg_data_t d = hg_pack(-1, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(0, d.vertex_count);
    TEST_ASSERT_EQUAL_INT(0, d.line_count);
}

/* ---- 28. Shader vertex source non-null and contains GLSL keywords ---- */
void test_shader_vert_source(void)
{
    const char *src = hg_line_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "gl_Position"));
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(src, "a_position"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* ---- 29. Shader fragment source non-null and contains GLSL keywords ---- */
void test_shader_frag_source(void)
{
    const char *src = hg_line_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(src, "out_color"));
}

/* ---- 30. Positions are within bounding box ---- */
void test_positions_within_bounds(void)
{
    float w = 2.0f, h = 3.0f;
    hg_data_t d = hg_pack(3, w, h, NULL);
    for (int v = 0; v < d.vertex_count; v++) {
        int base = v * HG_VERTEX_FLOATS;
        float x = d.vertices[base + 0];
        float y = d.vertices[base + 1];
        TEST_ASSERT_TRUE(x >= 0.0f - TOL);
        TEST_ASSERT_TRUE(x <= w + TOL);
        TEST_ASSERT_TRUE(y >= 0.0f - TOL);
        TEST_ASSERT_TRUE(y <= h + TOL);
    }
}

/* ---- 31. Y positions increase bottom to top ---- */
void test_y_positions_bottom_to_top(void)
{
    /* Hex 1 (all yang): 6 lines, 2 verts each.
     * Line i has vertices at index i*2 and i*2+1, Y at offset 1. */
    hg_data_t d = hg_pack(1, 1.0f, 2.0f, NULL);
    for (int line = 0; line < 5; line++) {
        int v_this = line * 2;
        int v_next = (line + 1) * 2;
        float y_this = d.vertices[v_this * HG_VERTEX_FLOATS + 1];
        float y_next = d.vertices[v_next * HG_VERTEX_FLOATS + 1];
        TEST_ASSERT_TRUE(y_next > y_this);
    }
}

/* ---- 32. All 64 hexagrams produce valid data ---- */
void test_all_64_valid(void)
{
    for (int kw = 1; kw <= 64; kw++) {
        hg_data_t d = hg_pack(kw, 1.0f, 1.0f, NULL);
        TEST_ASSERT_TRUE(d.vertex_count > 0);
        TEST_ASSERT_TRUE(d.line_count > 0);
        TEST_ASSERT_EQUAL_INT(expected_vertex_count(kw), d.vertex_count);
        TEST_ASSERT_EQUAL_INT(expected_line_count(kw), d.line_count);
    }
}

/* ---- 33. Nuclear derivation returns 0 for invalid ---- */
void test_nuclear_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, hg_nuclear_lower(0));
    TEST_ASSERT_EQUAL_INT(0, hg_nuclear_upper(0));
    TEST_ASSERT_EQUAL_INT(0, hg_nuclear_lower(65));
    TEST_ASSERT_EQUAL_INT(0, hg_nuclear_upper(-1));
}

/* ---- 34. Yin gap center is correct ---- */
void test_yin_gap_in_center(void)
{
    /* Hex 2 (all yin): width=6.0.
     * Yin gap = 1/6 of width = 1.0, centered.
     * Left segment: 0.0 to 2.5, Right segment: 3.5 to 6.0
     * Line 0, vert 0: x=0.0, vert 1: x=2.5
     * Line 0, vert 2: x=3.5, vert 3: x=6.0 */
    hg_data_t d = hg_pack(2, 6.0f, 1.0f, NULL);
    float x0 = d.vertices[0 * HG_VERTEX_FLOATS + 0]; /* left start */
    float x1 = d.vertices[1 * HG_VERTEX_FLOATS + 0]; /* left end */
    float x2 = d.vertices[2 * HG_VERTEX_FLOATS + 0]; /* right start */
    float x3 = d.vertices[3 * HG_VERTEX_FLOATS + 0]; /* right end */

    TEST_ASSERT_TRUE(NEAR(0.0f, x0));
    TEST_ASSERT_TRUE(NEAR(6.0f, x3));
    /* Gap should be centered: left_end + gap = right_start */
    float gap = x2 - x1;
    TEST_ASSERT_TRUE(NEAR(1.0f, gap));  /* 6.0 / 6.0 = 1.0 */
    /* Symmetry: left_end == width/2 - gap/2 */
    TEST_ASSERT_TRUE(NEAR(2.5f, x1));
    TEST_ASSERT_TRUE(NEAR(3.5f, x2));
}

/* ---- 35. Normal line colors use ct_system_primary ---- */
void test_normal_line_uses_primary_color(void)
{
    color_rgb_t primary = ct_system_primary(CT_SYSTEM_ICHING);
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, NULL);

    /* First vertex color */
    TEST_ASSERT_TRUE(NEAR(primary.r, d.vertices[2]));
    TEST_ASSERT_TRUE(NEAR(primary.g, d.vertices[3]));
    TEST_ASSERT_TRUE(NEAR(primary.b, d.vertices[4]));
}

/* ---- 36. Nuclear hexagram uses secondary color at reduced alpha ---- */
void test_nuclear_uses_secondary_color(void)
{
    hg_data_t d = hg_pack_with_nuclear(1, 1.0f, 1.0f, NULL, 1.5f, 0.5f);
    color_rgb_t secondary = ct_system_secondary(CT_SYSTEM_ICHING);

    /* Nuclear vertices start after main vertices */
    int nuc_start = d.main_vertex_count;
    int base = nuc_start * HG_VERTEX_FLOATS;
    TEST_ASSERT_TRUE(NEAR(secondary.r, d.vertices[base + 2]));
    TEST_ASSERT_TRUE(NEAR(secondary.g, d.vertices[base + 3]));
    TEST_ASSERT_TRUE(NEAR(secondary.b, d.vertices[base + 4]));
    /* Alpha should be reduced (less than 1.0) */
    TEST_ASSERT_TRUE(d.vertices[base + 5] < 1.0f);
}

/* ---- 37. Main vertex count field is correct ---- */
void test_main_vertex_count_field(void)
{
    hg_data_t d = hg_pack(30, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(d.vertex_count, d.main_vertex_count);
    TEST_ASSERT_EQUAL_INT(0, d.nuclear_vertex_count);
}

/* ---- 38. Nuclear pack has correct nuclear vertex count ---- */
void test_nuclear_vertex_count_field(void)
{
    hg_data_t d = hg_pack_with_nuclear(30, 1.0f, 1.0f, NULL, 2.0f, 0.5f);
    /* Nuclear hexagram also has lines — the vertex counts should add up */
    TEST_ASSERT_TRUE(d.nuclear_vertex_count > 0);
    TEST_ASSERT_EQUAL_INT(d.main_vertex_count + d.nuclear_vertex_count,
                          d.vertex_count);
}

/* ---- 39. Different bounding box sizes produce different positions ---- */
void test_different_sizes(void)
{
    hg_data_t d1 = hg_pack(1, 1.0f, 1.0f, NULL);
    hg_data_t d2 = hg_pack(1, 2.0f, 3.0f, NULL);
    /* Vertex positions should differ */
    int differs = 0;
    for (int i = 0; i < d1.vertex_count * HG_VERTEX_FLOATS; i++) {
        if (fabsf(d1.vertices[i] - d2.vertices[i]) > TOL) {
            differs = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differs);
}

/* ---- 40. Alpha is 1.0 for normal main lines ---- */
void test_normal_alpha_one(void)
{
    hg_data_t d = hg_pack(1, 1.0f, 1.0f, NULL);
    for (int v = 0; v < d.vertex_count; v++) {
        int base = v * HG_VERTEX_FLOATS;
        TEST_ASSERT_TRUE(NEAR(1.0f, d.vertices[base + 5]));
    }
}

/* ---- 41. Trigram pack for yin hexagram ---- */
void test_trigram_pack_yin(void)
{
    hg_data_t d = hg_pack_trigrams(2, 1.0f, 1.0f, 0.2f);
    TEST_ASSERT_EQUAL_INT(expected_vertex_count(2), d.vertex_count);
}

/* ---- 42. hg_vertex_bytes zero for empty data ---- */
void test_vertex_bytes_empty(void)
{
    hg_data_t d = hg_pack(0, 1.0f, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(0, hg_vertex_bytes(&d));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pack_all_yang);
    RUN_TEST(test_pack_all_yin);
    RUN_TEST(test_pack_mixed_zhun);
    RUN_TEST(test_yang_line_two_vertices);
    RUN_TEST(test_yin_line_four_vertices);
    RUN_TEST(test_vertex_count_hex29);
    RUN_TEST(test_vertex_count_hex30);
    RUN_TEST(test_line_count_hex63);
    RUN_TEST(test_changing_line_color_differs);
    RUN_TEST(test_no_changing_null);
    RUN_TEST(test_all_lines_changing);
    RUN_TEST(test_single_changing_line);
    RUN_TEST(test_pack_with_nuclear_counts);
    RUN_TEST(test_nuclear_hex1);
    RUN_TEST(test_nuclear_hex2);
    RUN_TEST(test_nuclear_hex3);
    RUN_TEST(test_nuclear_upper_hex3);
    RUN_TEST(test_nuclear_hex63);
    RUN_TEST(test_nuclear_upper_hex63);
    RUN_TEST(test_pack_trigrams_vertex_count);
    RUN_TEST(test_trigram_gap_y_positions);
    RUN_TEST(test_line_count_valid);
    RUN_TEST(test_line_count_invalid);
    RUN_TEST(test_vertex_bytes);
    RUN_TEST(test_invalid_zero);
    RUN_TEST(test_invalid_65);
    RUN_TEST(test_invalid_negative);
    RUN_TEST(test_shader_vert_source);
    RUN_TEST(test_shader_frag_source);
    RUN_TEST(test_positions_within_bounds);
    RUN_TEST(test_y_positions_bottom_to_top);
    RUN_TEST(test_all_64_valid);
    RUN_TEST(test_nuclear_invalid);
    RUN_TEST(test_yin_gap_in_center);
    RUN_TEST(test_normal_line_uses_primary_color);
    RUN_TEST(test_nuclear_uses_secondary_color);
    RUN_TEST(test_main_vertex_count_field);
    RUN_TEST(test_nuclear_vertex_count_field);
    RUN_TEST(test_different_sizes);
    RUN_TEST(test_normal_alpha_one);
    RUN_TEST(test_trigram_pack_yin);
    RUN_TEST(test_vertex_bytes_empty);
    return UNITY_END();
}
