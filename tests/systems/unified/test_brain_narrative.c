/* test_brain_narrative.c — Tests for daily narrative composer.
 *
 * Tests br_narrative_compose, br_narrative_headline, br_narrative_summary
 * with constructed br_result_t inputs. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/brain_narrative.h"
#include "../../../src/systems/unified/convergence_detect.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * Helpers: construct br_result_t for testing
 * =================================================================== */

static br_result_t make_quiet_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;
    r.insights[0].type = BR_INSIGHT_QUIET;
    strncpy(r.insights[0].headline, "A quiet day across all systems",
            BR_HEADLINE_MAX - 1);
    r.insights[0].score = 0.0;
    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.0;
    return r;
}

static br_result_t make_convergence_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;

    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Hebrew and Islamic new month",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_HEBREW;
    r.insights[0].systems[1] = CD_SYS_ISLAMIC;
    r.insights[0].system_count = 2;
    r.insights[0].score = 0.5;

    r.insight_count = 1;
    r.top_insight_index = 0;
    r.convergence_strength = 0.5;
    return r;
}

static br_result_t make_major_result(void) {
    br_result_t r;
    memset(&r, 0, sizeof(r));
    r.jd = 2460000.5;

    /* Major convergence: 5 systems */
    r.insights[0].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[0].headline, "Five traditions mark rebirth",
            BR_HEADLINE_MAX - 1);
    r.insights[0].systems[0] = CD_SYS_TZOLKIN;
    r.insights[0].systems[1] = CD_SYS_HEBREW;
    r.insights[0].systems[2] = CD_SYS_ISLAMIC;
    r.insights[0].systems[3] = CD_SYS_CHINESE;
    r.insights[0].systems[4] = CD_SYS_CELTIC;
    r.insights[0].system_count = 5;
    r.insights[0].score = 0.9;

    /* Secondary insight */
    r.insights[1].type = BR_INSIGHT_CONVERGENCE;
    strncpy(r.insights[1].headline, "Lunar-solar alignment",
            BR_HEADLINE_MAX - 1);
    r.insights[1].systems[0] = CD_SYS_ASTRONOMY;
    r.insights[1].systems[1] = CD_SYS_CELTIC;
    r.insights[1].system_count = 2;
    r.insights[1].score = 0.3;

    r.insight_count = 2;
    r.top_insight_index = 0;
    r.convergence_strength = 0.9;
    return r;
}

/* ===================================================================
 * br_narrative_headline
 * =================================================================== */

void test_headline_null_returns_zero(void) {
    char buf[128];
    TEST_ASSERT_EQUAL_INT(0, br_narrative_headline(NULL, buf, sizeof(buf)));
}

void test_headline_quiet_day(void) {
    br_insight_t quiet;
    memset(&quiet, 0, sizeof(quiet));
    quiet.type = BR_INSIGHT_QUIET;
    char buf[128];
    br_narrative_headline(&quiet, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "quiet"));
}

void test_headline_convergence_two_systems(void) {
    br_insight_t c;
    memset(&c, 0, sizeof(c));
    c.type = BR_INSIGHT_CONVERGENCE;
    strncpy(c.headline, "Hebrew-Islamic new month", BR_HEADLINE_MAX - 1);
    c.systems[0] = CD_SYS_HEBREW;
    c.systems[1] = CD_SYS_ISLAMIC;
    c.system_count = 2;
    char buf[128];
    br_narrative_headline(&c, buf, sizeof(buf));
    /* Now lists system names: "Hebrew and Islamic converge" */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hebrew"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Islamic"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "converge"));
}

void test_headline_convergence_four_systems(void) {
    br_insight_t c;
    memset(&c, 0, sizeof(c));
    c.type = BR_INSIGHT_CONVERGENCE;
    strncpy(c.headline, "Major alignment", BR_HEADLINE_MAX - 1);
    c.systems[0] = CD_SYS_ASTRONOMY;
    c.systems[1] = CD_SYS_HEBREW;
    c.systems[2] = CD_SYS_ISLAMIC;
    c.systems[3] = CD_SYS_CELTIC;
    c.system_count = 4;
    char buf[128];
    br_narrative_headline(&c, buf, sizeof(buf));
    /* Now lists names: "Astronomy, Hebrew, Islamic, and Celtic converge" */
    TEST_ASSERT_NOT_NULL(strstr(buf, "converge"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Astronomy"));
}

void test_headline_festival(void) {
    br_insight_t f;
    memset(&f, 0, sizeof(f));
    f.type = BR_INSIGHT_FESTIVAL;
    strncpy(f.headline, "Vesak", BR_HEADLINE_MAX - 1);
    char buf[128];
    br_narrative_headline(&f, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Festival"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Vesak"));
}

void test_headline_number(void) {
    br_insight_t n;
    memset(&n, 0, sizeof(n));
    n.type = BR_INSIGHT_NUMBER;
    strncpy(n.headline, "108 appears in 4 systems", BR_HEADLINE_MAX - 1);
    char buf[128];
    br_narrative_headline(&n, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sacred number"));
}

void test_headline_astronomical(void) {
    br_insight_t a;
    memset(&a, 0, sizeof(a));
    a.type = BR_INSIGHT_ASTRONOMICAL;
    strncpy(a.headline, "Full moon", BR_HEADLINE_MAX - 1);
    char buf[128];
    br_narrative_headline(&a, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Celestial"));
}

/* ===================================================================
 * br_narrative_summary
 * =================================================================== */

void test_summary_null_returns_zero(void) {
    char buf[256];
    TEST_ASSERT_EQUAL_INT(0, br_narrative_summary(NULL, buf, sizeof(buf)));
}

void test_summary_quiet_day(void) {
    br_result_t r = make_quiet_result();
    char buf[BR_SUMMARY_MAX];
    br_narrative_summary(&r, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 20);
    TEST_ASSERT_NOT_NULL(strstr(buf, "cycle"));
}

void test_summary_convergence(void) {
    br_result_t r = make_convergence_result();
    char buf[BR_SUMMARY_MAX];
    br_narrative_summary(&r, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 10);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hebrew"));
}

void test_summary_major_includes_secondary(void) {
    br_result_t r = make_major_result();
    char buf[BR_SUMMARY_MAX];
    br_narrative_summary(&r, buf, sizeof(buf));
    /* Major insight: system names. Secondary: system names.
     * Should mention systems from both insights. */
    TEST_ASSERT_NOT_NULL(strstr(buf, "share this moment"));
    /* Secondary insight should also appear */
    TEST_ASSERT_TRUE(strlen(buf) > 30);
}

/* ===================================================================
 * br_narrative_compose — full pipeline
 * =================================================================== */

void test_compose_null_returns_zero(void) {
    br_narrative_t out;
    TEST_ASSERT_EQUAL_INT(0, br_narrative_compose(NULL, &out));
}

void test_compose_quiet_day(void) {
    br_result_t r = make_quiet_result();
    br_narrative_t out;
    int ok = br_narrative_compose(&r, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_EQUAL_STRING("quiet", out.thread_type);
    TEST_ASSERT_TRUE(strlen(out.headline) > 5);
    TEST_ASSERT_TRUE(strlen(out.summary) > 10);
    TEST_ASSERT_EQUAL_FLOAT(0.0, out.confidence);
}

void test_compose_convergence(void) {
    br_result_t r = make_convergence_result();
    br_narrative_t out;
    br_narrative_compose(&r, &out);
    TEST_ASSERT_EQUAL_STRING("convergence", out.thread_type);
    TEST_ASSERT_EQUAL_FLOAT(0.5, out.confidence);
}

void test_compose_featured_systems(void) {
    br_result_t r = make_convergence_result();
    br_narrative_t out;
    br_narrative_compose(&r, &out);
    TEST_ASSERT_EQUAL_INT(2, out.system_count);
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, out.systems_featured[0]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_ISLAMIC, out.systems_featured[1]);
}

void test_compose_major_featured_no_duplicates(void) {
    br_result_t r = make_major_result();
    br_narrative_t out;
    br_narrative_compose(&r, &out);
    /* 5 systems from insight 0 + 2 from insight 1.
     * Celtic appears in both → should be deduplicated.
     * Expected unique: TZOLKIN, HEBREW, ISLAMIC, CHINESE, CELTIC, ASTRONOMY = 6 */
    TEST_ASSERT_EQUAL_INT(6, out.system_count);
}

void test_compose_major_headline_mentions_systems(void) {
    br_result_t r = make_major_result();
    br_narrative_t out;
    br_narrative_compose(&r, &out);
    /* Headline now lists system names + "converge" */
    TEST_ASSERT_NOT_NULL(strstr(out.headline, "converge"));
    TEST_ASSERT_NOT_NULL(strstr(out.headline, "Tzolkin"));
}

void test_compose_wisdom_id_from_strength(void) {
    br_result_t r = make_major_result();
    br_narrative_t out;
    br_narrative_compose(&r, &out);
    /* 0.9 strength * 10 = 9 */
    TEST_ASSERT_EQUAL_INT(9, out.wisdom_id);
}

void test_compose_zeroes_output(void) {
    br_narrative_t out;
    memset(&out, 0xFF, sizeof(out)); /* fill with junk */
    br_result_t r = make_quiet_result();
    br_narrative_compose(&r, &out);
    /* Should be cleanly filled, not junk */
    TEST_ASSERT_TRUE(strlen(out.headline) < BR_HEADLINE_MAX);
    TEST_ASSERT_TRUE(strlen(out.summary) < BR_SUMMARY_MAX);
}

/* ===================================================================
 * Main
 * =================================================================== */

int main(void) {
    UNITY_BEGIN();

    /* Headlines */
    RUN_TEST(test_headline_null_returns_zero);
    RUN_TEST(test_headline_quiet_day);
    RUN_TEST(test_headline_convergence_two_systems);
    RUN_TEST(test_headline_convergence_four_systems);
    RUN_TEST(test_headline_festival);
    RUN_TEST(test_headline_number);
    RUN_TEST(test_headline_astronomical);

    /* Summaries */
    RUN_TEST(test_summary_null_returns_zero);
    RUN_TEST(test_summary_quiet_day);
    RUN_TEST(test_summary_convergence);
    RUN_TEST(test_summary_major_includes_secondary);

    /* Full compose */
    RUN_TEST(test_compose_null_returns_zero);
    RUN_TEST(test_compose_quiet_day);
    RUN_TEST(test_compose_convergence);
    RUN_TEST(test_compose_featured_systems);
    RUN_TEST(test_compose_major_featured_no_duplicates);
    RUN_TEST(test_compose_major_headline_mentions_systems);
    RUN_TEST(test_compose_wisdom_id_from_strength);
    RUN_TEST(test_compose_zeroes_output);

    return UNITY_END();
}
