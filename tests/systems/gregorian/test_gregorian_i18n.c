/* test_gregorian_i18n.c — Tests for locale-aware Gregorian interpretation. */

#include "unity.h"
#include "systems/gregorian/gregorian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English (same as gi_interpret) ---- */

static void test_locale_en_january(void)
{
    gregorian_interp_t r = gi_interpret_locale(1, 15, 2, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Jan", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "January"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Wednesday"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Winter"));
}

static void test_locale_en_july(void)
{
    gregorian_interp_t r = gi_interpret_locale(7, 4, 4, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Jul", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "July"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Friday"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Summer"));
}

static void test_locale_en_matches_original(void)
{
    gregorian_interp_t en = gi_interpret_locale(3, 19, 2, I18N_LOCALE_EN);
    gregorian_interp_t orig = gi_interpret(3, 19, 2);
    TEST_ASSERT_EQUAL_STRING(orig.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(orig.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(orig.detail, en.detail);
}

/* ---- Spanish ---- */

static void test_locale_es_enero(void)
{
    gregorian_interp_t r = gi_interpret_locale(1, 15, 2, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Ene", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Enero"));
}

static void test_locale_es_day_name(void)
{
    gregorian_interp_t r = gi_interpret_locale(1, 15, 2, I18N_LOCALE_ES);
    /* day_of_week 2 = Wednesday = Miércoles */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "rcoles"));
}

static void test_locale_es_season(void)
{
    gregorian_interp_t r = gi_interpret_locale(1, 15, 2, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Invierno"));
}

static void test_locale_es_marzo(void)
{
    gregorian_interp_t r = gi_interpret_locale(3, 20, 3, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Mar", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Marzo"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Primavera"));
}

static void test_locale_es_julio(void)
{
    gregorian_interp_t r = gi_interpret_locale(7, 4, 4, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Jul", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Julio"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Verano"));
}

static void test_locale_es_detail_has_origin(void)
{
    gregorian_interp_t r = gi_interpret_locale(1, 1, 0, I18N_LOCALE_ES);
    /* Detail should contain Spanish origin text */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Janus"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Enero"));
}

static void test_locale_es_detail_season_label(void)
{
    gregorian_interp_t r = gi_interpret_locale(6, 21, 5, I18N_LOCALE_ES);
    /* Spanish template uses "Estación" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Verano"));
}

static void test_locale_es_december(void)
{
    gregorian_interp_t r = gi_interpret_locale(12, 25, 3, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Dic", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Diciembre"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Invierno"));
}

/* ---- Fallback (unsupported locale falls back to EN) ---- */

static void test_locale_fallback_to_en(void)
{
    /* Korean has no content translations — should fall back to EN */
    gregorian_interp_t ko = gi_interpret_locale(3, 20, 3, I18N_LOCALE_KO);
    gregorian_interp_t en = gi_interpret_locale(3, 20, 3, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING(en.glyph, ko.glyph);
    TEST_ASSERT_EQUAL_STRING(en.glance, ko.glance);
}

/* ---- Invalid input ---- */

static void test_locale_invalid_month(void)
{
    gregorian_interp_t r = gi_interpret_locale(0, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_EQUAL_STRING("?", r.glance);
}

static void test_locale_invalid_month_13(void)
{
    gregorian_interp_t r = gi_interpret_locale(13, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All months produce output ---- */

static void test_locale_es_all_months(void)
{
    for (int m = 1; m <= 12; m++) {
        gregorian_interp_t r = gi_interpret_locale(m, 1, 0, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_january);
    RUN_TEST(test_locale_en_july);
    RUN_TEST(test_locale_en_matches_original);

    /* Spanish */
    RUN_TEST(test_locale_es_enero);
    RUN_TEST(test_locale_es_day_name);
    RUN_TEST(test_locale_es_season);
    RUN_TEST(test_locale_es_marzo);
    RUN_TEST(test_locale_es_julio);
    RUN_TEST(test_locale_es_detail_has_origin);
    RUN_TEST(test_locale_es_detail_season_label);
    RUN_TEST(test_locale_es_december);

    /* Fallback */
    RUN_TEST(test_locale_fallback_to_en);

    /* Invalid */
    RUN_TEST(test_locale_invalid_month);
    RUN_TEST(test_locale_invalid_month_13);

    /* Coverage */
    RUN_TEST(test_locale_es_all_months);

    return UNITY_END();
}
