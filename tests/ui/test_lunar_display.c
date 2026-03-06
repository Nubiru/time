#include "../unity/unity.h"
#include "../../src/ui/lunar_display.h"

#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Reference JDs:
 * 2451550.26 = known new moon (2000-01-06 18:14 UT)
 * 2451550.26 + 14.765 ≈ full moon
 * 2451550.26 + 7.38 ≈ first quarter
 */
#define JD_NEW_MOON   2451550.26
#define JD_FULL_MOON  (2451550.26 + 14.765)
#define JD_FIRST_Q    (2451550.26 + 7.38)
#define JD_J2000      2451545.0

/* Display at key phases: 1-3 */

void test_display_new_moon(void)
{
    lunar_display_t d = lunar_display(JD_NEW_MOON);
    TEST_ASSERT_NOT_NULL(strstr(d.phase_line, "New Moon"));
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
}

void test_display_full_moon(void)
{
    lunar_display_t d = lunar_display(JD_FULL_MOON);
    TEST_ASSERT_NOT_NULL(strstr(d.phase_line, "Full Moon"));
}

void test_display_first_quarter(void)
{
    lunar_display_t d = lunar_display(JD_FIRST_Q);
    TEST_ASSERT_NOT_NULL(strstr(d.phase_line, "Quarter"));
}

/* Illumination formatting: 4-7 */

void test_illumination_zero(void)
{
    char buf[16];
    lunar_fmt_illumination(0.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "0"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "%"));
}

void test_illumination_half(void)
{
    char buf[16];
    lunar_fmt_illumination(0.5, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "50"));
}

void test_illumination_full(void)
{
    char buf[16];
    lunar_fmt_illumination(1.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "100"));
}

void test_illumination_precision(void)
{
    char buf[16];
    lunar_fmt_illumination(0.983, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "98"));
}

/* Age formatting: 8-9 */

void test_age_format(void)
{
    char buf[32];
    lunar_fmt_age(14.7, 29.53, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "14.7"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "29.5"));
}

void test_age_day_zero(void)
{
    char buf[32];
    lunar_fmt_age(0.0, 29.53, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "0.0"));
}

/* Moon sign: 10-12 */

void test_sign_aries(void)
{
    char buf[32];
    lunar_fmt_sign(0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Aries"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Moon"));
}

void test_sign_pisces(void)
{
    char buf[32];
    lunar_fmt_sign(11, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pisces"));
}

void test_sign_invalid(void)
{
    char buf[32];
    lunar_fmt_sign(-1, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

/* Countdown: 13-14 */

void test_countdown_new(void)
{
    char buf[32];
    lunar_fmt_countdown("New Moon", 12.3, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "New Moon"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "12.3"));
}

void test_countdown_full(void)
{
    char buf[32];
    lunar_fmt_countdown("Full Moon", 2.1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Full Moon"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2.1"));
}

/* Summary: 15-16 */

void test_summary_has_phase(void)
{
    lunar_display_t d = lunar_display(JD_J2000);
    TEST_ASSERT_TRUE(strlen(d.summary) > 10);
    /* Summary should contain a percentage */
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "%"));
}

void test_summary_has_sign(void)
{
    lunar_display_t d = lunar_display(JD_J2000);
    TEST_ASSERT_TRUE(strlen(d.moon_sign) > 0);
}

/* Illumination bar: 17-19 */

void test_bar_full(void)
{
    char buf[16];
    lunar_fmt_bar(1.0, buf, sizeof(buf), 8);
    /* All filled chars */
    TEST_ASSERT_EQUAL_INT(8, (int)strlen(buf));
}

void test_bar_empty(void)
{
    char buf[16];
    lunar_fmt_bar(0.0, buf, sizeof(buf), 8);
    TEST_ASSERT_EQUAL_INT(8, (int)strlen(buf));
}

void test_bar_half(void)
{
    char buf[16];
    lunar_fmt_bar(0.5, buf, sizeof(buf), 8);
    TEST_ASSERT_EQUAL_INT(8, (int)strlen(buf));
    /* Should have some filled and some empty */
    int filled = 0;
    for (int i = 0; i < 8; i++) {
        if (buf[i] == '#') filled++;
    }
    TEST_ASSERT_TRUE(filled >= 3 && filled <= 5);
}

/* Display fields populated: 20-21 */

void test_all_fields_populated(void)
{
    lunar_display_t d = lunar_display(JD_J2000);
    TEST_ASSERT_TRUE(strlen(d.phase_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.illumination) > 0);
    TEST_ASSERT_TRUE(strlen(d.age_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.moon_sign) > 0);
    TEST_ASSERT_TRUE(strlen(d.next_new) > 0);
    TEST_ASSERT_TRUE(strlen(d.next_full) > 0);
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
}

void test_display_at_various_jd(void)
{
    /* Test at multiple dates to ensure no crashes */
    double jds[] = {2451545.0, 2451600.0, 2451700.0, 2460000.0, 2440000.0};
    for (int i = 0; i < 5; i++) {
        lunar_display_t d = lunar_display(jds[i]);
        TEST_ASSERT_TRUE(strlen(d.phase_line) > 0);
        TEST_ASSERT_TRUE(strlen(d.summary) > 0);
    }
}

/* Purity: 22 */

void test_purity(void)
{
    lunar_display_t a = lunar_display(JD_J2000);
    lunar_display_t b = lunar_display(JD_J2000);
    TEST_ASSERT_EQUAL_STRING(a.phase_line, b.phase_line);
    TEST_ASSERT_EQUAL_STRING(a.illumination, b.illumination);
    TEST_ASSERT_EQUAL_STRING(a.summary, b.summary);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_display_new_moon);
    RUN_TEST(test_display_full_moon);
    RUN_TEST(test_display_first_quarter);
    RUN_TEST(test_illumination_zero);
    RUN_TEST(test_illumination_half);
    RUN_TEST(test_illumination_full);
    RUN_TEST(test_illumination_precision);
    RUN_TEST(test_age_format);
    RUN_TEST(test_age_day_zero);
    RUN_TEST(test_sign_aries);
    RUN_TEST(test_sign_pisces);
    RUN_TEST(test_sign_invalid);
    RUN_TEST(test_countdown_new);
    RUN_TEST(test_countdown_full);
    RUN_TEST(test_summary_has_phase);
    RUN_TEST(test_summary_has_sign);
    RUN_TEST(test_bar_full);
    RUN_TEST(test_bar_empty);
    RUN_TEST(test_bar_half);
    RUN_TEST(test_all_fields_populated);
    RUN_TEST(test_display_at_various_jd);
    RUN_TEST(test_purity);
    return UNITY_END();
}
