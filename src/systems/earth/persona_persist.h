/* persona_persist.h -- Persona data serialization for localStorage.
 * Converts between C structs and flat key=value strings.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PERSONA_PERSIST_H
#define TIME_PERSONA_PERSIST_H

#include "user_prefs.h"
#include "interest_profile.h"
#include "personal_space.h"
#include "privacy_framework.h"

#define PP_MAX_KEY 64
#define PP_MAX_VAL 256
#define PP_MAX_PAIRS 200

typedef struct {
    char key[PP_MAX_KEY];
    char val[PP_MAX_VAL];
} pp_pair_t;

typedef struct {
    pp_pair_t pairs[PP_MAX_PAIRS];
    int count;
} pp_store_t;

/* Create empty store. */
pp_store_t pp_create(void);

/* Set a key-value pair (overwrites if exists). Returns modified store. */
pp_store_t pp_set(pp_store_t store, const char *key, const char *val);

/* Get a value by key. Returns NULL if not found. */
const char *pp_get(const pp_store_t *store, const char *key);

/* Check if key exists. Returns 1/0. */
int pp_has(const pp_store_t *store, const char *key);

/* Remove a key. Returns modified store. */
pp_store_t pp_remove(pp_store_t store, const char *key);

/* Remove all keys with a given prefix. Returns modified store. */
pp_store_t pp_remove_prefix(pp_store_t store, const char *prefix);

/* Count of keys in the store. */
int pp_count(const pp_store_t *store);

/* --- Struct serializers --- */

/* Serialize user_prefs to store. Keys: time.visual.*, time.temporal.*, etc. */
pp_store_t pp_save_prefs(pp_store_t store, const up_prefs_t *prefs);

/* Deserialize user_prefs from store. Missing keys -> defaults. */
up_prefs_t pp_load_prefs(const pp_store_t *store);

/* Serialize birth input data (NOT the computed profile -- only inputs). */
pp_store_t pp_save_birth(pp_store_t store, int year, int month, int day,
                          int has_location, double lat, double lon);

/* Deserialize birth input. Returns 1 if birth data present, 0 if not. */
int pp_load_birth(const pp_store_t *store, int *year, int *month, int *day,
                   int *has_location, double *lat, double *lon);

/* Serialize interest profile. */
pp_store_t pp_save_interests(pp_store_t store, const ip_profile_t *profile);

/* Deserialize interest profile. */
ip_profile_t pp_load_interests(const pp_store_t *store);

/* Serialize personal space. */
pp_store_t pp_save_space(pp_store_t store, const ps_space_t *space);

/* Deserialize personal space. */
ps_space_t pp_load_space(const pp_store_t *store);

/* Serialize privacy settings. */
pp_store_t pp_save_privacy(pp_store_t store, const pf_settings_t *settings);

/* Deserialize privacy settings. */
pf_settings_t pp_load_privacy(const pp_store_t *store);

/* --- Bulk operations --- */

/* Serialize store to text (key=value\n lines). Returns bytes written. */
int pp_serialize(const pp_store_t *store, char *buf, int buf_size);

/* Deserialize text buffer to store. */
pp_store_t pp_deserialize(const char *buf, int buf_len);

/* Export store as JSON. Returns bytes written. */
int pp_export_json(const pp_store_t *store, char *buf, int buf_size);

#endif /* TIME_PERSONA_PERSIST_H */
