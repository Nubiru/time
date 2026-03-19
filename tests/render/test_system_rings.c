#include "../unity/unity.h"
#include "../../src/render/system_rings.h"
#include "../../src/systems/unified/today_summary.h"
#include "../../src/systems/unified/depth_ring.h"
#include "../../src/render/concentric_ring.h"

void setUp(void) {}
void tearDown(void) {}

/* 1. Zodiac cycle length is 12 */
void test_sr_cycle_length_zodiac(void)
{
    TEST_ASSERT_EQUAL_INT(12, sr_cycle_length(TS_SYS_ASTROLOGY));
}

/* 2. Tzolkin cycle length is 260 */
void test_sr_cycle_length_tzolkin(void)
{
    TEST_ASSERT_EQUAL_INT(260, sr_cycle_length(TS_SYS_TZOLKIN));
}

/* 3. I Ching cycle length is 64 */
void test_sr_cycle_length_iching(void)
{
    TEST_ASSERT_EQUAL_INT(64, sr_cycle_length(TS_SYS_ICHING));
}

/* 4. Chinese cycle length is 60 */
void test_sr_cycle_length_chinese(void)
{
    TEST_ASSERT_EQUAL_INT(60, sr_cycle_length(TS_SYS_CHINESE));
}

/* 5. Hebrew cycle length is 13 (months) */
void test_sr_cycle_length_hebrew(void)
{
    TEST_ASSERT_EQUAL_INT(13, sr_cycle_length(TS_SYS_HEBREW));
}

/* 6. Unknown system returns default 12 */
void test_sr_cycle_length_unknown(void)
{
    TEST_ASSERT_EQUAL_INT(12, sr_cycle_length(999));
    TEST_ASSERT_EQUAL_INT(12, sr_cycle_length(-1));
}

/* 7. System def has valid color components in [0,1] */
void test_sr_system_def_has_color(void)
{
    for (int i = 0; i < (int)TS_SYS_COUNT; i++) {
        sr_system_def_t def = sr_system_def(i);
        for (int c = 0; c < 3; c++) {
            TEST_ASSERT_TRUE(def.color[c] >= 0.0f);
            TEST_ASSERT_TRUE(def.color[c] <= 1.0f);
        }
    }
}

/* 8. System def has non-NULL label */
void test_sr_system_def_has_label(void)
{
    for (int i = 0; i < (int)TS_SYS_COUNT; i++) {
        sr_system_def_t def = sr_system_def(i);
        TEST_ASSERT_NOT_NULL(def.label);
    }
}

/* 9. System def today_index defaults to -1 */
void test_sr_system_def_today_default(void)
{
    sr_system_def_t def = sr_system_def(TS_SYS_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(-1, def.today_index);
}

/* 10. Build layout has correct ring_count */
void test_sr_build_layout_count(void)
{
    int ids[] = { TS_SYS_ASTROLOGY, TS_SYS_TZOLKIN, TS_SYS_ICHING };
    cr_layout_t layout = sr_build_layout(ids, NULL, 3, 1.0f, 0.1f);
    TEST_ASSERT_EQUAL_INT(3, layout.ring_count);
}

/* 11. Each ring has correct segment count */
void test_sr_build_layout_segments(void)
{
    int ids[] = { TS_SYS_ASTROLOGY, TS_SYS_TZOLKIN, TS_SYS_CHINESE };
    cr_layout_t layout = sr_build_layout(ids, NULL, 3, 1.0f, 0.1f);
    TEST_ASSERT_EQUAL_INT(12, layout.rings[0].segment_count);
    TEST_ASSERT_EQUAL_INT(260, layout.rings[1].segment_count);
    TEST_ASSERT_EQUAL_INT(60, layout.rings[2].segment_count);
}

/* 12. Today indices are propagated to highlight_index */
void test_sr_build_layout_highlight(void)
{
    int ids[] = { TS_SYS_ASTROLOGY, TS_SYS_ICHING };
    int today[] = { 5, 33 };
    cr_layout_t layout = sr_build_layout(ids, today, 2, 1.0f, 0.1f);
    TEST_ASSERT_EQUAL_INT(5, layout.rings[0].highlight_index);
    TEST_ASSERT_EQUAL_INT(33, layout.rings[1].highlight_index);
}

/* 13. Count > SR_MAX_SYSTEMS gets clamped */
void test_sr_build_layout_clamped(void)
{
    int ids[16];
    for (int i = 0; i < 16; i++) ids[i] = TS_SYS_GREGORIAN;
    cr_layout_t layout = sr_build_layout(ids, NULL, 16, 1.0f, 0.1f);
    TEST_ASSERT_TRUE(layout.ring_count <= SR_MAX_SYSTEMS);
    TEST_ASSERT_TRUE(layout.ring_count <= CR_MAX_RINGS);
}

/* 14. NULL today_indices gives all highlight_index = -1 */
void test_sr_build_layout_null_today(void)
{
    int ids[] = { TS_SYS_ASTROLOGY, TS_SYS_TZOLKIN };
    cr_layout_t layout = sr_build_layout(ids, NULL, 2, 1.0f, 0.1f);
    TEST_ASSERT_EQUAL_INT(-1, layout.rings[0].highlight_index);
    TEST_ASSERT_EQUAL_INT(-1, layout.rings[1].highlight_index);
}

/* 15. Depth layout produces exactly 6 rings */
void test_sr_depth_layout_6_rings(void)
{
    cr_layout_t layout = sr_depth_layout(NULL, 1.0f, 0.1f);
    TEST_ASSERT_EQUAL_INT(DEPTH_RING_COUNT, layout.ring_count);
}

/* 16. Each depth ring has a valid representative */
void test_sr_depth_representative_valid(void)
{
    for (int r = 0; r < DEPTH_RING_COUNT; r++) {
        int rep = sr_depth_representative(r);
        TEST_ASSERT_TRUE(rep >= 0);
        TEST_ASSERT_TRUE(rep < (int)TS_SYS_COUNT);
    }
}

/* 17. Depth layout segment counts are > 0 and <= CR_MAX_SEGMENTS */
void test_sr_depth_layout_segments_valid(void)
{
    cr_layout_t layout = sr_depth_layout(NULL, 1.0f, 0.1f);
    for (int i = 0; i < layout.ring_count; i++) {
        TEST_ASSERT_TRUE(layout.rings[i].segment_count > 0);
        TEST_ASSERT_TRUE(layout.rings[i].segment_count <= CR_MAX_SEGMENTS);
    }
}

/* 18. At least 10 known systems */
void test_sr_known_system_count(void)
{
    TEST_ASSERT_TRUE(sr_known_system_count() >= 10);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sr_cycle_length_zodiac);
    RUN_TEST(test_sr_cycle_length_tzolkin);
    RUN_TEST(test_sr_cycle_length_iching);
    RUN_TEST(test_sr_cycle_length_chinese);
    RUN_TEST(test_sr_cycle_length_hebrew);
    RUN_TEST(test_sr_cycle_length_unknown);
    RUN_TEST(test_sr_system_def_has_color);
    RUN_TEST(test_sr_system_def_has_label);
    RUN_TEST(test_sr_system_def_today_default);
    RUN_TEST(test_sr_build_layout_count);
    RUN_TEST(test_sr_build_layout_segments);
    RUN_TEST(test_sr_build_layout_highlight);
    RUN_TEST(test_sr_build_layout_clamped);
    RUN_TEST(test_sr_build_layout_null_today);
    RUN_TEST(test_sr_depth_layout_6_rings);
    RUN_TEST(test_sr_depth_representative_valid);
    RUN_TEST(test_sr_depth_layout_segments_valid);
    RUN_TEST(test_sr_known_system_count);
    return UNITY_END();
}
