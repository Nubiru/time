/* wisdom_engine.h -- Wisdom Correlation Engine.
 *
 * Pure module that takes a navigation context (active systems, user kin,
 * scale level, convergence score, view mode) and returns a ranked list of
 * contextually relevant wisdom quotes from the wisdom database.
 *
 * Correlation rules match quotes to what the user is currently viewing.
 * Supports exclusion of previously-seen quotes for session rotation.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_WISDOM_ENGINE_H
#define TIME_WISDOM_ENGINE_H

#include "wisdom.h"

#define WE_MAX_ACTIVE_SYSTEMS  16
#define WE_MAX_RESULTS         16
#define WE_MAX_SEEN            64

/* View mode — what the user is looking at */
typedef enum {
    WE_VIEW_SPACE = 0,     /* space view (outward — planets, zodiac, stars) */
    WE_VIEW_EARTH,         /* earth view (surface — tides, weather, geology) */
    WE_VIEW_TZOLKIN,       /* tzolkin board / mayan focus */
    WE_VIEW_ICHING,        /* I Ching hexagram display */
    WE_VIEW_CALENDAR,      /* calendar / reform / epoch focus */
    WE_VIEW_DEEP_TIME,     /* geological deep time scale */
    WE_VIEW_COUNT
} we_view_t;

/* Scale level — zoom/temporal scale */
typedef enum {
    WE_SCALE_HUMAN = 0,    /* days, months, human lifetime */
    WE_SCALE_HISTORICAL,   /* centuries, civilizations */
    WE_SCALE_GEOLOGICAL,   /* millions of years */
    WE_SCALE_COSMIC,       /* billions of years, universe scale */
    WE_SCALE_COUNT
} we_scale_t;

/* Active system flags (bitmask) */
typedef enum {
    WE_SYS_ASTRONOMY    = (1 << 0),
    WE_SYS_ASTROLOGY    = (1 << 1),
    WE_SYS_TZOLKIN      = (1 << 2),
    WE_SYS_ICHING       = (1 << 3),
    WE_SYS_CHINESE      = (1 << 4),
    WE_SYS_HUMAN_DESIGN = (1 << 5),
    WE_SYS_HEBREW       = (1 << 6),
    WE_SYS_ISLAMIC      = (1 << 7),
    WE_SYS_BUDDHIST     = (1 << 8),
    WE_SYS_HINDU        = (1 << 9),
    WE_SYS_KABBALAH     = (1 << 10),
    WE_SYS_GREGORIAN    = (1 << 11),
    WE_SYS_GEOLOGY      = (1 << 12),
    WE_SYS_EARTH        = (1 << 13)
} we_system_flag_t;

/* Navigation context — describes what the user is doing */
typedef struct {
    we_view_t view;            /* current view mode */
    we_scale_t scale;          /* temporal scale level */
    unsigned int active_systems; /* bitmask of we_system_flag_t */
    int user_seal;             /* user's Tzolkin seal (0-19), -1 if unknown */
    int user_tone;             /* user's Tzolkin tone (1-13), 0 if unknown */
    int convergence_score;     /* from convergence detector (0=none, higher=more) */
} we_context_t;

/* A scored quote result */
typedef struct {
    int quote_index;           /* index into wisdom database */
    int score;                 /* relevance score (higher = more relevant) */
} we_result_t;

/* A selection result set */
typedef struct {
    we_result_t results[WE_MAX_RESULTS];
    int count;
} we_selection_t;

/* Select contextually relevant quotes.
 * ctx: navigation context describing what user is viewing
 * seen: array of quote indices already shown (for rotation), NULL if none
 * seen_count: number of entries in seen array
 * Returns ranked selection (highest score first). */
we_selection_t we_select(const we_context_t *ctx,
                         const int *seen, int seen_count);

/* Score a single quote against a context.
 * Returns relevance score (0 = irrelevant, higher = more relevant). */
int we_score_quote(int quote_index, const we_context_t *ctx);

/* Get the best single quote for a context (convenience).
 * seen/seen_count for rotation. Returns quote index, -1 if none. */
int we_best_quote(const we_context_t *ctx,
                  const int *seen, int seen_count);

/* Get view name string. */
const char *we_view_name(we_view_t view);

/* Get scale name string. */
const char *we_scale_name(we_scale_t scale);

/* Map a wisdom tag to relevant system flags.
 * Returns bitmask of systems that correlate with this tag. */
unsigned int we_tag_to_systems(wisdom_tag_t tag);

/* Map a system flag to relevant wisdom tags.
 * Writes matching tags to out[], returns count. */
int we_system_to_tags(we_system_flag_t sys, wisdom_tag_t *out, int max_out);

/* Create a default context (space view, human scale, no systems, no kin). */
we_context_t we_context_default(void);

#endif /* TIME_WISDOM_ENGINE_H */
