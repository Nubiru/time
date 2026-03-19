/* test_sensitivity.c -- Cultural sensitivity tags tests.
 *
 * Tests sensitivity_name(), wisdom_by_sensitivity(),
 * cs_by_sensitivity(), ff_by_sensitivity(), and sum invariants. */

#include "../../tests/unity/unity.h"
#include "../../src/ui/sensitivity.h"
#include "../../src/systems/unified/wisdom.h"
#include "../../src/systems/unified/cultural_stories.h"
#include "../../src/systems/unified/fun_facts.h"

void setUp(void) {}
void tearDown(void) {}

/* ===== sensitivity_name ===== */

void test_sensitivity_name_general(void)
{
    TEST_ASSERT_EQUAL_STRING("General", sensitivity_name(SENSITIVITY_GENERAL));
}

void test_sensitivity_name_respectful(void)
{
    TEST_ASSERT_EQUAL_STRING("Respectful", sensitivity_name(SENSITIVITY_RESPECTFUL));
}

void test_sensitivity_name_sacred(void)
{
    TEST_ASSERT_EQUAL_STRING("Sacred", sensitivity_name(SENSITIVITY_SACRED));
}

void test_sensitivity_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", sensitivity_name((sensitivity_t)99));
}

void test_sensitivity_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", sensitivity_name((sensitivity_t)-1));
}

/* ===== wisdom_by_sensitivity ===== */

void test_wisdom_general_count_positive(void)
{
    int results[128];
    int count = wisdom_by_sensitivity(SENSITIVITY_GENERAL, results, 128);
    TEST_ASSERT_TRUE(count > 0);
}

void test_wisdom_respectful_count_positive(void)
{
    int results[128];
    int count = wisdom_by_sensitivity(SENSITIVITY_RESPECTFUL, results, 128);
    TEST_ASSERT_TRUE(count > 0);
}

void test_wisdom_sacred_count_small(void)
{
    int results[128];
    int count = wisdom_by_sensitivity(SENSITIVITY_SACRED, results, 128);
    TEST_ASSERT_TRUE(count >= 0);
    TEST_ASSERT_TRUE(count < 10);
}

void test_wisdom_sensitivity_sums_to_total(void)
{
    int results[128];
    int general = wisdom_by_sensitivity(SENSITIVITY_GENERAL, results, 128);
    int respectful = wisdom_by_sensitivity(SENSITIVITY_RESPECTFUL, results, 128);
    int sacred = wisdom_by_sensitivity(SENSITIVITY_SACRED, results, 128);
    TEST_ASSERT_EQUAL_INT(wisdom_quote_count(), general + respectful + sacred);
}

void test_wisdom_by_sensitivity_null_results(void)
{
    int count = wisdom_by_sensitivity(SENSITIVITY_GENERAL, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_wisdom_by_sensitivity_zero_max(void)
{
    int results[1];
    int count = wisdom_by_sensitivity(SENSITIVITY_GENERAL, results, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_wisdom_by_sensitivity_invalid_level(void)
{
    int results[10];
    int count = wisdom_by_sensitivity((sensitivity_t)99, results, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_wisdom_sacred_have_work_field(void)
{
    int results[16];
    int count = wisdom_by_sensitivity(SENSITIVITY_SACRED, results, 16);
    for (int i = 0; i < count; i++) {
        wisdom_t q = wisdom_quote_get(results[i]);
        TEST_ASSERT_TRUE_MESSAGE(q.work != NULL || q.author != NULL,
                                 "Sacred quotes should have attribution");
    }
}

void test_wisdom_general_includes_einstein(void)
{
    int idx = wisdom_by_author("Einstein");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_EQUAL_INT(SENSITIVITY_GENERAL, q.sensitivity);
}

void test_wisdom_respectful_includes_rumi(void)
{
    int idx = wisdom_by_author("Rumi");
    TEST_ASSERT_TRUE(idx >= 0);
    wisdom_t q = wisdom_quote_get(idx);
    TEST_ASSERT_EQUAL_INT(SENSITIVITY_RESPECTFUL, q.sensitivity);
}

/* ===== cs_by_sensitivity ===== */

void test_cs_general_count_positive(void)
{
    int results[64];
    int count = cs_by_sensitivity(SENSITIVITY_GENERAL, results, 64);
    TEST_ASSERT_TRUE(count > 0);
}

void test_cs_respectful_count_positive(void)
{
    int results[64];
    int count = cs_by_sensitivity(SENSITIVITY_RESPECTFUL, results, 64);
    TEST_ASSERT_TRUE(count > 0);
}

void test_cs_sensitivity_sums_to_total(void)
{
    int results[64];
    int general = cs_by_sensitivity(SENSITIVITY_GENERAL, results, 64);
    int respectful = cs_by_sensitivity(SENSITIVITY_RESPECTFUL, results, 64);
    int sacred = cs_by_sensitivity(SENSITIVITY_SACRED, results, 64);
    TEST_ASSERT_EQUAL_INT(cs_story_count(), general + respectful + sacred);
}

void test_cs_by_sensitivity_null_results(void)
{
    int count = cs_by_sensitivity(SENSITIVITY_GENERAL, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_cs_by_sensitivity_invalid_level(void)
{
    int results[10];
    int count = cs_by_sensitivity((sensitivity_t)-1, results, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== ff_by_sensitivity ===== */

void test_ff_general_count_positive(void)
{
    int results[128];
    int count = ff_by_sensitivity(SENSITIVITY_GENERAL, results, 128);
    TEST_ASSERT_TRUE(count > 0);
}

void test_ff_respectful_count_positive(void)
{
    int results[128];
    int count = ff_by_sensitivity(SENSITIVITY_RESPECTFUL, results, 128);
    TEST_ASSERT_TRUE(count > 0);
}

void test_ff_sensitivity_sums_to_total(void)
{
    int results[128];
    int general = ff_by_sensitivity(SENSITIVITY_GENERAL, results, 128);
    int respectful = ff_by_sensitivity(SENSITIVITY_RESPECTFUL, results, 128);
    int sacred = ff_by_sensitivity(SENSITIVITY_SACRED, results, 128);
    TEST_ASSERT_EQUAL_INT(ff_fact_count(), general + respectful + sacred);
}

void test_ff_by_sensitivity_null_results(void)
{
    int count = ff_by_sensitivity(SENSITIVITY_GENERAL, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_ff_by_sensitivity_invalid_level(void)
{
    int results[10];
    int count = ff_by_sensitivity((sensitivity_t)99, results, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_ff_general_majority(void)
{
    int results[128];
    int general = ff_by_sensitivity(SENSITIVITY_GENERAL, results, 128);
    int total = ff_fact_count();
    TEST_ASSERT_TRUE(general > total / 2);
}

/* ===== sensitivity field accessible via get ===== */

void test_wisdom_get_has_sensitivity(void)
{
    wisdom_t q = wisdom_quote_get(0);
    TEST_ASSERT_TRUE((int)q.sensitivity >= 0);
    TEST_ASSERT_TRUE(q.sensitivity < SENSITIVITY_COUNT);
}

void test_cs_get_has_sensitivity(void)
{
    cs_story_t s = cs_story_get(0);
    TEST_ASSERT_TRUE((int)s.sensitivity >= 0);
    TEST_ASSERT_TRUE(s.sensitivity < SENSITIVITY_COUNT);
}

void test_ff_get_has_sensitivity(void)
{
    ff_fact_t f = ff_fact_get(0);
    TEST_ASSERT_TRUE((int)f.sensitivity >= 0);
    TEST_ASSERT_TRUE(f.sensitivity < SENSITIVITY_COUNT);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_sensitivity_name_general);
    RUN_TEST(test_sensitivity_name_respectful);
    RUN_TEST(test_sensitivity_name_sacred);
    RUN_TEST(test_sensitivity_name_invalid);
    RUN_TEST(test_sensitivity_name_negative);

    RUN_TEST(test_wisdom_general_count_positive);
    RUN_TEST(test_wisdom_respectful_count_positive);
    RUN_TEST(test_wisdom_sacred_count_small);
    RUN_TEST(test_wisdom_sensitivity_sums_to_total);
    RUN_TEST(test_wisdom_by_sensitivity_null_results);
    RUN_TEST(test_wisdom_by_sensitivity_zero_max);
    RUN_TEST(test_wisdom_by_sensitivity_invalid_level);
    RUN_TEST(test_wisdom_sacred_have_work_field);
    RUN_TEST(test_wisdom_general_includes_einstein);
    RUN_TEST(test_wisdom_respectful_includes_rumi);

    RUN_TEST(test_cs_general_count_positive);
    RUN_TEST(test_cs_respectful_count_positive);
    RUN_TEST(test_cs_sensitivity_sums_to_total);
    RUN_TEST(test_cs_by_sensitivity_null_results);
    RUN_TEST(test_cs_by_sensitivity_invalid_level);

    RUN_TEST(test_ff_general_count_positive);
    RUN_TEST(test_ff_respectful_count_positive);
    RUN_TEST(test_ff_sensitivity_sums_to_total);
    RUN_TEST(test_ff_by_sensitivity_null_results);
    RUN_TEST(test_ff_by_sensitivity_invalid_level);
    RUN_TEST(test_ff_general_majority);

    RUN_TEST(test_wisdom_get_has_sensitivity);
    RUN_TEST(test_cs_get_has_sensitivity);
    RUN_TEST(test_ff_get_has_sensitivity);

    return UNITY_END();
}
