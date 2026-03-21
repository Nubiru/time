#include "../unity/unity.h"
#include "../../src/ui/kin_seven_gen.h"
#include "../../src/systems/tzolkin/dreamspell.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Source kin preservation --- */

void test_source_kin_preserved(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_EQUAL_INT(1, sg.source_kin);
}

void test_source_kin_preserved_kin_260(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(260);
    TEST_ASSERT_EQUAL_INT(260, sg.source_kin);
}

/* --- Level 0 destiny matches input kin --- */

void test_level0_destiny_is_input_kin(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_EQUAL_INT(1, sg.levels[0].destiny.kin);
}

void test_level0_destiny_kin44(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(44);
    TEST_ASSERT_EQUAL_INT(44, sg.levels[0].destiny.kin);
}

/* --- Level names --- */

void test_level_name_crown(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_EQUAL_STRING("Crown", sg.levels[0].level_name);
}

void test_level_name_heart(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_EQUAL_STRING("Heart", sg.levels[1].level_name);
}

void test_level_name_root(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_EQUAL_STRING("Root", sg.levels[2].level_name);
}

/* --- Center positions --- */

void test_level0_center_position(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, sg.levels[0].center_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, sg.levels[0].center_y);
}

void test_level1_center_position(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, sg.levels[1].center_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, sg.levels[1].center_y);
}

void test_level2_center_position(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, sg.levels[2].center_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, sg.levels[2].center_y);
}

/* --- All 15 oracle cells have valid kin --- */

void test_all_cells_valid_kin(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(100);
    for (int lvl = 0; lvl < KIN_SEVEN_GEN_LEVELS; lvl++) {
        kin_seven_gen_level_t *l = &sg.levels[lvl];
        TEST_ASSERT_TRUE(l->destiny.kin >= 1 && l->destiny.kin <= 260);
        TEST_ASSERT_TRUE(l->guide.kin >= 1 && l->guide.kin <= 260);
        TEST_ASSERT_TRUE(l->analog.kin >= 1 && l->analog.kin <= 260);
        TEST_ASSERT_TRUE(l->antipode.kin >= 1 && l->antipode.kin <= 260);
        TEST_ASSERT_TRUE(l->occult.kin >= 1 && l->occult.kin <= 260);
    }
}

/* --- All seals 0-19, tones 1-13, colors 0-3 --- */

void test_all_cells_valid_seal_tone_color(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(55);
    for (int lvl = 0; lvl < KIN_SEVEN_GEN_LEVELS; lvl++) {
        kin_seven_gen_cell_t cells[KIN_ORACLE_SIZE];
        cells[0] = sg.levels[lvl].destiny;
        cells[1] = sg.levels[lvl].guide;
        cells[2] = sg.levels[lvl].analog;
        cells[3] = sg.levels[lvl].antipode;
        cells[4] = sg.levels[lvl].occult;
        for (int c = 0; c < KIN_ORACLE_SIZE; c++) {
            TEST_ASSERT_TRUE(cells[c].seal >= 0 && cells[c].seal <= 19);
            TEST_ASSERT_TRUE(cells[c].tone >= 1 && cells[c].tone <= 13);
            TEST_ASSERT_TRUE(cells[c].color >= 0 && cells[c].color <= 3);
        }
    }
}

/* --- Level 1 destiny = level 0's analog --- */

void test_level1_destiny_is_level0_analog(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    dreamspell_oracle_t oracle0 = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(oracle0.analog, sg.levels[1].destiny.kin);
}

void test_level1_destiny_is_level0_analog_kin130(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(130);
    dreamspell_oracle_t oracle0 = dreamspell_oracle(130);
    TEST_ASSERT_EQUAL_INT(oracle0.analog, sg.levels[1].destiny.kin);
}

/* --- Level 2 destiny = level 0's guide --- */

void test_level2_destiny_is_level0_guide(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    dreamspell_oracle_t oracle0 = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(oracle0.guide, sg.levels[2].destiny.kin);
}

void test_level2_destiny_is_level0_guide_kin200(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(200);
    dreamspell_oracle_t oracle0 = dreamspell_oracle(200);
    TEST_ASSERT_EQUAL_INT(oracle0.guide, sg.levels[2].destiny.kin);
}

/* --- Invalid kin returns zeroed struct --- */

void test_invalid_kin_zero(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(0);
    TEST_ASSERT_EQUAL_INT(0, sg.source_kin);
    TEST_ASSERT_EQUAL_INT(0, sg.levels[0].destiny.kin);
    TEST_ASSERT_EQUAL_INT(0, sg.levels[1].destiny.kin);
    TEST_ASSERT_EQUAL_INT(0, sg.levels[2].destiny.kin);
}

void test_invalid_kin_261(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(261);
    TEST_ASSERT_EQUAL_INT(0, sg.source_kin);
    TEST_ASSERT_EQUAL_INT(0, sg.levels[0].destiny.kin);
}

/* --- Different kins produce different level 1/2 destiny values --- */

void test_different_kins_different_level1_destiny(void)
{
    kin_seven_gen_t sg1 = kin_seven_gen_compute(1);
    kin_seven_gen_t sg2 = kin_seven_gen_compute(50);
    /* Different source kins should generally produce different analog partners */
    TEST_ASSERT_TRUE(sg1.levels[1].destiny.kin != sg2.levels[1].destiny.kin);
}

void test_different_kins_different_level2_destiny(void)
{
    kin_seven_gen_t sg1 = kin_seven_gen_compute(1);
    kin_seven_gen_t sg2 = kin_seven_gen_compute(50);
    /* Different source kins should generally produce different guide kins */
    TEST_ASSERT_TRUE(sg1.levels[2].destiny.kin != sg2.levels[2].destiny.kin);
}

/* --- Seal/tone/color consistency with kin --- */

void test_cell_seal_matches_kin(void)
{
    kin_seven_gen_t sg = kin_seven_gen_compute(1);
    /* Kin 1: seal = (1-1) % 20 = 0, tone = (1-1) % 13 + 1 = 1, color = 0 % 4 = 0 */
    TEST_ASSERT_EQUAL_INT(0, sg.levels[0].destiny.seal);
    TEST_ASSERT_EQUAL_INT(1, sg.levels[0].destiny.tone);
    TEST_ASSERT_EQUAL_INT(0, sg.levels[0].destiny.color);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    kin_seven_gen_t a = kin_seven_gen_compute(77);
    kin_seven_gen_t b = kin_seven_gen_compute(77);
    TEST_ASSERT_EQUAL_INT(a.source_kin, b.source_kin);
    for (int lvl = 0; lvl < KIN_SEVEN_GEN_LEVELS; lvl++) {
        TEST_ASSERT_EQUAL_INT(a.levels[lvl].destiny.kin, b.levels[lvl].destiny.kin);
        TEST_ASSERT_EQUAL_INT(a.levels[lvl].guide.kin, b.levels[lvl].guide.kin);
        TEST_ASSERT_EQUAL_INT(a.levels[lvl].analog.kin, b.levels[lvl].analog.kin);
        TEST_ASSERT_EQUAL_INT(a.levels[lvl].antipode.kin, b.levels[lvl].antipode.kin);
        TEST_ASSERT_EQUAL_INT(a.levels[lvl].occult.kin, b.levels[lvl].occult.kin);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* source kin */
    RUN_TEST(test_source_kin_preserved);
    RUN_TEST(test_source_kin_preserved_kin_260);
    /* level 0 destiny */
    RUN_TEST(test_level0_destiny_is_input_kin);
    RUN_TEST(test_level0_destiny_kin44);
    /* level names */
    RUN_TEST(test_level_name_crown);
    RUN_TEST(test_level_name_heart);
    RUN_TEST(test_level_name_root);
    /* center positions */
    RUN_TEST(test_level0_center_position);
    RUN_TEST(test_level1_center_position);
    RUN_TEST(test_level2_center_position);
    /* valid cells */
    RUN_TEST(test_all_cells_valid_kin);
    RUN_TEST(test_all_cells_valid_seal_tone_color);
    /* level chain */
    RUN_TEST(test_level1_destiny_is_level0_analog);
    RUN_TEST(test_level1_destiny_is_level0_analog_kin130);
    RUN_TEST(test_level2_destiny_is_level0_guide);
    RUN_TEST(test_level2_destiny_is_level0_guide_kin200);
    /* invalid */
    RUN_TEST(test_invalid_kin_zero);
    RUN_TEST(test_invalid_kin_261);
    /* different kins */
    RUN_TEST(test_different_kins_different_level1_destiny);
    RUN_TEST(test_different_kins_different_level2_destiny);
    /* seal/tone/color */
    RUN_TEST(test_cell_seal_matches_kin);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
