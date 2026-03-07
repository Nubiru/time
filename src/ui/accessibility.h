#ifndef TIME_ACCESSIBILITY_H
#define TIME_ACCESSIBILITY_H

/* Screen reader / ARIA label text generation.
 * Pure module: all output via snprintf to caller-provided buffers.
 * No allocation, no globals, no side effects. */

#include <stdbool.h>

#define ARIA_MAX_LEN 256

/* Event types for accessibility announcements */
typedef enum {
    ARIA_EVENT_NONE = 0,
    ARIA_EVENT_SCALE_CHANGE,   /* zoom level changed */
    ARIA_EVENT_SYSTEM_FOCUS,   /* knowledge system focused */
    ARIA_EVENT_FESTIVAL,       /* festival date reached */
    ARIA_EVENT_ASPECT,         /* planetary aspect exact */
    ARIA_EVENT_TIME_JUMP,      /* time travel activated */
    ARIA_EVENT_VIEW_CHANGE,    /* space/earth view switch */
    ARIA_EVENT_COUNT
} aria_event_t;

/* Planet position data for sky description */
typedef struct {
    const char *name;      /* "Sun", "Moon", "Mars", etc. */
    const char *sign;      /* "Aries", "Taurus", etc. */
    double degree;         /* degree within sign (0-30) */
    bool is_retrograde;    /* true if retrograde */
} aria_planet_t;

/* Format a single planet position as text.
 * Output: "Sun in Aries at 15.3 degrees"
 * Retrograde: "Mars in Scorpio at 8.7 degrees (retrograde)"
 * Returns number of chars written (excluding null). */
int aria_planet_desc(const aria_planet_t *planet, char *out, int max_len);

/* Format multiple planet positions as sky description.
 * Output: "Sun in Aries at 15.0 degrees, Moon in Cancer at 22.0 degrees"
 * Returns chars written. */
int aria_sky_desc(const aria_planet_t *planets, int count,
                  char *out, int max_len);

/* Format a date/time for screen reader.
 * Output: "January 15, 2024 at 14:30 UTC"
 * Returns chars written. */
int aria_datetime_desc(int year, int month, int day, int hour, int minute,
                       char *out, int max_len);

/* Format a calendar system date.
 * Output: "Hebrew calendar: 5 Shevat 5784"
 * Returns chars written. */
int aria_calendar_desc(const char *system_name, const char *date_str,
                       char *out, int max_len);

/* Format a festival announcement.
 * Output: "Festival alert: Nayrouz, Coptic New Year"
 * Returns chars written. */
int aria_festival_desc(const char *system_name, const char *festival_name,
                       char *out, int max_len);

/* Format a scale/zoom level change.
 * Output: "Zoom level: Solar System scale"
 * Returns chars written. */
int aria_scale_desc(const char *scale_name, char *out, int max_len);

/* Format a view mode change.
 * Output: "View changed to Earth surface view"
 * Returns chars written. */
int aria_view_desc(const char *view_name, char *out, int max_len);

/* Format an event announcement.
 * Returns chars written. Returns 0 for NONE or invalid event. */
int aria_event_desc(aria_event_t event, const char *detail,
                    char *out, int max_len);

/* Event type name string. Returns static string. */
const char *aria_event_name(aria_event_t event);

/* Format retrograde status.
 * Output: "Mars is retrograde" or "Mars is direct"
 * Returns chars written. */
int aria_retrograde_desc(const char *planet_name, bool is_retrograde,
                         char *out, int max_len);

/* Format an aspect between two planets.
 * Output: "Sun and Moon in conjunction at 0.5 degrees separation"
 * Returns chars written. */
int aria_aspect_desc(const char *planet_a, const char *planet_b,
                     const char *aspect_name, double separation_deg,
                     char *out, int max_len);

/* Number of event types (equals ARIA_EVENT_COUNT). */
int aria_event_count(void);

#endif /* TIME_ACCESSIBILITY_H */
