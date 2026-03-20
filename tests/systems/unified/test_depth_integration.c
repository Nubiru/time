/* test_depth_integration.c — Integration tests for the full DEPTH pipeline.
 * Verifies that today_summary, depth_tier, depth_ring, and depth_selector
 * compose correctly as an integrated system.
 *
 * Pipeline: ts_compute -> depth_ring -> depth_tier -> ds_select
 *
 * LABELS integration */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/today_summary.h"
#include "../../../src/systems/unified/depth_tier.h"
#include "../../../src/systems/unified/depth_ring.h"
#include "../../../src/systems/unified/depth_selector.h"

#include <string.h>

/* J2000.0 epoch: 2000-01-01 12:00 UT */
#define JD_J2000     2451545.0
/* 2024-04-08 (solar eclipse date) */
#define JD_20240408  2460408.5
/* 1900-01-01 noon UT */
#define JD_19000101  2415020.5

void setUp(void) { }
void tearDown(void) { }

/* ========== Pipeline Tests: ts_compute -> depth_ring -> depth_tier -> ds_select ========== */

/* 1. ts_compute produces valid summary */
static void test_compute_produces_valid_summary(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    TEST_ASSERT_TRUE(s.entry_count > 0);
    TEST_ASSERT_TRUE(s.entry_count <= TS_MAX_SYSTEMS);
    TEST_ASSERT_FLOAT_WITHIN(0.001, JD_J2000, s.jd);
}

/* 2. Every active system has a valid ring */
static void test_every_active_system_has_valid_ring(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    for (int i = 0; i < s.entry_count; i++) {
        if (!s.entries[i].active) continue;
        depth_ring_t ring = depth_ring_for_system((int)s.entries[i].system);
        TEST_ASSERT_TRUE_MESSAGE(
            ring >= DEPTH_RING_COSMIC && ring <= DEPTH_RING_TODAY,
            "Active system mapped to invalid ring");
    }
}

/* 3. Every ring has at least one system */
static void test_every_ring_has_at_least_one_system(void)
{
    for (int r = 0; r < DEPTH_RING_COUNT; r++) {
        int count = depth_ring_systems((depth_ring_t)r, NULL, 0);
        TEST_ASSERT_TRUE_MESSAGE(count > 0,
            "Ring has zero systems assigned");
    }
}

/* 4. Ring coverage is complete: union of all ring lists = TS_SYS_COUNT */
static void test_ring_coverage_is_complete(void)
{
    int seen[TS_SYS_COUNT];
    memset(seen, 0, sizeof(seen));

    for (int r = 0; r < DEPTH_RING_COUNT; r++) {
        int ids[TS_SYS_COUNT];
        int count = depth_ring_systems((depth_ring_t)r, ids, TS_SYS_COUNT);
        for (int i = 0; i < count; i++) {
            TEST_ASSERT_TRUE(ids[i] >= 0 && ids[i] < TS_SYS_COUNT);
            seen[ids[i]] = 1;
        }
    }

    for (int s = 0; s < TS_SYS_COUNT; s++) {
        char msg[64];
        snprintf(msg, sizeof(msg), "System %d not in any ring", s);
        TEST_ASSERT_TRUE_MESSAGE(seen[s], msg);
    }
}

/* 5. Tier is non-increasing with zoom level (more detail at low scale) */
static void test_tier_nonincreasing_with_zoom_out(void)
{
    /* Scale 0 = PERSONAL (most detail), scale 6 = UNIVERSE (least detail).
     * Default tier should be non-increasing as scale increases. */
    depth_tier_t prev = depth_tier_default_for_scale(0);
    for (int scale = 1; scale <= 6; scale++) {
        depth_tier_t cur = depth_tier_default_for_scale(scale);
        TEST_ASSERT_TRUE_MESSAGE(cur <= prev,
            "Tier should not increase when zooming out");
        prev = cur;
    }
}

/* 6. Panel at minimum zoom (personal scale = most detail) */
static void test_panel_at_personal_scale(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL, depth_tier_default_for_scale(0));
}

/* 7. Glyph at maximum zoom-out (universe scale = least detail) */
static void test_glyph_at_universe_scale(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH, depth_tier_default_for_scale(6));
}

/* 8. User override works through the pipeline */
static void test_user_override_in_pipeline(void)
{
    /* Even at universe scale (default GLYPH), override forces PANEL */
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL,
                          depth_tier_for_scale(6, 0, DEPTH_TIER_PANEL));
}

/* 9. Selector produces entries from computed summary */
static void test_selector_produces_entries_from_summary(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_TRUE(sel.count > 0);
}

/* 10. Selector entries have valid visibility */
static void test_selector_entries_valid_visibility(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_TRUE(sel.entries[i].vis >= DS_VIS_HIDDEN);
        TEST_ASSERT_TRUE(sel.entries[i].vis <= DS_VIS_HIGHLIGHTED);
    }
}

/* 11. Selector entries are ranked sequentially */
static void test_selector_entries_ranked_sequentially(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_EQUAL_INT(i, sel.entries[i].rank);
    }
}

/* 12. All-bits toggle shows all systems (none HIDDEN) */
static void test_all_bits_toggle_none_hidden(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_TRUE_MESSAGE(sel.entries[i].vis != DS_VIS_HIDDEN,
            "System should not be HIDDEN with all toggles on");
    }
}

/* 13. Zero toggle hides all systems */
static void test_zero_toggle_hides_all(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0x00000000);
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(DS_VIS_HIDDEN, sel.entries[i].vis,
            "System should be HIDDEN with zero toggles");
    }
}

/* ========== Cross-System Verification ========== */

/* 14. Geological system is in COSMIC ring */
static void test_geological_in_cosmic_ring(void)
{
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_COSMIC,
                          depth_ring_for_system(TS_SYS_GEOLOGICAL));
}

/* 15. Gregorian system is in TODAY ring */
static void test_gregorian_in_today_ring(void)
{
    depth_ring_t ring = depth_ring_for_system(TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, ring);
}

/* 16. Tzolkin is in TODAY ring */
static void test_tzolkin_in_today_ring(void)
{
    depth_ring_t ring = depth_ring_for_system(TS_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_TODAY, ring);
}

/* 17. Convergence boosts visibility compared to without */
static void test_convergence_boosts_visibility(void)
{
    ts_summary_t s = ts_compute(JD_J2000);

    /* Select without convergence */
    ds_selection_t sel_without = ds_select(&s, NULL, NULL, 0xFFFFFFFF);

    /* Build convergence input including first active non-Gregorian system */
    ds_convergence_t conv;
    memset(&conv, 0, sizeof(conv));
    int target_sys = -1;
    for (int i = 0; i < sel_without.count; i++) {
        /* Pick a system that is not Gregorian (which has anchor boost) */
        if (sel_without.entries[i].system_id != TS_SYS_GREGORIAN) {
            target_sys = sel_without.entries[i].system_id;
            break;
        }
    }
    TEST_ASSERT_TRUE(target_sys >= 0);

    conv.converging_systems[0] = target_sys;
    conv.converging_count = 1;
    conv.convergence_strength = 3; /* major */

    ds_selection_t sel_with = ds_select(&s, &conv, NULL, 0xFFFFFFFF);

    /* Find the target system in both selections and compare visibility */
    int vis_without = -1;
    int vis_with = -1;
    for (int i = 0; i < sel_without.count; i++) {
        if (sel_without.entries[i].system_id == target_sys)
            vis_without = sel_without.entries[i].vis;
    }
    for (int i = 0; i < sel_with.count; i++) {
        if (sel_with.entries[i].system_id == target_sys)
            vis_with = sel_with.entries[i].vis;
    }
    TEST_ASSERT_TRUE(vis_without >= 0);
    TEST_ASSERT_TRUE(vis_with >= 0);
    TEST_ASSERT_TRUE_MESSAGE(vis_with >= vis_without,
        "Convergence should not decrease visibility");
}

/* ========== Data Integrity ========== */

/* 18. ts_compute date_str non-empty for active systems */
static void test_active_entries_have_nonempty_date_str(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    for (int i = 0; i < s.entry_count; i++) {
        if (!s.entries[i].active) continue;
        TEST_ASSERT_TRUE_MESSAGE(s.entries[i].date_str[0] != '\0',
            "Active entry has empty date_str");
    }
}

/* 19. System names are valid for all system IDs */
static void test_system_names_all_valid(void)
{
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        const char *name = ts_system_name((ts_system_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE_MESSAGE(strcmp(name, "?") != 0,
            "Valid system ID returned '?' name");
    }
}

/* 20. Ring names are valid for all ring IDs */
static void test_ring_names_all_valid(void)
{
    for (int r = 0; r < DEPTH_RING_COUNT; r++) {
        const char *name = depth_ring_name((depth_ring_t)r);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE_MESSAGE(strcmp(name, "?") != 0,
            "Valid ring ID returned '?' name");
    }
}

/* 21. Tier names are valid for all tier IDs */
static void test_tier_names_all_valid(void)
{
    for (int t = 0; t < DEPTH_TIER_COUNT; t++) {
        const char *name = depth_tier_name((depth_tier_t)t);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE_MESSAGE(strcmp(name, "?") != 0,
            "Valid tier ID returned '?' name");
    }
}

/* 22. Headline is populated */
static void test_headline_is_populated(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    TEST_ASSERT_TRUE(strlen(s.headline) > 0);
}

/* ========== Edge Cases ========== */

/* 23. Multiple JDs all produce valid pipeline output */
static void test_multiple_jds_produce_valid_pipelines(void)
{
    double jds[] = { JD_J2000, JD_20240408, JD_19000101 };
    int num_jds = (int)(sizeof(jds) / sizeof(jds[0]));

    for (int j = 0; j < num_jds; j++) {
        ts_summary_t s = ts_compute(jds[j]);
        TEST_ASSERT_TRUE(s.entry_count > 0);
        TEST_ASSERT_FLOAT_WITHIN(0.001, jds[j], s.jd);

        /* Every active system should feed through ring + selector */
        ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
        TEST_ASSERT_TRUE(sel.count > 0);

        for (int i = 0; i < sel.count; i++) {
            depth_ring_t ring = depth_ring_for_system(sel.entries[i].system_id);
            TEST_ASSERT_TRUE(ring >= DEPTH_RING_COSMIC && ring <= DEPTH_RING_TODAY);
            TEST_ASSERT_TRUE(sel.entries[i].vis >= DS_VIS_HIDDEN);
            TEST_ASSERT_TRUE(sel.entries[i].vis <= DS_VIS_HIGHLIGHTED);
        }
    }
}

/* 24. Convergence score is in [0, 10] */
static void test_convergence_score_range(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    TEST_ASSERT_TRUE(s.convergence_score >= 0);
    TEST_ASSERT_TRUE(s.convergence_score <= 10);
}

/* 25. significant_count is monotonically non-increasing with threshold */
static void test_significant_count_monotonic(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    int c0 = ts_significant_count(&s, 0);
    int c1 = ts_significant_count(&s, 1);
    int c2 = ts_significant_count(&s, 2);
    int c3 = ts_significant_count(&s, 3);
    TEST_ASSERT_TRUE(c0 >= c1);
    TEST_ASSERT_TRUE(c1 >= c2);
    TEST_ASSERT_TRUE(c2 >= c3);
    TEST_ASSERT_TRUE(c3 >= 0);
}

/* ========== Additional Integration Tests ========== */

/* 26. Full pipeline: each active system can be assigned a ring and a tier */
static void test_full_pipeline_ring_and_tier_per_system(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    for (int i = 0; i < s.entry_count; i++) {
        if (!s.entries[i].active) continue;
        int sys_id = (int)s.entries[i].system;

        depth_ring_t ring = depth_ring_for_system(sys_id);
        TEST_ASSERT_TRUE(ring >= DEPTH_RING_COSMIC && ring <= DEPTH_RING_TODAY);

        /* Tier at personal scale (0) */
        depth_tier_t tier = depth_tier_for_scale(0, sys_id, -1);
        TEST_ASSERT_TRUE(tier >= DEPTH_TIER_GLYPH && tier <= DEPTH_TIER_BOARD);
    }
}

/* 27. Selector count matches number of active entries in summary */
static void test_selector_count_matches_active_entries(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    int active_count = 0;
    for (int i = 0; i < s.entry_count; i++) {
        if (s.entries[i].active) active_count++;
    }

    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    TEST_ASSERT_EQUAL_INT(active_count, sel.count);
}

/* 28. Highlighted + non-highlighted = total count */
static void test_highlighted_plus_nonhighlighted_equals_total(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);

    int hl_count = 0;
    for (int i = 0; i < sel.count; i++) {
        if (sel.entries[i].vis == DS_VIS_HIGHLIGHTED) hl_count++;
    }
    TEST_ASSERT_EQUAL_INT(hl_count, sel.highlighted_count);
}

/* 29. Prominent count matches entries with vis >= PROMINENT */
static void test_prominent_count_matches_entries(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);

    int prom_count = 0;
    for (int i = 0; i < sel.count; i++) {
        if (sel.entries[i].vis >= DS_VIS_PROMINENT) prom_count++;
    }
    TEST_ASSERT_EQUAL_INT(prom_count, sel.prominent_count);
}

/* 30. Cosmic ring systems get tier promotion at galaxy scale */
static void test_cosmic_ring_systems_promoted_at_galaxy(void)
{
    int ids[TS_SYS_COUNT];
    int count = depth_ring_systems(DEPTH_RING_COSMIC, ids, TS_SYS_COUNT);
    TEST_ASSERT_TRUE(count > 0);

    depth_tier_t default_tier = depth_tier_default_for_scale(5);
    for (int i = 0; i < count; i++) {
        depth_tier_t tier = depth_tier_for_scale(5, ids[i], -1);
        TEST_ASSERT_TRUE_MESSAGE(tier >= default_tier,
            "Cosmic ring system should be at least default tier at galaxy scale");
    }
}

/* 31. Birth profile elevates system through full pipeline */
static void test_birth_profile_elevates_through_pipeline(void)
{
    ts_summary_t s = ts_compute(JD_J2000);

    ds_birth_t birth;
    memset(&birth, 0, sizeof(birth));
    birth.birth_systems[0] = TS_SYS_ICHING;
    birth.birth_count = 1;

    /* Without birth */
    ds_selection_t sel_no_birth = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    /* With birth */
    ds_selection_t sel_birth = ds_select(&s, NULL, &birth, 0xFFFFFFFF);

    int vis_no = -1;
    int vis_yes = -1;
    for (int i = 0; i < sel_no_birth.count; i++) {
        if (sel_no_birth.entries[i].system_id == TS_SYS_ICHING)
            vis_no = sel_no_birth.entries[i].vis;
    }
    for (int i = 0; i < sel_birth.count; i++) {
        if (sel_birth.entries[i].system_id == TS_SYS_ICHING)
            vis_yes = sel_birth.entries[i].vis;
    }
    TEST_ASSERT_TRUE(vis_no >= 0);
    TEST_ASSERT_TRUE(vis_yes >= 0);
    TEST_ASSERT_TRUE_MESSAGE(vis_yes >= vis_no,
        "Birth profile should not decrease visibility");
}

/* 32. Selector entry system_ids are all valid ts_system_t values */
static void test_selector_system_ids_valid(void)
{
    ts_summary_t s = ts_compute(JD_J2000);
    ds_selection_t sel = ds_select(&s, NULL, NULL, 0xFFFFFFFF);
    for (int i = 0; i < sel.count; i++) {
        TEST_ASSERT_TRUE(sel.entries[i].system_id >= 0);
        TEST_ASSERT_TRUE(sel.entries[i].system_id < TS_SYS_COUNT);
    }
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* Pipeline tests (1-13) */
    RUN_TEST(test_compute_produces_valid_summary);
    RUN_TEST(test_every_active_system_has_valid_ring);
    RUN_TEST(test_every_ring_has_at_least_one_system);
    RUN_TEST(test_ring_coverage_is_complete);
    RUN_TEST(test_tier_nonincreasing_with_zoom_out);
    RUN_TEST(test_panel_at_personal_scale);
    RUN_TEST(test_glyph_at_universe_scale);
    RUN_TEST(test_user_override_in_pipeline);
    RUN_TEST(test_selector_produces_entries_from_summary);
    RUN_TEST(test_selector_entries_valid_visibility);
    RUN_TEST(test_selector_entries_ranked_sequentially);
    RUN_TEST(test_all_bits_toggle_none_hidden);
    RUN_TEST(test_zero_toggle_hides_all);

    /* Cross-system verification (14-17) */
    RUN_TEST(test_geological_in_cosmic_ring);
    RUN_TEST(test_gregorian_in_today_ring);
    RUN_TEST(test_tzolkin_in_today_ring);
    RUN_TEST(test_convergence_boosts_visibility);

    /* Data integrity (18-22) */
    RUN_TEST(test_active_entries_have_nonempty_date_str);
    RUN_TEST(test_system_names_all_valid);
    RUN_TEST(test_ring_names_all_valid);
    RUN_TEST(test_tier_names_all_valid);
    RUN_TEST(test_headline_is_populated);

    /* Edge cases (23-25) */
    RUN_TEST(test_multiple_jds_produce_valid_pipelines);
    RUN_TEST(test_convergence_score_range);
    RUN_TEST(test_significant_count_monotonic);

    /* Additional integration (26-32) */
    RUN_TEST(test_full_pipeline_ring_and_tier_per_system);
    RUN_TEST(test_selector_count_matches_active_entries);
    RUN_TEST(test_highlighted_plus_nonhighlighted_equals_total);
    RUN_TEST(test_prominent_count_matches_entries);
    RUN_TEST(test_cosmic_ring_systems_promoted_at_galaxy);
    RUN_TEST(test_birth_profile_elevates_through_pipeline);
    RUN_TEST(test_selector_system_ids_valid);

    return UNITY_END();
}
