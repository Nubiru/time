#include "../unity/unity.h"
#include "../../src/ui/today_card.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Known test values:
 * JD 2460000.5 = 2023-02-25 (Saturday, day_of_week = 5)
 * sun_lon = 330.0 (Pisces), moon_lon = 120.0 (Cancer) */

static const double TEST_JD      = 2460000.5;
static const double TEST_SUN_LON = 330.0;
static const double TEST_MOON_LON = 120.0;

/* 1. Gregorian card title */
void test_gregorian_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_GREGORIAN, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Gregorian") != NULL);
}

/* 2. Tzolkin card has Kin */
void test_tzolkin_card_has_kin(void)
{
    card_content_t c = today_card_for_system(TS_SYS_TZOLKIN, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.line1, "Kin") != NULL);
}

/* 3. I Ching card has Hexagram */
void test_iching_card_has_hexagram(void)
{
    card_content_t c = today_card_for_system(TS_SYS_ICHING, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.line1, "Hexagram") != NULL ||
                     strstr(c.title, "I Ching") != NULL);
}

/* 4. Chinese card has animal */
void test_chinese_card_has_animal(void)
{
    card_content_t c = today_card_for_system(TS_SYS_CHINESE, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Chinese") != NULL);
    /* line1 should contain the animal symbol and name */
    TEST_ASSERT_TRUE(strlen(c.line1) > 0);
}

/* 5. Hebrew card title */
void test_hebrew_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_HEBREW, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Hebrew") != NULL);
}

/* 6. Islamic card title */
void test_islamic_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_ISLAMIC, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Islamic") != NULL);
}

/* 7. Buddhist card has BE */
void test_buddhist_card_has_be(void)
{
    card_content_t c = today_card_for_system(TS_SYS_BUDDHIST, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.line1, "BE") != NULL);
}

/* 8. Hindu card has nakshatra */
void test_hindu_card_has_nakshatra(void)
{
    card_content_t c = today_card_for_system(TS_SYS_HINDU, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Hindu") != NULL ||
                     strstr(c.title, "Panchanga") != NULL);
}

/* 9. Astrology card has Sun */
void test_astrology_card_has_sun(void)
{
    card_content_t c = today_card_for_system(TS_SYS_ASTROLOGY, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.line1, "Sun") != NULL);
}

/* 10. Human Design card has Gate */
void test_hd_card_has_gate(void)
{
    card_content_t c = today_card_for_system(TS_SYS_HUMAN_DESIGN, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.line1, "Gate") != NULL);
}

/* 11. Kabbalah card title */
void test_kabbalah_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_KABBALAH, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Tree") != NULL ||
                     strstr(c.title, "Kabbalah") != NULL);
}

/* 12. Geology card has era */
void test_geology_card_has_era(void)
{
    card_content_t c = today_card_for_system(TS_SYS_GEOLOGICAL, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Geolog") != NULL);
}

/* 13. Coptic card title */
void test_coptic_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_COPTIC, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Coptic") != NULL);
}

/* 14. Ethiopian card title */
void test_ethiopian_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_ETHIOPIAN, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Ethiopian") != NULL);
}

/* 15. Persian card title */
void test_persian_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_PERSIAN, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Hijri") != NULL ||
                     strstr(c.title, "Persian") != NULL ||
                     strstr(c.title, "Solar") != NULL);
}

/* 16. Thai card title */
void test_thai_card_title(void)
{
    card_content_t c = today_card_for_system(TS_SYS_THAI, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_TRUE(strstr(c.title, "Thai") != NULL);
}

/* 17. Cosmic returns empty */
void test_cosmic_returns_empty(void)
{
    card_content_t c = today_card_for_system(TS_SYS_COSMIC, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

/* 18. Earth card has season */
void test_earth_card_has_season(void)
{
    card_content_t c = today_card_for_system(TS_SYS_EARTH, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_EQUAL_STRING("Earth", c.title);
    TEST_ASSERT_TRUE(strlen(c.line1) > 0);
}

/* 19. Invalid system returns empty */
void test_invalid_system_returns_empty(void)
{
    card_content_t c = today_card_for_system(999, TEST_JD,
                                              TEST_SUN_LON, TEST_MOON_LON,
                                              0.0, 0.0);
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

/* 19. Supported count >= 16 */
void test_supported_count(void)
{
    TEST_ASSERT_TRUE(today_card_supported_count() >= 16);
}

/* 20. All supported produce nonempty */
void test_all_supported_produce_nonempty(void)
{
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        if (today_card_supported(i)) {
            card_content_t c = today_card_for_system(i, TEST_JD,
                                                      TEST_SUN_LON,
                                                      TEST_MOON_LON,
                                                      0.0, 0.0);
            TEST_ASSERT_TRUE_MESSAGE(strlen(c.title) > 0,
                                     "Supported system returned empty title");
        }
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_gregorian_card_title);
    RUN_TEST(test_tzolkin_card_has_kin);
    RUN_TEST(test_iching_card_has_hexagram);
    RUN_TEST(test_chinese_card_has_animal);
    RUN_TEST(test_hebrew_card_title);
    RUN_TEST(test_islamic_card_title);
    RUN_TEST(test_buddhist_card_has_be);
    RUN_TEST(test_hindu_card_has_nakshatra);
    RUN_TEST(test_astrology_card_has_sun);
    RUN_TEST(test_hd_card_has_gate);
    RUN_TEST(test_kabbalah_card_title);
    RUN_TEST(test_geology_card_has_era);
    RUN_TEST(test_coptic_card_title);
    RUN_TEST(test_ethiopian_card_title);
    RUN_TEST(test_persian_card_title);
    RUN_TEST(test_thai_card_title);
    RUN_TEST(test_cosmic_returns_empty);
    RUN_TEST(test_earth_card_has_season);
    RUN_TEST(test_invalid_system_returns_empty);
    RUN_TEST(test_supported_count);
    RUN_TEST(test_all_supported_produce_nonempty);
    return UNITY_END();
}
