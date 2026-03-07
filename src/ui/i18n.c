#include "i18n.h"

#include <string.h>

/* ---- Static translation table ---- */

static const i18n_entry_t default_strings[] = {
    /* System names (16) */
    { "system.gregorian",    "Gregorian" },
    { "system.hebrew",       "Hebrew" },
    { "system.islamic",      "Islamic" },
    { "system.chinese",      "Chinese" },
    { "system.hindu",        "Hindu" },
    { "system.buddhist",     "Buddhist" },
    { "system.mayan",        "Mayan" },
    { "system.coptic",       "Coptic" },
    { "system.ethiopian",    "Ethiopian" },
    { "system.zoroastrian",  "Zoroastrian" },
    { "system.celtic",       "Celtic" },
    { "system.myanmar",      "Myanmar" },
    { "system.egyptian",     "Egyptian" },
    { "system.astrology",    "Astrology" },
    { "system.iching",       "I Ching" },
    { "system.human_design", "Human Design" },

    /* UI labels (14) */
    { "ui.zoom_level",    "Zoom level" },
    { "ui.view_space",    "Space View" },
    { "ui.view_earth",    "Earth View" },
    { "ui.festival_alert","Festival alert" },
    { "ui.retrograde",    "retrograde" },
    { "ui.direct",        "direct" },
    { "ui.day",           "day" },
    { "ui.days",          "days" },
    { "ui.month",         "month" },
    { "ui.months",        "months" },
    { "ui.year",          "year" },
    { "ui.years",         "years" },
    { "ui.loading",       "Loading..." },
    { "ui.error",         "Error" }
};

#define DEFAULT_STRING_COUNT \
    ((int)(sizeof(default_strings) / sizeof(default_strings[0])))

/* ---- Locale data tables ---- */

static const char *locale_names[] = {
    "English",   /* EN */
    "Spanish",   /* ES */
    "Arabic",    /* AR */
    "Hebrew",    /* HE */
    "Chinese",   /* ZH */
    "Hindi",     /* HI */
    "Japanese",  /* JA */
    "Thai",      /* TH */
    "Myanmar",   /* MY */
    "Amharic"    /* AM */
};

static const char *locale_codes[] = {
    "en", "es", "ar", "he", "zh", "hi", "ja", "th", "my", "am"
};

static const i18n_plural_rule_t locale_plural_rules[] = {
    I18N_PLURAL_ONE_OTHER,   /* EN */
    I18N_PLURAL_ONE_OTHER,   /* ES */
    I18N_PLURAL_ARABIC,      /* AR */
    I18N_PLURAL_ONE_OTHER,   /* HE */
    I18N_PLURAL_EAST_ASIAN,  /* ZH */
    I18N_PLURAL_ONE_OTHER,   /* HI */
    I18N_PLURAL_EAST_ASIAN,  /* JA */
    I18N_PLURAL_EAST_ASIAN,  /* TH */
    I18N_PLURAL_EAST_ASIAN,  /* MY */
    I18N_PLURAL_ONE_OTHER    /* AM */
};

static const bool locale_rtl[] = {
    false,  /* EN */
    false,  /* ES */
    true,   /* AR */
    true,   /* HE */
    false,  /* ZH */
    false,  /* HI */
    false,  /* JA */
    false,  /* TH */
    false,  /* MY */
    false   /* AM */
};

static const char *form_names[] = {
    "one", "two", "few", "many", "other"
};

/* ---- Implementation ---- */

const char *i18n_get(const char *key)
{
    if (key == NULL) {
        return "";
    }
    for (int i = 0; i < DEFAULT_STRING_COUNT; i++) {
        if (strcmp(key, default_strings[i].key) == 0) {
            return default_strings[i].value;
        }
    }
    /* Key not found — return the key itself */
    return key;
}

const char *i18n_locale_name(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return "Unknown";
    }
    return locale_names[locale];
}

i18n_plural_rule_t i18n_plural_rule(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return I18N_PLURAL_ONE_OTHER;
    }
    return locale_plural_rules[locale];
}

static i18n_plural_form_t plural_one_other(int count)
{
    if (count == 1) {
        return I18N_FORM_ONE;
    }
    return I18N_FORM_OTHER;
}

static i18n_plural_form_t plural_east_asian(int count)
{
    (void)count;
    return I18N_FORM_OTHER;
}

static i18n_plural_form_t plural_arabic(int count)
{
    if (count < 0) {
        return I18N_FORM_OTHER;
    }
    if (count == 0) {
        return I18N_FORM_OTHER;
    }
    if (count == 1) {
        return I18N_FORM_ONE;
    }
    if (count == 2) {
        return I18N_FORM_TWO;
    }
    if (count >= 3 && count <= 10) {
        return I18N_FORM_FEW;
    }
    if (count >= 11 && count <= 99) {
        return I18N_FORM_MANY;
    }
    return I18N_FORM_OTHER;
}

static i18n_plural_form_t plural_slavic(int count)
{
    if (count < 0) {
        return I18N_FORM_OTHER;
    }

    int mod10 = count % 10;
    int mod100 = count % 100;

    /* Exception: 11-14 are always "many" */
    if (mod100 >= 11 && mod100 <= 14) {
        return I18N_FORM_MANY;
    }
    if (mod10 == 1) {
        return I18N_FORM_ONE;
    }
    if (mod10 >= 2 && mod10 <= 4) {
        return I18N_FORM_FEW;
    }
    return I18N_FORM_MANY;
}

i18n_plural_form_t i18n_plural_form(i18n_plural_rule_t rule, int count)
{
    switch (rule) {
    case I18N_PLURAL_ONE_OTHER:
        return plural_one_other(count);
    case I18N_PLURAL_EAST_ASIAN:
        return plural_east_asian(count);
    case I18N_PLURAL_ARABIC:
        return plural_arabic(count);
    case I18N_PLURAL_SLAVIC:
        return plural_slavic(count);
    default:
        return I18N_FORM_OTHER;
    }
}

const char *i18n_plural_form_name(i18n_plural_form_t form)
{
    if (form < 0 || form >= I18N_FORM_COUNT) {
        return "other";
    }
    return form_names[form];
}

int i18n_key_count(void)
{
    return DEFAULT_STRING_COUNT;
}

i18n_entry_t i18n_entry(int index)
{
    if (index < 0 || index >= DEFAULT_STRING_COUNT) {
        i18n_entry_t empty = { NULL, NULL };
        return empty;
    }
    return default_strings[index];
}

bool i18n_key_exists(const char *key)
{
    if (key == NULL || key[0] == '\0') {
        return false;
    }
    for (int i = 0; i < DEFAULT_STRING_COUNT; i++) {
        if (strcmp(key, default_strings[i].key) == 0) {
            return true;
        }
    }
    return false;
}

i18n_locale_t i18n_locale_from_code(const char *code)
{
    if (code == NULL || code[0] == '\0') {
        return I18N_LOCALE_EN;
    }
    for (int i = 0; i < I18N_LOCALE_COUNT; i++) {
        if (strcmp(code, locale_codes[i]) == 0) {
            return (i18n_locale_t)i;
        }
    }
    return I18N_LOCALE_EN;
}

const char *i18n_locale_code(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return "";
    }
    return locale_codes[locale];
}

bool i18n_is_rtl(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return false;
    }
    return locale_rtl[locale];
}
