/* test_tamil_i18n.c — Tests for locale-aware Tamil interpretation. */

#include "unity.h"
#include "systems/tamil/tamil_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    tamil_interp_t r = tmi_interpret_locale(1, 1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Chi", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Chittirai"));
}

static void test_locale_en_matches_base(void)
{
    tamil_interp_t en = tmi_interpret_locale(1, 1, 1, 1, I18N_LOCALE_EN);
    tamil_interp_t base = tmi_interpret(1, 1, 1, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    tamil_interp_t r = tmi_interpret_locale(1, 1, 1, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name stays system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Chittirai"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    tamil_interp_t fr = tmi_interpret_locale(1, 1, 1, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Chittirai"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    tamil_interp_t r = tmi_interpret_locale(0, 1, 1, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    for (int m = 1; m <= 12; m++) {
        tamil_interp_t r = tmi_interpret_locale(m, 1, 1, 0,
                                                I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Festival path ---- */

static void test_locale_es_festival(void)
{
    tamil_interp_t r = tmi_interpret_locale(1, 1, 1, 1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Puthandu"));
}

/* ---- All festivals ---- */

static void test_locale_all_festivals(void)
{
    for (int f = 1; f <= 4; f++) {
        tamil_interp_t r = tmi_interpret_locale(1, 1, 1, f,
                                                I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_festival);
    RUN_TEST(test_locale_all_festivals);

    return UNITY_END();
}
