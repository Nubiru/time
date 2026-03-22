/* test_islamic_i18n.c -- Tests for locale-aware Islamic interpretation. */

#include "unity.h"
#include "systems/islamic/islamic_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English (same as isi_interpret) ---- */

static void test_locale_en_muharram(void)
{
    hijri_date_t d = { 1446, 1, 10 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Muharram"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "First month"));
}

static void test_locale_en_has_same_content(void)
{
    hijri_date_t d = { 1446, 9, 1 };
    islamic_interp_t en = isi_interpret_locale(d, I18N_LOCALE_EN);
    /* EN locale fast path delegates to isi_interpret -- identical output */
    TEST_ASSERT_EQUAL_STRING("9", en.glyph);
    TEST_ASSERT_NOT_NULL(strstr(en.glance, "Ramadan"));
    TEST_ASSERT_NOT_NULL(strstr(en.glance, "burning heat"));
    TEST_ASSERT_NOT_NULL(strstr(en.detail, "fasting"));
}

static void test_locale_en_sacred_month(void)
{
    hijri_date_t d = { 1446, 1, 1 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_EN);
    /* Muharram is sacred */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sacred months"));
}

static void test_locale_en_event_present(void)
{
    hijri_date_t d = { 1446, 12, 10 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_EN);
    /* Dhul Hijjah has Hajj event */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hajj"));
}

/* ---- Spanish ---- */

static void test_locale_es_muharram_significance(void)
{
    hijri_date_t d = { 1446, 1, 10 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
    /* Spanish significance for Muharram */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Muharram"));
}

static void test_locale_es_brief_translated(void)
{
    hijri_date_t d = { 1446, 1, 10 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
    /* Spanish brief for month 1 contains "luto" (mourning) */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "luto"));
}

static void test_locale_es_ramadan(void)
{
    hijri_date_t d = { 1446, 9, 1 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
    /* Month name stays Arabic-origin */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Ramad"));
}

static void test_locale_es_sacred_translated(void)
{
    hijri_date_t d = { 1446, 7, 1 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
    /* Rajab is sacred -- Spanish text mentions "sagrados" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sagrados"));
}

static void test_locale_es_event_translated(void)
{
    hijri_date_t d = { 1446, 3, 12 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
    /* Rabi al-Awwal event: Mawlid -- Spanish says "nacimiento" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "nacimiento"));
}

static void test_locale_es_dhul_hijjah(void)
{
    hijri_date_t d = { 1446, 12, 10 };
    islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
    /* Dhul Hijjah name preserved */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dhul Hijjah"));
}

/* ---- Fallback ---- */

static void test_locale_fallback_to_en(void)
{
    hijri_date_t d = { 1446, 1, 10 };
    islamic_interp_t ko = isi_interpret_locale(d, I18N_LOCALE_KO);
    /* KO has no content table — falls back to EN */
    TEST_ASSERT_EQUAL_STRING("1", ko.glyph);
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "Muharram"));
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "First month"));
}

/* ---- All months produce output ---- */

static void test_locale_es_all_months(void)
{
    for (int m = 1; m <= 12; m++) {
        hijri_date_t d = { 1446, m, 1 };
        islamic_interp_t r = isi_interpret_locale(d, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_muharram);
    RUN_TEST(test_locale_en_has_same_content);
    RUN_TEST(test_locale_en_sacred_month);
    RUN_TEST(test_locale_en_event_present);

    /* Spanish */
    RUN_TEST(test_locale_es_muharram_significance);
    RUN_TEST(test_locale_es_brief_translated);
    RUN_TEST(test_locale_es_ramadan);
    RUN_TEST(test_locale_es_sacred_translated);
    RUN_TEST(test_locale_es_event_translated);
    RUN_TEST(test_locale_es_dhul_hijjah);

    /* Fallback */
    RUN_TEST(test_locale_fallback_to_en);

    /* Coverage */
    RUN_TEST(test_locale_es_all_months);

    return UNITY_END();
}
