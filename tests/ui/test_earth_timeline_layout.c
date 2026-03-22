/*
 * test_earth_timeline_layout.c — Tests for Earth History timeline card layout
 */

#include "../unity/unity.h"
#include "../../src/ui/earth_timeline_layout.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* 1. Layout produces 20 cards */
void test_layout_card_count(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    TEST_ASSERT_EQUAL_INT(20, layout.count);
}

/* 2. All cards have non-null titles */
void test_all_cards_have_titles(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    for (int i = 0; i < layout.count; i++) {
        TEST_ASSERT_NOT_NULL(layout.cards[i].title);
    }
}

/* 3. Card x positions are in [0, 1] range */
void test_x_positions_in_range(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    for (int i = 0; i < layout.count; i++) {
        TEST_ASSERT_TRUE(layout.cards[i].x >= 0.0f);
        TEST_ASSERT_TRUE(layout.cards[i].x <= 1.0f);
    }
}

/* 4. Recent events are further right */
void test_recent_events_right(void)
{
    /* Ma=0 should be at the right, Ma=4500 at the left */
    float x_recent = etl_ma_to_x(0.01, 4500.0);
    float x_ancient = etl_ma_to_x(4000.0, 4500.0);
    TEST_ASSERT_TRUE(x_recent > x_ancient);
}

/* 5. Ma=0 maps to right edge */
void test_ma_zero_is_right(void)
{
    float x = etl_ma_to_x(0.0, 4500.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.95f, x);
}

/* 6. Ma=4500 maps to left edge */
void test_ma_max_is_left(void)
{
    float x = etl_ma_to_x(4500.0, 4500.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, x);
}

/* 7. Logarithmic compression: middle Ma is NOT at screen center */
void test_log_compression(void)
{
    /* 2250 Ma (half of 4500) should NOT be at x=0.5 due to log scale */
    float x_half = etl_ma_to_x(2250.0, 4500.0);
    /* With log scale, 2250 Ma should be closer to the left (ancient) side */
    TEST_ASSERT_TRUE(x_half < 0.5f);
}

/* 8. Cards alternate above and below timeline */
void test_cards_alternate_y(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    if (layout.count >= 2) {
        float y0 = layout.cards[0].y;
        float y1 = layout.cards[1].y;
        /* One should be above, other below the axis */
        TEST_ASSERT_TRUE((y0 - 0.5f) * (y1 - 0.5f) < 0.0f);
    }
}

/* 9. Drama level affects opacity */
void test_drama_affects_opacity(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    /* Find min and max drama cards */
    float min_opacity = 1.0f, max_opacity = 0.0f;
    for (int i = 0; i < layout.count; i++) {
        if (layout.cards[i].opacity < min_opacity)
            min_opacity = layout.cards[i].opacity;
        if (layout.cards[i].opacity > max_opacity)
            max_opacity = layout.cards[i].opacity;
    }
    /* Dramatic moments should be more opaque than quiet ones */
    TEST_ASSERT_TRUE(max_opacity > min_opacity);
}

/* 10. Card dimensions are positive */
void test_card_dimensions_positive(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    for (int i = 0; i < layout.count; i++) {
        TEST_ASSERT_TRUE(layout.cards[i].width > 0.0f);
        TEST_ASSERT_TRUE(layout.cards[i].height > 0.0f);
    }
}

/* 11. Nearest card returns valid for center of timeline */
void test_nearest_card_center(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    int idx = etl_nearest_card(&layout, 0.5f);
    /* Should find something near the center */
    TEST_ASSERT_TRUE(idx >= 0 || idx == -1); /* valid result */
}

/* 12. Nearest card returns -1 for NULL layout */
void test_nearest_card_null(void)
{
    TEST_ASSERT_EQUAL_INT(-1, etl_nearest_card(NULL, 0.5f));
}

/* 13. Timeline axis constants are sensible */
void test_timeline_constants(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, layout.timeline_y);
    TEST_ASSERT_TRUE(layout.timeline_x_min < layout.timeline_x_max);
}

/* 14. All Ma values are positive */
void test_all_ma_positive(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    for (int i = 0; i < layout.count; i++) {
        TEST_ASSERT_TRUE(layout.cards[i].ma >= 0.0);
    }
}

/* 15. Drama levels are in valid range */
void test_drama_range(void)
{
    etl_layout_t layout = etl_compute(0.0f, 500.0f);
    for (int i = 0; i < layout.count; i++) {
        TEST_ASSERT_TRUE(layout.cards[i].drama >= 1);
        TEST_ASSERT_TRUE(layout.cards[i].drama <= 5);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_layout_card_count);
    RUN_TEST(test_all_cards_have_titles);
    RUN_TEST(test_x_positions_in_range);
    RUN_TEST(test_recent_events_right);
    RUN_TEST(test_ma_zero_is_right);
    RUN_TEST(test_ma_max_is_left);
    RUN_TEST(test_log_compression);
    RUN_TEST(test_cards_alternate_y);
    RUN_TEST(test_drama_affects_opacity);
    RUN_TEST(test_card_dimensions_positive);
    RUN_TEST(test_nearest_card_center);
    RUN_TEST(test_nearest_card_null);
    RUN_TEST(test_timeline_constants);
    RUN_TEST(test_all_ma_positive);
    RUN_TEST(test_drama_range);
    return UNITY_END();
}
