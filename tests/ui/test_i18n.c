#include "../unity/unity.h"
#include "../../src/ui/i18n.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- i18n_get: tests 1-20 ---- */

void test_get_system_gregorian(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", i18n_get("system.gregorian"));
}

void test_get_system_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", i18n_get("system.hebrew"));
}

void test_get_system_islamic(void)
{
    TEST_ASSERT_EQUAL_STRING("Islamic", i18n_get("system.islamic"));
}

void test_get_system_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", i18n_get("system.chinese"));
}

void test_get_system_hindu(void)
{
    TEST_ASSERT_EQUAL_STRING("Hindu", i18n_get("system.hindu"));
}

void test_get_system_buddhist(void)
{
    TEST_ASSERT_EQUAL_STRING("Buddhist", i18n_get("system.buddhist"));
}

void test_get_system_mayan(void)
{
    TEST_ASSERT_EQUAL_STRING("Mayan", i18n_get("system.mayan"));
}

void test_get_system_coptic(void)
{
    TEST_ASSERT_EQUAL_STRING("Coptic", i18n_get("system.coptic"));
}

void test_get_system_ethiopian(void)
{
    TEST_ASSERT_EQUAL_STRING("Ethiopian", i18n_get("system.ethiopian"));
}

void test_get_system_zoroastrian(void)
{
    TEST_ASSERT_EQUAL_STRING("Zoroastrian", i18n_get("system.zoroastrian"));
}

void test_get_system_celtic(void)
{
    TEST_ASSERT_EQUAL_STRING("Celtic", i18n_get("system.celtic"));
}

void test_get_system_myanmar(void)
{
    TEST_ASSERT_EQUAL_STRING("Myanmar", i18n_get("system.myanmar"));
}

void test_get_system_egyptian(void)
{
    TEST_ASSERT_EQUAL_STRING("Egyptian", i18n_get("system.egyptian"));
}

void test_get_system_astrology(void)
{
    TEST_ASSERT_EQUAL_STRING("Astrology", i18n_get("system.astrology"));
}

void test_get_system_iching(void)
{
    TEST_ASSERT_EQUAL_STRING("I Ching", i18n_get("system.iching"));
}

void test_get_system_human_design(void)
{
    TEST_ASSERT_EQUAL_STRING("Human Design", i18n_get("system.human_design"));
}

void test_get_ui_zoom_level(void)
{
    TEST_ASSERT_EQUAL_STRING("Zoom level", i18n_get("ui.zoom_level"));
}

void test_get_ui_view_space(void)
{
    TEST_ASSERT_EQUAL_STRING("Space View", i18n_get("ui.view_space"));
}

void test_get_ui_view_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth View", i18n_get("ui.view_earth"));
}

void test_get_unknown_key_returns_key(void)
{
    /* Unknown keys return the key itself, not NULL */
    const char *result = i18n_get("nonexistent.key");
    TEST_ASSERT_EQUAL_STRING("nonexistent.key", result);
}

/* ---- i18n_get: UI label tests 21-30 ---- */

void test_get_ui_festival_alert(void)
{
    TEST_ASSERT_EQUAL_STRING("Festival alert", i18n_get("ui.festival_alert"));
}

void test_get_ui_retrograde(void)
{
    TEST_ASSERT_EQUAL_STRING("retrograde", i18n_get("ui.retrograde"));
}

void test_get_ui_direct(void)
{
    TEST_ASSERT_EQUAL_STRING("direct", i18n_get("ui.direct"));
}

void test_get_ui_day(void)
{
    TEST_ASSERT_EQUAL_STRING("day", i18n_get("ui.day"));
}

void test_get_ui_days(void)
{
    TEST_ASSERT_EQUAL_STRING("days", i18n_get("ui.days"));
}

void test_get_ui_month(void)
{
    TEST_ASSERT_EQUAL_STRING("month", i18n_get("ui.month"));
}

void test_get_ui_months(void)
{
    TEST_ASSERT_EQUAL_STRING("months", i18n_get("ui.months"));
}

void test_get_ui_year(void)
{
    TEST_ASSERT_EQUAL_STRING("year", i18n_get("ui.year"));
}

void test_get_ui_years(void)
{
    TEST_ASSERT_EQUAL_STRING("years", i18n_get("ui.years"));
}

void test_get_ui_loading(void)
{
    TEST_ASSERT_EQUAL_STRING("Loading...", i18n_get("ui.loading"));
}

/* ---- i18n_get: edge cases 31-33 ---- */

void test_get_ui_error(void)
{
    TEST_ASSERT_EQUAL_STRING("Error", i18n_get("ui.error"));
}

void test_get_null_returns_empty(void)
{
    /* Passing NULL key should return empty string, not crash */
    const char *result = i18n_get(NULL);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("", result);
}

void test_get_empty_key_returns_itself(void)
{
    const char *result = i18n_get("");
    TEST_ASSERT_EQUAL_STRING("", result);
}

/* ---- i18n_key_count: test 34 ---- */

void test_key_count_at_least_30(void)
{
    TEST_ASSERT_TRUE(i18n_key_count() >= 30);
}

/* ---- i18n_key_exists: tests 35-38 ---- */

void test_key_exists_valid(void)
{
    TEST_ASSERT_TRUE(i18n_key_exists("system.gregorian"));
}

void test_key_exists_invalid(void)
{
    TEST_ASSERT_FALSE(i18n_key_exists("nonexistent.key"));
}

void test_key_exists_null(void)
{
    TEST_ASSERT_FALSE(i18n_key_exists(NULL));
}

void test_key_exists_empty(void)
{
    TEST_ASSERT_FALSE(i18n_key_exists(""));
}

/* ---- i18n_entry: tests 39-42 ---- */

void test_entry_valid_index(void)
{
    i18n_entry_t e = i18n_entry(0);
    TEST_ASSERT_NOT_NULL(e.key);
    TEST_ASSERT_NOT_NULL(e.value);
}

void test_entry_negative_index(void)
{
    i18n_entry_t e = i18n_entry(-1);
    TEST_ASSERT_NULL(e.key);
    TEST_ASSERT_NULL(e.value);
}

void test_entry_out_of_range(void)
{
    i18n_entry_t e = i18n_entry(9999);
    TEST_ASSERT_NULL(e.key);
    TEST_ASSERT_NULL(e.value);
}

void test_entry_last_valid(void)
{
    int count = i18n_key_count();
    i18n_entry_t e = i18n_entry(count - 1);
    TEST_ASSERT_NOT_NULL(e.key);
    TEST_ASSERT_NOT_NULL(e.value);
}

/* ---- i18n_locale_name: tests 43-47 ---- */

void test_locale_name_english(void)
{
    TEST_ASSERT_EQUAL_STRING("English", i18n_locale_name(I18N_LOCALE_EN));
}

void test_locale_name_arabic(void)
{
    TEST_ASSERT_EQUAL_STRING("Arabic", i18n_locale_name(I18N_LOCALE_AR));
}

void test_locale_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", i18n_locale_name(I18N_LOCALE_HE));
}

void test_locale_name_chinese(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese", i18n_locale_name(I18N_LOCALE_ZH));
}

void test_locale_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", i18n_locale_name(I18N_LOCALE_COUNT));
}

/* ---- i18n_locale_code: tests 48-51 ---- */

void test_locale_code_english(void)
{
    TEST_ASSERT_EQUAL_STRING("en", i18n_locale_code(I18N_LOCALE_EN));
}

void test_locale_code_arabic(void)
{
    TEST_ASSERT_EQUAL_STRING("ar", i18n_locale_code(I18N_LOCALE_AR));
}

void test_locale_code_japanese(void)
{
    TEST_ASSERT_EQUAL_STRING("ja", i18n_locale_code(I18N_LOCALE_JA));
}

void test_locale_code_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("", i18n_locale_code(I18N_LOCALE_COUNT));
}

/* ---- i18n_locale_from_code: tests 52-56 ---- */

void test_locale_from_code_en(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_EN, i18n_locale_from_code("en"));
}

void test_locale_from_code_ar(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_AR, i18n_locale_from_code("ar"));
}

void test_locale_from_code_zh(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_ZH, i18n_locale_from_code("zh"));
}

void test_locale_from_code_unknown(void)
{
    /* Unknown code returns EN (default) */
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_EN, i18n_locale_from_code("xx"));
}

void test_locale_from_code_null(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_EN, i18n_locale_from_code(NULL));
}

/* ---- i18n_is_rtl: tests 57-62 ---- */

void test_is_rtl_arabic(void)
{
    TEST_ASSERT_TRUE(i18n_is_rtl(I18N_LOCALE_AR));
}

void test_is_rtl_hebrew(void)
{
    TEST_ASSERT_TRUE(i18n_is_rtl(I18N_LOCALE_HE));
}

void test_is_rtl_english(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_EN));
}

void test_is_rtl_chinese(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_ZH));
}

void test_is_rtl_hindi(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_HI));
}

void test_is_rtl_invalid(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_COUNT));
}

/* ---- i18n_plural_rule: tests 63-68 ---- */

void test_plural_rule_english(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_EN));
}

void test_plural_rule_arabic(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ARABIC,
                          i18n_plural_rule(I18N_LOCALE_AR));
}

void test_plural_rule_chinese(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_ZH));
}

void test_plural_rule_japanese(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_JA));
}

void test_plural_rule_thai(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_TH));
}

void test_plural_rule_myanmar(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_EAST_ASIAN,
                          i18n_plural_rule(I18N_LOCALE_MY));
}

/* ---- i18n_plural_form ONE_OTHER: tests 69-73 ---- */

void test_one_other_zero(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ONE_OTHER, 0));
}

void test_one_other_one(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_ONE,
                          i18n_plural_form(I18N_PLURAL_ONE_OTHER, 1));
}

void test_one_other_two(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ONE_OTHER, 2));
}

void test_one_other_many(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ONE_OTHER, 100));
}

void test_one_other_negative(void)
{
    /* Negative counts use absolute value logic: -1 -> one */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ONE_OTHER, -1));
}

/* ---- i18n_plural_form EAST_ASIAN: tests 74-76 ---- */

void test_east_asian_zero(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_EAST_ASIAN, 0));
}

void test_east_asian_one(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_EAST_ASIAN, 1));
}

void test_east_asian_many(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_EAST_ASIAN, 1000));
}

/* ---- i18n_plural_form ARABIC: tests 77-84 ---- */

void test_arabic_zero(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 0));
}

void test_arabic_one(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_ONE,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 1));
}

void test_arabic_two(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_TWO,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 2));
}

void test_arabic_three(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_FEW,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 3));
}

void test_arabic_ten(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_FEW,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 10));
}

void test_arabic_eleven(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 11));
}

void test_arabic_ninety_nine(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 99));
}

void test_arabic_hundred(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 100));
}

/* ---- i18n_plural_form SLAVIC: tests 85-92 ---- */

void test_slavic_one(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_ONE,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 1));
}

void test_slavic_two(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_FEW,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 2));
}

void test_slavic_four(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_FEW,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 4));
}

void test_slavic_five(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 5));
}

void test_slavic_eleven(void)
{
    /* 11 ends in 1 but is in 11-14 exception range -> many */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 11));
}

void test_slavic_twelve(void)
{
    /* 12 ends in 2 but is in 12-14 exception -> many */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 12));
}

void test_slavic_twenty_one(void)
{
    /* 21 ends in 1, not 11 -> one */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_ONE,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 21));
}

void test_slavic_twenty_two(void)
{
    /* 22 ends in 2, not 12 -> few */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_FEW,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 22));
}

/* ---- i18n_plural_form_name: tests 93-97 ---- */

void test_form_name_one(void)
{
    TEST_ASSERT_EQUAL_STRING("one", i18n_plural_form_name(I18N_FORM_ONE));
}

void test_form_name_two(void)
{
    TEST_ASSERT_EQUAL_STRING("two", i18n_plural_form_name(I18N_FORM_TWO));
}

void test_form_name_few(void)
{
    TEST_ASSERT_EQUAL_STRING("few", i18n_plural_form_name(I18N_FORM_FEW));
}

void test_form_name_many(void)
{
    TEST_ASSERT_EQUAL_STRING("many", i18n_plural_form_name(I18N_FORM_MANY));
}

void test_form_name_other(void)
{
    TEST_ASSERT_EQUAL_STRING("other", i18n_plural_form_name(I18N_FORM_OTHER));
}

/* ---- i18n_plural_form_name edge: test 98 ---- */

void test_form_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("other", i18n_plural_form_name(I18N_FORM_COUNT));
}

/* ---- Additional locale tests: 99-102 ---- */

void test_locale_name_spanish(void)
{
    TEST_ASSERT_EQUAL_STRING("Spanish", i18n_locale_name(I18N_LOCALE_ES));
}

void test_locale_name_hindi(void)
{
    TEST_ASSERT_EQUAL_STRING("Hindi", i18n_locale_name(I18N_LOCALE_HI));
}

void test_locale_name_thai(void)
{
    TEST_ASSERT_EQUAL_STRING("Thai", i18n_locale_name(I18N_LOCALE_TH));
}

void test_locale_name_amharic(void)
{
    TEST_ASSERT_EQUAL_STRING("Amharic", i18n_locale_name(I18N_LOCALE_AM));
}

/* ---- Additional locale code tests: 103-106 ---- */

void test_locale_code_spanish(void)
{
    TEST_ASSERT_EQUAL_STRING("es", i18n_locale_code(I18N_LOCALE_ES));
}

void test_locale_code_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("he", i18n_locale_code(I18N_LOCALE_HE));
}

void test_locale_code_hindi(void)
{
    TEST_ASSERT_EQUAL_STRING("hi", i18n_locale_code(I18N_LOCALE_HI));
}

void test_locale_code_myanmar(void)
{
    TEST_ASSERT_EQUAL_STRING("my", i18n_locale_code(I18N_LOCALE_MY));
}

/* ---- Locale from code additional: 107-109 ---- */

void test_locale_from_code_es(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_ES, i18n_locale_from_code("es"));
}

void test_locale_from_code_he(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_HE, i18n_locale_from_code("he"));
}

void test_locale_from_code_empty(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_EN, i18n_locale_from_code(""));
}

/* ---- Plural rule edge cases: 110-112 ---- */

void test_plural_rule_spanish(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_ES));
}

void test_plural_rule_hindi(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_HI));
}

void test_plural_rule_amharic(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_PLURAL_ONE_OTHER,
                          i18n_plural_rule(I18N_LOCALE_AM));
}

/* ---- Slavic additional edge cases: 113-116 ---- */

void test_slavic_zero(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 0));
}

void test_slavic_fourteen(void)
{
    /* 14 is in 12-14 exception range -> many */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 14));
}

void test_slavic_hundred_one(void)
{
    /* 101 ends in 1, not 11 -> one */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_ONE,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 101));
}

void test_slavic_hundred_eleven(void)
{
    /* 111 ends in 11 -> many (exception) */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_SLAVIC, 111));
}

/* ---- Arabic additional edge cases: 117-119 ---- */

void test_arabic_fifty(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_MANY,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 50));
}

void test_arabic_large(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ARABIC, 500));
}

void test_arabic_negative(void)
{
    /* Negative count -> OTHER (safe default) */
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_ARABIC, -5));
}

/* ---- Invalid plural rule: test 120 ---- */

void test_plural_form_invalid_rule(void)
{
    TEST_ASSERT_EQUAL_INT(I18N_FORM_OTHER,
                          i18n_plural_form(I18N_PLURAL_RULE_COUNT, 5));
}

/* ---- Entry round-trip: tests 121-122 ---- */

void test_entry_key_value_match_get(void)
{
    /* First entry key should work with i18n_get */
    i18n_entry_t e = i18n_entry(0);
    TEST_ASSERT_NOT_NULL(e.key);
    const char *val = i18n_get(e.key);
    TEST_ASSERT_EQUAL_STRING(e.value, val);
}

void test_all_entries_have_valid_key_value(void)
{
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t e = i18n_entry(i);
        TEST_ASSERT_NOT_NULL(e.key);
        TEST_ASSERT_NOT_NULL(e.value);
        TEST_ASSERT_TRUE(strlen(e.key) > 0);
        TEST_ASSERT_TRUE(strlen(e.value) > 0);
    }
}

/* ---- RTL edge: tests 123-124 ---- */

void test_is_rtl_negative(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl((i18n_locale_t)-1));
}

void test_is_rtl_spanish(void)
{
    TEST_ASSERT_FALSE(i18n_is_rtl(I18N_LOCALE_ES));
}

/* ---- Expanded i18n keys: tests 125+ ---- */

void test_key_count_expanded(void)
{
    TEST_ASSERT_GREATER_OR_EQUAL(100, i18n_key_count());
}

void test_new_system_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("Kabbalah", i18n_get("system.kabbalah"));
    TEST_ASSERT_EQUAL_STRING("Geology", i18n_get("system.geology"));
    TEST_ASSERT_EQUAL_STRING("Earth", i18n_get("system.earth"));
    TEST_ASSERT_EQUAL_STRING("Unified", i18n_get("system.unified"));
}

void test_depth_tier_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("Surface", i18n_get("depth.surface"));
    TEST_ASSERT_EQUAL_STRING("Context", i18n_get("depth.context"));
    TEST_ASSERT_EQUAL_STRING("Cycle", i18n_get("depth.cycle"));
    TEST_ASSERT_EQUAL_STRING("Cosmic", i18n_get("depth.cosmic"));
}

void test_view_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("Space View", i18n_get("view.space"));
    TEST_ASSERT_EQUAL_STRING("Earth View", i18n_get("view.earth"));
}

void test_time_control_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("Paused", i18n_get("time.paused"));
    TEST_ASSERT_EQUAL_STRING("Speed", i18n_get("time.speed"));
    TEST_ASSERT_EQUAL_STRING("Now", i18n_get("time.now"));
}

void test_welcome_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("When did you arrive on this planet?",
                              i18n_get("welcome.when_born"));
    TEST_ASSERT_EQUAL_STRING("Skip", i18n_get("welcome.skip"));
}

void test_planet_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("Sun", i18n_get("planet.sun"));
    TEST_ASSERT_EQUAL_STRING("Moon", i18n_get("planet.moon"));
    TEST_ASSERT_EQUAL_STRING("Saturn", i18n_get("planet.saturn"));
}

void test_sensitivity_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("General", i18n_get("sensitivity.general"));
    TEST_ASSERT_EQUAL_STRING("Respectful", i18n_get("sensitivity.respectful"));
    TEST_ASSERT_EQUAL_STRING("Sacred", i18n_get("sensitivity.sacred"));
}

void test_get_locale_fallback(void)
{
    /* Translated locales return locale-specific text.
     * Invalid locale falls back to English. */
    TEST_ASSERT_EQUAL_STRING("En pausa",
                              i18n_get_locale("time.paused", I18N_LOCALE_ES));
    const char *ar = i18n_get_locale("time.paused", I18N_LOCALE_AR);
    TEST_ASSERT_TRUE(strcmp(ar, "Paused") != 0); /* Arabic, not English */
    TEST_ASSERT_EQUAL_STRING("Paused",
                              i18n_get_locale("time.paused", I18N_LOCALE_COUNT));
}

void test_get_locale_unknown_key(void)
{
    /* Unknown key returns key itself */
    TEST_ASSERT_EQUAL_STRING("nonexistent.key",
                              i18n_get_locale("nonexistent.key", I18N_LOCALE_EN));
}

void test_error_keys(void)
{
    TEST_ASSERT_EQUAL_STRING("No data available", i18n_get("error.no_data"));
    TEST_ASSERT_EQUAL_STRING("Invalid date", i18n_get("error.invalid_date"));
}

void test_hierarchical_naming(void)
{
    /* Verify all keys follow dotted hierarchical pattern */
    int count = i18n_key_count();
    for (int i = 0; i < count; i++) {
        i18n_entry_t entry = i18n_entry(i);
        TEST_ASSERT_NOT_NULL(entry.key);
        /* Every key should contain at least one dot */
        int has_dot = 0;
        for (const char *p = entry.key; *p; p++) {
            if (*p == '.') { has_dot = 1; break; }
        }
        TEST_ASSERT_TRUE_MESSAGE(has_dot, entry.key);
    }
}

/* ---- main runner ---- */

int main(void)
{
    UNITY_BEGIN();

    /* i18n_get: system names */
    RUN_TEST(test_get_system_gregorian);
    RUN_TEST(test_get_system_hebrew);
    RUN_TEST(test_get_system_islamic);
    RUN_TEST(test_get_system_chinese);
    RUN_TEST(test_get_system_hindu);
    RUN_TEST(test_get_system_buddhist);
    RUN_TEST(test_get_system_mayan);
    RUN_TEST(test_get_system_coptic);
    RUN_TEST(test_get_system_ethiopian);
    RUN_TEST(test_get_system_zoroastrian);
    RUN_TEST(test_get_system_celtic);
    RUN_TEST(test_get_system_myanmar);
    RUN_TEST(test_get_system_egyptian);
    RUN_TEST(test_get_system_astrology);
    RUN_TEST(test_get_system_iching);
    RUN_TEST(test_get_system_human_design);

    /* i18n_get: UI labels */
    RUN_TEST(test_get_ui_zoom_level);
    RUN_TEST(test_get_ui_view_space);
    RUN_TEST(test_get_ui_view_earth);
    RUN_TEST(test_get_unknown_key_returns_key);
    RUN_TEST(test_get_ui_festival_alert);
    RUN_TEST(test_get_ui_retrograde);
    RUN_TEST(test_get_ui_direct);
    RUN_TEST(test_get_ui_day);
    RUN_TEST(test_get_ui_days);
    RUN_TEST(test_get_ui_month);
    RUN_TEST(test_get_ui_months);
    RUN_TEST(test_get_ui_year);
    RUN_TEST(test_get_ui_years);
    RUN_TEST(test_get_ui_loading);
    RUN_TEST(test_get_ui_error);

    /* i18n_get: edge cases */
    RUN_TEST(test_get_null_returns_empty);
    RUN_TEST(test_get_empty_key_returns_itself);

    /* Key count + exists */
    RUN_TEST(test_key_count_at_least_30);
    RUN_TEST(test_key_exists_valid);
    RUN_TEST(test_key_exists_invalid);
    RUN_TEST(test_key_exists_null);
    RUN_TEST(test_key_exists_empty);

    /* Entry access */
    RUN_TEST(test_entry_valid_index);
    RUN_TEST(test_entry_negative_index);
    RUN_TEST(test_entry_out_of_range);
    RUN_TEST(test_entry_last_valid);

    /* Locale name */
    RUN_TEST(test_locale_name_english);
    RUN_TEST(test_locale_name_arabic);
    RUN_TEST(test_locale_name_hebrew);
    RUN_TEST(test_locale_name_chinese);
    RUN_TEST(test_locale_name_invalid);

    /* Locale code */
    RUN_TEST(test_locale_code_english);
    RUN_TEST(test_locale_code_arabic);
    RUN_TEST(test_locale_code_japanese);
    RUN_TEST(test_locale_code_invalid);

    /* Locale from code */
    RUN_TEST(test_locale_from_code_en);
    RUN_TEST(test_locale_from_code_ar);
    RUN_TEST(test_locale_from_code_zh);
    RUN_TEST(test_locale_from_code_unknown);
    RUN_TEST(test_locale_from_code_null);

    /* RTL detection */
    RUN_TEST(test_is_rtl_arabic);
    RUN_TEST(test_is_rtl_hebrew);
    RUN_TEST(test_is_rtl_english);
    RUN_TEST(test_is_rtl_chinese);
    RUN_TEST(test_is_rtl_hindi);
    RUN_TEST(test_is_rtl_invalid);

    /* Plural rules */
    RUN_TEST(test_plural_rule_english);
    RUN_TEST(test_plural_rule_arabic);
    RUN_TEST(test_plural_rule_chinese);
    RUN_TEST(test_plural_rule_japanese);
    RUN_TEST(test_plural_rule_thai);
    RUN_TEST(test_plural_rule_myanmar);

    /* Plural form: ONE_OTHER */
    RUN_TEST(test_one_other_zero);
    RUN_TEST(test_one_other_one);
    RUN_TEST(test_one_other_two);
    RUN_TEST(test_one_other_many);
    RUN_TEST(test_one_other_negative);

    /* Plural form: EAST_ASIAN */
    RUN_TEST(test_east_asian_zero);
    RUN_TEST(test_east_asian_one);
    RUN_TEST(test_east_asian_many);

    /* Plural form: ARABIC */
    RUN_TEST(test_arabic_zero);
    RUN_TEST(test_arabic_one);
    RUN_TEST(test_arabic_two);
    RUN_TEST(test_arabic_three);
    RUN_TEST(test_arabic_ten);
    RUN_TEST(test_arabic_eleven);
    RUN_TEST(test_arabic_ninety_nine);
    RUN_TEST(test_arabic_hundred);

    /* Plural form: SLAVIC */
    RUN_TEST(test_slavic_one);
    RUN_TEST(test_slavic_two);
    RUN_TEST(test_slavic_four);
    RUN_TEST(test_slavic_five);
    RUN_TEST(test_slavic_eleven);
    RUN_TEST(test_slavic_twelve);
    RUN_TEST(test_slavic_twenty_one);
    RUN_TEST(test_slavic_twenty_two);

    /* Plural form name */
    RUN_TEST(test_form_name_one);
    RUN_TEST(test_form_name_two);
    RUN_TEST(test_form_name_few);
    RUN_TEST(test_form_name_many);
    RUN_TEST(test_form_name_other);
    RUN_TEST(test_form_name_invalid);

    /* Additional locale tests */
    RUN_TEST(test_locale_name_spanish);
    RUN_TEST(test_locale_name_hindi);
    RUN_TEST(test_locale_name_thai);
    RUN_TEST(test_locale_name_amharic);
    RUN_TEST(test_locale_code_spanish);
    RUN_TEST(test_locale_code_hebrew);
    RUN_TEST(test_locale_code_hindi);
    RUN_TEST(test_locale_code_myanmar);
    RUN_TEST(test_locale_from_code_es);
    RUN_TEST(test_locale_from_code_he);
    RUN_TEST(test_locale_from_code_empty);

    /* Additional plural rule tests */
    RUN_TEST(test_plural_rule_spanish);
    RUN_TEST(test_plural_rule_hindi);
    RUN_TEST(test_plural_rule_amharic);

    /* Slavic edge cases */
    RUN_TEST(test_slavic_zero);
    RUN_TEST(test_slavic_fourteen);
    RUN_TEST(test_slavic_hundred_one);
    RUN_TEST(test_slavic_hundred_eleven);

    /* Arabic edge cases */
    RUN_TEST(test_arabic_fifty);
    RUN_TEST(test_arabic_large);
    RUN_TEST(test_arabic_negative);

    /* Invalid rule */
    RUN_TEST(test_plural_form_invalid_rule);

    /* Entry round-trip */
    RUN_TEST(test_entry_key_value_match_get);
    RUN_TEST(test_all_entries_have_valid_key_value);

    /* RTL edge */
    RUN_TEST(test_is_rtl_negative);
    RUN_TEST(test_is_rtl_spanish);

    /* Expanded i18n keys */
    RUN_TEST(test_key_count_expanded);
    RUN_TEST(test_new_system_keys);
    RUN_TEST(test_depth_tier_keys);
    RUN_TEST(test_view_keys);
    RUN_TEST(test_time_control_keys);
    RUN_TEST(test_welcome_keys);
    RUN_TEST(test_planet_keys);
    RUN_TEST(test_sensitivity_keys);
    RUN_TEST(test_get_locale_fallback);
    RUN_TEST(test_get_locale_unknown_key);
    RUN_TEST(test_error_keys);
    RUN_TEST(test_hierarchical_naming);

    return UNITY_END();
}
