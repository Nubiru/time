/* test_ethiopian_i18n.c — Tests for locale-aware Ethiopian interpretation. */

#include "unity.h"
#include "systems/ethiopian/ethiopian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    ethiopian_interp_t r = eti_interpret_locale(1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Mes", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Meskerem"));
}

static void test_locale_en_matches_base(void)
{
    ethiopian_interp_t en = eti_interpret_locale(1, 1, 1, I18N_LOCALE_EN);
    ethiopian_interp_t base = eti_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    ethiopian_interp_t r = eti_interpret_locale(1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Meskerem"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    ethiopian_interp_t fr = eti_interpret_locale(1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Meskerem"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    ethiopian_interp_t r = eti_interpret_locale(0, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 1; m <= 13; m++) {
        ethiopian_interp_t r = eti_interpret_locale(m, 1, 0,
                                                    I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Feast path ---- */

static void test_locale_es_feast(void)
{
    ethiopian_interp_t r = eti_interpret_locale(1, 1, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Enkutatash"));
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
    RUN_TEST(test_locale_es_feast);

    return UNITY_END();
}
