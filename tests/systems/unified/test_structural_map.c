/* test_structural_map.c — Structural Pattern Map tests
 * TDD RED phase: tests written before implementation.
 * Source: Golden Dawn correspondences, Sefer Yetzirah, traditional Kabbalistic
 * and cross-system structural parallels. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/structural_map.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== System Identifiers ===== */

void test_system_count(void)
{
    TEST_ASSERT_EQUAL_INT(SYS_COUNT, structural_system_count());
}

void test_system_name_kabbalah(void)
{
    TEST_ASSERT_EQUAL_STRING("Kabbalah", structural_system_name(SYS_KABBALAH));
}

void test_system_name_human_design(void)
{
    TEST_ASSERT_EQUAL_STRING("Human Design", structural_system_name(SYS_HUMAN_DESIGN));
}

void test_system_name_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", structural_system_name(SYS_ICHING));
}

void test_system_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", structural_system_name(SYS_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", structural_system_name((system_id_t)-1));
}

/* ===== Sacred Number Concordances ===== */

void test_concordance_count(void)
{
    int count = structural_concordance_count();
    TEST_ASSERT_EQUAL_INT(12, count);
}

void test_concordance_get_first(void)
{
    concordance_t c = structural_concordance_get(0);
    TEST_ASSERT_EQUAL_INT(3, c.number);
    TEST_ASSERT_NOT_NULL(c.description);
    TEST_ASSERT_TRUE(c.system_count >= 3);
}

void test_concordance_get_seven(void)
{
    /* 7 = planets, double letters, weekdays, chakras */
    concordance_t c = structural_concordance_for_number(7);
    TEST_ASSERT_EQUAL_INT(7, c.number);
    TEST_ASSERT_TRUE(c.system_count >= 4);
}

void test_concordance_get_twelve(void)
{
    concordance_t c = structural_concordance_for_number(12);
    TEST_ASSERT_EQUAL_INT(12, c.number);
    TEST_ASSERT_TRUE(c.system_count >= 4);
}

void test_concordance_get_sixtyfour(void)
{
    concordance_t c = structural_concordance_for_number(64);
    TEST_ASSERT_EQUAL_INT(64, c.number);
    TEST_ASSERT_TRUE(c.system_count >= 3);
}

void test_concordance_get_twentytwo(void)
{
    concordance_t c = structural_concordance_for_number(22);
    TEST_ASSERT_EQUAL_INT(22, c.number);
    TEST_ASSERT_TRUE(c.system_count >= 3);
}

void test_concordance_for_number_not_found(void)
{
    concordance_t c = structural_concordance_for_number(999);
    TEST_ASSERT_EQUAL_INT(-1, c.number);
}

void test_concordance_get_invalid_index(void)
{
    concordance_t c = structural_concordance_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, c.number);

    concordance_t c2 = structural_concordance_get(100);
    TEST_ASSERT_EQUAL_INT(-1, c2.number);
}

/* ===== Sacred Numbers ===== */

void test_sacred_number_count(void)
{
    TEST_ASSERT_EQUAL_INT(12, structural_sacred_number_count());
}

void test_sacred_number_first(void)
{
    TEST_ASSERT_EQUAL_INT(3, structural_sacred_number(0));
}

void test_sacred_number_last(void)
{
    TEST_ASSERT_EQUAL_INT(260, structural_sacred_number(11));
}

void test_sacred_number_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, structural_sacred_number(-1));
    TEST_ASSERT_EQUAL_INT(-1, structural_sacred_number(100));
}

/* ===== structural_appears_in_systems ===== */

void test_appears_in_systems_seven(void)
{
    /* 7 appears in >= 4 systems */
    TEST_ASSERT_TRUE(structural_appears_in_systems(7, 4));
}

void test_appears_in_systems_seven_too_many(void)
{
    /* 7 shouldn't appear in 10 systems */
    TEST_ASSERT_FALSE(structural_appears_in_systems(7, 10));
}

void test_appears_in_systems_not_in_table(void)
{
    TEST_ASSERT_FALSE(structural_appears_in_systems(999, 1));
}

/* ===== Planet Rows (7 Classical Planets) ===== */

void test_planet_row_sun(void)
{
    planet_row_t p = structural_planet_row(0);
    TEST_ASSERT_EQUAL_STRING("Sun", p.planet);
    TEST_ASSERT_EQUAL_STRING("Resh", p.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Sunday", p.weekday);
    TEST_ASSERT_EQUAL_STRING("Solar Plexus", p.chakra);
    TEST_ASSERT_EQUAL_INT(5, p.sefirah_index); /* Tiferet */
}

void test_planet_row_moon(void)
{
    planet_row_t p = structural_planet_row(1);
    TEST_ASSERT_EQUAL_STRING("Moon", p.planet);
    TEST_ASSERT_EQUAL_STRING("Gimel", p.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Monday", p.weekday);
    TEST_ASSERT_EQUAL_STRING("Third Eye", p.chakra);
    TEST_ASSERT_EQUAL_INT(8, p.sefirah_index); /* Yesod */
}

void test_planet_row_mars(void)
{
    planet_row_t p = structural_planet_row(2);
    TEST_ASSERT_EQUAL_STRING("Mars", p.planet);
    TEST_ASSERT_EQUAL_STRING("Pe", p.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Tuesday", p.weekday);
    TEST_ASSERT_EQUAL_STRING("Root", p.chakra);
    TEST_ASSERT_EQUAL_INT(4, p.sefirah_index); /* Gevurah */
}

void test_planet_row_mercury(void)
{
    planet_row_t p = structural_planet_row(3);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.planet);
    TEST_ASSERT_EQUAL_STRING("Bet", p.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Wednesday", p.weekday);
    TEST_ASSERT_EQUAL_STRING("Throat", p.chakra);
    TEST_ASSERT_EQUAL_INT(7, p.sefirah_index); /* Hod */
}

void test_planet_row_saturn(void)
{
    planet_row_t p = structural_planet_row(6);
    TEST_ASSERT_EQUAL_STRING("Saturn", p.planet);
    TEST_ASSERT_EQUAL_STRING("Tav", p.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Saturday", p.weekday);
    TEST_ASSERT_EQUAL_STRING("Sacral", p.chakra);
    TEST_ASSERT_EQUAL_INT(2, p.sefirah_index); /* Binah */
}

void test_planet_row_invalid(void)
{
    planet_row_t p = structural_planet_row(-1);
    TEST_ASSERT_EQUAL_STRING("?", p.planet);
    TEST_ASSERT_EQUAL_INT(-1, p.sefirah_index);

    planet_row_t p2 = structural_planet_row(7);
    TEST_ASSERT_EQUAL_STRING("?", p2.planet);
}

/* ===== Zodiac Rows (12 Signs) ===== */

void test_zodiac_row_aries(void)
{
    zodiac_row_t z = structural_zodiac_row(0);
    TEST_ASSERT_EQUAL_STRING("Aries", z.zodiac);
    TEST_ASSERT_EQUAL_STRING("He", z.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Rat (Zi)", z.chinese_branch);
    TEST_ASSERT_EQUAL_STRING("Emperor", z.tarot);
}

void test_zodiac_row_taurus(void)
{
    zodiac_row_t z = structural_zodiac_row(1);
    TEST_ASSERT_EQUAL_STRING("Taurus", z.zodiac);
    TEST_ASSERT_EQUAL_STRING("Vav", z.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Ox (Chou)", z.chinese_branch);
    TEST_ASSERT_EQUAL_STRING("Hierophant", z.tarot);
}

void test_zodiac_row_pisces(void)
{
    zodiac_row_t z = structural_zodiac_row(11);
    TEST_ASSERT_EQUAL_STRING("Pisces", z.zodiac);
    TEST_ASSERT_EQUAL_STRING("Qof", z.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Pig (Hai)", z.chinese_branch);
    TEST_ASSERT_EQUAL_STRING("Moon", z.tarot);
}

void test_zodiac_row_leo(void)
{
    zodiac_row_t z = structural_zodiac_row(4);
    TEST_ASSERT_EQUAL_STRING("Leo", z.zodiac);
    TEST_ASSERT_EQUAL_STRING("Tet", z.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Dragon (Chen)", z.chinese_branch);
    TEST_ASSERT_EQUAL_STRING("Strength", z.tarot);
}

void test_zodiac_row_invalid(void)
{
    zodiac_row_t z = structural_zodiac_row(-1);
    TEST_ASSERT_EQUAL_STRING("?", z.zodiac);
    TEST_ASSERT_EQUAL_INT(-1, z.sign_index);

    zodiac_row_t z2 = structural_zodiac_row(12);
    TEST_ASSERT_EQUAL_STRING("?", z2.zodiac);
}

/* ===== Sefirot <-> HD Center Mapping ===== */

void test_sefirah_to_hd_keter(void)
{
    TEST_ASSERT_EQUAL_STRING("Head", structural_sefirah_to_hd_center(0));
}

void test_sefirah_to_hd_chokmah(void)
{
    TEST_ASSERT_EQUAL_STRING("Ajna", structural_sefirah_to_hd_center(1));
}

void test_sefirah_to_hd_binah(void)
{
    /* Binah also maps to Ajna */
    TEST_ASSERT_EQUAL_STRING("Ajna", structural_sefirah_to_hd_center(2));
}

void test_sefirah_to_hd_chesed(void)
{
    TEST_ASSERT_EQUAL_STRING("G Center", structural_sefirah_to_hd_center(3));
}

void test_sefirah_to_hd_tiferet(void)
{
    /* Tiferet also maps to G Center */
    TEST_ASSERT_EQUAL_STRING("G Center", structural_sefirah_to_hd_center(5));
}

void test_sefirah_to_hd_malkuth(void)
{
    TEST_ASSERT_EQUAL_STRING("Root", structural_sefirah_to_hd_center(9));
}

void test_sefirah_to_hd_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", structural_sefirah_to_hd_center(-1));
    TEST_ASSERT_EQUAL_STRING("?", structural_sefirah_to_hd_center(10));
}

void test_hd_center_to_sefirah_head(void)
{
    /* Head -> Keter (0) */
    TEST_ASSERT_EQUAL_INT(0, structural_hd_center_to_sefirah(0));
}

void test_hd_center_to_sefirah_root(void)
{
    /* Root -> Malkuth (9) */
    TEST_ASSERT_EQUAL_INT(9, structural_hd_center_to_sefirah(8));
}

void test_hd_center_to_sefirah_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, structural_hd_center_to_sefirah(-1));
    TEST_ASSERT_EQUAL_INT(-1, structural_hd_center_to_sefirah(9));
}

/* ===== Concordance 260 (Tzolkin) ===== */

void test_concordance_260(void)
{
    concordance_t c = structural_concordance_for_number(260);
    TEST_ASSERT_EQUAL_INT(260, c.number);
    TEST_ASSERT_TRUE(c.system_count >= 2);
}

/* ===== Concordance 108 ===== */

void test_concordance_108(void)
{
    concordance_t c = structural_concordance_for_number(108);
    TEST_ASSERT_EQUAL_INT(108, c.number);
    TEST_ASSERT_TRUE(c.system_count >= 3);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* System identifiers */
    RUN_TEST(test_system_count);
    RUN_TEST(test_system_name_kabbalah);
    RUN_TEST(test_system_name_human_design);
    RUN_TEST(test_system_name_iching);
    RUN_TEST(test_system_name_invalid);

    /* Concordances */
    RUN_TEST(test_concordance_count);
    RUN_TEST(test_concordance_get_first);
    RUN_TEST(test_concordance_get_seven);
    RUN_TEST(test_concordance_get_twelve);
    RUN_TEST(test_concordance_get_sixtyfour);
    RUN_TEST(test_concordance_get_twentytwo);
    RUN_TEST(test_concordance_for_number_not_found);
    RUN_TEST(test_concordance_get_invalid_index);

    /* Sacred numbers */
    RUN_TEST(test_sacred_number_count);
    RUN_TEST(test_sacred_number_first);
    RUN_TEST(test_sacred_number_last);
    RUN_TEST(test_sacred_number_invalid);

    /* Appears in systems */
    RUN_TEST(test_appears_in_systems_seven);
    RUN_TEST(test_appears_in_systems_seven_too_many);
    RUN_TEST(test_appears_in_systems_not_in_table);

    /* Planet rows */
    RUN_TEST(test_planet_row_sun);
    RUN_TEST(test_planet_row_moon);
    RUN_TEST(test_planet_row_mars);
    RUN_TEST(test_planet_row_mercury);
    RUN_TEST(test_planet_row_saturn);
    RUN_TEST(test_planet_row_invalid);

    /* Zodiac rows */
    RUN_TEST(test_zodiac_row_aries);
    RUN_TEST(test_zodiac_row_taurus);
    RUN_TEST(test_zodiac_row_pisces);
    RUN_TEST(test_zodiac_row_leo);
    RUN_TEST(test_zodiac_row_invalid);

    /* Sefirot <-> HD Center */
    RUN_TEST(test_sefirah_to_hd_keter);
    RUN_TEST(test_sefirah_to_hd_chokmah);
    RUN_TEST(test_sefirah_to_hd_binah);
    RUN_TEST(test_sefirah_to_hd_chesed);
    RUN_TEST(test_sefirah_to_hd_tiferet);
    RUN_TEST(test_sefirah_to_hd_malkuth);
    RUN_TEST(test_sefirah_to_hd_invalid);
    RUN_TEST(test_hd_center_to_sefirah_head);
    RUN_TEST(test_hd_center_to_sefirah_root);
    RUN_TEST(test_hd_center_to_sefirah_invalid);

    /* Additional concordances */
    RUN_TEST(test_concordance_260);
    RUN_TEST(test_concordance_108);

    return UNITY_END();
}
