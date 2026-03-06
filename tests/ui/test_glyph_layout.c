#include "../unity/unity.h"
#include "../../src/ui/glyph_layout.h"

#define TOL 0.001f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

static glyph_style_t make_style(glyph_align_t align)
{
    glyph_style_t s;
    s.x = 100.0f;
    s.y = 50.0f;
    s.char_width = 10.0f;
    s.char_height = 20.0f;
    s.line_spacing = 5.0f;
    s.padding = 4.0f;
    s.align = align;
    return s;
}

/* 1. Text rect: single char dimensions */
void test_text_rect_1_char(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_text_rect(s, 1);
    TEST_ASSERT_TRUE(NEAR(18.0f, r.width));   /* 2*4 + 1*10 */
    TEST_ASSERT_TRUE(NEAR(28.0f, r.height));  /* 2*4 + 20 */
}

/* 2. Text rect: 10 chars */
void test_text_rect_10_chars(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_text_rect(s, 10);
    TEST_ASSERT_TRUE(NEAR(108.0f, r.width));  /* 2*4 + 10*10 */
}

/* 3. Text rect includes padding */
void test_text_rect_padding(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_text_rect(s, 5);
    float inner_w = r.width - 2.0f * s.padding;
    TEST_ASSERT_TRUE(NEAR(50.0f, inner_w));  /* 5*10 */
    float inner_h = r.height - 2.0f * s.padding;
    TEST_ASSERT_TRUE(NEAR(20.0f, inner_h));  /* char_height */
}

/* 4. Block rect: single line = same height as text rect */
void test_block_rect_1_line(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t t = glyph_text_rect(s, 5);
    glyph_rect_t b = glyph_block_rect(s, 1, 5);
    TEST_ASSERT_TRUE(NEAR(t.width, b.width));
    TEST_ASSERT_TRUE(NEAR(t.height, b.height));
}

/* 5. Block rect: 3 lines */
void test_block_rect_3_lines(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_block_rect(s, 3, 5);
    /* height = 2*4 + 3*20 + 2*5 = 78 */
    TEST_ASSERT_TRUE(NEAR(78.0f, r.height));
    TEST_ASSERT_TRUE(NEAR(58.0f, r.width));  /* 2*4 + 5*10 */
}

/* 6. char_x: ALIGN_LEFT, index 0 */
void test_char_x_left_first(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    float x = glyph_char_x(s, 0, 5);
    TEST_ASSERT_TRUE(NEAR(104.0f, x));  /* 100 + 4 */
}

/* 7. char_x: ALIGN_LEFT, index 3 */
void test_char_x_left_mid(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    float x = glyph_char_x(s, 3, 5);
    TEST_ASSERT_TRUE(NEAR(134.0f, x));  /* 100 + 4 + 3*10 */
}

/* 8. char_x: ALIGN_CENTER, index 0 */
void test_char_x_center_first(void)
{
    glyph_style_t s = make_style(ALIGN_CENTER);
    float x = glyph_char_x(s, 0, 5);
    /* 100 - 5*10/2 = 75 */
    TEST_ASSERT_TRUE(NEAR(75.0f, x));
}

/* 9. char_x: ALIGN_CENTER, text block centered around origin */
void test_char_x_center_symmetric(void)
{
    glyph_style_t s = make_style(ALIGN_CENTER);
    float first = glyph_char_x(s, 0, 4);      /* left edge of first char */
    float last_end = glyph_char_x(s, 3, 4) + s.char_width; /* right edge of last */
    float center = (first + last_end) / 2.0f;
    TEST_ASSERT_TRUE(NEAR(s.x, center));
}

/* 10. char_x: ALIGN_RIGHT, last char ends near origin - padding */
void test_char_x_right_last(void)
{
    glyph_style_t s = make_style(ALIGN_RIGHT);
    float x = glyph_char_x(s, 4, 5);
    float right_edge = x + s.char_width;
    TEST_ASSERT_TRUE(NEAR(s.x - s.padding, right_edge));
}

/* 11. line_y: first line */
void test_line_y_first(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    float y = glyph_line_y(s, 0);
    TEST_ASSERT_TRUE(NEAR(54.0f, y));  /* 50 + 4 */
}

/* 12. line_y: second line */
void test_line_y_second(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    float y = glyph_line_y(s, 1);
    TEST_ASSERT_TRUE(NEAR(79.0f, y));  /* 50 + 4 + 1*(20+5) */
}

/* 13. label_above: centered horizontally */
void test_label_above_centered(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_label_above(200.0f, 300.0f, 10.0f, s, 4);
    float center = r.x + r.width / 2.0f;
    TEST_ASSERT_TRUE(NEAR(200.0f, center));
}

/* 14. label_below: centered horizontally */
void test_label_below_centered(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_label_below(200.0f, 300.0f, 10.0f, s, 4);
    float center = r.x + r.width / 2.0f;
    TEST_ASSERT_TRUE(NEAR(200.0f, center));
}

/* 15. label_above: bottom edge at py - offset_y */
void test_label_above_y(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_label_above(100.0f, 200.0f, 5.0f, s, 3);
    float bottom = r.y + r.height;
    TEST_ASSERT_TRUE(NEAR(195.0f, bottom));  /* 200 - 5 */
}

/* 16. label_below: top edge at py + offset_y */
void test_label_below_y(void)
{
    glyph_style_t s = make_style(ALIGN_LEFT);
    glyph_rect_t r = glyph_label_below(100.0f, 200.0f, 5.0f, s, 3);
    TEST_ASSERT_TRUE(NEAR(205.0f, r.y));  /* 200 + 5 */
}

/* 17. Overlap: yes */
void test_overlap_yes(void)
{
    glyph_rect_t a = {10, 10, 50, 50};
    glyph_rect_t b = {30, 30, 50, 50};
    TEST_ASSERT_TRUE(glyph_rects_overlap(a, b));
}

/* 18. Overlap: no */
void test_overlap_no(void)
{
    glyph_rect_t a = {10, 10, 20, 20};
    glyph_rect_t b = {100, 100, 20, 20};
    TEST_ASSERT_FALSE(glyph_rects_overlap(a, b));
}

/* 19. Overlap: touching edges = no overlap */
void test_overlap_touching(void)
{
    glyph_rect_t a = {0, 0, 50, 50};
    glyph_rect_t b = {50, 0, 50, 50};
    TEST_ASSERT_FALSE(glyph_rects_overlap(a, b));
}

/* 20. Clamp: pushed from left */
void test_clamp_left(void)
{
    glyph_rect_t r = {-10, 20, 30, 30};
    glyph_rect_t c = glyph_clamp_to_screen(r, 800, 600);
    TEST_ASSERT_TRUE(NEAR(0.0f, c.x));
    TEST_ASSERT_TRUE(NEAR(20.0f, c.y));
}

/* 21. Clamp: pushed from bottom-right */
void test_clamp_bottom_right(void)
{
    glyph_rect_t r = {790, 580, 30, 30};
    glyph_rect_t c = glyph_clamp_to_screen(r, 800, 600);
    TEST_ASSERT_TRUE(NEAR(770.0f, c.x));
    TEST_ASSERT_TRUE(NEAR(570.0f, c.y));
}

/* 22. Clamp: no change when within bounds */
void test_clamp_no_change(void)
{
    glyph_rect_t r = {100, 100, 50, 50};
    glyph_rect_t c = glyph_clamp_to_screen(r, 800, 600);
    TEST_ASSERT_TRUE(r.x == c.x && r.y == c.y);
}

/* 23. HUD style non-zero */
void test_style_hud(void)
{
    glyph_style_t s = glyph_style_hud();
    TEST_ASSERT_TRUE(s.char_width > 0.0f);
    TEST_ASSERT_TRUE(s.char_height > 0.0f);
    TEST_ASSERT_TRUE(s.padding > 0.0f);
}

/* 24. Card style non-zero */
void test_style_card(void)
{
    glyph_style_t s = glyph_style_card();
    TEST_ASSERT_TRUE(s.char_width > 0.0f);
    TEST_ASSERT_TRUE(s.char_height > 0.0f);
    TEST_ASSERT_TRUE(s.padding > 0.0f);
}

/* 25. Purity */
void test_purity(void)
{
    glyph_style_t s = make_style(ALIGN_CENTER);
    glyph_rect_t a = glyph_text_rect(s, 8);
    glyph_rect_t b = glyph_text_rect(s, 8);
    TEST_ASSERT_TRUE(a.x == b.x);
    TEST_ASSERT_TRUE(a.y == b.y);
    TEST_ASSERT_TRUE(a.width == b.width);
    TEST_ASSERT_TRUE(a.height == b.height);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_text_rect_1_char);
    RUN_TEST(test_text_rect_10_chars);
    RUN_TEST(test_text_rect_padding);
    RUN_TEST(test_block_rect_1_line);
    RUN_TEST(test_block_rect_3_lines);
    RUN_TEST(test_char_x_left_first);
    RUN_TEST(test_char_x_left_mid);
    RUN_TEST(test_char_x_center_first);
    RUN_TEST(test_char_x_center_symmetric);
    RUN_TEST(test_char_x_right_last);
    RUN_TEST(test_line_y_first);
    RUN_TEST(test_line_y_second);
    RUN_TEST(test_label_above_centered);
    RUN_TEST(test_label_below_centered);
    RUN_TEST(test_label_above_y);
    RUN_TEST(test_label_below_y);
    RUN_TEST(test_overlap_yes);
    RUN_TEST(test_overlap_no);
    RUN_TEST(test_overlap_touching);
    RUN_TEST(test_clamp_left);
    RUN_TEST(test_clamp_bottom_right);
    RUN_TEST(test_clamp_no_change);
    RUN_TEST(test_style_hud);
    RUN_TEST(test_style_card);
    RUN_TEST(test_purity);
    return UNITY_END();
}
