/* test_zoroastrian_i18n.c — Tests for locale-aware Zoroastrian interpretation. */

#include "unity.h"
#include "systems/zoroastrian/zoroastrian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* month=1 (Farvardin), day=1 (Hormozd), festival=1 (Nowruz) */
    zoroastrian_interp_t r = zi_interpret_locale(1, 1, 1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Far", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nowruz"));
}

static void test_locale_en_matches_base(void)
{
    zoroastrian_interp_t en = zi_interpret_locale(1, 1, 1, I18N_LOCALE_EN);
    zoroastrian_interp_t base = zi_interpret(1, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    zoroastrian_interp_t r = zi_interpret_locale(1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Farvardin"));
}

static void test_locale_es_gatha(void)
{
    /* month=13, day=1 (Ahunavad Gatha) */
    zoroastrian_interp_t r = zi_interpret_locale(13, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Gth", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    zoroastrian_interp_t fr = zi_interpret_locale(1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Farvardin"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    zoroastrian_interp_t r = zi_interpret_locale(0, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* months 1-12 */
    for (int m = 1; m <= 12; m++) {
        zoroastrian_interp_t r = zi_interpret_locale(m, 1, 0, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
    /* Gatha days 1-5 */
    for (int d = 1; d <= 5; d++) {
        zoroastrian_interp_t r = zi_interpret_locale(13, d, 0, I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_gatha);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
