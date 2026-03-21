#include "../unity/unity.h"
#include "../../src/ui/kin_board_layout.h"
#include <stdbool.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Board structure --- */

void test_board_has_260_cells(void)
{
    kin_board_layout_t b = kin_board_compute(1);
    /* All 260 cells should have valid kin */
    int count = 0;
    for (int i = 0; i < KIN_BOARD_TOTAL; i++) {
        if (b.cells[i].kin >= 1 && b.cells[i].kin <= 260)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(260, count);
}

void test_kin1_at_row0_col0(void)
{
    kin_board_layout_t b = kin_board_compute(1);
    /* Kin 1 = seal 0 (Dragon), tone 1 → row 0, col 0 */
    TEST_ASSERT_EQUAL_INT(1, b.cells[0].kin);
}

void test_kin260_at_row19_col12(void)
{
    kin_board_layout_t b = kin_board_compute(260);
    /* Kin 260 = seal 19 (Sun), tone 13 → row 19, col 12 */
    int idx = 19 * KIN_BOARD_COLS + 12;
    TEST_ASSERT_EQUAL_INT(260, b.cells[idx].kin);
}

/* --- Highlight --- */

void test_today_kin1_highlighted(void)
{
    kin_board_layout_t b = kin_board_compute(1);
    TEST_ASSERT_EQUAL_INT(1, b.cells[0].highlighted);
}

void test_today_kin1_others_not_highlighted(void)
{
    kin_board_layout_t b = kin_board_compute(1);
    /* Check a few non-today cells */
    TEST_ASSERT_EQUAL_INT(0, b.cells[1].highlighted);
    TEST_ASSERT_EQUAL_INT(0, b.cells[13].highlighted);
    TEST_ASSERT_EQUAL_INT(0, b.cells[259].highlighted);
}

void test_today_kin0_no_highlight(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    for (int i = 0; i < KIN_BOARD_TOTAL; i++) {
        TEST_ASSERT_EQUAL_INT(0, b.cells[i].highlighted);
    }
}

/* --- All cells valid --- */

void test_all_cells_valid_kin(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    for (int i = 0; i < KIN_BOARD_TOTAL; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(1, b.cells[i].kin);
        TEST_ASSERT_LESS_OR_EQUAL(260, b.cells[i].kin);
    }
}

void test_all_cells_valid_seal_and_tone(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    for (int i = 0; i < KIN_BOARD_TOTAL; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(0, b.cells[i].seal);
        TEST_ASSERT_LESS_OR_EQUAL(19, b.cells[i].seal);
        TEST_ASSERT_GREATER_OR_EQUAL(1, b.cells[i].tone);
        TEST_ASSERT_LESS_OR_EQUAL(13, b.cells[i].tone);
    }
}

/* --- No duplicate kins --- */

void test_no_duplicate_kins(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    bool seen[261] = {false};
    for (int i = 0; i < KIN_BOARD_TOTAL; i++) {
        int k = b.cells[i].kin;
        TEST_ASSERT_FALSE(seen[k]);
        seen[k] = true;
    }
}

/* --- Cell positions --- */

void test_cell_position_origin(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    /* Cell [0][0] at (0, 0) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, b.cells[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, b.cells[0].y);
}

void test_cell_position_last(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    /* Cell [19][12] near (12/13, 19/20) */
    int idx = 19 * KIN_BOARD_COLS + 12;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 12.0f / 13.0f, b.cells[idx].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 19.0f / 20.0f, b.cells[idx].y);
}

void test_cell_dimensions(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f / 13.0f, b.cells[0].w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f / 20.0f, b.cells[0].h);
}

/* --- Portal count --- */

void test_portal_count_positive(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    TEST_ASSERT_GREATER_THAN(0, b.portal_count);
}

void test_portal_count_is_52(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    TEST_ASSERT_EQUAL_INT(52, b.portal_count);
}

/* --- kin_board_kin_at --- */

void test_kin_at_0_0_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_board_kin_at(0, 0));
}

void test_kin_at_19_12_is_260(void)
{
    TEST_ASSERT_EQUAL_INT(260, kin_board_kin_at(19, 12));
}

void test_kin_at_invalid_row_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_board_kin_at(-1, 0));
    TEST_ASSERT_EQUAL_INT(0, kin_board_kin_at(20, 0));
}

void test_kin_at_invalid_col_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_board_kin_at(0, -1));
    TEST_ASSERT_EQUAL_INT(0, kin_board_kin_at(0, 13));
}

/* --- kin_board_cell_index --- */

void test_cell_index_kin1_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_board_cell_index(1));
}

void test_cell_index_kin260_is_259(void)
{
    /* Kin 260: seal=19, tone=13 → index = 19*13 + 12 = 259 */
    TEST_ASSERT_EQUAL_INT(259, kin_board_cell_index(260));
}

void test_cell_index_kin0_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_board_cell_index(0));
}

void test_cell_index_kin261_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_board_cell_index(261));
}

/* --- Invalid today_kin --- */

void test_invalid_today_kin_261_returns_zeroed(void)
{
    kin_board_layout_t b = kin_board_compute(261);
    TEST_ASSERT_EQUAL_INT(0, b.today_kin);
    TEST_ASSERT_EQUAL_INT(0, b.portal_count);
    /* Cells should all be zeroed */
    TEST_ASSERT_EQUAL_INT(0, b.cells[0].kin);
}

void test_invalid_today_kin_negative_returns_zeroed(void)
{
    kin_board_layout_t b = kin_board_compute(-1);
    TEST_ASSERT_EQUAL_INT(0, b.today_kin);
    TEST_ASSERT_EQUAL_INT(0, b.cells[0].kin);
}

/* --- Today row/col --- */

void test_today_row_col_for_kin1(void)
{
    kin_board_layout_t b = kin_board_compute(1);
    TEST_ASSERT_EQUAL_INT(0, b.today_row);
    TEST_ASSERT_EQUAL_INT(0, b.today_col);
}

void test_today_row_col_for_kin260(void)
{
    kin_board_layout_t b = kin_board_compute(260);
    TEST_ASSERT_EQUAL_INT(19, b.today_row);
    TEST_ASSERT_EQUAL_INT(12, b.today_col);
}

/* --- Purity --- */

void test_purity_deterministic(void)
{
    kin_board_layout_t a = kin_board_compute(42);
    kin_board_layout_t b = kin_board_compute(42);
    TEST_ASSERT_EQUAL_INT(a.today_kin, b.today_kin);
    TEST_ASSERT_EQUAL_INT(a.portal_count, b.portal_count);
    TEST_ASSERT_EQUAL_INT(a.cells[0].kin, b.cells[0].kin);
    TEST_ASSERT_EQUAL_INT(a.cells[100].kin, b.cells[100].kin);
}

/* --- Cell index consistency with board --- */

void test_cell_index_matches_board(void)
{
    kin_board_layout_t b = kin_board_compute(0);
    /* For each cell, verify kin_board_cell_index returns that index */
    for (int i = 0; i < KIN_BOARD_TOTAL; i++) {
        int kin = b.cells[i].kin;
        int idx = kin_board_cell_index(kin);
        TEST_ASSERT_EQUAL_INT(i, idx);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* board structure */
    RUN_TEST(test_board_has_260_cells);
    RUN_TEST(test_kin1_at_row0_col0);
    RUN_TEST(test_kin260_at_row19_col12);
    /* highlight */
    RUN_TEST(test_today_kin1_highlighted);
    RUN_TEST(test_today_kin1_others_not_highlighted);
    RUN_TEST(test_today_kin0_no_highlight);
    /* all cells valid */
    RUN_TEST(test_all_cells_valid_kin);
    RUN_TEST(test_all_cells_valid_seal_and_tone);
    /* no duplicates */
    RUN_TEST(test_no_duplicate_kins);
    /* cell positions */
    RUN_TEST(test_cell_position_origin);
    RUN_TEST(test_cell_position_last);
    RUN_TEST(test_cell_dimensions);
    /* portal count */
    RUN_TEST(test_portal_count_positive);
    RUN_TEST(test_portal_count_is_52);
    /* kin_at */
    RUN_TEST(test_kin_at_0_0_is_1);
    RUN_TEST(test_kin_at_19_12_is_260);
    RUN_TEST(test_kin_at_invalid_row_returns_0);
    RUN_TEST(test_kin_at_invalid_col_returns_0);
    /* cell_index */
    RUN_TEST(test_cell_index_kin1_is_0);
    RUN_TEST(test_cell_index_kin260_is_259);
    RUN_TEST(test_cell_index_kin0_invalid);
    RUN_TEST(test_cell_index_kin261_invalid);
    /* invalid today_kin */
    RUN_TEST(test_invalid_today_kin_261_returns_zeroed);
    RUN_TEST(test_invalid_today_kin_negative_returns_zeroed);
    /* today row/col */
    RUN_TEST(test_today_row_col_for_kin1);
    RUN_TEST(test_today_row_col_for_kin260);
    /* purity */
    RUN_TEST(test_purity_deterministic);
    /* consistency */
    RUN_TEST(test_cell_index_matches_board);
    return UNITY_END();
}
