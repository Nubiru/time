#include "../unity/unity.h"
#include "../../src/ui/i18n.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ==== New locale enum entries (FR, PT, DE, RU, KO, TR, ID, BN, VI, SW, FA) ==== */

void test_locale_count_is_21(void)
{
    TEST_ASSERT_EQUAL_INT(21, I18N_LOCALE_COUNT);
}

void test_locale_name_french(void)
{
    TEST_ASSERT_EQUAL_STRING("French", i18n_locale_name(I18N_LOCALE_FR));
}

void test_locale_name_portuguese(void)
{
    TEST_ASSERT_EQUAL_STRING("Portuguese", i18n_locale_name(I18N_LOCALE_PT));
}

void test_locale_name_german(void)
{
    TEST_ASSERT_EQUAL_STRING("German", i18n_locale_name(I18N_LOCALE_DE));
}

void test_locale_name_russian(void)
{
    TEST_ASSERT_EQUAL_STRING("Russian", i18n_locale_name(I18N_LOCALE_RU));
}

void test_locale_name_korean(void)
{
    TEST_ASSERT_EQUAL_STRING("Korean", i18n_locale_name(I18N_LOCALE_KO));
}

void test_locale_name_turkish(void)
{
    TEST_ASSERT_EQUAL_STRING("Turkish", i18n_locale_name(I18N_LOCALE_TR));
}

void test_locale_name_indonesian(void)
{
    TEST_ASSERT_EQUAL_STRING("Indonesian", i18n_locale_name(I18N_LOCALE_ID));
}

void test_locale_name_bengali(void)
{
    TEST_ASSERT_EQUAL_STRING("Bengali", i18n_locale_name(I18N_LOCALE_BN));
}

void test_locale_name_vietnamese(void)
{
    TEST_ASSERT_EQUAL_STRING("Vietnamese", i18n_locale_name(I18N_LOCALE_VI));
}

void test_locale_name_swahili(void)
{
    TEST_ASSERT_EQUAL_STRING("Swahili", i18n_locale_name(I18N_LOCALE_SW));
}

void test_locale_name_persian(void)
{
    TEST_ASSERT_EQUAL_STRING("Persian", i18n_locale_name(I18N_LOCALE_FA));
}

/* ==== Locale codes for new locales ==== */

void test_locale_code_french(void)
{
    TEST_ASSERT_EQUAL_STRING("fr", i18n_locale_code(I18N_LOCALE_FR));
}

void test_locale_code_portuguese(void)
{
    TEST_ASSERT_EQUAL_STRING("pt", i18n_locale_code(I18N_LOCALE_PT));
}

void test_locale_code_german(void)
{
    TEST_ASSERT_EQUAL_STRING("de", i18n_locale_code(I18N_LOCALE_DE));
}

void test_locale_code_russian(void)
{
    TEST_ASSERT_EQUAL_STRING("ru", i18n_locale_code(I18N_LOCALE_RU));
}

void test_locale_code_korean(void)
{
    TEST_ASSERT_EQUAL_STRING("ko", i18n_locale_code(I18N_LOCALE_KO));
}

void test_locale_code_turkish(void)
{
    TEST_ASSERT_EQUAL_STRING("tr", i18n_locale_code(I18N_LOCALE_TR));
}

void test_locale_code_indonesian(void)
{
    TEST_ASSERT_EQUAL_STRING("id", i18n_locale_code(I18N_LOCALE_ID));
}

void test_locale_code_bengali(void)
{
    TEST_ASSERT_EQUAL_STRING("bn", i18n_locale_code(I18N_LOCALE_BN));
}

void test_locale_code_vietnamese(void)
{
    TEST_ASSERT_EQUAL_STRING("vi", i18n_locale_code(I18N_LOCALE_VI));
}

void test_locale_code_swahili(void)
{
    TEST_ASSERT_EQUAL_STRING("sw", i18n_locale_code(I18N_LOCALE_SW));
}

void test_locale_code_persian(void)
{
    TEST_ASSERT_EQUAL_STRING("fa", i18n_locale_code(I18N_LOCALE_FA));
}

/* ==== Locale from code (new locales) ==== */

void test_locale_from_code_fr(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_FR, i18n_locale_from_code("fr"));
}

void test_locale_from_code_pt(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_PT, i18n_locale_from_code("pt"));
}

void test_locale_from_code_de(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_DE, i18n_locale_from_code("de"));
}

void test_locale_from_code_ru(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_RU, i18n_locale_from_code("ru"));
}

void test_locale_from_code_ko(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_KO, i18n_locale_from_code("ko"));
}

void test_locale_from_code_fa(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_FA, i18n_locale_from_code("fa"));
}

/* ==== RTL for new locales ==== */

void test_is_rtl_persian(void)
{
    TEST_ASSERT_TRUE(i18n_is_rtl(I18N_LOCALE_FA));
}

void test_is_rtl_french(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_FR));
}

void test_is_rtl_russian(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_RU));
}

void test_is_rtl_korean(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_KO));
}

/* ==== Plural rules for new locales ==== */

void test_plural_rule_french(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_FR));
}

void test_plural_rule_russian(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_SLAVIC,
                          i18n_plural_rule(I18N_LOCALE_RU));
}

void test_plural_rule_korean(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_KO));
}

void test_plural_rule_turkish(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_TR));
}

void test_plural_rule_indonesian(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_ID));
}

void test_plural_rule_vietnamese(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_VI));
}

void test_plural_rule_persian(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_FA));
}

/* ==== Per-locale translation: Spanish (P0) ==== */

void test_es_system_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregoriano",
                              i18n_get_locale("system.gregorian", I18N_LOCALE_ES));
}

void test_es_system_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebreo",
                              i18n_get_locale("system.hebrew", I18N_LOCALE_ES));
}

void test_es_system_islamic(void)
{
    const char *val = i18n_get_locale("system.islamic", I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strcmp(val, "Islamic") != 0); /* not English */
    TEST_ASSERT_TRUE(strlen(val) > 0);
}

void test_es_ui_loading(void)
{
    TEST_ASSERT_EQUAL_STRING("Despertando...",
                              i18n_get_locale("ui.loading", I18N_LOCALE_ES));
}

void test_es_nav_home(void)
{
    TEST_ASSERT_EQUAL_STRING("Inicio",
                              i18n_get_locale("nav.home", I18N_LOCALE_ES));
}

void test_es_welcome_skip(void)
{
    TEST_ASSERT_EQUAL_STRING("Omitir",
                              i18n_get_locale("welcome.skip", I18N_LOCALE_ES));
}

void test_es_planet_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("Sol",
                              i18n_get_locale("planet.sun", I18N_LOCALE_ES));
}

void test_es_time_paused(void)
{
    TEST_ASSERT_EQUAL_STRING("En pausa",
                              i18n_get_locale("time.paused", I18N_LOCALE_ES));
}

void test_es_depth_cosmic(void)
{
    const char *val = i18n_get_locale("depth.cosmic", I18N_LOCALE_ES);
    TEST_ASSERT_TRUE(strcmp(val, "Cosmic") != 0); /* not English */
}

/* ==== Per-locale translation: Arabic (P0) ==== */

void test_ar_system_islamic(void)
{
    const char *val = i18n_get_locale("system.islamic", I18N_LOCALE_AR);
    TEST_ASSERT_TRUE(strcmp(val, "Islamic") != 0); /* Arabic text */
    TEST_ASSERT_TRUE(strlen(val) > 0);
}

void test_ar_nav_home(void)
{
    const char *val = i18n_get_locale("nav.home", I18N_LOCALE_AR);
    TEST_ASSERT_TRUE(strcmp(val, "Home") != 0); /* Arabic text */
}

void test_ar_planet_sun(void)
{
    const char *val = i18n_get_locale("planet.sun", I18N_LOCALE_AR);
    TEST_ASSERT_TRUE(strcmp(val, "Sun") != 0); /* Arabic text */
}

void test_ar_ui_loading(void)
{
    const char *val = i18n_get_locale("ui.loading", I18N_LOCALE_AR);
    TEST_ASSERT_TRUE(strcmp(val, "Loading...") != 0);
}

void test_ar_welcome_skip(void)
{
    const char *val = i18n_get_locale("welcome.skip", I18N_LOCALE_AR);
    TEST_ASSERT_TRUE(strcmp(val, "Skip") != 0);
}

/* ==== Per-locale translation: Chinese (P0) ==== */

void test_zh_system_chinese(void)
{
    const char *val = i18n_get_locale("system.chinese", I18N_LOCALE_ZH);
    TEST_ASSERT_TRUE(strcmp(val, "Chinese") != 0); /* Chinese text */
    TEST_ASSERT_TRUE(strlen(val) > 0);
}

void test_zh_system_iching(void)
{
    const char *val = i18n_get_locale("system.iching", I18N_LOCALE_ZH);
    TEST_ASSERT_TRUE(strcmp(val, "I Ching") != 0);
}

void test_zh_nav_home(void)
{
    const char *val = i18n_get_locale("nav.home", I18N_LOCALE_ZH);
    TEST_ASSERT_TRUE(strcmp(val, "Home") != 0);
}

void test_zh_planet_sun(void)
{
    const char *val = i18n_get_locale("planet.sun", I18N_LOCALE_ZH);
    TEST_ASSERT_TRUE(strcmp(val, "Sun") != 0);
}

void test_zh_ui_loading(void)
{
    const char *val = i18n_get_locale("ui.loading", I18N_LOCALE_ZH);
    TEST_ASSERT_TRUE(strcmp(val, "Loading...") != 0);
}

/* ==== Fallback chain: locale -> English -> key ==== */

void test_fallback_untranslated_locale_returns_english(void)
{
    /* Invalid locale index falls back to English */
    TEST_ASSERT_EQUAL_STRING("Gregorian",
                              i18n_get_locale("system.gregorian", I18N_LOCALE_COUNT));
}

void test_fallback_unknown_key_returns_key(void)
{
    /* Unknown key in any locale returns the key itself */
    TEST_ASSERT_EQUAL_STRING("nonexistent.key",
                              i18n_get_locale("nonexistent.key", I18N_LOCALE_ES));
}

void test_fallback_null_key_returns_empty(void)
{
    TEST_ASSERT_EQUAL_STRING("", i18n_get_locale(NULL, I18N_LOCALE_ES));
}

void test_english_locale_returns_english(void)
{
    /* Explicit English lookup returns English */
    TEST_ASSERT_EQUAL_STRING("Gregorian",
                              i18n_get_locale("system.gregorian", I18N_LOCALE_EN));
}

/* ==== Coverage API ==== */

void test_coverage_english_is_100(void)
{
    /* English is the default — always 100% */
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_EN));
}

void test_coverage_spanish_is_100(void)
{
    /* P0 language should have full coverage */
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_ES));
}

void test_coverage_arabic_is_100(void)
{
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_AR));
}

void test_coverage_chinese_is_100(void)
{
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_ZH));
}

void test_coverage_untranslated_is_zero(void)
{
    /* Out-of-range locale → 0% coverage */
    TEST_ASSERT_EQUAL_FLOAT(0.0f, i18n_locale_coverage((i18n_locale_t)(I18N_LOCALE_COUNT + 1)));
}

void test_coverage_invalid_is_zero(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.0f, i18n_locale_coverage(I18N_LOCALE_COUNT));
}

void test_translated_count_english(void)
{
    /* English == all keys */
    TEST_ASSERT_EQUAL_INT(i18n_key_count(), i18n_translated_count(I18N_LOCALE_EN));
}

void test_translated_count_spanish(void)
{
    TEST_ASSERT_EQUAL_INT(i18n_key_count(), i18n_translated_count(I18N_LOCALE_ES));
}

void test_translated_count_untranslated(void)
{
    TEST_ASSERT_EQUAL_INT(0, i18n_translated_count((i18n_locale_t)(I18N_LOCALE_COUNT + 1)));
}

/* ==== Completeness: all P0 keys translated ==== */

void test_es_all_keys_translated(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        const char *val = i18n_get_locale(e.key, I18N_LOCALE_ES);
        TEST_ASSERT_NOT_NULL(val);
        TEST_ASSERT_TRUE(strlen(val) > 0);
        /* Most keys should differ from English (except proper nouns) */
    }
}

void test_ar_all_keys_translated(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        const char *val = i18n_get_locale(e.key, I18N_LOCALE_AR);
        TEST_ASSERT_NOT_NULL(val);
        TEST_ASSERT_TRUE(strlen(val) > 0);
    }
}

void test_zh_all_keys_translated(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        const char *val = i18n_get_locale(e.key, I18N_LOCALE_ZH);
        TEST_ASSERT_NOT_NULL(val);
        TEST_ASSERT_TRUE(strlen(val) > 0);
    }
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* New locale enum */
    RUN_TEST(test_locale_count_is_21);
    RUN_TEST(test_locale_name_french);
    RUN_TEST(test_locale_name_portuguese);
    RUN_TEST(test_locale_name_german);
    RUN_TEST(test_locale_name_russian);
    RUN_TEST(test_locale_name_korean);
    RUN_TEST(test_locale_name_turkish);
    RUN_TEST(test_locale_name_indonesian);
    RUN_TEST(test_locale_name_bengali);
    RUN_TEST(test_locale_name_vietnamese);
    RUN_TEST(test_locale_name_swahili);
    RUN_TEST(test_locale_name_persian);

    /* New locale codes */
    RUN_TEST(test_locale_code_french);
    RUN_TEST(test_locale_code_portuguese);
    RUN_TEST(test_locale_code_german);
    RUN_TEST(test_locale_code_russian);
    RUN_TEST(test_locale_code_korean);
    RUN_TEST(test_locale_code_turkish);
    RUN_TEST(test_locale_code_indonesian);
    RUN_TEST(test_locale_code_bengali);
    RUN_TEST(test_locale_code_vietnamese);
    RUN_TEST(test_locale_code_swahili);
    RUN_TEST(test_locale_code_persian);

    /* Locale from code */
    RUN_TEST(test_locale_from_code_fr);
    RUN_TEST(test_locale_from_code_pt);
    RUN_TEST(test_locale_from_code_de);
    RUN_TEST(test_locale_from_code_ru);
    RUN_TEST(test_locale_from_code_ko);
    RUN_TEST(test_locale_from_code_fa);

    /* RTL */
    RUN_TEST(test_is_rtl_persian);
    RUN_TEST(test_is_rtl_french);
    RUN_TEST(test_is_rtl_russian);
    RUN_TEST(test_is_rtl_korean);

    /* Plural rules */
    RUN_TEST(test_plural_rule_french);
    RUN_TEST(test_plural_rule_russian);
    RUN_TEST(test_plural_rule_korean);
    RUN_TEST(test_plural_rule_turkish);
    RUN_TEST(test_plural_rule_indonesian);
    RUN_TEST(test_plural_rule_vietnamese);
    RUN_TEST(test_plural_rule_persian);

    /* Spanish translations */
    RUN_TEST(test_es_system_gregorian);
    RUN_TEST(test_es_system_hebrew);
    RUN_TEST(test_es_system_islamic);
    RUN_TEST(test_es_ui_loading);
    RUN_TEST(test_es_nav_home);
    RUN_TEST(test_es_welcome_skip);
    RUN_TEST(test_es_planet_sun);
    RUN_TEST(test_es_time_paused);
    RUN_TEST(test_es_depth_cosmic);

    /* Arabic translations */
    RUN_TEST(test_ar_system_islamic);
    RUN_TEST(test_ar_nav_home);
    RUN_TEST(test_ar_planet_sun);
    RUN_TEST(test_ar_ui_loading);
    RUN_TEST(test_ar_welcome_skip);

    /* Chinese translations */
    RUN_TEST(test_zh_system_chinese);
    RUN_TEST(test_zh_system_iching);
    RUN_TEST(test_zh_nav_home);
    RUN_TEST(test_zh_planet_sun);
    RUN_TEST(test_zh_ui_loading);

    /* Fallback chain */
    RUN_TEST(test_fallback_untranslated_locale_returns_english);
    RUN_TEST(test_fallback_unknown_key_returns_key);
    RUN_TEST(test_fallback_null_key_returns_empty);
    RUN_TEST(test_english_locale_returns_english);

    /* Coverage API */
    RUN_TEST(test_coverage_english_is_100);
    RUN_TEST(test_coverage_spanish_is_100);
    RUN_TEST(test_coverage_arabic_is_100);
    RUN_TEST(test_coverage_chinese_is_100);
    RUN_TEST(test_coverage_untranslated_is_zero);
    RUN_TEST(test_coverage_invalid_is_zero);
    RUN_TEST(test_translated_count_english);
    RUN_TEST(test_translated_count_spanish);
    RUN_TEST(test_translated_count_untranslated);

    /* Completeness */
    RUN_TEST(test_es_all_keys_translated);
    RUN_TEST(test_ar_all_keys_translated);
    RUN_TEST(test_zh_all_keys_translated);

    return UNITY_END();
}
