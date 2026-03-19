/* test_welcome_text.c -- Welcome Journey text content tests.
 * Tests personalized onboarding text generation for the welcome journey. */

#include "../unity/unity.h"
#include "../../src/ui/welcome_text.h"
#include "../../src/systems/unified/birth_profile.h"

#include <string.h>

static birth_profile_t s_profile;

void setUp(void)
{
    s_profile = bp_compute(1990, 6, 15);
}

void tearDown(void) {}

/* ===== Screen count ===== */

void test_screen_count(void)
{
    TEST_ASSERT_EQUAL_INT(10, WT_SCREEN_COUNT);
}

void test_reveal_screen_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, wt_reveal_screen_count());
}

/* ===== Screen titles ===== */

void test_intro_title(void)
{
    const char *t = wt_screen_title(WT_SCREEN_INTRO);
    TEST_ASSERT_NOT_NULL(t);
    TEST_ASSERT_NOT_NULL(strstr(t, "arrive"));
}

void test_intro_subtitle(void)
{
    const char *s = wt_screen_subtitle(WT_SCREEN_INTRO);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(strstr(s, "key"));
}

void test_location_title(void)
{
    const char *t = wt_screen_title(WT_SCREEN_LOCATION);
    TEST_ASSERT_NOT_NULL(t);
    TEST_ASSERT_NOT_NULL(strstr(t, "Earth"));
}

void test_summary_title(void)
{
    const char *t = wt_screen_title(WT_SCREEN_SUMMARY);
    TEST_ASSERT_NOT_NULL(t);
    TEST_ASSERT_NOT_NULL(strstr(t, "Welcome"));
}

/* ===== Screen names ===== */

void test_screen_name_intro(void)
{
    TEST_ASSERT_EQUAL_STRING("Intro", wt_screen_name(WT_SCREEN_INTRO));
}

void test_screen_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wt_screen_name(WT_SCREEN_COUNT));
}

void test_screen_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", wt_screen_name((wt_screen_t)-1));
}

/* ===== Is reveal screen ===== */

void test_is_reveal_intro(void)
{
    TEST_ASSERT_EQUAL_INT(0, wt_is_reveal_screen(WT_SCREEN_INTRO));
}

void test_is_reveal_tzolkin(void)
{
    TEST_ASSERT_EQUAL_INT(1, wt_is_reveal_screen(WT_SCREEN_REVEAL_TZOLKIN));
}

void test_is_reveal_astrology(void)
{
    TEST_ASSERT_EQUAL_INT(1, wt_is_reveal_screen(WT_SCREEN_REVEAL_ASTROLOGY));
}

void test_is_reveal_summary(void)
{
    TEST_ASSERT_EQUAL_INT(0, wt_is_reveal_screen(WT_SCREEN_SUMMARY));
}

void test_is_reveal_location(void)
{
    TEST_ASSERT_EQUAL_INT(0, wt_is_reveal_screen(WT_SCREEN_LOCATION));
}

/* ===== Reveal titles/subtitles not null ===== */

void test_reveal_titles_not_null(void)
{
    for (int s = WT_SCREEN_REVEAL_TZOLKIN; s <= WT_SCREEN_REVEAL_ASTROLOGY; s++) {
        const char *t = wt_screen_title((wt_screen_t)s);
        TEST_ASSERT_NOT_NULL(t);
        TEST_ASSERT_TRUE(strlen(t) > 0);
    }
}

void test_reveal_subtitles_not_null(void)
{
    for (int s = WT_SCREEN_REVEAL_TZOLKIN; s <= WT_SCREEN_REVEAL_ASTROLOGY; s++) {
        const char *sub = wt_screen_subtitle((wt_screen_t)s);
        TEST_ASSERT_NOT_NULL(sub);
        TEST_ASSERT_TRUE(strlen(sub) > 0);
    }
}

/* ===== Reveal text edge cases ===== */

void test_reveal_text_null_profile(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_TZOLKIN, NULL);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("", r);
}

void test_reveal_text_non_reveal_screen(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_INTRO, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("", r);
}

/* ===== Reveal text content ===== */

void test_reveal_tzolkin_contains_kin(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_TZOLKIN, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "Kin"));
}

void test_reveal_chinese_contains_year(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_CHINESE, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "Year"));
}

void test_reveal_hebrew_contains_hebrew(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_HEBREW, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "Hebrew"));
}

void test_reveal_islamic_contains_AH(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_ISLAMIC, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "AH"));
}

void test_reveal_buddhist_contains_BE(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_BUDDHIST, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "BE"));
}

void test_reveal_iching_contains_hexagram(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_ICHING, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "hexagram"));
}

void test_reveal_astrology_contains_sun(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_ASTROLOGY, &s_profile);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_NOT_NULL(strstr(r, "Sun"));
}

/* ===== Summary text ===== */

void test_summary_text_not_null(void)
{
    const char *s = wt_summary_text(&s_profile);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_summary_text_null_profile(void)
{
    const char *s = wt_summary_text(NULL);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_STRING("", s);
}

void test_summary_contains_traditions(void)
{
    const char *s = wt_summary_text(&s_profile);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(strstr(s, "tradition"));
}

/* ===== Additional coverage ===== */

void test_reveal_tzolkin_contains_castle(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_TZOLKIN, &s_profile);
    TEST_ASSERT_NOT_NULL(strstr(r, "Castle"));
}

void test_reveal_chinese_contains_polarity(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_CHINESE, &s_profile);
    /* Must contain either "Yang" or "Yin" */
    int has_yang = (strstr(r, "Yang") != NULL);
    int has_yin  = (strstr(r, "Yin") != NULL);
    TEST_ASSERT_TRUE(has_yang || has_yin);
}

void test_reveal_astrology_contains_degrees(void)
{
    const char *r = wt_reveal_text(WT_SCREEN_REVEAL_ASTROLOGY, &s_profile);
    TEST_ASSERT_NOT_NULL(strstr(r, "degrees"));
}

void test_summary_contains_arrival(void)
{
    const char *s = wt_summary_text(&s_profile);
    TEST_ASSERT_NOT_NULL(strstr(s, "arrival"));
}

void test_all_screen_titles_valid(void)
{
    for (int i = 0; i < WT_SCREEN_COUNT; i++) {
        const char *t = wt_screen_title((wt_screen_t)i);
        TEST_ASSERT_NOT_NULL(t);
        TEST_ASSERT_TRUE(strlen(t) > 0);
    }
}

void test_invalid_screen_title(void)
{
    const char *t = wt_screen_title(WT_SCREEN_COUNT);
    TEST_ASSERT_EQUAL_STRING("Unknown", t);
}

void test_invalid_screen_subtitle(void)
{
    const char *s = wt_screen_subtitle(WT_SCREEN_COUNT);
    TEST_ASSERT_EQUAL_STRING("", s);
}

/* ===== main runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Screen count */
    RUN_TEST(test_screen_count);
    RUN_TEST(test_reveal_screen_count);

    /* Titles */
    RUN_TEST(test_intro_title);
    RUN_TEST(test_intro_subtitle);
    RUN_TEST(test_location_title);
    RUN_TEST(test_summary_title);

    /* Screen names */
    RUN_TEST(test_screen_name_intro);
    RUN_TEST(test_screen_name_invalid);
    RUN_TEST(test_screen_name_negative);

    /* Is reveal */
    RUN_TEST(test_is_reveal_intro);
    RUN_TEST(test_is_reveal_tzolkin);
    RUN_TEST(test_is_reveal_astrology);
    RUN_TEST(test_is_reveal_summary);
    RUN_TEST(test_is_reveal_location);

    /* Reveal titles/subtitles */
    RUN_TEST(test_reveal_titles_not_null);
    RUN_TEST(test_reveal_subtitles_not_null);

    /* Reveal edge cases */
    RUN_TEST(test_reveal_text_null_profile);
    RUN_TEST(test_reveal_text_non_reveal_screen);

    /* Reveal content */
    RUN_TEST(test_reveal_tzolkin_contains_kin);
    RUN_TEST(test_reveal_chinese_contains_year);
    RUN_TEST(test_reveal_hebrew_contains_hebrew);
    RUN_TEST(test_reveal_islamic_contains_AH);
    RUN_TEST(test_reveal_buddhist_contains_BE);
    RUN_TEST(test_reveal_iching_contains_hexagram);
    RUN_TEST(test_reveal_astrology_contains_sun);

    /* Summary */
    RUN_TEST(test_summary_text_not_null);
    RUN_TEST(test_summary_text_null_profile);
    RUN_TEST(test_summary_contains_traditions);

    /* Additional coverage */
    RUN_TEST(test_reveal_tzolkin_contains_castle);
    RUN_TEST(test_reveal_chinese_contains_polarity);
    RUN_TEST(test_reveal_astrology_contains_degrees);
    RUN_TEST(test_summary_contains_arrival);
    RUN_TEST(test_all_screen_titles_valid);
    RUN_TEST(test_invalid_screen_title);
    RUN_TEST(test_invalid_screen_subtitle);

    return UNITY_END();
}
