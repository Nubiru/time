/* test_onboarding.c — Tests for Welcome Journey state machine.
 * TDD: RED phase first, then GREEN with implementation. */

#include "unity.h"
#include "systems/earth/onboarding.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Creation ===== */

void test_create_initial_state(void) {
    ob_flow_t f = ob_create();
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, f.engagement_time);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, f.state_timer);
    TEST_ASSERT_EQUAL_INT(0, f.invitation_count);
    TEST_ASSERT_EQUAL_INT(0, f.is_returning_user);
    TEST_ASSERT_EQUAL_INT(0, f.birth_date_entered);
    TEST_ASSERT_EQUAL_INT(0, f.birth_place_entered);
    TEST_ASSERT_EQUAL_INT(0, f.interests_entered);
    TEST_ASSERT_EQUAL_INT(0, f.onboarding_completed);
}

void test_create_returning(void) {
    ob_flow_t f = ob_create_returning();
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.is_returning_user);
    TEST_ASSERT_EQUAL_INT(1, f.onboarding_completed);
}

void test_default_timing(void) {
    ob_timing_t t = ob_default_timing();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, t.invitation_delay);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 8.0f, t.invitation_duration);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 300.0f, t.invitation_cooldown);
    TEST_ASSERT_EQUAL_INT(3, t.max_invitations);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, t.complete_duration);
}

/* ===== Invitation Logic ===== */

void test_no_invite_before_delay(void) {
    ob_flow_t f = ob_create();
    f.engagement_time = 30.0f;
    ob_timing_t t = ob_default_timing();
    TEST_ASSERT_EQUAL_INT(0, ob_should_invite(&f, &t));
}

void test_invite_after_delay(void) {
    ob_flow_t f = ob_create();
    f.engagement_time = 61.0f;
    ob_timing_t t = ob_default_timing();
    TEST_ASSERT_EQUAL_INT(1, ob_should_invite(&f, &t));
}

void test_no_invite_returning_user(void) {
    ob_flow_t f = ob_create_returning();
    f.engagement_time = 120.0f;
    ob_timing_t t = ob_default_timing();
    TEST_ASSERT_EQUAL_INT(0, ob_should_invite(&f, &t));
}

void test_no_invite_after_completed(void) {
    ob_flow_t f = ob_create();
    f.onboarding_completed = 1;
    f.engagement_time = 120.0f;
    ob_timing_t t = ob_default_timing();
    TEST_ASSERT_EQUAL_INT(0, ob_should_invite(&f, &t));
}

void test_no_invite_max_reached(void) {
    ob_flow_t f = ob_create();
    f.engagement_time = 1000.0f;
    f.invitation_count = 3;
    ob_timing_t t = ob_default_timing();
    TEST_ASSERT_EQUAL_INT(0, ob_should_invite(&f, &t));
}

void test_invite_cooldown(void) {
    ob_flow_t f = ob_create();
    f.invitation_count = 1;
    ob_timing_t t = ob_default_timing();

    /* Need 60 + 1*300 = 360 seconds for second invitation */
    f.engagement_time = 300.0f;
    TEST_ASSERT_EQUAL_INT(0, ob_should_invite(&f, &t));

    f.engagement_time = 361.0f;
    TEST_ASSERT_EQUAL_INT(1, ob_should_invite(&f, &t));
}

/* ===== State Transitions — Happy Path ===== */

void test_tick_triggers_invitation(void) {
    ob_flow_t f = ob_create();
    ob_timing_t t = ob_default_timing();

    /* Tick past the 60s delay */
    f = ob_process(f, OB_EVT_TICK, 61.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_INVITATION, f.state);
}

void test_invitation_tap_to_birth_date(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INVITATION;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_INVITATION_TAP, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_BIRTH_DATE, f.state);
}

void test_birth_date_entered(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_DATE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DATE_ENTERED, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_BIRTH_PLACE, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.birth_date_entered);
}

void test_birth_place_entered(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_PLACE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_PLACE_ENTERED, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_REVEAL, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.birth_place_entered);
}

void test_reveal_done(void) {
    ob_flow_t f = ob_create();
    f.state = OB_REVEAL;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_REVEAL_DONE, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_INTERESTS, f.state);
}

void test_interests_done(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INTERESTS;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_INTERESTS_DONE, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_COMPLETE, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.interests_entered);
}

void test_complete_auto_finish(void) {
    ob_flow_t f = ob_create();
    f.state = OB_COMPLETE;
    ob_timing_t t = ob_default_timing();

    /* Tick past the 3s complete_duration */
    f = ob_process(f, OB_EVT_TICK, 3.5f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.onboarding_completed);
}

/* ===== State Transitions — Skip/Dismiss ===== */

void test_invitation_timeout(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INVITATION;
    ob_timing_t t = ob_default_timing();

    /* Tick past 8s invitation_duration */
    f = ob_process(f, OB_EVT_TICK, 9.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.invitation_count);
}

void test_birth_date_skipped(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_DATE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DATE_SKIPPED, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
}

void test_birth_place_skipped(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_PLACE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_PLACE_SKIPPED, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_REVEAL, f.state);
}

void test_dismiss_from_birth_date(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_DATE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DISMISS, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
}

/* ===== Profile Tap ===== */

void test_profile_tap_direct(void) {
    ob_flow_t f = ob_create();
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_PROFILE_TAP, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_BIRTH_DATE, f.state);
}

void test_profile_tap_returning(void) {
    ob_flow_t f = ob_create_returning();
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_PROFILE_TAP, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
}

/* ===== Scene Dimming ===== */

void test_scene_dim_exploring(void) {
    ob_flow_t f = ob_create();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ob_scene_dim(&f));
}

void test_scene_dim_birth_date(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_DATE;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.618f, ob_scene_dim(&f));
}

void test_scene_dim_complete(void) {
    ob_flow_t f = ob_create();
    f.state = OB_COMPLETE;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, ob_scene_dim(&f));
}

/* ===== Edge Cases ===== */

void test_state_timer_resets(void) {
    ob_flow_t f = ob_create();
    ob_timing_t t = ob_default_timing();

    /* Accumulate some state_timer in INVITATION */
    f.state = OB_INVITATION;
    f = ob_process(f, OB_EVT_TICK, 3.0f, &t);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.0f, f.state_timer);

    /* Transition to BIRTH_DATE resets timer */
    f = ob_process(f, OB_EVT_INVITATION_TAP, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_BIRTH_DATE, f.state);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, f.state_timer);
}

void test_engagement_accumulates(void) {
    ob_flow_t f = ob_create();
    ob_timing_t t = ob_default_timing();

    /* Several small ticks before invitation threshold */
    f = ob_process(f, OB_EVT_TICK, 10.0f, &t);
    f = ob_process(f, OB_EVT_TICK, 15.0f, &t);
    f = ob_process(f, OB_EVT_TICK, 5.0f, &t);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, f.engagement_time);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
}

void test_state_name(void) {
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_EXPLORING));
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_INVITATION));
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_BIRTH_DATE));
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_BIRTH_PLACE));
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_REVEAL));
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_INTERESTS));
    TEST_ASSERT_NOT_NULL(ob_state_name(OB_COMPLETE));
    /* Verify specific names */
    TEST_ASSERT_EQUAL_STRING("Exploring", ob_state_name(OB_EXPLORING));
    TEST_ASSERT_EQUAL_STRING("Complete", ob_state_name(OB_COMPLETE));
}

/* ===== Additional Edge Cases ===== */

void test_invitation_dismiss_increments_count(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INVITATION;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DISMISS, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.invitation_count);
}

void test_complete_done_event(void) {
    ob_flow_t f = ob_create();
    f.state = OB_COMPLETE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_COMPLETE_DONE, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.onboarding_completed);
}

void test_dismiss_from_reveal_goes_to_interests(void) {
    ob_flow_t f = ob_create();
    f.state = OB_REVEAL;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DISMISS, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_INTERESTS, f.state);
}

void test_dismiss_from_interests_goes_to_complete(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INTERESTS;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DISMISS, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_COMPLETE, f.state);
    /* interests_entered stays 0 when dismissed */
    TEST_ASSERT_EQUAL_INT(0, f.interests_entered);
}

void test_ob_is_active_during_onboarding(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_DATE;
    TEST_ASSERT_EQUAL_INT(1, ob_is_active(&f));
}

void test_ob_is_active_false_when_exploring(void) {
    ob_flow_t f = ob_create();
    TEST_ASSERT_EQUAL_INT(0, ob_is_active(&f));
}

void test_ob_is_active_false_when_completed(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INTERESTS;
    f.onboarding_completed = 1;
    TEST_ASSERT_EQUAL_INT(0, ob_is_active(&f));
}

void test_ob_is_complete(void) {
    ob_flow_t f = ob_create();
    TEST_ASSERT_EQUAL_INT(0, ob_is_complete(&f));
    f.onboarding_completed = 1;
    TEST_ASSERT_EQUAL_INT(1, ob_is_complete(&f));
}

void test_scene_dim_invitation(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INVITATION;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, ob_scene_dim(&f));
}

void test_scene_dim_reveal(void) {
    ob_flow_t f = ob_create();
    f.state = OB_REVEAL;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.618f, ob_scene_dim(&f));
}

void test_scene_dim_interests(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INTERESTS;
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.618f, ob_scene_dim(&f));
}

void test_full_happy_path(void) {
    ob_flow_t f = ob_create();
    ob_timing_t t = ob_default_timing();

    /* Phase 1: Explore until invitation triggers */
    f = ob_process(f, OB_EVT_TICK, 61.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_INVITATION, f.state);

    /* Phase 2: Accept invitation */
    f = ob_process(f, OB_EVT_INVITATION_TAP, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_BIRTH_DATE, f.state);

    /* Phase 3: Enter birth date */
    f = ob_process(f, OB_EVT_DATE_ENTERED, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_BIRTH_PLACE, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.birth_date_entered);

    /* Phase 4: Enter birth place */
    f = ob_process(f, OB_EVT_PLACE_ENTERED, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_REVEAL, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.birth_place_entered);

    /* Phase 5: Reveal completes */
    f = ob_process(f, OB_EVT_REVEAL_DONE, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_INTERESTS, f.state);

    /* Phase 6: Select interests */
    f = ob_process(f, OB_EVT_INTERESTS_DONE, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_COMPLETE, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.interests_entered);

    /* Phase 7: Welcome home fades out */
    f = ob_process(f, OB_EVT_TICK, 3.5f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.onboarding_completed);
}

void test_dismiss_from_birth_place(void) {
    ob_flow_t f = ob_create();
    f.state = OB_BIRTH_PLACE;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_DISMISS, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
}

void test_profile_tap_after_completed(void) {
    ob_flow_t f = ob_create();
    f.onboarding_completed = 1;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_PROFILE_TAP, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
}

void test_invitation_timeout_explicit_event(void) {
    ob_flow_t f = ob_create();
    f.state = OB_INVITATION;
    ob_timing_t t = ob_default_timing();

    f = ob_process(f, OB_EVT_INVITATION_TIMEOUT, 0.0f, &t);
    TEST_ASSERT_EQUAL_INT(OB_EXPLORING, f.state);
    TEST_ASSERT_EQUAL_INT(1, f.invitation_count);
}

int main(void) {
    UNITY_BEGIN();

    /* Creation (3) */
    RUN_TEST(test_create_initial_state);
    RUN_TEST(test_create_returning);
    RUN_TEST(test_default_timing);

    /* Invitation Logic (6) */
    RUN_TEST(test_no_invite_before_delay);
    RUN_TEST(test_invite_after_delay);
    RUN_TEST(test_no_invite_returning_user);
    RUN_TEST(test_no_invite_after_completed);
    RUN_TEST(test_no_invite_max_reached);
    RUN_TEST(test_invite_cooldown);

    /* State Transitions — Happy Path (7) */
    RUN_TEST(test_tick_triggers_invitation);
    RUN_TEST(test_invitation_tap_to_birth_date);
    RUN_TEST(test_birth_date_entered);
    RUN_TEST(test_birth_place_entered);
    RUN_TEST(test_reveal_done);
    RUN_TEST(test_interests_done);
    RUN_TEST(test_complete_auto_finish);

    /* State Transitions — Skip/Dismiss (4) */
    RUN_TEST(test_invitation_timeout);
    RUN_TEST(test_birth_date_skipped);
    RUN_TEST(test_birth_place_skipped);
    RUN_TEST(test_dismiss_from_birth_date);

    /* Profile Tap (2) */
    RUN_TEST(test_profile_tap_direct);
    RUN_TEST(test_profile_tap_returning);

    /* Scene Dimming (3) */
    RUN_TEST(test_scene_dim_exploring);
    RUN_TEST(test_scene_dim_birth_date);
    RUN_TEST(test_scene_dim_complete);

    /* Edge Cases (3) */
    RUN_TEST(test_state_timer_resets);
    RUN_TEST(test_engagement_accumulates);
    RUN_TEST(test_state_name);

    /* Additional Edge Cases (12) */
    RUN_TEST(test_invitation_dismiss_increments_count);
    RUN_TEST(test_complete_done_event);
    RUN_TEST(test_dismiss_from_reveal_goes_to_interests);
    RUN_TEST(test_dismiss_from_interests_goes_to_complete);
    RUN_TEST(test_ob_is_active_during_onboarding);
    RUN_TEST(test_ob_is_active_false_when_exploring);
    RUN_TEST(test_ob_is_active_false_when_completed);
    RUN_TEST(test_ob_is_complete);
    RUN_TEST(test_scene_dim_invitation);
    RUN_TEST(test_scene_dim_reveal);
    RUN_TEST(test_scene_dim_interests);
    RUN_TEST(test_full_happy_path);
    RUN_TEST(test_dismiss_from_birth_place);
    RUN_TEST(test_profile_tap_after_completed);
    RUN_TEST(test_invitation_timeout_explicit_event);

    return UNITY_END();
}
