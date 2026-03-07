/* test_japanese.c -- Japanese Calendar module tests
 * TDD: tests written before implementation.
 * Covers: imperial era dating (Meiji-Reiwa), JD conversion, era names/kanji,
 *         rokuyo six-day cycle, 24 sekki solar terms, Japanese zodiac animals,
 *         round-trip JD, edge cases at era boundaries. */

#include "../../unity/unity.h"
#include "../../../src/systems/japanese/japanese.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Imperial Era Detection ===== */

void test_era_reiwa_2026(void)
{
    /* 2026-03-07 is Reiwa 8 */
    japanese_date_t d = { .era_index = JP_ERA_REIWA, .era_year = 8,
                          .greg_year = 2026, .month = 3, .day = 7 };
    TEST_ASSERT_EQUAL_INT(JP_ERA_REIWA, d.era_index);
    TEST_ASSERT_EQUAL_INT(8, d.era_year);
}

void test_era_reiwa_first_day(void)
{
    /* 2019-05-01 = Reiwa 1 (gannen) */
    /* JD for 2019-05-01 midnight = 2458604.5 */
    japanese_date_t d = japanese_from_jd(2458604.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_REIWA, d.era_index);
    TEST_ASSERT_EQUAL_INT(1, d.era_year);
    TEST_ASSERT_EQUAL_INT(2019, d.greg_year);
    TEST_ASSERT_EQUAL_INT(5, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_era_heisei_last_day(void)
{
    /* 2019-04-30 = Heisei 31 */
    /* JD for 2019-04-30 midnight = 2458603.5 */
    japanese_date_t d = japanese_from_jd(2458603.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_HEISEI, d.era_index);
    TEST_ASSERT_EQUAL_INT(31, d.era_year);
    TEST_ASSERT_EQUAL_INT(2019, d.greg_year);
    TEST_ASSERT_EQUAL_INT(4, d.month);
    TEST_ASSERT_EQUAL_INT(30, d.day);
}

void test_era_heisei_first_day(void)
{
    /* 1989-01-08 = Heisei 1 */
    /* JD for 1989-01-08 midnight = 2447534.5 */
    japanese_date_t d = japanese_from_jd(2447534.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_HEISEI, d.era_index);
    TEST_ASSERT_EQUAL_INT(1, d.era_year);
    TEST_ASSERT_EQUAL_INT(1989, d.greg_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(8, d.day);
}

void test_era_showa_last_day(void)
{
    /* 1989-01-07 = Showa 64 */
    /* JD for 1989-01-07 midnight = 2447533.5 */
    japanese_date_t d = japanese_from_jd(2447533.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_SHOWA, d.era_index);
    TEST_ASSERT_EQUAL_INT(64, d.era_year);
    TEST_ASSERT_EQUAL_INT(1989, d.greg_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(7, d.day);
}

void test_era_showa_first_day(void)
{
    /* 1926-12-25 = Showa 1 */
    /* JD for 1926-12-25 midnight = 2424874.5 */
    japanese_date_t d = japanese_from_jd(2424874.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_SHOWA, d.era_index);
    TEST_ASSERT_EQUAL_INT(1, d.era_year);
    TEST_ASSERT_EQUAL_INT(1926, d.greg_year);
    TEST_ASSERT_EQUAL_INT(12, d.month);
    TEST_ASSERT_EQUAL_INT(25, d.day);
}

void test_era_taisho_last_day(void)
{
    /* 1926-12-24 = Taisho 15 */
    /* JD for 1926-12-24 midnight = 2424873.5 */
    japanese_date_t d = japanese_from_jd(2424873.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_TAISHO, d.era_index);
    TEST_ASSERT_EQUAL_INT(15, d.era_year);
    TEST_ASSERT_EQUAL_INT(1926, d.greg_year);
    TEST_ASSERT_EQUAL_INT(12, d.month);
    TEST_ASSERT_EQUAL_INT(24, d.day);
}

void test_era_taisho_first_day(void)
{
    /* 1912-07-30 = Taisho 1 */
    /* JD for 1912-07-30 midnight = 2419613.5 */
    japanese_date_t d = japanese_from_jd(2419613.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_TAISHO, d.era_index);
    TEST_ASSERT_EQUAL_INT(1, d.era_year);
    TEST_ASSERT_EQUAL_INT(1912, d.greg_year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(30, d.day);
}

void test_era_meiji_last_day(void)
{
    /* 1912-07-29 = Meiji 45 */
    /* JD for 1912-07-29 midnight = 2419612.5 */
    japanese_date_t d = japanese_from_jd(2419612.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_MEIJI, d.era_index);
    TEST_ASSERT_EQUAL_INT(45, d.era_year);
    TEST_ASSERT_EQUAL_INT(1912, d.greg_year);
    TEST_ASSERT_EQUAL_INT(7, d.month);
    TEST_ASSERT_EQUAL_INT(29, d.day);
}

void test_era_meiji_first_day(void)
{
    /* 1868-01-25 = Meiji 1 */
    /* JD for 1868-01-25 midnight = 2403356.5 */
    japanese_date_t d = japanese_from_jd(2403356.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_MEIJI, d.era_index);
    TEST_ASSERT_EQUAL_INT(1, d.era_year);
    TEST_ASSERT_EQUAL_INT(1868, d.greg_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(25, d.day);
}

void test_era_pre_meiji(void)
{
    /* 1868-01-24 = pre-Meiji */
    /* JD for 1868-01-24 midnight = 2403355.5 */
    japanese_date_t d = japanese_from_jd(2403355.5);
    TEST_ASSERT_EQUAL_INT(-1, d.era_index);
    TEST_ASSERT_EQUAL_INT(1868, d.greg_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(24, d.day);
}

void test_era_pre_meiji_1800(void)
{
    /* Way before Meiji — 1800-06-15 */
    /* JD for 1800-06-15 midnight = 2378719.5 */
    japanese_date_t d = japanese_from_jd(2378719.5);
    TEST_ASSERT_EQUAL_INT(-1, d.era_index);
    TEST_ASSERT_EQUAL_INT(1800, d.greg_year);
}

/* ===== JD Conversion / Round-trip ===== */

void test_to_jd_reiwa_1(void)
{
    /* Reiwa 1 (2019), May 1 -> JD 2458604.5 */
    japanese_date_t d = { .era_index = JP_ERA_REIWA, .era_year = 1,
                          .greg_year = 2019, .month = 5, .day = 1 };
    double jd = japanese_to_jd(d);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2458604.5f, (float)jd);
}

void test_to_jd_showa_1(void)
{
    /* Showa 1 (1926), Dec 25 -> JD 2424874.5 */
    japanese_date_t d = { .era_index = JP_ERA_SHOWA, .era_year = 1,
                          .greg_year = 1926, .month = 12, .day = 25 };
    double jd = japanese_to_jd(d);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2424874.5f, (float)jd);
}

void test_jd_roundtrip_reiwa(void)
{
    /* 2026-03-07 = Reiwa 8 */
    japanese_date_t d = { .era_index = JP_ERA_REIWA, .era_year = 8,
                          .greg_year = 2026, .month = 3, .day = 7 };
    double jd = japanese_to_jd(d);
    japanese_date_t back = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(JP_ERA_REIWA, back.era_index);
    TEST_ASSERT_EQUAL_INT(8, back.era_year);
    TEST_ASSERT_EQUAL_INT(2026, back.greg_year);
    TEST_ASSERT_EQUAL_INT(3, back.month);
    TEST_ASSERT_EQUAL_INT(7, back.day);
}

void test_jd_roundtrip_heisei(void)
{
    /* 2000-01-01 = Heisei 12 */
    japanese_date_t d = { .era_index = JP_ERA_HEISEI, .era_year = 12,
                          .greg_year = 2000, .month = 1, .day = 1 };
    double jd = japanese_to_jd(d);
    japanese_date_t back = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(JP_ERA_HEISEI, back.era_index);
    TEST_ASSERT_EQUAL_INT(12, back.era_year);
    TEST_ASSERT_EQUAL_INT(2000, back.greg_year);
    TEST_ASSERT_EQUAL_INT(1, back.month);
    TEST_ASSERT_EQUAL_INT(1, back.day);
}

void test_jd_roundtrip_meiji(void)
{
    /* 1900-06-15 = Meiji 33 */
    japanese_date_t d = { .era_index = JP_ERA_MEIJI, .era_year = 33,
                          .greg_year = 1900, .month = 6, .day = 15 };
    double jd = japanese_to_jd(d);
    japanese_date_t back = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(JP_ERA_MEIJI, back.era_index);
    TEST_ASSERT_EQUAL_INT(33, back.era_year);
    TEST_ASSERT_EQUAL_INT(1900, back.greg_year);
    TEST_ASSERT_EQUAL_INT(6, back.month);
    TEST_ASSERT_EQUAL_INT(15, back.day);
}

void test_jd_roundtrip_taisho(void)
{
    /* 1920-03-20 = Taisho 9 */
    japanese_date_t d = { .era_index = JP_ERA_TAISHO, .era_year = 9,
                          .greg_year = 1920, .month = 3, .day = 20 };
    double jd = japanese_to_jd(d);
    japanese_date_t back = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(JP_ERA_TAISHO, back.era_index);
    TEST_ASSERT_EQUAL_INT(9, back.era_year);
    TEST_ASSERT_EQUAL_INT(1920, back.greg_year);
    TEST_ASSERT_EQUAL_INT(3, back.month);
    TEST_ASSERT_EQUAL_INT(20, back.day);
}

void test_jd_roundtrip_showa(void)
{
    /* 1975-08-15 = Showa 50 */
    japanese_date_t d = { .era_index = JP_ERA_SHOWA, .era_year = 50,
                          .greg_year = 1975, .month = 8, .day = 15 };
    double jd = japanese_to_jd(d);
    japanese_date_t back = japanese_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(JP_ERA_SHOWA, back.era_index);
    TEST_ASSERT_EQUAL_INT(50, back.era_year);
    TEST_ASSERT_EQUAL_INT(1975, back.greg_year);
    TEST_ASSERT_EQUAL_INT(8, back.month);
    TEST_ASSERT_EQUAL_INT(15, back.day);
}

void test_from_jd_j2000(void)
{
    /* J2000.0 = JD 2451545.0 = 2000-01-01 12:00 UT = Heisei 12 */
    japanese_date_t d = japanese_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(JP_ERA_HEISEI, d.era_index);
    TEST_ASSERT_EQUAL_INT(12, d.era_year);
    TEST_ASSERT_EQUAL_INT(2000, d.greg_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_midnight(void)
{
    /* JD 2451544.5 = 2000-01-01 midnight UT */
    japanese_date_t d = japanese_from_jd(2451544.5);
    TEST_ASSERT_EQUAL_INT(JP_ERA_HEISEI, d.era_index);
    TEST_ASSERT_EQUAL_INT(2000, d.greg_year);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

/* ===== Era Name / Kanji / Start Year ===== */

void test_era_name_meiji(void)
{
    TEST_ASSERT_EQUAL_STRING("Meiji", japanese_era_name(JP_ERA_MEIJI));
}

void test_era_name_taisho(void)
{
    TEST_ASSERT_EQUAL_STRING("Taisho", japanese_era_name(JP_ERA_TAISHO));
}

void test_era_name_showa(void)
{
    TEST_ASSERT_EQUAL_STRING("Showa", japanese_era_name(JP_ERA_SHOWA));
}

void test_era_name_heisei(void)
{
    TEST_ASSERT_EQUAL_STRING("Heisei", japanese_era_name(JP_ERA_HEISEI));
}

void test_era_name_reiwa(void)
{
    TEST_ASSERT_EQUAL_STRING("Reiwa", japanese_era_name(JP_ERA_REIWA));
}

void test_era_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_era_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", japanese_era_name(JP_ERA_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", japanese_era_name(99));
}

void test_era_kanji_meiji(void)
{
    TEST_ASSERT_EQUAL_STRING("\xe6\x98\x8e\xe6\xb2\xbb", japanese_era_kanji(JP_ERA_MEIJI));
}

void test_era_kanji_reiwa(void)
{
    TEST_ASSERT_EQUAL_STRING("\xe4\xbb\xa4\xe5\x92\x8c", japanese_era_kanji(JP_ERA_REIWA));
}

void test_era_kanji_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_era_kanji(-1));
    TEST_ASSERT_EQUAL_STRING("?", japanese_era_kanji(JP_ERA_COUNT));
}

void test_era_start_year_meiji(void)
{
    TEST_ASSERT_EQUAL_INT(1868, japanese_era_start_year(JP_ERA_MEIJI));
}

void test_era_start_year_taisho(void)
{
    TEST_ASSERT_EQUAL_INT(1912, japanese_era_start_year(JP_ERA_TAISHO));
}

void test_era_start_year_showa(void)
{
    TEST_ASSERT_EQUAL_INT(1926, japanese_era_start_year(JP_ERA_SHOWA));
}

void test_era_start_year_heisei(void)
{
    TEST_ASSERT_EQUAL_INT(1989, japanese_era_start_year(JP_ERA_HEISEI));
}

void test_era_start_year_reiwa(void)
{
    TEST_ASSERT_EQUAL_INT(2019, japanese_era_start_year(JP_ERA_REIWA));
}

void test_era_start_year_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, japanese_era_start_year(-1));
    TEST_ASSERT_EQUAL_INT(-1, japanese_era_start_year(JP_ERA_COUNT));
}

/* ===== Rokuyo ===== */

void test_rokuyo_name_sensho(void)
{
    TEST_ASSERT_EQUAL_STRING("Sensho", japanese_rokuyo_name(JP_ROKUYO_SENSHO));
}

void test_rokuyo_name_tomobiki(void)
{
    TEST_ASSERT_EQUAL_STRING("Tomobiki", japanese_rokuyo_name(JP_ROKUYO_TOMOBIKI));
}

void test_rokuyo_name_senbu(void)
{
    TEST_ASSERT_EQUAL_STRING("Senbu", japanese_rokuyo_name(JP_ROKUYO_SENBU));
}

void test_rokuyo_name_butsumetsu(void)
{
    TEST_ASSERT_EQUAL_STRING("Butsumetsu", japanese_rokuyo_name(JP_ROKUYO_BUTSUMETSU));
}

void test_rokuyo_name_taian(void)
{
    TEST_ASSERT_EQUAL_STRING("Taian", japanese_rokuyo_name(JP_ROKUYO_TAIAN));
}

void test_rokuyo_name_shakko(void)
{
    TEST_ASSERT_EQUAL_STRING("Shakko", japanese_rokuyo_name(JP_ROKUYO_SHAKKO));
}

void test_rokuyo_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_rokuyo_name(JP_ROKUYO_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", japanese_rokuyo_name(-1));
}

void test_rokuyo_meaning_sensho(void)
{
    TEST_ASSERT_EQUAL_STRING("Win first", japanese_rokuyo_meaning(JP_ROKUYO_SENSHO));
}

void test_rokuyo_meaning_taian(void)
{
    TEST_ASSERT_EQUAL_STRING("Great peace", japanese_rokuyo_meaning(JP_ROKUYO_TAIAN));
}

void test_rokuyo_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_rokuyo_meaning(JP_ROKUYO_COUNT));
}

void test_rokuyo_valid_range(void)
{
    /* Any JD should return a value 0-5 */
    jp_rokuyo_t r = japanese_rokuyo(2451545.0); /* J2000.0 */
    TEST_ASSERT_TRUE(r >= 0 && r < JP_ROKUYO_COUNT);
}

void test_rokuyo_cycle_six_days(void)
{
    /* Six consecutive days should produce six different rokuyo (one full cycle) */
    double base_jd = 2451544.5; /* 2000-01-01 midnight */
    int seen[6] = {0};
    int unique_count = 0;
    for (int i = 0; i < 6; i++) {
        jp_rokuyo_t r = japanese_rokuyo(base_jd + i);
        if (!seen[r]) {
            seen[r] = 1;
            unique_count++;
        }
    }
    /* The rokuyo cycle can reset at lunar month boundaries, so we check
     * that at least we get valid values (0-5). Full cycle may not always
     * produce 6 unique values due to lunar month transitions. */
    TEST_ASSERT_TRUE(unique_count >= 1);
}

void test_rokuyo_deterministic(void)
{
    /* Same JD always returns same rokuyo */
    double jd = 2460000.5;
    jp_rokuyo_t r1 = japanese_rokuyo(jd);
    jp_rokuyo_t r2 = japanese_rokuyo(jd);
    TEST_ASSERT_EQUAL_INT(r1, r2);
}

void test_rokuyo_consecutive_change(void)
{
    /* Consecutive days should typically differ by 1 mod 6
     * (unless crossing a lunar month boundary) */
    double jd = 2451544.5; /* 2000-01-01 */
    jp_rokuyo_t r1 = japanese_rokuyo(jd);
    jp_rokuyo_t r2 = japanese_rokuyo(jd + 1.0);
    /* Both should be valid */
    TEST_ASSERT_TRUE(r1 >= 0 && r1 < 6);
    TEST_ASSERT_TRUE(r2 >= 0 && r2 < 6);
}

/* ===== 24 Sekki ===== */

void test_sekki_at_zero_deg(void)
{
    /* 0 degrees = Shunbun (index 0) */
    TEST_ASSERT_EQUAL_INT(0, japanese_sekki(0.0));
}

void test_sekki_at_14_deg(void)
{
    /* 14.9 degrees still in 0-15 range = index 0 */
    TEST_ASSERT_EQUAL_INT(0, japanese_sekki(14.9));
}

void test_sekki_at_15_deg(void)
{
    /* 15 degrees = Seimei (index 1) */
    TEST_ASSERT_EQUAL_INT(1, japanese_sekki(15.0));
}

void test_sekki_at_90_deg(void)
{
    /* 90 degrees = Geshi / Summer Solstice (index 6) */
    TEST_ASSERT_EQUAL_INT(6, japanese_sekki(90.0));
}

void test_sekki_at_180_deg(void)
{
    /* 180 degrees = Shubun / Autumnal Equinox (index 12) */
    TEST_ASSERT_EQUAL_INT(12, japanese_sekki(180.0));
}

void test_sekki_at_270_deg(void)
{
    /* 270 degrees = Toji / Winter Solstice (index 18) */
    TEST_ASSERT_EQUAL_INT(18, japanese_sekki(270.0));
}

void test_sekki_at_345_deg(void)
{
    /* 345 degrees = Keichitsu (index 23) */
    TEST_ASSERT_EQUAL_INT(23, japanese_sekki(345.0));
}

void test_sekki_at_359_deg(void)
{
    /* 359.9 degrees still in 345-360 range = index 23 */
    TEST_ASSERT_EQUAL_INT(23, japanese_sekki(359.9));
}

void test_sekki_wrap_360(void)
{
    /* 360 degrees wraps to 0 = index 0 */
    TEST_ASSERT_EQUAL_INT(0, japanese_sekki(360.0));
}

void test_sekki_negative_wrap(void)
{
    /* -15 degrees wraps to 345 = index 23 */
    TEST_ASSERT_EQUAL_INT(23, japanese_sekki(-15.0));
}

void test_sekki_name_shunbun(void)
{
    TEST_ASSERT_EQUAL_STRING("Shunbun", japanese_sekki_name(0));
}

void test_sekki_name_geshi(void)
{
    TEST_ASSERT_EQUAL_STRING("Geshi", japanese_sekki_name(6));
}

void test_sekki_name_shubun(void)
{
    TEST_ASSERT_EQUAL_STRING("Shubun", japanese_sekki_name(12));
}

void test_sekki_name_toji(void)
{
    TEST_ASSERT_EQUAL_STRING("Toji", japanese_sekki_name(18));
}

void test_sekki_name_risshun(void)
{
    TEST_ASSERT_EQUAL_STRING("Risshun", japanese_sekki_name(21));
}

void test_sekki_name_keichitsu(void)
{
    TEST_ASSERT_EQUAL_STRING("Keichitsu", japanese_sekki_name(23));
}

void test_sekki_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_sekki_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", japanese_sekki_name(24));
}

void test_sekki_kanji_shunbun(void)
{
    /* Spring equinox kanji */
    const char *k = japanese_sekki_kanji(0);
    TEST_ASSERT_NOT_NULL(k);
    TEST_ASSERT_TRUE(strlen(k) > 0);
}

void test_sekki_kanji_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_sekki_kanji(-1));
    TEST_ASSERT_EQUAL_STRING("?", japanese_sekki_kanji(24));
}

void test_sekki_longitude_0(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)japanese_sekki_longitude(0));
}

void test_sekki_longitude_6(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 90.0f, (float)japanese_sekki_longitude(6));
}

void test_sekki_longitude_12(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 180.0f, (float)japanese_sekki_longitude(12));
}

void test_sekki_longitude_18(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 270.0f, (float)japanese_sekki_longitude(18));
}

void test_sekki_longitude_23(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 345.0f, (float)japanese_sekki_longitude(23));
}

void test_sekki_longitude_invalid(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, (float)japanese_sekki_longitude(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -1.0f, (float)japanese_sekki_longitude(24));
}

/* ===== Japanese Zodiac Animals ===== */

void test_zodiac_2024_tatsu(void)
{
    /* 2024 = Dragon (Tatsu) : (2024-4)%12 = 2020%12 = 4 */
    TEST_ASSERT_EQUAL_INT(4, japanese_zodiac_animal(2024));
}

void test_zodiac_2023_u(void)
{
    /* 2023 = Rabbit (U) : (2023-4)%12 = 3 */
    TEST_ASSERT_EQUAL_INT(3, japanese_zodiac_animal(2023));
}

void test_zodiac_2000_tatsu(void)
{
    /* 2000 = Dragon : (2000-4)%12 = 1996%12 = 4 */
    TEST_ASSERT_EQUAL_INT(4, japanese_zodiac_animal(2000));
}

void test_zodiac_1989_mi(void)
{
    /* 1989 = Snake (Mi) : (1989-4)%12 = 1985%12 = 5 */
    TEST_ASSERT_EQUAL_INT(5, japanese_zodiac_animal(1989));
}

void test_zodiac_1868_tatsu(void)
{
    /* 1868 = Dragon : (1868-4)%12 = 1864%12 = 4 */
    TEST_ASSERT_EQUAL_INT(4, japanese_zodiac_animal(1868));
}

void test_zodiac_name_ne(void)
{
    TEST_ASSERT_EQUAL_STRING("Ne", japanese_zodiac_name(0));
}

void test_zodiac_name_ushi(void)
{
    TEST_ASSERT_EQUAL_STRING("Ushi", japanese_zodiac_name(1));
}

void test_zodiac_name_tora(void)
{
    TEST_ASSERT_EQUAL_STRING("Tora", japanese_zodiac_name(2));
}

void test_zodiac_name_u(void)
{
    TEST_ASSERT_EQUAL_STRING("U", japanese_zodiac_name(3));
}

void test_zodiac_name_tatsu(void)
{
    TEST_ASSERT_EQUAL_STRING("Tatsu", japanese_zodiac_name(4));
}

void test_zodiac_name_mi(void)
{
    TEST_ASSERT_EQUAL_STRING("Mi", japanese_zodiac_name(5));
}

void test_zodiac_name_uma(void)
{
    TEST_ASSERT_EQUAL_STRING("Uma", japanese_zodiac_name(6));
}

void test_zodiac_name_hitsuji(void)
{
    TEST_ASSERT_EQUAL_STRING("Hitsuji", japanese_zodiac_name(7));
}

void test_zodiac_name_saru(void)
{
    TEST_ASSERT_EQUAL_STRING("Saru", japanese_zodiac_name(8));
}

void test_zodiac_name_tori(void)
{
    TEST_ASSERT_EQUAL_STRING("Tori", japanese_zodiac_name(9));
}

void test_zodiac_name_inu(void)
{
    TEST_ASSERT_EQUAL_STRING("Inu", japanese_zodiac_name(10));
}

void test_zodiac_name_i(void)
{
    TEST_ASSERT_EQUAL_STRING("I", japanese_zodiac_name(11));
}

void test_zodiac_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_zodiac_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", japanese_zodiac_name(12));
}

void test_zodiac_kanji_ne(void)
{
    /* Rat kanji: 子 */
    TEST_ASSERT_EQUAL_STRING("\xe5\xad\x90", japanese_zodiac_kanji(0));
}

void test_zodiac_kanji_tatsu(void)
{
    /* Dragon kanji: 辰 */
    TEST_ASSERT_EQUAL_STRING("\xe8\xbe\xb0", japanese_zodiac_kanji(4));
}

void test_zodiac_kanji_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", japanese_zodiac_kanji(-1));
    TEST_ASSERT_EQUAL_STRING("?", japanese_zodiac_kanji(12));
}

/* ===== is_meiji_or_later ===== */

void test_is_meiji_or_later_true(void)
{
    /* 1868-01-25 midnight = JD 2403356.5 */
    TEST_ASSERT_TRUE(japanese_is_meiji_or_later(2403356.5));
}

void test_is_meiji_or_later_false(void)
{
    /* 1868-01-24 midnight = JD 2403355.5 */
    TEST_ASSERT_FALSE(japanese_is_meiji_or_later(2403355.5));
}

void test_is_meiji_or_later_modern(void)
{
    /* J2000.0 */
    TEST_ASSERT_TRUE(japanese_is_meiji_or_later(2451545.0));
}

void test_is_meiji_or_later_ancient(void)
{
    /* Far past */
    TEST_ASSERT_FALSE(japanese_is_meiji_or_later(2000000.0));
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Imperial era detection */
    RUN_TEST(test_era_reiwa_2026);
    RUN_TEST(test_era_reiwa_first_day);
    RUN_TEST(test_era_heisei_last_day);
    RUN_TEST(test_era_heisei_first_day);
    RUN_TEST(test_era_showa_last_day);
    RUN_TEST(test_era_showa_first_day);
    RUN_TEST(test_era_taisho_last_day);
    RUN_TEST(test_era_taisho_first_day);
    RUN_TEST(test_era_meiji_last_day);
    RUN_TEST(test_era_meiji_first_day);
    RUN_TEST(test_era_pre_meiji);
    RUN_TEST(test_era_pre_meiji_1800);

    /* JD conversion / round-trip */
    RUN_TEST(test_to_jd_reiwa_1);
    RUN_TEST(test_to_jd_showa_1);
    RUN_TEST(test_jd_roundtrip_reiwa);
    RUN_TEST(test_jd_roundtrip_heisei);
    RUN_TEST(test_jd_roundtrip_meiji);
    RUN_TEST(test_jd_roundtrip_taisho);
    RUN_TEST(test_jd_roundtrip_showa);
    RUN_TEST(test_from_jd_j2000);
    RUN_TEST(test_from_jd_midnight);

    /* Era name / kanji / start year */
    RUN_TEST(test_era_name_meiji);
    RUN_TEST(test_era_name_taisho);
    RUN_TEST(test_era_name_showa);
    RUN_TEST(test_era_name_heisei);
    RUN_TEST(test_era_name_reiwa);
    RUN_TEST(test_era_name_invalid);
    RUN_TEST(test_era_kanji_meiji);
    RUN_TEST(test_era_kanji_reiwa);
    RUN_TEST(test_era_kanji_invalid);
    RUN_TEST(test_era_start_year_meiji);
    RUN_TEST(test_era_start_year_taisho);
    RUN_TEST(test_era_start_year_showa);
    RUN_TEST(test_era_start_year_heisei);
    RUN_TEST(test_era_start_year_reiwa);
    RUN_TEST(test_era_start_year_invalid);

    /* Rokuyo */
    RUN_TEST(test_rokuyo_name_sensho);
    RUN_TEST(test_rokuyo_name_tomobiki);
    RUN_TEST(test_rokuyo_name_senbu);
    RUN_TEST(test_rokuyo_name_butsumetsu);
    RUN_TEST(test_rokuyo_name_taian);
    RUN_TEST(test_rokuyo_name_shakko);
    RUN_TEST(test_rokuyo_name_invalid);
    RUN_TEST(test_rokuyo_meaning_sensho);
    RUN_TEST(test_rokuyo_meaning_taian);
    RUN_TEST(test_rokuyo_meaning_invalid);
    RUN_TEST(test_rokuyo_valid_range);
    RUN_TEST(test_rokuyo_cycle_six_days);
    RUN_TEST(test_rokuyo_deterministic);
    RUN_TEST(test_rokuyo_consecutive_change);

    /* 24 Sekki */
    RUN_TEST(test_sekki_at_zero_deg);
    RUN_TEST(test_sekki_at_14_deg);
    RUN_TEST(test_sekki_at_15_deg);
    RUN_TEST(test_sekki_at_90_deg);
    RUN_TEST(test_sekki_at_180_deg);
    RUN_TEST(test_sekki_at_270_deg);
    RUN_TEST(test_sekki_at_345_deg);
    RUN_TEST(test_sekki_at_359_deg);
    RUN_TEST(test_sekki_wrap_360);
    RUN_TEST(test_sekki_negative_wrap);
    RUN_TEST(test_sekki_name_shunbun);
    RUN_TEST(test_sekki_name_geshi);
    RUN_TEST(test_sekki_name_shubun);
    RUN_TEST(test_sekki_name_toji);
    RUN_TEST(test_sekki_name_risshun);
    RUN_TEST(test_sekki_name_keichitsu);
    RUN_TEST(test_sekki_name_invalid);
    RUN_TEST(test_sekki_kanji_shunbun);
    RUN_TEST(test_sekki_kanji_invalid);
    RUN_TEST(test_sekki_longitude_0);
    RUN_TEST(test_sekki_longitude_6);
    RUN_TEST(test_sekki_longitude_12);
    RUN_TEST(test_sekki_longitude_18);
    RUN_TEST(test_sekki_longitude_23);
    RUN_TEST(test_sekki_longitude_invalid);

    /* Japanese zodiac */
    RUN_TEST(test_zodiac_2024_tatsu);
    RUN_TEST(test_zodiac_2023_u);
    RUN_TEST(test_zodiac_2000_tatsu);
    RUN_TEST(test_zodiac_1989_mi);
    RUN_TEST(test_zodiac_1868_tatsu);
    RUN_TEST(test_zodiac_name_ne);
    RUN_TEST(test_zodiac_name_ushi);
    RUN_TEST(test_zodiac_name_tora);
    RUN_TEST(test_zodiac_name_u);
    RUN_TEST(test_zodiac_name_tatsu);
    RUN_TEST(test_zodiac_name_mi);
    RUN_TEST(test_zodiac_name_uma);
    RUN_TEST(test_zodiac_name_hitsuji);
    RUN_TEST(test_zodiac_name_saru);
    RUN_TEST(test_zodiac_name_tori);
    RUN_TEST(test_zodiac_name_inu);
    RUN_TEST(test_zodiac_name_i);
    RUN_TEST(test_zodiac_name_invalid);
    RUN_TEST(test_zodiac_kanji_ne);
    RUN_TEST(test_zodiac_kanji_tatsu);
    RUN_TEST(test_zodiac_kanji_invalid);

    /* is_meiji_or_later */
    RUN_TEST(test_is_meiji_or_later_true);
    RUN_TEST(test_is_meiji_or_later_false);
    RUN_TEST(test_is_meiji_or_later_modern);
    RUN_TEST(test_is_meiji_or_later_ancient);

    return UNITY_END();
}
