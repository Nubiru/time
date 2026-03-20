/* test_convergence_i18n.c — Tests for locale-aware convergence
 * interpretation. */

#include "unity.h"
#include "systems/unified/convergence_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English fast path ---- */

static void test_locale_en_first_entry(void)
{
    cd_system_t sys[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC, CD_SYS_CHINESE };
    convergence_interp_t r = cvi_interpret_locale(CD_STRENGTH_NOTABLE,
                                                   sys, 3, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("**", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Notable"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "3"));
}

static void test_locale_en_matches_base(void)
{
    cd_system_t sys[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC };
    convergence_interp_t en = cvi_interpret_locale(CD_STRENGTH_MINOR,
                                                    sys, 2, I18N_LOCALE_EN);
    convergence_interp_t base = cvi_interpret(CD_STRENGTH_MINOR, sys, 2);
    TEST_ASSERT_EQUAL_STRING(base.glyph, en.glyph);
    TEST_ASSERT_EQUAL_STRING(base.glance, en.glance);
    TEST_ASSERT_EQUAL_STRING(base.detail, en.detail);
}

/* ---- Spanish (ES = EN fallback for now) ---- */

static void test_locale_es_produces_output(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_CELTIC };
    convergence_interp_t r = cvi_interpret_locale(CD_STRENGTH_MINOR,
                                                   sys, 2, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

static void test_locale_es_null_systems(void)
{
    convergence_interp_t r = cvi_interpret_locale(CD_STRENGTH_MINOR,
                                                   NULL, 0, I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
}

/* ---- French fallback ---- */

static void test_locale_fr_fallback(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_CELTIC };
    convergence_interp_t fr = cvi_interpret_locale(CD_STRENGTH_MINOR,
                                                    sys, 2, I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strlen(fr.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(fr.glance) > 0);
}

/* ---- All strength levels ---- */

static void test_locale_all_strengths(void)
{
    cd_system_t sys[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC, CD_SYS_CHINESE,
                          CD_SYS_BUDDHIST, CD_SYS_HINDU };
    for (int s = CD_STRENGTH_MINOR; s <= CD_STRENGTH_RARE; s++) {
        convergence_interp_t r = cvi_interpret_locale((cd_strength_t)s,
                                                       sys, s + 1,
                                                       I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

/* ---- Rare convergence ---- */

static void test_locale_rare(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_HEBREW, CD_SYS_ISLAMIC,
                          CD_SYS_CHINESE, CD_SYS_BUDDHIST };
    convergence_interp_t r = cvi_interpret_locale(CD_STRENGTH_RARE,
                                                   sys, 5, I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("****", r.glyph);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_locale_en_first_entry);
    RUN_TEST(test_locale_en_matches_base);
    RUN_TEST(test_locale_es_produces_output);
    RUN_TEST(test_locale_es_null_systems);
    RUN_TEST(test_locale_fr_fallback);
    RUN_TEST(test_locale_all_strengths);
    RUN_TEST(test_locale_rare);

    return UNITY_END();
}
