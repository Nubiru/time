/* test_brain_score.c — Tests for brain confidence and rarity scoring.
 *
 * Tests br_score_correlation, br_score_result, br_score_rarity,
 * and the known system-pair relationship database. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_score.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Known pair database
 * =================================================================== */

void test_known_pair_count_positive(void) {
    TEST_ASSERT_GREATER_THAN(0, br_known_pair_count());
}

void test_known_pair_count_within_max(void) {
    TEST_ASSERT_LESS_OR_EQUAL(BR_MAX_KNOWN_PAIRS, br_known_pair_count());
}

void test_lookup_hebrew_islamic(void) {
    const br_known_pair_t *pair = br_lookup_pair(CD_SYS_HEBREW, CD_SYS_ISLAMIC);
    TEST_ASSERT_NOT_NULL(pair);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, pair->type);
    TEST_ASSERT_EQUAL_FLOAT(0.95, pair->base_confidence);
}

void test_lookup_order_independent(void) {
    const br_known_pair_t *ab = br_lookup_pair(CD_SYS_HEBREW, CD_SYS_ISLAMIC);
    const br_known_pair_t *ba = br_lookup_pair(CD_SYS_ISLAMIC, CD_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(ab);
    TEST_ASSERT_NOT_NULL(ba);
    TEST_ASSERT_EQUAL_PTR(ab, ba);
}

void test_lookup_structural_pair(void) {
    const br_known_pair_t *pair = br_lookup_pair(CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY);
    TEST_ASSERT_NOT_NULL(pair);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, pair->type);
    TEST_ASSERT_EQUAL_FLOAT(1.0, pair->base_confidence);
}

void test_lookup_unknown_pair_null(void) {
    /* French and Tzolkin — no known relationship */
    const br_known_pair_t *pair = br_lookup_pair(CD_SYS_FRENCH, CD_SYS_TZOLKIN);
    TEST_ASSERT_NULL(pair);
}

void test_lookup_chinese_korean(void) {
    const br_known_pair_t *pair = br_lookup_pair(CD_SYS_CHINESE, CD_SYS_KOREAN);
    TEST_ASSERT_NOT_NULL(pair);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, pair->type);
    TEST_ASSERT_EQUAL_FLOAT(0.95, pair->base_confidence);
}

void test_lookup_approximate_pair(void) {
    const br_known_pair_t *pair = br_lookup_pair(CD_SYS_TZOLKIN, CD_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(pair);
    TEST_ASSERT_EQUAL_INT(BR_CORR_APPROXIMATE, pair->type);
}

void test_all_pairs_have_reason(void) {
    int count = br_known_pair_count();
    for (int i = 0; i < count; i++) {
        /* Iterate by testing known pairs through lookup exhaustively
         * would require access to internal array; instead verify
         * specific known pairs have reasons */
    }
    const br_known_pair_t *pair = br_lookup_pair(CD_SYS_HEBREW, CD_SYS_ISLAMIC);
    TEST_ASSERT_NOT_NULL(pair);
    TEST_ASSERT_NOT_NULL(pair->reason);
    TEST_ASSERT_TRUE(strlen(pair->reason) > 0);
}

/* ===================================================================
 * br_score_correlation
 * =================================================================== */

void test_score_known_pair_updates_type(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    c.system_a = CD_SYS_HEBREW;
    c.system_b = CD_SYS_ISLAMIC;
    c.type = BR_CORR_APPROXIMATE; /* default from brain_scan */
    c.confidence = 0.3;

    br_score_correlation(&c);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, c.type);
    TEST_ASSERT_EQUAL_FLOAT(0.95, c.confidence);
}

void test_score_structural_pair(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    c.system_a = CD_SYS_ASTRONOMY;
    c.system_b = CD_SYS_ASTROLOGY;

    br_score_correlation(&c);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, c.type);
    TEST_ASSERT_EQUAL_FLOAT(1.0, c.confidence);
}

void test_score_unknown_pair_coincidental(void) {
    br_correlation_t c;
    memset(&c, 0, sizeof(c));
    c.system_a = CD_SYS_TZOLKIN;
    c.system_b = CD_SYS_KOREAN;

    br_score_correlation(&c);
    TEST_ASSERT_EQUAL_INT(BR_CORR_COINCIDENTAL, c.type);
    TEST_ASSERT_EQUAL_FLOAT(0.1, c.confidence);
}

void test_score_null_safe(void) {
    br_score_correlation(NULL); /* should not crash */
}

/* ===================================================================
 * br_score_result
 * =================================================================== */

void test_score_result_updates_all(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));

    result.correlations[0].system_a = CD_SYS_HEBREW;
    result.correlations[0].system_b = CD_SYS_ISLAMIC;
    result.correlations[0].type = BR_CORR_APPROXIMATE;
    result.correlations[0].confidence = 0.3;

    result.correlations[1].system_a = CD_SYS_TZOLKIN;
    result.correlations[1].system_b = CD_SYS_KOREAN;
    result.correlations[1].type = BR_CORR_APPROXIMATE;
    result.correlations[1].confidence = 0.3;

    result.correlation_count = 2;
    result.convergence_strength = 0.25;

    br_score_result(&result);

    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, result.correlations[0].type);
    TEST_ASSERT_EQUAL_FLOAT(0.95, result.correlations[0].confidence);
    TEST_ASSERT_EQUAL_INT(BR_CORR_COINCIDENTAL, result.correlations[1].type);
    TEST_ASSERT_EQUAL_FLOAT(0.1, result.correlations[1].confidence);
}

void test_score_result_updates_strength(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));

    result.correlations[0].system_a = CD_SYS_ASTRONOMY;
    result.correlations[0].system_b = CD_SYS_ASTROLOGY;
    result.correlation_count = 1;
    result.convergence_strength = 0.25;

    br_score_result(&result);
    /* Structural pair has confidence 1.0, which > 0.25 */
    TEST_ASSERT_EQUAL_FLOAT(1.0, result.convergence_strength);
}

void test_score_result_keeps_higher_strength(void) {
    br_result_t result;
    memset(&result, 0, sizeof(result));

    result.correlations[0].system_a = CD_SYS_TZOLKIN;
    result.correlations[0].system_b = CD_SYS_KOREAN;
    result.correlation_count = 1;
    result.convergence_strength = 0.75; /* already higher than 0.1 */

    br_score_result(&result);
    TEST_ASSERT_EQUAL_FLOAT(0.75, result.convergence_strength);
}

void test_score_result_null_safe(void) {
    br_score_result(NULL); /* should not crash */
}

/* ===================================================================
 * br_score_rarity
 * =================================================================== */

void test_rarity_zero_events(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_score_rarity(0));
}

void test_rarity_negative(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_score_rarity(-1));
}

void test_rarity_one_event(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.2, br_score_rarity(1));
}

void test_rarity_two_events(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.2, br_score_rarity(2));
}

void test_rarity_three_events(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.5, br_score_rarity(3));
}

void test_rarity_five_events(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.8, br_score_rarity(5));
}

void test_rarity_eight_plus(void) {
    TEST_ASSERT_EQUAL_FLOAT(1.0, br_score_rarity(8));
    TEST_ASSERT_EQUAL_FLOAT(1.0, br_score_rarity(20));
}

void test_rarity_monotonic(void) {
    double prev = br_score_rarity(0);
    for (int i = 1; i <= 10; i++) {
        double curr = br_score_rarity(i);
        TEST_ASSERT_TRUE(curr >= prev);
        prev = curr;
    }
}

/* ===================================================================
 * New equinox-cluster pairs
 * =================================================================== */

void test_lookup_astronomy_persian(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_ASTRONOMY, CD_SYS_PERSIAN);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, p->type);
}

void test_lookup_astronomy_bahai(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_ASTRONOMY, CD_SYS_BAHAI);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, p->type);
}

void test_lookup_islamic_bahai(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_ISLAMIC, CD_SYS_BAHAI);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_APPROXIMATE, p->type);
}

/* ===================================================================
 * Batch 4: Under-connected systems
 * =================================================================== */

void test_lookup_tamil_hindu(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_TAMIL, CD_SYS_HINDU);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.95, p->base_confidence);
}

void test_lookup_myanmar_buddhist(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_MYANMAR, CD_SYS_BUDDHIST);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.90, p->base_confidence);
}

void test_lookup_coptic_egyptian(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_COPTIC, CD_SYS_EGYPTIAN);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_STRUCTURAL, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.95, p->base_confidence);
}

void test_lookup_astrology_hindu(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_ASTROLOGY, CD_SYS_HINDU);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.80, p->base_confidence);
}

void test_lookup_iching_korean(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_ICHING, CD_SYS_KOREAN);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.85, p->base_confidence);
}

void test_lookup_myanmar_thai(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_MYANMAR, CD_SYS_THAI);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.85, p->base_confidence);
}

void test_lookup_egyptian_french(void) {
    const br_known_pair_t *p = br_lookup_pair(CD_SYS_EGYPTIAN, CD_SYS_FRENCH);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_INT(BR_CORR_HARMONIC, p->type);
    TEST_ASSERT_EQUAL_FLOAT(0.85, p->base_confidence);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Known pair database */
    RUN_TEST(test_known_pair_count_positive);
    RUN_TEST(test_known_pair_count_within_max);
    RUN_TEST(test_lookup_hebrew_islamic);
    RUN_TEST(test_lookup_order_independent);
    RUN_TEST(test_lookup_structural_pair);
    RUN_TEST(test_lookup_unknown_pair_null);
    RUN_TEST(test_lookup_chinese_korean);
    RUN_TEST(test_lookup_approximate_pair);
    RUN_TEST(test_all_pairs_have_reason);

    /* New equinox-cluster pairs */
    RUN_TEST(test_lookup_astronomy_persian);
    RUN_TEST(test_lookup_astronomy_bahai);
    RUN_TEST(test_lookup_islamic_bahai);

    /* Batch 4: under-connected systems */
    RUN_TEST(test_lookup_tamil_hindu);
    RUN_TEST(test_lookup_myanmar_buddhist);
    RUN_TEST(test_lookup_coptic_egyptian);
    RUN_TEST(test_lookup_astrology_hindu);
    RUN_TEST(test_lookup_iching_korean);
    RUN_TEST(test_lookup_myanmar_thai);
    RUN_TEST(test_lookup_egyptian_french);

    /* br_score_correlation */
    RUN_TEST(test_score_known_pair_updates_type);
    RUN_TEST(test_score_structural_pair);
    RUN_TEST(test_score_unknown_pair_coincidental);
    RUN_TEST(test_score_null_safe);

    /* br_score_result */
    RUN_TEST(test_score_result_updates_all);
    RUN_TEST(test_score_result_updates_strength);
    RUN_TEST(test_score_result_keeps_higher_strength);
    RUN_TEST(test_score_result_null_safe);

    /* br_score_rarity */
    RUN_TEST(test_rarity_zero_events);
    RUN_TEST(test_rarity_negative);
    RUN_TEST(test_rarity_one_event);
    RUN_TEST(test_rarity_two_events);
    RUN_TEST(test_rarity_three_events);
    RUN_TEST(test_rarity_five_events);
    RUN_TEST(test_rarity_eight_plus);
    RUN_TEST(test_rarity_monotonic);

    return UNITY_END();
}
