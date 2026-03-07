#include "../../unity/unity.h"
#include "../../../src/systems/aztec/aztec.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ========================================================================
 * Helper: Julian Day Number from Gregorian date (for test clarity)
 * Standard algorithm from Meeus, valid for Gregorian dates
 * ======================================================================== */
static double test_jd(int y, int m, int d)
{
    int a = (14 - m) / 12;
    int yr = y + 4800 - a;
    int mo = m + 12 * a - 3;
    int jdn = d + (153 * mo + 2) / 5 + 365 * yr
              + yr / 4 - yr / 100 + yr / 400 - 32045;
    return (double)jdn - 0.5;  /* midnight */
}

/* ========================================================================
 * Constants
 * ======================================================================== */

void test_calendar_round_days_returns_18980(void)
{
    TEST_ASSERT_EQUAL_INT(18980, aztec_calendar_round_days());
}

/* ========================================================================
 * Day sign names (20 signs)
 * ======================================================================== */

void test_day_sign_name_cipactli(void)
{
    TEST_ASSERT_EQUAL_STRING("Cipactli", aztec_day_sign_name(0));
}

void test_day_sign_name_ehecatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Ehecatl", aztec_day_sign_name(1));
}

void test_day_sign_name_calli(void)
{
    TEST_ASSERT_EQUAL_STRING("Calli", aztec_day_sign_name(2));
}

void test_day_sign_name_cuetzpalin(void)
{
    TEST_ASSERT_EQUAL_STRING("Cuetzpalin", aztec_day_sign_name(3));
}

void test_day_sign_name_coatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Coatl", aztec_day_sign_name(4));
}

void test_day_sign_name_miquiztli(void)
{
    TEST_ASSERT_EQUAL_STRING("Miquiztli", aztec_day_sign_name(5));
}

void test_day_sign_name_mazatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Mazatl", aztec_day_sign_name(6));
}

void test_day_sign_name_tochtli(void)
{
    TEST_ASSERT_EQUAL_STRING("Tochtli", aztec_day_sign_name(7));
}

void test_day_sign_name_atl(void)
{
    TEST_ASSERT_EQUAL_STRING("Atl", aztec_day_sign_name(8));
}

void test_day_sign_name_itzcuintli(void)
{
    TEST_ASSERT_EQUAL_STRING("Itzcuintli", aztec_day_sign_name(9));
}

void test_day_sign_name_ozomatli(void)
{
    TEST_ASSERT_EQUAL_STRING("Ozomatli", aztec_day_sign_name(10));
}

void test_day_sign_name_malinalli(void)
{
    TEST_ASSERT_EQUAL_STRING("Malinalli", aztec_day_sign_name(11));
}

void test_day_sign_name_acatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Acatl", aztec_day_sign_name(12));
}

void test_day_sign_name_ocelotl(void)
{
    TEST_ASSERT_EQUAL_STRING("Ocelotl", aztec_day_sign_name(13));
}

void test_day_sign_name_cuauhtli(void)
{
    TEST_ASSERT_EQUAL_STRING("Cuauhtli", aztec_day_sign_name(14));
}

void test_day_sign_name_cozcacuauhtli(void)
{
    TEST_ASSERT_EQUAL_STRING("Cozcacuauhtli", aztec_day_sign_name(15));
}

void test_day_sign_name_ollin(void)
{
    TEST_ASSERT_EQUAL_STRING("Ollin", aztec_day_sign_name(16));
}

void test_day_sign_name_tecpatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Tecpatl", aztec_day_sign_name(17));
}

void test_day_sign_name_quiahuitl(void)
{
    TEST_ASSERT_EQUAL_STRING("Quiahuitl", aztec_day_sign_name(18));
}

void test_day_sign_name_xochitl(void)
{
    TEST_ASSERT_EQUAL_STRING("Xochitl", aztec_day_sign_name(19));
}

void test_day_sign_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_day_sign_name(-1));
}

void test_day_sign_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_day_sign_name(20));
}

/* ========================================================================
 * Day sign meanings
 * ======================================================================== */

void test_day_sign_meaning_cipactli(void)
{
    TEST_ASSERT_EQUAL_STRING("Crocodile", aztec_day_sign_meaning(0));
}

void test_day_sign_meaning_ehecatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Wind", aztec_day_sign_meaning(1));
}

void test_day_sign_meaning_xochitl(void)
{
    TEST_ASSERT_EQUAL_STRING("Flower", aztec_day_sign_meaning(19));
}

void test_day_sign_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_day_sign_meaning(-1));
}

/* ========================================================================
 * Month names (Xiuhpohualli)
 * ======================================================================== */

void test_month_name_nemontemi(void)
{
    TEST_ASSERT_EQUAL_STRING("Nemontemi", aztec_month_name(0));
}

void test_month_name_atlcahualo(void)
{
    TEST_ASSERT_EQUAL_STRING("Atlcahualo", aztec_month_name(1));
}

void test_month_name_tlacaxipehualiztli(void)
{
    TEST_ASSERT_EQUAL_STRING("Tlacaxipehualiztli", aztec_month_name(2));
}

void test_month_name_izcalli(void)
{
    TEST_ASSERT_EQUAL_STRING("Izcalli", aztec_month_name(18));
}

void test_month_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_month_name(-1));
}

void test_month_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_month_name(19));
}

/* ========================================================================
 * Month meanings
 * ======================================================================== */

void test_month_meaning_nemontemi(void)
{
    TEST_ASSERT_EQUAL_STRING("Useless Days", aztec_month_meaning(0));
}

void test_month_meaning_atlcahualo(void)
{
    TEST_ASSERT_EQUAL_STRING("Cessation of Water", aztec_month_meaning(1));
}

void test_month_meaning_izcalli(void)
{
    TEST_ASSERT_EQUAL_STRING("Growth", aztec_month_meaning(18));
}

void test_month_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_month_meaning(-1));
}

/* ========================================================================
 * Month days
 * ======================================================================== */

void test_month_days_regular(void)
{
    /* Months 1-18 have 20 days each */
    for (int m = 1; m <= 18; m++) {
        TEST_ASSERT_EQUAL_INT(20, aztec_month_days(m));
    }
}

void test_month_days_nemontemi(void)
{
    TEST_ASSERT_EQUAL_INT(5, aztec_month_days(0));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, aztec_month_days(-1));
    TEST_ASSERT_EQUAL_INT(0, aztec_month_days(19));
}

/* ========================================================================
 * Year bearer names
 * ======================================================================== */

void test_year_bearer_name_tochtli(void)
{
    TEST_ASSERT_EQUAL_STRING("Tochtli", aztec_year_bearer_name(0));
}

void test_year_bearer_name_acatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Acatl", aztec_year_bearer_name(1));
}

void test_year_bearer_name_tecpatl(void)
{
    TEST_ASSERT_EQUAL_STRING("Tecpatl", aztec_year_bearer_name(2));
}

void test_year_bearer_name_calli(void)
{
    TEST_ASSERT_EQUAL_STRING("Calli", aztec_year_bearer_name(3));
}

void test_year_bearer_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", aztec_year_bearer_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", aztec_year_bearer_name(4));
}

/* ========================================================================
 * Tonalpohualli — GMT epoch reference (JDN 584283 = 4 Xochitl)
 * The Mayan Long Count 0.0.0.0.0 = Tzolkin 4 Ahau = Aztec 4 Xochitl
 * ======================================================================== */

void test_tonalpohualli_epoch_sign(void)
{
    /* JD 584283.0 = noon on the epoch day (JDN 584283) */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584283.0);
    TEST_ASSERT_EQUAL_INT(19, t.day_sign);  /* Xochitl */
}

void test_tonalpohualli_epoch_number(void)
{
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584283.0);
    TEST_ASSERT_EQUAL_INT(4, t.day_number);
}

void test_tonalpohualli_epoch_day_index(void)
{
    /* 4 Xochitl: sign=19, number=4. day_index = ?
     * Kin formula: ((sign) + 1) combined with number within 260 cycle.
     * day_index goes 1-260 representing position in the 260-day cycle.
     * 1 Cipactli = day_index 1.
     * The cycle: 1-Cipactli, 2-Ehecatl, 3-Calli, 4-Cuetzpalin, 5-Coatl,
     *            6-Miquiztli, 7-Mazatl, 8-Tochtli, 9-Atl, 10-Itzcuintli,
     *            11-Ozomatli, 12-Malinalli, 13-Acatl, 1-Ocelotl, 2-Cuauhtli ...
     * day_index = ((sign - (number-1) * 7) % 20 * 13 + ... ) -- complex
     * Simpler: use CRT. sign ≡ day_index-1 (mod 20), number ≡ day_index (mod 13) with 1-based.
     * For sign=19, number=4: day_index ≡ 20 (mod 20) ≡ 0 (mod 20), day_index ≡ 4 (mod 13).
     * day_index-1 ≡ 19 (mod 20) → day_index = 20k+20 for k≥0 → day_index ≡ 0 (mod 20).
     * day_index ≡ 4 (mod 13). 20 ≡ 4 (mod 13) → day_index=20 works? Check: 20%13=7≠4.
     * day_index=160: 160%20=0→sign=(160-1)%20=19 ✓. 160%13=160-12*13=160-156=4→ number=((160-1)%13)+1=(159%13)+1=3+1=4 ✓
     * day_index = 160. */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584283.0);
    TEST_ASSERT_EQUAL_INT(160, t.day_index);
}

void test_tonalpohualli_day_after_epoch(void)
{
    /* Day after 4 Xochitl = 5 Cipactli */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584284.0);
    TEST_ASSERT_EQUAL_INT(0, t.day_sign);   /* Cipactli */
    TEST_ASSERT_EQUAL_INT(5, t.day_number);
}

void test_tonalpohualli_two_days_after_epoch(void)
{
    /* Two days after epoch = 6 Ehecatl */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584285.0);
    TEST_ASSERT_EQUAL_INT(1, t.day_sign);   /* Ehecatl */
    TEST_ASSERT_EQUAL_INT(6, t.day_number);
}

void test_tonalpohualli_1_cipactli(void)
{
    /* 1 Cipactli is 100 days before 4 Xochitl (offset = -100 from epoch)
     * Actually: we need offset such that sign=0, number=1.
     * sign: (offset+19)%20=0 → offset%20=1
     * number: ((offset+3)%13)+1=1 → offset%13=10
     * CRT: offset≡1(mod 20), offset≡10(mod 13).
     * offset=20k+1. 20k+1≡10(mod 13)→7k≡9(mod 13)→k≡9*2(mod 13)=18%13=5.
     * k=5: offset=101. Check: 101%20=1→sign=(101+19)%20=120%20=0 ✓
     *                         101%13=101-7*13=101-91=10→number=((101+3)%13)+1=104%13+1=0+1=1 ✓
     * JD = 584283+101 = 584384.0. But offset=101 gives 1 Cipactli.
     * Actually, 1 Cipactli is the FIRST day (day_index=1). Offset should be
     * -159 from epoch to get day_index=1 (since epoch day_index=160).
     * offset=-159: sign=(-159+19)%20=-140%20=0 ✓ (in C: need care with negative mod)
     * number=((-159+3)%13)+1=(-156%13)+1=0+1=1 ✓
     * JD = 584283-159=584124.0 */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584124.0);
    TEST_ASSERT_EQUAL_INT(0, t.day_sign);   /* Cipactli */
    TEST_ASSERT_EQUAL_INT(1, t.day_number);
    TEST_ASSERT_EQUAL_INT(1, t.day_index);
}

void test_tonalpohualli_full_cycle(void)
{
    /* 260 days after 1 Cipactli returns to 1 Cipactli */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(584124.0 + 260.0);
    TEST_ASSERT_EQUAL_INT(0, t.day_sign);
    TEST_ASSERT_EQUAL_INT(1, t.day_number);
    TEST_ASSERT_EQUAL_INT(1, t.day_index);
}

void test_tonalpohualli_jan_1_2024(void)
{
    /* Jan 1, 2024 at midnight = JD 2460310.5
     * JDN = floor(2460310.5 + 0.5) = 2460311
     * offset = 2460311 - 584283 = 1876028
     * sign = (1876028 + 19) % 20 = 1876047 % 20 = 7 (Tochtli)
     * number = ((1876028 + 3) % 13) + 1 = 1876031 % 13 + 1 = 1 + 1 = 2 */
    double jd = test_jd(2024, 1, 1);
    aztec_tonalpohualli_t t = aztec_tonalpohualli(jd);
    TEST_ASSERT_EQUAL_INT(7, t.day_sign);   /* Tochtli (Rabbit) */
    TEST_ASSERT_EQUAL_INT(2, t.day_number);
}

void test_tonalpohualli_number_range(void)
{
    /* Numbers cycle 1-13 for 13 consecutive days */
    for (int i = 0; i < 13; i++) {
        aztec_tonalpohualli_t t = aztec_tonalpohualli(584283.0 + (double)i);
        TEST_ASSERT_TRUE(t.day_number >= 1 && t.day_number <= 13);
        /* Starting from 4 (epoch number), increment each day */
        int expected = ((4 - 1 + i) % 13) + 1;
        TEST_ASSERT_EQUAL_INT(expected, t.day_number);
    }
}

void test_tonalpohualli_sign_range(void)
{
    /* Signs cycle 0-19 for 20 consecutive days */
    for (int i = 0; i < 20; i++) {
        aztec_tonalpohualli_t t = aztec_tonalpohualli(584283.0 + (double)i);
        TEST_ASSERT_TRUE(t.day_sign >= 0 && t.day_sign <= 19);
        int expected = (19 + i) % 20;
        TEST_ASSERT_EQUAL_INT(expected, t.day_sign);
    }
}

void test_tonalpohualli_day_index_range(void)
{
    /* day_index should always be 1-260 */
    for (int i = 0; i < 260; i++) {
        aztec_tonalpohualli_t t = aztec_tonalpohualli(584124.0 + (double)i);
        TEST_ASSERT_TRUE(t.day_index >= 1 && t.day_index <= 260);
        TEST_ASSERT_EQUAL_INT(i + 1, t.day_index);
    }
}

void test_tonalpohualli_modern_date(void)
{
    /* March 7, 2026 */
    double jd = test_jd(2026, 3, 7);
    aztec_tonalpohualli_t t = aztec_tonalpohualli(jd);
    TEST_ASSERT_TRUE(t.day_sign >= 0 && t.day_sign <= 19);
    TEST_ASSERT_TRUE(t.day_number >= 1 && t.day_number <= 13);
    TEST_ASSERT_TRUE(t.day_index >= 1 && t.day_index <= 260);
}

void test_tonalpohualli_midnight_vs_noon(void)
{
    /* Midnight and noon of the same date should give the same result */
    aztec_tonalpohualli_t t1 = aztec_tonalpohualli(2460310.5);  /* midnight */
    aztec_tonalpohualli_t t2 = aztec_tonalpohualli(2460311.0);  /* noon */
    TEST_ASSERT_EQUAL_INT(t1.day_sign, t2.day_sign);
    TEST_ASSERT_EQUAL_INT(t1.day_number, t2.day_number);
    TEST_ASSERT_EQUAL_INT(t1.day_index, t2.day_index);
}

/* ========================================================================
 * Xiuhpohualli (365-day solar calendar)
 * ======================================================================== */

void test_xiuhpohualli_epoch(void)
{
    /* Epoch JDN 584283 = Haab 8 Cumku = Aztec month 18 (Izcalli), day 8 */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0);
    TEST_ASSERT_EQUAL_INT(18, x.month);  /* Izcalli */
    TEST_ASSERT_EQUAL_INT(8, x.day);
}

void test_xiuhpohualli_epoch_year_day(void)
{
    /* Month 18, day 8 = day 348 of the year (17*20 + 8 = 348) */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0);
    TEST_ASSERT_EQUAL_INT(348, x.year_day);
}

void test_xiuhpohualli_next_new_year(void)
{
    /* 18 days after epoch: should be month 1 (Atlcahualo), day 1 */
    /* Epoch is day 348. Remaining: 360-348=12 days in Izcalli, then 5 Nemontemi = 17.
     * So 18 days later is new year = Atlcahualo day 1 */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0 + 18.0);
    TEST_ASSERT_EQUAL_INT(1, x.month);  /* Atlcahualo */
    TEST_ASSERT_EQUAL_INT(1, x.day);
    TEST_ASSERT_EQUAL_INT(1, x.year_day);
}

void test_xiuhpohualli_end_of_izcalli(void)
{
    /* 12 days after epoch: month 18 (Izcalli), day 20 (last day of Izcalli) */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0 + 12.0);
    TEST_ASSERT_EQUAL_INT(18, x.month);
    TEST_ASSERT_EQUAL_INT(20, x.day);
    TEST_ASSERT_EQUAL_INT(360, x.year_day);
}

void test_xiuhpohualli_nemontemi_day_1(void)
{
    /* 13 days after epoch: Nemontemi day 1 */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0 + 13.0);
    TEST_ASSERT_EQUAL_INT(0, x.month);   /* Nemontemi */
    TEST_ASSERT_EQUAL_INT(1, x.day);
    TEST_ASSERT_EQUAL_INT(361, x.year_day);
}

void test_xiuhpohualli_nemontemi_day_5(void)
{
    /* 17 days after epoch: Nemontemi day 5 (last day of year) */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0 + 17.0);
    TEST_ASSERT_EQUAL_INT(0, x.month);
    TEST_ASSERT_EQUAL_INT(5, x.day);
    TEST_ASSERT_EQUAL_INT(365, x.year_day);
}

void test_xiuhpohualli_month_2_day_1(void)
{
    /* Month 2 day 1 = year_day 21. 21 - 1 = 20 days after new year.
     * New year is 18 days after epoch. So 18+20 = 38 days after epoch. */
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(584283.0 + 38.0);
    TEST_ASSERT_EQUAL_INT(2, x.month);  /* Tlacaxipehualiztli */
    TEST_ASSERT_EQUAL_INT(1, x.day);
    TEST_ASSERT_EQUAL_INT(21, x.year_day);
}

void test_xiuhpohualli_full_year_cycle(void)
{
    /* 365 days after new year = same position */
    double new_year_jd = 584283.0 + 18.0;
    aztec_xiuhpohualli_t x1 = aztec_xiuhpohualli(new_year_jd);
    aztec_xiuhpohualli_t x2 = aztec_xiuhpohualli(new_year_jd + 365.0);
    TEST_ASSERT_EQUAL_INT(x1.month, x2.month);
    TEST_ASSERT_EQUAL_INT(x1.day, x2.day);
    TEST_ASSERT_EQUAL_INT(x1.year_day, x2.year_day);
}

void test_xiuhpohualli_modern_date(void)
{
    /* Jan 1, 2024 — should produce valid month/day */
    double jd = test_jd(2024, 1, 1);
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(jd);
    TEST_ASSERT_TRUE((x.month >= 0 && x.month <= 18));
    if (x.month == 0) {
        TEST_ASSERT_TRUE(x.day >= 1 && x.day <= 5);
    } else {
        TEST_ASSERT_TRUE(x.day >= 1 && x.day <= 20);
    }
    TEST_ASSERT_TRUE(x.year_day >= 1 && x.year_day <= 365);
}

void test_xiuhpohualli_midnight_vs_noon(void)
{
    aztec_xiuhpohualli_t x1 = aztec_xiuhpohualli(2460310.5);
    aztec_xiuhpohualli_t x2 = aztec_xiuhpohualli(2460311.0);
    TEST_ASSERT_EQUAL_INT(x1.month, x2.month);
    TEST_ASSERT_EQUAL_INT(x1.day, x2.day);
    TEST_ASSERT_EQUAL_INT(x1.year_day, x2.year_day);
}

/* ========================================================================
 * Calendar Round (52-year cycle)
 * ======================================================================== */

void test_calendar_round_epoch_bearer(void)
{
    /* The epoch (JDN 584283) is partway through a year.
     * Its new year was 347 days earlier. The new year sign:
     * offset of new year = 0 - 347 = -347 from epoch.
     * sign = (-347 + 19) % 20 = (-328) % 20. C modulus: need care.
     * -328 + 17*20 = -328 + 340 = 12. sign = 12 = Acatl.
     * Acatl → year_bearer index 1. */
    aztec_calendar_round_t cr = aztec_calendar_round(584283.0);
    TEST_ASSERT_EQUAL_INT(1, cr.year_bearer);  /* Acatl */
}

void test_calendar_round_next_year_bearer(void)
{
    /* New year 18 days after epoch: offset=18.
     * sign = (18+19)%20 = 37%20 = 17 = Tecpatl → bearer 2 */
    aztec_calendar_round_t cr = aztec_calendar_round(584283.0 + 18.0);
    TEST_ASSERT_EQUAL_INT(2, cr.year_bearer);  /* Tecpatl */
}

void test_calendar_round_year_number_range(void)
{
    aztec_calendar_round_t cr = aztec_calendar_round(584283.0);
    TEST_ASSERT_TRUE(cr.year_number >= 1 && cr.year_number <= 13);
}

void test_calendar_round_round_year_range(void)
{
    aztec_calendar_round_t cr = aztec_calendar_round(584283.0);
    TEST_ASSERT_TRUE(cr.round_year >= 1 && cr.round_year <= 52);
}

void test_calendar_round_bearer_cycles_four(void)
{
    /* Year bearers should cycle through 4 types over 4 consecutive years */
    double new_year_jd = 584283.0 + 18.0;  /* First new year after epoch */
    int bearers[4];
    for (int i = 0; i < 4; i++) {
        aztec_calendar_round_t cr = aztec_calendar_round(new_year_jd + (double)(i * 365));
        bearers[i] = cr.year_bearer;
    }
    /* All four different bearers present */
    int seen[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_TRUE(bearers[i] >= 0 && bearers[i] <= 3);
        seen[bearers[i]] = 1;
    }
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

void test_calendar_round_52_year_cycle(void)
{
    /* After 52 years (18980 days), the Calendar Round repeats */
    double jd = test_jd(2024, 6, 15);
    aztec_calendar_round_t cr1 = aztec_calendar_round(jd);
    aztec_calendar_round_t cr2 = aztec_calendar_round(jd + 18980.0);
    TEST_ASSERT_EQUAL_INT(cr1.year_bearer, cr2.year_bearer);
    TEST_ASSERT_EQUAL_INT(cr1.year_number, cr2.year_number);
    TEST_ASSERT_EQUAL_INT(cr1.round_year, cr2.round_year);
}

void test_calendar_round_year_number_increments(void)
{
    /* Year number should increase by 1 each year (mod 13, 1-based) */
    double new_year_jd = 584283.0 + 18.0;
    aztec_calendar_round_t cr1 = aztec_calendar_round(new_year_jd);
    aztec_calendar_round_t cr2 = aztec_calendar_round(new_year_jd + 365.0);
    int expected_next = (cr1.year_number % 13) + 1;
    TEST_ASSERT_EQUAL_INT(expected_next, cr2.year_number);
}

void test_calendar_round_modern_date(void)
{
    double jd = test_jd(2026, 3, 7);
    aztec_calendar_round_t cr = aztec_calendar_round(jd);
    TEST_ASSERT_TRUE(cr.year_bearer >= 0 && cr.year_bearer <= 3);
    TEST_ASSERT_TRUE(cr.year_number >= 1 && cr.year_number <= 13);
    TEST_ASSERT_TRUE(cr.round_year >= 1 && cr.round_year <= 52);
}

/* ========================================================================
 * New Fire Ceremony (Xiuhmolpilli)
 * ======================================================================== */

void test_new_fire_on_calendar_round_boundary(void)
{
    /* Find a Calendar Round boundary: new year at day_index=1, round_year=52 ends,
     * new round starts. The New Fire happens every 18980 days.
     * Pick the first new year after epoch (offset 18), add multiples of 18980. */
    double new_year_jd = 584283.0 + 18.0;
    /* The New Fire occurs on the new year that completes a 52-year cycle.
     * That's at new_year_jd + n*18980 for some n where round_year wraps. */
    TEST_ASSERT_TRUE(aztec_is_new_fire(new_year_jd + 18980.0));
}

void test_new_fire_not_mid_year(void)
{
    /* Mid-year should not be a New Fire */
    TEST_ASSERT_FALSE(aztec_is_new_fire(584283.0 + 100.0));
}

void test_new_fire_not_random_new_year(void)
{
    /* A new year that is NOT on a Calendar Round boundary */
    double new_year_jd = 584283.0 + 18.0;
    TEST_ASSERT_FALSE(aztec_is_new_fire(new_year_jd + 365.0));
}

void test_days_to_new_fire_on_boundary(void)
{
    /* On a New Fire day, days_to_new_fire should be 0 */
    double new_year_jd = 584283.0 + 18.0;
    TEST_ASSERT_EQUAL_INT(0, aztec_days_to_new_fire(new_year_jd + 18980.0));
}

void test_days_to_new_fire_one_day_before(void)
{
    double new_year_jd = 584283.0 + 18.0;
    /* One day before the New Fire */
    TEST_ASSERT_EQUAL_INT(1, aztec_days_to_new_fire(new_year_jd + 18980.0 - 1.0));
}

void test_days_to_new_fire_decreases(void)
{
    /* days_to_new_fire should decrease as we approach the next ceremony */
    double jd1 = test_jd(2024, 1, 1);
    double jd2 = test_jd(2024, 1, 2);
    int d1 = aztec_days_to_new_fire(jd1);
    int d2 = aztec_days_to_new_fire(jd2);
    TEST_ASSERT_EQUAL_INT(d1 - 1, d2);
}

void test_days_to_new_fire_max_range(void)
{
    /* Should always be 0 to 18979 */
    double jd = test_jd(2024, 6, 15);
    int d = aztec_days_to_new_fire(jd);
    TEST_ASSERT_TRUE(d >= 0 && d <= 18979);
}

void test_days_to_new_fire_wraps_after_new_fire(void)
{
    /* Day after New Fire, days_to_new_fire should be 18979 */
    double new_year_jd = 584283.0 + 18.0;
    double new_fire = new_year_jd + 18980.0;
    int d = aztec_days_to_new_fire(new_fire + 1.0);
    TEST_ASSERT_EQUAL_INT(18979, d);
}

/* ========================================================================
 * Edge cases and consistency checks
 * ======================================================================== */

void test_tonalpohualli_negative_jd(void)
{
    /* Very old date — should still produce valid results */
    aztec_tonalpohualli_t t = aztec_tonalpohualli(0.0);
    TEST_ASSERT_TRUE(t.day_sign >= 0 && t.day_sign <= 19);
    TEST_ASSERT_TRUE(t.day_number >= 1 && t.day_number <= 13);
    TEST_ASSERT_TRUE(t.day_index >= 1 && t.day_index <= 260);
}

void test_xiuhpohualli_negative_jd(void)
{
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(0.0);
    TEST_ASSERT_TRUE(x.month >= 0 && x.month <= 18);
    TEST_ASSERT_TRUE(x.year_day >= 1 && x.year_day <= 365);
}

void test_tonalpohualli_far_future(void)
{
    /* Year 3000 — should still produce valid results */
    double jd = test_jd(3000, 1, 1);
    aztec_tonalpohualli_t t = aztec_tonalpohualli(jd);
    TEST_ASSERT_TRUE(t.day_sign >= 0 && t.day_sign <= 19);
    TEST_ASSERT_TRUE(t.day_number >= 1 && t.day_number <= 13);
    TEST_ASSERT_TRUE(t.day_index >= 1 && t.day_index <= 260);
}

void test_xiuhpohualli_year_days_sum(void)
{
    /* Sum of all month days should be 365 */
    int sum = 0;
    for (int m = 1; m <= 18; m++) {
        sum += aztec_month_days(m);
    }
    sum += aztec_month_days(0);  /* Nemontemi */
    TEST_ASSERT_EQUAL_INT(365, sum);
}

void test_tonalpohualli_consistency_260_days(void)
{
    /* Every day_index from 1-260 appears exactly once in a 260-day span */
    double start = test_jd(2024, 1, 1);
    int seen[261] = {0};
    for (int i = 0; i < 260; i++) {
        aztec_tonalpohualli_t t = aztec_tonalpohualli(start + (double)i);
        TEST_ASSERT_TRUE(t.day_index >= 1 && t.day_index <= 260);
        seen[t.day_index]++;
    }
    for (int i = 1; i <= 260; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

void test_xiuhpohualli_365_days_coverage(void)
{
    /* Over 365 consecutive days, year_day covers 1-365 */
    double start = 584283.0 + 18.0;  /* New year */
    for (int i = 0; i < 365; i++) {
        aztec_xiuhpohualli_t x = aztec_xiuhpohualli(start + (double)i);
        TEST_ASSERT_EQUAL_INT(i + 1, x.year_day);
    }
}

void test_tonalpohualli_and_xiuhpohualli_independence(void)
{
    /* Both systems can be computed for the same date */
    double jd = test_jd(2024, 7, 4);
    aztec_tonalpohualli_t t = aztec_tonalpohualli(jd);
    aztec_xiuhpohualli_t x = aztec_xiuhpohualli(jd);
    /* Both valid */
    TEST_ASSERT_TRUE(t.day_sign >= 0 && t.day_sign <= 19);
    TEST_ASSERT_TRUE(x.month >= 0 && x.month <= 18);
}

/* ========================================================================
 * Cross-verification: Tonalpohualli day_index consistency
 * ======================================================================== */

void test_day_index_increments_by_one(void)
{
    /* day_index should increase by 1 each day, wrapping 260→1 */
    double start = test_jd(2024, 1, 1);
    aztec_tonalpohualli_t prev = aztec_tonalpohualli(start);
    for (int i = 1; i < 300; i++) {
        aztec_tonalpohualli_t curr = aztec_tonalpohualli(start + (double)i);
        int expected = (prev.day_index % 260) + 1;
        TEST_ASSERT_EQUAL_INT(expected, curr.day_index);
        prev = curr;
    }
}

/* ========================================================================
 * Main
 * ======================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Constants */
    RUN_TEST(test_calendar_round_days_returns_18980);

    /* Day sign names — 20 tests */
    RUN_TEST(test_day_sign_name_cipactli);
    RUN_TEST(test_day_sign_name_ehecatl);
    RUN_TEST(test_day_sign_name_calli);
    RUN_TEST(test_day_sign_name_cuetzpalin);
    RUN_TEST(test_day_sign_name_coatl);
    RUN_TEST(test_day_sign_name_miquiztli);
    RUN_TEST(test_day_sign_name_mazatl);
    RUN_TEST(test_day_sign_name_tochtli);
    RUN_TEST(test_day_sign_name_atl);
    RUN_TEST(test_day_sign_name_itzcuintli);
    RUN_TEST(test_day_sign_name_ozomatli);
    RUN_TEST(test_day_sign_name_malinalli);
    RUN_TEST(test_day_sign_name_acatl);
    RUN_TEST(test_day_sign_name_ocelotl);
    RUN_TEST(test_day_sign_name_cuauhtli);
    RUN_TEST(test_day_sign_name_cozcacuauhtli);
    RUN_TEST(test_day_sign_name_ollin);
    RUN_TEST(test_day_sign_name_tecpatl);
    RUN_TEST(test_day_sign_name_quiahuitl);
    RUN_TEST(test_day_sign_name_xochitl);
    RUN_TEST(test_day_sign_name_invalid_negative);
    RUN_TEST(test_day_sign_name_invalid_high);

    /* Day sign meanings */
    RUN_TEST(test_day_sign_meaning_cipactli);
    RUN_TEST(test_day_sign_meaning_ehecatl);
    RUN_TEST(test_day_sign_meaning_xochitl);
    RUN_TEST(test_day_sign_meaning_invalid);

    /* Month names */
    RUN_TEST(test_month_name_nemontemi);
    RUN_TEST(test_month_name_atlcahualo);
    RUN_TEST(test_month_name_tlacaxipehualiztli);
    RUN_TEST(test_month_name_izcalli);
    RUN_TEST(test_month_name_invalid_negative);
    RUN_TEST(test_month_name_invalid_high);

    /* Month meanings */
    RUN_TEST(test_month_meaning_nemontemi);
    RUN_TEST(test_month_meaning_atlcahualo);
    RUN_TEST(test_month_meaning_izcalli);
    RUN_TEST(test_month_meaning_invalid);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_nemontemi);
    RUN_TEST(test_month_days_invalid);

    /* Year bearer names */
    RUN_TEST(test_year_bearer_name_tochtli);
    RUN_TEST(test_year_bearer_name_acatl);
    RUN_TEST(test_year_bearer_name_tecpatl);
    RUN_TEST(test_year_bearer_name_calli);
    RUN_TEST(test_year_bearer_name_invalid);

    /* Tonalpohualli */
    RUN_TEST(test_tonalpohualli_epoch_sign);
    RUN_TEST(test_tonalpohualli_epoch_number);
    RUN_TEST(test_tonalpohualli_epoch_day_index);
    RUN_TEST(test_tonalpohualli_day_after_epoch);
    RUN_TEST(test_tonalpohualli_two_days_after_epoch);
    RUN_TEST(test_tonalpohualli_1_cipactli);
    RUN_TEST(test_tonalpohualli_full_cycle);
    RUN_TEST(test_tonalpohualli_jan_1_2024);
    RUN_TEST(test_tonalpohualli_number_range);
    RUN_TEST(test_tonalpohualli_sign_range);
    RUN_TEST(test_tonalpohualli_day_index_range);
    RUN_TEST(test_tonalpohualli_modern_date);
    RUN_TEST(test_tonalpohualli_midnight_vs_noon);

    /* Xiuhpohualli */
    RUN_TEST(test_xiuhpohualli_epoch);
    RUN_TEST(test_xiuhpohualli_epoch_year_day);
    RUN_TEST(test_xiuhpohualli_next_new_year);
    RUN_TEST(test_xiuhpohualli_end_of_izcalli);
    RUN_TEST(test_xiuhpohualli_nemontemi_day_1);
    RUN_TEST(test_xiuhpohualli_nemontemi_day_5);
    RUN_TEST(test_xiuhpohualli_month_2_day_1);
    RUN_TEST(test_xiuhpohualli_full_year_cycle);
    RUN_TEST(test_xiuhpohualli_modern_date);
    RUN_TEST(test_xiuhpohualli_midnight_vs_noon);

    /* Calendar Round */
    RUN_TEST(test_calendar_round_epoch_bearer);
    RUN_TEST(test_calendar_round_next_year_bearer);
    RUN_TEST(test_calendar_round_year_number_range);
    RUN_TEST(test_calendar_round_round_year_range);
    RUN_TEST(test_calendar_round_bearer_cycles_four);
    RUN_TEST(test_calendar_round_52_year_cycle);
    RUN_TEST(test_calendar_round_year_number_increments);
    RUN_TEST(test_calendar_round_modern_date);

    /* New Fire */
    RUN_TEST(test_new_fire_on_calendar_round_boundary);
    RUN_TEST(test_new_fire_not_mid_year);
    RUN_TEST(test_new_fire_not_random_new_year);
    RUN_TEST(test_days_to_new_fire_on_boundary);
    RUN_TEST(test_days_to_new_fire_one_day_before);
    RUN_TEST(test_days_to_new_fire_decreases);
    RUN_TEST(test_days_to_new_fire_max_range);
    RUN_TEST(test_days_to_new_fire_wraps_after_new_fire);

    /* Edge cases */
    RUN_TEST(test_tonalpohualli_negative_jd);
    RUN_TEST(test_xiuhpohualli_negative_jd);
    RUN_TEST(test_tonalpohualli_far_future);
    RUN_TEST(test_xiuhpohualli_year_days_sum);
    RUN_TEST(test_tonalpohualli_consistency_260_days);
    RUN_TEST(test_xiuhpohualli_365_days_coverage);
    RUN_TEST(test_tonalpohualli_and_xiuhpohualli_independence);

    /* Cross-verification */
    RUN_TEST(test_day_index_increments_by_one);

    return UNITY_END();
}
