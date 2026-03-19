/* share_template.h — Platform-Specific Share Templates
 * Formats Time moments as shareable text for Twitter, Instagram,
 * WhatsApp, and clipboard. Includes cosmic haiku generator.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * All output written to caller-provided buffers or result structs. */

#ifndef TIME_SHARE_TEMPLATE_H
#define TIME_SHARE_TEMPLATE_H

#include "../systems/unified/today_summary.h"

#define SHT_TEXT_MAX 4096

typedef enum {
    SHT_TWITTER = 0,     /* 280 chars, concise with systems */
    SHT_INSTAGRAM,       /* 2200 chars, rich caption */
    SHT_WHATSAPP,        /* 1000 chars, message with context */
    SHT_CLIPBOARD,       /* 4096 chars, full detail */
    SHT_PLATFORM_COUNT
} sht_platform_t;

typedef struct {
    char text[SHT_TEXT_MAX];
    int text_len;
} sht_result_t;

/* Format share text for a specific platform.
 * Returns result with formatted text and length.
 * Returns text_len=0 for invalid platform or NULL summary. */
sht_result_t sht_format(sht_platform_t platform, const ts_summary_t *summary);

/* Generate a cosmic haiku: 3-line poetic summary of the moment.
 * Format: line1\nline2\nline3 (no trailing newline).
 * Returns chars written (excluding null), or 0 on error. */
int sht_cosmic_haiku(const ts_summary_t *summary, char *buf, int buf_size);

/* Platform character limit. */
int sht_char_limit(sht_platform_t platform);

/* Platform display name. Returns "Unknown" for invalid. */
const char *sht_platform_name(sht_platform_t platform);

#endif /* TIME_SHARE_TEMPLATE_H */
