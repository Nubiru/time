/* content_i18n.c — Content string translation for knowledge systems.
 *
 * Uses binary search over sorted key-value arrays generated from
 * data/content/ JSON files by scripts/gen_content.py.
 * Falls back: locale -> EN -> key itself. */

#include "content_i18n.h"

#include <string.h>

/* Generated content arrays from data/content/ JSON files */
#include "content_generated.h"

/* ---- Locale dispatch table ---- */

typedef struct {
    const i18n_entry_t *entries;
    int count;
} content_table_t;

static const content_table_t content_tables[I18N_LOCALE_COUNT] = {
    [I18N_LOCALE_EN] = { gen_content_en, GEN_CONTENT_EN_COUNT },
#ifdef GEN_CONTENT_ES_COUNT
    [I18N_LOCALE_ES] = { gen_content_es, GEN_CONTENT_ES_COUNT },
#endif
};

/* ---- Binary search over sorted entry array ---- */

static const char *bsearch_entry(const i18n_entry_t *entries, int count,
                                 const char *key)
{
    int lo = 0;
    int hi = count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = strcmp(key, entries[mid].key);
        if (cmp == 0) {
            return entries[mid].value;
        }
        if (cmp < 0) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return NULL;
}

/* ---- Public API ---- */

const char *content_get(const char *key, i18n_locale_t locale)
{
    if (key == NULL) {
        return "";
    }

    /* Try locale-specific table first (if not EN) */
    if (locale > I18N_LOCALE_EN && locale < I18N_LOCALE_COUNT) {
        const content_table_t *table = &content_tables[locale];
        if (table->entries != NULL && table->count > 0) {
            const char *val = bsearch_entry(table->entries, table->count,
                                            key);
            if (val != NULL) {
                return val;
            }
        }
    }

    /* Fall back to English */
    const char *val = bsearch_entry(gen_content_en, GEN_CONTENT_EN_COUNT,
                                    key);
    if (val != NULL) {
        return val;
    }

    /* Key not found — return the key itself */
    return key;
}

int content_key_count(void)
{
    return GEN_CONTENT_EN_COUNT;
}

bool content_key_exists(const char *key)
{
    if (key == NULL || key[0] == '\0') {
        return false;
    }
    return bsearch_entry(gen_content_en, GEN_CONTENT_EN_COUNT, key) != NULL;
}
