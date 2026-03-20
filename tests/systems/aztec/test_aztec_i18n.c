/* test_aztec_i18n.c — Tests for locale-aware Aztec interpretation. */

#include "unity.h"
#include "systems/aztec/aztec_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* sign=0 (Cipactli/Crocodile), number=1, bearer=0 (Tochtli), year=1 */
    aztec_interp_t r = azi_interpret_locale(0, 1, 0, 1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Cip", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Crocodile"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rabbit"));
}

static void test_locale_en_matches_base(void)
{
    aztec_interp_t en = azi_interpret_locale(0, 1, 0, 1, I18N_LOCALE_EN);
    aztec_interp_t base = azi_interpret(0, 1, 0, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    aztec_interp_t r = azi_interpret_locale(0, 1, 0, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Nahuatl name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glyph, "Cip"));
}

static void test_locale_es_bearer(void)
{
    /* bearer=1 (Acatl/Reed) */
    aztec_interp_t r = azi_interpret_locale(12, 4, 1, 7, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Acatl"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    aztec_interp_t fr = azi_interpret_locale(0, 1, 0, 1, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glyph, "Cip"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    aztec_interp_t r = azi_interpret_locale(-1, 1, 0, 1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* 20 day signs */
    for (int s = 0; s <= 19; s++) {
        aztec_interp_t r = azi_interpret_locale(s, 1, 0, 1, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
    /* 4 year bearers */
    for (int b = 0; b <= 3; b++) {
        aztec_interp_t r = azi_interpret_locale(0, 1, b, 1, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_locale_en_first_entry);
    RUN_TEST(test_locale_en_matches_base);
    RUN_TEST(test_locale_es_produces_output);
    RUN_TEST(test_locale_es_bearer);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
