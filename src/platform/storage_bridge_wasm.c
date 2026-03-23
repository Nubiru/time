/* storage_bridge_wasm.c — Browser localStorage implementation.
 * WASM-only (S1): uses EM_ASM, browser localStorage.
 *
 * NOTE: Uses HEAPU8 + TextEncoder/TextDecoder directly instead of
 * Emscripten helpers (UTF8ToString, stringToUTF8, lengthBytesUTF8)
 * because emcc 5.x removed those from default exports. */

#ifdef __EMSCRIPTEN__

#include "storage_bridge.h"
#include <emscripten.h>
#include <string.h>
#include <stdio.h>

/* Helper JS snippets used across EM_ASM blocks:
 *   _r(ptr)        = read C string from WASM memory → JS string
 *   _w(str,ptr,max) = write JS string → WASM memory, returns bytes written
 *   _bl(str)       = byte length of string in UTF-8
 */

/* ------------------------------------------------------------------ */
/* sb_save                                                              */
/* ------------------------------------------------------------------ */

void sb_save(const char *key, const char *value)
{
    if (!key || !value) return;
    EM_ASM({
        var d = new TextDecoder();
        function _r(p) { var e=p; while(HEAPU8[e])e++; return d.decode(HEAPU8.subarray(p,e)); }
        try {
            localStorage.setItem(_r($0), _r($1));
        } catch (e) {}
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
        var d = new TextDecoder();
        var enc = new TextEncoder();
        function _r(p) { var e=p; while(HEAPU8[e])e++; return d.decode(HEAPU8.subarray(p,e)); }

        var val = localStorage.getItem(_r($0));
        if (val === null) return 0;
        var maxLen = $2 - 1;
        if (maxLen <= 0) return 0;

        var encoded = enc.encode(val);
        if (encoded.length > maxLen) {
            encoded = encoded.subarray(0, maxLen);
        }
        HEAPU8.set(encoded, $1);
        HEAPU8[$1 + encoded.length] = 0;
        return encoded.length;
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
        var d = new TextDecoder();
        function _r(p) { var e=p; while(HEAPU8[e])e++; return d.decode(HEAPU8.subarray(p,e)); }
        return localStorage.getItem(_r($0)) !== null ? 1 : 0;
    }, key);
}

/* ------------------------------------------------------------------ */
/* sb_delete                                                            */
/* ------------------------------------------------------------------ */

void sb_delete(const char *key)
{
    if (!key) return;
    EM_ASM({
        var d = new TextDecoder();
        function _r(p) { var e=p; while(HEAPU8[e])e++; return d.decode(HEAPU8.subarray(p,e)); }
        localStorage.removeItem(_r($0));
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
        var enc = new TextEncoder();
        var obj = {};
        for (var i = 0; i < localStorage.length; i++) {
            var k = localStorage.key(i);
            if (k) obj[k] = localStorage.getItem(k);
        }
        var json = JSON.stringify(obj);
        var maxLen = $1 - 1;
        if (maxLen <= 0) return 0;

        var encoded = enc.encode(json);
        if (encoded.length > maxLen) {
            encoded = encoded.subarray(0, maxLen);
        }
        HEAPU8.set(encoded, $0);
        HEAPU8[$0 + encoded.length] = 0;
        return encoded.length;
    }, buf, buf_size);
}

/* ------------------------------------------------------------------ */
/* sb_download_file                                                     */
/* ------------------------------------------------------------------ */

void sb_download_file(const char *filename, const char *data, int data_len)
{
    if (!filename || !data || data_len <= 0) return;
    EM_ASM({
        var d = new TextDecoder();
        function _r(p) { var e=p; while(HEAPU8[e])e++; return d.decode(HEAPU8.subarray(p,e)); }
        var fname = _r($0);
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
/* sb_request_geolocation                                               */
/* ------------------------------------------------------------------ */

static sb_geo_callback_t s_geo_cb;

EMSCRIPTEN_KEEPALIVE void sb_geo_success(double lat, double lon)
{
    if (s_geo_cb) s_geo_cb(1, lat, lon);
}

void sb_request_geolocation(sb_geo_callback_t callback)
{
    s_geo_cb = callback;
    EM_ASM({
        if (navigator.geolocation) {
            navigator.geolocation.getCurrentPosition(function(pos) {
                if (Module._sb_geo_success) {
                    Module._sb_geo_success(pos.coords.latitude, pos.coords.longitude);
                }
            }, function(err) {
                /* Geolocation denied or unavailable — no-op */
            }, { enableHighAccuracy: false, timeout: 10000 });
        }
    });
}

#endif /* __EMSCRIPTEN__ */
