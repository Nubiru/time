#include "../unity/unity.h"
#include "../../src/ui/dignity_table_layout.h"
#include "../../src/systems/astrology/dignity.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Table geometry --- */

void test_table_bounds(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.table_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.table_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.table_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, layout.table_h);
}

/* --- Column headers --- */

void test_header_name_position(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Column 0, row 0: x = 0.05, y = 0.10 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.header_name.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.header_name.y);
}

void test_header_domicile_position(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Column 1: x = 0.05 + 0.18 = 0.23 */
    float col_w = 0.90f / 5.0f;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f + col_w, layout.header_domicile.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.header_domicile.y);
}

void test_header_fall_position(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Column 4: x = 0.05 + 4 * (0.90/5) */
    float col_w = 0.90f / 5.0f;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f + 4.0f * col_w, layout.header_fall.x);
}

void test_header_cell_width(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    float col_w = 0.90f / 5.0f;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.header_name.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.header_domicile.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.header_exaltation.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.header_detriment.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.header_fall.w);
}

/* --- Planet count and order --- */

void test_seven_planets(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_STRING("Sun",     layout.rows[0].planet_name);
    TEST_ASSERT_EQUAL_STRING("Moon",    layout.rows[1].planet_name);
    TEST_ASSERT_EQUAL_STRING("Mercury", layout.rows[2].planet_name);
    TEST_ASSERT_EQUAL_STRING("Venus",   layout.rows[3].planet_name);
    TEST_ASSERT_EQUAL_STRING("Mars",    layout.rows[4].planet_name);
    TEST_ASSERT_EQUAL_STRING("Jupiter", layout.rows[5].planet_name);
    TEST_ASSERT_EQUAL_STRING("Saturn",  layout.rows[6].planet_name);
}

void test_planet_indices(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(DIGNITY_SUN,     layout.rows[0].planet);
    TEST_ASSERT_EQUAL_INT(DIGNITY_MOON,    layout.rows[1].planet);
    TEST_ASSERT_EQUAL_INT(DIGNITY_MERCURY, layout.rows[2].planet);
    TEST_ASSERT_EQUAL_INT(DIGNITY_VENUS,   layout.rows[3].planet);
    TEST_ASSERT_EQUAL_INT(DIGNITY_MARS,    layout.rows[4].planet);
    TEST_ASSERT_EQUAL_INT(DIGNITY_JUPITER, layout.rows[5].planet);
    TEST_ASSERT_EQUAL_INT(DIGNITY_SATURN,  layout.rows[6].planet);
}

/* --- Domicile signs --- */

void test_sun_domicile_leo(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(4, layout.rows[0].domicile_sign);  /* Leo */
}

void test_moon_domicile_cancer(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(3, layout.rows[1].domicile_sign);  /* Cancer */
}

void test_mercury_domicile_gemini(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(2, layout.rows[2].domicile_sign);  /* Gemini */
}

void test_venus_domicile_taurus(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(1, layout.rows[3].domicile_sign);  /* Taurus */
}

void test_mars_domicile_aries(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(0, layout.rows[4].domicile_sign);  /* Aries */
}

void test_jupiter_domicile_sagittarius(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(8, layout.rows[5].domicile_sign);  /* Sagittarius */
}

void test_saturn_domicile_capricorn(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(9, layout.rows[6].domicile_sign);  /* Capricorn */
}

/* --- Exaltation signs --- */

void test_sun_exaltation_aries(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(0, layout.rows[0].exaltation_sign);  /* Aries */
}

void test_moon_exaltation_taurus(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(1, layout.rows[1].exaltation_sign);  /* Taurus */
}

void test_saturn_exaltation_libra(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    TEST_ASSERT_EQUAL_INT(6, layout.rows[6].exaltation_sign);  /* Libra */
}

/* --- Detriment = opposite of domicile --- */

void test_sun_detriment_aquarius(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Leo (4) opposite = Aquarius (10) */
    TEST_ASSERT_EQUAL_INT(10, layout.rows[0].detriment_sign);
}

void test_moon_detriment_capricorn(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Cancer (3) opposite = Capricorn (9) */
    TEST_ASSERT_EQUAL_INT(9, layout.rows[1].detriment_sign);
}

/* --- Fall = opposite of exaltation --- */

void test_sun_fall_libra(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Aries (0) opposite = Libra (6) */
    TEST_ASSERT_EQUAL_INT(6, layout.rows[0].fall_sign);
}

void test_moon_fall_scorpio(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    /* Taurus (1) opposite = Scorpio (7) */
    TEST_ASSERT_EQUAL_INT(7, layout.rows[1].fall_sign);
}

/* --- Cell positions --- */

void test_sun_name_cell_row_1(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    float row_h = 0.85f / 8.0f;
    /* Row 1 (first planet), column 0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.rows[0].name_cell.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f + row_h, layout.rows[0].name_cell.y);
}

void test_saturn_fall_cell_row_7(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    float col_w = 0.90f / 5.0f;
    float row_h = 0.85f / 8.0f;
    /* Row 7 (index 6), column 4 (fall) */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f + 4.0f * col_w, layout.rows[6].fall_cell.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f + 7.0f * row_h, layout.rows[6].fall_cell.y);
}

void test_all_cells_same_size(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    float col_w = 0.90f / 5.0f;
    float row_h = 0.85f / 8.0f;
    for (int i = 0; i < DT_PLANET_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.rows[i].name_cell.w);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, row_h, layout.rows[i].name_cell.h);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.rows[i].domicile_cell.w);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, col_w, layout.rows[i].fall_cell.w);
    }
}

/* --- dignity_table_row accessor --- */

void test_row_accessor_valid(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    dt_planet_row_t row = dignity_table_row(&layout, 0);
    TEST_ASSERT_EQUAL_INT(DIGNITY_SUN, row.planet);
    TEST_ASSERT_EQUAL_STRING("Sun", row.planet_name);
}

void test_row_accessor_last(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    dt_planet_row_t row = dignity_table_row(&layout, 6);
    TEST_ASSERT_EQUAL_INT(DIGNITY_SATURN, row.planet);
    TEST_ASSERT_EQUAL_STRING("Saturn", row.planet_name);
}

void test_row_accessor_invalid_negative(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    dt_planet_row_t row = dignity_table_row(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, row.planet);
    TEST_ASSERT_EQUAL_STRING("?", row.planet_name);
}

void test_row_accessor_invalid_7(void)
{
    dignity_table_layout_t layout = dignity_table_compute();
    dt_planet_row_t row = dignity_table_row(&layout, 7);
    TEST_ASSERT_EQUAL_INT(-1, row.planet);
}

void test_row_accessor_null_layout(void)
{
    dt_planet_row_t row = dignity_table_row(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, row.planet);
    TEST_ASSERT_EQUAL_STRING("?", row.planet_name);
}

/* --- Purity --- */

void test_purity_same_output(void)
{
    dignity_table_layout_t a = dignity_table_compute();
    dignity_table_layout_t b = dignity_table_compute();
    for (int i = 0; i < DT_PLANET_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(a.rows[i].planet, b.rows[i].planet);
        TEST_ASSERT_EQUAL_INT(a.rows[i].domicile_sign, b.rows[i].domicile_sign);
        TEST_ASSERT_EQUAL_INT(a.rows[i].exaltation_sign, b.rows[i].exaltation_sign);
        TEST_ASSERT_EQUAL_INT(a.rows[i].detriment_sign, b.rows[i].detriment_sign);
        TEST_ASSERT_EQUAL_INT(a.rows[i].fall_sign, b.rows[i].fall_sign);
        TEST_ASSERT_EQUAL_STRING(a.rows[i].planet_name, b.rows[i].planet_name);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* Table geometry */
    RUN_TEST(test_table_bounds);
    /* Column headers */
    RUN_TEST(test_header_name_position);
    RUN_TEST(test_header_domicile_position);
    RUN_TEST(test_header_fall_position);
    RUN_TEST(test_header_cell_width);
    /* Planet count and order */
    RUN_TEST(test_seven_planets);
    RUN_TEST(test_planet_indices);
    /* Domicile signs */
    RUN_TEST(test_sun_domicile_leo);
    RUN_TEST(test_moon_domicile_cancer);
    RUN_TEST(test_mercury_domicile_gemini);
    RUN_TEST(test_venus_domicile_taurus);
    RUN_TEST(test_mars_domicile_aries);
    RUN_TEST(test_jupiter_domicile_sagittarius);
    RUN_TEST(test_saturn_domicile_capricorn);
    /* Exaltation signs */
    RUN_TEST(test_sun_exaltation_aries);
    RUN_TEST(test_moon_exaltation_taurus);
    RUN_TEST(test_saturn_exaltation_libra);
    /* Detriment */
    RUN_TEST(test_sun_detriment_aquarius);
    RUN_TEST(test_moon_detriment_capricorn);
    /* Fall */
    RUN_TEST(test_sun_fall_libra);
    RUN_TEST(test_moon_fall_scorpio);
    /* Cell positions */
    RUN_TEST(test_sun_name_cell_row_1);
    RUN_TEST(test_saturn_fall_cell_row_7);
    RUN_TEST(test_all_cells_same_size);
    /* Accessor */
    RUN_TEST(test_row_accessor_valid);
    RUN_TEST(test_row_accessor_last);
    RUN_TEST(test_row_accessor_invalid_negative);
    RUN_TEST(test_row_accessor_invalid_7);
    RUN_TEST(test_row_accessor_null_layout);
    /* Purity */
    RUN_TEST(test_purity_same_output);
    return UNITY_END();
}
