/* test_pawukon.c -- Balinese Pawukon 210-day concurrent cycle calendar tests
 * TDD RED phase: tests written before implementation.
 * Epoch: JD 2379344.5 (15 July 1802 CE) = Wuku Sinta, day 0 of cycle.
 * 10 concurrent week cycles (1-day through 10-day), 30 wuku (named 7-day weeks).
 * Purely ceremonial calendar with no astronomical basis. */

#include "../../unity/unity.h"
#include "../../../src/systems/balinese/pawukon.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants ===== */

static const double EPOCH_JD = 2379344.5;

/* ===== pawukon_from_jd: epoch tests ===== */

void test_from_jd_epoch_day_in_cycle(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.day_in_cycle);
}

void test_from_jd_epoch_wuku(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.wuku);  /* Sinta */
}

void test_from_jd_epoch_wuku_day(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.wuku_day);
}

void test_from_jd_epoch_eka_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.eka_wara);
}

void test_from_jd_epoch_dwi_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.dwi_wara);
}

void test_from_jd_epoch_tri_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.tri_wara);
}

void test_from_jd_epoch_catur_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.catur_wara);
}

void test_from_jd_epoch_panca_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.panca_wara);
}

void test_from_jd_epoch_sad_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.sad_wara);
}

void test_from_jd_epoch_sapta_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.sapta_wara);
}

void test_from_jd_epoch_asta_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.asta_wara);
}

void test_from_jd_epoch_sanga_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.sanga_wara);
}

void test_from_jd_epoch_dasa_wara(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_EQUAL_INT(0, p.dasa_wara);
}

/* ===== pawukon_from_jd: day 1 ===== */

void test_from_jd_day1_in_cycle(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 1.0);
    TEST_ASSERT_EQUAL_INT(1, p.day_in_cycle);
}

void test_from_jd_day1_concurrent_weeks(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 1.0);
    TEST_ASSERT_EQUAL_INT(0, p.eka_wara);   /* always 0 */
    TEST_ASSERT_EQUAL_INT(1, p.dwi_wara);   /* 1%2 */
    TEST_ASSERT_EQUAL_INT(1, p.tri_wara);   /* 1%3 */
    TEST_ASSERT_EQUAL_INT(1, p.catur_wara); /* 1%4 */
    TEST_ASSERT_EQUAL_INT(1, p.panca_wara); /* 1%5 */
    TEST_ASSERT_EQUAL_INT(1, p.sad_wara);   /* 1%6 */
    TEST_ASSERT_EQUAL_INT(1, p.sapta_wara); /* 1%7 */
    TEST_ASSERT_EQUAL_INT(1, p.asta_wara);  /* 1%8 */
    TEST_ASSERT_EQUAL_INT(1, p.sanga_wara); /* 1%9 */
    TEST_ASSERT_EQUAL_INT(1, p.dasa_wara);  /* 1%10 */
}

/* ===== pawukon_from_jd: day 7 (start of wuku Landep) ===== */

void test_from_jd_day7_wuku(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 7.0);
    TEST_ASSERT_EQUAL_INT(1, p.wuku);  /* Landep */
    TEST_ASSERT_EQUAL_INT(0, p.wuku_day);
}

void test_from_jd_day7_concurrent_weeks(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 7.0);
    TEST_ASSERT_EQUAL_INT(0, p.eka_wara);   /* always 0 */
    TEST_ASSERT_EQUAL_INT(1, p.dwi_wara);   /* 7%2 */
    TEST_ASSERT_EQUAL_INT(1, p.tri_wara);   /* 7%3 */
    TEST_ASSERT_EQUAL_INT(3, p.catur_wara); /* 7%4 */
    TEST_ASSERT_EQUAL_INT(2, p.panca_wara); /* 7%5 */
    TEST_ASSERT_EQUAL_INT(1, p.sad_wara);   /* 7%6 */
    TEST_ASSERT_EQUAL_INT(0, p.sapta_wara); /* 7%7 */
    TEST_ASSERT_EQUAL_INT(7, p.asta_wara);  /* 7%8 */
    TEST_ASSERT_EQUAL_INT(7, p.sanga_wara); /* 7%9 */
    TEST_ASSERT_EQUAL_INT(7, p.dasa_wara);  /* 7%10 */
}

/* ===== pawukon_from_jd: day 209 (last day of cycle) ===== */

void test_from_jd_day209(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 209.0);
    TEST_ASSERT_EQUAL_INT(209, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(29, p.wuku);  /* Watugunung */
    TEST_ASSERT_EQUAL_INT(6, p.wuku_day);
    TEST_ASSERT_EQUAL_INT(0, p.eka_wara);   /* always 0 */
    TEST_ASSERT_EQUAL_INT(1, p.dwi_wara);   /* 209%2 */
    TEST_ASSERT_EQUAL_INT(2, p.tri_wara);   /* 209%3 */
    TEST_ASSERT_EQUAL_INT(1, p.catur_wara); /* 209%4 */
    TEST_ASSERT_EQUAL_INT(4, p.panca_wara); /* 209%5 */
    TEST_ASSERT_EQUAL_INT(5, p.sad_wara);   /* 209%6 */
    TEST_ASSERT_EQUAL_INT(6, p.sapta_wara); /* 209%7 */
    TEST_ASSERT_EQUAL_INT(1, p.asta_wara);  /* 209%8 */
    TEST_ASSERT_EQUAL_INT(2, p.sanga_wara); /* 209%9 */
    TEST_ASSERT_EQUAL_INT(9, p.dasa_wara);  /* 209%10 */
}

/* ===== pawukon_from_jd: day 210 (cycle wraps) ===== */

void test_from_jd_day210_cycle_wrap(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 210.0);
    TEST_ASSERT_EQUAL_INT(0, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(0, p.wuku);  /* back to Sinta */
}

/* ===== pawukon_from_jd: multiple cycles ===== */

void test_from_jd_two_cycles(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 420.0);
    TEST_ASSERT_EQUAL_INT(0, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(0, p.wuku);
}

void test_from_jd_three_cycles_plus_35(void)
{
    /* 3*210 + 35 = 665 days from epoch, day_in_cycle = 35 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 665.0);
    TEST_ASSERT_EQUAL_INT(35, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(5, p.wuku);   /* wuku 35/7 = 5 (Gumbreg) */
    TEST_ASSERT_EQUAL_INT(0, p.wuku_day);
}

/* ===== pawukon_from_jd: fractional JD ===== */

void test_from_jd_fractional_noon(void)
{
    /* Noon on epoch day = JD 2379345.0 => still day 0 */
    pawukon_t p = pawukon_from_jd(2379345.0);
    TEST_ASSERT_EQUAL_INT(0, p.day_in_cycle);
}

void test_from_jd_fractional_just_before_midnight(void)
{
    /* Just before next midnight = still day 0 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 0.9999);
    TEST_ASSERT_EQUAL_INT(0, p.day_in_cycle);
}

/* ===== pawukon_from_jd: dates before epoch ===== */

void test_from_jd_before_epoch(void)
{
    /* 1 day before epoch: day_in_cycle = 209 (wrap backward) */
    pawukon_t p = pawukon_from_jd(EPOCH_JD - 1.0);
    TEST_ASSERT_EQUAL_INT(209, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(29, p.wuku); /* Watugunung */
}

void test_from_jd_before_epoch_half_cycle(void)
{
    /* 105 days before epoch: day_in_cycle = 210-105 = 105 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD - 105.0);
    TEST_ASSERT_EQUAL_INT(105, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(15, p.wuku); /* wuku 105/7 = 15 (Pahang) */
}

void test_from_jd_before_epoch_full_cycle(void)
{
    /* 210 days before epoch: day_in_cycle = 0 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD - 210.0);
    TEST_ASSERT_EQUAL_INT(0, p.day_in_cycle);
}

/* ===== pawukon_from_jd: modern dates ===== */

void test_from_jd_2024_jan_1(void)
{
    /* JD 2460310.5 = 1 Jan 2024. Days from epoch = 2460310 - 2379344 = 80966.
     * 80966 % 210 = 80966 - 385*210 = 80966 - 80850 = 116. */
    pawukon_t p = pawukon_from_jd(2460310.5);
    TEST_ASSERT_EQUAL_INT(116, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(16, p.wuku);   /* wuku 116/7 = 16 (Krulut) */
    TEST_ASSERT_EQUAL_INT(4, p.wuku_day); /* 116%7 = 4 */
}

void test_from_jd_2025_jan_1(void)
{
    /* JD 2460676.5 = 1 Jan 2025. Days from epoch = 2460676 - 2379344 = 81332.
     * 81332 % 210 = 81332 - 387*210 = 81332 - 81270 = 62. */
    pawukon_t p = pawukon_from_jd(2460676.5);
    TEST_ASSERT_EQUAL_INT(62, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(8, p.wuku);    /* wuku 62/7 = 8 (Julungwangi) */
    TEST_ASSERT_EQUAL_INT(6, p.wuku_day); /* 62%7 = 6 */
}

void test_from_jd_2026_march_14(void)
{
    /* JD 2461114.5 = 14 Mar 2026. Days from epoch = 2461114 - 2379344 = 81770.
     * 81770 % 210 = 81770 - 389*210 = 81770 - 81690 = 80. */
    pawukon_t p = pawukon_from_jd(2461114.5);
    TEST_ASSERT_EQUAL_INT(80, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(11, p.wuku);   /* wuku 80/7 = 11 (Kuningan) */
    TEST_ASSERT_EQUAL_INT(3, p.wuku_day); /* 80%7 = 3 */
}

/* ===== pawukon_wuku_name ===== */

void test_wuku_name_sinta(void)
{
    TEST_ASSERT_EQUAL_STRING("Sinta", pawukon_wuku_name(0));
}

void test_wuku_name_landep(void)
{
    TEST_ASSERT_EQUAL_STRING("Landep", pawukon_wuku_name(1));
}

void test_wuku_name_kuningan(void)
{
    TEST_ASSERT_EQUAL_STRING("Kuningan", pawukon_wuku_name(11));
}

void test_wuku_name_watugunung(void)
{
    TEST_ASSERT_EQUAL_STRING("Watugunung", pawukon_wuku_name(29));
}

void test_wuku_name_pahang(void)
{
    TEST_ASSERT_EQUAL_STRING("Pahang", pawukon_wuku_name(15));
}

void test_wuku_name_wayang(void)
{
    TEST_ASSERT_EQUAL_STRING("Wayang", pawukon_wuku_name(26));
}

void test_wuku_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", pawukon_wuku_name(-1));
}

void test_wuku_name_invalid_30(void)
{
    TEST_ASSERT_EQUAL_STRING("?", pawukon_wuku_name(30));
}

/* ===== pawukon_day_name ===== */

void test_day_name_eka_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Luang", pawukon_day_name(1, 0));
}

void test_day_name_dwi_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Menga", pawukon_day_name(2, 0));
    TEST_ASSERT_EQUAL_STRING("Pepet", pawukon_day_name(2, 1));
}

void test_day_name_tri_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Pasah", pawukon_day_name(3, 0));
    TEST_ASSERT_EQUAL_STRING("Kajeng", pawukon_day_name(3, 2));
}

void test_day_name_catur_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Sri", pawukon_day_name(4, 0));
    TEST_ASSERT_EQUAL_STRING("Menala", pawukon_day_name(4, 3));
}

void test_day_name_panca_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Umanis", pawukon_day_name(5, 0));
    TEST_ASSERT_EQUAL_STRING("Kliwon", pawukon_day_name(5, 4));
}

void test_day_name_sad_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Tungleh", pawukon_day_name(6, 0));
    TEST_ASSERT_EQUAL_STRING("Maulu", pawukon_day_name(6, 5));
}

void test_day_name_sapta_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Redite", pawukon_day_name(7, 0));
    TEST_ASSERT_EQUAL_STRING("Saniscara", pawukon_day_name(7, 6));
}

void test_day_name_asta_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Sri", pawukon_day_name(8, 0));
    TEST_ASSERT_EQUAL_STRING("Uma", pawukon_day_name(8, 7));
}

void test_day_name_sanga_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Dangu", pawukon_day_name(9, 0));
    TEST_ASSERT_EQUAL_STRING("Dadi", pawukon_day_name(9, 8));
}

void test_day_name_dasa_wara(void)
{
    TEST_ASSERT_EQUAL_STRING("Pandita", pawukon_day_name(10, 0));
    TEST_ASSERT_EQUAL_STRING("Raksasa", pawukon_day_name(10, 9));
}

void test_day_name_invalid_week_length(void)
{
    TEST_ASSERT_EQUAL_STRING("?", pawukon_day_name(0, 0));
    TEST_ASSERT_EQUAL_STRING("?", pawukon_day_name(11, 0));
}

void test_day_name_invalid_day_index(void)
{
    TEST_ASSERT_EQUAL_STRING("?", pawukon_day_name(5, 5));
    TEST_ASSERT_EQUAL_STRING("?", pawukon_day_name(7, 7));
    TEST_ASSERT_EQUAL_STRING("?", pawukon_day_name(3, -1));
}

/* ===== pawukon_urip ===== */

void test_urip_umanis_redite(void)
{
    /* Umanis(0)=5, Redite(0)=5, total=10 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD); /* day 0: panca=0, sapta=0 */
    TEST_ASSERT_EQUAL_INT(10, pawukon_urip(p));
}

void test_urip_pahing_soma(void)
{
    /* day 1: panca=1 (Pahing=9), sapta=1 (Soma=4), total=13 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 1.0);
    TEST_ASSERT_EQUAL_INT(13, pawukon_urip(p));
}

void test_urip_pon_anggara(void)
{
    /* day 2: panca=2 (Pon=7), sapta=2 (Anggara=3), total=10 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 2.0);
    TEST_ASSERT_EQUAL_INT(10, pawukon_urip(p));
}

void test_urip_wage_buda(void)
{
    /* day 3: panca=3 (Wage=4), sapta=3 (Buda=7), total=11 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 3.0);
    TEST_ASSERT_EQUAL_INT(11, pawukon_urip(p));
}

void test_urip_kliwon_wraspati(void)
{
    /* day 4: panca=4 (Kliwon=8), sapta=4 (Wraspati=8), total=16 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 4.0);
    TEST_ASSERT_EQUAL_INT(16, pawukon_urip(p));
}

void test_urip_kliwon_saniscara(void)
{
    /* Kliwon(4)=8 + Saniscara(6)=9 = 17 -- this is a Tumpek day.
     * We need day d where d%5==4 and d%7==6.
     * d=34 works: 34%5=4, 34%7=6. */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 34.0);
    TEST_ASSERT_EQUAL_INT(17, pawukon_urip(p));
}

/* ===== pawukon_is_tumpek ===== */

void test_is_tumpek_day34(void)
{
    /* Day 34: panca=4 (Kliwon), sapta=6 (Saniscara) -> Tumpek */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 34.0);
    TEST_ASSERT_TRUE(pawukon_is_tumpek(p));
}

void test_is_tumpek_day69(void)
{
    /* Day 69: 69%5=4, 69%7=6 -> Tumpek */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 69.0);
    TEST_ASSERT_TRUE(pawukon_is_tumpek(p));
}

void test_is_not_tumpek_day0(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_FALSE(pawukon_is_tumpek(p));
}

void test_is_not_tumpek_day6(void)
{
    /* Day 6: sapta=6 but panca=1, not Kliwon */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 6.0);
    TEST_ASSERT_FALSE(pawukon_is_tumpek(p));
}

void test_tumpek_count_per_cycle(void)
{
    /* There should be exactly 6 Tumpek days per 210-day cycle */
    int count = 0;
    for (int d = 0; d < 210; d++) {
        pawukon_t p = pawukon_from_jd(EPOCH_JD + (double)d);
        if (pawukon_is_tumpek(p)) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(6, count);
}

/* ===== pawukon_is_kajeng_kliwon ===== */

void test_is_kajeng_kliwon_day14(void)
{
    /* Day 14: 14%3=2 (Kajeng), 14%5=4 (Kliwon) -> Kajeng Kliwon */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 14.0);
    TEST_ASSERT_TRUE(pawukon_is_kajeng_kliwon(p));
}

void test_is_not_kajeng_kliwon_day0(void)
{
    pawukon_t p = pawukon_from_jd(EPOCH_JD);
    TEST_ASSERT_FALSE(pawukon_is_kajeng_kliwon(p));
}

void test_is_not_kajeng_kliwon_day4(void)
{
    /* Day 4: panca=4 (Kliwon) but tri=1 (Beteng), not Kajeng */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 4.0);
    TEST_ASSERT_FALSE(pawukon_is_kajeng_kliwon(p));
}

void test_kajeng_kliwon_count_per_cycle(void)
{
    /* There should be exactly 14 Kajeng Kliwon days per 210-day cycle */
    int count = 0;
    for (int d = 0; d < 210; d++) {
        pawukon_t p = pawukon_from_jd(EPOCH_JD + (double)d);
        if (pawukon_is_kajeng_kliwon(p)) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(14, count);
}

/* ===== pawukon_ceremony ===== */

void test_ceremony_galungan(void)
{
    /* Galungan: wuku Dungulan (10), sapta_wara Buda (3)
     * Day in cycle = wuku*7 + sapta_wara = 10*7 + 3 = 73 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 73.0);
    TEST_ASSERT_EQUAL_INT(10, p.wuku);
    TEST_ASSERT_EQUAL_INT(3, p.sapta_wara); /* 73%7=3 */
    const char *name = pawukon_ceremony(p);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Galungan", name);
}

void test_ceremony_kuningan(void)
{
    /* Kuningan: wuku Kuningan (11), sapta_wara Saniscara (6)
     * Day in cycle = 11*7 + 6 = 83 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 83.0);
    TEST_ASSERT_EQUAL_INT(11, p.wuku);
    TEST_ASSERT_EQUAL_INT(6, p.sapta_wara); /* 83%7=6 */
    const char *name = pawukon_ceremony(p);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Kuningan", name);
}

void test_ceremony_saraswati(void)
{
    /* Saraswati: wuku Watugunung (29), sapta_wara Saniscara (6)
     * Day in cycle = 29*7 + 6 = 209 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 209.0);
    TEST_ASSERT_EQUAL_INT(29, p.wuku);
    TEST_ASSERT_EQUAL_INT(6, p.sapta_wara); /* 209%7=6 */
    const char *name = pawukon_ceremony(p);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Saraswati", name);
}

void test_ceremony_pagerwesi(void)
{
    /* Pagerwesi: wuku Sinta (0), sapta_wara Buda (3)
     * Day in cycle = 0*7 + 3 = 3 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 3.0);
    TEST_ASSERT_EQUAL_INT(0, p.wuku);
    TEST_ASSERT_EQUAL_INT(3, p.sapta_wara);
    const char *name = pawukon_ceremony(p);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Pagerwesi", name);
}

void test_ceremony_tumpek_landep(void)
{
    /* Tumpek Landep: wuku Landep (1), sapta_wara Saniscara (6)
     * Day in cycle = 1*7 + 6 = 13 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 13.0);
    TEST_ASSERT_EQUAL_INT(1, p.wuku);
    TEST_ASSERT_EQUAL_INT(6, p.sapta_wara);
    const char *name = pawukon_ceremony(p);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Tumpek Landep", name);
}

void test_ceremony_tumpek_wayang(void)
{
    /* Tumpek Wayang: wuku Wayang (26), sapta_wara Saniscara (6)
     * Day in cycle = 26*7 + 6 = 188 */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 188.0);
    TEST_ASSERT_EQUAL_INT(26, p.wuku);
    TEST_ASSERT_EQUAL_INT(6, p.sapta_wara);
    const char *name = pawukon_ceremony(p);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Tumpek Wayang", name);
}

void test_ceremony_none(void)
{
    /* Day 1: wuku Sinta, Soma -- no ceremony */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 1.0);
    TEST_ASSERT_NULL(pawukon_ceremony(p));
}

/* ===== pawukon_ceremony_count and ceremony_by_index ===== */

void test_ceremony_count(void)
{
    TEST_ASSERT_TRUE(pawukon_ceremony_count() >= 7);
}

void test_ceremony_by_index_first(void)
{
    pawukon_ceremony_t c = pawukon_ceremony_by_index(0);
    TEST_ASSERT_NOT_NULL(c.name);
}

void test_ceremony_by_index_invalid(void)
{
    pawukon_ceremony_t c = pawukon_ceremony_by_index(-1);
    TEST_ASSERT_NULL(c.name);
}

void test_ceremony_by_index_out_of_range(void)
{
    pawukon_ceremony_t c = pawukon_ceremony_by_index(99);
    TEST_ASSERT_NULL(c.name);
}

void test_ceremony_by_index_has_description(void)
{
    /* Check that each ceremony has a non-empty description */
    int n = pawukon_ceremony_count();
    for (int i = 0; i < n; i++) {
        pawukon_ceremony_t c = pawukon_ceremony_by_index(i);
        TEST_ASSERT_NOT_NULL(c.name);
        TEST_ASSERT_NOT_NULL(c.description);
        TEST_ASSERT_TRUE(strlen(c.description) > 0);
    }
}

/* ===== pawukon_day_of_cycle ===== */

void test_day_of_cycle_epoch(void)
{
    TEST_ASSERT_EQUAL_INT(0, pawukon_day_of_cycle(EPOCH_JD));
}

void test_day_of_cycle_day100(void)
{
    TEST_ASSERT_EQUAL_INT(100, pawukon_day_of_cycle(EPOCH_JD + 100.0));
}

void test_day_of_cycle_wraps(void)
{
    TEST_ASSERT_EQUAL_INT(0, pawukon_day_of_cycle(EPOCH_JD + 210.0));
    TEST_ASSERT_EQUAL_INT(5, pawukon_day_of_cycle(EPOCH_JD + 215.0));
}

void test_day_of_cycle_before_epoch(void)
{
    TEST_ASSERT_EQUAL_INT(209, pawukon_day_of_cycle(EPOCH_JD - 1.0));
    TEST_ASSERT_EQUAL_INT(200, pawukon_day_of_cycle(EPOCH_JD - 10.0));
}

/* ===== pawukon_days_until_wuku ===== */

void test_days_until_wuku_same(void)
{
    /* At epoch (wuku 0), days until wuku 0 should be 0 (we're at start of it) */
    /* Actually at day 0 of wuku 0, the wuku just started. days_until_wuku
     * should return 0 if we're already in the target wuku's first day,
     * but logically "days until start of wuku X" from here.
     * If we're at wuku 0 day 0, days until wuku 0 = 0. */
    int d = pawukon_days_until_wuku(EPOCH_JD, 0);
    TEST_ASSERT_EQUAL_INT(0, d);
}

void test_days_until_wuku_next(void)
{
    /* At epoch (wuku 0, day 0), days until wuku 1 (Landep) = 7 */
    int d = pawukon_days_until_wuku(EPOCH_JD, 1);
    TEST_ASSERT_EQUAL_INT(7, d);
}

void test_days_until_wuku_kuningan_from_epoch(void)
{
    /* At epoch, days until wuku 11 (Kuningan) = 11*7 = 77 */
    int d = pawukon_days_until_wuku(EPOCH_JD, 11);
    TEST_ASSERT_EQUAL_INT(77, d);
}

void test_days_until_wuku_wrap(void)
{
    /* At day 100 (wuku 14, day 2), days until wuku 0 (Sinta).
     * We are at day 100 in cycle. Wuku 0 starts at day 0.
     * Days until = 210 - 100 = 110. */
    int d = pawukon_days_until_wuku(EPOCH_JD + 100.0, 0);
    TEST_ASSERT_EQUAL_INT(110, d);
}

void test_days_until_wuku_midwuku(void)
{
    /* At day 3 (wuku 0, wuku_day 3), days until wuku 2 (Ukir).
     * Wuku 2 starts at day 14. Current day = 3.
     * Days until = 14 - 3 = 11. */
    int d = pawukon_days_until_wuku(EPOCH_JD + 3.0, 2);
    TEST_ASSERT_EQUAL_INT(11, d);
}

void test_days_until_wuku_invalid(void)
{
    /* Invalid wuku should return -1 */
    TEST_ASSERT_EQUAL_INT(-1, pawukon_days_until_wuku(EPOCH_JD, -1));
    TEST_ASSERT_EQUAL_INT(-1, pawukon_days_until_wuku(EPOCH_JD, 30));
}

/* ===== Additional concurrent-week spot checks ===== */

void test_concurrent_weeks_day42(void)
{
    /* Day 42 = 6*7, start of wuku Warigadean (7) */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 42.0);
    TEST_ASSERT_EQUAL_INT(42, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(6, p.wuku);    /* Gumbreg? No: wuku 0-5 are first 6, so 42/7=6 = Wariga */
    TEST_ASSERT_EQUAL_INT(0, p.wuku_day);
    TEST_ASSERT_EQUAL_INT(0, p.dwi_wara);    /* 42%2=0 */
    TEST_ASSERT_EQUAL_INT(0, p.tri_wara);    /* 42%3=0 */
    TEST_ASSERT_EQUAL_INT(2, p.catur_wara);  /* 42%4=2 */
    TEST_ASSERT_EQUAL_INT(2, p.panca_wara);  /* 42%5=2 */
    TEST_ASSERT_EQUAL_INT(0, p.sad_wara);    /* 42%6=0 */
    TEST_ASSERT_EQUAL_INT(0, p.sapta_wara);  /* 42%7=0 */
    TEST_ASSERT_EQUAL_INT(2, p.asta_wara);   /* 42%8=2 */
    TEST_ASSERT_EQUAL_INT(6, p.sanga_wara);  /* 42%9=6 */
    TEST_ASSERT_EQUAL_INT(2, p.dasa_wara);   /* 42%10=2 */
}

void test_concurrent_weeks_day119(void)
{
    /* Day 119: a day with diverse modular residues */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 119.0);
    TEST_ASSERT_EQUAL_INT(119, p.day_in_cycle);
    TEST_ASSERT_EQUAL_INT(17, p.wuku);       /* 119/7=17 (Tambir) */
    TEST_ASSERT_EQUAL_INT(0, p.wuku_day);    /* 119%7=0 */
    TEST_ASSERT_EQUAL_INT(1, p.dwi_wara);    /* 119%2=1 */
    TEST_ASSERT_EQUAL_INT(2, p.tri_wara);    /* 119%3=2 */
    TEST_ASSERT_EQUAL_INT(3, p.catur_wara);  /* 119%4=3 */
    TEST_ASSERT_EQUAL_INT(4, p.panca_wara);  /* 119%5=4 */
    TEST_ASSERT_EQUAL_INT(5, p.sad_wara);    /* 119%6=5 */
    TEST_ASSERT_EQUAL_INT(0, p.sapta_wara);  /* 119%7=0 */
    TEST_ASSERT_EQUAL_INT(7, p.asta_wara);   /* 119%8=7 */
    TEST_ASSERT_EQUAL_INT(2, p.sanga_wara);  /* 119%9=2 */
    TEST_ASSERT_EQUAL_INT(9, p.dasa_wara);   /* 119%10=9 */
}

/* ===== Tumpek positions within cycle ===== */

void test_tumpek_positions(void)
{
    /* All 6 Tumpek days occur where d%5==4 AND d%7==6.
     * These are at day_in_cycle: 34, 69, 104, 139, 174, 209.
     * (LCM(5,7)=35, first at 34, then every 35 days). */
    int expected[] = {34, 69, 104, 139, 174, 209};
    int found = 0;
    for (int d = 0; d < 210; d++) {
        pawukon_t p = pawukon_from_jd(EPOCH_JD + (double)d);
        if (pawukon_is_tumpek(p)) {
            TEST_ASSERT_TRUE(found < 6);
            TEST_ASSERT_EQUAL_INT(expected[found], d);
            found++;
        }
    }
    TEST_ASSERT_EQUAL_INT(6, found);
}

/* ===== Wuku name completeness ===== */

void test_wuku_names_all_30(void)
{
    /* Spot-check a selection across the full range */
    TEST_ASSERT_EQUAL_STRING("Sinta", pawukon_wuku_name(0));
    TEST_ASSERT_EQUAL_STRING("Ukir", pawukon_wuku_name(2));
    TEST_ASSERT_EQUAL_STRING("Kulantir", pawukon_wuku_name(3));
    TEST_ASSERT_EQUAL_STRING("Tolu", pawukon_wuku_name(4));
    TEST_ASSERT_EQUAL_STRING("Gumbreg", pawukon_wuku_name(5));
    TEST_ASSERT_EQUAL_STRING("Wariga", pawukon_wuku_name(6));
    TEST_ASSERT_EQUAL_STRING("Warigadean", pawukon_wuku_name(7));
    TEST_ASSERT_EQUAL_STRING("Julungwangi", pawukon_wuku_name(8));
    TEST_ASSERT_EQUAL_STRING("Sungsang", pawukon_wuku_name(9));
    TEST_ASSERT_EQUAL_STRING("Dungulan", pawukon_wuku_name(10));
    TEST_ASSERT_EQUAL_STRING("Langkir", pawukon_wuku_name(12));
    TEST_ASSERT_EQUAL_STRING("Medangsia", pawukon_wuku_name(13));
    TEST_ASSERT_EQUAL_STRING("Pujut", pawukon_wuku_name(14));
    TEST_ASSERT_EQUAL_STRING("Krulut", pawukon_wuku_name(16));
    TEST_ASSERT_EQUAL_STRING("Merakih", pawukon_wuku_name(17));
    TEST_ASSERT_EQUAL_STRING("Tambir", pawukon_wuku_name(18));
    TEST_ASSERT_EQUAL_STRING("Medangkungan", pawukon_wuku_name(19));
    TEST_ASSERT_EQUAL_STRING("Matal", pawukon_wuku_name(20));
    TEST_ASSERT_EQUAL_STRING("Uye", pawukon_wuku_name(21));
    TEST_ASSERT_EQUAL_STRING("Menail", pawukon_wuku_name(22));
    TEST_ASSERT_EQUAL_STRING("Prangbakat", pawukon_wuku_name(23));
    TEST_ASSERT_EQUAL_STRING("Bala", pawukon_wuku_name(24));
    TEST_ASSERT_EQUAL_STRING("Ugu", pawukon_wuku_name(25));
    TEST_ASSERT_EQUAL_STRING("Kelawu", pawukon_wuku_name(27));
    TEST_ASSERT_EQUAL_STRING("Dukut", pawukon_wuku_name(28));
}

/* ===== Edge case: large negative JD offset ===== */

void test_from_jd_large_negative_offset(void)
{
    /* 10000 days before epoch. 10000%210 = 10000 - 47*210 = 10000-9870 = 130.
     * So day_in_cycle = (210 - 130) % 210 = 80. */
    pawukon_t p = pawukon_from_jd(EPOCH_JD - 10000.0);
    TEST_ASSERT_EQUAL_INT(80, p.day_in_cycle);
}

void test_from_jd_large_positive_offset(void)
{
    /* 100000 days from epoch. 100000%210 = 100000 - 476*210 = 100000-99960 = 40. */
    pawukon_t p = pawukon_from_jd(EPOCH_JD + 100000.0);
    TEST_ASSERT_EQUAL_INT(40, p.day_in_cycle);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* epoch tests */
    RUN_TEST(test_from_jd_epoch_day_in_cycle);
    RUN_TEST(test_from_jd_epoch_wuku);
    RUN_TEST(test_from_jd_epoch_wuku_day);
    RUN_TEST(test_from_jd_epoch_eka_wara);
    RUN_TEST(test_from_jd_epoch_dwi_wara);
    RUN_TEST(test_from_jd_epoch_tri_wara);
    RUN_TEST(test_from_jd_epoch_catur_wara);
    RUN_TEST(test_from_jd_epoch_panca_wara);
    RUN_TEST(test_from_jd_epoch_sad_wara);
    RUN_TEST(test_from_jd_epoch_sapta_wara);
    RUN_TEST(test_from_jd_epoch_asta_wara);
    RUN_TEST(test_from_jd_epoch_sanga_wara);
    RUN_TEST(test_from_jd_epoch_dasa_wara);

    /* day 1 */
    RUN_TEST(test_from_jd_day1_in_cycle);
    RUN_TEST(test_from_jd_day1_concurrent_weeks);

    /* day 7 */
    RUN_TEST(test_from_jd_day7_wuku);
    RUN_TEST(test_from_jd_day7_concurrent_weeks);

    /* day 209 */
    RUN_TEST(test_from_jd_day209);

    /* cycle wrap */
    RUN_TEST(test_from_jd_day210_cycle_wrap);
    RUN_TEST(test_from_jd_two_cycles);
    RUN_TEST(test_from_jd_three_cycles_plus_35);

    /* fractional JD */
    RUN_TEST(test_from_jd_fractional_noon);
    RUN_TEST(test_from_jd_fractional_just_before_midnight);

    /* before epoch */
    RUN_TEST(test_from_jd_before_epoch);
    RUN_TEST(test_from_jd_before_epoch_half_cycle);
    RUN_TEST(test_from_jd_before_epoch_full_cycle);

    /* modern dates */
    RUN_TEST(test_from_jd_2024_jan_1);
    RUN_TEST(test_from_jd_2025_jan_1);
    RUN_TEST(test_from_jd_2026_march_14);

    /* wuku names */
    RUN_TEST(test_wuku_name_sinta);
    RUN_TEST(test_wuku_name_landep);
    RUN_TEST(test_wuku_name_kuningan);
    RUN_TEST(test_wuku_name_watugunung);
    RUN_TEST(test_wuku_name_pahang);
    RUN_TEST(test_wuku_name_wayang);
    RUN_TEST(test_wuku_name_invalid_negative);
    RUN_TEST(test_wuku_name_invalid_30);
    RUN_TEST(test_wuku_names_all_30);

    /* day names */
    RUN_TEST(test_day_name_eka_wara);
    RUN_TEST(test_day_name_dwi_wara);
    RUN_TEST(test_day_name_tri_wara);
    RUN_TEST(test_day_name_catur_wara);
    RUN_TEST(test_day_name_panca_wara);
    RUN_TEST(test_day_name_sad_wara);
    RUN_TEST(test_day_name_sapta_wara);
    RUN_TEST(test_day_name_asta_wara);
    RUN_TEST(test_day_name_sanga_wara);
    RUN_TEST(test_day_name_dasa_wara);
    RUN_TEST(test_day_name_invalid_week_length);
    RUN_TEST(test_day_name_invalid_day_index);

    /* urip */
    RUN_TEST(test_urip_umanis_redite);
    RUN_TEST(test_urip_pahing_soma);
    RUN_TEST(test_urip_pon_anggara);
    RUN_TEST(test_urip_wage_buda);
    RUN_TEST(test_urip_kliwon_wraspati);
    RUN_TEST(test_urip_kliwon_saniscara);

    /* tumpek */
    RUN_TEST(test_is_tumpek_day34);
    RUN_TEST(test_is_tumpek_day69);
    RUN_TEST(test_is_not_tumpek_day0);
    RUN_TEST(test_is_not_tumpek_day6);
    RUN_TEST(test_tumpek_count_per_cycle);
    RUN_TEST(test_tumpek_positions);

    /* kajeng kliwon */
    RUN_TEST(test_is_kajeng_kliwon_day14);
    RUN_TEST(test_is_not_kajeng_kliwon_day0);
    RUN_TEST(test_is_not_kajeng_kliwon_day4);
    RUN_TEST(test_kajeng_kliwon_count_per_cycle);

    /* ceremonies */
    RUN_TEST(test_ceremony_galungan);
    RUN_TEST(test_ceremony_kuningan);
    RUN_TEST(test_ceremony_saraswati);
    RUN_TEST(test_ceremony_pagerwesi);
    RUN_TEST(test_ceremony_tumpek_landep);
    RUN_TEST(test_ceremony_tumpek_wayang);
    RUN_TEST(test_ceremony_none);
    RUN_TEST(test_ceremony_count);
    RUN_TEST(test_ceremony_by_index_first);
    RUN_TEST(test_ceremony_by_index_invalid);
    RUN_TEST(test_ceremony_by_index_out_of_range);
    RUN_TEST(test_ceremony_by_index_has_description);

    /* day_of_cycle */
    RUN_TEST(test_day_of_cycle_epoch);
    RUN_TEST(test_day_of_cycle_day100);
    RUN_TEST(test_day_of_cycle_wraps);
    RUN_TEST(test_day_of_cycle_before_epoch);

    /* days_until_wuku */
    RUN_TEST(test_days_until_wuku_same);
    RUN_TEST(test_days_until_wuku_next);
    RUN_TEST(test_days_until_wuku_kuningan_from_epoch);
    RUN_TEST(test_days_until_wuku_wrap);
    RUN_TEST(test_days_until_wuku_midwuku);
    RUN_TEST(test_days_until_wuku_invalid);

    /* additional spot checks */
    RUN_TEST(test_concurrent_weeks_day42);
    RUN_TEST(test_concurrent_weeks_day119);

    /* edge cases */
    RUN_TEST(test_from_jd_large_negative_offset);
    RUN_TEST(test_from_jd_large_positive_offset);

    return UNITY_END();
}
