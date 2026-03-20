/* test_brain_explain.c — Tests for cross-system explanation engine.
 *
 * Validates explanation database, lookup, type classification,
 * and fallback behavior for unknown pairs. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_explain.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Database integrity
 * =================================================================== */

void test_explain_count_positive(void) {
    TEST_ASSERT_GREATER_THAN(20, br_explain_count());
}

void test_explain_count_reasonable(void) {
    TEST_ASSERT_LESS_OR_EQUAL(100, br_explain_count());
}

/* ===================================================================
 * Explanation type names
 * =================================================================== */

void test_type_name_shared_source(void) {
    TEST_ASSERT_EQUAL_STRING("Shared Source",
                             br_explain_type_name(BR_EXPLAIN_SHARED_SOURCE));
}

void test_type_name_transmission(void) {
    TEST_ASSERT_EQUAL_STRING("Transmission",
                             br_explain_type_name(BR_EXPLAIN_TRANSMISSION));
}

void test_type_name_convergent(void) {
    TEST_ASSERT_EQUAL_STRING("Convergent Evolution",
                             br_explain_type_name(BR_EXPLAIN_CONVERGENT));
}

void test_type_name_unknown(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown",
                             br_explain_type_name(BR_EXPLAIN_UNKNOWN));
}

void test_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", br_explain_type_name(99));
    TEST_ASSERT_EQUAL_STRING("?", br_explain_type_name(-1));
}

/* ===================================================================
 * Lookup — shared source pairs
 * =================================================================== */

void test_lookup_astronomy_astrology(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ASTRONOMY,
                                                  CD_SYS_ASTROLOGY);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_SHARED_SOURCE, e->why);
    TEST_ASSERT_NOT_NULL(e->brief);
    TEST_ASSERT_NOT_NULL(e->detail);
}

void test_lookup_hebrew_islamic_shared(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_HEBREW,
                                                  CD_SYS_ISLAMIC);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_SHARED_SOURCE, e->why);
    TEST_ASSERT_TRUE(strlen(e->detail) > 40);
}

/* ===================================================================
 * Lookup — transmission pairs
 * =================================================================== */

void test_lookup_chinese_korean_transmission(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_CHINESE,
                                                  CD_SYS_KOREAN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_TRANSMISSION, e->why);
}

void test_lookup_hindu_buddhist_transmission(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_HINDU,
                                                  CD_SYS_BUDDHIST);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_TRANSMISSION, e->why);
}

void test_lookup_zoroastrian_persian(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ZOROASTRIAN,
                                                  CD_SYS_PERSIAN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_TRANSMISSION, e->why);
}

void test_lookup_coptic_egyptian(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_COPTIC,
                                                  CD_SYS_EGYPTIAN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_TRANSMISSION, e->why);
}

/* ===================================================================
 * Lookup — convergent pairs
 * =================================================================== */

void test_lookup_tzolkin_hebrew_convergent(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_TZOLKIN,
                                                  CD_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_CONVERGENT, e->why);
}

void test_lookup_hebrew_chinese_convergent(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_HEBREW,
                                                  CD_SYS_CHINESE);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_CONVERGENT, e->why);
}

void test_lookup_tzolkin_hindu_convergent(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_TZOLKIN,
                                                  CD_SYS_HINDU);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_CONVERGENT, e->why);
}

/* ===================================================================
 * Lookup — order independence
 * =================================================================== */

void test_lookup_order_independent(void) {
    const br_explanation_t *ab = br_explain_lookup(CD_SYS_CHINESE,
                                                   CD_SYS_KOREAN);
    const br_explanation_t *ba = br_explain_lookup(CD_SYS_KOREAN,
                                                   CD_SYS_CHINESE);
    TEST_ASSERT_NOT_NULL(ab);
    TEST_ASSERT_EQUAL_PTR(ab, ba);
}

/* ===================================================================
 * Lookup — unknown pairs
 * =================================================================== */

void test_lookup_unknown_returns_null(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_FRENCH,
                                                  CD_SYS_TAMIL);
    TEST_ASSERT_NULL(e);
}

/* ===================================================================
 * br_explain — text output
 * =================================================================== */

void test_explain_known_pair_writes_detail(void) {
    char buf[BR_EXPLAIN_MAX];
    int len = br_explain(CD_SYS_HEBREW, CD_SYS_ISLAMIC, buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_TRUE(strlen(buf) > 30);
}

void test_explain_known_pair_content(void) {
    char buf[BR_EXPLAIN_MAX];
    br_explain(CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY, buf, sizeof(buf));
    /* Should contain key words from the detail */
    TEST_ASSERT_NOT_NULL(strstr(buf, "celestial"));
}

void test_explain_unknown_pair_fallback(void) {
    char buf[BR_EXPLAIN_MAX];
    int len = br_explain(CD_SYS_FRENCH, CD_SYS_TAMIL, buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    /* Fallback should mention natural cycles */
    TEST_ASSERT_NOT_NULL(strstr(buf, "natural cycles"));
}

void test_explain_null_buf_returns_zero(void) {
    int len = br_explain(CD_SYS_HEBREW, CD_SYS_ISLAMIC, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_explain_small_buf_truncates(void) {
    char buf[20];
    int len = br_explain(CD_SYS_HEBREW, CD_SYS_ISLAMIC, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(19, len);
    TEST_ASSERT_EQUAL_INT('\0', buf[19]);
}

void test_explain_order_independent_text(void) {
    char buf1[BR_EXPLAIN_MAX];
    char buf2[BR_EXPLAIN_MAX];
    br_explain(CD_SYS_CHINESE, CD_SYS_KOREAN, buf1, sizeof(buf1));
    br_explain(CD_SYS_KOREAN, CD_SYS_CHINESE, buf2, sizeof(buf2));
    TEST_ASSERT_EQUAL_STRING(buf1, buf2);
}

/* ===================================================================
 * All entries have required fields
 * =================================================================== */

void test_all_entries_have_brief(void) {
    /* Spot check several known pairs */
    const int pairs[][2] = {
        { CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY },
        { CD_SYS_HEBREW, CD_SYS_ISLAMIC },
        { CD_SYS_CHINESE, CD_SYS_KOREAN },
        { CD_SYS_TZOLKIN, CD_SYS_HINDU },
    };
    for (int i = 0; i < 4; i++) {
        const br_explanation_t *e = br_explain_lookup(pairs[i][0], pairs[i][1]);
        TEST_ASSERT_NOT_NULL(e);
        TEST_ASSERT_NOT_NULL(e->brief);
        TEST_ASSERT_TRUE(strlen(e->brief) > 5);
    }
}

void test_all_entries_have_detail(void) {
    const int pairs[][2] = {
        { CD_SYS_ASTRONOMY, CD_SYS_CELTIC },
        { CD_SYS_HINDU, CD_SYS_BUDDHIST },
        { CD_SYS_BAHAI, CD_SYS_PERSIAN },
        { CD_SYS_CELTIC, CD_SYS_TZOLKIN },
    };
    for (int i = 0; i < 4; i++) {
        const br_explanation_t *e = br_explain_lookup(pairs[i][0], pairs[i][1]);
        TEST_ASSERT_NOT_NULL(e);
        TEST_ASSERT_NOT_NULL(e->detail);
        TEST_ASSERT_TRUE(strlen(e->detail) > 20);
    }
}

/* ===================================================================
 * New equinox-cluster pairs
 * =================================================================== */

void test_lookup_astronomy_persian_shared(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ASTRONOMY, CD_SYS_PERSIAN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_SHARED_SOURCE, e->why);
}

void test_lookup_astronomy_bahai_shared(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ASTRONOMY, CD_SYS_BAHAI);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_SHARED_SOURCE, e->why);
}

void test_lookup_astronomy_hindu_shared(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ASTRONOMY, CD_SYS_HINDU);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_SHARED_SOURCE, e->why);
}

void test_lookup_astronomy_french_shared(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ASTRONOMY, CD_SYS_FRENCH);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_SHARED_SOURCE, e->why);
}

void test_lookup_islamic_bahai_transmission(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_ISLAMIC, CD_SYS_BAHAI);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_TRANSMISSION, e->why);
}

void test_lookup_bahai_french_convergent(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_BAHAI, CD_SYS_FRENCH);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_CONVERGENT, e->why);
}

void test_lookup_persian_french_convergent(void) {
    const br_explanation_t *e = br_explain_lookup(CD_SYS_PERSIAN, CD_SYS_FRENCH);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(BR_EXPLAIN_CONVERGENT, e->why);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Database */
    RUN_TEST(test_explain_count_positive);
    RUN_TEST(test_explain_count_reasonable);

    /* Type names */
    RUN_TEST(test_type_name_shared_source);
    RUN_TEST(test_type_name_transmission);
    RUN_TEST(test_type_name_convergent);
    RUN_TEST(test_type_name_unknown);
    RUN_TEST(test_type_name_invalid);

    /* Shared source pairs */
    RUN_TEST(test_lookup_astronomy_astrology);
    RUN_TEST(test_lookup_hebrew_islamic_shared);

    /* Transmission pairs */
    RUN_TEST(test_lookup_chinese_korean_transmission);
    RUN_TEST(test_lookup_hindu_buddhist_transmission);
    RUN_TEST(test_lookup_zoroastrian_persian);
    RUN_TEST(test_lookup_coptic_egyptian);

    /* Convergent pairs */
    RUN_TEST(test_lookup_tzolkin_hebrew_convergent);
    RUN_TEST(test_lookup_hebrew_chinese_convergent);
    RUN_TEST(test_lookup_tzolkin_hindu_convergent);

    /* Order independence */
    RUN_TEST(test_lookup_order_independent);

    /* Unknown pairs */
    RUN_TEST(test_lookup_unknown_returns_null);

    /* Text output */
    RUN_TEST(test_explain_known_pair_writes_detail);
    RUN_TEST(test_explain_known_pair_content);
    RUN_TEST(test_explain_unknown_pair_fallback);
    RUN_TEST(test_explain_null_buf_returns_zero);
    RUN_TEST(test_explain_small_buf_truncates);
    RUN_TEST(test_explain_order_independent_text);

    /* Field completeness */
    RUN_TEST(test_all_entries_have_brief);
    RUN_TEST(test_all_entries_have_detail);

    /* New equinox-cluster pairs */
    RUN_TEST(test_lookup_astronomy_persian_shared);
    RUN_TEST(test_lookup_astronomy_bahai_shared);
    RUN_TEST(test_lookup_astronomy_hindu_shared);
    RUN_TEST(test_lookup_astronomy_french_shared);
    RUN_TEST(test_lookup_islamic_bahai_transmission);
    RUN_TEST(test_lookup_bahai_french_convergent);
    RUN_TEST(test_lookup_persian_french_convergent);

    return UNITY_END();
}
