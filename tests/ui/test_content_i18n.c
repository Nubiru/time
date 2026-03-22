/* test_content_i18n.c — Tests for content translation module. */

#include "unity.h"
#include "ui/content_i18n.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Key count ---- */

static void test_key_count_positive(void)
{
    TEST_ASSERT_GREATER_THAN(0, content_key_count());
}

static void test_key_count_includes_hebrew(void)
{
    /* Gregorian 94 + Hebrew 81 = 175 */
    TEST_ASSERT_GREATER_OR_EQUAL(175, content_key_count());
}

/* ---- Key existence ---- */

static void test_key_exists_month_name(void)
{
    TEST_ASSERT_TRUE(content_key_exists("gregorian.month.1.name"));
}

static void test_key_exists_season(void)
{
    TEST_ASSERT_TRUE(content_key_exists("gregorian.season.0.name"));
}

static void test_key_exists_day(void)
{
    TEST_ASSERT_TRUE(content_key_exists("gregorian.day.0.name"));
}

static void test_key_exists_template(void)
{
    TEST_ASSERT_TRUE(content_key_exists("gregorian.tpl.glance"));
    TEST_ASSERT_TRUE(content_key_exists("gregorian.tpl.detail"));
}

static void test_key_not_exists(void)
{
    TEST_ASSERT_FALSE(content_key_exists("nonexistent.key"));
}

static void test_key_exists_null(void)
{
    TEST_ASSERT_FALSE(content_key_exists(NULL));
}

static void test_key_exists_empty(void)
{
    TEST_ASSERT_FALSE(content_key_exists(""));
}

/* ---- English lookups ---- */

static void test_get_en_month_name(void)
{
    const char *val = content_get("gregorian.month.1.name", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("January", val);
}

static void test_get_en_month_12(void)
{
    const char *val = content_get("gregorian.month.12.name", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("December", val);
}

static void test_get_en_season(void)
{
    const char *val = content_get("gregorian.season.0.name", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Spring", val);
}

static void test_get_en_day(void)
{
    const char *val = content_get("gregorian.day.0.name", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Monday", val);
}

static void test_get_en_day_planet(void)
{
    const char *val = content_get("gregorian.day.6.planet", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Sun", val);
}

static void test_get_en_template(void)
{
    const char *val = content_get("gregorian.tpl.glance", I18N_LOCALE_EN);
    TEST_ASSERT_NOT_NULL(strstr(val, "%1$s"));
    TEST_ASSERT_NOT_NULL(strstr(val, "%2$d"));
}

/* ---- Spanish lookups ---- */

static void test_get_es_month_name(void)
{
    const char *val = content_get("gregorian.month.1.name", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Enero", val);
}

static void test_get_es_month_3(void)
{
    const char *val = content_get("gregorian.month.3.name", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Marzo", val);
}

static void test_get_es_season(void)
{
    const char *val = content_get("gregorian.season.0.name", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Primavera", val);
}

static void test_get_es_day(void)
{
    const char *val = content_get("gregorian.day.0.name", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Lunes", val);
}

static void test_get_es_day_planet(void)
{
    const char *val = content_get("gregorian.day.0.planet", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Luna", val);
}

static void test_get_es_winter(void)
{
    const char *val = content_get("gregorian.season.3.name", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Invierno", val);
}

/* ---- Hebrew lookups ---- */

static void test_get_en_hebrew_month(void)
{
    const char *val = content_get("hebrew.month.1.name", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Nisan", val);
}

static void test_get_en_hebrew_letter(void)
{
    const char *val = content_get("hebrew.month.7.letter", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("Lamed", val);
}

static void test_get_es_hebrew_tribe(void)
{
    /* Judah -> Judá in Spanish */
    const char *val = content_get("hebrew.month.1.tribe", I18N_LOCALE_ES);
    TEST_ASSERT_NOT_NULL(strstr(val, "Jud"));
}

static void test_get_es_hebrew_sense(void)
{
    const char *val = content_get("hebrew.month.1.sense", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Habla", val);
}

static void test_hebrew_month_names_preserved(void)
{
    /* Hebrew month names are system-native — same in all languages */
    const char *en = content_get("hebrew.month.7.name", I18N_LOCALE_EN);
    const char *es = content_get("hebrew.month.7.name", I18N_LOCALE_ES);
    TEST_ASSERT_EQUAL_STRING("Tishrei", en);
    TEST_ASSERT_EQUAL_STRING("Tishrei", es);
}

/* ---- Fallback behavior ---- */

static void test_fallback_unknown_locale(void)
{
    /* Russian has no content translations — should fall back to EN */
    const char *val = content_get("gregorian.month.1.name", I18N_LOCALE_RU);
    TEST_ASSERT_EQUAL_STRING("January", val);
}

static void test_fallback_unknown_key(void)
{
    /* Unknown key returns the key itself */
    const char *val = content_get("nonexistent.key", I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("nonexistent.key", val);
}

static void test_null_key(void)
{
    const char *val = content_get(NULL, I18N_LOCALE_EN);
    TEST_ASSERT_EQUAL_STRING("", val);
}

/* ---- All months covered ---- */

static void test_all_12_months_en(void)
{
    const char *expected[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    char key[64];
    for (int i = 1; i <= 12; i++) {
        snprintf(key, sizeof(key), "gregorian.month.%d.name", i);
        TEST_ASSERT_EQUAL_STRING(expected[i - 1],
                                 content_get(key, I18N_LOCALE_EN));
    }
}

static void test_all_12_months_es(void)
{
    const char *expected[] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };
    char key[64];
    for (int i = 1; i <= 12; i++) {
        snprintf(key, sizeof(key), "gregorian.month.%d.name", i);
        TEST_ASSERT_EQUAL_STRING(expected[i - 1],
                                 content_get(key, I18N_LOCALE_ES));
    }
}

/* ---- All 7 days covered ---- */

static void test_all_7_days_es(void)
{
    const char *expected[] = {
        "Lunes", "Martes", "Mi\xc3\xa9rcoles", "Jueves",
        "Viernes", "S\xc3\xa1" "bado", "Domingo"
    };
    char key[64];
    for (int i = 0; i < 7; i++) {
        snprintf(key, sizeof(key), "gregorian.day.%d.name", i);
        TEST_ASSERT_EQUAL_STRING(expected[i],
                                 content_get(key, I18N_LOCALE_ES));
    }
}

/* ---- All 4 seasons covered ---- */

static void test_all_4_seasons_es(void)
{
    const char *expected[] = {
        "Primavera", "Verano", "Oto\xc3\xb1o", "Invierno"
    };
    char key[64];
    for (int i = 0; i < 4; i++) {
        snprintf(key, sizeof(key), "gregorian.season.%d.name", i);
        TEST_ASSERT_EQUAL_STRING(expected[i],
                                 content_get(key, I18N_LOCALE_ES));
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Key count */
    RUN_TEST(test_key_count_positive);
    RUN_TEST(test_key_count_includes_hebrew);

    /* Key existence */
    RUN_TEST(test_key_exists_month_name);
    RUN_TEST(test_key_exists_season);
    RUN_TEST(test_key_exists_day);
    RUN_TEST(test_key_exists_template);
    RUN_TEST(test_key_not_exists);
    RUN_TEST(test_key_exists_null);
    RUN_TEST(test_key_exists_empty);

    /* English lookups */
    RUN_TEST(test_get_en_month_name);
    RUN_TEST(test_get_en_month_12);
    RUN_TEST(test_get_en_season);
    RUN_TEST(test_get_en_day);
    RUN_TEST(test_get_en_day_planet);
    RUN_TEST(test_get_en_template);

    /* Spanish lookups */
    RUN_TEST(test_get_es_month_name);
    RUN_TEST(test_get_es_month_3);
    RUN_TEST(test_get_es_season);
    RUN_TEST(test_get_es_day);
    RUN_TEST(test_get_es_day_planet);
    RUN_TEST(test_get_es_winter);

    /* Hebrew */
    RUN_TEST(test_get_en_hebrew_month);
    RUN_TEST(test_get_en_hebrew_letter);
    RUN_TEST(test_get_es_hebrew_tribe);
    RUN_TEST(test_get_es_hebrew_sense);
    RUN_TEST(test_hebrew_month_names_preserved);

    /* Fallback */
    RUN_TEST(test_fallback_unknown_locale);
    RUN_TEST(test_fallback_unknown_key);
    RUN_TEST(test_null_key);

    /* Coverage */
    RUN_TEST(test_all_12_months_en);
    RUN_TEST(test_all_12_months_es);
    RUN_TEST(test_all_7_days_es);
    RUN_TEST(test_all_4_seasons_es);

    return UNITY_END();
}
