/* test_tzolkin_i18n.c — Tests for locale-aware Tzolkin interpretation. */

#include "unity.h"
#include "systems/tzolkin/tzolkin_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_kin1(void)
{
    tzolkin_interp_t r = ti_interpret_locale(1, 0, 1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("KIN 1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Dragon"));
}

static void test_locale_en_matches_base(void)
{
    tzolkin_interp_t en = ti_interpret_locale(207, 6, 12, I18N_LOCALE_EN);
    tzolkin_interp_t base = ti_interpret(207, 6, 12);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish ---- */

static void test_locale_es_dragon(void)
{
    /* Seal 0 = Dragon, Spanish name should be "Dragon" or "Drag\xc3\xb3n" */
    tzolkin_interp_t r = ti_interpret_locale(1, 0, 1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("KIN 1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Drag"));
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_all_seals(void)
{
    /* All 20 seals produce non-empty output in Spanish */
    for (int s = 0; s < 20; s++) {
        int kin = s + 1;  /* kin 1-20 covers all seals at tone 1 */
        tzolkin_interp_t r = ti_interpret_locale(kin, s, 1, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    /* FR falls back to EN content */
    tzolkin_interp_t fr = ti_interpret_locale(1, 0, 1, I18N_LOCALE_FR);
    TEST_ASSERT_EQUAL_STRING("KIN 1", fr.glyph);
    TEST_ASSERT_TRUE(strlen(fr.glance) > 0);
    TEST_ASSERT_TRUE(strlen(fr.detail) > 0);
}

/* ---- Invalid input ---- */

static void test_locale_invalid_kin(void)
{
    tzolkin_interp_t r = ti_interpret_locale(0, 0, 1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = ti_interpret_locale(261, 0, 1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

static void test_locale_invalid_seal(void)
{
    tzolkin_interp_t r = ti_interpret_locale(1, -1, 1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = ti_interpret_locale(1, 20, 1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

static void test_locale_invalid_tone(void)
{
    tzolkin_interp_t r = ti_interpret_locale(1, 0, 0, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = ti_interpret_locale(1, 0, 14, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

int main(void)
{
    UNITY_BEGIN();

    /* English fast path */
    RUN_TEST(test_locale_en_kin1);
    RUN_TEST(test_locale_en_matches_base);

    /* Spanish */
    RUN_TEST(test_locale_es_dragon);
    RUN_TEST(test_locale_es_all_seals);

    /* Fallback */
    RUN_TEST(test_locale_fr_fallback);

    /* Invalid */
    RUN_TEST(test_locale_invalid_kin);
    RUN_TEST(test_locale_invalid_seal);
    RUN_TEST(test_locale_invalid_tone);

    return UNITY_END();
}
