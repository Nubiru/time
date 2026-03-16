/* user_prefs.h -- User Preferences Store
 *
 * Unified user preferences covering visual, temporal, social, and
 * location categories.  Provides serialization to/from key=value text
 * (for localStorage via JS bridge), sensible defaults, partial merge,
 * validation, diff, and integration bridges to theme.h and
 * privacy_framework.h.
 *
 * Track 47.3 — foundation for all personalization (Tracks 47-49).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_USER_PREFS_H
#define TIME_USER_PREFS_H

#include "interest_profile.h"
#include "privacy_framework.h"
#include "../../ui/theme.h"

#define UP_MAX_SERIAL_SIZE 2048
#define UP_MAX_CALENDAR_SYSTEMS 16
#define UP_KEY_MAX 64
#define UP_VAL_MAX 128

/* Visual preferences */
typedef struct {
    theme_id_t theme;                /* COSMOS or DAWN */
    float font_scale;                /* 0.5-2.0 */
    float opacity_scale;             /* 0.5-1.0 */
    float animation_speed;           /* 0.5-2.0 */
    int layers_visible;              /* bitmask of render layers */
    int default_scale_level;         /* 0-6 initial camera scale */
} up_visual_t;

/* Temporal preferences */
typedef struct {
    int home_timezone_offset_min;    /* UTC offset in minutes (-720 to +840) */
    int preferred_calendars[UP_MAX_CALENDAR_SYSTEMS]; /* calendar system IDs, -1 terminated */
    int preferred_calendar_count;    /* active count */
    int use_24h_time;                /* 0=12h, 1=24h */
    int week_start_day;              /* 0=Sunday, 1=Monday */
} up_temporal_t;

/* Social preferences */
typedef struct {
    ip_activity_t activity_level;    /* introvert/balanced/extrovert */
    ip_visibility_t visibility;      /* anonymous/pseudonym/public */
    int notification_frequency;      /* 0=off, 1=low, 2=medium, 3=high */
    int roulette_opt_in;             /* 0=no, 1=yes */
    pf_consent_t consent;            /* privacy consent level */
    pf_retention_t retention;        /* data retention policy */
} up_social_t;

/* Location preferences */
typedef struct {
    double home_lat;                 /* home latitude (-90 to 90) */
    double home_lon;                 /* home longitude (-180 to 180) */
    double exploration_radius_km;    /* event discovery radius (1-500) */
    int location_set;                /* 0=not set, 1=user provided */
} up_location_t;

/* Complete user preferences */
typedef struct {
    up_visual_t visual;
    up_temporal_t temporal;
    up_social_t social;
    up_location_t location;
    int version;                     /* schema version for migration */
} up_prefs_t;

/* Serialized key-value pair */
typedef struct {
    char key[UP_KEY_MAX];
    char val[UP_VAL_MAX];
} up_kv_t;

/* Serialization result */
typedef struct {
    int length;                      /* bytes written (excluding null) */
    int truncated;                   /* 1 if output was truncated */
} up_serial_result_t;

/* Default preferences (anonymous, introvert, session-only, COSMOS theme). */
up_prefs_t up_default(void);

/* Validate and clamp all fields to safe ranges. */
up_prefs_t up_validate(up_prefs_t prefs);

/* Merge: apply non-default fields from patch onto base. */
up_prefs_t up_merge(up_prefs_t base, up_prefs_t patch);

/* Serialize to key=value text. Returns bytes written. */
up_serial_result_t up_serialize(const up_prefs_t *prefs, char *buf, int buf_size);

/* Deserialize from key=value text. Returns prefs (defaults for missing keys). */
up_prefs_t up_deserialize(const char *buf, int buf_len);

/* Serialize a single section. */
up_serial_result_t up_serialize_visual(const up_visual_t *v, char *buf, int buf_size);
up_serial_result_t up_serialize_temporal(const up_temporal_t *t, char *buf, int buf_size);
up_serial_result_t up_serialize_social(const up_social_t *s, char *buf, int buf_size);
up_serial_result_t up_serialize_location(const up_location_t *l, char *buf, int buf_size);

/* Count key-value pairs in serialized text. */
int up_count_keys(const char *buf, int buf_len);

/* Parse a single key-value pair from serialized text. Returns chars consumed, 0 on end. */
int up_parse_kv(const char *buf, int buf_len, up_kv_t *out);

/* Compare two prefs structs. Returns 0 if identical, bitmask of differing sections otherwise. */
int up_diff(const up_prefs_t *a, const up_prefs_t *b);

/* Section diff flags */
#define UP_DIFF_VISUAL   0x01
#define UP_DIFF_TEMPORAL 0x02
#define UP_DIFF_SOCIAL   0x04
#define UP_DIFF_LOCATION 0x08

/* Convert prefs to theme_prefs_t for theme system integration. */
theme_prefs_t up_to_theme_prefs(const up_visual_t *v);

/* Convert prefs to pf_settings_t for privacy system integration. */
pf_settings_t up_to_privacy_settings(const up_social_t *s);

/* Check if location is set and valid. */
int up_has_location(const up_location_t *l);

/* Human-readable preference summary (writes to buf). */
int up_summary(const up_prefs_t *prefs, char *buf, int buf_size);

#endif /* TIME_USER_PREFS_H */
