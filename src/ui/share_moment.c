/* share_moment.c — Shareable Moments: URL generation + share data.
 *
 * Pure string formatting from struct input. Generates URL params,
 * shareable text, and Open Graph descriptions from a moment in Time.
 *
 * No GL, no malloc, no globals, no side effects. */

#include "share_moment.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* View slug table                                                     */
/* ------------------------------------------------------------------ */

#define SM_VIEW_COUNT 7

static const char *VIEW_SLUGS[SM_VIEW_COUNT] = {
    "space",      /* VIEW_SPACE     = 0 */
    "earth",      /* VIEW_EARTH     = 1 */
    "galaxy",     /* VIEW_GALAXY    = 2 */
    "personal",   /* VIEW_PERSONAL  = 3 */
    "deep_time",  /* VIEW_DEEP_TIME = 4 */
    "city",       /* VIEW_CITY      = 5 */
    "room"        /* VIEW_ROOM      = 6 */
};

/* UTF-8 middle dot separator: " · " (space + U+00B7 + space = 4 bytes) */
static const char SM_SEP[] = " \xc2\xb7 ";
static const int SM_SEP_LEN = 4; /* strlen(" · ") in bytes: 1+2+1 */

/* Maximum entries to include in share text */
static const int SM_MAX_TEXT_ENTRIES = 5;

/* Maximum entries to include in OG description */
static const int SM_MAX_OG_ENTRIES = 3;

/* ------------------------------------------------------------------ */
/* sm_view_slug                                                        */
/* ------------------------------------------------------------------ */

const char *sm_view_slug(int view_id)
{
    if (view_id < 0 || view_id >= SM_VIEW_COUNT) {
        return "unknown";
    }
    return VIEW_SLUGS[view_id];
}

/* ------------------------------------------------------------------ */
/* sm_current                                                          */
/* ------------------------------------------------------------------ */

sm_moment_t sm_current(double jd, int view_id, float zoom,
                       double lat, double lon)
{
    sm_moment_t m;
    m.jd = jd;
    m.view_id = view_id;
    m.active_systems = 0;
    m.camera_zoom = zoom;
    m.observer_lat = lat;
    m.observer_lon = lon;
    return m;
}

/* ------------------------------------------------------------------ */
/* sm_format_url                                                       */
/* ------------------------------------------------------------------ */

int sm_format_url(sm_moment_t moment, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }

    int written = snprintf(buf, (size_t)buf_size,
        "?jd=%.3f&view=%s&zoom=%.2f&lat=%.2f&lon=%.2f",
        moment.jd,
        sm_view_slug(moment.view_id),
        (double)moment.camera_zoom,
        moment.observer_lat,
        moment.observer_lon);

    if (written < 0 || written >= buf_size) {
        /* Truncated or error — null-terminate and return 0 */
        buf[0] = '\0';
        return 0;
    }

    return written;
}

/* ------------------------------------------------------------------ */
/* Sorting helper: collect active entries sorted by significance       */
/* ------------------------------------------------------------------ */

typedef struct {
    int index;        /* index into summary->entries[] */
    int significance; /* cached significance for sorting */
} sm_sort_entry_t;

static int sm_collect_sorted(const ts_summary_t *summary,
                             sm_sort_entry_t *out, int max_entries)
{
    if (!summary || !out || max_entries <= 0) {
        return 0;
    }

    int count = 0;

    /* Collect active entries */
    for (int i = 0; i < summary->entry_count && i < TS_MAX_SYSTEMS; i++) {
        if (summary->entries[i].active && count < max_entries) {
            out[count].index = i;
            out[count].significance = summary->entries[i].significance;
            count++;
        }
    }

    /* Simple insertion sort by significance (descending) */
    for (int i = 1; i < count; i++) {
        sm_sort_entry_t key = out[i];
        int j = i - 1;
        while (j >= 0 && out[j].significance < key.significance) {
            out[j + 1] = out[j];
            j--;
        }
        out[j + 1] = key;
    }

    return count;
}

/* ------------------------------------------------------------------ */
/* sm_format_text                                                      */
/* ------------------------------------------------------------------ */

int sm_format_text(const ts_summary_t *summary, char *buf, int buf_size)
{
    if (!summary || !buf || buf_size <= 0) {
        if (buf && buf_size > 0) {
            buf[0] = '\0';
        }
        return 0;
    }

    buf[0] = '\0';

    /* Collect and sort active entries */
    sm_sort_entry_t sorted[TS_MAX_SYSTEMS];
    int count = sm_collect_sorted(summary, sorted, TS_MAX_SYSTEMS);

    if (count == 0) {
        return 0;
    }

    /* Limit to SM_MAX_TEXT_ENTRIES */
    if (count > SM_MAX_TEXT_ENTRIES) {
        count = SM_MAX_TEXT_ENTRIES;
    }

    int pos = 0;
    int remaining = buf_size - 1; /* reserve for null terminator */

    for (int i = 0; i < count && remaining > 0; i++) {
        const char *date_str = summary->entries[sorted[i].index].date_str;
        int date_len = (int)strlen(date_str);

        if (i > 0) {
            /* Check if separator + next entry fits */
            if (remaining < SM_SEP_LEN + date_len) {
                break; /* stop rather than truncate mid-entry */
            }
            memcpy(buf + pos, SM_SEP, (size_t)SM_SEP_LEN);
            pos += SM_SEP_LEN;
            remaining -= SM_SEP_LEN;
        }

        if (remaining < date_len) {
            break; /* stop rather than truncate mid-entry */
        }
        memcpy(buf + pos, date_str, (size_t)date_len);
        pos += date_len;
        remaining -= date_len;
    }

    buf[pos] = '\0';
    return pos;
}

/* ------------------------------------------------------------------ */
/* sm_format_og                                                        */
/* ------------------------------------------------------------------ */

int sm_format_og(const ts_summary_t *summary, char *buf, int buf_size)
{
    if (!summary || !buf || buf_size <= 0) {
        if (buf && buf_size > 0) {
            buf[0] = '\0';
        }
        return 0;
    }

    buf[0] = '\0';
    int pos = 0;
    int remaining = buf_size - 1;

    /* Start with headline if present */
    int headline_len = (int)strlen(summary->headline);
    if (headline_len > 0 && remaining > 0) {
        int copy_len = headline_len;
        if (copy_len > remaining) {
            copy_len = remaining;
        }
        memcpy(buf + pos, summary->headline, (size_t)copy_len);
        pos += copy_len;
        remaining -= copy_len;
    }

    /* Collect and sort active entries */
    sm_sort_entry_t sorted[TS_MAX_SYSTEMS];
    int count = sm_collect_sorted(summary, sorted, TS_MAX_SYSTEMS);

    /* Limit to SM_MAX_OG_ENTRIES */
    if (count > SM_MAX_OG_ENTRIES) {
        count = SM_MAX_OG_ENTRIES;
    }

    for (int i = 0; i < count && remaining > 0; i++) {
        const char *date_str = summary->entries[sorted[i].index].date_str;
        int date_len = (int)strlen(date_str);

        /* Add separator before each entry (including first, after headline) */
        if (pos > 0) {
            if (remaining < SM_SEP_LEN + date_len) {
                break;
            }
            memcpy(buf + pos, SM_SEP, (size_t)SM_SEP_LEN);
            pos += SM_SEP_LEN;
            remaining -= SM_SEP_LEN;
        }

        if (remaining < date_len) {
            break;
        }
        memcpy(buf + pos, date_str, (size_t)date_len);
        pos += date_len;
        remaining -= date_len;
    }

    buf[pos] = '\0';
    return pos;
}

/* ------------------------------------------------------------------ */
/* sm_generate                                                         */
/* ------------------------------------------------------------------ */

sm_share_t sm_generate(sm_moment_t moment, const ts_summary_t *summary)
{
    sm_share_t share;
    memset(&share, 0, sizeof(share));

    sm_format_url(moment, share.url_params, SM_URL_MAX);
    sm_format_text(summary, share.share_text, SM_TEXT_MAX);
    sm_format_og(summary, share.og_description, SM_DESC_MAX);

    return share;
}
