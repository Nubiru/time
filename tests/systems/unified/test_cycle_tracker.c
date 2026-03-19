/* test_cycle_tracker.c — Tests for personal cycle status tracker.
 * TDD: tests written first, implementation follows. */

#include "unity.h"
#include "systems/unified/cycle_tracker.h"
#include "systems/unified/birth_profile.h"
#include "math/julian.h"

#include <string.h>

/* Shared: born 1990-06-15, checking cycles on 2026-03-19 */
static birth_profile_t s_profile;
static ct_tracker_t s_tracker;
static double s_current_jd;

void setUp(void) {
    s_profile = bp_compute(1990, 6, 15);
    s_current_jd = gregorian_to_jd(2026, 3, 19);
    s_tracker = ct_compute(&s_profile, s_current_jd);
}

void tearDown(void) {}

/* ===== Basic Properties ===== */

void test_has_all_cycles(void) {
    TEST_ASSERT_EQUAL_INT(CT_CYCLE_COUNT, s_tracker.count);
}

void test_days_alive_positive(void) {
    TEST_ASSERT_TRUE(s_tracker.days_alive > 0);
    /* 1990-06-15 to 2026-03-19 ≈ 13061 days */
    TEST_ASSERT_INT_WITHIN(5, 13061, s_tracker.days_alive);
}

void test_age_years(void) {
    /* Born 1990, checking 2026, birthday June 15 hasn't passed yet in March */
    TEST_ASSERT_EQUAL_INT(35, s_tracker.age_years);
}

void test_days_until_birthday(void) {
    /* March 19 → June 15 = about 88 days */
    TEST_ASSERT_TRUE(s_tracker.days_until_birthday > 0);
    TEST_ASSERT_TRUE(s_tracker.days_until_birthday <= 366);
    TEST_ASSERT_INT_WITHIN(3, 88, s_tracker.days_until_birthday);
}

/* ===== Cycle Lookup ===== */

void test_cycle_by_id(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_WAVESPELL);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(CT_WAVESPELL, c->id);
}

void test_cycle_out_of_range(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_CYCLE_COUNT);
    TEST_ASSERT_NULL(c);
}

void test_cycle_null_tracker(void) {
    const ct_cycle_t *c = ct_cycle(NULL, CT_WAVESPELL);
    TEST_ASSERT_NULL(c);
}

/* ===== Wavespell Cycle (13 days) ===== */

void test_wavespell_name(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_WAVESPELL);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_STRING("Wavespell", c->name);
}

void test_wavespell_length(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_WAVESPELL);
    TEST_ASSERT_EQUAL_INT(13, c->cycle_length);
}

void test_wavespell_days_bounds(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_WAVESPELL);
    TEST_ASSERT_TRUE(c->days_elapsed >= 0);
    TEST_ASSERT_TRUE(c->days_elapsed < 13);
    TEST_ASSERT_TRUE(c->days_remaining >= 1);
    TEST_ASSERT_TRUE(c->days_remaining <= 13);
    TEST_ASSERT_EQUAL_INT(13, c->days_elapsed + c->days_remaining);
}

void test_wavespell_progress(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_WAVESPELL);
    TEST_ASSERT_TRUE(c->progress >= 0.0);
    TEST_ASSERT_TRUE(c->progress < 1.0);
}

void test_wavespell_value_nonempty(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_WAVESPELL);
    TEST_ASSERT_TRUE(strlen(c->value) > 0);
}

/* ===== Castle Cycle (52 days) ===== */

void test_castle_name(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_CASTLE);
    TEST_ASSERT_EQUAL_STRING("Castle", c->name);
}

void test_castle_length(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_CASTLE);
    TEST_ASSERT_EQUAL_INT(52, c->cycle_length);
}

void test_castle_days_bounds(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_CASTLE);
    TEST_ASSERT_TRUE(c->days_elapsed >= 0);
    TEST_ASSERT_TRUE(c->days_elapsed < 52);
    TEST_ASSERT_TRUE(c->days_remaining >= 1);
    TEST_ASSERT_TRUE(c->days_remaining <= 52);
    TEST_ASSERT_EQUAL_INT(52, c->days_elapsed + c->days_remaining);
}

/* ===== Tzolkin Round (260 days) ===== */

void test_tzolkin_round_name(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_TZOLKIN_ROUND);
    TEST_ASSERT_EQUAL_STRING("Tzolkin Round", c->name);
}

void test_tzolkin_round_length(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_TZOLKIN_ROUND);
    TEST_ASSERT_EQUAL_INT(260, c->cycle_length);
}

void test_tzolkin_round_bounds(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_TZOLKIN_ROUND);
    TEST_ASSERT_TRUE(c->days_elapsed >= 0);
    TEST_ASSERT_TRUE(c->days_elapsed < 260);
    TEST_ASSERT_TRUE(c->days_remaining >= 1);
    TEST_ASSERT_TRUE(c->days_remaining <= 260);
    TEST_ASSERT_EQUAL_INT(260, c->days_elapsed + c->days_remaining);
}

/* ===== Zodiac Transit (~30 days) ===== */

void test_zodiac_transit_name(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_ZODIAC_TRANSIT);
    TEST_ASSERT_EQUAL_STRING("Zodiac Transit", c->name);
}

void test_zodiac_transit_length(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_ZODIAC_TRANSIT);
    TEST_ASSERT_EQUAL_INT(30, c->cycle_length);
}

void test_zodiac_transit_bounds(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_ZODIAC_TRANSIT);
    TEST_ASSERT_TRUE(c->days_elapsed >= 0);
    TEST_ASSERT_TRUE(c->days_elapsed <= 30);
    TEST_ASSERT_TRUE(c->days_remaining >= 0);
    TEST_ASSERT_TRUE(c->days_remaining <= 30);
}

void test_zodiac_transit_value_nonempty(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_ZODIAC_TRANSIT);
    TEST_ASSERT_TRUE(strlen(c->value) > 0);
}

/* ===== Personal Year (365/366 days) ===== */

void test_personal_year_name(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_PERSONAL_YEAR);
    TEST_ASSERT_EQUAL_STRING("Personal Year", c->name);
}

void test_personal_year_length(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_PERSONAL_YEAR);
    /* Either 365 or 366 */
    TEST_ASSERT_TRUE(c->cycle_length == 365 || c->cycle_length == 366);
}

void test_personal_year_days_match_birthday(void) {
    const ct_cycle_t *c = ct_cycle(&s_tracker, CT_PERSONAL_YEAR);
    /* days_remaining should match days_until_birthday */
    TEST_ASSERT_EQUAL_INT(s_tracker.days_until_birthday, c->days_remaining);
}

/* ===== Transition Detection ===== */

void test_any_transition_soon_large_window(void) {
    /* With a 365-day window, at least one cycle should be near transition */
    TEST_ASSERT_EQUAL_INT(1, ct_any_transition_soon(&s_tracker, 365));
}

void test_transitions_soon_count(void) {
    int n = ct_transitions_soon(&s_tracker, 365);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n <= CT_CYCLE_COUNT);
}

void test_transitions_soon_tight_window(void) {
    /* Within 0 days: only cycles at exact transition */
    int n = ct_transitions_soon(&s_tracker, 0);
    TEST_ASSERT_TRUE(n >= 0);
}

/* ===== Cycle Names ===== */

void test_cycle_name_wavespell(void) {
    TEST_ASSERT_EQUAL_STRING("Wavespell", ct_cycle_name(CT_WAVESPELL));
}

void test_cycle_name_castle(void) {
    TEST_ASSERT_EQUAL_STRING("Castle", ct_cycle_name(CT_CASTLE));
}

void test_cycle_name_tzolkin_round(void) {
    TEST_ASSERT_EQUAL_STRING("Tzolkin Round", ct_cycle_name(CT_TZOLKIN_ROUND));
}

void test_cycle_name_zodiac(void) {
    TEST_ASSERT_EQUAL_STRING("Zodiac Transit", ct_cycle_name(CT_ZODIAC_TRANSIT));
}

void test_cycle_name_personal_year(void) {
    TEST_ASSERT_EQUAL_STRING("Personal Year", ct_cycle_name(CT_PERSONAL_YEAR));
}

void test_cycle_name_invalid(void) {
    TEST_ASSERT_NULL(ct_cycle_name(CT_CYCLE_COUNT));
}

/* ===== Different Dates ===== */

void test_next_day_advances_wavespell(void) {
    ct_tracker_t t1 = ct_compute(&s_profile, s_current_jd);
    ct_tracker_t t2 = ct_compute(&s_profile, s_current_jd + 1.0);
    const ct_cycle_t *c1 = ct_cycle(&t1, CT_WAVESPELL);
    const ct_cycle_t *c2 = ct_cycle(&t2, CT_WAVESPELL);
    /* Either elapsed advances by 1, or wavespell wraps (elapsed resets to 0) */
    if (c1->days_remaining > 1) {
        TEST_ASSERT_EQUAL_INT(c1->days_elapsed + 1, c2->days_elapsed);
    } else {
        TEST_ASSERT_EQUAL_INT(0, c2->days_elapsed);
    }
}

void test_days_alive_increases(void) {
    ct_tracker_t t2 = ct_compute(&s_profile, s_current_jd + 10.0);
    TEST_ASSERT_EQUAL_INT(s_tracker.days_alive + 10, t2.days_alive);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* Basic properties */
    RUN_TEST(test_has_all_cycles);
    RUN_TEST(test_days_alive_positive);
    RUN_TEST(test_age_years);
    RUN_TEST(test_days_until_birthday);

    /* Cycle lookup */
    RUN_TEST(test_cycle_by_id);
    RUN_TEST(test_cycle_out_of_range);
    RUN_TEST(test_cycle_null_tracker);

    /* Wavespell */
    RUN_TEST(test_wavespell_name);
    RUN_TEST(test_wavespell_length);
    RUN_TEST(test_wavespell_days_bounds);
    RUN_TEST(test_wavespell_progress);
    RUN_TEST(test_wavespell_value_nonempty);

    /* Castle */
    RUN_TEST(test_castle_name);
    RUN_TEST(test_castle_length);
    RUN_TEST(test_castle_days_bounds);

    /* Tzolkin Round */
    RUN_TEST(test_tzolkin_round_name);
    RUN_TEST(test_tzolkin_round_length);
    RUN_TEST(test_tzolkin_round_bounds);

    /* Zodiac Transit */
    RUN_TEST(test_zodiac_transit_name);
    RUN_TEST(test_zodiac_transit_length);
    RUN_TEST(test_zodiac_transit_bounds);
    RUN_TEST(test_zodiac_transit_value_nonempty);

    /* Personal Year */
    RUN_TEST(test_personal_year_name);
    RUN_TEST(test_personal_year_length);
    RUN_TEST(test_personal_year_days_match_birthday);

    /* Transition detection */
    RUN_TEST(test_any_transition_soon_large_window);
    RUN_TEST(test_transitions_soon_count);
    RUN_TEST(test_transitions_soon_tight_window);

    /* Cycle names */
    RUN_TEST(test_cycle_name_wavespell);
    RUN_TEST(test_cycle_name_castle);
    RUN_TEST(test_cycle_name_tzolkin_round);
    RUN_TEST(test_cycle_name_zodiac);
    RUN_TEST(test_cycle_name_personal_year);
    RUN_TEST(test_cycle_name_invalid);

    /* Different dates */
    RUN_TEST(test_next_day_advances_wavespell);
    RUN_TEST(test_days_alive_increases);

    return UNITY_END();
}
