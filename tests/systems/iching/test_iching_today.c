/* test_iching_today.c — Tests for I Ching "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test dates:
 *   JD 2451520.5 => (int)(2451520.0) = 2451520, 2451520 % 64 = 0 => hexagram 1
 *   JD 2451521.5 => (int)(2451521.0) = 2451521, 2451521 % 64 = 1 => hexagram 2
 *   JD 2451545.5 => (int)(2451545.0) = 2451545, 2451545 % 64 = 25 => hexagram 26
 *   JD 2460000.5 => used for general tests
 */

#include "../../unity/unity.h"
#include "../../../src/systems/iching/iching_today.h"
#include "../../../src/systems/iching/iching.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* === Known JD constants === */

/* Hexagram 1 (The Creative): all yang lines */
static const double JD_HEX1 = 2451520.5;

/* Hexagram 2 (The Receptive): all yin lines */
static const double JD_HEX2 = 2451521.5;

/* Hexagram 26 (Great Taming): JD 2451545.5 (J2000.0) */
static const double JD_HEX26 = 2451545.5;

/* General test date */
static const double JD_GENERAL = 2460000.5;

/* ===== ic_today — has_data and section_count ===== */

void test_today_has_data(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_today_king_wen_in_range(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_TRUE(p.king_wen >= 1);
    TEST_ASSERT_TRUE(p.king_wen <= 64);
}

/* ===== page_title ===== */

void test_title_contains_i_ching(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "I Ching"));
}

void test_title_contains_hexagram(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Hexagram"));
}

void test_title_hex1_contains_creative(void)
{
    ic_page_t p = ic_today(JD_HEX1);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "The Creative"));
}

void test_title_hex2_contains_receptive(void)
{
    ic_page_t p = ic_today(JD_HEX2);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "The Receptive"));
}

/* ===== king_wen field ===== */

void test_king_wen_hex1(void)
{
    ic_page_t p = ic_today(JD_HEX1);
    TEST_ASSERT_EQUAL_INT(1, p.king_wen);
}

void test_king_wen_hex2(void)
{
    ic_page_t p = ic_today(JD_HEX2);
    TEST_ASSERT_EQUAL_INT(2, p.king_wen);
}

void test_king_wen_hex26(void)
{
    ic_page_t p = ic_today(JD_HEX26);
    TEST_ASSERT_EQUAL_INT(26, p.king_wen);
}

/* ===== hexagram_section ===== */

void test_hexagram_section_contains_hexagram(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.hexagram_section, "Hexagram:"));
}

void test_hexagram_section_contains_element(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.hexagram_section, "Element:"));
}

void test_hexagram_section_contains_keywords(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.hexagram_section, "Keywords:"));
}

void test_hexagram_section_hex1_heaven(void)
{
    ic_page_t p = ic_today(JD_HEX1);
    TEST_ASSERT_NOT_NULL(strstr(p.hexagram_section, "Heaven"));
}

/* ===== trigram_section ===== */

void test_trigram_section_contains_upper(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.trigram_section, "Upper Trigram:"));
}

void test_trigram_section_contains_lower(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.trigram_section, "Lower Trigram:"));
}

void test_trigram_section_contains_reading(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.trigram_section, "Reading:"));
}

void test_trigram_section_contains_trigram_symbol(void)
{
    /* Hexagram 1: both trigrams are Heaven (index 0), symbol ☰ */
    ic_page_t p = ic_today(JD_HEX1);
    TEST_ASSERT_NOT_NULL(strstr(p.trigram_section, "\xe2\x98\xb0"));
}

void test_trigram_section_hex2_earth_symbol(void)
{
    /* Hexagram 2: both trigrams are Earth (index 7), symbol ☷ */
    ic_page_t p = ic_today(JD_HEX2);
    TEST_ASSERT_NOT_NULL(strstr(p.trigram_section, "\xe2\x98\xb7"));
}

/* ===== judgment_section ===== */

void test_judgment_section_contains_judgment(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.judgment_section, "Judgment"));
}

void test_judgment_section_hex1_has_text(void)
{
    ic_page_t p = ic_today(JD_HEX1);
    /* Hexagram 1 judgment: "Pure creative force initiates all things" */
    TEST_ASSERT_NOT_NULL(strstr(p.judgment_section, "creative"));
}

/* ===== image_section ===== */

void test_image_section_contains_image(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.image_section, "Image"));
}

void test_image_section_hex1_has_text(void)
{
    ic_page_t p = ic_today(JD_HEX1);
    /* Hexagram 1 image contains "heaven" */
    TEST_ASSERT_NOT_NULL(strstr(p.image_section, "heaven"));
}

/* ===== lines_section ===== */

void test_lines_section_contains_lines_header(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    TEST_ASSERT_NOT_NULL(strstr(p.lines_section, "Lines"));
}

void test_lines_section_hex1_all_yang(void)
{
    /* Hexagram 1: all 6 lines are yang (solid) */
    ic_page_t p = ic_today(JD_HEX1);
    /* Should contain yang line art but no yin line art */
    TEST_ASSERT_NOT_NULL(strstr(p.lines_section,
        "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81"
        "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81"
        "\xe2\x94\x81"));
    /* No broken yin lines */
    TEST_ASSERT_NULL(strstr(p.lines_section,
        "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81 "
        "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81"));
}

void test_lines_section_hex2_all_yin(void)
{
    /* Hexagram 2: all 6 lines are yin (broken) */
    ic_page_t p = ic_today(JD_HEX2);
    /* Should contain yin line art: "━━━ ━━━" */
    TEST_ASSERT_NOT_NULL(strstr(p.lines_section,
        "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81 "
        "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81"));
}

/* ===== Different JDs produce different hexagrams ===== */

void test_different_jds_different_hexagrams(void)
{
    ic_page_t p1 = ic_today(JD_HEX1);
    ic_page_t p2 = ic_today(JD_HEX2);
    TEST_ASSERT_NOT_EQUAL(p1.king_wen, p2.king_wen);
}

void test_adjacent_jds_differ(void)
{
    ic_page_t p1 = ic_today(JD_GENERAL);
    ic_page_t p2 = ic_today(JD_GENERAL + 1.0);
    TEST_ASSERT_NOT_EQUAL(p1.king_wen, p2.king_wen);
}

/* ===== ic_format ===== */

void test_format_produces_output(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    char buf[4096];
    int n = ic_format(&p, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void)
{
    ic_page_t p = ic_today(JD_HEX1);
    char buf[4096];
    ic_format(&p, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "I Ching"));
}

void test_format_contains_all_sections(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    char buf[4096];
    ic_format(&p, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hexagram:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Upper Trigram:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Judgment"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Image"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Lines"));
}

void test_format_null_page_returns_zero(void)
{
    char buf[256];
    int n = ic_format(NULL, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buf_returns_zero(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    int n = ic_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_zero_buf_size_returns_zero(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    char buf[16];
    int n = ic_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buf_truncates(void)
{
    ic_page_t p = ic_today(JD_GENERAL);
    char buf[32];
    int n = ic_format(&p, buf, sizeof(buf));
    /* Should write something but be truncated */
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n < 32);
}

void test_format_no_data_returns_zero(void)
{
    ic_page_t p;
    memset(&p, 0, sizeof(p));
    p.has_data = 0;
    char buf[256];
    int n = ic_format(&p, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

int main(void)
{
    UNITY_BEGIN();

    /* Basic fields */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);
    RUN_TEST(test_today_king_wen_in_range);

    /* Page title */
    RUN_TEST(test_title_contains_i_ching);
    RUN_TEST(test_title_contains_hexagram);
    RUN_TEST(test_title_hex1_contains_creative);
    RUN_TEST(test_title_hex2_contains_receptive);

    /* King Wen field */
    RUN_TEST(test_king_wen_hex1);
    RUN_TEST(test_king_wen_hex2);
    RUN_TEST(test_king_wen_hex26);

    /* Hexagram section */
    RUN_TEST(test_hexagram_section_contains_hexagram);
    RUN_TEST(test_hexagram_section_contains_element);
    RUN_TEST(test_hexagram_section_contains_keywords);
    RUN_TEST(test_hexagram_section_hex1_heaven);

    /* Trigram section */
    RUN_TEST(test_trigram_section_contains_upper);
    RUN_TEST(test_trigram_section_contains_lower);
    RUN_TEST(test_trigram_section_contains_reading);
    RUN_TEST(test_trigram_section_contains_trigram_symbol);
    RUN_TEST(test_trigram_section_hex2_earth_symbol);

    /* Judgment section */
    RUN_TEST(test_judgment_section_contains_judgment);
    RUN_TEST(test_judgment_section_hex1_has_text);

    /* Image section */
    RUN_TEST(test_image_section_contains_image);
    RUN_TEST(test_image_section_hex1_has_text);

    /* Lines section */
    RUN_TEST(test_lines_section_contains_lines_header);
    RUN_TEST(test_lines_section_hex1_all_yang);
    RUN_TEST(test_lines_section_hex2_all_yin);

    /* Different JDs */
    RUN_TEST(test_different_jds_different_hexagrams);
    RUN_TEST(test_adjacent_jds_differ);

    /* Format function */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_all_sections);
    RUN_TEST(test_format_null_page_returns_zero);
    RUN_TEST(test_format_null_buf_returns_zero);
    RUN_TEST(test_format_zero_buf_size_returns_zero);
    RUN_TEST(test_format_small_buf_truncates);
    RUN_TEST(test_format_no_data_returns_zero);

    return UNITY_END();
}
