#include "../unity/unity.h"
#include "../../src/systems/unified/cycle_analysis.h"
#include <limits.h>
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== 1. GCD basic ===== */

void test_gcd_basic(void)
{
    TEST_ASSERT_EQUAL_INT64(5, cycle_gcd(10, 15));
}

/* ===== 2. GCD coprime ===== */

void test_gcd_coprime(void)
{
    TEST_ASSERT_EQUAL_INT64(1, cycle_gcd(7, 12));
}

/* ===== 3. GCD same number ===== */

void test_gcd_same(void)
{
    TEST_ASSERT_EQUAL_INT64(42, cycle_gcd(42, 42));
}

/* ===== 4. GCD one is 1 ===== */

void test_gcd_one(void)
{
    TEST_ASSERT_EQUAL_INT64(1, cycle_gcd(1, 999));
}

/* ===== 5. GCD large numbers ===== */

void test_gcd_large(void)
{
    /* GCD(260, 365) = 5 — Tzolkin and Haab share factor 5 */
    TEST_ASSERT_EQUAL_INT64(5, cycle_gcd(260, 365));
}

/* ===== 6. GCD Nakshatras and zodiac share factor 3 ===== */

void test_gcd_nakshatra_zodiac(void)
{
    TEST_ASSERT_EQUAL_INT64(3, cycle_gcd(27, 12));
}

/* ===== 7. LCM basic ===== */

void test_lcm_basic(void)
{
    TEST_ASSERT_EQUAL_INT64(30, cycle_lcm(10, 15));
}

/* ===== 8. LCM coprime ===== */

void test_lcm_coprime(void)
{
    TEST_ASSERT_EQUAL_INT64(84, cycle_lcm(7, 12));
}

/* ===== 9. LCM Calendar Round: 260 * 365 / 5 = 18980 ===== */

void test_lcm_calendar_round(void)
{
    TEST_ASSERT_EQUAL_INT64(18980, cycle_lcm(260, 365));
}

/* ===== 10. LCM overflow returns 0 ===== */

void test_lcm_overflow(void)
{
    /* Two large coprime numbers whose LCM overflows long long */
    long long big_a = 999999999999999LL;
    long long big_b = 999999999999998LL;
    /* These are coprime; LCM = a * b which overflows */
    TEST_ASSERT_EQUAL_INT64(0, cycle_lcm(big_a, big_b));
}

/* ===== 11. GCD array ===== */

void test_gcd_array_basic(void)
{
    long long vals[] = {12, 18, 24};
    TEST_ASSERT_EQUAL_INT64(6, cycle_gcd_array(vals, 3));
}

/* ===== 12. GCD array single element ===== */

void test_gcd_array_single(void)
{
    long long vals[] = {42};
    TEST_ASSERT_EQUAL_INT64(42, cycle_gcd_array(vals, 1));
}

/* ===== 13. LCM array ===== */

void test_lcm_array_basic(void)
{
    long long vals[] = {4, 6, 10};
    TEST_ASSERT_EQUAL_INT64(60, cycle_lcm_array(vals, 3));
}

/* ===== 14. LCM array single element ===== */

void test_lcm_array_single(void)
{
    long long vals[] = {17};
    TEST_ASSERT_EQUAL_INT64(17, cycle_lcm_array(vals, 1));
}

/* ===== 15. LCM array overflow returns 0 ===== */

void test_lcm_array_overflow(void)
{
    long long vals[] = {999999999999999LL, 999999999999998LL, 7};
    TEST_ASSERT_EQUAL_INT64(0, cycle_lcm_array(vals, 3));
}

/* ===== 16. Cycle ratio ===== */

void test_cycle_ratio_basic(void)
{
    /* 365 / 7 = 52.142... */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 52.142857f, (float)cycle_ratio(365, 7));
}

/* ===== 17. Cycle ratio exact integer ===== */

void test_cycle_ratio_exact(void)
{
    /* 30 / 10 = 3.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3.0f, (float)cycle_ratio(30, 10));
}

/* ===== 18. Resonance detection ===== */

void test_resonance_calendar_round(void)
{
    cycle_resonance_t r = cycle_resonance(260, 365);
    TEST_ASSERT_EQUAL_INT64(18980, r.lcm);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)(260.0 / 365.0), (float)r.ratio);
}

/* ===== 19. Resonance near-integer detection ===== */

void test_resonance_near_integer(void)
{
    /* 384 / 30 = 12.8 — not near integer (within 1%) */
    cycle_resonance_t r = cycle_resonance(384, 30);
    TEST_ASSERT_EQUAL_INT(0, r.near_integer);
    TEST_ASSERT_EQUAL_INT(13, r.harmonic);
}

/* ===== 20. Resonance exact harmonic ===== */

void test_resonance_exact_harmonic(void)
{
    /* 21 / 7 = 3.0 exactly */
    cycle_resonance_t r = cycle_resonance(21, 7);
    TEST_ASSERT_EQUAL_INT(1, r.near_integer);
    TEST_ASSERT_EQUAL_INT(3, r.harmonic);
}

/* ===== 21. Is harmonic ===== */

void test_is_harmonic_true(void)
{
    /* 365 / 73 = 5.0 exactly */
    TEST_ASSERT_EQUAL_INT(1, cycle_is_harmonic(365, 73, 0.01));
}

void test_is_harmonic_false(void)
{
    /* 365 / 260 = 1.403... not near integer */
    TEST_ASSERT_EQUAL_INT(0, cycle_is_harmonic(365, 260, 0.01));
}

/* ===== 22. Cycle position ===== */

void test_cycle_position_basic(void)
{
    TEST_ASSERT_EQUAL_INT64(3, cycle_position(3, 7));
}

void test_cycle_position_wrap(void)
{
    TEST_ASSERT_EQUAL_INT64(1, cycle_position(8, 7));
}

void test_cycle_position_exact(void)
{
    TEST_ASSERT_EQUAL_INT64(0, cycle_position(7, 7));
}

/* ===== 23. Cycle position negative day count ===== */

void test_cycle_position_negative(void)
{
    /* -1 mod 7 should be 6 (position in cycle, never negative) */
    TEST_ASSERT_EQUAL_INT64(6, cycle_position(-1, 7));
}

void test_cycle_position_negative_large(void)
{
    /* -365 mod 260: (-365 % 260) in math = 155 */
    /* -365 = -2 * 260 + 155 */
    TEST_ASSERT_EQUAL_INT64(155, cycle_position(-365, 260));
}

/* ===== 24. Next alignment ===== */

void test_next_alignment_basic(void)
{
    /* Cycles of 4 and 6, LCM=12. At day 1, next alignment at day 12. */
    TEST_ASSERT_EQUAL_INT64(11, cycle_next_alignment(1, 4, 6));
}

void test_next_alignment_at_zero(void)
{
    /* At day 0, next alignment at LCM(4,6)=12 */
    TEST_ASSERT_EQUAL_INT64(12, cycle_next_alignment(0, 4, 6));
}

void test_next_alignment_at_alignment(void)
{
    /* At day 12 (already aligned), next alignment at day 24 = 12 days away */
    TEST_ASSERT_EQUAL_INT64(12, cycle_next_alignment(12, 4, 6));
}

/* ===== 25. Alignment count ===== */

void test_alignment_count_basic(void)
{
    /* LCM(4,6)=12. Alignments at 0,12,24. Range [0,25): 0,12,24 = 3 */
    TEST_ASSERT_EQUAL_INT(3, cycle_alignment_count(0, 25, 4, 6));
}

void test_alignment_count_none(void)
{
    /* Range [1,11]: no alignment (LCM=12) */
    TEST_ASSERT_EQUAL_INT(0, cycle_alignment_count(1, 11, 4, 6));
}

void test_alignment_count_calendar_round(void)
{
    /* LCM(260,365)=18980. Range [0, 18981): alignments at 0 and 18980 = 2 */
    TEST_ASSERT_EQUAL_INT(2, cycle_alignment_count(0, 18981, 260, 365));
}

/* ===== 26. Known cycle lengths ===== */

void test_known_length_tzolkin(void)
{
    TEST_ASSERT_EQUAL_INT64(260, cycle_known_length("Tzolkin"));
}

void test_known_length_haab(void)
{
    TEST_ASSERT_EQUAL_INT64(365, cycle_known_length("Haab"));
}

void test_known_length_calendar_round(void)
{
    TEST_ASSERT_EQUAL_INT64(18980, cycle_known_length("Calendar Round"));
}

void test_known_length_metonic(void)
{
    TEST_ASSERT_EQUAL_INT64(6940, cycle_known_length("Metonic"));
}

void test_known_length_saros(void)
{
    TEST_ASSERT_EQUAL_INT64(6585, cycle_known_length("Saros"));
}

void test_known_length_week(void)
{
    TEST_ASSERT_EQUAL_INT64(7, cycle_known_length("Week"));
}

void test_known_length_unknown(void)
{
    TEST_ASSERT_EQUAL_INT64(0, cycle_known_length("Nonexistent"));
}

/* ===== 27. Sacred number 108 ===== */

void test_sacred_number_108(void)
{
    /* 27 Nakshatras * 4 padas = 108 */
    TEST_ASSERT_EQUAL_INT64(108, 27 * 4);
    /* 108 / 12 = 9 (Navamsa = HD 9 centers) */
    TEST_ASSERT_EQUAL_INT64(9, 108 / 12);
}

/* ===== 28. Hexagram lines ===== */

void test_hexagram_lines_lunar(void)
{
    /* 64 hexagrams * 6 lines = 384 ~= 13 lunar months */
    TEST_ASSERT_EQUAL_INT64(384, 64 * 6);
    /* 13 synodic months ~= 383.9 days */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 384.0f, (float)(29.53059 * 13.0));
}

/* ===== 29. Purity — same inputs produce same outputs ===== */

void test_purity_gcd(void)
{
    long long a = cycle_gcd(260, 365);
    long long b = cycle_gcd(260, 365);
    TEST_ASSERT_EQUAL_INT64(a, b);
}

void test_purity_resonance(void)
{
    cycle_resonance_t a = cycle_resonance(260, 365);
    cycle_resonance_t b = cycle_resonance(260, 365);
    TEST_ASSERT_EQUAL_INT64(a.lcm, b.lcm);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)a.ratio, (float)b.ratio);
}

/* ===== 30. Edge cases ===== */

void test_gcd_array_empty(void)
{
    /* count 0 should return 0 */
    TEST_ASSERT_EQUAL_INT64(0, cycle_gcd_array(NULL, 0));
}

void test_lcm_array_empty(void)
{
    TEST_ASSERT_EQUAL_INT64(0, cycle_lcm_array(NULL, 0));
}

void test_cycle_position_zero_day(void)
{
    TEST_ASSERT_EQUAL_INT64(0, cycle_position(0, 260));
}

void test_next_alignment_tzolkin_haab(void)
{
    /* From day 1, next alignment of Tzolkin(260) and Haab(365) at LCM=18980 */
    long long next = cycle_next_alignment(1, 260, 365);
    TEST_ASSERT_EQUAL_INT64(18979, next);
}

void test_known_length_hijri(void)
{
    TEST_ASSERT_EQUAL_INT64(10631, cycle_known_length("Hijri 30-year"));
}

void test_known_length_sexagenary(void)
{
    TEST_ASSERT_EQUAL_INT64(21915, cycle_known_length("Sexagenary"));
}

void test_known_length_sabbatical(void)
{
    TEST_ASSERT_EQUAL_INT64(2557, cycle_known_length("Sabbatical"));
}

void test_known_length_jubilee(void)
{
    TEST_ASSERT_EQUAL_INT64(18263, cycle_known_length("Jubilee"));
}

void test_known_length_lunar_synodic(void)
{
    TEST_ASSERT_EQUAL_INT64(30, cycle_known_length("Lunar synodic"));
}

void test_known_length_solar_year(void)
{
    TEST_ASSERT_EQUAL_INT64(365, cycle_known_length("Solar year"));
}

int main(void)
{
    UNITY_BEGIN();
    /* GCD */
    RUN_TEST(test_gcd_basic);
    RUN_TEST(test_gcd_coprime);
    RUN_TEST(test_gcd_same);
    RUN_TEST(test_gcd_one);
    RUN_TEST(test_gcd_large);
    RUN_TEST(test_gcd_nakshatra_zodiac);
    /* LCM */
    RUN_TEST(test_lcm_basic);
    RUN_TEST(test_lcm_coprime);
    RUN_TEST(test_lcm_calendar_round);
    RUN_TEST(test_lcm_overflow);
    /* GCD/LCM array */
    RUN_TEST(test_gcd_array_basic);
    RUN_TEST(test_gcd_array_single);
    RUN_TEST(test_lcm_array_basic);
    RUN_TEST(test_lcm_array_single);
    RUN_TEST(test_lcm_array_overflow);
    /* Ratio */
    RUN_TEST(test_cycle_ratio_basic);
    RUN_TEST(test_cycle_ratio_exact);
    /* Resonance */
    RUN_TEST(test_resonance_calendar_round);
    RUN_TEST(test_resonance_near_integer);
    RUN_TEST(test_resonance_exact_harmonic);
    /* Harmonic */
    RUN_TEST(test_is_harmonic_true);
    RUN_TEST(test_is_harmonic_false);
    /* Position */
    RUN_TEST(test_cycle_position_basic);
    RUN_TEST(test_cycle_position_wrap);
    RUN_TEST(test_cycle_position_exact);
    RUN_TEST(test_cycle_position_negative);
    RUN_TEST(test_cycle_position_negative_large);
    /* Alignment */
    RUN_TEST(test_next_alignment_basic);
    RUN_TEST(test_next_alignment_at_zero);
    RUN_TEST(test_next_alignment_at_alignment);
    RUN_TEST(test_alignment_count_basic);
    RUN_TEST(test_alignment_count_none);
    RUN_TEST(test_alignment_count_calendar_round);
    /* Known cycles */
    RUN_TEST(test_known_length_tzolkin);
    RUN_TEST(test_known_length_haab);
    RUN_TEST(test_known_length_calendar_round);
    RUN_TEST(test_known_length_metonic);
    RUN_TEST(test_known_length_saros);
    RUN_TEST(test_known_length_week);
    RUN_TEST(test_known_length_unknown);
    /* Sacred relationships */
    RUN_TEST(test_sacred_number_108);
    RUN_TEST(test_hexagram_lines_lunar);
    /* Purity */
    RUN_TEST(test_purity_gcd);
    RUN_TEST(test_purity_resonance);
    /* Edge cases */
    RUN_TEST(test_gcd_array_empty);
    RUN_TEST(test_lcm_array_empty);
    RUN_TEST(test_cycle_position_zero_day);
    RUN_TEST(test_next_alignment_tzolkin_haab);
    RUN_TEST(test_known_length_hijri);
    RUN_TEST(test_known_length_sexagenary);
    RUN_TEST(test_known_length_sabbatical);
    RUN_TEST(test_known_length_jubilee);
    RUN_TEST(test_known_length_lunar_synodic);
    RUN_TEST(test_known_length_solar_year);
    return UNITY_END();
}
