/* test_pawukon_i18n.c — Tests for locale-aware Pawukon interpretation. */

#include "unity.h"
#include "systems/balinese/pawukon_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* wuku=0 (Sinta), day=1, urip=7, no tumpek, no kajeng kliwon */
    pawukon_interp_t r = pwi_interpret_locale(0, 1, 7, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Sin", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sinta"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Love"));
}

static void test_locale_en_matches_base(void)
{
    pawukon_interp_t en = pwi_interpret_locale(0, 1, 7, 0, 0, I18N_LOCALE_EN);
    pawukon_interp_t base = pwi_interpret(0, 1, 7, 0, 0);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    pawukon_interp_t r = pwi_interpret_locale(0, 1, 7, 0, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Wuku name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sinta"));
}

static void test_locale_es_kajeng_kliwon(void)
{
    /* Kajeng Kliwon active */
    pawukon_interp_t r = pwi_interpret_locale(0, 3, 9, 0, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_tumpek(void)
{
    /* Tumpek active */
    pawukon_interp_t r = pwi_interpret_locale(5, 7, 11, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    pawukon_interp_t fr = pwi_interpret_locale(0, 1, 7, 0, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glyph, "Sin"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    pawukon_interp_t r = pwi_interpret_locale(-1, 1, 0, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* 30 wuku (0-29) */
    for (int w = 0; w <= 29; w++) {
        pawukon_interp_t r = pwi_interpret_locale(w, 1, 7, 0, 0,
                                                   I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_kajeng_kliwon);
    RUN_TEST(test_locale_es_tumpek);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
