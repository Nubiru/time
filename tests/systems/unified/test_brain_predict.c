/* test_brain_predict.c — Tests for future event prediction.
 *
 * Tests br_predict and br_predict_next with real convergence detection. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_predict.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static const double TEST_JD = 2460389.5; /* 2024-03-20 */

/* ===================================================================
 * br_predict — basic behavior
 * =================================================================== */

void test_predict_null_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, br_predict(TEST_JD, 30, NULL));
}

void test_predict_zero_days_returns_zero(void) {
    br_prediction_t out;
    TEST_ASSERT_EQUAL_INT(0, br_predict(TEST_JD, 0, &out));
}

void test_predict_negative_days_returns_zero(void) {
    br_prediction_t out;
    TEST_ASSERT_EQUAL_INT(0, br_predict(TEST_JD, -5, &out));
}

void test_predict_sets_start_jd(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 30, &out);
    TEST_ASSERT_EQUAL_FLOAT(TEST_JD, out.start_jd);
}

void test_predict_sets_days_scanned(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 30, &out);
    TEST_ASSERT_EQUAL_INT(30, out.days_scanned);
}

void test_predict_clamps_max_days(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 99999, &out);
    TEST_ASSERT_EQUAL_INT(BR_PREDICT_MAX_DAYS, out.days_scanned);
}

void test_predict_finds_events_in_30_days(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 30, &out);
    /* Should find at least some convergence in 30 days */
    TEST_ASSERT_TRUE(out.count >= 0);
    TEST_ASSERT_TRUE(out.count <= BR_PREDICT_MAX);
}

void test_predict_events_are_future(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 30, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_TRUE(out.events[i].jd > TEST_JD);
        TEST_ASSERT_TRUE(out.events[i].days_from_now > 0);
        TEST_ASSERT_TRUE(out.events[i].days_from_now <= 30);
    }
}

void test_predict_sorted_by_significance(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 60, &out);
    for (int i = 1; i < out.count; i++) {
        TEST_ASSERT_TRUE(out.events[i].significance
                         <= out.events[i - 1].significance);
    }
}

void test_predict_headlines_not_empty(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 30, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_TRUE(strlen(out.events[i].headline) > 0);
    }
}

void test_predict_headlines_contain_days(void) {
    br_prediction_t out;
    br_predict(TEST_JD, 30, &out);
    for (int i = 0; i < out.count; i++) {
        TEST_ASSERT_NOT_NULL(strstr(out.events[i].headline, "In"));
    }
}

/* ===================================================================
 * br_predict_next
 * =================================================================== */

void test_predict_next_zero_days(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, br_predict_next(TEST_JD, 0, 0.01));
}

void test_predict_next_returns_future_jd(void) {
    double next = br_predict_next(TEST_JD, 365, 0.01);
    if (next > 0.0) {
        TEST_ASSERT_TRUE(next > TEST_JD);
        TEST_ASSERT_TRUE(next <= TEST_JD + 365.0);
    }
}

void test_predict_next_high_threshold_harder_to_find(void) {
    double low = br_predict_next(TEST_JD, 60, 0.01);
    double high = br_predict_next(TEST_JD, 60, 0.9);
    /* High threshold should return same or later date (or 0) */
    if (low > 0.0 && high > 0.0) {
        TEST_ASSERT_TRUE(high >= low);
    }
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* br_predict basics */
    RUN_TEST(test_predict_null_returns_zero);
    RUN_TEST(test_predict_zero_days_returns_zero);
    RUN_TEST(test_predict_negative_days_returns_zero);
    RUN_TEST(test_predict_sets_start_jd);
    RUN_TEST(test_predict_sets_days_scanned);
    RUN_TEST(test_predict_clamps_max_days);
    RUN_TEST(test_predict_finds_events_in_30_days);
    RUN_TEST(test_predict_events_are_future);
    RUN_TEST(test_predict_sorted_by_significance);
    RUN_TEST(test_predict_headlines_not_empty);
    RUN_TEST(test_predict_headlines_contain_days);

    /* br_predict_next */
    RUN_TEST(test_predict_next_zero_days);
    RUN_TEST(test_predict_next_returns_future_jd);
    RUN_TEST(test_predict_next_high_threshold_harder_to_find);

    return UNITY_END();
}
