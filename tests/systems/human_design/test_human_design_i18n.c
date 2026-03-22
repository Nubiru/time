/* test_human_design_i18n.c — Tests for locale-aware Human Design interpretation. */

#include "unity.h"
#include "systems/human_design/human_design_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void test_locale_en_generator(void)
{
    human_design_interp_t r = hdi_interpret_locale(1, 1, 1, 3, 1, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Gen", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Generator"));
}

static void test_locale_en_same_as_base(void)
{
    human_design_interp_t en = hdi_interpret_locale(0, 0, 1, 3, 1, I18N_LOCALE_EN);
    human_design_interp_t base = hdi_interpret(0, 0, 1, 3, 1);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

static void test_locale_es_manifestor(void)
{
    human_design_interp_t r = hdi_interpret_locale(0, 0, 1, 3, 1, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Manifestador"));
}

static void test_locale_es_generator(void)
{
    human_design_interp_t r = hdi_interpret_locale(1, 1, 1, 3, 1, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Generador"));
}

static void test_locale_fallback_to_en(void)
{
    human_design_interp_t my_mm = hdi_interpret_locale(1, 1, 1, 3, 1, I18N_LOCALE_MY);
    TEST_ASSERT_EQUAL_STRING("Gen", my_mm.glyph);
    TEST_ASSERT_NOT_NULL(strstr(my_mm.glance, "Generator"));
}

static void test_locale_invalid_type(void)
{
    human_design_interp_t r = hdi_interpret_locale(-1, 0, 1, 3, 1, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

static void test_locale_es_all_types(void)
{
    for (int t = 0; t <= 4; t++) {
        human_design_interp_t r = hdi_interpret_locale(t, 0, 1, 3, 1, I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_locale_en_generator);
    RUN_TEST(test_locale_en_same_as_base);
    RUN_TEST(test_locale_es_manifestor);
    RUN_TEST(test_locale_es_generator);
    RUN_TEST(test_locale_fallback_to_en);
    RUN_TEST(test_locale_invalid_type);
    RUN_TEST(test_locale_es_all_types);
    return UNITY_END();
}
