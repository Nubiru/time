/* test_korean_i18n.c — Tests for locale-aware Korean interpretation. */

#include "unity.h"
#include "systems/korean/korean_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    /* animal=0 (Rat), element=0 (Wood), polarity=0 (Yang), festival=0 (Seollal) */
    korean_interp_t r = ki_interpret_locale(4359, 0, 0, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dangun"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Seollal"));
}

static void test_locale_en_matches_base(void)
{
    korean_interp_t en = ki_interpret_locale(4359, 0, 0, 0, 0, I18N_LOCALE_EN);
    korean_interp_t base = ki_interpret(4359, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    korean_interp_t r = ki_interpret_locale(4359, 0, 0, 0, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Dangun year stays numeric */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "4359"));
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    korean_interp_t fr = ki_interpret_locale(4359, 0, 0, 0, 0, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "4359"));
}

/* ---- Invalid input ---- */

static void test_locale_invalid(void)
{
    korean_interp_t r = ki_interpret_locale(4359, -1, 0, 0, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ---- All festivals ---- */

static void test_locale_all_entries(void)
{
    for (int f = 0; f <= 6; f++) {
        korean_interp_t r = ki_interpret_locale(4359, 0, 0, 0, f,
                                                I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- All elements ---- */

static void test_locale_all_elements(void)
{
    for (int e = 0; e <= 4; e++) {
        korean_interp_t r = ki_interpret_locale(4359, 0, e, 0, -1,
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
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_invalid);
    RUN_TEST(test_locale_all_entries);
    RUN_TEST(test_locale_all_elements);

    return UNITY_END();
}
