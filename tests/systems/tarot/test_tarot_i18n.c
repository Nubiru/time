/* test_tarot_i18n.c — Tests for locale-aware Tarot interpretation. */

#include "unity.h"
#include "systems/tarot/tarot_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* major=0 (The Fool), no decan */
    tarot_interp_t r = trt_interpret_locale(0, -1, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("0", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Fool"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Potential"));
}

static void test_locale_en_matches_base(void)
{
    tarot_interp_t en = trt_interpret_locale(1, 0, 5, I18N_LOCALE_EN);
    tarot_interp_t base = trt_interpret(1, 0, 5);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    tarot_interp_t r = trt_interpret_locale(0, -1, -1, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Card name preserved as system-native */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Loco"));
}

static void test_locale_es_with_decan(void)
{
    /* The Magus + 5 of Wands */
    tarot_interp_t r = trt_interpret_locale(1, 0, 5, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Bastos"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    tarot_interp_t fr = trt_interpret_locale(0, -1, -1, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Fool"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    tarot_interp_t r = trt_interpret_locale(-1, -1, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All entries ---- */

static void test_locale_all_entries(void)
{
    /* 22 Major Arcana (0-21) */
    for (int m = 0; m <= 21; m++) {
        tarot_interp_t r = trt_interpret_locale(m, -1, -1, I18N_LOCALE_ES);
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
    RUN_TEST(test_locale_es_with_decan);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);

    return UNITY_END();
}
