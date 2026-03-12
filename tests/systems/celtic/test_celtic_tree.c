/* test_celtic_tree.c — Celtic Tree Calendar tests
 * TDD RED phase: tests written before implementation.
 * Robert Graves system: 13 months x 28 days + 1 intercalary = 365 days.
 * Year starts Dec 24 (Birch), ends Dec 23 (Yew intercalary day). */

#include "../../unity/unity.h"
#include "../../../src/systems/celtic/celtic_tree.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Tree names (14 tests) ===== */

void test_tree_name_birch(void)
{
    TEST_ASSERT_EQUAL_STRING("Birch", celtic_tree_name(1));
}

void test_tree_name_rowan(void)
{
    TEST_ASSERT_EQUAL_STRING("Rowan", celtic_tree_name(2));
}

void test_tree_name_ash(void)
{
    TEST_ASSERT_EQUAL_STRING("Ash", celtic_tree_name(3));
}

void test_tree_name_alder(void)
{
    TEST_ASSERT_EQUAL_STRING("Alder", celtic_tree_name(4));
}

void test_tree_name_willow(void)
{
    TEST_ASSERT_EQUAL_STRING("Willow", celtic_tree_name(5));
}

void test_tree_name_hawthorn(void)
{
    TEST_ASSERT_EQUAL_STRING("Hawthorn", celtic_tree_name(6));
}

void test_tree_name_oak(void)
{
    TEST_ASSERT_EQUAL_STRING("Oak", celtic_tree_name(7));
}

void test_tree_name_holly(void)
{
    TEST_ASSERT_EQUAL_STRING("Holly", celtic_tree_name(8));
}

void test_tree_name_hazel(void)
{
    TEST_ASSERT_EQUAL_STRING("Hazel", celtic_tree_name(9));
}

void test_tree_name_vine(void)
{
    TEST_ASSERT_EQUAL_STRING("Vine", celtic_tree_name(10));
}

void test_tree_name_ivy(void)
{
    TEST_ASSERT_EQUAL_STRING("Ivy", celtic_tree_name(11));
}

void test_tree_name_reed(void)
{
    TEST_ASSERT_EQUAL_STRING("Reed", celtic_tree_name(12));
}

void test_tree_name_elder(void)
{
    TEST_ASSERT_EQUAL_STRING("Elder", celtic_tree_name(13));
}

void test_tree_name_yew_intercalary(void)
{
    TEST_ASSERT_EQUAL_STRING("Yew", celtic_tree_name(0));
}

void test_tree_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_name(-1));
}

void test_tree_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_name(14));
}

/* ===== Ogham names (16 tests) ===== */

void test_ogham_birch(void)
{
    TEST_ASSERT_EQUAL_STRING("Beth", celtic_tree_ogham(1));
}

void test_ogham_rowan(void)
{
    TEST_ASSERT_EQUAL_STRING("Luis", celtic_tree_ogham(2));
}

void test_ogham_ash(void)
{
    TEST_ASSERT_EQUAL_STRING("Nion", celtic_tree_ogham(3));
}

void test_ogham_alder(void)
{
    TEST_ASSERT_EQUAL_STRING("Fearn", celtic_tree_ogham(4));
}

void test_ogham_willow(void)
{
    TEST_ASSERT_EQUAL_STRING("Saille", celtic_tree_ogham(5));
}

void test_ogham_hawthorn(void)
{
    TEST_ASSERT_EQUAL_STRING("Huath", celtic_tree_ogham(6));
}

void test_ogham_oak(void)
{
    TEST_ASSERT_EQUAL_STRING("Duir", celtic_tree_ogham(7));
}

void test_ogham_holly(void)
{
    TEST_ASSERT_EQUAL_STRING("Tinne", celtic_tree_ogham(8));
}

void test_ogham_hazel(void)
{
    TEST_ASSERT_EQUAL_STRING("Coll", celtic_tree_ogham(9));
}

void test_ogham_vine(void)
{
    TEST_ASSERT_EQUAL_STRING("Muin", celtic_tree_ogham(10));
}

void test_ogham_ivy(void)
{
    TEST_ASSERT_EQUAL_STRING("Gort", celtic_tree_ogham(11));
}

void test_ogham_reed(void)
{
    TEST_ASSERT_EQUAL_STRING("Ngetal", celtic_tree_ogham(12));
}

void test_ogham_elder(void)
{
    TEST_ASSERT_EQUAL_STRING("Ruis", celtic_tree_ogham(13));
}

void test_ogham_yew_intercalary(void)
{
    TEST_ASSERT_EQUAL_STRING("Idho", celtic_tree_ogham(0));
}

void test_ogham_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_ogham(-1));
}

void test_ogham_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_ogham(14));
}

/* ===== Ogham letters (16 tests) ===== */

void test_letter_birch(void)
{
    TEST_ASSERT_EQUAL_STRING("B", celtic_tree_letter(1));
}

void test_letter_rowan(void)
{
    TEST_ASSERT_EQUAL_STRING("L", celtic_tree_letter(2));
}

void test_letter_ash(void)
{
    TEST_ASSERT_EQUAL_STRING("N", celtic_tree_letter(3));
}

void test_letter_alder(void)
{
    TEST_ASSERT_EQUAL_STRING("F", celtic_tree_letter(4));
}

void test_letter_willow(void)
{
    TEST_ASSERT_EQUAL_STRING("S", celtic_tree_letter(5));
}

void test_letter_hawthorn(void)
{
    TEST_ASSERT_EQUAL_STRING("H", celtic_tree_letter(6));
}

void test_letter_oak(void)
{
    TEST_ASSERT_EQUAL_STRING("D", celtic_tree_letter(7));
}

void test_letter_holly(void)
{
    TEST_ASSERT_EQUAL_STRING("T", celtic_tree_letter(8));
}

void test_letter_hazel(void)
{
    TEST_ASSERT_EQUAL_STRING("C", celtic_tree_letter(9));
}

void test_letter_vine(void)
{
    TEST_ASSERT_EQUAL_STRING("M", celtic_tree_letter(10));
}

void test_letter_ivy(void)
{
    TEST_ASSERT_EQUAL_STRING("G", celtic_tree_letter(11));
}

void test_letter_reed(void)
{
    TEST_ASSERT_EQUAL_STRING("Ng", celtic_tree_letter(12));
}

void test_letter_elder(void)
{
    TEST_ASSERT_EQUAL_STRING("R", celtic_tree_letter(13));
}

void test_letter_yew_intercalary(void)
{
    TEST_ASSERT_EQUAL_STRING("I", celtic_tree_letter(0));
}

void test_letter_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_letter(-1));
}

void test_letter_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_letter(14));
}

/* ===== Meanings (16 tests) ===== */

void test_meaning_birch(void)
{
    const char *m = celtic_tree_meaning(1);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "beginnings") != NULL || strstr(m, "rebirth") != NULL);
}

void test_meaning_rowan(void)
{
    const char *m = celtic_tree_meaning(2);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Protection") != NULL || strstr(m, "vision") != NULL);
}

void test_meaning_ash(void)
{
    const char *m = celtic_tree_meaning(3);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "connection") != NULL || strstr(m, "World") != NULL);
}

void test_meaning_alder(void)
{
    const char *m = celtic_tree_meaning(4);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "endurance") != NULL || strstr(m, "Foundation") != NULL);
}

void test_meaning_willow(void)
{
    const char *m = celtic_tree_meaning(5);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "intuition") != NULL || strstr(m, "Moon") != NULL);
}

void test_meaning_hawthorn(void)
{
    const char *m = celtic_tree_meaning(6);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Cleansing") != NULL || strstr(m, "sacred") != NULL);
}

void test_meaning_oak(void)
{
    const char *m = celtic_tree_meaning(7);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Strength") != NULL || strstr(m, "sovereignty") != NULL);
}

void test_meaning_holly(void)
{
    const char *m = celtic_tree_meaning(8);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Defense") != NULL || strstr(m, "warrior") != NULL);
}

void test_meaning_hazel(void)
{
    const char *m = celtic_tree_meaning(9);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Wisdom") != NULL || strstr(m, "inspiration") != NULL);
}

void test_meaning_vine(void)
{
    const char *m = celtic_tree_meaning(10);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Harvest") != NULL || strstr(m, "prophecy") != NULL);
}

void test_meaning_ivy(void)
{
    const char *m = celtic_tree_meaning(11);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Persistence") != NULL || strstr(m, "spiral") != NULL);
}

void test_meaning_reed(void)
{
    const char *m = celtic_tree_meaning(12);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Harmony") != NULL || strstr(m, "direction") != NULL);
}

void test_meaning_elder(void)
{
    const char *m = celtic_tree_meaning(13);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Transformation") != NULL || strstr(m, "death") != NULL);
}

void test_meaning_yew_intercalary(void)
{
    const char *m = celtic_tree_meaning(0);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Immortality") != NULL || strstr(m, "rebirth") != NULL);
}

void test_meaning_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_meaning(-1));
}

void test_meaning_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", celtic_tree_meaning(14));
}

/* ===== Month days (5 tests) ===== */

void test_month_days_regular(void)
{
    TEST_ASSERT_EQUAL_INT(28, celtic_tree_month_days(1));
}

void test_month_days_last_regular(void)
{
    TEST_ASSERT_EQUAL_INT(28, celtic_tree_month_days(13));
}

void test_month_days_mid(void)
{
    TEST_ASSERT_EQUAL_INT(28, celtic_tree_month_days(7));
}

void test_month_days_intercalary(void)
{
    TEST_ASSERT_EQUAL_INT(1, celtic_tree_month_days(0));
}

void test_month_days_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, celtic_tree_month_days(14));
}

/* ===== Day of year (7 tests) ===== */

void test_day_of_year_first(void)
{
    /* Birch day 1 = Celtic day 1 */
    celtic_tree_date_t d = { .month = 1, .day = 1 };
    TEST_ASSERT_EQUAL_INT(1, celtic_tree_day_of_year(d));
}

void test_day_of_year_birch_last(void)
{
    /* Birch day 28 = Celtic day 28 */
    celtic_tree_date_t d = { .month = 1, .day = 28 };
    TEST_ASSERT_EQUAL_INT(28, celtic_tree_day_of_year(d));
}

void test_day_of_year_rowan_first(void)
{
    /* Rowan day 1 = Celtic day 29 */
    celtic_tree_date_t d = { .month = 2, .day = 1 };
    TEST_ASSERT_EQUAL_INT(29, celtic_tree_day_of_year(d));
}

void test_day_of_year_elder_last(void)
{
    /* Elder day 28 = Celtic day 364 */
    celtic_tree_date_t d = { .month = 13, .day = 28 };
    TEST_ASSERT_EQUAL_INT(364, celtic_tree_day_of_year(d));
}

void test_day_of_year_intercalary(void)
{
    /* Yew intercalary = Celtic day 365 */
    celtic_tree_date_t d = { .month = 0, .day = 1 };
    TEST_ASSERT_EQUAL_INT(365, celtic_tree_day_of_year(d));
}

void test_day_of_year_oak_mid(void)
{
    /* Oak (month 7) day 14 = (7-1)*28 + 14 = 182 */
    celtic_tree_date_t d = { .month = 7, .day = 14 };
    TEST_ASSERT_EQUAL_INT(182, celtic_tree_day_of_year(d));
}

void test_day_of_year_month_boundary(void)
{
    /* Ash (month 3) day 1 = (3-1)*28 + 1 = 57 */
    celtic_tree_date_t d = { .month = 3, .day = 1 };
    TEST_ASSERT_EQUAL_INT(57, celtic_tree_day_of_year(d));
}

/* ===== Is intercalary (4 tests) ===== */

void test_is_intercalary_true(void)
{
    celtic_tree_date_t d = { .month = 0, .day = 1 };
    TEST_ASSERT_TRUE(celtic_tree_is_intercalary(d));
}

void test_is_intercalary_false_birch(void)
{
    celtic_tree_date_t d = { .month = 1, .day = 1 };
    TEST_ASSERT_FALSE(celtic_tree_is_intercalary(d));
}

void test_is_intercalary_false_elder(void)
{
    celtic_tree_date_t d = { .month = 13, .day = 28 };
    TEST_ASSERT_FALSE(celtic_tree_is_intercalary(d));
}

void test_is_intercalary_false_mid(void)
{
    celtic_tree_date_t d = { .month = 7, .day = 14 };
    TEST_ASSERT_FALSE(celtic_tree_is_intercalary(d));
}

/* ===== JD from date / celtic_tree_from_jd (8 tests) ===== */

/* Known dates (verified via Gregorian-to-JD algorithm):
 * Dec 24, 2025 = JD 2461033.5 (Birch 1)
 * Jan 1, 2026  = JD 2461041.5 (Birch 9)
 * Jan 20, 2026 = JD 2461060.5 (Birch 28)
 * Jan 21, 2026 = JD 2461061.5 (Rowan 1)
 * Dec 22, 2026 = JD 2461396.5 (Elder 28)
 * Dec 23, 2026 = JD 2461397.5 (Yew intercalary)
 * Dec 24, 2026 = JD 2461398.5 (next Birch 1) */

void test_from_jd_birch_1(void)
{
    /* Dec 24, 2025 = Birch 1 */
    celtic_tree_date_t d = celtic_tree_from_jd(2461033.5);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_birch_9(void)
{
    /* Jan 1, 2026 = Birch 9 */
    celtic_tree_date_t d = celtic_tree_from_jd(2461041.5);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(9, d.day);
}

void test_from_jd_rowan_1(void)
{
    /* Jan 21, 2026 = Rowan 1 */
    celtic_tree_date_t d = celtic_tree_from_jd(2461061.5);
    TEST_ASSERT_EQUAL_INT(2, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_elder_28(void)
{
    /* Dec 22, 2026 = Elder 28 */
    celtic_tree_date_t d = celtic_tree_from_jd(2461396.5);
    TEST_ASSERT_EQUAL_INT(13, d.month);
    TEST_ASSERT_EQUAL_INT(28, d.day);
}

void test_from_jd_yew_intercalary(void)
{
    /* Dec 23, 2026 = Yew intercalary */
    celtic_tree_date_t d = celtic_tree_from_jd(2461397.5);
    TEST_ASSERT_EQUAL_INT(0, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_next_year_birch_1(void)
{
    /* Dec 24, 2026 = Birch 1 of next Celtic year */
    celtic_tree_date_t d = celtic_tree_from_jd(2461398.5);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_from_jd_willow_15(void)
{
    /* Willow (month 5) day 15 = Celtic day 127
     * JD = 2461033.5 + 126 = 2461159.5 (Apr 29, 2026) */
    celtic_tree_date_t d = celtic_tree_from_jd(2461159.5);
    TEST_ASSERT_EQUAL_INT(5, d.month);
    TEST_ASSERT_EQUAL_INT(15, d.day);
}

void test_from_jd_birch_28(void)
{
    /* Birch 28 = Celtic day 28
     * JD = 2461033.5 + 27 = 2461060.5 (Jan 20, 2026) */
    celtic_tree_date_t d = celtic_tree_from_jd(2461060.5);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(28, d.day);
}

/* ===== JD to date / celtic_tree_to_jd (7 tests) ===== */

void test_to_jd_birch_1(void)
{
    /* Birch 1, gregorian_year 2026 -> Dec 24, 2025 */
    celtic_tree_date_t d = { .month = 1, .day = 1 };
    double jd = celtic_tree_to_jd(d, 2026);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461033.5, jd);
}

void test_to_jd_rowan_1(void)
{
    /* Rowan 1, gregorian_year 2026 -> Jan 21, 2026 */
    celtic_tree_date_t d = { .month = 2, .day = 1 };
    double jd = celtic_tree_to_jd(d, 2026);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461061.5, jd);
}

void test_to_jd_yew_intercalary(void)
{
    /* Yew intercalary, gregorian_year 2026 -> Dec 23, 2026 */
    celtic_tree_date_t d = { .month = 0, .day = 1 };
    double jd = celtic_tree_to_jd(d, 2026);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461397.5, jd);
}

void test_to_jd_elder_28(void)
{
    /* Elder 28, gregorian_year 2026 -> Dec 22, 2026 */
    celtic_tree_date_t d = { .month = 13, .day = 28 };
    double jd = celtic_tree_to_jd(d, 2026);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461396.5, jd);
}

void test_to_jd_oak_1(void)
{
    /* Oak 1 = Celtic day 169
     * JD = 2461033.5 + 168 = 2461201.5 (Jun 10, 2026) */
    celtic_tree_date_t d = { .month = 7, .day = 1 };
    double jd = celtic_tree_to_jd(d, 2026);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461201.5, jd);
}

void test_to_jd_different_year(void)
{
    /* Birch 1, gregorian_year 2027 -> Dec 24, 2026 */
    celtic_tree_date_t d = { .month = 1, .day = 1 };
    double jd = celtic_tree_to_jd(d, 2027);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461398.5, jd);
}

void test_to_jd_hazel_14(void)
{
    /* Hazel (month 9) day 14 = Celtic day (8*28)+14 = 238
     * JD = 2461033.5 + 237 = 2461270.5 (Aug 18, 2026) */
    celtic_tree_date_t d = { .month = 9, .day = 14 };
    double jd = celtic_tree_to_jd(d, 2026);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2461270.5, jd);
}

/* ===== Round-trip tests (6 tests) ===== */

void test_round_trip_birch_1(void)
{
    celtic_tree_date_t orig = { .month = 1, .day = 1 };
    double jd = celtic_tree_to_jd(orig, 2026);
    celtic_tree_date_t back = celtic_tree_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_elder_28(void)
{
    celtic_tree_date_t orig = { .month = 13, .day = 28 };
    double jd = celtic_tree_to_jd(orig, 2026);
    celtic_tree_date_t back = celtic_tree_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_yew_intercalary(void)
{
    celtic_tree_date_t orig = { .month = 0, .day = 1 };
    double jd = celtic_tree_to_jd(orig, 2026);
    celtic_tree_date_t back = celtic_tree_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_hawthorn_14(void)
{
    celtic_tree_date_t orig = { .month = 6, .day = 14 };
    double jd = celtic_tree_to_jd(orig, 2026);
    celtic_tree_date_t back = celtic_tree_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_vine_1(void)
{
    celtic_tree_date_t orig = { .month = 10, .day = 1 };
    double jd = celtic_tree_to_jd(orig, 2026);
    celtic_tree_date_t back = celtic_tree_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

void test_round_trip_different_year(void)
{
    celtic_tree_date_t orig = { .month = 5, .day = 20 };
    double jd = celtic_tree_to_jd(orig, 2030);
    celtic_tree_date_t back = celtic_tree_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(orig.month, back.month);
    TEST_ASSERT_EQUAL_INT(orig.day, back.day);
}

/* ===== Tree info struct (5 tests) ===== */

void test_info_birch(void)
{
    celtic_tree_info_t info = celtic_tree_info(1);
    TEST_ASSERT_EQUAL_INT(1, info.id);
    TEST_ASSERT_EQUAL_STRING("Birch", info.tree);
    TEST_ASSERT_EQUAL_STRING("Beth", info.ogham);
    TEST_ASSERT_EQUAL_STRING("B", info.letter);
    TEST_ASSERT_NOT_NULL(info.meaning);
    TEST_ASSERT_EQUAL_INT(358, info.start_day_of_year);
}

void test_info_yew_intercalary(void)
{
    celtic_tree_info_t info = celtic_tree_info(0);
    TEST_ASSERT_EQUAL_INT(0, info.id);
    TEST_ASSERT_EQUAL_STRING("Yew", info.tree);
    TEST_ASSERT_EQUAL_STRING("Idho", info.ogham);
    TEST_ASSERT_EQUAL_STRING("I", info.letter);
    TEST_ASSERT_EQUAL_INT(357, info.start_day_of_year);
}

void test_info_invalid(void)
{
    celtic_tree_info_t info = celtic_tree_info(99);
    TEST_ASSERT_EQUAL_INT(-1, info.id);
    TEST_ASSERT_EQUAL_STRING("?", info.tree);
}

void test_info_oak_start_day(void)
{
    /* Oak starts Jun 10 = day 161 */
    celtic_tree_info_t info = celtic_tree_info(7);
    TEST_ASSERT_EQUAL_INT(161, info.start_day_of_year);
}

void test_info_rowan_start_day(void)
{
    /* Rowan starts Jan 21 = day 21 */
    celtic_tree_info_t info = celtic_tree_info(2);
    TEST_ASSERT_EQUAL_INT(21, info.start_day_of_year);
}

/* ===== Festival cross-reference (9 tests) ===== */

void test_festival_samhain(void)
{
    /* WHEEL_SAMHAIN = 0 -> Reed (month 12, Oct 28 - Nov 24) */
    TEST_ASSERT_EQUAL_INT(12, celtic_tree_for_festival(0));
}

void test_festival_yule(void)
{
    /* WHEEL_YULE = 1 -> Elder (month 13, Nov 25 - Dec 22) */
    TEST_ASSERT_EQUAL_INT(13, celtic_tree_for_festival(1));
}

void test_festival_imbolc(void)
{
    /* WHEEL_IMBOLC = 2 -> Rowan (month 2, Jan 21 - Feb 17) */
    TEST_ASSERT_EQUAL_INT(2, celtic_tree_for_festival(2));
}

void test_festival_ostara(void)
{
    /* WHEEL_OSTARA = 3 -> Alder (month 4, Mar 18 - Apr 14) */
    TEST_ASSERT_EQUAL_INT(4, celtic_tree_for_festival(3));
}

void test_festival_beltane(void)
{
    /* WHEEL_BELTANE = 4 -> Willow (month 5, Apr 15 - May 12) */
    TEST_ASSERT_EQUAL_INT(5, celtic_tree_for_festival(4));
}

void test_festival_litha(void)
{
    /* WHEEL_LITHA = 5 -> Oak (month 7, Jun 10 - Jul 7) */
    TEST_ASSERT_EQUAL_INT(7, celtic_tree_for_festival(5));
}

void test_festival_lughnasadh(void)
{
    /* WHEEL_LUGHNASADH = 6 -> Holly (month 8, Jul 8 - Aug 4) */
    TEST_ASSERT_EQUAL_INT(8, celtic_tree_for_festival(6));
}

void test_festival_mabon(void)
{
    /* WHEEL_MABON = 7 -> Vine (month 10, Sep 2 - Sep 29) */
    TEST_ASSERT_EQUAL_INT(10, celtic_tree_for_festival(7));
}

void test_festival_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, celtic_tree_for_festival(8));
}

void test_festival_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-1, celtic_tree_for_festival(-1));
}

/* ===== Edge cases (4 tests) ===== */

void test_from_jd_noon(void)
{
    /* JD at noon (x.0) should still map correctly
     * 2461034.0 = noon of Dec 24, 2025 -> still Birch 1 */
    celtic_tree_date_t d = celtic_tree_from_jd(2461034.0);
    TEST_ASSERT_EQUAL_INT(1, d.month);
    TEST_ASSERT_EQUAL_INT(1, d.day);
}

void test_all_months_have_28_days(void)
{
    for (int m = 1; m <= 13; m++) {
        TEST_ASSERT_EQUAL_INT(28, celtic_tree_month_days(m));
    }
}

void test_celtic_year_has_365_days(void)
{
    /* 13 * 28 + 1 = 365 */
    int total = 0;
    for (int m = 0; m <= 13; m++) {
        total += celtic_tree_month_days(m);
    }
    TEST_ASSERT_EQUAL_INT(CELTIC_TREE_YEAR_DAYS, total);
}

void test_constants(void)
{
    TEST_ASSERT_EQUAL_INT(13, CELTIC_TREE_MONTHS);
    TEST_ASSERT_EQUAL_INT(28, CELTIC_TREE_MONTH_DAYS);
    TEST_ASSERT_EQUAL_INT(365, CELTIC_TREE_YEAR_DAYS);
}

/* ===== Main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Tree names */
    RUN_TEST(test_tree_name_birch);
    RUN_TEST(test_tree_name_rowan);
    RUN_TEST(test_tree_name_ash);
    RUN_TEST(test_tree_name_alder);
    RUN_TEST(test_tree_name_willow);
    RUN_TEST(test_tree_name_hawthorn);
    RUN_TEST(test_tree_name_oak);
    RUN_TEST(test_tree_name_holly);
    RUN_TEST(test_tree_name_hazel);
    RUN_TEST(test_tree_name_vine);
    RUN_TEST(test_tree_name_ivy);
    RUN_TEST(test_tree_name_reed);
    RUN_TEST(test_tree_name_elder);
    RUN_TEST(test_tree_name_yew_intercalary);
    RUN_TEST(test_tree_name_invalid_negative);
    RUN_TEST(test_tree_name_invalid_high);

    /* Ogham names */
    RUN_TEST(test_ogham_birch);
    RUN_TEST(test_ogham_rowan);
    RUN_TEST(test_ogham_ash);
    RUN_TEST(test_ogham_alder);
    RUN_TEST(test_ogham_willow);
    RUN_TEST(test_ogham_hawthorn);
    RUN_TEST(test_ogham_oak);
    RUN_TEST(test_ogham_holly);
    RUN_TEST(test_ogham_hazel);
    RUN_TEST(test_ogham_vine);
    RUN_TEST(test_ogham_ivy);
    RUN_TEST(test_ogham_reed);
    RUN_TEST(test_ogham_elder);
    RUN_TEST(test_ogham_yew_intercalary);
    RUN_TEST(test_ogham_invalid_negative);
    RUN_TEST(test_ogham_invalid_high);

    /* Ogham letters */
    RUN_TEST(test_letter_birch);
    RUN_TEST(test_letter_rowan);
    RUN_TEST(test_letter_ash);
    RUN_TEST(test_letter_alder);
    RUN_TEST(test_letter_willow);
    RUN_TEST(test_letter_hawthorn);
    RUN_TEST(test_letter_oak);
    RUN_TEST(test_letter_holly);
    RUN_TEST(test_letter_hazel);
    RUN_TEST(test_letter_vine);
    RUN_TEST(test_letter_ivy);
    RUN_TEST(test_letter_reed);
    RUN_TEST(test_letter_elder);
    RUN_TEST(test_letter_yew_intercalary);
    RUN_TEST(test_letter_invalid_negative);
    RUN_TEST(test_letter_invalid_high);

    /* Meanings */
    RUN_TEST(test_meaning_birch);
    RUN_TEST(test_meaning_rowan);
    RUN_TEST(test_meaning_ash);
    RUN_TEST(test_meaning_alder);
    RUN_TEST(test_meaning_willow);
    RUN_TEST(test_meaning_hawthorn);
    RUN_TEST(test_meaning_oak);
    RUN_TEST(test_meaning_holly);
    RUN_TEST(test_meaning_hazel);
    RUN_TEST(test_meaning_vine);
    RUN_TEST(test_meaning_ivy);
    RUN_TEST(test_meaning_reed);
    RUN_TEST(test_meaning_elder);
    RUN_TEST(test_meaning_yew_intercalary);
    RUN_TEST(test_meaning_invalid_negative);
    RUN_TEST(test_meaning_invalid_high);

    /* Month days */
    RUN_TEST(test_month_days_regular);
    RUN_TEST(test_month_days_last_regular);
    RUN_TEST(test_month_days_mid);
    RUN_TEST(test_month_days_intercalary);
    RUN_TEST(test_month_days_invalid);

    /* Day of year */
    RUN_TEST(test_day_of_year_first);
    RUN_TEST(test_day_of_year_birch_last);
    RUN_TEST(test_day_of_year_rowan_first);
    RUN_TEST(test_day_of_year_elder_last);
    RUN_TEST(test_day_of_year_intercalary);
    RUN_TEST(test_day_of_year_oak_mid);
    RUN_TEST(test_day_of_year_month_boundary);

    /* Is intercalary */
    RUN_TEST(test_is_intercalary_true);
    RUN_TEST(test_is_intercalary_false_birch);
    RUN_TEST(test_is_intercalary_false_elder);
    RUN_TEST(test_is_intercalary_false_mid);

    /* JD from date */
    RUN_TEST(test_from_jd_birch_1);
    RUN_TEST(test_from_jd_birch_9);
    RUN_TEST(test_from_jd_rowan_1);
    RUN_TEST(test_from_jd_elder_28);
    RUN_TEST(test_from_jd_yew_intercalary);
    RUN_TEST(test_from_jd_next_year_birch_1);
    RUN_TEST(test_from_jd_willow_15);
    RUN_TEST(test_from_jd_birch_28);

    /* JD to date */
    RUN_TEST(test_to_jd_birch_1);
    RUN_TEST(test_to_jd_rowan_1);
    RUN_TEST(test_to_jd_yew_intercalary);
    RUN_TEST(test_to_jd_elder_28);
    RUN_TEST(test_to_jd_oak_1);
    RUN_TEST(test_to_jd_different_year);
    RUN_TEST(test_to_jd_hazel_14);

    /* Round-trip */
    RUN_TEST(test_round_trip_birch_1);
    RUN_TEST(test_round_trip_elder_28);
    RUN_TEST(test_round_trip_yew_intercalary);
    RUN_TEST(test_round_trip_hawthorn_14);
    RUN_TEST(test_round_trip_vine_1);
    RUN_TEST(test_round_trip_different_year);

    /* Tree info */
    RUN_TEST(test_info_birch);
    RUN_TEST(test_info_yew_intercalary);
    RUN_TEST(test_info_invalid);
    RUN_TEST(test_info_oak_start_day);
    RUN_TEST(test_info_rowan_start_day);

    /* Festival cross-reference */
    RUN_TEST(test_festival_samhain);
    RUN_TEST(test_festival_yule);
    RUN_TEST(test_festival_imbolc);
    RUN_TEST(test_festival_ostara);
    RUN_TEST(test_festival_beltane);
    RUN_TEST(test_festival_litha);
    RUN_TEST(test_festival_lughnasadh);
    RUN_TEST(test_festival_mabon);
    RUN_TEST(test_festival_invalid);
    RUN_TEST(test_festival_invalid_negative);

    /* Edge cases */
    RUN_TEST(test_from_jd_noon);
    RUN_TEST(test_all_months_have_28_days);
    RUN_TEST(test_celtic_year_has_365_days);
    RUN_TEST(test_constants);

    return UNITY_END();
}
