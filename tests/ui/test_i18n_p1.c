#include "../unity/unity.h"
#include "../../src/ui/i18n.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ==== French (FR) translations ==== */

void test_fr_system_gregorian(void)
{
    const char *val = i18n_get_locale("system.gregorian", I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strcmp(val, "Gregorian") != 0);
    TEST_ASSERT_TRUE(strlen(val) > 0);
}

void test_fr_ui_loading(void)
{
    TEST_ASSERT_EQUAL_STRING("Chargement...",
                              i18n_get_locale("ui.loading", I18N_LOCALE_FR));
}

void test_fr_nav_home(void)
{
    TEST_ASSERT_EQUAL_STRING("Accueil",
                              i18n_get_locale("nav.home", I18N_LOCALE_FR));
}

void test_fr_planet_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("Soleil",
                              i18n_get_locale("planet.sun", I18N_LOCALE_FR));
}

void test_fr_welcome_skip(void)
{
    TEST_ASSERT_EQUAL_STRING("Passer",
                              i18n_get_locale("welcome.skip", I18N_LOCALE_FR));
}

void test_fr_time_paused(void)
{
    const char *val = i18n_get_locale("time.paused", I18N_LOCALE_FR);
    TEST_ASSERT_TRUE(strcmp(val, "Paused") != 0);
}

/* ==== Portuguese (PT) translations ==== */

void test_pt_system_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregoriano",
                              i18n_get_locale("system.gregorian", I18N_LOCALE_PT));
}

void test_pt_ui_loading(void)
{
    TEST_ASSERT_EQUAL_STRING("Carregando...",
                              i18n_get_locale("ui.loading", I18N_LOCALE_PT));
}

void test_pt_nav_home(void)
{
    const char *val = i18n_get_locale("nav.home", I18N_LOCALE_PT);
    TEST_ASSERT_TRUE(strcmp(val, "Home") != 0);
}

void test_pt_planet_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("Sol",
                              i18n_get_locale("planet.sun", I18N_LOCALE_PT));
}

void test_pt_welcome_skip(void)
{
    TEST_ASSERT_EQUAL_STRING("Pular",
                              i18n_get_locale("welcome.skip", I18N_LOCALE_PT));
}

/* ==== German (DE) translations ==== */

void test_de_system_gregorian(void)
{
    const char *val = i18n_get_locale("system.gregorian", I18N_LOCALE_DE);
    TEST_ASSERT_TRUE(strcmp(val, "Gregorian") != 0);
    TEST_ASSERT_TRUE(strlen(val) > 0);
}

void test_de_ui_loading(void)
{
    TEST_ASSERT_EQUAL_STRING("Laden...",
                              i18n_get_locale("ui.loading", I18N_LOCALE_DE));
}

void test_de_nav_home(void)
{
    TEST_ASSERT_EQUAL_STRING("Startseite",
                              i18n_get_locale("nav.home", I18N_LOCALE_DE));
}

void test_de_planet_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("Sonne",
                              i18n_get_locale("planet.sun", I18N_LOCALE_DE));
}

void test_de_welcome_skip(void)
{
    const char *val = i18n_get_locale("welcome.skip", I18N_LOCALE_DE);
    TEST_ASSERT_TRUE(strcmp(val, "Skip") != 0);
}

void test_de_time_paused(void)
{
    TEST_ASSERT_EQUAL_STRING("Pausiert",
                              i18n_get_locale("time.paused", I18N_LOCALE_DE));
}

/* ==== Coverage: all P1 locales should be 100% ==== */

void test_fr_coverage_100(void)
{
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_FR));
}

void test_pt_coverage_100(void)
{
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_PT));
}

void test_de_coverage_100(void)
{
    TEST_ASSERT_EQUAL_FLOAT(1.0f, i18n_locale_coverage(I18N_LOCALE_DE));
}

/* ==== Completeness: all keys translated ==== */

void test_fr_all_keys_translated(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        const char *val = i18n_get_locale(e.key, I18N_LOCALE_FR);
        TEST_ASSERT_NOT_NULL(val);
        TEST_ASSERT_TRUE(strlen(val) > 0);
    }
}

void test_pt_all_keys_translated(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        const char *val = i18n_get_locale(e.key, I18N_LOCALE_PT);
        TEST_ASSERT_NOT_NULL(val);
        TEST_ASSERT_TRUE(strlen(val) > 0);
    }
}

void test_de_all_keys_translated(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        const char *val = i18n_get_locale(e.key, I18N_LOCALE_DE);
        TEST_ASSERT_NOT_NULL(val);
        TEST_ASSERT_TRUE(strlen(val) > 0);
    }
}

/* ==== Total translated locale count ==== */

void test_seven_locales_translated(void)
{
    /* EN (default) + ES + AR + ZH + FR + PT + DE = 7 */
    int translated = 0;
    for (int i = 0; i < I18N_LOCALE_COUNT; i++) {
        if (i18n_locale_coverage((i18n_locale_t)i) >= 1.0f) {
            translated++;
        }
    }
    TEST_ASSERT_TRUE(translated >= 7);
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* French */
    RUN_TEST(test_fr_system_gregorian);
    RUN_TEST(test_fr_ui_loading);
    RUN_TEST(test_fr_nav_home);
    RUN_TEST(test_fr_planet_sun);
    RUN_TEST(test_fr_welcome_skip);
    RUN_TEST(test_fr_time_paused);

    /* Portuguese */
    RUN_TEST(test_pt_system_gregorian);
    RUN_TEST(test_pt_ui_loading);
    RUN_TEST(test_pt_nav_home);
    RUN_TEST(test_pt_planet_sun);
    RUN_TEST(test_pt_welcome_skip);

    /* German */
    RUN_TEST(test_de_system_gregorian);
    RUN_TEST(test_de_ui_loading);
    RUN_TEST(test_de_nav_home);
    RUN_TEST(test_de_planet_sun);
    RUN_TEST(test_de_welcome_skip);
    RUN_TEST(test_de_time_paused);

    /* Coverage */
    RUN_TEST(test_fr_coverage_100);
    RUN_TEST(test_pt_coverage_100);
    RUN_TEST(test_de_coverage_100);

    /* Completeness */
    RUN_TEST(test_fr_all_keys_translated);
    RUN_TEST(test_pt_all_keys_translated);
    RUN_TEST(test_de_all_keys_translated);

    /* Total count */
    RUN_TEST(test_seven_locales_translated);

    return UNITY_END();
}
