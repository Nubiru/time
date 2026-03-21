/* test_depth_interpret.c -- Unified interpretation dispatcher tests
 *
 * TDD: tests written before implementation.
 * Tests routing of interpretation requests to system-specific modules. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/depth_interpret.h"
#include "../../../src/systems/buddhist/buddhist.h"
#include "../../../src/systems/buddhist/kalpa.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * depth_interpret_default_params — zeroed struct
 * =================================================================== */

void test_default_params_all_zeroed(void)
{
    di_params_t p = depth_interpret_default_params();
    TEST_ASSERT_EQUAL_INT(0, p.hebrew_month);
    TEST_ASSERT_EQUAL_INT(0, p.hebrew_day);
    TEST_ASSERT_EQUAL_INT(0, p.hebrew_year);
    TEST_ASSERT_EQUAL_INT(0, p.hebrew_sabbatical);
    TEST_ASSERT_EQUAL_INT(0, p.hebrew_is_leap);
    TEST_ASSERT_EQUAL_INT(0, p.islamic_month);
    TEST_ASSERT_EQUAL_INT(0, p.islamic_day);
    TEST_ASSERT_EQUAL_INT(0, p.islamic_year);
    TEST_ASSERT_EQUAL_INT(0, p.buddhist_year);
    TEST_ASSERT_EQUAL_INT(0, p.buddhist_month);
    TEST_ASSERT_EQUAL_INT(0, p.buddhist_uposatha);
    TEST_ASSERT_EQUAL_INT(0, p.buddhist_kalpa_phase);
    TEST_ASSERT_EQUAL_INT(0, p.hindu_tithi);
    TEST_ASSERT_NULL(p.hindu_nakshatra);
    TEST_ASSERT_NULL(p.hindu_yoga);
    TEST_ASSERT_EQUAL_INT(0, p.iching_king_wen);
    TEST_ASSERT_NULL(p.iching_upper_tri);
    TEST_ASSERT_NULL(p.iching_lower_tri);
    TEST_ASSERT_EQUAL_INT(-1, p.kabbalah_sefirah);
    TEST_ASSERT_NULL(p.kabbalah_planet);
}

/* ===================================================================
 * depth_interpret_supported — check each system
 * =================================================================== */

void test_supported_hebrew(void)
{
    TEST_ASSERT_EQUAL_INT(1, depth_interpret_supported(TS_SYS_HEBREW));
}

void test_supported_islamic(void)
{
    TEST_ASSERT_EQUAL_INT(1, depth_interpret_supported(TS_SYS_ISLAMIC));
}

void test_supported_buddhist(void)
{
    TEST_ASSERT_EQUAL_INT(1, depth_interpret_supported(TS_SYS_BUDDHIST));
}

void test_supported_hindu(void)
{
    TEST_ASSERT_EQUAL_INT(1, depth_interpret_supported(TS_SYS_HINDU));
}

void test_supported_iching(void)
{
    TEST_ASSERT_EQUAL_INT(1, depth_interpret_supported(TS_SYS_ICHING));
}

void test_supported_kabbalah(void)
{
    TEST_ASSERT_EQUAL_INT(1, depth_interpret_supported(TS_SYS_KABBALAH));
}

void test_unsupported_gregorian(void)
{
    TEST_ASSERT_EQUAL_INT(0, depth_interpret_supported(TS_SYS_GREGORIAN));
}

void test_unsupported_tzolkin(void)
{
    TEST_ASSERT_EQUAL_INT(0, depth_interpret_supported(TS_SYS_TZOLKIN));
}

void test_unsupported_astrology(void)
{
    TEST_ASSERT_EQUAL_INT(0, depth_interpret_supported(TS_SYS_ASTROLOGY));
}

void test_unsupported_invalid_99(void)
{
    TEST_ASSERT_EQUAL_INT(0, depth_interpret_supported(99));
}

void test_unsupported_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, depth_interpret_supported(-1));
}

/* ===================================================================
 * depth_interpret_supported_count
 * =================================================================== */

void test_supported_count_returns_6(void)
{
    TEST_ASSERT_EQUAL_INT(6, depth_interpret_supported_count());
}

/* ===================================================================
 * depth_interpret — Hebrew with valid params
 * =================================================================== */

void test_interpret_hebrew_valid(void)
{
    di_params_t p = depth_interpret_default_params();
    p.hebrew_month = 7;  /* Tishrei */
    p.hebrew_day = 1;
    p.hebrew_year = 5786;
    p.hebrew_sabbatical = 1;
    p.hebrew_is_leap = 0;

    depth_interp_t r = depth_interpret(TS_SYS_HEBREW, &p);
    TEST_ASSERT_EQUAL_INT(1, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HEBREW, r.system_id);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

void test_interpret_hebrew_glance_contains_info(void)
{
    di_params_t p = depth_interpret_default_params();
    p.hebrew_month = 7;
    p.hebrew_day = 1;
    p.hebrew_year = 5786;
    p.hebrew_sabbatical = 1;
    p.hebrew_is_leap = 0;

    depth_interp_t r = depth_interpret(TS_SYS_HEBREW, &p);
    /* Glance should contain year info */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "5786"));
}

/* ===================================================================
 * depth_interpret — Islamic with valid params
 * =================================================================== */

void test_interpret_islamic_valid(void)
{
    di_params_t p = depth_interpret_default_params();
    p.islamic_month = 9;  /* Ramadan */
    p.islamic_day = 1;
    p.islamic_year = 1447;

    depth_interp_t r = depth_interpret(TS_SYS_ISLAMIC, &p);
    TEST_ASSERT_EQUAL_INT(1, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ISLAMIC, r.system_id);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

/* ===================================================================
 * depth_interpret — Buddhist with valid params
 * =================================================================== */

void test_interpret_buddhist_valid(void)
{
    di_params_t p = depth_interpret_default_params();
    p.buddhist_year = 2569;
    p.buddhist_month = 6;
    p.buddhist_uposatha = UPOSATHA_FULL_MOON;
    p.buddhist_kalpa_phase = KALPA_PHASE_EXISTENCE;

    depth_interp_t r = depth_interpret(TS_SYS_BUDDHIST, &p);
    TEST_ASSERT_EQUAL_INT(1, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_BUDDHIST, r.system_id);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

/* ===================================================================
 * depth_interpret — Hindu with valid params
 * =================================================================== */

void test_interpret_hindu_valid(void)
{
    di_params_t p = depth_interpret_default_params();
    p.hindu_tithi = 5;
    p.hindu_nakshatra = "Ashvini";
    p.hindu_yoga = "Vishkambha";

    depth_interp_t r = depth_interpret(TS_SYS_HINDU, &p);
    TEST_ASSERT_EQUAL_INT(1, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_HINDU, r.system_id);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

/* ===================================================================
 * depth_interpret — I Ching with valid params
 * =================================================================== */

void test_interpret_iching_valid(void)
{
    di_params_t p = depth_interpret_default_params();
    p.iching_king_wen = 1;
    p.iching_upper_tri = "Heaven";
    p.iching_lower_tri = "Heaven";

    depth_interp_t r = depth_interpret(TS_SYS_ICHING, &p);
    TEST_ASSERT_EQUAL_INT(1, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_ICHING, r.system_id);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

void test_interpret_iching_detail_has_judgment(void)
{
    di_params_t p = depth_interpret_default_params();
    p.iching_king_wen = 1;
    p.iching_upper_tri = "Heaven";
    p.iching_lower_tri = "Heaven";

    depth_interp_t r = depth_interpret(TS_SYS_ICHING, &p);
    /* Detail should contain hexagram content from ii_interpret */
    TEST_ASSERT_TRUE(strlen(r.detail) > 20);
}

/* ===================================================================
 * depth_interpret — Kabbalah with valid params
 * =================================================================== */

void test_interpret_kabbalah_valid(void)
{
    di_params_t p = depth_interpret_default_params();
    p.kabbalah_sefirah = 0;   /* Keter */
    p.kabbalah_planet = "Neptune";

    depth_interp_t r = depth_interpret(TS_SYS_KABBALAH, &p);
    TEST_ASSERT_EQUAL_INT(1, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_KABBALAH, r.system_id);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

/* ===================================================================
 * depth_interpret — unsupported system
 * =================================================================== */

void test_interpret_unsupported_gregorian(void)
{
    di_params_t p = depth_interpret_default_params();

    depth_interp_t r = depth_interpret(TS_SYS_GREGORIAN, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
    TEST_ASSERT_EQUAL_INT(TS_SYS_GREGORIAN, r.system_id);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "not yet available"));
}

/* ===================================================================
 * depth_interpret — invalid system_id
 * =================================================================== */

void test_interpret_invalid_system_99(void)
{
    di_params_t p = depth_interpret_default_params();

    depth_interp_t r = depth_interpret(99, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Unknown system"));
}

/* ===================================================================
 * depth_interpret — NULL params
 * =================================================================== */

void test_interpret_null_params(void)
{
    depth_interp_t r = depth_interpret(TS_SYS_HEBREW, NULL);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

/* ===================================================================
 * depth_interpret — zero-valued params (no data for that system)
 * =================================================================== */

void test_interpret_hebrew_zero_month(void)
{
    di_params_t p = depth_interpret_default_params();
    /* hebrew_month = 0 means no data */

    depth_interp_t r = depth_interpret(TS_SYS_HEBREW, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

void test_interpret_islamic_zero_month(void)
{
    di_params_t p = depth_interpret_default_params();
    /* islamic_month = 0 means no data */

    depth_interp_t r = depth_interpret(TS_SYS_ISLAMIC, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

void test_interpret_buddhist_zero_year(void)
{
    di_params_t p = depth_interpret_default_params();
    /* buddhist_year = 0 means no data */

    depth_interp_t r = depth_interpret(TS_SYS_BUDDHIST, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

void test_interpret_hindu_zero_tithi(void)
{
    di_params_t p = depth_interpret_default_params();
    /* hindu_tithi = 0 means no data */

    depth_interp_t r = depth_interpret(TS_SYS_HINDU, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

void test_interpret_iching_zero_king_wen(void)
{
    di_params_t p = depth_interpret_default_params();
    /* iching_king_wen = 0 means no data */

    depth_interp_t r = depth_interpret(TS_SYS_ICHING, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

void test_interpret_kabbalah_negative_sefirah(void)
{
    di_params_t p = depth_interpret_default_params();
    /* kabbalah_sefirah = -1 means no data (default) */

    depth_interp_t r = depth_interpret(TS_SYS_KABBALAH, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
}

/* ===================================================================
 * depth_interpret — locale-aware dispatch
 * =================================================================== */

void test_interpret_default_locale_is_english(void)
{
    di_params_t p = depth_interpret_default_params();
    /* locale defaults to 0 = EN via memset */
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_EN, p.locale);
}

void test_interpret_unsupported_locale_es(void)
{
    di_params_t p = depth_interpret_default_params();
    p.locale = I18N_LOCALE_ES;

    depth_interp_t r = depth_interpret(TS_SYS_GREGORIAN, &p);
    TEST_ASSERT_EQUAL_INT(0, r.has_data);
    /* Spanish unsupported glance should contain Spanish text */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "no disponible"));
}

/* ===================================================================
 * main
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Default params */
    RUN_TEST(test_default_params_all_zeroed);

    /* Supported checks */
    RUN_TEST(test_supported_hebrew);
    RUN_TEST(test_supported_islamic);
    RUN_TEST(test_supported_buddhist);
    RUN_TEST(test_supported_hindu);
    RUN_TEST(test_supported_iching);
    RUN_TEST(test_supported_kabbalah);
    RUN_TEST(test_unsupported_gregorian);
    RUN_TEST(test_unsupported_tzolkin);
    RUN_TEST(test_unsupported_astrology);
    RUN_TEST(test_unsupported_invalid_99);
    RUN_TEST(test_unsupported_negative);

    /* Supported count */
    RUN_TEST(test_supported_count_returns_6);

    /* Hebrew interpret */
    RUN_TEST(test_interpret_hebrew_valid);
    RUN_TEST(test_interpret_hebrew_glance_contains_info);

    /* Islamic interpret */
    RUN_TEST(test_interpret_islamic_valid);

    /* Buddhist interpret */
    RUN_TEST(test_interpret_buddhist_valid);

    /* Hindu interpret */
    RUN_TEST(test_interpret_hindu_valid);

    /* I Ching interpret */
    RUN_TEST(test_interpret_iching_valid);
    RUN_TEST(test_interpret_iching_detail_has_judgment);

    /* Kabbalah interpret */
    RUN_TEST(test_interpret_kabbalah_valid);

    /* Unsupported system */
    RUN_TEST(test_interpret_unsupported_gregorian);

    /* Invalid system */
    RUN_TEST(test_interpret_invalid_system_99);

    /* NULL params */
    RUN_TEST(test_interpret_null_params);

    /* Zero-valued params */
    RUN_TEST(test_interpret_hebrew_zero_month);
    RUN_TEST(test_interpret_islamic_zero_month);
    RUN_TEST(test_interpret_buddhist_zero_year);
    RUN_TEST(test_interpret_hindu_zero_tithi);
    RUN_TEST(test_interpret_iching_zero_king_wen);
    RUN_TEST(test_interpret_kabbalah_negative_sefirah);

    /* Locale-aware dispatch */
    RUN_TEST(test_interpret_default_locale_is_english);
    RUN_TEST(test_interpret_unsupported_locale_es);

    return UNITY_END();
}
