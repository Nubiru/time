/* test_text_lod.c — Tests for text level-of-detail module (LANGUAGE L1.2)
 *
 * TDD: text_lod maps depth_tier + system data → text at correct verbosity,
 * with phi-timed transitions between depth levels. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/text_lod.h"

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Helper: build a ts_entry
 * ================================================================ */

static ts_entry_t make_entry(ts_system_t sys, const char *date_str,
                              const char *note, int sig)
{
    ts_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = sys;
    snprintf(e.date_str, TS_DATE_MAX, "%s", date_str);
    if (note) snprintf(e.note, TS_NOTE_MAX, "%s", note);
    e.significance = sig;
    e.active = 1;
    return e;
}

static depth_interp_t make_interp(const char *glyph, const char *glance,
                                   const char *detail, int sys_id)
{
    depth_interp_t di;
    memset(&di, 0, sizeof(di));
    if (glyph) snprintf(di.glyph, sizeof(di.glyph), "%s", glyph);
    if (glance) snprintf(di.glance, sizeof(di.glance), "%s", glance);
    if (detail) snprintf(di.detail, sizeof(di.detail), "%s", detail);
    di.system_id = sys_id;
    di.has_data = 1;
    return di;
}

/* ================================================================
 * text_lod_for_system: T0 (glyph) returns short text
 * ================================================================ */

void test_text_lod_glyph_returns_short(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HEBREW, "5786 Adar 19", "Purim", 3);
    depth_interp_t interp = make_interp("\xE2\x9C\xA1", "5786 Adar 19 — Purim",
                                         "The month of joy and hidden miracles.", TS_SYS_HEBREW);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_GLYPH);

    TEST_ASSERT_TRUE(lod.text[0] != '\0');
    /* At glyph tier, text should be very short */
    TEST_ASSERT_TRUE(strlen(lod.text) <= 32);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLYPH, lod.tier);
}

/* ================================================================
 * text_lod_for_system: T1 (glance) returns one-liner
 * ================================================================ */

void test_text_lod_glance_returns_oneliner(void)
{
    ts_entry_t entry = make_entry(TS_SYS_TZOLKIN,
                                   "Kin 207 Blue Crystal Hand", "Portal Day", 2);
    depth_interp_t interp = make_interp("K207", "Kin 207: Blue Crystal Hand — Portal Day",
                                         "Today is a portal day in the Dreamspell.", TS_SYS_TZOLKIN);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_GLANCE);

    TEST_ASSERT_TRUE(lod.text[0] != '\0');
    TEST_ASSERT_TRUE(strlen(lod.text) > 10);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE, lod.tier);
}

/* ================================================================
 * text_lod_for_system: T2 (card) includes note
 * ================================================================ */

void test_text_lod_card_includes_meaning(void)
{
    ts_entry_t entry = make_entry(TS_SYS_ICHING,
                                   "Hexagram 24: Return", "Solstice energy", 2);
    depth_interp_t interp = make_interp("24", "Hexagram 24: Return",
                                         "Return marks the winter solstice moment.", TS_SYS_ICHING);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_CARD);

    TEST_ASSERT_TRUE(lod.text[0] != '\0');
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD, lod.tier);
    /* Card level should be longer than glance */
    text_lod_t glance = text_lod_for_system(&entry, &interp, DEPTH_TIER_GLANCE);
    TEST_ASSERT_TRUE(strlen(lod.text) >= strlen(glance.text));
}

/* ================================================================
 * text_lod_for_system: T3 (panel) returns full detail
 * ================================================================ */

void test_text_lod_panel_returns_detail(void)
{
    ts_entry_t entry = make_entry(TS_SYS_CHINESE,
                                   "Year of the Snake", "Wood Snake", 1);
    depth_interp_t interp = make_interp("\xF0\x9F\x90\x8D", "Year of the Wood Snake",
                                         "The Snake brings transformation and hidden wisdom.", TS_SYS_CHINESE);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_PANEL);

    TEST_ASSERT_TRUE(lod.text[0] != '\0');
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL, lod.tier);
    /* Panel should include detail text */
    TEST_ASSERT_TRUE(strlen(lod.text) > 30);
}

/* ================================================================
 * text_lod_for_system: T4 (board) returns maximum detail
 * ================================================================ */

void test_text_lod_board_maximum(void)
{
    ts_entry_t entry = make_entry(TS_SYS_ASTROLOGY,
                                   "Sun in Pisces", "Neptune conjunction", 2);
    depth_interp_t interp = make_interp("\xE2\x99\x93", "Sun in Pisces",
                                         "Pisces season brings dissolution of boundaries.", TS_SYS_ASTROLOGY);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_BOARD);

    TEST_ASSERT_TRUE(lod.text[0] != '\0');
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_BOARD, lod.tier);
}

/* ================================================================
 * text_lod_for_system: NULL entry returns empty
 * ================================================================ */

void test_text_lod_null_entry(void)
{
    depth_interp_t interp = make_interp("X", "test", "detail", 0);
    text_lod_t lod = text_lod_for_system(NULL, &interp, DEPTH_TIER_GLANCE);
    TEST_ASSERT_EQUAL_STRING("", lod.text);
}

/* ================================================================
 * text_lod_for_system: NULL interp still uses entry data
 * ================================================================ */

void test_text_lod_null_interp_uses_entry(void)
{
    ts_entry_t entry = make_entry(TS_SYS_GREGORIAN,
                                   "March 19, 2026", NULL, 0);
    text_lod_t lod = text_lod_for_system(&entry, NULL, DEPTH_TIER_GLANCE);
    TEST_ASSERT_TRUE(lod.text[0] != '\0');
}

/* ================================================================
 * text_lod_for_system: inactive entry returns empty
 * ================================================================ */

void test_text_lod_inactive_entry(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HEBREW, "5786 Adar 19", NULL, 0);
    entry.active = 0;
    depth_interp_t interp = make_interp("X", "test", "detail", TS_SYS_HEBREW);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_GLANCE);
    TEST_ASSERT_EQUAL_STRING("", lod.text);
}

/* ================================================================
 * Transition: init state
 * ================================================================ */

void test_text_lod_transition_init(void)
{
    text_lod_transition_t tr;
    text_lod_transition_init(&tr, DEPTH_TIER_GLANCE);

    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE, tr.current_tier);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE, tr.target_tier);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)tr.opacity);
    TEST_ASSERT_FALSE(tr.transitioning);
}

/* ================================================================
 * Transition: set target triggers transition
 * ================================================================ */

void test_text_lod_transition_set_target(void)
{
    text_lod_transition_t tr;
    text_lod_transition_init(&tr, DEPTH_TIER_GLANCE);

    text_lod_transition_set_target(&tr, DEPTH_TIER_PANEL);

    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_GLANCE, tr.current_tier);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_PANEL, tr.target_tier);
    TEST_ASSERT_TRUE(tr.transitioning);
}

/* ================================================================
 * Transition: same target no-ops
 * ================================================================ */

void test_text_lod_transition_same_target(void)
{
    text_lod_transition_t tr;
    text_lod_transition_init(&tr, DEPTH_TIER_CARD);

    text_lod_transition_set_target(&tr, DEPTH_TIER_CARD);
    TEST_ASSERT_FALSE(tr.transitioning);
}

/* ================================================================
 * Transition: update progresses opacity
 * ================================================================ */

void test_text_lod_transition_update_progresses(void)
{
    text_lod_transition_t tr;
    text_lod_transition_init(&tr, DEPTH_TIER_GLANCE);
    text_lod_transition_set_target(&tr, DEPTH_TIER_PANEL);

    /* First half: fading out old */
    text_lod_transition_update(&tr, 0.5);
    TEST_ASSERT_TRUE(tr.transitioning);
    TEST_ASSERT_TRUE(tr.opacity < 1.0);
}

/* ================================================================
 * Transition: full duration completes transition
 * ================================================================ */

void test_text_lod_transition_completes(void)
{
    text_lod_transition_t tr;
    text_lod_transition_init(&tr, DEPTH_TIER_GLYPH);
    text_lod_transition_set_target(&tr, DEPTH_TIER_CARD);

    /* Run enough time to complete (phi-timed: ~1.618s total) */
    for (int i = 0; i < 20; i++) {
        text_lod_transition_update(&tr, 0.1);
    }

    TEST_ASSERT_FALSE(tr.transitioning);
    TEST_ASSERT_EQUAL_INT(DEPTH_TIER_CARD, tr.current_tier);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)tr.opacity);
}

/* ================================================================
 * Transition: NULL safety
 * ================================================================ */

void test_text_lod_transition_null_safety(void)
{
    text_lod_transition_init(NULL, DEPTH_TIER_GLYPH);
    text_lod_transition_set_target(NULL, DEPTH_TIER_CARD);
    text_lod_transition_update(NULL, 0.1);
    /* No crash = pass */
    TEST_ASSERT_TRUE(1);
}

/* ================================================================
 * text_lod_tier_name: maps correctly
 * ================================================================ */

void test_text_lod_tier_text_length(void)
{
    ts_entry_t entry = make_entry(TS_SYS_GREGORIAN, "March 19, 2026", NULL, 0);
    depth_interp_t interp = make_interp("19", "March 19, 2026",
                                         "The Gregorian calendar counts from an estimated birth year.", TS_SYS_GREGORIAN);

    /* Text should generally grow with tier */
    text_lod_t t0 = text_lod_for_system(&entry, &interp, DEPTH_TIER_GLYPH);
    text_lod_t t1 = text_lod_for_system(&entry, &interp, DEPTH_TIER_GLANCE);
    text_lod_t t3 = text_lod_for_system(&entry, &interp, DEPTH_TIER_PANEL);

    TEST_ASSERT_TRUE(strlen(t0.text) <= strlen(t1.text));
    TEST_ASSERT_TRUE(strlen(t1.text) <= strlen(t3.text));
}

/* ================================================================
 * text_lod_for_system: significance note appended at card+ tier
 * ================================================================ */

void test_text_lod_note_at_card_tier(void)
{
    ts_entry_t entry = make_entry(TS_SYS_BUDDHIST,
                                   "2570 BE", "Uposatha day", 2);
    depth_interp_t interp = make_interp("B", "2570 BE — Uposatha day",
                                         "The Buddhist calendar follows the Buddhasakarat era.",
                                         TS_SYS_BUDDHIST);

    text_lod_t lod = text_lod_for_system(&entry, &interp, DEPTH_TIER_CARD);

    /* Card tier should include the note */
    TEST_ASSERT_NOT_NULL(strstr(lod.text, "Uposatha"));
}

/* ================================================================
 * Runner
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_text_lod_glyph_returns_short);
    RUN_TEST(test_text_lod_glance_returns_oneliner);
    RUN_TEST(test_text_lod_card_includes_meaning);
    RUN_TEST(test_text_lod_panel_returns_detail);
    RUN_TEST(test_text_lod_board_maximum);
    RUN_TEST(test_text_lod_null_entry);
    RUN_TEST(test_text_lod_null_interp_uses_entry);
    RUN_TEST(test_text_lod_inactive_entry);
    RUN_TEST(test_text_lod_transition_init);
    RUN_TEST(test_text_lod_transition_set_target);
    RUN_TEST(test_text_lod_transition_same_target);
    RUN_TEST(test_text_lod_transition_update_progresses);
    RUN_TEST(test_text_lod_transition_completes);
    RUN_TEST(test_text_lod_transition_null_safety);
    RUN_TEST(test_text_lod_tier_text_length);
    RUN_TEST(test_text_lod_note_at_card_tier);

    return UNITY_END();
}
