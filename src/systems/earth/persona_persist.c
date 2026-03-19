/* persona_persist.c -- Persona data serialization for localStorage.
 *
 * Converts between C structs and flat key=value strings.
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * All buffers are fixed-size, all strings use snprintf. */

#include "persona_persist.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ----- helpers ----- */

static void safe_strcpy(char *dst, const char *src, int max) {
    if (!src) {
        dst[0] = '\0';
        return;
    }
    int i = 0;
    while (i < max - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* Find index of key in store. Returns -1 if not found. */
static int find_key(const pp_store_t *store, const char *key) {
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->pairs[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

/* Write int to store. */
static pp_store_t set_int(pp_store_t store, const char *key, int val) {
    char buf[PP_MAX_VAL];
    snprintf(buf, PP_MAX_VAL, "%d", val);
    return pp_set(store, key, buf);
}

/* Write float/double to store. */
static pp_store_t set_double(pp_store_t store, const char *key, double val) {
    char buf[PP_MAX_VAL];
    snprintf(buf, PP_MAX_VAL, "%.10g", val);
    return pp_set(store, key, buf);
}

/* Read int from store with default. */
static int get_int(const pp_store_t *store, const char *key, int def) {
    const char *v = pp_get(store, key);
    if (!v) return def;
    return atoi(v);
}

/* Read double from store with default. */
static double get_double(const pp_store_t *store, const char *key, double def) {
    const char *v = pp_get(store, key);
    if (!v) return def;
    return strtod(v, NULL);
}

/* Write a CSV of ints. */
static pp_store_t set_csv_ints(pp_store_t store, const char *key,
                                const int *arr, int count) {
    char buf[PP_MAX_VAL];
    int pos = 0;
    for (int i = 0; i < count; i++) {
        if (i > 0 && pos < PP_MAX_VAL - 1) {
            buf[pos++] = ',';
        }
        int written = snprintf(buf + pos, (size_t)(PP_MAX_VAL - pos), "%d", arr[i]);
        if (written < 0 || pos + written >= PP_MAX_VAL) break;
        pos += written;
    }
    buf[pos] = '\0';
    return pp_set(store, key, buf);
}

/* Parse CSV of ints. Returns count parsed. */
static int parse_csv_ints(const char *csv, int *out, int max) {
    if (!csv || csv[0] == '\0') return 0;
    int count = 0;
    const char *p = csv;
    while (*p != '\0' && count < max) {
        /* skip leading whitespace */
        while (*p == ' ') p++;
        if (*p == '\0') break;
        char *end = NULL;
        long val = strtol(p, &end, 10);
        if (end == p) break; /* no digit found */
        out[count++] = (int)val;
        p = end;
        if (*p == ',') p++;
    }
    return count;
}

/* Write a CSV of doubles. */
static pp_store_t set_csv_doubles(pp_store_t store, const char *key,
                                   const double *arr, int count) {
    char buf[PP_MAX_VAL];
    int pos = 0;
    for (int i = 0; i < count; i++) {
        if (i > 0 && pos < PP_MAX_VAL - 1) {
            buf[pos++] = ',';
        }
        int written = snprintf(buf + pos, (size_t)(PP_MAX_VAL - pos),
                               "%.10g", arr[i]);
        if (written < 0 || pos + written >= PP_MAX_VAL) break;
        pos += written;
    }
    buf[pos] = '\0';
    return pp_set(store, key, buf);
}

/* Parse CSV of doubles. Returns count parsed. */
static int parse_csv_doubles(const char *csv, double *out, int max) {
    if (!csv || csv[0] == '\0') return 0;
    int count = 0;
    const char *p = csv;
    while (*p != '\0' && count < max) {
        while (*p == ' ') p++;
        if (*p == '\0') break;
        char *end = NULL;
        double val = strtod(p, &end);
        if (end == p) break;
        out[count++] = val;
        p = end;
        if (*p == ',') p++;
    }
    return count;
}

/* Escape a value for text serialization. \\= for literal =, \\n for newline.
 * Returns chars written (excluding null). */
static int escape_val(const char *src, char *dst, int dst_size) {
    int si = 0;
    int di = 0;
    while (src[si] != '\0' && di < dst_size - 2) {
        if (src[si] == '=') {
            dst[di++] = '\\';
            if (di < dst_size - 1) dst[di++] = '=';
        } else if (src[si] == '\n') {
            dst[di++] = '\\';
            if (di < dst_size - 1) dst[di++] = 'n';
        } else if (src[si] == '\\') {
            dst[di++] = '\\';
            if (di < dst_size - 1) dst[di++] = '\\';
        } else {
            dst[di++] = src[si];
        }
        si++;
    }
    dst[di] = '\0';
    return di;
}

/* Unescape a value from text serialization. Returns chars written. */
static int unescape_val(const char *src, int src_len, char *dst, int dst_size) {
    int si = 0;
    int di = 0;
    while (si < src_len && di < dst_size - 1) {
        if (src[si] == '\\' && si + 1 < src_len) {
            if (src[si + 1] == '=') {
                dst[di++] = '=';
                si += 2;
            } else if (src[si + 1] == 'n') {
                dst[di++] = '\n';
                si += 2;
            } else if (src[si + 1] == '\\') {
                dst[di++] = '\\';
                si += 2;
            } else {
                dst[di++] = src[si++];
            }
        } else {
            dst[di++] = src[si++];
        }
    }
    dst[di] = '\0';
    return di;
}

/* Escape a value for JSON string. Returns chars written. */
static int json_escape(const char *src, char *dst, int dst_size) {
    int si = 0;
    int di = 0;
    while (src[si] != '\0' && di < dst_size - 2) {
        char c = src[si];
        if (c == '"' || c == '\\') {
            dst[di++] = '\\';
            if (di < dst_size - 1) dst[di++] = c;
        } else if (c == '\n') {
            dst[di++] = '\\';
            if (di < dst_size - 1) dst[di++] = 'n';
        } else if (c == '\t') {
            dst[di++] = '\\';
            if (di < dst_size - 1) dst[di++] = 't';
        } else {
            dst[di++] = c;
        }
        si++;
    }
    dst[di] = '\0';
    return di;
}

/* ----- core API ----- */

pp_store_t pp_create(void) {
    pp_store_t store;
    memset(&store, 0, sizeof(store));
    store.count = 0;
    return store;
}

pp_store_t pp_set(pp_store_t store, const char *key, const char *val) {
    if (!key || !val) return store;
    int idx = find_key(&store, key);
    if (idx >= 0) {
        /* overwrite existing */
        safe_strcpy(store.pairs[idx].val, val, PP_MAX_VAL);
        return store;
    }
    /* add new */
    if (store.count >= PP_MAX_PAIRS) {
        return store; /* silently refuse -- capacity reached */
    }
    safe_strcpy(store.pairs[store.count].key, key, PP_MAX_KEY);
    safe_strcpy(store.pairs[store.count].val, val, PP_MAX_VAL);
    store.count++;
    return store;
}

const char *pp_get(const pp_store_t *store, const char *key) {
    if (!store || !key) return NULL;
    int idx = find_key(store, key);
    if (idx < 0) return NULL;
    return store->pairs[idx].val;
}

int pp_has(const pp_store_t *store, const char *key) {
    if (!store || !key) return 0;
    return find_key(store, key) >= 0 ? 1 : 0;
}

pp_store_t pp_remove(pp_store_t store, const char *key) {
    if (!key) return store;
    int idx = find_key(&store, key);
    if (idx < 0) return store;
    /* shift remaining pairs down */
    for (int i = idx; i < store.count - 1; i++) {
        store.pairs[i] = store.pairs[i + 1];
    }
    store.count--;
    return store;
}

pp_store_t pp_remove_prefix(pp_store_t store, const char *prefix) {
    if (!prefix) return store;
    int plen = (int)strlen(prefix);
    int i = 0;
    while (i < store.count) {
        if (strncmp(store.pairs[i].key, prefix, (size_t)plen) == 0) {
            /* shift remaining down */
            for (int j = i; j < store.count - 1; j++) {
                store.pairs[j] = store.pairs[j + 1];
            }
            store.count--;
            /* don't increment i -- check the new element at this position */
        } else {
            i++;
        }
    }
    return store;
}

int pp_count(const pp_store_t *store) {
    if (!store) return 0;
    return store->count;
}

/* ----- struct serializers ----- */

pp_store_t pp_save_prefs(pp_store_t store, const up_prefs_t *prefs) {
    if (!prefs) return store;

    /* Visual */
    store = set_int(store, "time.visual.theme", (int)prefs->visual.theme);
    store = set_double(store, "time.visual.font_scale",
                       (double)prefs->visual.font_scale);
    store = set_double(store, "time.visual.opacity",
                       (double)prefs->visual.opacity_scale);
    store = set_double(store, "time.visual.anim_speed",
                       (double)prefs->visual.animation_speed);
    store = set_int(store, "time.visual.layers", prefs->visual.layers_visible);
    store = set_int(store, "time.visual.default_scale",
                    prefs->visual.default_scale_level);

    /* Temporal */
    store = set_int(store, "time.temporal.tz_offset",
                    prefs->temporal.home_timezone_offset_min);
    store = set_csv_ints(store, "time.temporal.calendars",
                         prefs->temporal.preferred_calendars,
                         prefs->temporal.preferred_calendar_count);
    store = set_int(store, "time.temporal.use_24h",
                    prefs->temporal.use_24h_time);
    store = set_int(store, "time.temporal.week_start",
                    prefs->temporal.week_start_day);

    /* Social */
    store = set_int(store, "time.social.activity",
                    (int)prefs->social.activity_level);
    store = set_int(store, "time.social.visibility",
                    (int)prefs->social.visibility);
    store = set_int(store, "time.social.notif_freq",
                    prefs->social.notification_frequency);
    store = set_int(store, "time.social.roulette",
                    prefs->social.roulette_opt_in);

    /* Location */
    store = set_double(store, "time.location.lat", prefs->location.home_lat);
    store = set_double(store, "time.location.lon", prefs->location.home_lon);
    store = set_double(store, "time.location.radius",
                       prefs->location.exploration_radius_km);
    store = set_int(store, "time.location.set", prefs->location.location_set);

    return store;
}

up_prefs_t pp_load_prefs(const pp_store_t *store) {
    up_prefs_t p = up_default();
    if (!store) return p;

    /* Visual */
    p.visual.theme = (theme_id_t)get_int(store, "time.visual.theme",
                                         (int)p.visual.theme);
    p.visual.font_scale = (float)get_double(store, "time.visual.font_scale",
                                            (double)p.visual.font_scale);
    p.visual.opacity_scale = (float)get_double(store, "time.visual.opacity",
                                               (double)p.visual.opacity_scale);
    p.visual.animation_speed = (float)get_double(
        store, "time.visual.anim_speed", (double)p.visual.animation_speed);
    p.visual.layers_visible = get_int(store, "time.visual.layers",
                                      p.visual.layers_visible);
    p.visual.default_scale_level = get_int(store, "time.visual.default_scale",
                                           p.visual.default_scale_level);

    /* Temporal */
    p.temporal.home_timezone_offset_min = get_int(
        store, "time.temporal.tz_offset",
        p.temporal.home_timezone_offset_min);
    p.temporal.use_24h_time = get_int(store, "time.temporal.use_24h",
                                      p.temporal.use_24h_time);
    p.temporal.week_start_day = get_int(store, "time.temporal.week_start",
                                        p.temporal.week_start_day);

    /* Parse calendar CSV */
    const char *cal_csv = pp_get(store, "time.temporal.calendars");
    if (cal_csv) {
        int cals[UP_MAX_CALENDAR_SYSTEMS];
        int n = parse_csv_ints(cal_csv, cals, UP_MAX_CALENDAR_SYSTEMS);
        p.temporal.preferred_calendar_count = n;
        for (int i = 0; i < n && i < UP_MAX_CALENDAR_SYSTEMS; i++) {
            p.temporal.preferred_calendars[i] = cals[i];
        }
        for (int i = n; i < UP_MAX_CALENDAR_SYSTEMS; i++) {
            p.temporal.preferred_calendars[i] = -1;
        }
    }

    /* Social */
    p.social.activity_level = (ip_activity_t)get_int(
        store, "time.social.activity", (int)p.social.activity_level);
    p.social.visibility = (ip_visibility_t)get_int(
        store, "time.social.visibility", (int)p.social.visibility);
    p.social.notification_frequency = get_int(
        store, "time.social.notif_freq", p.social.notification_frequency);
    p.social.roulette_opt_in = get_int(store, "time.social.roulette",
                                       p.social.roulette_opt_in);

    /* Location */
    p.location.home_lat = get_double(store, "time.location.lat",
                                     p.location.home_lat);
    p.location.home_lon = get_double(store, "time.location.lon",
                                     p.location.home_lon);
    p.location.exploration_radius_km = get_double(
        store, "time.location.radius", p.location.exploration_radius_km);
    p.location.location_set = get_int(store, "time.location.set",
                                      p.location.location_set);

    return p;
}

pp_store_t pp_save_birth(pp_store_t store, int year, int month, int day,
                          int has_location, double lat, double lon) {
    store = set_int(store, "time.birth.year", year);
    store = set_int(store, "time.birth.month", month);
    store = set_int(store, "time.birth.day", day);
    store = set_int(store, "time.birth.has_location", has_location);
    if (has_location) {
        store = set_double(store, "time.birth.lat", lat);
        store = set_double(store, "time.birth.lon", lon);
    }
    return store;
}

int pp_load_birth(const pp_store_t *store, int *year, int *month, int *day,
                   int *has_location, double *lat, double *lon) {
    if (!store) return 0;
    if (!pp_has(store, "time.birth.year")) return 0;

    if (year) *year = get_int(store, "time.birth.year", 0);
    if (month) *month = get_int(store, "time.birth.month", 0);
    if (day) *day = get_int(store, "time.birth.day", 0);
    int hl = get_int(store, "time.birth.has_location", 0);
    if (has_location) *has_location = hl;
    if (hl) {
        if (lat) *lat = get_double(store, "time.birth.lat", 0.0);
        if (lon) *lon = get_double(store, "time.birth.lon", 0.0);
    } else {
        if (lat) *lat = 0.0;
        if (lon) *lon = 0.0;
    }
    return 1;
}

pp_store_t pp_save_interests(pp_store_t store, const ip_profile_t *profile) {
    if (!profile) return store;
    store = set_csv_doubles(store, "time.interests.weights",
                            profile->weights, IP_MAX_INTERESTS);
    store = set_int(store, "time.interests.visibility",
                    (int)profile->visibility);
    store = set_int(store, "time.interests.activity",
                    (int)profile->activity_level);
    store = set_double(store, "time.interests.radius",
                       profile->location_radius_km);
    return store;
}

ip_profile_t pp_load_interests(const pp_store_t *store) {
    ip_profile_t p = ip_default_profile();
    if (!store) return p;

    const char *csv = pp_get(store, "time.interests.weights");
    if (csv) {
        parse_csv_doubles(csv, p.weights, IP_MAX_INTERESTS);
    }
    p.visibility = (ip_visibility_t)get_int(store, "time.interests.visibility",
                                            (int)p.visibility);
    p.activity_level = (ip_activity_t)get_int(store, "time.interests.activity",
                                              (int)p.activity_level);
    p.location_radius_km = get_double(store, "time.interests.radius",
                                      p.location_radius_km);
    return p;
}

pp_store_t pp_save_space(pp_store_t store, const ps_space_t *space) {
    if (!space) return store;

    /* Favorites */
    store = set_csv_ints(store, "time.space.favorites",
                         space->favorite_systems, space->favorite_count);

    /* Explored */
    store = set_csv_ints(store, "time.space.explored",
                         space->explored_systems, space->explored_count);

    /* Cycles */
    store = set_int(store, "time.space.cycle_count", space->cycle_count);
    for (int i = 0; i < space->cycle_count; i++) {
        char key[PP_MAX_KEY];
        const ps_cycle_t *c = &space->cycles[i];
        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.name", i);
        store = pp_set(store, key, c->name);
        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.system", i);
        store = set_int(store, key, c->system_id);
        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.start", i);
        store = set_double(store, key, c->start_jd);
        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.next", i);
        store = set_double(store, key, c->next_event_jd);
        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.active", i);
        store = set_int(store, key, c->active);
    }

    /* Bookmarks */
    store = set_int(store, "time.space.bookmark_count",
                    space->bookmark_count);
    for (int i = 0; i < space->bookmark_count; i++) {
        char key[PP_MAX_KEY];
        const ps_bookmark_t *b = &space->bookmarks[i];
        snprintf(key, PP_MAX_KEY, "time.space.bm.%d.jd", i);
        store = set_double(store, key, b->jd);
        snprintf(key, PP_MAX_KEY, "time.space.bm.%d.note", i);
        store = pp_set(store, key, b->note);
        snprintf(key, PP_MAX_KEY, "time.space.bm.%d.system", i);
        store = set_int(store, key, b->system_id);
    }

    return store;
}

ps_space_t pp_load_space(const pp_store_t *store) {
    ps_space_t s = ps_create();
    if (!store) return s;

    /* Favorites */
    const char *fav_csv = pp_get(store, "time.space.favorites");
    if (fav_csv) {
        s.favorite_count = parse_csv_ints(fav_csv, s.favorite_systems,
                                          PS_MAX_FAVORITES);
        /* add -1 terminator if room */
        if (s.favorite_count < PS_MAX_FAVORITES) {
            s.favorite_systems[s.favorite_count] = -1;
        }
    }

    /* Explored */
    const char *exp_csv = pp_get(store, "time.space.explored");
    if (exp_csv) {
        s.explored_count = parse_csv_ints(exp_csv, s.explored_systems,
                                          PS_MAX_EXPLORED);
    }

    /* Cycles */
    int cc = get_int(store, "time.space.cycle_count", 0);
    if (cc > PS_MAX_CYCLES) cc = PS_MAX_CYCLES;
    s.cycle_count = cc;
    for (int i = 0; i < cc; i++) {
        char key[PP_MAX_KEY];
        ps_cycle_t *c = &s.cycles[i];

        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.name", i);
        const char *name = pp_get(store, key);
        safe_strcpy(c->name, name ? name : "", PS_CYCLE_NAME_MAX);

        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.system", i);
        c->system_id = get_int(store, key, -1);

        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.start", i);
        c->start_jd = get_double(store, key, 0.0);

        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.next", i);
        c->next_event_jd = get_double(store, key, 0.0);

        snprintf(key, PP_MAX_KEY, "time.space.cycle.%d.active", i);
        c->active = get_int(store, key, 0);

        c->end_jd = 0.0; /* not persisted -- computed at runtime */
    }

    /* Bookmarks */
    int bc = get_int(store, "time.space.bookmark_count", 0);
    if (bc > PS_MAX_BOOKMARKS) bc = PS_MAX_BOOKMARKS;
    s.bookmark_count = bc;
    for (int i = 0; i < bc; i++) {
        char key[PP_MAX_KEY];
        ps_bookmark_t *b = &s.bookmarks[i];

        snprintf(key, PP_MAX_KEY, "time.space.bm.%d.jd", i);
        b->jd = get_double(store, key, 0.0);

        snprintf(key, PP_MAX_KEY, "time.space.bm.%d.note", i);
        const char *note = pp_get(store, key);
        safe_strcpy(b->note, note ? note : "", PS_NOTE_MAX);

        snprintf(key, PP_MAX_KEY, "time.space.bm.%d.system", i);
        b->system_id = get_int(store, key, -1);
    }

    return s;
}

pp_store_t pp_save_privacy(pp_store_t store, const pf_settings_t *settings) {
    if (!settings) return store;
    store = set_csv_ints(store, "time.privacy.fields",
                         settings->fields_shared, PF_FIELD_COUNT);
    store = set_int(store, "time.consent_level", (int)settings->consent);
    store = set_int(store, "time.retention", (int)settings->retention);
    return store;
}

pf_settings_t pp_load_privacy(const pp_store_t *store) {
    pf_settings_t s = pf_default_settings();
    if (!store) return s;

    const char *csv = pp_get(store, "time.privacy.fields");
    if (csv) {
        parse_csv_ints(csv, s.fields_shared, PF_FIELD_COUNT);
    }
    s.consent = (pf_consent_t)get_int(store, "time.consent_level",
                                      (int)s.consent);
    s.retention = (pf_retention_t)get_int(store, "time.retention",
                                          (int)s.retention);
    return s;
}

/* ----- bulk operations ----- */

int pp_serialize(const pp_store_t *store, char *buf, int buf_size) {
    if (!store || !buf || buf_size <= 0) return 0;
    int pos = 0;
    for (int i = 0; i < store->count; i++) {
        /* Escape the value */
        char escaped_val[PP_MAX_VAL * 2];
        escape_val(store->pairs[i].val, escaped_val,
                   (int)sizeof(escaped_val));

        int needed = snprintf(buf + pos, (size_t)(buf_size - pos),
                              "%s=%s\n",
                              store->pairs[i].key, escaped_val);
        if (needed < 0 || pos + needed >= buf_size) {
            buf[pos] = '\0';
            return pos;
        }
        pos += needed;
    }
    return pos;
}

pp_store_t pp_deserialize(const char *buf, int buf_len) {
    pp_store_t store = pp_create();
    if (!buf || buf_len <= 0) return store;

    int i = 0;
    while (i < buf_len) {
        /* Find end of line */
        int line_start = i;
        while (i < buf_len && buf[i] != '\n') i++;
        int line_len = i - line_start;
        if (i < buf_len) i++; /* skip newline */

        if (line_len == 0) continue;

        /* Find first unescaped '=' */
        int eq_pos = -1;
        for (int j = 0; j < line_len; j++) {
            if (buf[line_start + j] == '\\' && j + 1 < line_len) {
                j++; /* skip escaped char */
                continue;
            }
            if (buf[line_start + j] == '=') {
                eq_pos = j;
                break;
            }
        }
        if (eq_pos <= 0) continue; /* no key or no '=' */

        /* Extract key (no escaping needed in keys) */
        char key[PP_MAX_KEY];
        int klen = eq_pos;
        if (klen >= PP_MAX_KEY) klen = PP_MAX_KEY - 1;
        memcpy(key, buf + line_start, (size_t)klen);
        key[klen] = '\0';

        /* Extract and unescape value */
        char val[PP_MAX_VAL];
        int val_start = eq_pos + 1;
        int val_len = line_len - val_start;
        unescape_val(buf + line_start + val_start, val_len, val, PP_MAX_VAL);

        store = pp_set(store, key, val);
    }

    return store;
}

int pp_export_json(const pp_store_t *store, char *buf, int buf_size) {
    if (!store || !buf || buf_size <= 0) return 0;
    int pos = 0;

    /* Opening brace */
    int n = snprintf(buf + pos, (size_t)(buf_size - pos), "{\n");
    if (n < 0 || pos + n >= buf_size) return pos;
    pos += n;

    for (int i = 0; i < store->count; i++) {
        char escaped_key[PP_MAX_KEY * 2];
        char escaped_val[PP_MAX_VAL * 2];
        json_escape(store->pairs[i].key, escaped_key,
                    (int)sizeof(escaped_key));
        json_escape(store->pairs[i].val, escaped_val,
                    (int)sizeof(escaped_val));

        const char *comma = (i < store->count - 1) ? "," : "";
        n = snprintf(buf + pos, (size_t)(buf_size - pos),
                     "  \"%s\": \"%s\"%s\n",
                     escaped_key, escaped_val, comma);
        if (n < 0 || pos + n >= buf_size) {
            buf[pos] = '\0';
            return pos;
        }
        pos += n;
    }

    /* Closing brace */
    n = snprintf(buf + pos, (size_t)(buf_size - pos), "}\n");
    if (n < 0 || pos + n >= buf_size) return pos;
    pos += n;

    return pos;
}
