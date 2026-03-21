#include "../unity/unity.h"
#include "../../src/ui/bodygraph_chart_layout.h"
#include "../../src/systems/human_design/bodygraph.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Helper: build an active_gates array with specific gates set --- */

static void make_gates(int *gates, const int *list, int count)
{
    memset(gates, 0, 65 * sizeof(int));
    for (int i = 0; i < count; i++) {
        if (list[i] >= 1 && list[i] <= 64) {
            gates[list[i]] = 1;
        }
    }
}

/* --- NULL input (blank bodygraph) --- */

void test_null_input_defined_count(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_INT(0, layout.defined_count);
}

void test_null_input_type_reflector(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_INT(4, layout.type);
    TEST_ASSERT_EQUAL_STRING("Reflector", layout.type_name);
}

void test_null_input_channel_count(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_INT(36, layout.channel_count);
}

void test_null_input_no_active_channels(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_INT(0, layout.active_channel_count);
}

/* --- Bounds --- */

void test_bounds_x(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.bounds_x);
}

void test_bounds_y(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.bounds_y);
}

void test_bounds_w(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.bounds_w);
}

void test_bounds_h(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.bounds_h);
}

/* --- Center positions --- */

void test_center_count_is_nine(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    /* All 9 centers should be populated. */
    for (int i = 0; i < BG_CHART_CENTER_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.centers[i].id);
    }
}

void test_center_names(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_STRING("Head", layout.centers[0].name);
    TEST_ASSERT_EQUAL_STRING("Sacral", layout.centers[6].name);
    TEST_ASSERT_EQUAL_STRING("Root", layout.centers[8].name);
}

void test_center_positions_within_bounds(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < BG_CHART_CENTER_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.centers[i].x >= layout.bounds_x);
        TEST_ASSERT_TRUE(layout.centers[i].x <= layout.bounds_x + layout.bounds_w);
        TEST_ASSERT_TRUE(layout.centers[i].y >= layout.bounds_y);
        TEST_ASSERT_TRUE(layout.centers[i].y <= layout.bounds_y + layout.bounds_h);
    }
}

void test_center_sizes(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < BG_CHART_CENTER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.12f, layout.centers[i].w);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.08f, layout.centers[i].h);
    }
}

void test_head_above_root(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    /* Head (0) should have smaller y than Root (8). */
    TEST_ASSERT_TRUE(layout.centers[0].y < layout.centers[8].y);
}

void test_motor_centers(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    /* Heart(4), Sacral(6), Solar Plexus(7), Root(8) are motors. */
    TEST_ASSERT_EQUAL_INT(1, layout.centers[4].is_motor);  /* Heart */
    TEST_ASSERT_EQUAL_INT(1, layout.centers[6].is_motor);  /* Sacral */
    TEST_ASSERT_EQUAL_INT(1, layout.centers[7].is_motor);  /* Solar Plexus */
    TEST_ASSERT_EQUAL_INT(1, layout.centers[8].is_motor);  /* Root */
    /* Head, Ajna, Throat, G Center, Spleen are not motors. */
    TEST_ASSERT_EQUAL_INT(0, layout.centers[0].is_motor);  /* Head */
    TEST_ASSERT_EQUAL_INT(0, layout.centers[2].is_motor);  /* Throat */
}

void test_awareness_centers(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    /* Ajna(1), Spleen(5), Solar Plexus(7) are awareness. */
    TEST_ASSERT_EQUAL_INT(1, layout.centers[1].is_awareness);
    TEST_ASSERT_EQUAL_INT(1, layout.centers[5].is_awareness);
    TEST_ASSERT_EQUAL_INT(1, layout.centers[7].is_awareness);
    TEST_ASSERT_EQUAL_INT(0, layout.centers[0].is_awareness);
    TEST_ASSERT_EQUAL_INT(0, layout.centers[6].is_awareness);
}

/* --- Defined/undefined centers --- */

void test_no_gates_all_undefined(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < BG_CHART_CENTER_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.centers[i].is_defined);
    }
}

void test_channel_64_47_defines_head_ajna(void)
{
    /* Gates 64 and 47 form a channel between Head and Ajna. */
    int gates[65];
    int active[] = { 64, 47 };
    make_gates(gates, active, 2);

    bodygraph_chart_layout_t layout = bodygraph_chart_compute(gates);
    TEST_ASSERT_EQUAL_INT(1, layout.centers[0].is_defined);  /* Head */
    TEST_ASSERT_EQUAL_INT(1, layout.centers[1].is_defined);  /* Ajna */
    TEST_ASSERT_EQUAL_INT(0, layout.centers[2].is_defined);  /* Throat */
    TEST_ASSERT_EQUAL_INT(2, layout.defined_count);
}

void test_sacral_defined_gives_generator(void)
{
    /* Gates 53 and 42 form Root-Sacral channel, defining Sacral. */
    int gates[65];
    int active[] = { 53, 42 };
    make_gates(gates, active, 2);

    bodygraph_chart_layout_t layout = bodygraph_chart_compute(gates);
    TEST_ASSERT_EQUAL_INT(1, layout.centers[6].is_defined);  /* Sacral */
    TEST_ASSERT_EQUAL_INT(1, layout.type);
    TEST_ASSERT_EQUAL_STRING("Generator", layout.type_name);
}

void test_no_sacral_defined_gives_unknown(void)
{
    /* Gates 64+47: Head-Ajna channel. No Sacral. */
    int gates[65];
    int active[] = { 64, 47 };
    make_gates(gates, active, 2);

    bodygraph_chart_layout_t layout = bodygraph_chart_compute(gates);
    TEST_ASSERT_EQUAL_INT(0, layout.centers[6].is_defined);  /* Sacral not defined */
    TEST_ASSERT_EQUAL_INT(-1, layout.type);
    TEST_ASSERT_EQUAL_STRING("Unknown", layout.type_name);
}

/* --- Channels --- */

void test_total_channel_count(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_INT(36, layout.channel_count);
}

void test_channel_indices_sequential(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < layout.channel_count; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.channels[i].channel_index);
    }
}

void test_channel_gates_valid(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < layout.channel_count; i++) {
        TEST_ASSERT_TRUE(layout.channels[i].gate_a >= 1);
        TEST_ASSERT_TRUE(layout.channels[i].gate_a <= 64);
        TEST_ASSERT_TRUE(layout.channels[i].gate_b >= 1);
        TEST_ASSERT_TRUE(layout.channels[i].gate_b <= 64);
    }
}

void test_channel_centers_valid(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < layout.channel_count; i++) {
        TEST_ASSERT_TRUE(layout.channels[i].center_a >= 0);
        TEST_ASSERT_TRUE(layout.channels[i].center_a < BG_CHART_CENTER_COUNT);
        TEST_ASSERT_TRUE(layout.channels[i].center_b >= 0);
        TEST_ASSERT_TRUE(layout.channels[i].center_b < BG_CHART_CENTER_COUNT);
    }
}

void test_active_channel_with_both_gates(void)
{
    /* Gates 64+47 = channel 0 (Head-Ajna). */
    int gates[65];
    int active[] = { 64, 47 };
    make_gates(gates, active, 2);

    bodygraph_chart_layout_t layout = bodygraph_chart_compute(gates);
    TEST_ASSERT_EQUAL_INT(1, layout.channels[0].is_active);
    TEST_ASSERT_EQUAL_INT(1, layout.active_channel_count);
}

void test_inactive_channel_with_one_gate(void)
{
    /* Only gate 64, missing gate 47. */
    int gates[65];
    int active[] = { 64 };
    make_gates(gates, active, 1);

    bodygraph_chart_layout_t layout = bodygraph_chart_compute(gates);
    TEST_ASSERT_EQUAL_INT(0, layout.channels[0].is_active);
    TEST_ASSERT_EQUAL_INT(0, layout.active_channel_count);
}

void test_channel_endpoints_match_centers(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    for (int i = 0; i < layout.channel_count; i++) {
        int ca = layout.channels[i].center_a;
        int cb = layout.channels[i].center_b;
        TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.centers[ca].x, layout.channels[i].x0);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.centers[ca].y, layout.channels[i].y0);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.centers[cb].x, layout.channels[i].x1);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, layout.centers[cb].y, layout.channels[i].y1);
    }
}

/* --- Accessor functions --- */

void test_accessor_valid_center(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    chart_center_t c = bodygraph_chart_center(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, c.id);
    TEST_ASSERT_EQUAL_STRING("Head", c.name);
}

void test_accessor_invalid_center_negative(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    chart_center_t c = bodygraph_chart_center(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
    TEST_ASSERT_EQUAL_STRING("?", c.name);
}

void test_accessor_invalid_center_high(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    chart_center_t c = bodygraph_chart_center(&layout, 9);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
}

void test_accessor_null_layout(void)
{
    chart_center_t c = bodygraph_chart_center(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
}

void test_is_defined_accessor(void)
{
    int gates[65];
    int active[] = { 64, 47 };
    make_gates(gates, active, 2);

    bodygraph_chart_layout_t layout = bodygraph_chart_compute(gates);
    TEST_ASSERT_EQUAL_INT(1, bodygraph_chart_is_defined(&layout, 0));  /* Head */
    TEST_ASSERT_EQUAL_INT(0, bodygraph_chart_is_defined(&layout, 2));  /* Throat */
}

void test_is_defined_null_layout(void)
{
    TEST_ASSERT_EQUAL_INT(0, bodygraph_chart_is_defined(NULL, 0));
}

void test_is_defined_invalid_center(void)
{
    bodygraph_chart_layout_t layout = bodygraph_chart_compute(NULL);
    TEST_ASSERT_EQUAL_INT(0, bodygraph_chart_is_defined(&layout, -1));
    TEST_ASSERT_EQUAL_INT(0, bodygraph_chart_is_defined(&layout, 99));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    int gates[65];
    int active[] = { 64, 47, 53, 42 };
    make_gates(gates, active, 4);

    bodygraph_chart_layout_t a = bodygraph_chart_compute(gates);
    bodygraph_chart_layout_t b = bodygraph_chart_compute(gates);

    TEST_ASSERT_EQUAL_INT(a.defined_count, b.defined_count);
    TEST_ASSERT_EQUAL_INT(a.active_channel_count, b.active_channel_count);
    TEST_ASSERT_EQUAL_INT(a.type, b.type);
    for (int i = 0; i < BG_CHART_CENTER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.centers[i].x, b.centers[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.centers[i].y, b.centers[i].y);
        TEST_ASSERT_EQUAL_INT(a.centers[i].is_defined, b.centers[i].is_defined);
    }
}

void test_purity_different_input_different_output(void)
{
    int gates_a[65];
    int active_a[] = { 64, 47 };
    make_gates(gates_a, active_a, 2);

    int gates_b[65];
    int active_b[] = { 53, 42 };
    make_gates(gates_b, active_b, 2);

    bodygraph_chart_layout_t a = bodygraph_chart_compute(gates_a);
    bodygraph_chart_layout_t b = bodygraph_chart_compute(gates_b);

    /* Different active channels should differ. */
    TEST_ASSERT_EQUAL_INT(1, a.channels[0].is_active);   /* 64-47 */
    TEST_ASSERT_EQUAL_INT(0, b.channels[0].is_active);   /* not 64-47 */
}

int main(void)
{
    UNITY_BEGIN();
    /* NULL input */
    RUN_TEST(test_null_input_defined_count);
    RUN_TEST(test_null_input_type_reflector);
    RUN_TEST(test_null_input_channel_count);
    RUN_TEST(test_null_input_no_active_channels);
    /* Bounds */
    RUN_TEST(test_bounds_x);
    RUN_TEST(test_bounds_y);
    RUN_TEST(test_bounds_w);
    RUN_TEST(test_bounds_h);
    /* Center positions */
    RUN_TEST(test_center_count_is_nine);
    RUN_TEST(test_center_names);
    RUN_TEST(test_center_positions_within_bounds);
    RUN_TEST(test_center_sizes);
    RUN_TEST(test_head_above_root);
    RUN_TEST(test_motor_centers);
    RUN_TEST(test_awareness_centers);
    /* Defined/undefined */
    RUN_TEST(test_no_gates_all_undefined);
    RUN_TEST(test_channel_64_47_defines_head_ajna);
    RUN_TEST(test_sacral_defined_gives_generator);
    RUN_TEST(test_no_sacral_defined_gives_unknown);
    /* Channels */
    RUN_TEST(test_total_channel_count);
    RUN_TEST(test_channel_indices_sequential);
    RUN_TEST(test_channel_gates_valid);
    RUN_TEST(test_channel_centers_valid);
    RUN_TEST(test_active_channel_with_both_gates);
    RUN_TEST(test_inactive_channel_with_one_gate);
    RUN_TEST(test_channel_endpoints_match_centers);
    /* Accessors */
    RUN_TEST(test_accessor_valid_center);
    RUN_TEST(test_accessor_invalid_center_negative);
    RUN_TEST(test_accessor_invalid_center_high);
    RUN_TEST(test_accessor_null_layout);
    RUN_TEST(test_is_defined_accessor);
    RUN_TEST(test_is_defined_null_layout);
    RUN_TEST(test_is_defined_invalid_center);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_input_different_output);
    return UNITY_END();
}
