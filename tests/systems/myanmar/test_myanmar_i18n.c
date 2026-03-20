/* test_myanmar_i18n.c — Tests for locale-aware Myanmar interpretation. */

#include "unity.h"
#include "systems/myanmar/myanmar_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* month=0 (Tagu), day=1, no thingyan, common year */
    myanmar_interp_t r = mmi_interpret_locale(0, 1, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Tag", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tagu"));
}

static void test_locale_en_matches_base(void)
{
    myanmar_interp_t en = mmi_interpret_locale(0, 1, 1, 0, I18N_LOCALE_EN);
    myanmar_interp_t base = mmi_interpret(0, 1, 1, 0);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    myanmar_interp_t r = mmi_interpret_locale(0, 1, 0, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tagu"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    myanmar_interp_t fr = mmi_interpret_locale(0, 1, 0, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Tagu"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    myanmar_interp_t r = mmi_interpret_locale(-1, 1, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 0; m <= 11; m++) {
        myanmar_interp_t r = mmi_interpret_locale(m, 1, 0, 0,
                                                  I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Thingyan path ---- */

static void test_locale_es_thingyan(void)
{
    myanmar_interp_t r = mmi_interpret_locale(0, 1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thingyan"));
}

/* ---- Year types ---- */

static void test_locale_all_year_types(void)
{
    for (int yt = 0; yt <= 2; yt++) {
        myanmar_interp_t r = mmi_interpret_locale(0, 1, 0, yt,
                                                  I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_thingyan);
    RUN_TEST(test_locale_all_year_types);

    return UNITY_END();
}
