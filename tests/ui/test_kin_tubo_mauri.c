#include "../unity/unity.h"
#include "../../src/ui/kin_tubo_mauri.h"
#include "../../src/math/julian.h"

void setUp(void) {}
void tearDown(void) {}

/* --- July 26, 2026: Moon 1 Day 1, day_of_week=1 --- */

void test_jul26_today_gate_0(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_INT(0, tubo.today_gate);
}

void test_jul26_today_day_of_week_1(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, tubo.today_day_of_week);
}

/* --- 7 gates total --- */

void test_7_gates_sequential(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, tubo.gates[i].day_of_week);
    }
}

/* --- Gate 0: plasma, chakra, mantra not NULL --- */

void test_gate0_plasma_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_NOT_NULL(tubo.gates[0].plasma);
}

void test_gate0_chakra_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_NOT_NULL(tubo.gates[0].chakra);
}

void test_gate0_mantra_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_NOT_NULL(tubo.gates[0].mantra);
}

/* --- Gate 0 (Dali) at top y ~ 0.05 --- */

void test_gate0_y_top(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, tubo.gates[0].y);
}

/* --- Gate 6 (Silio) at bottom y ~ 0.95 --- */

void test_gate6_y_bottom(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.95f, tubo.gates[6].y);
}

/* --- All gates at x=0.5 --- */

void test_all_gates_x_centered(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, tubo.gates[i].x);
    }
}

/* --- Only 1 gate highlighted --- */

void test_only_one_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    int count = 0;
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        if (tubo.gates[i].highlighted)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- today_gate matches highlighted gate --- */

void test_today_gate_matches_highlighted(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_INT(1, tubo.gates[tubo.today_gate].highlighted);
}

/* --- All hk_numbers > 0 --- */

void test_all_hk_numbers_positive(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_TRUE(tubo.gates[i].hk_number > 0);
    }
}

/* --- consciousness not NULL for all gates --- */

void test_consciousness_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_NOT_NULL(tubo.gates[i].consciousness);
    }
}

/* --- mental_sphere not NULL for all gates --- */

void test_mental_sphere_not_null(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_NOT_NULL(tubo.gates[i].mental_sphere);
    }
}

/* --- Different days highlight different gates --- */

void test_different_days_different_gates(void)
{
    /* July 26 = day 1 of week -> gate 0 */
    double jd1 = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t t1 = kin_tubo_compute(jd1);

    /* July 27 = day 2 of week -> gate 1 */
    double jd2 = gregorian_to_jd(2026, 7, 27.0);
    kin_tubo_mauri_t t2 = kin_tubo_compute(jd2);

    TEST_ASSERT_EQUAL_INT(0, t1.today_gate);
    TEST_ASSERT_EQUAL_INT(1, t2.today_gate);
    TEST_ASSERT_NOT_EQUAL(t1.today_gate, t2.today_gate);
}

/* --- Invalid JD returns zeroed --- */

void test_invalid_jd_returns_zeroed(void)
{
    kin_tubo_mauri_t tubo = kin_tubo_compute(-1.0);
    TEST_ASSERT_EQUAL_INT(-1, tubo.today_gate);
    TEST_ASSERT_EQUAL_INT(0, tubo.today_day_of_week);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_EQUAL_INT(0, tubo.gates[i].day_of_week);
        TEST_ASSERT_NULL(tubo.gates[i].plasma);
    }
}

/* --- Day Out of Time returns zeroed --- */

void test_day_out_of_time_returns_zeroed(void)
{
    double jd = gregorian_to_jd(2027, 7, 25.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_INT(-1, tubo.today_gate);
    TEST_ASSERT_EQUAL_INT(0, tubo.today_day_of_week);
}

/* --- Purity: same input same output --- */

void test_purity(void)
{
    double jd = gregorian_to_jd(2026, 8, 1.0);
    kin_tubo_mauri_t t1 = kin_tubo_compute(jd);
    kin_tubo_mauri_t t2 = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_INT(t1.today_gate, t2.today_gate);
    TEST_ASSERT_EQUAL_INT(t1.today_day_of_week, t2.today_day_of_week);
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, t1.gates[i].x, t2.gates[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, t1.gates[i].y, t2.gates[i].y);
        TEST_ASSERT_EQUAL_INT(t1.gates[i].hk_number, t2.gates[i].hk_number);
    }
}

/* --- Gate data values match known Hunab Ku 21 data --- */

void test_gate0_dali_crown(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_STRING("DALI", tubo.gates[0].plasma);
    TEST_ASSERT_EQUAL_STRING("Crown", tubo.gates[0].chakra);
    TEST_ASSERT_EQUAL_STRING("OM", tubo.gates[0].mantra);
    TEST_ASSERT_EQUAL_INT(108, tubo.gates[0].hk_number);
}

void test_gate6_silio_heart(void)
{
    double jd = gregorian_to_jd(2026, 7, 26.0);
    kin_tubo_mauri_t tubo = kin_tubo_compute(jd);
    TEST_ASSERT_EQUAL_STRING("SILIO", tubo.gates[6].plasma);
    TEST_ASSERT_EQUAL_STRING("Heart", tubo.gates[6].chakra);
    TEST_ASSERT_EQUAL_STRING("HRAIM", tubo.gates[6].mantra);
    TEST_ASSERT_EQUAL_INT(441, tubo.gates[6].hk_number);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_jul26_today_gate_0);
    RUN_TEST(test_jul26_today_day_of_week_1);
    RUN_TEST(test_7_gates_sequential);
    RUN_TEST(test_gate0_plasma_not_null);
    RUN_TEST(test_gate0_chakra_not_null);
    RUN_TEST(test_gate0_mantra_not_null);
    RUN_TEST(test_gate0_y_top);
    RUN_TEST(test_gate6_y_bottom);
    RUN_TEST(test_all_gates_x_centered);
    RUN_TEST(test_only_one_highlighted);
    RUN_TEST(test_today_gate_matches_highlighted);
    RUN_TEST(test_all_hk_numbers_positive);
    RUN_TEST(test_consciousness_not_null);
    RUN_TEST(test_mental_sphere_not_null);
    RUN_TEST(test_different_days_different_gates);
    RUN_TEST(test_invalid_jd_returns_zeroed);
    RUN_TEST(test_day_out_of_time_returns_zeroed);
    RUN_TEST(test_purity);
    RUN_TEST(test_gate0_dali_crown);
    RUN_TEST(test_gate6_silio_heart);
    return UNITY_END();
}
