/*
 * test_msdf_text.c — Tests for MSDF text layout module
 */

#include "../unity/unity.h"
#include "../../src/render/msdf_text.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Atlas info --- */

void test_mono_atlas_dimensions(void)
{
    TEST_ASSERT_EQUAL_INT(512, msdf_text_atlas_width(MSDF_FONT_MONO));
    TEST_ASSERT_EQUAL_INT(512, msdf_text_atlas_height(MSDF_FONT_MONO));
}

#ifdef MSDF_SANS_ENABLED
void test_sans_atlas_dimensions(void)
{
    TEST_ASSERT_EQUAL_INT(512, msdf_text_atlas_width(MSDF_FONT_SANS));
    TEST_ASSERT_EQUAL_INT(512, msdf_text_atlas_height(MSDF_FONT_SANS));
}
#endif

void test_mono_glyph_count(void)
{
    TEST_ASSERT_EQUAL_INT(190, msdf_text_glyph_count(MSDF_FONT_MONO));
}

#ifdef MSDF_SANS_ENABLED
void test_sans_glyph_count(void)
{
    TEST_ASSERT_EQUAL_INT(191, msdf_text_glyph_count(MSDF_FONT_SANS));
}
#endif

/* --- Empty / NULL input --- */

void test_layout_null_text(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, NULL, 0, 0, 16.0f);
    TEST_ASSERT_EQUAL_INT(0, layout.count);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, layout.total_width);
}

void test_layout_empty_string(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, "", 0, 0, 16.0f);
    TEST_ASSERT_EQUAL_INT(0, layout.count);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, layout.total_width);
}

/* --- Basic layout --- */

void test_layout_single_char(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, "A", 0, 0, 32.0f);
    TEST_ASSERT_EQUAL_INT(1, layout.count);
    TEST_ASSERT_EQUAL_INT('A', layout.quads[0].codepoint);
    /* Quad should have positive width and height */
    TEST_ASSERT_TRUE(layout.quads[0].w > 0.0f);
    TEST_ASSERT_TRUE(layout.quads[0].h > 0.0f);
    /* UVs should be in [0, 1] */
    TEST_ASSERT_TRUE(layout.quads[0].u0 >= 0.0f && layout.quads[0].u0 <= 1.0f);
    TEST_ASSERT_TRUE(layout.quads[0].v0 >= 0.0f && layout.quads[0].v0 <= 1.0f);
    TEST_ASSERT_TRUE(layout.quads[0].u1 >= 0.0f && layout.quads[0].u1 <= 1.0f);
    TEST_ASSERT_TRUE(layout.quads[0].v1 >= 0.0f && layout.quads[0].v1 <= 1.0f);
    /* Total width equals advance */
    TEST_ASSERT_TRUE(layout.total_width > 0.0f);
}

void test_layout_multiple_chars(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, "Hi", 10.0f, 20.0f, 16.0f);
    TEST_ASSERT_EQUAL_INT(2, layout.count);
    TEST_ASSERT_EQUAL_INT('H', layout.quads[0].codepoint);
    TEST_ASSERT_EQUAL_INT('i', layout.quads[1].codepoint);
    /* Second char should be to the right of first */
    TEST_ASSERT_TRUE(layout.quads[1].x > layout.quads[0].x);
}

void test_mono_equal_advance(void)
{
    /* Monospace font: all characters should have equal advance */
    msdf_layout_t l1 = msdf_text_layout(MSDF_FONT_MONO, "iii", 0, 0, 32.0f);
    msdf_layout_t l2 = msdf_text_layout(MSDF_FONT_MONO, "WWW", 0, 0, 32.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, l1.total_width, l2.total_width);
}

#ifdef MSDF_SANS_ENABLED
void test_sans_variable_advance(void)
{
    /* Proportional font: 'i' should be narrower than 'W' */
    float w_i = msdf_text_width(MSDF_FONT_SANS, "iii", 32.0f);
    float w_w = msdf_text_width(MSDF_FONT_SANS, "WWW", 32.0f);
    TEST_ASSERT_TRUE(w_i < w_w);
}
#endif

/* --- Width measurement --- */

void test_width_null(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, msdf_text_width(MSDF_FONT_MONO, NULL, 16.0f));
}

void test_width_scales_with_font_size(void)
{
    float w16 = msdf_text_width(MSDF_FONT_MONO, "Hello", 16.0f);
    float w32 = msdf_text_width(MSDF_FONT_MONO, "Hello", 32.0f);
    /* Double font size should double width */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, w16 * 2.0f, w32);
}

void test_width_matches_layout(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, "Test", 0, 0, 24.0f);
    float width = msdf_text_width(MSDF_FONT_MONO, "Test", 24.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, layout.total_width, width);
}

/* --- Centered layout --- */

void test_centered_single_char(void)
{
    msdf_layout_t layout = msdf_text_centered(MSDF_FONT_MONO, "A", 100.0f, 50.0f, 32.0f);
    TEST_ASSERT_EQUAL_INT(1, layout.count);
    /* The first quad's x should be to the left of center */
    TEST_ASSERT_TRUE(layout.quads[0].x < 100.0f);
}

void test_centered_symmetric(void)
{
    float center = 200.0f;
    msdf_layout_t layout = msdf_text_centered(MSDF_FONT_MONO, "ABCDE", center, 50.0f, 32.0f);
    /* First quad x + total_width/2 should be close to center */
    float first_x = layout.quads[0].x;
    float midpoint = first_x + layout.total_width / 2.0f;
    TEST_ASSERT_FLOAT_WITHIN(5.0f, center, midpoint);
}

/* --- Wrapped layout --- */

void test_wrapped_short_line(void)
{
    /* Text shorter than max_width: should produce single line */
    msdf_layout_t layout = msdf_text_wrapped(MSDF_FONT_MONO, "Hi", 0, 0,
                                             32.0f, 1000.0f, 1.2f);
    TEST_ASSERT_EQUAL_INT(2, layout.count);
    /* Both quads on same baseline (y + h should be close, within glyph variation).
     * Different glyphs have different plane.top so quad y differs, but
     * they share the same baseline (cursor_y). Check bottom edges are similar. */
    float bottom0 = layout.quads[0].y + layout.quads[0].h;
    float bottom1 = layout.quads[1].y + layout.quads[1].h;
    TEST_ASSERT_FLOAT_WITHIN(3.0f, bottom0, bottom1);
}

void test_wrapped_line_break(void)
{
    /* Text that should wrap onto two lines */
    msdf_layout_t layout = msdf_text_wrapped(MSDF_FONT_MONO, "Hello World Test",
                                             10.0f, 20.0f, 16.0f,
                                             60.0f, 1.5f);
    /* Should have produced multiple lines */
    TEST_ASSERT_TRUE(layout.count > 0);
    /* Find if any quad has a different y than the first */
    int has_second_line = 0;
    float first_y = layout.quads[0].y;
    for (int i = 1; i < layout.count; i++) {
        if (fabsf(layout.quads[i].y - first_y) > 1.0f) {
            has_second_line = 1;
            /* Second line should be BELOW first */
            TEST_ASSERT_TRUE(layout.quads[i].y > first_y);
            break;
        }
    }
    TEST_ASSERT_TRUE(has_second_line);
}

void test_wrapped_null_text(void)
{
    msdf_layout_t layout = msdf_text_wrapped(MSDF_FONT_MONO, NULL, 0, 0,
                                             16.0f, 100.0f, 1.2f);
    TEST_ASSERT_EQUAL_INT(0, layout.count);
}

/* --- UV sanity --- */

void test_uv_within_atlas(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, "ABC!@#123", 0, 0, 16.0f);
    for (int i = 0; i < layout.count; i++) {
        TEST_ASSERT_TRUE(layout.quads[i].u0 >= 0.0f);
        TEST_ASSERT_TRUE(layout.quads[i].u1 <= 1.0f);
        TEST_ASSERT_TRUE(layout.quads[i].v0 >= 0.0f);
        TEST_ASSERT_TRUE(layout.quads[i].v1 <= 1.0f);
        /* u1 > u0 and v1 > v0 (non-degenerate except space) */
        if (layout.quads[i].codepoint != ' ') {
            TEST_ASSERT_TRUE(layout.quads[i].u1 > layout.quads[i].u0);
            TEST_ASSERT_TRUE(layout.quads[i].v1 > layout.quads[i].v0);
        }
    }
}

/* --- Space character --- */

void test_space_produces_no_quad(void)
{
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, "A B", 0, 0, 16.0f);
    /* Space has zero-area glyph, skip it in layout */
    TEST_ASSERT_EQUAL_INT(2, layout.count);
    TEST_ASSERT_EQUAL_INT('A', layout.quads[0].codepoint);
    TEST_ASSERT_EQUAL_INT('B', layout.quads[1].codepoint);
    /* B should be further right than A (space advance between them) */
    TEST_ASSERT_TRUE(layout.quads[1].x > layout.quads[0].x + layout.quads[0].w);
}

/* --- Max quads limit --- */

void test_max_quads_limit(void)
{
    /* Build a string longer than MSDF_TEXT_MAX_QUADS */
    char long_str[300];
    memset(long_str, 'X', 299);
    long_str[299] = '\0';
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, long_str, 0, 0, 16.0f);
    TEST_ASSERT_TRUE(layout.count <= MSDF_TEXT_MAX_QUADS);
}

/* --- Line height --- */

void test_line_height_scales(void)
{
    msdf_layout_t l16 = msdf_text_layout(MSDF_FONT_MONO, "A", 0, 0, 16.0f);
    msdf_layout_t l32 = msdf_text_layout(MSDF_FONT_MONO, "A", 0, 0, 32.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, l16.line_height * 2.0f, l32.line_height);
}

/* --- Position offset --- */

void test_position_offset(void)
{
    msdf_layout_t l0 = msdf_text_layout(MSDF_FONT_MONO, "A", 0, 0, 16.0f);
    msdf_layout_t l1 = msdf_text_layout(MSDF_FONT_MONO, "A", 100.0f, 200.0f, 16.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, l0.quads[0].x + 100.0f, l1.quads[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, l0.quads[0].y + 200.0f, l1.quads[0].y);
}

/* --- Unknown glyph fallback --- */

void test_unknown_glyph_skipped(void)
{
    /* Codepoint 0x0400 (Cyrillic) is not in the Latin-1 atlas */
    char text[] = { 'A', (char)0xD0, (char)0x80, 'B', '\0' };  /* "A" + U+0400 + "B" */
    msdf_layout_t layout = msdf_text_layout(MSDF_FONT_MONO, text, 0, 0, 16.0f);
    /* Should have A and B, skipping the unknown glyph */
    int has_a = 0, has_b = 0;
    for (int i = 0; i < layout.count; i++) {
        if (layout.quads[i].codepoint == 'A') has_a = 1;
        if (layout.quads[i].codepoint == 'B') has_b = 1;
    }
    TEST_ASSERT_TRUE(has_a);
    TEST_ASSERT_TRUE(has_b);
}

int main(void)
{
    UNITY_BEGIN();

    /* Atlas info */
    RUN_TEST(test_mono_atlas_dimensions);
#ifdef MSDF_SANS_ENABLED
    RUN_TEST(test_sans_atlas_dimensions);
#endif
    RUN_TEST(test_mono_glyph_count);
#ifdef MSDF_SANS_ENABLED
    RUN_TEST(test_sans_glyph_count);
#endif

    /* Empty / NULL */
    RUN_TEST(test_layout_null_text);
    RUN_TEST(test_layout_empty_string);

    /* Basic layout */
    RUN_TEST(test_layout_single_char);
    RUN_TEST(test_layout_multiple_chars);
    RUN_TEST(test_mono_equal_advance);
#ifdef MSDF_SANS_ENABLED
    RUN_TEST(test_sans_variable_advance);
#endif

    /* Width */
    RUN_TEST(test_width_null);
    RUN_TEST(test_width_scales_with_font_size);
    RUN_TEST(test_width_matches_layout);

    /* Centered */
    RUN_TEST(test_centered_single_char);
    RUN_TEST(test_centered_symmetric);

    /* Wrapped */
    RUN_TEST(test_wrapped_short_line);
    RUN_TEST(test_wrapped_line_break);
    RUN_TEST(test_wrapped_null_text);

    /* UV sanity */
    RUN_TEST(test_uv_within_atlas);

    /* Space */
    RUN_TEST(test_space_produces_no_quad);

    /* Limits */
    RUN_TEST(test_max_quads_limit);

    /* Line height */
    RUN_TEST(test_line_height_scales);

    /* Position */
    RUN_TEST(test_position_offset);

    /* Unknown glyph */
    RUN_TEST(test_unknown_glyph_skipped);

    return UNITY_END();
}
