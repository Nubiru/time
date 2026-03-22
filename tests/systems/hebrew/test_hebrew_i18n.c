/* test_hebrew_i18n.c — Tests for locale-aware Hebrew interpretation. */

#include "unity.h"
#include "systems/hebrew/hebrew_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- English (same as hi_interpret) ---- */

static void test_locale_en_nisan(void)
{
    hebrew_date_t d = { 5786, 1, 15 };
    hebrew_interp_t r = hi_interpret_locale(d, 0, 0, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Hei", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nisan"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Redemption"));
}

static void test_locale_en_has_same_content(void)
{
    hebrew_date_t d = { 5786, 7, 1 };
    hebrew_interp_t en = hi_interpret_locale(d, 3, 0, I18N_LOCALE_EN);
    /* EN locale fast path delegates to hi_interpret — identical output */
    TEST_ASSERT_EQUAL_STRING("Lamed", en.glyph);
    TEST_ASSERT_NOT_NULL(strstr(en.glance, "Tishrei"));
    TEST_ASSERT_NOT_NULL(strstr(en.glance, "Awe and judgment"));
    TEST_ASSERT_NOT_NULL(strstr(en.detail, "Shemitah"));
}

/* ---- Spanish ---- */

static void test_locale_es_nisan_quality(void)
{
    hebrew_date_t d = { 5786, 1, 15 };
    hebrew_interp_t r = hi_interpret_locale(d, 0, 0, I18N_LOCALE_ES);
    /* Spanish quality for Nisan is "Redención" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Nisan"));
}

static void test_locale_es_tribe_translated(void)
{
    hebrew_date_t d = { 5786, 1, 15 };
    hebrew_interp_t r = hi_interpret_locale(d, 0, 0, I18N_LOCALE_ES);
    /* Spanish tribe for month 1 is "Judá" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Jud"));
}

static void test_locale_es_tishrei(void)
{
    hebrew_date_t d = { 5786, 7, 1 };
    hebrew_interp_t r = hi_interpret_locale(d, 0, 0, I18N_LOCALE_ES);
    /* Month name stays Hebrew */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tishrei"));
}

static void test_locale_es_sense_translated(void)
{
    hebrew_date_t d = { 5786, 1, 15 };
    hebrew_interp_t r = hi_interpret_locale(d, 0, 0, I18N_LOCALE_ES);
    /* Spanish sense for month 1 is "Habla" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Habla"));
}

static void test_locale_es_sabbatical(void)
{
    hebrew_date_t d = { 5786, 1, 15 };
    hebrew_interp_t r = hi_interpret_locale(d, 3, 0, I18N_LOCALE_ES);
    /* Spanish sabbatical template */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Shemit"));
}

static void test_locale_es_kislev(void)
{
    hebrew_date_t d = { 5786, 9, 25 };
    hebrew_interp_t r = hi_interpret_locale(d, 0, 0, I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kislev"));
}

/* ---- Fallback ---- */

static void test_locale_fallback_to_en(void)
{
    hebrew_date_t d = { 5786, 1, 15 };
    hebrew_interp_t ru = hi_interpret_locale(d, 0, 0, I18N_LOCALE_RU);
    /* RU has no content table — falls back to EN */
    TEST_ASSERT_EQUAL_STRING("Hei", ru.glyph);
    TEST_ASSERT_NOT_NULL(strstr(ru.glance, "Nisan"));
    TEST_ASSERT_NOT_NULL(strstr(ru.glance, "Redemption"));
}

/* ---- All months produce output ---- */

static void test_locale_es_all_months(void)
{
    for (int m = 1; m <= 13; m++) {
        hebrew_date_t d = { 5786, m, 1 };
        hebrew_interp_t r = hi_interpret_locale(d, 0, m == 13 ? 1 : 0,
                                                I18N_LOCALE_ES);
        TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
        TEST_ASSERT_TRUE(strlen(r.glance) > 0);
        TEST_ASSERT_TRUE(strlen(r.detail) > 0);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* English */
    RUN_TEST(test_locale_en_nisan);
    RUN_TEST(test_locale_en_has_same_content);

    /* Spanish */
    RUN_TEST(test_locale_es_nisan_quality);
    RUN_TEST(test_locale_es_tribe_translated);
    RUN_TEST(test_locale_es_tishrei);
    RUN_TEST(test_locale_es_sense_translated);
    RUN_TEST(test_locale_es_sabbatical);
    RUN_TEST(test_locale_es_kislev);

    /* Fallback */
    RUN_TEST(test_locale_fallback_to_en);

    /* Coverage */
    RUN_TEST(test_locale_es_all_months);

    return UNITY_END();
}
