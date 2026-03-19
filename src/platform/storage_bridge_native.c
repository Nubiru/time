/* storage_bridge_native.c — In-memory storage implementation for native builds.
 * Used for testing. WASM builds use a separate Emscripten-based implementation. */

#include "storage_bridge.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    char key[SB_MAX_KEY];
    char val[SB_MAX_VAL];
    int  occupied;
} sb_entry_t;

static sb_entry_t s_store[SB_MAX_ENTRIES];
static int        s_count = 0;

/* Find index of key, or -1 if not found. */
static int sb_find(const char *key)
{
    for (int i = 0; i < SB_MAX_ENTRIES; i++) {
        if (s_store[i].occupied && strcmp(s_store[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

void sb_save(const char *key, const char *value)
{
    if (!key || !value) {
        return;
    }

    /* Overwrite if key already exists. */
    int idx = sb_find(key);
    if (idx >= 0) {
        snprintf(s_store[idx].val, SB_MAX_VAL, "%s", value);
        return;
    }

    /* Find first empty slot. */
    for (int i = 0; i < SB_MAX_ENTRIES; i++) {
        if (!s_store[i].occupied) {
            snprintf(s_store[i].key, SB_MAX_KEY, "%s", key);
            snprintf(s_store[i].val, SB_MAX_VAL, "%s", value);
            s_store[i].occupied = 1;
            s_count++;
            return;
        }
    }
    /* Store full — silently drop. */
}

int sb_load(const char *key, char *buf, int buf_size)
{
    if (!key || !buf || buf_size <= 0) {
        return 0;
    }

    int idx = sb_find(key);
    if (idx < 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size, "%s", s_store[idx].val);
    if (written < 0) {
        return 0;
    }
    /* snprintf returns what would have been written; clamp to actual buffer. */
    if (written >= buf_size) {
        return buf_size - 1;
    }
    return written;
}

int sb_exists(const char *key)
{
    if (!key) {
        return 0;
    }
    return sb_find(key) >= 0 ? 1 : 0;
}

void sb_delete(const char *key)
{
    if (!key) {
        return;
    }

    int idx = sb_find(key);
    if (idx < 0) {
        return;
    }

    /* Mark slot empty. */
    s_store[idx].occupied = 0;
    s_store[idx].key[0]   = '\0';
    s_store[idx].val[0]   = '\0';
    s_count--;

    /* Compact: move last occupied entry into the gap. */
    for (int i = SB_MAX_ENTRIES - 1; i > idx; i--) {
        if (s_store[i].occupied) {
            memcpy(&s_store[idx], &s_store[i], sizeof(sb_entry_t));
            s_store[i].occupied = 0;
            s_store[i].key[0]   = '\0';
            s_store[i].val[0]   = '\0';
            break;
        }
    }
}

void sb_delete_all(void)
{
    for (int i = 0; i < SB_MAX_ENTRIES; i++) {
        if (s_store[i].occupied &&
            strncmp(s_store[i].key, "time.", 5) == 0) {
            s_store[i].occupied = 0;
            s_store[i].key[0]   = '\0';
            s_store[i].val[0]   = '\0';
            s_count--;
        }
    }
}

int sb_count(void)
{
    return s_count;
}

int sb_export_json(char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }

    int pos = 0;
    int ret;

    ret = snprintf(buf + pos, (size_t)(buf_size - pos), "{");
    if (ret < 0 || ret >= buf_size - pos) {
        return 0;
    }
    pos += ret;

    int first = 1;
    for (int i = 0; i < SB_MAX_ENTRIES; i++) {
        if (!s_store[i].occupied) {
            continue;
        }

        ret = snprintf(buf + pos, (size_t)(buf_size - pos),
                       "%s\"%s\":\"%s\"",
                       first ? "" : ",",
                       s_store[i].key, s_store[i].val);
        if (ret < 0 || ret >= buf_size - pos) {
            /* Truncated — close brace and return what we have. */
            if (pos < buf_size - 1) {
                buf[pos] = '}';
                buf[pos + 1] = '\0';
                return pos + 1;
            }
            return pos;
        }
        pos += ret;
        first = 0;
    }

    ret = snprintf(buf + pos, (size_t)(buf_size - pos), "}");
    if (ret < 0 || ret >= buf_size - pos) {
        return pos;
    }
    pos += ret;

    return pos;
}

void sb_download_file(const char *filename, const char *data, int data_len)
{
    /* No-op on native. */
    (void)filename;
    (void)data;
    (void)data_len;
}

void sb_request_geolocation(sb_geo_callback_t callback)
{
    if (callback) {
        callback(1, 0.0, 0.0);
    }
}

void sb_reset(void)
{
    memset(s_store, 0, sizeof(s_store));
    s_count = 0;
}
