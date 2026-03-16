/* test_neighbor_roulette.c — Tests for the neighbor roulette data module.
 * Track 48.3: Anonymous profile cards, matching, privacy, cooldown. */

#include "unity.h"
#include "systems/earth/neighbor_roulette.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- State init / enable / disable ---- */

void test_init_state_disabled(void) {
    nr_state_t s = nr_init();
    TEST_ASSERT_EQUAL_INT(0, s.enabled);
}

void test_init_state_zero_spins(void) {
    nr_state_t s = nr_init();
    TEST_ASSERT_EQUAL_INT(0, s.spins_today);
}

void test_init_state_no_last_spin(void) {
    nr_state_t s = nr_init();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.last_spin_jd);
}

void test_set_enabled(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    TEST_ASSERT_EQUAL_INT(1, s.enabled);
}

void test_set_disabled(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    s = nr_set_enabled(s, 0);
    TEST_ASSERT_EQUAL_INT(0, s.enabled);
}

/* ---- Pseudonym generation: Kin ---- */

void test_pseudonym_kin_valid(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_kin(6, 12, buf, NR_PSEUDONYM_MAX);  /* seal 6 = Hand, tone 12 = Crystal */
    TEST_ASSERT_EQUAL_INT(1, ok);
    /* Color from seal: seal 6 % 4 = 2 -> Blue. Tone 12 = Crystal. Seal 6 = Hand. */
    TEST_ASSERT_EQUAL_STRING("Blue Crystal Hand", buf);
}

void test_pseudonym_kin_seal_zero(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_kin(0, 1, buf, NR_PSEUDONYM_MAX);  /* seal 0 = Dragon, tone 1 = Magnetic */
    TEST_ASSERT_EQUAL_INT(1, ok);
    /* seal 0 % 4 = 0 -> Red */
    TEST_ASSERT_EQUAL_STRING("Red Magnetic Dragon", buf);
}

void test_pseudonym_kin_seal_19(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_kin(19, 13, buf, NR_PSEUDONYM_MAX);  /* seal 19 = Sun, tone 13 = Cosmic */
    TEST_ASSERT_EQUAL_INT(1, ok);
    /* seal 19 % 4 = 3 -> Yellow */
    TEST_ASSERT_EQUAL_STRING("Yellow Cosmic Sun", buf);
}

void test_pseudonym_kin_invalid_seal(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_kin(-1, 1, buf, NR_PSEUDONYM_MAX);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

void test_pseudonym_kin_invalid_tone(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_kin(0, 0, buf, NR_PSEUDONYM_MAX);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

void test_pseudonym_kin_small_buffer(void) {
    char buf[8];
    int ok = nr_pseudonym_kin(0, 1, buf, 8);
    /* Should not overflow, but may truncate — returns 0 if truncated */
    TEST_ASSERT_EQUAL_INT(0, ok);
}

/* ---- Pseudonym generation: Zodiac ---- */

void test_pseudonym_zodiac_aries(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_zodiac(0, buf, NR_PSEUDONYM_MAX);
    TEST_ASSERT_EQUAL_INT(1, ok);
    /* Should start with "Aries " */
    TEST_ASSERT_EQUAL_INT(0, strncmp(buf, "Aries ", 6));
}

void test_pseudonym_zodiac_pisces(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_zodiac(11, buf, NR_PSEUDONYM_MAX);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_EQUAL_INT(0, strncmp(buf, "Pisces ", 7));
}

void test_pseudonym_zodiac_invalid(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_zodiac(-1, buf, NR_PSEUDONYM_MAX);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

void test_pseudonym_zodiac_invalid_high(void) {
    char buf[NR_PSEUDONYM_MAX];
    int ok = nr_pseudonym_zodiac(12, buf, NR_PSEUDONYM_MAX);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

/* ---- Fuzzy distance ---- */

void test_fuzzy_distance_nearby(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_NEARBY, nr_fuzzy_distance(2.0));
}

void test_fuzzy_distance_in_city(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_IN_CITY, nr_fuzzy_distance(10.0));
}

void test_fuzzy_distance_in_region(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_IN_REGION, nr_fuzzy_distance(50.0));
}

void test_fuzzy_distance_far(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_FAR, nr_fuzzy_distance(200.0));
}

void test_fuzzy_distance_boundary_5(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_IN_CITY, nr_fuzzy_distance(5.0));
}

void test_fuzzy_distance_boundary_25(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_IN_REGION, nr_fuzzy_distance(25.0));
}

void test_fuzzy_distance_boundary_100(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_FAR, nr_fuzzy_distance(100.0));
}

void test_fuzzy_distance_zero(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_NEARBY, nr_fuzzy_distance(0.0));
}

void test_fuzzy_distance_negative(void) {
    TEST_ASSERT_EQUAL_INT(NR_DIST_NEARBY, nr_fuzzy_distance(-5.0));
}

/* ---- Distance name ---- */

void test_distance_name_nearby(void) {
    TEST_ASSERT_EQUAL_STRING("Nearby", nr_distance_name(NR_DIST_NEARBY));
}

void test_distance_name_in_city(void) {
    TEST_ASSERT_EQUAL_STRING("In City", nr_distance_name(NR_DIST_IN_CITY));
}

void test_distance_name_in_region(void) {
    TEST_ASSERT_EQUAL_STRING("In Region", nr_distance_name(NR_DIST_IN_REGION));
}

void test_distance_name_far(void) {
    TEST_ASSERT_EQUAL_STRING("Far Away", nr_distance_name(NR_DIST_FAR));
}

void test_distance_name_invalid(void) {
    const char *name = nr_distance_name((nr_distance_t)99);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("?", name);
}

/* ---- Card creation with consent levels ---- */

void test_card_consent_none(void) {
    ip_profile_t profile = ip_default_profile();
    nr_card_t card = nr_create_card(&profile, 6, 12, 3, PF_CONSENT_NONE);
    TEST_ASSERT_EQUAL_STRING("Anonymous Visitor", card.pseudonym);
    TEST_ASSERT_EQUAL_INT(-1, card.kin_seal);
    TEST_ASSERT_EQUAL_INT(-1, card.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(0, card.top_interest_count);
}

void test_card_consent_basic(void) {
    ip_profile_t profile = ip_default_profile();
    profile = ip_set_weight(profile, 0, 0.9);
    profile = ip_set_weight(profile, 3, 0.7);
    nr_card_t card = nr_create_card(&profile, 6, 12, 3, PF_CONSENT_BASIC);
    /* Zodiac pseudonym, no kin */
    TEST_ASSERT_EQUAL_INT(-1, card.kin_seal);
    TEST_ASSERT_EQUAL_INT(3, card.zodiac_sign);
    /* Should have zodiac-based pseudonym (starts with sign name) */
    TEST_ASSERT_EQUAL_INT(0, strncmp(card.pseudonym, "Cancer ", 7));
    /* Should have top interests */
    TEST_ASSERT_TRUE(card.top_interest_count > 0);
}

void test_card_consent_full(void) {
    ip_profile_t profile = ip_default_profile();
    profile = ip_set_weight(profile, 0, 0.9);
    nr_card_t card = nr_create_card(&profile, 6, 12, 3, PF_CONSENT_FULL);
    /* Kin pseudonym */
    TEST_ASSERT_EQUAL_STRING("Blue Crystal Hand", card.pseudonym);
    TEST_ASSERT_EQUAL_INT(6, card.kin_seal);
    TEST_ASSERT_EQUAL_INT(3, card.zodiac_sign);
    TEST_ASSERT_TRUE(card.top_interest_count > 0);
}

/* ---- Match scoring ---- */

void test_match_score_identical(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 1.0);
    a = ip_set_weight(a, 1, 0.5);
    double score = nr_match_score(&a, &a);
    TEST_ASSERT_TRUE(score > 0.5);
}

void test_match_score_disjoint(void) {
    ip_profile_t a = ip_default_profile();
    ip_profile_t b = ip_default_profile();
    /* Both all zeros — no shared interests */
    float score = (float)nr_match_score(&a, &b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, score);
}

void test_match_score_range(void) {
    ip_profile_t a = ip_default_profile();
    ip_profile_t b = ip_default_profile();
    a = ip_set_weight(a, 0, 1.0);
    b = ip_set_weight(b, 0, 0.5);
    double score = nr_match_score(&a, &b);
    TEST_ASSERT_TRUE(score >= 0.0);
    TEST_ASSERT_TRUE(score <= 1.0);
}

/* ---- Anonymize ---- */

void test_anonymize_consent_none(void) {
    nr_card_t card;
    memset(&card, 0, sizeof(card));
    card.kin_seal = 6;
    card.zodiac_sign = 3;
    card.top_interest_count = 2;
    card.top_interests[0] = 0;
    card.top_interests[1] = 1;
    snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Blue Crystal Hand");

    nr_card_t anon = nr_anonymize(card, PF_CONSENT_NONE);
    TEST_ASSERT_EQUAL_STRING("Anonymous Visitor", anon.pseudonym);
    TEST_ASSERT_EQUAL_INT(-1, anon.kin_seal);
    TEST_ASSERT_EQUAL_INT(-1, anon.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(0, anon.top_interest_count);
}

void test_anonymize_consent_basic(void) {
    nr_card_t card;
    memset(&card, 0, sizeof(card));
    card.kin_seal = 6;
    card.zodiac_sign = 3;
    card.top_interest_count = 2;
    snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Blue Crystal Hand");

    nr_card_t anon = nr_anonymize(card, PF_CONSENT_BASIC);
    /* Basic: strip kin, keep zodiac */
    TEST_ASSERT_EQUAL_INT(-1, anon.kin_seal);
    TEST_ASSERT_EQUAL_INT(3, anon.zodiac_sign);
    TEST_ASSERT_TRUE(anon.top_interest_count <= card.top_interest_count);
}

void test_anonymize_consent_full(void) {
    nr_card_t card;
    memset(&card, 0, sizeof(card));
    card.kin_seal = 6;
    card.zodiac_sign = 3;
    card.top_interest_count = 2;
    snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Blue Crystal Hand");

    nr_card_t anon = nr_anonymize(card, PF_CONSENT_FULL);
    /* Full: keep everything */
    TEST_ASSERT_EQUAL_INT(6, anon.kin_seal);
    TEST_ASSERT_EQUAL_INT(3, anon.zodiac_sign);
    TEST_ASSERT_EQUAL_INT(2, anon.top_interest_count);
}

/* ---- Cooldown ---- */

void test_can_spin_initial(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double now_jd = 2460000.5;
    TEST_ASSERT_EQUAL_INT(1, nr_can_spin(s, now_jd, 1));
}

void test_cannot_spin_disabled(void) {
    nr_state_t s = nr_init();
    double now_jd = 2460000.5;
    /* Disabled = can't spin regardless */
    TEST_ASSERT_EQUAL_INT(0, nr_can_spin(s, now_jd, 1));
}

void test_record_spin_increments(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double now_jd = 2460000.5;
    s = nr_record_spin(s, now_jd);
    TEST_ASSERT_EQUAL_INT(1, s.spins_today);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)now_jd, (float)s.last_spin_jd);
}

void test_cannot_spin_after_max(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double now_jd = 2460000.5;
    s = nr_record_spin(s, now_jd);
    /* max_spins_per_day = 1, already used 1 */
    TEST_ASSERT_EQUAL_INT(0, nr_can_spin(s, now_jd, 1));
}

void test_can_spin_next_day(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double day1 = 2460000.5;
    s = nr_record_spin(s, day1);
    /* Next day (floor changes) */
    double day2 = 2460001.5;
    TEST_ASSERT_EQUAL_INT(1, nr_can_spin(s, day2, 1));
}

void test_cooldown_remaining_zero_initially(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double now_jd = 2460000.5;
    float rem = (float)nr_cooldown_remaining(s, now_jd, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, rem);
}

void test_cooldown_remaining_after_spin(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double now_jd = 2460000.5;
    s = nr_record_spin(s, now_jd);
    double rem = nr_cooldown_remaining(s, now_jd, 1);
    /* Should be time until next day boundary */
    TEST_ASSERT_TRUE(rem > 0.0);
    TEST_ASSERT_TRUE(rem <= 1.0);  /* max 1 day */
}

void test_cooldown_resets_on_new_day(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    s = nr_record_spin(s, 2460000.5);
    float rem = (float)nr_cooldown_remaining(s, 2460001.5, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, rem);
}

/* ---- Spin: best match ---- */

void test_spin_no_neighbors(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    ip_profile_t my = ip_default_profile();
    nr_spin_result_t result = nr_spin(s, 2460000.5, NULL, 0, &my);
    TEST_ASSERT_EQUAL_INT(0, result.valid);
}

void test_spin_picks_best_match(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);

    nr_card_t neighbors[3];
    memset(neighbors, 0, sizeof(neighbors));
    neighbors[0].match_score = 0.3;
    snprintf(neighbors[0].pseudonym, NR_PSEUDONYM_MAX, "A");
    neighbors[1].match_score = 0.9;
    snprintf(neighbors[1].pseudonym, NR_PSEUDONYM_MAX, "B");
    neighbors[2].match_score = 0.5;
    snprintf(neighbors[2].pseudonym, NR_PSEUDONYM_MAX, "C");

    ip_profile_t my = ip_default_profile();
    nr_spin_result_t result = nr_spin(s, 2460000.5, neighbors, 3, &my);
    TEST_ASSERT_EQUAL_INT(1, result.valid);
    TEST_ASSERT_EQUAL_STRING("B", result.card.pseudonym);
}

void test_spin_cooldown_blocks(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    s = nr_record_spin(s, 2460000.5);

    nr_card_t neighbors[1];
    memset(neighbors, 0, sizeof(neighbors));
    neighbors[0].match_score = 0.8;
    snprintf(neighbors[0].pseudonym, NR_PSEUDONYM_MAX, "X");

    ip_profile_t my = ip_default_profile();
    /* Same day, max 1 spin — should be on cooldown */
    nr_spin_result_t result = nr_spin(s, 2460000.5, neighbors, 1, &my);
    TEST_ASSERT_EQUAL_INT(1, result.cooldown_active);
    TEST_ASSERT_EQUAL_INT(0, result.valid);
}

void test_spin_disabled_returns_invalid(void) {
    nr_state_t s = nr_init();
    /* Not enabled */

    nr_card_t neighbors[1];
    memset(neighbors, 0, sizeof(neighbors));
    neighbors[0].match_score = 0.8;

    ip_profile_t my = ip_default_profile();
    nr_spin_result_t result = nr_spin(s, 2460000.5, neighbors, 1, &my);
    TEST_ASSERT_EQUAL_INT(0, result.valid);
}

/* ---- Edge cases ---- */

void test_card_null_profile(void) {
    nr_card_t card = nr_create_card(NULL, 6, 12, 3, PF_CONSENT_FULL);
    TEST_ASSERT_EQUAL_STRING("Anonymous Visitor", card.pseudonym);
    TEST_ASSERT_EQUAL_INT(0, card.top_interest_count);
}

void test_spin_count_null_neighbors(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    ip_profile_t my = ip_default_profile();
    nr_spin_result_t result = nr_spin(s, 2460000.5, NULL, 5, &my);
    TEST_ASSERT_EQUAL_INT(0, result.valid);
}

void test_multiple_spins_per_day(void) {
    nr_state_t s = nr_init();
    s = nr_set_enabled(s, 1);
    double now = 2460000.5;
    s = nr_record_spin(s, now);
    s = nr_record_spin(s, now + 0.01);
    TEST_ASSERT_EQUAL_INT(2, s.spins_today);
    /* With max 3 per day, should still be able */
    TEST_ASSERT_EQUAL_INT(1, nr_can_spin(s, now + 0.02, 3));
    /* With max 2, should not */
    TEST_ASSERT_EQUAL_INT(0, nr_can_spin(s, now + 0.02, 2));
}

/* ---- main ---- */

int main(void) {
    UNITY_BEGIN();

    /* State init / enable / disable */
    RUN_TEST(test_init_state_disabled);
    RUN_TEST(test_init_state_zero_spins);
    RUN_TEST(test_init_state_no_last_spin);
    RUN_TEST(test_set_enabled);
    RUN_TEST(test_set_disabled);

    /* Pseudonym: kin */
    RUN_TEST(test_pseudonym_kin_valid);
    RUN_TEST(test_pseudonym_kin_seal_zero);
    RUN_TEST(test_pseudonym_kin_seal_19);
    RUN_TEST(test_pseudonym_kin_invalid_seal);
    RUN_TEST(test_pseudonym_kin_invalid_tone);
    RUN_TEST(test_pseudonym_kin_small_buffer);

    /* Pseudonym: zodiac */
    RUN_TEST(test_pseudonym_zodiac_aries);
    RUN_TEST(test_pseudonym_zodiac_pisces);
    RUN_TEST(test_pseudonym_zodiac_invalid);
    RUN_TEST(test_pseudonym_zodiac_invalid_high);

    /* Fuzzy distance */
    RUN_TEST(test_fuzzy_distance_nearby);
    RUN_TEST(test_fuzzy_distance_in_city);
    RUN_TEST(test_fuzzy_distance_in_region);
    RUN_TEST(test_fuzzy_distance_far);
    RUN_TEST(test_fuzzy_distance_boundary_5);
    RUN_TEST(test_fuzzy_distance_boundary_25);
    RUN_TEST(test_fuzzy_distance_boundary_100);
    RUN_TEST(test_fuzzy_distance_zero);
    RUN_TEST(test_fuzzy_distance_negative);

    /* Distance name */
    RUN_TEST(test_distance_name_nearby);
    RUN_TEST(test_distance_name_in_city);
    RUN_TEST(test_distance_name_in_region);
    RUN_TEST(test_distance_name_far);
    RUN_TEST(test_distance_name_invalid);

    /* Card creation */
    RUN_TEST(test_card_consent_none);
    RUN_TEST(test_card_consent_basic);
    RUN_TEST(test_card_consent_full);

    /* Match scoring */
    RUN_TEST(test_match_score_identical);
    RUN_TEST(test_match_score_disjoint);
    RUN_TEST(test_match_score_range);

    /* Anonymize */
    RUN_TEST(test_anonymize_consent_none);
    RUN_TEST(test_anonymize_consent_basic);
    RUN_TEST(test_anonymize_consent_full);

    /* Cooldown */
    RUN_TEST(test_can_spin_initial);
    RUN_TEST(test_cannot_spin_disabled);
    RUN_TEST(test_record_spin_increments);
    RUN_TEST(test_cannot_spin_after_max);
    RUN_TEST(test_can_spin_next_day);
    RUN_TEST(test_cooldown_remaining_zero_initially);
    RUN_TEST(test_cooldown_remaining_after_spin);
    RUN_TEST(test_cooldown_resets_on_new_day);

    /* Spin */
    RUN_TEST(test_spin_no_neighbors);
    RUN_TEST(test_spin_picks_best_match);
    RUN_TEST(test_spin_cooldown_blocks);
    RUN_TEST(test_spin_disabled_returns_invalid);

    /* Edge cases */
    RUN_TEST(test_card_null_profile);
    RUN_TEST(test_spin_count_null_neighbors);
    RUN_TEST(test_multiple_spins_per_day);

    return UNITY_END();
}
