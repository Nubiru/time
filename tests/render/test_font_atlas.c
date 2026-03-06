#include "../unity/unity.h"
#include "../../src/render/font_atlas.h"
#include <string.h>

#define EPSILON 1e-4f

/* Standard test atlas: 16x6 grid, ASCII 32-126, 8x16 px cells */
static font_atlas_t atlas;

void setUp(void) {
    atlas = font_atlas_create(16, 6, 32, 126, 8.0f, 16.0f);
}

void tearDown(void) { }

/* --- Atlas creation tests --- */

void test_atlas_create(void) {
    TEST_ASSERT_EQUAL_INT(16, atlas.cols);
    TEST_ASSERT_EQUAL_INT(6, atlas.rows);
    TEST_ASSERT_EQUAL_INT(32, atlas.first_char);
    TEST_ASSERT_EQUAL_INT(126, atlas.last_char);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 8.0f, atlas.cell_width);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 16.0f, atlas.cell_height);
}

void test_atlas_covers_printable_ascii(void) {
    /* 95 printable ASCII chars (32-126) fit in 16*6=96 cells */
    int count = atlas.last_char - atlas.first_char + 1;
    TEST_ASSERT_EQUAL_INT(95, count);
    TEST_ASSERT_TRUE(count <= atlas.cols * atlas.rows);
}

/* --- UV tests --- */

void test_uv_space_returns_zero(void) {
    glyph_uv_t uv = font_glyph_uv(atlas, ' ');
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, uv.v0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, uv.u1);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, uv.v1);
}

void test_uv_char_A(void) {
    /* 'A' = ASCII 65, index = 65-32 = 33, col = 33%16 = 1, row = 33/16 = 2 */
    glyph_uv_t uv = font_glyph_uv(atlas, 'A');
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 1.0f / 16.0f, uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f / 6.0f, uv.v0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f / 16.0f, uv.u1);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f / 6.0f, uv.v1);
}

void test_uv_last_char_tilde(void) {
    glyph_uv_t uv = font_glyph_uv(atlas, '~');
    TEST_ASSERT_TRUE(uv.u0 >= 0.0f && uv.u0 <= 1.0f);
    TEST_ASSERT_TRUE(uv.v0 >= 0.0f && uv.v0 <= 1.0f);
    TEST_ASSERT_TRUE(uv.u1 >= 0.0f && uv.u1 <= 1.0f);
    TEST_ASSERT_TRUE(uv.v1 >= 0.0f && uv.v1 <= 1.0f);
    TEST_ASSERT_TRUE(uv.u1 > uv.u0);
    TEST_ASSERT_TRUE(uv.v1 > uv.v0);
}

void test_uv_out_of_range_returns_zero(void) {
    glyph_uv_t uv = font_glyph_uv(atlas, '\x01');
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, uv.u1);
}

/* --- Layout tests --- */

void test_layout_single_char(void) {
    text_layout_t lay = font_layout_text(atlas, "A", 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[0].x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[0].y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 8.0f, lay.quads[0].w);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 16.0f, lay.quads[0].h);
}

void test_layout_two_chars(void) {
    text_layout_t lay = font_layout_text(atlas, "AB", 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(2, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[0].x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 8.0f, lay.quads[1].x);
}

void test_layout_empty_string(void) {
    text_layout_t lay = font_layout_text(atlas, "", 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.total_width);
}

void test_layout_with_spaces(void) {
    /* "A B" = 3 chars, all in range. Space generates a quad with zero UVs. */
    text_layout_t lay = font_layout_text(atlas, "A B", 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(3, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 24.0f, lay.total_width);
    /* Space quad should have zero UVs */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[1].uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[1].uv.u1);
}

/* --- Measure tests --- */

void test_measure_width_hello(void) {
    float w = font_measure_width(atlas, "Hello", 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 40.0f, w); /* 5 * 8 */
}

void test_measure_width_scaled(void) {
    float w = font_measure_width(atlas, "Hello", 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 80.0f, w); /* 5 * 8 * 2 */
}

/* --- Wrapping tests --- */

void test_wrap_no_wrap_needed(void) {
    text_layout_t lay = font_layout_wrapped(atlas, "Hi", 0.0f, 0.0f, 1.0f, 100.0f, 1.0f);
    /* "Hi" = 16px wide, max_width=100, no wrap */
    TEST_ASSERT_EQUAL_INT(2, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[0].y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[1].y);
}

void test_wrap_forces_new_line(void) {
    /* "ABCD" at 8px each = 32px. max_width=20 -> wraps after 2 chars */
    text_layout_t lay = font_layout_wrapped(atlas, "ABCD", 0.0f, 0.0f, 1.0f, 20.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(4, lay.count);
    /* First two on line 0, next two on line 1 */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[0].y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[1].y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 16.0f, lay.quads[2].y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 16.0f, lay.quads[3].y);
}

void test_layout_newline(void) {
    text_layout_t lay = font_layout_text(atlas, "A\nB", 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(2, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[0].y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 16.0f, lay.quads[1].y);
    /* B resets to start x */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, lay.quads[1].x);
}

/* --- Centered layout tests --- */

void test_centered_single_char(void) {
    /* "A" centered at x=100: quad at 100 - 8/2 = 96 */
    text_layout_t lay = font_layout_centered(atlas, "A", 100.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(1, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 96.0f, lay.quads[0].x);
}

void test_centered_two_chars(void) {
    /* "AB" = 16px wide, centered at 100: starts at 100-8=92 */
    text_layout_t lay = font_layout_centered(atlas, "AB", 100.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(2, lay.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 92.0f, lay.quads[0].x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 100.0f, lay.quads[1].x);
}

/* --- Edge cases --- */

void test_max_256_chars(void) {
    char buf[300];
    memset(buf, 'A', 299);
    buf[299] = '\0';
    text_layout_t lay = font_layout_text(atlas, buf, 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(256, lay.count);
}

void test_null_text(void) {
    text_layout_t lay = font_layout_text(atlas, NULL, 0.0f, 0.0f, 1.0f);
    TEST_ASSERT_EQUAL_INT(0, lay.count);
}

/* --- Measure height --- */

void test_measure_height_single_line(void) {
    float h = font_measure_height(atlas, "Hello", 1.0f, 1000.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 16.0f, h);
}

void test_measure_height_wrapped(void) {
    /* "ABCD" at 8px each, max_width=20 -> 2 lines */
    float h = font_measure_height(atlas, "ABCD", 1.0f, 20.0f, 1.2f);
    /* line 1: 16px + line 2: 16*1.2 = 19.2 -> total = 16 + 19.2 = 35.2 */
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 35.2f, h);
}

/* --- Purity test --- */

void test_purity_same_input_same_output(void) {
    text_layout_t a = font_layout_text(atlas, "Test", 10.0f, 20.0f, 1.5f);
    text_layout_t b = font_layout_text(atlas, "Test", 10.0f, 20.0f, 1.5f);
    TEST_ASSERT_EQUAL_INT(a.count, b.count);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.total_width, b.total_width);
    for (int i = 0; i < a.count; i++) {
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.quads[i].x, b.quads[i].x);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, a.quads[i].uv.u0, b.quads[i].uv.u0);
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Atlas creation */
    RUN_TEST(test_atlas_create);
    RUN_TEST(test_atlas_covers_printable_ascii);

    /* UV */
    RUN_TEST(test_uv_space_returns_zero);
    RUN_TEST(test_uv_char_A);
    RUN_TEST(test_uv_last_char_tilde);
    RUN_TEST(test_uv_out_of_range_returns_zero);

    /* Layout */
    RUN_TEST(test_layout_single_char);
    RUN_TEST(test_layout_two_chars);
    RUN_TEST(test_layout_empty_string);
    RUN_TEST(test_layout_with_spaces);

    /* Measure */
    RUN_TEST(test_measure_width_hello);
    RUN_TEST(test_measure_width_scaled);

    /* Wrapping */
    RUN_TEST(test_wrap_no_wrap_needed);
    RUN_TEST(test_wrap_forces_new_line);
    RUN_TEST(test_layout_newline);

    /* Centered */
    RUN_TEST(test_centered_single_char);
    RUN_TEST(test_centered_two_chars);

    /* Edge cases */
    RUN_TEST(test_max_256_chars);
    RUN_TEST(test_null_text);

    /* Measure height */
    RUN_TEST(test_measure_height_single_line);
    RUN_TEST(test_measure_height_wrapped);

    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);

    return UNITY_END();
}
