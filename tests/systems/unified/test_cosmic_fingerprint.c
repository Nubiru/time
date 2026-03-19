/* test_cosmic_fingerprint.c — Tests for multi-system identity fingerprint.
 * TDD: tests written first, implementation follows. */

#include "unity.h"
#include "systems/unified/cosmic_fingerprint.h"
#include "systems/unified/birth_profile.h"

#include <string.h>

/* Shared profile: 1990-06-15 (known Kin 196 = Yellow Rhythmic Warrior) */
static birth_profile_t s_profile;
static cf_fingerprint_t s_fp;

void setUp(void) {
    s_profile = bp_compute(1990, 6, 15);
    s_fp = cf_compute(&s_profile);
}

void tearDown(void) {}

/* ===== Computation ===== */

void test_compute_populates_birth_date(void) {
    TEST_ASSERT_EQUAL_INT(1990, s_fp.birth_year);
    TEST_ASSERT_EQUAL_INT(6, s_fp.birth_month);
    TEST_ASSERT_EQUAL_INT(15, s_fp.birth_day);
}

void test_compute_has_all_systems(void) {
    TEST_ASSERT_EQUAL_INT(CF_SYS_COUNT, s_fp.count);
}

void test_all_entries_available(void) {
    for (int i = 0; i < CF_SYS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(1, s_fp.entries[i].available);
    }
}

void test_available_count(void) {
    TEST_ASSERT_EQUAL_INT(CF_SYS_COUNT, cf_available_count(&s_fp));
}

/* ===== Entry Lookup ===== */

void test_entry_by_system(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(CF_SYS_TZOLKIN, e->system);
}

void test_entry_out_of_range(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_COUNT);
    TEST_ASSERT_NULL(e);
}

void test_entry_negative_index(void) {
    const cf_entry_t *e = cf_entry(&s_fp, (cf_system_t)-1);
    TEST_ASSERT_NULL(e);
}

/* ===== System Names ===== */

void test_system_name_gregorian(void) {
    TEST_ASSERT_EQUAL_STRING("Gregorian", cf_system_name(CF_SYS_GREGORIAN));
}

void test_system_name_tzolkin(void) {
    TEST_ASSERT_EQUAL_STRING("Tzolkin", cf_system_name(CF_SYS_TZOLKIN));
}

void test_system_name_astrology(void) {
    TEST_ASSERT_EQUAL_STRING("Astrology", cf_system_name(CF_SYS_ASTROLOGY));
}

void test_system_name_chinese(void) {
    TEST_ASSERT_EQUAL_STRING("Chinese", cf_system_name(CF_SYS_CHINESE));
}

void test_system_name_iching(void) {
    TEST_ASSERT_EQUAL_STRING("I Ching", cf_system_name(CF_SYS_ICHING));
}

void test_system_name_human_design(void) {
    TEST_ASSERT_EQUAL_STRING("Human Design", cf_system_name(CF_SYS_HUMAN_DESIGN));
}

void test_system_name_hebrew(void) {
    TEST_ASSERT_EQUAL_STRING("Hebrew", cf_system_name(CF_SYS_HEBREW));
}

void test_system_name_islamic(void) {
    TEST_ASSERT_EQUAL_STRING("Islamic", cf_system_name(CF_SYS_ISLAMIC));
}

void test_system_name_buddhist(void) {
    TEST_ASSERT_EQUAL_STRING("Buddhist", cf_system_name(CF_SYS_BUDDHIST));
}

void test_system_name_invalid(void) {
    TEST_ASSERT_NULL(cf_system_name(CF_SYS_COUNT));
}

/* ===== Headlines ===== */

void test_headline_gregorian_contains_date(void) {
    const char *h = cf_headline(&s_fp, CF_SYS_GREGORIAN);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_TRUE(strlen(h) > 0);
    /* Should contain "June" and "1990" */
    TEST_ASSERT_NOT_NULL(strstr(h, "June"));
    TEST_ASSERT_NOT_NULL(strstr(h, "1990"));
}

void test_headline_tzolkin_contains_seal(void) {
    const char *h = cf_headline(&s_fp, CF_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_TRUE(strlen(h) > 0);
    /* Must contain the seal name from birth_profile */
    TEST_ASSERT_NOT_NULL(strstr(h, s_profile.tzolkin.seal_name));
}

void test_headline_astrology_contains_sign(void) {
    const char *h = cf_headline(&s_fp, CF_SYS_ASTROLOGY);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_NOT_NULL(strstr(h, s_profile.astrology.sun_sign_name));
}

void test_headline_chinese_contains_animal(void) {
    const char *h = cf_headline(&s_fp, CF_SYS_CHINESE);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_NOT_NULL(strstr(h, s_profile.chinese.animal_name));
}

void test_headline_iching_contains_name(void) {
    const char *h = cf_headline(&s_fp, CF_SYS_ICHING);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_NOT_NULL(strstr(h, s_profile.iching.name));
}

void test_headline_unavailable_returns_empty(void) {
    const char *h = cf_headline(&s_fp, CF_SYS_COUNT);
    TEST_ASSERT_EQUAL_STRING("", h);
}

/* ===== Subtitles ===== */

void test_subtitle_tzolkin_has_kin(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_NOT_NULL(strstr(e->subtitle, "Kin"));
}

void test_subtitle_hebrew_has_year(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_TRUE(strlen(e->subtitle) > 0);
}

void test_subtitle_islamic_has_ah(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_ISLAMIC);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_NOT_NULL(strstr(e->subtitle, "AH"));
}

void test_subtitle_buddhist_has_be(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_BUDDHIST);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_NOT_NULL(strstr(e->subtitle, "BE"));
}

/* ===== Glyph IDs ===== */

void test_glyph_tzolkin_is_seal(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_TZOLKIN);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(s_profile.tzolkin.seal, e->glyph_id);
}

void test_glyph_astrology_is_sign(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_ASTROLOGY);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(s_profile.astrology.sun_sign, e->glyph_id);
}

void test_glyph_chinese_is_animal(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_CHINESE);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(s_profile.chinese.animal, e->glyph_id);
}

void test_glyph_iching_is_hexagram(void) {
    const cf_entry_t *e = cf_entry(&s_fp, CF_SYS_ICHING);
    TEST_ASSERT_NOT_NULL(e);
    TEST_ASSERT_EQUAL_INT(s_profile.iching.king_wen, e->glyph_id);
}

/* ===== Reveal Order ===== */

void test_reveal_order_returns_all(void) {
    cf_system_t order[CF_SYS_COUNT];
    int n = cf_reveal_order(&s_fp, order, CF_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(CF_SYS_COUNT, n);
}

void test_reveal_order_first_is_gregorian(void) {
    cf_system_t order[CF_SYS_COUNT];
    cf_reveal_order(&s_fp, order, CF_SYS_COUNT);
    TEST_ASSERT_EQUAL_INT(CF_SYS_GREGORIAN, order[0]);
}

void test_reveal_order_limited_output(void) {
    cf_system_t order[3];
    int n = cf_reveal_order(&s_fp, order, 3);
    TEST_ASSERT_EQUAL_INT(3, n);
}

void test_reveal_order_unique(void) {
    cf_system_t order[CF_SYS_COUNT];
    cf_reveal_order(&s_fp, order, CF_SYS_COUNT);
    /* Each system should appear exactly once */
    for (int i = 0; i < CF_SYS_COUNT; i++) {
        for (int j = i + 1; j < CF_SYS_COUNT; j++) {
            TEST_ASSERT_NOT_EQUAL(order[i], order[j]);
        }
    }
}

/* ===== Format Summary ===== */

void test_format_summary_nonzero(void) {
    char buf[CF_SUMMARY_MAX];
    int n = cf_format_summary(&s_fp, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
}

void test_format_summary_contains_systems(void) {
    char buf[CF_SUMMARY_MAX];
    cf_format_summary(&s_fp, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Tzolkin"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Astrology"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Chinese"));
}

void test_format_summary_small_buffer(void) {
    char buf[32];
    int n = cf_format_summary(&s_fp, buf, sizeof(buf));
    /* Should not overflow */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(n < 32);
}

/* ===== Different Birth Dates ===== */

void test_different_date_different_headline(void) {
    birth_profile_t p2 = bp_compute(2000, 1, 1);
    cf_fingerprint_t fp2 = cf_compute(&p2);
    /* Different birth date should produce different Tzolkin headline */
    const char *h1 = cf_headline(&s_fp, CF_SYS_TZOLKIN);
    const char *h2 = cf_headline(&fp2, CF_SYS_TZOLKIN);
    /* They should both be non-empty */
    TEST_ASSERT_TRUE(strlen(h1) > 0);
    TEST_ASSERT_TRUE(strlen(h2) > 0);
    /* Very likely different (different dates) */
    TEST_ASSERT_TRUE(strcmp(h1, h2) != 0);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* Computation */
    RUN_TEST(test_compute_populates_birth_date);
    RUN_TEST(test_compute_has_all_systems);
    RUN_TEST(test_all_entries_available);
    RUN_TEST(test_available_count);

    /* Entry lookup */
    RUN_TEST(test_entry_by_system);
    RUN_TEST(test_entry_out_of_range);
    RUN_TEST(test_entry_negative_index);

    /* System names */
    RUN_TEST(test_system_name_gregorian);
    RUN_TEST(test_system_name_tzolkin);
    RUN_TEST(test_system_name_astrology);
    RUN_TEST(test_system_name_chinese);
    RUN_TEST(test_system_name_iching);
    RUN_TEST(test_system_name_human_design);
    RUN_TEST(test_system_name_hebrew);
    RUN_TEST(test_system_name_islamic);
    RUN_TEST(test_system_name_buddhist);
    RUN_TEST(test_system_name_invalid);

    /* Headlines */
    RUN_TEST(test_headline_gregorian_contains_date);
    RUN_TEST(test_headline_tzolkin_contains_seal);
    RUN_TEST(test_headline_astrology_contains_sign);
    RUN_TEST(test_headline_chinese_contains_animal);
    RUN_TEST(test_headline_iching_contains_name);
    RUN_TEST(test_headline_unavailable_returns_empty);

    /* Subtitles */
    RUN_TEST(test_subtitle_tzolkin_has_kin);
    RUN_TEST(test_subtitle_hebrew_has_year);
    RUN_TEST(test_subtitle_islamic_has_ah);
    RUN_TEST(test_subtitle_buddhist_has_be);

    /* Glyph IDs */
    RUN_TEST(test_glyph_tzolkin_is_seal);
    RUN_TEST(test_glyph_astrology_is_sign);
    RUN_TEST(test_glyph_chinese_is_animal);
    RUN_TEST(test_glyph_iching_is_hexagram);

    /* Reveal order */
    RUN_TEST(test_reveal_order_returns_all);
    RUN_TEST(test_reveal_order_first_is_gregorian);
    RUN_TEST(test_reveal_order_limited_output);
    RUN_TEST(test_reveal_order_unique);

    /* Format summary */
    RUN_TEST(test_format_summary_nonzero);
    RUN_TEST(test_format_summary_contains_systems);
    RUN_TEST(test_format_summary_small_buffer);

    /* Different dates */
    RUN_TEST(test_different_date_different_headline);

    return UNITY_END();
}
