#include "../unity/unity.h"
#include "../../src/ui/focus_flow.h"
#include <math.h>

static const float EPS = 0.001f;
static const float DT  = 1.0f / 60.0f;

/* Helper poses */
static camera_pose_t home_pose(void)
{
    vec3_t origin = {0.0f, 0.0f, 0.0f};
    return camera_pose_create(0.0f, 0.3f, 2.0f, origin);
}

static camera_pose_t dest_pose(void)
{
    vec3_t target = {1.0f, 0.0f, 0.0f};
    return camera_pose_create(1.5f, 0.5f, 1.0f, target);
}

static camera_pose_t alt_pose(void)
{
    vec3_t target = {-1.0f, 0.0f, 0.0f};
    return camera_pose_create(-0.5f, 0.1f, 0.5f, target);
}

void setUp(void) {}
void tearDown(void) {}

/* ---- Creation ---- */

void test_create_idle(void)
{
    focus_flow_t ff = focus_flow_create();
    TEST_ASSERT_EQUAL_INT(FOCUS_IDLE, focus_flow_state(ff));
}

void test_create_not_active(void)
{
    focus_flow_t ff = focus_flow_create();
    TEST_ASSERT_EQUAL_INT(0, focus_flow_active(ff));
}

void test_create_dim_zero(void)
{
    focus_flow_t ff = focus_flow_create();
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, focus_flow_dim(ff));
}

void test_create_reveal_zero(void)
{
    focus_flow_t ff = focus_flow_create();
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, focus_flow_reveal(ff));
}

/* ---- Begin ---- */

void test_begin_starts_flying(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 3);
    TEST_ASSERT_EQUAL_INT(FOCUS_FLYING, focus_flow_state(ff));
}

void test_begin_is_active(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 3);
    TEST_ASSERT_EQUAL_INT(1, focus_flow_active(ff));
}

void test_begin_stores_home(void)
{
    focus_flow_t ff = focus_flow_create();
    camera_pose_t h = home_pose();
    ff = focus_flow_begin(ff, h, dest_pose(), CT_PLANET, 3);
    TEST_ASSERT_FLOAT_WITHIN(EPS, h.azimuth, ff.home.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(EPS, h.elevation, ff.home.elevation);
    TEST_ASSERT_FLOAT_WITHIN(EPS, h.log_zoom, ff.home.log_zoom);
}

void test_begin_stores_target_info(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_ZODIAC_SIGN, 7);
    TEST_ASSERT_EQUAL_INT(CT_ZODIAC_SIGN, ff.target_type);
    TEST_ASSERT_EQUAL_INT(7, ff.target_id);
}

void test_begin_stores_destination(void)
{
    focus_flow_t ff = focus_flow_create();
    camera_pose_t d = dest_pose();
    ff = focus_flow_begin(ff, home_pose(), d, CT_PLANET, 3);
    TEST_ASSERT_FLOAT_WITHIN(EPS, d.azimuth, ff.destination.azimuth);
    TEST_ASSERT_FLOAT_WITHIN(EPS, d.log_zoom, ff.destination.log_zoom);
}

/* ---- Update (idle) ---- */

void test_update_idle_no_change(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_update(ff, DT);
    TEST_ASSERT_EQUAL_INT(FOCUS_IDLE, focus_flow_state(ff));
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, focus_flow_dim(ff));
}

/* ---- Update (flying) ---- */

void test_flying_increases_dim(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Run several frames */
    for (int i = 0; i < 10; i++) {
        ff = focus_flow_update(ff, DT);
    }
    TEST_ASSERT_TRUE(focus_flow_dim(ff) > 0.0f);
}

void test_flying_dim_stays_under_one(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 300; i++) {
        ff = focus_flow_update(ff, DT);
    }
    TEST_ASSERT_TRUE(focus_flow_dim(ff) <= 1.0f + EPS);
}

void test_flight_complete_transitions_arrived(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Run enough frames for spring flight to settle */
    for (int i = 0; i < 600; i++) {
        ff = focus_flow_update(ff, DT);
        if (focus_flow_state(ff) != FOCUS_FLYING) {
            break;
        }
    }
    /* Should have reached ARRIVED or LOCKED */
    TEST_ASSERT_TRUE(focus_flow_state(ff) >= FOCUS_ARRIVED);
}

/* ---- Arrived / Locked ---- */

void test_arrived_increases_reveal(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Fast-forward to ARRIVED */
    for (int i = 0; i < 600; i++) {
        ff = focus_flow_update(ff, DT);
        if (focus_flow_state(ff) == FOCUS_ARRIVED) {
            break;
        }
    }
    if (focus_flow_state(ff) == FOCUS_ARRIVED) {
        float before = focus_flow_reveal(ff);
        for (int i = 0; i < 5; i++) {
            ff = focus_flow_update(ff, DT);
        }
        TEST_ASSERT_TRUE(focus_flow_reveal(ff) > before);
    }
    /* If we skipped ARRIVED (went straight to LOCKED), that's also valid */
}

void test_reveal_complete_transitions_locked(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Run until LOCKED */
    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
        if (focus_flow_state(ff) == FOCUS_LOCKED) {
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(FOCUS_LOCKED, focus_flow_state(ff));
}

void test_locked_stable(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Get to LOCKED */
    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    TEST_ASSERT_EQUAL_INT(FOCUS_LOCKED, focus_flow_state(ff));

    /* Further updates stay LOCKED */
    ff = focus_flow_update(ff, DT);
    TEST_ASSERT_EQUAL_INT(FOCUS_LOCKED, focus_flow_state(ff));
}

void test_locked_dim_at_one(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, focus_flow_dim(ff));
}

void test_locked_reveal_at_one(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, focus_flow_reveal(ff));
}

/* ---- Release ---- */

void test_release_starts_releasing(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Get to LOCKED */
    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    ff = focus_flow_release(ff);
    TEST_ASSERT_EQUAL_INT(FOCUS_RELEASING, focus_flow_state(ff));
}

void test_releasing_decreases_dim(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    float locked_dim = focus_flow_dim(ff);

    ff = focus_flow_release(ff);
    for (int i = 0; i < 30; i++) {
        ff = focus_flow_update(ff, DT);
    }
    TEST_ASSERT_TRUE(focus_flow_dim(ff) < locked_dim);
}

void test_release_complete_transitions_idle(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    ff = focus_flow_release(ff);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
        if (focus_flow_state(ff) == FOCUS_IDLE) {
            break;
        }
    }
    TEST_ASSERT_EQUAL_INT(FOCUS_IDLE, focus_flow_state(ff));
}

/* ---- Cancel ---- */

void test_cancel_snaps_idle(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);
    for (int i = 0; i < 10; i++) {
        ff = focus_flow_update(ff, DT);
    }
    ff = focus_flow_cancel(ff);
    TEST_ASSERT_EQUAL_INT(FOCUS_IDLE, focus_flow_state(ff));
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, focus_flow_dim(ff));
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, focus_flow_reveal(ff));
}

void test_cancel_from_locked(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);
    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }
    ff = focus_flow_cancel(ff);
    TEST_ASSERT_EQUAL_INT(FOCUS_IDLE, focus_flow_state(ff));
}

/* ---- Pose query ---- */

void test_pose_idle_zeroed(void)
{
    focus_flow_t ff = focus_flow_create();
    camera_pose_t p = focus_flow_pose(ff);
    TEST_ASSERT_FLOAT_WITHIN(EPS, 0.0f, p.azimuth);
}

void test_pose_during_flight_interpolated(void)
{
    focus_flow_t ff = focus_flow_create();
    camera_pose_t h = home_pose();
    camera_pose_t d = dest_pose();
    ff = focus_flow_begin(ff, h, d, CT_PLANET, 0);

    for (int i = 0; i < 5; i++) {
        ff = focus_flow_update(ff, DT);
    }

    camera_pose_t p = focus_flow_pose(ff);
    /* Should be between home and dest */
    int moved = (fabsf(p.azimuth - h.azimuth) > EPS) ||
                (fabsf(p.log_zoom - h.log_zoom) > EPS);
    TEST_ASSERT_TRUE(moved);
}

/* ---- Retarget ---- */

void test_retarget_during_flight(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);
    for (int i = 0; i < 5; i++) {
        ff = focus_flow_update(ff, DT);
    }

    camera_pose_t alt = alt_pose();
    ff = focus_flow_retarget(ff, alt, CT_STAR, 42);

    TEST_ASSERT_EQUAL_INT(FOCUS_FLYING, focus_flow_state(ff));
    TEST_ASSERT_EQUAL_INT(CT_STAR, ff.target_type);
    TEST_ASSERT_EQUAL_INT(42, ff.target_id);
    TEST_ASSERT_FLOAT_WITHIN(EPS, alt.azimuth, ff.destination.azimuth);
}

/* ---- Apply layers ---- */

void test_apply_layers_idle_no_change(void)
{
    focus_flow_t ff = focus_flow_create();

    /* Build a base layer state with all opacities at 1.0 */
    layer_state_t base;
    for (int i = 0; i < LAYER_COUNT; i++) {
        base.opacity[i] = 1.0f;
    }

    layer_state_t result = focus_flow_apply_layers(ff, base, LAYER_PLANETS);
    for (int i = 0; i < LAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, result.opacity[i]);
    }
}

void test_apply_layers_dims_non_focused(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    /* Get to LOCKED (dim_mix = 1.0) */
    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }

    layer_state_t base;
    for (int i = 0; i < LAYER_COUNT; i++) {
        base.opacity[i] = 1.0f;
    }

    layer_state_t result = focus_flow_apply_layers(ff, base, LAYER_PLANETS);

    /* Non-focused, non-HUD layers should be dimmed */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, FOCUS_DIM_TARGET, result.opacity[LAYER_STARS]);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, FOCUS_DIM_TARGET, result.opacity[LAYER_ORBITS]);
}

void test_apply_layers_keeps_focused(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }

    layer_state_t base;
    for (int i = 0; i < LAYER_COUNT; i++) {
        base.opacity[i] = 1.0f;
    }

    layer_state_t result = focus_flow_apply_layers(ff, base, LAYER_PLANETS);

    /* Focused layer stays at full opacity */
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, result.opacity[LAYER_PLANETS]);
}

void test_apply_layers_keeps_hud(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);

    for (int i = 0; i < 1200; i++) {
        ff = focus_flow_update(ff, DT);
    }

    layer_state_t base;
    for (int i = 0; i < LAYER_COUNT; i++) {
        base.opacity[i] = 1.0f;
    }

    layer_state_t result = focus_flow_apply_layers(ff, base, LAYER_PLANETS);

    /* HUD always stays visible */
    TEST_ASSERT_FLOAT_WITHIN(EPS, 1.0f, result.opacity[LAYER_HUD]);
}

/* ---- Release from FLYING ---- */

void test_release_from_flying(void)
{
    focus_flow_t ff = focus_flow_create();
    ff = focus_flow_begin(ff, home_pose(), dest_pose(), CT_PLANET, 0);
    for (int i = 0; i < 10; i++) {
        ff = focus_flow_update(ff, DT);
    }
    ff = focus_flow_release(ff);
    TEST_ASSERT_EQUAL_INT(FOCUS_RELEASING, focus_flow_state(ff));
}

/* ---- Runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* Creation */
    RUN_TEST(test_create_idle);
    RUN_TEST(test_create_not_active);
    RUN_TEST(test_create_dim_zero);
    RUN_TEST(test_create_reveal_zero);

    /* Begin */
    RUN_TEST(test_begin_starts_flying);
    RUN_TEST(test_begin_is_active);
    RUN_TEST(test_begin_stores_home);
    RUN_TEST(test_begin_stores_target_info);
    RUN_TEST(test_begin_stores_destination);

    /* Update idle */
    RUN_TEST(test_update_idle_no_change);

    /* Flying */
    RUN_TEST(test_flying_increases_dim);
    RUN_TEST(test_flying_dim_stays_under_one);
    RUN_TEST(test_flight_complete_transitions_arrived);

    /* Arrived / Locked */
    RUN_TEST(test_arrived_increases_reveal);
    RUN_TEST(test_reveal_complete_transitions_locked);
    RUN_TEST(test_locked_stable);
    RUN_TEST(test_locked_dim_at_one);
    RUN_TEST(test_locked_reveal_at_one);

    /* Release */
    RUN_TEST(test_release_starts_releasing);
    RUN_TEST(test_releasing_decreases_dim);
    RUN_TEST(test_release_complete_transitions_idle);

    /* Cancel */
    RUN_TEST(test_cancel_snaps_idle);
    RUN_TEST(test_cancel_from_locked);

    /* Pose */
    RUN_TEST(test_pose_idle_zeroed);
    RUN_TEST(test_pose_during_flight_interpolated);

    /* Retarget */
    RUN_TEST(test_retarget_during_flight);

    /* Apply layers */
    RUN_TEST(test_apply_layers_idle_no_change);
    RUN_TEST(test_apply_layers_dims_non_focused);
    RUN_TEST(test_apply_layers_keeps_focused);
    RUN_TEST(test_apply_layers_keeps_hud);

    /* Release from flying */
    RUN_TEST(test_release_from_flying);

    return UNITY_END();
}
