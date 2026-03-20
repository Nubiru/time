/* test_bahai_i18n.c — Tests for locale-aware Baha'i interpretation. */

#include "unity.h"
#include "systems/bahai/bahai_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* month=1 (Baha/Splendour), day=1, no holy day */
    bahai_interp_t r = bhi_interpret_locale(1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Bah", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Baha"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Splendour"));
}

static void test_locale_en_matches_base(void)
{
    bahai_interp_t en = bhi_interpret_locale(1, 1, 1, I18N_LOCALE_EN);
    bahai_interp_t base = bhi_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    bahai_interp_t r = bhi_interpret_locale(1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Arabic month name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Baha"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    bahai_interp_t fr = bhi_interpret_locale(1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Baha"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    bahai_interp_t r = bhi_interpret_locale(-1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries (0=Ayyam-i-Ha, 1-19=regular months) ---- */

static void test_locale_all_entries(void)
{
    for (int m = 0; m <= 19; m++) {
        bahai_interp_t r = bhi_interpret_locale(m, 1, 0, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Holy day path ---- */

static void test_locale_es_holy_day(void)
{
    bahai_interp_t r = bhi_interpret_locale(1, 1, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Naw-Ruz"));
}

/* ---- All holy days ---- */

static void test_locale_all_holy_days(void)
{
    for (int h = 1; h <= 8; h++) {
        bahai_interp_t r = bhi_interpret_locale(1, 1, h, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
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
    RUN_TEST(test_locale_es_holy_day);
    RUN_TEST(test_locale_all_holy_days);

    return UNITY_END();
}
