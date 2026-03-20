/* test_birth_sky.c — Tests for birth_sky module.
 * Validates birth sky visual state, featured system selection,
 * and birth card formatting. */

#include "unity.h"
#include "render/birth_sky.h"
#include "systems/unified/today_summary.h"

#include <string.h>

/* Known date: 2000-07-26 (Kin 207 Blue Crystal Hand) */
static birth_profile_t s_profile;

void setUp(void) {
    s_profile = bp_compute(2000, 7, 26);
}

void tearDown(void) {}

/* --- 1. from_profile sets JD and active --- */
void test_birth_sky_from_known_date(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    TEST_ASSERT_TRUE(bs.active);
    TEST_ASSERT_FLOAT_WITHIN(1.0, s_profile.jd, bs.jd);
}

/* --- 2. Tzolkin always in featured --- */
void test_birth_sky_featured_includes_tzolkin(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    int found = 0;
    for (int i = 0; i < bs.featured_count; i++) {
        if (bs.featured[i] == TS_SYS_TZOLKIN) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- 3. Astrology always in featured --- */
void test_birth_sky_featured_includes_astrology(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    int found = 0;
    for (int i = 0; i < bs.featured_count; i++) {
        if (bs.featured[i] == TS_SYS_ASTROLOGY) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- 4. Chinese always in featured --- */
void test_birth_sky_featured_includes_chinese(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    int found = 0;
    for (int i = 0; i < bs.featured_count; i++) {
        if (bs.featured[i] == TS_SYS_CHINESE) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- 5. Featured count 3..6 --- */
void test_birth_sky_featured_count_range(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    TEST_ASSERT_GREATER_OR_EQUAL(3, bs.featured_count);
    TEST_ASSERT_LESS_OR_EQUAL(BS_MAX_FEATURED, bs.featured_count);
}

/* --- 6. Empty is inactive --- */
void test_birth_sky_empty_inactive(void) {
    birth_sky_t bs = birth_sky_empty();
    TEST_ASSERT_FALSE(bs.active);
}

/* --- 7. Empty is not valid --- */
void test_birth_sky_empty_not_valid(void) {
    birth_sky_t bs = birth_sky_empty();
    TEST_ASSERT_FALSE(birth_sky_is_valid(&bs));
}

/* --- 8. Valid after compute --- */
void test_birth_sky_valid_after_compute(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    TEST_ASSERT_TRUE(birth_sky_is_valid(&bs));
}

/* --- 9. Ring indices populated --- */
void test_birth_sky_ring_indices_set(void) {
    birth_sky_t bs = birth_sky_from_profile(&s_profile);
    int tz_idx = ring_today_index(&bs.ring_indices, TS_SYS_TZOLKIN);
    TEST_ASSERT_GREATER_OR_EQUAL(0, tz_idx);
}

/* --- 10. Tzolkin card title contains "Kin" --- */
void test_birth_card_tzolkin_title(void) {
    birth_card_t card = birth_card_tzolkin(&s_profile);
    TEST_ASSERT_NOT_NULL(strstr(card.title, "Kin"));
}

/* --- 11. Tzolkin card oracle has Guide --- */
void test_birth_card_tzolkin_oracle(void) {
    birth_card_t card = birth_card_tzolkin(&s_profile);
    TEST_ASSERT_NOT_NULL(strstr(card.line2, "Guide"));
}

/* --- 12. Astrology card title contains sign name --- */
void test_birth_card_astrology_title(void) {
    birth_card_t card = birth_card_astrology(&s_profile);
    /* Sun sign name should appear in title */
    TEST_ASSERT_NOT_NULL(strstr(card.title, "Sun"));
}

/* --- 13. Astrology card has HD gate --- */
void test_birth_card_astrology_hd_gate(void) {
    birth_card_t card = birth_card_astrology(&s_profile);
    TEST_ASSERT_NOT_NULL(strstr(card.line2, "Gate"));
}

/* --- 14. Chinese card title contains animal name --- */
void test_birth_card_chinese_title(void) {
    birth_card_t card = birth_card_chinese(&s_profile);
    /* Animal name from profile should appear in title */
    TEST_ASSERT_NOT_NULL(strstr(card.title, s_profile.chinese.animal_name));
}

/* --- 15. Chinese card line1 has "cycle" --- */
void test_birth_card_chinese_cycle(void) {
    birth_card_t card = birth_card_chinese(&s_profile);
    TEST_ASSERT_NOT_NULL(strstr(card.line1, "cycle"));
}

/* --- 16. I Ching card title has "Hexagram" --- */
void test_birth_card_iching_title(void) {
    birth_card_t card = birth_card_iching(&s_profile);
    TEST_ASSERT_NOT_NULL(strstr(card.title, "Hexagram"));
}

/* --- 17. birth_card_for_system with TS_SYS_TZOLKIN delegates --- */
void test_birth_card_for_system_tzolkin(void) {
    birth_card_t card = birth_card_for_system(&s_profile, TS_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(strstr(card.title, "Kin"));
}

/* --- 18. Unsupported system returns empty card --- */
void test_birth_card_for_system_unknown(void) {
    birth_card_t card = birth_card_for_system(&s_profile, -1);
    TEST_ASSERT_EQUAL_STRING("", card.title);
}

/* --- 19. Kabbalah returns empty card (not supported for birth) --- */
void test_birth_card_for_system_kabbalah_empty(void) {
    birth_card_t card = birth_card_for_system(&s_profile, TS_SYS_KABBALAH);
    TEST_ASSERT_EQUAL_STRING("", card.title);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_birth_sky_from_known_date);
    RUN_TEST(test_birth_sky_featured_includes_tzolkin);
    RUN_TEST(test_birth_sky_featured_includes_astrology);
    RUN_TEST(test_birth_sky_featured_includes_chinese);
    RUN_TEST(test_birth_sky_featured_count_range);
    RUN_TEST(test_birth_sky_empty_inactive);
    RUN_TEST(test_birth_sky_empty_not_valid);
    RUN_TEST(test_birth_sky_valid_after_compute);
    RUN_TEST(test_birth_sky_ring_indices_set);
    RUN_TEST(test_birth_card_tzolkin_title);
    RUN_TEST(test_birth_card_tzolkin_oracle);
    RUN_TEST(test_birth_card_astrology_title);
    RUN_TEST(test_birth_card_astrology_hd_gate);
    RUN_TEST(test_birth_card_chinese_title);
    RUN_TEST(test_birth_card_chinese_cycle);
    RUN_TEST(test_birth_card_iching_title);
    RUN_TEST(test_birth_card_for_system_tzolkin);
    RUN_TEST(test_birth_card_for_system_unknown);
    RUN_TEST(test_birth_card_for_system_kabbalah_empty);
    return UNITY_END();
}
