/* test_coptic.c — Coptic (Anno Martyrum) calendar tests
 * TDD RED phase: tests written before implementation.
 * Epoch: JD 1825029.5 = 29 Aug 284 CE (Julian) = AM 1, 1 Thout
 * 12 months x 30 days + Nasie (5 or 6 epagomenal) = 365 or 366 days.
 * Leap year when (year % 4) == 3. */

#include "../../unity/unity.h"
#include "../../../src/systems/coptic/coptic.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Epoch conversion tests ===== */

void test_epoch_to_jd(void)
{
    /* AM 1, 1 Thout = JD 1825029.5 => integer day 1825029 */
    coptic_date_t d = { .year = 1, .month = 1, .day = 1 };
    int jdn = (int)floor(coptic_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1825029, jdn);
}

void test_epoch_from_jd(void)
{
    /* JD 1825029.5 = AM 1, 1 Thout */
    coptic_date_t d = coptic_from_jd(1825029.5);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== End of year 1 (common year) ===== */

void test_last_day_of_year1(void)
{
    /* AM 1: (1 % 4 == 1) not a leap year -> Nasie has 5 days
     * Last day = day 365 = month 13, day 5
     * JD = 1825029.5 + 364 */
    coptic_date_t d = coptic_from_jd(1825029.5 + 364.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(5, d.day);
}

void test_last_day_of_year1_to_jd(void)
{
    /* AM 1, 13 Nasie day 5 = JD 1825029.5 + 364 => integer day 1825393 */
    coptic_date_t d = { .year = 1, .month = 13, .day = 5 };
    int jdn = (int)floor(coptic_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1825393, jdn);
}

/* ===== Start of year 2 ===== */

void test_first_day_of_year2(void)
{
    /* JD = 1825029.5 + 365 = 1825394.5 = AM 2, 1 Thout */
    coptic_date_t d = coptic_from_jd(1825029.5 + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_first_day_of_year2_to_jd(void)
{
    /* AM 2, 1 Thout = JD 1825029.5 + 365 => integer day 1825394 */
    coptic_date_t d = { .year = 2, .month = 1, .day = 1 };
    int jdn = (int)floor(coptic_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1825394, jdn);
}

/* ===== Leap year detection ===== */

void test_leap_year_am3(void)
{
    /* AM 3: 3 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(coptic_is_leap(3));
}

void test_leap_year_am4(void)
{
    /* AM 4: 4 % 4 == 0 -> not leap */
    TEST_ASSERT_FALSE(coptic_is_leap(4));
}

void test_leap_year_am7(void)
{
    /* AM 7: 7 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(coptic_is_leap(7));
}

void test_leap_year_am1(void)
{
    /* AM 1: 1 % 4 == 1 -> not leap */
    TEST_ASSERT_FALSE(coptic_is_leap(1));
}

void test_leap_year_am11(void)
{
    /* AM 11: 11 % 4 == 3 -> leap */
    TEST_ASSERT_TRUE(coptic_is_leap(11));
}

void test_leap_year_am2(void)
{
    /* AM 2: 2 % 4 == 2 -> not leap */
    TEST_ASSERT_FALSE(coptic_is_leap(2));
}

/* ===== Month days ===== */

void test_month_days_regular(void)
{
    /* Months 1-12 always have 30 days */
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(30, coptic_month_days(1, m));
    }
}

void test_month_days_nasie_common(void)
{
    /* Nasie in common year (AM 1) has 5 days */
    TEST_ASSERT_EQUAL_INT(5, coptic_month_days(1, 13));
}

void test_month_days_nasie_leap(void)
{
    /* Nasie in leap year (AM 3) has 6 days */
    TEST_ASSERT_EQUAL_INT(6, coptic_month_days(3, 13));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, coptic_month_days(1, 0));
    TEST_ASSERT_EQUAL_INT(0, coptic_month_days(1, 14));
    TEST_ASSERT_EQUAL_INT(0, coptic_month_days(1, -1));
}

/* ===== Year days ===== */

void test_year_days_common(void)
{
    /* AM 1: common year = 365 days */
    TEST_ASSERT_EQUAL_INT(365, coptic_year_days(1));
}

void test_year_days_leap(void)
{
    /* AM 3: leap year = 366 days */
    TEST_ASSERT_EQUAL_INT(366, coptic_year_days(3));
}

/* ===== Leap year boundary: AM 3 has 366 days ===== */

void test_am3_last_day_nasie6(void)
{
    /* AM 3 is leap -> Nasie has 6 days -> last day = Nasie 6
     * AM 1: 365 days, AM 2: 365 days = 730 days after epoch
     * AM 3, 1 Thout = epoch + 730
     * AM 3, Nasie 6 = epoch + 730 + 365 = epoch + 1095
     * Wait: need to compute properly with leap days.
     *
     * coptic_to_jd: epoch + 365*(y-1) + (y-1)/4 + 30*(m-1) + d - 1
     * AM 3, Nasie 6: epoch + 365*2 + 2/4 + 30*12 + 6 - 1
     *              = epoch + 730 + 0 + 360 + 5
     *              = epoch + 1095
     */
    coptic_date_t d = { .year = 3, .month = 13, .day = 6 };
    double jd = coptic_to_jd(d);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(3, back.year);
    TEST_ASSERT_EQUAL_INT(13, back.month);
    TEST_ASSERT_EQUAL_INT(6, back.day);
}

void test_am4_first_day_after_leap(void)
{
    /* AM 4, 1 Thout should be day after AM 3, Nasie 6
     * coptic_to_jd(AM 3, 13, 6) + 1 == coptic_to_jd(AM 4, 1, 1) */
    coptic_date_t leap_end = { .year = 3, .month = 13, .day = 6 };
    coptic_date_t next_year = { .year = 4, .month = 1, .day = 1 };
    double diff = coptic_to_jd(next_year) - coptic_to_jd(leap_end);
    TEST_ASSERT_TRUE(diff > 0.999 && diff < 1.001);
}

/* ===== Month names ===== */

void test_month_name_thout(void)
{
    TEST_ASSERT_EQUAL_STRING("Thout", coptic_month_name(1));
}

void test_month_name_paopi(void)
{
    TEST_ASSERT_EQUAL_STRING("Paopi", coptic_month_name(2));
}

void test_month_name_hathor(void)
{
    TEST_ASSERT_EQUAL_STRING("Hathor", coptic_month_name(3));
}

void test_month_name_koiak(void)
{
    TEST_ASSERT_EQUAL_STRING("Koiak", coptic_month_name(4));
}

void test_month_name_tobi(void)
{
    TEST_ASSERT_EQUAL_STRING("Tobi", coptic_month_name(5));
}

void test_month_name_meshir(void)
{
    TEST_ASSERT_EQUAL_STRING("Meshir", coptic_month_name(6));
}

void test_month_name_paremhat(void)
{
    TEST_ASSERT_EQUAL_STRING("Paremhat", coptic_month_name(7));
}

void test_month_name_parmouti(void)
{
    TEST_ASSERT_EQUAL_STRING("Parmouti", coptic_month_name(8));
}

void test_month_name_pashons(void)
{
    TEST_ASSERT_EQUAL_STRING("Pashons", coptic_month_name(9));
}

void test_month_name_paoni(void)
{
    TEST_ASSERT_EQUAL_STRING("Paoni", coptic_month_name(10));
}

void test_month_name_epip(void)
{
    TEST_ASSERT_EQUAL_STRING("Epip", coptic_month_name(11));
}

void test_month_name_mesori(void)
{
    TEST_ASSERT_EQUAL_STRING("Mesori", coptic_month_name(12));
}

void test_month_name_nasie(void)
{
    TEST_ASSERT_EQUAL_STRING("Nasie", coptic_month_name(13));
}

void test_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", coptic_month_name(0));
    TEST_ASSERT_EQUAL_STRING("?", coptic_month_name(14));
    TEST_ASSERT_EQUAL_STRING("?", coptic_month_name(-1));
}

/* ===== Feast detection ===== */

void test_feast_nayrouz(void)
{
    /* Nayrouz: month 1, day 1 (Coptic New Year) */
    coptic_date_t d = { .year = 1740, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_NAYROUZ, coptic_feast(d));
}

void test_feast_cross(void)
{
    /* Feast of the Cross: month 1, day 17 */
    coptic_date_t d = { .year = 1740, .month = 1, .day = 17 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_CROSS, coptic_feast(d));
}

void test_feast_christmas(void)
{
    /* Coptic Christmas: month 4, day 29 */
    coptic_date_t d = { .year = 1740, .month = 4, .day = 29 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_CHRISTMAS, coptic_feast(d));
}

void test_feast_epiphany(void)
{
    /* Theophany/Epiphany: month 5, day 11 */
    coptic_date_t d = { .year = 1740, .month = 5, .day = 11 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_EPIPHANY, coptic_feast(d));
}

void test_feast_annunciation(void)
{
    /* Annunciation: month 7, day 29 */
    coptic_date_t d = { .year = 1740, .month = 7, .day = 29 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_ANNUNCIATION, coptic_feast(d));
}

void test_feast_none_normal_day(void)
{
    /* Normal day, no feast */
    coptic_date_t d = { .year = 1740, .month = 3, .day = 15 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_NONE, coptic_feast(d));
}

void test_feast_none_nasie(void)
{
    /* Nasie day, no feast */
    coptic_date_t d = { .year = 1740, .month = 13, .day = 3 };
    TEST_ASSERT_EQUAL_INT(COPTIC_FEAST_NONE, coptic_feast(d));
}

/* ===== Feast names ===== */

void test_feast_name_nayrouz(void)
{
    TEST_ASSERT_EQUAL_STRING("Nayrouz", coptic_feast_name(COPTIC_FEAST_NAYROUZ));
}

void test_feast_name_cross(void)
{
    TEST_ASSERT_EQUAL_STRING("Feast of the Cross", coptic_feast_name(COPTIC_FEAST_CROSS));
}

void test_feast_name_christmas(void)
{
    TEST_ASSERT_EQUAL_STRING("Christmas", coptic_feast_name(COPTIC_FEAST_CHRISTMAS));
}

void test_feast_name_epiphany(void)
{
    TEST_ASSERT_EQUAL_STRING("Epiphany", coptic_feast_name(COPTIC_FEAST_EPIPHANY));
}

void test_feast_name_annunciation(void)
{
    TEST_ASSERT_EQUAL_STRING("Annunciation", coptic_feast_name(COPTIC_FEAST_ANNUNCIATION));
}

void test_feast_name_easter(void)
{
    /* Easter is movable — returns name but feast detection won't produce it */
    TEST_ASSERT_EQUAL_STRING("Easter", coptic_feast_name(COPTIC_FEAST_EASTER));
}

void test_feast_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", coptic_feast_name(COPTIC_FEAST_NONE));
}

void test_feast_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", coptic_feast_name(COPTIC_FEAST_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", coptic_feast_name(-1));
}

/* ===== J2000.0 approximate test ===== */

void test_j2000_approximate(void)
{
    /* JD 2451545.0 (1 Jan 2000 CE, 12:00 UT)
     * days = 626515
     * year = (4*626515 + 1463) / 1461 = 1716
     * day_in_year = 626515 - (365*1715 + 1716/4) + 1
     *            = 626515 - (625975 + 429) + 1 = 112
     * month = (112-1)/30 + 1 = 4 (Koiak)
     * day = 112 - 90 = 22
     * So: AM 1716, 22 Koiak */
    coptic_date_t d = coptic_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(1716, d.year);
    TEST_ASSERT_EQUAL_INT(4, d.month);
    TEST_ASSERT_EQUAL_INT(22, d.day);
}

/* ===== Roundtrip tests ===== */

void test_roundtrip_epoch(void)
{
    coptic_date_t orig = { .year = 1, .month = 1, .day = 1 };
    double jd = coptic_to_jd(orig);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_nasie(void)
{
    coptic_date_t orig = { .year = 500, .month = 13, .day = 3 };
    double jd = coptic_to_jd(orig);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_mid_year(void)
{
    coptic_date_t orig = { .year = 1000, .month = 7, .day = 15 };
    double jd = coptic_to_jd(orig);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_last_regular_day(void)
{
    coptic_date_t orig = { .year = 1740, .month = 12, .day = 30 };
    double jd = coptic_to_jd(orig);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_leap_nasie6(void)
{
    /* AM 3 is leap -> Nasie day 6 must roundtrip */
    coptic_date_t orig = { .year = 3, .month = 13, .day = 6 };
    double jd = coptic_to_jd(orig);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_large_year(void)
{
    coptic_date_t orig = { .year = 1742, .month = 9, .day = 22 };
    double jd = coptic_to_jd(orig);
    coptic_date_t back = coptic_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

/* ===== Day of year ===== */

void test_day_of_year_first(void)
{
    coptic_date_t d = { .year = 1, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(1, coptic_day_of_year(d));
}

void test_day_of_year_month2(void)
{
    /* month 2, day 1 = day 31 */
    coptic_date_t d = { .year = 1, .month = 2, .day = 1 };
    TEST_ASSERT_EQUAL_INT(31, coptic_day_of_year(d));
}

void test_day_of_year_last_regular(void)
{
    /* month 12, day 30 = day 360 */
    coptic_date_t d = { .year = 1, .month = 12, .day = 30 };
    TEST_ASSERT_EQUAL_INT(360, coptic_day_of_year(d));
}

void test_day_of_year_nasie1(void)
{
    /* month 13, day 1 = day 361 */
    coptic_date_t d = { .year = 1, .month = 13, .day = 1 };
    TEST_ASSERT_EQUAL_INT(361, coptic_day_of_year(d));
}

void test_day_of_year_nasie5(void)
{
    /* month 13, day 5 = day 365 */
    coptic_date_t d = { .year = 1, .month = 13, .day = 5 };
    TEST_ASSERT_EQUAL_INT(365, coptic_day_of_year(d));
}

void test_day_of_year_nasie6_leap(void)
{
    /* month 13, day 6 in a leap year = day 366 */
    coptic_date_t d = { .year = 3, .month = 13, .day = 6 };
    TEST_ASSERT_EQUAL_INT(366, coptic_day_of_year(d));
}

/* ===== coptic_year convenience ===== */

void test_coptic_year_epoch(void)
{
    TEST_ASSERT_EQUAL_INT(1, coptic_year(1825029.5));
}

void test_coptic_year_j2000(void)
{
    TEST_ASSERT_EQUAL_INT(1716, coptic_year(2451545.0));
}

/* ===== Mid-month and boundary tests ===== */

void test_mid_year_date(void)
{
    /* AM 1, day 31 = month 2, day 1 (Paopi) */
    coptic_date_t d = coptic_from_jd(1825029.5 + 30.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_nasie_day1(void)
{
    /* AM 1: day 361 = first Nasie day (month 13, day 1)
     * JD = 1825029.5 + 360 */
    coptic_date_t d = coptic_from_jd(1825029.5 + 360.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
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
    RUN_TEST(test_mid_year_date);
    RUN_TEST(test_nasie_day1);

    /* Leap year detection */
    RUN_TEST(test_leap_year_am3);
    RUN_TEST(test_leap_year_am4);
    RUN_TEST(test_leap_year_am7);
    RUN_TEST(test_leap_year_am1);
    RUN_TEST(test_leap_year_am11);
    RUN_TEST(test_leap_year_am2);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_nasie_common);
    RUN_TEST(test_month_days_nasie_leap);
    RUN_TEST(test_month_days_invalid);

    /* Year days */
    RUN_TEST(test_year_days_common);
    RUN_TEST(test_year_days_leap);

    /* Leap boundary */
    RUN_TEST(test_am3_last_day_nasie6);
    RUN_TEST(test_am4_first_day_after_leap);

    /* Month names */
    RUN_TEST(test_month_name_thout);
    RUN_TEST(test_month_name_paopi);
    RUN_TEST(test_month_name_hathor);
    RUN_TEST(test_month_name_koiak);
    RUN_TEST(test_month_name_tobi);
    RUN_TEST(test_month_name_meshir);
    RUN_TEST(test_month_name_paremhat);
    RUN_TEST(test_month_name_parmouti);
    RUN_TEST(test_month_name_pashons);
    RUN_TEST(test_month_name_paoni);
    RUN_TEST(test_month_name_epip);
    RUN_TEST(test_month_name_mesori);
    RUN_TEST(test_month_name_nasie);
    RUN_TEST(test_month_name_invalid);

    /* Feast detection */
    RUN_TEST(test_feast_nayrouz);
    RUN_TEST(test_feast_cross);
    RUN_TEST(test_feast_christmas);
    RUN_TEST(test_feast_epiphany);
    RUN_TEST(test_feast_annunciation);
    RUN_TEST(test_feast_none_normal_day);
    RUN_TEST(test_feast_none_nasie);

    /* Feast names */
    RUN_TEST(test_feast_name_nayrouz);
    RUN_TEST(test_feast_name_cross);
    RUN_TEST(test_feast_name_christmas);
    RUN_TEST(test_feast_name_epiphany);
    RUN_TEST(test_feast_name_annunciation);
    RUN_TEST(test_feast_name_easter);
    RUN_TEST(test_feast_name_none);
    RUN_TEST(test_feast_name_invalid);

    /* J2000.0 */
    RUN_TEST(test_j2000_approximate);

    /* Roundtrips */
    RUN_TEST(test_roundtrip_epoch);
    RUN_TEST(test_roundtrip_nasie);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_last_regular_day);
    RUN_TEST(test_roundtrip_leap_nasie6);
    RUN_TEST(test_roundtrip_large_year);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_month2);
    RUN_TEST(test_day_of_year_last_regular);
    RUN_TEST(test_day_of_year_nasie1);
    RUN_TEST(test_day_of_year_nasie5);
    RUN_TEST(test_day_of_year_nasie6_leap);

    /* Convenience functions */
    RUN_TEST(test_coptic_year_epoch);
    RUN_TEST(test_coptic_year_j2000);

    return UNITY_END();
}
