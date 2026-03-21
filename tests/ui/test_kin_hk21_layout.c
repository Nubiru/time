#include "../unity/unity.h"
#include "../../src/ui/kin_hk21_layout.h"

void setUp(void) {}
void tearDown(void) {}

/* --- kin_hk21_compute: seal 0 (Dragon) --- */

void test_seal_0_today_seal(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    TEST_ASSERT_EQUAL_INT(0, layout.today_seal);
}

void test_seal_0_today_node(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    TEST_ASSERT_EQUAL_INT(0, layout.today_node);
}

void test_seal_0_node_0_highlighted(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    TEST_ASSERT_EQUAL_INT(1, layout.nodes[0].highlighted);
}

/* --- 21 nodes total --- */

void test_21_nodes_valid_seal_codes(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, layout.nodes[i].seal_code);
    }
}

void test_all_seal_names_not_null(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_NOT_NULL(layout.nodes[i].seal_name);
    }
}

void test_all_statements_not_null(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_NOT_NULL(layout.nodes[i].statement);
    }
}

void test_all_planets_not_null(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_NOT_NULL(layout.nodes[i].planet);
    }
}

/* --- Hunab Ku center node --- */

void test_hunab_ku_center_position(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.nodes[20].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.nodes[20].y);
}

void test_hunab_ku_seal_code_21(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    TEST_ASSERT_EQUAL_INT(21, layout.nodes[20].seal_code);
}

void test_hunab_ku_name(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    TEST_ASSERT_EQUAL_STRING("Hunab Ku", layout.nodes[20].seal_name);
}

/* --- Only one node highlighted --- */

void test_only_one_highlighted(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(5);
    int count = 0;
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        if (layout.nodes[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_seal_5_highlights_node_5(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(5);
    /* seal 5 -> seal_code 6 -> node index 5 */
    TEST_ASSERT_EQUAL_INT(1, layout.nodes[5].highlighted);
}

/* --- All positions in valid range --- */

void test_all_positions_in_range(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(10);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_TRUE(layout.nodes[i].x >= 0.0f && layout.nodes[i].x <= 1.0f);
        TEST_ASSERT_TRUE(layout.nodes[i].y >= 0.0f && layout.nodes[i].y <= 1.0f);
    }
}

/* --- kin_hk21_seal_to_code --- */

void test_seal_to_code_0(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_hk21_seal_to_code(0));
}

void test_seal_to_code_19(void)
{
    TEST_ASSERT_EQUAL_INT(20, kin_hk21_seal_to_code(19));
}

void test_seal_to_code_invalid_neg(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_hk21_seal_to_code(-1));
}

void test_seal_to_code_invalid_20(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_hk21_seal_to_code(20));
}

/* --- Invalid seal returns zeroed struct --- */

void test_invalid_seal_neg1(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(-1);
    TEST_ASSERT_EQUAL_INT(0, layout.today_seal);
    TEST_ASSERT_EQUAL_INT(0, layout.today_node);
    TEST_ASSERT_NULL(layout.nodes[0].seal_name);
}

void test_invalid_seal_20(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(20);
    TEST_ASSERT_EQUAL_INT(0, layout.today_seal);
    TEST_ASSERT_NULL(layout.nodes[0].seal_name);
}

/* --- hk_numbers all > 0 --- */

void test_all_hk_numbers_positive(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_TRUE(layout.nodes[i].hk_number > 0);
    }
}

/* --- Domains all 0-4 --- */

void test_all_domains_valid(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < KIN_HK21_NODES; i++) {
        TEST_ASSERT_TRUE(layout.nodes[i].domain >= 0 && layout.nodes[i].domain <= 4);
    }
}

/* --- Ring positions --- */

void test_ring1_y_position(void)
{
    /* Seals 1-4 (nodes 0-3) are Gates of Light at y=0.1 */
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.10f, layout.nodes[i].y);
    }
}

void test_ring5_y_position(void)
{
    /* Seals 17-20 (nodes 16-19) are Portal Matrix at y=0.9 */
    kin_hk21_layout_t layout = kin_hk21_compute(0);
    for (int i = 16; i < 20; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.90f, layout.nodes[i].y);
    }
}

/* --- Seal 19 highlights last solar seal node --- */

void test_seal_19_highlights_node_19(void)
{
    kin_hk21_layout_t layout = kin_hk21_compute(19);
    TEST_ASSERT_EQUAL_INT(1, layout.nodes[19].highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.nodes[20].highlighted);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_seal_0_today_seal);
    RUN_TEST(test_seal_0_today_node);
    RUN_TEST(test_seal_0_node_0_highlighted);
    RUN_TEST(test_21_nodes_valid_seal_codes);
    RUN_TEST(test_all_seal_names_not_null);
    RUN_TEST(test_all_statements_not_null);
    RUN_TEST(test_all_planets_not_null);
    RUN_TEST(test_hunab_ku_center_position);
    RUN_TEST(test_hunab_ku_seal_code_21);
    RUN_TEST(test_hunab_ku_name);
    RUN_TEST(test_only_one_highlighted);
    RUN_TEST(test_seal_5_highlights_node_5);
    RUN_TEST(test_all_positions_in_range);
    RUN_TEST(test_seal_to_code_0);
    RUN_TEST(test_seal_to_code_19);
    RUN_TEST(test_seal_to_code_invalid_neg);
    RUN_TEST(test_seal_to_code_invalid_20);
    RUN_TEST(test_invalid_seal_neg1);
    RUN_TEST(test_invalid_seal_20);
    RUN_TEST(test_all_hk_numbers_positive);
    RUN_TEST(test_all_domains_valid);
    RUN_TEST(test_ring1_y_position);
    RUN_TEST(test_ring5_y_position);
    RUN_TEST(test_seal_19_highlights_node_19);
    return UNITY_END();
}
