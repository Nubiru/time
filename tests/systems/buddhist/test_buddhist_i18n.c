/* test_buddhist_i18n.c — Tests for locale-aware Buddhist interpretation. */

#include "unity.h"
#include "systems/buddhist/buddhist_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English (same as bi_interpret) ---- */

static void test_locale_en_vesakha(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 2, UPOSATHA_FULL_MOON,
                                              KALPA_PHASE_EXISTENCE,
                                              I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("BE", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Vesakha"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Full Moon"));
}

static void test_locale_en_has_same_content(void)
{
    buddhist_interp_t en = bi_interpret_locale(2569, 1, UPOSATHA_NONE,
                                               KALPA_PHASE_FORMATION,
                                               I18N_LOCALE_EN);
    /* EN locale fast path delegates to bi_interpret — identical output */
    TEST_ASSERT_EQUAL_STRING("BE", en.glyph);
    TEST_ASSERT_NOT_NULL(strstr(en.glance, "Citta"));
    TEST_ASSERT_NOT_NULL(strstr(en.glance, "Hot"));
    TEST_ASSERT_NOT_NULL(strstr(en.detail, "awakening"));
}

static void test_locale_en_uposatha_in_detail(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 2, UPOSATHA_FULL_MOON,
                                              KALPA_PHASE_EXISTENCE,
                                              I18N_LOCALE_EN);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Patimokkha"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Illumination"));
}

static void test_locale_en_kalpa_in_detail(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 1, UPOSATHA_NONE,
                                              KALPA_PHASE_EMPTINESS,
                                              I18N_LOCALE_EN);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Emptiness"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Samvattathaayi"));
}

/* ---- Spanish ---- */

static void test_locale_es_vesakha_glance(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 2, UPOSATHA_FULL_MOON,
                                              KALPA_PHASE_EXISTENCE,
                                              I18N_LOCALE_ES);
    /* Spanish uses "EB" (Era Budista) and "Observancia de Luna Llena" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "EB"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Vesakha"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Luna Llena"));
}

static void test_locale_es_season_translated(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 1, UPOSATHA_NONE,
                                              KALPA_PHASE_FORMATION,
                                              I18N_LOCALE_ES);
    /* Spanish "Hot" = "Calurosa" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Calurosa"));
}

static void test_locale_es_kalpa_translated(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 1, UPOSATHA_NONE,
                                              KALPA_PHASE_FORMATION,
                                              I18N_LOCALE_ES);
    /* Spanish kalpa phase "Formation" = "Formación" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Formaci"));
}

static void test_locale_es_meaning_translated(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 2, UPOSATHA_NONE,
                                              KALPA_PHASE_EXISTENCE,
                                              I18N_LOCALE_ES);
    /* Spanish meaning for month 2: "El mes más sagrado" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sagrado"));
}

static void test_locale_es_uposatha_practice(void)
{
    buddhist_interp_t r = bi_interpret_locale(2569, 2, UPOSATHA_FULL_MOON,
                                              KALPA_PHASE_EXISTENCE,
                                              I18N_LOCALE_ES);
    /* Spanish practice includes "Patimokkha" (proper noun preserved) */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Patimokkha"));
}

/* ---- Fallback ---- */

static void test_locale_fallback_to_en(void)
{
    buddhist_interp_t fr = bi_interpret_locale(2569, 2, UPOSATHA_FULL_MOON,
                                               KALPA_PHASE_EXISTENCE,
                                               I18N_LOCALE_FR);
    /* FR falls back to EN content */
    TEST_ASSERT_EQUAL_STRING("BE", fr.glyph);
    TEST_ASSERT_NOT_NULL(strstr(fr.glance, "Vesakha"));
    TEST_ASSERT_NOT_NULL(strstr(fr.detail, "Patimokkha"));
}

/* ---- Coverage: all months produce output ---- */

static void test_locale_es_all_months(void)
{
    for (int m = 1; m <= 12; m++) {
        buddhist_interp_t r = bi_interpret_locale(2569, m, UPOSATHA_NONE,
                                                  KALPA_PHASE_EXISTENCE,
                                                  I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_all_uposatha(void)
{
    for (int u = 0; u < 5; u++) {
        buddhist_interp_t r = bi_interpret_locale(2569, 1, (uposatha_type_t)u,
                                                  KALPA_PHASE_EXISTENCE,
                                                  I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

static void test_locale_es_all_kalpa_phases(void)
{
    for (int k = 0; k < 4; k++) {
        buddhist_interp_t r = bi_interpret_locale(2569, 1, UPOSATHA_NONE,
                                                  (kalpa_phase_t)k,
                                                  I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.detail) > 10);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_vesakha);
    RUN_TEST(test_locale_en_has_same_content);
    RUN_TEST(test_locale_en_uposatha_in_detail);
    RUN_TEST(test_locale_en_kalpa_in_detail);

    /* Spanish */
    RUN_TEST(test_locale_es_vesakha_glance);
    RUN_TEST(test_locale_es_season_translated);
    RUN_TEST(test_locale_es_kalpa_translated);
    RUN_TEST(test_locale_es_meaning_translated);
    RUN_TEST(test_locale_es_uposatha_practice);

    /* Fallback */
    RUN_TEST(test_locale_fallback_to_en);

    /* Coverage */
    RUN_TEST(test_locale_es_all_months);
    RUN_TEST(test_locale_es_all_uposatha);
    RUN_TEST(test_locale_es_all_kalpa_phases);

    return UNITY_END();
}
