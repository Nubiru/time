#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/cr_cycle.h"
#include "../../../src/systems/tzolkin/haab.h"
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Reference JD: 2460000.5 = 2023-02-25 (known date for verification) */
#define REF_JD 2460000.5
#define GMT_CORRELATION 584283L

/* Classic Maya Tzolkin from day count (matches cr_cycle internals).
 * Anchor: LC 0.0.0.0.0 = 4 Ahau (tone 4, seal 19). */
static int test_classic_seal(double jd)
{
    long dc = (long)floor(jd + 0.5) - GMT_CORRELATION;
    return (int)(((dc % 20) + 20 + 19) % 20);
}

static int test_classic_tone(double jd)
{
    long dc = (long)floor(jd + 0.5) - GMT_CORRELATION;
    return (int)(((dc % 13) + 13 + 3) % 13) + 1;
}

/* --- Basic cycle position --- */

static void test_day_position_in_range(void)
{
    int pos = cr_cycle_day(REF_JD);
    TEST_ASSERT_TRUE(pos >= 0);
    TEST_ASSERT_TRUE(pos < CR_CYCLE_DAYS);
}

static void test_day_advances_by_one(void)
{
    int pos1 = cr_cycle_day(REF_JD);
    int pos2 = cr_cycle_day(REF_JD + 1.0);
    TEST_ASSERT_EQUAL_INT((pos1 + 1) % CR_CYCLE_DAYS, pos2);
}

static void test_cycle_wraps_at_18980(void)
{
    int pos1 = cr_cycle_day(REF_JD);
    int pos2 = cr_cycle_day(REF_JD + 18980.0);
    TEST_ASSERT_EQUAL_INT(pos1, pos2);
}

static void test_year_in_range(void)
{
    int year = cr_cycle_year(REF_JD);
    TEST_ASSERT_TRUE(year >= 0);
    TEST_ASSERT_TRUE(year < CR_CYCLE_YEARS);
}

static void test_year_consistent_with_day(void)
{
    int pos = cr_cycle_day(REF_JD);
    int year = cr_cycle_year(REF_JD);
    TEST_ASSERT_EQUAL_INT(pos / CR_YEAR_DAYS, year);
}

/* --- cr_cycle_from_jd --- */

static void test_from_jd_fields_consistent(void)
{
    cr_cycle_t cr = cr_cycle_from_jd(REF_JD);
    TEST_ASSERT_EQUAL_INT(cr.day_in_round / CR_YEAR_DAYS, cr.year_in_round);
    TEST_ASSERT_EQUAL_INT(cr.day_in_round % CR_YEAR_DAYS, cr.day_in_year);
    TEST_ASSERT_TRUE(cr.day_in_round >= 0 && cr.day_in_round < CR_CYCLE_DAYS);
    TEST_ASSERT_TRUE(cr.year_in_round >= 0 && cr.year_in_round < CR_CYCLE_YEARS);
    TEST_ASSERT_TRUE(cr.day_in_year >= 0 && cr.day_in_year < CR_YEAR_DAYS);
}

static void test_from_jd_year_bearer_valid(void)
{
    cr_cycle_t cr = cr_cycle_from_jd(REF_JD);
    TEST_ASSERT_TRUE(cr.year_bearer_seal >= 0 && cr.year_bearer_seal <= 19);
    TEST_ASSERT_TRUE(cr.year_bearer_tone >= 1 && cr.year_bearer_tone <= 13);
}

static void test_from_jd_same_day_same_bearer(void)
{
    /* Two dates in the same Haab year should have the same year bearer */
    cr_cycle_t cr1 = cr_cycle_from_jd(REF_JD);
    cr_cycle_t cr2 = cr_cycle_from_jd(REF_JD + 1.0);
    if (cr1.year_in_round == cr2.year_in_round) {
        TEST_ASSERT_EQUAL_INT(cr1.year_bearer_seal, cr2.year_bearer_seal);
        TEST_ASSERT_EQUAL_INT(cr1.year_bearer_tone, cr2.year_bearer_tone);
    }
}

/* --- Year bearer properties --- */

static void test_bearer_seal_advances_by_5(void)
{
    /* Seal advances by 5 mod 20 per year (365 mod 20 = 5) */
    int seal0 = cr_cycle_year_bearer_seal(REF_JD, 0);
    int seal1 = cr_cycle_year_bearer_seal(REF_JD, 1);
    TEST_ASSERT_EQUAL_INT((seal0 + 5) % 20, seal1);
}

static void test_bearer_tone_advances_by_1(void)
{
    /* Tone advances by 1 per year (365 mod 13 = 1) */
    int tone0 = cr_cycle_year_bearer_tone(REF_JD, 0);
    int tone1 = cr_cycle_year_bearer_tone(REF_JD, 1);
    int expected = (tone0 % 13) + 1; /* tone 1-13 wraps */
    TEST_ASSERT_EQUAL_INT(expected, tone1);
}

static void test_only_4_bearer_seals(void)
{
    /* Over 52 years, only 4 distinct seals appear as year bearers */
    int seen[20] = {0};
    for (int y = 0; y < CR_CYCLE_YEARS; y++) {
        int s = cr_cycle_year_bearer_seal(REF_JD, y);
        seen[s] = 1;
    }
    int distinct = 0;
    for (int i = 0; i < 20; i++)
        if (seen[i]) distinct++;
    TEST_ASSERT_EQUAL_INT(4, distinct);
}

static void test_bearer_seals_spaced_by_5(void)
{
    /* The 4 bearer seals are spaced 5 apart */
    int s0 = cr_cycle_year_bearer_seal(REF_JD, 0);
    int s1 = cr_cycle_year_bearer_seal(REF_JD, 1);
    int s2 = cr_cycle_year_bearer_seal(REF_JD, 2);
    int s3 = cr_cycle_year_bearer_seal(REF_JD, 3);
    TEST_ASSERT_EQUAL_INT((s0 + 5) % 20, s1);
    TEST_ASSERT_EQUAL_INT((s1 + 5) % 20, s2);
    TEST_ASSERT_EQUAL_INT((s2 + 5) % 20, s3);
    TEST_ASSERT_EQUAL_INT(s0, cr_cycle_year_bearer_seal(REF_JD, 4));
}

static void test_bearer_seal_invalid_year(void)
{
    TEST_ASSERT_EQUAL_INT(-1, cr_cycle_year_bearer_seal(REF_JD, -1));
    TEST_ASSERT_EQUAL_INT(-1, cr_cycle_year_bearer_seal(REF_JD, 52));
}

static void test_bearer_tone_invalid_year(void)
{
    TEST_ASSERT_EQUAL_INT(0, cr_cycle_year_bearer_tone(REF_JD, -1));
    TEST_ASSERT_EQUAL_INT(0, cr_cycle_year_bearer_tone(REF_JD, 52));
}

/* --- New round detection --- */

static void test_is_new_at_round_start(void)
{
    /* Find a round start from REF_JD */
    int pos = cr_cycle_day(REF_JD);
    double start_jd = REF_JD - (double)pos;
    TEST_ASSERT_TRUE(cr_cycle_is_new(start_jd));
}

static void test_is_not_new_mid_round(void)
{
    int pos = cr_cycle_day(REF_JD);
    if (pos != 0) {
        TEST_ASSERT_FALSE(cr_cycle_is_new(REF_JD));
    }
}

/* --- Next start --- */

static void test_next_start_is_day_0(void)
{
    double next = cr_cycle_next_start(REF_JD);
    TEST_ASSERT_TRUE(cr_cycle_is_new(next));
}

static void test_next_start_within_one_round(void)
{
    double next = cr_cycle_next_start(REF_JD);
    double diff = next - (int)(REF_JD + 0.5);
    TEST_ASSERT_TRUE(diff > 0);
    TEST_ASSERT_TRUE(diff <= CR_CYCLE_DAYS);
}

/* --- Days remaining --- */

static void test_days_remaining_range(void)
{
    int rem = cr_cycle_days_remaining(REF_JD);
    TEST_ASSERT_TRUE(rem >= 0);
    TEST_ASSERT_TRUE(rem < CR_CYCLE_DAYS);
}

static void test_days_remaining_plus_position(void)
{
    int pos = cr_cycle_day(REF_JD);
    int rem = cr_cycle_days_remaining(REF_JD);
    TEST_ASSERT_EQUAL_INT(CR_CYCLE_DAYS - 1, pos + rem);
}

/* --- Flower visualization --- */

static void test_flower_angle_range(void)
{
    double angle = cr_cycle_flower_angle(REF_JD);
    TEST_ASSERT_TRUE(angle >= 0.0);
    TEST_ASSERT_TRUE(angle < 360.0);
}

static void test_flower_angle_proportional(void)
{
    /* Halfway through round = 180 degrees */
    int pos = cr_cycle_day(REF_JD);
    double start_jd = REF_JD - (double)pos;
    double mid_jd = start_jd + CR_CYCLE_DAYS / 2.0;
    double angle = cr_cycle_flower_angle(mid_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 180.0f, (float)angle);
}

static void test_flower_petal_range(void)
{
    int petal = cr_cycle_flower_petal(REF_JD);
    TEST_ASSERT_TRUE(petal >= 0);
    TEST_ASSERT_TRUE(petal < CR_CYCLE_YEARS);
}

static void test_flower_petal_equals_year(void)
{
    int petal = cr_cycle_flower_petal(REF_JD);
    int year = cr_cycle_year(REF_JD);
    TEST_ASSERT_EQUAL_INT(year, petal);
}

static void test_petal_progress_range(void)
{
    double prog = cr_cycle_petal_progress(REF_JD);
    TEST_ASSERT_TRUE(prog >= 0.0);
    TEST_ASSERT_TRUE(prog < 1.0);
}

static void test_petal_progress_at_year_start(void)
{
    int pos = cr_cycle_day(REF_JD);
    int year = pos / CR_YEAR_DAYS;
    double year_start = REF_JD - (double)(pos - year * CR_YEAR_DAYS);
    double prog = cr_cycle_petal_progress(year_start);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)prog);
}

/* --- Next occurrence --- */

static void test_next_occurrence_finds_match(void)
{
    /* Get Classic Tzolkin + Haab at REF_JD, search from REF_JD */
    int tone = test_classic_tone(REF_JD);
    int seal = test_classic_seal(REF_JD);
    haab_date_t h = haab_from_jd(REF_JD);
    double next = cr_cycle_next_occurrence(REF_JD, tone, seal,
                                            h.month, h.day);
    /* Should find it exactly 18,980 days later */
    double expected = (double)((long)(REF_JD + 0.5) + CR_CYCLE_DAYS);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, (float)expected, (float)next);
}

static void test_next_occurrence_invalid_tone(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f,
        (float)cr_cycle_next_occurrence(REF_JD, 0, 0, 0, 0));
}

static void test_next_occurrence_invalid_seal(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f,
        (float)cr_cycle_next_occurrence(REF_JD, 1, 20, 0, 0));
}

static void test_next_occurrence_invalid_haab(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f,
        (float)cr_cycle_next_occurrence(REF_JD, 1, 0, 19, 0));
}

/* --- Full 52-year sweep --- */

static void test_52_years_cover_18980_days(void)
{
    /* 52 * 365 = 18980 */
    TEST_ASSERT_EQUAL_INT(CR_CYCLE_DAYS, CR_CYCLE_YEARS * CR_YEAR_DAYS);
}

static void test_73_tzolkin_cycles_in_round(void)
{
    /* 73 * 260 = 18980 */
    TEST_ASSERT_EQUAL_INT(CR_CYCLE_DAYS, 73 * 260);
}

static void test_all_52_years_reachable(void)
{
    /* Walk through 52 years starting from round start */
    int pos = cr_cycle_day(REF_JD);
    double start = REF_JD - (double)pos;
    for (int y = 0; y < CR_CYCLE_YEARS; y++) {
        double jd = start + y * CR_YEAR_DAYS;
        int year = cr_cycle_year(jd);
        TEST_ASSERT_EQUAL_INT(y, year);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Basic position */
    RUN_TEST(test_day_position_in_range);
    RUN_TEST(test_day_advances_by_one);
    RUN_TEST(test_cycle_wraps_at_18980);
    RUN_TEST(test_year_in_range);
    RUN_TEST(test_year_consistent_with_day);

    /* from_jd */
    RUN_TEST(test_from_jd_fields_consistent);
    RUN_TEST(test_from_jd_year_bearer_valid);
    RUN_TEST(test_from_jd_same_day_same_bearer);

    /* Year bearer */
    RUN_TEST(test_bearer_seal_advances_by_5);
    RUN_TEST(test_bearer_tone_advances_by_1);
    RUN_TEST(test_only_4_bearer_seals);
    RUN_TEST(test_bearer_seals_spaced_by_5);
    RUN_TEST(test_bearer_seal_invalid_year);
    RUN_TEST(test_bearer_tone_invalid_year);

    /* New round */
    RUN_TEST(test_is_new_at_round_start);
    RUN_TEST(test_is_not_new_mid_round);

    /* Next start */
    RUN_TEST(test_next_start_is_day_0);
    RUN_TEST(test_next_start_within_one_round);

    /* Days remaining */
    RUN_TEST(test_days_remaining_range);
    RUN_TEST(test_days_remaining_plus_position);

    /* Flower */
    RUN_TEST(test_flower_angle_range);
    RUN_TEST(test_flower_angle_proportional);
    RUN_TEST(test_flower_petal_range);
    RUN_TEST(test_flower_petal_equals_year);
    RUN_TEST(test_petal_progress_range);
    RUN_TEST(test_petal_progress_at_year_start);

    /* Next occurrence */
    RUN_TEST(test_next_occurrence_finds_match);
    RUN_TEST(test_next_occurrence_invalid_tone);
    RUN_TEST(test_next_occurrence_invalid_seal);
    RUN_TEST(test_next_occurrence_invalid_haab);

    /* Sweep */
    RUN_TEST(test_52_years_cover_18980_days);
    RUN_TEST(test_73_tzolkin_cycles_in_round);
    RUN_TEST(test_all_52_years_reachable);

    return UNITY_END();
}
