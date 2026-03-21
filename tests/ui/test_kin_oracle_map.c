#include "../unity/unity.h"
#include "../../src/ui/kin_oracle_map.h"
#include "../../src/systems/tzolkin/dreamspell.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- kin 1: Red Magnetic Dragon --- */

void test_compute_kin_1_destiny_seal_is_0(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_seal);
}

void test_compute_kin_1_destiny_tone_is_1(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    TEST_ASSERT_EQUAL_INT(1, m.destiny_tone);
}

void test_compute_kin_1_destiny_color_is_0_red(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_color);
}

void test_compute_kin_1_destiny_kin_stored(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    TEST_ASSERT_EQUAL_INT(1, m.destiny_kin);
}

/* --- kin 1 oracle matches dreamspell_oracle(1) --- */

void test_compute_kin_1_analog_matches_oracle(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(o.analog, m.rules[0].result_kin);
}

void test_compute_kin_1_antipode_matches_oracle(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(o.antipode, m.rules[1].result_kin);
}

void test_compute_kin_1_hidden_is_260(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(1);
    TEST_ASSERT_EQUAL_INT(260, m.rules[4].result_kin);
}

/* --- kin 131: Blue Magnetic Monkey --- */

void test_compute_kin_131_destiny_seal_is_10(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(131);
    TEST_ASSERT_EQUAL_INT(10, m.destiny_seal);
}

void test_compute_kin_131_destiny_color_is_2_blue(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(131);
    TEST_ASSERT_EQUAL_INT(2, m.destiny_color);
}

/* --- all 5 rules have non-NULL position names --- */

void test_all_rules_have_position_names(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(42);
    for (int i = 0; i < KIN_ORACLE_RULES; i++) {
        TEST_ASSERT_NOT_NULL(m.rules[i].position);
    }
}

/* --- all 5 rules have non-NULL formulas --- */

void test_all_rules_have_formulas(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(42);
    for (int i = 0; i < KIN_ORACLE_RULES; i++) {
        TEST_ASSERT_NOT_NULL(m.rules[i].formula);
    }
}

/* --- all result_kins are 1-260 --- */

void test_all_result_kins_in_range(void)
{
    for (int kin = 1; kin <= 260; kin += 13) {
        kin_oracle_map_t m = kin_oracle_map_compute(kin);
        for (int i = 0; i < KIN_ORACLE_RULES; i++) {
            TEST_ASSERT_TRUE(m.rules[i].result_kin >= 1);
            TEST_ASSERT_TRUE(m.rules[i].result_kin <= 260);
        }
    }
}

/* --- all result_seals are 0-19 --- */

void test_all_result_seals_in_range(void)
{
    for (int kin = 1; kin <= 260; kin += 17) {
        kin_oracle_map_t m = kin_oracle_map_compute(kin);
        for (int i = 0; i < KIN_ORACLE_RULES; i++) {
            TEST_ASSERT_TRUE(m.rules[i].result_seal >= 0);
            TEST_ASSERT_TRUE(m.rules[i].result_seal <= 19);
        }
    }
}

/* --- all result_tones are 1-13 --- */

void test_all_result_tones_in_range(void)
{
    for (int kin = 1; kin <= 260; kin += 17) {
        kin_oracle_map_t m = kin_oracle_map_compute(kin);
        for (int i = 0; i < KIN_ORACLE_RULES; i++) {
            TEST_ASSERT_TRUE(m.rules[i].result_tone >= 1);
            TEST_ASSERT_TRUE(m.rules[i].result_tone <= 13);
        }
    }
}

/* --- all result_colors are 0-3 --- */

void test_all_result_colors_in_range(void)
{
    for (int kin = 1; kin <= 260; kin += 17) {
        kin_oracle_map_t m = kin_oracle_map_compute(kin);
        for (int i = 0; i < KIN_ORACLE_RULES; i++) {
            TEST_ASSERT_TRUE(m.rules[i].result_color >= 0);
            TEST_ASSERT_TRUE(m.rules[i].result_color <= 3);
        }
    }
}

/* --- kin 260: hidden = 1 (261-260) --- */

void test_compute_kin_260_hidden_is_1(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(260);
    TEST_ASSERT_EQUAL_INT(1, m.rules[4].result_kin);
}

/* --- invalid kin 0 returns zeroed --- */

void test_compute_kin_0_returns_zeroed(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(0);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_kin);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_seal);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_tone);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_color);
    TEST_ASSERT_NULL(m.rules[0].position);
}

/* --- invalid kin 261 returns zeroed --- */

void test_compute_kin_261_returns_zeroed(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(261);
    TEST_ASSERT_EQUAL_INT(0, m.destiny_kin);
    TEST_ASSERT_NULL(m.rules[0].position);
}

/* --- rule positions match expected names --- */

void test_rule_positions_correct(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(100);
    TEST_ASSERT_EQUAL_STRING("Analog",   m.rules[0].position);
    TEST_ASSERT_EQUAL_STRING("Antipode", m.rules[1].position);
    TEST_ASSERT_EQUAL_STRING("Occult",   m.rules[2].position);
    TEST_ASSERT_EQUAL_STRING("Guide",    m.rules[3].position);
    TEST_ASSERT_EQUAL_STRING("Hidden",   m.rules[4].position);
}

/* --- formulas contain descriptive text --- */

void test_formulas_contain_descriptive_text(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(100);
    TEST_ASSERT_NOT_NULL(strstr(m.rules[0].formula, "Seal"));
    TEST_ASSERT_NOT_NULL(strstr(m.rules[1].formula, "Seal"));
    TEST_ASSERT_NOT_NULL(strstr(m.rules[2].formula, "Kin"));
    TEST_ASSERT_NOT_NULL(strstr(m.rules[3].formula, "color"));
    TEST_ASSERT_NOT_NULL(strstr(m.rules[4].formula, "Kin"));
}

/* --- source_kin in all rules matches input --- */

void test_source_kin_matches_input(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(77);
    for (int i = 0; i < KIN_ORACLE_RULES; i++) {
        TEST_ASSERT_EQUAL_INT(77, m.rules[i].source_kin);
    }
}

/* --- purity: same input, same output --- */

void test_purity_same_input_same_output(void)
{
    kin_oracle_map_t a = kin_oracle_map_compute(55);
    kin_oracle_map_t b = kin_oracle_map_compute(55);
    TEST_ASSERT_EQUAL_INT(a.destiny_kin, b.destiny_kin);
    TEST_ASSERT_EQUAL_INT(a.destiny_seal, b.destiny_seal);
    TEST_ASSERT_EQUAL_INT(a.destiny_tone, b.destiny_tone);
    TEST_ASSERT_EQUAL_INT(a.destiny_color, b.destiny_color);
    for (int i = 0; i < KIN_ORACLE_RULES; i++) {
        TEST_ASSERT_EQUAL_INT(a.rules[i].result_kin, b.rules[i].result_kin);
        TEST_ASSERT_EQUAL_INT(a.rules[i].result_seal, b.rules[i].result_seal);
        TEST_ASSERT_EQUAL_INT(a.rules[i].result_tone, b.rules[i].result_tone);
        TEST_ASSERT_EQUAL_INT(a.rules[i].result_color, b.rules[i].result_color);
    }
}

/* --- result_color is consistent with result_seal --- */

void test_result_color_matches_seal(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(130);
    for (int i = 0; i < KIN_ORACLE_RULES; i++) {
        TEST_ASSERT_EQUAL_INT(m.rules[i].result_seal % 4,
                              m.rules[i].result_color);
    }
}

/* --- occult and hidden both use 261-K --- */

void test_occult_and_hidden_both_261_minus_k(void)
{
    kin_oracle_map_t m = kin_oracle_map_compute(50);
    TEST_ASSERT_EQUAL_INT(211, m.rules[2].result_kin); /* 261-50 */
    TEST_ASSERT_EQUAL_INT(211, m.rules[4].result_kin); /* 261-50 */
}

int main(void)
{
    UNITY_BEGIN();
    /* kin 1 */
    RUN_TEST(test_compute_kin_1_destiny_seal_is_0);
    RUN_TEST(test_compute_kin_1_destiny_tone_is_1);
    RUN_TEST(test_compute_kin_1_destiny_color_is_0_red);
    RUN_TEST(test_compute_kin_1_destiny_kin_stored);
    RUN_TEST(test_compute_kin_1_analog_matches_oracle);
    RUN_TEST(test_compute_kin_1_antipode_matches_oracle);
    RUN_TEST(test_compute_kin_1_hidden_is_260);
    /* kin 131 */
    RUN_TEST(test_compute_kin_131_destiny_seal_is_10);
    RUN_TEST(test_compute_kin_131_destiny_color_is_2_blue);
    /* rule completeness */
    RUN_TEST(test_all_rules_have_position_names);
    RUN_TEST(test_all_rules_have_formulas);
    /* range validation */
    RUN_TEST(test_all_result_kins_in_range);
    RUN_TEST(test_all_result_seals_in_range);
    RUN_TEST(test_all_result_tones_in_range);
    RUN_TEST(test_all_result_colors_in_range);
    /* kin 260 */
    RUN_TEST(test_compute_kin_260_hidden_is_1);
    /* invalid */
    RUN_TEST(test_compute_kin_0_returns_zeroed);
    RUN_TEST(test_compute_kin_261_returns_zeroed);
    /* rule data */
    RUN_TEST(test_rule_positions_correct);
    RUN_TEST(test_formulas_contain_descriptive_text);
    RUN_TEST(test_source_kin_matches_input);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    /* consistency */
    RUN_TEST(test_result_color_matches_seal);
    RUN_TEST(test_occult_and_hidden_both_261_minus_k);
    return UNITY_END();
}
