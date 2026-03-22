/* test_egyptian_i18n.c — Tests for locale-aware Egyptian interpretation. */

#include "unity.h"
#include "systems/egyptian/egyptian_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* month=1 (Thoth), day=14, sothic=500 */
    egyptian_interp_t r = ei_interpret_locale(1, 14, 500, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Tho", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thoth"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Akhet"));
}

static void test_locale_en_matches_base(void)
{
    egyptian_interp_t en = ei_interpret_locale(1, 14, 500, I18N_LOCALE_EN);
    egyptian_interp_t base = ei_interpret(1, 14, 500);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    egyptian_interp_t r = ei_interpret_locale(1, 14, 500, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Month name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Thoth"));
}

static void test_locale_es_epagomenal(void)
{
    /* month=13, day=1 (Osiris) */
    egyptian_interp_t r = ei_interpret_locale(13, 1, 500, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Epg", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Osiris"));
}

/* ---- Unsupported locale fallback ---- */

static void test_locale_unsupported_fallback(void)
{
    egyptian_interp_t ru = ei_interpret_locale(1, 14, 500, I18N_LOCALE_RU);
    TEST_ASSERT_TRUE(strlen(ru.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(ru.glance, "Thoth"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    egyptian_interp_t r = ei_interpret_locale(0, 1, 500, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* months 1-12 */
    for (int m = 1; m <= 12; m++) {
        egyptian_interp_t r = ei_interpret_locale(m, 1, 500, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
    /* epagomenal days 1-5 */
    for (int d = 1; d <= 5; d++) {
        egyptian_interp_t r = ei_interpret_locale(13, d, 500, I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_epagomenal);
    RUN_TEST(test_locale_unsupported_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
