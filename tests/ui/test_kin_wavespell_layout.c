#include "../unity/unity.h"
#include "../../src/ui/kin_wavespell_layout.h"

void setUp(void) {}
void tearDown(void) {}

/* --- kin_wavespell_compute: kin 1 (start of wavespell 1) --- */

void test_compute_kin_1_position_is_1(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    TEST_ASSERT_EQUAL_INT(1, ws.today_position);
}

void test_compute_kin_1_start_kin_is_1(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    TEST_ASSERT_EQUAL_INT(1, ws.start_kin);
}

void test_compute_kin_1_wavespell_number_is_1(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    TEST_ASSERT_EQUAL_INT(1, ws.wavespell_number);
}

void test_compute_kin_1_purpose_is_dragon(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    TEST_ASSERT_NOT_NULL(ws.purpose);
    TEST_ASSERT_EQUAL_STRING("Dragon", ws.purpose);
}

/* --- kin 13 (end of wavespell 1) --- */

void test_compute_kin_13_position_is_13(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(13);
    TEST_ASSERT_EQUAL_INT(13, ws.today_position);
    TEST_ASSERT_EQUAL_INT(1, ws.start_kin);
    TEST_ASSERT_EQUAL_INT(1, ws.wavespell_number);
}

/* --- kin 14 (start of wavespell 2) --- */

void test_compute_kin_14_position_is_1(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(14);
    TEST_ASSERT_EQUAL_INT(1, ws.today_position);
    TEST_ASSERT_EQUAL_INT(14, ws.start_kin);
    TEST_ASSERT_EQUAL_INT(2, ws.wavespell_number);
}

/* --- kin 125 (middle case, wavespell 10 position 8) --- */

void test_compute_kin_125_position_and_start(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(125);
    TEST_ASSERT_EQUAL_INT(8, ws.today_position);
    TEST_ASSERT_EQUAL_INT(118, ws.start_kin);
    TEST_ASSERT_EQUAL_INT(10, ws.wavespell_number);
}

/* --- All 13 cells have valid kin, seal, tone --- */

void test_compute_kin_1_all_cells_valid(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    for (int i = 0; i < KIN_WS_CELLS; i++) {
        TEST_ASSERT_TRUE(ws.cells[i].kin >= 1 && ws.cells[i].kin <= 260);
        TEST_ASSERT_TRUE(ws.cells[i].seal >= 0 && ws.cells[i].seal <= 19);
        TEST_ASSERT_TRUE(ws.cells[i].tone >= 1 && ws.cells[i].tone <= 13);
    }
}

/* --- Cell kins are consecutive --- */

void test_compute_cells_are_consecutive(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(40);
    for (int i = 0; i < KIN_WS_CELLS - 1; i++) {
        TEST_ASSERT_EQUAL_INT(ws.cells[i].kin + 1, ws.cells[i + 1].kin);
    }
}

/* --- today_position matches highlighted cell --- */

void test_compute_highlighted_matches_today_position(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(125);
    /* position 8 → cell[7] should be highlighted */
    TEST_ASSERT_EQUAL_INT(1, ws.cells[ws.today_position - 1].highlighted);
}

/* --- Only ONE cell is highlighted --- */

void test_compute_only_one_cell_highlighted(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(125);
    int count = 0;
    for (int i = 0; i < KIN_WS_CELLS; i++) {
        if (ws.cells[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Cell geometry: x positions --- */

void test_compute_cell_0_x_near_004(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.04f, ws.cells[0].x);
}

void test_compute_cell_12_x_near_096(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, ws.cells[12].x);
}

/* --- Cell geometry: y = 0.5 for all --- */

void test_compute_all_cells_y_05(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(200);
    for (int i = 0; i < KIN_WS_CELLS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, ws.cells[i].y);
    }
}

/* --- Cell geometry: w and h --- */

void test_compute_all_cells_size(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(42);
    for (int i = 0; i < KIN_WS_CELLS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.065f, ws.cells[i].w);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, ws.cells[i].h);
    }
}

/* --- wavespell_number matches dreamspell_wavespell --- */

void test_compute_wavespell_number_matches_dreamspell(void)
{
    /* kin 53: wavespell = (53-1)/13+1 = 52/13+1 = 4+1 = 5 */
    kin_wavespell_layout_t ws = kin_wavespell_compute(53);
    TEST_ASSERT_EQUAL_INT(5, ws.wavespell_number);
}

/* --- purpose is not NULL --- */

void test_compute_purpose_not_null(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(200);
    TEST_ASSERT_NOT_NULL(ws.purpose);
}

/* --- Invalid kin: 0 --- */

void test_compute_kin_0_returns_zeroed(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(0);
    TEST_ASSERT_EQUAL_INT(0, ws.today_position);
    TEST_ASSERT_EQUAL_INT(0, ws.wavespell_number);
    TEST_ASSERT_EQUAL_INT(0, ws.start_kin);
    TEST_ASSERT_NULL(ws.purpose);
}

/* --- Invalid kin: 261 --- */

void test_compute_kin_261_returns_zeroed(void)
{
    kin_wavespell_layout_t ws = kin_wavespell_compute(261);
    TEST_ASSERT_EQUAL_INT(0, ws.today_position);
    TEST_ASSERT_EQUAL_INT(0, ws.wavespell_number);
    TEST_ASSERT_EQUAL_INT(0, ws.start_kin);
    TEST_ASSERT_NULL(ws.purpose);
}

/* --- kin_wavespell_is_power_day --- */

void test_power_day_position_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_wavespell_is_power_day(1));
}

void test_power_day_position_5(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_wavespell_is_power_day(5));
}

void test_power_day_position_9(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_wavespell_is_power_day(9));
}

void test_power_day_position_13(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_wavespell_is_power_day(13));
}

void test_not_power_day_position_2(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_wavespell_is_power_day(2));
}

void test_not_power_day_position_6(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_wavespell_is_power_day(6));
}

void test_not_power_day_position_10(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_wavespell_is_power_day(10));
}

/* --- kin_wavespell_tone_name --- */

void test_tone_name_position_1_magnetic(void)
{
    TEST_ASSERT_EQUAL_STRING("Magnetic", kin_wavespell_tone_name(1));
}

void test_tone_name_position_13_cosmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmic", kin_wavespell_tone_name(13));
}

void test_tone_name_position_5_overtone(void)
{
    TEST_ASSERT_EQUAL_STRING("Overtone", kin_wavespell_tone_name(5));
}

void test_tone_name_invalid_returns_question(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kin_wavespell_tone_name(0));
    TEST_ASSERT_EQUAL_STRING("?", kin_wavespell_tone_name(14));
}

/* --- Purity: same input, same output --- */

void test_purity_same_input_same_output(void)
{
    kin_wavespell_layout_t a = kin_wavespell_compute(77);
    kin_wavespell_layout_t b = kin_wavespell_compute(77);
    TEST_ASSERT_EQUAL_INT(a.today_position, b.today_position);
    TEST_ASSERT_EQUAL_INT(a.wavespell_number, b.wavespell_number);
    TEST_ASSERT_EQUAL_INT(a.start_kin, b.start_kin);
    for (int i = 0; i < KIN_WS_CELLS; i++) {
        TEST_ASSERT_EQUAL_INT(a.cells[i].kin, b.cells[i].kin);
        TEST_ASSERT_EQUAL_INT(a.cells[i].highlighted, b.cells[i].highlighted);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* compute: kin 1 */
    RUN_TEST(test_compute_kin_1_position_is_1);
    RUN_TEST(test_compute_kin_1_start_kin_is_1);
    RUN_TEST(test_compute_kin_1_wavespell_number_is_1);
    RUN_TEST(test_compute_kin_1_purpose_is_dragon);
    /* compute: kin 13 */
    RUN_TEST(test_compute_kin_13_position_is_13);
    /* compute: kin 14 */
    RUN_TEST(test_compute_kin_14_position_is_1);
    /* compute: kin 125 (middle) */
    RUN_TEST(test_compute_kin_125_position_and_start);
    /* cell validity */
    RUN_TEST(test_compute_kin_1_all_cells_valid);
    RUN_TEST(test_compute_cells_are_consecutive);
    /* highlight */
    RUN_TEST(test_compute_highlighted_matches_today_position);
    RUN_TEST(test_compute_only_one_cell_highlighted);
    /* geometry */
    RUN_TEST(test_compute_cell_0_x_near_004);
    RUN_TEST(test_compute_cell_12_x_near_096);
    RUN_TEST(test_compute_all_cells_y_05);
    RUN_TEST(test_compute_all_cells_size);
    /* wavespell number cross-check */
    RUN_TEST(test_compute_wavespell_number_matches_dreamspell);
    /* purpose */
    RUN_TEST(test_compute_purpose_not_null);
    /* invalid */
    RUN_TEST(test_compute_kin_0_returns_zeroed);
    RUN_TEST(test_compute_kin_261_returns_zeroed);
    /* power days */
    RUN_TEST(test_power_day_position_1);
    RUN_TEST(test_power_day_position_5);
    RUN_TEST(test_power_day_position_9);
    RUN_TEST(test_power_day_position_13);
    RUN_TEST(test_not_power_day_position_2);
    RUN_TEST(test_not_power_day_position_6);
    RUN_TEST(test_not_power_day_position_10);
    /* tone names */
    RUN_TEST(test_tone_name_position_1_magnetic);
    RUN_TEST(test_tone_name_position_13_cosmic);
    RUN_TEST(test_tone_name_position_5_overtone);
    RUN_TEST(test_tone_name_invalid_returns_question);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
