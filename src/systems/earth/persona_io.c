/* persona_io.c -- Load/save persona data via storage bridge.
 *
 * Imperative shell: calls persona_persist (pure serialization) and
 * storage_bridge (platform I/O). No GL, no malloc. */

#include "persona_io.h"
#include "persona_persist.h"
#include "../../platform/storage_bridge.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ----- known keys for loading ----- */

static const char *s_static_keys[] = {
    "time.schema_version",
    "time.first_visit",
    "time.consent_level",
    "time.retention",
    "time.visual.theme",
    "time.visual.font_scale",
    "time.visual.opacity",
    "time.visual.anim_speed",
    "time.visual.layers",
    "time.visual.default_scale",
    "time.temporal.tz_offset",
    "time.temporal.calendars",
    "time.temporal.use_24h",
    "time.temporal.week_start",
    "time.social.activity",
    "time.social.visibility",
    "time.social.notif_freq",
    "time.social.roulette",
    "time.location.lat",
    "time.location.lon",
    "time.location.radius",
    "time.location.set",
    "time.birth.year",
    "time.birth.month",
    "time.birth.day",
    "time.birth.has_location",
    "time.birth.lat",
    "time.birth.lon",
    "time.interests.weights",
    "time.interests.visibility",
    "time.interests.activity",
    "time.interests.radius",
    "time.privacy.fields",
    "time.space.favorites",
    "time.space.explored",
    "time.space.cycle_count",
    "time.space.bookmark_count",
    NULL
};

/* Load all known keys from storage_bridge into a pp_store_t. */
static pp_store_t pio_load_store(void)
{
    pp_store_t store = pp_create();
    char buf[PP_MAX_VAL];

    /* Static keys */
    for (int i = 0; s_static_keys[i] != NULL; i++) {
        int len = sb_load(s_static_keys[i], buf, sizeof(buf));
        if (len > 0) {
            store = pp_set(store, s_static_keys[i], buf);
        }
    }

    /* Dynamic cycle keys */
    int cc_len = sb_load("time.space.cycle_count", buf, sizeof(buf));
    if (cc_len > 0) {
        int cycle_count = atoi(buf);
        for (int n = 0; n < cycle_count && n < PS_MAX_CYCLES; n++) {
            char key[PP_MAX_KEY];
            static const char *suffixes[] = {
                "name", "system", "start", "next", "active", NULL
            };
            for (int s = 0; suffixes[s] != NULL; s++) {
                snprintf(key, sizeof(key), "time.space.cycle.%d.%s",
                         n, suffixes[s]);
                int klen = sb_load(key, buf, sizeof(buf));
                if (klen > 0) {
                    store = pp_set(store, key, buf);
                }
            }
        }
    }

    /* Dynamic bookmark keys */
    int bc_len = sb_load("time.space.bookmark_count", buf, sizeof(buf));
    if (bc_len > 0) {
        int bm_count = atoi(buf);
        for (int n = 0; n < bm_count && n < PS_MAX_BOOKMARKS; n++) {
            char key[PP_MAX_KEY];
            static const char *suffixes[] = {
                "jd", "note", "system", NULL
            };
            for (int s = 0; suffixes[s] != NULL; s++) {
                snprintf(key, sizeof(key), "time.space.bm.%d.%s",
                         n, suffixes[s]);
                int klen = sb_load(key, buf, sizeof(buf));
                if (klen > 0) {
                    store = pp_set(store, key, buf);
                }
            }
        }
    }

    return store;
}

/* Write all pairs from a pp_store_t into storage_bridge. */
static void pio_flush_store(const pp_store_t *store)
{
    for (int i = 0; i < store->count; i++) {
        sb_save(store->pairs[i].key, store->pairs[i].val);
    }
}

/* ----- public API ----- */

int pio_load_all(up_prefs_t *prefs, ip_profile_t *interests,
                 ps_space_t *space, pf_settings_t *privacy,
                 int *birth_year, int *birth_month, int *birth_day,
                 int *birth_has_loc, double *birth_lat, double *birth_lon)
{
    pp_store_t store = pio_load_store();

    if (prefs) {
        *prefs = pp_load_prefs(&store);
    }
    if (interests) {
        *interests = pp_load_interests(&store);
    }
    if (space) {
        *space = pp_load_space(&store);
    }
    if (privacy) {
        *privacy = pp_load_privacy(&store);
    }

    int has_birth = pp_load_birth(&store, birth_year, birth_month, birth_day,
                                  birth_has_loc, birth_lat, birth_lon);
    return has_birth;
}

void pio_save_prefs(const up_prefs_t *prefs)
{
    if (!prefs) return;
    pp_store_t store = pp_create();
    store = pp_save_prefs(store, prefs);
    pio_flush_store(&store);
}

void pio_save_birth(int year, int month, int day,
                    int has_location, double lat, double lon)
{
    pp_store_t store = pp_create();
    store = pp_save_birth(store, year, month, day, has_location, lat, lon);
    pio_flush_store(&store);
}

void pio_save_interests(const ip_profile_t *profile)
{
    if (!profile) return;
    pp_store_t store = pp_create();
    store = pp_save_interests(store, profile);
    pio_flush_store(&store);
}

void pio_save_space(const ps_space_t *space)
{
    if (!space) return;
    pp_store_t store = pp_create();
    store = pp_save_space(store, space);
    pio_flush_store(&store);
}

void pio_save_privacy(const pf_settings_t *settings)
{
    if (!settings) return;
    pp_store_t store = pp_create();
    store = pp_save_privacy(store, settings);
    pio_flush_store(&store);
}

void pio_export(void)
{
    pp_store_t store = pio_load_store();
    char buf[PIO_EXPORT_BUF_SIZE];
    int len = pp_export_json(&store, buf, PIO_EXPORT_BUF_SIZE);
    if (len > 0) {
        sb_download_file("time-persona-export.json", buf, len);
    }
}

void pio_delete_all(void)
{
    sb_delete_all();
}

void pio_check_migration(void)
{
    char buf[PP_MAX_VAL];
    int len = sb_load("time.schema_version", buf, sizeof(buf));
    if (len > 0) {
        int version = atoi(buf);
        if (version >= PIO_SCHEMA_VERSION) {
            return; /* Already current. */
        }
        /* Future: run migration steps from version to PIO_SCHEMA_VERSION.
         * For now, version 1 is the only version, so nothing to migrate. */
    }

    /* Write current schema version. */
    char ver_str[16];
    snprintf(ver_str, sizeof(ver_str), "%d", PIO_SCHEMA_VERSION);
    sb_save("time.schema_version", ver_str);
}
