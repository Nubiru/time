/* content_i18n.h — Content string translation for knowledge systems.
 *
 * Provides locale-aware lookup for domain-specific content strings
 * (month names, season descriptions, calendar terms, narrative templates).
 * Complements i18n.h which handles UI labels.
 *
 * Pure module: static const tables, binary search, no allocation,
 * no globals, no side effects. */

#ifndef TIME_CONTENT_I18N_H
#define TIME_CONTENT_I18N_H

#include "i18n.h"
#include <stdbool.h>

/* Get a content string by key and locale.
 * Falls back: requested locale -> EN -> key itself.
 * Returns pointer to static string (no allocation).
 * Returns "" for NULL key. */
const char *content_get(const char *key, i18n_locale_t locale);

/* Total number of English content keys. */
int content_key_count(void);

/* Check if a content key exists in English defaults. */
bool content_key_exists(const char *key);

#endif /* TIME_CONTENT_I18N_H */
