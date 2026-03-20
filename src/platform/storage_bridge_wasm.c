/* storage_bridge_wasm.c — Browser localStorage implementation.
 * WASM-only (S1): uses EM_ASM, Emscripten APIs, browser localStorage.
 * Mirrors the storage_bridge_native.c API contract. */

#ifdef __EMSCRIPTEN__

#include "storage_bridge.h"
#include <emscripten.h>
#include <string.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/* sb_save                                                              */
/* ------------------------------------------------------------------ */

void sb_save(const char *key, const char *value)
{
    if (!key || !value) return;
    EM_ASM({
        try {
            localStorage.setItem(UTF8ToString($0), UTF8ToString($1));
        } catch (e) {
            /* QuotaExceededError — silently drop like native when full. */
        }
    }, key, value);
}

/* ------------------------------------------------------------------ */
/* sb_load                                                              */
/* ------------------------------------------------------------------ */

int sb_load(const char *key, char *buf, int buf_size)
{
    if (!key || !buf || buf_size <= 0) return 0;
    buf[0] = '\0';

    int written = EM_ASM_INT({
        var val = localStorage.getItem(UTF8ToString($0));
        if (val === null) return 0;
        var maxLen = $2 - 1; /* leave room for null terminator */
        if (maxLen <= 0) return 0;
        var len = lengthBytesUTF8(val);
        if (len > maxLen) {
            /* Truncate: encode only what fits. */
            val = val.substring(0, maxLen);
            len = lengthBytesUTF8(val);
            while (len > maxLen) {
                val = val.substring(0, val.length - 1);
                len = lengthBytesUTF8(val);
            }
        }
        stringToUTF8(val, $1, $2);
        return len;
    }, key, buf, buf_size);

    return written;
}

/* ------------------------------------------------------------------ */
/* sb_exists                                                            */
/* ------------------------------------------------------------------ */

int sb_exists(const char *key)
{
    if (!key) return 0;
    return EM_ASM_INT({
        return localStorage.getItem(UTF8ToString($0)) !== null ? 1 : 0;
    }, key);
}

/* ------------------------------------------------------------------ */
/* sb_delete                                                            */
/* ------------------------------------------------------------------ */

void sb_delete(const char *key)
{
    if (!key) return;
    EM_ASM({
        localStorage.removeItem(UTF8ToString($0));
    }, key);
}

/* ------------------------------------------------------------------ */
/* sb_delete_all                                                        */
/* ------------------------------------------------------------------ */

void sb_delete_all(void)
{
    EM_ASM({
        var toRemove = [];
        for (var i = 0; i < localStorage.length; i++) {
            var k = localStorage.key(i);
            if (k && k.substring(0, 5) === "time.") {
                toRemove.push(k);
            }
        }
        for (var j = 0; j < toRemove.length; j++) {
            localStorage.removeItem(toRemove[j]);
        }
    });
}

/* ------------------------------------------------------------------ */
/* sb_count                                                             */
/* ------------------------------------------------------------------ */

int sb_count(void)
{
    return EM_ASM_INT({
        var count = 0;
        for (var i = 0; i < localStorage.length; i++) {
            var k = localStorage.key(i);
            if (k && k.substring(0, 5) === "time.") {
                count++;
            }
        }
        return count;
    });
}

/* ------------------------------------------------------------------ */
/* sb_export_json                                                       */
/* ------------------------------------------------------------------ */

int sb_export_json(char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;

    return EM_ASM_INT({
        var obj = {};
        for (var i = 0; i < localStorage.length; i++) {
            var k = localStorage.key(i);
            if (k) {
                obj[k] = localStorage.getItem(k);
            }
        }
        var json = JSON.stringify(obj);
        var maxLen = $1 - 1;
        if (maxLen <= 0) return 0;
        var len = lengthBytesUTF8(json);
        if (len > maxLen) {
            json = json.substring(0, maxLen);
            len = lengthBytesUTF8(json);
            while (len > maxLen) {
                json = json.substring(0, json.length - 1);
                len = lengthBytesUTF8(json);
            }
        }
        stringToUTF8(json, $0, $1);
        return len;
    }, buf, buf_size);
}

/* ------------------------------------------------------------------ */
/* sb_download_file                                                     */
/* ------------------------------------------------------------------ */

void sb_download_file(const char *filename, const char *data, int data_len)
{
    if (!filename || !data || data_len <= 0) return;
    EM_ASM({
        var fname = UTF8ToString($0);
        var ptr   = $1;
        var len   = $2;
        var bytes = HEAPU8.slice(ptr, ptr + len);
        var blob  = new Blob([bytes], { type: "application/octet-stream" });
        var url   = URL.createObjectURL(blob);
        var a     = document.createElement("a");
        a.href     = url;
        a.download = fname;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
    }, filename, data, data_len);
}

/* ------------------------------------------------------------------ */
/* sb_request_geolocation (async via EMSCRIPTEN_KEEPALIVE callback)     */
/* ------------------------------------------------------------------ */

static sb_geo_callback_t s_geo_cb = NULL;

EMSCRIPTEN_KEEPALIVE
void sb_geo_result(int success, double lat, double lon)
{
    if (s_geo_cb) {
        s_geo_cb(success, lat, lon);
        s_geo_cb = NULL;
    }
}

void sb_request_geolocation(sb_geo_callback_t callback)
{
    if (!callback) return;
    s_geo_cb = callback;

    EM_ASM({
        if (!navigator.geolocation) {
            _sb_geo_result(0, 0.0, 0.0);
            return;
        }
        navigator.geolocation.getCurrentPosition(
            function(pos) {
                _sb_geo_result(1, pos.coords.latitude, pos.coords.longitude);
            },
            function(err) {
                _sb_geo_result(0, 0.0, 0.0);
            },
            { timeout: 10000, maximumAge: 300000 }
        );
    });
}

/* ------------------------------------------------------------------ */
/* sb_reset                                                             */
/* ------------------------------------------------------------------ */

void sb_reset(void)
{
    EM_ASM({
        var toRemove = [];
        for (var i = 0; i < localStorage.length; i++) {
            var k = localStorage.key(i);
            if (k && k.substring(0, 5) === "time.") {
                toRemove.push(k);
            }
        }
        for (var j = 0; j < toRemove.length; j++) {
            localStorage.removeItem(toRemove[j]);
        }
    });
}

#endif /* __EMSCRIPTEN__ */
