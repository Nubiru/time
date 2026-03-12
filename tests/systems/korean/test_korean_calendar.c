/* test_korean_calendar.c -- Korean Traditional Calendar module tests
 * TDD: tests written before implementation.
 * Covers: Dangun year, sexagenary cycle, stem/branch/animal/element names,
 *         polarity, sexagenary combination names, lunar month/day from JD,
 *         festival detection, festival by index, festival count,
 *         Seollal JD approximation, JD round-trip, invalid inputs. */

#include "../../unity/unity.h"
#include "../../../src/systems/korean/korean_calendar.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Dangun year conversion ===== */

void test_dangun_year_2024(void)
{
    TEST_ASSERT_EQUAL_INT(4357, korean_dangun_year(2024));
}

void test_dangun_year_2000(void)
{
    TEST_ASSERT_EQUAL_INT(4333, korean_dangun_year(2000));
}

void test_dangun_year_2025(void)
{
    TEST_ASSERT_EQUAL_INT(4358, korean_dangun_year(2025));
}

void test_dangun_year_2026(void)
{
    TEST_ASSERT_EQUAL_INT(4359, korean_dangun_year(2026));
}

void test_dangun_year_1(void)
{
    TEST_ASSERT_EQUAL_INT(2334, korean_dangun_year(1));
}

void test_dangun_year_1984(void)
{
    TEST_ASSERT_EQUAL_INT(4317, korean_dangun_year(1984));
}

void test_dangun_year_1988(void)
{
    TEST_ASSERT_EQUAL_INT(4321, korean_dangun_year(1988));
}

void test_dangun_year_negative(void)
{
    /* 2333 BCE = year -2332 in astronomical = Dangun year 1 */
    TEST_ASSERT_EQUAL_INT(1, korean_dangun_year(-2332));
}

void test_dangun_year_zero(void)
{
    /* Year 0 (1 BCE) => 0 + 2333 = 2333 */
    TEST_ASSERT_EQUAL_INT(2333, korean_dangun_year(0));
}

/* ===== Sexagenary cycle: year 2000 = Dragon, stem Gyeong(6), branch Jin(4) ===== */

void test_sexagenary_2000_stem(void)
{
    korean_date_t d = korean_from_jd(2451600.0); /* mid-Feb 2000 (after Seollal) */
    TEST_ASSERT_EQUAL_INT(6, d.stem); /* Gyeong */
}

void test_sexagenary_2000_branch(void)
{
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_EQUAL_INT(4, d.branch); /* Jin */
}

void test_sexagenary_2000_animal(void)
{
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_EQUAL_INT(4, d.animal); /* Dragon */
}

void test_sexagenary_2024_stem(void)
{
    /* 2024 Dragon year, stem Gap(0) */
    korean_date_t d = korean_from_jd(2460400.0); /* ~Apr 2024 */
    TEST_ASSERT_EQUAL_INT(0, d.stem); /* Gap */
}

void test_sexagenary_2024_branch(void)
{
    korean_date_t d = korean_from_jd(2460400.0);
    TEST_ASSERT_EQUAL_INT(4, d.branch); /* Jin */
}

void test_sexagenary_2025_stem(void)
{
    /* 2025 Snake year, stem Eul(1) */
    korean_date_t d = korean_from_jd(2460750.0); /* ~Mar 2025 */
    TEST_ASSERT_EQUAL_INT(1, d.stem);
}

void test_sexagenary_2025_branch(void)
{
    korean_date_t d = korean_from_jd(2460750.0);
    TEST_ASSERT_EQUAL_INT(5, d.branch); /* Sa */
}

void test_sexagenary_2026_stem(void)
{
    /* 2026 Horse year, stem Byeong(2) */
    korean_date_t d = korean_from_jd(2461100.0); /* ~Feb 2026 */
    TEST_ASSERT_EQUAL_INT(2, d.stem);
}

void test_sexagenary_2026_branch(void)
{
    korean_date_t d = korean_from_jd(2461100.0);
    TEST_ASSERT_EQUAL_INT(6, d.branch); /* O */
}

void test_sexagenary_1984_stem(void)
{
    /* 1984 Rat year, stem Gap(0) */
    korean_date_t d = korean_from_jd(2445800.0); /* mid-1984 */
    TEST_ASSERT_EQUAL_INT(0, d.stem);
}

void test_sexagenary_1984_branch(void)
{
    korean_date_t d = korean_from_jd(2445800.0);
    TEST_ASSERT_EQUAL_INT(0, d.branch); /* Ja */
}

void test_sexagenary_1984_cycle_year(void)
{
    korean_date_t d = korean_from_jd(2445800.0);
    TEST_ASSERT_EQUAL_INT(1, d.cycle_year);
}

void test_sexagenary_1988_stem(void)
{
    /* 1988 Dragon year, stem Mu(4) */
    korean_date_t d = korean_from_jd(2447300.0); /* ~Jun 1988 */
    TEST_ASSERT_EQUAL_INT(4, d.stem);
}

void test_sexagenary_1988_branch(void)
{
    korean_date_t d = korean_from_jd(2447300.0);
    TEST_ASSERT_EQUAL_INT(4, d.branch); /* Jin */
}

void test_sexagenary_element_2000(void)
{
    /* stem 6 -> element 3 = Metal */
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_EQUAL_INT(3, d.element);
}

void test_sexagenary_polarity_2000(void)
{
    /* stem 6 -> polarity 0 = Yang */
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_EQUAL_INT(0, d.polarity);
}

void test_sexagenary_polarity_2025(void)
{
    /* stem 1 -> polarity 1 = Yin */
    korean_date_t d = korean_from_jd(2460750.0);
    TEST_ASSERT_EQUAL_INT(1, d.polarity);
}

void test_sexagenary_dangun_2000(void)
{
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_EQUAL_INT(4333, d.dangun_year);
}

void test_sexagenary_dangun_2024(void)
{
    korean_date_t d = korean_from_jd(2460400.0);
    TEST_ASSERT_EQUAL_INT(4357, d.dangun_year);
}

/* ===== Stem names (all 10) ===== */

void test_stem_name_0(void) { TEST_ASSERT_EQUAL_STRING("Gap", korean_stem_name(0)); }
void test_stem_name_1(void) { TEST_ASSERT_EQUAL_STRING("Eul", korean_stem_name(1)); }
void test_stem_name_2(void) { TEST_ASSERT_EQUAL_STRING("Byeong", korean_stem_name(2)); }
void test_stem_name_3(void) { TEST_ASSERT_EQUAL_STRING("Jeong", korean_stem_name(3)); }
void test_stem_name_4(void) { TEST_ASSERT_EQUAL_STRING("Mu", korean_stem_name(4)); }
void test_stem_name_5(void) { TEST_ASSERT_EQUAL_STRING("Gi", korean_stem_name(5)); }
void test_stem_name_6(void) { TEST_ASSERT_EQUAL_STRING("Gyeong", korean_stem_name(6)); }
void test_stem_name_7(void) { TEST_ASSERT_EQUAL_STRING("Sin", korean_stem_name(7)); }
void test_stem_name_8(void) { TEST_ASSERT_EQUAL_STRING("Im", korean_stem_name(8)); }
void test_stem_name_9(void) { TEST_ASSERT_EQUAL_STRING("Gye", korean_stem_name(9)); }

void test_stem_name_invalid_neg(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_stem_name(-1));
}

void test_stem_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_stem_name(10));
}

/* ===== Branch names (all 12) ===== */

void test_branch_name_0(void) { TEST_ASSERT_EQUAL_STRING("Ja", korean_branch_name(0)); }
void test_branch_name_1(void) { TEST_ASSERT_EQUAL_STRING("Chuk", korean_branch_name(1)); }
void test_branch_name_2(void) { TEST_ASSERT_EQUAL_STRING("In", korean_branch_name(2)); }
void test_branch_name_3(void) { TEST_ASSERT_EQUAL_STRING("Myo", korean_branch_name(3)); }
void test_branch_name_4(void) { TEST_ASSERT_EQUAL_STRING("Jin", korean_branch_name(4)); }
void test_branch_name_5(void) { TEST_ASSERT_EQUAL_STRING("Sa", korean_branch_name(5)); }
void test_branch_name_6(void) { TEST_ASSERT_EQUAL_STRING("O", korean_branch_name(6)); }
void test_branch_name_7(void) { TEST_ASSERT_EQUAL_STRING("Mi", korean_branch_name(7)); }
void test_branch_name_8(void) { TEST_ASSERT_EQUAL_STRING("Sin", korean_branch_name(8)); }
void test_branch_name_9(void) { TEST_ASSERT_EQUAL_STRING("Yu", korean_branch_name(9)); }
void test_branch_name_10(void) { TEST_ASSERT_EQUAL_STRING("Sul", korean_branch_name(10)); }
void test_branch_name_11(void) { TEST_ASSERT_EQUAL_STRING("Hae", korean_branch_name(11)); }

void test_branch_name_invalid_neg(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_branch_name(-1));
}

void test_branch_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_branch_name(12));
}

/* ===== Animal names (all 12) ===== */

void test_animal_name_0(void) { TEST_ASSERT_EQUAL_STRING("Jwi", korean_animal_name(0)); }
void test_animal_name_1(void) { TEST_ASSERT_EQUAL_STRING("So", korean_animal_name(1)); }
void test_animal_name_2(void) { TEST_ASSERT_EQUAL_STRING("Horangi", korean_animal_name(2)); }
void test_animal_name_3(void) { TEST_ASSERT_EQUAL_STRING("Tokki", korean_animal_name(3)); }
void test_animal_name_4(void) { TEST_ASSERT_EQUAL_STRING("Yong", korean_animal_name(4)); }
void test_animal_name_5(void) { TEST_ASSERT_EQUAL_STRING("Baem", korean_animal_name(5)); }
void test_animal_name_6(void) { TEST_ASSERT_EQUAL_STRING("Mal", korean_animal_name(6)); }
void test_animal_name_7(void) { TEST_ASSERT_EQUAL_STRING("Yang", korean_animal_name(7)); }
void test_animal_name_8(void) { TEST_ASSERT_EQUAL_STRING("Wonsungi", korean_animal_name(8)); }
void test_animal_name_9(void) { TEST_ASSERT_EQUAL_STRING("Dak", korean_animal_name(9)); }
void test_animal_name_10(void) { TEST_ASSERT_EQUAL_STRING("Gae", korean_animal_name(10)); }
void test_animal_name_11(void) { TEST_ASSERT_EQUAL_STRING("Dwaeji", korean_animal_name(11)); }

void test_animal_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_animal_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", korean_animal_name(12));
}

/* ===== Element names (all 5) ===== */

void test_element_name_0(void) { TEST_ASSERT_EQUAL_STRING("Mok", korean_element_name(0)); }
void test_element_name_1(void) { TEST_ASSERT_EQUAL_STRING("Hwa", korean_element_name(1)); }
void test_element_name_2(void) { TEST_ASSERT_EQUAL_STRING("To", korean_element_name(2)); }
void test_element_name_3(void) { TEST_ASSERT_EQUAL_STRING("Geum", korean_element_name(3)); }
void test_element_name_4(void) { TEST_ASSERT_EQUAL_STRING("Su", korean_element_name(4)); }

void test_element_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_element_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", korean_element_name(5));
}

/* ===== Polarity names ===== */

void test_polarity_name_yang(void)
{
    TEST_ASSERT_EQUAL_STRING("Yang", korean_polarity_name(0));
}

void test_polarity_name_yin(void)
{
    TEST_ASSERT_EQUAL_STRING("Yin", korean_polarity_name(1));
}

void test_polarity_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_polarity_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", korean_polarity_name(2));
}

/* ===== Sexagenary combination names ===== */

void test_sexagenary_name_1(void)
{
    /* cycle_year 1 = Gap-Ja */
    TEST_ASSERT_EQUAL_STRING("Gap-Ja", korean_sexagenary_name(1));
}

void test_sexagenary_name_2(void)
{
    /* cycle_year 2 = Eul-Chuk */
    TEST_ASSERT_EQUAL_STRING("Eul-Chuk", korean_sexagenary_name(2));
}

void test_sexagenary_name_13(void)
{
    /* cycle_year 13: stem=(13-1)%10=2 = Byeong, branch=(13-1)%12=0 = Ja */
    TEST_ASSERT_EQUAL_STRING("Byeong-Ja", korean_sexagenary_name(13));
}

void test_sexagenary_name_60(void)
{
    /* cycle_year 60: stem=(60-1)%10=9 = Gye, branch=(60-1)%12=11 = Hae */
    TEST_ASSERT_EQUAL_STRING("Gye-Hae", korean_sexagenary_name(60));
}

void test_sexagenary_name_invalid_0(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_sexagenary_name(0));
}

void test_sexagenary_name_invalid_61(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_sexagenary_name(61));
}

void test_sexagenary_name_invalid_neg(void)
{
    TEST_ASSERT_EQUAL_STRING("?", korean_sexagenary_name(-1));
}

/* ===== Festival detection ===== */

void test_festival_seollal(void)
{
    const korean_festival_t *f = korean_festival(1, 1);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Seollal", f->name);
    TEST_ASSERT_EQUAL_INT(0, f->id);
}

void test_festival_daeboreum(void)
{
    const korean_festival_t *f = korean_festival(1, 15);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Daeboreum", f->name);
    TEST_ASSERT_EQUAL_INT(1, f->id);
}

void test_festival_samjinnal(void)
{
    const korean_festival_t *f = korean_festival(3, 3);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Samjinnal", f->name);
}

void test_festival_dano(void)
{
    const korean_festival_t *f = korean_festival(5, 5);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Dano", f->name);
}

void test_festival_chilseok(void)
{
    const korean_festival_t *f = korean_festival(7, 7);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Chilseok", f->name);
}

void test_festival_chuseok(void)
{
    const korean_festival_t *f = korean_festival(8, 15);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Chuseok", f->name);
    TEST_ASSERT_EQUAL_INT(5, f->id);
}

void test_festival_jungyangjeol(void)
{
    const korean_festival_t *f = korean_festival(9, 9);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_EQUAL_STRING("Jungyangjeol", f->name);
    TEST_ASSERT_EQUAL_INT(6, f->id);
}

void test_festival_none(void)
{
    const korean_festival_t *f = korean_festival(2, 14);
    TEST_ASSERT_NULL(f);
}

void test_festival_none_wrong_day(void)
{
    const korean_festival_t *f = korean_festival(1, 2);
    TEST_ASSERT_NULL(f);
}

/* ===== Festival by index ===== */

void test_festival_by_index_0(void)
{
    korean_festival_t f = korean_festival_by_index(0);
    TEST_ASSERT_EQUAL_INT(0, f.id);
    TEST_ASSERT_EQUAL_STRING("Seollal", f.name);
    TEST_ASSERT_EQUAL_INT(1, f.lunar_month);
    TEST_ASSERT_EQUAL_INT(1, f.lunar_day);
}

void test_festival_by_index_5(void)
{
    korean_festival_t f = korean_festival_by_index(5);
    TEST_ASSERT_EQUAL_INT(5, f.id);
    TEST_ASSERT_EQUAL_STRING("Chuseok", f.name);
    TEST_ASSERT_EQUAL_INT(8, f.lunar_month);
    TEST_ASSERT_EQUAL_INT(15, f.lunar_day);
}

void test_festival_by_index_6(void)
{
    korean_festival_t f = korean_festival_by_index(6);
    TEST_ASSERT_EQUAL_INT(6, f.id);
    TEST_ASSERT_EQUAL_STRING("Jungyangjeol", f.name);
}

void test_festival_by_index_invalid(void)
{
    korean_festival_t f = korean_festival_by_index(-1);
    TEST_ASSERT_EQUAL_INT(-1, f.id);
}

void test_festival_by_index_out_of_range(void)
{
    korean_festival_t f = korean_festival_by_index(7);
    TEST_ASSERT_EQUAL_INT(-1, f.id);
}

/* ===== Festival count ===== */

void test_festival_count(void)
{
    TEST_ASSERT_EQUAL_INT(KOREAN_FESTIVAL_COUNT, korean_festival_count());
    TEST_ASSERT_EQUAL_INT(7, korean_festival_count());
}

/* ===== Festival Korean script names ===== */

void test_festival_korean_script_seollal(void)
{
    const korean_festival_t *f = korean_festival(1, 1);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_NOT_NULL(f->korean);
}

void test_festival_korean_script_chuseok(void)
{
    const korean_festival_t *f = korean_festival(8, 15);
    TEST_ASSERT_NOT_NULL(f);
    TEST_ASSERT_NOT_NULL(f->korean);
}

/* ===== Seollal JD approximation ===== */

void test_seollal_2000(void)
{
    /* Seollal 2000 ~ Feb 5, 2000 */
    double jd = korean_seollal_jd(2000);
    /* Feb 5 2000 = JD ~2451579.5. Allow +/- 2 day tolerance for approximate new moon. */
    TEST_ASSERT_TRUE(fabs(jd - 2451579.5) <= 2.0);
}

void test_seollal_2024(void)
{
    /* Seollal 2024 ~ Feb 10, 2024 */
    double jd = korean_seollal_jd(2024);
    /* Feb 10 2024 = JD ~2460350.5 */
    TEST_ASSERT_TRUE(fabs(jd - 2460350.5) <= 2.0);
}

void test_seollal_2025(void)
{
    /* Seollal 2025 ~ Jan 29, 2025 */
    double jd = korean_seollal_jd(2025);
    /* Jan 29 2025 = JD ~2460704.5 */
    TEST_ASSERT_TRUE(fabs(jd - 2460704.5) <= 2.0);
}

void test_seollal_2026(void)
{
    /* Seollal 2026 ~ Feb 17, 2026 */
    double jd = korean_seollal_jd(2026);
    /* Feb 17 2026 = JD ~2461088.5 */
    TEST_ASSERT_TRUE(fabs(jd - 2461088.5) <= 2.0);
}

/* ===== Lunar month/day from JD ===== */

void test_lunar_month_seollal_2000(void)
{
    /* At Seollal, lunar month should be 1, lunar day should be ~1 */
    double jd = korean_seollal_jd(2000);
    korean_date_t d = korean_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, d.lunar_month);
    TEST_ASSERT_INT_WITHIN(1, 1, d.lunar_day); /* day 1 +/- 1 */
}

void test_lunar_month_chuseok_approx(void)
{
    /* Chuseok is lunar month 8, day 15.
     * Use korean_to_jd to get approximate JD, then verify round-trip. */
    double jd = korean_to_jd(2024, 8, 15);
    korean_date_t d = korean_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(8, d.lunar_month);
    TEST_ASSERT_INT_WITHIN(1, 15, d.lunar_day);
}

/* ===== JD round-trip ===== */

void test_jd_roundtrip_lunar_1_1(void)
{
    double jd = korean_to_jd(2024, 1, 1);
    korean_date_t d = korean_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(1, d.lunar_month);
    TEST_ASSERT_INT_WITHIN(1, 1, d.lunar_day);
}

void test_jd_roundtrip_lunar_5_5(void)
{
    double jd = korean_to_jd(2025, 5, 5);
    korean_date_t d = korean_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(5, d.lunar_month);
    TEST_ASSERT_INT_WITHIN(1, 5, d.lunar_day);
}

void test_jd_roundtrip_lunar_7_7(void)
{
    double jd = korean_to_jd(2025, 7, 7);
    korean_date_t d = korean_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(7, d.lunar_month);
    TEST_ASSERT_INT_WITHIN(1, 7, d.lunar_day);
}

/* ===== korean_from_jd before/after Lunar New Year ===== */

void test_from_jd_before_seollal_2025(void)
{
    /* Jan 1 2025 is before Seollal 2025 (~Jan 29).
     * Should still be in the 2024 Korean/lunar year. */
    double jd_jan1 = 2460676.5; /* Jan 1, 2025 */
    korean_date_t d = korean_from_jd(jd_jan1);
    TEST_ASSERT_EQUAL_INT(2024, d.gregorian_year);
}

void test_from_jd_after_seollal_2025(void)
{
    /* Feb 15 2025 is after Seollal 2025 (~Jan 29).
     * Should be in the 2025 Korean year. */
    double jd_feb15 = 2460721.5; /* Feb 15, 2025 */
    korean_date_t d = korean_from_jd(jd_feb15);
    TEST_ASSERT_EQUAL_INT(2025, d.gregorian_year);
}

/* ===== korean_to_jd basic ===== */

void test_to_jd_returns_valid(void)
{
    double jd = korean_to_jd(2024, 1, 1);
    /* Should be near Seollal 2024 (~Feb 10) */
    TEST_ASSERT_TRUE(fabs(jd - 2460350.5) <= 3.0);
}

void test_to_jd_month_8_day_15(void)
{
    /* Chuseok 2024 is lunar 8/15 */
    double jd = korean_to_jd(2024, 8, 15);
    /* Should be ~Sep 17, 2024 = JD ~2460570.5 */
    TEST_ASSERT_TRUE(fabs(jd - 2460570.5) <= 3.0);
}

/* ===== All 60 sexagenary names spot checks ===== */

void test_sexagenary_name_3(void)
{
    /* cycle_year 3: stem 2=Byeong, branch 2=In */
    TEST_ASSERT_EQUAL_STRING("Byeong-In", korean_sexagenary_name(3));
}

void test_sexagenary_name_11(void)
{
    /* cycle_year 11: stem=(11-1)%10=0=Gap, branch=(11-1)%12=10=Sul */
    TEST_ASSERT_EQUAL_STRING("Gap-Sul", korean_sexagenary_name(11));
}

void test_sexagenary_name_21(void)
{
    /* cycle_year 21: stem=(21-1)%10=0=Gap, branch=(21-1)%12=8=Sin */
    TEST_ASSERT_EQUAL_STRING("Gap-Sin", korean_sexagenary_name(21));
}

void test_sexagenary_name_30(void)
{
    /* cycle_year 30: stem=(30-1)%10=9=Gye, branch=(30-1)%12=5=Sa */
    TEST_ASSERT_EQUAL_STRING("Gye-Sa", korean_sexagenary_name(30));
}

void test_sexagenary_name_31(void)
{
    /* cycle_year 31: stem=(31-1)%10=0=Gap, branch=(31-1)%12=6=O */
    TEST_ASSERT_EQUAL_STRING("Gap-O", korean_sexagenary_name(31));
}

void test_sexagenary_name_41(void)
{
    /* cycle_year 41: stem=(41-1)%10=0=Gap, branch=(41-1)%12=4=Jin */
    TEST_ASSERT_EQUAL_STRING("Gap-Jin", korean_sexagenary_name(41));
}

void test_sexagenary_name_51(void)
{
    /* cycle_year 51: stem=(51-1)%10=0=Gap, branch=(51-1)%12=2=In */
    TEST_ASSERT_EQUAL_STRING("Gap-In", korean_sexagenary_name(51));
}

/* ===== Lunar day bounds ===== */

void test_lunar_day_positive(void)
{
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_TRUE(d.lunar_day >= 1 && d.lunar_day <= 30);
}

void test_lunar_month_valid_range(void)
{
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_TRUE(d.lunar_month >= 1 && d.lunar_month <= 12);
}

/* ===== Edge: cycle_year range ===== */

void test_cycle_year_range(void)
{
    korean_date_t d = korean_from_jd(2451600.0);
    TEST_ASSERT_TRUE(d.cycle_year >= 1 && d.cycle_year <= 60);
}

/* ===== Main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Dangun year */
    RUN_TEST(test_dangun_year_2024);
    RUN_TEST(test_dangun_year_2000);
    RUN_TEST(test_dangun_year_2025);
    RUN_TEST(test_dangun_year_2026);
    RUN_TEST(test_dangun_year_1);
    RUN_TEST(test_dangun_year_1984);
    RUN_TEST(test_dangun_year_1988);
    RUN_TEST(test_dangun_year_negative);
    RUN_TEST(test_dangun_year_zero);

    /* Sexagenary cycle */
    RUN_TEST(test_sexagenary_2000_stem);
    RUN_TEST(test_sexagenary_2000_branch);
    RUN_TEST(test_sexagenary_2000_animal);
    RUN_TEST(test_sexagenary_2024_stem);
    RUN_TEST(test_sexagenary_2024_branch);
    RUN_TEST(test_sexagenary_2025_stem);
    RUN_TEST(test_sexagenary_2025_branch);
    RUN_TEST(test_sexagenary_2026_stem);
    RUN_TEST(test_sexagenary_2026_branch);
    RUN_TEST(test_sexagenary_1984_stem);
    RUN_TEST(test_sexagenary_1984_branch);
    RUN_TEST(test_sexagenary_1984_cycle_year);
    RUN_TEST(test_sexagenary_1988_stem);
    RUN_TEST(test_sexagenary_1988_branch);
    RUN_TEST(test_sexagenary_element_2000);
    RUN_TEST(test_sexagenary_polarity_2000);
    RUN_TEST(test_sexagenary_polarity_2025);
    RUN_TEST(test_sexagenary_dangun_2000);
    RUN_TEST(test_sexagenary_dangun_2024);

    /* Stem names */
    RUN_TEST(test_stem_name_0);
    RUN_TEST(test_stem_name_1);
    RUN_TEST(test_stem_name_2);
    RUN_TEST(test_stem_name_3);
    RUN_TEST(test_stem_name_4);
    RUN_TEST(test_stem_name_5);
    RUN_TEST(test_stem_name_6);
    RUN_TEST(test_stem_name_7);
    RUN_TEST(test_stem_name_8);
    RUN_TEST(test_stem_name_9);
    RUN_TEST(test_stem_name_invalid_neg);
    RUN_TEST(test_stem_name_invalid_high);

    /* Branch names */
    RUN_TEST(test_branch_name_0);
    RUN_TEST(test_branch_name_1);
    RUN_TEST(test_branch_name_2);
    RUN_TEST(test_branch_name_3);
    RUN_TEST(test_branch_name_4);
    RUN_TEST(test_branch_name_5);
    RUN_TEST(test_branch_name_6);
    RUN_TEST(test_branch_name_7);
    RUN_TEST(test_branch_name_8);
    RUN_TEST(test_branch_name_9);
    RUN_TEST(test_branch_name_10);
    RUN_TEST(test_branch_name_11);
    RUN_TEST(test_branch_name_invalid_neg);
    RUN_TEST(test_branch_name_invalid_high);

    /* Animal names */
    RUN_TEST(test_animal_name_0);
    RUN_TEST(test_animal_name_1);
    RUN_TEST(test_animal_name_2);
    RUN_TEST(test_animal_name_3);
    RUN_TEST(test_animal_name_4);
    RUN_TEST(test_animal_name_5);
    RUN_TEST(test_animal_name_6);
    RUN_TEST(test_animal_name_7);
    RUN_TEST(test_animal_name_8);
    RUN_TEST(test_animal_name_9);
    RUN_TEST(test_animal_name_10);
    RUN_TEST(test_animal_name_11);
    RUN_TEST(test_animal_name_invalid);

    /* Element names */
    RUN_TEST(test_element_name_0);
    RUN_TEST(test_element_name_1);
    RUN_TEST(test_element_name_2);
    RUN_TEST(test_element_name_3);
    RUN_TEST(test_element_name_4);
    RUN_TEST(test_element_name_invalid);

    /* Polarity names */
    RUN_TEST(test_polarity_name_yang);
    RUN_TEST(test_polarity_name_yin);
    RUN_TEST(test_polarity_name_invalid);

    /* Sexagenary combination names */
    RUN_TEST(test_sexagenary_name_1);
    RUN_TEST(test_sexagenary_name_2);
    RUN_TEST(test_sexagenary_name_13);
    RUN_TEST(test_sexagenary_name_60);
    RUN_TEST(test_sexagenary_name_invalid_0);
    RUN_TEST(test_sexagenary_name_invalid_61);
    RUN_TEST(test_sexagenary_name_invalid_neg);
    RUN_TEST(test_sexagenary_name_3);
    RUN_TEST(test_sexagenary_name_11);
    RUN_TEST(test_sexagenary_name_21);
    RUN_TEST(test_sexagenary_name_30);
    RUN_TEST(test_sexagenary_name_31);
    RUN_TEST(test_sexagenary_name_41);
    RUN_TEST(test_sexagenary_name_51);

    /* Festival detection */
    RUN_TEST(test_festival_seollal);
    RUN_TEST(test_festival_daeboreum);
    RUN_TEST(test_festival_samjinnal);
    RUN_TEST(test_festival_dano);
    RUN_TEST(test_festival_chilseok);
    RUN_TEST(test_festival_chuseok);
    RUN_TEST(test_festival_jungyangjeol);
    RUN_TEST(test_festival_none);
    RUN_TEST(test_festival_none_wrong_day);

    /* Festival by index */
    RUN_TEST(test_festival_by_index_0);
    RUN_TEST(test_festival_by_index_5);
    RUN_TEST(test_festival_by_index_6);
    RUN_TEST(test_festival_by_index_invalid);
    RUN_TEST(test_festival_by_index_out_of_range);

    /* Festival count */
    RUN_TEST(test_festival_count);

    /* Festival Korean script */
    RUN_TEST(test_festival_korean_script_seollal);
    RUN_TEST(test_festival_korean_script_chuseok);

    /* Seollal JD */
    RUN_TEST(test_seollal_2000);
    RUN_TEST(test_seollal_2024);
    RUN_TEST(test_seollal_2025);
    RUN_TEST(test_seollal_2026);

    /* Lunar month/day */
    RUN_TEST(test_lunar_month_seollal_2000);
    RUN_TEST(test_lunar_month_chuseok_approx);

    /* JD round-trip */
    RUN_TEST(test_jd_roundtrip_lunar_1_1);
    RUN_TEST(test_jd_roundtrip_lunar_5_5);
    RUN_TEST(test_jd_roundtrip_lunar_7_7);

    /* Before/after Seollal */
    RUN_TEST(test_from_jd_before_seollal_2025);
    RUN_TEST(test_from_jd_after_seollal_2025);

    /* korean_to_jd */
    RUN_TEST(test_to_jd_returns_valid);
    RUN_TEST(test_to_jd_month_8_day_15);

    /* Bounds */
    RUN_TEST(test_lunar_day_positive);
    RUN_TEST(test_lunar_month_valid_range);
    RUN_TEST(test_cycle_year_range);

    return UNITY_END();
}
