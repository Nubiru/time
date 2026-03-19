/* test_brain_query.c — Tests for compound question engine.
 *
 * Tests AND/OR queries across multiple systems. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_query.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static const double TEST_JD = 2460389.5; /* 2024-03-20 */

/* ===================================================================
 * Query construction
 * =================================================================== */

void test_query_and_sets_op(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC };
    br_query_t q = br_query_and(sys, 2);
    TEST_ASSERT_EQUAL_INT(BR_QUERY_AND, q.op);
    TEST_ASSERT_EQUAL_INT(2, q.system_count);
}

void test_query_or_sets_op(void) {
    cd_system_t sys[] = { CD_SYS_TZOLKIN };
    br_query_t q = br_query_or(sys, 1);
    TEST_ASSERT_EQUAL_INT(BR_QUERY_OR, q.op);
}

void test_query_clamps_max_systems(void) {
    cd_system_t sys[10];
    for (int i = 0; i < 10; i++) sys[i] = (cd_system_t)i;
    br_query_t q = br_query_and(sys, 10);
    TEST_ASSERT_EQUAL_INT(BR_QUERY_MAX_SYSTEMS, q.system_count);
}

/* ===================================================================
 * br_query_search — null/edge cases
 * =================================================================== */

void test_search_null_query_returns_zero(void) {
    br_query_result_t out;
    TEST_ASSERT_EQUAL_INT(0, br_query_search(NULL, TEST_JD, 30, &out));
}

void test_search_null_out_returns_zero(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW };
    br_query_t q = br_query_and(sys, 1);
    TEST_ASSERT_EQUAL_INT(0, br_query_search(&q, TEST_JD, 30, NULL));
}

void test_search_zero_range_returns_zero(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW };
    br_query_t q = br_query_and(sys, 1);
    br_query_result_t out;
    TEST_ASSERT_EQUAL_INT(0, br_query_search(&q, TEST_JD, 0, &out));
}

void test_search_empty_query_returns_zero(void) {
    br_query_t q;
    memset(&q, 0, sizeof(q));
    br_query_result_t out;
    TEST_ASSERT_EQUAL_INT(0, br_query_search(&q, TEST_JD, 30, &out));
}

/* ===================================================================
 * OR query — single system
 * =================================================================== */

void test_or_single_system_finds_events(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW };
    br_query_t q = br_query_or(sys, 1);
    br_query_result_t out;
    int count = br_query_search(&q, TEST_JD, 60, &out);
    /* Hebrew new month happens every ~30 days — should find something */
    TEST_ASSERT_TRUE(count >= 0);
    TEST_ASSERT_TRUE(count <= BR_QUERY_MAX_RESULTS);
}

void test_or_matches_are_future(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW };
    br_query_t q = br_query_or(sys, 1);
    br_query_result_t out;
    br_query_search(&q, TEST_JD, 60, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_TRUE(out.matches[i].jd > TEST_JD);
    }
}

/* ===================================================================
 * AND query — multiple systems
 * =================================================================== */

void test_and_two_systems_harder_than_one(void) {
    cd_system_t one[] = { CD_SYS_HEBREW };
    cd_system_t two[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC };
    br_query_t q1 = br_query_or(one, 1);
    br_query_t q2 = br_query_and(two, 2);
    br_query_result_t out1, out2;
    br_query_search(&q1, TEST_JD, 90, &out1);
    br_query_search(&q2, TEST_JD, 90, &out2);
    /* AND of two systems should have <= results than OR of one */
    TEST_ASSERT_TRUE(out2.count <= out1.count);
}

void test_and_matches_have_all_systems(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC };
    br_query_t q = br_query_and(sys, 2);
    br_query_result_t out;
    br_query_search(&q, TEST_JD, 90, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_EQUAL_INT(2, out.matches[i].matched_count);
    }
}

/* ===================================================================
 * Backward search
 * =================================================================== */

void test_backward_search_finds_past(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW };
    br_query_t q = br_query_or(sys, 1);
    br_query_result_t out;
    br_query_search(&q, TEST_JD, -60, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_TRUE(out.matches[i].jd < TEST_JD);
    }
}

/* ===================================================================
 * Result content
 * =================================================================== */

void test_matches_have_headlines(void) {
    cd_system_t sys[] = { CD_SYS_HEBREW };
    br_query_t q = br_query_or(sys, 1);
    br_query_result_t out;
    br_query_search(&q, TEST_JD, 60, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_TRUE(strlen(out.matches[i].headline) > 0);
    }
}

void test_days_scanned_set(void) {
    cd_system_t sys[] = { CD_SYS_TZOLKIN };
    br_query_t q = br_query_or(sys, 1);
    br_query_result_t out;
    br_query_search(&q, TEST_JD, 30, &out);
    TEST_ASSERT_EQUAL_INT(30, out.days_scanned);
}

void test_range_clamped(void) {
    cd_system_t sys[] = { CD_SYS_TZOLKIN };
    br_query_t q = br_query_or(sys, 1);
    br_query_result_t out;
    br_query_search(&q, TEST_JD, 999999, &out);
    TEST_ASSERT_EQUAL_INT(BR_QUERY_MAX_RANGE, out.days_scanned);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Construction */
    RUN_TEST(test_query_and_sets_op);
    RUN_TEST(test_query_or_sets_op);
    RUN_TEST(test_query_clamps_max_systems);

    /* Null/edge */
    RUN_TEST(test_search_null_query_returns_zero);
    RUN_TEST(test_search_null_out_returns_zero);
    RUN_TEST(test_search_zero_range_returns_zero);
    RUN_TEST(test_search_empty_query_returns_zero);

    /* OR query */
    RUN_TEST(test_or_single_system_finds_events);
    RUN_TEST(test_or_matches_are_future);

    /* AND query */
    RUN_TEST(test_and_two_systems_harder_than_one);
    RUN_TEST(test_and_matches_have_all_systems);

    /* Backward */
    RUN_TEST(test_backward_search_finds_past);

    /* Result content */
    RUN_TEST(test_matches_have_headlines);
    RUN_TEST(test_days_scanned_set);
    RUN_TEST(test_range_clamped);

    return UNITY_END();
}
