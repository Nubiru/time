/* test_wisdom_engine.c -- Wisdom Correlation Engine tests
 * Tests contextual quote selection, scoring algorithm, tag/system mapping,
 * seen-quote exclusion, convergence boost, and edge cases. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/wisdom_engine.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== we_context_default ===== */

void test_context_default_view_is_space(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(WE_VIEW_SPACE, ctx.view);
}

void test_context_default_scale_is_human(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(WE_SCALE_HUMAN, ctx.scale);
}

void test_context_default_no_active_systems(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_UINT(0, ctx.active_systems);
}

void test_context_default_seal_unknown(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(-1, ctx.user_seal);
}

void test_context_default_tone_unknown(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.user_tone);
}

void test_context_default_convergence_zero(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(0, ctx.convergence_score);
}

/* ===== we_view_name ===== */

void test_view_name_space(void)
{
    TEST_ASSERT_EQUAL_STRING("Space", we_view_name(WE_VIEW_SPACE));
}

void test_view_name_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", we_view_name(WE_VIEW_EARTH));
}

void test_view_name_tzolkin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", we_view_name(WE_VIEW_TZOLKIN));
}

void test_view_name_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", we_view_name(WE_VIEW_ICHING));
}

void test_view_name_calendar(void)
{
    TEST_ASSERT_EQUAL_STRING("Calendar", we_view_name(WE_VIEW_CALENDAR));
}

void test_view_name_deep_time(void)
{
    TEST_ASSERT_EQUAL_STRING("Deep Time", we_view_name(WE_VIEW_DEEP_TIME));
}

void test_view_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", we_view_name(WE_VIEW_COUNT));
}

void test_view_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", we_view_name((we_view_t)-1));
}

/* ===== we_scale_name ===== */

void test_scale_name_human(void)
{
    TEST_ASSERT_EQUAL_STRING("Human", we_scale_name(WE_SCALE_HUMAN));
}

void test_scale_name_historical(void)
{
    TEST_ASSERT_EQUAL_STRING("Historical", we_scale_name(WE_SCALE_HISTORICAL));
}

void test_scale_name_geological(void)
{
    TEST_ASSERT_EQUAL_STRING("Geological", we_scale_name(WE_SCALE_GEOLOGICAL));
}

void test_scale_name_cosmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Cosmic", we_scale_name(WE_SCALE_COSMIC));
}

void test_scale_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", we_scale_name(WE_SCALE_COUNT));
}

/* ===== we_tag_to_systems ===== */

void test_tag_astronomy_maps_to_astronomy(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_ASTRONOMY);
    TEST_ASSERT_TRUE(flags & WE_SYS_ASTRONOMY);
}

void test_tag_calendar_maps_to_gregorian(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_CALENDAR);
    TEST_ASSERT_TRUE(flags & WE_SYS_GREGORIAN);
}

void test_tag_cosmos_maps_to_astronomy(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_COSMOS);
    TEST_ASSERT_TRUE(flags & WE_SYS_ASTRONOMY);
}

void test_tag_earth_maps_to_earth(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_EARTH);
    TEST_ASSERT_TRUE(flags & WE_SYS_EARTH);
}

void test_tag_nature_maps_to_earth(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_NATURE);
    TEST_ASSERT_TRUE(flags & WE_SYS_EARTH);
}

void test_tag_harmony_maps_to_human_design(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_HARMONY);
    TEST_ASSERT_TRUE(flags & WE_SYS_HUMAN_DESIGN);
}

void test_tag_unity_maps_to_kabbalah(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_UNITY);
    TEST_ASSERT_TRUE(flags & WE_SYS_KABBALAH);
}

void test_tag_cycles_maps_to_tzolkin(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_CYCLES);
    TEST_ASSERT_TRUE(flags & WE_SYS_TZOLKIN);
}

void test_tag_time_maps_to_gregorian(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_TIME);
    TEST_ASSERT_TRUE(flags & WE_SYS_GREGORIAN);
}

void test_tag_invalid_returns_zero(void)
{
    unsigned int flags = we_tag_to_systems(WISDOM_TAG_COUNT);
    TEST_ASSERT_EQUAL_UINT(0, flags);
}

/* ===== we_system_to_tags ===== */

void test_system_astronomy_yields_tags(void)
{
    wisdom_tag_t out[WISDOM_TAG_COUNT];
    int count = we_system_to_tags(WE_SYS_ASTRONOMY, out, WISDOM_TAG_COUNT);
    TEST_ASSERT_TRUE(count > 0);
    /* Astronomy system should yield ASTRONOMY tag */
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (out[i] == WISDOM_TAG_ASTRONOMY) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_system_earth_yields_earth_tag(void)
{
    wisdom_tag_t out[WISDOM_TAG_COUNT];
    int count = we_system_to_tags(WE_SYS_EARTH, out, WISDOM_TAG_COUNT);
    TEST_ASSERT_TRUE(count > 0);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (out[i] == WISDOM_TAG_EARTH) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_system_tzolkin_yields_cycles_tag(void)
{
    wisdom_tag_t out[WISDOM_TAG_COUNT];
    int count = we_system_to_tags(WE_SYS_TZOLKIN, out, WISDOM_TAG_COUNT);
    TEST_ASSERT_TRUE(count > 0);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (out[i] == WISDOM_TAG_CYCLES) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_system_to_tags_respects_max(void)
{
    wisdom_tag_t out[1];
    int count = we_system_to_tags(WE_SYS_ASTRONOMY, out, 1);
    TEST_ASSERT_TRUE(count <= 1);
}

void test_system_to_tags_null_out_returns_zero(void)
{
    int count = we_system_to_tags(WE_SYS_ASTRONOMY, NULL, 5);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== we_score_quote — view-based scoring ===== */

void test_score_space_view_boosts_astronomy_quote(void)
{
    /* Quote 5 (Pythagoras, "geometry in the humming...") has ASTRONOMY tag */
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    int score = we_score_quote(5, &ctx);
    /* Should have at least base(1) + view match(3) = 4 */
    TEST_ASSERT_TRUE(score >= 4);
}

void test_score_earth_view_boosts_earth_quote(void)
{
    /* Quote 51 (Gandhi, "The earth has enough...") has EARTH and NATURE tags */
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_EARTH;
    int score = we_score_quote(51, &ctx);
    TEST_ASSERT_TRUE(score >= 4);
}

void test_score_deep_time_view_boosts_cosmos_quote(void)
{
    /* Quote 91 (Sagan, "We are a way for the cosmos...") has COSMOS tag */
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_DEEP_TIME;
    int score = we_score_quote(91, &ctx);
    TEST_ASSERT_TRUE(score >= 4);
}

void test_score_calendar_view_boosts_calendar_quote(void)
{
    /* Quote 76 (Ecclesiastes, "To everything there is a season...") has CALENDAR tag */
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_CALENDAR;
    int score = we_score_quote(76, &ctx);
    TEST_ASSERT_TRUE(score >= 4);
}

void test_score_tzolkin_view_boosts_cycles_quote(void)
{
    /* Quote 69 (Arguelles, "The thirteen intentions...") has CYCLES and CALENDAR tags */
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_TZOLKIN;
    int score = we_score_quote(69, &ctx);
    TEST_ASSERT_TRUE(score >= 4);
}

void test_score_iching_view_boosts_harmony_quote(void)
{
    /* Quote 4 (Pythagoras, "All is number") has HARMONY tag */
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_ICHING;
    int score = we_score_quote(4, &ctx);
    TEST_ASSERT_TRUE(score >= 4);
}

/* ===== we_score_quote — active system scoring ===== */

void test_score_active_astronomy_boosts_astronomy_quote(void)
{
    we_context_t ctx = we_context_default();
    ctx.active_systems = WE_SYS_ASTRONOMY;
    /* Quote 7 (Heraclitus, "The sun is new each day") has ASTRONOMY tag */
    int score_with = we_score_quote(7, &ctx);

    ctx.active_systems = 0;
    int score_without = we_score_quote(7, &ctx);

    TEST_ASSERT_TRUE(score_with > score_without);
}

void test_score_multiple_active_systems_stack(void)
{
    we_context_t ctx = we_context_default();
    /* Quote 5 (Pythagoras) has HARMONY, MATHEMATICS, COSMOS, ASTRONOMY */
    ctx.active_systems = WE_SYS_ASTRONOMY;
    int score_one = we_score_quote(5, &ctx);

    ctx.active_systems = WE_SYS_ASTRONOMY | WE_SYS_ASTROLOGY;
    int score_two = we_score_quote(5, &ctx);

    /* More matching systems should give equal or higher score */
    TEST_ASSERT_TRUE(score_two >= score_one);
}

/* ===== we_score_quote — culture match (user seal) ===== */

void test_score_seal_mayan_culture_match(void)
{
    /* Seals 0-3 map to "Mayan" */
    we_context_t ctx = we_context_default();
    ctx.user_seal = 2; /* maps to Mayan */
    /* Quote 64 (Arguelles, "Time is not money...") is Mayan culture */
    int score_with = we_score_quote(64, &ctx);

    ctx.user_seal = -1; /* no seal */
    int score_without = we_score_quote(64, &ctx);

    TEST_ASSERT_TRUE(score_with > score_without);
}

void test_score_seal_eastern_culture_match(void)
{
    /* Seals 4-7 map to "Eastern" */
    we_context_t ctx = we_context_default();
    ctx.user_seal = 5; /* maps to Eastern */
    /* Quote 16 (Lao Tzu) is Eastern culture */
    int score_with = we_score_quote(16, &ctx);

    ctx.user_seal = -1;
    int score_without = we_score_quote(16, &ctx);

    TEST_ASSERT_TRUE(score_with > score_without);
}

void test_score_seal_greek_culture_match(void)
{
    /* Seals 8-11 map to "Greek" */
    we_context_t ctx = we_context_default();
    ctx.user_seal = 9; /* maps to Greek */
    /* Quote 0 (Plato) is Greek culture */
    int score_with = we_score_quote(0, &ctx);

    ctx.user_seal = -1;
    int score_without = we_score_quote(0, &ctx);

    TEST_ASSERT_TRUE(score_with > score_without);
}

void test_score_seal_indian_culture_match(void)
{
    /* Seals 12-15 map to "Indian" */
    we_context_t ctx = we_context_default();
    ctx.user_seal = 13; /* maps to Indian */
    /* Quote 48 (Upanishads) is Indian culture */
    int score_with = we_score_quote(48, &ctx);

    ctx.user_seal = -1;
    int score_without = we_score_quote(48, &ctx);

    TEST_ASSERT_TRUE(score_with > score_without);
}

void test_score_seal_islamic_culture_match(void)
{
    /* Seals 16-19 map to "Islamic" */
    we_context_t ctx = we_context_default();
    ctx.user_seal = 17; /* maps to Islamic */
    /* Quote 32 (Rumi) is Islamic culture */
    int score_with = we_score_quote(32, &ctx);

    ctx.user_seal = -1;
    int score_without = we_score_quote(32, &ctx);

    TEST_ASSERT_TRUE(score_with > score_without);
}

/* ===== we_score_quote — convergence boost ===== */

void test_score_convergence_boosts_unity_quote(void)
{
    we_context_t ctx = we_context_default();
    /* Quote 4 (Pythagoras, "All is number") has UNITY tag */
    ctx.convergence_score = 5;
    int score_conv = we_score_quote(4, &ctx);

    ctx.convergence_score = 0;
    int score_no_conv = we_score_quote(4, &ctx);

    TEST_ASSERT_EQUAL_INT(5, score_conv - score_no_conv);
}

void test_score_convergence_boosts_harmony_quote(void)
{
    we_context_t ctx = we_context_default();
    /* Quote 2 (Aristotle, "Nature does nothing in vain") has HARMONY tag */
    ctx.convergence_score = 3;
    int score_conv = we_score_quote(2, &ctx);

    ctx.convergence_score = 0;
    int score_no_conv = we_score_quote(2, &ctx);

    TEST_ASSERT_EQUAL_INT(3, score_conv - score_no_conv);
}

void test_score_convergence_does_not_boost_unrelated_quote(void)
{
    we_context_t ctx = we_context_default();
    /* Quote 109 ("The trouble is, you think you have time.") has TIME, CONSCIOUSNESS, POWER
     * -- no UNITY or HARMONY */
    ctx.convergence_score = 5;
    int score_conv = we_score_quote(109, &ctx);

    ctx.convergence_score = 0;
    int score_no_conv = we_score_quote(109, &ctx);

    TEST_ASSERT_EQUAL_INT(0, score_conv - score_no_conv);
}

/* ===== we_score_quote — base score ===== */

void test_score_base_always_at_least_one(void)
{
    we_context_t ctx = we_context_default();
    /* Every quote should have at least base score of 1 */
    int total = wisdom_quote_count();
    for (int i = 0; i < total; i++) {
        int score = we_score_quote(i, &ctx);
        TEST_ASSERT_TRUE(score >= 1);
    }
}

/* ===== we_score_quote — edge cases ===== */

void test_score_invalid_index_returns_zero(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(0, we_score_quote(-1, &ctx));
}

void test_score_out_of_range_returns_zero(void)
{
    we_context_t ctx = we_context_default();
    TEST_ASSERT_EQUAL_INT(0, we_score_quote(9999, &ctx));
}

void test_score_null_context_returns_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, we_score_quote(0, NULL));
}

/* ===== we_select — ranking ===== */

void test_select_returns_nonzero_count(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    we_selection_t sel = we_select(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
}

void test_select_results_sorted_descending(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_EARTH;
    ctx.active_systems = WE_SYS_EARTH;
    we_selection_t sel = we_select(&ctx, NULL, 0);
    for (int i = 1; i < sel.count; i++) {
        TEST_ASSERT_TRUE(sel.results[i - 1].score >= sel.results[i].score);
    }
}

void test_select_max_results_capped(void)
{
    we_context_t ctx = we_context_default();
    we_selection_t sel = we_select(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count <= WE_MAX_RESULTS);
}

void test_select_quote_indices_valid(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    we_selection_t sel = we_select(&ctx, NULL, 0);
    int total = wisdom_quote_count();
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_TRUE(sel.results[i].quote_index >= 0);
        TEST_ASSERT_TRUE(sel.results[i].quote_index < total);
    }
}

/* ===== we_select — seen-quote exclusion ===== */

void test_select_excludes_seen_quotes(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;

    /* First selection */
    we_selection_t sel1 = we_select(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel1.count > 0);

    /* Build seen array from first result */
    int seen[WE_MAX_RESULTS];
    for (int i = 0; i < sel1.count; i++) {
        seen[i] = sel1.results[i].quote_index;
    }

    /* Second selection excludes seen */
    we_selection_t sel2 = we_select(&ctx, seen, sel1.count);

    /* None of sel2's results should be in the seen list */
    for (int i = 0; i < sel2.count; i++) {
        for (int j = 0; j < sel1.count; j++) {
            TEST_ASSERT_NOT_EQUAL(sel2.results[i].quote_index, seen[j]);
        }
    }
}

void test_select_seen_null_with_count_treated_as_none(void)
{
    we_context_t ctx = we_context_default();
    /* NULL seen with non-zero count should be treated as no exclusion */
    we_selection_t sel = we_select(&ctx, NULL, 5);
    TEST_ASSERT_TRUE(sel.count > 0);
}

void test_select_all_seen_returns_zero(void)
{
    we_context_t ctx = we_context_default();
    /* Mark all quotes as seen */
    int total = wisdom_quote_count();
    int seen[512]; /* enough for all quotes */
    for (int i = 0; i < total && i < 512; i++) {
        seen[i] = i;
    }
    we_selection_t sel = we_select(&ctx, seen, total < 512 ? total : 512);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
}

/* ===== we_best_quote ===== */

void test_best_quote_returns_valid_index(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    int best = we_best_quote(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(best >= 0);
    TEST_ASSERT_TRUE(best < wisdom_quote_count());
}

void test_best_quote_matches_first_in_select(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_EARTH;
    ctx.active_systems = WE_SYS_EARTH;
    int best = we_best_quote(&ctx, NULL, 0);
    we_selection_t sel = we_select(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);
    TEST_ASSERT_EQUAL_INT(sel.results[0].quote_index, best);
}

void test_best_quote_excludes_seen(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    int first = we_best_quote(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(first >= 0);

    int seen[] = { first };
    int second = we_best_quote(&ctx, seen, 1);
    TEST_ASSERT_NOT_EQUAL(first, second);
}

void test_best_quote_all_seen_returns_negative(void)
{
    we_context_t ctx = we_context_default();
    int total = wisdom_quote_count();
    int seen[512];
    for (int i = 0; i < total && i < 512; i++) {
        seen[i] = i;
    }
    int best = we_best_quote(&ctx, seen, total < 512 ? total : 512);
    TEST_ASSERT_EQUAL_INT(-1, best);
}

void test_best_quote_null_context_returns_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-1, we_best_quote(NULL, NULL, 0));
}

/* ===== we_select — null context ===== */

void test_select_null_context_returns_empty(void)
{
    we_selection_t sel = we_select(NULL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
}

/* ===== scale scoring ===== */

void test_score_cosmic_scale_boosts_cosmos_quote(void)
{
    we_context_t ctx = we_context_default();
    ctx.scale = WE_SCALE_COSMIC;
    /* Quote 91 (Sagan, "We are a way for the cosmos...") has COSMOS tag */
    int score_cosmic = we_score_quote(91, &ctx);

    ctx.scale = WE_SCALE_HUMAN;
    int score_human = we_score_quote(91, &ctx);

    TEST_ASSERT_TRUE(score_cosmic > score_human);
}

void test_score_geological_scale_boosts_earth_quote(void)
{
    we_context_t ctx = we_context_default();
    ctx.scale = WE_SCALE_GEOLOGICAL;
    /* Quote 51 (Gandhi, "The earth has enough...") has EARTH tag */
    int score_geo = we_score_quote(51, &ctx);

    ctx.scale = WE_SCALE_HUMAN;
    int score_human = we_score_quote(51, &ctx);

    TEST_ASSERT_TRUE(score_geo > score_human);
}

void test_score_historical_scale_boosts_time_quote(void)
{
    we_context_t ctx = we_context_default();
    ctx.scale = WE_SCALE_HISTORICAL;
    /* Quote 0 (Plato, "Time is a moving image of eternity") has TIME tag */
    int score_hist = we_score_quote(0, &ctx);

    ctx.scale = WE_SCALE_HUMAN;
    int score_human = we_score_quote(0, &ctx);

    TEST_ASSERT_TRUE(score_hist >= score_human);
}

/* ===== determinism ===== */

void test_scoring_is_deterministic(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    ctx.active_systems = WE_SYS_ASTRONOMY | WE_SYS_ASTROLOGY;
    ctx.user_seal = 5;
    ctx.convergence_score = 3;

    we_selection_t sel1 = we_select(&ctx, NULL, 0);
    we_selection_t sel2 = we_select(&ctx, NULL, 0);

    TEST_ASSERT_EQUAL_INT(sel1.count, sel2.count);
    for (int i = 0; i < sel1.count; i++) {
        TEST_ASSERT_EQUAL_INT(sel1.results[i].quote_index,
                              sel2.results[i].quote_index);
        TEST_ASSERT_EQUAL_INT(sel1.results[i].score,
                              sel2.results[i].score);
    }
}

/* ===== combined scoring ===== */

void test_score_combined_view_and_system_and_seal(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    ctx.active_systems = WE_SYS_ASTRONOMY;
    ctx.user_seal = 9; /* Greek */
    /* Quote 5 (Pythagoras) is Greek, has ASTRONOMY, COSMOS tags
     * Expected: base(1) + view(3, ASTRONOMY/COSMOS match) + system(1, astronomy)
     *           + culture(2, Greek) = 7+ */
    int score = we_score_quote(5, &ctx);
    TEST_ASSERT_TRUE(score >= 7);
}

/* ===== we_select space view priorities ===== */

void test_select_space_view_prefers_astronomy_quotes(void)
{
    we_context_t ctx = we_context_default();
    ctx.view = WE_VIEW_SPACE;
    we_selection_t sel = we_select(&ctx, NULL, 0);
    TEST_ASSERT_TRUE(sel.count > 0);

    /* Top result should have ASTRONOMY or COSMOS tag */
    wisdom_t q = wisdom_quote_get(sel.results[0].quote_index);
    int has_astro_or_cosmos = 0;
    for (int j = 0; j < q.tag_count; j++) {
        if (q.tags[j] == WISDOM_TAG_ASTRONOMY || q.tags[j] == WISDOM_TAG_COSMOS) {
            has_astro_or_cosmos = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(has_astro_or_cosmos);
}

int main(void)
{
    UNITY_BEGIN();

    /* we_context_default */
    RUN_TEST(test_context_default_view_is_space);
    RUN_TEST(test_context_default_scale_is_human);
    RUN_TEST(test_context_default_no_active_systems);
    RUN_TEST(test_context_default_seal_unknown);
    RUN_TEST(test_context_default_tone_unknown);
    RUN_TEST(test_context_default_convergence_zero);

    /* we_view_name */
    RUN_TEST(test_view_name_space);
    RUN_TEST(test_view_name_earth);
    RUN_TEST(test_view_name_tzolkin);
    RUN_TEST(test_view_name_iching);
    RUN_TEST(test_view_name_calendar);
    RUN_TEST(test_view_name_deep_time);
    RUN_TEST(test_view_name_invalid);
    RUN_TEST(test_view_name_negative);

    /* we_scale_name */
    RUN_TEST(test_scale_name_human);
    RUN_TEST(test_scale_name_historical);
    RUN_TEST(test_scale_name_geological);
    RUN_TEST(test_scale_name_cosmic);
    RUN_TEST(test_scale_name_invalid);

    /* we_tag_to_systems */
    RUN_TEST(test_tag_astronomy_maps_to_astronomy);
    RUN_TEST(test_tag_calendar_maps_to_gregorian);
    RUN_TEST(test_tag_cosmos_maps_to_astronomy);
    RUN_TEST(test_tag_earth_maps_to_earth);
    RUN_TEST(test_tag_nature_maps_to_earth);
    RUN_TEST(test_tag_harmony_maps_to_human_design);
    RUN_TEST(test_tag_unity_maps_to_kabbalah);
    RUN_TEST(test_tag_cycles_maps_to_tzolkin);
    RUN_TEST(test_tag_time_maps_to_gregorian);
    RUN_TEST(test_tag_invalid_returns_zero);

    /* we_system_to_tags */
    RUN_TEST(test_system_astronomy_yields_tags);
    RUN_TEST(test_system_earth_yields_earth_tag);
    RUN_TEST(test_system_tzolkin_yields_cycles_tag);
    RUN_TEST(test_system_to_tags_respects_max);
    RUN_TEST(test_system_to_tags_null_out_returns_zero);

    /* we_score_quote — view */
    RUN_TEST(test_score_space_view_boosts_astronomy_quote);
    RUN_TEST(test_score_earth_view_boosts_earth_quote);
    RUN_TEST(test_score_deep_time_view_boosts_cosmos_quote);
    RUN_TEST(test_score_calendar_view_boosts_calendar_quote);
    RUN_TEST(test_score_tzolkin_view_boosts_cycles_quote);
    RUN_TEST(test_score_iching_view_boosts_harmony_quote);

    /* we_score_quote — active systems */
    RUN_TEST(test_score_active_astronomy_boosts_astronomy_quote);
    RUN_TEST(test_score_multiple_active_systems_stack);

    /* we_score_quote — culture match (seal) */
    RUN_TEST(test_score_seal_mayan_culture_match);
    RUN_TEST(test_score_seal_eastern_culture_match);
    RUN_TEST(test_score_seal_greek_culture_match);
    RUN_TEST(test_score_seal_indian_culture_match);
    RUN_TEST(test_score_seal_islamic_culture_match);

    /* we_score_quote — convergence */
    RUN_TEST(test_score_convergence_boosts_unity_quote);
    RUN_TEST(test_score_convergence_boosts_harmony_quote);
    RUN_TEST(test_score_convergence_does_not_boost_unrelated_quote);

    /* we_score_quote — base and edge cases */
    RUN_TEST(test_score_base_always_at_least_one);
    RUN_TEST(test_score_invalid_index_returns_zero);
    RUN_TEST(test_score_out_of_range_returns_zero);
    RUN_TEST(test_score_null_context_returns_zero);

    /* we_select — ranking */
    RUN_TEST(test_select_returns_nonzero_count);
    RUN_TEST(test_select_results_sorted_descending);
    RUN_TEST(test_select_max_results_capped);
    RUN_TEST(test_select_quote_indices_valid);

    /* we_select — seen exclusion */
    RUN_TEST(test_select_excludes_seen_quotes);
    RUN_TEST(test_select_seen_null_with_count_treated_as_none);
    RUN_TEST(test_select_all_seen_returns_zero);

    /* we_best_quote */
    RUN_TEST(test_best_quote_returns_valid_index);
    RUN_TEST(test_best_quote_matches_first_in_select);
    RUN_TEST(test_best_quote_excludes_seen);
    RUN_TEST(test_best_quote_all_seen_returns_negative);
    RUN_TEST(test_best_quote_null_context_returns_negative);

    /* we_select — edge cases */
    RUN_TEST(test_select_null_context_returns_empty);

    /* scale scoring */
    RUN_TEST(test_score_cosmic_scale_boosts_cosmos_quote);
    RUN_TEST(test_score_geological_scale_boosts_earth_quote);
    RUN_TEST(test_score_historical_scale_boosts_time_quote);

    /* determinism */
    RUN_TEST(test_scoring_is_deterministic);

    /* combined scoring */
    RUN_TEST(test_score_combined_view_and_system_and_seal);

    /* select preferences */
    RUN_TEST(test_select_space_view_prefers_astronomy_quotes);

    return UNITY_END();
}
