/* test_french_republican_i18n.c — Tests for locale-aware French Republican
 * interpretation. */

#include "unity.h"
#include "systems/french_republican/french_republican_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* rep_year=234, month=1 (Vendemiaire), day=1 */
    french_republican_interp_t r = fri_interpret_locale(234, 1, 1,
                                                        I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Ven", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Vendemiaire"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Grape Harvest"));
}

static void test_locale_en_matches_base(void)
{
    french_republican_interp_t en = fri_interpret_locale(234, 1, 1,
                                                         I18N_LOCALE_EN);
    french_republican_interp_t base = fri_interpret(234, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    french_republican_interp_t r = fri_interpret_locale(234, 1, 1,
                                                        I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* French month name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glyph, "Ven"));
}

static void test_locale_es_sansculottide(void)
{
    /* month=0 (Sansculottides), day=1 (Virtue) */
    french_republican_interp_t r = fri_interpret_locale(234, 0, 1,
                                                        I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("San", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Vert"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    french_republican_interp_t fr = fri_interpret_locale(234, 1, 1,
                                                         I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glyph, "Ven"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    french_republican_interp_t r = fri_interpret_locale(234, 15, 1,
                                                        I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* 12 regular months */
    for (int m = 1; m <= 12; m++) {
        french_republican_interp_t r = fri_interpret_locale(234, m, 1,
                                                            I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
    /* 6 Sansculottide days */
    for (int d = 1; d <= 6; d++) {
        french_republican_interp_t r = fri_interpret_locale(234, 0, d,
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
    RUN_TEST(test_locale_es_sansculottide);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
