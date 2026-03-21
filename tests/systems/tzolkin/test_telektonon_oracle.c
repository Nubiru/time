#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/telektonon_oracle.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Prophecy year count
 * ================================================================ */

static void test_prophecy_year_count_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, tko_prophecy_year_count());
}

/* ================================================================
 * Year 1: Victory Hits the Target — 1993-94, 1st Seal
 * ================================================================ */

static void test_year1_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(1);
    TEST_ASSERT_EQUAL_INT(1, y.year_number);
}

static void test_year1_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(1);
    TEST_ASSERT_EQUAL_INT(1993, y.start_greg_year);
}

static void test_year1_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Hits the Target",
                             tko_prophecy_year_name(1));
}

static void test_year1_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(1);
    TEST_ASSERT_EQUAL_STRING("1st Seal", y.victory_seal);
}

/* ================================================================
 * Year 2: Victory Flows — 1994-95, 2nd Seal
 * ================================================================ */

static void test_year2_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(2);
    TEST_ASSERT_EQUAL_INT(2, y.year_number);
}

static void test_year2_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(2);
    TEST_ASSERT_EQUAL_INT(1994, y.start_greg_year);
}

static void test_year2_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Flows",
                             tko_prophecy_year_name(2));
}

static void test_year2_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(2);
    TEST_ASSERT_EQUAL_STRING("2nd Seal", y.victory_seal);
}

/* ================================================================
 * Year 3: Victory Pacifies — 1995-96, 3rd Seal
 * ================================================================ */

static void test_year3_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(3);
    TEST_ASSERT_EQUAL_INT(3, y.year_number);
}

static void test_year3_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(3);
    TEST_ASSERT_EQUAL_INT(1995, y.start_greg_year);
}

static void test_year3_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Pacifies",
                             tko_prophecy_year_name(3));
}

static void test_year3_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(3);
    TEST_ASSERT_EQUAL_STRING("3rd Seal", y.victory_seal);
}

/* ================================================================
 * Year 4: Victory Establishes — 1996-97, 4th Seal
 * ================================================================ */

static void test_year4_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(4);
    TEST_ASSERT_EQUAL_INT(4, y.year_number);
}

static void test_year4_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(4);
    TEST_ASSERT_EQUAL_INT(1996, y.start_greg_year);
}

static void test_year4_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Establishes",
                             tko_prophecy_year_name(4));
}

static void test_year4_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(4);
    TEST_ASSERT_EQUAL_STRING("4th Seal", y.victory_seal);
}

/* ================================================================
 * Year 5: Victory Liberates — 1997-98, 5th Seal
 * ================================================================ */

static void test_year5_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(5);
    TEST_ASSERT_EQUAL_INT(5, y.year_number);
}

static void test_year5_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(5);
    TEST_ASSERT_EQUAL_INT(1997, y.start_greg_year);
}

static void test_year5_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Liberates",
                             tko_prophecy_year_name(5));
}

static void test_year5_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(5);
    TEST_ASSERT_EQUAL_STRING("5th Seal", y.victory_seal);
}

/* ================================================================
 * Year 6: Victory Purifies — 1998-99, 6th Seal
 * ================================================================ */

static void test_year6_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(6);
    TEST_ASSERT_EQUAL_INT(6, y.year_number);
}

static void test_year6_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(6);
    TEST_ASSERT_EQUAL_INT(1998, y.start_greg_year);
}

static void test_year6_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Purifies",
                             tko_prophecy_year_name(6));
}

static void test_year6_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(6);
    TEST_ASSERT_EQUAL_STRING("6th Seal", y.victory_seal);
}

/* ================================================================
 * Year 7: Victory Discharges — 1999-2000, 7th Seal
 * ================================================================ */

static void test_year7_number(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(7);
    TEST_ASSERT_EQUAL_INT(7, y.year_number);
}

static void test_year7_start_greg(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(7);
    TEST_ASSERT_EQUAL_INT(1999, y.start_greg_year);
}

static void test_year7_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Victory Discharges",
                             tko_prophecy_year_name(7));
}

static void test_year7_victory_seal(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(7);
    TEST_ASSERT_EQUAL_STRING("7th Seal", y.victory_seal);
}

/* ================================================================
 * Prophecy year boundaries — invalid numbers
 * ================================================================ */

static void test_year_get_0_returns_zeroed(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(0);
    TEST_ASSERT_EQUAL_INT(0, y.year_number);
    TEST_ASSERT_EQUAL_INT(0, y.start_greg_year);
    TEST_ASSERT_NULL(y.name);
    TEST_ASSERT_NULL(y.victory_seal);
}

static void test_year_get_8_returns_zeroed(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(8);
    TEST_ASSERT_EQUAL_INT(0, y.year_number);
    TEST_ASSERT_NULL(y.name);
    TEST_ASSERT_NULL(y.victory_seal);
}

static void test_year_get_negative_returns_zeroed(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(-1);
    TEST_ASSERT_EQUAL_INT(0, y.year_number);
    TEST_ASSERT_NULL(y.name);
}

static void test_year_get_99_returns_zeroed(void)
{
    tko_prophecy_year_t y = tko_prophecy_year_get(99);
    TEST_ASSERT_EQUAL_INT(0, y.year_number);
    TEST_ASSERT_NULL(y.name);
}

static void test_year_name_0_returns_null(void)
{
    TEST_ASSERT_NULL(tko_prophecy_year_name(0));
}

static void test_year_name_8_returns_null(void)
{
    TEST_ASSERT_NULL(tko_prophecy_year_name(8));
}

static void test_year_name_negative_returns_null(void)
{
    TEST_ASSERT_NULL(tko_prophecy_year_name(-1));
}

/* ================================================================
 * Position count
 * ================================================================ */

static void test_position_count_is_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, tko_position_count());
}

/* ================================================================
 * Position Crown — Power of Mind
 * ================================================================ */

static void test_crown_position_enum(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_CROWN);
    TEST_ASSERT_EQUAL_INT(TKO_CROWN, p.position);
}

static void test_crown_name(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_CROWN);
    TEST_ASSERT_EQUAL_STRING("Crown", p.name);
}

static void test_crown_name_es(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_CROWN);
    TEST_ASSERT_EQUAL_STRING("Corona", p.name_es);
}

static void test_crown_power(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_CROWN);
    TEST_ASSERT_EQUAL_STRING("Power of Mind", p.power);
}

static void test_crown_focus(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_CROWN);
    TEST_ASSERT_EQUAL_STRING("Mandate of Heaven, Purpose of Annual Oracle",
                             p.focus);
}

/* ================================================================
 * Position Heart — Power of Love
 * ================================================================ */

static void test_heart_position_enum(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_HEART);
    TEST_ASSERT_EQUAL_INT(TKO_HEART, p.position);
}

static void test_heart_name(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_HEART);
    TEST_ASSERT_EQUAL_STRING("Heart", p.name);
}

static void test_heart_name_es(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_HEART);
    TEST_ASSERT_EQUAL_STRING("Corazon", p.name_es);
}

static void test_heart_power(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_HEART);
    TEST_ASSERT_EQUAL_STRING("Power of Love", p.power);
}

static void test_heart_focus(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_HEART);
    TEST_ASSERT_EQUAL_STRING("Human Sentiments, Ring Oracle Result",
                             p.focus);
}

/* ================================================================
 * Position Root — Hidden Power of Earth
 * ================================================================ */

static void test_root_position_enum(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_ROOT);
    TEST_ASSERT_EQUAL_INT(TKO_ROOT, p.position);
}

static void test_root_name(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_ROOT);
    TEST_ASSERT_EQUAL_STRING("Root", p.name);
}

static void test_root_name_es(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_ROOT);
    TEST_ASSERT_EQUAL_STRING("Raiz", p.name_es);
}

static void test_root_power(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_ROOT);
    TEST_ASSERT_EQUAL_STRING("Hidden Power of Earth", p.power);
}

static void test_root_focus(void)
{
    tko_oracle_position_t p = tko_position_get(TKO_ROOT);
    TEST_ASSERT_EQUAL_STRING("Lost Generation Oracle", p.focus);
}

/* ================================================================
 * Position boundaries — invalid enum values
 * ================================================================ */

static void test_position_get_negative_returns_zeroed(void)
{
    tko_oracle_position_t p = tko_position_get((tko_position_t)-1);
    TEST_ASSERT_NULL(p.name);
    TEST_ASSERT_NULL(p.name_es);
    TEST_ASSERT_NULL(p.power);
    TEST_ASSERT_NULL(p.focus);
}

static void test_position_get_3_returns_zeroed(void)
{
    tko_oracle_position_t p = tko_position_get((tko_position_t)3);
    TEST_ASSERT_NULL(p.name);
    TEST_ASSERT_NULL(p.name_es);
    TEST_ASSERT_NULL(p.power);
    TEST_ASSERT_NULL(p.focus);
}

static void test_position_get_99_returns_zeroed(void)
{
    tko_oracle_position_t p = tko_position_get((tko_position_t)99);
    TEST_ASSERT_NULL(p.name);
}

static void test_position_name_negative_returns_null(void)
{
    TEST_ASSERT_NULL(tko_position_name((tko_position_t)-1));
}

static void test_position_name_3_returns_null(void)
{
    TEST_ASSERT_NULL(tko_position_name((tko_position_t)3));
}

/* ================================================================
 * Position name function — valid values
 * ================================================================ */

static void test_position_name_crown(void)
{
    TEST_ASSERT_EQUAL_STRING("Crown", tko_position_name(TKO_CROWN));
}

static void test_position_name_heart(void)
{
    TEST_ASSERT_EQUAL_STRING("Heart", tko_position_name(TKO_HEART));
}

static void test_position_name_root(void)
{
    TEST_ASSERT_EQUAL_STRING("Root", tko_position_name(TKO_ROOT));
}

/* ================================================================
 * Purity — same input, same output
 * ================================================================ */

static void test_purity_year_get_same_result(void)
{
    tko_prophecy_year_t a = tko_prophecy_year_get(4);
    tko_prophecy_year_t b = tko_prophecy_year_get(4);
    TEST_ASSERT_EQUAL_INT(a.year_number, b.year_number);
    TEST_ASSERT_EQUAL_INT(a.start_greg_year, b.start_greg_year);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.victory_seal, b.victory_seal);
}

static void test_purity_position_get_same_result(void)
{
    tko_oracle_position_t a = tko_position_get(TKO_HEART);
    tko_oracle_position_t b = tko_position_get(TKO_HEART);
    TEST_ASSERT_EQUAL_INT(a.position, b.position);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.name_es, b.name_es);
    TEST_ASSERT_EQUAL_STRING(a.power, b.power);
    TEST_ASSERT_EQUAL_STRING(a.focus, b.focus);
}

/* ================================================================
 * Sweeps: all years have valid data
 * ================================================================ */

static void test_all_years_have_valid_data(void)
{
    for (int i = 1; i <= 7; i++) {
        tko_prophecy_year_t y = tko_prophecy_year_get(i);
        TEST_ASSERT_EQUAL_INT(i, y.year_number);
        TEST_ASSERT_NOT_NULL(y.name);
        TEST_ASSERT_NOT_NULL(y.victory_seal);
        TEST_ASSERT_TRUE(strlen(y.name) > 0);
        TEST_ASSERT_TRUE(strlen(y.victory_seal) > 0);
    }
}

static void test_all_positions_have_valid_data(void)
{
    for (int i = 0; i < 3; i++) {
        tko_oracle_position_t p = tko_position_get((tko_position_t)i);
        TEST_ASSERT_EQUAL_INT(i, (int)p.position);
        TEST_ASSERT_NOT_NULL(p.name);
        TEST_ASSERT_NOT_NULL(p.name_es);
        TEST_ASSERT_NOT_NULL(p.power);
        TEST_ASSERT_NOT_NULL(p.focus);
        TEST_ASSERT_TRUE(strlen(p.name) > 0);
        TEST_ASSERT_TRUE(strlen(p.name_es) > 0);
        TEST_ASSERT_TRUE(strlen(p.power) > 0);
        TEST_ASSERT_TRUE(strlen(p.focus) > 0);
    }
}

/* ================================================================
 * Cross-checks: year numbers are sequential 1-7
 * ================================================================ */

static void test_years_are_sequential(void)
{
    for (int i = 1; i <= 7; i++) {
        tko_prophecy_year_t y = tko_prophecy_year_get(i);
        TEST_ASSERT_EQUAL_INT(i, y.year_number);
    }
}

/* ================================================================
 * Cross-checks: start years are sequential 1993-1999
 * ================================================================ */

static void test_start_years_are_sequential(void)
{
    for (int i = 1; i <= 7; i++) {
        tko_prophecy_year_t y = tko_prophecy_year_get(i);
        TEST_ASSERT_EQUAL_INT(1992 + i, y.start_greg_year);
    }
}

/* ================================================================
 * Cross-checks: year_name matches year_get name
 * ================================================================ */

static void test_year_name_matches_year_get(void)
{
    for (int i = 1; i <= 7; i++) {
        tko_prophecy_year_t y = tko_prophecy_year_get(i);
        const char *name = tko_prophecy_year_name(i);
        TEST_ASSERT_EQUAL_STRING(y.name, name);
    }
}

/* ================================================================
 * Cross-checks: position_name matches position_get name
 * ================================================================ */

static void test_position_name_matches_position_get(void)
{
    for (int i = 0; i < 3; i++) {
        tko_oracle_position_t p = tko_position_get((tko_position_t)i);
        const char *name = tko_position_name((tko_position_t)i);
        TEST_ASSERT_EQUAL_STRING(p.name, name);
    }
}

/* ================================================================
 * Cross-checks: all year names start with "Victory"
 * ================================================================ */

static void test_all_year_names_start_with_victory(void)
{
    for (int i = 1; i <= 7; i++) {
        const char *name = tko_prophecy_year_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_EQUAL_INT(0, strncmp(name, "Victory", 7));
    }
}

/* ================================================================
 * Cross-checks: all victory seals end with "Seal"
 * ================================================================ */

static void test_all_victory_seals_end_with_seal(void)
{
    for (int i = 1; i <= 7; i++) {
        tko_prophecy_year_t y = tko_prophecy_year_get(i);
        TEST_ASSERT_NOT_NULL(y.victory_seal);
        size_t len = strlen(y.victory_seal);
        TEST_ASSERT_TRUE(len >= 4);
        TEST_ASSERT_EQUAL_STRING("Seal", y.victory_seal + len - 4);
    }
}

/* ================================================================
 * Cross-checks: position enum values are 0, 1, 2
 * ================================================================ */

static void test_position_enum_values(void)
{
    TEST_ASSERT_EQUAL_INT(0, (int)TKO_CROWN);
    TEST_ASSERT_EQUAL_INT(1, (int)TKO_HEART);
    TEST_ASSERT_EQUAL_INT(2, (int)TKO_ROOT);
}

/* ================================================================
 * Cross-checks: each position has distinct power
 * ================================================================ */

static void test_positions_have_distinct_powers(void)
{
    tko_oracle_position_t crown = tko_position_get(TKO_CROWN);
    tko_oracle_position_t heart = tko_position_get(TKO_HEART);
    tko_oracle_position_t root  = tko_position_get(TKO_ROOT);
    TEST_ASSERT_TRUE(strcmp(crown.power, heart.power) != 0);
    TEST_ASSERT_TRUE(strcmp(crown.power, root.power) != 0);
    TEST_ASSERT_TRUE(strcmp(heart.power, root.power) != 0);
}

/* ================================================================
 * main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Counts */
    RUN_TEST(test_prophecy_year_count_is_7);
    RUN_TEST(test_position_count_is_3);

    /* Year 1 — Victory Hits the Target */
    RUN_TEST(test_year1_number);
    RUN_TEST(test_year1_start_greg);
    RUN_TEST(test_year1_name);
    RUN_TEST(test_year1_victory_seal);

    /* Year 2 — Victory Flows */
    RUN_TEST(test_year2_number);
    RUN_TEST(test_year2_start_greg);
    RUN_TEST(test_year2_name);
    RUN_TEST(test_year2_victory_seal);

    /* Year 3 — Victory Pacifies */
    RUN_TEST(test_year3_number);
    RUN_TEST(test_year3_start_greg);
    RUN_TEST(test_year3_name);
    RUN_TEST(test_year3_victory_seal);

    /* Year 4 — Victory Establishes */
    RUN_TEST(test_year4_number);
    RUN_TEST(test_year4_start_greg);
    RUN_TEST(test_year4_name);
    RUN_TEST(test_year4_victory_seal);

    /* Year 5 — Victory Liberates */
    RUN_TEST(test_year5_number);
    RUN_TEST(test_year5_start_greg);
    RUN_TEST(test_year5_name);
    RUN_TEST(test_year5_victory_seal);

    /* Year 6 — Victory Purifies */
    RUN_TEST(test_year6_number);
    RUN_TEST(test_year6_start_greg);
    RUN_TEST(test_year6_name);
    RUN_TEST(test_year6_victory_seal);

    /* Year 7 — Victory Discharges */
    RUN_TEST(test_year7_number);
    RUN_TEST(test_year7_start_greg);
    RUN_TEST(test_year7_name);
    RUN_TEST(test_year7_victory_seal);

    /* Year boundaries */
    RUN_TEST(test_year_get_0_returns_zeroed);
    RUN_TEST(test_year_get_8_returns_zeroed);
    RUN_TEST(test_year_get_negative_returns_zeroed);
    RUN_TEST(test_year_get_99_returns_zeroed);
    RUN_TEST(test_year_name_0_returns_null);
    RUN_TEST(test_year_name_8_returns_null);
    RUN_TEST(test_year_name_negative_returns_null);

    /* Position Crown */
    RUN_TEST(test_crown_position_enum);
    RUN_TEST(test_crown_name);
    RUN_TEST(test_crown_name_es);
    RUN_TEST(test_crown_power);
    RUN_TEST(test_crown_focus);

    /* Position Heart */
    RUN_TEST(test_heart_position_enum);
    RUN_TEST(test_heart_name);
    RUN_TEST(test_heart_name_es);
    RUN_TEST(test_heart_power);
    RUN_TEST(test_heart_focus);

    /* Position Root */
    RUN_TEST(test_root_position_enum);
    RUN_TEST(test_root_name);
    RUN_TEST(test_root_name_es);
    RUN_TEST(test_root_power);
    RUN_TEST(test_root_focus);

    /* Position boundaries */
    RUN_TEST(test_position_get_negative_returns_zeroed);
    RUN_TEST(test_position_get_3_returns_zeroed);
    RUN_TEST(test_position_get_99_returns_zeroed);
    RUN_TEST(test_position_name_negative_returns_null);
    RUN_TEST(test_position_name_3_returns_null);

    /* Position name function */
    RUN_TEST(test_position_name_crown);
    RUN_TEST(test_position_name_heart);
    RUN_TEST(test_position_name_root);

    /* Purity */
    RUN_TEST(test_purity_year_get_same_result);
    RUN_TEST(test_purity_position_get_same_result);

    /* Sweeps */
    RUN_TEST(test_all_years_have_valid_data);
    RUN_TEST(test_all_positions_have_valid_data);

    /* Cross-checks */
    RUN_TEST(test_years_are_sequential);
    RUN_TEST(test_start_years_are_sequential);
    RUN_TEST(test_year_name_matches_year_get);
    RUN_TEST(test_position_name_matches_position_get);
    RUN_TEST(test_all_year_names_start_with_victory);
    RUN_TEST(test_all_victory_seals_end_with_seal);
    RUN_TEST(test_position_enum_values);
    RUN_TEST(test_positions_have_distinct_powers);

    return UNITY_END();
}
