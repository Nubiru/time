#include "../unity/unity.h"
#include "../../src/ui/card_selector.h"
#include "../../src/ui/card_depth.h"

void setUp(void) {}
void tearDown(void) {}

/* --- test_select_at_today_depth ---
 * At zoom 5.5 (today scale), Tzolkin should be in selection
 * because it lives at depth 5.5 with priority 1.0. */
void test_select_at_today_depth(void)
{
    cs_selection_t sel = cs_select(5.5f, 16.0f / 9.0f);
    int idx = cs_find_system(&sel, 1); /* TS_SYS_TZOLKIN = 1 */
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, idx, "Tzolkin not found at today depth");
}

/* --- test_select_at_year_depth ---
 * At zoom 3.5 (year scale), Astrology should be in selection
 * because it lives at depth 3.5 with priority 0.9. */
void test_select_at_year_depth(void)
{
    cs_selection_t sel = cs_select(3.5f, 16.0f / 9.0f);
    int idx = cs_find_system(&sel, 9); /* TS_SYS_ASTROLOGY = 9 */
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, idx, "Astrology not found at year depth");
}

/* --- test_select_at_cosmic_depth ---
 * At zoom 0.5 (cosmic), Geological should be in selection
 * because it lives at depth 0.5. */
void test_select_at_cosmic_depth(void)
{
    cs_selection_t sel = cs_select(0.5f, 16.0f / 9.0f);
    int idx = cs_find_system(&sel, 18); /* TS_SYS_GEOLOGICAL = 18 */
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, idx, "Geological not found at cosmic depth");
}

/* --- test_select_filled_count ---
 * filled_count should be 1-5 (never 0 at any zoom). */
void test_select_filled_count(void)
{
    float zooms[] = { 0.0f, 0.5f, 1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.0f };
    for (int i = 0; i < 8; i++) {
        cs_selection_t sel = cs_select(zooms[i], 16.0f / 9.0f);
        TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(1, sel.filled_count,
            "filled_count should be >= 1");
        TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(CS_SLOT_COUNT, sel.filled_count,
            "filled_count should be <= 5");
    }
}

/* --- test_select_max_five ---
 * filled_count never exceeds 5. */
void test_select_max_five(void)
{
    cs_selection_t sel = cs_select(3.5f, 16.0f / 9.0f);
    TEST_ASSERT_LESS_OR_EQUAL(CS_SLOT_COUNT, sel.filled_count);
}

/* --- test_select_zoom_stored ---
 * sel.zoom_level equals the input zoom. */
void test_select_zoom_stored(void)
{
    cs_selection_t sel = cs_select(2.75f, 16.0f / 9.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.75f, sel.zoom_level);
}

/* --- test_select_relevance_ordered ---
 * Slots are ordered by relevance (slot 0 most relevant). */
void test_select_relevance_ordered(void)
{
    cs_selection_t sel = cs_select(3.5f, 16.0f / 9.0f);
    for (int i = 0; i < sel.filled_count - 1; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(sel.slots[i + 1].relevance,
                                            sel.slots[i].relevance);
    }
}

/* --- test_select_all_opacities_valid ---
 * All opacity values in [0.0, 1.0]. */
void test_select_all_opacities_valid(void)
{
    cs_selection_t sel = cs_select(4.0f, 16.0f / 9.0f);
    for (int i = 0; i < sel.filled_count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, sel.slots[i].opacity);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, sel.slots[i].opacity);
    }
}

/* --- test_select_all_scales_valid ---
 * All scale values in [0.0, 1.0]. */
void test_select_all_scales_valid(void)
{
    cs_selection_t sel = cs_select(4.0f, 16.0f / 9.0f);
    for (int i = 0; i < sel.filled_count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, sel.slots[i].scale);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, sel.slots[i].scale);
    }
}

/* --- test_find_system_present ---
 * cs_find_system for a system IN selection returns 0-4. */
void test_find_system_present(void)
{
    cs_selection_t sel = cs_select(5.5f, 16.0f / 9.0f);
    /* At least the first slot should be findable */
    int first_sys = sel.slots[0].system_id;
    TEST_ASSERT_GREATER_OR_EQUAL(0, first_sys);
    int idx = cs_find_system(&sel, first_sys);
    TEST_ASSERT_EQUAL(0, idx);
}

/* --- test_find_system_absent ---
 * cs_find_system for a distant system returns -1. */
void test_find_system_absent(void)
{
    /* At today zoom (5.5), Geological (depth 0.5) shouldn't be in top 5 */
    cs_selection_t sel = cs_select(5.5f, 16.0f / 9.0f);
    int idx = cs_find_system(&sel, 18); /* TS_SYS_GEOLOGICAL */
    TEST_ASSERT_EQUAL_MESSAGE(-1, idx,
        "Geological should not be in selection at today zoom");
}

/* --- test_slot_name_valid ---
 * cs_slot_name for filled slot returns non-NULL. */
void test_slot_name_valid(void)
{
    cs_selection_t sel = cs_select(3.0f, 16.0f / 9.0f);
    for (int i = 0; i < sel.filled_count; i++) {
        const char *name = cs_slot_name(&sel.slots[i]);
        TEST_ASSERT_NOT_NULL_MESSAGE(name, "Filled slot should have a name");
    }
}

/* --- test_slot_name_empty ---
 * cs_slot_name for empty slot (-1) returns NULL. */
void test_slot_name_empty(void)
{
    cs_slot_t empty_slot;
    empty_slot.system_id = -1;
    empty_slot.relevance = 0.0f;
    empty_slot.opacity = 0.0f;
    empty_slot.scale = 0.0f;
    const char *name = cs_slot_name(&empty_slot);
    TEST_ASSERT_NULL_MESSAGE(name, "Empty slot should return NULL name");
}

/* --- test_select_from_frame_matches ---
 * cs_select and cs_select_from_frame with same input give same result. */
void test_select_from_frame_matches(void)
{
    float zoom = 4.0f;
    float aspect = 16.0f / 9.0f;

    /* Get result via cs_select */
    cs_selection_t sel1 = cs_select(zoom, aspect);

    /* Get result via manual cd_compute + cs_select_from_frame */
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(zoom, planes, count, aspect);
    cs_selection_t sel2 = cs_select_from_frame(&frame);

    TEST_ASSERT_EQUAL(sel1.filled_count, sel2.filled_count);
    for (int i = 0; i < sel1.filled_count; i++) {
        TEST_ASSERT_EQUAL(sel1.slots[i].system_id, sel2.slots[i].system_id);
        TEST_ASSERT_FLOAT_WITHIN(0.001f,
            sel1.slots[i].relevance, sel2.slots[i].relevance);
    }
}

/* --- test_different_zoom_different_selection ---
 * zoom=1.0 and zoom=5.0 give different top system. */
void test_different_zoom_different_selection(void)
{
    cs_selection_t sel_near = cs_select(1.0f, 16.0f / 9.0f);
    cs_selection_t sel_far  = cs_select(5.0f, 16.0f / 9.0f);
    /* The top system should differ at very different zoom levels */
    TEST_ASSERT_NOT_EQUAL_MESSAGE(sel_near.slots[0].system_id,
                                   sel_far.slots[0].system_id,
        "Top system should differ between zoom 1.0 and 5.0");
}

/* --- test_empty_slots_have_minus_one ---
 * Unfilled slots have system_id == -1. */
void test_empty_slots_have_minus_one(void)
{
    cs_selection_t sel = cs_select(3.5f, 16.0f / 9.0f);
    for (int i = sel.filled_count; i < CS_SLOT_COUNT; i++) {
        TEST_ASSERT_EQUAL_MESSAGE(-1, sel.slots[i].system_id,
            "Unfilled slot should have system_id == -1");
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_select_at_today_depth);
    RUN_TEST(test_select_at_year_depth);
    RUN_TEST(test_select_at_cosmic_depth);
    RUN_TEST(test_select_filled_count);
    RUN_TEST(test_select_max_five);
    RUN_TEST(test_select_zoom_stored);
    RUN_TEST(test_select_relevance_ordered);
    RUN_TEST(test_select_all_opacities_valid);
    RUN_TEST(test_select_all_scales_valid);
    RUN_TEST(test_find_system_present);
    RUN_TEST(test_find_system_absent);
    RUN_TEST(test_slot_name_valid);
    RUN_TEST(test_slot_name_empty);
    RUN_TEST(test_select_from_frame_matches);
    RUN_TEST(test_different_zoom_different_selection);
    RUN_TEST(test_empty_slots_have_minus_one);
    return UNITY_END();
}
