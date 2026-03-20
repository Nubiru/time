/* test_buddhist_today.c — Tests for Buddhist "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test JD values:
 *   JD 2460400.5 => ~2024-03-20 (approx Citta, not Vassa)
 *   JD 2460550.5 => ~2024-08-18 (approx Savana/Potthapadha, Vassa likely)
 *   JD 2460700.5 => ~2025-01-15 (approx Magha, Cool season)
 *
 * Buddhist Era: CE + 543
 *   CE 2024 => BE 2567
 *   CE 2025 => BE 2568
 */

#include "../../unity/unity.h"
#include "../../../src/systems/buddhist/buddhist_today.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Test JD constants ===== */

/* ~2024-03-20: Hot season month, unlikely Uposatha */
static const double JD_MARCH_2024 = 2460389.5;

/* ~2024-08-18: Rainy season, Vassa period */
static const double JD_AUG_2024 = 2460540.5;

/* ~2025-01-15: Cool season month */
static const double JD_JAN_2025 = 2460690.5;

/* ~2026-03-20: Another date for variation */
static const double JD_MARCH_2026 = 2461119.5;

/* ===== bt_today — has_data and section_count ===== */

void test_today_has_data(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_today_has_data_different_date(void) {
    bt_page_t p = bt_today(JD_JAN_2025);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

/* ===== page_title ===== */

void test_title_contains_buddhist(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Buddhist"));
}

void test_title_contains_be(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "BE"));
}

void test_title_contains_be_year(void) {
    /* CE 2024 => BE 2567 */
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "2567"));
}

void test_title_different_year(void) {
    /* CE 2026 => BE 2569 */
    bt_page_t p = bt_today(JD_MARCH_2026);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "2569"));
}

/* ===== era_section ===== */

void test_era_section_contains_label(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.era_section, "Buddhist Era:"));
}

void test_era_section_contains_be_year(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.era_section, "2567"));
}

void test_era_section_contains_common_era(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.era_section, "Common Era:"));
}

void test_era_section_contains_ce_year(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.era_section, "2024"));
}

void test_era_section_contains_difference(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.era_section, "543 years"));
}

/* ===== uposatha_section ===== */

void test_uposatha_section_contains_moon_age(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.uposatha_section, "Moon Age:"));
}

void test_uposatha_section_nonempty(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_TRUE(strlen(p.uposatha_section) > 0);
}

void test_uposatha_section_has_content_for_different_date(void) {
    bt_page_t p = bt_today(JD_AUG_2024);
    TEST_ASSERT_TRUE(strlen(p.uposatha_section) > 0);
}

/* ===== is_uposatha field ===== */

void test_is_uposatha_valid_range(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_TRUE(p.is_uposatha == 0 || p.is_uposatha == 1);
}

void test_is_uposatha_consistent_with_section(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    if (p.is_uposatha) {
        TEST_ASSERT_NOT_NULL(strstr(p.uposatha_section, "Uposatha Day:"));
    } else {
        TEST_ASSERT_NOT_NULL(strstr(p.uposatha_section, "No observance today"));
    }
}

/* ===== month_section ===== */

void test_month_section_contains_month_label(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Month:"));
}

void test_month_section_contains_vassa_label(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Vassa"));
}

void test_month_section_contains_season(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Season:"));
}

void test_month_section_different_dates_vary(void) {
    bt_page_t p1 = bt_today(JD_MARCH_2024);
    bt_page_t p2 = bt_today(JD_AUG_2024);
    /* Different months should produce different sections */
    TEST_ASSERT_TRUE(strcmp(p1.month_section, p2.month_section) != 0);
}

/* ===== is_vassa field ===== */

void test_is_vassa_valid_range(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_TRUE(p.is_vassa == 0 || p.is_vassa == 1);
}

/* ===== kalpa_section ===== */

void test_kalpa_section_contains_cosmic_phase(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.kalpa_section, "Cosmic Phase:"));
}

void test_kalpa_section_contains_maha_kalpa(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.kalpa_section, "Maha-Kalpa:"));
}

void test_kalpa_section_contains_antarakalpa(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.kalpa_section, "Antarakalpa:"));
}

void test_kalpa_section_contains_fraction(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_NOT_NULL(strstr(p.kalpa_section, "Cycle Fraction:"));
}

/* ===== practice_section ===== */

void test_practice_section_nonempty(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    TEST_ASSERT_TRUE(strlen(p.practice_section) > 0);
}

void test_practice_section_different_date(void) {
    bt_page_t p = bt_today(JD_JAN_2025);
    TEST_ASSERT_TRUE(strlen(p.practice_section) > 0);
}

/* ===== bt_format ===== */

void test_format_produces_output(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[4096];
    int len = bt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[4096];
    bt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Buddhist"));
}

void test_format_contains_era(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[4096];
    bt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Buddhist Era:"));
}

void test_format_contains_month(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[4096];
    bt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Month:"));
}

void test_format_contains_cosmic(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[4096];
    bt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Cosmic Phase:"));
}

void test_format_null_page(void) {
    char buf[256];
    int len = bt_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    int len = bt_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_zero_buf_size(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[1] = { 'X' };
    int len = bt_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_small_buffer(void) {
    bt_page_t p = bt_today(JD_MARCH_2024);
    char buf[32];
    int len = bt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* has_data / section_count */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);
    RUN_TEST(test_today_has_data_different_date);

    /* page_title */
    RUN_TEST(test_title_contains_buddhist);
    RUN_TEST(test_title_contains_be);
    RUN_TEST(test_title_contains_be_year);
    RUN_TEST(test_title_different_year);

    /* era_section */
    RUN_TEST(test_era_section_contains_label);
    RUN_TEST(test_era_section_contains_be_year);
    RUN_TEST(test_era_section_contains_common_era);
    RUN_TEST(test_era_section_contains_ce_year);
    RUN_TEST(test_era_section_contains_difference);

    /* uposatha_section */
    RUN_TEST(test_uposatha_section_contains_moon_age);
    RUN_TEST(test_uposatha_section_nonempty);
    RUN_TEST(test_uposatha_section_has_content_for_different_date);

    /* is_uposatha */
    RUN_TEST(test_is_uposatha_valid_range);
    RUN_TEST(test_is_uposatha_consistent_with_section);

    /* month_section */
    RUN_TEST(test_month_section_contains_month_label);
    RUN_TEST(test_month_section_contains_vassa_label);
    RUN_TEST(test_month_section_contains_season);
    RUN_TEST(test_month_section_different_dates_vary);

    /* is_vassa */
    RUN_TEST(test_is_vassa_valid_range);

    /* kalpa_section */
    RUN_TEST(test_kalpa_section_contains_cosmic_phase);
    RUN_TEST(test_kalpa_section_contains_maha_kalpa);
    RUN_TEST(test_kalpa_section_contains_antarakalpa);
    RUN_TEST(test_kalpa_section_contains_fraction);

    /* practice_section */
    RUN_TEST(test_practice_section_nonempty);
    RUN_TEST(test_practice_section_different_date);

    /* bt_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_era);
    RUN_TEST(test_format_contains_month);
    RUN_TEST(test_format_contains_cosmic);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_zero_buf_size);
    RUN_TEST(test_format_small_buffer);

    return UNITY_END();
}
