/* share_moment.h — Shareable Moments: URL generation + share data.
 *
 * Pure module: generates share data (URL params, shareable text,
 * Open Graph description) from a moment in Time. No DOM, no clipboard,
 * no networking — pure string formatting from struct input.
 *
 * No GL, no malloc, no globals, no side effects. */

#ifndef TIME_SHARE_MOMENT_H
#define TIME_SHARE_MOMENT_H

#include "../systems/unified/today_summary.h"

#define SM_URL_MAX  512
#define SM_TEXT_MAX 280
#define SM_DESC_MAX 200

/* A moment captured for sharing */
typedef struct {
    double jd;             /* Julian Day — canonical time representation */
    int    view_id;        /* VIEW_SPACE=0, VIEW_EARTH=1, etc. */
    int    active_systems; /* bitmask of which systems to highlight */
    float  camera_zoom;    /* camera log_zoom float */
    double observer_lat;   /* observer latitude in degrees */
    double observer_lon;   /* observer longitude in degrees */
} sm_moment_t;

/* Generated share data for a moment */
typedef struct {
    char url_params[SM_URL_MAX];     /* "?jd=...&view=...&zoom=...&lat=...&lon=..." */
    char share_text[SM_TEXT_MAX];    /* top entries joined with separator, 280 max */
    char og_description[SM_DESC_MAX]; /* Open Graph description, 200 max */
} sm_share_t;

/* Create moment from current state params. */
sm_moment_t sm_current(double jd, int view_id, float zoom,
                       double lat, double lon);

/* Generate all share data from moment + pre-computed summary. */
sm_share_t sm_generate(sm_moment_t moment, const ts_summary_t *summary);

/* Format URL query string from moment.
 * Example: "?jd=2451545.000&view=space&zoom=3.40&lat=51.48&lon=-0.00"
 * Returns chars written (excluding null), or 0 on error. */
int sm_format_url(sm_moment_t moment, char *buf, int buf_size);

/* Format shareable text from summary (top significant entries, 280 char max).
 * Entries sorted by significance (higher first), joined with " · ".
 * Returns chars written (excluding null), or 0 on error. */
int sm_format_text(const ts_summary_t *summary, char *buf, int buf_size);

/* Format Open Graph description from summary (200 char max).
 * Includes headline + top 2-3 entries. More concise than share text.
 * Returns chars written (excluding null), or 0 on error. */
int sm_format_og(const ts_summary_t *summary, char *buf, int buf_size);

/* Get view name slug for URL from view_id.
 * Returns lowercase: "space", "earth", "galaxy", "personal",
 * "deep_time", "city", "room". Returns "unknown" for invalid IDs. */
const char *sm_view_slug(int view_id);

#endif /* TIME_SHARE_MOMENT_H */
