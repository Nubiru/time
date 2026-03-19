/* test_brain_cluster.c — Tests for automatic system clustering.
 *
 * Validates that systems are correctly grouped by calendar type. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_cluster.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Calendar type names
 * =================================================================== */

void test_type_name_lunar(void) {
    TEST_ASSERT_EQUAL_STRING("Lunar", br_cal_type_name(BR_CAL_LUNAR));
}

void test_type_name_solar(void) {
    TEST_ASSERT_EQUAL_STRING("Solar", br_cal_type_name(BR_CAL_SOLAR));
}

void test_type_name_lunisolar(void) {
    TEST_ASSERT_EQUAL_STRING("Lunisolar", br_cal_type_name(BR_CAL_LUNISOLAR));
}

void test_type_name_cyclical(void) {
    TEST_ASSERT_EQUAL_STRING("Cyclical", br_cal_type_name(BR_CAL_CYCLICAL));
}

void test_type_name_observational(void) {
    TEST_ASSERT_EQUAL_STRING("Observational", br_cal_type_name(BR_CAL_OBSERVATIONAL));
}

void test_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", br_cal_type_name(99));
}

/* ===================================================================
 * br_cluster_compute
 * =================================================================== */

void test_compute_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_cluster_compute(NULL));
}

void test_compute_returns_clusters(void) {
    br_cluster_result_t result;
    int count = br_cluster_compute(&result);
    TEST_ASSERT_GREATER_THAN(0, count);
    TEST_ASSERT_EQUAL_INT(count, result.count);
}

void test_compute_count_within_max(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    TEST_ASSERT_TRUE(result.count <= BR_CLUSTER_MAX);
}

void test_compute_five_clusters(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    TEST_ASSERT_EQUAL_INT(5, result.count);
}

void test_clusters_have_names(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    for (int i = 0; i < result.count; i++) {
        TEST_ASSERT_NOT_NULL(result.clusters[i].name);
        TEST_ASSERT_TRUE(strlen(result.clusters[i].name) > 0);
    }
}

void test_clusters_have_reasons(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    for (int i = 0; i < result.count; i++) {
        TEST_ASSERT_NOT_NULL(result.clusters[i].reason);
        TEST_ASSERT_TRUE(strlen(result.clusters[i].reason) > 10);
    }
}

void test_clusters_have_members(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    for (int i = 0; i < result.count; i++) {
        TEST_ASSERT_GREATER_THAN(0, result.clusters[i].member_count);
    }
}

/* ===================================================================
 * Known system placements
 * =================================================================== */

void test_islamic_is_lunar(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_ISLAMIC);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_LUNAR, c->type);
}

void test_hebrew_is_lunisolar(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_LUNISOLAR, c->type);
}

void test_chinese_is_lunisolar(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_CHINESE);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_LUNISOLAR, c->type);
}

void test_persian_is_solar(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_PERSIAN);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_SOLAR, c->type);
}

void test_tzolkin_is_cyclical(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_CYCLICAL, c->type);
}

void test_astronomy_is_observational(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_ASTRONOMY);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_OBSERVATIONAL, c->type);
}

void test_korean_is_lunisolar(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_KOREAN);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(BR_CAL_LUNISOLAR, c->type);
}

void test_hebrew_and_chinese_same_cluster(void) {
    br_cluster_result_t result;
    br_cluster_compute(&result);
    const br_cluster_t *h = br_cluster_for_system(&result, CD_SYS_HEBREW);
    const br_cluster_t *c = br_cluster_for_system(&result, CD_SYS_CHINESE);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_PTR(h, c);
}

/* ===================================================================
 * br_cluster_for_system — null safety
 * =================================================================== */

void test_cluster_for_system_null_result(void) {
    TEST_ASSERT_NULL(br_cluster_for_system(NULL, CD_SYS_HEBREW));
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Type names */
    RUN_TEST(test_type_name_lunar);
    RUN_TEST(test_type_name_solar);
    RUN_TEST(test_type_name_lunisolar);
    RUN_TEST(test_type_name_cyclical);
    RUN_TEST(test_type_name_observational);
    RUN_TEST(test_type_name_invalid);

    /* Cluster computation */
    RUN_TEST(test_compute_null_returns_zero);
    RUN_TEST(test_compute_returns_clusters);
    RUN_TEST(test_compute_count_within_max);
    RUN_TEST(test_compute_five_clusters);
    RUN_TEST(test_clusters_have_names);
    RUN_TEST(test_clusters_have_reasons);
    RUN_TEST(test_clusters_have_members);

    /* Known placements */
    RUN_TEST(test_islamic_is_lunar);
    RUN_TEST(test_hebrew_is_lunisolar);
    RUN_TEST(test_chinese_is_lunisolar);
    RUN_TEST(test_persian_is_solar);
    RUN_TEST(test_tzolkin_is_cyclical);
    RUN_TEST(test_astronomy_is_observational);
    RUN_TEST(test_korean_is_lunisolar);
    RUN_TEST(test_hebrew_and_chinese_same_cluster);

    /* Null safety */
    RUN_TEST(test_cluster_for_system_null_result);

    return UNITY_END();
}
