/* test_celtic_i18n.c — Tests for locale-aware Celtic interpretation. */

#include "unity.h"
#include "systems/celtic/celtic_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* tree_month=1 (Birch/Beith), day=1, no festival */
    celtic_interp_t r = cti_interpret_locale(1, 1, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Beith", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Birch"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pioneer"));
}

static void test_locale_en_matches_base(void)
{
    celtic_interp_t en = cti_interpret_locale(7, 15, 5, I18N_LOCALE_EN);
    celtic_interp_t base = cti_interpret(7, 15, 5);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    celtic_interp_t r = cti_interpret_locale(1, 1, -1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Tree name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Birch"));
}

static void test_locale_es_festival(void)
{
    /* Samhain (festival 0) */
    celtic_interp_t r = cti_interpret_locale(13, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Samhain"));
}

/* ---- Unsupported locale fallback ---- */

static void test_locale_unsupported_fallback(void)
{
    celtic_interp_t ko = cti_interpret_locale(1, 1, -1, I18N_LOCALE_KO);
    TEST_ASSERT_TRUE(strlen(ko.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "Birch"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    celtic_interp_t r = cti_interpret_locale(-1, 1, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* 0=Yew intercalary, 1-13=months */
    for (int m = 0; m <= 13; m++) {
        celtic_interp_t r = cti_interpret_locale(m, 1, -1, I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_festival);
    RUN_TEST(test_locale_unsupported_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
