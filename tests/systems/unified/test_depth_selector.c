/* test_depth_selector.c — Tests for depth selector module
 * Ranks knowledge systems by visibility tier based on significance,
 * convergence, birth profile, and user toggles.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/depth_selector.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== Helper: build a minimal ts_summary_t ========== */

static ts_summary_t make_empty_summary(void)
{
    ts_summary_t s;
    memset(&s, 0, sizeof(s));
    s.jd = 2460000.5;
    return s;
}

/* Add an active entry to summary. Returns pointer to the entry. */
static ts_entry_t *add_entry(ts_summary_t *s, ts_system_t sys, int significance)
{
    ts_entry_t *e = &s->entries[s->entry_count];
    memset(e, 0, sizeof(*e));
    e->system = sys;
    e->significance = significance;
    e->active = 1;
    s->entry_count++;
    return e;
}

/* ========== ds_visibility_count ========== */

static void test_visibility_count_returns_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, ds_visibility_count());
}

/* ========== ds_visibility_name ========== */

static void test_name_hidden(void)
{
    TEST_ASSERT_EQUAL_STRING("Hidden", ds_visibility_name(DS_VIS_HIDDEN));
}

static void test_name_dormant(void)
{
    TEST_ASSERT_EQUAL_STRING("Dormant", ds_visibility_name(DS_VIS_DORMANT));
}

static void test_name_visible(void)
{
    TEST_ASSERT_EQUAL_STRING("Visible", ds_visibility_name(DS_VIS_VISIBLE));
}

static void test_name_prominent(void)
{
    TEST_ASSERT_EQUAL_STRING("Prominent", ds_visibility_name(DS_VIS_PROMINENT));
}

static void test_name_highlighted(void)
{
    TEST_ASSERT_EQUAL_STRING("Highlighted", ds_visibility_name(DS_VIS_HIGHLIGHTED));
}

static void test_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ds_visibility_name((ds_visibility_t)99));
}

static void test_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", ds_visibility_name((ds_visibility_t)-1));
}

/* ========== ds_select — NULL input ========== */

static void test_null_today_returns_empty(void)
{
    ds_selection_t sel = ds_select(NULL, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
    TEST_ASSERT_EQUAL_INT(0, sel.highlighted_count);
    TEST_ASSERT_EQUAL_INT(0, sel.prominent_count);
}

/* ========== ds_select — single system, default behavior ========== */

static void test_single_system_starts_dormant(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 0); /* significance 0 */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_DORMANT, sel.entries[0].vis);
}

/* ========== Gregorian always at least VISIBLE ========== */

static void test_gregorian_always_visible(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_GREGORIAN, 0); /* significance 0 */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_VISIBLE, sel.entries[0].vis);
}

static void test_gregorian_visible_even_with_low_significance(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_GREGORIAN, 1); /* significance 1, still not >=2 */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_TRUE(sel.entries[0].vis >= DS_VIS_VISIBLE);
}

/* ========== Significance thresholds ========== */

static void test_significance_2_promotes_to_prominent(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 2);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_PROMINENT, sel.entries[0].vis);
}

static void test_significance_3_promotes_to_highlighted(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_CHINESE, 3);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

static void test_significance_1_stays_dormant(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_ICHING, 1); /* below threshold for PROMINENT */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_DORMANT, sel.entries[0].vis);
}

/* ========== Birth profile promotion ========== */

static void test_birth_system_promoted_to_visible(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_HUMAN_DESIGN, 0); /* starts DORMANT */

    ds_birth_t birth = {0};
    birth.birth_systems[0] = TS_SYS_HUMAN_DESIGN;
    birth.birth_count = 1;

    ds_selection_t sel = ds_select(&s, NULL, &birth, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_TRUE(sel.entries[0].vis >= DS_VIS_VISIBLE);
}

static void test_birth_does_not_demote_higher_vis(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 3); /* HIGHLIGHTED from significance */

    ds_birth_t birth = {0};
    birth.birth_systems[0] = TS_SYS_TZOLKIN;
    birth.birth_count = 1;

    ds_selection_t sel = ds_select(&s, NULL, &birth, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

static void test_null_birth_skips_birth_promotion(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_ICHING, 0);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_DORMANT, sel.entries[0].vis);
}

/* ========== Convergence promotion ========== */

static void test_convergence_promotes_by_one_tier(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_HEBREW, 0); /* DORMANT */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_HEBREW;
    conv.converging_count = 1;
    conv.convergence_strength = 1; /* minor */

    ds_selection_t sel = ds_select(&s, &conv, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_VISIBLE, sel.entries[0].vis);
}

static void test_convergence_stacks_with_significance(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_BUDDHIST, 2); /* PROMINENT from significance */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_BUDDHIST;
    conv.converging_count = 1;
    conv.convergence_strength = 1; /* minor */

    ds_selection_t sel = ds_select(&s, &conv, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    /* PROMINENT + 1 = HIGHLIGHTED */
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

static void test_strong_convergence_forces_highlighted(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_PERSIAN, 0); /* DORMANT */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_PERSIAN;
    conv.converging_count = 1;
    conv.convergence_strength = 3; /* major */

    ds_selection_t sel = ds_select(&s, &conv, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

static void test_convergence_does_not_exceed_highlighted(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_CHINESE, 3); /* HIGHLIGHTED from significance */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_CHINESE;
    conv.converging_count = 1;
    conv.convergence_strength = 4; /* rare */

    ds_selection_t sel = ds_select(&s, &conv, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    /* Should clamp at HIGHLIGHTED */
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

static void test_null_convergence_skips_convergence(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_HEBREW, 0);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_DORMANT, sel.entries[0].vis);
}

/* ========== User toggles ========== */

static void test_toggle_all_on_means_no_filtering(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 2);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_PROMINENT, sel.entries[0].vis);
}

static void test_toggle_off_demotes_to_hidden(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 2); /* Would be PROMINENT */

    /* All bits clear => everything hidden */
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0x00000000);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIDDEN, sel.entries[0].vis);
}

static void test_toggle_specific_bit_hides_system(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 2); /* PROMINENT */
    add_entry(&s, TS_SYS_CHINESE, 2); /* PROMINENT */

    /* Clear bit for TS_SYS_TZOLKIN (=1), keep TS_SYS_CHINESE (=3) */
    unsigned int toggles = 0xFFFFFFFF & ~(1u << TS_SYS_TZOLKIN);

    ds_selection_t sel = ds_select(&s, NULL, NULL, toggles);
    TEST_ASSERT_EQUAL_INT(2, sel.count);

    /* Find each system in results */
    int tzolkin_vis = -1;
    int chinese_vis = -1;
    for (int i = 0; i < sel.count; i++) {
        if (sel.entries[i].system_id == TS_SYS_TZOLKIN)
            tzolkin_vis = sel.entries[i].vis;
        if (sel.entries[i].system_id == TS_SYS_CHINESE)
            chinese_vis = sel.entries[i].vis;
    }
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIDDEN, tzolkin_vis);
    TEST_ASSERT_EQUAL_INT(DS_VIS_PROMINENT, chinese_vis);
}

static void test_toggle_promotes_dormant_to_visible(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_ICHING, 0); /* DORMANT */

    /* Bit for TS_SYS_ICHING is set -> promotes DORMANT to VISIBLE */
    unsigned int toggles = (1u << TS_SYS_ICHING);

    ds_selection_t sel = ds_select(&s, NULL, NULL, toggles);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_VISIBLE, sel.entries[0].vis);
}

/* ========== Ranking ========== */

static void test_ranking_highlighted_first(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_ICHING, 0);    /* DORMANT */
    add_entry(&s, TS_SYS_GREGORIAN, 0); /* VISIBLE (anchor) */
    add_entry(&s, TS_SYS_CHINESE, 3);   /* HIGHLIGHTED */
    add_entry(&s, TS_SYS_TZOLKIN, 2);   /* PROMINENT */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(4, sel.count);

    /* Rank 0 should be HIGHLIGHTED */
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
    TEST_ASSERT_EQUAL_INT(TS_SYS_CHINESE, sel.entries[0].system_id);
    TEST_ASSERT_EQUAL_INT(0, sel.entries[0].rank);

    /* Rank 1 should be PROMINENT */
    TEST_ASSERT_EQUAL_INT(DS_VIS_PROMINENT, sel.entries[1].vis);
    TEST_ASSERT_EQUAL_INT(1, sel.entries[1].rank);

    /* Rank 2 should be VISIBLE */
    TEST_ASSERT_EQUAL_INT(DS_VIS_VISIBLE, sel.entries[2].vis);
    TEST_ASSERT_EQUAL_INT(2, sel.entries[2].rank);

    /* Rank 3 should be DORMANT */
    TEST_ASSERT_EQUAL_INT(DS_VIS_DORMANT, sel.entries[3].vis);
    TEST_ASSERT_EQUAL_INT(3, sel.entries[3].rank);
}

static void test_same_tier_sorted_by_significance_desc(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_HEBREW, 2);    /* PROMINENT, sig=2 */
    add_entry(&s, TS_SYS_BUDDHIST, 2);  /* PROMINENT, sig=2 — same sig, first-in stable */
    add_entry(&s, TS_SYS_CHINESE, 3);   /* HIGHLIGHTED, sig=3 */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(3, sel.count);

    /* Rank 0: HIGHLIGHTED Chinese (sig 3) */
    TEST_ASSERT_EQUAL_INT(TS_SYS_CHINESE, sel.entries[0].system_id);
    /* Rank 1, 2: both PROMINENT, same significance -> stable order */
    TEST_ASSERT_EQUAL_INT(DS_VIS_PROMINENT, sel.entries[1].vis);
    TEST_ASSERT_EQUAL_INT(DS_VIS_PROMINENT, sel.entries[2].vis);
}

/* ========== Counts ========== */

static void test_highlighted_count(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_CHINESE, 3);   /* HIGHLIGHTED */
    add_entry(&s, TS_SYS_HEBREW, 3);    /* HIGHLIGHTED */
    add_entry(&s, TS_SYS_TZOLKIN, 2);   /* PROMINENT */
    add_entry(&s, TS_SYS_GREGORIAN, 0); /* VISIBLE */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(2, sel.highlighted_count);
}

static void test_prominent_count(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_CHINESE, 3);   /* HIGHLIGHTED */
    add_entry(&s, TS_SYS_HEBREW, 3);    /* HIGHLIGHTED */
    add_entry(&s, TS_SYS_TZOLKIN, 2);   /* PROMINENT */
    add_entry(&s, TS_SYS_GREGORIAN, 0); /* VISIBLE */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    /* prominent_count = entries with vis >= PROMINENT = 2 HIGHLIGHTED + 1 PROMINENT = 3 */
    TEST_ASSERT_EQUAL_INT(3, sel.prominent_count);
}

/* ========== Score stored for debugging ========== */

static void test_score_reflects_significance(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_TZOLKIN, 2);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    /* Score should be non-negative and reflect significance */
    TEST_ASSERT_TRUE(sel.entries[0].score >= 2);
}

/* ========== Multiple simultaneous promotions ========== */

static void test_birth_plus_convergence_plus_significance(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_HINDU, 2); /* PROMINENT from significance */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_HINDU;
    conv.converging_count = 1;
    conv.convergence_strength = 1; /* minor */

    ds_birth_t birth = {0};
    birth.birth_systems[0] = TS_SYS_HINDU;
    birth.birth_count = 1;

    ds_selection_t sel = ds_select(&s, &conv, &birth, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    /* PROMINENT + convergence +1 = HIGHLIGHTED. Birth: at least VISIBLE (already higher) */
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

/* ========== Empty summary (no active entries) ========== */

static void test_empty_summary_returns_empty_selection(void)
{
    ts_summary_t s = make_empty_summary();
    /* No entries added */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
    TEST_ASSERT_EQUAL_INT(0, sel.highlighted_count);
    TEST_ASSERT_EQUAL_INT(0, sel.prominent_count);
}

/* ========== Inactive entries are skipped ========== */

static void test_inactive_entries_skipped(void)
{
    ts_summary_t s = make_empty_summary();
    ts_entry_t *e = add_entry(&s, TS_SYS_TZOLKIN, 3);
    e->active = 0; /* mark inactive */

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(0, sel.count);
}

/* ========== Many systems ========== */

static void test_many_systems_all_ranked(void)
{
    ts_summary_t s = make_empty_summary();
    /* Add 5 systems with varying significance */
    add_entry(&s, TS_SYS_GREGORIAN, 0);
    add_entry(&s, TS_SYS_TZOLKIN, 1);
    add_entry(&s, TS_SYS_CHINESE, 2);
    add_entry(&s, TS_SYS_HEBREW, 3);
    add_entry(&s, TS_SYS_ISLAMIC, 0);

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(5, sel.count);

    /* Verify all ranks are unique 0..4 */
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_EQUAL_INT(i, sel.entries[i].rank);
    }
}

/* ========== Strong convergence with strength 4 (rare) ========== */

static void test_rare_convergence_highlights(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_COPTIC, 0); /* DORMANT */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_COPTIC;
    conv.converging_count = 1;
    conv.convergence_strength = 4; /* rare */

    ds_selection_t sel = ds_select(&s, &conv, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_HIGHLIGHTED, sel.entries[0].vis);
}

/* ========== Toggle on DORMANT system (bit set, vis=DORMANT) ========== */

static void test_toggle_on_dormant_promotes_to_visible_not_higher(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_KOREAN, 0); /* DORMANT */

    /* Only TS_SYS_KOREAN bit set */
    unsigned int toggles = (1u << TS_SYS_KOREAN);

    ds_selection_t sel = ds_select(&s, NULL, NULL, toggles);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    TEST_ASSERT_EQUAL_INT(DS_VIS_VISIBLE, sel.entries[0].vis);
}

/* ========== Convergence with strength 2 (notable) — not enough for auto-highlight ========== */

static void test_notable_convergence_promotes_by_one_only(void)
{
    ts_summary_t s = make_empty_summary();
    add_entry(&s, TS_SYS_ETHIOPIAN, 0); /* DORMANT */

    ds_convergence_t conv = {0};
    conv.converging_systems[0] = TS_SYS_ETHIOPIAN;
    conv.converging_count = 1;
    conv.convergence_strength = 2; /* notable, but < 3 */

    ds_selection_t sel = ds_select(&s, &conv, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(1, sel.count);
    /* DORMANT + 1 = VISIBLE (not HIGHLIGHTED since strength < 3) */
    TEST_ASSERT_EQUAL_INT(DS_VIS_VISIBLE, sel.entries[0].vis);
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* visibility helpers */
    RUN_TEST(test_visibility_count_returns_5);
    RUN_TEST(test_name_hidden);
    RUN_TEST(test_name_dormant);
    RUN_TEST(test_name_visible);
    RUN_TEST(test_name_prominent);
    RUN_TEST(test_name_highlighted);
    RUN_TEST(test_name_invalid);
    RUN_TEST(test_name_negative);

    /* NULL input */
    RUN_TEST(test_null_today_returns_empty);

    /* single system defaults */
    RUN_TEST(test_single_system_starts_dormant);
    RUN_TEST(test_gregorian_always_visible);
    RUN_TEST(test_gregorian_visible_even_with_low_significance);

    /* significance thresholds */
    RUN_TEST(test_significance_2_promotes_to_prominent);
    RUN_TEST(test_significance_3_promotes_to_highlighted);
    RUN_TEST(test_significance_1_stays_dormant);

    /* birth profile */
    RUN_TEST(test_birth_system_promoted_to_visible);
    RUN_TEST(test_birth_does_not_demote_higher_vis);
    RUN_TEST(test_null_birth_skips_birth_promotion);

    /* convergence */
    RUN_TEST(test_convergence_promotes_by_one_tier);
    RUN_TEST(test_convergence_stacks_with_significance);
    RUN_TEST(test_strong_convergence_forces_highlighted);
    RUN_TEST(test_convergence_does_not_exceed_highlighted);
    RUN_TEST(test_null_convergence_skips_convergence);
    RUN_TEST(test_notable_convergence_promotes_by_one_only);
    RUN_TEST(test_rare_convergence_highlights);

    /* user toggles */
    RUN_TEST(test_toggle_all_on_means_no_filtering);
    RUN_TEST(test_toggle_off_demotes_to_hidden);
    RUN_TEST(test_toggle_specific_bit_hides_system);
    RUN_TEST(test_toggle_promotes_dormant_to_visible);
    RUN_TEST(test_toggle_on_dormant_promotes_to_visible_not_higher);

    /* ranking */
    RUN_TEST(test_ranking_highlighted_first);
    RUN_TEST(test_same_tier_sorted_by_significance_desc);

    /* counts */
    RUN_TEST(test_highlighted_count);
    RUN_TEST(test_prominent_count);

    /* score */
    RUN_TEST(test_score_reflects_significance);

    /* combined promotions */
    RUN_TEST(test_birth_plus_convergence_plus_significance);

    /* edge cases */
    RUN_TEST(test_empty_summary_returns_empty_selection);
    RUN_TEST(test_inactive_entries_skipped);
    RUN_TEST(test_many_systems_all_ranked);

    return UNITY_END();
}
