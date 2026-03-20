/* test_numerology_i18n.c — Tests for locale-aware Numerology interpretation. */

#include "unity.h"
#include "systems/numerology/numerology_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_path1(void)
{
    numerology_interp_t r = ni_interpret_locale(1, -1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pioneer"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Pioneer"));
}

static void test_locale_en_matches_base(void)
{
    numerology_interp_t en = ni_interpret_locale(7, 3, I18N_LOCALE_EN);
    numerology_interp_t base = ni_interpret(7, 3);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish ---- */

static void test_locale_es_pioneer(void)
{
    /* Number 1 in Spanish: "El Pionero" */
    numerology_interp_t r = ni_interpret_locale(1, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("1", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pionero"));
}

static void test_locale_es_master_11(void)
{
    numerology_interp_t r = ni_interpret_locale(11, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("11", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_master_22(void)
{
    numerology_interp_t r = ni_interpret_locale(22, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("22", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_master_33(void)
{
    numerology_interp_t r = ni_interpret_locale(33, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("33", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_all_numbers(void)
{
    /* All 12 numbers (1-9, 11, 22, 33) produce output in Spanish */
    int nums[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 22, 33 };
    for (int i = 0; i < 12; i++) {
        numerology_interp_t r = ni_interpret_locale(nums[i], -1,
                                                     I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_with_personal_year(void)
{
    numerology_interp_t r = ni_interpret_locale(7, 3, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    /* Detail should include personal year info */
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
}

/* ---- Fallback ---- */

static void test_locale_fr_fallback(void)
{
    numerology_interp_t fr = ni_interpret_locale(1, -1, I18N_LOCALE_FR);
    TEST_ASSERT_EQUAL_STRING("1", fr.glyph);
    TEST_ASSERT_TRUE(strlen(fr.glance) > 0);
    TEST_ASSERT_TRUE(strlen(fr.detail) > 0);
}

/* ---- Invalid ---- */

static void test_locale_invalid_number(void)
{
    numerology_interp_t r = ni_interpret_locale(0, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = ni_interpret_locale(10, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);

    r = ni_interpret_locale(12, -1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_path1);
    RUN_TEST(test_locale_en_matches_base);

    /* Spanish */
    RUN_TEST(test_locale_es_pioneer);
    RUN_TEST(test_locale_es_master_11);
    RUN_TEST(test_locale_es_master_22);
    RUN_TEST(test_locale_es_master_33);
    RUN_TEST(test_locale_es_all_numbers);
    RUN_TEST(test_locale_es_with_personal_year);

    /* Fallback */
    RUN_TEST(test_locale_fr_fallback);

    /* Invalid */
    RUN_TEST(test_locale_invalid_number);

    return UNITY_END();
}
