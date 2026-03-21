#include "../unity/unity.h"
#include "../../src/ui/kin_sync_matrix.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Helper: a known valid JD (2026-08-01 = Moon 1 Day 7) --- */

static double valid_jd(void)
{
    return gregorian_to_jd(2026, 8, 1.5);
}

/* --- Grid structure --- */

void test_441_cells_total(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    int count = 0;
    for (int i = 0; i < KIN_SM_TOTAL; i++) {
        if (m.cells[i].row >= 1 && m.cells[i].row <= 21 &&
            m.cells[i].col >= 1 && m.cells[i].col <= 21)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(441, count);
}

void test_cell_1_1_position(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* Cell (1,1) at index 0 */
    TEST_ASSERT_EQUAL_INT(1, m.cells[0].row);
    TEST_ASSERT_EQUAL_INT(1, m.cells[0].col);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, m.cells[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, m.cells[0].y);
}

void test_cell_21_21_position(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* Cell (21,21) at index 440 */
    int idx = 20 * KIN_SM_SIZE + 20;
    TEST_ASSERT_EQUAL_INT(21, m.cells[idx].row);
    TEST_ASSERT_EQUAL_INT(21, m.cells[idx].col);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, m.cells[idx].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, m.cells[idx].y);
}

void test_center_cell_is_center(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* Center is (11,11) -> index (10*21+10) = 220 */
    int idx = 10 * KIN_SM_SIZE + 10;
    TEST_ASSERT_EQUAL_INT(11, m.cells[idx].row);
    TEST_ASSERT_EQUAL_INT(11, m.cells[idx].col);
    TEST_ASSERT_EQUAL_INT(1, m.cells[idx].is_center);
    TEST_ASSERT_EQUAL_INT(11, m.cells[idx].circuit);
}

void test_circuits_range_1_to_11(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    for (int i = 0; i < KIN_SM_TOTAL; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(1, m.cells[i].circuit);
        TEST_ASSERT_LESS_OR_EQUAL(11, m.cells[i].circuit);
    }
}

void test_tubo_mauri_col_11_is_axis(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* All cells in column 11 should have is_axis=1 */
    for (int row = 1; row <= KIN_SM_SIZE; row++) {
        int idx = (row - 1) * KIN_SM_SIZE + 10; /* col=11 -> index offset 10 */
        TEST_ASSERT_EQUAL_INT(1, m.cells[idx].is_axis);
    }
}

void test_equatorial_row_11_is_axis(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* All cells in row 11 should have is_axis=1 */
    for (int col = 1; col <= KIN_SM_SIZE; col++) {
        int idx = 10 * KIN_SM_SIZE + (col - 1); /* row=11 -> row-1=10 */
        TEST_ASSERT_EQUAL_INT(1, m.cells[idx].is_axis);
    }
}

void test_non_axis_cell_is_axis_zero(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* Cell (1,1) is not on any axis */
    TEST_ASSERT_EQUAL_INT(0, m.cells[0].is_axis);
    /* Cell (5,5) is not on any axis */
    int idx = 4 * KIN_SM_SIZE + 4;
    TEST_ASSERT_EQUAL_INT(0, m.cells[idx].is_axis);
}

/* --- Markers --- */

void test_valid_jd_produces_markers(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    TEST_ASSERT_GREATER_OR_EQUAL(1, m.marker_count);
    TEST_ASSERT_LESS_OR_EQUAL(KIN_SM_MAX_ACTIVE, m.marker_count);
}

void test_markers_have_valid_row_col(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    for (int i = 0; i < m.marker_count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(1, m.markers[i].row);
        TEST_ASSERT_LESS_OR_EQUAL(21, m.markers[i].row);
        TEST_ASSERT_GREATER_OR_EQUAL(1, m.markers[i].col);
        TEST_ASSERT_LESS_OR_EQUAL(21, m.markers[i].col);
    }
}

void test_markers_have_labels(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    for (int i = 0; i < m.marker_count; i++) {
        TEST_ASSERT_NOT_NULL(m.markers[i].label);
    }
}

void test_active_cells_marked(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* For each marker, the corresponding cell must be active */
    for (int i = 0; i < m.marker_count; i++) {
        int idx = (m.markers[i].row - 1) * KIN_SM_SIZE + (m.markers[i].col - 1);
        TEST_ASSERT_EQUAL_INT(1, m.cells[idx].active);
    }
}

void test_circuit_count_always_11(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    TEST_ASSERT_EQUAL_INT(11, m.circuit_count);
}

/* --- Circuit distribution --- */

void test_circuit_1_outermost_has_most_cells(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    int circuit_1_count = 0;
    int circuit_2_count = 0;
    for (int i = 0; i < KIN_SM_TOTAL; i++) {
        if (m.cells[i].circuit == 1) circuit_1_count++;
        if (m.cells[i].circuit == 2) circuit_2_count++;
    }
    TEST_ASSERT_GREATER_THAN(circuit_2_count, circuit_1_count);
}

void test_circuit_11_center_has_1_cell(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    int count = 0;
    for (int i = 0; i < KIN_SM_TOTAL; i++) {
        if (m.cells[i].circuit == 11) count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Invalid JD --- */

void test_invalid_jd_negative_returns_no_markers(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(0, m.marker_count);
}

/* --- Center marker always present --- */

void test_center_marker_present(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    int found = 0;
    for (int i = 0; i < m.marker_count; i++) {
        if (m.markers[i].row == 21 && m.markers[i].col == 21 &&
            m.markers[i].number == 441) {
            found = 1;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, found);
}

/* --- Cell positions normalized --- */

void test_all_cell_positions_normalized(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    for (int i = 0; i < KIN_SM_TOTAL; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, m.cells[i].x);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, m.cells[i].x);
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, m.cells[i].y);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, m.cells[i].y);
    }
}

/* --- Markers have valid numbers --- */

void test_marker_numbers_in_range(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    for (int i = 0; i < m.marker_count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(1, m.markers[i].number);
        TEST_ASSERT_LESS_OR_EQUAL(441, m.markers[i].number);
    }
}

/* --- Non-center, non-axis cells not marked center --- */

void test_non_center_cells_not_center(void)
{
    kin_sync_matrix_t m = kin_sync_matrix_compute(valid_jd());
    /* Cell (1,1) should not be center */
    TEST_ASSERT_EQUAL_INT(0, m.cells[0].is_center);
    /* Cell (1,21) should not be center */
    TEST_ASSERT_EQUAL_INT(0, m.cells[20].is_center);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_441_cells_total);
    RUN_TEST(test_cell_1_1_position);
    RUN_TEST(test_cell_21_21_position);
    RUN_TEST(test_center_cell_is_center);
    RUN_TEST(test_circuits_range_1_to_11);
    RUN_TEST(test_tubo_mauri_col_11_is_axis);
    RUN_TEST(test_equatorial_row_11_is_axis);
    RUN_TEST(test_non_axis_cell_is_axis_zero);
    RUN_TEST(test_valid_jd_produces_markers);
    RUN_TEST(test_markers_have_valid_row_col);
    RUN_TEST(test_markers_have_labels);
    RUN_TEST(test_active_cells_marked);
    RUN_TEST(test_circuit_count_always_11);
    RUN_TEST(test_circuit_1_outermost_has_most_cells);
    RUN_TEST(test_circuit_11_center_has_1_cell);
    RUN_TEST(test_invalid_jd_negative_returns_no_markers);
    RUN_TEST(test_center_marker_present);
    RUN_TEST(test_all_cell_positions_normalized);
    RUN_TEST(test_marker_numbers_in_range);
    RUN_TEST(test_non_center_cells_not_center);
    return UNITY_END();
}
