#include "../unity/unity.h"
#include "../../src/ui/kin_base_matrix.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Grid structure --- */

void test_441_cells_total(void)
{
    /* KIN_BM_TOTAL must equal 441. */
    TEST_ASSERT_EQUAL_INT(441, KIN_BM_TOTAL);
}

void test_cell_numbers_sequential(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    for (int i = 0; i < KIN_BM_TOTAL; i++) {
        TEST_ASSERT_EQUAL_INT(i, m.cells[i].number);
    }
}

void test_cell_0_position(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, m.cells[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, m.cells[0].y);
}

void test_cell_440_position(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    /* row=20, col=20 -> x=20/21, y=20/21 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 20.0f / 21.0f, m.cells[440].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 20.0f / 21.0f, m.cells[440].y);
}

void test_center_cell_220_is_center(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, m.cells[220].is_center);
}

void test_no_other_cell_is_center(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    for (int i = 0; i < KIN_BM_TOTAL; i++) {
        if (i == 220) continue;
        TEST_ASSERT_EQUAL_INT(0, m.cells[i].is_center);
    }
}

void test_center_number_is_220(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    TEST_ASSERT_EQUAL_INT(220, m.center_number);
}

/* --- kin_bm_number_to_pos --- */

void test_number_to_pos_zero(void)
{
    int row, col;
    kin_bm_number_to_pos(0, &row, &col);
    TEST_ASSERT_EQUAL_INT(0, row);
    TEST_ASSERT_EQUAL_INT(0, col);
}

void test_number_to_pos_220(void)
{
    int row, col;
    kin_bm_number_to_pos(220, &row, &col);
    TEST_ASSERT_EQUAL_INT(10, row);
    TEST_ASSERT_EQUAL_INT(10, col);
}

void test_number_to_pos_440(void)
{
    int row, col;
    kin_bm_number_to_pos(440, &row, &col);
    TEST_ASSERT_EQUAL_INT(20, row);
    TEST_ASSERT_EQUAL_INT(20, col);
}

void test_number_to_pos_negative(void)
{
    int row, col;
    kin_bm_number_to_pos(-1, &row, &col);
    TEST_ASSERT_EQUAL_INT(-1, row);
    TEST_ASSERT_EQUAL_INT(-1, col);
}

void test_number_to_pos_441(void)
{
    int row, col;
    kin_bm_number_to_pos(441, &row, &col);
    TEST_ASSERT_EQUAL_INT(-1, row);
    TEST_ASSERT_EQUAL_INT(-1, col);
}

/* --- kin_bm_pos_to_number --- */

void test_pos_to_number_0_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_bm_pos_to_number(0, 0));
}

void test_pos_to_number_10_10(void)
{
    TEST_ASSERT_EQUAL_INT(220, kin_bm_pos_to_number(10, 10));
}

void test_pos_to_number_20_20(void)
{
    TEST_ASSERT_EQUAL_INT(440, kin_bm_pos_to_number(20, 20));
}

void test_pos_to_number_invalid_row(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_bm_pos_to_number(21, 0));
}

void test_pos_to_number_invalid_col(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_bm_pos_to_number(0, 21));
}

void test_pos_to_number_negative_row(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_bm_pos_to_number(-1, 0));
}

/* --- Active positions for a known date --- */

/* Feb 14, 2013 = Kin 2: White Lunar Wind (seal 1, tone 2).
 * 13-Moon: day 203 of year -> moon 8, day 8, heptad 2, day_of_week 1.
 * sync_seal_hk(2) = 144 (Wind), sync_heptad_gate_hk(1) = 108 (Dali),
 * sync_tonal_value(2) = 2*144 = 288.
 * Three distinct active positions: 144, 108, 288. */

void test_known_date_active_count(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    TEST_ASSERT_EQUAL_INT(3, m.active_count);
}

void test_known_date_active_numbers_valid(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    for (int i = 0; i < m.active_count; i++) {
        TEST_ASSERT_TRUE(m.active[i].number >= 0);
        TEST_ASSERT_TRUE(m.active[i].number <= 440);
    }
}

void test_known_date_active_labels_not_null(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    for (int i = 0; i < m.active_count; i++) {
        TEST_ASSERT_NOT_NULL(m.active[i].label);
    }
}

void test_known_date_destiny_at_144(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    /* Destiny (seal Wind, code 2) -> HK 144 */
    TEST_ASSERT_EQUAL_INT(144, m.active[0].number);
    TEST_ASSERT_EQUAL_STRING("Destiny", m.active[0].label);
}

void test_known_date_gate_at_108(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    /* Heptad Gate (day 1/Dali) -> HK 108 */
    TEST_ASSERT_EQUAL_INT(108, m.active[1].number);
    TEST_ASSERT_EQUAL_STRING("Heptad Gate", m.active[1].label);
}

void test_known_date_tonal_at_288(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    /* Tonal Value: tone 2 * lens 144 = 288 */
    TEST_ASSERT_EQUAL_INT(288, m.active[2].number);
    TEST_ASSERT_EQUAL_STRING("Tonal Value", m.active[2].label);
}

void test_known_date_active_cells_marked(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, m.cells[144].active);
    TEST_ASSERT_EQUAL_INT(1, m.cells[108].active);
    TEST_ASSERT_EQUAL_INT(1, m.cells[288].active);
}

/* --- Duplicate suppression --- */

/* Dec 21, 2012 = Kin 207: Blue Crystal Hand (seal 6, tone 12).
 * 13-Moon day_of_week = 2 -> gate_hk = 291.
 * seal_code = 7 (Monkey) -> SEAL_HK[6] = 291 (same as gate!).
 * tonal = 12 * 288 = 3456 (out of range).
 * Only 1 active position (291). */

void test_duplicate_suppression(void)
{
    double jd = gregorian_to_jd(2012, 12, 21.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, m.active_count);
    TEST_ASSERT_EQUAL_INT(291, m.active[0].number);
}

/* --- Invalid JD --- */

void test_invalid_jd_negative(void)
{
    kin_base_matrix_t m = kin_base_matrix_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, m.active_count);
}

void test_invalid_jd_grid_still_initialized(void)
{
    kin_base_matrix_t m = kin_base_matrix_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, m.cells[0].number);
    TEST_ASSERT_EQUAL_INT(440, m.cells[440].number);
    TEST_ASSERT_EQUAL_INT(1, m.cells[220].is_center);
    TEST_ASSERT_EQUAL_INT(220, m.center_number);
}

/* --- Active row/col consistency --- */

void test_active_row_col_match_number(void)
{
    double jd = gregorian_to_jd(2013, 2, 14.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    for (int i = 0; i < m.active_count; i++) {
        int expected_row = m.active[i].number / 21;
        int expected_col = m.active[i].number % 21;
        TEST_ASSERT_EQUAL_INT(expected_row, m.active[i].row);
        TEST_ASSERT_EQUAL_INT(expected_col, m.active[i].col);
    }
}

/* --- Tonal value out of range --- */

void test_high_tone_no_tonal_active(void)
{
    /* Kin 207 (tone 12): tonal = 12*288 = 3456, out of 0-440 range.
     * Tonal value should NOT appear in active list. */
    double jd = gregorian_to_jd(2012, 12, 21.0);
    kin_base_matrix_t m = kin_base_matrix_compute(jd);
    for (int i = 0; i < m.active_count; i++) {
        /* No "Tonal Value" label should appear since it's out of range. */
        TEST_ASSERT_FALSE(m.active[i].number == 3456);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Grid structure */
    RUN_TEST(test_441_cells_total);
    RUN_TEST(test_cell_numbers_sequential);
    RUN_TEST(test_cell_0_position);
    RUN_TEST(test_cell_440_position);
    RUN_TEST(test_center_cell_220_is_center);
    RUN_TEST(test_no_other_cell_is_center);
    RUN_TEST(test_center_number_is_220);

    /* number_to_pos */
    RUN_TEST(test_number_to_pos_zero);
    RUN_TEST(test_number_to_pos_220);
    RUN_TEST(test_number_to_pos_440);
    RUN_TEST(test_number_to_pos_negative);
    RUN_TEST(test_number_to_pos_441);

    /* pos_to_number */
    RUN_TEST(test_pos_to_number_0_0);
    RUN_TEST(test_pos_to_number_10_10);
    RUN_TEST(test_pos_to_number_20_20);
    RUN_TEST(test_pos_to_number_invalid_row);
    RUN_TEST(test_pos_to_number_invalid_col);
    RUN_TEST(test_pos_to_number_negative_row);

    /* Active positions (known date) */
    RUN_TEST(test_known_date_active_count);
    RUN_TEST(test_known_date_active_numbers_valid);
    RUN_TEST(test_known_date_active_labels_not_null);
    RUN_TEST(test_known_date_destiny_at_144);
    RUN_TEST(test_known_date_gate_at_108);
    RUN_TEST(test_known_date_tonal_at_288);
    RUN_TEST(test_known_date_active_cells_marked);

    /* Duplicate suppression */
    RUN_TEST(test_duplicate_suppression);

    /* Invalid JD */
    RUN_TEST(test_invalid_jd_negative);
    RUN_TEST(test_invalid_jd_grid_still_initialized);

    /* Row/col consistency */
    RUN_TEST(test_active_row_col_match_number);

    /* Out-of-range tonal */
    RUN_TEST(test_high_tone_no_tonal_active);

    return UNITY_END();
}
