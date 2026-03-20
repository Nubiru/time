#include "../unity/unity.h"
#include "../../src/ui/card_data.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Tzolkin: 1-5 */

void test_tzolkin_title(void)
{
    card_content_t c = card_format_tzolkin(0, 1, 1, "Dragon");
    TEST_ASSERT_EQUAL_STRING("Tzolkin", c.title);
}

void test_tzolkin_kin_1(void)
{
    card_content_t c = card_format_tzolkin(0, 1, 1, "Dragon");
    TEST_ASSERT_TRUE(strstr(c.line1, "Kin 1") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Dragon") != NULL);
}

void test_tzolkin_kin_260(void)
{
    card_content_t c = card_format_tzolkin(19, 13, 260, "Sun");
    TEST_ASSERT_TRUE(strstr(c.line1, "260") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Sun") != NULL);
}

void test_tzolkin_null_seal_name(void)
{
    card_content_t c = card_format_tzolkin(0, 1, 1, NULL);
    TEST_ASSERT_TRUE(strlen(c.line3) > 0);
    TEST_ASSERT_TRUE(strstr(c.line3, "?") != NULL);
}

void test_tzolkin_detail_nonempty(void)
{
    card_content_t c = card_format_tzolkin(7, 12, 207, "Hand");
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
    TEST_ASSERT_TRUE(strstr(c.detail, "207") != NULL);
}

/* I Ching: 6-9 */

void test_iching_title(void)
{
    int lines[6] = {1, 1, 1, 1, 1, 1};
    card_content_t c = card_format_iching(1, "The Creative", "Heaven", "Heaven", lines);
    TEST_ASSERT_EQUAL_STRING("I Ching", c.title);
}

void test_iching_hex_1(void)
{
    int lines[6] = {1, 1, 1, 1, 1, 1};
    card_content_t c = card_format_iching(1, "The Creative", "Heaven", "Heaven", lines);
    TEST_ASSERT_TRUE(strstr(c.line1, "1") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Creative") != NULL);
}

void test_iching_hex_64(void)
{
    int lines[6] = {0, 1, 0, 1, 0, 1};
    card_content_t c = card_format_iching(64, "Before Completion", "Fire", "Water", lines);
    TEST_ASSERT_TRUE(strstr(c.line1, "64") != NULL);
}

void test_iching_null_lines(void)
{
    card_content_t c = card_format_iching(2, "The Receptive", "Earth", "Earth", NULL);
    TEST_ASSERT_TRUE(strlen(c.line3) > 0);
}

/* Chinese: 10-12 */

void test_chinese_title(void)
{
    card_content_t c = card_format_chinese("\xf0\x9f\x90\x89", "Wood", "Snake", "Yin", 5, 5);
    TEST_ASSERT_EQUAL_STRING("Chinese Year", c.title);
}

void test_chinese_content(void)
{
    card_content_t c = card_format_chinese("\xf0\x9f\x90\x89", "Wood", "Snake", "Yin", 5, 5);
    TEST_ASSERT_TRUE(strstr(c.line1, "Snake") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Wood") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Yin") != NULL);
}

void test_chinese_null_inputs(void)
{
    card_content_t c = card_format_chinese(NULL, NULL, NULL, NULL, 0, 0);
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strlen(c.line1) > 0);
}

/* Human Design: 13-16 */

void test_hd_title(void)
{
    card_content_t c = card_format_human_design(1, 3, 2, 5, "The Creative", "Self-Expression");
    TEST_ASSERT_EQUAL_STRING("Human Design", c.title);
}

void test_hd_gate_1(void)
{
    card_content_t c = card_format_human_design(1, 1, 2, 1, "The Creative", "Self-Expression");
    TEST_ASSERT_TRUE(strstr(c.line1, "1.1") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "2.1") != NULL);
}

void test_hd_gate_64(void)
{
    card_content_t c = card_format_human_design(64, 6, 63, 6, "Confusion", "Before Completion");
    TEST_ASSERT_TRUE(strstr(c.line1, "64.6") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Confusion") != NULL);
}

void test_hd_null_names(void)
{
    card_content_t c = card_format_human_design(17, 3, 18, 4, NULL, NULL);
    TEST_ASSERT_TRUE(strlen(c.line3) > 0);
}

/* Astrology: 17-19 */

void test_astrology_title(void)
{
    card_content_t c = card_format_astrology(0, 15.5, 3, 22.3, 7, 5.1);
    TEST_ASSERT_EQUAL_STRING("Astrology", c.title);
}

void test_astrology_signs(void)
{
    card_content_t c = card_format_astrology(0, 15.5, 3, 22.3, 7, 5.1);
    TEST_ASSERT_TRUE(strstr(c.line1, "Aries") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Cancer") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Scorpio") != NULL);
}

void test_astrology_invalid_sign(void)
{
    card_content_t c = card_format_astrology(99, 0.0, -1, 0.0, 12, 0.0);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

/* Purity: 20 */

void test_purity(void)
{
    card_content_t a = card_format_tzolkin(7, 12, 207, "Hand");
    card_content_t b = card_format_tzolkin(7, 12, 207, "Hand");
    TEST_ASSERT_EQUAL_STRING(a.title, b.title);
    TEST_ASSERT_EQUAL_STRING(a.line1, b.line1);
    TEST_ASSERT_EQUAL_STRING(a.detail, b.detail);
}

/* Buffer safety: 21-22 */

void test_long_name_no_overflow(void)
{
    char long_name[200];
    memset(long_name, 'A', 199);
    long_name[199] = '\0';
    card_content_t c = card_format_tzolkin(0, 1, 1, long_name);
    /* Should not crash, line3 should be truncated */
    TEST_ASSERT_TRUE(strlen(c.line3) < sizeof(c.line3));
}

void test_all_fields_null_terminated(void)
{
    card_content_t c = card_format_iching(1, "Test", "A", "B", NULL);
    TEST_ASSERT_TRUE(c.title[sizeof(c.title) - 1] == '\0' || strlen(c.title) < sizeof(c.title));
    TEST_ASSERT_TRUE(strlen(c.line1) < sizeof(c.line1));
    TEST_ASSERT_TRUE(strlen(c.line2) < sizeof(c.line2));
    TEST_ASSERT_TRUE(strlen(c.line3) < sizeof(c.line3));
    TEST_ASSERT_TRUE(strlen(c.detail) < sizeof(c.detail));
}

/* Extra: detail contains key data */

void test_astrology_detail(void)
{
    card_content_t c = card_format_astrology(4, 10.0, 8, 20.0, 0, 5.0);
    TEST_ASSERT_TRUE(strstr(c.detail, "Leo") != NULL);
    TEST_ASSERT_TRUE(strstr(c.detail, "Sagittarius") != NULL);
    TEST_ASSERT_TRUE(strstr(c.detail, "Aries") != NULL);
}

/* Hebrew: 24-25 */

void test_card_format_hebrew(void)
{
    card_content_t c = card_format_hebrew(5786, 7, 15, "Tishrei");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Hebrew") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Tishrei") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "15") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "5786") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_hebrew_null(void)
{
    card_content_t c = card_format_hebrew(5786, 1, 1, NULL);
    TEST_ASSERT_TRUE(strlen(c.line1) > 0);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

/* Islamic: 26-27 */

void test_card_format_islamic(void)
{
    card_content_t c = card_format_islamic(1448, 9, 1, "Ramadan");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Islamic") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Ramadan") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "AH") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "1448") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_islamic_null(void)
{
    card_content_t c = card_format_islamic(1448, 1, 1, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

/* Buddhist: 28-29 */

void test_card_format_buddhist(void)
{
    card_content_t c = card_format_buddhist(2569, 2, "Full Moon", 1.0);
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.line1, "BE") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "2569") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Full Moon") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_buddhist_no_observance(void)
{
    card_content_t c = card_format_buddhist(2569, 0, NULL, 0.45);
    TEST_ASSERT_TRUE(strstr(c.line2, "No observance") != NULL);
}

/* Hindu: 30-31 */

void test_card_format_hindu(void)
{
    card_content_t c = card_format_hindu(0, "Ashwini", 5, 2, "Vishkambha");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Hindu") != NULL ||
                     strstr(c.title, "Panchanga") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Ashwini") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Vishkambha") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_hindu_null(void)
{
    card_content_t c = card_format_hindu(0, NULL, 1, 0, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "?") != NULL);
}

/* Kabbalah: 32-33 */

void test_card_format_kabbalah(void)
{
    card_content_t c = card_format_kabbalah(1, "Keter", "Crown", 0, "Aleph");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Tree") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Keter") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Crown") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Path 0") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_kabbalah_no_path(void)
{
    card_content_t c = card_format_kabbalah(9, "Malkuth", "Kingdom", -1, NULL);
    TEST_ASSERT_TRUE(strlen(c.line3) == 0);
}

/* Geology: 34-35 */

void test_card_format_geology(void)
{
    card_content_t c = card_format_geology("Phanerozoic", "Cenozoic",
                                           "Quaternary", "Holocene");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Geolog") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Phanerozoic") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "Cenozoic") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Quaternary") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Holocene") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_geology_null(void)
{
    card_content_t c = card_format_geology(NULL, NULL, NULL, NULL);
    TEST_ASSERT_TRUE(strlen(c.line1) > 0);
}

/* Gregorian: 36-37 */

void test_card_format_gregorian(void)
{
    card_content_t c = card_format_gregorian(2026, 3, 19, 2, "Spring");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Gregorian") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Wed") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "2026-03-19") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line3, "Spring") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_gregorian_null_season(void)
{
    card_content_t c = card_format_gregorian(2026, 1, 1, 3, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Thu") != NULL);
    TEST_ASSERT_TRUE(strlen(c.line3) == 0);
}

/* Coptic: 38-39 */

void test_card_format_coptic(void)
{
    card_content_t c = card_format_coptic(1742, 7, 10, "Baramhat");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Coptic") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Baramhat") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "AM") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "1742") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_coptic_null(void)
{
    card_content_t c = card_format_coptic(1742, 1, 1, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

/* Ethiopian: 40-41 */

void test_card_format_ethiopian(void)
{
    card_content_t c = card_format_ethiopian(2018, 7, 10, "Megabit");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Ethiopian") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Megabit") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "2018") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_ethiopian_null(void)
{
    card_content_t c = card_format_ethiopian(2018, 1, 1, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

/* Persian: 42-43 */

void test_card_format_persian(void)
{
    card_content_t c = card_format_persian(1404, 12, 28, "Esfand");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Hijri") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Esfand") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "SH") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "1404") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_persian_null(void)
{
    card_content_t c = card_format_persian(1404, 1, 1, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

/* Thai: 44-45 */

void test_card_format_thai(void)
{
    card_content_t c = card_format_thai(2569, 3, 19, "Minakhom");
    TEST_ASSERT_TRUE(strlen(c.title) > 0);
    TEST_ASSERT_TRUE(strstr(c.title, "Thai") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "Minakhom") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "BE") != NULL);
    TEST_ASSERT_TRUE(strstr(c.line2, "2569") != NULL);
    TEST_ASSERT_TRUE(strlen(c.detail) > 0);
}

void test_card_format_thai_null(void)
{
    card_content_t c = card_format_thai(2569, 1, 1, NULL);
    TEST_ASSERT_TRUE(strstr(c.line1, "?") != NULL);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_tzolkin_title);
    RUN_TEST(test_tzolkin_kin_1);
    RUN_TEST(test_tzolkin_kin_260);
    RUN_TEST(test_tzolkin_null_seal_name);
    RUN_TEST(test_tzolkin_detail_nonempty);
    RUN_TEST(test_iching_title);
    RUN_TEST(test_iching_hex_1);
    RUN_TEST(test_iching_hex_64);
    RUN_TEST(test_iching_null_lines);
    RUN_TEST(test_chinese_title);
    RUN_TEST(test_chinese_content);
    RUN_TEST(test_chinese_null_inputs);
    RUN_TEST(test_hd_title);
    RUN_TEST(test_hd_gate_1);
    RUN_TEST(test_hd_gate_64);
    RUN_TEST(test_hd_null_names);
    RUN_TEST(test_astrology_title);
    RUN_TEST(test_astrology_signs);
    RUN_TEST(test_astrology_invalid_sign);
    RUN_TEST(test_purity);
    RUN_TEST(test_long_name_no_overflow);
    RUN_TEST(test_all_fields_null_terminated);
    RUN_TEST(test_astrology_detail);
    RUN_TEST(test_card_format_hebrew);
    RUN_TEST(test_card_format_hebrew_null);
    RUN_TEST(test_card_format_islamic);
    RUN_TEST(test_card_format_islamic_null);
    RUN_TEST(test_card_format_buddhist);
    RUN_TEST(test_card_format_buddhist_no_observance);
    RUN_TEST(test_card_format_hindu);
    RUN_TEST(test_card_format_hindu_null);
    RUN_TEST(test_card_format_kabbalah);
    RUN_TEST(test_card_format_kabbalah_no_path);
    RUN_TEST(test_card_format_geology);
    RUN_TEST(test_card_format_geology_null);
    RUN_TEST(test_card_format_gregorian);
    RUN_TEST(test_card_format_gregorian_null_season);
    RUN_TEST(test_card_format_coptic);
    RUN_TEST(test_card_format_coptic_null);
    RUN_TEST(test_card_format_ethiopian);
    RUN_TEST(test_card_format_ethiopian_null);
    RUN_TEST(test_card_format_persian);
    RUN_TEST(test_card_format_persian_null);
    RUN_TEST(test_card_format_thai);
    RUN_TEST(test_card_format_thai_null);
    return UNITY_END();
}
