#include "../unity/unity.h"
#include "../../src/ui/kin_oracle_layout.h"

void setUp(void) {}
void tearDown(void) {}

/* --- kin_oracle_compute: kin 1 (Red Magnetic Dragon) --- */

void test_compute_kin_1_destiny(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, layout.destiny.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.destiny.seal);   /* Dragon */
    TEST_ASSERT_EQUAL_INT(1, layout.destiny.tone);    /* Magnetic */
    TEST_ASSERT_EQUAL_INT(0, layout.destiny.color);   /* Red */
}

void test_compute_kin_1_guide(void)
{
    /* Tone 1 → self-guiding → guide kin = 1 */
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, layout.guide.kin);
}

void test_compute_kin_1_analog(void)
{
    /* analog_seal = (19-0)%20 = 19 (Sun), tone 1 → kin 40 */
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(40, layout.analog.kin);
}

void test_compute_kin_1_antipode(void)
{
    /* antipode_seal = (0+10)%20 = 10 (Monkey), tone 1 → kin 131 */
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(131, layout.antipode.kin);
}

void test_compute_kin_1_occult(void)
{
    /* occult = 261 - 1 = 260 */
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(260, layout.occult.kin);
}

/* --- Positions --- */

void test_destiny_position_center(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.destiny.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.destiny.y);
}

void test_guide_position_top(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.guide.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, layout.guide.y);
}

void test_analog_position_right(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.85f, layout.analog.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.analog.y);
}

void test_antipode_position_left(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.15f, layout.antipode.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.antipode.y);
}

void test_occult_position_bottom(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.occult.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.85f, layout.occult.y);
}

/* --- Cell dimensions --- */

void test_all_cells_have_standard_size(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cells[5] = {
        layout.destiny, layout.guide, layout.analog,
        layout.antipode, layout.occult
    };
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, cells[i].w);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.20f, cells[i].h);
    }
}

/* --- Highlighted --- */

void test_destiny_is_highlighted(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, layout.destiny.highlighted);
}

void test_others_not_highlighted(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(0, layout.guide.highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.analog.highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.antipode.highlighted);
    TEST_ASSERT_EQUAL_INT(0, layout.occult.highlighted);
}

/* --- Hidden kin --- */

void test_hidden_kin(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    TEST_ASSERT_EQUAL_INT(260, layout.hidden_kin);
}

void test_hidden_kin_130(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(130);
    TEST_ASSERT_EQUAL_INT(131, layout.hidden_kin);
}

/* --- Valid seal and tone for all oracle cells --- */

void test_all_cells_valid_seal_and_tone(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(42);
    kin_cell_t cells[5] = {
        layout.destiny, layout.guide, layout.analog,
        layout.antipode, layout.occult
    };
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_TRUE(cells[i].kin >= 1);
        TEST_ASSERT_TRUE(cells[i].kin <= 260);
        TEST_ASSERT_TRUE(cells[i].seal >= 0);
        TEST_ASSERT_TRUE(cells[i].seal <= 19);
        TEST_ASSERT_TRUE(cells[i].tone >= 1);
        TEST_ASSERT_TRUE(cells[i].tone <= 13);
        TEST_ASSERT_TRUE(cells[i].color >= 0);
        TEST_ASSERT_TRUE(cells[i].color <= 3);
    }
}

/* --- Invalid kin --- */

void test_invalid_kin_zero_returns_zeroed(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(0);
    TEST_ASSERT_EQUAL_INT(0, layout.destiny.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.guide.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.analog.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.antipode.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.occult.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.hidden_kin);
}

void test_invalid_kin_261_returns_zeroed(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(261);
    TEST_ASSERT_EQUAL_INT(0, layout.destiny.kin);
    TEST_ASSERT_EQUAL_INT(0, layout.hidden_kin);
}

/* --- kin_oracle_position_name --- */

void test_position_name_destiny(void)
{
    TEST_ASSERT_EQUAL_STRING("Destiny", kin_oracle_position_name(0));
}

void test_position_name_guide(void)
{
    TEST_ASSERT_EQUAL_STRING("Guide", kin_oracle_position_name(1));
}

void test_position_name_analog(void)
{
    TEST_ASSERT_EQUAL_STRING("Analog", kin_oracle_position_name(2));
}

void test_position_name_antipode(void)
{
    TEST_ASSERT_EQUAL_STRING("Antipode", kin_oracle_position_name(3));
}

void test_position_name_occult(void)
{
    TEST_ASSERT_EQUAL_STRING("Occult", kin_oracle_position_name(4));
}

void test_position_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kin_oracle_position_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", kin_oracle_position_name(5));
}

/* --- kin_oracle_cell_at --- */

void test_cell_at_destiny(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cell = kin_oracle_cell_at(&layout, 0);
    TEST_ASSERT_EQUAL_INT(1, cell.kin);
    TEST_ASSERT_EQUAL_INT(1, cell.highlighted);
}

void test_cell_at_guide(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cell = kin_oracle_cell_at(&layout, 1);
    TEST_ASSERT_EQUAL_INT(1, cell.kin);  /* self-guiding */
}

void test_cell_at_analog(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cell = kin_oracle_cell_at(&layout, 2);
    TEST_ASSERT_EQUAL_INT(40, cell.kin);
}

void test_cell_at_antipode(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cell = kin_oracle_cell_at(&layout, 3);
    TEST_ASSERT_EQUAL_INT(131, cell.kin);
}

void test_cell_at_occult(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cell = kin_oracle_cell_at(&layout, 4);
    TEST_ASSERT_EQUAL_INT(260, cell.kin);
}

void test_cell_at_invalid_index(void)
{
    kin_oracle_layout_t layout = kin_oracle_compute(1);
    kin_cell_t cell = kin_oracle_cell_at(&layout, 5);
    TEST_ASSERT_EQUAL_INT(0, cell.kin);
}

void test_cell_at_null_layout(void)
{
    kin_cell_t cell = kin_oracle_cell_at(NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, cell.kin);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    kin_oracle_layout_t a = kin_oracle_compute(207);
    kin_oracle_layout_t b = kin_oracle_compute(207);
    TEST_ASSERT_EQUAL_INT(a.destiny.kin, b.destiny.kin);
    TEST_ASSERT_EQUAL_INT(a.guide.kin, b.guide.kin);
    TEST_ASSERT_EQUAL_INT(a.analog.kin, b.analog.kin);
    TEST_ASSERT_EQUAL_INT(a.antipode.kin, b.antipode.kin);
    TEST_ASSERT_EQUAL_INT(a.occult.kin, b.occult.kin);
    TEST_ASSERT_EQUAL_INT(a.hidden_kin, b.hidden_kin);
}

int main(void)
{
    UNITY_BEGIN();
    /* compute: kin 1 oracle members */
    RUN_TEST(test_compute_kin_1_destiny);
    RUN_TEST(test_compute_kin_1_guide);
    RUN_TEST(test_compute_kin_1_analog);
    RUN_TEST(test_compute_kin_1_antipode);
    RUN_TEST(test_compute_kin_1_occult);
    /* positions */
    RUN_TEST(test_destiny_position_center);
    RUN_TEST(test_guide_position_top);
    RUN_TEST(test_analog_position_right);
    RUN_TEST(test_antipode_position_left);
    RUN_TEST(test_occult_position_bottom);
    /* cell dimensions */
    RUN_TEST(test_all_cells_have_standard_size);
    /* highlighted */
    RUN_TEST(test_destiny_is_highlighted);
    RUN_TEST(test_others_not_highlighted);
    /* hidden kin */
    RUN_TEST(test_hidden_kin);
    RUN_TEST(test_hidden_kin_130);
    /* valid seal/tone */
    RUN_TEST(test_all_cells_valid_seal_and_tone);
    /* invalid kin */
    RUN_TEST(test_invalid_kin_zero_returns_zeroed);
    RUN_TEST(test_invalid_kin_261_returns_zeroed);
    /* position names */
    RUN_TEST(test_position_name_destiny);
    RUN_TEST(test_position_name_guide);
    RUN_TEST(test_position_name_analog);
    RUN_TEST(test_position_name_antipode);
    RUN_TEST(test_position_name_occult);
    RUN_TEST(test_position_name_invalid);
    /* cell_at */
    RUN_TEST(test_cell_at_destiny);
    RUN_TEST(test_cell_at_guide);
    RUN_TEST(test_cell_at_analog);
    RUN_TEST(test_cell_at_antipode);
    RUN_TEST(test_cell_at_occult);
    RUN_TEST(test_cell_at_invalid_index);
    RUN_TEST(test_cell_at_null_layout);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
