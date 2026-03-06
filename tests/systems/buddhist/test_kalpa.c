/* test_kalpa.c — Buddhist cosmic time cycle tests (Track 16.2)
 * Covers: kalpa_position, phase names/meanings, antarakalpa info,
 *         geological mapping, cosmic age mapping, durations, ratios,
 *         boundary conditions, struct integrity. */

#include "../../unity/unity.h"
#include "../../../src/systems/buddhist/kalpa.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants verification ===== */

void test_antarakalpa_years(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1.0, 16800000.0, BUDDHIST_ANTARAKALPA_YEARS);
}

void test_kalpa_years(void)
{
    /* 20 antarakalpas = 336 million years */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 336000000.0, BUDDHIST_KALPA_YEARS);
}

void test_maha_kalpa_years(void)
{
    /* 4 kalpas = 1.344 billion years */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 1344000000.0, BUDDHIST_MAHA_KALPA_YEARS);
}

void test_kalpa_antarakalpas(void)
{
    TEST_ASSERT_EQUAL_INT(20, BUDDHIST_KALPA_ANTARAKALPAS);
}

void test_maha_kalpa_kalpas(void)
{
    TEST_ASSERT_EQUAL_INT(4, BUDDHIST_MAHA_KALPA_KALPAS);
}

/* ===== Phase count and duration ===== */

void test_phase_count(void)
{
    TEST_ASSERT_EQUAL_INT(4, kalpa_phase_count());
}

void test_phase_duration(void)
{
    /* Each phase = 1 kalpa = 336 million years */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 336000000.0, kalpa_phase_duration());
}

void test_maha_kalpa_duration(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1.0, 1344000000.0, kalpa_maha_kalpa_duration());
}

void test_antarakalpa_duration(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1.0, 16800000.0, kalpa_antarakalpa_duration());
}

/* ===== Phase names ===== */

void test_phase_name_formation(void)
{
    TEST_ASSERT_EQUAL_STRING("Formation", kalpa_phase_name(KALPA_PHASE_FORMATION));
}

void test_phase_name_existence(void)
{
    TEST_ASSERT_EQUAL_STRING("Existence", kalpa_phase_name(KALPA_PHASE_EXISTENCE));
}

void test_phase_name_destruction(void)
{
    TEST_ASSERT_EQUAL_STRING("Destruction", kalpa_phase_name(KALPA_PHASE_DESTRUCTION));
}

void test_phase_name_emptiness(void)
{
    TEST_ASSERT_EQUAL_STRING("Emptiness", kalpa_phase_name(KALPA_PHASE_EMPTINESS));
}

void test_phase_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kalpa_phase_name(KALPA_PHASE_COUNT));
}

void test_phase_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kalpa_phase_name((kalpa_phase_t)-1));
}

/* ===== Phase meanings (Pali names) ===== */

void test_phase_meaning_formation(void)
{
    TEST_ASSERT_EQUAL_STRING("Vivatta-kappa", kalpa_phase_meaning(KALPA_PHASE_FORMATION));
}

void test_phase_meaning_existence(void)
{
    TEST_ASSERT_EQUAL_STRING("Vivatta-tthayi", kalpa_phase_meaning(KALPA_PHASE_EXISTENCE));
}

void test_phase_meaning_destruction(void)
{
    TEST_ASSERT_EQUAL_STRING("Samvatta-kappa", kalpa_phase_meaning(KALPA_PHASE_DESTRUCTION));
}

void test_phase_meaning_emptiness(void)
{
    TEST_ASSERT_EQUAL_STRING("Samvatta-tthayi", kalpa_phase_meaning(KALPA_PHASE_EMPTINESS));
}

void test_phase_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kalpa_phase_meaning(KALPA_PHASE_COUNT));
}

/* ===== kalpa_position from cosmic age ===== */

void test_position_zero_age(void)
{
    kalpa_position_t p = kalpa_position(0.0);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
    TEST_ASSERT_EQUAL_INT(0, p.antarakalpa_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.phase_fraction);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.maha_kalpa_fraction);
}

void test_position_mid_formation(void)
{
    /* Half of formation phase = 168 million years */
    double age = 168000000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.5, p.phase_fraction);
}

void test_position_existence_phase(void)
{
    /* Start of existence: 336 million years into maha-kalpa */
    double age = 336000000.0 + 1000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_EXISTENCE, p.phase);
}

void test_position_destruction_phase(void)
{
    /* Destruction starts at 2 * 336M = 672M */
    double age = 672000000.0 + 1000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_DESTRUCTION, p.phase);
}

void test_position_emptiness_phase(void)
{
    /* Emptiness starts at 3 * 336M = 1008M */
    double age = 1008000000.0 + 1000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_EMPTINESS, p.phase);
}

void test_position_second_maha_kalpa(void)
{
    /* Second maha-kalpa starts at 1.344 billion years */
    double age = BUDDHIST_MAHA_KALPA_YEARS + 1000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_EQUAL_INT(1, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
}

void test_position_years_in_phase(void)
{
    double age = 168000000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 168000000.0, p.years_in_phase);
}

void test_position_maha_kalpa_fraction(void)
{
    /* At 672M = halfway through maha-kalpa */
    double age = 672000000.0;
    kalpa_position_t p = kalpa_position(age);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.5, p.maha_kalpa_fraction);
}

/* ===== Antarakalpa info ===== */

void test_antarakalpa_odd_decreasing(void)
{
    /* Odd antarakalpas: decreasing lifespan */
    antarakalpa_info_t info = kalpa_antarakalpa_info(1, 0.0);
    TEST_ASSERT_FALSE(info.is_increasing);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 84000.0, info.max_lifespan_years);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 10.0, info.min_lifespan_years);
}

void test_antarakalpa_even_increasing(void)
{
    /* Even antarakalpas: increasing lifespan */
    antarakalpa_info_t info = kalpa_antarakalpa_info(2, 0.0);
    TEST_ASSERT_TRUE(info.is_increasing);
}

void test_antarakalpa_zero_increasing(void)
{
    /* Antarakalpa 0 is even -> increasing */
    antarakalpa_info_t info = kalpa_antarakalpa_info(0, 0.5);
    TEST_ASSERT_TRUE(info.is_increasing);
}

void test_antarakalpa_decreasing_start(void)
{
    /* Odd, fraction=0.0 -> start at max lifespan */
    antarakalpa_info_t info = kalpa_antarakalpa_info(1, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 84000.0, info.current_lifespan);
}

void test_antarakalpa_decreasing_end(void)
{
    /* Odd, fraction=1.0 -> end at min lifespan */
    antarakalpa_info_t info = kalpa_antarakalpa_info(1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 10.0, info.current_lifespan);
}

void test_antarakalpa_increasing_start(void)
{
    /* Even, fraction=0.0 -> start at min lifespan */
    antarakalpa_info_t info = kalpa_antarakalpa_info(2, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 10.0, info.current_lifespan);
}

void test_antarakalpa_increasing_end(void)
{
    /* Even, fraction=1.0 -> end at max lifespan */
    antarakalpa_info_t info = kalpa_antarakalpa_info(2, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 84000.0, info.current_lifespan);
}

void test_antarakalpa_mid_interpolation(void)
{
    /* Odd, fraction=0.5 -> midpoint between 84000 and 10 */
    antarakalpa_info_t info = kalpa_antarakalpa_info(1, 0.5);
    double expected = 10.0 + (84000.0 - 10.0) * 0.5;
    TEST_ASSERT_FLOAT_WITHIN(1.0, expected, info.current_lifespan);
}

void test_antarakalpa_number_preserved(void)
{
    antarakalpa_info_t info = kalpa_antarakalpa_info(7, 0.3);
    TEST_ASSERT_EQUAL_INT(7, info.antarakalpa_number);
}

/* ===== Geological mapping ===== */

void test_from_geological_present(void)
{
    /* ma=0 -> cosmic_age = 4.54 billion years */
    kalpa_position_t p = kalpa_from_geological(0.0);
    /* 4.54e9 / BUDDHIST_MAHA_KALPA_YEARS = 3.378... -> maha_kalpa 3 */
    TEST_ASSERT_EQUAL_INT(3, p.maha_kalpa_number);
}

void test_from_geological_earth_formation(void)
{
    /* ma=4540 -> cosmic_age = 0 years (Earth just forming) */
    kalpa_position_t p = kalpa_from_geological(4540.0);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.phase_fraction);
}

void test_from_geological_dinosaur_extinction(void)
{
    /* ma=66 -> cosmic_age = (4540-66)*1e6 = 4.474 billion years */
    kalpa_position_t p = kalpa_from_geological(66.0);
    TEST_ASSERT_TRUE(p.maha_kalpa_number >= 3);
}

/* ===== Cosmic age mapping ===== */

void test_from_cosmic_age_big_bang(void)
{
    kalpa_position_t p = kalpa_from_cosmic_age(0.0);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
}

void test_from_cosmic_age_13_8_billion(void)
{
    /* 13.8 billion years / BUDDHIST_MAHA_KALPA_YEARS = 10.27... */
    kalpa_position_t p = kalpa_from_cosmic_age(13.8e9);
    TEST_ASSERT_EQUAL_INT(10, p.maha_kalpa_number);
}

void test_from_cosmic_age_one_maha_kalpa(void)
{
    kalpa_position_t p = kalpa_from_cosmic_age(BUDDHIST_MAHA_KALPA_YEARS);
    TEST_ASSERT_EQUAL_INT(1, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
}

/* ===== Cosmic ratio ===== */

void test_cosmic_ratio(void)
{
    /* BUDDHIST_MAHA_KALPA_YEARS / 13.8e9 ~ 0.0974 */
    double ratio = kalpa_cosmic_ratio();
    TEST_ASSERT_FLOAT_WITHIN(0.001, BUDDHIST_MAHA_KALPA_YEARS / 13.8e9, ratio);
}

void test_cosmic_ratio_range(void)
{
    double ratio = kalpa_cosmic_ratio();
    TEST_ASSERT_TRUE(ratio > 0.09);
    TEST_ASSERT_TRUE(ratio < 0.10);
}

/* ===== Negative input safety ===== */

void test_position_negative_age(void)
{
    /* Negative cosmic age should clamp to 0 */
    kalpa_position_t p = kalpa_position(-100.0);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, p.phase);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.phase_fraction);
}

void test_from_geological_beyond_earth(void)
{
    /* ma > 4540 should clamp to cosmic_age 0 */
    kalpa_position_t p = kalpa_from_geological(5000.0);
    TEST_ASSERT_EQUAL_INT(0, p.maha_kalpa_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, p.phase_fraction);
}

void test_antarakalpa_fraction_clamped(void)
{
    /* fraction > 1.0 should clamp */
    antarakalpa_info_t info = kalpa_antarakalpa_info(1, 2.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 10.0, info.current_lifespan);
}

void test_antarakalpa_fraction_negative_clamped(void)
{
    /* fraction < 0.0 should clamp */
    antarakalpa_info_t info = kalpa_antarakalpa_info(1, -1.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 84000.0, info.current_lifespan);
}

/* ===== Main runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Constants (5 tests) */
    RUN_TEST(test_antarakalpa_years);
    RUN_TEST(test_kalpa_years);
    RUN_TEST(test_maha_kalpa_years);
    RUN_TEST(test_kalpa_antarakalpas);
    RUN_TEST(test_maha_kalpa_kalpas);

    /* Phase count and duration (4 tests) */
    RUN_TEST(test_phase_count);
    RUN_TEST(test_phase_duration);
    RUN_TEST(test_maha_kalpa_duration);
    RUN_TEST(test_antarakalpa_duration);

    /* Phase names (6 tests) */
    RUN_TEST(test_phase_name_formation);
    RUN_TEST(test_phase_name_existence);
    RUN_TEST(test_phase_name_destruction);
    RUN_TEST(test_phase_name_emptiness);
    RUN_TEST(test_phase_name_invalid);
    RUN_TEST(test_phase_name_negative);

    /* Phase meanings (5 tests) */
    RUN_TEST(test_phase_meaning_formation);
    RUN_TEST(test_phase_meaning_existence);
    RUN_TEST(test_phase_meaning_destruction);
    RUN_TEST(test_phase_meaning_emptiness);
    RUN_TEST(test_phase_meaning_invalid);

    /* Position from cosmic age (8 tests) */
    RUN_TEST(test_position_zero_age);
    RUN_TEST(test_position_mid_formation);
    RUN_TEST(test_position_existence_phase);
    RUN_TEST(test_position_destruction_phase);
    RUN_TEST(test_position_emptiness_phase);
    RUN_TEST(test_position_second_maha_kalpa);
    RUN_TEST(test_position_years_in_phase);
    RUN_TEST(test_position_maha_kalpa_fraction);

    /* Antarakalpa info (9 tests) */
    RUN_TEST(test_antarakalpa_odd_decreasing);
    RUN_TEST(test_antarakalpa_even_increasing);
    RUN_TEST(test_antarakalpa_zero_increasing);
    RUN_TEST(test_antarakalpa_decreasing_start);
    RUN_TEST(test_antarakalpa_decreasing_end);
    RUN_TEST(test_antarakalpa_increasing_start);
    RUN_TEST(test_antarakalpa_increasing_end);
    RUN_TEST(test_antarakalpa_mid_interpolation);
    RUN_TEST(test_antarakalpa_number_preserved);

    /* Geological mapping (3 tests) */
    RUN_TEST(test_from_geological_present);
    RUN_TEST(test_from_geological_earth_formation);
    RUN_TEST(test_from_geological_dinosaur_extinction);

    /* Cosmic age mapping (3 tests) */
    RUN_TEST(test_from_cosmic_age_big_bang);
    RUN_TEST(test_from_cosmic_age_13_8_billion);
    RUN_TEST(test_from_cosmic_age_one_maha_kalpa);

    /* Cosmic ratio (2 tests) */
    RUN_TEST(test_cosmic_ratio);
    RUN_TEST(test_cosmic_ratio_range);

    /* Safety / boundary (4 tests) */
    RUN_TEST(test_position_negative_age);
    RUN_TEST(test_from_geological_beyond_earth);
    RUN_TEST(test_antarakalpa_fraction_clamped);
    RUN_TEST(test_antarakalpa_fraction_negative_clamped);

    return UNITY_END();
}
