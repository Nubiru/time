#include "../unity/unity.h"
#include "../../src/ui/daily_hd_layout.h"
#include "../../src/systems/human_design/human_design.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Card frame --- */

void test_card_x(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
}

void test_card_y(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
}

void test_card_w(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
}

void test_card_h(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Gate count --- */

void test_gate_count(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_EQUAL_INT(2, layout.gate_count);
}

/* --- Sun gate --- */

void test_sun_gate_at_zero_degrees(void)
{
    /* At 0 degrees, Sun is in gate 17 (first in the HD gate sequence). */
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    hd_gate_t expected = hd_gate_from_longitude(0.0);
    TEST_ASSERT_EQUAL_INT(expected.gate, layout.sun_gate.gate);
    TEST_ASSERT_EQUAL_INT(expected.line, layout.sun_gate.line);
}

void test_sun_gate_name_at_zero(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    const char *expected_name = hd_gate_name(layout.sun_gate.gate);
    TEST_ASSERT_EQUAL_STRING(expected_name, layout.sun_gate.gate_name);
}

void test_sun_gate_keyword_at_zero(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    const char *expected_kw = hd_gate_keyword(layout.sun_gate.gate);
    TEST_ASSERT_EQUAL_STRING(expected_kw, layout.sun_gate.keyword);
}

void test_sun_gate_position(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.22f, layout.sun_gate.slot.y);
}

void test_sun_gate_at_180(void)
{
    daily_hd_layout_t layout = daily_hd_compute(180.0);
    hd_gate_t expected = hd_gate_from_longitude(180.0);
    TEST_ASSERT_EQUAL_INT(expected.gate, layout.sun_gate.gate);
}

/* --- Earth gate --- */

void test_earth_gate_at_zero_degrees(void)
{
    /* Earth = Sun + 180, so at Sun=0, Earth is at 180 degrees. */
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    hd_gate_t expected = hd_gate_from_longitude(180.0);
    TEST_ASSERT_EQUAL_INT(expected.gate, layout.earth_gate.gate);
    TEST_ASSERT_EQUAL_INT(expected.line, layout.earth_gate.line);
}

void test_earth_gate_name_at_zero(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    const char *expected_name = hd_gate_name(layout.earth_gate.gate);
    TEST_ASSERT_EQUAL_STRING(expected_name, layout.earth_gate.gate_name);
}

void test_earth_gate_keyword_at_zero(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    const char *expected_kw = hd_gate_keyword(layout.earth_gate.gate);
    TEST_ASSERT_EQUAL_STRING(expected_kw, layout.earth_gate.keyword);
}

void test_earth_gate_position(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.48f, layout.earth_gate.slot.y);
}

void test_earth_below_sun(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_TRUE(layout.earth_gate.slot.y > layout.sun_gate.slot.y);
}

/* --- Slot positions --- */

void test_title_slot_position(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
}

void test_cross_slot_position(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.14f, layout.cross_slot.y);
}

void test_sun_label_position(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.sun_label.y);
}

void test_earth_label_position(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.42f, layout.earth_label.y);
}

void test_slots_horizontal_alignment(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    /* All text slots share the same x position. */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.title_slot.x, layout.sun_label.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.title_slot.x, layout.earth_label.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.title_slot.x, layout.cross_slot.x);
}

/* --- Accessor functions --- */

void test_accessor_sun_gate_name(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    const char *name = daily_hd_sun_gate_name(&layout);
    TEST_ASSERT_EQUAL_STRING(layout.sun_gate.gate_name, name);
}

void test_accessor_earth_gate_name(void)
{
    daily_hd_layout_t layout = daily_hd_compute(0.0);
    const char *name = daily_hd_earth_gate_name(&layout);
    TEST_ASSERT_EQUAL_STRING(layout.earth_gate.gate_name, name);
}

void test_accessor_sun_null_layout(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_hd_sun_gate_name(NULL));
}

void test_accessor_earth_null_layout(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_hd_earth_gate_name(NULL));
}

/* --- Longitude edge cases --- */

void test_longitude_360(void)
{
    /* 360 should wrap to 0. */
    daily_hd_layout_t a = daily_hd_compute(0.0);
    daily_hd_layout_t b = daily_hd_compute(360.0);
    TEST_ASSERT_EQUAL_INT(a.sun_gate.gate, b.sun_gate.gate);
    TEST_ASSERT_EQUAL_INT(a.earth_gate.gate, b.earth_gate.gate);
}

void test_longitude_negative(void)
{
    /* -90 should normalize to 270. */
    daily_hd_layout_t a = daily_hd_compute(-90.0);
    daily_hd_layout_t b = daily_hd_compute(270.0);
    TEST_ASSERT_EQUAL_INT(a.sun_gate.gate, b.sun_gate.gate);
    TEST_ASSERT_EQUAL_INT(a.earth_gate.gate, b.earth_gate.gate);
}

void test_longitude_large(void)
{
    /* 720 should wrap to 0. */
    daily_hd_layout_t a = daily_hd_compute(720.0);
    daily_hd_layout_t b = daily_hd_compute(0.0);
    TEST_ASSERT_EQUAL_INT(a.sun_gate.gate, b.sun_gate.gate);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_hd_layout_t a = daily_hd_compute(45.0);
    daily_hd_layout_t b = daily_hd_compute(45.0);
    TEST_ASSERT_EQUAL_INT(a.sun_gate.gate, b.sun_gate.gate);
    TEST_ASSERT_EQUAL_INT(a.sun_gate.line, b.sun_gate.line);
    TEST_ASSERT_EQUAL_INT(a.earth_gate.gate, b.earth_gate.gate);
    TEST_ASSERT_EQUAL_INT(a.earth_gate.line, b.earth_gate.line);
    TEST_ASSERT_EQUAL_STRING(a.sun_gate.gate_name, b.sun_gate.gate_name);
    TEST_ASSERT_EQUAL_STRING(a.earth_gate.gate_name, b.earth_gate.gate_name);
}

void test_purity_different_input_different_output(void)
{
    daily_hd_layout_t a = daily_hd_compute(0.0);
    daily_hd_layout_t b = daily_hd_compute(90.0);
    /* Different longitudes should produce different gates. */
    int any_diff = (a.sun_gate.gate != b.sun_gate.gate) ||
                   (a.earth_gate.gate != b.earth_gate.gate);
    TEST_ASSERT_EQUAL_INT(1, any_diff);
}

int main(void)
{
    UNITY_BEGIN();
    /* Card frame */
    RUN_TEST(test_card_x);
    RUN_TEST(test_card_y);
    RUN_TEST(test_card_w);
    RUN_TEST(test_card_h);
    /* Gate count */
    RUN_TEST(test_gate_count);
    /* Sun gate */
    RUN_TEST(test_sun_gate_at_zero_degrees);
    RUN_TEST(test_sun_gate_name_at_zero);
    RUN_TEST(test_sun_gate_keyword_at_zero);
    RUN_TEST(test_sun_gate_position);
    RUN_TEST(test_sun_gate_at_180);
    /* Earth gate */
    RUN_TEST(test_earth_gate_at_zero_degrees);
    RUN_TEST(test_earth_gate_name_at_zero);
    RUN_TEST(test_earth_gate_keyword_at_zero);
    RUN_TEST(test_earth_gate_position);
    RUN_TEST(test_earth_below_sun);
    /* Slot positions */
    RUN_TEST(test_title_slot_position);
    RUN_TEST(test_cross_slot_position);
    RUN_TEST(test_sun_label_position);
    RUN_TEST(test_earth_label_position);
    RUN_TEST(test_slots_horizontal_alignment);
    /* Accessors */
    RUN_TEST(test_accessor_sun_gate_name);
    RUN_TEST(test_accessor_earth_gate_name);
    RUN_TEST(test_accessor_sun_null_layout);
    RUN_TEST(test_accessor_earth_null_layout);
    /* Edge cases */
    RUN_TEST(test_longitude_360);
    RUN_TEST(test_longitude_negative);
    RUN_TEST(test_longitude_large);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_input_different_output);
    return UNITY_END();
}
