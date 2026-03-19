/* share_template.c — Platform-Specific Share Templates
 *
 * Formats today_summary data into platform-specific text for sharing.
 * Pure functions: no globals, no malloc, no side effects. */

#include "share_template.h"

#include <stdio.h>
#include <string.h>

/* ---- Local system name table (avoids linking today_summary.c) ---- */

static const char *sht_sys_names[TS_SYS_COUNT] = {
    "Gregorian", "Tzolkin", "Haab", "Chinese", "Hebrew",
    "Islamic", "Buddhist", "Hindu", "I Ching", "Astrology",
    "Human Design", "Kabbalah", "Coptic", "Ethiopian", "Persian",
    "Japanese", "Korean", "Thai", "Geological", "Cosmic"
};

static const char *sht_system_name(ts_system_t sys)
{
    if (sys < 0 || sys >= TS_SYS_COUNT) return "Unknown";
    return sht_sys_names[sys];
}

/* ---- Platform metadata ---- */

static const char *platform_names[] = {
    "Twitter", "Instagram", "WhatsApp", "Clipboard"
};

static const int platform_limits[] = {
    280, 2200, 1000, 4096
};

const char *sht_platform_name(sht_platform_t platform)
{
    if (platform < 0 || platform >= SHT_PLATFORM_COUNT) return "Unknown";
    return platform_names[platform];
}

int sht_char_limit(sht_platform_t platform)
{
    if (platform < 0 || platform >= SHT_PLATFORM_COUNT) return 0;
    return platform_limits[platform];
}

/* ---- Helpers ---- */

/* Append string to buffer, respecting max size. Returns new offset. */
static int buf_append(char *buf, int offset, int max, const char *str)
{
    if (buf == NULL || str == NULL) return offset;
    int len = (int)strlen(str);
    int avail = max - offset - 1; /* leave room for null */
    if (avail <= 0) return offset;
    if (len > avail) len = avail;
    memcpy(buf + offset, str, (size_t)len);
    buf[offset + len] = '\0';
    return offset + len;
}

/* Find best entry: highest significance active entry. */
static int best_entry_index(const ts_summary_t *s)
{
    int best = -1;
    int best_sig = -1;
    for (int i = 0; i < s->entry_count; i++) {
        if (s->entries[i].active && s->entries[i].significance > best_sig) {
            best_sig = s->entries[i].significance;
            best = i;
        }
    }
    return best;
}

/* ---- Twitter: concise, 280 char max ---- */

static sht_result_t format_twitter(const ts_summary_t *s)
{
    sht_result_t r = { .text_len = 0 };
    int off = 0;
    int max = 280;

    /* Headline */
    if (s->headline[0] != '\0') {
        off = buf_append(r.text, off, max, s->headline);
    }

    /* Top entries by significance */
    for (int sig = 3; sig >= 0; sig--) {
        for (int i = 0; i < s->entry_count && off < max - 20; i++) {
            if (!s->entries[i].active || s->entries[i].significance != sig) {
                continue;
            }
            if (off > 0) off = buf_append(r.text, off, max, " | ");
            off = buf_append(r.text, off, max, s->entries[i].date_str);
            if (s->entries[i].note[0] != '\0') {
                off = buf_append(r.text, off, max, " (");
                off = buf_append(r.text, off, max, s->entries[i].note);
                off = buf_append(r.text, off, max, ")");
            }
        }
    }

    /* Attribution */
    if (off < max - 8) {
        off = buf_append(r.text, off, max, " — Time");
    }

    r.text_len = off;
    return r;
}

/* ---- Instagram: rich caption, 2200 char max ---- */

static sht_result_t format_instagram(const ts_summary_t *s)
{
    sht_result_t r = { .text_len = 0 };
    int off = 0;
    int max = 2200;

    /* Title */
    off = buf_append(r.text, off, max, "This moment in Time:\n\n");

    /* Headline */
    if (s->headline[0] != '\0') {
        off = buf_append(r.text, off, max, s->headline);
        off = buf_append(r.text, off, max, "\n\n");
    }

    /* All active entries, one per line with system name */
    for (int i = 0; i < s->entry_count; i++) {
        if (!s->entries[i].active) continue;
        const char *name = sht_system_name(s->entries[i].system);
        off = buf_append(r.text, off, max, name);
        off = buf_append(r.text, off, max, ": ");
        off = buf_append(r.text, off, max, s->entries[i].date_str);
        if (s->entries[i].note[0] != '\0') {
            off = buf_append(r.text, off, max, " — ");
            off = buf_append(r.text, off, max, s->entries[i].note);
        }
        off = buf_append(r.text, off, max, "\n");
    }

    /* Footer */
    off = buf_append(r.text, off, max, "\nExplore at Time");

    r.text_len = off;
    return r;
}

/* ---- WhatsApp: message format, 1000 char max ---- */

static sht_result_t format_whatsapp(const ts_summary_t *s)
{
    sht_result_t r = { .text_len = 0 };
    int off = 0;
    int max = 1000;

    /* Opening */
    off = buf_append(r.text, off, max, "Right now in Time:\n");

    /* Headline */
    if (s->headline[0] != '\0') {
        off = buf_append(r.text, off, max, s->headline);
        off = buf_append(r.text, off, max, "\n");
    }

    /* Top entries (max 5 for readability) */
    int shown = 0;
    for (int i = 0; i < s->entry_count && shown < 5; i++) {
        if (!s->entries[i].active) continue;
        off = buf_append(r.text, off, max, "- ");
        off = buf_append(r.text, off, max, s->entries[i].date_str);
        if (s->entries[i].note[0] != '\0') {
            off = buf_append(r.text, off, max, " (");
            off = buf_append(r.text, off, max, s->entries[i].note);
            off = buf_append(r.text, off, max, ")");
        }
        off = buf_append(r.text, off, max, "\n");
        shown++;
    }

    r.text_len = off;
    return r;
}

/* ---- Clipboard: full detail, 4096 char max ---- */

static sht_result_t format_clipboard(const ts_summary_t *s)
{
    sht_result_t r = { .text_len = 0 };
    int off = 0;
    int max = 4096;

    /* Header */
    off = buf_append(r.text, off, max, "=== This Moment in Time ===\n\n");

    /* Headline */
    if (s->headline[0] != '\0') {
        off = buf_append(r.text, off, max, s->headline);
        off = buf_append(r.text, off, max, "\n\n");
    }

    /* Convergence */
    if (s->convergence_score > 0) {
        char conv_buf[64];
        snprintf(conv_buf, sizeof(conv_buf),
                 "Convergence: %d/10\n\n", s->convergence_score);
        off = buf_append(r.text, off, max, conv_buf);
    }

    /* All entries grouped by significance */
    for (int sig = 3; sig >= 0; sig--) {
        for (int i = 0; i < s->entry_count; i++) {
            if (!s->entries[i].active || s->entries[i].significance != sig) {
                continue;
            }
            const char *name = sht_system_name(s->entries[i].system);
            off = buf_append(r.text, off, max, name);
            off = buf_append(r.text, off, max, ": ");
            off = buf_append(r.text, off, max, s->entries[i].date_str);
            if (s->entries[i].note[0] != '\0') {
                off = buf_append(r.text, off, max, " — ");
                off = buf_append(r.text, off, max, s->entries[i].note);
            }
            off = buf_append(r.text, off, max, "\n");
        }
    }

    /* Footer */
    off = buf_append(r.text, off, max, "\n— Generated by Time");

    r.text_len = off;
    return r;
}

/* ---- Public API ---- */

sht_result_t sht_format(sht_platform_t platform, const ts_summary_t *summary)
{
    sht_result_t empty = { .text_len = 0 };
    empty.text[0] = '\0';

    if (summary == NULL) return empty;

    switch (platform) {
    case SHT_TWITTER:   return format_twitter(summary);
    case SHT_INSTAGRAM: return format_instagram(summary);
    case SHT_WHATSAPP:  return format_whatsapp(summary);
    case SHT_CLIPBOARD: return format_clipboard(summary);
    default:            return empty;
    }
}

/* ---- Cosmic Haiku ---- */

int sht_cosmic_haiku(const ts_summary_t *summary, char *buf, int buf_size)
{
    if (summary == NULL || buf == NULL || buf_size < 2) return 0;

    /* Find best entry for the middle line */
    int best = best_entry_index(summary);

    /* Line 1: time/convergence context */
    const char *line1;
    if (summary->convergence_score >= 7) {
        line1 = "Systems align as one";
    } else if (summary->convergence_score >= 4) {
        line1 = "Cycles weave together";
    } else {
        line1 = "Time turns its wheel";
    }

    /* Line 2: best system's date (truncated to fit) */
    char line2[TS_DATE_MAX];
    if (best >= 0 && summary->entries[best].active) {
        snprintf(line2, sizeof(line2), "%s", summary->entries[best].date_str);
        /* Truncate to ~30 chars for haiku feel */
        if (strlen(line2) > 30) line2[30] = '\0';
    } else {
        snprintf(line2, sizeof(line2), "The stars keep turning");
    }

    /* Line 3: closing reflection */
    const char *line3;
    if (summary->entry_count >= 10) {
        line3 = "All time is now";
    } else if (summary->convergence_score >= 5) {
        line3 = "Worlds converge here";
    } else {
        line3 = "This moment, yours";
    }

    int written = snprintf(buf, (size_t)buf_size, "%s\n%s\n%s", line1, line2, line3);
    if (written < 0) return 0;
    if (written >= buf_size) written = buf_size - 1;
    return written;
}
