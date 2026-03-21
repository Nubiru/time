#include "../unity/unity.h"
#include "../../src/ui/bagua_layout.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* --- Wheel geometry --- */

void test_center_position(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_y);
}

void test_radius(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.35f, layout.radius);
}

/* --- Later Heaven positions --- */

void test_south_is_li(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(2, layout.trigrams[0].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Fire", layout.trigrams[0].name);
    TEST_ASSERT_EQUAL_STRING("S", layout.trigrams[0].direction);
}

void test_southwest_is_kun(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(7, layout.trigrams[1].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Earth", layout.trigrams[1].name);
    TEST_ASSERT_EQUAL_STRING("SW", layout.trigrams[1].direction);
}

void test_west_is_dui(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[2].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Lake", layout.trigrams[2].name);
    TEST_ASSERT_EQUAL_STRING("W", layout.trigrams[2].direction);
}

void test_northwest_is_qian(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[3].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Heaven", layout.trigrams[3].name);
    TEST_ASSERT_EQUAL_STRING("NW", layout.trigrams[3].direction);
}

void test_north_is_kan(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(5, layout.trigrams[4].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Water", layout.trigrams[4].name);
    TEST_ASSERT_EQUAL_STRING("N", layout.trigrams[4].direction);
}

void test_northeast_is_gen(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(6, layout.trigrams[5].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Mountain", layout.trigrams[5].name);
    TEST_ASSERT_EQUAL_STRING("NE", layout.trigrams[5].direction);
}

void test_east_is_zhen(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(3, layout.trigrams[6].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Thunder", layout.trigrams[6].name);
    TEST_ASSERT_EQUAL_STRING("E", layout.trigrams[6].direction);
}

void test_southeast_is_xun(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(4, layout.trigrams[7].trigram_index);
    TEST_ASSERT_EQUAL_STRING("Wind", layout.trigrams[7].name);
    TEST_ASSERT_EQUAL_STRING("SE", layout.trigrams[7].direction);
}

/* --- Element associations --- */

void test_element_fire(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_STRING("Fire", layout.trigrams[0].element);
}

void test_element_earth(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_STRING("Earth", layout.trigrams[1].element);
}

void test_element_heaven(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_STRING("Heaven", layout.trigrams[3].element);
}

void test_element_water(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_STRING("Water", layout.trigrams[4].element);
}

/* --- Trigram lines (from iching.h convention) --- */

void test_lines_qian_heaven(void)
{
    /* Qian (Heaven) = all yang: {1,1,1} */
    bagua_layout_t layout = bagua_layout_compute(-1);
    /* Qian is at position 3 (NW) */
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[3].lines[0]);
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[3].lines[1]);
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[3].lines[2]);
}

void test_lines_kun_earth(void)
{
    /* Kun (Earth) = all yin: {0,0,0} */
    bagua_layout_t layout = bagua_layout_compute(-1);
    /* Kun is at position 1 (SW) */
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[1].lines[0]);
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[1].lines[1]);
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[1].lines[2]);
}

void test_lines_li_fire(void)
{
    /* Li (Fire) = {1,0,1} */
    bagua_layout_t layout = bagua_layout_compute(-1);
    /* Li is at position 0 (S) */
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[0].lines[0]);
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[0].lines[1]);
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[0].lines[2]);
}

void test_lines_kan_water(void)
{
    /* Kan (Water) = {0,1,0} */
    bagua_layout_t layout = bagua_layout_compute(-1);
    /* Kan is at position 4 (N) */
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[4].lines[0]);
    TEST_ASSERT_EQUAL_INT(1, layout.trigrams[4].lines[1]);
    TEST_ASSERT_EQUAL_INT(0, layout.trigrams[4].lines[2]);
}

/* --- Position calculations --- */

void test_south_position_bottom(void)
{
    /* South at 270 degrees: x = 0.5 + 0.35*cos(270) = 0.5 + 0,
     * y = 0.5 - 0.35*sin(270) = 0.5 + 0.35 = 0.85 */
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.trigrams[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.85f, layout.trigrams[0].y);
}

void test_north_position_top(void)
{
    /* North at 90 degrees: x = 0.5, y = 0.5 - 0.35 = 0.15 */
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.trigrams[4].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, layout.trigrams[4].y);
}

void test_east_position_right(void)
{
    /* East at 0 degrees: x = 0.5 + 0.35 = 0.85, y = 0.5 */
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.85f, layout.trigrams[6].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.trigrams[6].y);
}

void test_west_position_left(void)
{
    /* West at 180 degrees: x = 0.5 - 0.35 = 0.15, y = 0.5 */
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, layout.trigrams[2].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.trigrams[2].y);
}

void test_all_positions_on_circle(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    for (int i = 0; i < BAGUA_TRIGRAM_COUNT; i++) {
        float dx = layout.trigrams[i].x - 0.50f;
        float dy = layout.trigrams[i].y - 0.50f;
        float r = sqrtf(dx * dx + dy * dy);
        TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.35f, r);
    }
}

/* --- Angles --- */

void test_south_angle(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 270.0f, layout.trigrams[0].angle_deg);
}

void test_north_angle(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 90.0f, layout.trigrams[4].angle_deg);
}

void test_east_angle(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, layout.trigrams[6].angle_deg);
}

void test_west_angle(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 180.0f, layout.trigrams[2].angle_deg);
}

/* --- Highlighting --- */

void test_highlight_none(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    TEST_ASSERT_EQUAL_INT(-1, layout.highlighted);
}

void test_highlight_zero(void)
{
    bagua_layout_t layout = bagua_layout_compute(0);
    TEST_ASSERT_EQUAL_INT(0, layout.highlighted);
}

void test_highlight_seven(void)
{
    bagua_layout_t layout = bagua_layout_compute(7);
    TEST_ASSERT_EQUAL_INT(7, layout.highlighted);
}

void test_highlight_invalid_high(void)
{
    bagua_layout_t layout = bagua_layout_compute(8);
    TEST_ASSERT_EQUAL_INT(-1, layout.highlighted);
}

void test_highlight_invalid_low(void)
{
    bagua_layout_t layout = bagua_layout_compute(-99);
    TEST_ASSERT_EQUAL_INT(-1, layout.highlighted);
}

/* --- Unicode symbols --- */

void test_symbols_not_null(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    for (int i = 0; i < BAGUA_TRIGRAM_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.trigrams[i].symbol);
        TEST_ASSERT_TRUE(strlen(layout.trigrams[i].symbol) > 0);
    }
}

/* --- Accessor: bagua_trigram_at --- */

void test_accessor_valid(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    bagua_trigram_t t = bagua_trigram_at(&layout, 0);
    TEST_ASSERT_EQUAL_INT(2, t.trigram_index);  /* Li at position 0 */
    TEST_ASSERT_EQUAL_STRING("Fire", t.name);
}

void test_accessor_invalid_negative(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    bagua_trigram_t t = bagua_trigram_at(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, t.trigram_index);
}

void test_accessor_invalid_high(void)
{
    bagua_layout_t layout = bagua_layout_compute(-1);
    bagua_trigram_t t = bagua_trigram_at(&layout, 8);
    TEST_ASSERT_EQUAL_INT(-1, t.trigram_index);
}

void test_accessor_null_layout(void)
{
    bagua_trigram_t t = bagua_trigram_at(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, t.trigram_index);
}

/* --- Accessor: bagua_direction_name --- */

void test_direction_name_south(void)
{
    TEST_ASSERT_EQUAL_STRING("S", bagua_direction_name(0));
}

void test_direction_name_north(void)
{
    TEST_ASSERT_EQUAL_STRING("N", bagua_direction_name(4));
}

void test_direction_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", bagua_direction_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", bagua_direction_name(8));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    bagua_layout_t a = bagua_layout_compute(3);
    bagua_layout_t b = bagua_layout_compute(3);
    TEST_ASSERT_EQUAL_INT(a.highlighted, b.highlighted);
    for (int i = 0; i < BAGUA_TRIGRAM_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.trigrams[i].trigram_index,
                              b.trigrams[i].trigram_index);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.trigrams[i].x, b.trigrams[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.trigrams[i].y, b.trigrams[i].y);
    }
}

void test_purity_different_highlight(void)
{
    bagua_layout_t a = bagua_layout_compute(0);
    bagua_layout_t b = bagua_layout_compute(5);
    TEST_ASSERT_NOT_EQUAL(a.highlighted, b.highlighted);
    /* Positions should be identical regardless of highlight */
    for (int i = 0; i < BAGUA_TRIGRAM_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.trigrams[i].x, b.trigrams[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.trigrams[i].y, b.trigrams[i].y);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* Wheel geometry */
    RUN_TEST(test_center_position);
    RUN_TEST(test_radius);
    /* Later Heaven positions */
    RUN_TEST(test_south_is_li);
    RUN_TEST(test_southwest_is_kun);
    RUN_TEST(test_west_is_dui);
    RUN_TEST(test_northwest_is_qian);
    RUN_TEST(test_north_is_kan);
    RUN_TEST(test_northeast_is_gen);
    RUN_TEST(test_east_is_zhen);
    RUN_TEST(test_southeast_is_xun);
    /* Elements */
    RUN_TEST(test_element_fire);
    RUN_TEST(test_element_earth);
    RUN_TEST(test_element_heaven);
    RUN_TEST(test_element_water);
    /* Trigram lines */
    RUN_TEST(test_lines_qian_heaven);
    RUN_TEST(test_lines_kun_earth);
    RUN_TEST(test_lines_li_fire);
    RUN_TEST(test_lines_kan_water);
    /* Position calculations */
    RUN_TEST(test_south_position_bottom);
    RUN_TEST(test_north_position_top);
    RUN_TEST(test_east_position_right);
    RUN_TEST(test_west_position_left);
    RUN_TEST(test_all_positions_on_circle);
    /* Angles */
    RUN_TEST(test_south_angle);
    RUN_TEST(test_north_angle);
    RUN_TEST(test_east_angle);
    RUN_TEST(test_west_angle);
    /* Highlighting */
    RUN_TEST(test_highlight_none);
    RUN_TEST(test_highlight_zero);
    RUN_TEST(test_highlight_seven);
    RUN_TEST(test_highlight_invalid_high);
    RUN_TEST(test_highlight_invalid_low);
    /* Symbols */
    RUN_TEST(test_symbols_not_null);
    /* Accessor: trigram_at */
    RUN_TEST(test_accessor_valid);
    RUN_TEST(test_accessor_invalid_negative);
    RUN_TEST(test_accessor_invalid_high);
    RUN_TEST(test_accessor_null_layout);
    /* Accessor: direction_name */
    RUN_TEST(test_direction_name_south);
    RUN_TEST(test_direction_name_north);
    RUN_TEST(test_direction_name_invalid);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_highlight);
    return UNITY_END();
}
