#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/matrix_441.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Constants
 * ================================================================ */

static void test_size_is_21(void)
{
    TEST_ASSERT_EQUAL_INT(21, M441_SIZE);
}

static void test_center_is_11(void)
{
    TEST_ASSERT_EQUAL_INT(11, M441_CENTER);
}

static void test_total_is_441(void)
{
    TEST_ASSERT_EQUAL_INT(441, M441_TOTAL);
}

static void test_circuit_count_constant_is_11(void)
{
    TEST_ASSERT_EQUAL_INT(11, M441_CIRCUIT_COUNT);
}

static void test_441_equals_21_squared(void)
{
    TEST_ASSERT_EQUAL_INT(441, 21 * 21);
}

/* ================================================================
 * m441_circuit_count
 * ================================================================ */

static void test_circuit_count_returns_11(void)
{
    TEST_ASSERT_EQUAL_INT(11, m441_circuit_count());
}

/* ================================================================
 * m441_total_units
 * ================================================================ */

static void test_total_units_returns_441(void)
{
    TEST_ASSERT_EQUAL_INT(441, m441_total_units());
}

/* ================================================================
 * m441_dimensions_of_time
 * ================================================================ */

static void test_dimensions_of_time_returns_9(void)
{
    TEST_ASSERT_EQUAL_INT(9, m441_dimensions_of_time());
}

static void test_9_times_49_equals_441(void)
{
    TEST_ASSERT_EQUAL_INT(441, 9 * 49);
}

/* ================================================================
 * m441_circuit_get — all 11 circuits
 * ================================================================ */

static void test_circuit_1_mercury(void)
{
    m441_circuit_t c = m441_circuit_get(1);
    TEST_ASSERT_EQUAL_INT(1, c.number);
    TEST_ASSERT_EQUAL_STRING("Mercury", c.planet);
    TEST_ASSERT_EQUAL_INT(80, c.units);
}

static void test_circuit_2_venus(void)
{
    m441_circuit_t c = m441_circuit_get(2);
    TEST_ASSERT_EQUAL_INT(2, c.number);
    TEST_ASSERT_EQUAL_STRING("Venus", c.planet);
    TEST_ASSERT_EQUAL_INT(72, c.units);
}

static void test_circuit_3_earth(void)
{
    m441_circuit_t c = m441_circuit_get(3);
    TEST_ASSERT_EQUAL_INT(3, c.number);
    TEST_ASSERT_EQUAL_STRING("Earth", c.planet);
    TEST_ASSERT_EQUAL_INT(64, c.units);
}

static void test_circuit_4_mars(void)
{
    m441_circuit_t c = m441_circuit_get(4);
    TEST_ASSERT_EQUAL_INT(4, c.number);
    TEST_ASSERT_EQUAL_STRING("Mars", c.planet);
    TEST_ASSERT_EQUAL_INT(56, c.units);
}

static void test_circuit_5_maldek(void)
{
    m441_circuit_t c = m441_circuit_get(5);
    TEST_ASSERT_EQUAL_INT(5, c.number);
    TEST_ASSERT_EQUAL_STRING("Maldek", c.planet);
    TEST_ASSERT_EQUAL_INT(48, c.units);
}

static void test_circuit_6_jupiter(void)
{
    m441_circuit_t c = m441_circuit_get(6);
    TEST_ASSERT_EQUAL_INT(6, c.number);
    TEST_ASSERT_EQUAL_STRING("Jupiter", c.planet);
    TEST_ASSERT_EQUAL_INT(40, c.units);
}

static void test_circuit_7_saturn(void)
{
    m441_circuit_t c = m441_circuit_get(7);
    TEST_ASSERT_EQUAL_INT(7, c.number);
    TEST_ASSERT_EQUAL_STRING("Saturn", c.planet);
    TEST_ASSERT_EQUAL_INT(32, c.units);
}

static void test_circuit_8_uranus(void)
{
    m441_circuit_t c = m441_circuit_get(8);
    TEST_ASSERT_EQUAL_INT(8, c.number);
    TEST_ASSERT_EQUAL_STRING("Uranus", c.planet);
    TEST_ASSERT_EQUAL_INT(24, c.units);
}

static void test_circuit_9_neptune(void)
{
    m441_circuit_t c = m441_circuit_get(9);
    TEST_ASSERT_EQUAL_INT(9, c.number);
    TEST_ASSERT_EQUAL_STRING("Neptune", c.planet);
    TEST_ASSERT_EQUAL_INT(16, c.units);
}

static void test_circuit_10_pluto(void)
{
    m441_circuit_t c = m441_circuit_get(10);
    TEST_ASSERT_EQUAL_INT(10, c.number);
    TEST_ASSERT_EQUAL_STRING("Pluto", c.planet);
    TEST_ASSERT_EQUAL_INT(8, c.units);
}

static void test_circuit_11_galactic_nucleus(void)
{
    m441_circuit_t c = m441_circuit_get(11);
    TEST_ASSERT_EQUAL_INT(11, c.number);
    TEST_ASSERT_EQUAL_STRING("Galactic Nucleus", c.planet);
    TEST_ASSERT_EQUAL_INT(1, c.units);
}

/* ================================================================
 * m441_circuit_get — units decrease by 8
 * ================================================================ */

static void test_units_decrease_by_8(void)
{
    for (int i = 1; i <= 9; i++) {
        m441_circuit_t curr = m441_circuit_get(i);
        m441_circuit_t next = m441_circuit_get(i + 1);
        TEST_ASSERT_EQUAL_INT(8, curr.units - next.units);
    }
}

static void test_circuit_11_units_is_1(void)
{
    m441_circuit_t c = m441_circuit_get(11);
    TEST_ASSERT_EQUAL_INT(1, c.units);
}

static void test_unit_sum_equals_441(void)
{
    int sum = 0;
    for (int i = 1; i <= M441_CIRCUIT_COUNT; i++) {
        m441_circuit_t c = m441_circuit_get(i);
        sum += c.units;
    }
    TEST_ASSERT_EQUAL_INT(441, sum);
}

/* ================================================================
 * m441_circuit_get — invalid input
 * ================================================================ */

static void test_circuit_get_0_zeroed(void)
{
    m441_circuit_t c = m441_circuit_get(0);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

static void test_circuit_get_12_zeroed(void)
{
    m441_circuit_t c = m441_circuit_get(12);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

static void test_circuit_get_negative_zeroed(void)
{
    m441_circuit_t c = m441_circuit_get(-1);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

static void test_circuit_get_large_zeroed(void)
{
    m441_circuit_t c = m441_circuit_get(999);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    TEST_ASSERT_NULL(c.planet);
    TEST_ASSERT_EQUAL_INT(0, c.units);
}

/* ================================================================
 * m441_circuit_for_position — center
 * ================================================================ */

static void test_position_center_is_circuit_11(void)
{
    TEST_ASSERT_EQUAL_INT(11, m441_circuit_for_position(11, 11));
}

/* ================================================================
 * m441_circuit_for_position — four corners = circuit 1
 * ================================================================ */

static void test_position_corner_1_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(1, 1));
}

static void test_position_corner_1_21(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(1, 21));
}

static void test_position_corner_21_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(21, 1));
}

static void test_position_corner_21_21(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(21, 21));
}

/* ================================================================
 * m441_circuit_for_position — edge midpoints = circuit 1
 * ================================================================ */

static void test_position_edge_top_mid(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(1, 11));
}

static void test_position_edge_bottom_mid(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(21, 11));
}

static void test_position_edge_left_mid(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(11, 1));
}

static void test_position_edge_right_mid(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_circuit_for_position(11, 21));
}

/* ================================================================
 * m441_circuit_for_position — circuit 10 (ring around center)
 * ================================================================ */

static void test_position_10_12(void)
{
    TEST_ASSERT_EQUAL_INT(10, m441_circuit_for_position(10, 12));
}

static void test_position_12_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, m441_circuit_for_position(12, 10));
}

static void test_position_10_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, m441_circuit_for_position(10, 10));
}

static void test_position_12_12(void)
{
    TEST_ASSERT_EQUAL_INT(10, m441_circuit_for_position(12, 12));
}

/* ================================================================
 * m441_circuit_for_position — intermediate circuits
 * ================================================================ */

static void test_position_circuit_2(void)
{
    /* (2,2) -> max(|2-11|,|2-11|) = 9 -> circuit 11-9 = 2 */
    TEST_ASSERT_EQUAL_INT(2, m441_circuit_for_position(2, 2));
}

static void test_position_circuit_5(void)
{
    /* (5,11) -> max(|5-11|,0) = 6 -> circuit 11-6 = 5 */
    TEST_ASSERT_EQUAL_INT(5, m441_circuit_for_position(5, 11));
}

static void test_position_circuit_6(void)
{
    /* (11,6) -> max(0,|6-11|) = 5 -> circuit 11-5 = 6 */
    TEST_ASSERT_EQUAL_INT(6, m441_circuit_for_position(11, 6));
}

static void test_position_circuit_8(void)
{
    /* (8,11) -> max(|8-11|,0) = 3 -> circuit 11-3 = 8 */
    TEST_ASSERT_EQUAL_INT(8, m441_circuit_for_position(8, 11));
}

static void test_position_circuit_9(void)
{
    /* (9,9) -> max(|9-11|,|9-11|) = 2 -> circuit 11-2 = 9 */
    TEST_ASSERT_EQUAL_INT(9, m441_circuit_for_position(9, 9));
}

/* ================================================================
 * m441_circuit_for_position — symmetry
 * ================================================================ */

static void test_position_symmetry_horizontal(void)
{
    for (int c = 1; c <= 21; c++) {
        int top = m441_circuit_for_position(1, c);
        int bot = m441_circuit_for_position(21, c);
        TEST_ASSERT_EQUAL_INT(top, bot);
    }
}

static void test_position_symmetry_vertical(void)
{
    for (int r = 1; r <= 21; r++) {
        int left  = m441_circuit_for_position(r, 1);
        int right = m441_circuit_for_position(r, 21);
        TEST_ASSERT_EQUAL_INT(left, right);
    }
}

static void test_position_symmetry_diagonal(void)
{
    for (int i = 1; i <= 21; i++) {
        int diag_main = m441_circuit_for_position(i, i);
        int diag_anti = m441_circuit_for_position(i, 22 - i);
        TEST_ASSERT_EQUAL_INT(diag_main, diag_anti);
    }
}

/* ================================================================
 * m441_circuit_for_position — unit count per circuit
 * ================================================================ */

static void test_position_count_per_circuit(void)
{
    int counts[M441_CIRCUIT_COUNT];
    memset(counts, 0, sizeof(counts));

    for (int r = 1; r <= 21; r++) {
        for (int c = 1; c <= 21; c++) {
            int circ = m441_circuit_for_position(r, c);
            TEST_ASSERT_TRUE(circ >= 1 && circ <= 11);
            counts[circ - 1]++;
        }
    }

    for (int i = 0; i < M441_CIRCUIT_COUNT; i++) {
        m441_circuit_t expected = m441_circuit_get(i + 1);
        TEST_ASSERT_EQUAL_INT(expected.units, counts[i]);
    }
}

/* ================================================================
 * m441_circuit_for_position — out of bounds
 * ================================================================ */

static void test_position_row_0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(0, 11));
}

static void test_position_row_22_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(22, 11));
}

static void test_position_col_0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(11, 0));
}

static void test_position_col_22_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(11, 22));
}

static void test_position_negative_row_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(-5, 11));
}

static void test_position_negative_col_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(11, -3));
}

static void test_position_both_out_of_bounds_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(0, 0));
}

static void test_position_large_values_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_circuit_for_position(100, 200));
}

/* ================================================================
 * m441_is_tubo_mauri
 * ================================================================ */

static void test_tubo_mauri_col_11_all_rows(void)
{
    for (int r = 1; r <= 21; r++) {
        TEST_ASSERT_EQUAL_INT(1, m441_is_tubo_mauri(r, 11));
    }
}

static void test_tubo_mauri_col_1_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(11, 1));
}

static void test_tubo_mauri_col_21_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(11, 21));
}

static void test_tubo_mauri_col_10_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(5, 10));
}

static void test_tubo_mauri_col_12_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(5, 12));
}

static void test_tubo_mauri_out_of_bounds_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(0, 11));
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(22, 11));
    TEST_ASSERT_EQUAL_INT(0, m441_is_tubo_mauri(11, 0));
}

/* ================================================================
 * m441_is_equatorial
 * ================================================================ */

static void test_equatorial_row_11_all_cols(void)
{
    for (int c = 1; c <= 21; c++) {
        TEST_ASSERT_EQUAL_INT(1, m441_is_equatorial(11, c));
    }
}

static void test_equatorial_row_1_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(1, 11));
}

static void test_equatorial_row_21_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(21, 11));
}

static void test_equatorial_row_10_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(10, 5));
}

static void test_equatorial_row_12_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(12, 5));
}

static void test_equatorial_out_of_bounds_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(11, 0));
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(11, 22));
    TEST_ASSERT_EQUAL_INT(0, m441_is_equatorial(0, 11));
}

/* ================================================================
 * m441_is_center
 * ================================================================ */

static void test_center_11_11(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_is_center(11, 11));
}

static void test_center_10_11_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_center(10, 11));
}

static void test_center_11_10_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_center(11, 10));
}

static void test_center_1_1_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_center(1, 1));
}

static void test_center_21_21_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_center(21, 21));
}

static void test_center_out_of_bounds_false(void)
{
    TEST_ASSERT_EQUAL_INT(0, m441_is_center(0, 0));
    TEST_ASSERT_EQUAL_INT(0, m441_is_center(22, 22));
}

/* ================================================================
 * Structural: center is both tubo mauri and equatorial
 * ================================================================ */

static void test_center_is_tubo_mauri_and_equatorial(void)
{
    TEST_ASSERT_EQUAL_INT(1, m441_is_tubo_mauri(11, 11));
    TEST_ASSERT_EQUAL_INT(1, m441_is_equatorial(11, 11));
    TEST_ASSERT_EQUAL_INT(1, m441_is_center(11, 11));
}

/* ================================================================
 * Structural: tubo mauri count = 21, equatorial count = 21
 * ================================================================ */

static void test_tubo_mauri_count_is_21(void)
{
    int count = 0;
    for (int r = 1; r <= 21; r++) {
        for (int c = 1; c <= 21; c++) {
            if (m441_is_tubo_mauri(r, c))
                count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(21, count);
}

static void test_equatorial_count_is_21(void)
{
    int count = 0;
    for (int r = 1; r <= 21; r++) {
        for (int c = 1; c <= 21; c++) {
            if (m441_is_equatorial(r, c))
                count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(21, count);
}

/* ================================================================
 * Purity: same input gives same output
 * ================================================================ */

static void test_purity_circuit_get(void)
{
    m441_circuit_t a = m441_circuit_get(5);
    m441_circuit_t b = m441_circuit_get(5);
    TEST_ASSERT_EQUAL_INT(a.number, b.number);
    TEST_ASSERT_EQUAL_STRING(a.planet, b.planet);
    TEST_ASSERT_EQUAL_INT(a.units, b.units);
}

static void test_purity_circuit_for_position(void)
{
    TEST_ASSERT_EQUAL_INT(
        m441_circuit_for_position(7, 3),
        m441_circuit_for_position(7, 3)
    );
}

static void test_purity_helpers(void)
{
    TEST_ASSERT_EQUAL_INT(m441_circuit_count(), m441_circuit_count());
    TEST_ASSERT_EQUAL_INT(m441_total_units(), m441_total_units());
    TEST_ASSERT_EQUAL_INT(m441_dimensions_of_time(), m441_dimensions_of_time());
    TEST_ASSERT_EQUAL_INT(
        m441_is_tubo_mauri(5, 11),
        m441_is_tubo_mauri(5, 11)
    );
    TEST_ASSERT_EQUAL_INT(
        m441_is_equatorial(11, 5),
        m441_is_equatorial(11, 5)
    );
    TEST_ASSERT_EQUAL_INT(
        m441_is_center(11, 11),
        m441_is_center(11, 11)
    );
}

/* ================================================================
 * Sweep: all valid positions produce valid circuit numbers
 * ================================================================ */

static void test_all_positions_have_valid_circuit(void)
{
    for (int r = 1; r <= 21; r++) {
        for (int c = 1; c <= 21; c++) {
            int circ = m441_circuit_for_position(r, c);
            TEST_ASSERT_TRUE(circ >= 1 && circ <= 11);
        }
    }
}

/* ================================================================
 * main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Constants */
    RUN_TEST(test_size_is_21);
    RUN_TEST(test_center_is_11);
    RUN_TEST(test_total_is_441);
    RUN_TEST(test_circuit_count_constant_is_11);
    RUN_TEST(test_441_equals_21_squared);

    /* m441_circuit_count */
    RUN_TEST(test_circuit_count_returns_11);

    /* m441_total_units */
    RUN_TEST(test_total_units_returns_441);

    /* m441_dimensions_of_time */
    RUN_TEST(test_dimensions_of_time_returns_9);
    RUN_TEST(test_9_times_49_equals_441);

    /* m441_circuit_get — all 11 circuits */
    RUN_TEST(test_circuit_1_mercury);
    RUN_TEST(test_circuit_2_venus);
    RUN_TEST(test_circuit_3_earth);
    RUN_TEST(test_circuit_4_mars);
    RUN_TEST(test_circuit_5_maldek);
    RUN_TEST(test_circuit_6_jupiter);
    RUN_TEST(test_circuit_7_saturn);
    RUN_TEST(test_circuit_8_uranus);
    RUN_TEST(test_circuit_9_neptune);
    RUN_TEST(test_circuit_10_pluto);
    RUN_TEST(test_circuit_11_galactic_nucleus);

    /* Units pattern */
    RUN_TEST(test_units_decrease_by_8);
    RUN_TEST(test_circuit_11_units_is_1);
    RUN_TEST(test_unit_sum_equals_441);

    /* m441_circuit_get — invalid */
    RUN_TEST(test_circuit_get_0_zeroed);
    RUN_TEST(test_circuit_get_12_zeroed);
    RUN_TEST(test_circuit_get_negative_zeroed);
    RUN_TEST(test_circuit_get_large_zeroed);

    /* circuit_for_position — center */
    RUN_TEST(test_position_center_is_circuit_11);

    /* circuit_for_position — corners */
    RUN_TEST(test_position_corner_1_1);
    RUN_TEST(test_position_corner_1_21);
    RUN_TEST(test_position_corner_21_1);
    RUN_TEST(test_position_corner_21_21);

    /* circuit_for_position — edge midpoints */
    RUN_TEST(test_position_edge_top_mid);
    RUN_TEST(test_position_edge_bottom_mid);
    RUN_TEST(test_position_edge_left_mid);
    RUN_TEST(test_position_edge_right_mid);

    /* circuit_for_position — circuit 10 ring */
    RUN_TEST(test_position_10_12);
    RUN_TEST(test_position_12_10);
    RUN_TEST(test_position_10_10);
    RUN_TEST(test_position_12_12);

    /* circuit_for_position — intermediate circuits */
    RUN_TEST(test_position_circuit_2);
    RUN_TEST(test_position_circuit_5);
    RUN_TEST(test_position_circuit_6);
    RUN_TEST(test_position_circuit_8);
    RUN_TEST(test_position_circuit_9);

    /* circuit_for_position — symmetry */
    RUN_TEST(test_position_symmetry_horizontal);
    RUN_TEST(test_position_symmetry_vertical);
    RUN_TEST(test_position_symmetry_diagonal);

    /* circuit_for_position — unit count verification */
    RUN_TEST(test_position_count_per_circuit);

    /* circuit_for_position — out of bounds */
    RUN_TEST(test_position_row_0_returns_0);
    RUN_TEST(test_position_row_22_returns_0);
    RUN_TEST(test_position_col_0_returns_0);
    RUN_TEST(test_position_col_22_returns_0);
    RUN_TEST(test_position_negative_row_returns_0);
    RUN_TEST(test_position_negative_col_returns_0);
    RUN_TEST(test_position_both_out_of_bounds_returns_0);
    RUN_TEST(test_position_large_values_returns_0);

    /* Tubo Mauri */
    RUN_TEST(test_tubo_mauri_col_11_all_rows);
    RUN_TEST(test_tubo_mauri_col_1_false);
    RUN_TEST(test_tubo_mauri_col_21_false);
    RUN_TEST(test_tubo_mauri_col_10_false);
    RUN_TEST(test_tubo_mauri_col_12_false);
    RUN_TEST(test_tubo_mauri_out_of_bounds_returns_0);

    /* Equatorial */
    RUN_TEST(test_equatorial_row_11_all_cols);
    RUN_TEST(test_equatorial_row_1_false);
    RUN_TEST(test_equatorial_row_21_false);
    RUN_TEST(test_equatorial_row_10_false);
    RUN_TEST(test_equatorial_row_12_false);
    RUN_TEST(test_equatorial_out_of_bounds_returns_0);

    /* Center */
    RUN_TEST(test_center_11_11);
    RUN_TEST(test_center_10_11_false);
    RUN_TEST(test_center_11_10_false);
    RUN_TEST(test_center_1_1_false);
    RUN_TEST(test_center_21_21_false);
    RUN_TEST(test_center_out_of_bounds_false);

    /* Structural */
    RUN_TEST(test_center_is_tubo_mauri_and_equatorial);
    RUN_TEST(test_tubo_mauri_count_is_21);
    RUN_TEST(test_equatorial_count_is_21);

    /* Purity */
    RUN_TEST(test_purity_circuit_get);
    RUN_TEST(test_purity_circuit_for_position);
    RUN_TEST(test_purity_helpers);

    /* Sweep */
    RUN_TEST(test_all_positions_have_valid_circuit);

    return UNITY_END();
}
