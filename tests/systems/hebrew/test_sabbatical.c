/* test_sabbatical.c — Sabbatical (Shemitah), Jubilee cycles & Hebrew holidays
 * TDD RED phase: tests written before implementation.
 * Known Shemitah years: 5782, 5789, 5796 (year % 7 == 0).
 * Jubilee: 50-year cycle (year % 50 == 0). */

#include "../../unity/unity.h"
#include "../../../src/systems/hebrew/sabbatical.h"
#include "../../../src/systems/hebrew/hebrew.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Sabbatical (Shemitah) cycle tests ===== */

void test_sabbatical_info_5782_is_shemitah(void)
{
    /* 5782 is a known Shemitah year => year_in_cycle=7, is_shemitah=true */
    sabbatical_info_t info = sabbatical_info(5782);
    TEST_ASSERT_EQUAL_INT(7, info.year_in_cycle);
    TEST_ASSERT_TRUE(info.is_shemitah);
}

void test_sabbatical_info_5783_first_of_cycle(void)
{
    /* 5783 follows a Shemitah => year_in_cycle=1, not Shemitah */
    sabbatical_info_t info = sabbatical_info(5783);
    TEST_ASSERT_EQUAL_INT(1, info.year_in_cycle);
    TEST_ASSERT_FALSE(info.is_shemitah);
}

void test_sabbatical_info_5785_mid_cycle(void)
{
    /* 5785 => year_in_cycle = ((5785-1)%7)+1 = 3 */
    sabbatical_info_t info = sabbatical_info(5785);
    TEST_ASSERT_EQUAL_INT(3, info.year_in_cycle);
    TEST_ASSERT_FALSE(info.is_shemitah);
}

void test_sabbatical_is_shemitah_5782_true(void)
{
    TEST_ASSERT_TRUE(sabbatical_is_shemitah(5782));
}

void test_sabbatical_is_shemitah_5781_false(void)
{
    TEST_ASSERT_FALSE(sabbatical_is_shemitah(5781));
}

void test_sabbatical_is_shemitah_5789_true(void)
{
    /* 5789 is the next known Shemitah after 5782 */
    TEST_ASSERT_TRUE(sabbatical_is_shemitah(5789));
}

void test_sabbatical_next_from_shemitah_year(void)
{
    /* Already a Shemitah year => returns itself */
    TEST_ASSERT_EQUAL_INT(5782, sabbatical_next(5782));
}

void test_sabbatical_next_from_5783(void)
{
    /* 5783 is year 1 of next cycle => next Shemitah = 5789 */
    TEST_ASSERT_EQUAL_INT(5789, sabbatical_next(5783));
}

void test_sabbatical_next_from_5780(void)
{
    /* 5780 % 7 = 5780 - 826*7 = 5780 - 5782 = -2? No: 5780/7=825.71..
     * 5780 % 7 = 5780 - 825*7 = 5780 - 5775 = 5. Not zero => next = 5782 */
    TEST_ASSERT_EQUAL_INT(5782, sabbatical_next(5780));
}

/* ===== Jubilee cycle tests ===== */

void test_jubilee_info_5750_is_jubilee(void)
{
    /* 5750 % 50 == 0 => year_in_cycle=50, is_jubilee=true */
    jubilee_info_t info = jubilee_info(5750);
    TEST_ASSERT_EQUAL_INT(50, info.year_in_cycle);
    TEST_ASSERT_TRUE(info.is_jubilee);
    TEST_ASSERT_EQUAL_INT(7, info.sabbatical_count);
}

void test_jubilee_info_5751_not_jubilee(void)
{
    /* 5751: position 1, not jubilee, sabbatical_count=0 */
    jubilee_info_t info = jubilee_info(5751);
    TEST_ASSERT_EQUAL_INT(1, info.year_in_cycle);
    TEST_ASSERT_FALSE(info.is_jubilee);
    TEST_ASSERT_EQUAL_INT(0, info.sabbatical_count);
}

void test_jubilee_info_range_check(void)
{
    /* year_in_cycle must be between 1 and 50 for any year */
    int i;
    for (i = 5700; i <= 5800; i++) {
        jubilee_info_t info = jubilee_info(i);
        TEST_ASSERT_TRUE(info.year_in_cycle >= 1);
        TEST_ASSERT_TRUE(info.year_in_cycle <= 50);
    }
}

void test_jubilee_is_jubilee_5800_true(void)
{
    /* 5800 % 50 == 0 */
    TEST_ASSERT_TRUE(jubilee_is_jubilee(5800));
}

void test_jubilee_is_jubilee_5799_false(void)
{
    TEST_ASSERT_FALSE(jubilee_is_jubilee(5799));
}

void test_jubilee_next_from_5750(void)
{
    /* Already jubilee => returns itself */
    TEST_ASSERT_EQUAL_INT(5750, jubilee_next(5750));
}

void test_jubilee_next_from_5751(void)
{
    /* Next jubilee after 5751 is 5800 */
    TEST_ASSERT_EQUAL_INT(5800, jubilee_next(5751));
}

void test_jubilee_next_from_5799(void)
{
    /* One year before => next = 5800 */
    TEST_ASSERT_EQUAL_INT(5800, jubilee_next(5799));
}

void test_jubilee_sabbatical_count_mid(void)
{
    /* year 5765: position = ((5765-1)%50)+1 = (5764%50)+1 = 14+1 = 15 */
    /* sabbatical_count = (15-1)/7 = 14/7 = 2 */
    jubilee_info_t info = jubilee_info(5765);
    TEST_ASSERT_EQUAL_INT(15, info.year_in_cycle);
    TEST_ASSERT_EQUAL_INT(2, info.sabbatical_count);
}

/* ===== Holiday tests ===== */

void test_holiday_count_is_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, hebrew_holiday_count());
}

void test_holiday_get_rosh_hashanah(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 0);
    TEST_ASSERT_EQUAL_STRING("Rosh Hashanah", h.name);
    TEST_ASSERT_EQUAL_INT(7, h.month);
    TEST_ASSERT_EQUAL_INT(1, h.day);
}

void test_holiday_get_yom_kippur(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 1);
    TEST_ASSERT_EQUAL_STRING("Yom Kippur", h.name);
    TEST_ASSERT_EQUAL_INT(7, h.month);
    TEST_ASSERT_EQUAL_INT(10, h.day);
}

void test_holiday_get_passover(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 7);
    TEST_ASSERT_EQUAL_STRING("Passover", h.name);
    TEST_ASSERT_EQUAL_INT(1, h.month);
    TEST_ASSERT_EQUAL_INT(15, h.day);
}

void test_holiday_get_purim_common_year(void)
{
    /* 5786 is common (not leap) => Purim in month 12 (Adar) */
    hebrew_holiday_t h = hebrew_holiday_get(5786, 6);
    TEST_ASSERT_EQUAL_STRING("Purim", h.name);
    TEST_ASSERT_EQUAL_INT(12, h.month);
    TEST_ASSERT_EQUAL_INT(14, h.day);
}

void test_holiday_get_purim_leap_year(void)
{
    /* 5787 is leap => Purim in month 13 (Adar II) */
    hebrew_holiday_t h = hebrew_holiday_get(5787, 6);
    TEST_ASSERT_EQUAL_STRING("Purim", h.name);
    TEST_ASSERT_EQUAL_INT(13, h.month);
    TEST_ASSERT_EQUAL_INT(14, h.day);
}

void test_holiday_get_jd_nonzero(void)
{
    /* Every holiday should have a valid (non-zero) JD */
    int i;
    for (i = 0; i < hebrew_holiday_count(); i++) {
        hebrew_holiday_t h = hebrew_holiday_get(5786, i);
        TEST_ASSERT_TRUE(h.jd > 0.0);
    }
}

void test_holiday_get_invalid_index_negative(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, -1);
    TEST_ASSERT_NULL(h.name);
}

void test_holiday_get_invalid_index_too_large(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 10);
    TEST_ASSERT_NULL(h.name);
}

void test_holiday_find_passover(void)
{
    hebrew_holiday_t found = hebrew_holiday_find(5786, "Passover");
    hebrew_holiday_t got   = hebrew_holiday_get(5786, 7);
    TEST_ASSERT_EQUAL_STRING(found.name, got.name);
    TEST_ASSERT_EQUAL_INT(found.month, got.month);
    TEST_ASSERT_EQUAL_INT(found.day, got.day);
    double jd_diff = found.jd - got.jd;
    if (jd_diff < 0.0) jd_diff = -jd_diff;
    TEST_ASSERT_TRUE(jd_diff < 0.01);
}

void test_holiday_find_nonexistent(void)
{
    hebrew_holiday_t h = hebrew_holiday_find(5786, "Christmas");
    TEST_ASSERT_NULL(h.name);
}

void test_holiday_find_case_sensitive(void)
{
    /* "passover" (lowercase) should NOT match "Passover" */
    hebrew_holiday_t h = hebrew_holiday_find(5786, "passover");
    TEST_ASSERT_NULL(h.name);
}

void test_holiday_rosh_hashanah_jd_matches_new_year(void)
{
    /* Rosh Hashanah JD should match hebrew_to_jd for 1 Tishrei */
    hebrew_holiday_t h = hebrew_holiday_get(5785, 0);
    hebrew_date_t d = { .year = 5785, .month = 7, .day = 1 };
    double expected_jd = hebrew_to_jd(d);
    double diff = h.jd - expected_jd;
    if (diff < 0.0) diff = -diff;
    TEST_ASSERT_TRUE(diff < 0.01);
}

/* ===== Purity tests ===== */

void test_purity_sabbatical_info(void)
{
    /* Same input always produces same output */
    sabbatical_info_t a = sabbatical_info(5786);
    sabbatical_info_t b = sabbatical_info(5786);
    TEST_ASSERT_EQUAL_INT(a.year_in_cycle, b.year_in_cycle);
    TEST_ASSERT_EQUAL(a.is_shemitah, b.is_shemitah);
}

void test_purity_jubilee_info(void)
{
    jubilee_info_t a = jubilee_info(5786);
    jubilee_info_t b = jubilee_info(5786);
    TEST_ASSERT_EQUAL_INT(a.year_in_cycle, b.year_in_cycle);
    TEST_ASSERT_EQUAL(a.is_jubilee, b.is_jubilee);
    TEST_ASSERT_EQUAL_INT(a.sabbatical_count, b.sabbatical_count);
}

void test_purity_holiday_get(void)
{
    hebrew_holiday_t a = hebrew_holiday_get(5786, 3);
    hebrew_holiday_t b = hebrew_holiday_get(5786, 3);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
    double jd_diff = a.jd - b.jd;
    if (jd_diff < 0.0) jd_diff = -jd_diff;
    TEST_ASSERT_TRUE(jd_diff < 0.01);
}

/* ===== Additional coverage ===== */

void test_holiday_sukkot(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 2);
    TEST_ASSERT_EQUAL_STRING("Sukkot", h.name);
    TEST_ASSERT_EQUAL_INT(7, h.month);
    TEST_ASSERT_EQUAL_INT(15, h.day);
}

void test_holiday_hanukkah(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 4);
    TEST_ASSERT_EQUAL_STRING("Hanukkah", h.name);
    TEST_ASSERT_EQUAL_INT(9, h.month);
    TEST_ASSERT_EQUAL_INT(25, h.day);
}

void test_holiday_shavuot(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 8);
    TEST_ASSERT_EQUAL_STRING("Shavuot", h.name);
    TEST_ASSERT_EQUAL_INT(3, h.month);
    TEST_ASSERT_EQUAL_INT(6, h.day);
}

void test_holiday_tisha_bav(void)
{
    hebrew_holiday_t h = hebrew_holiday_get(5786, 9);
    TEST_ASSERT_EQUAL_STRING("Tisha B'Av", h.name);
    TEST_ASSERT_EQUAL_INT(5, h.month);
    TEST_ASSERT_EQUAL_INT(9, h.day);
}

int main(void)
{
    UNITY_BEGIN();

    /* Sabbatical (Shemitah) cycle */
    RUN_TEST(test_sabbatical_info_5782_is_shemitah);
    RUN_TEST(test_sabbatical_info_5783_first_of_cycle);
    RUN_TEST(test_sabbatical_info_5785_mid_cycle);
    RUN_TEST(test_sabbatical_is_shemitah_5782_true);
    RUN_TEST(test_sabbatical_is_shemitah_5781_false);
    RUN_TEST(test_sabbatical_is_shemitah_5789_true);
    RUN_TEST(test_sabbatical_next_from_shemitah_year);
    RUN_TEST(test_sabbatical_next_from_5783);
    RUN_TEST(test_sabbatical_next_from_5780);

    /* Jubilee cycle */
    RUN_TEST(test_jubilee_info_5750_is_jubilee);
    RUN_TEST(test_jubilee_info_5751_not_jubilee);
    RUN_TEST(test_jubilee_info_range_check);
    RUN_TEST(test_jubilee_is_jubilee_5800_true);
    RUN_TEST(test_jubilee_is_jubilee_5799_false);
    RUN_TEST(test_jubilee_next_from_5750);
    RUN_TEST(test_jubilee_next_from_5751);
    RUN_TEST(test_jubilee_next_from_5799);
    RUN_TEST(test_jubilee_sabbatical_count_mid);

    /* Holidays */
    RUN_TEST(test_holiday_count_is_10);
    RUN_TEST(test_holiday_get_rosh_hashanah);
    RUN_TEST(test_holiday_get_yom_kippur);
    RUN_TEST(test_holiday_get_passover);
    RUN_TEST(test_holiday_get_purim_common_year);
    RUN_TEST(test_holiday_get_purim_leap_year);
    RUN_TEST(test_holiday_get_jd_nonzero);
    RUN_TEST(test_holiday_get_invalid_index_negative);
    RUN_TEST(test_holiday_get_invalid_index_too_large);
    RUN_TEST(test_holiday_find_passover);
    RUN_TEST(test_holiday_find_nonexistent);
    RUN_TEST(test_holiday_find_case_sensitive);
    RUN_TEST(test_holiday_rosh_hashanah_jd_matches_new_year);

    /* Purity */
    RUN_TEST(test_purity_sabbatical_info);
    RUN_TEST(test_purity_jubilee_info);
    RUN_TEST(test_purity_holiday_get);

    /* Additional coverage */
    RUN_TEST(test_holiday_sukkot);
    RUN_TEST(test_holiday_hanukkah);
    RUN_TEST(test_holiday_shavuot);
    RUN_TEST(test_holiday_tisha_bav);

    return UNITY_END();
}
