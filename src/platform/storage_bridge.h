/* storage_bridge.h — Platform abstraction for persistent storage.
 * Native builds: in-memory store (for testing).
 * WASM builds: browser localStorage via Emscripten JS interop.
 *
 * This header defines the interface. Implementation is platform-specific. */

#ifndef TIME_STORAGE_BRIDGE_H
#define TIME_STORAGE_BRIDGE_H

#define SB_MAX_KEY     64
#define SB_MAX_VAL     4096
#define SB_MAX_ENTRIES 256

/* Save a key-value pair to persistent storage. */
void sb_save(const char *key, const char *value);

/* Load a value by key. Returns bytes written to buf (0 if not found). */
int sb_load(const char *key, char *buf, int buf_size);

/* Check if a key exists. Returns 1 if exists, 0 if not. */
int sb_exists(const char *key);

/* Delete a single key. */
void sb_delete(const char *key);

/* Delete all keys with prefix "time." */
void sb_delete_all(void);

/* Count of stored keys. */
int sb_count(void);

/* Export all stored keys as JSON string. Returns bytes written. */
int sb_export_json(char *buf, int buf_size);

/* Trigger file download (no-op on native, triggers browser download on WASM). */
void sb_download_file(const char *filename, const char *data, int data_len);

/* Geolocation callback type. */
typedef void (*sb_geo_callback_t)(int success, double lat, double lon);

/* Request geolocation (calls back with stub 0.0,0.0 on native, real API on WASM). */
void sb_request_geolocation(sb_geo_callback_t callback);

/* Reset all storage (for test isolation). */
void sb_reset(void);

#endif /* TIME_STORAGE_BRIDGE_H */
