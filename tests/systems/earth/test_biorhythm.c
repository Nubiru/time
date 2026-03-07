/* test_biorhythm.c — Tests for biological rhythms / astronomical correlations.
 * TDD RED phase: all tests written before implementation.
 * Pure data + computation module. No GL, no malloc, no globals. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/biorhythm.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== Cycle Count ===== */

void test_cycle_count_is_8(void) {
    TEST_ASSERT_EQUAL_INT(8, biorhythm_cycle_count());
}

void test_cycle_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(BIORHYTHM_CYCLE_COUNT, biorhythm_cycle_count());
}

/* ===== Rhythm Data Retrieval ===== */

void test_heartbeat_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_HEARTBEAT);
    TEST_ASSERT_EQUAL_INT(BIO_HEARTBEAT, r.type);
    TEST_ASSERT_EQUAL_STRING("Heartbeat", r.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 72.0f, (float)r.rate_per_minute);
    TEST_ASSERT_NOT_NULL(r.description);
}

void test_breath_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_BREATH);
    TEST_ASSERT_EQUAL_INT(BIO_BREATH, r.type);
    TEST_ASSERT_EQUAL_STRING("Breath", r.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 15.0f, (float)r.rate_per_minute);
}

void test_circadian_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_CIRCADIAN);
    TEST_ASSERT_EQUAL_INT(BIO_CIRCADIAN, r.type);
    TEST_ASSERT_EQUAL_STRING("Circadian", r.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)r.period_hours);
}

void test_menstrual_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_MENSTRUAL);
    TEST_ASSERT_EQUAL_INT(BIO_MENSTRUAL, r.type);
    TEST_ASSERT_EQUAL_STRING("Menstrual", r.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 708.0f, (float)r.period_hours);
}

void test_sleep_ultradian_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_SLEEP_ULTRADIAN);
    TEST_ASSERT_EQUAL_INT(BIO_SLEEP_ULTRADIAN, r.type);
    TEST_ASSERT_EQUAL_STRING("Sleep Cycle", r.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, (float)r.period_hours);
}

void test_blink_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_BLINK);
    TEST_ASSERT_EQUAL_INT(BIO_BLINK, r.type);
    TEST_ASSERT_EQUAL_STRING("Blink", r.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 17.0f, (float)r.rate_per_minute);
}

void test_cell_division_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_CELL_DIVISION);
    TEST_ASSERT_EQUAL_INT(BIO_CELL_DIVISION, r.type);
    TEST_ASSERT_EQUAL_STRING("Cell Division", r.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 24.0f, (float)r.period_hours);
}

void test_bone_remodel_rhythm(void) {
    bio_rhythm_t r = biorhythm_get(BIO_BONE_REMODEL);
    TEST_ASSERT_EQUAL_INT(BIO_BONE_REMODEL, r.type);
    TEST_ASSERT_EQUAL_STRING("Bone Remodeling", r.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2880.0f, (float)r.period_hours);
}

/* ===== Cycle Name Lookup ===== */

void test_cycle_name_heartbeat(void) {
    const char *name = biorhythm_cycle_name(BIO_HEARTBEAT);
    TEST_ASSERT_EQUAL_STRING("Heartbeat", name);
}

void test_cycle_name_breath(void) {
    const char *name = biorhythm_cycle_name(BIO_BREATH);
    TEST_ASSERT_EQUAL_STRING("Breath", name);
}

void test_cycle_name_bone_remodel(void) {
    const char *name = biorhythm_cycle_name(BIO_BONE_REMODEL);
    TEST_ASSERT_EQUAL_STRING("Bone Remodeling", name);
}

/* ===== Heartbeats Calculation ===== */

void test_heartbeats_one_hour(void) {
    /* 72 bpm * 60 min = 4320 beats/hour */
    double result = biorhythm_heartbeats(1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 4320.0f, (float)result);
}

void test_heartbeats_24_hours(void) {
    /* 72 * 60 * 24 = 103,680 beats/day */
    double result = biorhythm_heartbeats(24.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 103680.0f, (float)result);
}

void test_heartbeats_zero(void) {
    double result = biorhythm_heartbeats(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)result);
}

/* ===== Breaths Calculation ===== */

void test_breaths_one_hour(void) {
    /* 15/min * 60 = 900 breaths/hour */
    double result = biorhythm_breaths(1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 900.0f, (float)result);
}

void test_breaths_24_hours(void) {
    /* 15 * 60 * 24 = 21,600 breaths/day */
    double result = biorhythm_breaths(24.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 21600.0f, (float)result);
}

/* ===== Beats Per Day ===== */

void test_beats_per_day_72bpm(void) {
    /* 72 * 60 * 24 = 103,680 */
    double result = biorhythm_beats_per_day(72.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 103680.0f, (float)result);
}

void test_beats_per_day_60bpm(void) {
    /* 60 * 60 * 24 = 86,400 */
    double result = biorhythm_beats_per_day(60.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 86400.0f, (float)result);
}

/* ===== Breaths Per Day ===== */

void test_breaths_per_day_15rpm(void) {
    /* 15 * 60 * 24 = 21,600 */
    double result = biorhythm_breaths_per_day(15.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 21600.0f, (float)result);
}

void test_breaths_per_day_12rpm(void) {
    /* 12 * 60 * 24 = 17,280 */
    double result = biorhythm_breaths_per_day(12.0);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 17280.0f, (float)result);
}

/* ===== Correlation Data ===== */

void test_correlation_count_is_6(void) {
    TEST_ASSERT_EQUAL_INT(6, biorhythm_correlation_count());
}

void test_correlation_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(BIORHYTHM_CORRELATION_COUNT, biorhythm_correlation_count());
}

void test_correlation_0_heartbeat_precession(void) {
    bio_correlation_t c = biorhythm_correlation_get(0);
    TEST_ASSERT_NOT_NULL(c.bio_description);
    TEST_ASSERT_NOT_NULL(c.astro_description);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 72.0f, (float)c.bio_value);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 72.0f, (float)c.astro_value);
    TEST_ASSERT_NOT_NULL(c.significance);
}

void test_correlation_1_kali_yuga(void) {
    bio_correlation_t c = biorhythm_correlation_get(1);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 4320.0f, (float)c.bio_value);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 432000.0f, (float)c.astro_value);
}

void test_correlation_2_breaths_precession(void) {
    bio_correlation_t c = biorhythm_correlation_get(2);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 21600.0f, (float)c.bio_value);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 25920.0f, (float)c.astro_value);
}

void test_correlation_3_menstrual_synodic(void) {
    bio_correlation_t c = biorhythm_correlation_get(3);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 29.5f, (float)c.bio_value);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 29.53f, (float)c.astro_value);
}

void test_correlation_4_beats_day_precession(void) {
    bio_correlation_t c = biorhythm_correlation_get(4);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 103680.0f, (float)c.bio_value);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 103680.0f, (float)c.astro_value);
}

void test_correlation_5_breaths_mala(void) {
    bio_correlation_t c = biorhythm_correlation_get(5);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 900.0f, (float)c.bio_value);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 108.0f, (float)c.astro_value);
}

/* ===== Sacred Number Matching ===== */

void test_match_sacred_exact_72(void) {
    /* 72 / 1 = 72, exactly a sacred number */
    int result = biorhythm_match_sacred(72.0, 1.0);
    TEST_ASSERT_EQUAL_INT(72, result);
}

void test_match_sacred_exact_108(void) {
    /* 108 / 1 = 108 */
    int result = biorhythm_match_sacred(108.0, 1.0);
    TEST_ASSERT_EQUAL_INT(108, result);
}

void test_match_sacred_ratio_12(void) {
    /* 360 / 30 = 12 */
    int result = biorhythm_match_sacred(360.0, 30.0);
    TEST_ASSERT_EQUAL_INT(12, result);
}

void test_match_sacred_direct_match(void) {
    /* 29.5 / 29.53 ~ 0.999, within 5% of 1.0 */
    int result = biorhythm_match_sacred(29.5, 29.53);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_match_sacred_inverse_used(void) {
    /* 1 / 72 < 1, so invert to 72 */
    int result = biorhythm_match_sacred(1.0, 72.0);
    TEST_ASSERT_EQUAL_INT(72, result);
}

void test_match_sacred_no_match(void) {
    /* 17 / 3 = 5.667 — not close to any sacred number */
    int result = biorhythm_match_sacred(17.0, 3.0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_match_sacred_within_5_percent(void) {
    /* 70 / 1 = 70, within 5% of 72 (72*0.95=68.4, 72*1.05=75.6) */
    int result = biorhythm_match_sacred(70.0, 1.0);
    TEST_ASSERT_EQUAL_INT(72, result);
}

void test_match_sacred_432(void) {
    /* 432 / 1 = 432 */
    int result = biorhythm_match_sacred(432.0, 1.0);
    TEST_ASSERT_EQUAL_INT(432, result);
}

/* ===== Lifetime Calculations ===== */

void test_lifetime_heartbeats_80_years(void) {
    /* 80 * 365.25 * 24 * 60 * 72 = ~3,027,456,000 */
    double result = biorhythm_lifetime_heartbeats(80.0);
    double expected = 80.0 * 365.25 * 24.0 * 60.0 * 72.0;
    /* Use relative tolerance for large numbers */
    TEST_ASSERT_FLOAT_WITHIN((float)(expected * 0.001), (float)expected, (float)result);
}

void test_lifetime_heartbeats_1_year(void) {
    /* 1 * 365.25 * 24 * 60 * 72 = 37,843,200 */
    double result = biorhythm_lifetime_heartbeats(1.0);
    double expected = 365.25 * 24.0 * 60.0 * 72.0;
    TEST_ASSERT_FLOAT_WITHIN((float)(expected * 0.001), (float)expected, (float)result);
}

void test_lifetime_breaths_80_years(void) {
    /* 80 * 365.25 * 24 * 60 * 15 = ~631,152,000 */
    double result = biorhythm_lifetime_breaths(80.0);
    double expected = 80.0 * 365.25 * 24.0 * 60.0 * 15.0;
    TEST_ASSERT_FLOAT_WITHIN((float)(expected * 0.001), (float)expected, (float)result);
}

void test_lifetime_breaths_zero(void) {
    double result = biorhythm_lifetime_breaths(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)result);
}

/* ===== All rhythm descriptions are non-null ===== */

void test_all_rhythms_have_descriptions(void) {
    for (int i = 0; i < biorhythm_cycle_count(); i++) {
        bio_rhythm_t r = biorhythm_get((bio_cycle_t)i);
        TEST_ASSERT_NOT_NULL(r.description);
        TEST_ASSERT_TRUE(strlen(r.description) > 0);
    }
}

/* ===== All correlations have significance ===== */

void test_all_correlations_have_significance(void) {
    for (int i = 0; i < biorhythm_correlation_count(); i++) {
        bio_correlation_t c = biorhythm_correlation_get(i);
        TEST_ASSERT_NOT_NULL(c.significance);
        TEST_ASSERT_TRUE(strlen(c.significance) > 0);
    }
}

/* ===== All correlations have ratio set ===== */

void test_all_correlations_have_ratio(void) {
    for (int i = 0; i < biorhythm_correlation_count(); i++) {
        bio_correlation_t c = biorhythm_correlation_get(i);
        TEST_ASSERT_TRUE(c.ratio > 0.0);
    }
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* Cycle count */
    RUN_TEST(test_cycle_count_is_8);
    RUN_TEST(test_cycle_count_matches_define);

    /* Rhythm data */
    RUN_TEST(test_heartbeat_rhythm);
    RUN_TEST(test_breath_rhythm);
    RUN_TEST(test_circadian_rhythm);
    RUN_TEST(test_menstrual_rhythm);
    RUN_TEST(test_sleep_ultradian_rhythm);
    RUN_TEST(test_blink_rhythm);
    RUN_TEST(test_cell_division_rhythm);
    RUN_TEST(test_bone_remodel_rhythm);

    /* Cycle name lookup */
    RUN_TEST(test_cycle_name_heartbeat);
    RUN_TEST(test_cycle_name_breath);
    RUN_TEST(test_cycle_name_bone_remodel);

    /* Heartbeats */
    RUN_TEST(test_heartbeats_one_hour);
    RUN_TEST(test_heartbeats_24_hours);
    RUN_TEST(test_heartbeats_zero);

    /* Breaths */
    RUN_TEST(test_breaths_one_hour);
    RUN_TEST(test_breaths_24_hours);

    /* Beats per day */
    RUN_TEST(test_beats_per_day_72bpm);
    RUN_TEST(test_beats_per_day_60bpm);

    /* Breaths per day */
    RUN_TEST(test_breaths_per_day_15rpm);
    RUN_TEST(test_breaths_per_day_12rpm);

    /* Correlations */
    RUN_TEST(test_correlation_count_is_6);
    RUN_TEST(test_correlation_count_matches_define);
    RUN_TEST(test_correlation_0_heartbeat_precession);
    RUN_TEST(test_correlation_1_kali_yuga);
    RUN_TEST(test_correlation_2_breaths_precession);
    RUN_TEST(test_correlation_3_menstrual_synodic);
    RUN_TEST(test_correlation_4_beats_day_precession);
    RUN_TEST(test_correlation_5_breaths_mala);

    /* Sacred number matching */
    RUN_TEST(test_match_sacred_exact_72);
    RUN_TEST(test_match_sacred_exact_108);
    RUN_TEST(test_match_sacred_ratio_12);
    RUN_TEST(test_match_sacred_direct_match);
    RUN_TEST(test_match_sacred_inverse_used);
    RUN_TEST(test_match_sacred_no_match);
    RUN_TEST(test_match_sacred_within_5_percent);
    RUN_TEST(test_match_sacred_432);

    /* Lifetime calculations */
    RUN_TEST(test_lifetime_heartbeats_80_years);
    RUN_TEST(test_lifetime_heartbeats_1_year);
    RUN_TEST(test_lifetime_breaths_80_years);
    RUN_TEST(test_lifetime_breaths_zero);

    /* Sweep tests */
    RUN_TEST(test_all_rhythms_have_descriptions);
    RUN_TEST(test_all_correlations_have_significance);
    RUN_TEST(test_all_correlations_have_ratio);

    return UNITY_END();
}
