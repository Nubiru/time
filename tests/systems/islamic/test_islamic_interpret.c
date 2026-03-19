/* test_islamic_interpret.c -- Islamic month & prayer interpretation data + tiered text
 * TDD RED phase: tests written before implementation.
 * 12 months (4 sacred), 5 daily prayers, tiered interpretation.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/islamic/islamic_interpret.h"
#include "../../../src/systems/islamic/hijri.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== isi_month_count / isi_prayer_count ===== */

void test_month_count_returns_12(void)
{
    TEST_ASSERT_EQUAL_INT(12, isi_month_count());
}

void test_prayer_count_returns_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, isi_prayer_count());
}

/* ===== isi_month_data -- specific months ===== */

void test_month_data_muharram(void)
{
    isi_month_t m = isi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Muharram", m.name);
    TEST_ASSERT_EQUAL_INT(1, m.is_sacred);
    TEST_ASSERT_TRUE(strlen(m.significance) > 0);
    TEST_ASSERT_TRUE(strlen(m.event) > 0);  /* Ashura */
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_rajab(void)
{
    isi_month_t m = isi_month_data(7);
    TEST_ASSERT_EQUAL_INT(7, m.month);
    TEST_ASSERT_EQUAL_STRING("Rajab", m.name);
    TEST_ASSERT_EQUAL_INT(1, m.is_sacred);
    TEST_ASSERT_TRUE(strstr(m.significance, "respect") != NULL);
    TEST_ASSERT_TRUE(strlen(m.event) > 0);  /* Isra and Mi'raj */
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_ramadan(void)
{
    isi_month_t m = isi_month_data(9);
    TEST_ASSERT_EQUAL_INT(9, m.month);
    TEST_ASSERT_EQUAL_STRING("Ramadan", m.name);
    TEST_ASSERT_EQUAL_INT(0, m.is_sacred);
    TEST_ASSERT_TRUE(strstr(m.significance, "fasting") != NULL);
    TEST_ASSERT_TRUE(strlen(m.event) > 0);  /* Sawm, Laylat al-Qadr */
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

void test_month_data_dhul_hijjah(void)
{
    isi_month_t m = isi_month_data(12);
    TEST_ASSERT_EQUAL_INT(12, m.month);
    TEST_ASSERT_EQUAL_STRING("Dhul Hijjah", m.name);
    TEST_ASSERT_EQUAL_INT(1, m.is_sacred);
    TEST_ASSERT_TRUE(strstr(m.significance, "pilgrimage") != NULL);
    TEST_ASSERT_TRUE(strlen(m.event) > 0);  /* Hajj, Eid al-Adha */
    TEST_ASSERT_TRUE(strlen(m.brief) > 0);
}

/* ===== isi_month_data -- all 12 months have non-empty strings ===== */

void test_month_data_all_strings_nonempty(void)
{
    int i;
    for (i = 1; i <= 12; i++) {
        isi_month_t m = isi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_TRUE(strlen(m.name) > 0);
        TEST_ASSERT_TRUE(strlen(m.significance) > 0);
        /* event may be "" for months with no specific event */
        TEST_ASSERT_NOT_NULL(m.event);
        TEST_ASSERT_TRUE(strlen(m.brief) > 0);
    }
}

/* ===== isi_month_data -- sacred months ===== */

void test_sacred_months_muharram(void)
{
    TEST_ASSERT_EQUAL_INT(1, isi_month_data(1).is_sacred);
}

void test_sacred_months_rajab(void)
{
    TEST_ASSERT_EQUAL_INT(1, isi_month_data(7).is_sacred);
}

void test_sacred_months_dhul_qadah(void)
{
    TEST_ASSERT_EQUAL_INT(1, isi_month_data(11).is_sacred);
}

void test_sacred_months_dhul_hijjah(void)
{
    TEST_ASSERT_EQUAL_INT(1, isi_month_data(12).is_sacred);
}

/* ===== isi_month_data -- non-sacred months ===== */

void test_nonsacred_safar(void)
{
    TEST_ASSERT_EQUAL_INT(0, isi_month_data(2).is_sacred);
}

void test_nonsacred_rabi_al_awwal(void)
{
    TEST_ASSERT_EQUAL_INT(0, isi_month_data(3).is_sacred);
}

void test_nonsacred_ramadan(void)
{
    TEST_ASSERT_EQUAL_INT(0, isi_month_data(9).is_sacred);
}

/* ===== isi_month_data -- invalid months ===== */

void test_month_data_invalid_zero(void)
{
    isi_month_t m = isi_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
    TEST_ASSERT_EQUAL_STRING("?", m.significance);
    TEST_ASSERT_EQUAL_STRING("?", m.event);
    TEST_ASSERT_EQUAL_STRING("?", m.brief);
}

void test_month_data_invalid_13(void)
{
    isi_month_t m = isi_month_data(13);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

void test_month_data_invalid_negative(void)
{
    isi_month_t m = isi_month_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.name);
}

/* ===== isi_prayer_data -- all 5 prayers ===== */

void test_prayer_fajr(void)
{
    isi_prayer_t p = isi_prayer_data(0);
    TEST_ASSERT_EQUAL_INT(0, p.index);
    TEST_ASSERT_EQUAL_STRING("Fajr", p.name);
    TEST_ASSERT_TRUE(strlen(p.arabic) > 0);
    TEST_ASSERT_TRUE(strlen(p.time_desc) > 0);
    TEST_ASSERT_TRUE(strlen(p.meaning) > 0);
}

void test_prayer_dhuhr(void)
{
    isi_prayer_t p = isi_prayer_data(1);
    TEST_ASSERT_EQUAL_INT(1, p.index);
    TEST_ASSERT_EQUAL_STRING("Dhuhr", p.name);
    TEST_ASSERT_TRUE(strlen(p.meaning) > 0);
}

void test_prayer_asr(void)
{
    isi_prayer_t p = isi_prayer_data(2);
    TEST_ASSERT_EQUAL_INT(2, p.index);
    TEST_ASSERT_EQUAL_STRING("Asr", p.name);
    TEST_ASSERT_TRUE(strlen(p.meaning) > 0);
}

void test_prayer_maghrib(void)
{
    isi_prayer_t p = isi_prayer_data(3);
    TEST_ASSERT_EQUAL_INT(3, p.index);
    TEST_ASSERT_EQUAL_STRING("Maghrib", p.name);
    TEST_ASSERT_TRUE(strlen(p.meaning) > 0);
}

void test_prayer_isha(void)
{
    isi_prayer_t p = isi_prayer_data(4);
    TEST_ASSERT_EQUAL_INT(4, p.index);
    TEST_ASSERT_EQUAL_STRING("Isha", p.name);
    TEST_ASSERT_TRUE(strlen(p.meaning) > 0);
}

void test_prayer_all_strings_nonempty(void)
{
    int i;
    for (i = 0; i < 5; i++) {
        isi_prayer_t p = isi_prayer_data(i);
        TEST_ASSERT_EQUAL_INT(i, p.index);
        TEST_ASSERT_TRUE(strlen(p.name) > 0);
        TEST_ASSERT_TRUE(strlen(p.arabic) > 0);
        TEST_ASSERT_TRUE(strlen(p.time_desc) > 0);
        TEST_ASSERT_TRUE(strlen(p.meaning) > 0);
    }
}

/* ===== isi_prayer_data -- invalid ===== */

void test_prayer_invalid_5(void)
{
    isi_prayer_t p = isi_prayer_data(5);
    TEST_ASSERT_EQUAL_INT(-1, p.index);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
    TEST_ASSERT_EQUAL_STRING("?", p.arabic);
    TEST_ASSERT_EQUAL_STRING("?", p.time_desc);
    TEST_ASSERT_EQUAL_STRING("?", p.meaning);
}

void test_prayer_invalid_negative(void)
{
    isi_prayer_t p = isi_prayer_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, p.index);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
}

/* ===== isi_interpret -- glyph ===== */

void test_interpret_glyph_nonempty(void)
{
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
}

void test_interpret_glyph_is_month_number(void)
{
    hijri_date_t d = { .year = 1447, .month = 9, .day = 1 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_EQUAL_STRING("9", r.glyph);
}

void test_interpret_glyph_month_12(void)
{
    hijri_date_t d = { .year = 1447, .month = 12, .day = 10 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_EQUAL_STRING("12", r.glyph);
}

/* ===== isi_interpret -- glance ===== */

void test_interpret_glance_contains_year(void)
{
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "1447"));
}

void test_interpret_glance_contains_month_name(void)
{
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rajab"));
}

void test_interpret_glance_contains_significance(void)
{
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "respect"));
}

void test_interpret_glance_contains_day(void)
{
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "3"));
}

/* ===== isi_interpret -- detail ===== */

void test_interpret_detail_contains_brief(void)
{
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    /* Rajab brief mentions "spiritual ascent" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "spiritual ascent"));
}

void test_interpret_detail_sacred_month_mentioned(void)
{
    /* Rajab is sacred -- detail should mention "sacred" */
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sacred"));
}

void test_interpret_detail_nonsacred_no_sacred_text(void)
{
    /* Safar is not sacred -- detail should NOT mention "sacred" */
    hijri_date_t d = { .year = 1447, .month = 2, .day = 10 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NULL(strstr(r.detail, "sacred"));
}

void test_interpret_detail_event_included_when_present(void)
{
    /* Rajab has event: Isra and Mi'raj */
    hijri_date_t d = { .year = 1447, .month = 7, .day = 3 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Isra"));
}

void test_interpret_ramadan_detail_mentions_fasting(void)
{
    hijri_date_t d = { .year = 1447, .month = 9, .day = 1 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "fasting"));
}

void test_interpret_dhul_hijjah_detail_mentions_hajj(void)
{
    hijri_date_t d = { .year = 1447, .month = 12, .day = 10 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hajj"));
}

void test_interpret_dhul_hijjah_detail_mentions_sacred(void)
{
    hijri_date_t d = { .year = 1447, .month = 12, .day = 10 };
    islamic_interp_t r = isi_interpret(d);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sacred"));
}

/* ===== Purity tests ===== */

void test_purity_month_data(void)
{
    isi_month_t a = isi_month_data(7);
    isi_month_t b = isi_month_data(7);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_INT(a.is_sacred, b.is_sacred);
    TEST_ASSERT_EQUAL_STRING(a.significance, b.significance);
    TEST_ASSERT_EQUAL_STRING(a.event, b.event);
    TEST_ASSERT_EQUAL_STRING(a.brief, b.brief);
}

void test_purity_prayer_data(void)
{
    isi_prayer_t a = isi_prayer_data(2);
    isi_prayer_t b = isi_prayer_data(2);
    TEST_ASSERT_EQUAL_INT(a.index, b.index);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.arabic, b.arabic);
    TEST_ASSERT_EQUAL_STRING(a.time_desc, b.time_desc);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
}

void test_purity_interpret(void)
{
    hijri_date_t d = { .year = 1447, .month = 9, .day = 27 };
    islamic_interp_t a = isi_interpret(d);
    islamic_interp_t b = isi_interpret(d);
    TEST_ASSERT_EQUAL_STRING(a.glyph, b.glyph);
    TEST_ASSERT_EQUAL_STRING(a.glance, b.glance);
    TEST_ASSERT_EQUAL_STRING(a.detail, b.detail);
}

int main(void)
{
    UNITY_BEGIN();

    /* counts */
    RUN_TEST(test_month_count_returns_12);
    RUN_TEST(test_prayer_count_returns_5);

    /* isi_month_data -- specific months */
    RUN_TEST(test_month_data_muharram);
    RUN_TEST(test_month_data_rajab);
    RUN_TEST(test_month_data_ramadan);
    RUN_TEST(test_month_data_dhul_hijjah);

    /* isi_month_data -- bulk */
    RUN_TEST(test_month_data_all_strings_nonempty);

    /* isi_month_data -- sacred */
    RUN_TEST(test_sacred_months_muharram);
    RUN_TEST(test_sacred_months_rajab);
    RUN_TEST(test_sacred_months_dhul_qadah);
    RUN_TEST(test_sacred_months_dhul_hijjah);

    /* isi_month_data -- non-sacred */
    RUN_TEST(test_nonsacred_safar);
    RUN_TEST(test_nonsacred_rabi_al_awwal);
    RUN_TEST(test_nonsacred_ramadan);

    /* isi_month_data -- invalid */
    RUN_TEST(test_month_data_invalid_zero);
    RUN_TEST(test_month_data_invalid_13);
    RUN_TEST(test_month_data_invalid_negative);

    /* isi_prayer_data -- all 5 */
    RUN_TEST(test_prayer_fajr);
    RUN_TEST(test_prayer_dhuhr);
    RUN_TEST(test_prayer_asr);
    RUN_TEST(test_prayer_maghrib);
    RUN_TEST(test_prayer_isha);
    RUN_TEST(test_prayer_all_strings_nonempty);

    /* isi_prayer_data -- invalid */
    RUN_TEST(test_prayer_invalid_5);
    RUN_TEST(test_prayer_invalid_negative);

    /* isi_interpret -- glyph */
    RUN_TEST(test_interpret_glyph_nonempty);
    RUN_TEST(test_interpret_glyph_is_month_number);
    RUN_TEST(test_interpret_glyph_month_12);

    /* isi_interpret -- glance */
    RUN_TEST(test_interpret_glance_contains_year);
    RUN_TEST(test_interpret_glance_contains_month_name);
    RUN_TEST(test_interpret_glance_contains_significance);
    RUN_TEST(test_interpret_glance_contains_day);

    /* isi_interpret -- detail */
    RUN_TEST(test_interpret_detail_contains_brief);
    RUN_TEST(test_interpret_detail_sacred_month_mentioned);
    RUN_TEST(test_interpret_detail_nonsacred_no_sacred_text);
    RUN_TEST(test_interpret_detail_event_included_when_present);
    RUN_TEST(test_interpret_ramadan_detail_mentions_fasting);
    RUN_TEST(test_interpret_dhul_hijjah_detail_mentions_hajj);
    RUN_TEST(test_interpret_dhul_hijjah_detail_mentions_sacred);

    /* purity */
    RUN_TEST(test_purity_month_data);
    RUN_TEST(test_purity_prayer_data);
    RUN_TEST(test_purity_interpret);

    return UNITY_END();
}
