/* test_narrative.c — Tests for the narrative composer (LANGUAGE L1.1)
 *
 * TDD: these tests define the expected behavior of narrative.h/.c.
 * The narrative composer takes brain_language_bridge context + wisdom +
 * depth tier and produces human-readable composed text. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/narrative.h"

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Helper: build a minimal br_lang_context_t with filled slots
 * ================================================================ */

static br_lang_context_t make_context(const char *headline,
                                       const char *thread_type,
                                       const char *conv_adj,
                                       const char *sys_count,
                                       double strength)
{
    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.convergence_strength = strength;
    ctx.show_wisdom = (strength >= 0.6) ? 1 : 0;
    ctx.wisdom_id = (strength >= 0.6) ? 7 : -1;

    /* Fill slots manually */
    if (headline) {
        ctx.slots[BR_LANG_SLOT_HEADLINE].type = BR_LANG_SLOT_HEADLINE;
        snprintf(ctx.slots[BR_LANG_SLOT_HEADLINE].value,
                 BR_LANG_SLOT_MAX, "%s", headline);
        ctx.slots[BR_LANG_SLOT_HEADLINE].filled = 1;
        ctx.slot_count++;
    }
    if (thread_type) {
        ctx.slots[BR_LANG_SLOT_THREAD_TYPE].type = BR_LANG_SLOT_THREAD_TYPE;
        snprintf(ctx.slots[BR_LANG_SLOT_THREAD_TYPE].value,
                 BR_LANG_SLOT_MAX, "%s", thread_type);
        ctx.slots[BR_LANG_SLOT_THREAD_TYPE].filled = 1;
        ctx.slot_count++;
    }
    if (conv_adj) {
        ctx.slots[BR_LANG_SLOT_CONVERGENCE_ADJ].type = BR_LANG_SLOT_CONVERGENCE_ADJ;
        snprintf(ctx.slots[BR_LANG_SLOT_CONVERGENCE_ADJ].value,
                 BR_LANG_SLOT_MAX, "%s", conv_adj);
        ctx.slots[BR_LANG_SLOT_CONVERGENCE_ADJ].filled = 1;
        ctx.slot_count++;
    }
    if (sys_count) {
        ctx.slots[BR_LANG_SLOT_SYSTEM_COUNT].type = BR_LANG_SLOT_SYSTEM_COUNT;
        snprintf(ctx.slots[BR_LANG_SLOT_SYSTEM_COUNT].value,
                 BR_LANG_SLOT_MAX, "%s", sys_count);
        ctx.slots[BR_LANG_SLOT_SYSTEM_COUNT].filled = 1;
        ctx.slot_count++;
    }
    return ctx;
}

/* ================================================================
 * narr_init: initialization produces valid state
 * ================================================================ */

void test_narr_init_produces_valid_state(void)
{
    narr_state_t state;
    narr_init(&state);
    TEST_ASSERT_EQUAL_INT(0, state.compose_count);
    TEST_ASSERT_EQUAL_INT(0, state.seen_count);
}

/* ================================================================
 * narr_compose: basic composition with convergence thread
 * ================================================================ */

void test_narr_compose_convergence_produces_text(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Three lunar cycles align",
        "convergence",
        "rare",
        "3 systems",
        0.75
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = "What you seek is seeking you.",
        .wisdom_author = "Rumi",
        .wisdom_work = "Masnavi",
        .wisdom_year = 1258,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = "Tzolkin"
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
    TEST_ASSERT_TRUE(out.has_wisdom);
    TEST_ASSERT_TRUE(out.wisdom_text[0] != '\0');
    TEST_ASSERT_EQUAL_INT(1, state.compose_count);
}

/* ================================================================
 * narr_compose: quiet thread produces simpler text
 * ================================================================ */

void test_narr_compose_quiet_no_wisdom(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "An ordinary day unfolds",
        "quiet",
        "ordinary",
        "2 systems",
        0.15
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_SURFACE,
        .system_name = NULL
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
    TEST_ASSERT_FALSE(out.has_wisdom);
}

/* ================================================================
 * narr_compose: NULL brain context returns 0
 * ================================================================ */

void test_narr_compose_null_ctx_returns_zero(void)
{
    narr_state_t state;
    narr_init(&state);

    narr_input_t input = {
        .brain_ctx = NULL,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_SURFACE,
        .system_name = NULL
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(0, result);
}

/* ================================================================
 * narr_compose: NULL output returns 0
 * ================================================================ */

void test_narr_compose_null_output_returns_zero(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context("headline", "quiet", "ordinary", "1 system", 0.1);
    narr_input_t input = { .brain_ctx = &ctx };

    int result = narr_compose(&state, &input, NULL);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* ================================================================
 * Depth levels: surface gives shorter text
 * ================================================================ */

void test_narr_compose_surface_is_shorter(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Convergence of 4 systems",
        "convergence",
        "significant",
        "4 systems",
        0.55
    );

    narr_input_t input_surface = {
        .brain_ctx = &ctx,
        .wisdom_text = "Time is a river.",
        .wisdom_author = "Marcus Aurelius",
        .wisdom_work = NULL,
        .wisdom_year = 170,
        .depth = NARR_DEPTH_SURFACE,
        .system_name = "Astrology"
    };

    narr_input_t input_full = input_surface;
    input_full.depth = NARR_DEPTH_FULL;

    narr_output_t out_surface, out_full;
    narr_compose(&state, &input_surface, &out_surface);
    narr_compose(&state, &input_full, &out_full);

    /* Surface text should be shorter than full narrative */
    TEST_ASSERT_TRUE(strlen(out_surface.body) <= strlen(out_full.body));
}

/* ================================================================
 * narr_compose: festival thread mentions celebration
 * ================================================================ */

void test_narr_compose_festival_thread(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Today is the Spring Equinox",
        "festival",
        "significant",
        "5 systems",
        0.65
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = "Gregorian"
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
}

/* ================================================================
 * narr_compose: wisdom attribution formatted correctly
 * ================================================================ */

void test_narr_compose_wisdom_attribution(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Alignment",
        "convergence",
        "rare",
        "3 systems",
        0.8
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = "Know thyself.",
        .wisdom_author = "Oracle at Delphi",
        .wisdom_work = NULL,
        .wisdom_year = -600,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = NULL
    };

    narr_output_t out;
    narr_compose(&state, &input, &out);

    TEST_ASSERT_TRUE(out.has_wisdom);
    /* Attribution must include author */
    TEST_ASSERT_NOT_NULL(strstr(out.wisdom_text, "Oracle at Delphi"));
}

/* ================================================================
 * narr_compose: wisdom with work and year BCE
 * ================================================================ */

void test_narr_compose_wisdom_with_work_and_bce(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Headline", "convergence", "rare", "2 systems", 0.7
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = "The unexamined life is not worth living.",
        .wisdom_author = "Socrates",
        .wisdom_work = "Apology",
        .wisdom_year = -399,
        .depth = NARR_DEPTH_FULL,
        .system_name = NULL
    };

    narr_output_t out;
    narr_compose(&state, &input, &out);

    TEST_ASSERT_TRUE(out.has_wisdom);
    TEST_ASSERT_NOT_NULL(strstr(out.wisdom_text, "Socrates"));
    TEST_ASSERT_NOT_NULL(strstr(out.wisdom_text, "Apology"));
    TEST_ASSERT_NOT_NULL(strstr(out.wisdom_text, "BCE"));
}

/* ================================================================
 * narr_compose: wisdom with CE year
 * ================================================================ */

void test_narr_compose_wisdom_with_ce_year(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Headline", "quiet", "ordinary", "1 system", 0.7
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = "The wound is the place where the Light enters you.",
        .wisdom_author = "Rumi",
        .wisdom_work = "Masnavi",
        .wisdom_year = 1258,
        .depth = NARR_DEPTH_FULL,
        .system_name = NULL
    };

    narr_output_t out;
    narr_compose(&state, &input, &out);

    TEST_ASSERT_TRUE(out.has_wisdom);
    TEST_ASSERT_NOT_NULL(strstr(out.wisdom_text, "1258 CE"));
}

/* ================================================================
 * Novelty: seen template IDs tracked
 * ================================================================ */

void test_narr_novelty_tracking(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Headline A", "convergence", "notable", "2 systems", 0.5
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = NULL
    };

    narr_output_t out1, out2;
    narr_compose(&state, &input, &out1);
    TEST_ASSERT_EQUAL_INT(1, state.compose_count);
    TEST_ASSERT_TRUE(state.seen_count > 0);

    /* Second composition uses same thread type — seen tracker updates */
    narr_compose(&state, &input, &out2);
    TEST_ASSERT_EQUAL_INT(2, state.compose_count);
}

/* ================================================================
 * narr_compose: number thread
 * ================================================================ */

void test_narr_compose_number_thread(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "The number 108 appears across 4 systems",
        "number",
        "significant",
        "4 systems",
        0.55
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = "Buddhist"
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
}

/* ================================================================
 * narr_compose: cycle thread
 * ================================================================ */

void test_narr_compose_cycle_thread(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "A new Metonic cycle begins",
        "cycle",
        "rare",
        "2 systems",
        0.7
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_CYCLE,
        .system_name = "Hebrew"
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
}

/* ================================================================
 * narr_compose: astronomical thread
 * ================================================================ */

void test_narr_compose_astronomical_thread(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Mars enters retrograde",
        "astronomical",
        "notable",
        "3 systems",
        0.45
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = "Astronomy"
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
}

/* ================================================================
 * narr_depth_name: name lookup
 * ================================================================ */

void test_narr_depth_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Surface", narr_depth_name(NARR_DEPTH_SURFACE));
    TEST_ASSERT_EQUAL_STRING("Context", narr_depth_name(NARR_DEPTH_CONTEXT));
    TEST_ASSERT_EQUAL_STRING("Cycle", narr_depth_name(NARR_DEPTH_CYCLE));
    TEST_ASSERT_EQUAL_STRING("Full", narr_depth_name(NARR_DEPTH_FULL));
    TEST_ASSERT_EQUAL_STRING("?", narr_depth_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", narr_depth_name(99));
}

/* ================================================================
 * narr_thread_index: thread type to index
 * ================================================================ */

void test_narr_thread_index(void)
{
    TEST_ASSERT_EQUAL_INT(0, narr_thread_index("convergence"));
    TEST_ASSERT_EQUAL_INT(1, narr_thread_index("festival"));
    TEST_ASSERT_EQUAL_INT(2, narr_thread_index("number"));
    TEST_ASSERT_EQUAL_INT(3, narr_thread_index("cycle"));
    TEST_ASSERT_EQUAL_INT(4, narr_thread_index("astronomical"));
    TEST_ASSERT_EQUAL_INT(5, narr_thread_index("quiet"));
    TEST_ASSERT_EQUAL_INT(-1, narr_thread_index("unknown"));
    TEST_ASSERT_EQUAL_INT(-1, narr_thread_index(NULL));
}

/* ================================================================
 * narr_format_attribution: formatting
 * ================================================================ */

void test_narr_format_attribution_full(void)
{
    char buf[256];
    int len = narr_format_attribution("Rumi", "Masnavi", 1258, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_EQUAL_STRING("Rumi, Masnavi (1258 CE)", buf);
}

void test_narr_format_attribution_no_work(void)
{
    char buf[256];
    int len = narr_format_attribution("Plato", NULL, -380, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_EQUAL_STRING("Plato (380 BCE)", buf);
}

void test_narr_format_attribution_no_year(void)
{
    char buf[256];
    int len = narr_format_attribution("Anonymous", NULL, 0, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_EQUAL_STRING("Anonymous", buf);
}

void test_narr_format_attribution_null_author(void)
{
    char buf[256];
    int len = narr_format_attribution(NULL, NULL, 0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
    TEST_ASSERT_EQUAL_STRING("", buf);
}

/* ================================================================
 * narr_compose: personal note slot fills
 * ================================================================ */

void test_narr_compose_with_personal_note(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Alignment with your birth seal",
        "convergence",
        "extraordinary",
        "5 systems",
        0.95
    );
    /* Add personal note slot */
    ctx.slots[BR_LANG_SLOT_PERSONAL_NOTE].type = BR_LANG_SLOT_PERSONAL_NOTE;
    snprintf(ctx.slots[BR_LANG_SLOT_PERSONAL_NOTE].value,
             BR_LANG_SLOT_MAX, "Your birth kin resonates today");
    ctx.slots[BR_LANG_SLOT_PERSONAL_NOTE].filled = 1;
    ctx.slot_count++;

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = "As above, so below.",
        .wisdom_author = "Hermes Trismegistus",
        .wisdom_work = "Emerald Tablet",
        .wisdom_year = -200,
        .depth = NARR_DEPTH_FULL,
        .system_name = "Human Design"
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.has_personal);
    TEST_ASSERT_TRUE(out.personal[0] != '\0');
}

/* ================================================================
 * narr_compose: depth SURFACE only shows headline
 * ================================================================ */

void test_narr_compose_depth_surface_minimal(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "Short headline",
        "quiet",
        "ordinary",
        "1 system",
        0.1
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = "Long wisdom that should not appear at surface depth.",
        .wisdom_author = "Philosopher",
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_SURFACE,
        .system_name = NULL
    };

    narr_output_t out;
    narr_compose(&state, &input, &out);

    /* At surface depth, wisdom is suppressed */
    TEST_ASSERT_FALSE(out.has_wisdom);
}

/* ================================================================
 * narr_compose: handles empty headline gracefully
 * ================================================================ */

void test_narr_compose_empty_headline(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.convergence_strength = 0.1;

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL,
        .wisdom_author = NULL,
        .wisdom_work = NULL,
        .wisdom_year = 0,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = NULL
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    /* Should still produce something (fallback) */
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(out.body[0] != '\0');
}

/* ================================================================
 * Locale tests
 * ================================================================ */

static void test_narr_compose_default_locale_is_en(void)
{
    narr_state_t state;
    narr_init(&state);

    narr_input_t input;
    memset(&input, 0, sizeof(input));
    /* locale defaults to 0 = EN */
    TEST_ASSERT_EQUAL_INT(0, input.locale);
}

static void test_narr_compose_hebrew_uses_translated_template(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    /* Set thread type to "quiet" and fill headline */
    snprintf(ctx.slots[BR_LANG_SLOT_THREAD_TYPE].value,
             BR_LANG_SLOT_MAX, "quiet");
    ctx.slots[BR_LANG_SLOT_THREAD_TYPE].filled = 1;
    snprintf(ctx.slots[BR_LANG_SLOT_HEADLINE].value,
             BR_LANG_SLOT_MAX, "Test headline");
    ctx.slots[BR_LANG_SLOT_HEADLINE].filled = 1;
    ctx.slot_count = BR_LANG_SLOT_COUNT;

    narr_input_t input;
    memset(&input, 0, sizeof(input));
    input.brain_ctx = &ctx;
    input.depth = NARR_DEPTH_CONTEXT;
    input.locale = 3; /* I18N_LOCALE_HE = 3 (EN=0, ES=1, AR=2, HE=3) */

    narr_output_t out;
    int ok = narr_compose(&state, &input, &out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    /* Hebrew template should NOT contain English "ordinary day" */
    TEST_ASSERT_NULL(strstr(out.body, "ordinary day"));
    /* Body should have content (Hebrew template was used) */
    TEST_ASSERT_TRUE(strlen(out.body) > 0);
}

/* ================================================================
 * Deep templates: CYCLE depth uses longer multi-sentence text
 * ================================================================ */

static void test_narr_compose_cycle_depth_uses_deep_template(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "4 systems converge",
        "convergence",
        "significant",
        "4 systems",
        0.7
    );

    /* CONTEXT depth */
    narr_input_t input_ctx = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL, .wisdom_author = NULL,
        .wisdom_work = NULL, .wisdom_year = 0,
        .depth = NARR_DEPTH_CONTEXT,
        .system_name = "Astronomy"
    };

    narr_output_t out_ctx;
    narr_compose(&state, &input_ctx, &out_ctx);

    /* CYCLE depth — should use deep template (longer) */
    narr_input_t input_cyc = input_ctx;
    input_cyc.depth = NARR_DEPTH_CYCLE;

    narr_output_t out_cyc;
    narr_compose(&state, &input_cyc, &out_cyc);

    /* Deep template text should be longer than context template */
    TEST_ASSERT_TRUE(strlen(out_cyc.body) > strlen(out_ctx.body));
}

static void test_narr_compose_full_depth_uses_deep_template(void)
{
    narr_state_t state;
    narr_init(&state);

    br_lang_context_t ctx = make_context(
        "A quiet moment",
        "quiet",
        "ordinary",
        "0 systems",
        0.1
    );

    narr_input_t input = {
        .brain_ctx = &ctx,
        .wisdom_text = NULL, .wisdom_author = NULL,
        .wisdom_work = NULL, .wisdom_year = 0,
        .depth = NARR_DEPTH_FULL,
        .system_name = NULL
    };

    narr_output_t out;
    int result = narr_compose(&state, &input, &out);

    TEST_ASSERT_EQUAL_INT(1, result);
    /* Deep template should produce multi-sentence text */
    TEST_ASSERT_TRUE(strlen(out.body) > 40);
}

/* ================================================================
 * Runner
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_narr_init_produces_valid_state);
    RUN_TEST(test_narr_compose_convergence_produces_text);
    RUN_TEST(test_narr_compose_quiet_no_wisdom);
    RUN_TEST(test_narr_compose_null_ctx_returns_zero);
    RUN_TEST(test_narr_compose_null_output_returns_zero);
    RUN_TEST(test_narr_compose_surface_is_shorter);
    RUN_TEST(test_narr_compose_festival_thread);
    RUN_TEST(test_narr_compose_wisdom_attribution);
    RUN_TEST(test_narr_compose_wisdom_with_work_and_bce);
    RUN_TEST(test_narr_compose_wisdom_with_ce_year);
    RUN_TEST(test_narr_novelty_tracking);
    RUN_TEST(test_narr_compose_number_thread);
    RUN_TEST(test_narr_compose_cycle_thread);
    RUN_TEST(test_narr_compose_astronomical_thread);
    RUN_TEST(test_narr_depth_name);
    RUN_TEST(test_narr_thread_index);
    RUN_TEST(test_narr_format_attribution_full);
    RUN_TEST(test_narr_format_attribution_no_work);
    RUN_TEST(test_narr_format_attribution_no_year);
    RUN_TEST(test_narr_format_attribution_null_author);
    RUN_TEST(test_narr_compose_with_personal_note);
    RUN_TEST(test_narr_compose_depth_surface_minimal);
    RUN_TEST(test_narr_compose_empty_headline);

    /* Locale */
    RUN_TEST(test_narr_compose_default_locale_is_en);
    RUN_TEST(test_narr_compose_hebrew_uses_translated_template);

    /* Deep templates */
    RUN_TEST(test_narr_compose_cycle_depth_uses_deep_template);
    RUN_TEST(test_narr_compose_full_depth_uses_deep_template);

    return UNITY_END();
}
