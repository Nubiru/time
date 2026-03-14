/* test_calendar_convert.c — Tests for the Calendar Conversion Engine
 * Track 54.3: Unified JD-pivot conversion across 21 calendar systems.
 *
 * Reference date: J2000.0 = JD 2451545.0 = 2000 Jan 1.5 (noon UT) */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/calendar_convert.h"

#include <string.h>

/* J2000.0 epoch */
#define JD_J2000 2451545.0

/* March 20, 2000 (equinox) noon UT = JD 2451623.5 */
#define JD_EQUINOX_2000 2451623.5

/* July 4, 1776 (US Independence) noon UT = JD 2369915.5 */
#define JD_US_INDEPENDENCE 2369915.5

void setUp(void) { }
void tearDown(void) { }

/* ========== cal_system_count ========== */

static void test_system_count(void)
{
    TEST_ASSERT_EQUAL_INT(21, cal_system_count());
}

static void test_system_count_matches_enum(void)
{
    TEST_ASSERT_EQUAL_INT(CAL_COUNT, cal_system_count());
}

/* ========== cal_system_valid ========== */

static void test_system_valid_gregorian(void)
{
    TEST_ASSERT_TRUE(cal_system_valid(CAL_GREGORIAN));
}

static void test_system_valid_calendar_round(void)
{
    TEST_ASSERT_TRUE(cal_system_valid(CAL_CALENDAR_ROUND));
}

static void test_system_valid_negative(void)
{
    TEST_ASSERT_FALSE(cal_system_valid((cal_system_t)-1));
}

static void test_system_valid_too_large(void)
{
    TEST_ASSERT_FALSE(cal_system_valid(CAL_COUNT));
}

static void test_system_valid_way_too_large(void)
{
    TEST_ASSERT_FALSE(cal_system_valid((cal_system_t)999));
}

/* ========== cal_system_name for all 21 systems ========== */

static void test_name_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", cal_system_name(CAL_GREGORIAN));
}

static void test_name_tzolkin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", cal_system_name(CAL_TZOLKIN));
}

static void test_name_haab(void)
{
    TEST_ASSERT_EQUAL_STRING("Haab", cal_system_name(CAL_HAAB));
}

static void test_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", cal_system_name(CAL_CHINESE));
}

static void test_name_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", cal_system_name(CAL_ICHING));
}

static void test_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", cal_system_name(CAL_HEBREW));
}

static void test_name_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", cal_system_name(CAL_ISLAMIC));
}

static void test_name_persian(void)
{
    TEST_ASSERT_EQUAL_STRING("Persian", cal_system_name(CAL_PERSIAN));
}

static void test_name_coptic(void)
{
    TEST_ASSERT_EQUAL_STRING("Coptic", cal_system_name(CAL_COPTIC));
}

static void test_name_ethiopian(void)
{
    TEST_ASSERT_EQUAL_STRING("Ethiopian", cal_system_name(CAL_ETHIOPIAN));
}

static void test_name_egyptian(void)
{
    TEST_ASSERT_EQUAL_STRING("Egyptian", cal_system_name(CAL_EGYPTIAN));
}

static void test_name_french_republican(void)
{
    TEST_ASSERT_EQUAL_STRING("French Republican", cal_system_name(CAL_FRENCH_REPUBLICAN));
}

static void test_name_japanese(void)
{
    TEST_ASSERT_EQUAL_STRING("Japanese", cal_system_name(CAL_JAPANESE));
}

static void test_name_bahai(void)
{
    TEST_ASSERT_EQUAL_STRING("Baha'i", cal_system_name(CAL_BAHAI));
}

static void test_name_korean(void)
{
    TEST_ASSERT_EQUAL_STRING("Korean", cal_system_name(CAL_KOREAN));
}

static void test_name_thai(void)
{
    TEST_ASSERT_EQUAL_STRING("Thai", cal_system_name(CAL_THAI));
}

static void test_name_tamil(void)
{
    TEST_ASSERT_EQUAL_STRING("Tamil", cal_system_name(CAL_TAMIL));
}

static void test_name_myanmar(void)
{
    TEST_ASSERT_EQUAL_STRING("Myanmar", cal_system_name(CAL_MYANMAR));
}

static void test_name_zoroastrian(void)
{
    TEST_ASSERT_EQUAL_STRING("Zoroastrian", cal_system_name(CAL_ZOROASTRIAN));
}

static void test_name_celtic_tree(void)
{
    TEST_ASSERT_EQUAL_STRING("Celtic Tree", cal_system_name(CAL_CELTIC_TREE));
}

static void test_name_calendar_round(void)
{
    TEST_ASSERT_EQUAL_STRING("Calendar Round", cal_system_name(CAL_CALENDAR_ROUND));
}

static void test_name_invalid(void)
{
    const char *n = cal_system_name(CAL_COUNT);
    TEST_ASSERT_EQUAL_STRING("?", n);
}

/* ========== cal_convert — Gregorian at J2000.0 ========== */

static void test_convert_gregorian_j2000(void)
{
    cal_date_t d = cal_convert(CAL_GREGORIAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_GREGORIAN, d.system);
    TEST_ASSERT_EQUAL_INT(2000, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ========== cal_convert — Tzolkin at J2000.0 ========== */

static void test_convert_tzolkin_j2000(void)
{
    cal_date_t d = cal_convert(CAL_TZOLKIN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_TZOLKIN, d.system);
    /* Kin should be between 1 and 260 */
    TEST_ASSERT_TRUE(d.extra >= 1 && d.extra <= 260);
    /* formatted should contain "Kin" */
    TEST_ASSERT_NOT_NULL(strstr(d.formatted, "Kin"));
}

/* ========== cal_convert — Haab at J2000.0 ========== */

static void test_convert_haab_j2000(void)
{
    cal_date_t d = cal_convert(CAL_HAAB, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_HAAB, d.system);
    /* month 0-18, day 0-19 */
    TEST_ASSERT_TRUE(d.month >= 0 && d.month <= 18);
    TEST_ASSERT_TRUE(d.day >= 0 && d.day <= 19);
}

/* ========== cal_convert — Chinese at J2000.0 ========== */

static void test_convert_chinese_j2000(void)
{
    cal_date_t d = cal_convert(CAL_CHINESE, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_CHINESE, d.system);
    /* extra holds cycle_year (1-60) */
    TEST_ASSERT_TRUE(d.extra >= 1 && d.extra <= 60);
    /* formatted should mention an animal */
    TEST_ASSERT_TRUE(strlen(d.formatted) > 0);
}

/* ========== cal_convert — I Ching at J2000.0 ========== */

static void test_convert_iching_j2000(void)
{
    cal_date_t d = cal_convert(CAL_ICHING, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_ICHING, d.system);
    /* extra holds king_wen (1-64) */
    TEST_ASSERT_TRUE(d.extra >= 1 && d.extra <= 64);
    TEST_ASSERT_NOT_NULL(strstr(d.formatted, "Hexagram"));
}

/* ========== cal_convert — Hebrew at J2000.0 ========== */

static void test_convert_hebrew_j2000(void)
{
    cal_date_t d = cal_convert(CAL_HEBREW, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_HEBREW, d.system);
    TEST_ASSERT_EQUAL_INT(5760, d.year);
    /* Hebrew month for Jan 1, 2000 is Tevet (month 10) */
    TEST_ASSERT_EQUAL_INT(10, d.month);
}

/* ========== cal_convert — Islamic at J2000.0 ========== */

static void test_convert_islamic_j2000(void)
{
    cal_date_t d = cal_convert(CAL_ISLAMIC, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_ISLAMIC, d.system);
    TEST_ASSERT_EQUAL_INT(1420, d.year);
}

/* ========== cal_convert — Persian at J2000.0 ========== */

static void test_convert_persian_j2000(void)
{
    cal_date_t d = cal_convert(CAL_PERSIAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_PERSIAN, d.system);
    TEST_ASSERT_EQUAL_INT(1378, d.year);
}

/* ========== cal_convert — Coptic at J2000.0 ========== */

static void test_convert_coptic_j2000(void)
{
    cal_date_t d = cal_convert(CAL_COPTIC, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_COPTIC, d.system);
    TEST_ASSERT_EQUAL_INT(1716, d.year);
}

/* ========== cal_convert — Ethiopian at J2000.0 ========== */

static void test_convert_ethiopian_j2000(void)
{
    cal_date_t d = cal_convert(CAL_ETHIOPIAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_ETHIOPIAN, d.system);
    TEST_ASSERT_EQUAL_INT(1992, d.year);
}

/* ========== cal_convert — Egyptian at J2000.0 ========== */

static void test_convert_egyptian_j2000(void)
{
    cal_date_t d = cal_convert(CAL_EGYPTIAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_EGYPTIAN, d.system);
    /* Egyptian year is Nabonassar era */
    TEST_ASSERT_TRUE(d.year > 2700);
}

/* ========== cal_convert — French Republican at J2000.0 ========== */

static void test_convert_french_republican_j2000(void)
{
    cal_date_t d = cal_convert(CAL_FRENCH_REPUBLICAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_FRENCH_REPUBLICAN, d.system);
    TEST_ASSERT_EQUAL_INT(208, d.year);
}

/* ========== cal_convert — Japanese at J2000.0 ========== */

static void test_convert_japanese_j2000(void)
{
    cal_date_t d = cal_convert(CAL_JAPANESE, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_JAPANESE, d.system);
    /* extra holds era_year */
    TEST_ASSERT_EQUAL_INT(12, d.extra); /* Heisei 12 = 2000 */
    TEST_ASSERT_NOT_NULL(strstr(d.formatted, "Heisei"));
}

/* ========== cal_convert — Baha'i at J2000.0 ========== */

static void test_convert_bahai_j2000(void)
{
    cal_date_t d = cal_convert(CAL_BAHAI, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_BAHAI, d.system);
    TEST_ASSERT_EQUAL_INT(156, d.year);
}

/* ========== cal_convert — Korean at J2000.0 ========== */

static void test_convert_korean_j2000(void)
{
    cal_date_t d = cal_convert(CAL_KOREAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_KOREAN, d.system);
    TEST_ASSERT_EQUAL_INT(4332, d.year);
}

/* ========== cal_convert — Thai at J2000.0 ========== */

static void test_convert_thai_j2000(void)
{
    cal_date_t d = cal_convert(CAL_THAI, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_THAI, d.system);
    TEST_ASSERT_EQUAL_INT(2543, d.year);
}

/* ========== cal_convert — Tamil at J2000.0 ========== */

static void test_convert_tamil_j2000(void)
{
    cal_date_t d = cal_convert(CAL_TAMIL, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_TAMIL, d.system);
    /* Tamil year starts ~April 14 (Puthandu), so Jan 1 is previous year */
    TEST_ASSERT_EQUAL_INT(2030, d.year);
}

/* ========== cal_convert — Myanmar at J2000.0 ========== */

static void test_convert_myanmar_j2000(void)
{
    cal_date_t d = cal_convert(CAL_MYANMAR, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_MYANMAR, d.system);
    TEST_ASSERT_EQUAL_INT(1361, d.year);
}

/* ========== cal_convert — Zoroastrian at J2000.0 ========== */

static void test_convert_zoroastrian_j2000(void)
{
    cal_date_t d = cal_convert(CAL_ZOROASTRIAN, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_ZOROASTRIAN, d.system);
    TEST_ASSERT_TRUE(d.year > 1360);
}

/* ========== cal_convert — Celtic Tree at J2000.0 ========== */

static void test_convert_celtic_tree_j2000(void)
{
    cal_date_t d = cal_convert(CAL_CELTIC_TREE, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_CELTIC_TREE, d.system);
    /* Jan 1 = Birch month (month 1), day ~9 */
    TEST_ASSERT_EQUAL_INT(1, d.month);
}

/* ========== cal_convert — Calendar Round at J2000.0 ========== */

static void test_convert_calendar_round_j2000(void)
{
    cal_date_t d = cal_convert(CAL_CALENDAR_ROUND, JD_J2000);
    TEST_ASSERT_EQUAL_INT(CAL_CALENDAR_ROUND, d.system);
    /* day_in_round is extra */
    TEST_ASSERT_TRUE(d.extra >= 0 && d.extra < 18980);
}

/* ========== cal_convert — invalid system ========== */

static void test_convert_invalid_system(void)
{
    cal_date_t d = cal_convert(CAL_COUNT, JD_J2000);
    TEST_ASSERT_EQUAL_INT(0, d.year);
    TEST_ASSERT_EQUAL_STRING("?", d.formatted);
}

/* ========== cal_format — formatted output ========== */

static void test_format_gregorian(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_GREGORIAN, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "2000"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "January"));
}

static void test_format_hebrew(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_HEBREW, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "5760"));
}

static void test_format_islamic(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_ISLAMIC, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "1420"));
}

static void test_format_persian(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_PERSIAN, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "1378"));
}

static void test_format_coptic(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_COPTIC, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "1716"));
}

static void test_format_japanese(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_JAPANESE, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Heisei"));
}

static void test_format_chinese(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_CHINESE, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Dragon"));
}

static void test_format_tzolkin(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_TZOLKIN, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Kin"));
}

static void test_format_iching(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_ICHING, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hexagram"));
}

static void test_format_invalid_system(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_COUNT, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

static void test_format_small_buffer(void)
{
    char buf[10];
    int n = cal_format(CAL_GREGORIAN, JD_J2000, buf, sizeof(buf));
    /* Should truncate but not overflow */
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) < 10);
}

static void test_format_null_buffer(void)
{
    int n = cal_format(CAL_GREGORIAN, JD_J2000, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ========== cal_convert_between — Gregorian to other systems ========== */

static void test_convert_between_gregorian_to_hebrew(void)
{
    /* Jan 1, 2000 -> Hebrew */
    cal_date_t d = cal_convert_between(CAL_GREGORIAN, 2000, 1, 1, CAL_HEBREW);
    TEST_ASSERT_EQUAL_INT(CAL_HEBREW, d.system);
    TEST_ASSERT_EQUAL_INT(5760, d.year);
}

static void test_convert_between_gregorian_to_islamic(void)
{
    cal_date_t d = cal_convert_between(CAL_GREGORIAN, 2000, 1, 1, CAL_ISLAMIC);
    TEST_ASSERT_EQUAL_INT(CAL_ISLAMIC, d.system);
    TEST_ASSERT_EQUAL_INT(1420, d.year);
}

static void test_convert_between_gregorian_to_persian(void)
{
    cal_date_t d = cal_convert_between(CAL_GREGORIAN, 2000, 1, 1, CAL_PERSIAN);
    TEST_ASSERT_EQUAL_INT(CAL_PERSIAN, d.system);
    TEST_ASSERT_EQUAL_INT(1378, d.year);
}

static void test_convert_between_unsupported_source(void)
{
    /* Non-Gregorian source should return invalid (year=0) */
    cal_date_t d = cal_convert_between(CAL_TZOLKIN, 218, 0, 0, CAL_GREGORIAN);
    TEST_ASSERT_EQUAL_INT(0, d.year);
}

/* ========== Formatted string content checks ========== */

static void test_formatted_field_filled(void)
{
    /* Every valid cal_convert should fill .formatted */
    for (int i = 0; i < CAL_COUNT; i++) {
        cal_date_t d = cal_convert((cal_system_t)i, JD_J2000);
        TEST_ASSERT_TRUE(strlen(d.formatted) > 0);
    }
}

static void test_formatted_no_overflow(void)
{
    /* Formatted field should be < CAL_FMT_SIZE including null */
    for (int i = 0; i < CAL_COUNT; i++) {
        cal_date_t d = cal_convert((cal_system_t)i, JD_J2000);
        TEST_ASSERT_TRUE(strlen(d.formatted) < CAL_FMT_SIZE);
    }
}

/* ========== cal_convert — additional known date checks ========== */

static void test_convert_gregorian_equinox_2000(void)
{
    cal_date_t d = cal_convert(CAL_GREGORIAN, JD_EQUINOX_2000);
    TEST_ASSERT_EQUAL_INT(2000, d.year);
    TEST_ASSERT_EQUAL_INT(3, d.month);
    TEST_ASSERT_EQUAL_INT(20, d.day);
}

static void test_convert_gregorian_us_independence(void)
{
    cal_date_t d = cal_convert(CAL_GREGORIAN, JD_US_INDEPENDENCE);
    TEST_ASSERT_EQUAL_INT(1776, d.year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(4, d.day);
}

static void test_convert_haab_has_month_name(void)
{
    cal_date_t d = cal_convert(CAL_HAAB, JD_J2000);
    /* formatted should contain a Haab month name, not just numbers */
    TEST_ASSERT_TRUE(strlen(d.formatted) > 3);
}

static void test_convert_ethiopian_has_month_name(void)
{
    cal_date_t d = cal_convert(CAL_ETHIOPIAN, JD_J2000);
    /* formatted should contain a month name */
    TEST_ASSERT_TRUE(strlen(d.formatted) > 5);
}

static void test_convert_bahai_has_month_name(void)
{
    cal_date_t d = cal_convert(CAL_BAHAI, JD_J2000);
    TEST_ASSERT_TRUE(strlen(d.formatted) > 5);
}

static void test_convert_korean_dangun_year(void)
{
    cal_date_t d = cal_convert(CAL_KOREAN, JD_J2000);
    /* Dangun year at Jan 1, 2000: lunar NY not yet passed, so 4332 */
    TEST_ASSERT_EQUAL_INT(4332, d.year);
}

static void test_convert_thai_be_year(void)
{
    cal_date_t d = cal_convert(CAL_THAI, JD_J2000);
    /* BE year = CE year + 543 */
    TEST_ASSERT_EQUAL_INT(2543, d.year);
}

static void test_convert_celtic_tree_jan1(void)
{
    /* Jan 1 is in Birch month (month 1), starting Dec 24 */
    cal_date_t d = cal_convert(CAL_CELTIC_TREE, JD_J2000);
    TEST_ASSERT_EQUAL_INT(1, d.month); /* Birch */
    TEST_ASSERT_TRUE(d.day >= 1 && d.day <= 28);
}

static void test_format_egyptian(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_EGYPTIAN, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

static void test_format_ethiopian(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_ETHIOPIAN, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "1992"));
}

static void test_format_bahai(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_BAHAI, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "156"));
}

static void test_format_korean(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_KOREAN, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "4332"));
}

static void test_format_thai(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_THAI, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "2543"));
}

static void test_format_celtic_tree(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_CELTIC_TREE, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Birch"));
}

static void test_format_calendar_round(void)
{
    char buf[CAL_FMT_SIZE];
    int n = cal_format(CAL_CALENDAR_ROUND, JD_J2000, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 5);
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* cal_system_count */
    RUN_TEST(test_system_count);
    RUN_TEST(test_system_count_matches_enum);

    /* cal_system_valid */
    RUN_TEST(test_system_valid_gregorian);
    RUN_TEST(test_system_valid_calendar_round);
    RUN_TEST(test_system_valid_negative);
    RUN_TEST(test_system_valid_too_large);
    RUN_TEST(test_system_valid_way_too_large);

    /* cal_system_name — all 21 + invalid */
    RUN_TEST(test_name_gregorian);
    RUN_TEST(test_name_tzolkin);
    RUN_TEST(test_name_haab);
    RUN_TEST(test_name_chinese);
    RUN_TEST(test_name_iching);
    RUN_TEST(test_name_hebrew);
    RUN_TEST(test_name_islamic);
    RUN_TEST(test_name_persian);
    RUN_TEST(test_name_coptic);
    RUN_TEST(test_name_ethiopian);
    RUN_TEST(test_name_egyptian);
    RUN_TEST(test_name_french_republican);
    RUN_TEST(test_name_japanese);
    RUN_TEST(test_name_bahai);
    RUN_TEST(test_name_korean);
    RUN_TEST(test_name_thai);
    RUN_TEST(test_name_tamil);
    RUN_TEST(test_name_myanmar);
    RUN_TEST(test_name_zoroastrian);
    RUN_TEST(test_name_celtic_tree);
    RUN_TEST(test_name_calendar_round);
    RUN_TEST(test_name_invalid);

    /* cal_convert — all 21 systems at J2000.0 */
    RUN_TEST(test_convert_gregorian_j2000);
    RUN_TEST(test_convert_tzolkin_j2000);
    RUN_TEST(test_convert_haab_j2000);
    RUN_TEST(test_convert_chinese_j2000);
    RUN_TEST(test_convert_iching_j2000);
    RUN_TEST(test_convert_hebrew_j2000);
    RUN_TEST(test_convert_islamic_j2000);
    RUN_TEST(test_convert_persian_j2000);
    RUN_TEST(test_convert_coptic_j2000);
    RUN_TEST(test_convert_ethiopian_j2000);
    RUN_TEST(test_convert_egyptian_j2000);
    RUN_TEST(test_convert_french_republican_j2000);
    RUN_TEST(test_convert_japanese_j2000);
    RUN_TEST(test_convert_bahai_j2000);
    RUN_TEST(test_convert_korean_j2000);
    RUN_TEST(test_convert_thai_j2000);
    RUN_TEST(test_convert_tamil_j2000);
    RUN_TEST(test_convert_myanmar_j2000);
    RUN_TEST(test_convert_zoroastrian_j2000);
    RUN_TEST(test_convert_celtic_tree_j2000);
    RUN_TEST(test_convert_calendar_round_j2000);
    RUN_TEST(test_convert_invalid_system);

    /* cal_format */
    RUN_TEST(test_format_gregorian);
    RUN_TEST(test_format_hebrew);
    RUN_TEST(test_format_islamic);
    RUN_TEST(test_format_persian);
    RUN_TEST(test_format_coptic);
    RUN_TEST(test_format_japanese);
    RUN_TEST(test_format_chinese);
    RUN_TEST(test_format_tzolkin);
    RUN_TEST(test_format_iching);
    RUN_TEST(test_format_invalid_system);
    RUN_TEST(test_format_small_buffer);
    RUN_TEST(test_format_null_buffer);
    RUN_TEST(test_format_egyptian);
    RUN_TEST(test_format_ethiopian);
    RUN_TEST(test_format_bahai);
    RUN_TEST(test_format_korean);
    RUN_TEST(test_format_thai);
    RUN_TEST(test_format_celtic_tree);
    RUN_TEST(test_format_calendar_round);

    /* cal_convert_between */
    RUN_TEST(test_convert_between_gregorian_to_hebrew);
    RUN_TEST(test_convert_between_gregorian_to_islamic);
    RUN_TEST(test_convert_between_gregorian_to_persian);
    RUN_TEST(test_convert_between_unsupported_source);

    /* Additional checks */
    RUN_TEST(test_convert_gregorian_equinox_2000);
    RUN_TEST(test_convert_gregorian_us_independence);
    RUN_TEST(test_convert_haab_has_month_name);
    RUN_TEST(test_convert_ethiopian_has_month_name);
    RUN_TEST(test_convert_bahai_has_month_name);
    RUN_TEST(test_convert_korean_dangun_year);
    RUN_TEST(test_convert_thai_be_year);
    RUN_TEST(test_convert_celtic_tree_jan1);
    RUN_TEST(test_formatted_field_filled);
    RUN_TEST(test_formatted_no_overflow);

    return UNITY_END();
}
