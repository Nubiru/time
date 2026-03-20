/* test_hindu_today.c -- Tests for Hindu "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test inputs:
 *   sun_lon=359.0, moon_lon=45.0, JD=2460394.5, ce_year=2024
 *
 * Expected panchanga from these inputs:
 *   elongation = 46.0 => tithi 4 (Chaturthi), Shukla paksha, day 4
 *   ayanamsa ~24.19 => moon_sid ~20.81 => nakshatra index 1 (Bharani)
 *   sun_sid ~334.81, yoga_sum ~355.62 => yoga index 26 (Vaidhriti)
 *   karana position 7 => index 7 (Vishti)
 *   vara: JD 2460394.5 => day 1 => Somavara (Monday)
 *   yuga: 2024 CE => Kali Yuga, ~5125 years elapsed
 */

#include "../../unity/unity.h"
#include "../../../src/systems/hindu/hindu_today.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Test constants ===== */

static const double TEST_JD = 2460394.5;
static const int TEST_CE_YEAR = 2024;
static const hn_planets_t TEST_PLANETS = { 359.0, 45.0 };

/* ===== hn_today — has_data and section_count ===== */

void test_has_data_with_valid_planets(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_has_data_null_planets(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, NULL);
    TEST_ASSERT_EQUAL_INT(0, p.has_data);
}

void test_section_count_is_five(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_section_count_zero_when_null(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, NULL);
    TEST_ASSERT_EQUAL_INT(0, p.section_count);
}

/* ===== page_title ===== */

void test_title_contains_hindu(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Hindu"));
}

void test_title_contains_paksha_name(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Shukla"));
}

void test_title_contains_tithi_name(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Chaturthi"));
}

void test_title_format(void)
{
    /* "Hindu -- Shukla Chaturthi" */
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Hindu"));
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Shukla"));
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Chaturthi"));
}

/* ===== panchanga_section ===== */

void test_panchanga_section_contains_panchanga(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Panchanga"));
}

void test_panchanga_section_contains_tithi(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Tithi:"));
}

void test_panchanga_section_contains_vara(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Vara:"));
}

void test_panchanga_section_contains_nakshatra(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Nakshatra:"));
}

void test_panchanga_section_contains_yoga(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Yoga:"));
}

void test_panchanga_section_contains_karana(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Karana:"));
}

void test_panchanga_section_tithi_number(void)
{
    /* Should contain "(4/30)" */
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "(4/30)"));
}

void test_panchanga_section_vara_day(void)
{
    /* JD 2460394.5 is Monday => Somavara */
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.panchanga_section, "Somavara"));
}

/* ===== tithi_section ===== */

void test_tithi_section_contains_tithi(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.tithi_section, "Tithi:"));
}

void test_tithi_section_contains_paksha(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.tithi_section, "Paksha:"));
}

void test_tithi_section_contains_deity(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.tithi_section, "Deity:"));
}

void test_tithi_section_contains_quality(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.tithi_section, "Quality:"));
}

void test_tithi_section_contains_activity(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.tithi_section, "Activity:"));
}

void test_tithi_section_paksha_day(void)
{
    /* Shukla tithi 4 => day 4 of 15 */
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.tithi_section, "day 4 of 15"));
}

/* ===== nakshatra_section ===== */

void test_nakshatra_section_contains_nakshatra(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.nakshatra_section, "Nakshatra:"));
}

void test_nakshatra_section_contains_deity(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.nakshatra_section, "Deity:"));
}

void test_nakshatra_section_contains_symbol(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.nakshatra_section, "Symbol:"));
}

void test_nakshatra_section_contains_ruling_planet(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.nakshatra_section, "Ruling Planet:"));
}

void test_nakshatra_section_contains_quality(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.nakshatra_section, "Quality:"));
}

void test_nakshatra_section_contains_meaning(void)
{
    /* nakshatra_get(1) = Bharani, meaning = "She who bears" */
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.nakshatra_section, "Bharani"));
}

/* ===== yuga_section ===== */

void test_yuga_section_contains_kali(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.yuga_section, "Kali"));
}

void test_yuga_section_contains_dharma(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.yuga_section, "Dharma:"));
}

void test_yuga_section_contains_years(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.yuga_section, "Years"));
}

void test_yuga_section_contains_maha_yuga(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.yuga_section, "Maha-Yuga:"));
}

void test_yuga_section_contains_current_age(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.yuga_section, "Current Age:"));
}

void test_yuga_section_dharma_25_percent(void)
{
    /* Kali Yuga has 25% dharma */
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_NOT_NULL(strstr(p.yuga_section, "25%"));
}

/* ===== guidance_section ===== */

void test_guidance_section_nonempty(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    TEST_ASSERT_TRUE(strlen(p.guidance_section) > 0);
}

/* ===== hn_format ===== */

void test_format_produces_output(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    char buf[4096];
    int len = hn_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    char buf[4096];
    hn_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hindu"));
}

void test_format_contains_panchanga(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    char buf[4096];
    hn_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Panchanga"));
}

void test_format_null_page(void)
{
    char buf[256];
    int len = hn_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    int len = hn_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_zero_buf_size(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    char buf[1] = { 'X' };
    int len = hn_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_small_buffer(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, &TEST_PLANETS);
    char buf[32];
    int len = hn_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < 32);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ===== hn_default_planets ===== */

void test_default_planets_zero(void)
{
    hn_planets_t dp = hn_default_planets();
    TEST_ASSERT_TRUE(dp.sun_lon == 0.0);
    TEST_ASSERT_TRUE(dp.moon_lon == 0.0);
}

/* ===== Different longitudes give different panchanga ===== */

void test_different_longitudes_different_tithi(void)
{
    hn_planets_t p1 = { 0.0, 45.0 };
    hn_planets_t p2 = { 0.0, 180.0 };
    hn_page_t page1 = hn_today(TEST_JD, TEST_CE_YEAR, &p1);
    hn_page_t page2 = hn_today(TEST_JD, TEST_CE_YEAR, &p2);
    /* Different moon longitudes should produce different panchanga sections */
    TEST_ASSERT_TRUE(strcmp(page1.panchanga_section, page2.panchanga_section) != 0);
}

void test_different_longitudes_different_nakshatra(void)
{
    hn_planets_t p1 = { 0.0, 10.0 };
    hn_planets_t p2 = { 0.0, 200.0 };
    hn_page_t page1 = hn_today(TEST_JD, TEST_CE_YEAR, &p1);
    hn_page_t page2 = hn_today(TEST_JD, TEST_CE_YEAR, &p2);
    TEST_ASSERT_TRUE(strcmp(page1.nakshatra_section, page2.nakshatra_section) != 0);
}

/* ===== null planets produces empty sections ===== */

void test_null_planets_empty_title(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, NULL);
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(p.page_title));
}

void test_null_planets_empty_panchanga(void)
{
    hn_page_t p = hn_today(TEST_JD, TEST_CE_YEAR, NULL);
    TEST_ASSERT_EQUAL_INT(0, (int)strlen(p.panchanga_section));
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* has_data and section_count */
    RUN_TEST(test_has_data_with_valid_planets);
    RUN_TEST(test_has_data_null_planets);
    RUN_TEST(test_section_count_is_five);
    RUN_TEST(test_section_count_zero_when_null);

    /* page_title */
    RUN_TEST(test_title_contains_hindu);
    RUN_TEST(test_title_contains_paksha_name);
    RUN_TEST(test_title_contains_tithi_name);
    RUN_TEST(test_title_format);

    /* panchanga_section */
    RUN_TEST(test_panchanga_section_contains_panchanga);
    RUN_TEST(test_panchanga_section_contains_tithi);
    RUN_TEST(test_panchanga_section_contains_vara);
    RUN_TEST(test_panchanga_section_contains_nakshatra);
    RUN_TEST(test_panchanga_section_contains_yoga);
    RUN_TEST(test_panchanga_section_contains_karana);
    RUN_TEST(test_panchanga_section_tithi_number);
    RUN_TEST(test_panchanga_section_vara_day);

    /* tithi_section */
    RUN_TEST(test_tithi_section_contains_tithi);
    RUN_TEST(test_tithi_section_contains_paksha);
    RUN_TEST(test_tithi_section_contains_deity);
    RUN_TEST(test_tithi_section_contains_quality);
    RUN_TEST(test_tithi_section_contains_activity);
    RUN_TEST(test_tithi_section_paksha_day);

    /* nakshatra_section */
    RUN_TEST(test_nakshatra_section_contains_nakshatra);
    RUN_TEST(test_nakshatra_section_contains_deity);
    RUN_TEST(test_nakshatra_section_contains_symbol);
    RUN_TEST(test_nakshatra_section_contains_ruling_planet);
    RUN_TEST(test_nakshatra_section_contains_quality);
    RUN_TEST(test_nakshatra_section_contains_meaning);

    /* yuga_section */
    RUN_TEST(test_yuga_section_contains_kali);
    RUN_TEST(test_yuga_section_contains_dharma);
    RUN_TEST(test_yuga_section_contains_years);
    RUN_TEST(test_yuga_section_contains_maha_yuga);
    RUN_TEST(test_yuga_section_contains_current_age);
    RUN_TEST(test_yuga_section_dharma_25_percent);

    /* guidance_section */
    RUN_TEST(test_guidance_section_nonempty);

    /* hn_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_panchanga);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_zero_buf_size);
    RUN_TEST(test_format_small_buffer);

    /* hn_default_planets */
    RUN_TEST(test_default_planets_zero);

    /* Different longitudes */
    RUN_TEST(test_different_longitudes_different_tithi);
    RUN_TEST(test_different_longitudes_different_nakshatra);

    /* NULL planets edge cases */
    RUN_TEST(test_null_planets_empty_title);
    RUN_TEST(test_null_planets_empty_panchanga);

    return UNITY_END();
}
