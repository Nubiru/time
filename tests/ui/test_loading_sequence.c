#include "../unity/unity.h"
#include "../../src/ui/loading_sequence.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ==== Initialization ==== */

void test_ls_init(void)
{
    ls_state_t s = ls_init();
    TEST_ASSERT_EQUAL_INT(LS_PHASE_INIT, s.current_phase);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.phase_progress);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.total_progress);
    TEST_ASSERT_EQUAL_INT(0, s.quote_index);
}

/* ==== Phase count ==== */

void test_ls_phase_count(void)
{
    TEST_ASSERT_EQUAL_INT(LS_PHASE_COUNT, ls_phase_count());
    TEST_ASSERT_EQUAL_INT(8, ls_phase_count());
}

/* ==== Phase info ==== */

void test_ls_phase_info_valid(void)
{
    for (int i = 0; i < LS_PHASE_COUNT; i++) {
        ls_phase_info_t info = ls_phase_info((ls_phase_t)i);
        TEST_ASSERT_EQUAL_INT(i, info.phase);
        TEST_ASSERT_NOT_NULL(info.name);
        TEST_ASSERT_TRUE(strlen(info.name) > 0);
        TEST_ASSERT_NOT_NULL(info.description);
        TEST_ASSERT_TRUE(strlen(info.description) > 0);
        TEST_ASSERT_TRUE(info.weight > 0.0f);
    }
}

void test_ls_phase_info_weights_sum(void)
{
    float sum = 0.0f;
    for (int i = 0; i < LS_PHASE_COUNT; i++) {
        ls_phase_info_t info = ls_phase_info((ls_phase_t)i);
        sum += info.weight;
    }
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, sum);
}

void test_ls_phase_info_progress_ordered(void)
{
    float prev_start = -1.0f;
    for (int i = 0; i < LS_PHASE_COUNT; i++) {
        ls_phase_info_t info = ls_phase_info((ls_phase_t)i);
        TEST_ASSERT_TRUE(info.progress_start > prev_start ||
                         (i == 0 && info.progress_start == 0.0f));
        TEST_ASSERT_TRUE(info.progress_end > info.progress_start ||
                         (i == LS_PHASE_COUNT - 1));
        prev_start = info.progress_start;
    }
}

void test_ls_phase_info_progress_bounds(void)
{
    ls_phase_info_t first = ls_phase_info(LS_PHASE_INIT);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, first.progress_start);

    ls_phase_info_t last = ls_phase_info(LS_PHASE_COMPLETE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, last.progress_end);
}

void test_ls_phase_info_invalid(void)
{
    ls_phase_info_t info = ls_phase_info((ls_phase_t)99);
    TEST_ASSERT_NULL(info.name);
    TEST_ASSERT_NULL(info.description);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, info.weight);
}

/* ==== Advance phase ==== */

void test_ls_advance_phase(void)
{
    ls_state_t s = ls_init();
    s.phase_progress = 0.75f;

    ls_state_t next = ls_advance_phase(s);
    TEST_ASSERT_EQUAL_INT(LS_PHASE_ASTRONOMY, next.current_phase);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, next.phase_progress);
}

void test_ls_advance_phase_sequential(void)
{
    ls_state_t s = ls_init();
    for (int i = 0; i < LS_PHASE_COUNT - 1; i++) {
        s = ls_advance_phase(s);
        TEST_ASSERT_EQUAL_INT(i + 1, s.current_phase);
    }
}

void test_ls_advance_past_complete(void)
{
    ls_state_t s = ls_init();
    /* Advance to COMPLETE */
    for (int i = 0; i < LS_PHASE_COUNT; i++) {
        s = ls_advance_phase(s);
    }
    TEST_ASSERT_EQUAL_INT(LS_PHASE_COMPLETE, s.current_phase);

    /* Advancing past COMPLETE stays at COMPLETE */
    ls_state_t beyond = ls_advance_phase(s);
    TEST_ASSERT_EQUAL_INT(LS_PHASE_COMPLETE, beyond.current_phase);
}

/* ==== Set progress ==== */

void test_ls_set_progress(void)
{
    ls_state_t s = ls_init();
    ls_state_t updated = ls_set_progress(s, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, updated.phase_progress);
    TEST_ASSERT_TRUE(updated.total_progress > 0.0f);
}

void test_ls_set_progress_clamps(void)
{
    ls_state_t s = ls_init();

    ls_state_t lo = ls_set_progress(s, -0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, lo.phase_progress);

    ls_state_t hi = ls_set_progress(s, 1.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, hi.phase_progress);
}

/* ==== Total progress ==== */

void test_ls_total_progress_start(void)
{
    float p = ls_total_progress(LS_PHASE_INIT, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, p);
}

void test_ls_total_progress_end(void)
{
    float p = ls_total_progress(LS_PHASE_COMPLETE, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, p);
}

void test_ls_total_progress_mid(void)
{
    /* Middle phase at 50% should give an intermediate total */
    float p = ls_total_progress(LS_PHASE_KNOWLEDGE, 0.5f);
    TEST_ASSERT_TRUE(p > 0.0f);
    TEST_ASSERT_TRUE(p < 1.0f);
}

void test_ls_total_progress_monotonic(void)
{
    /* As phases advance, total progress should increase */
    float prev = 0.0f;
    for (int i = 0; i < LS_PHASE_COUNT; i++) {
        float p = ls_total_progress((ls_phase_t)i, 0.5f);
        TEST_ASSERT_TRUE(p >= prev);
        prev = p;
    }
}

/* ==== Quotes ==== */

void test_ls_quote_count(void)
{
    int count = ls_quote_count();
    TEST_ASSERT_TRUE(count >= 8);
    TEST_ASSERT_TRUE(count <= LS_MAX_QUOTES);
}

void test_ls_quote_for_phase(void)
{
    ls_quote_t q = ls_quote_for_phase(LS_PHASE_ASTRONOMY, 42);
    TEST_ASSERT_NOT_NULL(q.text);
    TEST_ASSERT_TRUE(strlen(q.text) > 0);
    TEST_ASSERT_NOT_NULL(q.author);
    TEST_ASSERT_TRUE(strlen(q.author) > 0);
}

void test_ls_quote_deterministic(void)
{
    ls_quote_t q1 = ls_quote_for_phase(LS_PHASE_CALENDARS, 123);
    ls_quote_t q2 = ls_quote_for_phase(LS_PHASE_CALENDARS, 123);
    TEST_ASSERT_EQUAL_STRING(q1.text, q2.text);
    TEST_ASSERT_EQUAL_STRING(q1.author, q2.author);
}

void test_ls_quote_at_valid(void)
{
    for (int i = 0; i < ls_quote_count(); i++) {
        ls_quote_t q = ls_quote_at(i);
        TEST_ASSERT_NOT_NULL(q.text);
        TEST_ASSERT_NOT_NULL(q.author);
    }
}

void test_ls_quote_at_invalid(void)
{
    ls_quote_t q = ls_quote_at(-1);
    TEST_ASSERT_NULL(q.text);
    TEST_ASSERT_NULL(q.author);

    ls_quote_t q2 = ls_quote_at(999);
    TEST_ASSERT_NULL(q2.text);
    TEST_ASSERT_NULL(q2.author);
}

void test_ls_quote_phase_affinity(void)
{
    /* Quotes returned for a specific phase should have matching affinity
     * or be general (-1) */
    ls_quote_t q = ls_quote_for_phase(LS_PHASE_INIT, 0);
    TEST_ASSERT_TRUE(q.phase_affinity == LS_PHASE_INIT ||
                     q.phase_affinity == -1);
}

/* ==== Completeness ==== */

void test_ls_is_complete_false(void)
{
    ls_state_t s = ls_init();
    TEST_ASSERT_FALSE(ls_is_complete(s));
}

void test_ls_is_complete_mid(void)
{
    ls_state_t s = ls_init();
    s.current_phase = LS_PHASE_GEOMETRY;
    TEST_ASSERT_FALSE(ls_is_complete(s));
}

void test_ls_is_complete_true(void)
{
    ls_state_t s = ls_init();
    s.current_phase = LS_PHASE_COMPLETE;
    s.phase_progress = 1.0f;
    TEST_ASSERT_TRUE(ls_is_complete(s));
}

/* ==== Title and subtitle ==== */

void test_ls_title(void)
{
    const char *t = ls_title();
    TEST_ASSERT_NOT_NULL(t);
    TEST_ASSERT_TRUE(strlen(t) > 0);
}

void test_ls_subtitle(void)
{
    const char *s = ls_subtitle();
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_ls_title_content(void)
{
    TEST_ASSERT_EQUAL_STRING("TIME", ls_title());
}

void test_ls_subtitle_content(void)
{
    TEST_ASSERT_EQUAL_STRING("Every pixel earned.", ls_subtitle());
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* Initialization */
    RUN_TEST(test_ls_init);

    /* Phase count */
    RUN_TEST(test_ls_phase_count);

    /* Phase info */
    RUN_TEST(test_ls_phase_info_valid);
    RUN_TEST(test_ls_phase_info_weights_sum);
    RUN_TEST(test_ls_phase_info_progress_ordered);
    RUN_TEST(test_ls_phase_info_progress_bounds);
    RUN_TEST(test_ls_phase_info_invalid);

    /* Advance phase */
    RUN_TEST(test_ls_advance_phase);
    RUN_TEST(test_ls_advance_phase_sequential);
    RUN_TEST(test_ls_advance_past_complete);

    /* Set progress */
    RUN_TEST(test_ls_set_progress);
    RUN_TEST(test_ls_set_progress_clamps);

    /* Total progress */
    RUN_TEST(test_ls_total_progress_start);
    RUN_TEST(test_ls_total_progress_end);
    RUN_TEST(test_ls_total_progress_mid);
    RUN_TEST(test_ls_total_progress_monotonic);

    /* Quotes */
    RUN_TEST(test_ls_quote_count);
    RUN_TEST(test_ls_quote_for_phase);
    RUN_TEST(test_ls_quote_deterministic);
    RUN_TEST(test_ls_quote_at_valid);
    RUN_TEST(test_ls_quote_at_invalid);
    RUN_TEST(test_ls_quote_phase_affinity);

    /* Completeness */
    RUN_TEST(test_ls_is_complete_false);
    RUN_TEST(test_ls_is_complete_mid);
    RUN_TEST(test_ls_is_complete_true);

    /* Title and subtitle */
    RUN_TEST(test_ls_title);
    RUN_TEST(test_ls_subtitle);
    RUN_TEST(test_ls_title_content);
    RUN_TEST(test_ls_subtitle_content);

    return UNITY_END();
}
