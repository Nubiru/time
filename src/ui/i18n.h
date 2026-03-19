#ifndef TIME_I18N_H
#define TIME_I18N_H

/* Translation Keys (i18n) — internationalization support.
 * Pure module: static const string tables, no allocation,
 * no globals, no side effects. Provides keyed lookup for
 * all user-facing strings with default English values.
 * Supports pluralization rules for different language families. */

#include <stdbool.h>

#define I18N_MAX_KEY_LEN    64
#define I18N_MAX_VALUE_LEN  128
#define I18N_MAX_KEYS       256

/* Locale identifier */
typedef enum {
    I18N_LOCALE_EN = 0,      /* English (default) */
    I18N_LOCALE_ES,          /* Spanish */
    I18N_LOCALE_AR,          /* Arabic */
    I18N_LOCALE_HE,          /* Hebrew */
    I18N_LOCALE_ZH,          /* Chinese */
    I18N_LOCALE_HI,          /* Hindi */
    I18N_LOCALE_JA,          /* Japanese */
    I18N_LOCALE_TH,          /* Thai */
    I18N_LOCALE_MY,          /* Myanmar/Burmese */
    I18N_LOCALE_AM,          /* Amharic */
    I18N_LOCALE_COUNT
} i18n_locale_t;

/* Plural form rule */
typedef enum {
    I18N_PLURAL_ONE_OTHER = 0,     /* English/Germanic: 1=one, else other */
    I18N_PLURAL_EAST_ASIAN,        /* Chinese/Japanese/Thai: no plural */
    I18N_PLURAL_ARABIC,            /* Arabic: 0,1,2,few(3-10),many(11-99),other */
    I18N_PLURAL_SLAVIC,            /* Russian: 1=one, 2-4=few, 5-20=many, repeat */
    I18N_PLURAL_RULE_COUNT
} i18n_plural_rule_t;

/* Plural form category */
typedef enum {
    I18N_FORM_ONE = 0,     /* singular */
    I18N_FORM_TWO,         /* dual (Arabic) */
    I18N_FORM_FEW,         /* few */
    I18N_FORM_MANY,        /* many */
    I18N_FORM_OTHER,       /* default/plural */
    I18N_FORM_COUNT
} i18n_plural_form_t;

/* Translation entry */
typedef struct {
    const char *key;       /* e.g. "system.gregorian" */
    const char *value;     /* e.g. "Gregorian" */
} i18n_entry_t;

/* Get a translated string by key. Returns the key itself if not found.
 * Returns pointer to static string (no allocation).
 * Returns "" for NULL key. */
const char *i18n_get(const char *key);

/* Get locale display name. Returns "Unknown" for invalid locale. */
const char *i18n_locale_name(i18n_locale_t locale);

/* Get the plural rule for a locale. */
i18n_plural_rule_t i18n_plural_rule(i18n_locale_t locale);

/* Get plural form for a number under a given rule.
 * Negative counts return OTHER. */
i18n_plural_form_t i18n_plural_form(i18n_plural_rule_t rule, int count);

/* Plural form name string ("one", "two", "few", "many", "other"). */
const char *i18n_plural_form_name(i18n_plural_form_t form);

/* Get total number of default translation keys. */
int i18n_key_count(void);

/* Get a translation entry by index (0-based).
 * Returns {NULL, NULL} for invalid index. */
i18n_entry_t i18n_entry(int index);

/* Check if a key exists in the default strings. */
bool i18n_key_exists(const char *key);

/* Get locale enum from ISO 639-1 code string ("en", "ar", etc.).
 * Returns I18N_LOCALE_EN for unknown or NULL codes. */
i18n_locale_t i18n_locale_from_code(const char *code);

/* Get ISO 639-1 code for a locale. Returns "" for invalid. */
const char *i18n_locale_code(i18n_locale_t locale);

/* Is this locale RTL (right-to-left)? */
bool i18n_is_rtl(i18n_locale_t locale);

/* Get a translated string for a specific locale. Falls back:
 * requested locale -> English -> key itself.
 * Currently only English translations are stored, so this
 * always returns the English value. Future: per-locale tables. */
const char *i18n_get_locale(const char *key, i18n_locale_t locale);

#endif /* TIME_I18N_H */
