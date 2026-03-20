/* test_persian_i18n.c — Tests for locale-aware Persian interpretation. */

#include "unity.h"
#include "systems/persian/persian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    persian_interp_t r = pi_interpret_locale(1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Far", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
}

static void test_locale_en_matches_base(void)
{
    persian_interp_t en = pi_interpret_locale(1, 1, 1, I18N_LOCALE_EN);
    persian_interp_t base = pi_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    persian_interp_t r = pi_interpret_locale(1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    persian_interp_t fr = pi_interpret_locale(1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Farvardin"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    persian_interp_t r = pi_interpret_locale(0, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 1; m <= 12; m++) {
        persian_interp_t r = pi_interpret_locale(m, 1, 0, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Festival path ---- */

static void test_locale_es_festival(void)
{
    persian_interp_t r = pi_interpret_locale(1, 1, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nowruz"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_locale_en_first_entry);
    RUN_TEST(test_locale_en_matches_base);
    RUN_TEST(test_locale_es_produces_output);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);
    RUN_TEST(test_locale_es_festival);

    return UNITY_END();
}
