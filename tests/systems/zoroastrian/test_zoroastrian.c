/* test_zoroastrian.c — Yazdgerdi (Zoroastrian) calendar tests
 * TDD RED phase: tests written before implementation.
 * Epoch: JD 1952063.5 = 16 June 632 CE (Julian) = Year 1, 1 Farvardin
 * 12 months x 30 days + 5 Gatha epagomenal days = 365 days, no leap. */

#include "../../unity/unity.h"
#include "../../../src/systems/zoroastrian/zoroastrian.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Epoch conversion tests ===== */

void test_epoch_to_jd(void)
{
    /* Year 1, 1 Farvardin = JD 1952063.5 => integer day 1952063 */
    zoro_date_t d = { .year = 1, .month = 1, .day = 1 };
    int jdn = (int)floor(zoro_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1952063, jdn);
}

void test_epoch_from_jd(void)
{
    /* JD 1952063.5 = Year 1, 1 Farvardin */
    zoro_date_t d = zoro_from_jd(1952063.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== End of year 1 ===== */

void test_last_day_of_year1(void)
{
    /* Day 365 of year 1 = Gatha day 5 (month 13, day 5) */
    /* JD = 1952063.5 + 364 = 1952427.5 */
    zoro_date_t d = zoro_from_jd(1952063.5 + 364.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(5, d.day);
}

void test_last_day_of_year1_to_jd(void)
{
    /* Gatha day 5 of year 1 = JD 1952063.5 + 364 => integer day 1952427 */
    zoro_date_t d = { .year = 1, .month = 13, .day = 5 };
    int jdn = (int)floor(zoro_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1952427, jdn);
}

/* ===== Start of year 2 ===== */

void test_first_day_of_year2(void)
{
    /* JD = 1952063.5 + 365 = 1952428.5 = Year 2, 1 Farvardin */
    zoro_date_t d = zoro_from_jd(1952063.5 + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_first_day_of_year2_to_jd(void)
{
    /* Year 2, 1 Farvardin = JD 1952063.5 + 365 => integer day 1952428 */
    zoro_date_t d = { .year = 2, .month = 1, .day = 1 };
    int jdn = (int)floor(zoro_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1952428, jdn);
}

/* ===== J2000.0 test ===== */

void test_j2000(void)
{
    /* JD 2451545.0 (1 Jan 2000 CE)
     * days_since_epoch = 2451545.0 - 1952063.5 = 499481.5
     * floor = 499481
     * year = 499481 / 365 + 1 = 1368 + 1 = 1369
     * day_of_year = 499481 % 365 + 1 = 499481 - 1368*365 + 1
     *            = 499481 - 499320 + 1 = 162
     * month = (162-1)/30 + 1 = 161/30 + 1 = 5 + 1 = 6
     * day = (162-1)%30 + 1 = 161%30 + 1 = 11 + 1 = 12 */
    zoro_date_t d = zoro_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(1369, d.year);
    TEST_ASSERT_EQUAL_INT(6, d.month);
    TEST_ASSERT_EQUAL_INT(12, d.day);
}

/* ===== Mid-month test ===== */

void test_mid_year_date(void)
{
    /* Year 1, day 31 = month 2, day 1 (Ordibehesht) */
    zoro_date_t d = zoro_from_jd(1952063.5 + 30.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_gatha_day1(void)
{
    /* Day 361 of year = first Gatha day (month 13, day 1)
     * JD = 1952063.5 + 360 */
    zoro_date_t d = zoro_from_jd(1952063.5 + 360.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Month names ===== */

void test_month_name_farvardin(void)
{
    TEST_ASSERT_EQUAL_STRING("Farvardin", zoro_month_name(1));
}

void test_month_name_ordibehesht(void)
{
    TEST_ASSERT_EQUAL_STRING("Ordibehesht", zoro_month_name(2));
}

void test_month_name_khordad(void)
{
    TEST_ASSERT_EQUAL_STRING("Khordad", zoro_month_name(3));
}

void test_month_name_tir(void)
{
    TEST_ASSERT_EQUAL_STRING("Tir", zoro_month_name(4));
}

void test_month_name_amordad(void)
{
    TEST_ASSERT_EQUAL_STRING("Amordad", zoro_month_name(5));
}

void test_month_name_shahrivar(void)
{
    TEST_ASSERT_EQUAL_STRING("Shahrivar", zoro_month_name(6));
}

void test_month_name_mehr(void)
{
    TEST_ASSERT_EQUAL_STRING("Mehr", zoro_month_name(7));
}

void test_month_name_aban(void)
{
    TEST_ASSERT_EQUAL_STRING("Aban", zoro_month_name(8));
}

void test_month_name_azar(void)
{
    TEST_ASSERT_EQUAL_STRING("Azar", zoro_month_name(9));
}

void test_month_name_dey(void)
{
    TEST_ASSERT_EQUAL_STRING("Dey", zoro_month_name(10));
}

void test_month_name_bahman(void)
{
    TEST_ASSERT_EQUAL_STRING("Bahman", zoro_month_name(11));
}

void test_month_name_esfand(void)
{
    TEST_ASSERT_EQUAL_STRING("Esfand", zoro_month_name(12));
}

void test_month_name_gatha(void)
{
    TEST_ASSERT_EQUAL_STRING("Gatha", zoro_month_name(13));
}

void test_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", zoro_month_name(0));
    TEST_ASSERT_EQUAL_STRING("?", zoro_month_name(14));
    TEST_ASSERT_EQUAL_STRING("?", zoro_month_name(-1));
}

/* ===== Day names ===== */

void test_day_name_hormozd(void)
{
    TEST_ASSERT_EQUAL_STRING("Hormozd", zoro_day_name(1));
}

void test_day_name_aneran(void)
{
    TEST_ASSERT_EQUAL_STRING("Aneran", zoro_day_name(30));
}

void test_day_name_mid(void)
{
    TEST_ASSERT_EQUAL_STRING("Mehr", zoro_day_name(16));
}

void test_day_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", zoro_day_name(0));
    TEST_ASSERT_EQUAL_STRING("?", zoro_day_name(31));
    TEST_ASSERT_EQUAL_STRING("?", zoro_day_name(-1));
}

/* ===== Gatha day names ===== */

void test_gatha_name_1(void)
{
    TEST_ASSERT_EQUAL_STRING("Ahunavad", zoro_gatha_name(1));
}

void test_gatha_name_5(void)
{
    TEST_ASSERT_EQUAL_STRING("Vahishtoisht", zoro_gatha_name(5));
}

void test_gatha_name_3(void)
{
    TEST_ASSERT_EQUAL_STRING("Spentamad", zoro_gatha_name(3));
}

void test_gatha_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", zoro_gatha_name(0));
    TEST_ASSERT_EQUAL_STRING("?", zoro_gatha_name(6));
}

/* ===== Month days ===== */

void test_month_days_regular(void)
{
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(30, zoro_month_days(m));
    }
}

void test_month_days_gatha(void)
{
    TEST_ASSERT_EQUAL_INT(5, zoro_month_days(13));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, zoro_month_days(0));
    TEST_ASSERT_EQUAL_INT(0, zoro_month_days(14));
}

/* ===== Festival detection ===== */

void test_festival_nowruz(void)
{
    zoro_date_t d = { .year = 1394, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_NOWRUZ, zoro_festival(d));
}

void test_festival_gahanbar1(void)
{
    /* Maidyozarem: month 2, day 15 */
    zoro_date_t d = { .year = 1394, .month = 2, .day = 15 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_GAHANBAR_1, zoro_festival(d));
}

void test_festival_gahanbar2(void)
{
    /* Maidyoshem: month 4, day 15 */
    zoro_date_t d = { .year = 1394, .month = 4, .day = 15 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_GAHANBAR_2, zoro_festival(d));
}

void test_festival_gahanbar3(void)
{
    /* Paitishem: month 6, day 30 */
    zoro_date_t d = { .year = 1394, .month = 6, .day = 30 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_GAHANBAR_3, zoro_festival(d));
}

void test_festival_gahanbar4(void)
{
    /* Ayathrem: month 7, day 30 */
    zoro_date_t d = { .year = 1394, .month = 7, .day = 30 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_GAHANBAR_4, zoro_festival(d));
}

void test_festival_gahanbar5(void)
{
    /* Maidyarem: month 10, day 20 */
    zoro_date_t d = { .year = 1394, .month = 10, .day = 20 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_GAHANBAR_5, zoro_festival(d));
}

void test_festival_gahanbar6_all_gatha(void)
{
    /* Hamaspathmaidyem: month 13, all 5 Gatha days */
    for (int g = 1; g <= 5; g++) {
        zoro_date_t d = { .year = 1394, .month = 13, .day = g };
        TEST_ASSERT_EQUAL_INT(ZORO_FEST_GAHANBAR_6, zoro_festival(d));
    }
}

void test_festival_yalda(void)
{
    /* Yalda: month 10, day 1 */
    zoro_date_t d = { .year = 1394, .month = 10, .day = 1 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_YALDA, zoro_festival(d));
}

void test_festival_sadeh(void)
{
    /* Sadeh: month 11, day 10 */
    zoro_date_t d = { .year = 1394, .month = 11, .day = 10 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_SADEH, zoro_festival(d));
}

void test_festival_none(void)
{
    /* Normal day */
    zoro_date_t d = { .year = 1394, .month = 3, .day = 15 };
    TEST_ASSERT_EQUAL_INT(ZORO_FEST_NONE, zoro_festival(d));
}

/* ===== Festival names ===== */

void test_festival_name_nowruz(void)
{
    TEST_ASSERT_EQUAL_STRING("Nowruz", zoro_festival_name(ZORO_FEST_NOWRUZ));
}

void test_festival_name_gahanbar1(void)
{
    TEST_ASSERT_EQUAL_STRING("Maidyozarem", zoro_festival_name(ZORO_FEST_GAHANBAR_1));
}

void test_festival_name_gahanbar6(void)
{
    TEST_ASSERT_EQUAL_STRING("Hamaspathmaidyem", zoro_festival_name(ZORO_FEST_GAHANBAR_6));
}

void test_festival_name_yalda(void)
{
    TEST_ASSERT_EQUAL_STRING("Yalda", zoro_festival_name(ZORO_FEST_YALDA));
}

void test_festival_name_sadeh(void)
{
    TEST_ASSERT_EQUAL_STRING("Sadeh", zoro_festival_name(ZORO_FEST_SADEH));
}

void test_festival_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", zoro_festival_name(ZORO_FEST_NONE));
}

void test_festival_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", zoro_festival_name(ZORO_FEST_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", zoro_festival_name(-1));
}

/* ===== Roundtrip tests ===== */

void test_roundtrip_epoch(void)
{
    zoro_date_t orig = { .year = 1, .month = 1, .day = 1 };
    double jd = zoro_to_jd(orig);
    zoro_date_t back = zoro_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_gatha(void)
{
    zoro_date_t orig = { .year = 500, .month = 13, .day = 3 };
    double jd = zoro_to_jd(orig);
    zoro_date_t back = zoro_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_mid_year(void)
{
    zoro_date_t orig = { .year = 1000, .month = 7, .day = 15 };
    double jd = zoro_to_jd(orig);
    zoro_date_t back = zoro_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_last_regular_day(void)
{
    zoro_date_t orig = { .year = 1394, .month = 12, .day = 30 };
    double jd = zoro_to_jd(orig);
    zoro_date_t back = zoro_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

/* ===== zoro_year convenience ===== */

void test_zoro_year_epoch(void)
{
    TEST_ASSERT_EQUAL_INT(1, zoro_year(1952063.5));
}

void test_zoro_year_j2000(void)
{
    TEST_ASSERT_EQUAL_INT(1369, zoro_year(2451545.0));
}

/* ===== zoro_is_gatha ===== */

void test_is_gatha_true(void)
{
    zoro_date_t d = { .year = 1, .month = 13, .day = 3 };
    TEST_ASSERT_TRUE(zoro_is_gatha(d));
}

void test_is_gatha_false(void)
{
    zoro_date_t d = { .year = 1, .month = 6, .day = 15 };
    TEST_ASSERT_FALSE(zoro_is_gatha(d));
}

/* ===== zoro_day_of_year ===== */

void test_day_of_year_first(void)
{
    zoro_date_t d = { .year = 1, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(1, zoro_day_of_year(d));
}

void test_day_of_year_month2(void)
{
    /* month 2, day 1 = day 31 */
    zoro_date_t d = { .year = 1, .month = 2, .day = 1 };
    TEST_ASSERT_EQUAL_INT(31, zoro_day_of_year(d));
}

void test_day_of_year_last_regular(void)
{
    /* month 12, day 30 = day 360 */
    zoro_date_t d = { .year = 1, .month = 12, .day = 30 };
    TEST_ASSERT_EQUAL_INT(360, zoro_day_of_year(d));
}

void test_day_of_year_gatha1(void)
{
    /* month 13, day 1 = day 361 */
    zoro_date_t d = { .year = 1, .month = 13, .day = 1 };
    TEST_ASSERT_EQUAL_INT(361, zoro_day_of_year(d));
}

void test_day_of_year_gatha5(void)
{
    /* month 13, day 5 = day 365 */
    zoro_date_t d = { .year = 1, .month = 13, .day = 5 };
    TEST_ASSERT_EQUAL_INT(365, zoro_day_of_year(d));
}

/* ===== All 30 day names coverage ===== */

void test_all_day_names(void)
{
    static const char *expected[] = {
        "Hormozd", "Bahman", "Ordibehesht", "Shahrivar", "Sepandarmaz",
        "Khordad", "Amordad", "Dey-be-Azar", "Azar", "Aban",
        "Khorshed", "Mah", "Tir", "Gosh", "Dey-be-Mehr",
        "Mehr", "Sorush", "Rashn", "Farvardin", "Bahram",
        "Ram", "Bad", "Dey-be-Din", "Din", "Ard",
        "Ashtad", "Asman", "Zamyad", "Mahraspand", "Aneran"
    };
    for (int i = 1; i <= 30; i++) {
        TEST_ASSERT_EQUAL_STRING(expected[i - 1], zoro_day_name(i));
    }
}

/* ===== All 5 Gatha names coverage ===== */

void test_all_gatha_names(void)
{
    static const char *expected[] = {
        "Ahunavad", "Ushtavad", "Spentamad", "Vohukhshathra", "Vahishtoisht"
    };
    for (int i = 1; i <= 5; i++) {
        TEST_ASSERT_EQUAL_STRING(expected[i - 1], zoro_gatha_name(i));
    }
}

/* ===== Approximate year 1392 (Feb 2023) ===== */

void test_approx_2023(void)
{
    /* JD 2460000.0 ~ Feb 2023
     * days_since_epoch = floor(2460000.0 - 1952063.5) = 507936
     * year = 507936 / 365 + 1 = 1391 + 1 = 1392
     * day_of_year = 507936 % 365 + 1 = 507936 - 1391*365 + 1
     *            = 507936 - 507715 + 1 = 222
     * month = (222-1)/30 + 1 = 221/30 + 1 = 7 + 1 = 8
     * day = (222-1)%30 + 1 = 221%30 + 1 = 11 + 1 = 12 */
    zoro_date_t d = zoro_from_jd(2460000.0);
    TEST_ASSERT_EQUAL_INT(1392, d.year);
    TEST_ASSERT_EQUAL_INT(8, d.month);
    TEST_ASSERT_EQUAL_INT(12, d.day);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Epoch and basic conversions */
    RUN_TEST(test_epoch_to_jd);
    RUN_TEST(test_epoch_from_jd);
    RUN_TEST(test_last_day_of_year1);
    RUN_TEST(test_last_day_of_year1_to_jd);
    RUN_TEST(test_first_day_of_year2);
    RUN_TEST(test_first_day_of_year2_to_jd);
    RUN_TEST(test_j2000);
    RUN_TEST(test_mid_year_date);
    RUN_TEST(test_gatha_day1);
    RUN_TEST(test_approx_2023);

    /* Month names */
    RUN_TEST(test_month_name_farvardin);
    RUN_TEST(test_month_name_ordibehesht);
    RUN_TEST(test_month_name_khordad);
    RUN_TEST(test_month_name_tir);
    RUN_TEST(test_month_name_amordad);
    RUN_TEST(test_month_name_shahrivar);
    RUN_TEST(test_month_name_mehr);
    RUN_TEST(test_month_name_aban);
    RUN_TEST(test_month_name_azar);
    RUN_TEST(test_month_name_dey);
    RUN_TEST(test_month_name_bahman);
    RUN_TEST(test_month_name_esfand);
    RUN_TEST(test_month_name_gatha);
    RUN_TEST(test_month_name_invalid);

    /* Day names */
    RUN_TEST(test_day_name_hormozd);
    RUN_TEST(test_day_name_aneran);
    RUN_TEST(test_day_name_mid);
    RUN_TEST(test_day_name_invalid);
    RUN_TEST(test_all_day_names);

    /* Gatha names */
    RUN_TEST(test_gatha_name_1);
    RUN_TEST(test_gatha_name_5);
    RUN_TEST(test_gatha_name_3);
    RUN_TEST(test_gatha_name_invalid);
    RUN_TEST(test_all_gatha_names);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_gatha);
    RUN_TEST(test_month_days_invalid);

    /* Festivals */
    RUN_TEST(test_festival_nowruz);
    RUN_TEST(test_festival_gahanbar1);
    RUN_TEST(test_festival_gahanbar2);
    RUN_TEST(test_festival_gahanbar3);
    RUN_TEST(test_festival_gahanbar4);
    RUN_TEST(test_festival_gahanbar5);
    RUN_TEST(test_festival_gahanbar6_all_gatha);
    RUN_TEST(test_festival_yalda);
    RUN_TEST(test_festival_sadeh);
    RUN_TEST(test_festival_none);

    /* Festival names */
    RUN_TEST(test_festival_name_nowruz);
    RUN_TEST(test_festival_name_gahanbar1);
    RUN_TEST(test_festival_name_gahanbar6);
    RUN_TEST(test_festival_name_yalda);
    RUN_TEST(test_festival_name_sadeh);
    RUN_TEST(test_festival_name_none);
    RUN_TEST(test_festival_name_invalid);

    /* Roundtrips */
    RUN_TEST(test_roundtrip_epoch);
    RUN_TEST(test_roundtrip_gatha);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_last_regular_day);

    /* Convenience functions */
    RUN_TEST(test_zoro_year_epoch);
    RUN_TEST(test_zoro_year_j2000);
    RUN_TEST(test_is_gatha_true);
    RUN_TEST(test_is_gatha_false);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_month2);
    RUN_TEST(test_day_of_year_last_regular);
    RUN_TEST(test_day_of_year_gatha1);
    RUN_TEST(test_day_of_year_gatha5);

    return UNITY_END();
}
