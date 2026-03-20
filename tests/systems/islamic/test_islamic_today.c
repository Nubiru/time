/* test_islamic_today.c — Tests for Islamic "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test dates (tabular Hijri):
 *   JD 2460499.5 => 1 Muharram 1446 (sacred month, leap year)
 *   JD 2460735.5 => 1 Ramadan 1446 (not sacred, fasting month)
 *   JD 2460705.5 => 1 Rajab 1446 (sacred month)
 *   JD 2460853.5 => 1 Muharram 1447 (common year)
 *
 * Makkah location: lat=21.4225, lon=39.8262
 */

#include "../../unity/unity.h"
#include "../../../src/systems/islamic/islamic_today.h"
#include "../../../src/systems/islamic/hijri.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Known JD constants ===== */

/* 1 Muharram 1446 — sacred month, leap year */
static const double JD_MUHARRAM_1446 = 2460499.5;

/* 1 Ramadan 1446 — fasting month, not sacred */
static const double JD_RAMADAN_1446 = 2460735.5;

/* 1 Rajab 1446 — sacred month (month 7)
 * months 1-6: 30+29+30+29+30+29 = 177 days from Muharram 1
 * day_of_year = 178 */
static const double JD_RAJAB_1446 = 2460676.5;

/* 1 Muharram 1447 — leap year (1447: (11*1447+14)%30 = 1 < 11 => leap) */
static const double JD_MUHARRAM_1447 = 2460853.5;

/* Makkah coordinates */
static const it_location_t LOC_MAKKAH = { 21.4225, 39.8262 };

/* Zero location (no prayer times) */
static const it_location_t LOC_ZERO = { 0.0, 0.0 };

/* ===== it_today — has_data and section_count ===== */

void test_today_has_data(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

/* ===== page_title ===== */

void test_title_contains_islamic(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Islamic"));
}

void test_title_contains_month_name(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Muharram"));
}

void test_title_contains_year_1446(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "1446"));
}

void test_title_contains_day_1(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "1 "));
}

void test_title_ramadan(void) {
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Ramadan"));
}

void test_title_rajab(void) {
    it_page_t p = it_today(JD_RAJAB_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Rajab"));
}

/* ===== date_section ===== */

void test_date_section_contains_date(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Date:"));
}

void test_date_section_contains_ah(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "AH"));
}

void test_date_section_contains_month_name(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Muharram"));
}

void test_date_section_contains_day_of_total(void) {
    /* 1 Muharram: "Day 1 of 30" */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Day 1 of 30"));
}

void test_date_section_leap_year(void) {
    /* 1446 is leap: (11*1446+14)%30 = 15920%30 = 20 => 20 >= 11, NOT leap
     * Wait, let me recalculate: (11*1446+14) = 15906+14 = 15920
     * 15920 % 30 = 15920 - 530*30 = 15920 - 15900 = 20 => 20 < 11? No.
     * So 1446 is NOT leap. */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Common year"));
}

void test_date_section_day_of_year(void) {
    /* 1 Muharram: day of year = 1 */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Day of year: 1 of"));
}

void test_date_section_ramadan_day_of_year(void) {
    /* 1 Ramadan: months 1-8 = 30+29+30+29+30+29+30+29 = 236 => day 237 */
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.date_section, "Day of year: 237 of"));
}

/* ===== month_section ===== */

void test_month_section_contains_month(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Month:"));
}

void test_month_section_contains_sacred(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Sacred:"));
}

void test_month_section_muharram_sacred_yes(void) {
    /* Muharram is sacred */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Sacred: Yes"));
}

void test_month_section_ramadan_sacred_no(void) {
    /* Ramadan is NOT sacred (only Muharram, Rajab, Dhul Qi'dah, Dhul Hijjah) */
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Sacred: No"));
}

void test_month_section_contains_significance(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Significance:"));
}

void test_month_section_contains_observance(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Observance:"));
}

void test_month_section_muharram_event(void) {
    /* Muharram has Ashura event */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Ashura"));
}

void test_month_section_safar_no_event(void) {
    /* Safar (month 2): 1 Safar 1446 = 2460499.5 + 30 = 2460529.5 */
    it_page_t p = it_today(2460529.5, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Observance: None"));
}

/* ===== prayer_section ===== */

void test_prayer_section_contains_header(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Prayer Times"));
}

void test_prayer_section_contains_fajr(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Fajr:"));
}

void test_prayer_section_contains_dhuhr(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Dhuhr:"));
}

void test_prayer_section_contains_asr(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Asr:"));
}

void test_prayer_section_contains_maghrib(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Maghrib:"));
}

void test_prayer_section_contains_isha(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Isha:"));
}

void test_prayer_section_contains_sunrise(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Sunrise:"));
}

void test_prayer_section_contains_colon_time(void) {
    /* Times should be HH:MM format, so at least one colon in time value */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    /* After "Fajr:" there should be a time with ":" */
    char *fajr = strstr(p.prayer_section, "Fajr:");
    TEST_ASSERT_NOT_NULL(fajr);
    /* After the label, find the next colon (the time separator) */
    char *time_colon = strstr(fajr + 5, ":");
    TEST_ASSERT_NOT_NULL(time_colon);
}

void test_prayer_section_zero_location(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_ZERO);
    TEST_ASSERT_NOT_NULL(strstr(p.prayer_section, "Location required"));
}

/* ===== next_prayer_line ===== */

void test_next_prayer_contains_label(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.next_prayer_line, "Next Prayer:"));
}

void test_next_prayer_zero_location(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_ZERO);
    TEST_ASSERT_NOT_NULL(strstr(p.next_prayer_line, "Provide location"));
}

void test_next_prayer_has_at(void) {
    /* With a valid location, should contain "at" for the time */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.next_prayer_line, "at"));
}

/* ===== sacred_section ===== */

void test_sacred_section_nonempty(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_TRUE(strlen(p.sacred_section) > 0);
}

void test_sacred_section_muharram_sacred(void) {
    /* Muharram is sacred — should mention al-ashhur al-hurum */
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.sacred_section, "sacred months"));
}

void test_sacred_section_rajab_sacred(void) {
    /* Rajab is sacred */
    it_page_t p = it_today(JD_RAJAB_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.sacred_section, "sacred months"));
}

void test_sacred_section_ramadan_not_sacred(void) {
    /* Ramadan is NOT sacred — should NOT mention al-ashhur al-hurum */
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_NULL(strstr(p.sacred_section, "sacred months"));
}

void test_sacred_section_ramadan_has_brief(void) {
    /* Ramadan still has a brief */
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_TRUE(strlen(p.sacred_section) > 0);
}

/* ===== is_sacred_month field ===== */

void test_is_sacred_month_muharram(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    TEST_ASSERT_EQUAL_INT(1, p.is_sacred_month);
}

void test_is_sacred_month_ramadan(void) {
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_EQUAL_INT(0, p.is_sacred_month);
}

void test_is_sacred_month_rajab(void) {
    it_page_t p = it_today(JD_RAJAB_1446, LOC_MAKKAH);
    TEST_ASSERT_EQUAL_INT(1, p.is_sacred_month);
}

/* ===== it_format ===== */

void test_format_produces_output(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[4096];
    int len = it_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[4096];
    it_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Islamic"));
}

void test_format_contains_date_section(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[4096];
    it_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Date:"));
}

void test_format_contains_month_section(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[4096];
    it_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Month:"));
}

void test_format_contains_prayer_section(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[4096];
    it_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Prayer"));
}

void test_format_null_page(void) {
    char buf[256];
    int len = it_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    int len = it_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_zero_buf_size(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[1] = { 'X' };
    int len = it_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_small_buffer(void) {
    it_page_t p = it_today(JD_MUHARRAM_1446, LOC_MAKKAH);
    char buf[32];
    int len = it_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ===== Different months / years ===== */

void test_year_1447_title(void) {
    it_page_t p = it_today(JD_MUHARRAM_1447, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "1447"));
}

void test_year_1447_has_data(void) {
    it_page_t p = it_today(JD_MUHARRAM_1447, LOC_MAKKAH);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_ramadan_significance(void) {
    /* Ramadan month_section should mention fasting-related significance */
    it_page_t p = it_today(JD_RAMADAN_1446, LOC_MAKKAH);
    TEST_ASSERT_NOT_NULL(strstr(p.month_section, "Significance:"));
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* has_data / section_count */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);

    /* page_title */
    RUN_TEST(test_title_contains_islamic);
    RUN_TEST(test_title_contains_month_name);
    RUN_TEST(test_title_contains_year_1446);
    RUN_TEST(test_title_contains_day_1);
    RUN_TEST(test_title_ramadan);
    RUN_TEST(test_title_rajab);

    /* date_section */
    RUN_TEST(test_date_section_contains_date);
    RUN_TEST(test_date_section_contains_ah);
    RUN_TEST(test_date_section_contains_month_name);
    RUN_TEST(test_date_section_contains_day_of_total);
    RUN_TEST(test_date_section_leap_year);
    RUN_TEST(test_date_section_day_of_year);
    RUN_TEST(test_date_section_ramadan_day_of_year);

    /* month_section */
    RUN_TEST(test_month_section_contains_month);
    RUN_TEST(test_month_section_contains_sacred);
    RUN_TEST(test_month_section_muharram_sacred_yes);
    RUN_TEST(test_month_section_ramadan_sacred_no);
    RUN_TEST(test_month_section_contains_significance);
    RUN_TEST(test_month_section_contains_observance);
    RUN_TEST(test_month_section_muharram_event);
    RUN_TEST(test_month_section_safar_no_event);

    /* prayer_section */
    RUN_TEST(test_prayer_section_contains_header);
    RUN_TEST(test_prayer_section_contains_fajr);
    RUN_TEST(test_prayer_section_contains_dhuhr);
    RUN_TEST(test_prayer_section_contains_asr);
    RUN_TEST(test_prayer_section_contains_maghrib);
    RUN_TEST(test_prayer_section_contains_isha);
    RUN_TEST(test_prayer_section_contains_sunrise);
    RUN_TEST(test_prayer_section_contains_colon_time);
    RUN_TEST(test_prayer_section_zero_location);

    /* next_prayer_line */
    RUN_TEST(test_next_prayer_contains_label);
    RUN_TEST(test_next_prayer_zero_location);
    RUN_TEST(test_next_prayer_has_at);

    /* sacred_section */
    RUN_TEST(test_sacred_section_nonempty);
    RUN_TEST(test_sacred_section_muharram_sacred);
    RUN_TEST(test_sacred_section_rajab_sacred);
    RUN_TEST(test_sacred_section_ramadan_not_sacred);
    RUN_TEST(test_sacred_section_ramadan_has_brief);

    /* is_sacred_month */
    RUN_TEST(test_is_sacred_month_muharram);
    RUN_TEST(test_is_sacred_month_ramadan);
    RUN_TEST(test_is_sacred_month_rajab);

    /* it_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_date_section);
    RUN_TEST(test_format_contains_month_section);
    RUN_TEST(test_format_contains_prayer_section);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_zero_buf_size);
    RUN_TEST(test_format_small_buffer);

    /* Different months / years */
    RUN_TEST(test_year_1447_title);
    RUN_TEST(test_year_1447_has_data);
    RUN_TEST(test_ramadan_significance);

    return UNITY_END();
}
