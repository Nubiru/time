/* test_geology_i18n.c — Tests for locale-aware Geology interpretation. */

#include "unity.h"
#include "systems/geology/geology_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_phanerozoic(void)
{
    geology_interp_t r = gli_interpret_locale(3, -1, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Pha", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Phanerozoic"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Phanerozoic"));
}

static void test_locale_en_matches_base(void)
{
    geology_interp_t en = gli_interpret_locale(3, 4, 5, I18N_LOCALE_EN);
    geology_interp_t base = gli_interpret(3, 4, 5);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish ---- */

static void test_locale_es_phanerozoic(void)
{
    /* Phanerozoic in Spanish: "Fanerozoico" */
    geology_interp_t r = gli_interpret_locale(3, -1, -1, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Fane"));
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_all_eons(void)
{
    for (int e = 0; e < 4; e++) {
        geology_interp_t r = gli_interpret_locale(e, -1, -1, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_with_extinction(void)
{
    geology_interp_t r = gli_interpret_locale(3, 4, -1, I18N_LOCALE_ES);
    /* Detail should include extinction info */
    TEST_ASSERT_TRUE(strlen(r.detail) > 20);
}

static void test_locale_es_with_supercontinent(void)
{
    geology_interp_t r = gli_interpret_locale(3, -1, 5, I18N_LOCALE_ES);
    /* Detail should include supercontinent info */
    TEST_ASSERT_TRUE(strlen(r.detail) > 20);
}

static void test_locale_es_full(void)
{
    /* All three pieces: eon + extinction + supercontinent */
    geology_interp_t r = gli_interpret_locale(3, 2, 5, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 50);
}

/* ---- Fallback ---- */

static void test_locale_fr_fallback(void)
{
    geology_interp_t fr = gli_interpret_locale(3, -1, -1, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(fr.glance) > 0);
    TEST_ASSERT_TRUE(strlen(fr.detail) > 0);
}

/* ---- Invalid ---- */

static void test_locale_invalid_eon(void)
{
    geology_interp_t r = gli_interpret_locale(-1, -1, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = gli_interpret_locale(4, -1, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_phanerozoic);
    RUN_TEST(test_locale_en_matches_base);

    /* Spanish */
    RUN_TEST(test_locale_es_phanerozoic);
    RUN_TEST(test_locale_es_all_eons);
    RUN_TEST(test_locale_es_with_extinction);
    RUN_TEST(test_locale_es_with_supercontinent);
    RUN_TEST(test_locale_es_full);

    /* Fallback */
    RUN_TEST(test_locale_fr_fallback);

    /* Invalid */
    RUN_TEST(test_locale_invalid_eon);

    return UNITY_END();
}
