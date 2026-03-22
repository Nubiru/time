/* test_chinese_i18n.c — Tests for locale-aware Chinese interpretation. */

#include "unity.h"
#include "systems/chinese/chinese_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English (same as ci_interpret) ---- */

static void test_locale_en_rat(void)
{
    chinese_interp_t r = ci_interpret_locale(0, 4, 0, 1,
                                             I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Rat", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rat"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Water"));
}

static void test_locale_en_has_same_content(void)
{
    chinese_interp_t en = ci_interpret_locale(2, 0, 0, 3,
                                              I18N_LOCALE_EN);
    chinese_interp_t plain = ci_interpret(2, 0, 0, 3);
    /* EN locale fast path delegates to ci_interpret — identical output */
    TEST_ASSERT_EQUAL_STRING(plain.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(plain.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(plain.detail, en.detail);
}

static void test_locale_en_detail_content(void)
{
    chinese_interp_t r = ci_interpret_locale(4, 1, 0, 5,
                                             I18N_LOCALE_EN);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Fire"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Year 5 of 60"));
}

/* ---- Spanish ---- */

static void test_locale_es_rata(void)
{
    chinese_interp_t r = ci_interpret_locale(0, 4, 0, 1,
                                             I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Rat", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rata"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Agua"));
}

static void test_locale_es_tigre(void)
{
    chinese_interp_t r = ci_interpret_locale(2, 0, 0, 3,
                                             I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Tig", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tigre"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Madera"));
}

static void test_locale_es_dragon(void)
{
    chinese_interp_t r = ci_interpret_locale(4, 1, 0, 5,
                                             I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Fuego"));
}

static void test_locale_es_element_name(void)
{
    chinese_interp_t r = ci_interpret_locale(1, 3, 1, 10,
                                             I18N_LOCALE_ES);
    /* Metal -> Metal in Spanish, Buey for Ox */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Metal"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Buey"));
}

static void test_locale_es_polarity_yin(void)
{
    chinese_interp_t r = ci_interpret_locale(3, 2, 1, 20,
                                             I18N_LOCALE_ES);
    /* Yin stays as Yin in Spanish */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yin"));
}

static void test_locale_es_polarity_yang(void)
{
    chinese_interp_t r = ci_interpret_locale(0, 0, 0, 1,
                                             I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yang"));
}

static void test_locale_es_detail_strengths(void)
{
    chinese_interp_t r = ci_interpret_locale(0, 4, 0, 1,
                                             I18N_LOCALE_ES);
    /* Spanish strengths for Rat */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ingenioso"));
}

/* ---- Fallback ---- */

static void test_locale_fallback_to_en(void)
{
    chinese_interp_t ko = ci_interpret_locale(0, 4, 0, 1,
                                              I18N_LOCALE_KO);
    /* KO has no content table — falls back to EN */
    TEST_ASSERT_EQUAL_STRING("Rat", ko.glyph);
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "Rat"));
    TEST_ASSERT_NOT_NULL(strstr(ko.glance, "Water"));
}

static void test_locale_invalid_animal(void)
{
    chinese_interp_t r = ci_interpret_locale(-1, 0, 0, 1,
                                             I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_EQUAL_STRING("?", r.glance);
    TEST_ASSERT_EQUAL_STRING("?", r.detail);
}

static void test_locale_invalid_element(void)
{
    chinese_interp_t r = ci_interpret_locale(0, 5, 0, 1,
                                             I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_EQUAL_STRING("?", r.glance);
    TEST_ASSERT_EQUAL_STRING("?", r.detail);
}

/* ---- All 12 animals produce output ---- */

static void test_locale_es_all_animals(void)
{
    for (int a = 0; a < 12; a++) {
        chinese_interp_t r = ci_interpret_locale(a, a % 5, a % 2,
                                                 a + 1,
                                                 I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_en_all_animals(void)
{
    for (int a = 0; a < 12; a++) {
        chinese_interp_t r = ci_interpret_locale(a, a % 5, a % 2,
                                                 a + 1,
                                                 I18N_LOCALE_EN);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_rat);
    RUN_TEST(test_locale_en_has_same_content);
    RUN_TEST(test_locale_en_detail_content);

    /* Spanish */
    RUN_TEST(test_locale_es_rata);
    RUN_TEST(test_locale_es_tigre);
    RUN_TEST(test_locale_es_dragon);
    RUN_TEST(test_locale_es_element_name);
    RUN_TEST(test_locale_es_polarity_yin);
    RUN_TEST(test_locale_es_polarity_yang);
    RUN_TEST(test_locale_es_detail_strengths);

    /* Fallback */
    RUN_TEST(test_locale_fallback_to_en);
    RUN_TEST(test_locale_invalid_animal);
    RUN_TEST(test_locale_invalid_element);

    /* Coverage */
    RUN_TEST(test_locale_es_all_animals);
    RUN_TEST(test_locale_en_all_animals);

    return UNITY_END();
}
