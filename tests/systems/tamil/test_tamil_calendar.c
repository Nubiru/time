/* test_tamil_calendar.c — Tamil sidereal solar calendar tests
 * TDD RED phase: tests written before implementation.
 *
 * Tamil solar calendar: 12 months aligned to sidereal zodiac signs.
 * Thiruvalluvar era: year 1 = 31 BCE. CE offset: TV = CE + 31.
 * Puthandu (Tamil New Year): Sun enters sidereal Aries (~April 14).
 * 60-year Jovian (Brihaspati) cycle.
 *
 * Sources: traditional Tamil panchangam, Lahiri ayanamsa. */

#include "../../unity/unity.h"
#include "../../../src/systems/tamil/tamil_calendar.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Sidereal solar longitude tests ===== */

void test_sidereal_sun_j2000(void)
{
    /* JD 2451545.0 = Jan 1, 2000 12:00 UT
     * Mean tropical: ~280.46 deg
     * Lahiri ayanamsa at J2000 ~ 23.85 deg
     * Sidereal ~ 280.46 - 23.85 = ~256.6 deg (Sagittarius) */
    double sid = tamil_sidereal_sun(2451545.0);
    TEST_ASSERT_TRUE(sid >= 250.0 && sid <= 270.0);
}

void test_sidereal_sun_in_range(void)
{
    /* Any date should produce 0-360 */
    double sid = tamil_sidereal_sun(2460000.0);
    TEST_ASSERT_TRUE(sid >= 0.0 && sid < 360.0);
}

void test_sidereal_sun_different_century(void)
{
    /* JD 2415020.0 = Jan 0, 1900, 12:00 UT
     * Should still produce valid sidereal longitude */
    double sid = tamil_sidereal_sun(2415020.0);
    TEST_ASSERT_TRUE(sid >= 0.0 && sid < 360.0);
}

void test_sidereal_sun_puthandu_approx(void)
{
    /* Around April 14, 2000: Sun should be near sidereal 0 deg (Aries) */
    double jd_apr14 = 2451649.0; /* ~April 14, 2000 */
    double sid = tamil_sidereal_sun(jd_apr14);
    /* Should be near 0 or near 360 (Aries cusp) — within ~15 deg */
    TEST_ASSERT_TRUE(sid < 15.0 || sid > 345.0);
}

void test_sidereal_sun_increases_over_year(void)
{
    /* Over ~30 days, sidereal Sun should advance ~30 degrees */
    double sid1 = tamil_sidereal_sun(2451545.0);
    double sid2 = tamil_sidereal_sun(2451545.0 + 30.0);
    double diff = sid2 - sid1;
    if (diff < 0.0) diff += 360.0;
    TEST_ASSERT_TRUE(diff > 25.0 && diff < 35.0);
}

/* ===== Month from JD tests ===== */

void test_month_from_jd_chittirai(void)
{
    /* Around April 20, 2000: Sun in sidereal Aries = Chittirai (month 1) */
    tamil_date_t d = tamil_from_jd(2451655.0);
    TEST_ASSERT_EQUAL_INT(1, d.month);
}

void test_month_from_jd_thai(void)
{
    /* Around mid-January: Sun in sidereal Capricorn = Thai (month 10) */
    /* Jan 15, 2000 = JD 2451559.0 approximately */
    tamil_date_t d = tamil_from_jd(2451559.0);
    TEST_ASSERT_EQUAL_INT(10, d.month);
}

void test_month_from_jd_avani(void)
{
    /* Around mid-August: Sun in sidereal Leo = Avani (month 5) */
    /* Aug 20, 2000 = JD ~2451777.0 */
    tamil_date_t d = tamil_from_jd(2451777.0);
    TEST_ASSERT_EQUAL_INT(5, d.month);
}

void test_month_from_jd_margazhi(void)
{
    /* Around mid-December: Sun in sidereal Sagittarius = Margazhi (month 9) */
    /* Dec 20, 2000 = JD ~2451899.0 */
    tamil_date_t d = tamil_from_jd(2451899.0);
    TEST_ASSERT_EQUAL_INT(9, d.month);
}

void test_month_range_always_valid(void)
{
    /* Month should always be 1-12 */
    tamil_date_t d = tamil_from_jd(2451545.0);
    TEST_ASSERT_TRUE(d.month >= 1 && d.month <= 12);
}

/* ===== Year from JD tests ===== */

void test_year_from_jd_2000(void)
{
    /* CE 2000 after Puthandu -> TV 2031 */
    tamil_date_t d = tamil_from_jd(2451660.0); /* late April 2000 */
    TEST_ASSERT_EQUAL_INT(2031, d.year);
}

void test_year_from_jd_2026(void)
{
    /* CE 2026 after Puthandu -> TV 2057 */
    /* April 20, 2026 = JD ~2461149.0 */
    tamil_date_t d = tamil_from_jd(2461149.0);
    TEST_ASSERT_EQUAL_INT(2057, d.year);
}

void test_year_from_jd_1900(void)
{
    /* CE 1900 after Puthandu -> TV 1931 */
    /* April 20, 1900 = JD ~2415128.0 */
    tamil_date_t d = tamil_from_jd(2415128.0);
    TEST_ASSERT_EQUAL_INT(1931, d.year);
}

void test_year_from_jd_before_puthandu(void)
{
    /* Jan 15, 2000 is before Puthandu 2000 -> still in TV 2030 */
    tamil_date_t d = tamil_from_jd(2451559.0);
    TEST_ASSERT_EQUAL_INT(2030, d.year);
}

void test_year_from_jd_1987(void)
{
    /* 1987 CE after Puthandu -> TV 2018 (reference year for Jovian cycle) */
    /* April 20, 1987 = JD ~2446907.0 */
    tamil_date_t d = tamil_from_jd(2446907.0);
    TEST_ASSERT_EQUAL_INT(2018, d.year);
}

/* ===== JD round-trip tests ===== */

void test_roundtrip_chittirai_1(void)
{
    tamil_date_t orig = { .year = 2031, .month = 1, .day = 1, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_mid_year(void)
{
    tamil_date_t orig = { .year = 2057, .month = 6, .day = 15, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_panguni(void)
{
    tamil_date_t orig = { .year = 2031, .month = 12, .day = 20, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_thai(void)
{
    tamil_date_t orig = { .year = 2050, .month = 10, .day = 5, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_year_boundary(void)
{
    /* Last month of a year */
    tamil_date_t orig = { .year = 2040, .month = 12, .day = 30, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_first_day(void)
{
    /* First day of a Tamil year */
    tamil_date_t orig = { .year = 2020, .month = 1, .day = 1, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_aadi(void)
{
    tamil_date_t orig = { .year = 2045, .month = 4, .day = 10, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_karthigai(void)
{
    tamil_date_t orig = { .year = 2035, .month = 8, .day = 25, .jovian = 0 };
    double jd = tamil_to_jd(orig);
    tamil_date_t back = tamil_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

/* ===== Month names tests ===== */

void test_month_name_chittirai(void)
{
    TEST_ASSERT_EQUAL_STRING("Chittirai", tamil_month_name(1));
}

void test_month_name_vaigasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Vaigasi", tamil_month_name(2));
}

void test_month_name_aani(void)
{
    TEST_ASSERT_EQUAL_STRING("Aani", tamil_month_name(3));
}

void test_month_name_aadi(void)
{
    TEST_ASSERT_EQUAL_STRING("Aadi", tamil_month_name(4));
}

void test_month_name_avani(void)
{
    TEST_ASSERT_EQUAL_STRING("Avani", tamil_month_name(5));
}

void test_month_name_purattasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Purattasi", tamil_month_name(6));
}

void test_month_name_aippasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Aippasi", tamil_month_name(7));
}

void test_month_name_karthigai(void)
{
    TEST_ASSERT_EQUAL_STRING("Karthigai", tamil_month_name(8));
}

void test_month_name_margazhi(void)
{
    TEST_ASSERT_EQUAL_STRING("Margazhi", tamil_month_name(9));
}

void test_month_name_thai(void)
{
    TEST_ASSERT_EQUAL_STRING("Thai", tamil_month_name(10));
}

void test_month_name_maasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Maasi", tamil_month_name(11));
}

void test_month_name_panguni(void)
{
    TEST_ASSERT_EQUAL_STRING("Panguni", tamil_month_name(12));
}

void test_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tamil_month_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tamil_month_name(13));
    TEST_ASSERT_EQUAL_STRING("?", tamil_month_name(-1));
}

/* ===== Month meanings tests ===== */

void test_month_meaning_chittirai(void)
{
    TEST_ASSERT_EQUAL_STRING("Beautiful/Brilliant", tamil_month_meaning(1));
}

void test_month_meaning_vaigasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Brightness", tamil_month_meaning(2));
}

void test_month_meaning_aani(void)
{
    TEST_ASSERT_EQUAL_STRING("Twins", tamil_month_meaning(3));
}

void test_month_meaning_aadi(void)
{
    TEST_ASSERT_EQUAL_STRING("Beginning/First", tamil_month_meaning(4));
}

void test_month_meaning_avani(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", tamil_month_meaning(5));
}

void test_month_meaning_purattasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Dew/Cloud", tamil_month_meaning(6));
}

void test_month_meaning_aippasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Deepening", tamil_month_meaning(7));
}

void test_month_meaning_karthigai(void)
{
    TEST_ASSERT_EQUAL_STRING("Pleiades", tamil_month_meaning(8));
}

void test_month_meaning_margazhi(void)
{
    TEST_ASSERT_EQUAL_STRING("Path", tamil_month_meaning(9));
}

void test_month_meaning_thai(void)
{
    TEST_ASSERT_EQUAL_STRING("Auspicious", tamil_month_meaning(10));
}

void test_month_meaning_maasi(void)
{
    TEST_ASSERT_EQUAL_STRING("Measure", tamil_month_meaning(11));
}

void test_month_meaning_panguni(void)
{
    TEST_ASSERT_EQUAL_STRING("Flowering", tamil_month_meaning(12));
}

void test_month_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tamil_month_meaning(0));
    TEST_ASSERT_EQUAL_STRING("?", tamil_month_meaning(13));
    TEST_ASSERT_EQUAL_STRING("?", tamil_month_meaning(-1));
}

/* ===== Month days tests ===== */

void test_month_days_chittirai(void)
{
    TEST_ASSERT_EQUAL_INT(31, tamil_month_days(1));
}

void test_month_days_vaigasi(void)
{
    TEST_ASSERT_EQUAL_INT(31, tamil_month_days(2));
}

void test_month_days_aani(void)
{
    TEST_ASSERT_EQUAL_INT(32, tamil_month_days(3));
}

void test_month_days_aadi(void)
{
    TEST_ASSERT_EQUAL_INT(31, tamil_month_days(4));
}

void test_month_days_avani(void)
{
    TEST_ASSERT_EQUAL_INT(31, tamil_month_days(5));
}

void test_month_days_purattasi(void)
{
    TEST_ASSERT_EQUAL_INT(30, tamil_month_days(6));
}

void test_month_days_aippasi(void)
{
    TEST_ASSERT_EQUAL_INT(30, tamil_month_days(7));
}

void test_month_days_karthigai(void)
{
    TEST_ASSERT_EQUAL_INT(29, tamil_month_days(8));
}

void test_month_days_margazhi(void)
{
    TEST_ASSERT_EQUAL_INT(30, tamil_month_days(9));
}

void test_month_days_thai(void)
{
    TEST_ASSERT_EQUAL_INT(29, tamil_month_days(10));
}

void test_month_days_maasi(void)
{
    TEST_ASSERT_EQUAL_INT(30, tamil_month_days(11));
}

void test_month_days_panguni(void)
{
    TEST_ASSERT_EQUAL_INT(31, tamil_month_days(12));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tamil_month_days(0));
    TEST_ASSERT_EQUAL_INT(0, tamil_month_days(13));
    TEST_ASSERT_EQUAL_INT(0, tamil_month_days(-1));
}

/* ===== Jovian cycle name tests ===== */

void test_jovian_name_first(void)
{
    TEST_ASSERT_EQUAL_STRING("Prabhava", tamil_jovian_name(1));
}

void test_jovian_name_last(void)
{
    TEST_ASSERT_EQUAL_STRING("Akshaya", tamil_jovian_name(60));
}

void test_jovian_name_middle(void)
{
    TEST_ASSERT_EQUAL_STRING("Durmukhi", tamil_jovian_name(30));
}

void test_jovian_name_27th(void)
{
    TEST_ASSERT_EQUAL_STRING("Vijaya", tamil_jovian_name(27));
}

void test_jovian_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tamil_jovian_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tamil_jovian_name(61));
    TEST_ASSERT_EQUAL_STRING("?", tamil_jovian_name(-1));
}

/* ===== Jovian year computation tests ===== */

void test_jovian_year_1987(void)
{
    /* 1987 CE = TV 2018 = Prabhava (year 1 of cycle) */
    int tv = tamil_thiruvalluvar_from_ce(1987);
    TEST_ASSERT_EQUAL_INT(1, tamil_jovian_year(tv));
}

void test_jovian_year_2026(void)
{
    /* 2026 CE = TV 2057 = 2057 - 2018 = 39 offset, (39 % 60) + 1 = 40
     * Year 40 = Parabhava */
    int tv = tamil_thiruvalluvar_from_ce(2026);
    TEST_ASSERT_EQUAL_INT(40, tamil_jovian_year(tv));
}

void test_jovian_year_2047(void)
{
    /* 2047 CE = TV 2078. (2078-2018) % 60 + 1 = 60 + 1 = 61? No.
     * (2078-2018) = 60, 60%60=0, 0+1=1 => Prabhava again (full cycle) */
    int tv = tamil_thiruvalluvar_from_ce(2047);
    TEST_ASSERT_EQUAL_INT(1, tamil_jovian_year(tv));
}

void test_jovian_year_wraps(void)
{
    /* 1988 CE = TV 2019. (2019-2018)%60 + 1 = 2 = Vibhava */
    int tv = tamil_thiruvalluvar_from_ce(1988);
    TEST_ASSERT_EQUAL_INT(2, tamil_jovian_year(tv));
}

void test_jovian_year_before_reference(void)
{
    /* 1986 CE = TV 2017. (2017-2018) = -1.
     * Need proper modulo: ((-1 % 60) + 60) % 60 + 1 = 60 = Akshaya */
    int tv = tamil_thiruvalluvar_from_ce(1986);
    TEST_ASSERT_EQUAL_INT(60, tamil_jovian_year(tv));
}

/* ===== Thiruvalluvar <-> CE conversion tests ===== */

void test_tv_from_ce_2000(void)
{
    TEST_ASSERT_EQUAL_INT(2031, tamil_thiruvalluvar_from_ce(2000));
}

void test_ce_from_tv_2031(void)
{
    TEST_ASSERT_EQUAL_INT(2000, tamil_ce_from_thiruvalluvar(2031));
}

void test_tv_ce_roundtrip(void)
{
    int ce = 1947;
    int tv = tamil_thiruvalluvar_from_ce(ce);
    int back = tamil_ce_from_thiruvalluvar(tv);
    TEST_ASSERT_EQUAL_INT(ce, back);
}

void test_tv_from_ce_negative(void)
{
    /* CE 0 (1 BCE) = TV 31 */
    TEST_ASSERT_EQUAL_INT(31, tamil_thiruvalluvar_from_ce(0));
}

/* ===== Festival detection tests ===== */

void test_festival_puthandu(void)
{
    tamil_date_t d = { .year = 2057, .month = 1, .day = 1, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_PUTHANDU, tamil_festival(d));
}

void test_festival_pongal(void)
{
    tamil_date_t d = { .year = 2057, .month = 10, .day = 1, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_PONGAL, tamil_festival(d));
}

void test_festival_chithirai(void)
{
    tamil_date_t d = { .year = 2057, .month = 1, .day = 15, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_CHITHIRAI_FEST, tamil_festival(d));
}

void test_festival_deepavali(void)
{
    tamil_date_t d = { .year = 2057, .month = 7, .day = 15, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_DEEPAVALI, tamil_festival(d));
}

void test_festival_none_normal_day(void)
{
    tamil_date_t d = { .year = 2057, .month = 3, .day = 10, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_NONE, tamil_festival(d));
}

void test_festival_none_month2(void)
{
    tamil_date_t d = { .year = 2057, .month = 2, .day = 1, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_NONE, tamil_festival(d));
}

void test_festival_puthandu_different_year(void)
{
    tamil_date_t d = { .year = 2020, .month = 1, .day = 1, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_PUTHANDU, tamil_festival(d));
}

void test_festival_pongal_different_year(void)
{
    tamil_date_t d = { .year = 2020, .month = 10, .day = 1, .jovian = 0 };
    TEST_ASSERT_EQUAL_INT(TAMIL_FEST_PONGAL, tamil_festival(d));
}

/* ===== Festival name tests ===== */

void test_festival_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("", tamil_festival_name(TAMIL_FEST_NONE));
}

void test_festival_name_puthandu(void)
{
    TEST_ASSERT_EQUAL_STRING("Puthandu", tamil_festival_name(TAMIL_FEST_PUTHANDU));
}

void test_festival_name_pongal(void)
{
    TEST_ASSERT_EQUAL_STRING("Thai Pongal", tamil_festival_name(TAMIL_FEST_PONGAL));
}

void test_festival_name_chithirai(void)
{
    TEST_ASSERT_EQUAL_STRING("Chithirai Thiruvizha", tamil_festival_name(TAMIL_FEST_CHITHIRAI_FEST));
}

void test_festival_name_deepavali(void)
{
    TEST_ASSERT_EQUAL_STRING("Deepavali", tamil_festival_name(TAMIL_FEST_DEEPAVALI));
}

void test_festival_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tamil_festival_name(TAMIL_FEST_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", tamil_festival_name(-1));
}

/* ===== tamil_to_jd basic tests ===== */

void test_to_jd_produces_valid_jd(void)
{
    /* Tamil year 2031 month 1 day 1 should be around April 14, 2000 */
    tamil_date_t d = { .year = 2031, .month = 1, .day = 1, .jovian = 0 };
    double jd = tamil_to_jd(d);
    /* April 14, 2000 ~ JD 2451649 */
    TEST_ASSERT_TRUE(fabs(jd - 2451649.0) < 5.0);
}

void test_to_jd_month_progression(void)
{
    /* Month 2 day 1 should be ~31 days after month 1 day 1 */
    tamil_date_t d1 = { .year = 2031, .month = 1, .day = 1, .jovian = 0 };
    tamil_date_t d2 = { .year = 2031, .month = 2, .day = 1, .jovian = 0 };
    double diff = tamil_to_jd(d2) - tamil_to_jd(d1);
    TEST_ASSERT_TRUE(fabs(diff - 31.0) < 1.0);
}

void test_to_jd_year_length(void)
{
    /* Year to year should be ~365.25 days */
    tamil_date_t d1 = { .year = 2031, .month = 1, .day = 1, .jovian = 0 };
    tamil_date_t d2 = { .year = 2032, .month = 1, .day = 1, .jovian = 0 };
    double diff = tamil_to_jd(d2) - tamil_to_jd(d1);
    TEST_ASSERT_TRUE(fabs(diff - 365.0) < 1.0);
}

/* ===== tamil_from_jd populates jovian field ===== */

void test_from_jd_populates_jovian(void)
{
    /* 1987 CE (Puthandu) = TV 2018, Jovian = 1 (Prabhava) */
    tamil_date_t d = tamil_from_jd(2446907.0); /* ~April 20, 1987 */
    TEST_ASSERT_TRUE(d.jovian >= 1 && d.jovian <= 60);
}

void test_from_jd_day_range(void)
{
    /* Day should be 1-32 for any date */
    tamil_date_t d = tamil_from_jd(2451545.0);
    TEST_ASSERT_TRUE(d.day >= 1 && d.day <= 32);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Sidereal solar longitude */
    RUN_TEST(test_sidereal_sun_j2000);
    RUN_TEST(test_sidereal_sun_in_range);
    RUN_TEST(test_sidereal_sun_different_century);
    RUN_TEST(test_sidereal_sun_puthandu_approx);
    RUN_TEST(test_sidereal_sun_increases_over_year);

    /* Month from JD */
    RUN_TEST(test_month_from_jd_chittirai);
    RUN_TEST(test_month_from_jd_thai);
    RUN_TEST(test_month_from_jd_avani);
    RUN_TEST(test_month_from_jd_margazhi);
    RUN_TEST(test_month_range_always_valid);

    /* Year from JD */
    RUN_TEST(test_year_from_jd_2000);
    RUN_TEST(test_year_from_jd_2026);
    RUN_TEST(test_year_from_jd_1900);
    RUN_TEST(test_year_from_jd_before_puthandu);
    RUN_TEST(test_year_from_jd_1987);

    /* JD round-trips */
    RUN_TEST(test_roundtrip_chittirai_1);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_panguni);
    RUN_TEST(test_roundtrip_thai);
    RUN_TEST(test_roundtrip_year_boundary);
    RUN_TEST(test_roundtrip_first_day);
    RUN_TEST(test_roundtrip_aadi);
    RUN_TEST(test_roundtrip_karthigai);

    /* Month names */
    RUN_TEST(test_month_name_chittirai);
    RUN_TEST(test_month_name_vaigasi);
    RUN_TEST(test_month_name_aani);
    RUN_TEST(test_month_name_aadi);
    RUN_TEST(test_month_name_avani);
    RUN_TEST(test_month_name_purattasi);
    RUN_TEST(test_month_name_aippasi);
    RUN_TEST(test_month_name_karthigai);
    RUN_TEST(test_month_name_margazhi);
    RUN_TEST(test_month_name_thai);
    RUN_TEST(test_month_name_maasi);
    RUN_TEST(test_month_name_panguni);
    RUN_TEST(test_month_name_invalid);

    /* Month meanings */
    RUN_TEST(test_month_meaning_chittirai);
    RUN_TEST(test_month_meaning_vaigasi);
    RUN_TEST(test_month_meaning_aani);
    RUN_TEST(test_month_meaning_aadi);
    RUN_TEST(test_month_meaning_avani);
    RUN_TEST(test_month_meaning_purattasi);
    RUN_TEST(test_month_meaning_aippasi);
    RUN_TEST(test_month_meaning_karthigai);
    RUN_TEST(test_month_meaning_margazhi);
    RUN_TEST(test_month_meaning_thai);
    RUN_TEST(test_month_meaning_maasi);
    RUN_TEST(test_month_meaning_panguni);
    RUN_TEST(test_month_meaning_invalid);

    /* Month days */
    RUN_TEST(test_month_days_chittirai);
    RUN_TEST(test_month_days_vaigasi);
    RUN_TEST(test_month_days_aani);
    RUN_TEST(test_month_days_aadi);
    RUN_TEST(test_month_days_avani);
    RUN_TEST(test_month_days_purattasi);
    RUN_TEST(test_month_days_aippasi);
    RUN_TEST(test_month_days_karthigai);
    RUN_TEST(test_month_days_margazhi);
    RUN_TEST(test_month_days_thai);
    RUN_TEST(test_month_days_maasi);
    RUN_TEST(test_month_days_panguni);
    RUN_TEST(test_month_days_invalid);

    /* Jovian cycle names */
    RUN_TEST(test_jovian_name_first);
    RUN_TEST(test_jovian_name_last);
    RUN_TEST(test_jovian_name_middle);
    RUN_TEST(test_jovian_name_27th);
    RUN_TEST(test_jovian_name_invalid);

    /* Jovian year computation */
    RUN_TEST(test_jovian_year_1987);
    RUN_TEST(test_jovian_year_2026);
    RUN_TEST(test_jovian_year_2047);
    RUN_TEST(test_jovian_year_wraps);
    RUN_TEST(test_jovian_year_before_reference);

    /* Thiruvalluvar <-> CE */
    RUN_TEST(test_tv_from_ce_2000);
    RUN_TEST(test_ce_from_tv_2031);
    RUN_TEST(test_tv_ce_roundtrip);
    RUN_TEST(test_tv_from_ce_negative);

    /* Festival detection */
    RUN_TEST(test_festival_puthandu);
    RUN_TEST(test_festival_pongal);
    RUN_TEST(test_festival_chithirai);
    RUN_TEST(test_festival_deepavali);
    RUN_TEST(test_festival_none_normal_day);
    RUN_TEST(test_festival_none_month2);
    RUN_TEST(test_festival_puthandu_different_year);
    RUN_TEST(test_festival_pongal_different_year);

    /* Festival names */
    RUN_TEST(test_festival_name_none);
    RUN_TEST(test_festival_name_puthandu);
    RUN_TEST(test_festival_name_pongal);
    RUN_TEST(test_festival_name_chithirai);
    RUN_TEST(test_festival_name_deepavali);
    RUN_TEST(test_festival_name_invalid);

    /* to_jd basic */
    RUN_TEST(test_to_jd_produces_valid_jd);
    RUN_TEST(test_to_jd_month_progression);
    RUN_TEST(test_to_jd_year_length);

    /* from_jd field checks */
    RUN_TEST(test_from_jd_populates_jovian);
    RUN_TEST(test_from_jd_day_range);

    return UNITY_END();
}
