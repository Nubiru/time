/* pre.js — Injected before Emscripten runtime.
 * Polyfills string helpers removed in emcc 5.x. */
if (typeof globalThis.lengthBytesUTF8 === 'undefined') {
    globalThis.lengthBytesUTF8 = function(str) {
        return new TextEncoder().encode(str).length;
    };
}
if (typeof globalThis.UTF8ToString === 'undefined') {
    globalThis.UTF8ToString = function(ptr, maxLen) {
        var HEAPU8 = Module.HEAPU8;
        if (!HEAPU8 || !ptr) return '';
        var end = ptr;
        if (maxLen) { var limit = ptr + maxLen; while (end < limit && HEAPU8[end]) end++; }
        else { while (HEAPU8[end]) end++; }
        return new TextDecoder().decode(HEAPU8.subarray(ptr, end));
    };
}
if (typeof globalThis.stringToUTF8 === 'undefined') {
    globalThis.stringToUTF8 = function(str, outPtr, maxLen) {
        var HEAPU8 = Module.HEAPU8;
        if (!HEAPU8) return 0;
        var encoded = new TextEncoder().encode(str);
        var len = Math.min(encoded.length, maxLen - 1);
        HEAPU8.set(encoded.subarray(0, len), outPtr);
        HEAPU8[outPtr + len] = 0;
        return len;
    };
}
