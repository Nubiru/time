/* test_egyptian.c — Egyptian civil calendar tests
 * TDD RED phase: tests written before implementation.
 * Epoch: JD 1448638.0 = 26 Feb 747 BCE (Julian) = Year 1, Thoth 1
 * 12 months x 30 days + 5 epagomenal days = 365 days, no leap year.
 * Sothic cycle: 1461 civil years. */

#include "../../unity/unity.h"
#include "../../../src/systems/egyptian/egyptian.h"
#include <math.h>
#include <string.h>

#define EGYPT_EPOCH_JD 1448638.0

void setUp(void) {}
void tearDown(void) {}

/* ===== Epoch conversion tests ===== */

void test_epoch_to_jd(void)
{
    /* Year 1, Thoth 1 = JD 1448638.0 => integer day 1448638 */
    egypt_date_t d = { .year = 1, .month = 1, .day = 1 };
    int jdn = (int)floor(egypt_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1448638, jdn);
}

void test_epoch_from_jd(void)
{
    /* JD 1448638.0 = Year 1, Thoth 1 */
    egypt_date_t d = egypt_from_jd(1448638.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== End of year 1 ===== */

void test_last_day_of_year1(void)
{
    /* Day 365 of year 1 = Epagomenal day 5 (month 13, day 5) */
    /* JD = 1448638.0 + 364 = 1449002.0 */
    egypt_date_t d = egypt_from_jd(EGYPT_EPOCH_JD + 364.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(5, d.day);
}

void test_last_day_of_year1_to_jd(void)
{
    /* Epagomenal day 5 of year 1 = JD 1448638.0 + 364 => integer 1449002 */
    egypt_date_t d = { .year = 1, .month = 13, .day = 5 };
    int jdn = (int)floor(egypt_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1449002, jdn);
}

/* ===== Start of year 2 ===== */

void test_first_day_of_year2(void)
{
    /* JD = 1448638.0 + 365 = 1449003.0 = Year 2, Thoth 1 */
    egypt_date_t d = egypt_from_jd(EGYPT_EPOCH_JD + 365.0);
    TEST_ASSERT_EQUAL_INT(2, d.year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_first_day_of_year2_to_jd(void)
{
    /* Year 2, Thoth 1 = JD 1448638.0 + 365 => integer day 1449003 */
    egypt_date_t d = { .year = 2, .month = 1, .day = 1 };
    int jdn = (int)floor(egypt_to_jd(d));
    TEST_ASSERT_EQUAL_INT(1449003, jdn);
}

/* ===== J2000.0 test ===== */

void test_j2000(void)
{
    /* JD 2451545.0 (1 Jan 2000 CE)
     * days_since_epoch = floor(2451545.0 - 1448638.0) = 1002907
     * year = 1002907 / 365 + 1 = 2747 + 1 = 2748
     * day_of_year = 1002907 % 365 + 1 = 1002907 - 2747*365 + 1
     *            = 1002907 - 1002655 + 1 = 253
     * month = (253-1)/30 + 1 = 252/30 + 1 = 8 + 1 = 9
     * day = (253-1)%30 + 1 = 252%30 + 1 = 12 + 1 = 13 */
    egypt_date_t d = egypt_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(2748, d.year);
    TEST_ASSERT_EQUAL_INT(9, d.month);
    TEST_ASSERT_EQUAL_INT(13, d.day);
}

/* ===== Mid-month test ===== */

void test_mid_year_date(void)
{
    /* Year 1, day 31 = month 2 (Phaophi), day 1 */
    egypt_date_t d = egypt_from_jd(EGYPT_EPOCH_JD + 30.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_epagomenal_day1(void)
{
    /* Day 361 of year = first epagomenal day (month 13, day 1)
     * JD = 1448638.0 + 360 */
    egypt_date_t d = egypt_from_jd(EGYPT_EPOCH_JD + 360.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Month names ===== */

void test_month_name_thoth(void)
{
    TEST_ASSERT_EQUAL_STRING("Thoth", egypt_month_name(1));
}

void test_month_name_phaophi(void)
{
    TEST_ASSERT_EQUAL_STRING("Phaophi", egypt_month_name(2));
}

void test_month_name_athyr(void)
{
    TEST_ASSERT_EQUAL_STRING("Athyr", egypt_month_name(3));
}

void test_month_name_choiak(void)
{
    TEST_ASSERT_EQUAL_STRING("Choiak", egypt_month_name(4));
}

void test_month_name_tybi(void)
{
    TEST_ASSERT_EQUAL_STRING("Tybi", egypt_month_name(5));
}

void test_month_name_mechir(void)
{
    TEST_ASSERT_EQUAL_STRING("Mechir", egypt_month_name(6));
}

void test_month_name_phamenoth(void)
{
    TEST_ASSERT_EQUAL_STRING("Phamenoth", egypt_month_name(7));
}

void test_month_name_pharmuthi(void)
{
    TEST_ASSERT_EQUAL_STRING("Pharmuthi", egypt_month_name(8));
}

void test_month_name_pachons(void)
{
    TEST_ASSERT_EQUAL_STRING("Pachons", egypt_month_name(9));
}

void test_month_name_payni(void)
{
    TEST_ASSERT_EQUAL_STRING("Payni", egypt_month_name(10));
}

void test_month_name_epiphi(void)
{
    TEST_ASSERT_EQUAL_STRING("Epiphi", egypt_month_name(11));
}

void test_month_name_mesore(void)
{
    TEST_ASSERT_EQUAL_STRING("Mesore", egypt_month_name(12));
}

void test_month_name_epagomenal(void)
{
    TEST_ASSERT_EQUAL_STRING("Epagomenal", egypt_month_name(13));
}

void test_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", egypt_month_name(0));
    TEST_ASSERT_EQUAL_STRING("?", egypt_month_name(14));
    TEST_ASSERT_EQUAL_STRING("?", egypt_month_name(-1));
}

/* ===== Season mapping ===== */

void test_season_akhet(void)
{
    /* Months 1-4: Akhet (Inundation) */
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_AKHET, egypt_season(1));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_AKHET, egypt_season(2));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_AKHET, egypt_season(3));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_AKHET, egypt_season(4));
}

void test_season_peret(void)
{
    /* Months 5-8: Peret (Emergence/Growth) */
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_PERET, egypt_season(5));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_PERET, egypt_season(6));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_PERET, egypt_season(7));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_PERET, egypt_season(8));
}

void test_season_shemu(void)
{
    /* Months 9-12: Shemu (Harvest) */
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_SHEMU, egypt_season(9));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_SHEMU, egypt_season(10));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_SHEMU, egypt_season(11));
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_SHEMU, egypt_season(12));
}

void test_season_epagomenal(void)
{
    /* Month 13: Epagomenal */
    TEST_ASSERT_EQUAL_INT(EGYPT_SEASON_EPAG, egypt_season(13));
}

/* ===== Season names ===== */

void test_season_name_akhet(void)
{
    TEST_ASSERT_EQUAL_STRING("Akhet", egypt_season_name(EGYPT_SEASON_AKHET));
}

void test_season_name_peret(void)
{
    TEST_ASSERT_EQUAL_STRING("Peret", egypt_season_name(EGYPT_SEASON_PERET));
}

void test_season_name_shemu(void)
{
    TEST_ASSERT_EQUAL_STRING("Shemu", egypt_season_name(EGYPT_SEASON_SHEMU));
}

void test_season_name_epagomenal(void)
{
    TEST_ASSERT_EQUAL_STRING("Epagomenal", egypt_season_name(EGYPT_SEASON_EPAG));
}

void test_season_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", egypt_season_name(EGYPT_SEASON_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", egypt_season_name(-1));
}

/* ===== Month days ===== */

void test_month_days_regular(void)
{
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_EQUAL_INT(30, egypt_month_days(m));
    }
}

void test_month_days_epagomenal(void)
{
    TEST_ASSERT_EQUAL_INT(5, egypt_month_days(13));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, egypt_month_days(0));
    TEST_ASSERT_EQUAL_INT(0, egypt_month_days(14));
}

/* ===== Day of year ===== */

void test_day_of_year_first(void)
{
    egypt_date_t d = { .year = 1, .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(1, egypt_day_of_year(d));
}

void test_day_of_year_month2(void)
{
    /* month 2, day 1 = day 31 */
    egypt_date_t d = { .year = 1, .month = 2, .day = 1 };
    TEST_ASSERT_EQUAL_INT(31, egypt_day_of_year(d));
}

void test_day_of_year_last_regular(void)
{
    /* month 12, day 30 = day 360 */
    egypt_date_t d = { .year = 1, .month = 12, .day = 30 };
    TEST_ASSERT_EQUAL_INT(360, egypt_day_of_year(d));
}

void test_day_of_year_epag1(void)
{
    /* month 13, day 1 = day 361 */
    egypt_date_t d = { .year = 1, .month = 13, .day = 1 };
    TEST_ASSERT_EQUAL_INT(361, egypt_day_of_year(d));
}

void test_day_of_year_epag5(void)
{
    /* month 13, day 5 = day 365 */
    egypt_date_t d = { .year = 1, .month = 13, .day = 5 };
    TEST_ASSERT_EQUAL_INT(365, egypt_day_of_year(d));
}

void test_day_of_year_month6(void)
{
    /* month 6, day 15 = (5*30) + 15 = 165 */
    egypt_date_t d = { .year = 100, .month = 6, .day = 15 };
    TEST_ASSERT_EQUAL_INT(165, egypt_day_of_year(d));
}

/* ===== Epagomenal detection ===== */

void test_is_epagomenal_true(void)
{
    egypt_date_t d = { .year = 1, .month = 13, .day = 3 };
    TEST_ASSERT_TRUE(egypt_is_epagomenal(d));
}

void test_is_epagomenal_false(void)
{
    egypt_date_t d = { .year = 1, .month = 6, .day = 15 };
    TEST_ASSERT_FALSE(egypt_is_epagomenal(d));
}

void test_is_epagomenal_month12_false(void)
{
    egypt_date_t d = { .year = 1, .month = 12, .day = 30 };
    TEST_ASSERT_FALSE(egypt_is_epagomenal(d));
}

/* ===== Sothic cycle position ===== */

void test_sothic_at_epoch(void)
{
    /* At epoch: cycle_year=0, progress=0.0, drift_days=0 */
    egypt_sothic_t s = egypt_sothic_position(EGYPT_EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, s.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, s.progress);
    TEST_ASSERT_EQUAL_INT(0, s.drift_days);
}

void test_sothic_after_4_years(void)
{
    /* After 4 civil years (4*365=1460 days), drift = 1 day */
    double jd = EGYPT_EPOCH_JD + 4.0 * 365.0;
    egypt_sothic_t s = egypt_sothic_position(jd);
    TEST_ASSERT_EQUAL_INT(4, s.cycle_year);
    TEST_ASSERT_EQUAL_INT(1, s.drift_days);
}

void test_sothic_at_quarter_cycle(void)
{
    /* At 365 years: cycle_year=365, progress ~0.25, drift ~91 days */
    double jd = EGYPT_EPOCH_JD + 365.0 * 365.0;
    egypt_sothic_t s = egypt_sothic_position(jd);
    TEST_ASSERT_EQUAL_INT(365, s.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 365.0 / 1461.0, s.progress);
    TEST_ASSERT_EQUAL_INT(91, s.drift_days);
}

void test_sothic_at_full_cycle(void)
{
    /* At 1461 years: completes one cycle, back to 0 */
    double jd = EGYPT_EPOCH_JD + 1461.0 * 365.0;
    egypt_sothic_t s = egypt_sothic_position(jd);
    TEST_ASSERT_EQUAL_INT(0, s.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, s.progress);
    TEST_ASSERT_EQUAL_INT(0, s.drift_days);
}

void test_sothic_at_half_cycle(void)
{
    /* At ~730 years: about half through Sothic cycle */
    double jd = EGYPT_EPOCH_JD + 730.0 * 365.0;
    egypt_sothic_t s = egypt_sothic_position(jd);
    TEST_ASSERT_EQUAL_INT(730, s.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 730.0 / 1461.0, s.progress);
    TEST_ASSERT_EQUAL_INT(182, s.drift_days);  /* 730/4 = 182 */
}

/* ===== Season month number ===== */

void test_season_month_thoth(void)
{
    /* Thoth (month 1) = season month 1 of Akhet */
    TEST_ASSERT_EQUAL_INT(1, egypt_season_month(1));
}

void test_season_month_choiak(void)
{
    /* Choiak (month 4) = season month 4 of Akhet */
    TEST_ASSERT_EQUAL_INT(4, egypt_season_month(4));
}

void test_season_month_tybi(void)
{
    /* Tybi (month 5) = season month 1 of Peret */
    TEST_ASSERT_EQUAL_INT(1, egypt_season_month(5));
}

void test_season_month_pharmuthi(void)
{
    /* Pharmuthi (month 8) = season month 4 of Peret */
    TEST_ASSERT_EQUAL_INT(4, egypt_season_month(8));
}

void test_season_month_pachons(void)
{
    /* Pachons (month 9) = season month 1 of Shemu */
    TEST_ASSERT_EQUAL_INT(1, egypt_season_month(9));
}

void test_season_month_mesore(void)
{
    /* Mesore (month 12) = season month 4 of Shemu */
    TEST_ASSERT_EQUAL_INT(4, egypt_season_month(12));
}

void test_season_month_epagomenal(void)
{
    /* Month 13: epagomenal, return 0 (not part of any season) */
    TEST_ASSERT_EQUAL_INT(0, egypt_season_month(13));
}

void test_season_month_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, egypt_season_month(0));
    TEST_ASSERT_EQUAL_INT(0, egypt_season_month(14));
}

/* ===== egypt_year convenience ===== */

void test_egypt_year_epoch(void)
{
    TEST_ASSERT_EQUAL_INT(1, egypt_year(EGYPT_EPOCH_JD));
}

void test_egypt_year_j2000(void)
{
    TEST_ASSERT_EQUAL_INT(2748, egypt_year(2451545.0));
}

void test_egypt_year_year2(void)
{
    TEST_ASSERT_EQUAL_INT(2, egypt_year(EGYPT_EPOCH_JD + 365.0));
}

/* ===== Roundtrip tests ===== */

void test_roundtrip_epoch(void)
{
    egypt_date_t orig = { .year = 1, .month = 1, .day = 1 };
    double jd = egypt_to_jd(orig);
    egypt_date_t back = egypt_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_epagomenal(void)
{
    egypt_date_t orig = { .year = 500, .month = 13, .day = 3 };
    double jd = egypt_to_jd(orig);
    egypt_date_t back = egypt_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_mid_year(void)
{
    egypt_date_t orig = { .year = 1000, .month = 7, .day = 15 };
    double jd = egypt_to_jd(orig);
    egypt_date_t back = egypt_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_last_regular_day(void)
{
    egypt_date_t orig = { .year = 2000, .month = 12, .day = 30 };
    double jd = egypt_to_jd(orig);
    egypt_date_t back = egypt_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_roundtrip_large_year(void)
{
    egypt_date_t orig = { .year = 2748, .month = 9, .day = 13 };
    double jd = egypt_to_jd(orig);
    egypt_date_t back = egypt_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.year, back.year);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

/* ===== Edge cases ===== */

void test_month12_day30(void)
{
    /* Last day of regular months = day 360 */
    egypt_date_t d = egypt_from_jd(EGYPT_EPOCH_JD + 359.0);
    TEST_ASSERT_EQUAL_INT(1, d.year);
    TEST_ASSERT_EQUAL_INT(12, d.month);
    TEST_ASSERT_EQUAL_INT(30, d.day);
}

void test_boundary_month12_to_epag(void)
{
    /* Day 360 = month 12, day 30; day 361 = month 13, day 1 */
    egypt_date_t d360 = egypt_from_jd(EGYPT_EPOCH_JD + 359.0);
    egypt_date_t d361 = egypt_from_jd(EGYPT_EPOCH_JD + 360.0);

    TEST_ASSERT_EQUAL_INT(12, d360.month);
    TEST_ASSERT_EQUAL_INT(30, d360.day);
    TEST_ASSERT_EQUAL_INT(13, d361.month);
    TEST_ASSERT_EQUAL_INT(1, d361.day);
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
    RUN_TEST(test_epagomenal_day1);

    /* Month names */
    RUN_TEST(test_month_name_thoth);
    RUN_TEST(test_month_name_phaophi);
    RUN_TEST(test_month_name_athyr);
    RUN_TEST(test_month_name_choiak);
    RUN_TEST(test_month_name_tybi);
    RUN_TEST(test_month_name_mechir);
    RUN_TEST(test_month_name_phamenoth);
    RUN_TEST(test_month_name_pharmuthi);
    RUN_TEST(test_month_name_pachons);
    RUN_TEST(test_month_name_payni);
    RUN_TEST(test_month_name_epiphi);
    RUN_TEST(test_month_name_mesore);
    RUN_TEST(test_month_name_epagomenal);
    RUN_TEST(test_month_name_invalid);

    /* Seasons */
    RUN_TEST(test_season_akhet);
    RUN_TEST(test_season_peret);
    RUN_TEST(test_season_shemu);
    RUN_TEST(test_season_epagomenal);

    /* Season names */
    RUN_TEST(test_season_name_akhet);
    RUN_TEST(test_season_name_peret);
    RUN_TEST(test_season_name_shemu);
    RUN_TEST(test_season_name_epagomenal);
    RUN_TEST(test_season_name_invalid);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_epagomenal);
    RUN_TEST(test_month_days_invalid);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_month2);
    RUN_TEST(test_day_of_year_last_regular);
    RUN_TEST(test_day_of_year_epag1);
    RUN_TEST(test_day_of_year_epag5);
    RUN_TEST(test_day_of_year_month6);

    /* Epagomenal detection */
    RUN_TEST(test_is_epagomenal_true);
    RUN_TEST(test_is_epagomenal_false);
    RUN_TEST(test_is_epagomenal_month12_false);

    /* Sothic cycle */
    RUN_TEST(test_sothic_at_epoch);
    RUN_TEST(test_sothic_after_4_years);
    RUN_TEST(test_sothic_at_quarter_cycle);
    RUN_TEST(test_sothic_at_full_cycle);
    RUN_TEST(test_sothic_at_half_cycle);

    /* Season month */
    RUN_TEST(test_season_month_thoth);
    RUN_TEST(test_season_month_choiak);
    RUN_TEST(test_season_month_tybi);
    RUN_TEST(test_season_month_pharmuthi);
    RUN_TEST(test_season_month_pachons);
    RUN_TEST(test_season_month_mesore);
    RUN_TEST(test_season_month_epagomenal);
    RUN_TEST(test_season_month_invalid);

    /* egypt_year convenience */
    RUN_TEST(test_egypt_year_epoch);
    RUN_TEST(test_egypt_year_j2000);
    RUN_TEST(test_egypt_year_year2);

    /* Roundtrips */
    RUN_TEST(test_roundtrip_epoch);
    RUN_TEST(test_roundtrip_epagomenal);
    RUN_TEST(test_roundtrip_mid_year);
    RUN_TEST(test_roundtrip_last_regular_day);
    RUN_TEST(test_roundtrip_large_year);

    /* Edge cases */
    RUN_TEST(test_month12_day30);
    RUN_TEST(test_boundary_month12_to_epag);

    return UNITY_END();
}
