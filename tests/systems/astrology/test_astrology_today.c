/* test_astrology_today.c — Full Astrology "Today" page
 * TDD RED phase: tests written before implementation.
 * Sun/Moon sign + decan, planetary hour, active aspects, dignity. */

#include "../../unity/unity.h"
#include "../../../src/systems/astrology/astrology_today.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Test fixture: known planetary longitudes ===== */

/* Sun=359.4 (Pisces 29.4), Moon=72.5 (Gemini 12.5),
 * Mercury=340.0 (Pisces 10.0), Venus=38.0 (Taurus 8.0),
 * Mars=95.0 (Cancer 5.0), Jupiter=80.0 (Gemini 20.0),
 * Saturn=355.0 (Pisces 25.0) */
static at_planets_t test_planets(void)
{
    at_planets_t p;
    p.longitudes[0] = 359.4;  /* Sun     */
    p.longitudes[1] = 72.5;   /* Moon    */
    p.longitudes[2] = 340.0;  /* Mercury */
    p.longitudes[3] = 38.0;   /* Venus   */
    p.longitudes[4] = 95.0;   /* Mars    */
    p.longitudes[5] = 80.0;   /* Jupiter */
    p.longitudes[6] = 355.0;  /* Saturn  */
    return p;
}

/* JD 2460394.5 = 2024-03-20 00:00 UT (March equinox) */
static const double TEST_JD = 2460394.5;

/* ===== at_today — valid input ===== */

void test_today_valid_has_data(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_EQUAL_INT(1, page.has_data);
}

void test_today_section_count_is_5(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_EQUAL_INT(5, page.section_count);
}

/* ===== at_today — NULL input ===== */

void test_today_null_planets_no_data(void)
{
    at_page_t page = at_today(TEST_JD, NULL);
    TEST_ASSERT_EQUAL_INT(0, page.has_data);
}

/* ===== page_title ===== */

void test_page_title_contains_astrology(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.page_title, "Astrology"));
}

void test_page_title_contains_sun_in(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.page_title, "Sun in"));
}

void test_page_title_contains_sign_name(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* Sun at 359.4 => Pisces */
    TEST_ASSERT_NOT_NULL(strstr(page.page_title, "Pisces"));
}

/* ===== sun_section ===== */

void test_sun_section_contains_sun_sign(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Sun Sign:"));
}

void test_sun_section_contains_pisces(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Pisces"));
}

void test_sun_section_contains_archetype(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Archetype:"));
}

void test_sun_section_contains_decan(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* 359.4 => Pisces 29.4 => Decan 3 */
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Decan: 3rd"));
}

void test_sun_section_contains_element(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Element:"));
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Water"));
}

void test_sun_section_contains_ruler(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Ruler:"));
}

void test_sun_section_contains_keywords(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Keywords:"));
}

void test_sun_section_contains_shadow(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Shadow:"));
}

/* ===== moon_section ===== */

void test_moon_section_contains_moon_sign(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.moon_section, "Moon Sign:"));
}

void test_moon_section_contains_gemini(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* Moon at 72.5 => Gemini */
    TEST_ASSERT_NOT_NULL(strstr(page.moon_section, "Gemini"));
}

void test_moon_section_contains_emotional_tone(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.moon_section, "Emotional tone:"));
}

void test_moon_section_contains_element(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* Gemini => Air */
    TEST_ASSERT_NOT_NULL(strstr(page.moon_section, "Element: Air"));
}

/* ===== hour_section ===== */

void test_hour_section_contains_planetary_hour(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.hour_section, "Planetary Hour:"));
}

void test_hour_section_contains_day_ruler(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.hour_section, "Day Ruler:"));
}

void test_hour_section_contains_hour_of_24(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.hour_section, "of 24"));
}

/* ===== aspects_section ===== */

void test_aspects_section_contains_aspects(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* Could be "Active Aspects:" or "No significant aspects" */
    TEST_ASSERT_TRUE(
        strstr(page.aspects_section, "Aspects") != NULL ||
        strstr(page.aspects_section, "aspects") != NULL);
}

void test_aspects_section_max_five(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* Count newlines — each aspect is on its own line.
     * Header + up to 5 aspects = at most 6 non-empty lines. */
    int newlines = 0;
    for (int i = 0; page.aspects_section[i] != '\0'; i++) {
        if (page.aspects_section[i] == '\n') newlines++;
    }
    /* At most: header + 5 aspect lines = 5 newlines */
    TEST_ASSERT_TRUE(newlines <= 6);
}

/* ===== dignity_section ===== */

void test_dignity_section_contains_dignity(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Dignity"));
}

void test_dignity_section_contains_sun_in(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Sun in"));
}

void test_dignity_section_contains_all_planets(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Moon"));
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Mercury"));
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Venus"));
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Mars"));
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Jupiter"));
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Saturn"));
}

void test_dignity_section_contains_score(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    /* Venus at 38.0 => Taurus => Domicile (+5) */
    TEST_ASSERT_NOT_NULL(strstr(page.dignity_section, "Domicile"));
}

/* ===== at_format ===== */

void test_format_produces_output(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    char buf[4096];
    int len = at_format(&page, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_all_sections(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    char buf[4096];
    at_format(&page, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun Sign:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon Sign:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Planetary Hour:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Dignity"));
}

void test_format_null_page_returns_zero(void)
{
    char buf[256];
    int len = at_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_null_buf_returns_zero(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    int len = at_format(&page, NULL, 100);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_tiny_buf_returns_zero(void)
{
    at_planets_t p = test_planets();
    at_page_t page = at_today(TEST_JD, &p);
    char buf[2];
    int len = at_format(&page, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== at_default_planets ===== */

void test_default_planets_all_zero(void)
{
    at_planets_t p = at_default_planets();
    for (int i = 0; i < AT_PLANET_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)p.longitudes[i]);
    }
}

/* ===== Different positions — Aries ===== */

void test_aries_sun(void)
{
    at_planets_t p = test_planets();
    p.longitudes[0] = 15.0; /* Aries 15 */
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.page_title, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Decan: 2nd"));
}

void test_leo_sun_first_decan(void)
{
    at_planets_t p = test_planets();
    p.longitudes[0] = 125.0; /* Leo 5 */
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.page_title, "Leo"));
    TEST_ASSERT_NOT_NULL(strstr(page.sun_section, "Decan: 1st"));
}

void test_scorpio_moon(void)
{
    at_planets_t p = test_planets();
    p.longitudes[1] = 220.0; /* Scorpio 10 */
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_NOT_NULL(strstr(page.moon_section, "Scorpio"));
    TEST_ASSERT_NOT_NULL(strstr(page.moon_section, "Element: Water"));
}

/* ===== All zeros (Aries) ===== */

void test_all_zeros_aries(void)
{
    at_planets_t p = at_default_planets();
    at_page_t page = at_today(TEST_JD, &p);
    TEST_ASSERT_EQUAL_INT(1, page.has_data);
    TEST_ASSERT_NOT_NULL(strstr(page.page_title, "Aries"));
}

/* ===== No-data page format returns zero ===== */

void test_format_no_data_page(void)
{
    at_page_t page = at_today(TEST_JD, NULL);
    char buf[256];
    int len = at_format(&page, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Core: valid / null */
    RUN_TEST(test_today_valid_has_data);
    RUN_TEST(test_today_section_count_is_5);
    RUN_TEST(test_today_null_planets_no_data);

    /* Page title */
    RUN_TEST(test_page_title_contains_astrology);
    RUN_TEST(test_page_title_contains_sun_in);
    RUN_TEST(test_page_title_contains_sign_name);

    /* Sun section */
    RUN_TEST(test_sun_section_contains_sun_sign);
    RUN_TEST(test_sun_section_contains_pisces);
    RUN_TEST(test_sun_section_contains_archetype);
    RUN_TEST(test_sun_section_contains_decan);
    RUN_TEST(test_sun_section_contains_element);
    RUN_TEST(test_sun_section_contains_ruler);
    RUN_TEST(test_sun_section_contains_keywords);
    RUN_TEST(test_sun_section_contains_shadow);

    /* Moon section */
    RUN_TEST(test_moon_section_contains_moon_sign);
    RUN_TEST(test_moon_section_contains_gemini);
    RUN_TEST(test_moon_section_contains_emotional_tone);
    RUN_TEST(test_moon_section_contains_element);

    /* Hour section */
    RUN_TEST(test_hour_section_contains_planetary_hour);
    RUN_TEST(test_hour_section_contains_day_ruler);
    RUN_TEST(test_hour_section_contains_hour_of_24);

    /* Aspects section */
    RUN_TEST(test_aspects_section_contains_aspects);
    RUN_TEST(test_aspects_section_max_five);

    /* Dignity section */
    RUN_TEST(test_dignity_section_contains_dignity);
    RUN_TEST(test_dignity_section_contains_sun_in);
    RUN_TEST(test_dignity_section_contains_all_planets);
    RUN_TEST(test_dignity_section_contains_score);

    /* Format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_all_sections);
    RUN_TEST(test_format_null_page_returns_zero);
    RUN_TEST(test_format_null_buf_returns_zero);
    RUN_TEST(test_format_tiny_buf_returns_zero);

    /* Default planets */
    RUN_TEST(test_default_planets_all_zero);

    /* Different positions */
    RUN_TEST(test_aries_sun);
    RUN_TEST(test_leo_sun_first_decan);
    RUN_TEST(test_scorpio_moon);
    RUN_TEST(test_all_zeros_aries);

    /* No-data page format */
    RUN_TEST(test_format_no_data_page);

    return UNITY_END();
}
