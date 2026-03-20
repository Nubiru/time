/* test_japanese_i18n.c — Tests for locale-aware Japanese interpretation. */

#include "unity.h"
#include "systems/japanese/japanese_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* era=4 (Reiwa), year=8, rokuyo=4 (Taian), sekki=0 (Shunbun) */
    japanese_interp_t r = ji_interpret_locale(4, 8, 4, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Rei", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Reiwa"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Shunbun"));
}

static void test_locale_en_matches_base(void)
{
    japanese_interp_t en = ji_interpret_locale(4, 8, 4, 0, I18N_LOCALE_EN);
    japanese_interp_t base = ji_interpret(4, 8, 4, 0);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    japanese_interp_t r = ji_interpret_locale(4, 8, 4, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Era name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Reiwa"));
}

static void test_locale_es_rokuyo(void)
{
    /* Butsumetsu (rokuyo=3) — most inauspicious */
    japanese_interp_t r = ji_interpret_locale(4, 8, 3, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Butsumetsu"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    japanese_interp_t fr = ji_interpret_locale(4, 8, 4, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Reiwa"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    japanese_interp_t r = ji_interpret_locale(-1, 1, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* 5 eras */
    for (int e = 0; e <= 4; e++) {
        japanese_interp_t r = ji_interpret_locale(e, 1, 0, 0, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
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
    RUN_TEST(test_locale_es_rokuyo);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
