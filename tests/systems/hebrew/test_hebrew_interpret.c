/* test_hebrew_interpret.c — Hebrew month interpretation data + tiered text
 * TDD RED phase: tests written before implementation.
 * 13 months (Sefer Yetzirah tradition): letter, tribe, sense, quality, brief.
 * Composition: glyph (T0), glance (T1), detail (T3). */

#include "../../unity/unity.h"
#include "../../../src/systems/hebrew/hebrew_interpret.h"
#include "../../../src/systems/hebrew/hebrew.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== hi_month_count ===== */

void test_month_count_returns_13(void)
{
    TEST_ASSERT_EQUAL_INT(13, hi_month_count());
}

/* ===== hi_month_data — valid months 1-13 ===== */

void test_month_data_nisan(void)
{
    hi_month_t m = hi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Hei", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Judah", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Speech", m.sense);
    TEST_ASSERT_EQUAL_STRING("Redemption", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_iyar(void)
{
    hi_month_t m = hi_month_data(2);
    TEST_ASSERT_EQUAL_INT(2, m.month);
    TEST_ASSERT_EQUAL_STRING("Vav", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Issachar", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Thought", m.sense);
    TEST_ASSERT_EQUAL_STRING("Healing", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_sivan(void)
{
    hi_month_t m = hi_month_data(3);
    TEST_ASSERT_EQUAL_INT(3, m.month);
    TEST_ASSERT_EQUAL_STRING("Zayin", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Zebulun", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Motion", m.sense);
    TEST_ASSERT_EQUAL_STRING("Revelation", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_tammuz(void)
{
    hi_month_t m = hi_month_data(4);
    TEST_ASSERT_EQUAL_INT(4, m.month);
    TEST_ASSERT_EQUAL_STRING("Chet", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Reuben", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Sight", m.sense);
    TEST_ASSERT_EQUAL_STRING("Vision through darkness", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_av(void)
{
    hi_month_t m = hi_month_data(5);
    TEST_ASSERT_EQUAL_INT(5, m.month);
    TEST_ASSERT_EQUAL_STRING("Tet", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Simeon", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Hearing", m.sense);
    TEST_ASSERT_EQUAL_STRING("Transformation of sorrow", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_elul(void)
{
    hi_month_t m = hi_month_data(6);
    TEST_ASSERT_EQUAL_INT(6, m.month);
    TEST_ASSERT_EQUAL_STRING("Yod", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Gad", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Action", m.sense);
    TEST_ASSERT_EQUAL_STRING("Repentance", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_tishrei(void)
{
    hi_month_t m = hi_month_data(7);
    TEST_ASSERT_EQUAL_INT(7, m.month);
    TEST_ASSERT_EQUAL_STRING("Lamed", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Ephraim", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Touch", m.sense);
    TEST_ASSERT_EQUAL_STRING("Awe and judgment", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_cheshvan(void)
{
    hi_month_t m = hi_month_data(8);
    TEST_ASSERT_EQUAL_INT(8, m.month);
    TEST_ASSERT_EQUAL_STRING("Nun", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Manasseh", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Smell", m.sense);
    TEST_ASSERT_EQUAL_STRING("Quiet depth", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_kislev(void)
{
    hi_month_t m = hi_month_data(9);
    TEST_ASSERT_EQUAL_INT(9, m.month);
    TEST_ASSERT_EQUAL_STRING("Samekh", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Benjamin", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Sleep", m.sense);
    TEST_ASSERT_EQUAL_STRING("Trust and miracles", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_tevet(void)
{
    hi_month_t m = hi_month_data(10);
    TEST_ASSERT_EQUAL_INT(10, m.month);
    TEST_ASSERT_EQUAL_STRING("Ayin", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Dan", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Anger (righteous)", m.sense);
    TEST_ASSERT_EQUAL_STRING("Transformation", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_shevat(void)
{
    hi_month_t m = hi_month_data(11);
    TEST_ASSERT_EQUAL_INT(11, m.month);
    TEST_ASSERT_EQUAL_STRING("Tzadi", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Asher", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Taste", m.sense);
    TEST_ASSERT_EQUAL_STRING("Renewal", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_adar(void)
{
    hi_month_t m = hi_month_data(12);
    TEST_ASSERT_EQUAL_INT(12, m.month);
    TEST_ASSERT_EQUAL_STRING("Qof", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Naphtali", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Laughter", m.sense);
    TEST_ASSERT_EQUAL_STRING("Joy and reversal", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_adar_ii(void)
{
    hi_month_t m = hi_month_data(13);
    TEST_ASSERT_EQUAL_INT(13, m.month);
    TEST_ASSERT_EQUAL_STRING("Qof", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("Naphtali", m.tribe);
    TEST_ASSERT_EQUAL_STRING("Laughter", m.sense);
    TEST_ASSERT_EQUAL_STRING("Joy doubled", m.quality);
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

/* ===== hi_month_data — invalid months ===== */

void test_month_data_invalid_zero(void)
{
    hi_month_t m = hi_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING("?", m.tribe);
    TEST_ASSERT_EQUAL_STRING("?", m.sense);
    TEST_ASSERT_EQUAL_STRING("?", m.quality);
    TEST_ASSERT_EQUAL_STRING("?", m.brief);
}

void test_month_data_invalid_14(void)
{
    hi_month_t m = hi_month_data(14);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.hebrew_letter);
}

void test_month_data_invalid_negative(void)
{
    hi_month_t m = hi_month_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.hebrew_letter);
}

/* ===== hi_month_data — all months have non-empty strings ===== */

void test_month_data_all_strings_nonempty(void)
{
    int i;
    for (i = 1; i <= 13; i++) {
        hi_month_t m = hi_month_data(i);
        TEST_ASSERT_TRUE(strlen(m.hebrew_letter) > 0);
        TEST_ASSERT_TRUE(strlen(m.tribe) > 0);
        TEST_ASSERT_TRUE(strlen(m.sense) > 0);
        TEST_ASSERT_TRUE(strlen(m.quality) > 0);
        TEST_ASSERT_TRUE(strlen(m.brief) > 0);
    }
}

/* ===== hi_interpret — glyph ===== */

void test_interpret_glyph_nonempty(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
}

void test_interpret_glyph_matches_letter(void)
{
    /* Nisan => letter "Hei" */
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 1 };
    hebrew_interp_t r = hi_interpret(d, 1, 0);
    TEST_ASSERT_EQUAL_STRING("Hei", r.glyph);
}

void test_interpret_glyph_tishrei(void)
{
    hebrew_date_t d = { .year = 5786, .month = 7, .day = 1 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_EQUAL_STRING("Lamed", r.glyph);
}

/* ===== hi_interpret — glance ===== */

void test_interpret_glance_contains_year(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "5786"));
}

void test_interpret_glance_contains_month_name(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nisan"));
}

void test_interpret_glance_contains_quality(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Redemption"));
}

void test_interpret_glance_contains_day(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "15"));
}

/* ===== hi_interpret — detail ===== */

void test_interpret_detail_contains_letter(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hei"));
}

void test_interpret_detail_contains_tribe(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Judah"));
}

void test_interpret_detail_contains_sense(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Speech"));
}

void test_interpret_detail_contains_sabbatical_pos(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Shemitah year 4 of 7"));
}

void test_interpret_detail_omits_sabbatical_when_zero(void)
{
    hebrew_date_t d = { .year = 5786, .month = 1, .day = 15 };
    hebrew_interp_t r = hi_interpret(d, 0, 0);
    TEST_ASSERT_NULL(strstr(r.detail, "Shemitah"));
}

/* ===== hi_interpret — leap year Adar II ===== */

void test_interpret_adar_ii_leap(void)
{
    /* 5787 is a leap year */
    hebrew_date_t d = { .year = 5787, .month = 13, .day = 14 };
    hebrew_interp_t r = hi_interpret(d, 5, 1);
    TEST_ASSERT_EQUAL_STRING("Qof", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Joy doubled"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Naphtali"));
}

/* ===== hi_interpret — specific key months ===== */

void test_interpret_tishrei_detail(void)
{
    hebrew_date_t d = { .year = 5786, .month = 7, .day = 1 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ephraim"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Touch"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Awe and judgment"));
}

void test_interpret_adar_common(void)
{
    hebrew_date_t d = { .year = 5786, .month = 12, .day = 14 };
    hebrew_interp_t r = hi_interpret(d, 4, 0);
    TEST_ASSERT_EQUAL_STRING("Qof", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Joy and reversal"));
}

/* ===== Purity tests ===== */

void test_purity_month_data(void)
{
    hi_month_t a = hi_month_data(7);
    hi_month_t b = hi_month_data(7);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_STRING(a.hebrew_letter, b.hebrew_letter);
    TEST_ASSERT_EQUAL_STRING(a.tribe, b.tribe);
    TEST_ASSERT_EQUAL_STRING(a.sense, b.sense);
    TEST_ASSERT_EQUAL_STRING(a.quality, b.quality);
    TEST_ASSERT_EQUAL_STRING(a.brief, b.brief);
}

void test_purity_interpret(void)
{
    hebrew_date_t d = { .year = 5786, .month = 9, .day = 25 };
    hebrew_interp_t a = hi_interpret(d, 3, 0);
    hebrew_interp_t b = hi_interpret(d, 3, 0);
    TEST_ASSERT_EQUAL_STRING(a.glyph, b.glyph);
    TEST_ASSERT_EQUAL_STRING(a.glance, b.glance);
    TEST_ASSERT_EQUAL_STRING(a.detail, b.detail);
}

int main(void)
{
    UNITY_BEGIN();

    /* hi_month_count */
    RUN_TEST(test_month_count_returns_13);

    /* hi_month_data — all 13 months */
    RUN_TEST(test_month_data_nisan);
    RUN_TEST(test_month_data_iyar);
    RUN_TEST(test_month_data_sivan);
    RUN_TEST(test_month_data_tammuz);
    RUN_TEST(test_month_data_av);
    RUN_TEST(test_month_data_elul);
    RUN_TEST(test_month_data_tishrei);
    RUN_TEST(test_month_data_cheshvan);
    RUN_TEST(test_month_data_kislev);
    RUN_TEST(test_month_data_tevet);
    RUN_TEST(test_month_data_shevat);
    RUN_TEST(test_month_data_adar);
    RUN_TEST(test_month_data_adar_ii);

    /* hi_month_data — invalid */
    RUN_TEST(test_month_data_invalid_zero);
    RUN_TEST(test_month_data_invalid_14);
    RUN_TEST(test_month_data_invalid_negative);

    /* hi_month_data — bulk checks */
    RUN_TEST(test_month_data_all_strings_nonempty);

    /* hi_interpret — glyph */
    RUN_TEST(test_interpret_glyph_nonempty);
    RUN_TEST(test_interpret_glyph_matches_letter);
    RUN_TEST(test_interpret_glyph_tishrei);

    /* hi_interpret — glance */
    RUN_TEST(test_interpret_glance_contains_year);
    RUN_TEST(test_interpret_glance_contains_month_name);
    RUN_TEST(test_interpret_glance_contains_quality);
    RUN_TEST(test_interpret_glance_contains_day);

    /* hi_interpret — detail */
    RUN_TEST(test_interpret_detail_contains_letter);
    RUN_TEST(test_interpret_detail_contains_tribe);
    RUN_TEST(test_interpret_detail_contains_sense);
    RUN_TEST(test_interpret_detail_contains_sabbatical_pos);
    RUN_TEST(test_interpret_detail_omits_sabbatical_when_zero);

    /* hi_interpret — leap year / key months */
    RUN_TEST(test_interpret_adar_ii_leap);
    RUN_TEST(test_interpret_tishrei_detail);
    RUN_TEST(test_interpret_adar_common);

    /* Purity */
    RUN_TEST(test_purity_month_data);
    RUN_TEST(test_purity_interpret);

    return UNITY_END();
}
