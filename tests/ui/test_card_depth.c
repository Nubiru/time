#include "../unity/unity.h"
#include "../../src/ui/card_depth.h"

void setUp(void) {}
void tearDown(void) {}

/* --- cd_default_planes tests --- */

void test_default_planes_count(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    TEST_ASSERT_GREATER_OR_EQUAL(15, count);
    TEST_ASSERT_LESS_OR_EQUAL(CD_MAX_CARDS, count);
}

void test_default_planes_tzolkin_depth(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (planes[i].system_id == 1) { /* TS_SYS_TZOLKIN */
            TEST_ASSERT_FLOAT_WITHIN(0.01f, 5.5f, planes[i].depth);
            found = 1;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "Tzolkin plane not found");
}

void test_default_planes_geology_depth(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (planes[i].system_id == 18) { /* TS_SYS_GEOLOGICAL */
            TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, planes[i].depth);
            found = 1;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "Geological plane not found");
}

void test_default_planes_astrology_depth(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (planes[i].system_id == 9) { /* TS_SYS_ASTROLOGY */
            TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.5f, planes[i].depth);
            found = 1;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "Astrology plane not found");
}

/* --- cd_compute tests --- */

void test_compute_focal_at_tzolkin(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(5.5f, planes, count, 16.0f / 9.0f);

    const cd_card_state_t *tzk = cd_find_system(&frame, 1); /* TS_SYS_TZOLKIN */
    TEST_ASSERT_NOT_NULL(tzk);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, tzk->scale);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, tzk->opacity);
}

void test_compute_distant_fades(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(5.5f, planes, count, 16.0f / 9.0f);

    const cd_card_state_t *geo = cd_find_system(&frame, 18); /* TS_SYS_GEOLOGICAL */
    TEST_ASSERT_NOT_NULL(geo);
    TEST_ASSERT_LESS_THAN_FLOAT(0.1f, geo->opacity);
}

void test_compute_focal_index_valid(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(3.5f, planes, count, 16.0f / 9.0f);

    TEST_ASSERT_GREATER_OR_EQUAL(0, frame.focal_index);
    TEST_ASSERT_LESS_THAN(frame.count, frame.focal_index);

    /* The focal card should have the highest opacity */
    float max_opacity = 0.0f;
    int max_idx = -1;
    for (int i = 0; i < frame.count; i++) {
        if (frame.cards[i].opacity > max_opacity) {
            max_opacity = frame.cards[i].opacity;
            max_idx = i;
        }
    }
    TEST_ASSERT_EQUAL(max_idx, frame.focal_index);
}

void test_compute_focal_depth_matches(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(2.75f, planes, count, 16.0f / 9.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.75f, frame.focal_depth);
}

void test_compute_all_scales_positive(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(3.0f, planes, count, 16.0f / 9.0f);

    for (int i = 0; i < frame.count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, frame.cards[i].scale);
    }
}

void test_compute_all_opacities_bounded(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(3.0f, planes, count, 16.0f / 9.0f);

    for (int i = 0; i < frame.count; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0f, frame.cards[i].opacity);
        TEST_ASSERT_LESS_OR_EQUAL_FLOAT(1.0f, frame.cards[i].opacity);
    }
}

/* --- cd_visible_count tests --- */

void test_visible_count_at_focal(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(5.5f, planes, count, 16.0f / 9.0f);

    int vis = cd_visible_count(&frame);
    TEST_ASSERT_GREATER_OR_EQUAL(2, vis);
}

void test_visible_count_at_middle(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(3.0f, planes, count, 16.0f / 9.0f);

    int vis = cd_visible_count(&frame);
    TEST_ASSERT_GREATER_OR_EQUAL(1, vis);
}

/* --- cd_find_system tests --- */

void test_find_system_exists(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(3.0f, planes, count, 16.0f / 9.0f);

    const cd_card_state_t *card = cd_find_system(&frame, 1); /* TS_SYS_TZOLKIN */
    TEST_ASSERT_NOT_NULL(card);
    TEST_ASSERT_EQUAL(1, card->system_id);
}

void test_find_system_missing(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(3.0f, planes, count, 16.0f / 9.0f);

    const cd_card_state_t *card = cd_find_system(&frame, 999);
    TEST_ASSERT_NULL(card);
}

/* --- cd_depth_distance tests --- */

void test_depth_distance_positive(void)
{
    float d = cd_depth_distance(3.0f, 5.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, d);
}

void test_depth_distance_negative(void)
{
    float d = cd_depth_distance(5.0f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -2.0f, d);
}

void test_depth_distance_zero(void)
{
    float d = cd_depth_distance(3.5f, 3.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, d);
}

/* --- scale maximization at focal plane --- */

void test_scale_at_focal_plane_max(void)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);

    /* Place camera exactly at Tzolkin's depth (5.5) */
    cd_frame_t frame = cd_compute(5.5f, planes, count, 16.0f / 9.0f);

    const cd_card_state_t *tzk = cd_find_system(&frame, 1); /* TS_SYS_TZOLKIN */
    TEST_ASSERT_NOT_NULL(tzk);

    /* Tzolkin at depth 5.5, camera at 5.5 => distance 0 => highest scale */
    for (int i = 0; i < frame.count; i++) {
        if (frame.cards[i].system_id != 1) {
            TEST_ASSERT_LESS_OR_EQUAL_FLOAT(tzk->scale, frame.cards[i].scale);
        }
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_planes_count);
    RUN_TEST(test_default_planes_tzolkin_depth);
    RUN_TEST(test_default_planes_geology_depth);
    RUN_TEST(test_default_planes_astrology_depth);
    RUN_TEST(test_compute_focal_at_tzolkin);
    RUN_TEST(test_compute_distant_fades);
    RUN_TEST(test_compute_focal_index_valid);
    RUN_TEST(test_compute_focal_depth_matches);
    RUN_TEST(test_compute_all_scales_positive);
    RUN_TEST(test_compute_all_opacities_bounded);
    RUN_TEST(test_visible_count_at_focal);
    RUN_TEST(test_visible_count_at_middle);
    RUN_TEST(test_find_system_exists);
    RUN_TEST(test_find_system_missing);
    RUN_TEST(test_depth_distance_positive);
    RUN_TEST(test_depth_distance_negative);
    RUN_TEST(test_depth_distance_zero);
    RUN_TEST(test_scale_at_focal_plane_max);
    return UNITY_END();
}
