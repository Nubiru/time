/* test_interest_profile.c — Tests for interest profile data module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/interest_profile.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== ip_default_profile ===== */

void test_default_profile_all_weights_zero(void) {
    ip_profile_t p = ip_default_profile();
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[i]);
    }
}

void test_default_profile_all_category_affinity_zero(void) {
    ip_profile_t p = ip_default_profile();
    for (int i = 0; i < IP_CATEGORY_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.category_affinity[i]);
    }
}

void test_default_profile_visibility_anonymous(void) {
    ip_profile_t p = ip_default_profile();
    TEST_ASSERT_EQUAL_INT(IP_VIS_ANONYMOUS, p.visibility);
}

void test_default_profile_activity_balanced(void) {
    ip_profile_t p = ip_default_profile();
    TEST_ASSERT_EQUAL_INT(IP_ACT_BALANCED, p.activity_level);
}

void test_default_profile_location_radius(void) {
    ip_profile_t p = ip_default_profile();
    TEST_ASSERT_TRUE(p.location_radius_km > 0.0);
}

/* ===== ip_interest_count ===== */

void test_interest_count_is_20(void) {
    TEST_ASSERT_EQUAL_INT(20, ip_interest_count());
}

void test_interest_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(IP_MAX_INTERESTS, ip_interest_count());
}

/* ===== ip_interest_get ===== */

void test_get_surfing_id(void) {
    ip_interest_t x = ip_interest_get(0);
    TEST_ASSERT_EQUAL_INT(0, x.id);
}

void test_get_surfing_name(void) {
    ip_interest_t x = ip_interest_get(0);
    TEST_ASSERT_EQUAL_STRING("Surfing", x.name);
}

void test_get_surfing_category(void) {
    ip_interest_t x = ip_interest_get(0);
    TEST_ASSERT_EQUAL_INT(IP_NATURE, x.category);
}

void test_get_surfing_time_system(void) {
    ip_interest_t x = ip_interest_get(0);
    TEST_ASSERT_EQUAL_INT(0, x.time_system);
}

void test_get_surfing_description(void) {
    ip_interest_t x = ip_interest_get(0);
    TEST_ASSERT_NOT_NULL(x.description);
    TEST_ASSERT_TRUE(strlen(x.description) > 0);
}

void test_get_stargazing(void) {
    ip_interest_t x = ip_interest_get(1);
    TEST_ASSERT_EQUAL_STRING("Stargazing", x.name);
    TEST_ASSERT_EQUAL_INT(IP_NATURE, x.category);
    TEST_ASSERT_EQUAL_INT(1, x.time_system);
}

void test_get_hiking(void) {
    ip_interest_t x = ip_interest_get(2);
    TEST_ASSERT_EQUAL_STRING("Hiking", x.name);
    TEST_ASSERT_EQUAL_INT(IP_NATURE, x.category);
    TEST_ASSERT_EQUAL_INT(2, x.time_system);
}

void test_get_climbing(void) {
    ip_interest_t x = ip_interest_get(3);
    TEST_ASSERT_EQUAL_STRING("Climbing", x.name);
    TEST_ASSERT_EQUAL_INT(IP_NATURE, x.category);
    TEST_ASSERT_EQUAL_INT(3, x.time_system);
}

void test_get_history(void) {
    ip_interest_t x = ip_interest_get(4);
    TEST_ASSERT_EQUAL_STRING("History", x.name);
    TEST_ASSERT_EQUAL_INT(IP_CULTURE, x.category);
    TEST_ASSERT_EQUAL_INT(4, x.time_system);
}

void test_get_art(void) {
    ip_interest_t x = ip_interest_get(5);
    TEST_ASSERT_EQUAL_STRING("Art", x.name);
    TEST_ASSERT_EQUAL_INT(IP_CULTURE, x.category);
    TEST_ASSERT_EQUAL_INT(-1, x.time_system);
}

void test_get_music(void) {
    ip_interest_t x = ip_interest_get(6);
    TEST_ASSERT_EQUAL_STRING("Music", x.name);
    TEST_ASSERT_EQUAL_INT(IP_CULTURE, x.category);
    TEST_ASSERT_EQUAL_INT(5, x.time_system);
}

void test_get_food(void) {
    ip_interest_t x = ip_interest_get(7);
    TEST_ASSERT_EQUAL_STRING("Food", x.name);
    TEST_ASSERT_EQUAL_INT(IP_CULTURE, x.category);
    TEST_ASSERT_EQUAL_INT(-1, x.time_system);
}

void test_get_meetups(void) {
    ip_interest_t x = ip_interest_get(8);
    TEST_ASSERT_EQUAL_STRING("Meetups", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SOCIAL, x.category);
    TEST_ASSERT_EQUAL_INT(-1, x.time_system);
}

void test_get_festivals(void) {
    ip_interest_t x = ip_interest_get(10);
    TEST_ASSERT_EQUAL_STRING("Festivals", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SOCIAL, x.category);
    TEST_ASSERT_EQUAL_INT(6, x.time_system);
}

void test_get_games(void) {
    ip_interest_t x = ip_interest_get(11);
    TEST_ASSERT_EQUAL_STRING("Games", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SOCIAL, x.category);
    TEST_ASSERT_EQUAL_INT(7, x.time_system);
}

void test_get_meditation(void) {
    ip_interest_t x = ip_interest_get(12);
    TEST_ASSERT_EQUAL_STRING("Meditation", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SPIRITUAL, x.category);
    TEST_ASSERT_EQUAL_INT(8, x.time_system);
}

void test_get_ceremony(void) {
    ip_interest_t x = ip_interest_get(13);
    TEST_ASSERT_EQUAL_STRING("Ceremony", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SPIRITUAL, x.category);
    TEST_ASSERT_EQUAL_INT(9, x.time_system);
}

void test_get_pilgrimage(void) {
    ip_interest_t x = ip_interest_get(14);
    TEST_ASSERT_EQUAL_STRING("Pilgrimage", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SPIRITUAL, x.category);
    TEST_ASSERT_EQUAL_INT(10, x.time_system);
}

void test_get_yoga(void) {
    ip_interest_t x = ip_interest_get(15);
    TEST_ASSERT_EQUAL_STRING("Yoga", x.name);
    TEST_ASSERT_EQUAL_INT(IP_SPIRITUAL, x.category);
    TEST_ASSERT_EQUAL_INT(11, x.time_system);
}

void test_get_lectures(void) {
    ip_interest_t x = ip_interest_get(16);
    TEST_ASSERT_EQUAL_STRING("Lectures", x.name);
    TEST_ASSERT_EQUAL_INT(IP_KNOWLEDGE, x.category);
    TEST_ASSERT_EQUAL_INT(12, x.time_system);
}

void test_get_museums(void) {
    ip_interest_t x = ip_interest_get(18);
    TEST_ASSERT_EQUAL_STRING("Museums", x.name);
    TEST_ASSERT_EQUAL_INT(IP_KNOWLEDGE, x.category);
    TEST_ASSERT_EQUAL_INT(13, x.time_system);
}

void test_get_reading(void) {
    ip_interest_t x = ip_interest_get(19);
    TEST_ASSERT_EQUAL_STRING("Reading", x.name);
    TEST_ASSERT_EQUAL_INT(IP_KNOWLEDGE, x.category);
    TEST_ASSERT_EQUAL_INT(14, x.time_system);
}

void test_get_invalid_negative(void) {
    ip_interest_t x = ip_interest_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, x.id);
}

void test_get_invalid_over_max(void) {
    ip_interest_t x = ip_interest_get(20);
    TEST_ASSERT_EQUAL_INT(-1, x.id);
}

void test_get_invalid_large(void) {
    ip_interest_t x = ip_interest_get(999);
    TEST_ASSERT_EQUAL_INT(-1, x.id);
}

/* ===== ip_interests_in_category ===== */

void test_nature_has_4_interests(void) {
    int ids[IP_MAX_INTERESTS];
    int n = ip_interests_in_category(IP_NATURE, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_nature_ids_are_0_to_3(void) {
    int ids[IP_MAX_INTERESTS];
    int n = ip_interests_in_category(IP_NATURE, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_EQUAL_INT(0, ids[0]);
    TEST_ASSERT_EQUAL_INT(1, ids[1]);
    TEST_ASSERT_EQUAL_INT(2, ids[2]);
    TEST_ASSERT_EQUAL_INT(3, ids[3]);
}

void test_culture_has_4_interests(void) {
    int ids[IP_MAX_INTERESTS];
    int n = ip_interests_in_category(IP_CULTURE, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_social_has_4_interests(void) {
    int ids[IP_MAX_INTERESTS];
    int n = ip_interests_in_category(IP_SOCIAL, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_spiritual_has_4_interests(void) {
    int ids[IP_MAX_INTERESTS];
    int n = ip_interests_in_category(IP_SPIRITUAL, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_knowledge_has_4_interests(void) {
    int ids[IP_MAX_INTERESTS];
    int n = ip_interests_in_category(IP_KNOWLEDGE, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(4, n);
}

void test_category_limited_output(void) {
    int ids[2];
    int n = ip_interests_in_category(IP_NATURE, ids, 2);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT(0, ids[0]);
    TEST_ASSERT_EQUAL_INT(1, ids[1]);
}

void test_all_categories_sum_to_20(void) {
    int ids[IP_MAX_INTERESTS];
    int total = 0;
    for (int c = 0; c < IP_CATEGORY_COUNT; c++) {
        total += ip_interests_in_category((ip_category_t)c, ids, IP_MAX_INTERESTS);
    }
    TEST_ASSERT_EQUAL_INT(20, total);
}

/* ===== ip_set_weight ===== */

void test_set_weight_updates_value(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 0.8);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, (float)p.weights[0]);
}

void test_set_weight_clamps_above_1(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 1.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)p.weights[0]);
}

void test_set_weight_clamps_below_0(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, -0.5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[0]);
}

void test_set_weight_invalid_id_no_change(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, -1, 0.5);
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[i]);
    }
}

void test_set_weight_over_max_no_change(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 20, 0.5);
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[i]);
    }
}

void test_set_weight_preserves_other_weights(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 0.9);
    p = ip_set_weight(p, 5, 0.3);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.9f, (float)p.weights[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, (float)p.weights[5]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[1]);
}

void test_set_weight_returns_by_value(void) {
    ip_profile_t p = ip_default_profile();
    ip_profile_t q = ip_set_weight(p, 0, 0.7);
    /* Original unchanged */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[0]);
    /* New one updated */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.7f, (float)q.weights[0]);
}

/* ===== ip_recompute_categories ===== */

void test_recompute_nature_category(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 1.0); /* Surfing */
    p = ip_set_weight(p, 1, 0.5); /* Stargazing */
    p = ip_recompute_categories(p);
    /* Nature affinity = average of nature weights = (1.0 + 0.5 + 0 + 0) / 4 = 0.375 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.375f, (float)p.category_affinity[IP_NATURE]);
}

void test_recompute_empty_profile(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_recompute_categories(p);
    for (int i = 0; i < IP_CATEGORY_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.category_affinity[i]);
    }
}

void test_recompute_full_nature(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 1.0);
    p = ip_set_weight(p, 1, 1.0);
    p = ip_set_weight(p, 2, 1.0);
    p = ip_set_weight(p, 3, 1.0);
    p = ip_recompute_categories(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)p.category_affinity[IP_NATURE]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.category_affinity[IP_CULTURE]);
}

void test_recompute_mixed_categories(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 0.8);  /* Nature: Surfing */
    p = ip_set_weight(p, 4, 0.6);  /* Culture: History */
    p = ip_set_weight(p, 12, 0.4); /* Spiritual: Meditation */
    p = ip_recompute_categories(p);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, (float)p.category_affinity[IP_NATURE]);      /* 0.8/4 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, (float)p.category_affinity[IP_CULTURE]);    /* 0.6/4 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, (float)p.category_affinity[IP_SPIRITUAL]);   /* 0.4/4 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.category_affinity[IP_SOCIAL]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.category_affinity[IP_KNOWLEDGE]);
}

/* ===== ip_active_interests ===== */

void test_active_interests_none_when_empty(void) {
    ip_profile_t p = ip_default_profile();
    int ids[IP_MAX_INTERESTS];
    int n = ip_active_interests(p, 0.1, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_active_interests_finds_set_weights(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 0.8);
    p = ip_set_weight(p, 5, 0.3);
    p = ip_set_weight(p, 12, 0.6);
    int ids[IP_MAX_INTERESTS];
    int n = ip_active_interests(p, 0.1, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(3, n);
}

void test_active_interests_threshold_filters(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 0.8);
    p = ip_set_weight(p, 5, 0.3);
    p = ip_set_weight(p, 12, 0.6);
    int ids[IP_MAX_INTERESTS];
    int n = ip_active_interests(p, 0.5, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_EQUAL_INT(2, n);
}

void test_active_interests_limited_output(void) {
    ip_profile_t p = ip_default_profile();
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        p = ip_set_weight(p, i, 1.0);
    }
    int ids[5];
    int n = ip_active_interests(p, 0.1, ids, 5);
    TEST_ASSERT_EQUAL_INT(5, n);
}

void test_active_interests_zero_threshold(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 3, 0.01);
    int ids[IP_MAX_INTERESTS];
    /* Threshold 0.0 should include anything > 0 */
    int n = ip_active_interests(p, 0.0, ids, IP_MAX_INTERESTS);
    TEST_ASSERT_TRUE(n >= 1);
}

/* ===== ip_match ===== */

void test_match_identical_profiles_high_score(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 0.8);
    a = ip_set_weight(a, 1, 0.6);
    a = ip_set_weight(a, 4, 0.4);
    a = ip_recompute_categories(a);

    ip_match_t m = ip_match(a, a);
    TEST_ASSERT_TRUE(m.score >= 0.8);
}

void test_match_empty_profiles_zero_score(void) {
    ip_profile_t a = ip_default_profile();
    ip_profile_t b = ip_default_profile();
    ip_match_t m = ip_match(a, b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)m.score);
}

void test_match_orthogonal_profiles_low_score(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 1.0);
    a = ip_recompute_categories(a);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 12, 1.0);
    b = ip_recompute_categories(b);

    ip_match_t m = ip_match(a, b);
    TEST_ASSERT_TRUE(m.score < 0.3);
}

void test_match_score_bounded_0_to_1(void) {
    ip_profile_t a = ip_default_profile();
    ip_profile_t b = ip_default_profile();
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        a = ip_set_weight(a, i, 1.0);
        b = ip_set_weight(b, i, 1.0);
    }
    a = ip_recompute_categories(a);
    b = ip_recompute_categories(b);

    ip_match_t m = ip_match(a, b);
    TEST_ASSERT_TRUE(m.score >= 0.0 && m.score <= 1.0);
}

void test_match_category_scores_populated(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 0.9);
    a = ip_set_weight(a, 1, 0.7);
    a = ip_recompute_categories(a);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 0, 0.8);
    b = ip_set_weight(b, 1, 0.5);
    b = ip_recompute_categories(b);

    ip_match_t m = ip_match(a, b);
    /* Nature category should have a score > 0 */
    TEST_ASSERT_TRUE(m.category_scores[IP_NATURE] > 0.0);
}

void test_match_top_shared_is_highest_common(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 0.3);
    a = ip_set_weight(a, 5, 0.9);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 0, 0.2);
    b = ip_set_weight(b, 5, 0.8);

    ip_match_t m = ip_match(a, b);
    /* min(0.9,0.8) = 0.8 for id 5; min(0.3,0.2) = 0.2 for id 0 */
    TEST_ASSERT_EQUAL_INT(5, m.top_shared);
}

void test_match_complementary_is_max_difference(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 1.0);
    a = ip_set_weight(a, 5, 0.1);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 0, 0.1);
    b = ip_set_weight(b, 5, 1.0);

    ip_match_t m = ip_match(a, b);
    /* |1.0 - 0.1| = 0.9 for id 0 and id 5 */
    TEST_ASSERT_TRUE(m.complementary == 0 || m.complementary == 5);
}

void test_match_one_empty_profile(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 1.0);
    a = ip_recompute_categories(a);

    ip_profile_t b = ip_default_profile();

    ip_match_t m = ip_match(a, b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)m.score);
}

void test_match_partial_overlap(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 0.9);
    a = ip_set_weight(a, 1, 0.7);
    a = ip_set_weight(a, 4, 0.5);
    a = ip_recompute_categories(a);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 0, 0.8);
    b = ip_set_weight(b, 2, 0.6);
    b = ip_set_weight(b, 4, 0.4);
    b = ip_recompute_categories(b);

    ip_match_t m = ip_match(a, b);
    TEST_ASSERT_TRUE(m.score > 0.0);
    TEST_ASSERT_TRUE(m.score < 1.0);
}

void test_match_symmetric(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 0, 0.9);
    a = ip_set_weight(a, 5, 0.5);
    a = ip_recompute_categories(a);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 0, 0.6);
    b = ip_set_weight(b, 12, 0.7);
    b = ip_recompute_categories(b);

    ip_match_t m1 = ip_match(a, b);
    ip_match_t m2 = ip_match(b, a);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)m1.score, (float)m2.score);
}

/* ===== ip_category_name ===== */

void test_category_name_nature(void) {
    TEST_ASSERT_EQUAL_STRING("Nature", ip_category_name(IP_NATURE));
}

void test_category_name_culture(void) {
    TEST_ASSERT_EQUAL_STRING("Culture", ip_category_name(IP_CULTURE));
}

void test_category_name_social(void) {
    TEST_ASSERT_EQUAL_STRING("Social", ip_category_name(IP_SOCIAL));
}

void test_category_name_spiritual(void) {
    TEST_ASSERT_EQUAL_STRING("Spiritual", ip_category_name(IP_SPIRITUAL));
}

void test_category_name_knowledge(void) {
    TEST_ASSERT_EQUAL_STRING("Knowledge", ip_category_name(IP_KNOWLEDGE));
}

/* ===== ip_visibility_name ===== */

void test_visibility_name_anonymous(void) {
    TEST_ASSERT_EQUAL_STRING("Anonymous", ip_visibility_name(IP_VIS_ANONYMOUS));
}

void test_visibility_name_pseudonym(void) {
    TEST_ASSERT_EQUAL_STRING("Pseudonym", ip_visibility_name(IP_VIS_PSEUDONYM));
}

void test_visibility_name_public(void) {
    TEST_ASSERT_EQUAL_STRING("Public", ip_visibility_name(IP_VIS_PUBLIC));
}

/* ===== ip_activity_name ===== */

void test_activity_name_introvert(void) {
    TEST_ASSERT_EQUAL_STRING("Introvert", ip_activity_name(IP_ACT_INTROVERT));
}

void test_activity_name_balanced(void) {
    TEST_ASSERT_EQUAL_STRING("Balanced", ip_activity_name(IP_ACT_BALANCED));
}

void test_activity_name_extrovert(void) {
    TEST_ASSERT_EQUAL_STRING("Extrovert", ip_activity_name(IP_ACT_EXTROVERT));
}

/* ===== ip_category_count ===== */

void test_category_count_is_5(void) {
    TEST_ASSERT_EQUAL_INT(5, ip_category_count());
}

void test_category_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(IP_CATEGORY_COUNT, ip_category_count());
}

void test_category_count_matches_enum(void) {
    TEST_ASSERT_EQUAL_INT(IP_CAT_COUNT, ip_category_count());
}

/* ===== ip_time_system_for ===== */

void test_time_system_for_surfing(void) {
    const char *name = ip_time_system_for(0);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Tidal Systems", name);
}

void test_time_system_for_stargazing(void) {
    const char *name = ip_time_system_for(1);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Star Field", name);
}

void test_time_system_for_art_returns_null(void) {
    /* Art has time_system = -1 */
    const char *name = ip_time_system_for(5);
    TEST_ASSERT_NULL(name);
}

void test_time_system_for_food_returns_null(void) {
    const char *name = ip_time_system_for(7);
    TEST_ASSERT_NULL(name);
}

void test_time_system_for_meetups_returns_null(void) {
    const char *name = ip_time_system_for(8);
    TEST_ASSERT_NULL(name);
}

void test_time_system_for_meditation(void) {
    const char *name = ip_time_system_for(12);
    TEST_ASSERT_NOT_NULL(name);
}

void test_time_system_for_reading(void) {
    const char *name = ip_time_system_for(19);
    TEST_ASSERT_NOT_NULL(name);
}

void test_time_system_for_invalid_negative(void) {
    const char *name = ip_time_system_for(-1);
    TEST_ASSERT_NULL(name);
}

void test_time_system_for_invalid_over_max(void) {
    const char *name = ip_time_system_for(20);
    TEST_ASSERT_NULL(name);
}

/* ===== Data integrity ===== */

void test_all_interests_have_names(void) {
    for (int i = 0; i < ip_interest_count(); i++) {
        ip_interest_t interest = ip_interest_get(i);
        TEST_ASSERT_NOT_NULL(interest.name);
        TEST_ASSERT_TRUE(strlen(interest.name) > 0);
    }
}

void test_all_interests_have_descriptions(void) {
    for (int i = 0; i < ip_interest_count(); i++) {
        ip_interest_t interest = ip_interest_get(i);
        TEST_ASSERT_NOT_NULL(interest.description);
        TEST_ASSERT_TRUE(strlen(interest.description) > 0);
    }
}

void test_all_interests_valid_category(void) {
    for (int i = 0; i < ip_interest_count(); i++) {
        ip_interest_t interest = ip_interest_get(i);
        TEST_ASSERT_TRUE(interest.category >= 0 && interest.category < IP_CAT_COUNT);
    }
}

void test_all_interests_sequential_ids(void) {
    for (int i = 0; i < ip_interest_count(); i++) {
        ip_interest_t interest = ip_interest_get(i);
        TEST_ASSERT_EQUAL_INT(i, interest.id);
    }
}

void test_time_system_values_in_range(void) {
    for (int i = 0; i < ip_interest_count(); i++) {
        ip_interest_t interest = ip_interest_get(i);
        TEST_ASSERT_TRUE(interest.time_system >= -1);
    }
}

/* ===== Edge cases ===== */

void test_set_weight_exact_zero(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.weights[0]);
}

void test_set_weight_exact_one(void) {
    ip_profile_t p = ip_default_profile();
    p = ip_set_weight(p, 0, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)p.weights[0]);
}

void test_match_single_shared_interest(void) {
    ip_profile_t a = ip_default_profile();
    a = ip_set_weight(a, 7, 1.0);
    a = ip_recompute_categories(a);

    ip_profile_t b = ip_default_profile();
    b = ip_set_weight(b, 7, 1.0);
    b = ip_recompute_categories(b);

    ip_match_t m = ip_match(a, b);
    TEST_ASSERT_TRUE(m.score > 0.5);
    TEST_ASSERT_EQUAL_INT(7, m.top_shared);
}

void test_match_category_scores_bounded(void) {
    ip_profile_t a = ip_default_profile();
    ip_profile_t b = ip_default_profile();
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        a = ip_set_weight(a, i, 1.0);
        b = ip_set_weight(b, i, 0.5);
    }
    a = ip_recompute_categories(a);
    b = ip_recompute_categories(b);

    ip_match_t m = ip_match(a, b);
    for (int c = 0; c < IP_CATEGORY_COUNT; c++) {
        TEST_ASSERT_TRUE(m.category_scores[c] >= 0.0);
        TEST_ASSERT_TRUE(m.category_scores[c] <= 1.0);
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Default profile */
    RUN_TEST(test_default_profile_all_weights_zero);
    RUN_TEST(test_default_profile_all_category_affinity_zero);
    RUN_TEST(test_default_profile_visibility_anonymous);
    RUN_TEST(test_default_profile_activity_balanced);
    RUN_TEST(test_default_profile_location_radius);

    /* Interest count */
    RUN_TEST(test_interest_count_is_20);
    RUN_TEST(test_interest_count_matches_define);

    /* Interest get: individual interests */
    RUN_TEST(test_get_surfing_id);
    RUN_TEST(test_get_surfing_name);
    RUN_TEST(test_get_surfing_category);
    RUN_TEST(test_get_surfing_time_system);
    RUN_TEST(test_get_surfing_description);
    RUN_TEST(test_get_stargazing);
    RUN_TEST(test_get_hiking);
    RUN_TEST(test_get_climbing);
    RUN_TEST(test_get_history);
    RUN_TEST(test_get_art);
    RUN_TEST(test_get_music);
    RUN_TEST(test_get_food);
    RUN_TEST(test_get_meetups);
    RUN_TEST(test_get_festivals);
    RUN_TEST(test_get_games);
    RUN_TEST(test_get_meditation);
    RUN_TEST(test_get_ceremony);
    RUN_TEST(test_get_pilgrimage);
    RUN_TEST(test_get_yoga);
    RUN_TEST(test_get_lectures);
    RUN_TEST(test_get_museums);
    RUN_TEST(test_get_reading);

    /* Interest get: bounds */
    RUN_TEST(test_get_invalid_negative);
    RUN_TEST(test_get_invalid_over_max);
    RUN_TEST(test_get_invalid_large);

    /* Interests in category */
    RUN_TEST(test_nature_has_4_interests);
    RUN_TEST(test_nature_ids_are_0_to_3);
    RUN_TEST(test_culture_has_4_interests);
    RUN_TEST(test_social_has_4_interests);
    RUN_TEST(test_spiritual_has_4_interests);
    RUN_TEST(test_knowledge_has_4_interests);
    RUN_TEST(test_category_limited_output);
    RUN_TEST(test_all_categories_sum_to_20);

    /* Set weight */
    RUN_TEST(test_set_weight_updates_value);
    RUN_TEST(test_set_weight_clamps_above_1);
    RUN_TEST(test_set_weight_clamps_below_0);
    RUN_TEST(test_set_weight_invalid_id_no_change);
    RUN_TEST(test_set_weight_over_max_no_change);
    RUN_TEST(test_set_weight_preserves_other_weights);
    RUN_TEST(test_set_weight_returns_by_value);

    /* Recompute categories */
    RUN_TEST(test_recompute_nature_category);
    RUN_TEST(test_recompute_empty_profile);
    RUN_TEST(test_recompute_full_nature);
    RUN_TEST(test_recompute_mixed_categories);

    /* Active interests */
    RUN_TEST(test_active_interests_none_when_empty);
    RUN_TEST(test_active_interests_finds_set_weights);
    RUN_TEST(test_active_interests_threshold_filters);
    RUN_TEST(test_active_interests_limited_output);
    RUN_TEST(test_active_interests_zero_threshold);

    /* Match */
    RUN_TEST(test_match_identical_profiles_high_score);
    RUN_TEST(test_match_empty_profiles_zero_score);
    RUN_TEST(test_match_orthogonal_profiles_low_score);
    RUN_TEST(test_match_score_bounded_0_to_1);
    RUN_TEST(test_match_category_scores_populated);
    RUN_TEST(test_match_top_shared_is_highest_common);
    RUN_TEST(test_match_complementary_is_max_difference);
    RUN_TEST(test_match_one_empty_profile);
    RUN_TEST(test_match_partial_overlap);
    RUN_TEST(test_match_symmetric);

    /* Category name */
    RUN_TEST(test_category_name_nature);
    RUN_TEST(test_category_name_culture);
    RUN_TEST(test_category_name_social);
    RUN_TEST(test_category_name_spiritual);
    RUN_TEST(test_category_name_knowledge);

    /* Visibility name */
    RUN_TEST(test_visibility_name_anonymous);
    RUN_TEST(test_visibility_name_pseudonym);
    RUN_TEST(test_visibility_name_public);

    /* Activity name */
    RUN_TEST(test_activity_name_introvert);
    RUN_TEST(test_activity_name_balanced);
    RUN_TEST(test_activity_name_extrovert);

    /* Category count */
    RUN_TEST(test_category_count_is_5);
    RUN_TEST(test_category_count_matches_define);
    RUN_TEST(test_category_count_matches_enum);

    /* Time system for */
    RUN_TEST(test_time_system_for_surfing);
    RUN_TEST(test_time_system_for_stargazing);
    RUN_TEST(test_time_system_for_art_returns_null);
    RUN_TEST(test_time_system_for_food_returns_null);
    RUN_TEST(test_time_system_for_meetups_returns_null);
    RUN_TEST(test_time_system_for_meditation);
    RUN_TEST(test_time_system_for_reading);
    RUN_TEST(test_time_system_for_invalid_negative);
    RUN_TEST(test_time_system_for_invalid_over_max);

    /* Data integrity */
    RUN_TEST(test_all_interests_have_names);
    RUN_TEST(test_all_interests_have_descriptions);
    RUN_TEST(test_all_interests_valid_category);
    RUN_TEST(test_all_interests_sequential_ids);
    RUN_TEST(test_time_system_values_in_range);

    /* Edge cases */
    RUN_TEST(test_set_weight_exact_zero);
    RUN_TEST(test_set_weight_exact_one);
    RUN_TEST(test_match_single_shared_interest);
    RUN_TEST(test_match_category_scores_bounded);

    return UNITY_END();
}
