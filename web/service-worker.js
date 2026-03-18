/* Time — Service Worker
 * Cache-first strategy for offline support.
 * All computation is local (WASM) — no API calls needed. */

var CACHE_NAME = 'time-v1';
var ASSETS = [
    './',
    './index.html',
    './index.js',
    './index.wasm',
    './style.css'
];

/* Install: pre-cache all assets */
self.addEventListener('install', function(event) {
    event.waitUntil(
        caches.open(CACHE_NAME).then(function(cache) {
            return cache.addAll(ASSETS);
        })
    );
});

/* Fetch: serve from cache, fall back to network */
self.addEventListener('fetch', function(event) {
    event.respondWith(
        caches.match(event.request).then(function(cached) {
            return cached || fetch(event.request);
        })
    );
});

/* Activate: clean up old caches on version bump */
self.addEventListener('activate', function(event) {
    event.waitUntil(
        caches.keys().then(function(names) {
            return Promise.all(
                names.filter(function(n) { return n !== CACHE_NAME; })
                     .map(function(n) { return caches.delete(n); })
            );
        })
    );
});
