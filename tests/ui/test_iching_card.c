#include "../unity/unity.h"
#include "../../src/ui/iching_card.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Reference:
 * Hexagram 1 = Qian, Heaven/Heaven, all yang
 * Hexagram 2 = Kun, Earth/Earth, all yin
 * Hexagram 42 = Yi (Increase), Thunder/Wind
 * Hexagram 63 = Ji Ji, Water/Fire
 * Hexagram 64 = Wei Ji, Fire/Water
 *
 * JD 2451545.0 (J2000) -> iching_from_jd gives some hexagram
 */

#define JD_J2000 2451545.0

/* Display from JD: 1-3 */

void test_display_from_jd(void)
{
    iching_display_t d = iching_display_from_jd(JD_J2000);
    TEST_ASSERT_TRUE(strlen(d.title) > 0);
    TEST_ASSERT_TRUE(strlen(d.trigrams) > 0);
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
}

void test_display_from_jd_fields(void)
{
    iching_display_t d = iching_display_from_jd(JD_J2000);
    TEST_ASSERT_TRUE(strlen(d.nuclear_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.unicode_sym) > 0);
    TEST_ASSERT_TRUE(d.king_wen >= 1 && d.king_wen <= 64);
}

void test_display_various_jd(void)
{
    double jds[] = {2451545.0, 2451600.0, 2451700.0, 2460000.0, 2440000.0};
    for (int i = 0; i < 5; i++) {
        iching_display_t d = iching_display_from_jd(jds[i]);
        TEST_ASSERT_TRUE(strlen(d.title) > 0);
        TEST_ASSERT_TRUE(d.king_wen >= 1 && d.king_wen <= 64);
    }
}

/* Display from hexagram number: 4-7 */

void test_display_hex1_qian(void)
{
    iching_display_t d = iching_display_from_number(1);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Qian"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "1"));
    TEST_ASSERT_NOT_NULL(strstr(d.trigrams, "Heaven"));
}

void test_display_hex2_kun(void)
{
    iching_display_t d = iching_display_from_number(2);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Kun"));
    TEST_ASSERT_NOT_NULL(strstr(d.trigrams, "Earth"));
}

void test_display_hex42_yi(void)
{
    iching_display_t d = iching_display_from_number(42);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Yi"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "42"));
}

void test_display_hex64_wei_ji(void)
{
    iching_display_t d = iching_display_from_number(64);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Wei Ji"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "64"));
}

/* Title formatting: 8-9 */

void test_title_format(void)
{
    char buf[48];
    iching_fmt_title(1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Qian"));
}

void test_title_hex63(void)
{
    char buf[48];
    iching_fmt_title(63, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "63"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Ji Ji"));
}

/* Trigram formatting: 10-12 */

void test_trigrams_heaven_heaven(void)
{
    char buf[48];
    iching_fmt_trigrams(0, 0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Heaven"));
}

void test_trigrams_wind_thunder(void)
{
    char buf[48];
    iching_fmt_trigrams(4, 3, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Wind"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Thunder"));
}

void test_trigrams_water_fire(void)
{
    char buf[48];
    iching_fmt_trigrams(5, 2, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Water"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Fire"));
}

/* Nuclear line: 13-14 */

void test_nuclear_hex1(void)
{
    iching_display_t d = iching_display_from_number(1);
    /* Nuclear of hex 1 (all yang) = hex 1 */
    TEST_ASSERT_NOT_NULL(strstr(d.nuclear_line, "1"));
}

void test_nuclear_hex64(void)
{
    iching_display_t d = iching_display_from_number(64);
    TEST_ASSERT_TRUE(strlen(d.nuclear_line) > 0);
}

/* Unicode symbol: 15-16 */

void test_unicode_hex1(void)
{
    iching_display_t d = iching_display_from_number(1);
    TEST_ASSERT_TRUE(strlen(d.unicode_sym) > 0);
}

void test_unicode_hex42(void)
{
    iching_display_t d = iching_display_from_number(42);
    TEST_ASSERT_TRUE(strlen(d.unicode_sym) > 0);
}

/* Summary: 17-18 */

void test_summary_has_name(void)
{
    iching_display_t d = iching_display_from_number(42);
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Yi"));
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "42"));
}

void test_summary_has_trigrams(void)
{
    iching_display_t d = iching_display_from_number(1);
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Heaven"));
}

/* ASCII art: 19-20 */

void test_ascii_populated(void)
{
    iching_display_t d = iching_display_from_number(1);
    TEST_ASSERT_TRUE(strlen(d.ascii_art[0]) > 0);
    TEST_ASSERT_TRUE(strlen(d.ascii_art[5]) > 0);
}

void test_ascii_hex2_yin(void)
{
    iching_display_t d = iching_display_from_number(2);
    /* Hex 2 is all yin — each line should have a gap */
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_NOT_NULL(strstr(d.ascii_art[i], "   "));
    }
}

/* All fields + purity: 21-22 */

void test_all_fields_populated(void)
{
    iching_display_t d = iching_display_from_number(30);
    TEST_ASSERT_TRUE(strlen(d.title) > 0);
    TEST_ASSERT_TRUE(strlen(d.trigrams) > 0);
    TEST_ASSERT_TRUE(strlen(d.nuclear_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.unicode_sym) > 0);
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
    TEST_ASSERT_TRUE(d.king_wen == 30);
}

void test_purity(void)
{
    iching_display_t a = iching_display_from_number(42);
    iching_display_t b = iching_display_from_number(42);
    TEST_ASSERT_EQUAL_STRING(a.title, b.title);
    TEST_ASSERT_EQUAL_STRING(a.trigrams, b.trigrams);
    TEST_ASSERT_EQUAL_STRING(a.summary, b.summary);
    TEST_ASSERT_EQUAL_INT(a.king_wen, b.king_wen);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_display_from_jd);
    RUN_TEST(test_display_from_jd_fields);
    RUN_TEST(test_display_various_jd);
    RUN_TEST(test_display_hex1_qian);
    RUN_TEST(test_display_hex2_kun);
    RUN_TEST(test_display_hex42_yi);
    RUN_TEST(test_display_hex64_wei_ji);
    RUN_TEST(test_title_format);
    RUN_TEST(test_title_hex63);
    RUN_TEST(test_trigrams_heaven_heaven);
    RUN_TEST(test_trigrams_wind_thunder);
    RUN_TEST(test_trigrams_water_fire);
    RUN_TEST(test_nuclear_hex1);
    RUN_TEST(test_nuclear_hex64);
    RUN_TEST(test_unicode_hex1);
    RUN_TEST(test_unicode_hex42);
    RUN_TEST(test_summary_has_name);
    RUN_TEST(test_summary_has_trigrams);
    RUN_TEST(test_ascii_populated);
    RUN_TEST(test_ascii_hex2_yin);
    RUN_TEST(test_all_fields_populated);
    RUN_TEST(test_purity);
    return UNITY_END();
}
